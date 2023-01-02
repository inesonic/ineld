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
* This file implements the \ref Ld::HtmlBraceConditionalOperatorTranslator class.
***********************************************************************************************************************/

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>

#include "ld_format_structures.h"
#include "ld_font_format.h"
#include "ld_brace_conditional_operator_element.h"
#include "ld_brace_conditional_format.h"
#include "ld_html_code_generation_engine.h"
#include "ld_xml_export_context.h"
#include "ld_html_translator.h"
#include "ld_html_brace_conditional_operator_translator.h"

namespace Ld {
    HtmlBraceConditionalOperatorTranslator::~HtmlBraceConditionalOperatorTranslator() {}


    QString HtmlBraceConditionalOperatorTranslator::elementName() const {
        return Ld::BraceConditionalOperatorElement::elementName;
    }


    bool HtmlBraceConditionalOperatorTranslator::body(ElementPointer element, HtmlCodeGenerationEngine& engine) {
        bool                               result;

        HtmlCodeGenerationEngine::MathMode mathMode       = engine.mathMode();
        XmlExportContext&                  context        = engine.context();

        QSharedPointer<BraceConditionalOperatorElement>
            braceElement = element.dynamicCast<BraceConditionalOperatorElement>();
        assert(!braceElement.isNull());

        QSharedPointer<BraceConditionalFormat> format = braceElement->format().dynamicCast<BraceConditionalFormat>();
        QString                                conditionPreambleText;
        QString                                elseConditionText;
        if (!format.isNull()) {
            conditionPreambleText = format->conditionPreambleString();
            elseConditionText     = format->elseConditionString();
        } else {
            conditionPreambleText = tr("if ");
            elseConditionText     = tr("otherwise");
        }

        unsigned long numberExplicitCases = braceElement->numberExplicitConditions();
        bool          explicitElseCase    = braceElement->hasExplicitElseCondition();

        if (explicitElseCase) {
            ++numberExplicitCases;
        }

        switch (mathMode) {
            case HtmlCodeGenerationEngine::MathMode::NO_MATH: {
                result = true;
                break;
            }

            case HtmlCodeGenerationEngine::MathMode::MATHML:
            case HtmlCodeGenerationEngine::MathMode::MATHJAX_MATHML: {
                QString escapedConditionPreambleText = conditionPreambleText
                                                       .toHtmlEscaped()
                                                       .replace(QChar(' '), QString("&nbsp;"));

                QString escapedElseConditionText = elseConditionText
                                                   .toHtmlEscaped()
                                                   .replace(QChar(' '), QString("&nbsp;"));

                context.writeStartElement("mrow");
                context.writeStartElement("mo");
                context.writeAttribute("fence", "true");
                context.writeCharacters("{");
                context.writeEndElement(); // mo

                context.writeStartElement("mtable");
                context.writeAttribute("columnalign", "left");

                for (unsigned long i=0 ; i<numberExplicitCases ; ++i) {
                    ElementPointer valueElement     = braceElement->valueElement(i);
                    ElementPointer conditionElement = braceElement->conditionElement(i);

                    context.writeStartElement("mtr");

                    context.writeStartElement("mtd");
                    engine.translateChild(valueElement);
                    context.writeEndElement(); // mtd

                    context.writeStartElement("mtd");

                    context.writeStartElement("mtext");
                    context.writeCharacters(escapedConditionPreambleText);
                    context.writeEndElement(); // mtext

                    engine.translateChild(conditionElement);

                    context.writeEndElement(); // mtd
                    context.writeEndElement(); // mtr
                }

                if (!explicitElseCase) {
                    ElementPointer valueElement = braceElement->elseValueElement();

                    context.writeStartElement("mtr");

                    context.writeStartElement("mtd");
                    engine.translateChild(valueElement);
                    context.writeEndElement(); // mtd

                    context.writeStartElement("mtd");
                    context.writeStartElement("mtext");
                    context.writeCharacters(escapedElseConditionText);
                    context.writeEndElement(); // mtext
                    context.writeEndElement(); // mtd

                    context.writeEndElement(); // mtr
                }

                context.writeEndElement(); // mtable
                context.writeEndElement(); // mrow

                result = true;
                break;
            }

            case HtmlCodeGenerationEngine::MathMode::MATHJAX_LATEX: {
                context.writeCharacters("\\begin{cases} ");

                for (unsigned long i=0 ; i<numberExplicitCases ; ++i) {
                    if (i != 0) {
                        context.writeCharacters(" \\\\\n");
                    }

                    ElementPointer valueElement     = braceElement->valueElement(i);
                    ElementPointer conditionElement = braceElement->conditionElement(i);

                    engine.translateChild(valueElement);

                    // TODO: We should probably escape the text below to remove LaTeX symbols.
                    context.writeCharacters(QString(" & \\text{%1}").arg(conditionPreambleText));

                    engine.translateChild(conditionElement);
                }

                if (!explicitElseCase) {
                    context.writeCharacters(" \\\\\n");

                    ElementPointer valueElement = braceElement->elseValueElement();
                    engine.translateChild(valueElement);

                    // TODO: We should probably escape the text below to remove LaTeX symbols.
                    context.writeCharacters(QString(" & \\text{%1}").arg(elseConditionText));
                }

                context.writeCharacters(" \\end{cases}");

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
