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
* This file implements the \ref Ld::ElementWithPositionalChildren class.
***********************************************************************************************************************/

#include <QList>
#include <QSharedPointer>

#include "ld_element_structures.h"
#include "ld_format_organizer.h"
#include "ld_xml_writer.h"
#include "ld_xml_attributes.h"
#include "ld_program_file.h"
#include "ld_cursor_state_collection.h"
#include "ld_visual_with_positional_children.h"
#include "ld_element.h"
#include "ld_element_with_positional_children.h"

namespace Ld {
    ElementWithPositionalChildren::ElementWithPositionalChildren() {}


    ElementWithPositionalChildren::~ElementWithPositionalChildren() {}


    ElementPointer ElementWithPositionalChildren::clone(bool includeChildren) const {
        QSharedPointer<ElementWithPositionalChildren> element = cloneThisElementOnly()
                                                                .dynamicCast<ElementWithPositionalChildren>();

        if (!element.isNull() && includeChildren) {
            bool success = cloneChildren(element);
            if (!success) {
                element.clear();
            }
        }

        return element;
    }


    ElementPointer ElementWithPositionalChildren::clone(
            unsigned long        startingTextIndex,
            unsigned             startingRegionIndex,
            unsigned long        endingTextIndex,
            unsigned             endingRegionIndex,
            bool                 includeChildren
        ) const {
        QSharedPointer<ElementWithPositionalChildren> element = cloneThisElementOnly(
            startingTextIndex,
            startingRegionIndex,
            endingTextIndex,
            endingRegionIndex
        ).dynamicCast<ElementWithPositionalChildren>();

        if (!element.isNull() && includeChildren) {
            bool success = cloneChildren(element);
            if (!success) {
                element.clear();
            }
        }

        return element;
    }


    Element::ChildPlacement ElementWithPositionalChildren::childPlacement() const {
        return Element::ChildPlacement::POSITIONAL;
    }


    void ElementWithPositionalChildren::setVisual(VisualWithPositionalChildren* newVisual) {
        Element::setVisual(newVisual);
    }


    VisualWithPositionalChildren* ElementWithPositionalChildren::visual() const {
        return dynamic_cast<VisualWithPositionalChildren*>(Element::visual());
    }


    unsigned long ElementWithPositionalChildren::numberChildren() const {
        return currentChildren.size();
    }


