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
* This file implements the \ref Ld::CursorCollection class.
***********************************************************************************************************************/

#include <QSharedPointer>
#include <QWeakPointer>
#include <QSet>

#include "ld_cursor.h"
#include "ld_cursor_weak_collection.h"
#include "ld_cursor_state_collection.h"
#include "ld_cursor_collection.h"

namespace Ld {
    CursorCollection::CursorCollection() {}


    CursorCollection::CursorCollection(const CursorCollection& other):QSet<CursorPointer>(other) {}


    CursorCollection::CursorCollection(const CursorStateCollection& other) {
        operator=(other);
    }


    CursorCollection::CursorCollection(const CursorWeakCollection& other) {
        operator=(other);
    }


    CursorCollection::~CursorCollection() {}


    CursorCollection& CursorCollection::operator=(const CursorCollection& other) {
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


    CursorCollection& CursorCollection::operator=(const CursorStateCollection& other) {
        clear();

        for (  CursorStateCollection::IndexesByCursor::const_iterator pos = other.currentIndexesByCursor.constBegin(),
                                                                      end = other.currentIndexesByCursor.constEnd()
             ; pos != end
             ; ++pos
            ) {
            CursorPointer cursor = pos.key();
            insert(cursor);
        }

        return *this;
    }


    CursorCollection& CursorCollection::operator=(const CursorWeakCollection& other) {
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
