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
* This file implements the \ref Ld::CursorStateData class.
***********************************************************************************************************************/

#include <QSharedPointer>
#include <QWeakPointer>
#include <QScopedPointer>
#include <QHash>

#include "ld_element_structures.h"
#include "ld_element.h"
#include "ld_element_cursor.h"
#include "ld_cursor.h"
#include "ld_cursor_state_data.h"

/***********************************************************************************************************************
 * Ld::CursorStateData::CursorData
 */

namespace Ld {
    CursorStateData::CursorData::CursorData() {
        invalidate();
    }


    CursorStateData::CursorData::CursorData(const ElementCursor& elementCursor) {
        updateFromElementCursor(elementCursor);
    }


    CursorStateData::CursorData::CursorData(const CursorStateData::CursorData& other) {
        currentElement     = other.currentElement;
        currentChildIndex  = other.currentChildIndex;
        currentRegionIndex = other.currentRegionIndex;
        currentTextIndex   = other.currentTextIndex;
    }


    CursorStateData::CursorData::~CursorData() {}


    bool CursorStateData::CursorData::isValid() const {
        return !currentElement.isNull();
    }


    bool CursorStateData::CursorData::isInvalid() const {
        return !isValid();
    }


    ElementCursor CursorStateData::CursorData::toElementCursor() const {
        ElementCursor newCursor;

        if (!currentElement.isNull()) {
            if (currentChildIndex == Element::invalidChildIndex) {
                if (currentRegionIndex == Element::invalidRegionIndex) {
                    Q_ASSERT(currentTextIndex == Element::invalidTextIndex);
                    newCursor = ElementCursor(currentElement);
                } else {
                    Q_ASSERT(currentTextIndex != Element::invalidTextIndex);
                    newCursor = ElementCursor(currentTextIndex, currentRegionIndex, currentElement);
                }
            } else {
                newCursor = ElementCursor(currentElement, currentChildIndex);
            }
        }

        return newCursor;
    }


    void CursorStateData::CursorData::updateAfterTextInsertion(
            ElementPointer element,
            unsigned long  textIndex,
            unsigned       regionIndex,
            unsigned long  insertionLength
        ) {
        if (currentChildIndex == Element::invalidChildIndex &&
            currentElement == element                       &&
            regionIndex == currentRegionIndex               &&
            textIndex <= currentTextIndex                      ) {
            currentTextIndex += insertionLength;
        }
    }


    void CursorStateData::CursorData::updateAfterTextDeleted(
            ElementPointer element,
            unsigned long  textIndex,
            unsigned       regionIndex,
            unsigned long  deletedCount
        ) {
        if (currentChildIndex == Element::invalidChildIndex &&
            currentElement == element                       &&
            regionIndex == currentRegionIndex               &&
            textIndex < currentTextIndex                       ) {
            if (deletedCount >= currentTextIndex) {
                currentTextIndex = 0;
            } else {
                currentTextIndex -= deletedCount;
            }
        }
    }


    void CursorStateData::CursorData::updateAfterSplit(ElementPointer oldElement, ElementPointer newElement) {
        if (currentChildIndex == Element::invalidChildIndex   &&
            currentRegionIndex != Element::invalidRegionIndex    ) {
            Q_ASSERT(currentTextIndex != Element::invalidTextIndex);

            if (currentElement == oldElement) {
                QString       oldText       = oldElement->text(currentRegionIndex);
                unsigned long oldTextLength = static_cast<unsigned long>(oldText.length());
                if (currentTextIndex >= oldTextLength) {
                    currentElement    = newElement;
                    currentTextIndex -= oldTextLength;
                }
            } else if (oldElement->childPlacement() == Element::ChildPlacement::POSITIONAL &&
                       newElement->childPlacement() == Element::ChildPlacement::POSITIONAL &&
                       oldElement->descendants().contains(currentElement)                     ) {
                unsigned long oldElementNumberChildren = oldElement->numberChildren();
                unsigned long newElementNumberChildren = newElement->numberChildren();

                if (oldElementNumberChildren > 0 && newElementNumberChildren > 0) {
                    ElementPointer lastChild = oldElement->child(oldElementNumberChildren - 1);
                    if (currentElement == lastChild) {
                        ElementPointer firstChild    = newElement->child(0);
                        QString        oldText       = lastChild->text(currentRegionIndex);
                        unsigned long  oldTextLength = static_cast<unsigned long>(oldText.length());

                        if (currentTextIndex >= oldTextLength) {
                            currentElement    = firstChild;
                            currentTextIndex -= oldTextLength;
                        }
                    }
                }
            }
        }
    }


