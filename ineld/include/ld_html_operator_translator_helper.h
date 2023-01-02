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
* This header defines the \ref Ld::HtmlOperatorTranslatorHelper class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_HTML_OPERATOR_TRANSLATOR_HELPER_H
#define LD_HTML_OPERATOR_TRANSLATOR_HELPER_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_element_structures.h"

namespace Ld {
    /**
     * Base class for HTML translators used to render operators.
     */
    class LD_PUBLIC_API HtmlOperatorTranslatorHelper {
        Q_DECLARE_TR_FUNCTIONS(Ld::HtmlOperatorTranslatorHelper)

        public:
            virtual ~HtmlOperatorTranslatorHelper();

        protected:
            /**
             * Method you can overload to calculate the parenthesis values from a format.  The default implementation
             * will determine if parenthesis are needed by looking at the precedence of this element relative to the
             * parent.  The default implementation will also pre-load parenthesis strings based on the current element
             * format.
             *
             * \param[in] element                 A pointer to the element to be translated.
             *
             * \param[in] latexMode               If true, the parenthesis strings should be LaTeX commands.  If false,
             *                                    the parenthesis strings should be properly encoded characters.
             *
             * \param[out] leftParenthesisString  The string to insert for the left parenthesis.
             *
             * \param[out] rightParenthesisString The string to be inserted for the right parenthesis.
             *
             * \return Returns true if parenthesis should be included.  Returns false if parenthesis should not be
             *         included.
             */
            virtual bool includeParenthesis(
                ElementPointer element,
                bool           latexMode,
                QString&       leftParenthesisString,
                QString&       rightParenthesisString
            ) const;

            /**
             * Method you can overload to calculate the parenthesis values from a format.  The default implementation
             * will determine if parenthesis are needed by looking at the precedence of this element relative to the
             * parent.  The default implementation will also pre-load parenthesis strings based on the current element
             * format.
             *
             * \param[in] element                 A pointer to the element to be translated.
             *
             * \param[in] latexMode               If true, the parenthesis strings should be LaTeX commands.  If false,
             *                                    the parenthesis strings should be properly encoded characters.
             *
             * \param[out] leftParenthesisString  The string to insert for the left parenthesis.
             *
             * \param[out] rightParenthesisString The string to be inserted for the right parenthesis.
             *
             * \return Returns true if parenthesis should be included.  Returns false if parenthesis should not be
             *         included.
             */
            virtual bool includeParameterParenthesis(
                ElementPointer element,
                bool           latexMode,
                QString&       leftParenthesisString,
                QString&       rightParenthesisString
            ) const;
    };
};

#endif
