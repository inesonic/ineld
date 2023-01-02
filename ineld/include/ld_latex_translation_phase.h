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
* This header defines the \ref Ld::LaTeXTranslationPhase class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_LATEX_TRANSLATION_PHASE_H
#define LD_LATEX_TRANSLATION_PHASE_H

#include <QString>

#include "ld_common.h"
#include "ld_translation_phase.h"

namespace Ld {
    /**
     * Class that manages LaTeX code translation phases.
     */
    class LD_PUBLIC_API LaTeXTranslationPhase:public TranslationPhase {
        public:
            /**
             * Enumeration of the translation phases.
             */
            enum class Phase:unsigned {
                /**
                 * Phase used to identify dependencies.
                 */
                IDENTIFY_DEPENDENCIES = 0,

                /**
                 * Phase used to add content to the preamble (excluding dependencies with external LaTeX packages).
                 */
                PREAMBLE = 1,

                /**
                 * Phase that inserts the document content.
                 */
                BODY = 2
            };

            ~LaTeXTranslationPhase() override;

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
    };
};

#endif
