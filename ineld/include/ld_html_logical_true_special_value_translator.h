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
* This header defines the \ref Ld::HtmlLogicalTrueSpecialValueTranslator class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_HTML_LOGICAL_TRUE_SPECIAL_VALUE_TRANSLATOR_H
#define LD_HTML_LOGICAL_TRUE_SPECIAL_VALUE_TRANSLATOR_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>

#include "ld_common.h"
#include "ld_html_special_symbol_translator_base.h"

namespace Ld {
    class HtmlCodeGenerationEngine;

    /**
     * HTML translator class for the \ref Ld::LogicalTrueSpecialValueElement class.
     */
    class LD_PUBLIC_API HtmlLogicalTrueSpecialValueTranslator:public HtmlSpecialSymbolTranslatorBase {
        Q_DECLARE_TR_FUNCTIONS(Ld::HtmlLogicalTrueSpecialValueTranslator)

        public:
            ~HtmlLogicalTrueSpecialValueTranslator() override;

            /**
             * Method that returns the name of the element this translator operates on.
             *
             * \return Returns the name of the element this translator operates on.
             */
            QString elementName() const override;

        protected:
            /**
             * Pure virtual method you should overload to provide the MathML operator character.
             *
             * \return returns the MathML operator character string.
             */
            QString mathMlOperator() const final;

            /**
             * Pure virtual method you should overload to provide the LaTeX operator command.
             *
             * \return returns the LaTeX operator string.
             */
            QString latexOperator() const final;
    };
};

#endif
