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
* This file implements the \ref Ld::CursorStateCollection::Private class and other derived classes.
***********************************************************************************************************************/

#include <QSharedPointer>
#include <QWeakPointer>
#include <QSet>

#include "ld_element_cursor.h"
#include "ld_element.h"
#include "ld_cursor.h"
#include "ld_cursor_state_collection.h"
#include "ld_cursor_state_collection_private.h"

/***********************************************************************************************************************
 * CursorStateData::Private
 */

namespace Ld {
    CursorStateData::Private::Private() {}


    CursorStateData::Private::~Private() {}


    void CursorStateData::Private::updateAfterTextInsertion(QSharedPointer<Element>, unsigned long, unsigned, long) {}


    void CursorStateData::Private::updateAfterSplit(QSharedPointer<Element>, QSharedPointer<Element>) {}


    void CursorStateData::Private::updateBeforeMerge(QSharedPointer<Element>, QSharedPointer<Element>) {}


    void CursorStateData::Private::updateBeforeDelete(QSharedPointer<Element>, bool) {}
}

/***********************************************************************************************************************
 * CursorStateData::Invalid
 */

namespace Ld {
    CursorStateData::Invalid::Invalid() {}


    CursorStateData::Invalid::~Invalid() {}


    CursorStateData::Private* CursorStateData::Invalid::clone() const {
        return new CursorStateData::Invalid;
    }


    CursorTrackingMechanism CursorStateData::Invalid::trackingMechanism() const {
        return CursorTrackingMechanism::INVALID;
    }


    ElementCursor CursorStateData::Invalid::toElementCursor() const {
        return ElementCursor();
    }
}

/***********************************************************************************************************************
 * CursorStateData::WholeElement
 */

namespace Ld {
    CursorStateData::WholeElement::WholeElement(QSharedPointer<Element> element) {
        currentElement = element;
    }


    CursorStateData::WholeElement::~WholeElement() {}


    CursorStateData::Private* CursorStateData::WholeElement::clone() const {
        return new CursorStateData::WholeElement(currentElement);
    }


    CursorTrackingMechanism CursorStateData::WholeElement::trackingMechanism() const {
        return CursorTrackingMechanism::WHOLE_ELEMENT;
    }


    void CursorStateData::WholeElement::setElement(QSharedPointer<Element> newElement) {
        currentElement = newElement;
    }


    QSharedPointer<Element> CursorStateData::WholeElement::element() const {
        return currentElement;
    }


    ElementCursor CursorStateData::WholeElement::toElementCursor() const {
        ElementCursor newCursor;

        if (!currentElement.isNull()) {
            newCursor = ElementCursor(currentElement);
        }

        return newCursor;
    }


    void CursorStateData::WholeElement::updateBeforeMerge(
            QSharedPointer<Element> leftElement,
            QSharedPointer<Element> rightElement
        ) {
        if (currentElement == rightElement) {
            currentElement = leftElement;
        }
    }


    void CursorStateData::WholeElement::updateBeforeDelete(
            QSharedPointer<Element> element,
            bool                    preferEarlierPosition
        ) {
        ElementCursor newCursor(currentElement);

        if (currentElement == element || element->descendants().contains(currentElement)) {
            newCursor.setElement(element);
            newCursor.fixPosition(!preferEarlierPosition, false);

            if (preferEarlierPosition) {
                bool isFirst = newCursor.moveBackwardByElement();
                newCursor.fixPosition(false, isFirst);

                if (newCursor.element() == currentElement) {
                    bool isLast = newCursor.moveForwardByElement();
                    newCursor.fixPosition(true, isLast);

                    if (newCursor.element() == currentElement) {
                        newCursor.setInvalid();
                    }
                }
            } else {
                bool isLast = newCursor.moveForwardByElement();
                newCursor.fixPosition(true, isLast);

                if (newCursor.element() == currentElement) {
                    bool isFirst = newCursor.moveBackwardByElement();
                    newCursor.fixPosition(false, isFirst);

                    if (newCursor.element() == currentElement) {
                        newCursor.setInvalid();
                    }
                }
            }
        }

        if (newCursor.isValid()) {
            currentElement = newCursor.element();
        } else {
            currentElement.reset();
        }
    }
}

/***********************************************************************************************************************
 * CursorStateData::ParentIndex
 */

namespace Ld {
    CursorStateData::ParentIndex::ParentIndex(QSharedPointer<Element> parent, unsigned long childIndex) {
        currentParent     = parent;
        currentChildIndex = childIndex;
    }


