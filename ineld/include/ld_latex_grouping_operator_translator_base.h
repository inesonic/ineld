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
* This header defines the \ref Ld::LaTeXGroupingOperatorTranslatorBase class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_LATEX_GROUPING_OPERATOR_TRANSLATOR_BASE_H
#define LD_LATEX_GROUPING_OPERATOR_TRANSLATOR_BASE_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_latex_translator.h"
#include "ld_latex_operator_translator_base.h"

namespace Ld {
    class LaTeXCodeGenerationEngine;

    /**
     * Base class for LaTeX translators used to render grouping operators.
     */
    class LD_PUBLIC_API LaTeXGroupingOperatorTranslatorBase:public LaTeXOperatorTranslatorBase {
        Q_DECLARE_TR_FUNCTIONS(Ld::LaTeXGroupingOperatorTranslatorBase)

        public:
            ~LaTeXGroupingOperatorTranslatorBase() override;

            /**
             * Method that is called to insert the LaTeX body content.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            bool body(ElementPointer element, LaTeXCodeGenerationEngine& generationEngine) override;

        protected:
            /**
             * Pure virtual method you should overload to provide the LaTeX left parenthesis character.  You do not
             * need to supply a LaTeX "\left" command.
             *
             * \param[in] element The element representing this operator.
             *
             * \return Returns the LaTeX character for the left parenthesis.
             */
            virtual QString latexLeftParenthesis(ElementPointer element) const = 0;

            /**
             * Pure virtual method you should overload to provide the LaTeX right parenthesis character.  You do not
             * need to supply a LaTeX "\right" command.
             *
             * \param[in] element The element representing this operator.
             *
             * \return Returns the LaTeX character for the right parenthesis.
             */
            virtual QString latexRightParenthesis(ElementPointer element) const = 0;
    };
};

#endif
