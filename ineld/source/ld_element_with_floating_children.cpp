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
* This file implements the \ref Ld::ElementWithFloatingChildren class.
***********************************************************************************************************************/

#include <QList>
#include <QSharedPointer>

#include "ld_element_structures.h"
#include "ld_format_organizer.h"
#include "ld_cursor_state_collection.h"
#include "ld_visual_with_floating_children.h"
#include "ld_location.h"
#include "ld_xml_writer.h"
#include "ld_xml_attributes.h"
#include "ld_element.h"
#include "ld_element_with_floating_children.h"

namespace Ld {
    ElementWithFloatingChildren::ElementWithFloatingChildren() {}


    ElementWithFloatingChildren::~ElementWithFloatingChildren() {}


    ElementPointer ElementWithFloatingChildren::clone(bool includeChildren) const {
        QSharedPointer<ElementWithFloatingChildren> element = cloneThisElementOnly()
                                                                .dynamicCast<ElementWithFloatingChildren>();

        if (!element.isNull() && includeChildren) {
            bool success = cloneChildren(element);
            if (!success) {
                element.clear();
            }
        }

        return element;
    }


    ElementPointer ElementWithFloatingChildren::clone(
            unsigned long        startingTextIndex,
            unsigned             startingRegionIndex,
            unsigned long        endingTextIndex,
            unsigned             endingRegionIndex,
            bool                 includeChildren
        ) const {
        QSharedPointer<ElementWithFloatingChildren> element = cloneThisElementOnly(
            startingTextIndex,
            startingRegionIndex,
            endingTextIndex,
            endingRegionIndex
        ).dynamicCast<ElementWithFloatingChildren>();

        if (!element.isNull() && includeChildren) {
            bool success = cloneChildren(element);
            if (!success) {
                element.clear();
            }
        }

        return element;
    }


    Element::ChildPlacement ElementWithFloatingChildren::childPlacement() const {
        return Element::ChildPlacement::FLOATING;
    }


    void ElementWithFloatingChildren::setVisual(VisualWithFloatingChildren* newVisual) {
        Element::setVisual(newVisual);
    }


    VisualWithFloatingChildren* ElementWithFloatingChildren::visual() const {
        return dynamic_cast<VisualWithFloatingChildren*>(Element::visual());
    }


    unsigned long ElementWithFloatingChildren::numberChildren() const {
        return currentChildrenByLocation.size();
    }


    bool ElementWithFloatingChildren::removeChild(
            ElementPointer         childToRemove,
            CursorStateCollection* cursorStateCollection
        ) {
        bool success;

        if (currentLocationsByChild.contains(childToRemove)) {
            Location childLocation = currentLocationsByChild.value(childToRemove);

            if (visual() != nullptr) {
                visual()->aboutToRemoveChild(childLocation, childToRemove);
            }

            childToRemove->aboutToUngraftFromTree();

            if (cursorStateCollection != nullptr) {
                ElementPointerSet descendants = childToRemove->descendants();
                for (  CursorStateCollection::iterator it = cursorStateCollection->begin(),
                                                       end = cursorStateCollection->end()
                     ; it != end
                     ; ++it
                    ) {
                    if (it->isValid()                             &&
                        (descendants.contains(it->element()) ||
                         descendants.contains(it->parent())  ||
                         it->element() == childToRemove         )     ) {
                        ElementPointer nextSibling = childToRemove->nextSibling();
                        if (!nextSibling.isNull()) {
                            it->updateToFirstPositionIn(nextSibling, true);
                        } else {
                            ElementPointer previousSibling = childToRemove->previousSibling();
                            if (!previousSibling.isNull()) {
                                it->updateToLastPositionIn(previousSibling, true);
                            } else {
                                ElementPointer strongThis = weakThis().toStrongRef();
                                if (!strongThis->parent().isNull()) {
                                    it->updateToLastPositionIn(strongThis, true);
                                } else {
                                    it->invalidate();
                                }
                            }
                        }
                    }
                }
            }

            int numberRemoved = currentLocationsByChild.remove(childToRemove);
            Q_ASSERT(numberRemoved == 1);

            numberRemoved = currentChildrenByLocation.remove(childLocation);
            Q_ASSERT(numberRemoved == 1);

            childToRemove->currentParent.clear();

            success = true;
        } else {
            success = false;
        }

        return success;
    }


