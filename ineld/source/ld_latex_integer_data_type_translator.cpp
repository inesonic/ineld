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
* This file implements the \ref Ld::LaTeXIntegerDataTypeTranslator class.
***********************************************************************************************************************/

#include <QString>

#include <model_intrinsic_types.h>
#include <model_variant.h>

#include <util_string.h>

#include "ld_element.h"
#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_font_format.h"
#include "ld_integer_data_type_format.h"
#include "ld_value_field_format.h"
#include "ld_integer_type_element.h"
#include "ld_latex_code_generator.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_latex_code_generator_diagnostic.h"
#include "ld_text_export_context.h"
#include "ld_latex_data_type_translator.h"
#include "ld_latex_integer_data_type_translator.h"

namespace Ld {
    const LaTeXIntegerDataTypeTranslator LaTeXIntegerDataTypeTranslator::instance;

    LaTeXIntegerDataTypeTranslator::LaTeXIntegerDataTypeTranslator() {}


    LaTeXIntegerDataTypeTranslator::~LaTeXIntegerDataTypeTranslator() {}


    QString LaTeXIntegerDataTypeTranslator::elementName() const {
        return IntegerTypeElement::elementName;
    }


    bool LaTeXIntegerDataTypeTranslator::toLaTeX(
            const Model::Variant&      variant,
            ElementPointer             element,
            LaTeXCodeGenerationEngine& engine
        ) const {
        bool           success;
        Model::Integer value = variant.toInteger(&success);

        if (success) {
            bool                                      upperCase;
            unsigned                                  numberDigits;
            unsigned                                  base;
            IntegerDataTypeFormat::IntegerNumberStyle integerNumberStyle;

            FormatPointer format = element->format();
            if (!format.isNull()                                                             &&
                (format->capabilities().contains(Ld::IntegerDataTypeFormat::formatName) ||
                 format->capabilities().contains(Ld::ValueFieldFormat::formatName)         )    ) {
                QSharedPointer<Ld::IntegerDataTypeFormat> integerFormat = format.dynamicCast<IntegerDataTypeFormat>();

                upperCase          = integerFormat->upperCase();
                numberDigits       = integerFormat->width();
                base               = integerFormat->base();
                integerNumberStyle = integerFormat->integerNumberStyle();
            } else {
                upperCase          = IntegerDataTypeFormat::defaultUpperCase;
                numberDigits       = IntegerDataTypeFormat::defaultWidth;
                base               = IntegerDataTypeFormat::defaultBase;
                integerNumberStyle = IntegerDataTypeFormat::defaultIntegerNumberStyle;
            }

            TextExportContext& context = engine.context();
            if (integerNumberStyle != Ld::IntegerDataTypeFormat::IntegerNumberStyle::SUBSCRIPT_STYLE) {
                QString integerString = Util::longLongIntegerToUnicodeString(
                    value,
                    integerNumberStyle,
                    numberDigits,
                    base,
                    upperCase
                );

                context << integerString;
            } else {
                QString integerString = Util::longLongIntegerToUnicodeString(
                    value,
                    Util::IntegerNumberStyle::NONE,
                    numberDigits,
                    base,
                    upperCase
                );

                context << integerString;

                QString baseString = QString::number(base);
                if (baseString.length() == 1) {
                    context << tr(" _ %1").arg(baseString);
                } else {
                    context << tr(" _ { %1 }").arg(baseString);
                }
            }
        }

        return success;
    }
}
