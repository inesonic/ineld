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
* This file implements the \ref Ld::CppTranslator class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>

#include <ud_usage_data.h>

#include "ld_element_structures.h"
#include "ld_element.h"
#include "ld_cpp_translation_phase.h"
#include "ld_cpp_code_generator_diagnostic.h"
#include "ld_cpp_code_generation_engine.h"
#include "ld_cpp_translator.h"

namespace Ld {
    CppTranslator::~CppTranslator() {}

    bool CppTranslator::translate(ElementPointer element, CodeGenerationEngine& codeGenerationEngine) {
        bool  success;
        const TranslationPhase&    translationPhase     = codeGenerationEngine.translationPhase();
        const CppTranslationPhase& cppTranslationPhase  = dynamic_cast<const CppTranslationPhase&>(translationPhase);
        CppCodeGenerationEngine&   engine               = dynamic_cast<CppCodeGenerationEngine&>(codeGenerationEngine);
        switch (cppTranslationPhase.phase()) {
            case CppTranslationPhase::Phase::IDENTIFY_DEPENDENCIES_AND_EXPLICIT_TYPES: {
                Ud::UsageData* usageData = engine.usageData();
                if (usageData != nullptr) {
                    usageData->adjustEvent(QString("CppCodeGenerationEngine_%2").arg(element->typeName()));
                }

                success = identifyDependenciesAndExplicitTypes(element, engine);
                break;
            }

            case CppTranslationPhase::Phase::IDENTIFY_INFERRED_TYPES: {
                success = identifyInferredTypes(element, engine);
                break;
            }

            case CppTranslationPhase::Phase::DECLARATIONS: {
                success = declarations(element, engine);
                break;
            }

            case CppTranslationPhase::Phase::METHOD_DECLARATIONS: {
                success = methodDeclarations(element, engine);
                break;
            }

            case CppTranslationPhase::Phase::THREAD_IMPLEMENTATION: {
                success = threadImplementation(element, engine);
                break;
            }

            case CppTranslationPhase::Phase::THREAD_POST_PROCESSING: {
                success = threadPostProcessing(element, engine);
                break;
            }

            case CppTranslationPhase::Phase::THREAD_CLEANUP: {
                success = threadCleanup(element, engine);
                break;
            }

            case CppTranslationPhase::Phase::METHOD_DEFINITIONS: {
                success = methodDefinitions(element, engine);
                break;
            }

            default: {
                CppCodeGeneratorDiagnostic* diagnostic = new CppCodeGeneratorDiagnostic(
                    nullptr,
                    CppCodeGeneratorDiagnostic::Type::INTERNAL_ERROR,
                    cppTranslationPhase,
                    CppCodeGeneratorDiagnostic::Code::UNEXPECTED_TRANSLATION_PHASE
                );

                engine.translationErrorDetected(diagnostic);

                success = false;
                break;
            }
        }

        return success;
    }


    bool CppTranslator::identifyDependenciesAndExplicitTypes(ElementPointer element, CppCodeGenerationEngine& engine) {
        return translateAllChildren(element, engine);
    }


    bool CppTranslator::identifyInferredTypes(ElementPointer element, CppCodeGenerationEngine& engine) {
        return translateAllChildren(element, engine);
    }


    bool CppTranslator::declarations(ElementPointer, CppCodeGenerationEngine&) {
        return true;
    }


    bool CppTranslator::methodDeclarations(ElementPointer element, CppCodeGenerationEngine& engine) {
        return translateAllChildren(element, engine);
    }


    bool CppTranslator::threadImplementation(ElementPointer element, CppCodeGenerationEngine& engine) {
        return translateAllChildren(element, engine);
    }


    bool CppTranslator::threadPostProcessing(ElementPointer, CppCodeGenerationEngine&) {
        return true;
    }


    bool CppTranslator::threadCleanup(ElementPointer, CppCodeGenerationEngine&) {
        return true;
    }


    bool CppTranslator::methodDefinitions(ElementPointer element, CppCodeGenerationEngine& engine) {
        return translateAllChildren(element, engine);
    }
}