    void ElementWithFloatingChildren::removeChildren(CursorStateCollection* cursorStateCollection) {
        updateCursorsFromDescendantsToThis(cursorStateCollection);

        ElementPointerList childElements = currentLocationsByChild.keys();

        for (  ElementPointerList::const_iterator it=childElements.begin(),end=childElements.end()
             ; it!=end
             ; ++it
            ) {
            ElementPointer childElement  = *it;
            Location childLocation = currentLocationsByChild.value(childElement);

            childElement->removeChildren(nullptr);

            if (visual() != nullptr) {
                visual()->aboutToRemoveChild(childLocation, childElement);
            }

            childElement->aboutToUngraftFromTree();

            int numberRemoved = currentLocationsByChild.remove(childElement);
            Q_ASSERT(numberRemoved == 1);

            numberRemoved = currentChildrenByLocation.remove(childLocation);
            Q_ASSERT(numberRemoved == 1);

            childElement->currentParent.clear();
        }
    }


    void ElementWithFloatingChildren::insertChild(
            ElementPointer         childElement,
            const Location&        location,
            CursorStateCollection* /* cursorStateCollection -- No adjustments needed for this operation */
        ) {
        assert(currentWeakThis); // This assert will fire if Element::setWeakThis was never called.

        ElementPointer oldParent = childElement->parent();
        if (oldParent) {
            oldParent->removeChild(childElement, nullptr);
        }

        childElement->currentParent = currentWeakThis;

        currentChildrenByLocation.insert(location, childElement);
        currentLocationsByChild.insert(childElement, location);

        childElement->graftedToTree();

        if (visual() != nullptr) {
            visual()->childInserted(childElement, location);
        }
    }


    void ElementWithFloatingChildren::insertChild(
            ElementPointer         childElement,
            double                 x,
            double                 y,
            CursorStateCollection* cursorStateCollection
        ) {
        insertChild(childElement, Location(x, y), cursorStateCollection);
    }


    bool ElementWithFloatingChildren::relocateChild(
            ElementPointer         childElement,
            const Location&        location,
            CursorStateCollection* cursorStateCollection
        ) {
        bool success;

        if (currentLocationsByChild.contains(childElement)) {
            Location oldLocation = currentLocationsByChild.value(childElement);

            int numberRemoved = currentLocationsByChild.remove(childElement);
            Q_ASSERT(numberRemoved == 1);

            numberRemoved = currentChildrenByLocation.remove(oldLocation);
            Q_ASSERT(numberRemoved == 1);

            currentLocationsByChild.insert(childElement, location);
            currentChildrenByLocation.insert(location, childElement);

            if (visual() != nullptr) {
                visual()->childRelocated(childElement, oldLocation, location);
            }

            if (cursorStateCollection != nullptr) {
                // TODO: What do we do with the cursors under this scenario ?
            }

            success = true;
        } else {
            success = false;
        }

        return success;
    }


    bool ElementWithFloatingChildren::relocateChild(
            ElementPointer         childElement,
            double                 x,
            double                 y,
            CursorStateCollection* cursorStateCollection
        ) {
        return relocateChild(childElement, Location(x, y), cursorStateCollection);
    }


    ElementPointer ElementWithFloatingChildren::child(unsigned long index) const {
        ElementPointer childElement;

        if (index < static_cast<unsigned long>(currentChildrenByLocation.size())) {
            QMap<Location, ElementPointer>::const_iterator it = currentChildrenByLocation.begin() + index;
            childElement = it.value();
        }

        return childElement;
    }


    Location ElementWithFloatingChildren::childLocation(ElementPointer childElement) const {
        return currentLocationsByChild.value(childElement);
    }


    Location ElementWithFloatingChildren::childLocation(unsigned long childIndex) const {
        const ElementPointer childElement = child(childIndex);
        return childLocation(childElement);
    }


    ElementPointerList ElementWithFloatingChildren::children() const {
        return currentChildrenByLocation.values();
    }


    QMap<Location, ElementPointer> ElementWithFloatingChildren::childrenByLocation() const {
        return currentChildrenByLocation;
    }


