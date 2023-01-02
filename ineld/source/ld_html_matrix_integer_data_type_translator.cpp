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
* This file implements the \ref Ld::HtmlMatrixIntegerDataTypeTranslator class.
***********************************************************************************************************************/

#include <QString>

#include <cassert>

#include <model_intrinsic_types.h>
#include <model_variant.h>
#include <model_matrix_integer.h>

#include <util_string.h>

#include "ld_element.h"
#include "ld_format_structures.h"
#include "ld_matrix_data_type_format.h"
#include "ld_integer_data_type_format.h"
#include "ld_matrix_integer_data_type_format.h"
#include "ld_value_field_format.h"
#include "ld_element_structures.h"
#include "ld_html_code_generator.h"
#include "ld_html_code_generation_engine.h"
#include "ld_html_code_generator_diagnostic.h"
#include "ld_xml_export_context.h"
#include "ld_identifier_container.h"
#include "ld_html_data_type_translator.h"
#include "ld_html_matrix_integer_data_type_translator.h"

/***********************************************************************************************************************
 * HtmlMatrixIntegerDataTypeTranslator::Context:
 */

namespace Ld {
    HtmlMatrixIntegerDataTypeTranslator::Context::Context(
            const Model::MatrixInteger& matrix,
            FormatPointer               format
        ):MatrixDataTypeDecoder::Context(
            format
        ),HtmlMatrixDataTypeTranslator::Context(
            matrix,
            format
        ),MatrixIntegerDataTypeDecoder::Context(
            matrix,
            format
        ) {}


    HtmlMatrixIntegerDataTypeTranslator::Context::~Context() {}


    bool HtmlMatrixIntegerDataTypeTranslator::Context::toMathML(
            unsigned long             row,
            unsigned long             column,
            ElementPointer            /* element */,
            HtmlCodeGenerationEngine& engine
        ) {
        XmlExportContext& xmlContext = engine.context();

        const Model::MatrixInteger&
            matrixInteger = dynamic_cast<const Model::MatrixInteger&>(MatrixIntegerDataTypeDecoder::Context::matrix());
        Model::Integer v = matrixInteger(row, column);
        QString        s = Util::longLongIntegerToUnicodeString(
            v,
            integerNumberStyle(),
            numberDigits(),
            base(),
            upperCase()
        );

        xmlContext.writeStartElement("mn");
        xmlContext.writeCharacters(s);
        xmlContext.writeEndElement(); // mn

        return true;
    }


    bool HtmlMatrixIntegerDataTypeTranslator::Context::toLaTeX(
            unsigned long             row,
            unsigned long             column,
            ElementPointer            /* element */,
            HtmlCodeGenerationEngine& engine
        ) {
        XmlExportContext& xmlContext = engine.context();

        const Model::MatrixInteger&
            matrixInteger = dynamic_cast<const Model::MatrixInteger&>(MatrixIntegerDataTypeDecoder::Context::matrix());
        Model::Integer v = matrixInteger(row, column);
        QString        s = Util::longLongIntegerToUnicodeString(
            v,
            integerNumberStyle(),
            numberDigits(),
            base(),
            upperCase()
        );

        xmlContext.writeCharacters(s);

        return true;
    }


    bool HtmlMatrixIntegerDataTypeTranslator::Context::toHtml(
            unsigned long             row,
            unsigned long             column,
            ElementPointer            /* element */,
            HtmlCodeGenerationEngine& engine
        ) {
        const Model::MatrixInteger&
            matrixInteger = dynamic_cast<const Model::MatrixInteger&>(MatrixIntegerDataTypeDecoder::Context::matrix());
        Model::Integer v = matrixInteger(row, column);
        QString        s = Util::longLongIntegerToUnicodeString(
            v,
            integerNumberStyle(),
            numberDigits(),
            base(),
            upperCase()
        );

        insertText(s, format(), engine);

        return true;
    }
}

/***********************************************************************************************************************
 * HtmlMatrixIntegerDataTypeTranslator:
 */

namespace Ld {
    const HtmlMatrixIntegerDataTypeTranslator HtmlMatrixIntegerDataTypeTranslator::instance;

    HtmlMatrixIntegerDataTypeTranslator::HtmlMatrixIntegerDataTypeTranslator() {}


    HtmlMatrixIntegerDataTypeTranslator::~HtmlMatrixIntegerDataTypeTranslator() {}


    bool HtmlMatrixIntegerDataTypeTranslator::toMathML(
            const Model::Variant&     variant,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
        ) const {
        bool                 success;
        Model::MatrixInteger matrix = variant.toMatrixInteger(&success);

        if (success) {
            FormatPointer format = element->format();
            Context context(matrix, format);
            success = translateToMathML(context, element, engine);
        }

        return success;
    }


    bool HtmlMatrixIntegerDataTypeTranslator::toLaTeX(
            const Model::Variant&     variant,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
        ) const {
        bool                 success;
        Model::MatrixInteger matrix = variant.toMatrixInteger(&success);

        if (success) {
            FormatPointer format = element->format();
            Context context(matrix, format);
            success = translateToLaTeX(context, element, engine);
        }

        return success;
    }


    bool HtmlMatrixIntegerDataTypeTranslator::toHtml(
            const Model::Variant&     variant,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
        ) const {
        bool                 success;
        Model::MatrixInteger matrix = variant.toMatrixInteger(&success);

        if (success) {
            FormatPointer format = element->format();
            Context context(matrix, format);
            success = translateToMathML(context, element, engine);
        }

        return success;
    }
}
