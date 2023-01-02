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
* This file implements the \ref Ld::HtmlMatrixBooleanDataTypeTranslator class.
***********************************************************************************************************************/

#include <QString>

#include <cassert>

#include <model_intrinsic_types.h>
#include <model_variant.h>
#include <model_matrix_boolean.h>

#include <util_string.h>

#include "ld_element.h"
#include "ld_format_structures.h"
#include "ld_matrix_data_type_format.h"
#include "ld_boolean_data_type_format.h"
#include "ld_matrix_boolean_data_type_format.h"
#include "ld_value_field_format.h"
#include "ld_element_structures.h"
#include "ld_html_code_generator.h"
#include "ld_html_code_generation_engine.h"
#include "ld_html_code_generator_diagnostic.h"
#include "ld_xml_export_context.h"
#include "ld_identifier_container.h"
#include "ld_html_data_type_translator.h"
#include "ld_html_matrix_boolean_data_type_translator.h"

/***********************************************************************************************************************
 * HtmlMatrixBooleanDataTypeTranslator::Context:
 */

namespace Ld {
    HtmlMatrixBooleanDataTypeTranslator::Context::Context(
            const Model::MatrixBoolean& matrix,
            FormatPointer               format
        ):MatrixDataTypeDecoder::Context(
            format
        ),HtmlMatrixDataTypeTranslator::Context(
            matrix,
            format
        ),MatrixBooleanDataTypeDecoder::Context(
            matrix,
            format
        ) {}


    HtmlMatrixBooleanDataTypeTranslator::Context::~Context() {}


    bool HtmlMatrixBooleanDataTypeTranslator::Context::toMathML(
            unsigned long             row,
            unsigned long             column,
            ElementPointer            /* element */,
            HtmlCodeGenerationEngine& engine
        ) {
        XmlExportContext& xmlContext = engine.context();

        const Model::MatrixBoolean&
            matrixBoolean = dynamic_cast<const Model::MatrixBoolean&>(MatrixBooleanDataTypeDecoder::Context::matrix());
        Model::Boolean v = matrixBoolean(row, column);

        xmlContext.writeStartElement("mn");
        xmlContext.writeCharacters(Util::booleanToUnicodeString(v, booleanStyle()));
        xmlContext.writeEndElement(); // mn

        return true;
    }


    bool HtmlMatrixBooleanDataTypeTranslator::Context::toLaTeX(
            unsigned long             row,
            unsigned long             column,
            ElementPointer            /* element */,
            HtmlCodeGenerationEngine& engine
        ) {
        XmlExportContext& xmlContext = engine.context();

        const Model::MatrixBoolean&
            matrixBoolean = dynamic_cast<const Model::MatrixBoolean&>(MatrixBooleanDataTypeDecoder::Context::matrix());
        Model::Boolean v = matrixBoolean(row, column);

        xmlContext.writeCharacters(Util::booleanToUnicodeString(v, booleanStyle()));

        return true;
    }


    bool HtmlMatrixBooleanDataTypeTranslator::Context::toHtml(
            unsigned long             row,
            unsigned long             column,
            ElementPointer            /* element */,
            HtmlCodeGenerationEngine& engine
        ) {
        const Model::MatrixBoolean&
            matrixBoolean = dynamic_cast<const Model::MatrixBoolean&>(MatrixBooleanDataTypeDecoder::Context::matrix());
        Model::Boolean v = matrixBoolean(row, column);
        QString        s = Util::booleanToUnicodeString(v, booleanStyle());

        insertText(s, format(), engine);

        return true;
    }
}

/***********************************************************************************************************************
 * HtmlMatrixBooleanDataTypeTranslator:
 */

namespace Ld {
    const HtmlMatrixBooleanDataTypeTranslator HtmlMatrixBooleanDataTypeTranslator::instance;

    HtmlMatrixBooleanDataTypeTranslator::HtmlMatrixBooleanDataTypeTranslator() {}


    HtmlMatrixBooleanDataTypeTranslator::~HtmlMatrixBooleanDataTypeTranslator() {}


    bool HtmlMatrixBooleanDataTypeTranslator::toMathML(
            const Model::Variant&     variant,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
        ) const {
        bool                 success;
        Model::MatrixBoolean matrix = variant.toMatrixBoolean(&success);

        if (success) {
            FormatPointer format = element->format();
            Context context(matrix, format);
            success = translateToMathML(context, element, engine);
        }

        return success;
    }


    bool HtmlMatrixBooleanDataTypeTranslator::toLaTeX(
            const Model::Variant&     variant,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
        ) const {
        bool                 success;
        Model::MatrixBoolean matrix = variant.toMatrixBoolean(&success);

        if (success) {
            FormatPointer format = element->format();
            Context context(matrix, format);
            success = translateToLaTeX(context, element, engine);
        }

        return success;
    }


    bool HtmlMatrixBooleanDataTypeTranslator::toHtml(
            const Model::Variant&     variant,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
        ) const {
        bool                 success;
        Model::MatrixBoolean matrix = variant.toMatrixBoolean(&success);

        if (success) {
            FormatPointer format = element->format();
            Context context(matrix, format);
            success = translateToMathML(context, element, engine);
        }

        return success;
    }
}
