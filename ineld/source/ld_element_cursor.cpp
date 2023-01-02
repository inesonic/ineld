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
* This file implements the \ref Ld::ElementCursor class.
***********************************************************************************************************************/

#include <QSharedPointer>
#include <QList>
#include <QMultiMap>
#include <QSet>

#include <QDebug>

#include <cassert>

#include "ld_element_structures.h"
#include "ld_element.h"
#include "ld_element_with_fixed_children.h"
#include "ld_element_with_positional_children.h"
#include "ld_element_with_floating_children.h"
#include "ld_element_with_grouped_children.h"
#include "ld_location.h"
#include "ld_cursor_state_collection.h"
#include "ld_element_position.h"
#include "ld_element_cursor.h"

namespace Ld {
    ElementCursor::ElementCursor() {
        setWholeElement();
    }


    ElementCursor::ElementCursor(ElementPointer newElement):ElementPosition(newElement) {
        setWholeElement();
    }


    ElementCursor::ElementCursor(
            ElementPointer newParent,
            unsigned long  childIndex
        ):ElementPosition(
            newParent,
            childIndex
        ) {
        setWholeElement();
    }


    ElementCursor::ElementCursor(
            unsigned       textIndex,
            unsigned       regionIndex,
            ElementPointer newElement
        ):ElementPosition(
            newElement
        ) {
        setPositionInElement(textIndex, regionIndex);
    }


    ElementCursor::ElementCursor(
            unsigned       textIndex,
            unsigned       regionIndex,
            ElementPointer newParent,
            unsigned long  childIndex
        ):ElementPosition(
            newParent,
            childIndex
        ) {
        setPositionInElement(textIndex, regionIndex);
    }


    ElementCursor::ElementCursor(const ElementCursor& other):ElementPosition(other) {
        currentTextIndex   = other.currentTextIndex;
        currentRegionIndex = other.currentRegionIndex;
    }


    ElementCursor::~ElementCursor() {}


    bool ElementCursor::isValid() const {
        bool valid = false;

        if (ElementPosition::isValid()) {
            ElementPointer thisElement = element();

            if (!thisElement.isNull()) {
                if (currentRegionIndex == Element::invalidRegionIndex &&
                    currentTextIndex == Element::invalidTextIndex        ) {
                    valid = true;
                } else {
                    unsigned numberRegions = thisElement->numberTextRegions();
                    if (currentRegionIndex < numberRegions) {
                        QString text = thisElement->text(currentRegionIndex);
                        valid = (currentTextIndex <= static_cast<unsigned long>(text.length()));
                    }
                }
            } else {
                valid = (
                       currentRegionIndex == Element::invalidRegionIndex
                    && currentTextIndex == Element::invalidTextIndex
                );
            }
        }

        return valid;
    }


    bool ElementCursor::isInvalid() const {
        return !isValid();
    }


    bool ElementCursor::isWholeElement() const {
        return currentTextIndex == Element::invalidTextIndex && currentRegionIndex == Element::invalidRegionIndex;
    }


    bool ElementCursor::atStartOfElement() const {
        bool result;

        ElementPointer element = ElementCursor::element();
        if (!element.isNull()) {
            unsigned numberRegions = element->numberTextRegions();
            result = (numberRegions > 0 && currentRegionIndex == 0 && currentTextIndex == 0);
        } else {
            result = false;
        }

        return result;
    }


    bool ElementCursor::atEndOfElement(bool includeEmptyRegions) const {
        bool result = false;

        ElementPointer element = ElementCursor::element();
        if (!element.isNull()) {
            unsigned numberRegions = element->numberTextRegions();

            if (!includeEmptyRegions) {
                while (numberRegions > 1 && element->text(numberRegions).isEmpty()) {
                    --numberRegions;
                }
            }

            if (numberRegions > 0) {
                unsigned lastRegion = numberRegions - 1;
                if (lastRegion == currentRegionIndex) {
                    unsigned long regionLength = static_cast<unsigned long>(element->text(lastRegion).length());
                    result = (currentTextIndex == regionLength);
                }
            }
        }

        return result;
    }