    bool ElementWithPositionalChildren::removeChild(
            unsigned long          childIndex,
            CursorStateCollection* cursorStateCollection
        ) {
        bool     success        = false;
        unsigned numberChildren = static_cast<unsigned>(currentChildren.size());

        if (childIndex < numberChildren) {
            ElementPointer childElement = currentChildren.at(childIndex);

            if (visual() != nullptr) {
                visual()->aboutToRemoveChild(childIndex, childElement);
            }

            childElement->aboutToUngraftFromTree();

            if (cursorStateCollection != nullptr) {
                ElementPointerSet descendants = childElement->descendants();
                descendants.insert(childElement);

                for (  CursorStateCollection::iterator it  = cursorStateCollection->begin(),
                                                       end = cursorStateCollection->end()
                     ; it != end
                     ; ++it
                    ) {
                    if (it->isValid()                             &&
                        (descendants.contains(it->element()) ||
                         descendants.contains(it->parent())  ||
                         it->element() == childElement          )     ) {
                        ElementPointer nextSibling = childElement->nextSibling();
                        if (!nextSibling.isNull()) {
                            it->updateToFirstPositionIn(nextSibling, true);
                        } else {
                            ElementPointer previousSibling = childElement->previousSibling();
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

            childElement->currentParent.clear();
            currentChildren.removeAt(childIndex);

            success = true;
        }

        return success;
    }


    bool ElementWithPositionalChildren::removeChild(
            ElementPointer         childToRemove,
            CursorStateCollection* cursorStateCollection
        ) {
        unsigned long index = currentChildren.indexOf(childToRemove);
        return removeChild(static_cast<unsigned long>(index), cursorStateCollection);
    }


    void ElementWithPositionalChildren::removeChildren(CursorStateCollection* cursorStateCollection) {
        updateCursorsFromDescendantsToThis(cursorStateCollection);

        while (!currentChildren.isEmpty()) {
            ElementPointer childElement = currentChildren.first();

            childElement->removeChildren(nullptr);

            if (visual() != nullptr) {
                visual()->aboutToRemoveChild(0, childElement);
            }

            childElement->aboutToUngraftFromTree();

            currentChildren.removeFirst();
            childElement->currentParent.clear();
        }
    }


    void ElementWithPositionalChildren::insertBefore(
            unsigned long          childIndex,
            ElementPointer         childElement,
            CursorStateCollection* /* cursorStateCollection -- No adjustments needed for this operation */
        ) {
        assert(currentWeakThis); // This assert will fire if Element::setWeakThis was never called.

        ElementPointer oldParent = childElement->parent();
        if (oldParent) {
            oldParent->removeChild(childElement, nullptr);
        }

        currentChildren.insert(childIndex, childElement);
        childElement->currentParent = currentWeakThis;

        childElement->graftedToTree();

        if (visual() != nullptr) {
            visual()->childInsertedBefore(childIndex, childElement);
        }
    }


    void ElementWithPositionalChildren::insertAfter(
            unsigned long          childIndex,
            ElementPointer         childElement,
            CursorStateCollection* /* cursorStateCollection -- No adjustments needed for this operation */
        ) {
        assert(currentWeakThis); // This assert will fire if Element::setWeakThis was never called.

        ElementPointer oldParent = childElement->parent();
        if (oldParent) {
            oldParent->removeChild(childElement, nullptr);
        }

        currentChildren.insert(childIndex+1, childElement);
        childElement->currentParent = currentWeakThis;

        childElement->graftedToTree();

        if (visual() != nullptr) {
            visual()->childInsertedAfter(childIndex, childElement);
        }
    }


    void ElementWithPositionalChildren::prepend(
            ElementPointer         childElement,
            CursorStateCollection* cursorStateCollection
        ) {
        insertBefore(0, childElement, cursorStateCollection);
    }


    void ElementWithPositionalChildren::append(
            ElementPointer         childElement,
            CursorStateCollection* cursorStateCollection
        ) {
        unsigned long numberChildren = static_cast<unsigned long>(currentChildren.size());
        unsigned long childIndex     = numberChildren > 0 ? numberChildren - 1 : 0;

        insertAfter(childIndex, childElement, cursorStateCollection);
    }


    ElementPointer ElementWithPositionalChildren::child(unsigned long index) const {
        ElementPointer childElement;

        if (static_cast<unsigned long>(currentChildren.size()) > index) {
            childElement = currentChildren.at(index);
        }

        return childElement;
    }


    ElementPointerList ElementWithPositionalChildren::children() const {
        return currentChildren;
    }


    unsigned long ElementWithPositionalChildren::indexOfChild(ElementPointer childElement) const {
        unsigned long numberChildren = static_cast<unsigned long>(currentChildren.size());
        unsigned long index          = 0;

        while (index < numberChildren && currentChildren[index] != childElement) {
            ++index;
        }

        return index >= numberChildren ? invalidChildIndex : index;
    }


    void ElementWithPositionalChildren::graftedToTree() {
        ElementPointer p = parent();
        assert(p);

        p->descendantAdded(currentWeakThis.toStrongRef());

        for (auto it=currentChildren.begin(),end=currentChildren.end() ; it!=end ; ++it) {
            ElementPointer childElement = *it;
            childElement->graftedToTree();
        }

        Element::graftedToTree();
    }


    void ElementWithPositionalChildren::aboutToUngraftFromTree() {
        Element::aboutToUngraftFromTree();

        for (auto it=currentChildren.begin(),end=currentChildren.end() ; it!=end ; ++it) {
            ElementPointer childElement = *it;
            childElement->aboutToUngraftFromTree();
        }

        parent()->descendantRemoved(currentWeakThis.toStrongRef());
    }


    void ElementWithPositionalChildren::writeChildren(
            QSharedPointer<XmlWriter>       writer,
            QSharedPointer<FormatOrganizer> formatIdentifiers,
            ProgramFile&                    programFile,
            const XmlAttributes&            inheritedAttributes
        ) const {
        // Children must be written in order.  The index attribute is used to confirm correct ordering rather than to
        // control ordering.

        for (int index=0 ; index<currentChildren.size() ; ++index) {
            ElementPointer childElement = currentChildren[index];

            XmlAttributes childAttributes = inheritedAttributes;
            childAttributes.append("index", index);

            childElement->writeXml(writer, formatIdentifiers, programFile, childAttributes);
        }
    }


    void ElementWithPositionalChildren::readChild(
            QSharedPointer<XmlReader>  reader,
            const QString&             tagName,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            const XmlAttributes&       attributes,
            unsigned                   xmlVersion
        ) {
        ElementPointer childElement;
        int            childIndex = invalidChildIndex;

        if (attributes.hasAttribute("index")) {
            bool ok;
            childIndex = attributes.value<int>("index", &ok);
            if (!ok) {
                QString value = attributes.value<QString>("index");
                reader->raiseError(tr("Tag \"%1\", invalid positional child index \"%2\"").arg(tagName).arg(value));
            };
        } else {
            reader->raiseError(tr("Tag \"%1\" missing positional child index").arg(tagName));
        }

        if (!reader->hasError() && childIndex != currentChildren.size()) {
            reader->raiseError(tr("Tag \"%1\", invalid positional child index \"%1\"").arg(tagName).arg(childIndex));
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
            append(childElement, nullptr);
        }
    }


    bool ElementWithPositionalChildren::cloneChildren(QSharedPointer<ElementWithPositionalChildren> element) const {
        bool success = true;

        ElementPointerList::const_iterator it  = currentChildren.constBegin();
        ElementPointerList::const_iterator end = currentChildren.constEnd();
        while (success && it != end) {
            ElementPointer clonedChild = (*it)->clone(true);
            if (!clonedChild.isNull()) {
                element->append(clonedChild, nullptr);
            } else {
                success = false;
            }

            ++it;
        }

        return success;
    }
}
