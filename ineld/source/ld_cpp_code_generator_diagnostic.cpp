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
* This file implements the \ref Ld::CppCodeGeneratorDiagnostic class.
***********************************************************************************************************************/

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QSharedPointer>

#include <cbe_cpp_compiler_error_codes.h>

#include "ld_element_structures.h"
#include "ld_element.h"
#include "ld_assignment_operator_element.h"
#include "ld_compound_statement_operator_element.h"
#include "ld_subscript_row_column_operator_element.h"
#include "ld_subscript_index_operator_element.h"
#include "ld_function_element.h"
#include "ld_translation_phase.h"
#include "ld_cpp_translation_phase.h"
#include "ld_cpp_context.h"
#include "ld_code_generator.h"
#include "ld_cpp_code_generator_diagnostic_private.h"
#include "ld_diagnostic.h"
#include "ld_cpp_code_generator_diagnostic.h"

namespace Ld {
    static const CppTranslationPhase  defaultTranslationPhase;
    static QSharedPointer<CppContext> defaultContext;

    CppCodeGeneratorDiagnostic::CppCodeGeneratorDiagnostic() {}


    CppCodeGeneratorDiagnostic::CppCodeGeneratorDiagnostic(
            ElementPointer                           element,
            Type                                     diagnosticType,
            const CppTranslationPhase&               translationPhase,
            CppCodeGeneratorDiagnostic::Code         diagnosticCode,
            const QString&                           backendMessage,
            QSharedPointer<CppContext>               context,
            CppCodeGeneratorDiagnostic::CompilerCode compilerDiagnosticCode,
            unsigned long                            byteOffset,
            unsigned long                            lineNumber,
            unsigned long                            columnNumber
        ):Diagnostic(
            element,
            QString("CppCodeGenerator"),
            diagnosticType
        ),impl(new CppCodeGeneratorDiagnostic::Private(
            translationPhase,
            diagnosticCode,
            backendMessage,
            context,
            compilerDiagnosticCode,
            byteOffset,
            lineNumber,
            columnNumber
        )) {}


    CppCodeGeneratorDiagnostic::CppCodeGeneratorDiagnostic(
            const CppCodeGeneratorDiagnostic& other
        ):Diagnostic(
            other
        ) {
        impl = other.impl;
    }


    CppCodeGeneratorDiagnostic::~CppCodeGeneratorDiagnostic() {}


    QString CppCodeGeneratorDiagnostic::diagnosticMessage() const {
        QString message;
        if (impl.isNull()) {
            message = tr("No diagnostic.");
        } else {
            QString description;

            switch (diagnosticCode()) {
                case Code::NO_DIAGNOSTIC: {
                    description = tr("No diagnostic message.");
                    break;
                }

                case Code::MISSING_PARAMETER_OR_FIELD: {
                    description = tr("Missing parameter or field.");
                    break;
                }

                case Code::COMPILER_DIAGNOSTIC: {
                    description = compilerDiagnosticMessage();
                    break;
                }

                case Code::LINKER_DIAGNOSTIC: {
                    description = tr("Link error.");
                    break;
                }

                case Code::UNEXPECTED_TRANSLATION_PHASE: {
                    description = tr("Unexpected translation phase.  Please report this error to Inesonic, LLC.");
                    break;
                }

                case Code::MISSING_TRANSLATOR: {
                    description = tr("Missing translator for \"%1\".  Please report this error to Inesonic, LLC.")
                                  .arg(element()->typeName());
                    break;
                }

                case Code::MISSING_DATA_TYPE_TRANSLATOR: {
                    description = tr("Missing data type translator for \"%1\".  "
                                     "Please report this error to Inesonic, LLC."
                                  ).arg(element()->typeName());
                    break;
                }

                case Code::MISSING_DATA_TYPE_SET_OPERATION_TRANSLATOR: {
                    description = tr("Missing set operation translator data type \"%1\".  "
                                     "Please report this error to Inesonic, LLC."
                                  ).arg(element()->typeName());
                    break;
                }

                case Code::CONFLICTING_DATA_TYPE_ASSIGNMENT: {
                    description = tr("Variable has already been assigned a different data type.");
                    break;
                }

                case Code::INVALID_TYPE_ASSIGNMENT: {
                    description = tr("You can not assign a data type to an expression of this type.");
                    break;
                }

                case Code::INCOMPATIBLE_TYPE_IN_ASSIGNMENT: {
                    description = tr(
                        "Data type on right side of assignment is incompatible with other types already assigned to "
                        "this variable."
                    );
                    break;
                }

                case Code::MALFORMED_LITERAL_VALUE: {
                    description = tr("Literal value is malformed.");
                    break;
                }

                case Code::INVALID_VARIABLE_OR_FUNCTION_NAME: {
                    description = tr("Invalid variable or function name.");
                    break;
                }

                case Code::EXPECTED_VARIABLE: {
                    description = tr("Expected a variable.");
                    break;
                }

                case Code::UNDEFINED_VARIABLE: {
                    description = tr("Undefined variable.");
                    break;
                }

                case Code::UNEXPECTED_OR_INCOMPATIBLE_OPERATOR: {
                    description = tr("Operator incompatible with operation.");
                    break;
                }

                case Code::FUNCTION_ALREADY_DEFINED: {
                    description = tr("Function already defined.");
                    break;
                }

                case Code::DIVISION_BY_BOOLEAN_IS_UNSAFE: {
                    description = tr("Division by boolean value is unsafe.");
                    break;
                }

                case Code::DIVISION_BY_MATRIX_NOT_SUPPORTED: {
                    description = tr("Can not divide by a matrix.");
                    break;
                }

                case Code::CAN_ONLY_SUBSCRIPT_MATRICES: {
                    description = tr("Only matrices can be subscripted.");
                    break;
                }

                case Code::CAN_ONLY_SUBSCRIPT_TUPLES_AND_MATRICES: {
                    description = tr("Only tuples and matrices can be subscripted.");
                    break;
                }

                case Code::BOOLEAN_SUBSCRIPTS_NOT_SUPPORTED: {
                    description = tr("Boolean subscripts are not supported.");
                    break;
                }

                case Code::EMPTY_ITERABLE: {
                    description = tr("Set, tuple, or matrix can not be empty when used here.");
                    break;
                }

                default: {
                    description = tr("Undefined error %1.").arg(static_cast<unsigned>(diagnosticCode()));
                    break;
                }
            }

            message = tr("%1: %2").arg(diagnosticTypeMessage(), description);
        }

        return message;
    }


