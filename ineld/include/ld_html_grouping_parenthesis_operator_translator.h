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
* This header defines the \ref Ld::HtmlGroupingParenthesisOperatorTranslator class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_HTML_GROUPING_PARENTHESIS_OPERATOR_TRANSLATOR_H
#define LD_HTML_GROUPING_PARENTHESIS_OPERATOR_TRANSLATOR_H

#include <QString>

#include "ld_common.h"
#include "ld_format_structures.h"
#include "ld_parenthesis_format.h"
#include "ld_html_simple_grouping_operator_translator_base.h"

namespace Ld {
    /**
     * HTML translator class for the \ref Ld::GroupingParenthesisOperatorElement class.
     */
    class LD_PUBLIC_API HtmlGroupingParenthesisOperatorTranslator:public HtmlSimpleGroupingOperatorTranslatorBase {
        public:
            ~HtmlGroupingParenthesisOperatorTranslator() override;

            /**
             * Method that returns the name of the element this translator operates on.
             *
             * \return Returns the name of the element this translator operates on.
             */
            QString elementName() const override;

        protected:
            /**
             * Method that indicates the default parenthesis style for this translator.
             *
             * \return Returns the default parenthesis style.
             */
            ParenthesisFormat::ParenthesisStyle defaultParenthesisStyle() const override;
    };
};

#endif
