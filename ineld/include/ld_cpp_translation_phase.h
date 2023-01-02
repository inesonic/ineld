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
* This header defines the \ref Ld::CppTranslationPhase class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CPP_TRANSLATION_PHASE_H
#define LD_CPP_TRANSLATION_PHASE_H

#include <QString>

#include "ld_common.h"
#include "ld_translation_phase.h"

namespace Ld {
    /**
     * Class that manages C++ code translation phases.
     */
    class LD_PUBLIC_API CppTranslationPhase:public TranslationPhase {
        public:
            /**
             * Enumeration of the translation phases.
             */
            enum class Phase:unsigned {
                /**
                 * Phase used to identify necessary headers or PCH files.  Also used to identify explicitly declared
                 * data types.
                 */
                IDENTIFY_DEPENDENCIES_AND_EXPLICIT_TYPES = 0,

                /**
                 * Phase used to determine types of variables that must be inferred by context.
                 */
                IDENTIFY_INFERRED_TYPES = 1,

                /**
                 * Phase used to insert additional declarations not included in headers.
                 */
                DECLARATIONS = 2,

                /**
                 * Phase used to insert the model class declaration.
                 */
                MODEL_CLASS_DECLARATION = 3,

                /**
                 * Phase used to insert additional model class method declarations.
                 */
                METHOD_DECLARATIONS = 4,

                /**
                 * Phase used to insert module class scope variables.
                 */
                CLASS_SCOPE_VARIABLES = 5,

                /**
                 * Phase used to insert the end of the model class declaration.
                 */
                END_MODEL_CLASS_DECLARATION = 6,

                /**
                 * Phase used to insert a model's thread definition.
                 */
                THREAD_DEFINITION = 7,

                /**
                 * Phase used to insert additional local variables.
                 */
                THREAD_LOCALS = 8,

                /**
                 * Phase used to insert the model implementation code.
                 */
                THREAD_IMPLEMENTATION = 9,

                /**
                 * Phase used to perform any final, under-the-hood, post-processing for the model.
                 */
                THREAD_POST_PROCESSING = 10,

                /**
                 * Phased used to perform any final clean-up such as deallocating memory.
                 */
                THREAD_CLEANUP = 11,

                /**
                 * Phase used to insert the end of the thread.
                 */
                THREAD_END = 12,

                /**
                 * Phase used to insert method definitions.
                 */
                METHOD_DEFINITIONS = 13,

                /**
                 * Phase used to insert the model's book-keeping methods.
                 */
                BOOKKEEPING_DEFINITION = 14,

                /**
                 * Phase used to insert the model allocator function.
                 */
                MODEL_ALLOCATOR = 15,

                /**
                 * Compilation phase.
                 */
                CONVERT_IR_TO_OBJECT = 16,

                /**
                 * Link phase.
                 */
                LINK = 17
            };

            /**
             * Constructor
             *
             * \param[in] generateDynamicLibrary If true, the CPP code generator will run all available phases.  If
             *                                   false, the last translation phase will be skipped.
             */
            CppTranslationPhase(bool generateDynamicLibrary = true);

            ~CppTranslationPhase() override;

            /**
             * Method that returns the phase name for a specific phase.
             *
             * \param[in] number The phase number to get the name of.
             *
             * \return Returns the class name for this translation phase.
             */
            QString phaseName(unsigned number) const final;

            /**
             * Pure virtual method you can call to determine now the current phase should manage per-element
             * translation.
             *
             * \return Returns a value indicating how per-element translation should be performed.
             */
            TranslationPhase::TranslationMode translationMode(unsigned number) const final;

            /**
             * Pure virtual method that returns the number of translation phases.
             *
             * \return Returns the number of translation phases.
             */
            unsigned numberPhases() const final;

            /**
             * Method that returns the enumerated value of the phase.
             *
             * \return Returns the phase as a enumerated value.
             */
            Phase phase() const;

            /**
             * Method that indicates the number of threads to be built.
             *
             * \return Returns the number of threads required for this model.  This method currently always returns 1.
             */
            unsigned numberThreads() const;

            /**
             * Method that returns the current thread ID.
             *
             * \return Returns the zero based thread ID.  This method currently always returnes 0.
             */
            unsigned threadId() const;

        private:
            /**
             * The number of translation phases.
             */
            unsigned currentNumberPhases;
    };
};

#endif
