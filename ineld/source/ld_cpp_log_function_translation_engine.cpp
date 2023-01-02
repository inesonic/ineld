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
* This file implements the \ref Ld::CppLogFunctionTranslationEngine class.
***********************************************************************************************************************/

#include "ld_element_structures.h"
#include "ld_function_element.h"
#include "ld_function_data.h"
#include "ld_factorial_operator_element.h"
#include "ld_cpp_code_generation_engine.h"
#include "ld_cpp_code_generator_diagnostic.h"
#include "ld_cpp_context.h"
#include "ld_cpp_function_translation_engine.h"
#include "ld_cpp_log_function_translation_engine.h"

namespace Ld {
    const CppLogFunctionTranslationEngine CppLogFunctionTranslationEngine::instance;

    CppLogFunctionTranslationEngine::CppLogFunctionTranslationEngine() {}


    CppLogFunctionTranslationEngine::~CppLogFunctionTranslationEngine() {}


    bool CppLogFunctionTranslationEngine::identifyDependenciesAndExplicitTypes(
            const FunctionData&             functionData,
            QSharedPointer<FunctionElement> element,
            CppCodeGenerationEngine&        engine
        ) const {
        return identifyChildDependenciesAndExplicitTypes(functionData, element, engine);
    }


    bool CppLogFunctionTranslationEngine::implementation(
            const FunctionData&             functionData,
            QSharedPointer<FunctionElement> element,
            CppCodeGenerationEngine&        engine
        ) const {
        QString            functionMethod            = functionData.internalName();
        ElementPointerList logParameters             = element->parameterElements();
        ElementPointerList parameters                = logParameters;
        bool               requiresPerThreadInstance = functionData.requiresPerThreadInstance();

        if (logParameters.size() == 2) {
            ElementPointer child0        = logParameters.at(0);
            ElementPointer child1        = logParameters.at(1);
            QString        childTypeName = child1->typeName();

            if (childTypeName == Ld::FunctionElement::elementName) {
                QString text1 = child1->text(0);
                QString text2 = child1->text(1);

                if (text1 == QString(QChar(0x0393)) && text2.isEmpty()) {
                    functionMethod = "M::logGamma";
                    parameters     = child1->children();
                    parameters.prepend(child0);
                }
            } else if (childTypeName == Ld::FactorialOperatorElement::elementName) {
                functionMethod = "M::logFactorial";
                parameters     = child1->children();
                parameters.prepend(child0);
            } /* else if (childTypeName == Ld::BinomailOperatorElement::elementName) {
            } */

            CppContext& context = engine.context();
            context(element) << QString("%1(").arg(functionMethod);

            bool commaRequired = false;
            if (requiresPerThreadInstance) {
                context(element) << "pt";
                commaRequired = true;
            }

            unsigned long numberChildren = static_cast<unsigned long>(parameters.size());
            for (unsigned i=0 ; i<numberChildren ; ++i) {
                ElementPointer child = parameters.at(i);
                if (!child.isNull() && !child->isPlaceholder()) {
                    if (commaRequired) {
                        context(element) << ",(";
                    } else {
                        context(element) << "(";
                        commaRequired = true;
                    }

                    engine.translateChild(child);
                    context(element) << ")";
                }
            }

            context(element) << ")";
        }

        return true;
    }
}
