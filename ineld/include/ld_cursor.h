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
* This header defines the \ref Ld::Cursor class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CURSOR_H
#define LD_CURSOR_H

#include <QSharedPointer>
#include <QWeakPointer>
#include <QRegularExpression>
#include <QMap>

#include <cstdint>

#include <util_hash_functions.h>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_location.h"
#include "ld_capabilities.h"
#include "ld_element_cursor.h"

namespace Ld {
    class RootElement;
    class Format;
    class Cursor;
    class CursorStateCollection;

    /**
     * Type used to point to an \ref Ld::Cursor instance.
     */
    typedef QSharedPointer<Cursor> CursorPointer;

    /**
     * Type you can use as a weak pointer to a \ref Ld::Cursor instance.
     */
    typedef QWeakPointer<Cursor> CursorWeakPointer;

    /**
     * This class provides minimal logic that would typically be used for an application cursor and exists so that
     * cursors can be paired to represent the bounds of a selection.  The class also provides basic functions to move
     * the main "element" cursor through the element tree and to perform searches of the element tree for strings.
     *
     *   The class extends the
     * capabilities of the \ref Ld::ElementCursor classes to provide basic support selection ranges and the ability to
     * selectively modify cursor movements through \ref Ld::Element classes that would normally not accept user
     * activity.
     *
     * Internally the class manages an element cursor used to indicate the location of a user cursor.  A second cursor
     * can updated with the current cursor position to mark and record the starting location for a selection.  As a
     * convenience, a number of methods include a "selection" parameter that can be used to update the selection cursor
     * before the active cursor is changed.  The selection cursor will be set if "selection" is true on this call and
     * unset in the previous call.  Similarly, the selection cursor will be cleared if "selection" is fall in this call
     * and set in the previous call.
     *
     * If the selection cursor is valid and different from the current position then the cursor type will be reported
     * as "Ld::Cursor::Type::SELECTION."
     *
     * Note that, by design, no attempt to interpret or modify the selection cursor is provided.
     *
     * Lastly, the class includes search functions that can be used to search forwards or backwards for a given string.
     */
    class LD_PUBLIC_API Cursor {
        public:
            /**
             * Enumeration of supported search results.
             */
            enum class SearchResult {
                /**
                 * Indicates the search string was found.
                 */
                FOUND,

                /**
                 * Indicates the search string was not found.
                 */
                NOT_FOUND,

                /**
                 * Indicates an invalid starting cursor position.
                 */
                INVALID_STARTING_POSITION,

                /**
                 * Indicates an invalid regular expression.
                 */
                INVALID_REGULAR_EXPRESSION
            };

            /**
             * Constructor
             *
             * \param[in] newRootElement Pointer to the root element.
             */
            Cursor(QWeakPointer<RootElement> newRootElement = QWeakPointer<RootElement>());

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            Cursor(const Cursor& other);

            virtual ~Cursor();

            /**
             * Method you can use to set a weak pointer to the root element tracked by this cursor.
             *
             * \param[in] newRootElement The new weak pointer to the root element to be tracked.
             */
            void setRootElement(QWeakPointer<RootElement> newRootElement);

            /**
             * Method you can use to obtain a weak pointer to the root element.
             *
             * \return Returns a weak pointer to the root element.
             */
            QWeakPointer<RootElement> rootElement() const;

            /**
             * Method you can use to query the requirements for insertion at the current cursor position.
             *
             * \return Returns the requirements for insertion at the current cursor position.
             */
            Capabilities requirementsAtCursor() const;

            /**
             * Method you can use to obtain the element cursor position.
             *
             * \return Returns the calculated \ref Ld::ElementCursor instance representing the cursor position.
             */
            const ElementCursor& elementCursor() const;

            /**
             * Method you can use to obtain an element cursor representing the other end of a selected area.
             *
             * \return Returns the calculated \ref Ld::ElementCursor instance representing the other end of the selected
             *         area.  If there is no selection, this method will return an invalid element cursor.
             */
            const ElementCursor& selectionCursor() const;

