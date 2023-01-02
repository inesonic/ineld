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
* This file implements the \ref Ld::LaTeXFunctionTranslator class.
***********************************************************************************************************************/

#include <QString>

#include "ld_element_structures.h"
#include "ld_function_element.h"
#include "ld_list_placeholder_element.h"
#include "ld_operator_format.h"
#include "ld_variable_name.h"
#include "ld_function_data.h"
#include "ld_function_database.h"
#include "ld_text_export_context.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_latex_function_translation_engine.h"
#include "ld_latex_translator.h"
#include "ld_latex_function_translator.h"

namespace Ld {
    LaTeXFunctionTranslator::~LaTeXFunctionTranslator() {}


    QString LaTeXFunctionTranslator::elementName() const {
        return FunctionElement::elementName;
    }


    bool LaTeXFunctionTranslator::identifyDependencies(ElementPointer element, LaTeXCodeGenerationEngine& engine) {
        bool result;

        QSharedPointer<FunctionElement> functionElement = element.dynamicCast<FunctionElement>();
        QString                         functionName0   = functionElement->text(0);
        QString                         functionName1   = functionElement->text(1);

        const FunctionData& functionData = FunctionDatabase::function(VariableName(functionName0, functionName1));
        if (functionData.isValid()) {
            const LaTeXFunctionTranslationEngine*
                translationEngine = dynamic_cast<const LaTeXFunctionTranslationEngine*>(
                    functionData.translator(LaTeXCodeGenerator::codeGeneratorName)
                );

            assert(translationEngine != nullptr);

            result = translationEngine->identifyDependencies(
                functionData,
                functionElement,
                engine
            );
        } else {
            result = true;
        }

        return result;
    }


    bool LaTeXFunctionTranslator::body(ElementPointer element, LaTeXCodeGenerationEngine& engine) {
        bool result;

        QSharedPointer<FunctionElement> functionElement = element.dynamicCast<FunctionElement>();
        QString                         functionName0   = functionElement->text(0);
        QString                         functionName1   = functionElement->text(1);

        const FunctionData& functionData = FunctionDatabase::function(VariableName(functionName0, functionName1));
        if (functionData.isValid()) {
            const LaTeXFunctionTranslationEngine*
                translationEngine = dynamic_cast<const LaTeXFunctionTranslationEngine*>(
                    functionData.translator(LaTeXCodeGenerator::codeGeneratorName)
                );

            assert(translationEngine != nullptr);

            result = translationEngine->body(
                functionData,
                functionElement,
                engine
            );
        } else {
            result = LaTeXFunctionTranslationEngine::instance.body(FunctionData(), functionElement, engine);
        }

        return result;
    }
}
