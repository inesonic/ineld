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
* This header defines the Ld::CursorStateCollectionEntry::Private base class and several derived classes class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CURSOR_STATE_COLLECTION_ENTRY_PRIVATE_H
#define LD_CURSOR_STATE_COLLECTION_ENTRY_PRIVATE_H

#include <QSharedPointer>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_element_cursor.h"
#include "ld_cursor_state_collection_entry.h"

namespace Ld {
    /**
     * Pure virtual private implementation base class for the \ref Ld::CursorStateCollectionEntry class.
     */
    class LD_PUBLIC_API CursorStateCollectionEntry::Private {
        public:
            virtual ~Private() = default;

            /**
             * Method that determines if this cursor state collection entry is valid.
             *
             * \return Returns true if the cursor state collection entry is valid.  Returns false if the cursor state
             *         collection entry is invalid.
             */
            virtual bool isValid() const = 0;

            /**
             * Method you can use to determine if this cursor state collection entry points to a whole element.
             *
             * \return Returns true if this instance points to an entire element.  Returns false if this instance points
             *         to an individual character in an element.
             */
            virtual bool isWholeElement() const = 0;

            /**
             * Method you can use to determine if this cursor state collection entry points to text in an element.
             *
             * \return Returns true if this instance points to text in an element.  Returns false if this instance
             *         points to an entire element.
             */
            virtual bool isTextInElement() const = 0;

            /**
             * Method you can use to determine the type of entry being pointed to.
             *
             * \return Returns the position type being pointed to by this instance.
             */
            virtual CursorStateCollectionEntry::PositionType positionType() const = 0;

            /**
             * Method you can use to determine the element this instance points to.
             *
             * \return Returns a shared pointer to the element being pointed to.
             */
            virtual ElementPointer element() const = 0;

            /**
             * Method you can use to determine the parent of the element this instance points to.
             *
             * \return Returns a shared pointer to the element's parent.
             */
            virtual ElementPointer parent() const = 0;

            /**
             * Method you can use to determine the current index of this child under the parent.
             *
             * \return Returns the child index of this element.  A value of \ref Element::invalidChildIndex will be
             *         returned if the currently has no parent or if the child index is otherwise invalid.
             */
            virtual unsigned long childIndex() const = 0;

            /**
             * Method you can use to obtain the text index associated with this cursor state.
             *
             * \return Returns the text index tied to this cursor state.  The value \ref Element::invalidTextIndex is
             *         returned if the cursor state points to an entire element, a null element, or is invalid.
             */
            virtual unsigned long textIndex() const = 0;

            /**
             * Method you can use to obtain the region index assocaited with this cursor state.
             *
             * \return Returns the region index tied to this cursor state.  The value \ref Element::invalidRegionIndex
             *         is returned if the cursor points to an entire element, a null element, or is invalid.
             */
            virtual unsigned regionIndex() const = 0;

            /**
             * Method you can use to convert this instance to a \ref Ld::ElementCursor instance.
             *
             * \return Returns an element cursor instance.
             */
            virtual ElementCursor toElementCursor() const = 0;

            /**
             * Comparison operator
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the values are equal.  Returns false if the values are different.
             */
            virtual bool operator==(const CursorStateCollectionEntry::Private& other) const = 0;
    };

    /**
     * Class used to track an invalid cursor entry.
     */
    class LD_PUBLIC_API CursorStateCollectionEntry::Invalid:public CursorStateCollectionEntry::Private {
        public:
            /**
             * Constructor.
             */
            Invalid();

            ~Invalid() override;

            /**
             * Method that determines if this cursor state collection entry is valid.
             *
             * \return Returns true if the cursor state collection entry is valid.  Returns false if the cursor state
             *         collection entry is invalid.  This implementation always returns false.
             */
            bool isValid() const override;

            /**
             * Method you can use to determine if this cursor state collection entry points to a whole element.
             *
             * \return Returns true if this instance points to an entire element.  Returns false if this instance points
             *         to an individual character in an element.  This implementation always returns false.
             */
            bool isWholeElement() const override;

            /**
             * Method you can use to determine if this cursor state collection entry points to text in an element.
             *
             * \return Returns true if this instance points to text in an element.  Returns false if this instance
             *         points to an entire element.  This implementation always returns false.
             */
            bool isTextInElement() const override;

            /**
             * Method you can use to determine the type of entry being pointed to.
             *
             * \return Returns the position type being pointed to by this instance.  This implementation always returns
             *         \ref Ld::CursorStateCollectionEntry::PositionType::INVALID.
             */
            CursorStateCollectionEntry::PositionType positionType() const override;

            /**
             * Method you can use to determine the element this instance points to.
             *
             * \return Returns a shared pointer to the element being pointed to.  This implementation always returns a
             *         null pointer.
             */
            ElementPointer element() const override;

