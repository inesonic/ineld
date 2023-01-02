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
* This file implements the \ref Ld::ElementWithGroupedChildren class.
***********************************************************************************************************************/

#include <QList>
#include <QSharedPointer>

#include "ld_element_structures.h"
#include "ld_format_organizer.h"
#include "ld_xml_writer.h"
#include "ld_xml_attributes.h"
#include "ld_program_file.h"
#include "ld_cursor_state_collection.h"
#include "ld_visual_with_grouped_children.h"
#include "ld_element_with_positional_children.h"
#include "ld_element_with_grouped_children.h"

namespace Ld {
    const unsigned ElementWithGroupedChildren::invalidGroup = static_cast<unsigned>(-1);


    ElementWithGroupedChildren::ElementWithGroupedChildren() {
        firstChildIndexInGroup.append(0);
    }


    Element::ChildPlacement ElementWithGroupedChildren::childPlacement() const {
        return Element::ChildPlacement::GROUPED;
    }


    ElementWithGroupedChildren::~ElementWithGroupedChildren() {}


    ElementPointer ElementWithGroupedChildren::clone(bool includeChildren) const {
        QSharedPointer<ElementWithGroupedChildren> element = cloneThisElementOnly()
                                                             .dynamicCast<ElementWithGroupedChildren>();

        if (!element.isNull()) {
            if (includeChildren) {
                bool success = cloneChildren(element);
                if (!success) {
                    element.clear();
                }
            } else {
                element->firstChildIndexInGroup.clear();
                unsigned numberGroups = static_cast<unsigned>(firstChildIndexInGroup.size());
                for (unsigned groupIndex=0 ; groupIndex<numberGroups ; ++groupIndex) {
                    element->firstChildIndexInGroup.append(0);
                }
            }
        }

        return element;
    }


    ElementPointer ElementWithGroupedChildren::clone(
            unsigned long startingTextIndex,
            unsigned      startingRegionIndex,
            unsigned long endingTextIndex,
            unsigned      endingRegionIndex,
            bool          includeChildren
        ) const {
        QSharedPointer<ElementWithGroupedChildren> element = cloneThisElementOnly(
            startingTextIndex,
            startingRegionIndex,
            endingTextIndex,
            endingRegionIndex
        ).dynamicCast<ElementWithGroupedChildren>();

        if (!element.isNull()) {
            if (includeChildren) {
                bool success = cloneChildren(element);
                if (!success) {
                    element.clear();
                }
            } else {
                element->firstChildIndexInGroup.clear();
                unsigned numberGroups = static_cast<unsigned>(firstChildIndexInGroup.size());
                for (unsigned groupIndex=0 ; groupIndex<numberGroups ; ++groupIndex) {
                    element->firstChildIndexInGroup.append(0);
                }
            }
        }

        return element;
    }


    void ElementWithGroupedChildren::setVisual(VisualWithGroupedChildren* newVisual) {
        ElementWithPositionalChildren::setVisual(newVisual);
    }


    VisualWithGroupedChildren* ElementWithGroupedChildren::visual() const {
        return dynamic_cast<VisualWithGroupedChildren*>(ElementWithPositionalChildren::visual());
    }


    bool ElementWithGroupedChildren::removeChild(
            unsigned long          childIndex,
            CursorStateCollection* cursorStateCollection
        ) {
        bool success;

        if (childIndex < numberChildren()) {
            unsigned long childIndexInGroup;
            unsigned      groupIndex = groupContainingChild(childIndex, &childIndexInGroup);

            success = removeChildFromGroup(groupIndex, childIndexInGroup, cursorStateCollection);
        } else {
            success = false;
        }

        return success;
    }


    void ElementWithGroupedChildren::removeChildren(CursorStateCollection* cursorStateCollection) {
        ElementWithPositionalChildren::removeChildren(cursorStateCollection);

        unsigned numberGroups = static_cast<unsigned>(firstChildIndexInGroup.size());
        for (unsigned groupIndex=0 ; groupIndex<numberGroups ; ++groupIndex) {
            firstChildIndexInGroup[groupIndex] = 0;
        }
    }


