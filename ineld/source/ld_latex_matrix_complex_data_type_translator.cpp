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
* This file implements the \ref Ld::LaTeXMatrixComplexDataTypeTranslator class.
***********************************************************************************************************************/

#include <QString>

#include <cassert>

#include <model_intrinsic_types.h>
#include <model_variant.h>
#include <model_matrix_complex.h>

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
#include "ld_latex_matrix_data_type_translator.h"
#include "ld_latex_matrix_complex_data_type_translator.h"

/***********************************************************************************************************************
 * LaTeXMatrixComplexDataTypeTranslator::Context:
 */

namespace Ld {
    LaTeXMatrixComplexDataTypeTranslator::Context::Context(
            const Model::MatrixComplex& matrix,
            FormatPointer            format
        ):MatrixDataTypeDecoder::Context(
            format
        ),LaTeXMatrixDataTypeTranslator::Context(
            matrix,
            format
        ),MatrixComplexDataTypeDecoder::Context(
            matrix,
            format
        ) {}


    LaTeXMatrixComplexDataTypeTranslator::Context::~Context() {}


    bool LaTeXMatrixComplexDataTypeTranslator::Context::toLaTeX(
            unsigned long              row,
            unsigned long              column,
            ElementPointer             /* element */,
            LaTeXCodeGenerationEngine& engine
        ) {
        TextExportContext& textContext = engine.context();

        const Model::MatrixComplex&
            matrixComplex = dynamic_cast<const Model::MatrixComplex&>(MatrixComplexDataTypeDecoder::Context::matrix());
        Model::Complex v = matrixComplex(row, column) * multiplier();

        QString realBaselineString;
        QString realSuperscriptString;
        QString imaginaryBaselineString;
        QString imaginarySuperscriptString;
        QString imaginaryUnitString;

        generateComplexValueStrings(
            v,
            realNumberStyle(),
            precision(),
            upperCase(),
            imaginaryUnit(),
            realBaselineString,
            realSuperscriptString,
            imaginaryBaselineString,
            imaginarySuperscriptString,
            imaginaryUnitString
        );

        if (!realBaselineString.isEmpty()) {
            if (!realSuperscriptString.isEmpty()) {
                textContext <<
                    tr("\\text{%1}\\times\\text{10}^\\text{%2}").arg(realBaselineString).arg(realSuperscriptString);
            } else {
                textContext << tr("\\text{%1}").arg(realBaselineString);
            }
        }

        if (!imaginaryBaselineString.isEmpty()) {
            QString firstCharacter = imaginaryBaselineString.left(1);
            if (firstCharacter != tr("+") && firstCharacter != tr("-") && !realBaselineString.isEmpty()) {
                textContext << tr("\\text{+}");
            }

            if (!imaginarySuperscriptString.isEmpty()) {
                textContext <<
                    tr("\\text{%1}\\times\\text{10}^\\text{%2}")
                        .arg(imaginaryBaselineString)
                        .arg(imaginarySuperscriptString);
            } else {
                textContext << tr("\\text{%1}").arg(imaginaryBaselineString);
            }

            textContext << tr("\\text{%1}").arg(imaginaryUnitString);
        }

        return true;
    }
}

/***********************************************************************************************************************
 * LaTeXMatrixComplexDataTypeTranslator:
 */

namespace Ld {
    const LaTeXMatrixComplexDataTypeTranslator LaTeXMatrixComplexDataTypeTranslator::instance;

    LaTeXMatrixComplexDataTypeTranslator::LaTeXMatrixComplexDataTypeTranslator() {}


    LaTeXMatrixComplexDataTypeTranslator::~LaTeXMatrixComplexDataTypeTranslator() {}


    bool LaTeXMatrixComplexDataTypeTranslator::toLaTeX(
            const Model::Variant&      variant,
            ElementPointer             element,
            LaTeXCodeGenerationEngine& engine
        ) const {
        bool                 success;
        Model::MatrixComplex matrix = variant.toMatrixComplex(&success);

        if (success) {
            FormatPointer format = element->format();
            Context context(matrix, format);
            success = translateToLaTeX(context, element, engine);
        }

        return success;
    }
}
