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
* This file implements the \ref Ld::ElementPosition class.
***********************************************************************************************************************/

#include <QSharedPointer>

#include <cassert>
#include <algorithm>

#include "ld_element_structures.h"
#include "ld_element.h"
#include "ld_element_position.h"

namespace Ld {
    ElementPosition::ElementPosition() {
        currentRoot.clear();
        currentBranchPaths.clear();
    }


    ElementPosition::ElementPosition(ElementPointer newElement) {
        setElement(newElement);
    }


    ElementPosition::ElementPosition(ElementPointer newParent, unsigned long childIndex) {
        setElementUnderParent(newParent, childIndex);
    }


    ElementPosition::ElementPosition(const ElementPosition& other) {
        currentRoot        = other.currentRoot;
        currentBranchPaths = other.currentBranchPaths;
    }


    ElementPosition::~ElementPosition() {}


    bool ElementPosition::isValid() const {
        ElementPointer element        = currentRoot.toStrongRef();
        unsigned       numberBranches = static_cast<unsigned>(currentBranchPaths.size());
        unsigned long  branchIndex    = Element::invalidChildIndex;
        unsigned       index          = 0;
        ElementPointer parent;

        while (!element.isNull() && index < numberBranches) {
            branchIndex = currentBranchPaths.at(index);
            parent      = element;
            element     = element->child(branchIndex);
            ++index;
        }

        return (index == numberBranches) && !parent.isNull() && branchIndex < parent->numberChildren();
    }


    bool ElementPosition::isInvalid() const {
        return !isValid();
    }


    void ElementPosition::setElement(ElementPointer newElement) {
        currentRoot.clear();
        currentBranchPaths.clear();

        ElementPointer element = newElement;
        while (!element.isNull()) {
            ElementPointer parent = element->parent();

            if (!parent.isNull()) {
                unsigned long branchIndex = parent->indexOfChild(element);
                currentBranchPaths.prepend(branchIndex);
            }

            currentRoot = element.toWeakRef();
            element     = parent;
        }
    }


    void ElementPosition::setElementUnderParent(ElementPointer newParent, unsigned long childIndex) {
        setElement(newParent);
        currentBranchPaths.append(childIndex);
    }


    void ElementPosition::setInvalid() {
        currentRoot.clear();
        currentBranchPaths.clear();
    }


    ElementPointer ElementPosition::element() const {
        ElementPointer element = currentRoot.toStrongRef();

        unsigned numberBranches = static_cast<unsigned>(currentBranchPaths.size());
        for (unsigned branchIndex=0 ; branchIndex<numberBranches ; ++branchIndex) {
            Q_ASSERT(!element.isNull());

            unsigned long childIndex     = currentBranchPaths.at(branchIndex);
            unsigned long numberChildren = element->numberChildren();

            Q_ASSERT(childIndex < numberChildren);

            element = element->child(childIndex);
        }

        return element;
    }


    ElementPointer ElementPosition::parent() const {
        ElementPointer element = currentRoot.toStrongRef();

        unsigned numberBranches = static_cast<unsigned>(currentBranchPaths.size());
        if (numberBranches > 1) {
            --numberBranches;
            for (unsigned branchIndex=0 ; branchIndex<numberBranches ; ++branchIndex) {
                Q_ASSERT(!element.isNull());

                unsigned long childIndex     = currentBranchPaths.at(branchIndex);
                unsigned long numberChildren = element->numberChildren();

                Q_ASSERT(childIndex < numberChildren);

                element = element->child(childIndex);
            }
        }

        return element;
    }


    ElementPointer ElementPosition::root() const {
        return currentRoot.toStrongRef();
    }


    unsigned long ElementPosition::childIndex() const {
        return currentBranchPaths.isEmpty() ? Element::invalidChildIndex : currentBranchPaths.last();
    }


    unsigned ElementPosition::depth() const {
        return static_cast<unsigned>(currentBranchPaths.size());
    }


    ElementPosition ElementPosition::nextElement() const {
        ElementPosition newPosition(*this);
        newPosition.moveForwardByElement();

        return newPosition;
    }


    ElementPosition ElementPosition::previousElement() const {
        ElementPosition newPosition(*this);
        newPosition.moveBackwardByElement();

        return newPosition;
    }


    ElementPosition ElementPosition::firstElement() const {
        ElementPosition newPosition(*this);
        newPosition.moveToFirstElement();

        return newPosition;
    }


    ElementPosition ElementPosition::lastElement() const {
        ElementPosition newPosition(*this);
        newPosition.moveToLastElement();

        return newPosition;
    }


    bool ElementPosition::moveForwardByElement() {
        bool isLast = false;

        if (!currentBranchPaths.isEmpty()) {
            ElementPointer element = ElementPosition::element();
            if (!element.isNull() && element->numberChildren() > 0) {
                currentBranchPaths.append(0);
            } else {
                ElementPointer parent         = element.isNull() ? ElementPosition::parent() : element->parent();
                unsigned       numberChildren = parent->numberChildren();
                unsigned       nextChildIndex = currentBranchPaths.last() + 1;

                while (!parent.isNull() && nextChildIndex >= numberChildren) {
                    element = parent;
                    parent  = element->parent();

                    if (!parent.isNull()) {
                        currentBranchPaths.removeLast();
                        numberChildren = parent->numberChildren();
                        nextChildIndex = currentBranchPaths.last() + 1;
                    }
                }

                if (parent.isNull()) {
                    isLast = true;
                    moveToLastElement();
                } else {
                    currentBranchPaths.last() = nextChildIndex;
                }
            }
        } else {
            isLast = true;
        }

        return isLast;
    }


