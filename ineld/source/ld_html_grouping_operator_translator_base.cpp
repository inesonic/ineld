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
* This file implements the \ref Ld::HtmlGroupingOperatorTranslatorBase class.
***********************************************************************************************************************/

#include <QString>

#include <cassert>

#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_format.h"
#include "ld_operator_format.h"
#include "ld_xml_export_context.h"
#include "ld_code_generation_engine.h"
#include "ld_html_code_generator_diagnostic.h"
#include "ld_html_code_generation_engine.h"
#include "ld_html_translator.h"
#include "ld_html_grouping_operator_translator_base.h"

namespace Ld {
    HtmlGroupingOperatorTranslatorBase::~HtmlGroupingOperatorTranslatorBase() {}


    bool HtmlGroupingOperatorTranslatorBase::body(ElementPointer element, HtmlCodeGenerationEngine& engine) {
        bool                               result;
        HtmlCodeGenerationEngine::MathMode mathMode = engine.mathMode();
        XmlExportContext&                  context  = engine.context();
        ElementPointer                     child0   = element->child(0);

        switch (mathMode) {
            case HtmlCodeGenerationEngine::MathMode::NO_MATH: {
                result = true;
                break;
            }

            case HtmlCodeGenerationEngine::MathMode::MATHML:
            case HtmlCodeGenerationEngine::MathMode::MATHJAX_MATHML: {
                QString leftParenthesis  = mathMlLeftParenthesis(element);
                QString rightParenthesis = mathMlRightParenthesis(element);

                context.writeStartElement("mo");
                context.writeAttribute("fence", "true");
                context.writeAttribute("stretchy", "true");
                context.writeCharacters(leftParenthesis);
                context.writeEndElement(); // mo
                context.writeStartElement("mrow");

                engine.translateChild(child0);

                context.writeEndElement(); // mrow
                context.writeStartElement("mo");
                context.writeAttribute("fence", "true");
                context.writeAttribute("stretchy", "true");
                context.writeCharacters(rightParenthesis);
                context.writeEndElement(); // mo

                result = true;
                break;
            }

            case HtmlCodeGenerationEngine::MathMode::MATHJAX_LATEX: {
                QString leftParenthesis  = latexLeftParenthesis(element);
                QString rightParenthesis = latexRightParenthesis(element);

                context.writeCharacters("\\left ");
                context.writeCharacters(leftParenthesis);

                engine.translateChild(child0);

                context.writeCharacters("\\right ");
                context.writeCharacters(rightParenthesis);

                result = true;
                break;
            }

            case HtmlCodeGenerationEngine::MathMode::IMAGES: {
                result = writeAsImage(element, engine);
                break;
            }

            default: {
                result = false;

                assert(false);
                break;
            }
        }

        return result;
    }
}
