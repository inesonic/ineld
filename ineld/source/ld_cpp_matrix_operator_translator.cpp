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
* \file Ld::CppMatrixOperatorTranslator class.
***********************************************************************************************************************/

#include <QString>

#include "ld_matrix_operator_element.h"
#include "ld_data_type.h"
#include "ld_cpp_context.h"
#include "ld_cpp_code_generation_engine.h"
#include "ld_cpp_code_generator_diagnostic.h"
#include "ld_cpp_binary_operator_translator_base.h"
#include "ld_cpp_matrix_operator_translator.h"

namespace Ld {
    CppMatrixOperatorTranslator::~CppMatrixOperatorTranslator() {}


    QString CppMatrixOperatorTranslator::elementName() const {
        return MatrixOperatorElement::elementName;
    }


    bool CppMatrixOperatorTranslator::threadImplementation(ElementPointer element, CppCodeGenerationEngine& engine) {
        CppContext& context = engine.context();

        bool                success   = true;
        DataType::ValueType valueType = element->valueType();
        QString             matrixTypeString;
        switch (valueType) {
            case DataType::ValueType::MATRIX_BOOLEAN: { matrixTypeString = "MatrixBoolean";   break; }
            case DataType::ValueType::MATRIX_INTEGER: { matrixTypeString = "MatrixInteger";   break; }
            case DataType::ValueType::MATRIX_REAL:    { matrixTypeString = "MatrixReal";      break; }
            case DataType::ValueType::MATRIX_COMPLEX: { matrixTypeString = "MatrixComplex";   break; }

            default: {
                success = false;
                break;
            }
        }

        if (!success) {
            engine.translationErrorDetected(
                new CppCodeGeneratorDiagnostic(
                    element,
                    CppCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                    dynamic_cast<const CppTranslationPhase&>(engine.translationPhase()),
                    CppCodeGeneratorDiagnostic::Code::INCOMPATIBLE_TYPE_IN_ASSIGNMENT,
                    QString(),
                    engine.contextPointer(),
                    CppCodeGeneratorDiagnostic::CompilerCode::NO_ERROR,
                    context.byteOffset(),
                    context.lineNumber(),
                    context.columnNumber()
                )
            );
        } else {
            QSharedPointer<MatrixOperatorElement> matrixElement = element.dynamicCast<MatrixOperatorElement>();
            assert(!matrixElement.isNull());

            unsigned long numberRows    = matrixElement->numberRows();
            unsigned long numberColumns = matrixElement->numberColumns();

            context(element) << QString("M::%1::build(%2,%3").arg(matrixTypeString).arg(numberRows).arg(numberColumns);

            for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                for (unsigned long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                    ElementPointer child = matrixElement->child(rowIndex, columnIndex);

                    context(element) << ",(";
                    engine.translateChild(child);
                    context(element) << ")";
                }
            }

            context(element) << ")";
        }

        return success;
    }


    bool CppMatrixOperatorTranslator::methodDefinitions(ElementPointer element, CppCodeGenerationEngine& engine) {
        return threadImplementation(element, engine);
    }
}
