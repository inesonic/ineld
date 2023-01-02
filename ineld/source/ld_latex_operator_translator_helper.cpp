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
* This file implements the \ref Ld::LaTeXOperatorTranslatorHelper class.
***********************************************************************************************************************/

#include <QString>

#include "ld_element_structures.h"
#include "ld_function_element.h"
#include "ld_latex_operator_translator_helper.h"

namespace Ld {
    LaTeXOperatorTranslatorHelper::~LaTeXOperatorTranslatorHelper() {}


    bool LaTeXOperatorTranslatorHelper::includeParenthesis(
            ElementPointer element,
            QString&       leftParenthesisString,
            QString&       rightParenthesisString
        ) const {
        bool                      includeParenthesis = false;
        Element::ParenthesisStyle parenthesisStyle   = element->parenthesisStyle();

        switch (parenthesisStyle) {
            case Element::ParenthesisStyle::INVALID_OR_NONE: {
                includeParenthesis = false;
                break;
            }

            case Element::ParenthesisStyle::PARENTHESIS: {
                includeParenthesis = true;
                leftParenthesisString  = tr("\\left (");
                rightParenthesisString = tr("\\right )");

                break;
            }

            case Element::ParenthesisStyle::BRACKETS: {
                includeParenthesis = true;
                leftParenthesisString  = tr("\\left [");
                rightParenthesisString = tr("\\right ]");

                break;
            }

            case Element::ParenthesisStyle::BRACES: {
                includeParenthesis = true;
                leftParenthesisString  = tr("\\left \\lbrace");
                rightParenthesisString = tr("\\right \\rbrace");

                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        return includeParenthesis;
    }


    bool LaTeXOperatorTranslatorHelper::includeParameterParenthesis(
            ElementPointer element,
            QString&       leftParenthesisString,
            QString&       rightParenthesisString
        ) const {
        bool                                includeParenthesis = false;
        QSharedPointer<Ld::FunctionElement> functionElement    = element.dynamicCast<Ld::FunctionElement>();
        assert(!functionElement.isNull());

        Element::ParenthesisStyle parenthesisStyle = functionElement->parameterParenthesisStyle();

        switch (parenthesisStyle) {
            case Element::ParenthesisStyle::INVALID_OR_NONE: {
                includeParenthesis = false;
                break;
            }

            case Element::ParenthesisStyle::PARENTHESIS: {
                includeParenthesis = true;
                leftParenthesisString  = tr("\\left (");
                rightParenthesisString = tr("\\right )");

                break;
            }

            case Element::ParenthesisStyle::BRACKETS: {
                includeParenthesis = true;
                leftParenthesisString  = tr("\\left [");
                rightParenthesisString = tr("\\right ]");

                break;
            }

            case Element::ParenthesisStyle::BRACES: {
                includeParenthesis = true;
                leftParenthesisString  = tr("\\left \\lbrace");
                rightParenthesisString = tr("\\right \\rbrace");

                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        return includeParenthesis;
    }
}
