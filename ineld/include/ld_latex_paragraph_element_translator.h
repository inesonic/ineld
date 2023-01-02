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
* This header defines the \ref Ld::LaTeXParagraphElementTranslator class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_LATEX_PARAGRAPH_ELEMENT_TRANSLATOR_H
#define LD_LATEX_PARAGRAPH_ELEMENT_TRANSLATOR_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QRegularExpression>
#include <QString>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_latex_translator.h"

namespace Ld {
    class LaTeXCodeGenerationEngine;

    /**
     * LaTeX translator for the paragraph element.
     */
    class LD_PUBLIC_API LaTeXParagraphElementTranslator:public LaTeXTranslator {
        Q_DECLARE_TR_FUNCTIONS(Ld::LaTeXParagraphElementTranslator)

        public:
            ~LaTeXParagraphElementTranslator() override;

            /**
             * Method that returns the name of the element this translator operates on.
             *
             * \return Returns the name of the element this translator operates on.
             */
            QString elementName() const final;

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
            bool identifyDependencies(ElementPointer element, LaTeXCodeGenerationEngine& generationEngine) final;

            /**
             * Method that is called to insert the LaTeX body content.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            bool body(ElementPointer element, LaTeXCodeGenerationEngine& generationEngine) final;

        private:
            /**
             * A regular expression used to modify list number prefixes and suffixes for LaTeX.
             */
            static const QRegularExpression listSpecialCharacters;

            /**
             * Method that escapes an ordered list prefix or suffix
             *
             * \param[in] unescapedText          The raw, unescaped text.
             *
             * \param[in] unicodeTranslationMode Indicates the desired mode to use to translate Unicode.
             *
             * \return Returns the text escaped and, if needed, contained within braces.
             */
            static QString escapedTextForOrderedList(
                const QString&                         unescapedText,
                LaTeXCodeGenerationEngine::UnicodeMode unicodeTranslationMode
            );

            /**
             * Method that adds a ragged2e environment to control paragraph justification.
             *
             * \param[in] format           The format indicating the justification.
             *
             * \param[in] generationEngine The engine being used to generate the LaTeX output.
             */
            void addRagged2eEnvironment(FormatPointer format, LaTeXCodeGenerationEngine& generationEngine);

            /**
             * Method that determines if we should end the current list and insert the correct ending context.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             */
            static bool endListIfNeeded(ElementPointer element, LaTeXCodeGenerationEngine& generationEngine);
    };
};

#endif