    CursorStateData::ParentIndex::~ParentIndex() {}


    CursorStateData::Private* CursorStateData::ParentIndex::clone() const {
        return new CursorStateData::ParentIndex(currentParent, currentChildIndex);
    }


    CursorTrackingMechanism CursorStateData::ParentIndex::trackingMechanism() const {
        return CursorTrackingMechanism::PARENT_AND_CHILD_INDEX;
    }


    void CursorStateData::ParentIndex::setParent(QSharedPointer<Element> newParent) {
        currentParent = newParent;
    }


    QSharedPointer<Element> CursorStateData::ParentIndex::parent() const {
        return currentParent;
    }


    void CursorStateData::ParentIndex::setChildIndex(unsigned long newChildIndex) {
        currentChildIndex = newChildIndex;
    }


    unsigned long CursorStateData::ParentIndex::childIndex() const {
        return currentChildIndex;
    }


    ElementCursor CursorStateData::ParentIndex::toElementCursor() const {
        return ElementCursor(currentParent, currentChildIndex);
    }


    void CursorStateData::ParentIndex::updateAfterSplit(QSharedPointer<Element>, QSharedPointer<Element> newElement) {
        QSharedPointer<Element> newElementParent = newElement->parent();

        if (newElementParent == currentParent) {
            unsigned long newChildIndex = currentParent->indexOfChild(newElement);
            if (newChildIndex <= currentChildIndex) {
                ++currentChildIndex;
            }
        }
    }


    void CursorStateData::ParentIndex::updateBeforeMerge(
            QSharedPointer<Element> leftElement,
            QSharedPointer<Element> rightElement
        ) {
        if (currentParent->child(currentChildIndex) == rightElement) {
            currentParent     = leftElement->parent();
            currentChildIndex = currentParent->indexOfChild(leftElement);
        } else {
            QSharedPointer<Element> parent = rightElement->parent();

            if (parent == currentParent && parent->indexOfChild(rightElement) > currentChildIndex) {
                --currentChildIndex;
            }
        }
    }


    void CursorStateData::ParentIndex::updateBeforeDelete(
            QSharedPointer<Element> element,
            bool                    preferEarlierPosition
        ) {
        ElementCursor newCursor(currentParent, currentChildIndex);
        newCursor.fixPosition(!preferEarlierPosition, false);

        QSharedPointer<Element> currentElement = newCursor.element();
        bool                    walkBackwards  = preferEarlierPosition;

        while (!currentElement.isNull()                                                       &&
               (currentElement == element || element->descendants().contains(currentElement))    ) {
            bool reverseDirection = false;
            if (walkBackwards) {
                reverseDirection = newCursor.moveBackwardByElement();
            } else {
                reverseDirection = newCursor.moveForwardByElement();
            }

            reverseDirection = newCursor.fixPosition(!walkBackwards, reverseDirection);

            if (reverseDirection) {
                walkBackwards = !walkBackwards;
            }

            currentElement = newCursor.element();
        }

        currentParent = newCursor.parent();
        if (currentParent == element->parent() && currentParent->indexOfChild(element) > currentChildIndex) {
            --currentChildIndex;
        }

        currentChildIndex = newCursor.childIndex();
    }
}

/***********************************************************************************************************************
 * CursorStateData::TextRegionIndex
 */

namespace Ld {
    CursorStateData::TextRegionIndex::TextRegionIndex(
            QSharedPointer<Element> element,
            unsigned long           textIndex,
            unsigned                regionIndex
        ):WholeElement(
             element
        ) {
        currentTextIndex   = textIndex;
        currentRegionIndex = regionIndex;
    }


    CursorStateData::TextRegionIndex::~TextRegionIndex() {}


    CursorStateData::Private* CursorStateData::TextRegionIndex::clone() const {
        return new CursorStateData::TextRegionIndex(currentElement, currentTextIndex, currentRegionIndex);
    }


    CursorTrackingMechanism CursorStateData::TextRegionIndex::trackingMechanism() const {
        return CursorTrackingMechanism::TEXT_REGION_INDEX;
    }


    unsigned long CursorStateData::TextRegionIndex::textIndex() const {
        return currentTextIndex;
    }


    unsigned CursorStateData::TextRegionIndex::regionIndex() const {
        return currentRegionIndex;
    }


    ElementCursor CursorStateData::TextRegionIndex::toElementCursor() const {
        return ElementCursor(currentTextIndex, currentRegionIndex, element());
    }


