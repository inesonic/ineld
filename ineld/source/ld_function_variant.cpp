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
* This file implements the \ref Ld::FunctionVariant class.
***********************************************************************************************************************/

#include <QString>
#include <QList>
#include <QHash>

#include <cassert>

#include <model_api_types.h>

#include "ld_variable_name.h"
#include "ld_data_type.h"
#include "ld_capabilities.h"
#include "ld_element_structures.h"
#include "ld_function_variant.h"
#include "ld_function_variant_private.h"

namespace Ld {
    void FunctionVariant::clearDatabase() {
        Private::clearDatabase();
    }


    FunctionVariant::FunctionVariant() {}


    FunctionVariant::FunctionVariant(
            DataType::ValueType        returnValueType,
            QList<DataType::ValueType> parameterTypes,
            QList<QString>             parameterDescriptions
        ) {
        configure(QString(), QList<DataType::ValueType>(), returnValueType, parameterTypes, parameterDescriptions);
    }


    FunctionVariant::FunctionVariant(
            const QString&             libraryName,
            DataType::ValueType        returnValueType,
            QList<DataType::ValueType> parameterTypes,
            QList<QString>             parameterDescriptions
        ) {
        configure(libraryName, QList<DataType::ValueType>(), returnValueType, parameterTypes, parameterDescriptions);
    }


    FunctionVariant::FunctionVariant(
            const QList<DataType::ValueType>& supportedVariadicTypes,
            DataType::ValueType               returnValueType,
            QList<DataType::ValueType>        parameterTypes,
            QList<QString>                    parameterDescriptions
        ) {
        configure(QString(), supportedVariadicTypes, returnValueType, parameterTypes, parameterDescriptions);
    }


    FunctionVariant::FunctionVariant(
            const QString&                    libraryName,
            const QList<DataType::ValueType>& supportedVariadicTypes,
            DataType::ValueType               returnValueType,
            QList<DataType::ValueType>        parameterTypes,
            QList<QString>                    parameterDescriptions
        ) {
        configure(libraryName, supportedVariadicTypes, returnValueType, parameterTypes, parameterDescriptions);
    }


    FunctionVariant::FunctionVariant(
            const QList<DataType::ValueType>& supportedVariadicTypes,
            DataType::ValueType               returnValueType
        ) {
        configure(QString(), supportedVariadicTypes, returnValueType, QList<DataType::ValueType>(), QList<QString>());
    }


    FunctionVariant::FunctionVariant(
            const QString&                    libraryName,
            const QList<DataType::ValueType>& supportedVariadicTypes,
            DataType::ValueType               returnValueType
        ) {
        configure(
            libraryName,
            supportedVariadicTypes,
            returnValueType,
            QList<DataType::ValueType>(),
            QList<QString>()
        );
    }


    FunctionVariant::FunctionVariant(const FunctionVariant& other):impl(other.impl) {}


    FunctionVariant::~FunctionVariant() {}


    bool FunctionVariant::allowsVariadicParameters() const {
        return impl->allowsVariadicParameters();
    }


    bool FunctionVariant::doesNotAllowVariadicParameters() const {
        return !impl->allowsVariadicParameters();
    }


    const QList<DataType::ValueType>& FunctionVariant::supportedVariadicTypes() const {
        return impl->supportedVariadicTypes();
    }


    bool FunctionVariant::isValid() const {
        return !impl.isNull();
    }


    bool FunctionVariant::isInvalid() const {
        return impl.isNull();
    }


    QString FunctionVariant::libraryName() const {
        return impl->libraryName();
    }


    DataType::ValueType FunctionVariant::returnValueType() const {
        return impl->returnValueType();
    }


    unsigned FunctionVariant::numberParameters() const {
        return impl->numberParameters();
    }


    const QList<DataType::ValueType>& FunctionVariant::parameterTypes() const {
        return impl->parameterTypes();
    }


    const QList<QString> FunctionVariant::parameterDescriptions() const {
        return impl->parameterDescriptions();
    }


    QList<Capabilities> FunctionVariant::parameterCapabilities() const {
        return impl->parameterCapabilities();
    }


    Capabilities FunctionVariant::variantCapabilities() const {
        return impl->variantCapabilities();
    }


    FunctionVariant& FunctionVariant::operator=(const FunctionVariant& other) {
        impl = other.impl;
        return *this;
    }


    bool FunctionVariant::operator==(const FunctionVariant& other) const {
        return impl->operator==(*other.impl);
    }


    bool FunctionVariant::operator!=(const FunctionVariant& other) const {
        return !operator==(other);
    }


    bool FunctionVariant::operator<(const FunctionVariant& other) const {
        return impl->operator<(*other.impl);
    }


    bool FunctionVariant::operator>(const FunctionVariant& other) const {
        return impl->operator>(*other.impl);
    }


    bool FunctionVariant::operator<=(const FunctionVariant& other) const {
        return !operator>(other);
    }


    bool FunctionVariant::operator>=(const FunctionVariant& other) const {
        return !operator<(other);
    }


    void FunctionVariant::configure(
            const QString&                    libraryName,
            const QList<DataType::ValueType>& supportedVariadicTypes,
            DataType::ValueType               returnValueType,
            QList<DataType::ValueType>        parameterTypes,
            QList<QString>                    parameterDescriptions
        ) {
        Private* privateInstance = new Private(
            libraryName,
            supportedVariadicTypes,
            returnValueType,
            parameterTypes,
            parameterDescriptions
        );

        impl = Private::registerPrivateInstance(privateInstance);
    }


    void FunctionVariant::buildParameterData(
            QList<DataType::ValueType>& parameterTypesList,
            QList<QString>&             parameterDescriptionList,
            DataType::ValueType         parameterType,
            const QString&              parameterDescription
        ) {
        parameterTypesList.append(parameterType);
        parameterDescriptionList.append(parameterDescription);
    }
}
