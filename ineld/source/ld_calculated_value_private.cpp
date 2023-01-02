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
* This file implements the \ref Ld::CalculatedValue::Private class.
***********************************************************************************************************************/

#include <QCoreApplication>
#include <QString>

#include <model_variant.h>

#include "ld_data_type.h"
#include "ld_data_type_decoder.h"
#include "ld_variable_name.h"
#include "ld_calculated_value.h"
#include "ld_calculated_value_private.h"

namespace Ld {
    CalculatedValue::Private::Private(
            const Ld::VariableName& name,
            const Model::Variant&   variant
        ):Model::Variant(
            variant
        ),currentVariableName(
            name
        ) {}


    bool CalculatedValue::Private::isValid() const {
        return valueType() != Ld::DataType::ValueType::NONE;
    }


    const Ld::VariableName& CalculatedValue::Private::name() const {
        return currentVariableName;
    }


    const DataType& CalculatedValue::Private::dataType() const {
        if (currentDataType.isInvalid()) {
            currentDataType = DataType::fromValueType(valueType());
        }

        return currentDataType;
    }


    const QString& CalculatedValue::Private::description() const {
        if (currentDescription.isEmpty()) {
            const Ld::DataType& dataType = CalculatedValue::Private::dataType();

            if (dataType.isInvalid()) {
                currentDescription = tr("Unknown type");
            } else {
                const Ld::DataTypeDecoder* decoder = dataType.decoder();
                currentDescription = decoder->toDescription(*this);
            }
        }

        return currentDescription;
    }


    const QString& CalculatedValue::Private::debugString() const {
        if (currentDebugString.isEmpty()) {
            const Ld::DataType& dataType = CalculatedValue::Private::dataType();

            if (dataType.isInvalid()) {
                currentDebugString = tr("Unknown type");
            } else {
                const Ld::DataTypeDecoder* decoder = dataType.decoder();
                if (dataType.properties() & (Ld::DataType::matrix | Ld::DataType::container)) {
                    currentDebugString = tr("%1\nUse inspector to view contents.").arg(decoder->toDescription(*this));
                } else {
                    currentDebugString = tr("%1\n%2")
                                         .arg(decoder->toDescription(*this))
                                         .arg(decoder->toString(*this));
                }
            }
        }

        return currentDebugString;
    }


    const QString& CalculatedValue::Private::detailedDescription() const {
        if (currentDetailedDescription.isEmpty()) {
            const Ld::DataType& dataType = CalculatedValue::Private::dataType();

            if (dataType.isInvalid()) {
                currentDetailedDescription = tr("Unknown type");
            } else {
                const Ld::DataTypeDecoder* decoder = dataType.decoder();
                if (dataType.properties() & (Ld::DataType::matrix | Ld::DataType::container)) {
                    currentDebugString = tr("%1").arg(decoder->toDescription(*this));
                } else {
                    currentDebugString = tr("%1 : %2")
                                         .arg(decoder->toString(*this))
                                         .arg(decoder->toDescription(*this));
                }
            }
        }

        return currentDebugString;
    }
}
