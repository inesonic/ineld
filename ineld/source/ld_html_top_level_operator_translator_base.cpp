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
* This file implements the \ref Ld::HtmlTopLevelOperatorTranslatorBase class.
***********************************************************************************************************************/

#include <QString>

#include "ld_element_structures.h"
#include "ld_paragraph_element.h"
#include "ld_text_element.h"
#include "ld_font_format.h"
#include "ld_xml_export_context.h"
#include "ld_code_generation_engine.h"
#include "ld_html_code_generator_diagnostic.h"
#include "ld_html_code_generation_engine.h"
#include "ld_html_translator.h"
#include "ld_html_top_level_operator_translator_base.h"

namespace Ld {
    HtmlTopLevelOperatorTranslatorBase::~HtmlTopLevelOperatorTranslatorBase() {}


    bool HtmlTopLevelOperatorTranslatorBase::body(ElementPointer element, HtmlCodeGenerationEngine& engine) {
        bool                               result;
        HtmlCodeGenerationEngine::MathMode mathMode = engine.mathMode();

        XmlExportContext& context = engine.context();

        ElementPointer child0 = element->child(0);
        ElementPointer child1 = element->child(1);

        switch (mathMode) {
            case HtmlCodeGenerationEngine::MathMode::NO_MATH: {
                result = true;
                break;
            }

            case HtmlCodeGenerationEngine::MathMode::MATHML:
            case HtmlCodeGenerationEngine::MathMode::MATHJAX_MATHML: {
                engine.enterMathMode(engine.useInlineMathMode(element));

                engine.translateChild(child0);

                context.writeStartElement("mo");
                context.writeAttribute("stretchy", "false");
                context.writeCharacters(mathMlOperator());
                context.writeEndElement(); // End mo

                engine.translateChild(child1);

                engine.exitMathMode();

                result = true;
                break;
            }

            case HtmlCodeGenerationEngine::MathMode::MATHJAX_LATEX: {
                engine.enterMathMode(engine.useInlineMathMode(element));

                engine.translateChild(child0);
                context.writeCharacters(tr(" %1 ").arg(latexOperator()));
                engine.translateChild(child1);

                engine.exitMathMode();

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