    void CursorStateData::CursorData::updateBeforeElementDelete(
            ElementPointer element,
            bool           preferEarlierPosition
        ) {
        ElementPointerSet descendants = element->descendants();
        descendants.insert(element);

        if (descendants.contains(currentElement)) {
            ElementCursor adjustmentCursor(element);
            bool walkBackwards = preferEarlierPosition;

            ElementPointer parent                 = adjustmentCursor.parent();
            unsigned long  childIndex             = adjustmentCursor.childIndex();
            unsigned       numberDirectionChanges = 0;
            while (numberDirectionChanges < 2                                                        &&
                   (descendants.contains(parent) || descendants.contains(parent->child(childIndex)))    ) {
                bool changeDirection;
                if (walkBackwards) {
                    changeDirection = adjustmentCursor.moveBackwardByElement();
                } else {
                    changeDirection = adjustmentCursor.moveForwardByElement();
                }
                changeDirection = adjustmentCursor.fixPosition(!walkBackwards, changeDirection);

                parent     = adjustmentCursor.parent();
                childIndex = adjustmentCursor.childIndex();

                if (changeDirection) {
                    walkBackwards = !walkBackwards;
                    ++numberDirectionChanges;
                }
            }

            if (numberDirectionChanges >= 2) {
                invalidate();
            } else {
                if (walkBackwards) {
                    adjustmentCursor.moveToLastPositionInElement();
                } else {
                    adjustmentCursor.moveToFirstPositionInElement();
                }

                updateFromElementCursor(adjustmentCursor);

                if (currentChildIndex != Element::invalidChildIndex) {
                    ElementPointer parent = element->parent();
                    if (parent == currentElement) {
                        unsigned long elementIndex = parent->indexOfChild(element);
                        if (elementIndex < currentChildIndex) {
                            --currentChildIndex;
                        }
                    }
                }
            }
         }
    }


    void CursorStateData::CursorData::updateBeforeRangeDelete(
            const ElementCursor& startingCursor,
            const ElementCursor& endingCursor
        ) {
        if (isValid()) {
            ElementCursor cursor = toElementCursor();
            if (cursor >= startingCursor && cursor <= endingCursor) {
                if (startingCursor.isWholeElement()) {
                    cursor = startingCursor;
                    bool isFirst = cursor.moveBackwardByElement();
                    isFirst = cursor.fixPosition(false, isFirst);

                    if (isFirst) {
                        cursor = endingCursor;
                        bool isLast = cursor.moveForwardByElement();
                        isLast = cursor.fixPosition(true, isLast);
                        cursor.moveToFirstPositionInElement();
                    } else {
                        cursor.moveToLastPositionInElement();
                    }

                    updateFromElementCursor(cursor);
                } else {
                    updateFromElementCursor(startingCursor);
                }
            }
        }
    }


    CursorStateData::CursorData& CursorStateData::CursorData::operator=(const CursorStateData::CursorData& other) {
        currentElement     = other.currentElement;
        currentChildIndex  = other.currentChildIndex;
        currentRegionIndex = other.currentRegionIndex;
        currentTextIndex   = other.currentTextIndex;

        return *this;
    }


    bool CursorStateData::CursorData::operator==(const CursorStateData::CursorData& other) const {
        return (
               currentElement == other.currentElement
            && currentChildIndex == other.currentChildIndex
            && currentRegionIndex == other.currentRegionIndex
            && currentTextIndex == other.currentTextIndex
        );
    }


    bool CursorStateData::CursorData::operator!=(const CursorStateData::CursorData& other) const {
        return (
               currentElement != other.currentElement
            || currentChildIndex != other.currentChildIndex
            || currentRegionIndex != other.currentRegionIndex
            || currentTextIndex != other.currentTextIndex
        );
    }


    void CursorStateData::CursorData::invalidate() {
        currentElement.reset();
        currentChildIndex  = Element::invalidChildIndex;
        currentRegionIndex = Element::invalidRegionIndex;
        currentTextIndex   = Element::invalidTextIndex;
    }