    void CursorStateData::TextRegionIndex::updateAfterTextInsertion(
            QSharedPointer<Element> element,
            unsigned long           textIndex,
            unsigned                regionIndex,
            long                    insertionLength
        ) {
        if (element == TextRegionIndex::element() && regionIndex == currentRegionIndex) {
            if ((insertionLength >= 0          &&
                 textIndex <= currentTextIndex    )                   ||
                (insertionLength < 0                             &&
                 textIndex - insertionLength <= currentTextIndex    )    ) {
                currentTextIndex += insertionLength;
            }
        }
    }


    void CursorStateData::TextRegionIndex::updateAfterSplit(
            QSharedPointer<Element> oldElement,
            QSharedPointer<Element> newElement
        ) {
        if (element() == oldElement) {
            unsigned long oldTextLength = static_cast<unsigned long>(oldElement->text(currentRegionIndex).length());
            if (currentTextIndex >= oldTextLength) {
                currentElement    = newElement;
                currentTextIndex -= oldTextLength;
            }
        }
    }


    void CursorStateData::TextRegionIndex::updateBeforeMerge(
            QSharedPointer<Element> leftElement,
            QSharedPointer<Element> rightElement
        ) {
        if (currentElement == rightElement) {
            unsigned long leftTextLength = static_cast<unsigned long>(leftElement->text(currentRegionIndex).length());

            currentElement    = leftElement;
            currentTextIndex += leftTextLength;
        }
    }


    void CursorStateData::TextRegionIndex::
//    ElementCursor CursorStateData::TextRegionIndex::elementCursor(
//            QSharedPointer<Element> element,
//            unsigned long           textIndex,
//            unsigned                regionIndex,
//            long                    insertionLength
//        ) const {
//    }


//    ElementCursor CursorStateData::TextRegionIndex::elementCursorAfterSplit(
//            QSharedPointer<Element> oldElement,
//            QSharedPointer<Element> newElement,
//            unsigned long           textIndex,
//            unsigned                regionIndex
//        ) const {
//    }


//    ElementCursor CursorStateData::TextRegionIndex::elementCursorAfterMerge(
//            QSharedPointer<Element> leftElement,
//            QSharedPointer<Element> rightElement
//        ) const {
//        ElementCursor newCursor;

//        if (currentElement == rightElement) {

//        }

//        if (currentElement == firstElement && currentRegionIndex == regionIndex) {
//            result = ElementCursor(currentTextIndex, regionIndex, secondElement);
//        } else if (currentElement == secondElement && currentRegionIndex == regionIndex) {
//            result = ElementCursor(currentTextIndex + textIndex, regionIndex, secondElement);
//        } else {
//            result = ElementCursor(currentTextIndex, currentRegionIndex, currentElement);
//        }

//        return result;
//    }


//    ElementCursor CursorStateData::TextRegionIndex::elementCursorAfterDelete(
//            QSharedPointer<Element> element,
//            bool                    preferEarlierPosition
//        ) const {
//        ElementCursor newCursor(currentTextIndex, currentRegionIndex, currentElement);

//        if (currentElement == element || element->descendants().contains(currentElement)) {
//            newCursor.setElement(element);
//            newCursor.fixPosition(true, false);

//            if (preferEarlierPosition) {
//                bool isFirst = newCursor.moveBackwardByElement();
//                newCursor.fixPosition(false, isFirst);

//                if (newCursor.element() == currentElement) {
//                    bool isLast = newCursor.moveForwardByElement();
//                    newCursor.fixPosition(true, isLast);

//                    if (newCursor.element() == currentElement) {
//                        newCursor.setInvalid();
//                    } else {
//                        newCursor.moveToFirstPositionInElement();
//                    }
//                } else {
//                    newCursor.moveToLastPositionInElement();
//                }
//            } else {
//                bool isLast = newCursor.moveForwardByElement();
//                newCursor.fixPosition(true, isLast);

//                if (newCursor.element() == currentElement) {
//                    bool isFirst = newCursor.moveBackwardByElement();
//                    newCursor.fixPosition(false, isFirst);

//                    if (newCursor.element() == currentElement) {
//                        newCursor.setInvalid();
//                    } else {
//                        newCursor.moveToLastPositionInElement();
//                    }
//                } else {
//                    newCursor.moveToFirstPositionInElement();
//                }
//            }
//        }

//        return newCursor;
//    }
}
