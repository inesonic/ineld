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
* This file implements the \ref Ld::DataType::Private class.
***********************************************************************************************************************/

#include <QString>
#include <QList>
#include <QHash>

#include <cassert>

#include <model_api_types.h>

#include "ld_element_structures.h"
#include "ld_element.h"
#include "ld_data_type_translator.h"
#include "ld_data_type_decoder.h"
#include "ld_data_type.h"
#include "ld_data_type_private.h"

namespace Ld {
    DataType::Private::Private(
            DataType::ValueType valueType,
            const QString&      description,
            const QString&      typeSymbolElementName,
            DataType::Property  properties
        ) {
        currentValueType             = valueType;
        currentDescription           = description;
        currentTypeSymbolElementName = typeSymbolElementName;
        currentProperties            = properties;
        currentDecoder               = nullptr;
    }


    DataType::Private::~Private() {}


    DataType::ValueType DataType::Private::valueType() const {
        return currentValueType;
    }


    const QString& DataType::Private::description() const {
        return currentDescription;
    }


    const QString& DataType::Private::typeSymbolElementName() const {
        return currentTypeSymbolElementName;
    }


    ElementPointer DataType::Private::typeSymbolElement() const {
        return Element::create(currentTypeSymbolElementName);
    }


    DataType::Property DataType::Private::properties() const {
        return currentProperties;
    }


    bool DataType::Private::registerTranslator(const QString& generatorName, const DataTypeTranslator* newTranslator) {
        bool result;
        if (!currentTranslators.contains(generatorName)) {
            currentTranslators.insert(generatorName, newTranslator);
            result = true;
        } else {
            result = false;
        }

        return result;
    }


    const DataTypeTranslator* DataType::Private::translator(const QString& generatorName) const {
        return currentTranslators.value(generatorName);
    }


    bool DataType::Private::registerDecoder(const DataTypeDecoder* newDecoder) {
        bool result;

        if (currentDecoder == nullptr) {
            currentDecoder = newDecoder;
            result         = true;
        } else {
            result = false;
        }

        return result;
    }


    const DataTypeDecoder* DataType::Private::decoder() const {
        return currentDecoder;
    }


    void DataType::Private::destroyDecoder() {
        delete currentDecoder;
    }
}
