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
* This file implements the \ref Ld::LaTeXMatrixRealDataTypeTranslator class.
***********************************************************************************************************************/

#include <QString>

#include <cassert>

#include <model_intrinsic_types.h>
#include <model_variant.h>
#include <model_matrix_real.h>

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
#include "ld_latex_matrix_data_type_translator.h"
#include "ld_latex_matrix_real_data_type_translator.h"

/***********************************************************************************************************************
 * LaTeXMatrixRealDataTypeTranslator::Context:
 */

namespace Ld {
    LaTeXMatrixRealDataTypeTranslator::Context::Context(
            const Model::MatrixReal& matrix,
            FormatPointer            format
        ):MatrixDataTypeDecoder::Context(
            format
        ),LaTeXMatrixDataTypeTranslator::Context(
            matrix,
            format
        ),MatrixRealDataTypeDecoder::Context(
            matrix,
            format
        ) {}


    LaTeXMatrixRealDataTypeTranslator::Context::~Context() {}


    bool LaTeXMatrixRealDataTypeTranslator::Context::toLaTeX(
            unsigned long              row,
            unsigned long              column,
            ElementPointer             /* element */,
            LaTeXCodeGenerationEngine& engine
        ) {
        TextExportContext& textContext = engine.context();

        const Model::MatrixReal&
            matrixReal = dynamic_cast<const Model::MatrixReal&>(MatrixRealDataTypeDecoder::Context::matrix());
        Model::Real v = matrixReal(row, column) * multiplier();

        QString mantissaString;
        QString exponentString;

        Util::longDoubleToMantissaAndExponentStrings(
            v,
            realNumberStyle(),
            precision(),
            mantissaString,
            exponentString
        );

        textContext << mantissaString;
        if (!exponentString.isEmpty()) {
            textContext << tr(" ^ { %1 }").arg(exponentString);
        }

        return true;
    }
}

/***********************************************************************************************************************
 * LaTeXMatrixRealDataTypeTranslator:
 */

namespace Ld {
    const LaTeXMatrixRealDataTypeTranslator LaTeXMatrixRealDataTypeTranslator::instance;

    LaTeXMatrixRealDataTypeTranslator::LaTeXMatrixRealDataTypeTranslator() {}


    LaTeXMatrixRealDataTypeTranslator::~LaTeXMatrixRealDataTypeTranslator() {}


    bool LaTeXMatrixRealDataTypeTranslator::toLaTeX(
            const Model::Variant&      variant,
            ElementPointer             element,
            LaTeXCodeGenerationEngine& engine
        ) const {
        bool              success;
        Model::MatrixReal matrix = variant.toMatrixReal(&success);

        if (success) {
            FormatPointer format = element->format();
            Context context(matrix, format);
            success = translateToLaTeX(context, element, engine);
        }

        return success;
    }
}