    void CursorStateData::CursorData::updateFromElementCursor(const ElementCursor& elementCursor) {
        invalidate();

        if (!elementCursor.isInvalid()) {
            ElementPointer parent     = elementCursor.parent();
            unsigned long  childIndex = elementCursor.childIndex();
            ElementPointer element    = parent->child(childIndex);

            if (element.isNull()) {
                currentElement    = parent;
                currentChildIndex = childIndex;
            } else {
                currentElement = element;

                if (!elementCursor.isWholeElement()) {
                    currentRegionIndex = elementCursor.regionIndex();
                    currentTextIndex   = elementCursor.textIndex();
                }
            }
        }
    }
}

/***********************************************************************************************************************
 * Ld::CursorStateData
 */

namespace Ld {
    CursorStateData::CursorStateData() {}


    CursorStateData::CursorStateData(
            CursorPointer cursor
        ):elementCursorData(
            cursor->elementCursor()
        ),selectionCursorData(
            cursor->selectionCursor()
        ) {}


    CursorStateData::CursorStateData(
            const CursorStateData& other
        ):elementCursorData(
            other.elementCursorData
        ),selectionCursorData(
            other.selectionCursorData
        ) {}


    CursorStateData::~CursorStateData() {}


    bool CursorStateData::isValid() const {
        return elementCursorData.isValid() && selectionCursorData.isValid();
    }


    bool CursorStateData::isInvalid() const {
        return elementCursorData.isInvalid() || selectionCursorData.isInvalid();
    }


    void CursorStateData::captureState(CursorPointer cursor) {
        elementCursorData   = CursorData(cursor->elementCursor());
        selectionCursorData = CursorData(cursor->selectionCursor());
    }


    void CursorStateData::restoreState(CursorPointer cursor, bool /* implicitlyChanged */) {
        cursor->updateFromElementCursor(elementCursorData.toElementCursor(), false);
        cursor->updateSelectionFromElementCursor(selectionCursorData.toElementCursor());
    }


    void CursorStateData::updateAfterTextInsertion(
            ElementPointer element,
            unsigned long  textIndex,
            unsigned       regionIndex,
            unsigned long  insertionLength
        ) {
        elementCursorData.updateAfterTextInsertion(element, textIndex, regionIndex, insertionLength);
        selectionCursorData.updateAfterTextInsertion(element, textIndex, regionIndex, insertionLength);
    }


    void CursorStateData::updateAfterTextDeleted(
            ElementPointer element,
            unsigned long  textIndex,
            unsigned       regionIndex,
            unsigned long  deletedCount
        ) {
        elementCursorData.updateAfterTextDeleted(element, textIndex, regionIndex, deletedCount);
        selectionCursorData.updateAfterTextDeleted(element, textIndex, regionIndex, deletedCount);
    }


    void CursorStateData::updateAfterSplit(ElementPointer oldElement, ElementPointer newElement) {
        elementCursorData.updateAfterSplit(oldElement, newElement);
        selectionCursorData.updateAfterSplit(oldElement, newElement);
    }


    void CursorStateData::updateBeforeElementDelete(ElementPointer element, bool preferEarlierPosition) {
        elementCursorData.updateBeforeElementDelete(element, preferEarlierPosition);
        selectionCursorData.updateBeforeElementDelete(element, preferEarlierPosition);
    }


    void CursorStateData::updateBeforeRangeDelete(
            const ElementCursor& startingCursor,
            const ElementCursor& endingCursor
        ) {
        ElementCursor elementCursor   = elementCursorData.toElementCursor();
        ElementCursor selectionCursor = selectionCursorData.toElementCursor();

        bool startingCursorBeforeEndingCursor = (startingCursor < endingCursor);
        const ElementCursor rangeTopCursor    = startingCursorBeforeEndingCursor ? startingCursor : endingCursor;
        const ElementCursor rangeBottomCursor = startingCursorBeforeEndingCursor ? endingCursor   : startingCursor;

        elementCursorData.updateBeforeRangeDelete(rangeTopCursor, rangeBottomCursor);
        selectionCursorData.updateBeforeRangeDelete(rangeTopCursor, rangeBottomCursor);

        if (elementCursorData == selectionCursorData) {
            selectionCursorData = CursorStateData::CursorData();
        }
    }


    CursorStateData& CursorStateData::operator=(const CursorStateData& other) {
        elementCursorData = other.elementCursorData;
        selectionCursorData = other.selectionCursorData;

        return *this;
    }


    bool CursorStateData::operator==(const CursorStateData& other) const {
        return elementCursorData == other.elementCursorData && selectionCursorData == other.selectionCursorData;
    }


    bool CursorStateData::operator!=(const CursorStateData& other) const {
        return elementCursorData != other.elementCursorData || selectionCursorData != other.selectionCursorData;
    }
}
