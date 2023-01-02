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
* This file implements the \ref Ld::LaTeXFunctionTranslationEngine class.
***********************************************************************************************************************/

#include <QString>

#include "ld_element_structures.h"
#include "ld_function_element.h"
#include "ld_function_format.h"
#include "ld_function_data.h"
#include "ld_list_placeholder_element.h"
#include "ld_latex_translator.h"
#include "ld_text_export_context.h"
#include "ld_latex_code_generator.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_latex_operator_translator_helper.h"
#include "ld_function_translation_engine.h"
#include "ld_latex_function_translation_engine.h"

namespace Ld {
    const LaTeXFunctionTranslationEngine LaTeXFunctionTranslationEngine::instance;

    LaTeXFunctionTranslationEngine::LaTeXFunctionTranslationEngine() {}


    LaTeXFunctionTranslationEngine::~LaTeXFunctionTranslationEngine() {}


    bool LaTeXFunctionTranslationEngine::identifyDependencies(
            const FunctionData&             /* functionData */,
            QSharedPointer<FunctionElement> /* element */,
            LaTeXCodeGenerationEngine&      /* engine */
        ) const {
        return true;
    }


    bool LaTeXFunctionTranslationEngine::body(
            const FunctionData&             /* functionData */,
            QSharedPointer<FunctionElement> element,
            LaTeXCodeGenerationEngine&      engine
        ) const {
        TextExportContext& context             = engine.context();

        QString            functionName0       = element->text(0);
        QString            functionName1       = element->text(1);
        unsigned long      numberChildren      = element->numberChildren();
        unsigned long      numberValidChildren = numberChildren;

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

        QString leftParenthesis;
        QString rightParenthesis;
        bool    requiresParenthesis = includeParenthesis(element, leftParenthesis, rightParenthesis);


        QString leftParameterParenthesis;
        QString rightParameterParenthesis;
        bool    requiresParameterParenthesis = includeParameterParenthesis(
            element,
            leftParameterParenthesis,
            rightParameterParenthesis
        );

        if (requiresParenthesis) {
            context << QString(" %1 ").arg(leftParenthesis);
        } else {
            context << "\\thinspace ";
        }

        context << QString("\\mathtt{%1}").arg(functionName0);

        if (useSubscriptParameter && numberValidChildren > 0) {
            if (!functionName1.isEmpty()) {
                context << QString("_{ \\mathtt{%1},").arg(functionName1);
                engine.translateChild(element->child(0));
                context << QString(" }");
            } else {
                context << QString("_{ ").arg(functionName1);
                engine.translateChild(element->child(0));
                context << QString(" }");
            }
        } else {
            if (!functionName1.isEmpty()) {
                context << QString("_\\mathtt{%1}").arg(functionName1);
            }
        }

        if (requiresParameterParenthesis) {
            context << tr(" %1 ").arg(leftParameterParenthesis);
        } else {
            context << "\\thinspace ";
        }

        for (unsigned long i=firstParameter ; i<numberValidChildren ; ++i) {
            ElementPointer child = element->child(i);
            if (!child.isNull()) {
                if (child->typeName() != ListPlaceholderElement::elementName) {
                    if (i != firstParameter) {
                        context << ", ";
                    }
                }

                engine.translateChild(child);
            }
        }

        if (requiresParameterParenthesis) {
            context << QString(" %1 ").arg(rightParameterParenthesis);
        }

        if (requiresParenthesis) {
            context << QString(" %1 ").arg(rightParenthesis);
        }

        return true;
    }
}
