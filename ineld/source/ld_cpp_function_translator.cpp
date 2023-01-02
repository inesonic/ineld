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
* This file implements the \ref Ld::CppFunctionTranslator class.
***********************************************************************************************************************/

#include <QString>

#include <ud_usage_data.h>

#include "ld_function_element.h"
#include "ld_element_structures.h"
#include "ld_list_placeholder_element.h"
#include "ld_variable_name.h"
#include "ld_function_data.h"
#include "ld_function_database.h"
#include "ld_operation.h"
#include "ld_cpp_code_generator.h"
#include "ld_cpp_code_generation_engine.h"
#include "ld_cpp_code_generator_diagnostic.h"
#include "ld_cpp_context.h"
#include "ld_cpp_function_translation_engine.h"
#include "ld_cpp_translator.h"
#include "ld_cpp_function_translator.h"

namespace Ld {
    CppFunctionTranslator::~CppFunctionTranslator() {}


    QString CppFunctionTranslator::elementName() const {
        return Ld::FunctionElement::elementName;
    }


    bool CppFunctionTranslator::identifyDependenciesAndExplicitTypes(
            ElementPointer           element,
            CppCodeGenerationEngine& engine
        ) {
        bool success = true;

        QString text1 = element->text(0);
        if (!text1.isEmpty()) {
            QString      text2 = element->text(1);
            VariableName variableName(text1, text2);

            QSharedPointer<FunctionElement> functionElement = element.dynamicCast<FunctionElement>();
            assert(!functionElement.isNull());

            const FunctionData& functionData = FunctionDatabase::function(variableName);
            if (functionData.isValid()) {
                const CppFunctionTranslationEngine*
                    translationEngine = dynamic_cast<const CppFunctionTranslationEngine*>(
                        functionData.translator(CppCodeGenerator::codeGeneratorName)
                    );

                assert(translationEngine != nullptr);

                success = translationEngine->identifyDependenciesAndExplicitTypes(
                    functionData,
                    functionElement,
                    engine
                );
            } else {
                success = translateAllChildren(element, engine);
            }

            Ud::UsageData* usageData = engine.usageData();
            if (usageData != nullptr) {
                usageData->adjustEvent(
                    QString("CppCodeGenerationEngine_FunctionElement_'%1'_'%2'").arg(text1).arg(text2)
                );
            }
        } else {
            CppContext& context = engine.context();
            engine.translationErrorDetected(
                new CppCodeGeneratorDiagnostic(
                    element,
                    CppCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                    dynamic_cast<const CppTranslationPhase&>(engine.translationPhase()),
                    CppCodeGeneratorDiagnostic::Code::INVALID_VARIABLE_OR_FUNCTION_NAME,
                    QString(),
                    engine.contextPointer(),
                    CppCodeGeneratorDiagnostic::CompilerCode::NO_ERROR,
                    context.byteOffset(),
                    context.lineNumber(),
                    context.columnNumber()
                )
            );

            success = false;
        }

        return success;
    }


    bool CppFunctionTranslator::threadImplementation(ElementPointer element, CppCodeGenerationEngine& engine) {
        bool success = true;

        CppContext& context = engine.context();

        QString text1 = element->text(0);
        assert(!text1.isEmpty()); // Checked above.

        QString      text2 = element->text(1);
        VariableName variableName(text1, text2);

        IdentifierContainer identifier = engine.identifier(text1, text2);
        if (identifier.isValid()) {
            QString internalName = identifier.internalName();
            context(element) << QString("%1(pt").arg(internalName);

            unsigned long numberChildren = element->numberChildren();
            for (unsigned i=0 ; i<numberChildren ; ++i) {
                ElementPointer child = element->child(i);
                if (!child.isNull() && !child->isPlaceholder()) {
                    context(element) << ",(";
                    engine.translateChild(child);
                    context(element) << ")";
                }
            }

            context(element) << ")";
        } else {
            const FunctionData& functionData = FunctionDatabase::function(variableName);
            if (functionData.isValid()) {
                const CppFunctionTranslationEngine*
                    translationEngine = dynamic_cast<const CppFunctionTranslationEngine*>(
                        functionData.translator(CppCodeGenerator::codeGeneratorName)
                    );

                assert(translationEngine != nullptr);

                QSharedPointer<FunctionElement> functionElement = element.dynamicCast<FunctionElement>();
                assert(!functionElement.isNull());

                success = translationEngine->implementation(functionData, functionElement, engine);
            } else {
                engine.translationErrorDetected(
                    new CppCodeGeneratorDiagnostic(
                        element,
                        CppCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                        dynamic_cast<const CppTranslationPhase&>(engine.translationPhase()),
                        CppCodeGeneratorDiagnostic::Code::INVALID_VARIABLE_OR_FUNCTION_NAME,
                        QString(),
                        engine.contextPointer(),
                        CppCodeGeneratorDiagnostic::CompilerCode::NO_ERROR,
                        context.byteOffset(),
                        context.lineNumber(),
                        context.columnNumber()
                    )
                );

                success = false;
            }
        }

        return success;
    }


    bool CppFunctionTranslator::methodDefinitions(ElementPointer element, CppCodeGenerationEngine& engine) {
        return threadImplementation(element, engine);
    }
}
