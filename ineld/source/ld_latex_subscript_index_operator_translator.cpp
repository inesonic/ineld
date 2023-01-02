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
* This file implements the \ref Ld::LaTeXSubscriptIndexOperatorTranslator class.
***********************************************************************************************************************/

#include <QString>

#include "ld_format_structures.h"
#include "ld_element_structures.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_text_export_context.h"
#include "ld_variable_element.h"
#include "ld_subscript_index_operator_element.h"
#include "ld_subscript_row_column_operator_element.h"
#include "ld_latex_operator_translator_base.h"
#include "ld_latex_subscript_index_operator_translator.h"

namespace Ld {
    LaTeXSubscriptIndexOperatorTranslator::~LaTeXSubscriptIndexOperatorTranslator() {}


    QString LaTeXSubscriptIndexOperatorTranslator::elementName() const {
        return SubscriptIndexOperatorElement::elementName;
    }


    bool LaTeXSubscriptIndexOperatorTranslator::body(ElementPointer element, LaTeXCodeGenerationEngine& engine) {
        TextExportContext& context = engine.context();
        ElementPointer     child0  = element->child(0);
        ElementPointer     child1  = element->child(1);

        bool includeComma = false;
        if (!child0.isNull()) {
            QString typeName = child0->typeName();
            includeComma = (
                   (typeName == Ld::VariableElement::elementName && !child0->text(1).isEmpty())
                || typeName == Ld::SubscriptIndexOperatorElement::elementName
                || typeName == Ld::SubscriptRowColumnOperatorElement::elementName
            );
        }

        QString leftParenthesis;
        QString rightParenthesis;
        bool    requiresParenthesis = includeParenthesis(element, leftParenthesis, rightParenthesis);

        if (requiresParenthesis) {
            context << leftParenthesis;
        }

        context << "{ ";
        engine.translateChild(child0);
        context << " }_{";

        context << (includeComma ? "," : " ");

        engine.translateChild(child1);
        context << " }";

        if (requiresParenthesis) {
            context << rightParenthesis;
        }

        return true;
    }
}
