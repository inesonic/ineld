/*-*-c++-*-*************************************************************************************************************
* Copyright 2016 - 2023 Inesonic, LLC.
*
* This file is licensed under two licenses.
*
* Inesonic Commercial License, Version 1:
*   All rights reserved.  Inesonic, LLC retains all rights to this software, including the right to relicense the
*   software in source or binary formats under different terms.  Unauthorized use under the terms of this license is
*   strictly prohibited.
*
* GNU Public License, Version 2:
*   This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
*   License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later
*   version.
*
*   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
*   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
*   details.
*
*   You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
*   Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
********************************************************************************************************************//**
* \file
*
* This file implements the \ref Ld::DataType class.
***********************************************************************************************************************/

#include <QString>
#include <QList>
#include <QHash>

#include <cassert>

#include <util_hash_functions.h>

#include <model_api_types.h>

#include "ld_element_structures.h"
#include "ld_element.h"
#include "ld_data_type_translator.h"
#include "ld_data_type_private.h"
#include "ld_data_type.h"

#include "dynamic_conversion_table.inc"

namespace Ld {
    const DataType::Property DataType::defaultProperties = DataType::assignable;

    QHash<DataType::ValueType, DataType> DataType::dataTypesByValueType;
    QHash<QString, DataType>             DataType::dataTypesByElement;
    QHash<QString, DataType>             DataType::matrixDataTypesByElement;
    QList<DataType>                      DataType::allDataTypes;
    DataType                             DataType::currentDefaultDataType;
    const DataType                       DataType::invalidDataType;
    const QString                        DataType::emptyString;
    const QList<DataType>                DataType::emptyList;

    bool DataType::registerType(
            DataType::ValueType valueType,
            const QString&      description,
            const QString&      typeSymbolElementName,
            DataType::Property  properties
        ) {
        bool result;

        if (!dataTypesByValueType.contains(valueType)) {
            bool matrixType = ((properties & matrix) != 0);

            if (typeSymbolElementName.isEmpty()) {
                DataType newDataType(valueType, description, typeSymbolElementName, properties);

                allDataTypes.append(newDataType);
                dataTypesByValueType.insert(valueType, newDataType);

                result = true;
            } else if (( matrixType && !matrixDataTypesByElement.contains(typeSymbolElementName)) ||
                       (!matrixType && !dataTypesByElement.contains(typeSymbolElementName))          ) {
                DataType newDataType(valueType, description, typeSymbolElementName, properties);

                allDataTypes.append(newDataType);
                dataTypesByValueType.insert(valueType, newDataType);

                if (matrixType) {
                    matrixDataTypesByElement.insert(typeSymbolElementName, newDataType);
                } else {
                    dataTypesByElement.insert(typeSymbolElementName, newDataType);
                }

                result = true;
            } else {
                result = false;
            }
        } else {
            result = false;
        }

        return result;
    }


    const QList<DataType>& DataType::allTypes() {
        return allDataTypes;
    }


    DataType DataType::fromValueType(DataType::ValueType valueType) {
        return dataTypesByValueType[valueType];
    }


    DataType DataType::fromSymbol(const QString& typeName, bool matrixType) {
        return matrixType ? matrixDataTypesByElement.value(typeName) : dataTypesByElement.value(typeName);
    }


    DataType DataType::fromSymbol(ElementPointer element, bool matrixType) {
        return element.isNull() ? invalidDataType : fromSymbol(element->typeName(), matrixType);
    }


    void DataType::setDefaultDataType(const DataType& defaultType) {
        currentDefaultDataType = defaultType;
    }


    DataType DataType::defaultDataType() {
        return currentDefaultDataType;
    }


    bool DataType::registerTranslator(
            const QString&            generatorName,
            DataType::ValueType       valueType,
            const DataTypeTranslator* newTranslator
        ) {
        bool result;

        if (dataTypesByValueType.contains(valueType)) {
            QSharedPointer<DataType::Private> impl = dataTypesByValueType.value(valueType).impl;
            result = impl->registerTranslator(generatorName, newTranslator);
        } else {
            result = false;
        }

        return result;
    }


    bool DataType::registerDecoder(DataType::ValueType valueType, const DataTypeDecoder* newDecoder) {
        bool result;

        if (dataTypesByValueType.contains(valueType)) {
            QSharedPointer<DataType::Private> impl = dataTypesByValueType.value(valueType).impl;
            result = impl->registerDecoder(newDecoder);
        } else {
            result = false;
        }

        return result;
    }


    void DataType::destroyDecoders() {
        for (QList<DataType>::iterator it=allDataTypes.begin(),end=allDataTypes.end() ; it!=end ; ++it) {
            it->impl->destroyDecoder();
        }
    }


