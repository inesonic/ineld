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
* This file implements the \ref Ld::HtmlVariableTranslator class.
***********************************************************************************************************************/

#include <QString>

#include "ld_element_structures.h"
#include "ld_variable_element.h"
#include "ld_font_format.h"
#include "ld_xml_export_context.h"
#include "ld_code_generation_engine.h"
#include "ld_html_code_generator_diagnostic.h"
#include "ld_html_code_generation_engine.h"
#include "ld_html_translator.h"
#include "ld_html_variable_translator.h"

namespace Ld {
    HtmlVariableTranslator::~HtmlVariableTranslator() {}


    QString HtmlVariableTranslator::elementName() const {
        return Ld::VariableElement::elementName;
    }


    bool HtmlVariableTranslator::body(ElementPointer element, HtmlCodeGenerationEngine& engine) {
        bool                               result;
        HtmlCodeGenerationEngine::MathMode mathMode = engine.mathMode();
        XmlExportContext&                  context  = engine.context();
        QString                            text0    = element->text(0);
        QString                            text1    = element->text(1);

        switch (mathMode) {
            case HtmlCodeGenerationEngine::MathMode::NO_MATH: {
                result = true;
                break;
            }

            case HtmlCodeGenerationEngine::MathMode::MATHML:
            case HtmlCodeGenerationEngine::MathMode::MATHJAX_MATHML: {
                if (!text1.isEmpty()) {
                    context.writeStartElement("msub");
                    context.writeStartElement("mi");
                    context.writeCharacters(text0);
                    context.writeEndElement(); // mi
                    context.writeStartElement("mi");
                    context.writeCharacters(text1);
                    context.writeEndElement(); // mi
                    context.writeEndElement(); // msub
                } else {
                    context.writeStartElement("mi");
                    context.writeCharacters(text0);
                    context.writeEndElement(); // mi
                }

                result = true;
                break;
            }

            case HtmlCodeGenerationEngine::MathMode::MATHJAX_LATEX: {
                context.writeCharacters(text0);
                if (text1.length() == 1) {
                    context.writeCharacters(tr(" _ %1").arg(text1));
                } else if (text1.length() > 1) {
                    context.writeCharacters(tr(" _ { %1 }").arg(text1));
                }

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
