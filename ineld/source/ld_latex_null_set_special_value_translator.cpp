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
* This file implements the \ref Ld::LaTeXNullSetSpecialValueTranslator class.
***********************************************************************************************************************/

#include <QString>

#include "ld_element_structures.h"
#include "ld_null_set_special_value_element.h"
#include "ld_text_export_context.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_latex_translator.h"
#include "ld_latex_null_set_special_value_translator.h"

namespace Ld {
    LaTeXNullSetSpecialValueTranslator::~LaTeXNullSetSpecialValueTranslator() {}


    QString LaTeXNullSetSpecialValueTranslator::elementName() const {
        return NullSetSpecialValueElement::elementName;
    }


    bool LaTeXNullSetSpecialValueTranslator::body(ElementPointer /* element*/, LaTeXCodeGenerationEngine& engine) {
        TextExportContext& context = engine.context();
        context << "\\varnothing ";

        return true;
    }
}