    void ElementWithGroupedChildren::insertBefore(
            unsigned long          childIndex,
            ElementPointer         childElement,
            CursorStateCollection* cursorStateCollection
        ) {
        unsigned long childIndexInGroup;
        unsigned      groupIndex;

        if (childIndex == invalidChildIndex || childIndex >= numberChildren()) {
            groupIndex        = invalidGroup;
            childIndexInGroup = invalidChildIndex;
        } else {
            groupIndex = groupContainingChild(childIndex, &childIndexInGroup);
        }

        insertIntoGroupBefore(groupIndex, childIndexInGroup, childElement, cursorStateCollection);
    }


    void ElementWithGroupedChildren::insertAfter(
            unsigned long          childIndex,
            ElementPointer         childElement,
            CursorStateCollection* cursorStateCollection
        ) {
        unsigned long childIndexInGroup;
        unsigned      groupIndex;

        if (childIndex == invalidChildIndex || childIndex >= numberChildren()) {
            groupIndex        = invalidGroup;
            childIndexInGroup = invalidChildIndex;
        } else {
            groupIndex = groupContainingChild(childIndex, &childIndexInGroup);
        }

        insertIntoGroupAfter(groupIndex, childIndexInGroup, childElement, cursorStateCollection);
    }


    unsigned ElementWithGroupedChildren::numberGroups() const {
        return static_cast<unsigned>(firstChildIndexInGroup.size());
    }


    unsigned long ElementWithGroupedChildren::numberChildrenInGroup(unsigned groupIndex) const {
        unsigned long result;

        unsigned      numberGroups = static_cast<unsigned>(firstChildIndexInGroup.size());

        if (groupIndex < numberGroups) {
            result = baseChildIndex(groupIndex + 1) - baseChildIndex(groupIndex);
        } else {
            result = invalidChildIndex;
        }

        return result;
    }


    ElementPointer ElementWithGroupedChildren::childInGroup(
            unsigned      groupIndex,
            unsigned long childIndexInGroup
        ) const {
        ElementPointer result;

        unsigned long childIndex = childIndexFromGroup(groupIndex, childIndexInGroup);
        if (childIndex != invalidChildIndex) {
            result = child(childIndex);
        }

        return result;
    }


    ElementPointerList ElementWithGroupedChildren::childrenInGroup(unsigned groupIndex) const {
        ElementPointerList result;
        unsigned           numberGroups = static_cast<unsigned>(firstChildIndexInGroup.size());

        if (groupIndex < numberGroups) {
            unsigned long groupBaseIndex = firstChildIndexInGroup.at(groupIndex);
            unsigned long numberChildren = baseChildIndex(groupIndex + 1) - baseChildIndex(groupIndex);

            result = children().mid(groupBaseIndex, numberChildren);
        }

        return result;
    }


    unsigned long ElementWithGroupedChildren::childIndexFromGroup(
            unsigned      groupIndex,
            unsigned long childIndexInGroup
        ) const {
        unsigned long result;
        unsigned      numberGroups = ElementWithGroupedChildren::numberGroups();

        if (groupIndex < numberGroups) {
            unsigned      nextGroupIndex = groupIndex + 1;
            unsigned long groupBaseIndex = firstChildIndexInGroup.at(groupIndex);

            unsigned long numberChildrenInGroup;
            if (nextGroupIndex < numberGroups) {
                numberChildrenInGroup = firstChildIndexInGroup.at(nextGroupIndex) - groupBaseIndex;
            } else {
                numberChildrenInGroup = numberChildren() - groupBaseIndex;
            }

            if (childIndexInGroup < numberChildrenInGroup) {
                result = groupBaseIndex + childIndexInGroup;
            } else {
                result = invalidChildIndex;
            }
        } else {
            result = invalidChildIndex;
        }

        return result;
    }


