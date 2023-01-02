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
* This header defines the \ref Ld::CppForAllInOperatorTranslator class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CPP_FOR_ALL_IN_OPERATOR_TRANSLATOR_H
#define LD_CPP_FOR_ALL_IN_OPERATOR_TRANSLATOR_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>

#include "ld_common.h"
#include "ld_data_type.h"
#include "ld_element_structures.h"
#include "ld_cpp_translator.h"

namespace Ld {
    class Range2Element;
    class Range3Element;
    class VariableElement;
    class CppCodeGenerationEngine;

    /**
     * C++ translator for the \ref Ld::ForAllInOperatorElement class.
     */
    class LD_PUBLIC_API CppForAllInOperatorTranslator:public CppTranslator {
        Q_DECLARE_TR_FUNCTIONS(Ld::CppForAllInOperatorTranslator)

        public:
            ~CppForAllInOperatorTranslator() override;

            /**
             * Method that returns the name of the element this translator operates on.
             *
             * \return Returns the name of the element this translator operates on.
             */
            QString elementName() const override;

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
             * the default implementation will traverse every child of each element in depth first left-to-right order.
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
             * Method that is called during translation to insert the actual implementation.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.  The default implementation always returns true.
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

        private:
            /**
             * Method that obtains the index to hold the iterable's values.
             *
             * \param[in] element Pointer to the for-all element.
             *
             * \return Returns the iterable.
             */
            static ElementPointer indexElement(ElementPointer element);

            /**
             * Method that obtains the iterable that should be used to construct the for-all operation.
             *
             * \param[in] element Pointer to the for-all element.
             *
             * \return Returns the iterable.
             */
            static ElementPointer iterableElement(ElementPointer element);

            /**
             * Method that obtains the operation element that should be executed for each iterable value.
             *
             * \param[in] element Pointer to the for-all element.
             *
             * \return Returns the iterable.
             */
            static ElementPointer operationElement(ElementPointer element);

            /**
             * Method that identifies the best type to use for the contents of an iterable.
             *
             * \param[in] iterable Pointer to the iterable type.
             *
             * \return Returns the best type to use to represent the contents of the iterable.
             */
            static DataType::ValueType iterableContentsType(ElementPointer iterable);

            /**
             * Method that is called to create the implementation for a range iterator with an implied increment of
             * 1.
             *
             * \param[in]     index            The index variable.
             *
             * \param[in]     iterable         The iterable to perform the iteration over.
             *
             * \param[in]     operation        The element containing the operation to be performed.
             *
             * \param[in]     element          The element we are translating.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            bool range2IteratorThreadImplementation(
                QSharedPointer<VariableElement> index,
                QSharedPointer<Range2Element>   iterable,
                ElementPointer                  operation,
                ElementPointer                  element,
                CppCodeGenerationEngine&        generationEngine
            );

            /**
             * Method that is called to create the implementation for a range iterator with three terms.
             *
             * \param[in]     index            The index variable.
             *
             * \param[in]     iterable         The iterable to perform the iteration over.
             *
             * \param[in]     operation        The element containing the operation to be performed.
             *
             * \param[in]     element          The element we are translating.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            bool range3IteratorThreadImplementation(
                QSharedPointer<VariableElement> index,
                QSharedPointer<Range3Element>   iterable,
                ElementPointer                  operation,
                ElementPointer                  element,
                CppCodeGenerationEngine&        generationEngine
            );

            /**
             * Method that is called to create the implementation for a variable based tterator.
             *
             * \param[in]     index            The index variable.
             *
             * \param[in]     iterable         The iterable to perform the iteration over.
             *
             * \param[in]     operation        The element containing the operation to be performed.
             *
             * \param[in]     element          The element we are translating.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            bool variableIteratorThreadImplementation(
                QSharedPointer<VariableElement> index,
                QSharedPointer<VariableElement> iterable,
                ElementPointer                  operation,
                ElementPointer                  element,
                CppCodeGenerationEngine&        generationEngine
            );

            /**
             * Method that is called to create the implementation for matrix based tterator.
             *
             * \param[in]     index            The index variable.
             *
             * \param[in]     iterable         The iterable to perform the iteration over.
             *
             * \param[in]     operation        The element containing the operation to be performed.
             *
             * \param[in]     element          The element we are translating.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            bool matrixIteratorThreadImplementation(
                QSharedPointer<VariableElement> index,
                ElementPointer                  iterable,
                ElementPointer                  operation,
                ElementPointer                  element,
                CppCodeGenerationEngine&        generationEngine
            );

            /**
             * Method that is called to create the implementation for an arbitrary operator based tterator.
             *
             * \param[in]     index            The index variable.
             *
             * \param[in]     iterable         The iterable to perform the iteration over.
             *
             * \param[in]     operation        The element containing the operation to be performed.
             *
             * \param[in]     element          The element we are translating.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            bool operatorIteratorThreadImplementation(
                QSharedPointer<VariableElement> index,
                ElementPointer                  iterable,
                ElementPointer                  operation,
                ElementPointer                  element,
                CppCodeGenerationEngine&        generationEngine
            );
    };
};

#endif