    DataType::DynamicConversion DataType::canDynamicallyConvert(
            DataType::ValueType fromType,
            DataType::ValueType toType
        ) {
        return dynamicConversionTable[static_cast<unsigned>(fromType)][static_cast<unsigned>(toType)];
    }


    DataType::DataType() {}


    DataType::DataType(const DataType& other):impl(other.impl) {}


    DataType::~DataType() {}


    bool DataType::isValid() const {
        return !impl.isNull();
    }


    bool DataType::isInvalid() const {
        return impl.isNull();
    }


    DataType::ValueType DataType::valueType() const {
        return impl.isNull() ? DataType::ValueType::NONE : impl->valueType();
    }


    const QString& DataType::description() const {
        return impl.isNull() ? emptyString : impl->description();
    }


    const QString& DataType::typeSymbolElementName() const {
        return impl.isNull() ? emptyString : impl->typeSymbolElementName();
    }


    ElementPointer DataType::typeSymbolElement() const {
        return impl.isNull() ? ElementPointer() : impl->typeSymbolElement();
    }


    DataType::Property DataType::properties() const {
        return impl.isNull() ? nullProperty : impl->properties();
    }


    bool DataType::registerTranslator(const QString& generatorName, const DataTypeTranslator* newTranslator) {
        return impl.isNull() ? false : impl->registerTranslator(generatorName, newTranslator);
    }


    const DataTypeTranslator* DataType::translator(const QString& generatorName) const {
        return impl.isNull() ? nullptr : impl->translator(generatorName);
    }


    const DataTypeTranslator* DataType::translator(const QString& generatorName, DataType::ValueType valueType) {
        const DataType& dataType = dataTypesByValueType.value(valueType);
        return dataType.translator(generatorName);
    }


    bool DataType::registerDecoder(const DataTypeDecoder* newDecoder) {
        return impl->registerDecoder(newDecoder);
    }


    const DataTypeDecoder* DataType::decoder() const {
        return impl->decoder();
    }


    const DataTypeDecoder* DataType::decoder(DataType::ValueType valueType) {
        const DataType& dataType = dataTypesByValueType.value(valueType);
        return dataType.impl->decoder();
    }


    DataType::ValueType DataType::baseTypeFromMatrixType(DataType::ValueType matrixValueType) {
        ValueType result;

        switch (matrixValueType) {
            case ValueType::VARIANT: {
                result = ValueType::VARIANT;
                break;
            }

            case ValueType::MATRIX_BOOLEAN: {
                result = ValueType::BOOLEAN;
                break;
            }

            case ValueType::MATRIX_INTEGER: {
                result = ValueType::INTEGER;
                break;
            }

            case ValueType::MATRIX_REAL: {
                result = ValueType::REAL;
                break;
            }

            case ValueType::MATRIX_COMPLEX: {
                result = ValueType::COMPLEX;
                break;
            }

            default: {
                result = ValueType::NONE;
                break;
            }
        }

        return result;
    }


    DataType::ValueType DataType::matrixTypeFromBaseType(DataType::ValueType baseType) {
        ValueType result;

        switch (baseType) {
            case ValueType::BOOLEAN: {
                result = ValueType::MATRIX_BOOLEAN;
                break;
            }

            case ValueType::INTEGER: {
                result = ValueType::MATRIX_INTEGER;
                break;
            }

            case ValueType::REAL: {
                result = ValueType::MATRIX_REAL;
                break;
            }

            case ValueType::COMPLEX: {
                result = ValueType::MATRIX_COMPLEX;
                break;
            }

            default: {
                result = ValueType::NONE;
                break;
            }
        }

        return result;
    }


    DataType& DataType::operator=(const DataType& other) {
        impl = other.impl;
        return *this;
    }


    bool DataType::operator==(const DataType& other) const {
        return impl == other.impl;
    }


    bool DataType::operator!=(const DataType& other) const {
        return impl != other.impl;
    }


    DataType:: DataType(
            DataType::ValueType valueType,
            const QString&      description,
            const QString&      typeSymbolElementName,
            DataType::Property  properties
        ):impl(new Private(
            valueType,
            description,
            typeSymbolElementName,
            properties
        )) {}


    Util::HashResult qHash(const DataType& entry, Util::HashSeed seed) {
        return ::qHash(entry.impl.data(), seed);
    }
}


namespace Model {
    Util::HashResult qHash(ValueType valueType, Util::HashSeed seed) {
        return ::qHash(static_cast<unsigned char>(valueType), seed);
    }
}
