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
* This header defines the \ref Ld::ElementCursor class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_ELEMENT_CURSOR_H
#define LD_ELEMENT_CURSOR_H

#include <QSharedPointer>
#include <QWeakPointer>
#include <QList>
#include <QSet>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_element_position.h"

namespace Ld {
    class ElementCursor;
    class CursorStateCollection;

    /**
     * Type you can use as a pointer to a \ref Ld::ElementCursor instance.
     */
    typedef QSharedPointer<ElementCursor> ElementCursorPointer;

    /**
     * Type you can use as a weak pointer to a \ref Ld::ElementCursor instance.
     */
    typedef QWeakPointer<ElementCursor> ElementCursorWeakPointer;

    /**
     * You can use this class as a cursor into a specific element.  The class extends the \ref ElementPosition class to
     * include tracking of the location within text fields of an element.
     */
    class LD_PUBLIC_API ElementCursor:public ElementPosition {
        public:
            /**
             * Enumeration used to indicate the supported mechanisms used to handle child elements.
             */
            enum class PositionType {
                /**
                 * Indicates this class instance does not point to a valid location.
                 */
                INVALID,

                /**
                 * Indicates this class instance points to an entire element that should be treated as a single unit.
                 */
                ELEMENT,

                /**
                 * Indicates this class instance points to text within an element.
                 */
                TEXT,
            };

            ElementCursor();

            /**
             * Constructor
             *
             * \param[in] newElement The element we should point to.
             */
            ElementCursor(ElementPointer newElement);

            /**
             * Constructor
             *
             * \param[in] newParent  Parent of the element we want to point to
             *
             * \param[in] childIndex The index of the element relative to the parent.
             */
            ElementCursor(ElementPointer newParent, unsigned long childIndex);

            /**
             * Constructor
             *
             * \param[in] textIndex   A zero based index into a specified text region.
             *
             * \param[in] regionIndex The zero based text region where the cursor points.
             *
             * \param[in] newElement  The element we should point to.
             */
            ElementCursor(unsigned textIndex, unsigned regionIndex, ElementPointer newElement);

            /**
             * Constructor
             *
             * \param[in] textIndex   A zero based index into a specified text region.
             *
             * \param[in] regionIndex The zero based text region where the cursor points.
             *
             * \param[in] newParent   Parent of the element we want to point to
             *
             * \param[in] childIndex  The index of the element relative to the parent.
             */
            ElementCursor(
                unsigned       textIndex,
                unsigned       regionIndex,
                ElementPointer newParent,
                unsigned long  childIndex
            );

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            ElementCursor(const ElementCursor& other);

            ~ElementCursor();

            /**
             * Method you can call to determine if the current position is valid.
             *
             * \return Returns true if the current position is valid.  Returns false if the current position is invalid.
             */
            bool isValid() const;

            /**
             * Method you can call to determine if the current position is invalid.
             *
             * \return Returns true if the current position is invalid.  Returns false if the current position is valid.
             */
            bool isInvalid() const;

            /**
             * Method you can use to determine if the current position points to the entire element or a location in an
             * element.
             *
             * \return Returns true if the cursor points to the entire element.  Returns false if the cursor points to
             *         text within the element.
             */
            bool isWholeElement() const;

            /**
             * Method you can use to determine if the current position points to the start of an element.
             *
             * \return Returns true if the cursor points to the first position in an element.  This method will return
             *         false if the cursor points to the entire element.
             */
            bool atStartOfElement() const;

            /**
             * Method you can use to determine if the current position points to the end of an element.
             *
             * \param[in] includeEmptyRegions If true, the empty regions will be included when determining if we've
             *                                pointing to the end of the element.
             *
             * \return Returns true if the cursor points to the last location in the current element.
             */
            bool atEndOfElement(bool includeEmptyRegions = true) const;

            /**
             * Method you can use to determine the position type.
             *
             * \return Returns the position type.  The default implementation returns \ref PositionType::ELEMENT.
             */
            PositionType positionType() const;

            /**
             * Method you can use to set the element associated with this position.
             *
             * \param[in] newElement The new element to tie to this position.  Note that this method will assert if the
             *                       element is the document root.
             */
            void setElement(ElementPointer newElement);

            /**
             * Method you can use to set the element based on a parent and child index.
             *
             * \param[in] newParent  Parent of the element we want to point to
             *
             * \param[in] childIndex The index of the element relative to the parent.
             */
            void setElementUnderParent(ElementPointer newParent, unsigned long childIndex);

            /**
             * Method you can use to indicate that the cursor should point to the entire element.
             */
            void setWholeElement();

            /**
             * Method you can use to force this cursor to report itself as having an invalid position.
             */
            void setInvalid();

            /**
             * Method you can use to set the location within the current element.
             *
             * \param[in] textIndex   The zero based text index.
             *
             * \param[in] regionIndex The zero based region index.
             *
             * \return Returns true if the position is valid.  Returns false if the position is invalid.  The current
             *         position will be unchanged if an invalid position is specified.
             */
            bool setPositionInElement(unsigned textIndex, unsigned regionIndex = 0);

            /**
             * Method you can use to obtain the current text index.
             *
             * \return Returns the current text index.
             */
            unsigned long textIndex() const;

            /**
             * Method you can use to obtain the current region index.
             *
             * \return Returns the current region index.
             */
            unsigned regionIndex() const;

            /**
             * Method you can call to move forward by element.  This overloaded version also resets the text index and
             * region to the start of the next element.
             *
             * \return Returns true if we've reached the last element.  Returns false if we have not reached the last
             *         element.
             */
            bool moveForwardByElement();

