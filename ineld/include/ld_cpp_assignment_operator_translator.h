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
* This header defines the \ref Ld::CppAssignmentOperatorTranslator class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CPP_ASSIGNMENT_OPERATOR_TRANSLATOR_H
#define LD_CPP_ASSIGNMENT_OPERATOR_TRANSLATOR_H

#include "ld_common.h"
#include "ld_data_type.h"
#include "ld_cpp_binary_operator_translator_base.h"

namespace Ld {
    class FunctionDatabase;
    class AssignmentOperatorElement;
    class VariableElement;
    class FunctionElement;

    /**
     * C++ translator class for the assignment operator.
     */
    class LD_PUBLIC_API CppAssignmentOperatorTranslator:public CppBinaryOperatorTranslatorBase {
        public:
            ~CppAssignmentOperatorTranslator() override;

            /**
             * Method that returns the name of the element this translator operates on.
             *
             * \return Returns the name of the element this translator operates on.
             */
            QString elementName() const final;

            /**
             * Method that is called prior to actually performing any translation to identify any and all required
             * headers and required static or dynamic libraries.  This method may also be used to identify any
             * identifiers associated with this element.
             *
             * This specific version checks for function declarations and adjusts the current scope based on the left
             * hand argument.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.  The default implementation always returns true.
             */
            bool identifyDependenciesAndExplicitTypes(
                ElementPointer           element,
                CppCodeGenerationEngine& generationEngine
            ) override;

            /**
             * Method that is called during translation to identify the types of variables that must be inferred.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.  The default implementation always returns
             *         true.
             */
            bool identifyInferredTypes(ElementPointer element, CppCodeGenerationEngine& generationEngine) override;

            /**
             * Method that is called during translation to insert method declarations.
             *
             * If this assignment defines a function then this method will declare the function.  If this assignment
             * defines a variable, then this method will do nothing.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.  The default implementation always returns
             *         true.
             */
            bool methodDeclarations(ElementPointer element, CppCodeGenerationEngine& generationEngine) override;

            /**
             * Method that is called during translation to insert the actual implementation.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.  The default implementation always returns
             *         true.
             */
            bool threadImplementation(ElementPointer element, CppCodeGenerationEngine& generationEngine) override;

            /**
             * Method that is called during translation to insert any required method definitions.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.  The default implementation always returns true.
             */
            bool methodDefinitions(ElementPointer element, CppCodeGenerationEngine& generationEngine) override;

        protected:
            /**
             * Method that returns the C++ operator to be used.  This method is only called when the operator is being
             * used as a relational operator.
             *
             * \return Returns the C++ operator to present for this operation.
             */
            QString cppOperator() const override;

        private:
            /**
             * Enumeration of assignment types.
             */
            enum class AssignmentType {
                /**
                 * Indicates a traditional assignment.  Assignment is of the form \f$ a = x \f$.
                 */
                SIMPLE,

                /**
                 * Indicates a simple index index subscripted assignment.  Assignment is of the form \f$ a _ i = x \f$.
                 * Such assignments suggest a is an tuple or matrix depending on the value of x.
                 */
                MATRIX_OR_TUPLE,

                /**
                 * Indicates a simple row/column subscripted assignment.  Assignment is of the form
                 * \f$ a _ { i,j } = x \f$.  Such assignments suggest a is a matrix.
                 */
                MATRIX,

                /**
                 * Indicates a complex subscripted assignment.  Assignment is of one of the forms:
                 *
                 *     * \f$ \left a _ i \right ) _ j = x \f$
                 *     * \f$ \left a _ i \right ) _ { j,k } = x \f$
                 *
                 * Assignments of these styles indicate that a must be a tuple.
                 */
                TUPLE,

                /**
                 * Indicates one of two types of explicit assignments.  Assignment is one of the forms:
                 *
                 *     * \f$ a \in \mathbb{T} = x \f$, indicating a is explicitly of type T
                 *     * \f$ a _ i \in \mathbb{T} = x \f$, indicating a is a matrix of values of type T.
                 *     * \f$ a _ { i,j } \in \mathbb{T} = x \f$, indicating a is a matrix of values of type T.
                 */
                EXPLICIT
            };