            /**
             * Method you can use to update the cursor directly from an \ref Ld::ElementCursor instance.
             *
             * \param[in] newPosition   The new element cursor position.
             *
             * \param[in] keepSelection If true, this method will not clear the active selection unless the starting
             *                          and ending positions match.
             */
            void updateFromElementCursor(const ElementCursor& newPosition, bool keepSelection = false);

            /**
             * Method you can use to update the selection cursor directly from an \ref Ld::ElementCursor instance.
             *
             * \param[in] newPosition The new selection position.  An invalid position will disable the selection.
             */
            void updateSelectionFromElementCursor(const ElementCursor& newPosition);

            /**
             * Method you can use to move the cursor forward by an element.
             *
             * \return Returns true the cursor was moved.  Returns false if the cursor was already at the end of the
             *         document view.
             */
            bool moveForwardByElement();

            /**
             * Method you can use to move the cursor backward by an element.
             *
             * \return Returns true if the cursor was moved.  Returns false if the cursor was already at the beginning
             *         of the document view.
             */
            bool moveBackwardByElement();

            /**
             * Method you can use to move the cursor forward by a single region in an element.
             *
             * \return Returns true the cursor was moved.  Returns false if the cursor was already at the end of the
             *         document view.
             */
            bool moveForwardByRegion();

            /**
             * Method you can use to move the cursor backward by a single region in an element.
             *
             * \return Returns true if the cursor was moved.  Returns false if the cursor was already at the beginning
             *         of the document view.
             */
            bool moveBackwardByRegion();

            /**
             * Method you can use to move the cursor forward by whitespace, then by regions, then by elements.
             *
             * \return Returns true if the cursor was moved.  Returns false if the cursor was already at the end of the
             *         document view.
             */
            bool moveForwardByWhitespace();

            /**
             * Method you can use to move backward by whitespace, then by region, then by element.
             *
             * \return Returns true if the cursor was moved.  Returns false if the cursor was already at the beginning
             *         of the document view.
             */
            bool moveBackwardByWhitespace();

            /**
             * Method you can use to move forward by character, then by region, then by element.
             *
             * \return Returns true if the cursor was moved.  Returns false if the cursor was already at the end of the
             *         document view.
             */
            bool moveForwardByCharacter();

            /**
             * Method you can use to move backward by character, then by region, then by element.
             *
             * \return Returns true if the cursor was moved.  Returns false if the cursor was already at the beginning
             *         of the document view.
             */
            bool moveBackwardByCharacter();

            /**
             * Method you can call to move the cursor to the start of the current document.
             *
             * \return Returns true if the cursor was moved.  Returns false if the cursor was already at the start of
             *         the document.
             */
            bool moveToStartOfDocumentView();

            /**
             * Method you can call to move the cursor to the end of the current document.
             *
             * \return Returns true if the cursor was moved.  Returns false if the cursor was already at the start of
             *         the document.
             */
            bool moveToEndOfDocumentView();

            /**
             * Method you can use to explicitly start a selection operation.
             */
            void startSelection();

            /**
             * Method you can use to clear the current selection.
             */
            void clearSelection();

            /**
             * Method you can use to select the entire document view contents.
             */
            void selectAll();

            /**
             * Method you can use to search forward for a string from the current location.
             *
             * \param[in] searchText        The search text to be located.
             *
             * \param[in] caseSensitive     If true, the search will require an exact case match.  If false, the search
             *                              will ignore case.
             *
             * \param[in] wholeWordsOnly    If true, the search will require that the search string represent an entire
             *                              word or group of words.  Strings making up a part of a word will be
             *                              ignored.
             *
             * \param[in] regularExpression If true, the search string will be treated as a regular expression.  If
             *                              false, the search string will be treated as normal text.
             *
             * \param[in] okToLoop          If true, the search will restart at the beginning.  If false, the search
             *                              will end at the end of the element tree.
             *
             * \return Returns the result from the search.
             */
            SearchResult searchForward(
                const QString& searchText,
                bool           caseSensitive,
                bool           wholeWordsOnly,
                bool           regularExpression,
                bool           okToLoop = true
            );

