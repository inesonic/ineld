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
* This file implements the \ref Ld::HtmlDataTypeTranslator class.
***********************************************************************************************************************/

#include <QString>

#include "ld_element.h"
#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_font_format.h"
#include "ld_html_code_generator.h"
#include "ld_html_code_generation_engine.h"
#include "ld_html_code_generator_diagnostic.h"
#include "ld_xml_export_context.h"
#include "ld_data_type_translator.h"
#include "ld_html_data_type_translator.h"

namespace Ld {
    HtmlDataTypeTranslator::HtmlDataTypeTranslator() {}


    HtmlDataTypeTranslator::~HtmlDataTypeTranslator() {}


    bool HtmlDataTypeTranslator::generateDefaultMathML(
            ElementPointer            /* element */,
            HtmlCodeGenerationEngine& engine
        ) {
        XmlExportContext& context = engine.context();
        context.writeStartElement("mtext");
        context.writeCharacters(tr("???"));
        context.writeEndElement();

        return true;
    }


    bool HtmlDataTypeTranslator::generateDefaultHtml(ElementPointer element, HtmlCodeGenerationEngine& engine) {
        bool              success       = true;
        XmlExportContext& context       = engine.context();
        unsigned          numberEndTags = 0;
        Ld::FormatPointer format        = element->format();

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
            context.writeCharacters("???");
        }

        while (numberEndTags > 0) {
            context.writeEndElement();
            --numberEndTags;
        }

        return success;
    }
}