    unsigned ElementWithGroupedChildren::groupContainingChild(
            unsigned long  childIndex,
            unsigned long* childIndexInGroup
        ) const {
        unsigned result;

        unsigned long numberChildren = ElementWithGroupedChildren::numberChildren();
        if (childIndex < numberChildren) {
            // We locate the group using a binary search.  The firstChildIndexInGroup list is designed to be
            // monotonically increasing so a binary search (or Newton-Raphson) will work nicely.

            unsigned numberGroups    = static_cast<unsigned>(firstChildIndexInGroup.size());
            unsigned lowerGroupIndex = 0;
            unsigned upperGroupIndex = numberGroups; // One past so result can reach the last entry in all cases.

            result = (lowerGroupIndex + upperGroupIndex) / 2;
            unsigned long baseChildIndex = ElementWithGroupedChildren::baseChildIndex(result);

            while (upperGroupIndex - lowerGroupIndex > 1) {
                if (baseChildIndex > childIndex) {
                    upperGroupIndex = result;
                } else {
                    lowerGroupIndex = result;
                }

                result = (lowerGroupIndex + upperGroupIndex) / 2;
                baseChildIndex = ElementWithGroupedChildren::baseChildIndex(result);
            }

            assert(baseChildIndex <= childIndex); // Sanity check

            // The result may point to a group with no children.  We check for that below and walk the group number
            // upward until we find the first group with at least one child.

            while (result < numberGroups                                                      &&
                   baseChildIndex == ElementWithGroupedChildren::baseChildIndex((result + 1))    ) {
                ++result;
            }

            assert(result < numberGroups);

            if (childIndexInGroup != nullptr) {
                *childIndexInGroup = childIndex - baseChildIndex;
            }
        } else {
            result = invalidGroup;
            if (childIndexInGroup != nullptr) {
                *childIndexInGroup = invalidChildIndex;
            }
        }

        return result;
    }


    bool ElementWithGroupedChildren::removeChildFromGroup(
            unsigned               groupIndex,
            unsigned long          childIndexInGroup,
            CursorStateCollection* cursorStateCollection
        ) {
        bool          success;
        unsigned long childIndex = childIndexFromGroup(groupIndex, childIndexInGroup);

        if (childIndex != invalidChildIndex) {
            if (visual() != nullptr) {
                ElementPointer element = child(childIndex);
                visual()->aboutToRemoveChildFromGroup(childIndex, groupIndex, childIndexInGroup, element);
            }

            success = ElementWithPositionalChildren::removeChild(childIndex, cursorStateCollection);

            if (success) {
                unsigned numberGroups = static_cast<unsigned>(firstChildIndexInGroup.size());
                for (unsigned nextGroup=groupIndex+1 ; nextGroup<numberGroups ; ++nextGroup) {
                    --firstChildIndexInGroup[nextGroup];
                }
            }
        } else {
            success = false;
        }

        return success;
    }


    bool ElementWithGroupedChildren::removeChildrenFromGroup(
            unsigned               groupIndex,
            CursorStateCollection* cursorStateCollection
        ) {
        bool success;

        if (groupIndex < static_cast<unsigned>(firstChildIndexInGroup.size())) {
            unsigned long childIndex = childIndexFromGroup(groupIndex, 0);
            if (childIndex != invalidChildIndex) {
                unsigned long numberChildrenInGroup = ElementWithGroupedChildren::numberChildrenInGroup(groupIndex);

                assert(childIndex < numberChildren());

                if (visual() != nullptr) {
                    visual()->aboutToRemoveChildrenFromGroup(childIndex, numberChildrenInGroup, groupIndex);
                }

                success = true;
                unsigned long numberRemovedChildren = 0;
                while (success && numberRemovedChildren < numberChildrenInGroup) {
                    success = ElementWithPositionalChildren::removeChild(childIndex, cursorStateCollection);
                    if (success) {
                        ++numberRemovedChildren;
                    }
                }

                unsigned numberGroups = static_cast<unsigned>(firstChildIndexInGroup.size());
                for (unsigned nextGroup=groupIndex+1 ; nextGroup<numberGroups ; ++nextGroup) {
                    assert(firstChildIndexInGroup[nextGroup] >= childIndex + numberRemovedChildren);
                    firstChildIndexInGroup[nextGroup] -= numberRemovedChildren;
                }
            } else {
                success = true;
            }
        } else {
            success = false;
        }

        return success;
    }


