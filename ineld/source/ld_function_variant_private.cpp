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
* This file implements the \ref Ld::FunctionVariant::Private class.
***********************************************************************************************************************/

#include <QString>
#include <QList>
#include <QMap>

#include <cassert>

#include <model_api_types.h>

#include "ld_variable_name.h"
#include "ld_data_type.h"
#include "ld_capabilities.h"
#include "ld_element_structures.h"
#include "ld_function_variant.h"
#include "ld_function_variant_private.h"

/***********************************************************************************************************************
 * Ld::FunctionVariant::Private::Container
 */

namespace Ld {
    FunctionVariant::Private::Container::Container() {}


    FunctionVariant::Private::Container::Container(
            FunctionVariant::Private* privateImplementation
        ):currentInstance(
            privateImplementation
        ) {}


    FunctionVariant::Private::Container::Container(
            QSharedPointer<FunctionVariant::Private> privateImplementation
        ):currentInstance(
            privateImplementation
        ) {}


    FunctionVariant::Private::Container::Container(const Container& other):currentInstance(other.currentInstance) {}


    FunctionVariant::Private::Container::~Container() {}


    bool FunctionVariant::Private::Container::isValid() const {
        return !currentInstance.isNull();
    }


    bool FunctionVariant::Private::Container::isInvalid() const {
        return currentInstance.isNull();
    }


    QSharedPointer<FunctionVariant::Private> FunctionVariant::Private::Container::pointer() const {
        return currentInstance;
    }


    bool FunctionVariant::Private::Container::allowsVariadicParameters() const {
        return currentInstance->allowsVariadicParameters();
    }


    const QList<DataType::ValueType>& FunctionVariant::Private::Container::supportedVariadicTypes() const {
        return currentInstance->supportedVariadicTypes();
    }


    DataType::ValueType FunctionVariant::Private::Container::returnValueType() const {
        return currentInstance->returnValueType();
    }


    unsigned FunctionVariant::Private::Container::numberParameters() const {
        return currentInstance->numberParameters();
    }


    const QList<DataType::ValueType>& FunctionVariant::Private::Container::parameterTypes() const {
        return currentInstance->parameterTypes();
    }


    const QList<QString> FunctionVariant::Private::Container::parameterDescriptions() const {
        return currentInstance->parameterDescriptions();
    }


    QList<Capabilities> FunctionVariant::Private::Container::parameterCapabilities() const {
        return currentInstance->parameterCapabilities();
    }


    Capabilities FunctionVariant::Private::Container::variantCapabilities() const {
        return currentInstance->variantCapabilities();
    }


    FunctionVariant::Private::Container& FunctionVariant::Private::Container::operator=(
            const FunctionVariant::Private::Container& other
        ) {
        currentInstance = other.currentInstance;
        return *this;
    }


    bool FunctionVariant::Private::Container::operator==(const FunctionVariant::Private::Container& other) const {
        return (
               !currentInstance.isNull()
            && !other.currentInstance.isNull()
            && currentInstance->supportedVariadicTypes() == other.currentInstance->supportedVariadicTypes()
            && currentInstance->returnValueType() == other.currentInstance->returnValueType()
            && currentInstance->parameterCapabilities() == other.currentInstance->parameterCapabilities()
            && currentInstance->parameterDescriptions() == other.currentInstance->parameterDescriptions()
        );
    }


    bool FunctionVariant::Private::Container::operator!=(const FunctionVariant::Private::Container& other) const {
        return !operator==(other);
    }


    bool FunctionVariant::Private::Container::operator<(const FunctionVariant::Private::Container& other) const {
        bool result;
        if (currentInstance.isNull() || other.currentInstance.isNull()) {
            result = false;
        } else {
            unsigned thisRVT  = static_cast<unsigned>(currentInstance->returnValueType());
            unsigned otherRVT = static_cast<unsigned>(other.currentInstance->returnValueType());

            if (thisRVT < otherRVT) {
                result = true;
            } else if (thisRVT > otherRVT) {
                result = false;
            } else /* if (thisRVT == otherRVT) */ {
                const QList<DataType::ValueType>& thisSVT   = currentInstance->supportedVariadicTypes();
                const QList<DataType::ValueType>& otherSVT  = other.currentInstance->supportedVariadicTypes();
                int                               svtResult = compare(thisSVT, otherSVT);

                if (svtResult < 0) {
                    result = true;
                } else if (svtResult > 0) {
                    result = false;
                } else {
                    const QList<DataType::ValueType>& thisPT   = currentInstance->parameterTypes();
                    const QList<DataType::ValueType>& otherPT  = other.currentInstance->parameterTypes();
                    int                               ptResult = compare(thisPT, otherPT);

                    if (ptResult < 0) {
                        result = true;
                    } else if (ptResult > 0) {
                        result = false;
                    } else {
                        const QList<QString>& thisPD      = currentInstance->parameterDescriptions();
                        const QList<QString>& otherPD     = other.currentInstance->parameterDescriptions();
                        unsigned              thisPdSize  = static_cast<unsigned>(thisPD.size());
                        unsigned              otherPdSize = static_cast<unsigned>(otherPD.size());

                        if (thisPdSize < otherPdSize) {
                            result = true; // This should never happen but we check anyway.
                        } else if (thisPdSize > otherPdSize) {
                            result = false; // This should never happen but we check anyway.
                        } else {
                            unsigned index = 0;
                            while (index < thisPdSize && thisPD.at(index) == otherPD.at(index)) {
                                ++index;
                            }

                            if (index < thisPdSize) {
                                result = (thisPD.at(index) < otherPD.at(index));
                            } else {
                                result = false;
                            }
                        }
                    }
                }
            }
        }

        return result;
    }