            /**
             * Method you can use to search backward for a string from the current location.
             *
             * \param[in] searchText        The search text to be located.
             *
             * \param[in] caseSensitive     If true, the search will require an exact case match.  If false, the search
             *                              will ignore case.
             *
             * \param[in] wholeWordsOnly    If true, the search will require that the search string represent an entire
             *                              word or group of words.  Strings making up a part of a word will be
             *                              ignored.
             *
             * \param[in] regularExpression If true, the search string will be treated as a regular expression.  If
             *                              false, the search string will be treated as normal text.
             *
             * \param[in] okToLoop          If true, the search will restart at the end.  If false, the search
             *                              will end at the beginning of the element tree.
             *
             * \return Returns the result from the search.
             */
            SearchResult searchBackward(
                const QString& searchText,
                bool           caseSensitive,
                bool           wholeWordsOnly,
                bool           regularExpression,
                bool           okToLoop = true
            );

            /**
             * Method you can use to obtain a list of every element at the given cursor position.  The returned list
             * will vary depending on whether or not there is a selection.
             *
             * \param[in] includeAncestors If true, we will include ancestors.  If false, ancestors will be excluded.
             *
             * \return Returns a list of every element under the cursor.  If there is a selection, the method will
             *         return every selected element in arbitrary order.  If there is no selection, the method will
             *         return the stack of elements starting at the root element and working downward.
             */
            Ld::ElementPointerList elementsUnderCursor(bool includeAncestors = true) const;

            /**
             * Method you can use to assign a value to this cursor.
             *
             * \param[in] other The instance to be assigned to this instance.
             */
            virtual Cursor& operator=(const Cursor& other);

        protected:
            /**
             * Method that performs necessary pre-checking of the cursor state before cursor adjustments can be
             * performed.  You can use this method to make certain that the cursor is valid before performing
             * cursor adjustments.
             *
             * \param[out] canNotMove A reference to a location that will be populated with a boolean true if the cursor
             *                        position could not be changed.   Returns false if this method returns true or the
             *                        cursor position was properly initialized.
             *
             * \return Returns true if the movement method can behave normally.  Returns false if the movement method
             *         should return immediately without normal adjustments.
             */
            bool okToAdjustCursor(bool& canNotMove);

            /**
             * Method that is called when the cursor is updated.  You can overload this method to receive notification
             * when the user changes the cursor position.  The default implementation simply returns without performing
             * any action.
             *
             * You can optionally call this method if you've changed the element under the cursor to trigger updates
             * in code that is monitoring the cursor for changes.
             *
             * The default implementation does nothing.
             *
             * \param[in] movingForward If true, the cursor position has moved forward.  If false, the cursor position
             *                          has moved backward.
             */
            virtual void cursorChanged(bool movingForward);

            /**
             * Method that is triggered when the selection cursor is changed.  The default implementation simply
             * returns.
             */
            virtual void selectionCursorChanged();

        private:
            /**
             * Method that searches forwards through a text stream for a given text string.
             *
             * \param[in] searchText            The text to be located.  This value is only used if the
             *                                  useRegularExpressions parameter is false.
             *
             * \param[in] regularExpression     The compiled regular expression.  This value is only used if the
             *                                  useRegularExpressions parameter is true.
             *
             * \param[in] textStream            The text stream to scan for the desired string.
             *
             * \param[in] streamOffsets         Cursor positions representing offsets into the text stream.  This value
             *                                  is used to reconstruct cursor positions.
             *
             * \param[in] caseSensitive         If true, the search should be case sensitive.  If false, the search
             *                                  should ignore case.  This value is ignored if useRegularExpressions is
             *                                  true.
             *
             * \param[in] wholeWordsOnly        If true, the search should locate whole words.  If false.  The search
             *                                  should identify substrings within words.  This value is ignored if
             *                                  useRegularExpressions is true.
             *
             * \param[in] useRegularExpressions If true, the search will be a regular expression search. If false, the
             *                                  search will be a regular searcn.
             *
             * \param[in] elementCursor         The new element cursor value.
             *
             * \param[in] selectionCursor       The new selection cursor value.
             *
             * \return Returns the search result from this search.
             */
            SearchResult searchForwardThroughTextStream(
                const QString&                            searchText,
                const QRegularExpression&                 regularExpression,
                const QString&                            textStream,
                const QMap<unsigned long, ElementCursor>& streamOffsets,
                bool                                      caseSensitive,
                bool                                      wholeWordsOnly,
                bool                                      useRegularExpressions,
                ElementCursor&                            elementCursor,
                ElementCursor&                            selectionCursor
            );

