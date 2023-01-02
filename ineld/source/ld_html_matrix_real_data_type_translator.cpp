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
* This file implements the \ref Ld::HtmlMatrixRealDataTypeTranslator class.
***********************************************************************************************************************/

#include <QString>

#include <cassert>

#include <model_intrinsic_types.h>
#include <model_variant.h>
#include <model_matrix_real.h>

#include <util_string.h>

#include "ld_element.h"
#include "ld_format_structures.h"
#include "ld_matrix_data_type_format.h"
#include "ld_real_data_type_format.h"
#include "ld_matrix_real_data_type_format.h"
#include "ld_value_field_format.h"
#include "ld_element_structures.h"
#include "ld_html_code_generator.h"
#include "ld_html_code_generation_engine.h"
#include "ld_html_code_generator_diagnostic.h"
#include "ld_xml_export_context.h"
#include "ld_identifier_container.h"
#include "ld_html_data_type_translator.h"
#include "ld_html_matrix_real_data_type_translator.h"

/***********************************************************************************************************************
 * HtmlMatrixRealDataTypeTranslator::Context:
 */

namespace Ld {
    HtmlMatrixRealDataTypeTranslator::Context::Context(
            const Model::MatrixReal& matrix,
            FormatPointer               format
        ):MatrixDataTypeDecoder::Context(
            format
        ),HtmlMatrixDataTypeTranslator::Context(
            matrix,
            format
        ),MatrixRealDataTypeDecoder::Context(
            matrix,
            format
        ) {}


    HtmlMatrixRealDataTypeTranslator::Context::~Context() {}


    bool HtmlMatrixRealDataTypeTranslator::Context::toMathML(
            unsigned long             row,
            unsigned long             column,
            ElementPointer            /* element */,
            HtmlCodeGenerationEngine& engine
        ) {
        XmlExportContext& xmlContext = engine.context();

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

        if (exponentString.isEmpty()) {
            xmlContext.writeStartElement("mn");
            xmlContext.writeCharacters(mantissaString);
            xmlContext.writeEndElement(); // mn
        } else {
            xmlContext.writeStartElement("msup");
            xmlContext.writeStartElement("mn");
            xmlContext.writeCharacters(mantissaString);
            xmlContext.writeEndElement(); // mn
            xmlContext.writeStartElement("mn");
            xmlContext.writeCharacters(exponentString);
            xmlContext.writeEndElement(); // mn
            xmlContext.writeEndElement(); // msup
        }

        return true;
    }


    bool HtmlMatrixRealDataTypeTranslator::Context::toLaTeX(
            unsigned long             row,
            unsigned long             column,
            ElementPointer            /* element */,
            HtmlCodeGenerationEngine& engine
        ) {
        XmlExportContext& xmlContext = engine.context();

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

        if (exponentString.isEmpty()) {
            xmlContext.writeCharacters(mantissaString);
        } else {
            xmlContext.writeCharacters(tr("%1 ^ { %2 }").arg(mantissaString).arg(exponentString));
        }

        return true;
    }


    bool HtmlMatrixRealDataTypeTranslator::Context::toHtml(
            unsigned long             row,
            unsigned long             column,
            ElementPointer            /* element */,
            HtmlCodeGenerationEngine& engine
        ) {
        XmlExportContext& xmlContext = engine.context();

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

        insertText(mantissaString, format(), engine);
        if (!exponentString.isEmpty()) {
            xmlContext.writeStartElement("sup");
            insertText(exponentString, format(), engine);
            xmlContext.writeEndElement();
        }

        return true;
    }
}

/***********************************************************************************************************************
 * HtmlMatrixRealDataTypeTranslator:
 */

namespace Ld {
    const HtmlMatrixRealDataTypeTranslator HtmlMatrixRealDataTypeTranslator::instance;

    HtmlMatrixRealDataTypeTranslator::HtmlMatrixRealDataTypeTranslator() {}


    HtmlMatrixRealDataTypeTranslator::~HtmlMatrixRealDataTypeTranslator() {}


    bool HtmlMatrixRealDataTypeTranslator::toMathML(
            const Model::Variant&     variant,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
        ) const {
        bool              success;
        Model::MatrixReal matrix = variant.toMatrixReal(&success);

        if (success) {
            FormatPointer format = element->format();
            Context context(matrix, format);
            success = translateToMathML(context, element, engine);
        }

        return success;
    }


    bool HtmlMatrixRealDataTypeTranslator::toLaTeX(
            const Model::Variant&     variant,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
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


    bool HtmlMatrixRealDataTypeTranslator::toHtml(
            const Model::Variant&     variant,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
        ) const {
        bool              success;
        Model::MatrixReal matrix = variant.toMatrixReal(&success);

        if (success) {
            FormatPointer format = element->format();
            Context context(matrix, format);
            success = translateToMathML(context, element, engine);
        }

        return success;
    }
}
