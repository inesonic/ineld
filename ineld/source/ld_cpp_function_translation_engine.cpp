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
* This file implements the \ref Ld::CppFunctionTranslationEngine class.
***********************************************************************************************************************/

#include "ld_element_structures.h"
#include "ld_function_element.h"
#include "ld_function_data.h"
#include "ld_cpp_code_generation_engine.h"
#include "ld_cpp_code_generator_diagnostic.h"
#include "ld_cpp_context.h"
#include "ld_function_translation_engine.h"
#include "ld_cpp_function_translation_engine.h"

namespace Ld {
    const CppFunctionTranslationEngine CppFunctionTranslationEngine::instance;

    CppFunctionTranslationEngine::CppFunctionTranslationEngine() {}


    CppFunctionTranslationEngine::~CppFunctionTranslationEngine() {}


    bool CppFunctionTranslationEngine::identifyDependenciesAndExplicitTypes(
            const FunctionData&             functionData,
            QSharedPointer<FunctionElement> element,
            CppCodeGenerationEngine&        engine
        ) const {
        QList<QString> libraries = functionData.allLibraries();
        if (!libraries.isEmpty()) {
            for (QList<QString>::const_iterator it=libraries.constBegin(),end=libraries.constEnd() ; it!=end ; ++it) {
                const QString& libraryName = *it;
                engine.dependsOnHeader(libraryName);
                engine.dependsOnLibrary(libraryName);
            }
        }

        return identifyChildDependenciesAndExplicitTypes(functionData, element, engine);
    }


    bool CppFunctionTranslationEngine::implementation(
            const FunctionData&             functionData,
            QSharedPointer<FunctionElement> element,
            CppCodeGenerationEngine&        engine
        ) const {
        bool success = true;

        CppContext& context                   = engine.context();
        QString     internalName              = functionData.internalName();
        bool        requiresPerThreadInstance = functionData.requiresPerThreadInstance();

        context(element) << QString("%1(").arg(internalName);

        bool commaRequired = false;
        if (requiresPerThreadInstance) {
            context(element) << "pt";
            commaRequired = true;
        }

        unsigned long numberChildren = element->numberChildren();
        unsigned long i              = 0;
        while (success && i < numberChildren) {
            ElementPointer child = element->child(i);
            if (!child.isNull() && !child->isPlaceholder()) {
                if (commaRequired) {
                    context(element) << ",(";
                } else {
                    commaRequired = true;
                    context(element) << "(";
                }

                success = engine.translateChild(child);
                context(element) << ")";
            }

            ++i;
        }

        context(element) << ")";

        return success;
    }


    bool CppFunctionTranslationEngine::identifyChildDependenciesAndExplicitTypes(
            const FunctionData&             /* functionData */,
            QSharedPointer<FunctionElement> element,
            CppCodeGenerationEngine&        engine
        ) {
        bool     success        = true;
        unsigned numberChildren = element->numberChildren();
        unsigned index          = 0;

        while (success && index < numberChildren) {
            ElementPointer childElement = element->child(index);
            if (!childElement.isNull()) {
                success = engine.translateChild(childElement);
            }

            ++index;
        }

        return success;
    }
}
