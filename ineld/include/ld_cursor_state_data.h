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
* This header defines the \ref Ld::CursorStateData class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CURSOR_STATE_DATA_H
#define LD_CURSOR_STATE_DATA_H

#include <QSharedPointer>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_element_cursor.h"
#include "ld_cursor.h"

namespace Ld {
    /**
     * Class used by \ref Ld::CursorStateCollection to store information about a specific cursor.  The class is
     * is designed to allow cursor state information to be preserved through updates to the element tree.
     *
     * You should either instantiate instances using the constructor to capture the cursor state data or you should
     * call \ref Ld::CursorStateData::captureState to capture the cursor state.  You can then call the "updateState"
     * methods one or more times as you manipulate the element tree to keep the cursor data in sync.  When done, you
     * can call the \ref Ld::CursorStateData::restoreState methods to restore the cursors using the new information.
     *
     * Note that the \ref Ld::CursorStateCollection class that integrates these functions into a QHash allowing you to
     * more easily manipulate multiple cursors at once.
     */
    class LD_PUBLIC_API CursorStateData {
        public:
            CursorStateData();

            /**
             * Constructor
             *
             * \param[in] cursor The cursor to be tracked by this instance.
             */
            CursorStateData(CursorPointer cursor);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            CursorStateData(const CursorStateData& other);

            ~CursorStateData();

            /**
             * Method that determines if this cursor state data is valid.
             *
             * \return Returns true if the cursor state data is valid.  Returns false if the cursor state data is
             *         invalid.
             */
            bool isValid() const;

            /**
             * Method that determines if this cursor state data is invalid.
             *
             * \return Returns true if the cursor state data is invalid.  Returns false if the cursor state data is
             *         valid.
             */
            bool isInvalid() const;

            /**
             * Method you can call to update the underlying cursor state data.  You can use this method to perform
             * multiple cursor position update stages using the same structure.
             *
             * \param[in] cursor The cursor to capture the state data for.
             */
            void captureState(CursorPointer cursor);

            /**
             * Method you can call to restore the underlying cursor state.  This version assumes no changes so cursor
             * states are exactly as they were when captured.
             *
             * \param[in] cursor            The cursor that should be restored using this state information.
             *
             * \param[in] implicitlyChanged If true, this call was triggered by user activity at another cursor.  If
             *                              false, this call was triggered by user activity at this cursor position.
             */
            void restoreState(CursorPointer cursor, bool implicitlyChanged);

            /**
             * Method you can call to update this cursor based on newly inserted text.
             *
             * \param[in] element         The element where the insertion occurred.
             *
             * \param[in] textIndex       The text index prior to the insertion (or removal) operation.
             *
             * \param[in] regionIndex     The region index of the text region where the operation was performed.
             *
             * \param[in] insertionLength The number of newly inserted characters.
             */
            void updateAfterTextInsertion(
                ElementPointer element,
                unsigned long  textIndex,
                unsigned       regionIndex,
                unsigned long  insertionLength = 1
            );

            /**
             * Method you can call to update this cursor based on deleted text.
             *
             * \param[in] element      The element where the insertion occurred.
             *
             * \param[in] textIndex    The text index prior to the insertion (or removal) operation.
             *
             * \param[in] regionIndex  The region index of the text region where the operation was performed.
             *
             * \param[in] deletedCount The number of characters that were deleted.
             */
            void updateAfterTextDeleted(
                ElementPointer element,
                unsigned long  textIndex,
                unsigned       regionIndex,
                unsigned long  deletedCount = 1
            );

            /**
             * Method you can call to update this cursor based on a frame, such as a paragraph, being split.  Call this
             * method after the split is performed.
             *
             * \param[in] oldElement  The element where the split was performed.
             *
             * \param[in] newElement  The new element containing text after the split.
             */
            void updateAfterSplit(ElementPointer oldElement, ElementPointer newElement);

            /**
             * Method you can call to update this cursor based on an element being deleted.  Unlike other methods, you
             * should call this element before the element is deleted.
             *
             * \param[in] element               The element that is about to be deleted.
             *
             * \param[in] preferEarlierPosition If true, cursors should prefer an earlier position.  If false, cursors
             *                                  should prefer a later position.
             */
            void updateBeforeElementDelete(ElementPointer element, bool preferEarlierPosition);

            /**
             * Method you can call to update this cursor based on a selected range being deleted.  Unlike other
             * methods, you should call this method before the range is deleted.
             *
             * \param[in] startingCursor The starting cursor for the range.
             *
             * \param[in] endingCursor   The ending cursor for the range.
             */
            void updateBeforeRangeDelete(const ElementCursor& startingCursor, const ElementCursor& endingCursor);

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be assigned to this.
             *
             * \return Returns a reference to this instance.
             */
            CursorStateData& operator=(const CursorStateData& other);