    bool FunctionVariant::Private::Container::operator>(const FunctionVariant::Private::Container& other) const {
        bool result;
        if (currentInstance.isNull() || other.currentInstance.isNull()) {
            result = false;
        } else {
            unsigned thisRVT  = static_cast<unsigned>(currentInstance->returnValueType());
            unsigned otherRVT = static_cast<unsigned>(other.currentInstance->returnValueType());

            if (thisRVT < otherRVT) {
                result = false;
            } else if (thisRVT > otherRVT) {
                result = true;
            } else /* if (thisRVT == otherRVT) */ {
                const QList<DataType::ValueType>& thisSVT   = currentInstance->supportedVariadicTypes();
                const QList<DataType::ValueType>& otherSVT  = other.currentInstance->supportedVariadicTypes();
                int                               svtResult = compare(thisSVT, otherSVT);

                if (svtResult < 0) {
                    result = false;
                } else if (svtResult > 0) {
                    result = true;
                } else {
                    const QList<DataType::ValueType>& thisPT   = currentInstance->parameterTypes();
                    const QList<DataType::ValueType>& otherPT  = other.currentInstance->parameterTypes();
                    int                               ptResult = compare(thisPT, otherPT);

                    if (ptResult < 0) {
                        result = false;
                    } else if (ptResult > 0) {
                        result = true;
                    } else {
                        const QList<QString>& thisPD      = currentInstance->parameterDescriptions();
                        const QList<QString>& otherPD     = other.currentInstance->parameterDescriptions();
                        unsigned              thisPdSize  = static_cast<unsigned>(thisPD.size());
                        unsigned              otherPdSize = static_cast<unsigned>(otherPD.size());

                        if (thisPdSize < otherPdSize) {
                            result = false; // This should never happen but we check anyway.
                        } else if (thisPdSize > otherPdSize) {
                            result = true; // This should never happen but we check anyway.
                        } else {
                            unsigned index = 0;
                            while (index < thisPdSize && thisPD.at(index) == otherPD.at(index)) {
                                ++index;
                            }

                            if (index < thisPdSize) {
                                result = (thisPD.at(index) > otherPD.at(index));
                            } else {
                                result = false;
                            }
                        }
                    }
                }
            }
        }

        return result;
    }


    bool FunctionVariant::Private::Container::operator<=(const FunctionVariant::Private::Container& other) const {
        return !operator>(other);
    }


    bool FunctionVariant::Private::Container::operator>=(const FunctionVariant::Private::Container& other) const {
        return !operator<(other);
    }


    int FunctionVariant::Private::Container::compare(
            const QList<DataType::ValueType>& list1,
            const QList<DataType::ValueType>& list2
        ) {
        int result;

        unsigned list1Size = static_cast<unsigned>(list1.size());
        unsigned list2Size = static_cast<unsigned>(list2.size());

        if (list1Size < list2Size) {
            result = -1;
        } else if (list1Size > list2Size) {
            result = +1;
        } else {
            unsigned index = 0;
            while (index < list1Size && list1.at(index) == list2.at(index)) {
                ++index;
            }

            if (index < list1Size) {
                result = static_cast<int>(list1.at(index)) - static_cast<int>(list2.at(index));
            } else {
                result = 0;
            }
        }

        return result;
    }
}

/***********************************************************************************************************************
 * Ld::FunctionVariant::Private
 */

namespace Ld {
    QMap<FunctionVariant::Private::Container, unsigned> FunctionVariant::Private::currentFunctionVariants;
    bool                                                FunctionVariant::Private::currentStaleIndexes = false;

    void FunctionVariant::Private::clearDatabase() {
        currentFunctionVariants.clear();
        currentStaleIndexes = false;
    }


    QSharedPointer<FunctionVariant::Private> FunctionVariant::Private::registerPrivateInstance(
            FunctionVariant::Private* instance
        ) {
        return registerPrivateInstance(QSharedPointer<Private>(instance));
    }


    QSharedPointer<FunctionVariant::Private> FunctionVariant::Private::registerPrivateInstance(
            QSharedPointer<FunctionVariant::Private> instance
        ) {
        QSharedPointer<Private> result = instance;

        Container container(instance);
        QMap<Container, unsigned>::iterator it = currentFunctionVariants.find(container);
        if (it != currentFunctionVariants.end()) {
            result = it.key().pointer();
        } else {
            currentFunctionVariants.insert(container, 0);
            currentStaleIndexes = true;
        }

        return result;
    }


