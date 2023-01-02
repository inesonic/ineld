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
* This header defines the \ref Ld::CursorStateCollectionEntry class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CURSOR_STATE_COLLECTION_ENTRY_H
#define LD_CURSOR_STATE_COLLECTION_ENTRY_H

#include <QSharedPointer>

#include <util_hash_functions.h>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_element_cursor.h"

namespace Ld {
    /**
     * Class used by \ref Ld::CursorStateCollection to both perform and track updates to an individual element cursor
     * instance.
     *
     * Note that this class uses pseudo-copy on write sematics to ensure fast copy operations.
     */
    class LD_PUBLIC_API CursorStateCollectionEntry {
        public:
            /**
             * Type used to indicate the type of cursor entry being pointed to.
             */
            typedef ElementCursor::PositionType PositionType;

            CursorStateCollectionEntry();

            /**
             * Constructor
             *
             * \param[in] elementCursor The element cursor to be tracked by this instance.
             */
            CursorStateCollectionEntry(const ElementCursor& elementCursor);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            CursorStateCollectionEntry(const CursorStateCollectionEntry& other);

            ~CursorStateCollectionEntry();

            /**
             * Method that determines if this cursor state collection entry is valid.
             *
             * \return Returns true if the cursor state collection entry is valid.  Returns false if the cursor state
             *         collection entry is invalid.
             */
            bool isValid() const;

            /**
             * Method that determines if this cursor state collection entry is invalid.
             *
             * \return Returns true if the cursor state collection entry is invalid.  Returns false if the cursor state
             *         collection entry is valid.
             */
            bool isInvalid() const;

            /**
             * Method you can use to determine if this cursor state collection entry points to a whole element.
             *
             * \return Returns true if this instance points to an entire element.  Returns false if this instance points
             *         to an individual character in an element.
             */
            bool isWholeElement() const;

            /**
             * Method you can use to determine if this cursor state collection entry points to text in an element.
             *
             * \return Returns true if this instance points to text in an element.  Returns false if this instance
             *         points to an entire element.
             */
            bool isTextInElement() const;

            /**
             * Method you can use to determine the type of entry being pointed to.
             *
             * \return Returns the position type being pointed to by this instance.
             */
            PositionType positionType() const;

            /**
             * Method you can use to determine the element this instance points to.
             *
             * \return Returns a shared pointer to the element being pointed to.
             */
            ElementPointer element() const;

            /**
             * Method you can use to determine the parent of the element this instance points to.
             *
             * \return Returns a shared pointer to the element's parent.
             */
            ElementPointer parent() const;

            /**
             * Method you can use to determine the current index of this child under the parent.
             *
             * \return Returns the child index of this element.  A value of \ref Element::invalidChildIndex will be
             *         returned if the currently has no parent or if the child index is otherwise invalid.
             */
            unsigned long childIndex() const;

            /**
             * Method you can use to obtain the text index associated with this cursor state.
             *
             * \return Returns the text index tied to this cursor state.  The value \ref Element::invalidTextIndex is
             *         returned if the cursor state points to an entire element, a null element, or is invalid.
             */
            unsigned long textIndex() const;

            /**
             * Method you can use to obtain the region index assocaited with this cursor state.
             *
             * \return Returns the region index tied to this cursor state.  The value \ref Element::invalidRegionIndex
             *         is returned if the cursor points to an entire element, a null element, or is invalid.
             */
            unsigned regionIndex() const;

            /**
             * Method you can use to invalidate this cursor.
             */
            void invalidate();

            /**
             * Method you can use to update this instance directly from an element cursor.
             *
             * \param[in] elementCursor The element cursor to use to update this entry.
             */
            void update(const ElementCursor& elementCursor);

            /**
             * Method you can use to update this instance to point to an entire element.
             *
             * \param[in] element       A pointer to the desired element.
             *
             * \param[in] preferForward If true, the cursor should be adjusted forward if the element can not be
             *                          parked on.  If false, the cursor should be adjusted backwards if the cursor can
             *                          not be parked on.
             */
            void update(ElementPointer element, bool preferForward);

            /**
             * Method you can use to update this instance to point to a child position under a parent.
             *
             * \param[in] parent        A pointer to the parent instance.
             *
             * \param[in] childIndex    The zero based index of the element under the parent.
             *
             * \param[in] preferForward If true, the cursor should be adjusted forward if the element can not be
             *                          parked on.  If false, the cursor should be adjusted backwards if the cursor can
             *                          not be parked on.
             */
            void update(ElementPointer parent, unsigned long childIndex, bool preferForward);

            /**
             * Method you can use to update this instance to point to a text position in an element in region 0.
             *
             * \param[in] textIndex     The text index where the cursor should now point.
             *
             * \param[in] element       A pointer to the element.
             */
            void update(unsigned long textIndex, ElementPointer element);

            /**
             * Method you can use to update this instance to point to a text position in an element in an arbitrary
             * region.
             *
             * \param[in] textIndex   The text index where the cursor should now point.
             *
             * \param[in] regionIndex The zero based region index to point into.
             *
             * \param[in] element     A pointer to the element.
             */
            void update(unsigned long textIndex, unsigned regionIndex, ElementPointer element);

            /**
             * Method you can use to update this instance to point to the first position of an element.  The entire
             * element will be selected if the element does not support text.
             *
             * \param[in] element       A pointer to the desired element.
             *
             * \param[in] preferForward If true, the cursor should be adjusted forward if the element can not be
             *                          parked on.  If false, the cursor should be adjusted backwards if the cursor can
             *                          not be parked on.
             */
            void updateToFirstPositionIn(ElementPointer element, bool preferForward);

            /**
             * Method you can use to update this instance to point to the last position of an element.  The entire
             * element will be selected if the element does not support text.
             *
             * \param[in] element       A pointer to the desired element.
             *
             * \param[in] preferForward If true, the cursor should be adjusted forward if the element can not be
             *                          parked on.  If false, the cursor should be adjusted backwards if the cursor can
             *                          not be parked on.
             */
            void updateToLastPositionIn(ElementPointer element, bool preferForward);

            /**
             * Method you can use to convert this instance to a \ref Ld::ElementCursor instance.
             *
             * \return Returns an element cursor instance.
             */
            ElementCursor toElementCursor() const;

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be assigned to this.
             *
             * \return Returns a reference to this instance.
             */
            CursorStateCollectionEntry& operator=(const CursorStateCollectionEntry& other);

            /**
             * Comparison operator
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the values are equal.  Returns false if the values are different.
             */
            bool operator==(const CursorStateCollectionEntry& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the values are different.  Returns false if the values are equal.
             */
            bool operator!=(const CursorStateCollectionEntry& other) const;

        private:
            /**
             * Private implementation base class.
             */
            class Private;

            /**
             * Private invalid entry class.
             */
            class Invalid;

            /**
             * Private whole element entry class.
             */
            class WholeElement;

            /**
             * Private null entry class.
             */
            class NullEntry;

            /**
             * Private text location entry class.
             */
            class TextLocation;

            /**
             * The underlying shared data pointer.
             */
            QSharedPointer<Private> impl;
    };

    /**
     * Function that calculates a hash for a \ref Ld::CursorStateCollectionEntry.
     *
     * \param[in] key  The key to calculate a hash for.
     *
     * \param[in] seed An optional seed used to add entropy to the hash.
     */
    LD_PUBLIC_API Util::HashResult qHash(const CursorStateCollectionEntry& key, Util::HashSeed seed = 0);
}

#endif
