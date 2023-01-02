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
* \file Ld::CppSubscriptIndexOperatorTranslator class.
***********************************************************************************************************************/

#include <QString>

#include "ld_element_structures.h"
#include "ld_element.h"
#include "ld_format.h"
#include "ld_operator_format.h"
#include "ld_cpp_code_generation_engine.h"
#include "ld_cpp_code_generator_diagnostic.h"
#include "ld_cpp_context.h"
#include "ld_subscript_index_operator_element.h"
#include "ld_cpp_binary_operator_translator_base.h"
#include "ld_cpp_subscript_index_operator_translator.h"

namespace Ld {
    CppSubscriptIndexOperatorTranslator::~CppSubscriptIndexOperatorTranslator() {}


    QString CppSubscriptIndexOperatorTranslator::elementName() const {
        return SubscriptIndexOperatorElement::elementName;
    }


    bool CppSubscriptIndexOperatorTranslator::threadImplementation(
            ElementPointer           element,
            CppCodeGenerationEngine& engine
        ) {
        bool success;

        CppContext& context = engine.context();

        ElementPointer child0 = element->child(0);
        ElementPointer child1 = element->child(1);

        if (!child0.isNull() || !child1.isNull()) {
            DataType::ValueType child0ValueType = child0->valueType();
            if (child0ValueType == DataType::ValueType::VARIANT        ||
                child0ValueType == DataType::ValueType::TUPLE          ||
                child0ValueType == DataType::ValueType::MATRIX_BOOLEAN ||
                child0ValueType == DataType::ValueType::MATRIX_INTEGER ||
                child0ValueType == DataType::ValueType::MATRIX_REAL    ||
                child0ValueType == DataType::ValueType::MATRIX_COMPLEX    ) {
                DataType::ValueType child1ValueType = child1->valueType();
                if (child1ValueType != DataType::ValueType::BOOLEAN        &&
                    child1ValueType != DataType::ValueType::MATRIX_BOOLEAN    ) {
                    context(element) << QString("(");
                    engine.translateChild(child0);
                    context(element) << ").at(";
                    engine.translateChild(child1);
                    context(element) << ")";

                    success = true;
                } else {
                    engine.translationErrorDetected(
                        new CppCodeGeneratorDiagnostic(
                            child1,
                            CppCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                            dynamic_cast<const CppTranslationPhase&>(engine.translationPhase()),
                            CppCodeGeneratorDiagnostic::Code::BOOLEAN_SUBSCRIPTS_NOT_SUPPORTED,
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
            } else {
                engine.translationErrorDetected(
                    new CppCodeGeneratorDiagnostic(
                        child0,
                        CppCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                        dynamic_cast<const CppTranslationPhase&>(engine.translationPhase()),
                        CppCodeGeneratorDiagnostic::Code::CAN_ONLY_SUBSCRIPT_TUPLES_AND_MATRICES,
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


    bool CppSubscriptIndexOperatorTranslator::methodDefinitions(
            ElementPointer           element,
            CppCodeGenerationEngine& engine
        ) {
        return threadImplementation(element, engine);
    }
}
