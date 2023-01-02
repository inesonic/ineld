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
* This header defines the \ref Ld::CppTranslator class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CPP_TRANSLATOR_H
#define LD_CPP_TRANSLATOR_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QByteArray>
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_translator.h"

namespace Ld {
    class CppCodeGenerationEngine;

    /**
     * Pure virtual base class for per-element translators that convert the element in the syntax tree to a C++
     * representation.
     */
    class LD_PUBLIC_API CppTranslator:public Translator {
        Q_DECLARE_TR_FUNCTIONS(Ld::CppTranslator)

        public:
            ~CppTranslator() override;

            /**
             * Method that calls the correct translation method for the required translation.
             *
             * \param[in]     element              The element to be translated.
             *
             * \param[in,out] codeGenerationEngine The code generation engine to be invoked for this translation.
             *
             * \return Returns true on success, returns false on error.
             */
            bool translate(ElementPointer element, CodeGenerationEngine& codeGenerationEngine) final;

            /**
             * Method that is called prior to actually performing any translation to identify any and all required
             * headers and required static or dynamic libraries.
             *
             * This method may also be used to identify any varables types that are explicitly declared.
             *
             * The default implementation will traverse every child of even element in depth first left-to-right order.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            virtual bool identifyDependenciesAndExplicitTypes(
                ElementPointer           element,
                CppCodeGenerationEngine& generationEngine
            );

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
            virtual bool identifyInferredTypes(ElementPointer element, CppCodeGenerationEngine& generationEngine);

            /**
             * Method that is called during translation to insert prototypes and declarations.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.  The default implementation always returns
             *         true.
             */
            virtual bool declarations(ElementPointer element, CppCodeGenerationEngine& generationEngine);

            /**
             * Method that is called during translation to insert method declarations.
             *
             * The default implementation will traverse every child of each element in depth first left-to-right order.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.  The default implementation always returns
             *         true.
             */
            virtual bool methodDeclarations(ElementPointer element, CppCodeGenerationEngine& generationEngine);

            /**
             * Method that is called during translation to insert the actual implementation.
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
            virtual bool threadImplementation(ElementPointer element, CppCodeGenerationEngine& generationEngine);

            /**
             * Method that is called during translation to insert any post-processing at the end of the thead.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.  The default implementation always returns
             *         true.
             */
            virtual bool threadPostProcessing(ElementPointer element, CppCodeGenerationEngine& generationEngine);

            /**
             * Method that is called during translation to insert any required clean-up code at the end of the thread.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.  The default implementation always returns
             *         true.
             */
            virtual bool threadCleanup(ElementPointer element, CppCodeGenerationEngine& generationEngine);

            /**
             * Method that is called during translation to insert any required method definitions.
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
            virtual bool methodDefinitions(ElementPointer element, CppCodeGenerationEngine& generationEngine);
    };
};

#endif