            /**
             * Method you can use to determine the parent of the element this instance points to.
             *
             * \return Returns a shared pointer to the element's parent.  This implementation always returns a null
             *         pointer.
             */
            ElementPointer parent() const override;

            /**
             * Method you can use to determine the current index of this child under the parent.
             *
             * \return Returns the child index of this element.  A value of \ref Element::invalidChildIndex will be
             *         returned if the currently has no parent or if the child index is otherwise invalid.  This
             *         implementation always returns \ref Element::invalidChildIndex.
             */
            unsigned long childIndex() const override;

            /**
             * Method you can use to obtain the text index associated with this cursor state.
             *
             * \return Returns the text index tied to this cursor state.  The value \ref Element::invalidTextIndex is
             *         returned if the cursor state points to an entire element, a null element, or is invalid.  This
             *         implementation always returns \ref Element::invalidTextIndex.
             */
            unsigned long textIndex() const override;

            /**
             * Method you can use to obtain the region index assocaited with this cursor state.
             *
             * \return Returns the region index tied to this cursor state.  The value \ref Element::invalidRegionIndex
             *         is returned if the cursor points to an entire element, a null element, or is invalid.  This
             *         implementation always returns \ref Element::invalidRegionIndex.
             */
            unsigned regionIndex() const override;

            /**
             * Method you can use to convert this instance to a \ref Ld::ElementCursor instance.
             *
             * \return Returns an element cursor instance.  This implementation always returns an invalid cursor
             *         instance.
             */
            ElementCursor toElementCursor() const override;

            /**
             * Comparison operator
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the values are equal.  Returns false if the values are different.  This
             *         implementation returns true only if the other entry is invalid.
             */
            bool operator==(const CursorStateCollectionEntry::Private& other) const override;
    };

    /**
     * Class used to track a cursor pointing to a whole element.
     */
    class LD_PUBLIC_API CursorStateCollectionEntry::WholeElement:public CursorStateCollectionEntry::Invalid {
        public:
            /**
             * Constructor
             *
             * \param[in] element The element tracked by this instance.
             */
            WholeElement(ElementPointer element);

            ~WholeElement() override;

            /**
             * Method that determines if this cursor state collection entry is valid.
             *
             * \return Returns true if the cursor state collection entry is valid.  Returns false if the cursor state
             *         collection entry is invalid.  This implementation always returns true.
             */
            bool isValid() const override;

            /**
             * Method you can use to determine if this cursor state collection entry points to a whole element.
             *
             * \return Returns true if this instance points to an entire element.  Returns false if this instance points
             *         to an individual character in an element.  This implementation always returns true.
             */
            bool isWholeElement() const override;

            /**
             * Method you can use to determine the type of entry being pointed to.
             *
             * \return Returns the position type being pointed to by this instance.  This implementation always returns
             *         \ref Ld::CursorStateCollectionEntry::PositionType::ELEMENT.
             */
            CursorStateCollectionEntry::PositionType positionType() const override;

            /**
             * Method you can use to determine the element this instance points to.
             *
             * \return Returns a shared pointer to the element being pointed to.  This implementation returns a pointer
             *         to the requested element.
             */
            ElementPointer element() const override;

            /**
             * Method you can use to determine the parent of the element this instance points to.
             *
             * \return Returns a shared pointer to the element's parent.  This implementation returns a pointer to the
             *         element's parent.
             */
            ElementPointer parent() const override;

            /**
             * Method you can use to determine the current index of this child under the parent.
             *
             * \return Returns the child index of this element.  A value of \ref Element::invalidChildIndex will be
             *         returned if the currently has no parent or if the child index is otherwise invalid.  This
             *         implementation always returns \ref Element::invalidChildIndex.
             */
            unsigned long childIndex() const override;

            /**
             * Method you can use to convert this instance to a \ref Ld::ElementCursor instance.
             *
             * \return Returns an element cursor instance.  This implementation always returns an invalid cursor
             *         instance.
             */
            ElementCursor toElementCursor() const override;

            /**
             * Comparison operator
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the values are equal.  Returns false if the values are different.  This
             *         implementation returns true only if the other entry points to the same element and both
             *         entries point to whole element instances.
             */
            bool operator==(const CursorStateCollectionEntry::Private& other) const override;

        private:
            /**
             * Pointer to the underlying element.
             */
            ElementPointer currentElement;
    };

    /**
     * Class used to track a cursor pointing to a null entry.
     */
    class LD_PUBLIC_API CursorStateCollectionEntry::NullEntry:public CursorStateCollectionEntry::Invalid {
        public:
            /**
             * Constructor
             *
             * \param[in] parent     The parent element holding the null entry.
             *
             * \param[in] childIndex The index into the parent where the entry resides.
             */
            NullEntry(ElementPointer parent, unsigned long childIndex);