            /**
             * Method that searches backwards through a text stream for a given text string.
             *
             * \param[in] searchText            The text to be located.  This value is only used if the
             *                                  useRegularExpressions parameter is false.
             *
             * \param[in] regularExpression     The compiled regular expression.  This value is only used if the
             *                                  useRegularExpressions parameter is true.
             *
             * \param[in] textStream            The text stream to scan for the desired string.
             *
             * \param[in] streamOffsets         Cursor positions representing offsets into the text stream.  This value
             *                                  is used to reconstruct cursor positions.
             *
             * \param[in] caseSensitive         If true, the search should be case sensitive.  If false, the search
             *                                  should ignore case.  This value is ignored if useRegularExpressions is
             *                                  true.
             *
             * \param[in] wholeWordsOnly        If true, the search should locate whole words.  If false.  The search
             *                                  should identify substrings within words.  This value is ignored if
             *                                  useRegularExpressions is true.
             *
             * \param[in] useRegularExpressions If true, the search will be a regular expression search. If false, the
             *                                  search will be a regular searcn.
             *
             * \param[in] elementCursor         The new element cursor value.
             *
             * \param[in] selectionCursor       The new selection cursor value.
             *
             * \return Returns the search result from this search.
             */
            SearchResult searchBackwardThroughTextStream(
                const QString&                            searchText,
                const QRegularExpression&                 regularExpression,
                const QString&                            textStream,
                const QMap<unsigned long, ElementCursor>& streamOffsets,
                bool                                      caseSensitive,
                bool                                      wholeWordsOnly,
                bool                                      useRegularExpressions,
                ElementCursor&                            elementCursor,
                ElementCursor&                            selectionCursor
            );

            /**
             * Method that adjusts the stream offsets from a backwards search so that the element cursor can be
             * properly resolved.
             *
             * \param[in] backwardsOffsets The offsets relative to the end of the stream.
             *
             * \return Returns offsets relative to the start of the stream.
             */
            static QMap<unsigned long, ElementCursor> adjustBackwardStreamOffsets(
                const QMap<unsigned long, ElementCursor>& backwardsOffsets
            );

            /**
             * Method that creates an element cursor based on an index into a stream and map of cursors to stream
             * offsets.
             *
             * \param[in] index         The index into the stream where the cursor should point.
             *
             * \param[in] streamOffsets A map of offsets into the stream.
             *
             * \return Returns an element cursor representing the desired position.
             */
            static ElementCursor elementCursorFromStreamIndex(
                unsigned long                             index,
                const QMap<unsigned long, ElementCursor>& streamOffsets
            );

            /**
             * The cursor position within the element tree.
             */
            ElementCursor currentElementCursor;

            /**
             * Location of starting side of a selection within the element tree.
             */
            ElementCursor currentSelectionCursor;

            /**
             * Weak pointer to the root element.
             */
            QWeakPointer<RootElement> currentRootElement;

            /**
             * Flag that indicates if the last action was a selection action.  We populate the selection cursor when this
             * value is false and the current selection is true.
             */
            bool lastWasSelection;
    };

    /**
     * Hash function used to manage smart pointers to cursors within a set.
     *
     * \param[in] cursor A weak pointer to the cursor instance to hash.  We hash based on the pointer address.
     *
     * \param[in] seed   An optional seed to apply to the hash.
     *
     * \return Returns a hash for this cursor.
     */
    Util::HashResult qHash(CursorWeakPointer cursor, Util::HashSeed seed = 0);

    /**
     * Hash function used to manage smart pointers to cursors within a set.
     *
     * \param[in] cursor A shared pointer to the cursor instance to hash.  We hash based on the pointer address.
     *
     * \param[in] seed   An optional seed to apply to the hash.
     *
     * \return Returns a hash for this cursor.
     */
    Util::HashResult qHash(CursorPointer cursor, Util::HashSeed seed = 0);
}

#endif
