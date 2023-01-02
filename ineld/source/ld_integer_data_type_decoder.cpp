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
* This file implements the \ref Ld::IntegerDataTypeDecoder class.
***********************************************************************************************************************/

#include <QString>

#include <util_string.h>

#include <model_intrinsic_types.h>
#include <model_variant.h>

#include "ld_format_structures.h"
#include "ld_integer_data_type_format.h"
#include "ld_data_type_decoder.h"
#include "ld_integer_data_type_decoder.h"
#include "ld_value_field_format.h"

namespace Ld {
    IntegerDataTypeDecoder::IntegerDataTypeDecoder() {}


    IntegerDataTypeDecoder::~IntegerDataTypeDecoder() {}


    QString IntegerDataTypeDecoder::toString(const Model::Variant& value, Ld::FormatPointer format) const {
        QString result;

        bool           ok;
        Model::Integer numericValue = value.toInteger(&ok);

        if (ok) {
            unsigned                                      numberDigits;
            unsigned                                      base;
            bool                                          upperCase;
            Ld::IntegerDataTypeFormat::IntegerNumberStyle style;

            if (!format.isNull()                                                             &&
                (format->capabilities().contains(Ld::IntegerDataTypeFormat::formatName) ||
                 format->capabilities().contains(Ld::ValueFieldFormat::formatName)         )    ) {
                QSharedPointer<Ld::IntegerDataTypeFormat>
                    integerFormat = format.dynamicCast<Ld::IntegerDataTypeFormat>();
                numberDigits = integerFormat->width();
                base         = integerFormat->base();
                upperCase    = integerFormat->upperCase();
                style        = integerFormat->integerNumberStyle();
            } else {
                numberDigits = Ld::IntegerDataTypeFormat::defaultWidth;
                base         = Ld::IntegerDataTypeFormat::defaultBase;
                upperCase    = true;
                style        = Ld::IntegerDataTypeFormat::defaultIntegerNumberStyle;
            }

            result = Util::longLongIntegerToUnicodeString(
                numericValue,
                style,
                numberDigits == Ld::IntegerDataTypeFormat::ignoreWidth ? Util::ignoreNumberDigits : numberDigits,
                base,
                upperCase
            );
        } else {
            result = tr("???");
        }

        return result;
    }


    QString IntegerDataTypeDecoder::toDescription(const Model::Variant& /* value */) const {
        return tr("Integer (signed, 64-bit)");
    }
}
