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
* This file implements the \ref Ld::CppVariableTranslator class.
***********************************************************************************************************************/

#include <QString>

#include "ld_variable_element.h"
#include "ld_element_structures.h"
#include "ld_element_value_data.h"
#include "ld_function_element.h"
#include "ld_root_element.h"
#include "ld_identifier.h"
#include "ld_identifier_container.h"
#include "ld_data_type.h"
#include "ld_data_type_translator.h"
#include "ld_cpp_data_type_translator.h"
#include "ld_cpp_context.h"
#include "ld_cpp_code_generator_diagnostic.h"
#include "ld_cpp_code_generation_engine.h"
#include "ld_cpp_translator.h"
#include "ld_cpp_variable_translator.h"

namespace Ld {
    CppVariableTranslator::~CppVariableTranslator() {}


    QString CppVariableTranslator::elementName() const {
        return Ld::VariableElement::elementName;
    }


    bool CppVariableTranslator::identifyDependenciesAndExplicitTypes(
            ElementPointer           element,
            CppCodeGenerationEngine& engine
        ) {
        QString text1 = element->text(0);
        QString text2 = element->text(1);

        IdentifierContainer identifier = engine.identifier(text1, text2);
        if (identifier.isInvalid()) {
            ElementPointer        parentScope = engine.currentScope();
            Identifier::DefinedAs definedAs;

            assert(!parentScope.isNull());
            if (parentScope->typeName() == Ld::FunctionElement::elementName && engine.asLValue()) {
                definedAs = Identifier::DefinedAs::FUNCTION_PARAMETER;
            } else if (parentScope->typeName() == Ld::RootElement::elementName) {
                definedAs = Identifier::DefinedAs::GLOBAL_SCOPE_VARIABLE;
            } else {
                definedAs = Identifier::DefinedAs::LOCAL_SCOPE_VARIABLE;
            }

            identifier = new Identifier(text1, text2, definedAs, parentScope);
            engine.addIdentifier(identifier);
        }

        identifier->insertElementData(ElementValueData(element));
        element.dynamicCast<VariableElement>()->setIdentifier(identifier);

        return true;
    }


    bool CppVariableTranslator::threadImplementation(ElementPointer element, CppCodeGenerationEngine& engine) {
        bool success = true;

        CppContext&         context    = engine.context();
        QString             text1      = element->text(0);
        QString             text2      = element->text(1);
        IdentifierContainer identifier = engine.identifier(text1, text2);

        if (identifier.isValid()) {
            QString internalName = identifier.internalName();
            context(element) << internalName;
        } else {
            engine.translationErrorDetected(
                new CppCodeGeneratorDiagnostic(
                    element,
                    CppCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                    dynamic_cast<const CppTranslationPhase&>(engine.translationPhase()),
                    CppCodeGeneratorDiagnostic::Code::UNDEFINED_VARIABLE,
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


    bool CppVariableTranslator::methodDefinitions(ElementPointer element, CppCodeGenerationEngine& engine) {
        return threadImplementation(element, engine);
    }
}
