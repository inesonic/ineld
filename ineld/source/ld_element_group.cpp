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
* This file implements the \ref Ld::ElementGroup class.
***********************************************************************************************************************/

#include <QList>
#include <QSharedPointer>

#include "ld_element_structures.h"
#include "ld_element.h"
#include "ld_element_group.h"

namespace Ld {
    ElementGroup::ElementGroup() {}


    ElementGroup::ElementGroup(const ElementGroup& other):ElementPointerList(other) {}


    ElementGroup::ElementGroup(const ElementPointerList list):ElementPointerList(list) {}


    ElementGroup::~ElementGroup() {}


    ElementPointer ElementGroup::precedingElement() const {
        ElementPointer result;

        if (!isEmpty()) {
            result = first()->previousSibling();
        }

        return result;
    }


    ElementPointer ElementGroup::followingElement() const {
        ElementPointer result;

        if (!isEmpty()) {
            result = last()->nextSibling();
        }

        return result;
    }


    QList<ElementGroup> ElementGroup::convertToGroups(const ElementPointerList& elements) {
        typedef QHash<ElementPointer, unsigned> ListHash;

        ElementGroupList result;
        ListHash         elementInGroup;

        for (  ElementPointerList::const_iterator elementIterator    = elements.constBegin(),
                                                  elementEndIterator = elements.constEnd()
             ; elementIterator != elementEndIterator
             ; ++elementIterator
            ) {
            ElementPointer element = *elementIterator;
            if (!element.isNull()) {
                ElementPointer previousSibling = element->previousSibling();
                ElementPointer nextSibling     = element->nextSibling();

                if (elementInGroup.contains(previousSibling)) {
                    unsigned previousGroupIndex = elementInGroup.value(previousSibling);
                    ElementGroup& previousGroup = result[previousGroupIndex];

                    previousGroup.append(element);
                    elementInGroup.insert(element, previousGroupIndex);

                    if (elementInGroup.contains(nextSibling)) {
                        unsigned nextGroupIndex = elementInGroup.value(nextSibling);
                        const ElementGroup& nextGroup = result[nextGroupIndex];
                        previousGroup += nextGroup;

                        for (  ElementGroup::const_iterator nextGroupIterator    = nextGroup.constBegin(),
                                                            nextGroupEndIterator = nextGroup.constEnd()
                             ; nextGroupIterator != nextGroupEndIterator
                             ; ++nextGroupIterator
                            ) {
                            ElementPointer movedElement = *nextGroupIterator;
                            elementInGroup[movedElement] = previousGroupIndex;
                        }
                    }
                } else if (elementInGroup.contains(nextSibling)) {
                    unsigned nextGroupIndex = elementInGroup.value(nextSibling);
                    ElementGroup& nextGroup = result[nextGroupIndex];

                    nextGroup.prepend(element);
                    elementInGroup.insert(element, nextGroupIndex);
                } else {
                    unsigned newGroupIndex = static_cast<unsigned>(result.size());
                    ElementGroup newGroup;
                    newGroup << element;

                    result.append(newGroup);
                    elementInGroup.insert(element, newGroupIndex);
                }
            }
        }

        if (!result.isEmpty()) {
            QList<unsigned> validGroupIndexList = elementInGroup.values();
            QSet<unsigned> validGroupIndexes(validGroupIndexList.begin(), validGroupIndexList.end());
            for (int groupIndex=result.size()-1 ; groupIndex >= 0 ; --groupIndex) {
                if (!validGroupIndexes.contains(groupIndex)) {
                    result.removeAt(groupIndex);
                }
            }
        }

        return result;
    }


    ElementGroup& ElementGroup::operator=(const ElementGroup& other) {
        ElementPointerList::operator=(other);
        return *this;
    }
}
