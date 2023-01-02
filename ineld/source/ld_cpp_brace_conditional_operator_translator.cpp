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
* \file Ld::CppBraceConditionalOperatorTranslator class.
***********************************************************************************************************************/

#include <QString>

#include "ld_brace_conditional_operator_element.h"
#include "ld_cpp_code_generator_diagnostic.h"
#include "ld_cpp_code_generation_engine.h"
#include "ld_cpp_context.h"
#include "ld_cpp_translator.h"
#include "ld_cpp_brace_conditional_operator_translator.h"

namespace Ld {
    CppBraceConditionalOperatorTranslator::~CppBraceConditionalOperatorTranslator() {}


    QString CppBraceConditionalOperatorTranslator::elementName() const {
        return BraceConditionalOperatorElement::elementName;
    }


    bool CppBraceConditionalOperatorTranslator::threadImplementation(
            ElementPointer           element,
            CppCodeGenerationEngine& engine
        ) {
        CppContext& context = engine.context();

        QSharedPointer<BraceConditionalOperatorElement>
            braceElement = element.dynamicCast<BraceConditionalOperatorElement>();
        assert(!braceElement.isNull());

        unsigned long numberExplicitCases = braceElement->numberExplicitConditions();
        unsigned      nestingLevel        = 1;

        context(element) << "(";
        for (unsigned long i=0 ; i<numberExplicitCases ; ++i) {
            ElementPointer valueElement     = braceElement->valueElement(i);
            ElementPointer conditionElement = braceElement->conditionElement(i);

            context(element) << "M::inlineToBoolean(";
            engine.translateChild(conditionElement);
            context(element) << ")?(";
            engine.translateChild(valueElement);
            context(element) << "):(";

            ++nestingLevel;
        }

        ElementPointer elseValueElement = braceElement->elseValueElement();
        engine.translateChild(elseValueElement);

        context(element) << QString(")").repeated(nestingLevel);

        return true;
    }


    bool CppBraceConditionalOperatorTranslator::methodDefinitions(
            ElementPointer           element,
            CppCodeGenerationEngine& engine
        ) {
        return threadImplementation(element, engine);
    }
}
