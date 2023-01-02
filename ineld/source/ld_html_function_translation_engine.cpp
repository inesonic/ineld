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
* This file implements the \ref Ld::HtmlFunctionTranslationEngine class.
***********************************************************************************************************************/

#include "ld_element_structures.h"
#include "ld_function_element.h"
#include "ld_function_format.h"
#include "ld_list_placeholder_element.h"
#include "ld_function_data.h"
#include "ld_html_code_generation_engine.h"
#include "ld_html_code_generator_diagnostic.h"
#include "ld_xml_export_context.h"
#include "ld_function_translation_engine.h"
#include "ld_html_function_translation_engine.h"

namespace Ld {
    const HtmlFunctionTranslationEngine HtmlFunctionTranslationEngine::instance;

    HtmlFunctionTranslationEngine::HtmlFunctionTranslationEngine() {}


    HtmlFunctionTranslationEngine::~HtmlFunctionTranslationEngine() {}


    bool HtmlFunctionTranslationEngine::header(
            const FunctionData&             /* functionData */,
            QSharedPointer<FunctionElement> /* element */,
            HtmlCodeGenerationEngine&       /* engine */
        ) const {
        return true;
    }


    bool HtmlFunctionTranslationEngine::body(
            const FunctionData&             /* functionData */,
            QSharedPointer<FunctionElement> element,
            HtmlCodeGenerationEngine&       engine
        ) const {
        bool result = true;

        QString                            functionName0       = element->text(0);
        QString                            functionName1       = element->text(1);
        HtmlCodeGenerationEngine::MathMode mathMode            = engine.mathMode();
        XmlExportContext&                  context             = engine.context();
        unsigned long                      numberChildren      = element->numberChildren();
        unsigned long                      numberValidChildren = numberChildren;

        while (numberValidChildren > 0 && element->child(numberValidChildren - 1)->isPlaceholder()) {
            --numberValidChildren;
        }

        bool              useSubscriptParameter = false;
        Ld::FormatPointer format                = element->format();
        if (!format.isNull() && format->capabilities().contains(Ld::FunctionFormat::formatName)) {
            QSharedPointer<Ld::FunctionFormat> functionFormat = format.dynamicCast<Ld::FunctionFormat>();
            useSubscriptParameter = functionFormat->subscriptedParameter();
        }

        unsigned long firstParameter = useSubscriptParameter ? 1 : 0;

        switch (mathMode) {
            case HtmlCodeGenerationEngine::MathMode::NO_MATH: {
                break;
            }

            case HtmlCodeGenerationEngine::MathMode::MATHML:
            case HtmlCodeGenerationEngine::MathMode::MATHJAX_MATHML: {
                QString leftParenthesis;
                QString rightParenthesis;
                bool    requiresParenthesis = includeParenthesis(element, false, leftParenthesis, rightParenthesis);

                QString leftParameterParenthesis;
                QString rightParameterParenthesis;
                bool    requiresParameterParenthesis = includeParameterParenthesis(
                    element,
                    false,
                    leftParameterParenthesis,
                    rightParameterParenthesis
                );

                if (requiresParenthesis) {
                    context.writeStartElement("mrow");
                    context.writeStartElement("mo");
                    context.writeAttribute("fence", "true");
                    context.writeAttribute("stretchy", "true");
                    context.writeCharacters(leftParenthesis);
                    context.writeEndElement(); // mo
                    context.writeEndElement(); // mrow
                }

                context.writeStartElement("mrow");

                if (!functionName1.isEmpty()) {
                    context.writeStartElement("msub");

                    context.writeStartElement("mi");
                    context.writeCharacters(functionName0);
                    context.writeEndElement(); // mi

                    if(useSubscriptParameter) {
                        context.writeStartElement("mrow");

                        context.writeStartElement("mi");
                        context.writeCharacters(functionName1);
                        context.writeCharacters(QString(","));
                        context.writeEndElement(); //mi
                        engine.translateChild(element->child(0));
                        context.writeEndElement(); // mrow
                    } else {
                        context.writeStartElement("mi");
                        context.writeCharacters(functionName1);
                        context.writeEndElement(); //mi
                    }

                    context.writeEndElement(); // msub
                } else if (useSubscriptParameter) {
                    context.writeStartElement("msub");

                    context.writeStartElement("mi");
                    context.writeCharacters(functionName0);
                    context.writeEndElement(); // mi
                    context.writeStartElement("mrow");
                    engine.translateChild(element->child(0));
                    context.writeEndElement(); // mrow

                    context.writeEndElement(); // msub
                } else {
                    context.writeStartElement("mi");
                    context.writeCharacters(functionName0);
                    context.writeEndElement(); // mi
                }

                if (requiresParameterParenthesis) {
                    context.writeStartElement("mrow");
                    context.writeStartElement("mo");
                    context.writeAttribute("fence", "true");
                    context.writeAttribute("stretchy", "true");
                    context.writeCharacters(leftParameterParenthesis);
                    context.writeEndElement(); // mo
                }

                for (unsigned long i=firstParameter ; i<numberValidChildren ; ++i) {
                    ElementPointer child = element->child(i);
                    if (!child.isNull()) {
                        if (child->typeName() != ListPlaceholderElement::elementName) {
                            if (i != firstParameter) {
                                context.writeStartElement("mo");
                                context.writeCharacters(", ");
                                context.writeEndElement();
                            }
                        }

                        engine.translateChild(child);
                    }
                }

                if (requiresParameterParenthesis) {
                    context.writeStartElement("mo");
                    context.writeAttribute("fence", "true");
                    context.writeAttribute("stretchy", "true");
                    context.writeCharacters(rightParameterParenthesis);
                    context.writeEndElement(); // mo
                    context.writeEndElement(); // mrow
                }

                if (requiresParenthesis) {
                    context.writeStartElement("mo");
                    context.writeAttribute("fence", "true");
                    context.writeAttribute("stretchy", "true");
                    context.writeCharacters(rightParenthesis);
                    context.writeEndElement(); // mo
                    context.writeEndElement(); // mrow
                }

                break;
            }

            case HtmlCodeGenerationEngine::MathMode::MATHJAX_LATEX: {
                QString leftParenthesis;
                QString rightParenthesis;
                bool    requiresParenthesis = includeParenthesis(element, true, leftParenthesis, rightParenthesis);

                QString leftParameterParenthesis;
                QString rightParameterParenthesis;
                bool    requiresParameterParenthesis = includeParameterParenthesis(
                    element,
                    true,
                    leftParameterParenthesis,
                    rightParameterParenthesis
                );

                if (requiresParenthesis) {
                    context.writeCharacters(tr(" %1 ").arg(leftParenthesis));
                } else {
                    context.writeCharacters("\\thinspace ");
                }

                context.writeCharacters(QString("\\mathtt{%1}").arg(functionName0));

                if (useSubscriptParameter && numberValidChildren > 0) {
                    if (!functionName1.isEmpty()) {
                        context.writeCharacters(QString("_{ \\mathtt{%1},").arg(functionName1));
                        engine.translateChild(element->child(0));
                        context.writeCharacters(QString(" }"));
                    } else {
                        context.writeCharacters(QString("_{").arg(functionName1));
                        engine.translateChild(element->child(0));
                        context.writeCharacters(QString("}"));
                    }
                } else {
                    if (!functionName1.isEmpty()) {
                        context.writeCharacters(QString("_\\mathtt{%1}").arg(functionName1));
                    }
                }

                if (requiresParameterParenthesis) {
                    context.writeCharacters(tr(" %1 ").arg(leftParameterParenthesis));
                } else {
                    context.writeCharacters("\\thinspace ");
                }

                for (unsigned long i=firstParameter ; i<numberValidChildren ; ++i) {
                    ElementPointer child = element->child(i);
                    if (!child.isNull()) {
                        if (child->typeName() != ListPlaceholderElement::elementName) {
                            if (i != firstParameter) {
                                context.writeCharacters(", ");
                            }
                        }

                        engine.translateChild(child);
                    }
                }

                if (requiresParameterParenthesis) {
                    context.writeCharacters(tr(" %1 ").arg(rightParameterParenthesis));
                }

                if (requiresParenthesis) {
                    context.writeCharacters(tr(" %1 ").arg(rightParenthesis));
                }

                break;
            }

            case HtmlCodeGenerationEngine::MathMode::IMAGES: {
                result = writeAsImage(element, engine);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        return result;
    }
}
