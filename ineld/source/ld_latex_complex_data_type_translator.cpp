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
* This file implements the \ref Ld::LaTeXComplexDataTypeTranslator class.
***********************************************************************************************************************/

#include <QString>

#include <model_complex.h>
#include <model_variant.h>

#include <util_string.h>

#include "ld_complex_data_type_translator_helpers.h"
#include "ld_element.h"
#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_font_format.h"
#include "ld_complex_data_type_format.h"
#include "ld_value_field_format.h"
#include "ld_complex_type_element.h"
#include "ld_latex_code_generator.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_latex_code_generator_diagnostic.h"
#include "ld_text_export_context.h"
#include "ld_latex_data_type_translator.h"
#include "ld_latex_complex_data_type_translator.h"

namespace Ld {
    const LaTeXComplexDataTypeTranslator LaTeXComplexDataTypeTranslator::instance;

    LaTeXComplexDataTypeTranslator::LaTeXComplexDataTypeTranslator() {}


    LaTeXComplexDataTypeTranslator::~LaTeXComplexDataTypeTranslator() {}


    QString LaTeXComplexDataTypeTranslator::elementName() const {
        return ComplexTypeElement::elementName;
    }


    bool LaTeXComplexDataTypeTranslator::toLaTeX(
            const Model::Variant&      variant,
            ElementPointer             element,
            LaTeXCodeGenerationEngine& engine
        ) const {
        QString realBaselineString;
        QString realSuperscriptString;
        QString imaginaryBaselineString;
        QString imaginarySuperscriptString;
        QString imaginaryUnitString;

        bool success = generateComplexValueStrings(
            variant,
            element,
            realBaselineString,
            realSuperscriptString,
            imaginaryBaselineString,
            imaginarySuperscriptString,
            imaginaryUnitString
        );

        if (success) {
            TextExportContext& context = engine.context();

            if (!realBaselineString.isEmpty()) {
                context << realBaselineString;

                if (!realSuperscriptString.isEmpty()) {
                    if (realSuperscriptString.length() == 1) {
                        context << tr(" \\times 10 ^ %1 ").arg(realSuperscriptString);
                    } else {
                        context << tr(" \\times 10 ^ { %1 } ").arg(realSuperscriptString);
                    }
                }
            }

            if (!imaginaryBaselineString.isEmpty()) {
                QString firstCharacter = imaginaryBaselineString.left(1);
                if (firstCharacter != tr("+") && firstCharacter != tr("-") && !realBaselineString.isEmpty()) {
                    context << tr("+ ");
                }

                context << imaginaryBaselineString;

                if (!imaginarySuperscriptString.isEmpty()) {
                    if (realSuperscriptString.length() == 1) {
                        context << tr(" \\times 10 ^ %1 ").arg(imaginarySuperscriptString);
                    } else {
                        context << tr(" \\times 10 ^ { %1 } ").arg(imaginarySuperscriptString);
                    }
                }

                context << imaginaryUnitString << " ";
            }
        }

        return success;
    }
}
