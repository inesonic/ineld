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
* This file implements the \ref Ld::LaTeXDivisionOperatorTranslatorBase class.
***********************************************************************************************************************/

#include <QString>

#include "ld_element_structures.h"
#include "ld_division_operator_element.h"
#include "ld_text_export_context.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_latex_translator.h"
#include "ld_latex_division_operator_translator_base.h"

namespace Ld {
    LaTeXDivisionOperatorTranslatorBase::~LaTeXDivisionOperatorTranslatorBase() {}


    bool LaTeXDivisionOperatorTranslatorBase::body(ElementPointer element, LaTeXCodeGenerationEngine& engine) {
        bool          result = false;
        FormatPointer format = element->format();

        if (format->capabilities().contains(DivisionOperatorFormat::formatName)) {
            QSharedPointer<DivisionOperatorFormat>
                divisionOperatorFormat = format.dynamicCast<DivisionOperatorFormat>();

            DivisionOperatorFormat::DivisionSymbol symbol = divisionOperatorFormat->divisionSymbol();
            if (symbol == DivisionOperatorFormat::DivisionSymbol::DEFAULT) {
                symbol = defaultDivisionSymbol();
            }

            switch (symbol) {
                case DivisionOperatorFormat::DivisionSymbol::INVALID:
                case DivisionOperatorFormat::DivisionSymbol::OBELUS: {
                    result = LaTeXBinaryOperatorTranslatorBase::body(element, engine);
                    break;
                }

                case DivisionOperatorFormat::DivisionSymbol::FRACTION: {
                    TextExportContext& context  = engine.context();
                    ElementPointer     child0   = element->child(0);
                    ElementPointer     child1   = element->child(1);

                    QString leftParenthesis;
                    QString rightParenthesis;
                    bool    requiresParenthesis = includeParenthesis(element, leftParenthesis, rightParenthesis);

                    if (requiresParenthesis) {
                        context << leftParenthesis;
                    }

                    context << tr("\\frac{ ");
                    engine.translateChild(child0);
                    context << tr(" }{ ");
                    engine.translateChild(child1);
                    context << tr(" }");

                    if (requiresParenthesis) {
                        context << rightParenthesis;
                    }

                    break;
                }

                case DivisionOperatorFormat::DivisionSymbol::DEFAULT: {
                    assert(false);
                    break;
                }

                default: {
                    assert(false);
                    break;
                }
        }

        } else {
            result = LaTeXBinaryOperatorTranslatorBase::body(element, engine);
        }

        return result;
    }


    QString LaTeXDivisionOperatorTranslatorBase::latexOperator(FormatPointer /* operatorFormat */) const {
        return tr(" \\div ");
    }
}