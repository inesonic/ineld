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
* This file implements the \ref Ld::LaTeXRealDataTypeTranslator class.
***********************************************************************************************************************/

#include <QString>

#include <model_intrinsic_types.h>
#include <model_variant.h>

#include <util_string.h>

#include "ld_element.h"
#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_font_format.h"
#include "ld_real_data_type_format.h"
#include "ld_value_field_format.h"
#include "ld_real_type_element.h"
#include "ld_latex_code_generator.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_latex_code_generator_diagnostic.h"
#include "ld_text_export_context.h"
#include "ld_latex_data_type_translator.h"
#include "ld_latex_real_data_type_translator.h"

namespace Ld {
    const LaTeXRealDataTypeTranslator LaTeXRealDataTypeTranslator::instance;

    LaTeXRealDataTypeTranslator::LaTeXRealDataTypeTranslator() {}


    LaTeXRealDataTypeTranslator::~LaTeXRealDataTypeTranslator() {}


    QString LaTeXRealDataTypeTranslator::elementName() const {
        return RealTypeElement::elementName;
    }


    bool LaTeXRealDataTypeTranslator::toLaTeX(
            const Model::Variant&      variant,
            ElementPointer             element,
            LaTeXCodeGenerationEngine& engine
        ) const {
        bool        success;
        Model::Real value = variant.toReal(&success);

        if (success) {
            bool                                upperCase;
            unsigned                            precision;
            double                              multiplier;
            RealDataTypeFormat::RealNumberStyle realNumberStyle;

            FormatPointer format = element->format();
            if (!format.isNull()                                                         &&
                (format->capabilities().contains(RealDataTypeFormat::formatName) ||
                 format->capabilities().contains(ValueFieldFormat::formatName)         )    ) {
                QSharedPointer<Ld::RealDataTypeFormat> realFormat = format.dynamicCast<RealDataTypeFormat>();

                upperCase       = realFormat->upperCase();
                precision       = realFormat->precision();
                multiplier      = realFormat->multiplier();
                realNumberStyle = realFormat->realNumberStyle();
            } else {
                upperCase       = RealDataTypeFormat::defaultUpperCase;
                precision       = RealDataTypeFormat::defaultPrecision;
                multiplier      = RealDataTypeFormat::defaultMultiplier;
                realNumberStyle = RealDataTypeFormat::defaultRealNumberStyle;
            }

            QString mantissaString;
            QString exponentString;

            Util::longDoubleToMantissaAndExponentStrings(
                value * multiplier,
                realNumberStyle,
                precision,
                mantissaString,
                exponentString
            );

            TextExportContext& context = engine.context();

            if (exponentString.isEmpty()) {
                context << mantissaString;
            } else {
                if (realNumberStyle == RealDataTypeFormat::RealNumberStyle::COMPUTER_SCIENTIFIC ||
                    realNumberStyle == RealDataTypeFormat::RealNumberStyle::COMPUTER_CONCISE       ) {
                    QString text =   upperCase
                                   ? tr("%1E%2").arg(mantissaString).arg(exponentString)
                                   : tr("%1e%2").arg(mantissaString).arg(exponentString);
                    context << text;
                } else {
                    if (exponentString.length() == 1) {
                        context << tr("%1 \\times 10 ^ %2").arg(mantissaString).arg(exponentString);
                    } else {
                        context <<  tr("%1 \\times 10 ^ { %2 }").arg(mantissaString).arg(exponentString);
                    }
                }
            }
        }

        return success;
    }
}
