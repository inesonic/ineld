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
* \file Ld::CppForAllInOperatorTranslator class.
***********************************************************************************************************************/

#include <QString>

#include "ld_for_all_in_operator_element.h"
#include "ld_root_element.h"
#include "ld_function_element.h"
#include "ld_set_element.h"
#include "ld_tuple_element.h"
#include "ld_range_2_element.h"
#include "ld_range_3_element.h"
#include "ld_matrix_operator_element.h"
#include "ld_list_placeholder_element.h"
#include "ld_data_type.h"
#include "ld_identifier_database.h"
#include "ld_variable_element.h"
#include "ld_cpp_code_generator_diagnostic.h"
#include "ld_cpp_code_generation_engine.h"
#include "ld_cpp_context.h"
#include "ld_cpp_translator.h"
#include "ld_cpp_for_all_in_operator_translator.h"

namespace Ld {
    CppForAllInOperatorTranslator::~CppForAllInOperatorTranslator() {}


    QString CppForAllInOperatorTranslator::elementName() const {
        return ForAllInOperatorElement::elementName;
    }


    bool CppForAllInOperatorTranslator::identifyDependenciesAndExplicitTypes(
            ElementPointer           element,
            CppCodeGenerationEngine& engine
        ) {
        ElementPointer index     = indexElement(element);
        ElementPointer iterable  = iterableElement(element);
        ElementPointer operation = operationElement(element);

        engine.setAsLValue();
        bool success = engine.translateChild(index);

        engine.setAsRValue();
        success = engine.translateChild(iterable) && success;
        success = engine.translateChild(operation) && success;

        return success;
    }


