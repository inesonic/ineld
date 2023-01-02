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
* This file implements the \ref Ld::CppElementOfSetOperatorTranslator class.
***********************************************************************************************************************/

#include <QString>

#include <cassert>

#include "ld_element_of_set_operator_element.h"
#include "ld_element_structures.h"
#include "ld_capabilities.h"
#include "ld_assignment_operator_element.h"
#include "ld_root_element.h"
#include "ld_function_element.h"
#include "ld_variable_element.h"
#include "ld_subscript_index_operator_element.h"
#include "ld_subscript_row_column_operator_element.h"
#include "ld_identifier.h"
#include "ld_identifier_container.h"
#include "ld_identifier_database.h"
#include "ld_data_type.h"
#include "ld_cpp_code_generator.h"
#include "ld_cpp_code_generation_engine.h"
#include "ld_cpp_code_generator_diagnostic.h"
#include "ld_cpp_data_type_translator.h"
#include "ld_cpp_context.h"
#include "ld_cpp_translator.h"
#include "ld_cpp_element_of_set_operator_translator.h"

namespace Ld {
    CppElementOfSetOperatorTranslator::~CppElementOfSetOperatorTranslator() {}


    QString CppElementOfSetOperatorTranslator::elementName() const {
        return Ld::ElementOfSetOperatorElement::elementName;
    }


    bool CppElementOfSetOperatorTranslator::identifyDependenciesAndExplicitTypes(
            ElementPointer           element,
            CppCodeGenerationEngine& engine
        ) {
        bool           success;
        ElementPointer parent  = element->parent();
        CppContext&    context = engine.context();

        if (!parent.isNull()) {
            unsigned long childIndex           = parent->indexOfChild(element);
            Capabilities  requiredCapabilities = parent->parentRequires(childIndex);

            if (requiredCapabilities.intersects(Ld::Capabilities::typeDeclaration)) {
                ElementPointer child0 = element->child(0);
                ElementPointer child1 = element->child(1);

                if (!child0.isNull() && !child1.isNull()) {
                    QString text1;
                    QString text2;
                    bool    matrixOperator = false;

                    QString typeName = child0->typeName();
                    ElementPointer variableElement;
                    if (typeName == Ld::VariableElement::elementName) {
                        text1            = child0->text(0);
                        text2            = child0->text(1);
                        matrixOperator   = false;
                        variableElement  = child0;
                    } else if (typeName == Ld::SubscriptIndexOperatorElement::elementName     ||
                               typeName == Ld::SubscriptRowColumnOperatorElement::elementName    ) {
                        ElementPointer subscriptElement = child0->child(0);
                        if (subscriptElement->typeName() == Ld::VariableElement::elementName) {
                            text1            = subscriptElement->text(0);
                            text2            = subscriptElement->text(1);
                            matrixOperator   = true;
                            variableElement  = subscriptElement;
                        }
                    }

                    if (!variableElement.isNull()) {
                        if (!text1.isEmpty()) {
                            IdentifierContainer container = engine.identifier(text1, text2);
                            if (container.isInvalid()) {
                                ElementPointer currentScope = engine.currentScope();
                                Identifier::DefinedAs definedAs;
                                if (currentScope->typeName() == Ld::RootElement::elementName) {
                                    definedAs = Identifier::DefinedAs::GLOBAL_SCOPE_VARIABLE;
                                } else if (currentScope->typeName() == Ld::FunctionElement::elementName) {
                                    definedAs = Identifier::DefinedAs::FUNCTION_PARAMETER;
                                } else {
                                    definedAs = Identifier::DefinedAs::LOCAL_SCOPE_VARIABLE;
                                }

                                container = new Identifier(text1, text2, definedAs, currentScope);
                                engine.addIdentifier(container);
                            }

                            DataType newDataType = DataType::fromSymbol(child1, matrixOperator);
                            if (newDataType.isValid()) {
                                if (container.dataType().isInvalid()) {
                                    container.setDataType(newDataType);
                                    container.setTypeExplicitlySet();
                                    container.insertElementData(ElementValueData(variableElement));

                                    success = true;
                                } else if (container.dataType() != newDataType) {
                                    if (container.typeExplicitlySet()) {
                                        engine.translationErrorDetected(
                                            new CppCodeGeneratorDiagnostic(
                                                element,
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
                                    } else {
                                        container.setDataType(newDataType);
                                        container.setTypeExplicitlySet();
                                        container.insertElementData(ElementValueData(variableElement));

                                        success = true;
                                    }
                                } else {
                                    container.setTypeExplicitlySet();
                                    success = true;
                                }
                            } else {
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
                    } else {
                        engine.translationErrorDetected(
                            new CppCodeGeneratorDiagnostic(
                                element,
                                CppCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                                dynamic_cast<const CppTranslationPhase&>(engine.translationPhase()),
                                CppCodeGeneratorDiagnostic::Code::INVALID_TYPE_ASSIGNMENT,
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
            } else {
                success = true;
            }
        } else {
            success = false;
        }

        return success;
    }


    bool CppElementOfSetOperatorTranslator::threadImplementation(
            ElementPointer           element,
            CppCodeGenerationEngine& engine
        ) {
        bool           success;
        ElementPointer parent  = element->parent();
        CppContext&    context = engine.context();

        if (!parent.isNull()) {
            unsigned long childIndex           = parent->indexOfChild(element);
            Capabilities  requiredCapabilities = parent->parentRequires(childIndex);

            if (!requiredCapabilities.intersects(Ld::Capabilities::expressions)) {
                ElementPointer child0 = element->child(0);
                ElementPointer child1 = element->child(1);

                if (!child0.isNull() && !child1.isNull()) {
                    context(element) << "M::isElementOfSet((";
                    engine.translateChild(child0);
                    context(element) << "),(";
                    engine.translateChild(child1);
                    context(element) << "))";

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
            } else {
                success = true;
            }
        } else {
            success = false;
        }

        return success;
    }


    bool CppElementOfSetOperatorTranslator::methodDefinitions(
            ElementPointer           element,
            CppCodeGenerationEngine& engine
        ) {
        return threadImplementation(element, engine);
    }
}
