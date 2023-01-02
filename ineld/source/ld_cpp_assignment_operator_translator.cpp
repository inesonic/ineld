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
* This file implements the \ref Ld::CppAssignmentOperatorTranslator class.
***********************************************************************************************************************/

#include <QString>

#include <model_variant.h>

#include "ld_assignment_operator_element.h"
#include "ld_element_structures.h"
#include "ld_data_type.h"
#include "ld_root_element.h"
#include "ld_variable_element.h"
#include "ld_function_element.h"
#include "ld_list_placeholder_element.h"
#include "ld_element_of_set_operator_element.h"
#include "ld_subscript_index_operator_element.h"
#include "ld_subscript_row_column_operator_element.h"
#include "ld_identifier.h"
#include "ld_identifier_container.h"
#include "ld_operation.h"
#include "ld_operation_database.h"
#include "ld_cpp_code_generator.h"
#include "ld_cpp_code_generation_engine.h"
#include "ld_cpp_code_generator_diagnostic.h"
#include "ld_cpp_context.h"
#include "ld_data_type_translator.h"
#include "ld_cpp_data_type_translator.h"
#include "ld_cpp_binary_operator_translator_base.h"
#include "ld_cpp_assignment_operator_translator.h"

namespace Ld {
    static const DataType::ValueType subscriptedTypeConversion[DataType::numberValueTypes] = {
        DataType::ValueType::NONE,           // NONE
        DataType::ValueType::VARIANT,        // VARIANT
        DataType::ValueType::MATRIX_BOOLEAN, // BOOLEAN
        DataType::ValueType::MATRIX_INTEGER, // INTEGER
        DataType::ValueType::MATRIX_REAL,    // REAL
        DataType::ValueType::MATRIX_COMPLEX, // COMPLEX
        DataType::ValueType::TUPLE,          // SET
        DataType::ValueType::TUPLE,          // TUPLE
        DataType::ValueType::TUPLE,          // MATRIX_BOOLEAN
        DataType::ValueType::TUPLE,          // MATRIX_INTEGER
        DataType::ValueType::TUPLE,          // MATRIX_REAL
        DataType::ValueType::TUPLE           // MATRIX_COMPLEX
    };

    CppAssignmentOperatorTranslator::~CppAssignmentOperatorTranslator() {}


    QString CppAssignmentOperatorTranslator::elementName() const {
        return Ld::AssignmentOperatorElement::elementName;
    }


