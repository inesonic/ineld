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
* This file implements the \ref Ld::HtmlTextElementTranslator class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>
#include <QColor>

#include <algorithm>

#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_text_element.h"
#include "ld_font_format.h"
#include "ld_character_format.h"
#include "ld_xml_export_context.h"
#include "ld_code_generation_engine.h"
#include "ld_html_code_generator_diagnostic.h"
#include "ld_html_code_generation_engine.h"
#include "ld_html_translator.h"
#include "ld_html_text_element_translator.h"

namespace Ld {
    HtmlTextElementTranslator::~HtmlTextElementTranslator() {}


    QString HtmlTextElementTranslator::elementName() const {
        return Ld::TextElement::elementName;
    }


    bool HtmlTextElementTranslator::body(ElementPointer element, HtmlCodeGenerationEngine& engine) {
        bool    success;
        QString text          = element->text();
        bool    escapeMathJax = (engine.mathMode() == HtmlCodeGenerationEngine::MathMode::MATHJAX_LATEX);

        if (!text.isEmpty()) {
            XmlExportContext& context = engine.context();
            FormatPointer     format = element->format();

            if (!format.isNull()) {
                if (engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML5_WITH_CSS) {
                    QString className = engine.cssClassForFormat(format);
                    if (!className.isEmpty()) {
                        context.writeStartElement("span");
                        context.writeAttribute("class", className);
                        success = writeEscapedText(text, engine, escapeMathJax);
                        context.writeEndElement(); // span tag
                    } else {
                        success = writeEscapedText(text, engine, escapeMathJax);
                    }
                } else if (engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML4_WITHOUT_CSS) {
                    QSharedPointer<FontFormat> fontFormat = format.dynamicCast<FontFormat>();

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

                    unsigned numberEndTags = 1;

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

                    if (format->capabilities().contains(CharacterFormat::formatName)) {
                        QSharedPointer<CharacterFormat> characterFormat = format.dynamicCast<CharacterFormat>();

                        if (characterFormat->position() == CharacterFormat::Position::SUBSCRIPT) {
                            context.writeStartElement("sub");
                            ++numberEndTags;
                        } else if (characterFormat->position() == CharacterFormat::Position::SUPERSCRIPT) {
                            context.writeStartElement("sup");
                            ++numberEndTags;
                        }
                    }

                    success = writeEscapedText(text, engine, escapeMathJax);

                    while (numberEndTags > 0) {
                        context.writeEndElement();
                        --numberEndTags;
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
            } else {
               success = writeEscapedText(text, engine, escapeMathJax);
            }
        } else {
            success = true;
        }

        return success;
    }
}