    bool ElementPosition::moveBackwardByElement() {
        bool isFirst = false;

        if (!currentBranchPaths.isEmpty()) {
            unsigned long childIndex = currentBranchPaths.last();
            if (childIndex > 0) {
                currentBranchPaths.last() = childIndex - 1;

                ElementPointer element = ElementPosition::element();
                while (!element.isNull()) {
                    unsigned long numberChildren = element->numberChildren();
                    if (numberChildren > 0) {
                        unsigned long childIndex = numberChildren - 1;
                        currentBranchPaths.append(childIndex);
                        element = element->child(childIndex);
                    } else {
                        element.clear();
                    }
                }
            } else {
                currentBranchPaths.removeLast();
                if (currentBranchPaths.isEmpty()) {
                    currentBranchPaths.append(0);
                    isFirst = true;
                }
            }
        } else {
            isFirst = true;
        }

        return isFirst;
    }


    void ElementPosition::moveToFirstElement() {
        currentBranchPaths.clear();

        ElementPointer root = currentRoot.toStrongRef();
        if (root->numberChildren() > 0) {
            currentBranchPaths.append(0);
        }
    }


    void ElementPosition::moveToLastElement() {
        currentBranchPaths.clear();
        ElementPointer element = currentRoot.toStrongRef();
        while (!element.isNull()) {
            unsigned long numberChildren = element->numberChildren();

            if (numberChildren > 0) {
                unsigned long childIndex = numberChildren - 1;
                currentBranchPaths.append(childIndex);
                element = element->child(childIndex);
            } else {
                element.clear();
            }
        }
    }


    ElementPointerList ElementPosition::elementStack() const {
        ElementPointerList elements;

        ElementPointer parent = ElementPosition::parent();
        if (!parent.isNull()) {
            elements.append(parent);

            unsigned long  childIndex = ElementPosition::childIndex();
            ElementPointer element    = parent->child(childIndex);
            elements.append(element);

            parent = parent->parent();
            while (!parent.isNull()) {
                elements.prepend(parent);
                parent = parent->parent();
            };
        }

        return elements;
    }


    ElementPosition& ElementPosition::operator=(const ElementPosition& other) {
        currentRoot        = other.currentRoot;
        currentBranchPaths = other.currentBranchPaths;

        return *this;
    }


    bool ElementPosition::operator<(const ElementPosition& other) const {
        bool precedes;

        if (currentRoot == other.currentRoot) {
            unsigned numberBranches        = static_cast<unsigned>(currentBranchPaths.size());
            unsigned otherNumberBranches   = static_cast<unsigned>(other.currentBranchPaths.size());
            unsigned minimumNumberBranches = std::min(numberBranches, otherNumberBranches);
            unsigned branchIndex           = 0;

            while (branchIndex < minimumNumberBranches                                            &&
                   currentBranchPaths.at(branchIndex) == other.currentBranchPaths.at(branchIndex)    ) {
                ++branchIndex;
            }

            if (branchIndex < minimumNumberBranches) {
                precedes = currentBranchPaths.at(branchIndex) < other.currentBranchPaths.at(branchIndex);
            } else {
                precedes = (numberBranches < otherNumberBranches);
            }
        } else {
            precedes = false;
        }

        return precedes;
    }


    bool ElementPosition::operator==(const ElementPosition& other) const {
        return other.currentRoot == currentRoot && other.currentBranchPaths == currentBranchPaths;
    }


    bool ElementPosition::operator!=(const ElementPosition& other) const {
        return other.currentRoot != currentRoot || other.currentBranchPaths != currentBranchPaths;
    }


    bool ElementPosition::operator>(const ElementPosition& other) const {
        bool follows;

        if (currentRoot == other.currentRoot) {
            unsigned numberBranches        = static_cast<unsigned>(currentBranchPaths.size());
            unsigned otherNumberBranches   = static_cast<unsigned>(other.currentBranchPaths.size());
            unsigned minimumNumberBranches = std::min(numberBranches, otherNumberBranches);
            unsigned branchIndex           = 0;

            while (branchIndex < minimumNumberBranches                                            &&
                   currentBranchPaths.at(branchIndex) == other.currentBranchPaths.at(branchIndex)    ) {
                ++branchIndex;
            }

            if (branchIndex < minimumNumberBranches) {
                follows = currentBranchPaths.at(branchIndex) > other.currentBranchPaths.at(branchIndex);
            } else {
                follows = (numberBranches > otherNumberBranches);
            }
        } else {
            follows = false;
        }

        return follows;
    }


    bool ElementPosition::operator<=(const ElementPosition& other) const {
        return operator==(other) || operator<(other);
    }


    bool ElementPosition::operator>=(const ElementPosition& other) const {
        return operator==(other) || operator>(other);
    }
}