            ~NullEntry() override;

            /**
             * Method that determines if this cursor state collection entry is valid.
             *
             * \return Returns true if the cursor state collection entry is valid.  Returns false if the cursor state
             *         collection entry is invalid.  This implementation always returns true.
             */
            bool isValid() const override;

            /**
             * Method you can use to determine if this cursor state collection entry points to a whole element.
             *
             * \return Returns true if this instance points to an entire element.  Returns false if this instance points
             *         to an individual character in an element.  This implementation always returns true.
             */
            bool isWholeElement() const override;

            /**
             * Method you can use to determine the type of entry being pointed to.
             *
             * \return Returns the position type being pointed to by this instance.  This implementation always returns
             *         \ref Ld::CursorStateCollectionEntry::PositionType::ELEMENT.
             */
            CursorStateCollectionEntry::PositionType positionType() const override;

            /**
             * Method you can use to determine the parent of the element this instance points to.
             *
             * \return Returns a shared pointer to the element's parent.  This implementation returns a pointer to the
             *         element's parent.
             */
            ElementPointer parent() const override;

            /**
             * Method you can use to determine the current index of this child under the parent.
             *
             * \return Returns the child index of this element.  A value of \ref Element::invalidChildIndex will be
             *         returned if the currently has no parent or if the child index is otherwise invalid.  This
             *         implementation always returns \ref Element::invalidChildIndex.
             */
            unsigned long childIndex() const override;

            /**
             * Method you can use to convert this instance to a \ref Ld::ElementCursor instance.
             *
             * \return Returns an element cursor instance.  This implementation always returns an invalid cursor
             *         instance.
             */
            ElementCursor toElementCursor() const override;

            /**
             * Comparison operator
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the values are equal.  Returns false if the values are different.  This
             *         implementation returns true only if the other entry points to the same element and both
             *         entries point to whole element instances.
             */
            bool operator==(const CursorStateCollectionEntry::Private& other) const override;

        private:
            /**
             * Pointer to the underlying parent.
             */
            ElementPointer currentParent;

            /**
             * The child index under this parent.
             */
            unsigned long currentChildIndex;
    };

    /**
     * Class used to track a cursor pointing to a text location in an element.
     */
    class LD_PUBLIC_API CursorStateCollectionEntry::TextLocation:public CursorStateCollectionEntry::WholeElement {
        public:
            /**
             * Constructor
             *
             * \param[in] element     The element where the cursor points into.
             *
             * \param[in] textIndex   The text index where the cursor points.
             *
             * \param[in] regionIndex The region index where the cursor points.
             */
            TextLocation(ElementPointer element, unsigned long textIndex, unsigned regionIndex);

            ~TextLocation() override;

            /**
             * Method you can use to determine if this cursor state collection entry points to a whole element.
             *
             * \return Returns true if this instance points to an entire element.  Returns false if this instance points
             *         to an individual character in an element.  This implementation always returns false.
             */
            bool isWholeElement() const override;

            /**
             * Method you can use to determine if this cursor state collection entry points to text in an element.
             *
             * \return Returns true if this instance points to text in an element.  Returns false if this instance
             *         points to an entire element.  This implementation always returns true.
             */
            bool isTextInElement() const override;

            /**
             * Method you can use to determine the type of entry being pointed to.
             *
             * \return Returns the position type being pointed to by this instance.  This implementation always returns
             *         \ref Ld::CursorStateCollectionEntry::PositionType::TEXT.
             */
            CursorStateCollectionEntry::PositionType positionType() const override;

            /**
             * Method you can use to obtain the text index associated with this cursor state.
             *
             * \return Returns the text index tied to this cursor state.  The value \ref Element::invalidTextIndex is
             *         returned if the cursor state points to an entire element, a null element, or is invalid.
             */
            unsigned long textIndex() const override;

            /**
             * Method you can use to obtain the region index assocaited with this cursor state.
             *
             * \return Returns the region index tied to this cursor state.  The value \ref Element::invalidRegionIndex
             *         is returned if the cursor points to an entire element, a null element, or is invalid.
             */
            unsigned regionIndex() const override;

            /**
             * Method you can use to convert this instance to a \ref Ld::ElementCursor instance.
             *
             * \return Returns an element cursor instance.  This implementation always returns an invalid cursor
             *         instance.
             */
            ElementCursor toElementCursor() const override;

            /**
             * Comparison operator
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the values are equal.  Returns false if the values are different.  This
             *         implementation returns true only if the other entry points to the same element and both
             *         entries point to whole element instances.
             */
            bool operator==(const CursorStateCollectionEntry::Private& other) const override;

        private:
            /**
             * The current text index.
             */
            unsigned long currentTextIndex;

            /**
             * The current region index.
             */
            unsigned currentRegionIndex;
    };
}

#endif
