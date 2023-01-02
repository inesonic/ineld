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
* This file implements the \ref Ld::HtmlCompoundStatementOperatorTranslator class.
***********************************************************************************************************************/

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>

#include "ld_format_structures.h"
#include "ld_font_format.h"
#include "ld_compound_statement_operator_element.h"
#include "ld_html_code_generation_engine.h"
#include "ld_xml_export_context.h"
#include "ld_html_translator.h"
#include "ld_html_compound_statement_operator_translator.h"

namespace Ld {
    HtmlCompoundStatementOperatorTranslator::~HtmlCompoundStatementOperatorTranslator() {}


    QString HtmlCompoundStatementOperatorTranslator::elementName() const {
        return Ld::CompoundStatementOperatorElement::elementName;
    }


    bool HtmlCompoundStatementOperatorTranslator::body(ElementPointer element, HtmlCodeGenerationEngine& engine) {
        bool                               result;

        HtmlCodeGenerationEngine::MathMode mathMode       = engine.mathMode();
        XmlExportContext&                  context        = engine.context();
        unsigned long                      numberChildren = element->numberChildren();

        engine.enterMathMode(engine.useInlineMathMode(element));

        switch (mathMode) {
            case HtmlCodeGenerationEngine::MathMode::NO_MATH: {
                result = true;
                break;
            }

            case HtmlCodeGenerationEngine::MathMode::MATHML:
            case HtmlCodeGenerationEngine::MathMode::MATHJAX_MATHML: {
                context.writeStartElement("mrow");
                context.writeStartElement("mo");
                context.writeAttribute("stretchy", "true");
                context.writeAttribute("symmetric", "false");
                context.writeCharacters("|");
                context.writeEndElement(); // mo

                context.writeStartElement("mtable");
                context.writeAttribute("align", "baseline 1");
                context.writeAttribute("columnalign", "left");

                for (unsigned long i=0 ; i<numberChildren ; ++i) {
                    context.writeStartElement("mtr");
                    context.writeStartElement("mtd");

                    ElementPointer child = element->child(i);
                    engine.translateChild(child);

                    context.writeEndElement(); // mtd
                    context.writeEndElement(); // mtr
                }

                context.writeEndElement(); // mtable
                context.writeEndElement(); // mrow

                result = true;
                break;
            }

            case HtmlCodeGenerationEngine::MathMode::MATHJAX_LATEX: {
                context.writeCharacters("\\begin{array}[t]{|l}");
                for (unsigned long i=0 ; i<numberChildren ; ++i) {
                    if (i != 0) {
                        context.writeCharacters(" \\\\\n");
                    }

                    ElementPointer child = element->child(i);
                    engine.translateChild(child);
                }
                context.writeCharacters(" \\end{array}");

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

        engine.exitMathMode();

        return result;
    }
}
