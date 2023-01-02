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
* \file Ld::CppElseIfOperatorTranslator class.
***********************************************************************************************************************/

#include <QString>

#include "ld_else_if_operator_element.h"
#include "ld_root_element.h"
#include "ld_set_element.h"
#include "ld_tuple_element.h"
#include "ld_range_2_element.h"
#include "ld_range_3_element.h"
#include "ld_list_placeholder_element.h"
#include "ld_data_type.h"
#include "ld_identifier_database.h"
#include "ld_variable_element.h"
#include "ld_cpp_code_generator_diagnostic.h"
#include "ld_cpp_code_generation_engine.h"
#include "ld_cpp_context.h"
#include "ld_cpp_translator.h"
#include "ld_cpp_else_if_operator_translator.h"

namespace Ld {
    CppElseIfOperatorTranslator::~CppElseIfOperatorTranslator() {}


    QString CppElseIfOperatorTranslator::elementName() const {
        return ElseIfOperatorElement::elementName;
    }


    bool CppElseIfOperatorTranslator::threadImplementation(ElementPointer element, CppCodeGenerationEngine& engine) {
        bool success = true;

        ElementPointer condition = element->child(0);
        ElementPointer operation = element->child(1);

        CppContext& context = engine.context();

        if (!condition.isNull() && !operation.isNull()) {
            context(element) << "else if (M::inlineToBoolean(";
            engine.translateChild(condition);
            context(element) << ")) {\n";
            context.startedNewStatement();
            engine.translateChild(operation);
            context.startNewStatement();
            context(element) << "}\n";
            context.startedNewStatement();

            success = true;
        } else {
            engine.translationErrorDetected(
                new CppCodeGeneratorDiagnostic(
                    element,
                    CppCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                    dynamic_cast<const CppTranslationPhase&>(engine.translationPhase()),
                    CppCodeGeneratorDiagnostic::Code::MISSING_PARAMETER_OR_FIELD,
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


    bool CppElseIfOperatorTranslator::methodDefinitions(ElementPointer element, CppCodeGenerationEngine& engine) {
        bool success;

        if (!engine.atGlobalScope()) {
            success = threadImplementation(element, engine);
        } else {
            success = true;
        }

        return success;
    }
}
