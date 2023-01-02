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
* \file Ld::CppSubscriptRowColumnOperatorTranslator class.
***********************************************************************************************************************/

#include <QString>

#include "ld_element_structures.h"
#include "ld_element.h"
#include "ld_format.h"
#include "ld_operator_format.h"
#include "ld_cpp_code_generation_engine.h"
#include "ld_cpp_code_generator_diagnostic.h"
#include "ld_cpp_context.h"
#include "ld_subscript_row_column_operator_element.h"
#include "ld_cpp_translator.h"
#include "ld_cpp_subscript_row_column_operator_translator.h"

namespace Ld {
    CppSubscriptRowColumnOperatorTranslator::~CppSubscriptRowColumnOperatorTranslator() {}


    QString CppSubscriptRowColumnOperatorTranslator::elementName() const {
        return SubscriptRowColumnOperatorElement::elementName;
    }


    bool CppSubscriptRowColumnOperatorTranslator::threadImplementation(
            ElementPointer           element,
            CppCodeGenerationEngine& engine
        ) {
        bool success = true;

        CppContext& context = engine.context();

        ElementPointer child0 = element->child(0);
        ElementPointer child1 = element->child(1);
        ElementPointer child2 = element->child(2);

        if (!child0.isNull() && !child1.isNull() && !child2.isNull()) {
            DataType::ValueType child0ValueType = child0->valueType();
            if (child0ValueType == DataType::ValueType::VARIANT        ||
                child0ValueType == DataType::ValueType::MATRIX_BOOLEAN ||
                child0ValueType == DataType::ValueType::MATRIX_INTEGER ||
                child0ValueType == DataType::ValueType::MATRIX_REAL    ||
                child0ValueType == DataType::ValueType::MATRIX_COMPLEX    ) {
                DataType::ValueType child1ValueType = child1->valueType();
                if (child1ValueType == DataType::ValueType::BOOLEAN        ||
                    child1ValueType == DataType::ValueType::MATRIX_BOOLEAN    ) {
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

                DataType::ValueType child2ValueType = child2->valueType();
                if (child2ValueType == DataType::ValueType::BOOLEAN        ||
                    child2ValueType == DataType::ValueType::MATRIX_BOOLEAN    ) {
                    engine.translationErrorDetected(
                        new CppCodeGeneratorDiagnostic(
                            child2,
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

                if (success) {
                    context(element) << QString("(");
                    engine.translateChild(child0);
                    context(element) << ").at((";
                    engine.translateChild(child1);
                    context(element) << "),(";
                    engine.translateChild(child2);
                    context(element) << "))";
                }
            } else {
                engine.translationErrorDetected(
                    new CppCodeGeneratorDiagnostic(
                        child0,
                        CppCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                        dynamic_cast<const CppTranslationPhase&>(engine.translationPhase()),
                        CppCodeGeneratorDiagnostic::Code::CAN_ONLY_SUBSCRIPT_MATRICES,
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


    bool CppSubscriptRowColumnOperatorTranslator::methodDefinitions(
            ElementPointer           element,
            CppCodeGenerationEngine& engine
        ) {
        return threadImplementation(element, engine);
    }
}