    ElementCursor::PositionType ElementCursor::positionType() const {
        PositionType result = PositionType::INVALID;

        if (ElementPosition::isValid()) {
            if (isWholeElement()) {
                result = PositionType::ELEMENT;
            } else {
                ElementPointer thisElement = element();

                unsigned numberRegions = thisElement->numberTextRegions();
                if (currentRegionIndex < numberRegions) {
                    QString text = thisElement->text(currentRegionIndex);
                    if (currentTextIndex <= static_cast<unsigned long>(text.length())) {
                        result = PositionType::TEXT;
                    }
                }
            }
        }

        return result;
    }


    void ElementCursor::setElement(ElementPointer newElement) {
        ElementPosition::setElement(newElement);
        setWholeElement();
    }


    void ElementCursor::setElementUnderParent(ElementPointer newParent, unsigned long childIndex) {
        ElementPosition::setElementUnderParent(newParent, childIndex);
        setWholeElement();
    }


    void ElementCursor::setWholeElement() {
        currentTextIndex    = Element::invalidTextIndex;
        currentRegionIndex  = Element::invalidRegionIndex;
    }


    void ElementCursor::setInvalid() {
        ElementPosition::setInvalid();
        setWholeElement();
    }


    bool ElementCursor::setPositionInElement(unsigned textIndex, unsigned regionIndex) {
        bool result = false;

        ElementPointer thisElement = element();
        if (!thisElement.isNull()) {
            unsigned numberRegions = thisElement->numberTextRegions();

            if (regionIndex < numberRegions) {
                QString text = thisElement->text(regionIndex);
                if (textIndex <= static_cast<unsigned>(text.length())) {
                    currentTextIndex   = textIndex;
                    currentRegionIndex = regionIndex;

                    result = true;
                }
            }
        }
        return result;
    }


    unsigned long ElementCursor::textIndex() const {
        return currentTextIndex;
    }


    unsigned ElementCursor::regionIndex() const {
        return currentRegionIndex;
    }


    bool ElementCursor::moveForwardByElement() {
        bool isLast = ElementPosition::moveForwardByElement();
        setWholeElement();

        return isLast;
    }


    bool ElementCursor::moveBackwardByElement() {
        bool isFirst = ElementPosition::moveBackwardByElement();
        setWholeElement();

        return isFirst;
    }


    bool ElementCursor::moveForwardByRegion() {
        bool isLast = false;

        ElementPointer thisElement = element();
        if (!thisElement.isNull()) {
            unsigned numberRegions = thisElement->numberTextRegions();
            if (numberRegions > 0 && currentRegionIndex < numberRegions - 1) {
                ++currentRegionIndex;
                currentTextIndex = 0;
            } else {
                isLast = ElementPosition::moveForwardByElement();

                if (!isLast) {
                    moveToFirstPositionInElement();
                }
            }
        } else {
            isLast = ElementPosition::moveForwardByElement();

            if (!isLast) {
                moveToFirstPositionInElement();
            }
        }

        return isLast;
    }


    bool ElementCursor::moveBackwardByRegion() {
        bool isFirst = false;

        ElementPointer thisElement = element();
        if (!thisElement.isNull()) {
            unsigned numberRegions = thisElement->numberTextRegions();

            if (numberRegions > 0 && currentRegionIndex > 0) {
                --currentRegionIndex;

                if (currentRegionIndex >= numberRegions) {
                    currentRegionIndex = numberRegions - 1;
                }

                QString text = thisElement->text(currentRegionIndex);
                currentTextIndex = static_cast<unsigned long>(text.length());
            } else {
                isFirst = ElementPosition::moveBackwardByElement();

                if (!isFirst) {
                    moveToLastPositionInElement();
                }
            }
        } else {
            isFirst = ElementPosition::moveBackwardByElement();

            if (!isFirst) {
                moveToLastPositionInElement();
            }
        }

        return isFirst;
    }


