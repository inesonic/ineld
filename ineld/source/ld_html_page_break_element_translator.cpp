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
* This file implements the \ref Ld::HtmlPageBreakElementTranslator class.
***********************************************************************************************************************/

#include <QString>

#include "ld_element_structures.h"
#include "ld_page_break_element.h"
#include "ld_xml_export_context.h"
#include "ld_code_generation_engine.h"
#include "ld_html_code_generator_diagnostic.h"
#include "ld_html_code_generation_engine.h"
#include "ld_html_translator.h"
#include "ld_html_page_break_element_translator.h"

namespace Ld {
    HtmlPageBreakElementTranslator::~HtmlPageBreakElementTranslator() {}


    QString HtmlPageBreakElementTranslator::elementName() const {
        return Ld::PageBreakElement::elementName;
    }


    bool HtmlPageBreakElementTranslator::body(ElementPointer, HtmlCodeGenerationEngine& engine) {
        bool success;

        if (engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML5_WITH_CSS) {
            XmlExportContext& context = engine.context();

            context.writeStartElement("p");
            context.writeAttribute("style", "page-break-after: always;");
            context.writeEntityReference("nbsp");
            context.writeEndElement();

            context.writeStartElement("p");
            context.writeAttribute("style", "page-break-before: always;");
            context.writeEntityReference("nbsp");
            context.writeEndElement();

            success = true;
        } else if (engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML4_WITHOUT_CSS) {
            success = true;
        } else {
            HtmlCodeGeneratorDiagnostic* diagnostic = new HtmlCodeGeneratorDiagnostic(
                nullptr,
                HtmlCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                dynamic_cast<const HtmlTranslationPhase&>(engine.translationPhase()),
                HtmlCodeGeneratorDiagnostic::Code::HTML_STYLE_NOT_SUPPORTED,
                engine.contextPointer()
            );

            engine.translationErrorDetected(diagnostic);
            success = false;
        }

        return success;
    }
}
