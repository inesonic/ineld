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
* This file implements the \ref Ld::HtmlLogicalUnaryNotOperatorTranslator class.
***********************************************************************************************************************/

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS
#include <QString>

#include "ld_format_structures.h"
#include "ld_logical_unary_not_operator_format.h"
#include "ld_xml_export_context.h"
#include "ld_code_generation_engine.h"
#include "ld_html_code_generation_engine.h"
#include "ld_html_operator_translator_base.h"
#include "ld_html_logical_not_operator_translator_base.h"

namespace Ld {
    HtmlLogicalNotOperatorTranslatorBase::~HtmlLogicalNotOperatorTranslatorBase() {}


    bool HtmlLogicalNotOperatorTranslatorBase::body(ElementPointer element, HtmlCodeGenerationEngine& engine) {
        bool                                            result;
        LogicalUnaryNotOperatorFormat::LogicalNotSymbol symbol   = logicalNotStyle(element->format());
        HtmlCodeGenerationEngine::MathMode              mathMode = engine.mathMode();
        XmlExportContext&                               context  = engine.context();
        ElementPointer                                  child0   = element->child(0);

        switch (mathMode) {
            case HtmlCodeGenerationEngine::MathMode::NO_MATH: {
                result = true;
                break;
            }

            case HtmlCodeGenerationEngine::MathMode::MATHML:
            case HtmlCodeGenerationEngine::MathMode::MATHJAX_MATHML: {
                QString leftParenthesis;
                QString rightParenthesis;
                bool    requiresParenthesis = includeParenthesis(element, false, leftParenthesis, rightParenthesis);

                QString operatorString;
                if (symbol == LogicalUnaryNotOperatorFormat::LogicalNotSymbol::OVERBAR) {
                    context.writeStartElement("mover");
                    context.writeAttribute("accent", "true");
                    context.writeStartElement("mrow");
                } else {
                    if (symbol == LogicalUnaryNotOperatorFormat::LogicalNotSymbol::TILDE) {
                        operatorString = tr("~");
                    } else {
                        operatorString = QChar(0x00AC);
                    }
                }

                if (requiresParenthesis) {
                    context.writeStartElement("mo");
                    context.writeAttribute("fence", "true");
                    context.writeAttribute("stretchy", "true");
                    context.writeCharacters(leftParenthesis);
                    context.writeEndElement(); // mo
                    context.writeStartElement("mrow");
                }

                if (!operatorString.isEmpty()) {
                    context.writeStartElement("mo");
                    context.writeAttribute("stretchy", "false");
                    context.writeCharacters(operatorString);
                    context.writeEndElement(); // mo
                }

                engine.translateChild(child0);

                if (requiresParenthesis) {
                    context.writeEndElement(); // mrow
                    context.writeStartElement("mo");
                    context.writeAttribute("fence", "true");
                    context.writeAttribute("stretchy", "true");
                    context.writeCharacters(rightParenthesis);
                    context.writeEndElement(); // mo
                }

                if (symbol == LogicalUnaryNotOperatorFormat::LogicalNotSymbol::OVERBAR) {
                    context.writeEndElement(); // mrow
                    context.writeStartElement("mo");
                    context.writeCharacters(tr("%1").arg(QChar(0x00AF))); // macron
                    context.writeEndElement(); // mo
                    context.writeEndElement(); // mover
                }

                result = true;
                break;
            }

            case HtmlCodeGenerationEngine::MathMode::MATHJAX_LATEX: {
                QString leftParenthesis;
                QString rightParenthesis;
                bool    requiresParenthesis = includeParenthesis(element, true, leftParenthesis, rightParenthesis);

                QString operatorString;
                if (symbol == LogicalUnaryNotOperatorFormat::LogicalNotSymbol::OVERBAR) {
                    context.writeCharacters(tr("\\overline{ "));
                } else {
                    if (symbol == LogicalUnaryNotOperatorFormat::LogicalNotSymbol::TILDE) {
                        operatorString = tr("{\\raise.18ex\\hbox{$\\scriptstyle\\sim$}}");
                    } else {
                        operatorString = tr("{\\neg}");
                    }
                }

                if (requiresParenthesis) {
                    context.writeCharacters(leftParenthesis);
                }

                if (!operatorString.isEmpty()) {
                    context.writeCharacters(operatorString);
                }

                engine.translateChild(child0);

                if (requiresParenthesis) {
                    context.writeCharacters(rightParenthesis);
                }

                if (symbol == LogicalUnaryNotOperatorFormat::LogicalNotSymbol::OVERBAR) {
                    context.writeCharacters(tr(" }"));
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


    LogicalUnaryNotOperatorFormat::LogicalNotSymbol HtmlLogicalNotOperatorTranslatorBase::logicalNotStyle(
            Ld::FormatPointer format
        ) const {
        Ld::LogicalUnaryNotOperatorFormat::LogicalNotSymbol result;

        if (format->capabilities().contains(Ld::LogicalUnaryNotOperatorFormat::formatName)) {
            QSharedPointer<Ld::LogicalUnaryNotOperatorFormat>
                logicalNotFormat = format.dynamicCast<Ld::LogicalUnaryNotOperatorFormat>();

            result = logicalNotFormat->logicalNotSymbol();

            if (result == Ld::LogicalUnaryNotOperatorFormat::LogicalNotSymbol::DEFAULT) {
                result = defaultLogicalNotSymbol();
            }
        } else {
            result = defaultLogicalNotSymbol();
        }

        return result;
    }
}
