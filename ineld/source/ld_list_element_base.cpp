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
* This file implements the \ref Ld::ListElementBase class.
***********************************************************************************************************************/

#include <QString>

#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_element_with_positional_children.h"
#include "ld_root_element.h"
#include "ld_list_placeholder_element.h"
#include "ld_function_placeholder_element.h"
#include "ld_font_format.h"
#include "ld_character_format.h"
#include "ld_element_cursor.h"
#include "ld_format.h"
#include "ld_list_element_base.h"

namespace Ld {
    ListElementBase::ListElementBase() {}


    ListElementBase::~ListElementBase() {}


    bool ListElementBase::isListElement() const {
        return true;
    }


    bool ListElementBase::isFunctionLogicalStop(unsigned long functionChildIndex) const {
        unsigned long nc = numberChildren();
        unsigned long i  = functionChildIndex;

        ElementPointer element;
        QString        tn;
        do {
            ++i;
            if (i < nc) {
                element = child(i);
                tn = element->typeName();
            }
        } while (i < nc                                                        &&
                 (tn == Ld::ListPlaceholderElement::elementName           ||
                  (tn == Ld::FunctionPlaceholderElement::elementName &&
                   element->text().isEmpty()                            )    )    );

        return (i >= nc);
    }


    void ListElementBase::updateAfterGraft() {
        if (format().isNull()) {
            ElementPointer rootElement = root();
            assert(!rootElement.isNull());

            if (rootElement->typeName() == Ld::RootElement::elementName) {
                QSharedPointer<Ld::RootElement> root = rootElement.dynamicCast<Ld::RootElement>();
                assert(!root.isNull());

                setFormat(root->defaultMathTextFormat());
            }
        }
    }
}