            /**
             * Method that identifies the variable element that is ultimately being assigned to as well as basic
             * characteristics of the assignment that impact the resulting variable type.
             *
             * \param[in]     leftChild      The left child of the assignment.
             *
             * \param[in,out] engine         The generation engine driving the conversion.  This value is used to
             *                               report errors discovered by this method.
             *
             * \param[out]    assignmentType The type of assignment.
             *
             * \param[out]    valueType      The value type.  This value is only meaningful if the assignmentType is
             *                               explicit.
             *
             * \return Returns a shared pointer to the variable element that is ultimately being assigned to.  A
             *         null pointer is returned if an error is detected.
             */
            static ElementPointer parseLeftChild(
                ElementPointer           leftChild,
                CppCodeGenerationEngine& engine,
                AssignmentType&          assignmentType,
                DataType::ValueType&     valueType
            );

            /**
             * Method that determines type information during a variable assignment.
             *
             * \param[in]     assignmentElement A pointer to the assignment element to be translated.
             *
             * \param[in]     variableElement   The variable element found on the left side of the assignment.
             *
             * \param[in,out] engine            The generation engine driving the conversion.
             *
             * \param[in]     assignmentType    The assignment type.  This function will assert if the assignment type
             *                                  is Ld::CppAssignmentOperatorTranslator::AssignmentType::EXPLICIT.
             *
             * \return Returns true on success, returns false on error.
             */
            static bool identifyInferredVariableTypes(
                QSharedPointer<AssignmentOperatorElement> assignmentElement,
                QSharedPointer<VariableElement>           variableElement,
                CppCodeGenerationEngine&                  engine,
                AssignmentType                            assignmentType
            );

            /**
             * Method that determines type information during a function declaration assignment.
             *
             * \param[in]     assignmentElement  A pointer to the assignment element to be translated.
             *
             * \param[in]     functionElement    The variable element found on the left side of the assignment.
             *
             * \param[in,out] engine             The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            static bool identifyInferredFunctionTypes(
                QSharedPointer<AssignmentOperatorElement> assignmentElement,
                QSharedPointer<FunctionElement>           functionElement,
                CppCodeGenerationEngine&                  engine
            );

            /**
             * Method that inserts a function prototype into the context.  Scope will be adjusted for the function
             * parameters so you must manually pop the scope once on exit.
             *
             * \param[in] functionElement            The function to generate the prototype from.
             *
             * \param[in] engine                     The code generation engine driving the translation.
             *
             * \param[in] additionalFunctionPrefix   Prefix to be inserted in front of the function name.  This is
             *                                       used to instantiate wrapper functions.
             *
             * \param[in] includeReturnType          Include the function return type before the function name.
             *
             * \param[in] includeParameterTypes      Include the types for each parameter.
             *
             * \param[in] includeModelImplementation Include a pointer to the model implementation as the first
             *                                       parameter.
             *
             * \param[in] includePerThread           Include a reference to the per-thread instance as the second
             *                                       parameter.
             *
             * \return Returns true on success, returns false on error.
             */
            static bool insertFunctionPrototype(
                QSharedPointer<FunctionElement> functionElement,
                CppCodeGenerationEngine&        engine,
                const QString&                  additionalFunctionPrefix,
                bool                            includeReturnType,
                bool                            includeParameterTypes,
                bool                            includeModelImplementation,
                bool                            includePerThread
            );

            /**
             * Method you can use to insert a list of function parameters with or without type declarations for each
             * parameter.  This method does not adjust the current scope so you must manually set the scope before
             * calling this method.  Also note that this method will not include leading/trailing parens.
             *
             * \param[in] functionElement            The function to generate the prototype from.
             *
             * \param[in] engine                     The code generation engine driving the translation.
             *
             * \param[in] includeParameterTypes      Include the types for each parameter.
             *
             * \param[in] includeModelImplementation Include a pointer to the model implementation as the first
             *                                       parameter.
             *
             * \param[in] includePerThread           Include a reference to the per-thread instance as the second
             *                                       parameter.
             *
             * \return Returns true on success, returns false on error.
             */
            static bool insertFunctionParameters(
                QSharedPointer<FunctionElement> functionElement,
                CppCodeGenerationEngine&        engine,
                bool                            includeParameterTypes,
                bool                            includeModelImplementation,
                bool                            includePerThread
            );
    };
};

#endif