    bool ElementCursor::moveForwardByWhitespace() {
        bool isLast = false;

        ElementPointer thisElement = element();
        if (!thisElement.isNull()) {
            unsigned numberRegions = thisElement->numberTextRegions();
            if (numberRegions > 0 && currentRegionIndex < numberRegions) {
                QString       text       = thisElement->text(currentRegionIndex);
                unsigned long textLength = static_cast<unsigned long>(text.length());

                if (currentTextIndex >= textLength) {
                    isLast = moveForwardByRegion();
                } else {
                    while (currentTextIndex < textLength && text.at(currentTextIndex).isSpace()) {
                        ++currentTextIndex;
                    }

                    while (currentTextIndex < textLength && !text.at(currentTextIndex).isSpace()) {
                        ++currentTextIndex;
                    }
                }
            } else {
                isLast = moveForwardByRegion();
            }
        } else {
            isLast = ElementPosition::moveForwardByElement();

            if (!isLast) {
                moveToFirstPositionInElement();
            }
        }

        return isLast;
    }


    bool ElementCursor::moveBackwardByWhitespace() {
        bool isFirst = false;

        ElementPointer thisElement = element();
        if (!thisElement.isNull()) {
            unsigned numberRegions = thisElement->numberTextRegions();
            if (numberRegions > 0 && currentRegionIndex < numberRegions) {
                QString       text       = thisElement->text(currentRegionIndex);
                unsigned long textLength = static_cast<unsigned long>(text.length());

                if (textLength > 0 && currentTextIndex > 0) {
                    if (currentTextIndex >= textLength) {
                        currentTextIndex = textLength - 1;
                    }

                    while (currentTextIndex > 0 && text.at(currentTextIndex).isSpace()) {
                        --currentTextIndex;
                    }

                    while (currentTextIndex > 0 && !text.at(currentTextIndex).isSpace()) {
                        --currentTextIndex;
                    }
                } else {
                    isFirst = moveBackwardByRegion();
                }
            } else {
                isFirst = moveBackwardByRegion();
            }
        } else {
            isFirst = ElementPosition::moveBackwardByElement();

            if (!isFirst) {
                moveToLastPositionInElement();
            }
        }

        return isFirst;
    }


    bool ElementCursor::moveForwardByCharacter() {
        bool isLast = false;

        ElementPointer thisElement = element();
        if (!thisElement.isNull()) {
            unsigned numberRegions = thisElement->numberTextRegions();
            if (numberRegions > 0 && currentRegionIndex < numberRegions) {
                QString       text       = thisElement->text(currentRegionIndex);
                unsigned long textLength = static_cast<unsigned long>(text.length());

                if (currentTextIndex >= textLength) {
                    isLast = moveForwardByRegion();
                } else {
                    ++currentTextIndex;
                }
            } else {
                isLast = moveForwardByRegion();
            }
        } else {
            isLast = ElementPosition::moveForwardByElement();

            if (!isLast) {
                moveToFirstPositionInElement();
            }
        }

        return isLast;
    }


    bool ElementCursor::moveBackwardByCharacter() {
        bool isFirst = false;

        ElementPointer thisElement = element();
        if (!thisElement.isNull()) {
            unsigned numberRegions = thisElement->numberTextRegions();
            if (numberRegions > 0 && currentRegionIndex < numberRegions) {
                if (currentTextIndex > 0) {
                    QString       text       = thisElement->text(currentRegionIndex);
                    unsigned long textLength = static_cast<unsigned long>(text.length());

                    if (currentTextIndex > textLength) {
                        currentTextIndex = textLength;
                    } else {
                        --currentTextIndex;
                    }
                } else {
                    isFirst = moveBackwardByRegion();
                }
            } else {
                isFirst = moveBackwardByRegion();
            }
        } else {
            isFirst = ElementPosition::moveBackwardByElement();

            if (!isFirst) {
                moveToLastPositionInElement();
            }
        }

        return isFirst;
    }


