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
* This header defines the \ref Ld::LaTeXFloorOperatorTranslator class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_LATEX_FLOOR_OPERATOR_TRANSLATOR_H
#define LD_LATEX_FLOOR_OPERATOR_TRANSLATOR_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>

#include "ld_common.h"
#include "ld_format_structures.h"
#include "ld_parenthesis_format.h"
#include "ld_latex_grouping_operator_translator_base.h"

namespace Ld {
    class LaTeXCodeGenerationEngine;

    /**
     * LaTeX translator class for the \ref Ld::FloorOperatorElement class.
     */
    class LD_PUBLIC_API LaTeXFloorOperatorTranslator:public LaTeXGroupingOperatorTranslatorBase {
        Q_DECLARE_TR_FUNCTIONS(Ld::LaTeXFloorOperatorTranslator)

        public:
            ~LaTeXFloorOperatorTranslator() override;

            /**
             * Method that returns the name of the element this translator operates on.
             *
             * \return Returns the name of the element this translator operates on.
             */
            QString elementName() const override;

        protected:
            /**
             * Pure virtual method you should overload to provide the LaTeX left parenthesis character.  You do not
             * need to supply a LaTeX "\left" command.
             *
             * \param[in] element The element representing this operator.
             *
             * \return Returns the LaTeX character for the left parenthesis.
             */
            QString latexLeftParenthesis(ElementPointer element) const override;

            /**
             * Pure virtual method you should overload to provide the LaTeX right parenthesis character.  You do not
             * need to supply a LaTeX "\right" command.
             *
             * \param[in] element The element representing this operator.
             *
             * \return Returns the LaTeX character for the right parenthesis.
             */
            QString latexRightParenthesis(ElementPointer element) const override;
    };
};

#endif
