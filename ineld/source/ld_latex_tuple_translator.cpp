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
* This file implements the \ref Ld::LaTeXTupleTranslator class.
***********************************************************************************************************************/

#include <QString>

#include "ld_element_structures.h"
#include "ld_tuple_element.h"
#include "ld_list_placeholder_element.h"
#include "ld_text_export_context.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_latex_translator.h"
#include "ld_latex_tuple_translator.h"

namespace Ld {
    LaTeXTupleTranslator::~LaTeXTupleTranslator() {}


    QString LaTeXTupleTranslator::elementName() const {
        return TupleElement::elementName;
    }


    bool LaTeXTupleTranslator::body(ElementPointer element, LaTeXCodeGenerationEngine& engine) {
        TextExportContext& context = engine.context();

        context << "\\left (";

        unsigned long numberChildren = element->numberChildren();
        for (unsigned long childIndex=0 ; childIndex<numberChildren ; ++childIndex) {
            Ld::ElementPointer childElement = element->child(childIndex);

            if (childIndex > 0                                                            &&
                (childElement.isNull()                                               ||
                 childElement->typeName() != Ld::ListPlaceholderElement::elementName    )    ) {
                context << tr(", ");
            }

            engine.translateChild(childElement);
        }

        context << " \\right )";

        return true;
    }
}
