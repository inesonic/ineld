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
* This file implements the \ref Ld::CalculatedValue class.
***********************************************************************************************************************/

#include <QString>

#include <model_variant.h>

#include "ld_data_type.h"
#include "ld_variable_name.h"
#include "ld_calculated_value.h"
#include "ld_calculated_value_private.h"

namespace Ld {
    static const VariableName dummyVariableName;
    static const QString      dummyString;
    static const DataType     dummyDataType;

    CalculatedValue::CalculatedValue() {}


    CalculatedValue::CalculatedValue(
            const Ld::VariableName& name,
            const Model::Variant&   variant
        ):impl(
            new Private(name, variant)
        ) {}


    CalculatedValue::CalculatedValue(
            const QString&        name,
            const Model::Variant& variant
        ):impl(
            new Private(
                VariableName(name),
                variant
            )
        ) {}


    CalculatedValue::CalculatedValue(
            const QString&        name1,
            const QString&        name2,
            const Model::Variant& variant
        ):impl(
            new Private(
                VariableName(name1, name2),
                variant
            )
        ) {}


    CalculatedValue::CalculatedValue(
            const CalculatedValue& other
        ):impl(
            other.impl
        ) {}


    bool CalculatedValue::isValid() const {
        return !impl.isNull() && impl->isValid();
    }


    bool CalculatedValue::isInvalid() const {
        return !isValid();
    }


    const Ld::VariableName& CalculatedValue::name() const {
        return !impl.isNull() ? impl->name() : dummyVariableName;
    }


    QString CalculatedValue::name1() const {
        return !impl.isNull() ? impl->name().text1() : QString();
    }


    QString CalculatedValue::name2() const {
        return !impl.isNull() ? impl->name().text2() : QString();
    }


    Ld::DataType::ValueType CalculatedValue::valueType() const {
        return !impl.isNull() ? impl->valueType() : DataType::ValueType::NONE;
    }


    const Model::Variant& CalculatedValue::variant() const {
        return *impl;
    }


    const DataType& CalculatedValue::dataType() const {
        return !impl.isNull() ? impl->dataType() : dummyDataType;
    }


    const QString& CalculatedValue::description() const {
        return !impl.isNull() ? impl->description(): dummyString;
    }


    const QString& CalculatedValue::debugString() const {
        return !impl.isNull() ? impl->debugString() : dummyString;
    }


    const QString& CalculatedValue::detailedDescription() const {
        return !impl.isNull() ? impl->detailedDescription() : dummyString;
    }


    CalculatedValue& CalculatedValue::operator=(const CalculatedValue& other) {
        impl = other.impl;
        return *this;
    }
}
