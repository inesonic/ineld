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
* This file implements the \ref Ld::HtmlMatrixComplexDataTypeTranslator class.
***********************************************************************************************************************/

#include <QString>

#include <cassert>

#include <model_intrinsic_types.h>
#include <model_variant.h>
#include <model_complex.h>
#include <model_matrix_complex.h>

#include <util_string.h>

#include "ld_complex_data_type_translator_helpers.h"

#include "ld_element.h"
#include "ld_format_structures.h"
#include "ld_matrix_data_type_format.h"
#include "ld_complex_data_type_format.h"
#include "ld_matrix_complex_data_type_format.h"
#include "ld_value_field_format.h"
#include "ld_element_structures.h"
#include "ld_html_code_generator.h"
#include "ld_html_code_generation_engine.h"
#include "ld_html_code_generator_diagnostic.h"
#include "ld_xml_export_context.h"
#include "ld_identifier_container.h"
#include "ld_html_data_type_translator.h"
#include "ld_html_matrix_complex_data_type_translator.h"

/***********************************************************************************************************************
 * HtmlMatrixComplexDataTypeTranslator::Context:
 */

namespace Ld {
    HtmlMatrixComplexDataTypeTranslator::Context::Context(
            const Model::MatrixComplex& matrix,
            FormatPointer               format
        ):MatrixDataTypeDecoder::Context(
            format
        ),HtmlMatrixDataTypeTranslator::Context(
            matrix,
            format
        ),MatrixComplexDataTypeDecoder::Context(
            matrix,
            format
        ) {}


    HtmlMatrixComplexDataTypeTranslator::Context::~Context() {}


    bool HtmlMatrixComplexDataTypeTranslator::Context::toMathML(
            unsigned long             row,
            unsigned long             column,
            ElementPointer            /* element */,
            HtmlCodeGenerationEngine& engine
        ) {
        XmlExportContext& xmlContext = engine.context();

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

        bool needRow = (
               !realSuperscriptString.isEmpty()
            || !imaginarySuperscriptString.isEmpty()
            || (!realBaselineString.isEmpty() && !imaginaryBaselineString.isEmpty())
        );

        if (needRow) {
            xmlContext.writeStartElement("mrow");
        }

        if (!realBaselineString.isEmpty()) {
            xmlContext.writeStartElement("mn");
            xmlContext.writeCharacters(realBaselineString);
            xmlContext.writeEndElement(); // mn

            if (!realSuperscriptString.isEmpty()) {
                xmlContext.writeStartElement("mo");
                xmlContext.writeAttribute("stretchy", "false");
                xmlContext.writeCharacters(QChar(0x00D7));
                xmlContext.writeEndElement(); // mo

                xmlContext.writeStartElement("msup");
                xmlContext.writeStartElement("mn");
                xmlContext.writeCharacters(tr("10"));
                xmlContext.writeEndElement(); // mn

                if (realSuperscriptString.left(1) == tr("-")) {
                    xmlContext.writeStartElement("mrow");

                    xmlContext.writeStartElement("mo");
                    xmlContext.writeAttribute("stretchy", "false");
                    xmlContext.writeCharacters(tr("-"));
                    xmlContext.writeEndElement(); // mo

                    xmlContext.writeStartElement("mn");
                    xmlContext.writeCharacters(realSuperscriptString.mid(1));
                    xmlContext.writeEndElement(); // mn

                    xmlContext.writeEndElement(); // mrow
                } else {
                    xmlContext.writeStartElement("mn");
                    xmlContext.writeCharacters(realSuperscriptString);
                    xmlContext.writeEndElement(); // mn
                }

                xmlContext.writeEndElement(); // msup
            }
        }

        if (!imaginaryBaselineString.isEmpty()) {
            QString firstCharacter = imaginaryBaselineString.left(1);
            if (firstCharacter != tr("+") && firstCharacter != tr("-") && !realBaselineString.isEmpty()) {
                xmlContext.writeStartElement("mo");
                xmlContext.writeAttribute("stretchy", "false");
                xmlContext.writeCharacters(tr("+"));
                xmlContext.writeEndElement(); // mo
            }

            xmlContext.writeStartElement("mn");
            xmlContext.writeCharacters(imaginaryBaselineString);
            xmlContext.writeEndElement(); // mn

            if (!imaginarySuperscriptString.isEmpty()) {
                xmlContext.writeStartElement("mo");
                xmlContext.writeAttribute("stretchy", "false");
                xmlContext.writeCharacters(QChar(0x00D7));
                xmlContext.writeEndElement(); // mo

                xmlContext.writeStartElement("msup");
                xmlContext.writeStartElement("mn");
                xmlContext.writeCharacters(tr("10"));
                xmlContext.writeEndElement(); // mn

                if (imaginarySuperscriptString.left(1) == tr("-")) {
                    xmlContext.writeStartElement("mrow");

                    xmlContext.writeStartElement("mo");
                    xmlContext.writeAttribute("stretchy", "false");
                    xmlContext.writeCharacters(tr("-"));
                    xmlContext.writeEndElement(); // mo

                    xmlContext.writeStartElement("mn");
                    xmlContext.writeCharacters(imaginarySuperscriptString.mid(1));
                    xmlContext.writeEndElement(); // mn

                    xmlContext.writeEndElement(); // mrow
                } else {
                    xmlContext.writeStartElement("mn");
                    xmlContext.writeCharacters(imaginarySuperscriptString);
                    xmlContext.writeEndElement(); // mn
                }

                xmlContext.writeEndElement(); // msup
            }

            xmlContext.writeStartElement("mn");
            xmlContext.writeCharacters(imaginaryUnitString);
            xmlContext.writeEndElement(); // mn
        }

        if (needRow) {
            xmlContext.writeEndElement(); // mrow
        }

        return true;
    }