            /**
             * Method you can call to move backwards by element.  This overloaded version also resets the text index and
             * region just past the end of the text in the last region.
             *
             * \return Returns true if we've reached the first element.  Returns false if we have not reached the first
             *         element.
             */
            bool moveBackwardByElement();

            /**
             * Method you can call to move forward by region, then by element.
             *
             * \return Returns true if we've reached the last region in the last element.  Returns false if we have not
             *         reached the last region in the last element.
             */
            bool moveForwardByRegion();

            /**
             * Method you can call to move backward by region, then by element.
             *
             * \return Returns true if we've reached the first region in the first element.  Returns false if we have
             *         not reached the first region in the first element.
             */
            bool moveBackwardByRegion();

            /**
             * Method you can call to move forward by whitespace, then by region, then by element.
             *
             * \return Returns true if we've reached the last character in the last region in the last element.  Returns
             *         false otherwise.
             */
            bool moveForwardByWhitespace();

            /**
             * Method you can call to move backward by whitespace, then by region, then by element.
             *
             * \return Returns true if we've reached the first character in the first region in the first element.
             *         Returns false otherwise.
             */
            bool moveBackwardByWhitespace();

            /**
             * Method you can call to move forward by character, then by region, then by element.
             *
             * \return Returns true if we've reached the last character in the last region in the last element.  Returns
             *         false otherwise.
             */
            bool moveForwardByCharacter();

            /**
             * Method you can call to move backward by character, then by region, then by element.
             *
             * \return Returns true if we've reached the first character in the first region in the first element.
             *         Returns false otherwise.
             */
            bool moveBackwardByCharacter();

            /**
             * Method you can call to move to the first element.  Calling this method is similar to repeatedly calling
             * \ref ElementCursor::moveBackwardByElement until the method returns true.
             */
            void moveToFirstElement();

            /**
             * Method you can call to move to the last element.  Calling this method is similar to repeatedly calling
             * \ref ElementCursor::moveForwardByElement until the method returns true.
             */
            void moveToLastElement();

            /**
             * Method you can call to move to the beginning.  Calling this method is similar to repeatedly calling
             * \ref ElementCursor::moveBackwardByCharacter until the method returns true.
             */
            void moveToBeginning();

            /**
             * Method you can call to move to the end.  Calling this method is similar to repeatedly calling
             * \ref ElementCursor::moveForwardByCharacter until the method returns true.
             */
            void moveToEnd();

            /**
             * Method that is used to set the cursor to the very first position in the current element.
             */
            void moveToFirstPositionInElement();

            /**
             * Method that is used to set the cursor to the very last position in the current element.
             */
            void moveToLastPositionInElement();

            /**
             * Method that performs further adjustment if the cursor has parked at an invalid location.
             *
             * \param[in] movingForward If true, the cursor is attempting to move foreward.  If false, the cursor is
             *                          attempting to move backward.
             *
             * \param[in] isEnd         Holds true if the previous movement reached the beginning or end.
             *
             * \return Returns true if we've reached the end position.  Returns false if we have not reached the end
             *         position.  Note that "end" is defined either either the beginning or end depending on the
             *         indicated direction of movement.
             */
            bool fixPosition(bool movingForward, bool isEnd);

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this object.
             */
            ElementCursor& operator=(const ElementCursor& other);

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if this instance precedes the other instance.  Returns false if this instance is
             *         the same as or follows the other instance.
             */
            bool operator<(const ElementCursor& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if this instance points to the same element as the other instance.  Returns false if
             *         the instances point to different objects.  This method will always return false if either
             *         instance is invalid.
             */
            bool operator==(const ElementCursor& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if this instance points to a different object than the other instance.  Returns
             *         false if both instances point to the same element.
             */
            bool operator!=(const ElementCursor& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if this instance should follow the other instance.  Returns false if this instance
             *         points to the same element or should precede the other instance.  This method will always return
             *         false if either instance is invalid.
             */
            bool operator>(const ElementCursor& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if this instance precedes the other instance or if the two instances point to the
             *         same object.  Returns false if this instance follows the other instance.  This method will always
             *         return false if either instance is invalid.
             */
            bool operator<=(const ElementCursor& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if this instance follows the other instance or if the two instances point to the
             *         same object.  Returns false if this instance precedes the other instance.  This method will
             *         always return false if either instance is invalid.
             */
            bool operator>=(const ElementCursor& other) const;

            /**
             * Method you can use to identify all the elements over a given range.
             *
             * \param[in] cursor1 The first cursor in the range.
             *
             * \param[in] cursor2 The second cursor in the range.
             *
             * \return Returns a set containing all non-null elements over the specified range.
             */
            static ElementPointerSet elementsInRange(const ElementCursor& cursor1, const ElementCursor& cursor2);

        private:
            /**
             * Method that checks if the cursor position is at a location that does not allow parking.
             *
             * \return Returns true if the cursor should not park at the current location.  Returns false if the cursor
             *         can park at the current location.
             */
            bool mustAdvance() const;

            /**
             * Method that locates the last valid child under a given element.
             *
             * \param[in] element The element to identify the last valid child of.
             */
            static ElementPointer lastValidChildOf(ElementPointer element);

            /**
             * Method that locates the last descendant of a given element.
             *
             * \param[in] element The element to identify the last valid descendant of.
             */
            static ElementPointer lastValidDescendantOf(ElementPointer element);

            /**
             * Shared pointer to the current zero based character position in the current region.
             */
            unsigned long currentTextIndex;

            /**
             * Index of the current region.
             */
            unsigned currentRegionIndex;
    };
};

#endif