            /**
             * Comparison operator
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the values are equal.  Returns false if the values are different.
             */
            bool operator==(const CursorStateData& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the values are different.  Returns false if the values are equal.
             */
            bool operator!=(const CursorStateData& other) const;

        private:
            /**
             * Private class used to manage state data and updates for a single \ref Ld::ElementCursor instance.
             */
            class CursorData {
                public:
                    CursorData();

                    /**
                     * Constructor
                     *
                     * \param[in] elementCursor The element cursor that we need to track.
                     */
                    CursorData(const ElementCursor& elementCursor);

                    /**
                     * Copy constructor
                     *
                     * \param[in] other The instance to be copied.
                     */
                    CursorData(const CursorData& other);

                    ~CursorData();

                    /**
                     * Method you can use to determine if this cursor data is valid.
                     *
                     * \return Returns true if the cursor data is valid.  Returns false if the cursor data is invalid.
                     */
                    bool isValid() const;

                    /**
                     * Method you can use to determine if this cursor data is invalid.
                     *
                     * \return Returns true if the cursor data is invalid.  Returns false if the cursor data is valid.
                     */
                    bool isInvalid() const;

                    /**
                     * Method you can call to obtain a properly adjusted element cursor instance.
                     *
                     * \return Returns an adjusted element cursor instanace.
                     */
                    ElementCursor toElementCursor() const;

                    /**
                     * Method you can call to obtain a properly adjusted element cursor instance.  This version will
                     * adjust the element cursor based on newly inserted or removed text.
                     *
                     * \param[in] element         The element where the insertion occurred.
                     *
                     * \param[in] textIndex       The text index prior to the insertion (or removal) operation.
                     *
                     * \param[in] regionIndex     The region index of the text region where the operation was
                     *                            performed.
                     *
                     * \param[in] insertionLength The number of newly inserted characters.
                     */
                    void updateAfterTextInsertion(
                        ElementPointer element,
                        unsigned long  textIndex,
                        unsigned       regionIndex,
                        unsigned long  insertionLength = 1
                    );

                    /**
                     * Method you can call to obtain a properly adjusted element cursor instance.  This version will
                     * adjust the element cursor based on one or more characters being deleted.
                     *
                     * \param[in] element      The element where the insertion occurred.
                     *
                     * \param[in] textIndex    The text index prior to the insertion (or removal) operation.
                     *
                     * \param[in] regionIndex  The region index of the text region where the operation was performed.
                     *
                     * \param[in] deletedCount The number of characters that were deleted.
                     */
                    void updateAfterTextDeleted(
                        ElementPointer element,
                        unsigned long  textIndex,
                        unsigned       regionIndex,
                        unsigned long  deletedCount = 1
                    );

                    /**
                     * Method you can call to obtain a properly adjusted element cursor instance.  This version will
                     * adjust the element cursor based on a text field split across frames.
                     *
                     * The default implementation simply returns.
                     *
                     * \param[in] oldElement  The element where the split was performed.
                     *
                     * \param[in] newElement  The new element containing text after the split.
                     */
                    void updateAfterSplit(ElementPointer oldElement, ElementPointer newElement);

                    /**
                     * Method you can call to obtain a properly adjusted element cursor instance.  This version will
                     * adjust the element cursor based on the assumption that an element is about to be deleted.
                     *
                     * \param[in] element               The element that is about to be deleted.
                     *
                     * \param[in] preferEarlierPosition If true, cursors should prefer an earlier position.  If false,
                     *                                  cursors should prefer a later position.
                     */
                    void updateBeforeElementDelete(ElementPointer element, bool preferEarlierPosition);

                    /**
                     * Method you can call to update this cursor based on a selected range being deleted.
                     *
                     * \param[in] startingCursor          The starting cursor for the range.  This cursor must always
                     *                                    point to the earlier point in the range being deleted.
                     *
                     * \param[in] endingCursor            The ending cursor for the range.  This cursor must always
                     *                                    point to the later point in the range being deleted.
                     */
                    void updateBeforeRangeDelete(
                        const ElementCursor& startingCursor,
                        const ElementCursor& endingCursor
                    );

                    /**
                     * Assignment operator
                     *
                     * \param[in] other The instance to be copied.
                     *
                     * \return Returns a reference to this instance.
                     */
                    CursorData& operator=(const CursorData& other);

                    /**
                     * Comparison operator
                     *
                     * \param[in] other The instance to be compared against.
                     *
                     * \return Returns true if the values are equal.  Returns false if the values are different.
                     */
                    bool operator==(const CursorData& other) const;

                    /**
                     * Comparison operator
                     *
                     * \param[in] other The instance to be compared against.
                     *
                     * \return Returns true if the values are different.  Returns false if the values are equal.
                     */
                    bool operator!=(const CursorData& other) const;

                private:
                    /**
                     * Method that invalidates this instance.
                     */
                    void invalidate();

                    /**
                     * Method that updates this instance from an element cursor.
                     *
                     * \param[in] elementCursor The element cursor to update the class from.
                     */
                    void updateFromElementCursor(const ElementCursor& elementCursor);

                    /**
                     * A shared pointer to the element or parent of the element being tracked.
                     */
                    ElementPointer currentElement;

                    /**
                     * The child index of the child under the parent.  A value of \ref Ld::Element::invalidChildIndex
                     * indicates that Ld::CursorStateData::CursorState::currentElement points to the element rather
                     * than to the element's parent.
                     */
                    unsigned long currentChildIndex;

                    /**
                     * The zero based region index into the element.  A value of \ref Ld::Element::invalidRegionIndex
                     * indicates that we're tracking an entire element.
                     */
                    unsigned currentRegionIndex;

                    /**
                     * The zero based text index into the element.  A value of \ref Ld::Element::invalidTextIndex
                     * indicates that we're tracking an entire element.
                     */
                    unsigned long currentTextIndex;
            };

            /**
             * Private instance used to track and restore the state of the element cursor.
             */
            CursorData elementCursorData;

            /**
             * Private instance used to track and restore the state of the selection cursor.
             */
            CursorData selectionCursorData;
    };
}

#endif