    const CppTranslationPhase& CppCodeGeneratorDiagnostic::translationPhase() const {
        return impl.isNull() ? defaultTranslationPhase : impl->translationPhase();
    }


    CppCodeGeneratorDiagnostic::Code CppCodeGeneratorDiagnostic::diagnosticCode() const {
        return impl.isNull() ? Code::NO_DIAGNOSTIC : impl->diagnosticCode();
    }


    QSharedPointer<CppContext> CppCodeGeneratorDiagnostic::context() const {
        return impl.isNull() ? defaultContext : impl->context();
    }


    unsigned long CppCodeGeneratorDiagnostic::byteOffset() const {
        return impl.isNull() ? invalidByteOffset : impl->byteOffset();
    }


    unsigned long CppCodeGeneratorDiagnostic::lineNumber() const {
        return impl.isNull() ? invalidLineNumber : impl->lineNumber();
    }


    unsigned long CppCodeGeneratorDiagnostic::columnNumber() const {
        return impl.isNull() ? invalidColumnNumber : impl->columnNumber();
    }


    CppCodeGeneratorDiagnostic::CompilerCode CppCodeGeneratorDiagnostic::compilerCode() const {
        return impl.isNull() ? CppCodeGeneratorDiagnostic::CompilerCode::NO_ERROR : impl->compilerCode();
    }


    QString CppCodeGeneratorDiagnostic::backendMessage() const {
        return impl.isNull() ? QString() : impl->backendMessage();
    }


    CppCodeGeneratorDiagnostic& CppCodeGeneratorDiagnostic::operator=(const CppCodeGeneratorDiagnostic& other) {
        Diagnostic::operator=(other);
        impl = other.impl;

        return *this;
    }


