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
* This header defines the \ref Ld::Translator class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_TRANSLATOR_H
#define LD_TRANSLATOR_H

#include <QString>
#include <QSharedPointer>
#include <QByteArray>

#include <functional>

#include "ld_common.h"
#include "ld_element_structures.h"

namespace Ld {
    class TranslationPhase;
    class CodeGenerationEngine;

    /**
     * Pure virtual base class used to describe a phase of a translation performed by a \ref Ld::CodeGenerator class
     * instance.
     */
    class LD_PUBLIC_API Translator {
        public:
            virtual ~Translator();

            /**
             * Pure virtual method that returns the name of the element this translator operates on.
             *
             * \return Returns the name of the element this translator operates on.
             */
            virtual QString elementName() const = 0;

            /**
             * Pure virtual method that calls the correct translation method for the required translation.
             *
             * \param[in]     element              The element to be translated.
             *
             * \param[in,out] codeGenerationEngine The code generation engine to be invoked for this translation.
             *
             * \return Returns true on success, returns false on error.
             */
            virtual bool translate(ElementPointer element, CodeGenerationEngine& codeGenerationEngine) = 0;

            /**
             * Cast operator
             *
             * \return Returns the name of the translator.
             */
            operator QString() const;

        protected:
            /**
             * Method you can call from a translator to translate every child in order.
             *
             * \param[in]     element              A pointer to the element to be translated.
             *
             * \param[in,out] codeGenerationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            bool translateAllChildren(ElementPointer element, CodeGenerationEngine& codeGenerationEngine);
    };
};

#endif
