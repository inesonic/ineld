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
* This file implements the \ref Ld::HtmlComplexDataTypeTranslator class.
***********************************************************************************************************************/

#include <QString>

#include <model_intrinsic_types.h>
#include <model_complex.h>
#include <model_variant.h>

#include <util_string.h>

#include "ld_complex_data_type_translator_helpers.h"
#include "ld_element.h"
#include "ld_complex_type_element.h"
#include "ld_complex_data_type_format.h"
#include "ld_value_field_format.h"
#include "ld_element_structures.h"
#include "ld_html_code_generator.h"
#include "ld_html_code_generation_engine.h"
#include "ld_html_code_generator_diagnostic.h"
#include "ld_xml_export_context.h"
#include "ld_identifier_container.h"
#include "ld_html_data_type_translator.h"
#include "ld_html_complex_data_type_translator.h"

namespace Ld {
    const HtmlComplexDataTypeTranslator HtmlComplexDataTypeTranslator::instance;

    HtmlComplexDataTypeTranslator::HtmlComplexDataTypeTranslator() {}


    HtmlComplexDataTypeTranslator::~HtmlComplexDataTypeTranslator() {}


    QString HtmlComplexDataTypeTranslator::elementName() const {
        return ComplexTypeElement::elementName;
    }


    bool HtmlComplexDataTypeTranslator::toMathML(
            const Model::Variant&     variant,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
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
            bool needRow = (
                   !realSuperscriptString.isEmpty()
                || !imaginarySuperscriptString.isEmpty()
                || (!realBaselineString.isEmpty() && !imaginaryBaselineString.isEmpty())
            );

            XmlExportContext& context = engine.context();

            if (needRow) {
                context.writeStartElement("mrow");
            }

            if (!realBaselineString.isEmpty()) {
                context.writeStartElement("mn");
                context.writeCharacters(realBaselineString);
                context.writeEndElement(); // mn

                if (!realSuperscriptString.isEmpty()) {
                    context.writeStartElement("mo");
                    context.writeAttribute("stretchy", "false");
                    context.writeCharacters(QChar(0x00D7));
                    context.writeEndElement(); // mo
                    context.writeStartElement("msup");
                    context.writeStartElement("mn");
                    context.writeCharacters(tr("10"));
                    context.writeEndElement(); // mn
                    if (realSuperscriptString.left(1) == tr("-")) {
                        context.writeStartElement("mrow");
                        context.writeStartElement("mo");
                        context.writeAttribute("stretchy", "false");
                        context.writeCharacters(tr("-"));
                        context.writeEndElement(); // mo

                        context.writeStartElement("mn");
                        context.writeCharacters(realSuperscriptString.mid(1));
                        context.writeEndElement(); // mn
                    } else {
                        context.writeStartElement("mn");
                        context.writeCharacters(realSuperscriptString);
                        context.writeEndElement(); // mn
                    }

                    context.writeEndElement(); // msup
                }
            }

            if (!imaginaryBaselineString.isEmpty()) {
                QString firstCharacter = imaginaryBaselineString.left(1);
                if (firstCharacter != tr("+") && firstCharacter != tr("-") && !realBaselineString.isEmpty()) {
                    context.writeStartElement("mo");
                    context.writeAttribute("stretchy", "false");
                    context.writeCharacters(tr("+"));
                    context.writeEndElement(); // mo
                }

                context.writeStartElement("mn");
                context.writeCharacters(imaginaryBaselineString);
                context.writeEndElement(); // mn

                if (!imaginarySuperscriptString.isEmpty()) {
                    context.writeStartElement("mo");
                    context.writeAttribute("stretchy", "false");
                    context.writeCharacters(QChar(0x00D7));
                    context.writeEndElement(); // mo
                    context.writeStartElement("msup");
                    context.writeStartElement("mn");
                    context.writeCharacters(tr("10"));
                    context.writeEndElement(); // mn
                    if (imaginarySuperscriptString.left(1) == tr("-")) {
                        context.writeStartElement("mrow");

                        context.writeStartElement("mo");
                        context.writeAttribute("stretchy", "false");
                        context.writeCharacters(tr("-"));
                        context.writeEndElement(); // mo

                        context.writeStartElement("mn");
                        context.writeCharacters(imaginarySuperscriptString.mid(1));
                        context.writeEndElement(); // mn

                        context.writeEndElement(); // mrow
                    } else {
                        context.writeStartElement("mn");
                        context.writeCharacters(imaginarySuperscriptString);
                        context.writeEndElement(); // mn
                    }

                    context.writeEndElement(); // msup
                }

                context.writeStartElement("mn");
                context.writeCharacters(imaginaryUnitString);
                context.writeEndElement(); // mn
            }

            if (needRow) {
                context.writeEndElement(); // mrow
            }
        }