    QString CppCodeGeneratorDiagnostic::compilerDiagnosticMessage() const {
        QString message = tr("Undefined compile error %1").arg(static_cast<unsigned>(compilerCode()));

        switch (impl->compilerCode()) {
            case CppCodeGeneratorDiagnostic::CompilerCode::NO_ERROR: {
                message = tr("No compiler error.");
                break;
            }

            case CppCodeGeneratorDiagnostic::CompilerCode::ERROR_NO_MEMBER: {
                ElementPointer element = CppCodeGeneratorDiagnostic::element();
                if (!element.isNull()) {
                    ElementPointer parent   = element->parent();
                    QString        typeName = parent->typeName();

                    if (typeName == Ld::SubscriptIndexOperatorElement::elementName     ||
                        typeName == Ld::SubscriptRowColumnOperatorElement::elementName    ) {
                        message = tr("Subscript is not supported.");
                    } else {
                        message = tr("Unsupported function or operator");
                    }
                } else {
                    message = tr("Unsupported function or operator");
                }

                break;
            }

            case CppCodeGeneratorDiagnostic::CompilerCode::ERROR_FE_NOT_A_PCH_FILE:
            case CppCodeGeneratorDiagnostic::CompilerCode::ERROR_FE_PCH_FILE_OVERRIDDEN:
            case CppCodeGeneratorDiagnostic::CompilerCode::ERROR_FE_PCH_MALFORMED:
            case CppCodeGeneratorDiagnostic::CompilerCode::ERROR_FE_PCH_MALFORMED_BLOCK: {
                message = tr("Malformed or corrupted AST file (%1).").arg(static_cast<unsigned>(impl->compilerCode()));
                break;
            }

            case CppCodeGeneratorDiagnostic::CompilerCode::ERROR_EXPECTED_EXPRESSION: {
                message = tr("Expected expression.");
                break;
            }

            case CppCodeGeneratorDiagnostic::CompilerCode::ERROR_TYPECHECK_INVALID_OPERANDS: {
                message = tr("Incompatible types for operation.");
                break;
            }

            case CppCodeGeneratorDiagnostic::CompilerCode::ERROR_TYPECHECK_NONVIABLE_CONDITION: {
                message = tr("Incompatible parameter type.");
                break;
            }

            case CppCodeGeneratorDiagnostic::CompilerCode::ERROR_OVL_NO_VIABLE_FUNCTION_IN_CALL: {
                ElementPointer element = CppCodeGeneratorDiagnostic::element();
                if (!element.isNull()) {
                    QString typeName = element->typeName();

                    if (typeName == Ld::AssignmentOperatorElement::elementName) {
                        ElementPointer child0 = element->child(0);
                        ElementPointer child1 = element->child(1);

                        if (!child0.isNull() && !child1.isNull()) {
                            if (child0->typeName() != Ld::FunctionElement::elementName) {
                                if (child1->typeName() == Ld::CompoundStatementOperatorElement::elementName) {
                                    message = tr(
                                        "Compound statement missing a therefore operator or the type of the value "
                                        "provided in the therefore operator is not compatible."
                                    );
                                } else {
                                    message = tr("The right hand value of the assignment is not compatible.");
                                }
                            } else {
                                message = tr("Invalid function definition.");
                            }
                        }
                    } else {
                        message = tr("Incompatible type for value.");
                    }
                }

                break;
            }

            case CppCodeGeneratorDiagnostic::CompilerCode::ERROR_OVL_AMBIGUOUS_CALL:
            case CppCodeGeneratorDiagnostic::CompilerCode::ERROR_OVL_AMBIGUOUS_MEMBER_CALL:
            case CppCodeGeneratorDiagnostic::CompilerCode::ERROR_OVL_AMBIGUOUS_OBJECT_CALL: {
                message = tr("Can not resolve function by parameters.");
                break;
            }

            case CppCodeGeneratorDiagnostic::CompilerCode::ERROR_OVL_AMBIGUOUS_CONVERSION_IN_CAST: {
                message = tr("Can not determine correct type conversion at compile type.");
                break;
            }

            case CppCodeGeneratorDiagnostic::CompilerCode::ERROR_OVL_AMBIGUOUS_INIT: {
                message = tr("Can not determine correct type during instantiation.");
                break;
            }

            case CppCodeGeneratorDiagnostic::CompilerCode::ERROR_OVL_AMBIGUOUS_OPER_BINARY:
            case CppCodeGeneratorDiagnostic::CompilerCode::ERROR_OVL_AMBIGUOUS_OPER_UNARY: {
                message = tr("Can not resolve operator by parameters.");
                break;
            }

            case CppCodeGeneratorDiagnostic::CompilerCode::EXT_INIT_LIST_VARIABLE_NARROWING: {
                message = tr("Unexpected variable type narrowing.");
                break;
            }

            case CppCodeGeneratorDiagnostic::CompilerCode::ERROR_UNDECLARED_VAR_USE: {
                message = tr("Undefined function.");
                break;
            }

            case CppCodeGeneratorDiagnostic::CompilerCode::ERROR_TYPECHECK_CONVERT_INCOMPATIBLE: {
                message = tr("Incompatible assignment.");
                break;
            }

            case CppCodeGeneratorDiagnostic::CompilerCode::ERROR_OVL_NO_VIABLE_MEMBER_FUNCTION_IN_CALL: {
                message = tr("Invalid type for subscript.");
                break;
            }

            default: {
                break;
            }
        }

        return message;
    }
}
