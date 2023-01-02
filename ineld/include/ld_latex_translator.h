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
* This header defines the \ref Ld::LaTeXTranslator class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_LATEX_TRANSLATOR_H
#define LD_LATEX_TRANSLATOR_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QMap>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_special_characters.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_translator.h"

namespace Ld {
    class LaTeXCodeGenerationEngine;

    /**
     * Virtual base class for per-element translators that convert the element in the syntax tree to a LaTeX
     * representation.
     */
    class LD_PUBLIC_API LaTeXTranslator:public Translator {
        Q_DECLARE_TR_FUNCTIONS(Ld::LaTeXTranslator)

        public:
            ~LaTeXTranslator() override;

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
             * Method that is called during the \ref Ld::LaTeXTranslationPhase::Phase::IDENTIFY_DEPENDENCIES phase to
             * collect a list of packages needed by this element translator.  You can also use this phase to define
             * any special custom commands.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            virtual bool identifyDependencies(ElementPointer element, LaTeXCodeGenerationEngine& generationEngine);

            /**
             * Method that is called to insert the LaTeX body content.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            virtual bool body(ElementPointer element, LaTeXCodeGenerationEngine& generationEngine);

            /**
             * Method you can call to escape LaTeX special characters.
             *
             * \param[in] rawText                The unescaped text.
             *
             * \param[in] unicodeTranslationMode Indicates the desired mode to use to translate Unicode.
             *
             * \param[in] textMode               If true, the text is being inserted in text mode.  If false, the text
             *                                   is being inserted in math mode.
             *
             * \return Returns the text, properly escaped.
             */
            static QString escapeText(
                const QString&                         rawText,
                LaTeXCodeGenerationEngine::UnicodeMode unicodeTranslationMode,
                bool                                   textMode
            );

        private:
            /**
             * Static map of LaTeX commands by unicode value.
             */
            static QMap<SpecialCharacters::UnicodeValue, QString> commandsByUnicodeValue;
    };
};

#endif
