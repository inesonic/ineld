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
* This file implements the \ref Ld::HtmlDivisionOperatorTranslatorBase class.
***********************************************************************************************************************/

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>

#include "ld_format_structures.h"
#include "ld_xml_export_context.h"
#include "ld_html_code_generation_engine.h"
#include "ld_division_operator_format.h"
#include "ld_html_division_operator_translator_base.h"

namespace Ld {
    HtmlDivisionOperatorTranslatorBase::~HtmlDivisionOperatorTranslatorBase() {}


    bool HtmlDivisionOperatorTranslatorBase::body(ElementPointer element, HtmlCodeGenerationEngine& engine) {
        bool          result = false;
        FormatPointer format = element->format();

        if (format->capabilities().contains(DivisionOperatorFormat::formatName)) {
            QSharedPointer<DivisionOperatorFormat>
                divisionOperatorFormat = format.dynamicCast<DivisionOperatorFormat>();

            DivisionOperatorFormat::DivisionSymbol symbol = divisionOperatorFormat->divisionSymbol();
            if (symbol == DivisionOperatorFormat::DivisionSymbol::DEFAULT) {
                symbol = defaultDivisionSymbol();
            }

            switch (symbol) {
                case DivisionOperatorFormat::DivisionSymbol::INVALID:
                case DivisionOperatorFormat::DivisionSymbol::OBELUS: {
                    result = HtmlBinaryOperatorTranslatorBase::body(element, engine);
                    break;
                }

                case DivisionOperatorFormat::DivisionSymbol::FRACTION: {
                    switch (engine.mathMode()) {
                        case HtmlCodeGenerationEngine::MathMode::NO_MATH: {
                            result = true;
                            break;
                        }

                        case HtmlCodeGenerationEngine::MathMode::MATHML:
                        case HtmlCodeGenerationEngine::MathMode::MATHJAX_MATHML: {
                            result = mathMlFraction(element, engine);
                            break;
                        }

                        case HtmlCodeGenerationEngine::MathMode::MATHJAX_LATEX: {
                            result = latexFraction(element, engine);
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

                    break;
                }

                case DivisionOperatorFormat::DivisionSymbol::DEFAULT: {
                    assert(false);
                    break;
                }

                default: {
                    assert(false);
                    break;
                }
        }

        } else {
            result = HtmlBinaryOperatorTranslatorBase::body(element, engine);
        }

        return result;
    }


    QString HtmlDivisionOperatorTranslatorBase::mathMlOperator(FormatPointer /* operatorFormat */) const {
        return tr("%1").arg(QChar(0x00F7));
    }


    QString HtmlDivisionOperatorTranslatorBase::latexOperator(FormatPointer /* operatorFormat */) const {
        return tr(" \\div ");
    }


    bool HtmlDivisionOperatorTranslatorBase::mathMlFraction(
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
        ) {
        XmlExportContext& context  = engine.context();
        ElementPointer    child0   = element->child(0);
        ElementPointer    child1   = element->child(1);

        QString leftParenthesis;
        QString rightParenthesis;
        bool    requiresParenthesis = includeParenthesis(element, false, leftParenthesis, rightParenthesis);

        if (requiresParenthesis) {
            context.writeStartElement("mo");
            context.writeAttribute("fence", "true");
            context.writeAttribute("stretchy", "true");
            context.writeCharacters(leftParenthesis);
            context.writeEndElement(); // mo
            context.writeStartElement("mrow");
        }

        context.writeStartElement("mfrac");
        engine.translateChild(child0);
        engine.translateChild(child1);
        context.writeEndElement(); // mfrac

        if (requiresParenthesis) {
            context.writeEndElement(); // mrow
            context.writeStartElement("mo");
            context.writeAttribute("fence", "true");
            context.writeAttribute("stretchy", "true");
            context.writeCharacters(rightParenthesis);
            context.writeEndElement(); // mo
        }

        return true;
    }


    bool HtmlDivisionOperatorTranslatorBase::latexFraction(
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
        ) {
        XmlExportContext& context  = engine.context();
        ElementPointer    child0   = element->child(0);
        ElementPointer    child1   = element->child(1);

        QString leftParenthesis;
        QString rightParenthesis;
        bool    requiresParenthesis = includeParenthesis(element, true, leftParenthesis, rightParenthesis);

        if (requiresParenthesis) {
            context.writeCharacters(leftParenthesis);
        }

        context.writeCharacters("\\frac{ ");
        engine.translateChild(child0);
        context.writeCharacters(" }{ ");
        engine.translateChild(child1);
        context.writeCharacters(" }");

        if (requiresParenthesis) {
            context.writeCharacters(rightParenthesis);
        }

        return true;
    }
}