    unsigned long ElementWithFloatingChildren::indexOfChild(ElementPointer childElement) const {
        ElementPointerList childList = currentChildrenByLocation.values();

        unsigned long numberChildren = static_cast<unsigned long>(childList.size());
        unsigned long index          = 0;

        while (index < numberChildren && childList[index] != childElement) {
            ++index;
        }

        return index >= numberChildren ? invalidChildIndex : index;
    }


    void ElementWithFloatingChildren::graftedToTree() {
        parent()->descendantAdded(currentWeakThis.toStrongRef());

        for (  QMap<Location, ElementPointer>::const_iterator it  = currentChildrenByLocation.constBegin(),
                                                              end = currentChildrenByLocation.constEnd()
             ; it != end
             ; ++it
            ) {
            ElementPointer childElement = it.value();
            childElement->graftedToTree();
        }

        Element::graftedToTree();
    }


    void ElementWithFloatingChildren::aboutToUngraftFromTree() {
        Element::aboutToUngraftFromTree();

        for (  QMap<Location, ElementPointer>::const_iterator it  = currentChildrenByLocation.constBegin(),
                                                              end = currentChildrenByLocation.constEnd()
             ; it != end
             ; ++it
            ) {
            ElementPointer childElement = it.value();
            childElement->aboutToUngraftFromTree();
        }

        parent()->descendantRemoved(currentWeakThis.toStrongRef());
    }


    void ElementWithFloatingChildren::writeChildren(
            QSharedPointer<XmlWriter>       writer,
            QSharedPointer<FormatOrganizer> formatIdentifiers,
            ProgramFile&                    programFile,
            const XmlAttributes&            inheritedAttributes
        ) const {
        for (  QMap<Location, ElementPointer>::const_iterator pos = currentChildrenByLocation.constBegin(),
                                                              end = currentChildrenByLocation.end()
             ; pos != end
             ; ++pos
            ) {
            ElementPointer childElement  = pos.value();
            Location                childLocation = pos.key();

            XmlAttributes childAttributes = inheritedAttributes;
            childAttributes.append("x", childLocation.x());
            childAttributes.append("y", childLocation.y());

            childElement->writeXml(writer, formatIdentifiers, programFile, childAttributes);
        }
    }


    void ElementWithFloatingChildren::readChild(
            QSharedPointer<XmlReader>  reader,
            const QString&             tagName,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            const XmlAttributes&       attributes,
            unsigned                   xmlVersion
        ) {
        ElementPointer childElement = nullptr;
        double         childX       = 0;
        double         childY       = 0;

        if (attributes.hasAttribute("x")) {
            bool ok;
            childX = attributes.value<double>("x", &ok);
            if (!ok) {
                QString value = attributes.value<QString>("x");
                reader->raiseError(tr("Tag \"%1\", invalid floating child x \"%2\"").arg(tagName).arg(value));
            }
        } else {
            reader->raiseError(tr("Tag \"%1\" missing floating child x").arg(tagName));
        }

        if (attributes.hasAttribute("y")) {
            bool ok;
            childY = attributes.value<double>("y", &ok);
            if (!ok) {
                QString value = attributes.value<QString>("y");
                reader->raiseError(tr("Tag \"%1\", invalid floating child y \"%2\"").arg(tagName).arg(value));
            }
        } else {
            reader->raiseError(tr("Tag \"%1\" missing floating child y").arg(tagName));
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
            insertChild(childElement, childX, childY, nullptr); // FIXME:
        }
    }


    bool ElementWithFloatingChildren::cloneChildren(QSharedPointer<ElementWithFloatingChildren> element) const {
        bool success = true;

        QMap<Location, ElementPointer>::const_iterator pos = currentChildrenByLocation.constBegin();
        QMap<Location, ElementPointer>::const_iterator end = currentChildrenByLocation.constEnd();

        while (success && pos != end) {
            Location       childLocation = pos.key();
            ElementPointer childElement  = pos.value();
            ElementPointer clonedChild   = childElement->clone(true);

            if (!clonedChild.isNull()) {
                element->insertChild(clonedChild, childLocation, nullptr); // FIXME:
            } else {
                success = false;
            }

            ++pos;
        }

        return success;
    }
}