    void ElementCursor::moveToFirstElement() {
        ElementPosition::moveToFirstElement();
        setWholeElement();
    }


    void ElementCursor::moveToLastElement() {
        ElementPosition::moveToLastElement();
        setWholeElement();
    }


    void ElementCursor::moveToBeginning() {
        ElementPosition::moveToFirstElement();
        moveToFirstPositionInElement();
    }


    void ElementCursor::moveToEnd() {
        ElementPosition::moveToLastElement();
        moveToLastPositionInElement();
    }


    void ElementCursor::moveToFirstPositionInElement() {
        ElementPointer thisElement = element();
        if (!thisElement.isNull()) {
            unsigned numberRegions = thisElement->numberTextRegions();

            if (numberRegions > 0) {
                currentRegionIndex = 0;
                currentTextIndex   = 0;
            } else {
                setWholeElement();
            }

        } else {
            setWholeElement();
        }
    }


    void ElementCursor::moveToLastPositionInElement() {
        ElementPointer thisElement = element();

        if (!thisElement.isNull()) {
            unsigned numberRegions = thisElement->numberTextRegions();

            if (numberRegions > 0) {
                currentRegionIndex = numberRegions - 1;
                QString text = thisElement->text(currentRegionIndex);
                currentTextIndex = static_cast<unsigned long>(text.length());
            } else {
                setWholeElement();
            }
        } else {
            setWholeElement();
        }
    }


    bool ElementCursor::fixPosition(bool movingForward, bool isEnd) {
        if (isValid()) {
            bool moveForward = isEnd ? !movingForward : movingForward;
            bool reachedEnd  = false;

            while (!reachedEnd && mustAdvance()) {
                if (moveForward) {
                    reachedEnd = moveForwardByCharacter();
                } else {
                    reachedEnd = moveBackwardByCharacter();
                }
            }

            if (reachedEnd) {
                if (mustAdvance()) {
                    do {
                        if (!moveForward) {
                            reachedEnd = moveForwardByCharacter();
                        } else {
                            reachedEnd = moveBackwardByCharacter();
                        }
                    } while (!reachedEnd && mustAdvance());
                }

                isEnd = true;
            }
        }

        return isEnd;
    }


    ElementCursor& ElementCursor::operator=(const ElementCursor& other) {
        ElementPosition::operator=(other);

        currentTextIndex   = other.currentTextIndex;
        currentRegionIndex = other.currentRegionIndex;

        return *this;
    }




    bool ElementCursor::operator<(const ElementCursor& other) const {
        bool precedes = false;

        if (ElementPosition::operator<(other)) {
            precedes = true;
        } else if (ElementPosition::operator==(other)) {
            if (other.currentRegionIndex > currentRegionIndex) {
                precedes = true;
            } else if (other.currentRegionIndex == currentRegionIndex && other.currentTextIndex > currentTextIndex) {
                precedes = true;
            }
        }

        return precedes;
    }


    bool ElementCursor::operator==(const ElementCursor& other) const {
        return (
               ElementPosition::operator==(other)
            && other.currentRegionIndex == currentRegionIndex
            && other.currentTextIndex == currentTextIndex
        );
    }


    bool ElementCursor::operator!=(const ElementCursor& other) const {
        return (
               ElementPosition::operator!=(other)
            || other.currentRegionIndex != currentRegionIndex
            || other.currentTextIndex != currentTextIndex
        );
    }


