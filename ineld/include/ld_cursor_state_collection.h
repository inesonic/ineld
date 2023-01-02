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
* This header defines the \ref Ld::CursorStateCollection class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CURSOR_STATE_COLLECTION_H
#define LD_CURSOR_STATE_COLLECTION_H

#include <QHash>
#include <QList>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_element_cursor.h"
#include "ld_cursor.h"
#include "ld_cursor_state_collection_entry.h"

namespace Ld {
    class CursorWeakCollection;
    class CursorCollection;

    /**
     * This class provides an API and list of containers you can use to track cursors.  The class also includes methods
     * to save and restore cursor state information.
     *
     * Commands can use this class to update cursor information while manipulating the underlying program data
     * structures.
     *
     * The class derives from a list of \ref Ld::CursorStateCollectionEntry instances.  You can manipulate these
     * instances in order to keep cursor position information coherent with the document structure.  The actual
     * \ref Cursor instances will be updated when \ref CursorStateCollection::updateCursorState is called.
     */
    class LD_PUBLIC_API CursorStateCollection:public QList<CursorStateCollectionEntry> {
        friend class CursorCollection;
        friend class CursorWeakCollection;

        public:
            CursorStateCollection();

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            CursorStateCollection(const CursorStateCollection& other);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            CursorStateCollection(const CursorCollection& other);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            CursorStateCollection(const CursorWeakCollection& other);

            ~CursorStateCollection();

            /**
             * Method you can use to clear this class instance.
             */
            void clear();

            /**
             * Method you can call to insert a new cursor.  The cursor state will be captured automatically.
             *
             * \param[in] cursor A shared pointer to the cursor to be captured.
             */
            void insert(CursorPointer cursor);

            /**
             * Method you can call to update the underlying cursor state data.  You can use this method to perform
             * multiple cursor position update stages using the same structure.
             */
            void captureCursorState();

            /**
             * Method you can call to update all the cursors based on new cursor state data.
             *
             * \param[in] clearSelections If true, all selections will be cleared.  If false, selections will remain
             *                            untouched.
             */
            void updateCursorState(bool clearSelections);

            /**
             * Method you can call to force all cursors to be assigned to a specific cursor position.
             *
             * \param[in] referenceCursor The cursor specifying the desired final position.
             */
            void setCursorsTo(const Cursor& referenceCursor);

            /**
             * Assignment operator
             *
             * \param[in] other The instance to assign to this instance.
             */
            CursorStateCollection& operator=(const CursorStateCollection& other);

            /**
             * Assignment operator
             *
             * \param[in] other The instance to assign to this instance.
             */
            CursorStateCollection& operator=(const CursorCollection& other);

            /**
             * Assignment operator
             *
             * \param[in] other The instance to assign to this instance.
             */
            CursorStateCollection& operator=(const CursorWeakCollection& other);

        private:
            /**
             * Type used to track the associated cursor instances.
             */
            typedef QHash<CursorPointer, unsigned> IndexesByCursor;

            /**
             * A hash used to relate \ref Ld::Cursor instances to their associated cursor entries.  Entries will always
             * be in pairs with the first entry pointing to the element cursor and the second entry pointing to the
             * selection cursor.
             */
            IndexesByCursor currentIndexesByCursor;
    };
}

#endif