    bool CppAssignmentOperatorTranslator::identifyDependenciesAndExplicitTypes(
            ElementPointer           element,
            CppCodeGenerationEngine& engine
        ) {
        bool success = true;

        ElementPointer child0 = element->child(0);
        ElementPointer child1 = element->child(1);

        if (!child0.isNull() && !child1.isNull()) {
            if (element->valueType() == Ld::DataType::ValueType::BOOLEAN) {
                // Equals is being used as a relational operator.

                engine.setAsRValue();
                engine.translateChild(child0);
                engine.translateChild(child1);
            } else if (child0->typeName() == FunctionElement::elementName) {
                // Assignment is being used as a function declaration.

                QSharedPointer<FunctionElement> functionElement = child0.dynamicCast<FunctionElement>();
                QString                         text1           = functionElement->text(0);

                if (!text1.isEmpty()) {
                    QString text2 = functionElement->text(1);

                    ElementPointer      parentScope = engine.currentScope();
                    IdentifierContainer identifier  = engine.identifier(text1, text2, parentScope);
                    if (identifier.isInvalid()) {
                        Identifier::DefinedAs definedAs;
                        if (parentScope->typeName() == Ld::RootElement::elementName) {
                            definedAs = Identifier::DefinedAs::GLOBAL_SCOPE_FUNCTION;
                        } else {
                            definedAs = Identifier::DefinedAs::LOCAL_SCOPE_FUNCTION;
                        }

                        identifier = IdentifierContainer(new Identifier(text1, text2, definedAs, parentScope));

                        engine.addIdentifier(identifier);
                        functionElement->setIdentifier(identifier);

                        ElementPointerList parameterElements = functionElement->parameterElements();

                        engine.pushNewScope(functionElement, true);
                        engine.setAsLValue();

                        ElementPointerList::const_iterator parameterIterator    = parameterElements.constBegin();
                        ElementPointerList::const_iterator parameterEndIterator = parameterElements.constEnd();
                        while (success && parameterIterator != parameterEndIterator) {
                            success = engine.translateChild(*parameterIterator);
                            ++parameterIterator;
                        }

                        engine.clearForcedScope();
                        engine.setAsRValue();

                        success = engine.translateChild(child1) && success;
                        engine.popCurrentScope();
                    } else {
                        CppContext& context = engine.context();

                        engine.translationErrorDetected(
                            new CppCodeGeneratorDiagnostic(
                                child0,
                                CppCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                                dynamic_cast<const CppTranslationPhase&>(engine.translationPhase()),
                                CppCodeGeneratorDiagnostic::Code::FUNCTION_ALREADY_DEFINED,
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
                            child0,
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
            } else if (child0->typeName() == SubscriptIndexOperatorElement::elementName) {
                QSharedPointer<SubscriptIndexOperatorElement>
                    subscriptOperator = child0.dynamicCast<SubscriptIndexOperatorElement>();

                ElementPointer subChild0 = subscriptOperator->child(0);
                ElementPointer subChild1 = subscriptOperator->child(1);

                engine.setAsLValue();
                engine.translateChild(subChild0);

                engine.setAsRValue();
                engine.translateChild(subChild1);

                engine.translateChild(child1);
            } else if (child0->typeName() == SubscriptRowColumnOperatorElement::elementName) {
                QSharedPointer<SubscriptRowColumnOperatorElement>
                    subscriptOperator = child0.dynamicCast<SubscriptRowColumnOperatorElement>();

                ElementPointer subChild0 = subscriptOperator->child(0);
                ElementPointer subChild1 = subscriptOperator->child(1);
                ElementPointer subChild2 = subscriptOperator->child(2);

                engine.setAsLValue();
                engine.translateChild(subChild0);

                engine.setAsRValue();
                engine.translateChild(subChild1);
                engine.translateChild(subChild2);

                engine.translateChild(child1);
            } else if (child0->typeName() == ElementOfSetOperatorElement::elementName) {
                engine.setAsLValue();
                engine.translateChild(child0);

                engine.setAsRValue();
                engine.translateChild(child1);
            } else if (child0->typeName() == VariableElement::elementName) {
                engine.setAsLValue();
                engine.translateChild(child0);

                engine.setAsRValue();
                engine.translateChild(child1);
            } else {
                CppContext& context = engine.context();

                engine.translationErrorDetected(
                    new CppCodeGeneratorDiagnostic(
                        child0,
                        CppCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                        dynamic_cast<const CppTranslationPhase&>(engine.translationPhase()),
                        CppCodeGeneratorDiagnostic::Code::UNEXPECTED_OR_INCOMPATIBLE_OPERATOR,
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


    bool CppAssignmentOperatorTranslator::identifyInferredTypes(
            ElementPointer           element,
            CppCodeGenerationEngine& engine
        ) {
        bool success = true;

        // We use the element value type to determine how the operator is being used.
        DataType::ValueType elementValueType = element->valueType();

        if (elementValueType != DataType::ValueType::BOOLEAN) {
            // Used as an assignment operator.

            QSharedPointer<RootElement> root = element->root().dynamicCast<RootElement>();
            if (!root.isNull()) {
                ElementPointer child0 = element->child(0);
                if (!child0.isNull()) {
                    if (child0->typeName() == FunctionElement::elementName) {
                        success = identifyInferredFunctionTypes(
                            element.dynamicCast<AssignmentOperatorElement>(),
                            child0.dynamicCast<FunctionElement>(),
                            engine
                        );
                    } else {
                        AssignmentType      assignmentType;
                        DataType::ValueType valueType;

                        ElementPointer variableElement = parseLeftChild(child0, engine, assignmentType, valueType);
                        if (!variableElement.isNull()) {
                            if (assignmentType != AssignmentType::EXPLICIT) {
                                success = identifyInferredVariableTypes(
                                    element.dynamicCast<AssignmentOperatorElement>(),
                                    variableElement.dynamicCast<VariableElement>(),
                                    engine,
                                    assignmentType
                                );
                            } else {
                                engine.setAsLValue();
                                success = engine.translateChild(element->child(0));
                                engine.setAsRValue();
                                success = engine.translateChild(element->child(1)) && success;
                            }
                        } else {
                            success = false;
                        }
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
            }
        } else {
            // Used as a relational comparison operator -- Do nothing here.
        }

        return success;
    }


    bool CppAssignmentOperatorTranslator::methodDeclarations(ElementPointer element, CppCodeGenerationEngine& engine) {
        bool           success;

        ElementPointer child0 = element->child(0);
        assert(!child0.isNull()); // Should have been checked by now.

        if (child0->typeName() == FunctionElement::elementName && engine.atGlobalScope()) {
            insertFunctionPrototype(
                child0.dynamicCast<FunctionElement>(),
                engine,
                QString(),
                true,
                true,
                false,
                true
            );

            engine.popCurrentScope();

            CppContext& context = engine.context();
            context.startNewStatement();
            context(element) << "static ";
            context.startedNewStatement();

            insertFunctionPrototype(
                child0.dynamicCast<FunctionElement>(),
                engine,
                QString("W"),
                true,
                true,
                true,
                true
            );

            engine.popCurrentScope();

            success = true;
        } else {
            success = true;
        }

        return success;
    }


    bool CppAssignmentOperatorTranslator::threadImplementation(
            ElementPointer           element,
            CppCodeGenerationEngine& engine
        ) {
        bool success = true;

        // We use the element value type to determine how the operator is being used.
        DataType::ValueType elementValueType = element->valueType();

        if (elementValueType == DataType::ValueType::BOOLEAN) {
            // Used as a relational comparison operator.
            success = CppBinaryOperatorTranslatorBase::threadImplementation(element, engine);
        } else {
            // Used as an assignment operator.

            CppContext& context = engine.context();

            ElementPointer child0 = element->child(0);
            ElementPointer child1 = element->child(1);

            QString child0TypeName = child0->typeName();
            if (child0TypeName != FunctionElement::elementName) {
                ElementPointer valueElement;
                QString        valueElementTypeName;
                if (child0TypeName == ElementOfSetOperatorElement::elementName) {
                    valueElement         = child0->child(0);
                    valueElementTypeName = valueElement->typeName();
                } else {
                    valueElement         = child0;
                    valueElementTypeName = child0TypeName;
                }

                if (valueElementTypeName == VariableElement::elementName) {
                    engine.insertOperationCheckpoint(element);
                    context(element) << "M::assign(";
                    success = engine.translateChild(valueElement);
                    context(element) << ",";
                    success = engine.translateChild(child1) && success;
                    context(element) << ")";
                } else if (valueElementTypeName == SubscriptIndexOperatorElement::elementName) {
                    ElementPointer subscriptChild0 = valueElement->child(0);
                    ElementPointer subscriptChild1 = valueElement->child(1);

                    engine.insertOperationCheckpoint(element);
                    context(element) << "(";
                    success = engine.translateChild(subscriptChild0);
                    context(element) << ").update((";
                    success = engine.translateChild(subscriptChild1) && success;
                    context(element) << "),(";
                    success = engine.translateChild(child1) && success;
                    context(element) << "))";
                } else if (valueElementTypeName == SubscriptRowColumnOperatorElement::elementName) {
                    ElementPointer subscriptChild0 = valueElement->child(0);
                    ElementPointer subscriptChild1 = valueElement->child(1);
                    ElementPointer subscriptChild2 = valueElement->child(2);

                    engine.insertOperationCheckpoint(element);
                    context(element) << "(";
                    success = engine.translateChild(subscriptChild0);
                    context(element) << ").update((";
                    success = engine.translateChild(subscriptChild1) && success;
                    context(element) << "),(";
                    success = engine.translateChild(subscriptChild2) && success;
                    context(element) << "),(";
                    success = engine.translateChild(child1) && success;
                    context(element) << "))";
                } else {
                    engine.translationErrorDetected(
                        new CppCodeGeneratorDiagnostic(
                            valueElement,
                            CppCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                            dynamic_cast<const CppTranslationPhase&>(engine.translationPhase()),
                            CppCodeGeneratorDiagnostic::Code::UNEXPECTED_OR_INCOMPATIBLE_OPERATOR,
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
            } else /* if (child0TypeName == FunctionElement::elementName) */ {
                if (!engine.atGlobalScope()) {
                    // We have a local scope function.  For this we code as a C++ lambda.

                    QSharedPointer<FunctionElement> functionElement = child0.dynamicCast<FunctionElement>();
                    IdentifierContainer             identifier      = functionElement->identifier();
                    assert(identifier.isValid());

                    context.startNewStatement();
                    context(element) << QString("auto %1 = [&](").arg(identifier.internalName());

                    engine.pushNewScope(functionElement);
                    insertFunctionParameters(functionElement, engine, true, false, true);
                    context(element) << "){\nreturn ";

                    engine.translateChild(child1);
                    context.startNewStatement();
                    context(element) << "};\n";
                    context.startedNewStatement();

                    engine.popCurrentScope();
                } else {
                    // Global functions are parsed when we process method definitions below.
                }
            }
        }

        return success;
    }


    bool CppAssignmentOperatorTranslator::methodDefinitions(ElementPointer element, CppCodeGenerationEngine& engine) {
        bool success;

        ElementPointer child0 = element->child(0);
        assert(!child0.isNull()); // Should have been checked by now.

        if (child0->typeName() == FunctionElement::elementName) {
            ElementPointer child1 = element->child(1);
            assert(!child1.isNull()); // Should have been checked by now.

            insertFunctionPrototype(
                child0.dynamicCast<FunctionElement>(),
                engine,
                QString("ModelImpl::"),
                true,
                true,
                false,
                true
            );

            CppContext& context = engine.context();
            context(element) << " {\n";
            context.startedNewStatement();

            context(element) << "return ";
            success = engine.translateChild(child1);

            context.startNewStatement();
            context(element) << "}\n";
            context.startedNewStatement();

            engine.popCurrentScope();

            success = insertFunctionPrototype(
                child0.dynamicCast<FunctionElement>(),
                engine,
                QString("ModelImpl::W"),
                true,
                true,
                true,
                true
            ) && success;

            context(element) << " {\nreturn modelImplementation->";
            context.startedNewStatement();
            success = insertFunctionPrototype(
                child0.dynamicCast<FunctionElement>(),
                engine,
                QString(),
                false,
                false,
                false,
                true
            ) && success;
            context(element) << ";\n}\n";
            context.startedNewStatement();

            engine.popCurrentScope();
            engine.popCurrentScope();
        } else {
            if (!engine.atGlobalScope()) {
                success = threadImplementation(element, engine);
            } else {
                success = true;
            }
        }

        return success;
    }


    QString CppAssignmentOperatorTranslator::cppOperator() const {
        return QString("==");
    }


    ElementPointer CppAssignmentOperatorTranslator::parseLeftChild(
            ElementPointer                                   leftChild,
            CppCodeGenerationEngine&                         engine,
            CppAssignmentOperatorTranslator::AssignmentType& assignmentType,
            DataType::ValueType&                             valueType
        ) {
        ElementPointer result;

        QString leftChildType = leftChild->typeName();
        if (leftChildType == VariableElement::elementName) {
            result         = leftChild;
            assignmentType = AssignmentType::SIMPLE;
        } else if (leftChildType == ElementOfSetOperatorElement::elementName) {
            ElementPointer setChild0 = leftChild->child(0);
            ElementPointer setChild1 = leftChild->child(1);

            if (!setChild0.isNull() && !setChild1.isNull()) {
                QString        setChild0TypeName = setChild0->typeName();
                ElementPointer variableElement;
                bool           isMatrixType = false;

                if (setChild0TypeName == VariableElement::elementName) {
                    variableElement = setChild0;
                } else if (setChild0TypeName == SubscriptIndexOperatorElement::elementName     ||
                           setChild0TypeName == SubscriptRowColumnOperatorElement::elementName    ) {
                    variableElement = setChild0->child(0);
                    isMatrixType    = true;
                }

                if (!variableElement.isNull() && variableElement->typeName() == VariableElement::elementName) {
                    valueType = DataType::fromSymbol(setChild1, isMatrixType).valueType();

                    if (valueType == DataType::ValueType::NONE) {
                        CppContext& context = engine.context();
                        engine.translationErrorDetected(
                            new CppCodeGeneratorDiagnostic(
                                setChild1,
                                CppCodeGeneratorDiagnostic::Type::INTERNAL_ERROR,
                                dynamic_cast<const CppTranslationPhase&>(engine.translationPhase()),
                                CppCodeGeneratorDiagnostic::Code::MISSING_DATA_TYPE_TRANSLATOR,
                                QString(),
                                engine.contextPointer(),
                                CppCodeGeneratorDiagnostic::CompilerCode::NO_ERROR,
                                context.byteOffset(),
                                context.lineNumber(),
                                context.columnNumber()
                            )
                        );
                    } else {
                        result         = variableElement;
                        assignmentType = AssignmentType::EXPLICIT;
                    }
                } else {
                    CppContext& context          = engine.context();
                    bool        missingParameter = variableElement.isNull();

                    engine.translationErrorDetected(
                        new CppCodeGeneratorDiagnostic(
                            missingParameter ? leftChild : setChild0,
                            CppCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                            dynamic_cast<const CppTranslationPhase&>(engine.translationPhase()),
                              missingParameter
                            ? CppCodeGeneratorDiagnostic::Code::MISSING_PARAMETER_OR_FIELD
                            : CppCodeGeneratorDiagnostic::Code::UNEXPECTED_OR_INCOMPATIBLE_OPERATOR,
                            QString(),
                            engine.contextPointer(),
                            CppCodeGeneratorDiagnostic::CompilerCode::NO_ERROR,
                            context.byteOffset(),
                            context.lineNumber(),
                            context.columnNumber()
                        )
                    );
                }
            } else {
                CppContext& context = engine.context();

                engine.translationErrorDetected(
                    new CppCodeGeneratorDiagnostic(
                        leftChild,
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
            }
        } else if (leftChildType == SubscriptIndexOperatorElement::elementName     ||
                   leftChildType == SubscriptRowColumnOperatorElement::elementName    ) {
            ElementPointer parent;
            QString        parentTypeName;
            ElementPointer variableElement         = leftChild;
            QString        variableElementTypeName = leftChildType;
            bool           hasSubscriptGrandparent = false;
            bool           isValidConstruct        = true;
            while (isValidConstruct && variableElement->typeName() != VariableElement::elementName) {
                if (variableElement->numberChildren() > 0) {
                    if (!parent.isNull()) {
                        hasSubscriptGrandparent = true;
                    }

                    parent         = variableElement;
                    parentTypeName = variableElementTypeName;

                    if (!parent->child(0).isNull()) {
                        variableElement         = parent->child(0);
                        variableElementTypeName = variableElement->typeName();
                        isValidConstruct        = (
                               variableElementTypeName == SubscriptIndexOperatorElement::elementName
                            || variableElementTypeName == SubscriptRowColumnOperatorElement::elementName
                            || variableElementTypeName == VariableElement::elementName
                        );
                    } else {
                        isValidConstruct = false;
                    }
                } else {
                    isValidConstruct = false;
                }
            }

            if (isValidConstruct                                                   &&
                (!hasSubscriptGrandparent                                     ||
                 parentTypeName == SubscriptIndexOperatorElement::elementName    )    ) {
                if (hasSubscriptGrandparent) {
                    result         = variableElement;
                    assignmentType = AssignmentType::TUPLE;
                } else if (parentTypeName == SubscriptRowColumnOperatorElement::elementName) {
                    result         = variableElement;
                    assignmentType = AssignmentType::MATRIX;
                } else {
                    result         = variableElement;
                    assignmentType = AssignmentType::MATRIX_OR_TUPLE;
                }
            } else {
                CppContext& context = engine.context();

                engine.translationErrorDetected(
                    new CppCodeGeneratorDiagnostic(
                        parent,
                        CppCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                        dynamic_cast<const CppTranslationPhase&>(engine.translationPhase()),
                        CppCodeGeneratorDiagnostic::Code::UNEXPECTED_OR_INCOMPATIBLE_OPERATOR,
                        QString(),
                        engine.contextPointer(),
                        CppCodeGeneratorDiagnostic::CompilerCode::NO_ERROR,
                        context.byteOffset(),
                        context.lineNumber(),
                        context.columnNumber()
                    )
                );
            }
        } else {
            CppContext& context = engine.context();

            engine.translationErrorDetected(
                new CppCodeGeneratorDiagnostic(
                    leftChild,
                    CppCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                    dynamic_cast<const CppTranslationPhase&>(engine.translationPhase()),
                    CppCodeGeneratorDiagnostic::Code::UNEXPECTED_OR_INCOMPATIBLE_OPERATOR,
                    QString(),
                    engine.contextPointer(),
                    CppCodeGeneratorDiagnostic::CompilerCode::NO_ERROR,
                    context.byteOffset(),
                    context.lineNumber(),
                    context.columnNumber()
                )
            );
        }

        return result;
    }


    bool CppAssignmentOperatorTranslator::identifyInferredVariableTypes(
            QSharedPointer<AssignmentOperatorElement>       assignmentElement,
            QSharedPointer<VariableElement>                 variableElement,
            CppCodeGenerationEngine&                        engine,
            CppAssignmentOperatorTranslator::AssignmentType assignmentType
        ) {
        bool           success = true;
        ElementPointer child1  = assignmentElement->child(1);

        if (!child1.isNull()) {
            success = engine.translateChild(child1);
            if (success) {
                QString text1 = variableElement->text(0);
                QString text2 = variableElement->text(1);

                if (!text1.isEmpty()) {
                    DataType::ValueType rightSideValueType = DataType::ValueType::NONE;

                    switch (assignmentType) {
                        case AssignmentType::SIMPLE: {
                            rightSideValueType = child1->valueType();
                            break;
                        }

                        case AssignmentType::MATRIX_OR_TUPLE: {
                            DataType::ValueType child1Type = child1->valueType();
                            rightSideValueType = subscriptedTypeConversion[static_cast<unsigned char>(child1Type)];

                            break;
                        }

                        case AssignmentType::MATRIX: {
                            rightSideValueType = DataType::matrixTypeFromBaseType(child1->valueType());
                            break;
                        }

                        case AssignmentType::TUPLE: {
                            rightSideValueType = DataType::ValueType::TUPLE;
                            break;
                        }

                        case AssignmentType::EXPLICIT: {
                            assert(false);
                            break;
                        }
                    }

                    if (rightSideValueType != DataType::ValueType::NONE) {
                        IdentifierContainer identifier = engine.identifier(text1, text2);
                        assert(identifier.isValid());

                        if (identifier.dataType().isInvalid()) {
                            identifier.setDataType(DataType::fromValueType(rightSideValueType));
                        } else {
                            DataType::ValueType currentValueType = identifier.dataType().valueType();

                            if (!identifier.typeExplicitlySet()) {
                                DataType::ValueType bestUpcast;

                                if (currentValueType == DataType::ValueType::TUPLE   ||
                                    rightSideValueType == DataType::ValueType::TUPLE    ) {
                                    // FIXME: Can't have tuples with multiple index values.
                                    bestUpcast = DataType::ValueType::TUPLE;
                                } else {
                                    bestUpcast = DataType::bestUpcast(currentValueType, rightSideValueType);
                                }

                                if (bestUpcast != DataType::ValueType::NONE) {
                                    identifier->setDataType(DataType::fromValueType(bestUpcast));
                                } else {
                                    CppContext& context = engine.context();

                                    engine.translationErrorDetected(
                                        new CppCodeGeneratorDiagnostic(
                                            assignmentElement,
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
                            } else if (rightSideValueType != DataType::ValueType::VARIANT) {
                                DataType::DynamicConversion dynamicConversion = DataType::canDynamicallyConvert(
                                    rightSideValueType,
                                    currentValueType
                                );

                                if (dynamicConversion == DataType::DynamicConversion::NEVER) {
                                    CppContext& context = engine.context();

                                    engine.translationErrorDetected(
                                        new CppCodeGeneratorDiagnostic(
                                            assignmentElement,
                                            CppCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                                            dynamic_cast<const CppTranslationPhase&>(engine.translationPhase()),
                                            CppCodeGeneratorDiagnostic::Code::CONFLICTING_DATA_TYPE_ASSIGNMENT,
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
                } else {
                    CppContext& context = engine.context();

                    engine.translationErrorDetected(
                        new CppCodeGeneratorDiagnostic(
                            variableElement,
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
        } else {
            CppContext& context = engine.context();
            engine.translationErrorDetected(
                new CppCodeGeneratorDiagnostic(
                    child1,
                    CppCodeGeneratorDiagnostic::Type::INTERNAL_ERROR,
                    dynamic_cast<const CppTranslationPhase&>(engine.translationPhase()),
                    CppCodeGeneratorDiagnostic::Code::MISSING_DATA_TYPE_TRANSLATOR,
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


    bool CppAssignmentOperatorTranslator::identifyInferredFunctionTypes(
            QSharedPointer<AssignmentOperatorElement> assignmentElement,
            QSharedPointer<FunctionElement>           functionElement,
            CppCodeGenerationEngine&                  engine
        ) {
        bool    success = true;
        QString text1   = functionElement->text(0);

        if (!text1.isEmpty()) {
            ElementPointer child1 = assignmentElement->child(1);
            if (!child1.isNull()) {
                IdentifierContainer functionIdentifier = functionElement->identifier();
                assert(functionIdentifier.isValid());

                ElementPointerList parameterElements = functionElement->parameterElements();

                engine.pushNewScope(functionElement);
                engine.setAsLValue();

                ElementPointerList::const_iterator parameterIterator    = parameterElements.constBegin();
                ElementPointerList::const_iterator parameterEndIterator = parameterElements.constEnd();
                while (success && parameterIterator != parameterEndIterator) {
                    success = engine.translateChild(*parameterIterator);
                    ++parameterIterator;
                }

                engine.setAsRValue();
                success = engine.translateChild(child1);
                engine.popCurrentScope();

                if (success) {
                    DataType::ValueType returnType = child1->valueType();
                    DataType dataType = DataType::fromValueType(returnType);

                    functionIdentifier->setDataType(dataType);
                }
            } else {
                CppContext& context = engine.context();

                engine.translationErrorDetected(
                    new CppCodeGeneratorDiagnostic(
                        assignmentElement,
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

        } else {
            CppContext& context = engine.context();

            engine.translationErrorDetected(
                new CppCodeGeneratorDiagnostic(
                    functionElement,
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


    bool CppAssignmentOperatorTranslator::insertFunctionPrototype(
            QSharedPointer<FunctionElement> functionElement,
            CppCodeGenerationEngine&        engine,
            const QString&                  additionalFunctionPrefix,
            bool                            includeReturnType,
            bool                            includeParameterTypes,
            bool                            includeModelImplementation,
            bool                            includePerThread
        ) {
        IdentifierContainer functionIdentifier = functionElement->identifier();
        assert(functionIdentifier.isValid()); // The identifier should already exist.

        QString functionInternalName = functionIdentifier.internalName();

        CppContext& context = engine.context();
        context.startNewStatement();

        if (includeReturnType) {
            DataType functionReturnType = functionIdentifier.dataType();
            if (functionReturnType.isInvalid()) {
                functionReturnType = DataType::defaultDataType();
            }

            const CppDataTypeTranslator* translator = dynamic_cast<const CppDataTypeTranslator*>(
                functionReturnType.translator(CppCodeGenerator::codeGeneratorName)
            );
            assert(translator != nullptr);

            QString returnTypeString = translator->dataTypeName();
            context(functionElement) << QString("%1 ").arg(returnTypeString);
        }

        context(functionElement) << QString("%2%3(").arg(additionalFunctionPrefix, functionInternalName);

        engine.pushNewScope(functionElement);
        bool success = insertFunctionParameters(
            functionElement,
            engine,
            includeParameterTypes,
            includeModelImplementation,
            includePerThread
        );

        context(functionElement) << ")";

        return success;
    }


    bool CppAssignmentOperatorTranslator::insertFunctionParameters(
            QSharedPointer<FunctionElement> functionElement,
            CppCodeGenerationEngine&        engine,
            bool                            includeParameterTypes,
            bool                            includeModelImplementation,
            bool                            includePerThread
        ) {
        CppContext& context      = engine.context();
        bool        includeComma = false;

        if (includeModelImplementation) {
            if (includeParameterTypes) {
                context(functionElement) << "ModelImpl* ";
            }

            context(functionElement) << "modelImplementation";
            includeComma = true;
        }

        if (includePerThread) {
            if (includeComma) {
                context(functionElement) << ",";
            }

            if (includeParameterTypes) {
                context(functionElement) << "M::PerThread& ";
            }

            context(functionElement) << "pt";
            includeComma = true;
        }

        unsigned long numberParameters = functionElement->numberChildren();
        unsigned long parameterIndex   = 0;
        while (parameterIndex < numberParameters) {
            ElementPointer parameter = functionElement->child(parameterIndex);

            if (!parameter->isPlaceholder()) {
                AssignmentType      assignmentType;
                DataType::ValueType valueType;
                ElementPointer      variable  = parseLeftChild(
                    parameter,
                    engine,
                    assignmentType,
                    valueType
                );

                if (!variable.isNull()) {
                    QString             variableText1       = variable->text(0);
                    QString             variableText2       = variable->text(1);
                    IdentifierContainer parameterIdentifier = engine.identifier(
                        variableText1,
                        variableText2,
                        functionElement
                    );
                    assert(parameterIdentifier.isValid());

                    parameterIdentifier->setDefinedAs(Identifier::DefinedAs::FUNCTION_PARAMETER);

                    if (includeComma) {
                        context(functionElement) << ",";
                    }
                    includeComma = true;

                    if (includeParameterTypes) {
                        DataType parameterDataType = parameterIdentifier.dataType();
                        if (parameterDataType.isInvalid()) {
                            parameterDataType = DataType::defaultDataType();
                        }

                        const CppDataTypeTranslator* parameterTranslator = dynamic_cast<const CppDataTypeTranslator*>(
                            parameterDataType.translator(CppCodeGenerator::codeGeneratorName)
                        );
                        assert(parameterTranslator != nullptr);

                        QString parameterTypeString = parameterTranslator->dataTypeName();
                        context(functionElement) << QString("%1 ").arg(parameterTypeString);
                    }

                    QString parameterName = parameterIdentifier.internalName();
                    context(functionElement) << parameterName;
                }
            }

            ++parameterIndex;
        }

        return true;
    }
}
