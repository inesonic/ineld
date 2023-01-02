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
* This file implements the \ref Ld::HtmlFunctionTranslator class.
***********************************************************************************************************************/

#include <QString>

#include "ld_element_structures.h"
#include "ld_function_element.h"
#include "ld_function_data.h"
#include "ld_function_database.h"
#include "ld_variable_name.h"
#include "ld_list_placeholder_element.h"
#include "ld_xml_export_context.h"
#include "ld_code_generation_engine.h"
#include "ld_html_function_translation_engine.h"
#include "ld_html_code_generator_diagnostic.h"
#include "ld_html_code_generation_engine.h"
#include "ld_html_translator.h"
#include "ld_html_function_translator.h"

namespace Ld {
    HtmlFunctionTranslator::~HtmlFunctionTranslator() {}


    QString HtmlFunctionTranslator::elementName() const {
        return Ld::FunctionElement::elementName;
    }


    bool HtmlFunctionTranslator::header(ElementPointer element, HtmlCodeGenerationEngine& engine) {
        bool success = true;

        QString text1 = element->text(0);
        if (!text1.isEmpty()) {
            QString      text2 = element->text(1);
            VariableName variableName(text1, text2);

            const FunctionData& functionData = FunctionDatabase::function(variableName);
            if (functionData.isValid()) {
                const HtmlFunctionTranslationEngine*
                    translationEngine = dynamic_cast<const HtmlFunctionTranslationEngine*>(
                        functionData.translator(HtmlCodeGenerator::codeGeneratorName)
                    );

                assert(translationEngine != nullptr);

                QSharedPointer<FunctionElement> functionElement = element.dynamicCast<FunctionElement>();
                assert(!functionElement.isNull());

                success = translationEngine->header(
                    functionData,
                    functionElement,
                    engine
                );
            }
        }

        return success;
    }


    bool HtmlFunctionTranslator::body(ElementPointer element, HtmlCodeGenerationEngine& engine) {
        bool                            result;

        QSharedPointer<FunctionElement> functionElement = element.dynamicCast<FunctionElement>();
        QString                         functionName0   = functionElement->text(0);
        QString                         functionName1   = functionElement->text(1);
        VariableName                    variableName(functionName0, functionName1);

        const FunctionData& functionData = FunctionDatabase::function(variableName);
        if (functionData.isValid()) {
            const HtmlFunctionTranslationEngine*
                translationEngine = dynamic_cast<const HtmlFunctionTranslationEngine*>(
                    functionData.translator(HtmlCodeGenerator::codeGeneratorName)
                );

            assert(translationEngine != nullptr);

            QSharedPointer<FunctionElement> functionElement = element.dynamicCast<FunctionElement>();
            assert(!functionElement.isNull());

            result = translationEngine->body(
                functionData,
                functionElement,
                engine
            );
        } else {
            QSharedPointer<Ld::FunctionElement> functionElement = element.dynamicCast<Ld::FunctionElement>();
            result = HtmlFunctionTranslationEngine::instance.body(FunctionData(), functionElement, engine);
        }

        return result;
    }
}
