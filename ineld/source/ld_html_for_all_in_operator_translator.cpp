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
* This file implements the \ref Ld::HtmlForAllInOperatorTranslator class.
***********************************************************************************************************************/

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>

#include "ld_format_structures.h"
#include "ld_font_format.h"
#include "ld_for_all_in_operator_element.h"
#include "ld_html_code_generation_engine.h"
#include "ld_xml_export_context.h"
#include "ld_html_translator.h"
#include "ld_html_for_all_in_operator_translator.h"

namespace Ld {
    HtmlForAllInOperatorTranslator::~HtmlForAllInOperatorTranslator() {}


    QString HtmlForAllInOperatorTranslator::elementName() const {
        return Ld::ForAllInOperatorElement::elementName;
    }


    bool HtmlForAllInOperatorTranslator::body(ElementPointer element, HtmlCodeGenerationEngine& engine) {
        bool                               result;

        QSharedPointer<Ld::ForAllInOperatorElement> forAllElement = element.dynamicCast<Ld::ForAllInOperatorElement>();

        HtmlCodeGenerationEngine::MathMode mathMode = engine.mathMode();
        XmlExportContext&                  context  = engine.context();
        ElementPointer                     child0   = element->child(0);
        ElementPointer                     child1   = element->child(1);
        ElementPointer                     child2   = element->child(2);

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
                context.writeCharacters(tr("%1").arg(QChar(0x2200)));
                context.writeEndElement(); // mo

                engine.translateChild(child0);

                context.writeStartElement("mo");
                context.writeCharacters(forAllElement->isSetIterable() ? tr("%1").arg(QChar(0x2208)) : tr(" over "));
                context.writeEndElement();

                engine.translateChild(child1);

                context.writeStartElement("mo");
                context.writeCharacters(" : ");
                context.writeEndElement(); // mo

                engine.translateChild(child2);

                context.writeEndElement(); // mrow

                result = true;
                break;
            }

            case HtmlCodeGenerationEngine::MathMode::MATHJAX_LATEX: {
                context.writeCharacters("{\\forall}");

                engine.translateChild(child0);
                context.writeCharacters(forAllElement->isSetIterable() ? "{\\in}" : "{\\text{ over }}");
                engine.translateChild(child1);
                context.writeCharacters("{\\: : \\:}");
                engine.translateChild(child2);

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