    bool CppForAllInOperatorTranslator::identifyInferredTypes(
            ElementPointer           element,
            CppCodeGenerationEngine& engine
        ) {
        bool success = true;

        ElementPointer index     = indexElement(element);
        ElementPointer iterable  = iterableElement(element);
        ElementPointer operation = operationElement(element);

        if (!index.isNull() && !iterable.isNull() && !operation.isNull()) {
            QString indexTypeName = index->typeName();
            if (indexTypeName == VariableElement::elementName) {
                success = engine.translateChild(iterable);

                if (success) {
                    DataType::ValueType inferredIndexValueType = iterableContentsType(iterable);
                    if (inferredIndexValueType != DataType::ValueType::NONE) {
                        QSharedPointer<VariableElement> variable = index.dynamicCast<VariableElement>();
                        QString                         text0    = variable->text(0);
                        QString                         text1    = variable->text(1);

                        IdentifierContainer identifier = engine.identifier(text0, text1);
                        if (identifier.isInvalid()) {
                            DataType              inferredType = DataType::fromValueType(inferredIndexValueType);
                            ElementPointer        parentScope  = engine.currentScope();
                            Identifier::DefinedAs definedAs;

                            if (parentScope->typeName() == RootElement::elementName) {
                                definedAs = Identifier::DefinedAs::GLOBAL_SCOPE_VARIABLE;
                            } else if (parentScope->typeName() == FunctionElement::elementName) {
                                definedAs = Identifier::DefinedAs::FUNCTION_PARAMETER;
                            } else {
                                definedAs = Identifier::DefinedAs::LOCAL_SCOPE_VARIABLE;
                            }

                            identifier = new Identifier(text0, text1, definedAs, parentScope, inferredType);
                            identifier.setTypeimplicitlySet();

                            engine.addIdentifier(identifier);
                        } else {
                            DataType currentDataType = identifier.dataType();

                            if (identifier.typeImplicitlySet()) {
                                DataType::ValueType currentValueType = currentDataType.valueType();
                                DataType::ValueType bestValueType;

                                if (currentValueType == DataType::ValueType::NONE) {
                                    bestValueType = inferredIndexValueType;
                                } else {
                                    bestValueType = DataType::bestUpcast(currentValueType, inferredIndexValueType);
                                }

                                if (bestValueType == DataType::ValueType::NONE) {
                                    CppContext& context = engine.context();

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

                                    success = false;
                                } else if (bestValueType != currentValueType) {
                                    DataType bestDataType = DataType::fromValueType(bestValueType);
                                    identifier->setDataType(bestDataType);
                                }
                            } else {
                                DataType::ValueType currentValueType = currentDataType.valueType();
                                DataType::ValueType bestValueType    = DataType::bestUpcast(
                                    currentValueType,
                                    inferredIndexValueType
                                );

                                if (bestValueType == DataType::ValueType::NONE) {
                                    CppContext& context = engine.context();

                                    engine.translationErrorDetected(
                                        new CppCodeGeneratorDiagnostic(
                                            index,
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

                                    success = false;
                                }
                            }
                        }
                    }
                }
            } else {
                CppContext& context = engine.context();

                engine.translationErrorDetected(
                    new CppCodeGeneratorDiagnostic(
                        index,
                        CppCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                        dynamic_cast<const CppTranslationPhase&>(engine.translationPhase()),
                        CppCodeGeneratorDiagnostic::Code::EXPECTED_VARIABLE,
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
                engine.translateChild(operation);
            }
        } else {
            CppContext& context = engine.context();

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


    bool CppForAllInOperatorTranslator::threadImplementation(ElementPointer element, CppCodeGenerationEngine& engine) {
        bool success = true;

        ElementPointer index     = indexElement(element);
        ElementPointer iterable  = iterableElement(element);
        ElementPointer operation = operationElement(element);

        if (!index.isNull() && !iterable.isNull() && !operation.isNull()) {
            if (index->typeName() == VariableElement::elementName) {
                QSharedPointer<VariableElement> variableIndex = index.dynamicCast<VariableElement>();
                engine.insertOperationCheckpoint(element);

                DataType::ValueType iterableValueType = iterable->valueType();
                QString             iterableTypeName  = iterable->typeName();

                if (iterableValueType == DataType::ValueType::MATRIX_BOOLEAN ||
                    iterableValueType == DataType::ValueType::MATRIX_INTEGER ||
                    iterableValueType == DataType::ValueType::MATRIX_REAL    ||
                    iterableValueType == DataType::ValueType::MATRIX_COMPLEX    ) {
                    if (iterableTypeName == MatrixOperatorElement::elementName) {
                        unsigned long  numberIterableChildren = iterable->numberChildren();
                        bool           hasRealChild           = false;
                        unsigned long  childIndex             = 0;
                        while (childIndex<numberIterableChildren && !hasRealChild) {
                            ElementPointer child = iterable->child(childIndex);
                            if (!child.isNull() && !child->isPlaceholder()) {
                                hasRealChild = true;
                            } else {
                                ++childIndex;
                            }
                        }

                        if (hasRealChild) {
                            success = matrixIteratorThreadImplementation(
                                index.dynamicCast<VariableElement>(),
                                iterable,
                                operation,
                                element,
                                engine
                            );
                        } else {
                            CppContext& context = engine.context();

                            engine.translationErrorDetected(
                                new CppCodeGeneratorDiagnostic(
                                    index,
                                    CppCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                                    dynamic_cast<const CppTranslationPhase&>(engine.translationPhase()),
                                    CppCodeGeneratorDiagnostic::Code::EMPTY_ITERABLE,
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
                        success = matrixIteratorThreadImplementation(
                            index.dynamicCast<VariableElement>(),
                            iterable,
                            operation,
                            element,
                            engine
                        );
                    }
                } else {
                    if (iterableTypeName == SetElement::elementName || iterableTypeName == TupleElement::elementName) {
                        unsigned long  numberIterableChildren = iterable->numberChildren();
                        unsigned long  numberNonPlaceholders  = 0;
                        ElementPointer firstIterableChild;
                        for (unsigned long childIndex=0 ; childIndex<numberIterableChildren ; ++childIndex) {
                            ElementPointer child = iterable->child(childIndex);
                            if (!child.isNull() && !child->isPlaceholder()) {
                                if (firstIterableChild.isNull()) {
                                    firstIterableChild = child;
                                }

                                ++numberNonPlaceholders;
                            }
                        }

                        if (numberNonPlaceholders == 1) {
                            if (firstIterableChild->typeName() == Range2Element::elementName) {
                                success = range2IteratorThreadImplementation(
                                    index.dynamicCast<VariableElement>(),
                                    firstIterableChild.dynamicCast<Range2Element>(),
                                    operation,
                                    element,
                                    engine
                                );
                            } else if (firstIterableChild->typeName() == Range3Element::elementName) {
                                success = range3IteratorThreadImplementation(
                                    index.dynamicCast<VariableElement>(),
                                    firstIterableChild.dynamicCast<Range3Element>(),
                                    operation,
                                    element,
                                    engine
                                );
                            } else {
                                success = operatorIteratorThreadImplementation(
                                    index.dynamicCast<VariableElement>(),
                                    iterable,
                                    operation,
                                    element,
                                    engine
                                );
                            }
                        } else if (numberNonPlaceholders > 1) {
                            success = operatorIteratorThreadImplementation(
                                index.dynamicCast<VariableElement>(),
                                iterable,
                                operation,
                                element,
                                engine
                            );
                        } else {
                            CppContext& context = engine.context();

                            engine.translationErrorDetected(
                                new CppCodeGeneratorDiagnostic(
                                    index,
                                    CppCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                                    dynamic_cast<const CppTranslationPhase&>(engine.translationPhase()),
                                    CppCodeGeneratorDiagnostic::Code::EMPTY_ITERABLE,
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
                    } else if (iterableTypeName == Range2Element::elementName) {
                        success = range2IteratorThreadImplementation(
                            index.dynamicCast<VariableElement>(),
                            iterable.dynamicCast<Range2Element>(),
                            operation,
                            element,
                            engine
                        );
                    } else if (iterableTypeName == Range3Element::elementName) {
                        success = range3IteratorThreadImplementation(
                            index.dynamicCast<VariableElement>(),
                            iterable.dynamicCast<Range3Element>(),
                            operation,
                            element,
                            engine
                        );
                    } else if (iterableTypeName == VariableElement::elementName) {
                        success = variableIteratorThreadImplementation(
                            index.dynamicCast<VariableElement>(),
                            iterable.dynamicCast<VariableElement>(),
                            operation,
                            element,
                            engine
                        );
                    } else {
                        success = operatorIteratorThreadImplementation(
                            index.dynamicCast<VariableElement>(),
                            iterable,
                            operation,
                            element,
                            engine
                        );
                    }
                }
            } else {
                CppContext& context = engine.context();

                engine.translationErrorDetected(
                    new CppCodeGeneratorDiagnostic(
                        index,
                        CppCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                        dynamic_cast<const CppTranslationPhase&>(engine.translationPhase()),
                        CppCodeGeneratorDiagnostic::Code::EXPECTED_VARIABLE,
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
            CppContext& context = engine.context();

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


    bool CppForAllInOperatorTranslator::methodDefinitions(ElementPointer element, CppCodeGenerationEngine& engine) {
        bool success;

        if (!engine.atGlobalScope()) {
            success = threadImplementation(element, engine);
        } else {
            success = true;
        }

        return success;
    }


    ElementPointer CppForAllInOperatorTranslator::indexElement(ElementPointer element) {
        return element->child(0);
    }


    ElementPointer CppForAllInOperatorTranslator::iterableElement(ElementPointer element) {
        return element->child(1);
    }


    ElementPointer CppForAllInOperatorTranslator::operationElement(ElementPointer element) {
        return element->child(2);
    }


    DataType::ValueType CppForAllInOperatorTranslator::iterableContentsType(ElementPointer iterable) {
        DataType::ValueType result;

        QString iterableTypeName = iterable->typeName();
        if (iterableTypeName == Range2Element::elementName || iterableTypeName == Range3Element::elementName) {
            result = iterable->valueType();
        } else if (iterableTypeName == SetElement::elementName || iterableTypeName == TupleElement::elementName) {
            // There's no way to always reliably identify types in a set or tuple and sets/tuples can store
            // values that are incompatible with each other.  To address this, we try to dope out the best type
            // but resolve to a variant type if the types could not be determined or are incompatible.

            result = DataType::ValueType::NONE;

            unsigned long iterableNumberChildren = iterable->numberChildren();
            unsigned long childIndex             = 0;
            while (childIndex < iterableNumberChildren && result != DataType::ValueType::VARIANT) {
                ElementPointer iterableChild = iterable->child(childIndex);
                if (!iterableChild->isPlaceholder()) {
                    if (childIndex == 0) {
                        result = iterableChild->valueType();
                    } else {
                        DataType::ValueType childType = iterableChild->valueType();
                        if (childType == DataType::ValueType::NONE) {
                            result = DataType::ValueType::VARIANT;
                        } else {
                            result = DataType::bestUpcast(result, childType);
                            if (result == DataType::ValueType::NONE) {
                                result = DataType::ValueType::VARIANT;
                            }
                        }
                    }
                }

                ++childIndex;
            }

            if (result == DataType::ValueType::NONE) {
                result = DataType::ValueType::VARIANT;
            }
        } else {
            DataType::ValueType iterableValueType = iterable->valueType();
            if (iterableValueType == DataType::ValueType::SET || iterableValueType == DataType::ValueType::TUPLE) {
                result = DataType::ValueType::VARIANT;
            } else {
                // The line below will return DataType::ValueType::NONE if the iterable value type is not a matrix.
                result = DataType::baseTypeFromMatrixType(iterableValueType);
            }
        }

        return result;
    }


    bool CppForAllInOperatorTranslator::range2IteratorThreadImplementation(
            QSharedPointer<VariableElement> index,
            QSharedPointer<Range2Element>   iterable,
            ElementPointer                  operation,
            ElementPointer                  element,
            CppCodeGenerationEngine&        engine
        ) {
        bool success = true;

        CppContext& context = engine.context();

        ElementPointer rangeStartElement = iterable->child(0);
        ElementPointer rangeEndElement   = iterable->child(1);

        if (!rangeStartElement.isNull() && !rangeEndElement.isNull()) {
            unsigned long lineNumber = context.lineNumber();

            QString rangeStartVariable = QString("LRS%1").arg(lineNumber);
            QString rangeEndVariable   = QString("LRE%1").arg(lineNumber);

            context(element) << QString("auto %1 = (").arg(rangeStartVariable);
            success = engine.translateChild(rangeStartElement);
            context(element) << ")";

            context.startNewStatement();
            context(element) << QString("auto %1 = (").arg(rangeEndVariable);
            success = engine.translateChild(rangeEndElement) && success;
            context(element) << ")";

            context.startNewStatement();
            context(element) << QString("M::internalValidateRange(%1,%2)")
                                .arg(rangeStartVariable, rangeEndVariable);

            context.startNewStatement();
            context(element) << "for (";

            engine.setAsLValue();
            success = engine.translateChild(index) && success;
            engine.setAsRValue();

            context(element) << QString("=%1 ; ").arg(rangeStartVariable);
            success = engine.translateChild(index) && success;
            context(element) << QString("<=%2 ; ++").arg(rangeEndVariable);
            success = engine.translateChild(index) && success;
            context(element) << ") {\n";

            context.startedNewStatement();
            success = engine.translateChild(operation) && success;
            context.startNewStatement();
            context(element) << "}\n";
            context.startedNewStatement();
        } else {
            engine.translationErrorDetected(
                new CppCodeGeneratorDiagnostic(
                    iterable,
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


    bool CppForAllInOperatorTranslator::range3IteratorThreadImplementation(
            QSharedPointer<VariableElement> index,
            QSharedPointer<Range3Element>   iterable,
            ElementPointer                  operation,
            ElementPointer                  element,
            CppCodeGenerationEngine&        engine
        ) {
        bool success;

        CppContext& context = engine.context();

        ElementPointer rangeFirstValueElement  = iterable->child(0);
        ElementPointer rangeSecondValueElement = iterable->child(1);
        ElementPointer rangeEndElement         = iterable->child(2);

        if (!rangeFirstValueElement.isNull() && !rangeSecondValueElement.isNull() && !rangeEndElement.isNull()) {
            // Our iterable is a range with explicit increment, construct as a for loop.

            unsigned long lineNumber = context.lineNumber();

            QString rangeFirstValueVariable  = QString("LRF%1").arg(lineNumber);
            QString rangeSecondValueVariable = QString("LRS%1").arg(lineNumber);
            QString rangeEndVariable         = QString("LRE%1").arg(lineNumber);
            QString rangeMultiplierVariable  = QString("LRM%1").arg(lineNumber);
            QString rangeCountTermsVariable  = QString("LRC%1").arg(lineNumber);
            QString rangeIndexVariable       = QString("LRI%1").arg(lineNumber);

            context.startNewStatement();
            context(element) << QString("auto %1 = (").arg(rangeFirstValueVariable);
            engine.translateChild(rangeFirstValueElement);
            context(element) << ")";

            context.startNewStatement();
            context(element) << QString("auto %1 = (").arg(rangeSecondValueVariable);
            engine.translateChild(rangeSecondValueElement);
            context(element) << ")";

            context.startNewStatement();
            context(element) << QString("auto %1 = (").arg(rangeEndVariable);
            engine.translateChild(rangeEndElement);
            context(element) << ")";

            context.startNewStatement();
            context(element) << QString("M::internalValidateRange(%1,%2,%3)")
                                .arg(rangeFirstValueVariable, rangeSecondValueVariable, rangeEndVariable);

            context.startNewStatement();
            context(element) << QString("auto %1 = (%2 - %3)")
                                .arg(rangeMultiplierVariable, rangeSecondValueVariable, rangeFirstValueVariable);

            context.startNewStatement();
            context(element) << QString("auto %1 = (%2 - %3)/%4")
                                .arg(
                                    rangeCountTermsVariable,
                                    rangeEndVariable,
                                    rangeFirstValueVariable,
                                    rangeMultiplierVariable
                                );

            context.startNewStatement();
            context(element) << QString("for (auto %1=0 ; %1<=%2 ; ++%1) {\n")
                                .arg(rangeIndexVariable, rangeCountTermsVariable);

            context.startedNewStatement();
            engine.setAsLValue();
            engine.translateChild(index);
            engine.setAsRValue();
            context(element) << QString(" = %1 + (%2 * %3)")
                                .arg(rangeFirstValueVariable, rangeMultiplierVariable, rangeIndexVariable);
            context.startNewStatement();

            engine.translateChild(operation);
            context.startNewStatement();
            context(element) << "}\n";
            context.startedNewStatement();

            success = true;
        } else {
            engine.translationErrorDetected(
                new CppCodeGeneratorDiagnostic(
                    iterable,
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


    bool CppForAllInOperatorTranslator::variableIteratorThreadImplementation(
            QSharedPointer<VariableElement> index,
            QSharedPointer<VariableElement> iterable,
            ElementPointer                  operation,
            ElementPointer                  element,
            CppCodeGenerationEngine&        engine
        ) {
        CppContext&   context            = engine.context();
        unsigned long lineNumber         = context.lineNumber();
        QString       rangeIndexVariable = QString("LRI%1").arg(lineNumber);

        context.startNewStatement();
        context(element) << QString("for (const auto& %1 : ").arg(rangeIndexVariable);
        engine.translateChild(iterable);
        context(element) << ") {\n";

        engine.setAsLValue();
        engine.translateChild(index);
        engine.setAsRValue();

        context(element) << QString(" = %1;\n").arg(rangeIndexVariable);

        context.startedNewStatement();
        engine.translateChild(operation);
        context.startNewStatement();
        context(element) << "};\n";
        context.startedNewStatement();

        return true;
    }


    bool CppForAllInOperatorTranslator::matrixIteratorThreadImplementation(
            QSharedPointer<VariableElement> index,
            ElementPointer                  iterable,
            ElementPointer                  operation,
            ElementPointer                  element,
            CppCodeGenerationEngine&        engine
        ) {
        return operatorIteratorThreadImplementation(index, iterable, operation, element, engine);
    }


    bool CppForAllInOperatorTranslator::operatorIteratorThreadImplementation(
            QSharedPointer<VariableElement> index,
            ElementPointer                  iterable,
            ElementPointer                  operation,
            ElementPointer                  element,
            CppCodeGenerationEngine&        engine
        ) {
        CppContext&   context               = engine.context();
        unsigned long lineNumber            = context.lineNumber();
        QString       rangeIterableVariable = QString("LRO%1").arg(lineNumber);
        QString       rangeIndexVariable    = QString("LRI%1").arg(lineNumber);

        context.startNewStatement();
        context(element) << QString("auto %1 = (").arg(rangeIterableVariable);
        engine.translateChild(iterable);
        context(element) << ")";

        context.startNewStatement();
        context(element) << QString("for (const auto& %1 : %2) {\n")
                            .arg(rangeIndexVariable, rangeIterableVariable);

        context(element) << QString("M::assign(");
        engine.setAsLValue();
        engine.translateChild(index);
        engine.setAsRValue();
        context(element) << QString(",%1);\n").arg(rangeIndexVariable);

        context.startedNewStatement();
        engine.translateChild(operation);
        context.startNewStatement();
        context(element) << "};\n";
        context.startedNewStatement();

        return true;
    }
}
