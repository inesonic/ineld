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
#include <QHash>

#include <util_hash_functions.h>

#include "ld_element_structures.h"
#include "ld_element.h"
#include "ld_element_cursor.h"
#include "ld_cursor_state_collection_entry_private.h"
#include "ld_cursor_state_collection_entry.h"

namespace Ld {
    CursorStateCollectionEntry::CursorStateCollectionEntry() {
        invalidate();
    }


    CursorStateCollectionEntry::CursorStateCollectionEntry(const ElementCursor& elementCursor) {
        update(elementCursor);
    }


    CursorStateCollectionEntry::CursorStateCollectionEntry(const CursorStateCollectionEntry& other) {
        impl = other.impl;
    }


    CursorStateCollectionEntry::~CursorStateCollectionEntry() {}


    bool CursorStateCollectionEntry::isValid() const {
        return impl->isValid();
    }


    bool CursorStateCollectionEntry::isInvalid() const {
        return !impl->isValid();
    }


    bool CursorStateCollectionEntry::isWholeElement() const {
        return impl->isWholeElement();
    }


    bool CursorStateCollectionEntry::isTextInElement() const {
        return impl->isTextInElement();
    }


    CursorStateCollectionEntry::PositionType CursorStateCollectionEntry::positionType() const {
        return impl->positionType();
    }


    ElementPointer CursorStateCollectionEntry::element() const {
        return impl->element();
    }


    ElementPointer CursorStateCollectionEntry::parent() const {
        return impl->parent();
    }


    unsigned long CursorStateCollectionEntry::childIndex() const {
        return impl->childIndex();
    }


    unsigned long CursorStateCollectionEntry::textIndex() const {
        return impl->textIndex();
    }


    unsigned CursorStateCollectionEntry::regionIndex() const {
        return impl->regionIndex();
    }


    void CursorStateCollectionEntry::invalidate() {
        impl.reset(new Invalid);
    }


    void CursorStateCollectionEntry::update(const ElementCursor& elementCursor) {
        if (elementCursor.isValid()) {
            ElementPointer parent     = elementCursor.parent();
            unsigned long  childIndex = elementCursor.childIndex();

            ElementPointer element    = parent->child(childIndex);
            if (element.isNull()) {
                impl.reset(new NullEntry(parent, childIndex));
            } else {
                if (elementCursor.isWholeElement()) {
                    impl.reset(new WholeElement(element));
                } else {
                    unsigned long textIndex   = elementCursor.textIndex();
                    unsigned      regionIndex = elementCursor.regionIndex();

                    impl.reset(new TextLocation(element, textIndex, regionIndex));
                }
            }
        } else {
            impl.reset(new Invalid);
        }
    }


    void CursorStateCollectionEntry::update(ElementPointer element, bool preferForward) {
        if (element->cursorParkMode() == Ld::Element::CursorParkMode::CAN_NOT_PARK) {
            ElementCursor cursor(element);
            cursor.fixPosition(preferForward, false);

            ElementPointer finalElement = cursor.element();
            if (cursor.isWholeElement()) {
                impl.reset(new WholeElement(finalElement));
            } else if (finalElement.isNull()) {
                impl.reset(new NullEntry(cursor.parent(), cursor.childIndex()));
            } else {
                impl.reset(new TextLocation(finalElement, cursor.textIndex(), cursor.regionIndex()));
            }
        } else {
            impl.reset(new WholeElement(element));
        }
    }


    void CursorStateCollectionEntry::update(ElementPointer parent, unsigned long childIndex, bool preferForward) {
        ElementCursor cursor(parent, childIndex);
        cursor.fixPosition(preferForward, false);

        ElementPointer element = cursor.element();
        if (cursor.isWholeElement()) {
            impl.reset(new WholeElement(cursor.element()));
        } else if (element.isNull()) {
            impl.reset(new NullEntry(cursor.parent(), cursor.childIndex()));
        } else {
            impl.reset(new TextLocation(element, cursor.textIndex(), cursor.regionIndex()));
        }
    }


    void CursorStateCollectionEntry::update(unsigned long textIndex, ElementPointer element) {
        impl.reset(new TextLocation(element, textIndex, 0));
    }


    void CursorStateCollectionEntry::update(unsigned long textIndex, unsigned regionIndex, ElementPointer element) {
        impl.reset(new TextLocation(element, textIndex, regionIndex));
    }


    void CursorStateCollectionEntry::updateToFirstPositionIn(ElementPointer element, bool preferForward) {
        if (element->numberTextRegions() > 0) {
            update(0, 0, element);
        } else {
            update(element, preferForward);
        }
    }


    void CursorStateCollectionEntry::updateToLastPositionIn(ElementPointer element, bool preferForward) {
        unsigned numberTextRegions = element->numberTextRegions();
        if (numberTextRegions > 0) {
            unsigned      regionIndex = numberTextRegions - 1;
            unsigned long textIndex   = static_cast<unsigned long>(element->text(regionIndex).length());

            update(textIndex, regionIndex, element);
        } else {
            update(element, preferForward);
        }
    }


    ElementCursor CursorStateCollectionEntry::toElementCursor() const {
        return impl->toElementCursor();
    }


    CursorStateCollectionEntry& CursorStateCollectionEntry::operator=(const CursorStateCollectionEntry& other) {
        impl = other.impl;
        return *this;
    }


    bool CursorStateCollectionEntry::operator==(const CursorStateCollectionEntry& other) const {
        return *other.impl == *impl;
    }


    bool CursorStateCollectionEntry::operator!=(const CursorStateCollectionEntry& other) const {
        return !(*other.impl == *impl);
    }


    Util::HashResult qHash(const CursorStateCollectionEntry& key, Util::HashSeed seed) {
        Util::HashResult result;

        if (key.isValid()) {
            ElementPointer element = key.element();
            if (!element.isNull()) {
                result = qHash(element, seed);
            } else {
                result = static_cast<Util::HashResult>(seed);
            }

            result ^= (
                  qHash(key.parent(), seed)
                ^ ::qHash(key.childIndex(), seed)
                ^ ::qHash(key.textIndex(), seed)
                ^ ::qHash(key.regionIndex(), seed)
            );
        } else {
            result = static_cast<Util::HashResult>(seed);
        }

        return result;
    }
}