    FunctionVariant::Private::Private(
            const QString&                    libraryName,
            const QList<DataType::ValueType>& supportedVariadicTypes,
            DataType::ValueType               returnValueType,
            QList<DataType::ValueType>        parameterTypes,
            QList<QString>                    parameterDescriptions
        ):currentLibrary(
            libraryName
        ),currentIndex(
            0
        ),currentSupportedVariadicTypes(
            supportedVariadicTypes
        ),currentReturnValueType(
            returnValueType
        ),currentParameterTypes(
            parameterTypes
        ),currentParameterDescriptions(
            parameterDescriptions
        ) {
        assert(currentParameterTypes.size() == currentParameterDescriptions.size());

        currentVariantCapabilities = capabilitiesFromValueType(returnValueType);
        for (  QList<DataType::ValueType>::const_iterator it  = parameterTypes.begin(), end = parameterTypes.end()
             ; it!=end
             ; ++it
            ) {
            currentParameterCapabilities.append(capabilitiesFromValueType(*it));
        }
    }


    FunctionVariant::Private::~Private() {}


    bool FunctionVariant::Private::allowsVariadicParameters() const {
        return !currentSupportedVariadicTypes.isEmpty();
    }


    const QList<DataType::ValueType>& FunctionVariant::Private::supportedVariadicTypes() const {
        return currentSupportedVariadicTypes;
    }


    QString FunctionVariant::Private::libraryName() const {
        return currentLibrary;
    }


    DataType::ValueType FunctionVariant::Private::returnValueType() const {
        return currentReturnValueType;
    }


    unsigned FunctionVariant::Private::numberParameters() const {
        return static_cast<unsigned>(currentParameterTypes.size());
    }


    const QList<DataType::ValueType>& FunctionVariant::Private::parameterTypes() const {
        return currentParameterTypes;
    }


    const QList<QString> FunctionVariant::Private::parameterDescriptions() const {
        return currentParameterDescriptions;
    }


    QList<Capabilities> FunctionVariant::Private::parameterCapabilities() const {
        return currentParameterCapabilities;
    }


    Capabilities FunctionVariant::Private::variantCapabilities() const {
        return currentVariantCapabilities;
    }


    bool FunctionVariant::Private::operator==(const FunctionVariant::Private& other) const {
        updateDatabase();
        return currentIndex == other.currentIndex;
    }


    bool FunctionVariant::Private::operator!=(const FunctionVariant::Private& other) const {
        return !operator==(other);
    }


    bool FunctionVariant::Private::operator<(const FunctionVariant::Private& other) const {
        updateDatabase();
        return currentIndex < other.currentIndex;
    }


    bool FunctionVariant::Private::operator>(const FunctionVariant::Private& other) const {
        updateDatabase();
        return currentIndex > other.currentIndex;
    }


    bool FunctionVariant::Private::operator<=(const FunctionVariant::Private& other) const {
        return !operator>(other);
    }


    bool FunctionVariant::Private::operator>=(const FunctionVariant::Private& other) const {
        return !operator<(other);
    }


    Capabilities FunctionVariant::Private::capabilitiesFromValueType(DataType::ValueType valueType) {
        Capabilities result;

        switch (valueType) {
            case DataType::ValueType::NONE:           { result = Capabilities();                           break; }
            case DataType::ValueType::VARIANT:        { result = Capabilities::allNumericAndMatrixRValues; break; }
            case DataType::ValueType::BOOLEAN:        { result = Capabilities::allBooleanRValues;          break; }
            case DataType::ValueType::INTEGER:        { result = Capabilities::allIntegerRValues;          break; }
            case DataType::ValueType::REAL:           { result = Capabilities::allRealRValues;             break; }
            case DataType::ValueType::COMPLEX:        { result = Capabilities::allComplexRValues;          break; }
            case DataType::ValueType::SET:            { result = Capabilities::allSetRValues;              break; }
            case DataType::ValueType::TUPLE:          { result = Capabilities::allTupleRValues;            break; }
            case DataType::ValueType::MATRIX_BOOLEAN: { result = Capabilities::allMatrixBooleanRValues;    break; }
            case DataType::ValueType::MATRIX_INTEGER: { result = Capabilities::allMatrixIntegerRValues;    break; }
            case DataType::ValueType::MATRIX_REAL:    { result = Capabilities::allMatrixRealRValues;       break; }
            case DataType::ValueType::MATRIX_COMPLEX: { result = Capabilities::allMatrixComplexRValues;    break; }

            case DataType::ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    void FunctionVariant::Private::updateDatabase() {
        if (currentStaleIndexes) {
            currentStaleIndexes = false;

            unsigned index = 0;
            for (  QMap<Container, unsigned>::iterator it  = currentFunctionVariants.begin(),
                                                       end = currentFunctionVariants.end()
                 ; it != end
                 ; ++it
                ) {
                QSharedPointer<Private> impl = it.key().pointer();
                impl->currentIndex = index;
                it.value()         = index;

                ++index;
            }
        }
    }
}
