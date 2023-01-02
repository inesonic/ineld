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
* This file implements the \ref Ld::CursorStateCollectionEntry class.
***********************************************************************************************************************/

#include <QSharedPointer>
#include <QSharedDataPointer>
#include <QSharedData>

#include "ld_element_structures.h"
#include "ld_element.h"
#include "ld_element_cursor.h"
#include "ld_cursor_state_collection_entry.h"
#include "ld_cursor_state_collection_entry_private.h"

/***********************************************************************************************************************
 * Ld::CursorStateCollectionEntry::Invalid
 */

namespace Ld {
    CursorStateCollectionEntry::Invalid::Invalid() {}


    CursorStateCollectionEntry::Invalid::~Invalid() {}


    bool CursorStateCollectionEntry::Invalid::isValid() const {
        return false;
    }


    bool CursorStateCollectionEntry::Invalid::isWholeElement() const {
        return false;
    }


    bool CursorStateCollectionEntry::Invalid::isTextInElement() const {
        return false;
    }


    CursorStateCollectionEntry::PositionType CursorStateCollectionEntry::Invalid::positionType() const {
        return CursorStateCollectionEntry::PositionType::INVALID;
    }


    ElementPointer CursorStateCollectionEntry::Invalid::element() const {
        return ElementPointer();
    }


    ElementPointer CursorStateCollectionEntry::Invalid::parent() const {
        return ElementPointer();
    }


    unsigned long CursorStateCollectionEntry::Invalid::childIndex() const {
        return Element::invalidChildIndex;
    }


    unsigned long CursorStateCollectionEntry::Invalid::textIndex() const {
        return Element::invalidTextIndex;
    }


    unsigned CursorStateCollectionEntry::Invalid::regionIndex() const {
        return Element::invalidRegionIndex;
    }


    ElementCursor CursorStateCollectionEntry::Invalid::toElementCursor() const {
        return ElementCursor();
    }


    bool CursorStateCollectionEntry::Invalid::operator==(const CursorStateCollectionEntry::Private& other) const {
        return !other.isValid();
    }
}

/***********************************************************************************************************************
 * Ld::CursorStateCollectionEntry::WholeElement
 */

namespace Ld {
    CursorStateCollectionEntry::WholeElement::WholeElement(ElementPointer element) {
        Q_ASSERT(!element.isNull());
        currentElement = element;
    }


    CursorStateCollectionEntry::WholeElement::~WholeElement() {}


    bool CursorStateCollectionEntry::WholeElement::isValid() const {
        return true;
    }


    bool CursorStateCollectionEntry::WholeElement::isWholeElement() const {
        return true;
    }


    CursorStateCollectionEntry::PositionType CursorStateCollectionEntry::WholeElement::positionType() const {
        return CursorStateCollectionEntry::PositionType::ELEMENT;
    }


    ElementPointer CursorStateCollectionEntry::WholeElement::element() const {
        return currentElement;
    }


    ElementPointer CursorStateCollectionEntry::WholeElement::parent() const {
        return currentElement->parent();
    }


    unsigned long CursorStateCollectionEntry::WholeElement::childIndex() const {
        unsigned childIndex;

        ElementPointer parent = currentElement->parent();
        if (!parent.isNull()) {
            childIndex = parent->indexOfChild(currentElement);
        } else {
            childIndex = Element::invalidChildIndex;
        }

        return childIndex;
    }


    ElementCursor CursorStateCollectionEntry::WholeElement::toElementCursor() const {
        return ElementCursor(currentElement);
    }


    bool CursorStateCollectionEntry::WholeElement::operator==(const CursorStateCollectionEntry::Private& other) const {
        return other.isWholeElement() && other.element() == currentElement;
    }
}

/***********************************************************************************************************************
 * Ld::CursorStateCollectionEntry::NullEntry
 */

namespace Ld {
    CursorStateCollectionEntry::NullEntry::NullEntry(ElementPointer parent, unsigned long childIndex) {
        currentParent     = parent;
        currentChildIndex = childIndex;
    }


    CursorStateCollectionEntry::NullEntry::~NullEntry() {}


    bool CursorStateCollectionEntry::NullEntry::isValid() const {
        return true;
    }


    bool CursorStateCollectionEntry::NullEntry::isWholeElement() const {
        return true;
    }


    CursorStateCollectionEntry::PositionType CursorStateCollectionEntry::NullEntry::positionType() const {
        return CursorStateCollectionEntry::PositionType::ELEMENT;
    }


    ElementPointer CursorStateCollectionEntry::NullEntry::parent() const {
        return currentParent;
    }


    unsigned long CursorStateCollectionEntry::NullEntry::childIndex() const {
        return currentChildIndex;
    }


    ElementCursor CursorStateCollectionEntry::NullEntry::toElementCursor() const {
        return ElementCursor(currentParent, currentChildIndex);
    }


    bool CursorStateCollectionEntry::NullEntry::operator==(const CursorStateCollectionEntry::Private& other) const {
        return (
               other.isWholeElement()
            && other.parent()     == currentParent
            && other.childIndex() == currentChildIndex
        );
    }
}

/***********************************************************************************************************************
 * Ld::CursorStateCollectionEntry::TextLocation
 */

namespace Ld {
    CursorStateCollectionEntry::TextLocation::TextLocation(
            ElementPointer element,
            unsigned long  textIndex,
            unsigned       regionIndex
        ):WholeElement(
            element
        ) {
        currentTextIndex   = textIndex;
        currentRegionIndex = regionIndex;
    }


    CursorStateCollectionEntry::TextLocation::~TextLocation() {}


    bool CursorStateCollectionEntry::TextLocation::isWholeElement() const {
        return false;
    }


    bool CursorStateCollectionEntry::TextLocation::isTextInElement() const {
        return true;
    }


    CursorStateCollectionEntry::PositionType CursorStateCollectionEntry::TextLocation::positionType() const {
        return CursorStateCollectionEntry::PositionType::TEXT;
    }


    unsigned long CursorStateCollectionEntry::TextLocation::textIndex() const {
        return currentTextIndex;
    }


    unsigned CursorStateCollectionEntry::TextLocation::regionIndex() const {
        return currentRegionIndex;
    }


    ElementCursor CursorStateCollectionEntry::TextLocation::toElementCursor() const {
        return ElementCursor(currentTextIndex, currentRegionIndex, element());
    }


    bool CursorStateCollectionEntry::TextLocation::operator==(const CursorStateCollectionEntry::Private& other) const {
        return (
                other.isTextInElement()
            && other.textIndex() == currentTextIndex
            && other.regionIndex() == currentRegionIndex
        );
    }
}