    void ElementWithGroupedChildren::insertIntoGroupBefore(
            unsigned               groupIndex,
            unsigned long          childIndexInGroup,
            ElementPointer         childElement,
            CursorStateCollection* cursorStateCollection
        ) {
        assert(!firstChildIndexInGroup.isEmpty());

        if (groupIndex == invalidGroup || groupIndex >= static_cast<unsigned>(firstChildIndexInGroup.size())) {
            groupIndex = static_cast<unsigned>(firstChildIndexInGroup.size()) - 1;
        }

        unsigned long childIndex = childIndexFromGroup(groupIndex, childIndexInGroup);
        if (childIndex == invalidChildIndex) {
            childIndex        = baseChildIndex(groupIndex + 1);
            childIndexInGroup = childIndex - baseChildIndex(groupIndex);
        }

        ElementWithPositionalChildren::insertBefore(childIndex, childElement, cursorStateCollection);

        childrenAdded(groupIndex);

        if (visual() != nullptr) {
            visual()->childInsertedIntoGroupBefore(childIndex, groupIndex, childIndexInGroup, childElement);
        }
    }


    void ElementWithGroupedChildren::insertIntoGroupAfter(
            unsigned               groupIndex,
            unsigned long          childIndexInGroup,
            ElementPointer         childElement,
            CursorStateCollection* cursorStateCollection
        ) {
        assert(!firstChildIndexInGroup.isEmpty());

        if (groupIndex == invalidGroup || groupIndex >= static_cast<unsigned>(firstChildIndexInGroup.size())) {
            groupIndex = static_cast<unsigned>(firstChildIndexInGroup.size()) - 1;
        }

        unsigned long childIndex = childIndexFromGroup(groupIndex, childIndexInGroup);
        if (childIndex == invalidChildIndex) {
            childIndex                       = baseChildIndex(groupIndex + 1);
            unsigned long groupStartingIndex = baseChildIndex(groupIndex);

            ElementWithPositionalChildren::insertBefore(childIndex, childElement, cursorStateCollection);
            childrenAdded(groupIndex);

            if (visual() != nullptr) {
                visual()->childInsertedIntoGroupAfter(
                    childIndex == 0 ? invalidChildIndex : childIndex - 1,
                    groupIndex,
                    childIndex > groupStartingIndex ? (childIndex - groupStartingIndex - 1) : invalidChildIndex,
                    childElement
                );
            }
        } else {
            ElementWithPositionalChildren::insertAfter(childIndex, childElement, cursorStateCollection);
            childrenAdded(groupIndex);

            if (visual() != nullptr) {
                visual()->childInsertedIntoGroupAfter(childIndex, groupIndex, childIndexInGroup, childElement);
            }
        }
    }


    void ElementWithGroupedChildren::prependToGroup(
            unsigned               groupIndex,
            ElementPointer         childElement,
            CursorStateCollection* cursorStateCollection
        ) {
        insertIntoGroupBefore(groupIndex, 0, childElement, cursorStateCollection);
    }


    void ElementWithGroupedChildren::appendToGroup(
            unsigned               groupIndex,
            ElementPointer         childElement,
            CursorStateCollection* cursorStateCollection
        ) {
        insertIntoGroupAfter(groupIndex, invalidChildIndex, childElement, cursorStateCollection);
    }


