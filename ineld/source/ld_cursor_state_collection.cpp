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
* This file implements the \ref Ld::CursorStateCollection class.
***********************************************************************************************************************/

#include <QList>
#include <QHash>

#include "ld_element_structures.h"
#include "ld_element_cursor.h"
#include "ld_cursor.h"
#include "ld_element.h"
#include "ld_cursor_weak_collection.h"
#include "ld_cursor_collection.h"
#include "ld_cursor_state_collection_entry.h"
#include "ld_cursor_state_collection.h"

namespace Ld {
    CursorStateCollection::CursorStateCollection() {}


    CursorStateCollection::CursorStateCollection(
            const CursorStateCollection& other
        ):QList<CursorStateCollectionEntry>(
            other
        ) {}


    CursorStateCollection::CursorStateCollection(const CursorCollection& other) {
        operator=(other);
    }


    CursorStateCollection::CursorStateCollection(const CursorWeakCollection& other) {
        operator=(other);
    }


    CursorStateCollection::~CursorStateCollection() {}


    void CursorStateCollection::clear() {
        QList<CursorStateCollectionEntry>::clear();
        currentIndexesByCursor.clear();
    }


    void CursorStateCollection::insert(CursorPointer cursor) {
        CursorStateCollectionEntry elementCursorEntry(cursor->elementCursor());
        CursorStateCollectionEntry selectionCursorEntry(cursor->selectionCursor());

        unsigned baseIndex = static_cast<unsigned>(size());
        append(elementCursorEntry);
        append(selectionCursorEntry);

        currentIndexesByCursor.insert(cursor, baseIndex);
    }


    void CursorStateCollection::captureCursorState() {
        for (  IndexesByCursor::const_iterator pos = currentIndexesByCursor.constBegin(),
                                               end = currentIndexesByCursor.constEnd()
             ; pos != end
             ; ++pos
            ) {
            CursorPointer cursor    = pos.key();
            unsigned      baseIndex = pos.value();

            (*this)[baseIndex + 0].update(cursor->elementCursor());
            (*this)[baseIndex + 1].update(cursor->selectionCursor());
        }
    }


    void CursorStateCollection::updateCursorState(bool clearSelections) {
        for (  IndexesByCursor::const_iterator pos = currentIndexesByCursor.constBegin(),
                                               end = currentIndexesByCursor.constEnd()
             ; pos != end
             ; ++pos
            ) {
            CursorPointer cursor    = pos.key();
            unsigned      baseIndex = pos.value();

            ElementCursor elementCursor = at(baseIndex + 0).toElementCursor();

            if (clearSelections) {
                cursor->updateFromElementCursor(elementCursor, false);
            } else {
                ElementCursor selectionCursor = at(baseIndex + 1).toElementCursor();

                if (selectionCursor.isInvalid() || elementCursor == selectionCursor) {
                    cursor->updateFromElementCursor(elementCursor, false);
                } else {
                    cursor->updateFromElementCursor(elementCursor, true);
                    cursor->updateSelectionFromElementCursor(selectionCursor);
                }
            }
        }
    }


    void CursorStateCollection::setCursorsTo(const Cursor& referenceCursor) {
        for (  IndexesByCursor::const_iterator pos = currentIndexesByCursor.constBegin(),
                                               end = currentIndexesByCursor.constEnd()
             ; pos != end
             ; ++pos
            ) {
            CursorPointer cursor = pos.key();
            *cursor = referenceCursor;
        }
    }


    CursorStateCollection& CursorStateCollection::operator=(const CursorStateCollection& other) {
        QList<CursorStateCollectionEntry>::operator=(other);
        currentIndexesByCursor = other.currentIndexesByCursor;

        return *this;
    }


    CursorStateCollection& CursorStateCollection::operator=(const CursorCollection& other) {
        clear();

        for (  CursorCollection::const_iterator it  = other.constBegin(),
                                                end = other.constEnd()
             ; it != end
             ; ++it
            ) {
            insert(*it);
        }

        return *this;
    }


    CursorStateCollection& CursorStateCollection::operator=(const CursorWeakCollection& other) {
        clear();

        for (  CursorWeakCollection::const_iterator it  = other.constBegin(),
                                                    end = other.constEnd()
             ; it != end
             ; ++it
            ) {
            CursorPointer cursor = it->toStrongRef();
            if (!cursor.isNull()) {
                insert(cursor);
            }
        }

        return *this;
    }
}