        return success;
    }


    bool HtmlComplexDataTypeTranslator::toLaTeX(
            const Model::Variant&     variant,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
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
            XmlExportContext& context = engine.context();

            if (!realBaselineString.isEmpty()) {
                if (!realSuperscriptString.isEmpty()) {
                    context.writeCharacters(
                        tr("\\text{%1}\\times\\text{10}^\\text{%2}").arg(realBaselineString).arg(realSuperscriptString)
                    );
                } else {
                    context.writeCharacters(tr("\\text{%1}").arg(realBaselineString));
                }
            }

            if (!imaginaryBaselineString.isEmpty()) {
                QString firstCharacter = imaginaryBaselineString.left(1);
                if (firstCharacter != tr("+") && firstCharacter != tr("-") && !realBaselineString.isEmpty()) {
                    context.writeCharacters(tr("\\text{+}"));
                }

                if (!imaginarySuperscriptString.isEmpty()) {
                    context.writeCharacters(
                        tr("\\text{%1}\\times\\text{10}^\\text{%2}")
                            .arg(imaginaryBaselineString)
                            .arg(imaginarySuperscriptString)
                    );
                } else {
                    context.writeCharacters(tr("\\text{%1}").arg(imaginaryBaselineString));
                }

                context.writeCharacters(tr("\\text{%1}").arg(imaginaryUnitString));
            }
        }

        return success;
    }


    bool HtmlComplexDataTypeTranslator::toHtml(
            const Model::Variant&     variant,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
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
            FormatPointer format = element->format();

            XmlExportContext& context       = engine.context();
            unsigned          numberEndTags = 0;

            if (engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML5_WITH_CSS) {
                QString className = engine.cssClassForFormat(format);
                if (!className.isEmpty()) {
                    context.writeStartElement("span");
                    context.writeAttribute("class", className);
                    numberEndTags = 1;
                }
            } else if (engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML4_WITHOUT_CSS) {
                QSharedPointer<FontFormat> fontFormat = format.dynamicCast<FontFormat>();
                if (!fontFormat.isNull()) {
                    context.writeStartElement("font");
                    context.writeAttribute("face", fontFormat->family());

                    int size = std::max(1, std::min(7, 3 + (static_cast<int>(fontFormat->fontSize()) - 12) / 2));
                    if (size != 3) {
                        context.writeAttribute("size", QString::number(size));
                    }

                    QColor fontColor = fontFormat->fontColor();
                    if (fontColor.isValid()) {
                        context.writeAttribute("color", fontColor.name());
                    }

                    numberEndTags = 1;

                    if (fontFormat->italics()) {
                        context.writeStartElement("i");
                        ++numberEndTags;
                    }

                    unsigned fontWeightValue   = static_cast<unsigned>(fontFormat->fontWeight());
                    unsigned normalWeightValue = static_cast<unsigned>(FontFormat::Weight::NORMAL);
                    if (fontWeightValue > normalWeightValue) {
                        context.writeStartElement("b");
                        ++numberEndTags;
                    }

                    if (fontFormat->underline()) {
                        context.writeStartElement("u");
                        ++numberEndTags;
                    }

                    if (fontFormat->strikeout()) {
                        context.writeStartElement("s");
                        ++numberEndTags;
                    }
                }
            } else {
                HtmlCodeGeneratorDiagnostic* diagnostic = new HtmlCodeGeneratorDiagnostic(
                    element,
                    HtmlCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                    dynamic_cast<const HtmlTranslationPhase&>(engine.translationPhase()),
                    HtmlCodeGeneratorDiagnostic::Code::HTML_STYLE_NOT_SUPPORTED,
                    engine.contextPointer()
                );

                engine.translationErrorDetected(diagnostic);
                success = false;
            }

            if (success) {
                if (!realBaselineString.isEmpty()) {
                    context.writeCharacters(realBaselineString);
                }

                if (!realSuperscriptString.isEmpty()) {
                    context.writeCharacters(QChar(0x00D7) + tr("10"));
                    context.writeStartElement("sup");
                    context.writeCharacters(realSuperscriptString);
                    context.writeEndElement(); // sup
                }

                if (!imaginaryBaselineString.isEmpty()) {
                    QString firstCharacter = imaginaryBaselineString.left(1);
                    if (firstCharacter != tr("+") && firstCharacter != tr("-")) {
                        context.writeCharacters(tr("+"));
                    }

                    context.writeCharacters(imaginaryBaselineString);

                    if (!imaginarySuperscriptString.isEmpty()) {
                        context.writeCharacters(QChar(0x00D7) + tr("10"));
                        context.writeStartElement("sup");
                        context.writeCharacters(imaginarySuperscriptString);
                        context.writeEndElement(); // sup
                    }

                    context.writeCharacters(imaginaryUnitString);
                }
            }

            while (numberEndTags > 0) {
                context.writeEndElement();
                --numberEndTags;
            }
        }

        return success;
    }
}