    void ElementWithGroupedChildren::removeGroups(
            unsigned               startingGroupIndex,
            unsigned               numberGroups,
            CursorStateCollection* cursorStateCollection
        ) {
        unsigned groupsRemaining = numberGroups;
        bool     success         = true;
        while (success && groupsRemaining > 0) {
            success = removeChildrenFromGroup(startingGroupIndex, cursorStateCollection);
            if (success) {
                firstChildIndexInGroup.removeAt(startingGroupIndex);
                --groupsRemaining;
            }
        }

        if (visual() != nullptr) {
            visual()->groupsRemoved(startingGroupIndex, numberGroups);
        }
    }


    void ElementWithGroupedChildren::insertGroupsBefore(unsigned groupIndex, unsigned numberGroups) {
        unsigned currentNumberGroups = static_cast<unsigned>(firstChildIndexInGroup.size());
        if (groupIndex != invalidGroup && groupIndex < currentNumberGroups) {
            unsigned long childIndex = firstChildIndexInGroup[groupIndex];
            for (unsigned i=0 ; i<numberGroups ; ++i) {
                firstChildIndexInGroup.insert(groupIndex, childIndex);
            }

            if (visual() != nullptr) {
                visual()->groupsInserted(groupIndex, numberGroups);
            }
        } else {
            unsigned newGroupIndex = static_cast<unsigned>(firstChildIndexInGroup.size());

            unsigned long childIndex = numberChildren();
            for (unsigned i=0 ; i<numberGroups ; ++i) {
                firstChildIndexInGroup.append(childIndex);
            }

            if (visual() != nullptr) {
                visual()->groupsInserted(newGroupIndex, numberGroups);
            }
        }
    }


    void ElementWithGroupedChildren::insertGroupsAfter(unsigned groupIndex, unsigned numberGroups) {
        insertGroupsBefore(groupIndex + 1, numberGroups);
    }