    bool HtmlMatrixComplexDataTypeTranslator::Context::toLaTeX(
            unsigned long             row,
            unsigned long             column,
            ElementPointer            /* element */,
            HtmlCodeGenerationEngine& engine
        ) {
        XmlExportContext& xmlContext = engine.context();

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
                xmlContext.writeCharacters(
                    tr("\\text{%1}\\times\\text{10}^\\text{%2}").arg(realBaselineString).arg(realSuperscriptString)
                );
            } else {
                xmlContext.writeCharacters(tr("\\text{%1}").arg(realBaselineString));
            }
        }

        if (!imaginaryBaselineString.isEmpty()) {
            QString firstCharacter = imaginaryBaselineString.left(1);
            if (firstCharacter != tr("+") && firstCharacter != tr("-") && !realBaselineString.isEmpty()) {
                xmlContext.writeCharacters(tr("\\text{+}"));
            }

            if (!imaginarySuperscriptString.isEmpty()) {
                xmlContext.writeCharacters(
                    tr("\\text{%1}\\times\\text{10}^\\text{%2}")
                        .arg(imaginaryBaselineString)
                        .arg(imaginarySuperscriptString)
                );
            } else {
                xmlContext.writeCharacters(tr("\\text{%1}").arg(imaginaryBaselineString));
            }

            xmlContext.writeCharacters(tr("\\text{%1}").arg(imaginaryUnitString));
        }

        return true;
    }


    bool HtmlMatrixComplexDataTypeTranslator::Context::toHtml(
            unsigned long             row,
            unsigned long             column,
            ElementPointer            /* element */,
            HtmlCodeGenerationEngine& engine
        ) {
        XmlExportContext& xmlContext = engine.context();

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
            insertText(realBaselineString, format(), engine);

            if (!realSuperscriptString.isEmpty()) {
                xmlContext.writeCharacters(tr("&times;10"));
                xmlContext.writeStartElement("sup");
                insertText(realSuperscriptString, format(), engine);
                xmlContext.writeEndElement();
            }
        }

        if (!imaginaryBaselineString.isEmpty()) {
            insertText(imaginaryBaselineString, format(), engine);

            if (!imaginarySuperscriptString.isEmpty()) {
                xmlContext.writeCharacters(tr("&times;10"));
                xmlContext.writeStartElement("sup");
                insertText(imaginarySuperscriptString, format(), engine);
                xmlContext.writeEndElement();
            }

            xmlContext.writeCharacters(imaginaryUnitString);
        }

        return true;
    }
}

/***********************************************************************************************************************
 * HtmlMatrixComplexDataTypeTranslator:
 */

namespace Ld {
    const HtmlMatrixComplexDataTypeTranslator HtmlMatrixComplexDataTypeTranslator::instance;

    HtmlMatrixComplexDataTypeTranslator::HtmlMatrixComplexDataTypeTranslator() {}


    HtmlMatrixComplexDataTypeTranslator::~HtmlMatrixComplexDataTypeTranslator() {}


    bool HtmlMatrixComplexDataTypeTranslator::toMathML(
            const Model::Variant&     variant,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
        ) const {
        bool                 success;
        Model::MatrixComplex matrix = variant.toMatrixComplex(&success);

        if (success) {
            FormatPointer format = element->format();
            Context context(matrix, format);
            success = translateToMathML(context, element, engine);
        }

        return success;
    }


    bool HtmlMatrixComplexDataTypeTranslator::toLaTeX(
            const Model::Variant&     variant,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
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


    bool HtmlMatrixComplexDataTypeTranslator::toHtml(
            const Model::Variant&     variant,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
        ) const {
        bool                 success;
        Model::MatrixComplex matrix = variant.toMatrixComplex(&success);

        if (success) {
            FormatPointer format = element->format();
            Context context(matrix, format);
            success = translateToMathML(context, element, engine);
        }

        return success;
    }
}