    bool ElementCursor::operator>(const ElementCursor& other) const {
        bool follows = false;

        if (ElementPosition::operator>(other)) {
            follows = true;
        } else if (ElementPosition::operator==(other)) {
            if (other.currentRegionIndex < currentRegionIndex) {
                follows = true;
            } else if (other.currentRegionIndex == currentRegionIndex && other.currentTextIndex < currentTextIndex) {
                follows = true;
            }
        }

        return follows;
    }


    bool ElementCursor::operator<=(const ElementCursor& other) const {
        bool precedesOrEquals = false;

        if (ElementPosition::operator<(other)) {
            precedesOrEquals = true;
        } else if (ElementPosition::operator==(other)) {
            if (other.currentRegionIndex > currentRegionIndex) {
                precedesOrEquals = true;
            } else if (other.currentRegionIndex == currentRegionIndex && other.currentTextIndex >= currentTextIndex) {
                precedesOrEquals = true;
            }
        }

        return precedesOrEquals;
    }


    bool ElementCursor::operator>=(const ElementCursor& other) const {
        bool followsOrEquals = false;

        if (ElementPosition::operator>(other)) {
            followsOrEquals = true;
        } else if (ElementPosition::operator==(other)) {
            if (other.currentRegionIndex < currentRegionIndex) {
                followsOrEquals = true;
            } else if (other.currentRegionIndex == currentRegionIndex && other.currentTextIndex <= currentTextIndex) {
                followsOrEquals = true;
            }
        }

        return followsOrEquals;
    }


    ElementPointerSet ElementCursor::elementsInRange(
            const ElementCursor& cursor1,
            const ElementCursor& cursor2
        ) {
        const ElementCursor& startingCursor = cursor1 < cursor2  ? cursor1 : cursor2;
        const ElementCursor& endingCursor   = cursor2 <= cursor1 ? cursor1 : cursor2;

        ElementPointerSet allElementsInRange;

        ElementPosition cursor = startingCursor;
        bool            done   = false;
        do {
            ElementPointer element = cursor.element();
            if (!element.isNull()) {
                allElementsInRange.insert(element);
            }

            done = (cursor == endingCursor);
            cursor.ElementPosition::moveForwardByElement();
        } while (!done);

        return allElementsInRange;
    }


    bool ElementCursor::mustAdvance() const {
        bool mustAdvance = false;
        if (isValid()) {
            ElementPointer element = ElementCursor::element();
            if (!element.isNull()) {
                Element::CursorParkMode parkMode = element->cursorParkMode();
                switch (parkMode) {
                    case Element::CursorParkMode::CAN_NOT_PARK: {
                        mustAdvance = true;
                        break;
                    }

                    case Element::CursorParkMode::PARK_ON_TEXT_ONLY: {
                        if (isWholeElement()) {
                            mustAdvance = true;
                        }

                        break;
                    }

                    case Element::CursorParkMode::PARK_ON_ELEMENT_ONLY:
                    case Element::CursorParkMode::PARK_ON_TEXT_AND_ELEMENT: {
                        break;
                    }

                    default: {
                        Q_ASSERT(false); // Unexpected Element::CursorParkMode
                        break;
                    }
                }
            } else {
                Q_ASSERT(!parent().isNull());
            }
        }

        return mustAdvance;
    }


    ElementPointer ElementCursor::lastValidChildOf(ElementPointer element) {
        ElementPointer result;
        unsigned long  numberChildren = element->numberChildren();

        if (numberChildren > 0) {
            unsigned long  childIndex = numberChildren;
            do {
                -- childIndex;
                result = element->child(childIndex);
            } while (result.isNull() && childIndex > 0);
        }

        return result;
    }


    ElementPointer ElementCursor::lastValidDescendantOf(ElementPointer element) {
        ElementPointer lastParent = element;
        ElementPointer lastDescendant;
        do {
            lastDescendant = lastParent;
            lastParent = lastValidChildOf(lastDescendant);
        } while (!lastParent.isNull());

        return lastDescendant;
    }
}