    void ElementWithGroupedChildren::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        attributes.append("number_groups", static_cast<unsigned>(firstChildIndexInGroup.size()));
        ElementWithPositionalChildren::writeAddAttributes(attributes, formats, programFile);
    }


    void ElementWithGroupedChildren::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        ElementWithPositionalChildren::readAttributes(reader, attributes, formats, programFile, xmlVersion);

        if (!reader->hasError()) {
            if (attributes.hasAttribute("number_groups")) {
                bool     ok;
                unsigned newNumberGroups = attributes.value<unsigned>("number_groups", &ok);
                if (!ok) {
                    QString tagName = reader->qualifiedName().toString();
                    QString value   = attributes.value<QString>("number_groups");

                    reader->raiseError(tr("Tag \"%1\" has invalid number groups \"%1\"").arg(tagName).arg(value));
                } else {
                    firstChildIndexInGroup.clear();

                    firstChildIndexInGroup.append(0);
                    unsigned long numberChildren = ElementWithPositionalChildren::numberChildren();
                    for (unsigned groupIndex=1 ; groupIndex<newNumberGroups ; ++groupIndex) {
                        firstChildIndexInGroup.append(numberChildren);
                    }
                }
            } else {
                QString tagName = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\" is missing number_groups attribute").arg(tagName));
            }
        }
    }


    void ElementWithGroupedChildren::writeChildren(
            QSharedPointer<XmlWriter>       writer,
            QSharedPointer<FormatOrganizer> formatIdentifiers,
            ProgramFile&                    programFile,
            const XmlAttributes&            inheritedAttributes
        ) const {
        unsigned numberGroups = static_cast<unsigned>(firstChildIndexInGroup.size());
        for (unsigned groupIndex=0 ; groupIndex<numberGroups ; ++groupIndex) {
            ElementPointerList childrenInGroup = ElementWithGroupedChildren::childrenInGroup(groupIndex);

            unsigned numberChildrenThisGroup = static_cast<unsigned>(childrenInGroup.size());
            for (unsigned long childIndexInGroup=0 ; childIndexInGroup<numberChildrenThisGroup ; ++childIndexInGroup) {
                ElementPointer childElement = childrenInGroup.at(childIndexInGroup);

                XmlAttributes childAttributes = inheritedAttributes;
                childAttributes.append("group", groupIndex);
                childAttributes.append("index", childIndexInGroup);

                childElement->writeXml(writer, formatIdentifiers, programFile, childAttributes);
            }
        }
    }


    void ElementWithGroupedChildren::readChild(
            QSharedPointer<XmlReader>  reader,
            const QString&             tagName,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            const XmlAttributes&       attributes,
            unsigned                   xmlVersion
        ) {
        ElementPointer childElement;
        unsigned       numberGroups      = static_cast<unsigned>(firstChildIndexInGroup.size());
        unsigned       groupIndex        = invalidGroup;
        unsigned long  childIndexInGroup = invalidChildIndex;

        if (attributes.hasAttribute("group")) {
            bool ok;
            groupIndex = attributes.value<unsigned>("group", &ok);
            if (!ok) {
                QString value = attributes.value<QString>("group");
                reader->raiseError(tr("Tag \"%1\", invalid group index \"%2\"").arg(tagName).arg(value));
            };
        } else {
            reader->raiseError(tr("Tag \"%1\" missing group index").arg(tagName));
        }

        if (!reader->hasError() && (groupIndex == invalidGroup || groupIndex >= numberGroups)) {
            reader->raiseError(tr("Tag \"%1\", invalid group index \"%2\"").arg(tagName).arg(groupIndex));
        }

        if (!reader->hasError()) {
            if (attributes.hasAttribute("index")) {
                bool ok;
                childIndexInGroup = attributes.value<unsigned long>("index", &ok);
                if (!ok || childIndexInGroup != numberChildrenInGroup(groupIndex)) {
                    QString value = attributes.value<QString>("index");
                    reader->raiseError(
                        tr("Tag \"%1\", invalid child index (in group) \"%1\"").arg(tagName).arg(value)
                    );
                }
            } else {
                reader->raiseError(tr("Tag \"%1\", missing index (in group)").arg(tagName));
            }
        }

        if (!reader->hasError()) {
            childElement = create(tagName);
            if (!childElement) {
                reader->raiseError(tr("Tag \"%1\" invalid name").arg(tagName));
            }
        }

        if (!reader->hasError()) {
            childElement->readXml(reader, formats, programFile, attributes, xmlVersion);
        }

        if (!reader->hasError()) {
            appendToGroup(groupIndex, childElement, nullptr);
        }
    }


    bool ElementWithGroupedChildren::cloneChildren(QSharedPointer<ElementWithPositionalChildren> element) const {
        QSharedPointer<ElementWithGroupedChildren> sourceElement = element.dynamicCast<ElementWithGroupedChildren>();

        bool success = ElementWithPositionalChildren::cloneChildren(element);
        if (success) {
            sourceElement->firstChildIndexInGroup = firstChildIndexInGroup;
        }

        return success;
    }


    unsigned long ElementWithGroupedChildren::baseChildIndex(unsigned groupIndex) const {
        unsigned long result;

        if (groupIndex < static_cast<unsigned>(firstChildIndexInGroup.size())) {
            result = firstChildIndexInGroup.at(groupIndex);
        } else {
            result = numberChildren();
        }

        return result;
    }


    void ElementWithGroupedChildren::childrenAdded(unsigned groupIndex, unsigned long numberAdded) {
        unsigned numberGroups = static_cast<unsigned>(firstChildIndexInGroup.size());
        for (unsigned nextGroup=groupIndex+1 ; nextGroup<numberGroups ; ++ nextGroup) {
            firstChildIndexInGroup[nextGroup] += numberAdded;
        }

        assert(firstChildIndexInGroup.last() <= numberChildren());
    }
}
