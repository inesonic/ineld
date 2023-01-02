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
* This header defines the \ref Ld::TranslationPhase class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_TRANSLATION_PHASE_H
#define LD_TRANSLATION_PHASE_H

#include <QString>
#include <QMap>
#include <QByteArray>
#include <QSharedPointer>

#include "ld_common.h"

namespace Ld {
    /**
     * Base class used to describe a phase of a translation performed by a \ref Ld::CodeGenerator class instance.
     */
    class LD_PUBLIC_API TranslationPhase {
        public:
            /**
             * Enumeration used to indicate how the per-element translators should be handled.
             */
            enum class TranslationMode {
                /**
                 * Indicates that no per-element translation is required.  All translation can be performed by just the
                 * code generation engine.
                 */
                NO_PER_ELEMENT_TRANSLATION = 0,

                /**
                 * Indicates that per-element translation must honor the element heirarchy.  Each element is responsible
                 * for triggering the translation of each child element.
                 */
                HONOR_HEIRARCHY,

                /**
                 * Indicates that per-element translation can occur in arbitrary order.  The code generator will call
                 * the translation methods for each element.
                 */
                IGNORE_HEIRARCHY

            };

            TranslationPhase();

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            TranslationPhase(const TranslationPhase& other);

            virtual ~TranslationPhase();

            /**
             * Method that returns the phase name for this translation phase.
             *
             * \return Returns the class name for this translation phase.
             */
            QString currentPhaseName() const;

            /**
             * Pure virtual method that returns the phase name for a specific phase.
             *
             * \param[in] number The phase number to get the name of.
             *
             * \return Returns the class name for this translation phase.  The default implementation returns an
             *         empty string.
             */
            virtual QString phaseName(unsigned number) const;

            /**
             * Method you can call to determine now the current phase should manage per-element translation.
             *
             * \return Returns a value indicating how per-element translation should be performed.
             */
            TranslationMode currentTranslationMode() const;

            /**
             * Pure virtual method you can call to determine how the current phase should manage per-element
             * translation.
             *
             * \return Returns a value indicating how per-element translation should be performed.  The default
             *         implementation returns \ref Ld::TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION.
             */
            virtual TranslationMode translationMode(unsigned number) const;

            /**
             * Pure virtual method that returns the number of translation phases.
             *
             * \return Returns the number of translation phases.  The default implementation returns 0.
             */
            virtual unsigned numberPhases() const;

            /**
             * Method you can call to get the current phase number.
             *
             * \return Returns the current phase number.
             */
            unsigned phaseNumber() const;

            /**
             * Method you can call to determine if this is the last phase.
             *
             * \return Returns true if this is the last translation phase.  Returns false if there are additional phases
             *         to be performed.
             */
            bool lastPhase() const;

            /**
             * Method that resets the translation phase to the first phase.
             */
            void reset();

            /**
             * Method that advances the translation phase to the next phase.  The method will now allow you to advance
             * past the last translation phase.
             */
            void nextPhase();

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this object.
             */
            TranslationPhase& operator=(const TranslationPhase& other);

            /**
             * Method that can be used to determine if two output types are identical.  Comparison is made based on the
             * output type identifier.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the output types are identical.  Returns false if the output types are different.
             */
            bool operator==(const TranslationPhase& other) const;

            /**
             * Method that can be used to determine if two output types are different.  Comparison is made based on the
             * output type identifier.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the output types are different.  Returns false if the output types are identical.
             */
            bool operator!=(const TranslationPhase& other) const;

            /**
             * Cast operator.
             *
             * \return Returns the phase number for this translation phase.
             */
            operator unsigned() const;

        private:
            unsigned currentPhaseNumber;
    };
};

#endif
