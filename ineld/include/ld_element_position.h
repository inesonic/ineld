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
* This header defines the \ref Ld::ElementPosition class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_ELEMENT_POSITION_H
#define LD_ELEMENT_POSITION_H

#include <QSharedPointer>
#include <QWeakPointer>
#include <QList>

#include "ld_common.h"
#include "ld_element_structures.h"

namespace Ld {
    class Format;

    /**
     * You can use this as a common base class to track a position of an element within a program.  Note that this
     * class tracks elements by branch index values and thus can function even if class instances are replaced in-line
     * within elements.
     */
    class LD_PUBLIC_API ElementPosition {
        public:
            ElementPosition();

            /**
             * Constructor
             *
             * \param[in] newElement The element we should point to.
             */
            ElementPosition(ElementPointer newElement);

            /**
             * Constructor
             *
             * \param[in] newParent  Parent of the element we want to point to
             *
             * \param[in] childIndex The index of the element relative to the parent.
             */
            ElementPosition(ElementPointer newParent, unsigned long childIndex);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            ElementPosition(const ElementPosition& other);

            ~ElementPosition();

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
             * Method you can use to set the element associated with this position.
             *
             * \param[in] newElement The new element to tie to this position.  Note that this method will assert if the
             *                       element is the document root.
             */
            void setElement(ElementPointer newElement);

            /**
             * Method you can use to set the element based on a parent and child index.
             *
             * \param[in] newParent  Parent of the element we want to point to.  Setting this value to a null pointer
             *                       will cause the cursor to report an invalid position.
             *
             * \param[in] childIndex The index of the element relative to the parent.
             */
            void setElementUnderParent(ElementPointer newParent, unsigned long childIndex);

            /**
             * Method you can use to force this cursor to report itself as having an invalid position.
             */
            void setInvalid();

            /**
             * Method you can use to obtain the element associated with this position.  Note that this method will
             * assert if the cursor state is invalid or incoherent with the state of the element tree.
             *
             * \return Returns a shared pointer to the element associated with this position.
             */
            ElementPointer element() const;

            /**
             * Method you can use to obtain the parent element associated with this position.
             *
             * \return Returns a shared pointer to the element's parent.
             */
            ElementPointer parent() const;

            /**
             * Method you can use to obtain the root element this cursor is operating under.
             *
             * \return Returns a shared pointer to the root element.  A null pointer may be returned if the cursor is
             *         invalid.
             */
            ElementPointer root() const;

            /**
             * Method you can use to obtain the child index of this element under the parent.
             *
             * \return Returns the zero based child index of the element.
             */
            unsigned long childIndex() const;

            /**
             * Method you can use to obtain the current depth under the root.
             *
             * \return Returns the current depth under the root.
             */
            unsigned depth() const;

            /**
             * Method you can call to obtain the next element.
             *
             * \return Returns a \ref ElementPosition instance pointing to the next element.
             */
            ElementPosition nextElement() const;

            /**
             * Method you can call to obtain the previous element.
             *
             * \return Returns a \ref ElementPosition instance pointing to the previous element.
             */
            ElementPosition previousElement() const;

            /**
             * Method you can call to obtain the first element.
             *
             * \return Returns a \ref ElementPosition instance pointing to the first element.
             */
            ElementPosition firstElement() const;

            /**
             * Method you can call to obtain the last element.
             *
             * \return Returns a \ref ElementPosition instance pointing to the last element.
             */
            ElementPosition lastElement() const;

            /**
             * Method you can call to move forward by element.
             *
             * \return Returns true if we've attempted to move past the last element.  Returns false if we're either
             *         not at the last element or have just reached the last element.
             */
            bool moveForwardByElement();

            /**
             * Method you can call to move backwards by element.
             *
             * \return Returns true if we've attempted to move past the first element.  Returns false if we've either
             *         not reached the first element or have just reached the first element.
             */
            bool moveBackwardByElement();

            /**
             * Method you can call to move to the first element.  Calling this method is similar to repeatedly calling
             * \ref ElementPosition::moveBackwardByElement until the method returns true.
             */
            void moveToFirstElement();

            /**
             * Method you can call to move to the last element.  Calling this method is similar to repeatedly calling
             * \ref ElementPosition::moveForwardByElement until the method returns true.
             */
            void moveToLastElement();

            /**
             * Method you can use to obtain the stack of elements under the cursor.
             *
             * \return Return an ordered list of all the elements under the cursor.  Element 0 will be the root
             *         element.  The last element is the deepest element and may be a null pointer if cursor points to
             *         an empty location.
             */
            ElementPointerList elementStack() const;

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this object.
             */
            ElementPosition& operator=(const ElementPosition& other);

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if this instance precedes the other instance.  Returns false if this instance is
             *         the same as or follows the other instance.
             */
            bool operator<(const ElementPosition& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if this instance points to the same element as the other instance.  Returns false if
             *         the instances point to different objects.  This method will always return false if either
             *         instance is invalid.
             */
            bool operator==(const ElementPosition& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if this instance points to a different object than the other instance.  Returns
             *         false if both instances point to the same element.
             */
            bool operator!=(const ElementPosition& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if this instance should follow the other instance.  Returns false if this instance
             *         points to the same element or should precede the other instance.  This method will always return
             *         false if either instance is invalid.
             */
            bool operator>(const ElementPosition& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if this instance precedes the other instance or if the two instances point to the
             *         same object.  Returns false if this instance follows the other instance.  This method will always
             *         return false if either instance is invalid.
             */
            bool operator<=(const ElementPosition& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if this instance follows the other instance or if the two instances point to the
             *         same object.  Returns false if this instance precedes the other instance.  This method will
             *         always return false if either instance is invalid.
             */
            bool operator>=(const ElementPosition& other) const;

        private:
            /**
             * A weak pointer to the current root element.
             */
            ElementWeakPointer currentRoot;

            /**
             * The list of branch paths needed to get to a specific position.
             */
            QList<unsigned long> currentBranchPaths;
    };
};

#endif
