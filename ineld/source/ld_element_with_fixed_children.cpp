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
* This file implements the \ref Ld::ElementWithFixedChildren class.
***********************************************************************************************************************/

#include <QList>
#include <QSharedPointer>

#include "ld_element_structures.h"
#include "ld_program_file.h"
#include "ld_format_organizer.h"
#include "ld_cursor_state_collection.h"
#include "ld_visual_with_fixed_children.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_attributes.h"
#include "ld_program_file.h"
#include "ld_element.h"
#include "ld_element_with_fixed_children.h"

namespace Ld {
    ElementWithFixedChildren::ElementWithFixedChildren() {}


    ElementWithFixedChildren::~ElementWithFixedChildren() {}


    ElementPointer ElementWithFixedChildren::clone(bool includeChildren) const {
        QSharedPointer<ElementWithFixedChildren> element = cloneThisElementOnly()
                                                           .dynamicCast<ElementWithFixedChildren>();

        element->setNumberChildren(numberChildren(), nullptr);

        if (!element.isNull() && includeChildren) {
            bool success = cloneChildren(element);
            if (!success) {
                element.clear();
            }
        }

        return element;
    }


    ElementPointer ElementWithFixedChildren::clone(
            unsigned long startingTextIndex,
            unsigned      startingRegionIndex,
            unsigned long endingTextIndex,
            unsigned      endingRegionIndex,
            bool          includeChildren
        ) const {
        QSharedPointer<ElementWithFixedChildren> element = cloneThisElementOnly(
            startingTextIndex,
            startingRegionIndex,
            endingTextIndex,
            endingRegionIndex
        ).dynamicCast<ElementWithFixedChildren>();

        element->setNumberChildren(numberChildren(), nullptr);

        if (!element.isNull() && includeChildren) {
            bool success = cloneChildren(element);
            if (!success) {
                element.clear();
            }
        }

        return element;
    }


    Element::ChildPlacement ElementWithFixedChildren::childPlacement() const {
        return Element::ChildPlacement::FIXED;
    }


    void ElementWithFixedChildren::setVisual(VisualWithFixedChildren* newVisual) {
        Element::setVisual(newVisual);
    }


    VisualWithFixedChildren* ElementWithFixedChildren::visual() const {
        return dynamic_cast<VisualWithFixedChildren*>(Element::visual());
    }


    unsigned long ElementWithFixedChildren::numberChildren() const {
        return currentChildren.size();
    }


    bool ElementWithFixedChildren::removeChild(
            unsigned long          childIndex,
            CursorStateCollection* cursorStateCollection
        ) {
        bool     success        = false;
        unsigned numberChildren = static_cast<unsigned>(currentChildren.size());

        if (childIndex < numberChildren) {
            ElementPointer childElement = currentChildren.at(childIndex);

            if (!childElement.isNull()) {
                if (visual() != nullptr) {
                    visual()->aboutToRemoveChild(childIndex, childElement);
                }

                childElement->aboutToUngraftFromTree();

                childElement->currentParent.clear();
                currentChildren[childIndex].reset();

                if (cursorStateCollection != nullptr) {
                    ElementPointerSet descendants = childElement->descendants();

                    for (  CursorStateCollection::iterator it = cursorStateCollection->begin(),
                                                           end = cursorStateCollection->end()
                         ; it != end
                         ; ++it
                        ) {
                        ElementPointer cursorElement = it->element();
                        ElementPointer cursorParent  = it->parent();
                        if (it->isValid()                                   &&
                            ((!cursorElement.isNull()             &&
                              descendants.contains(cursorElement)    ) ||
                             descendants.contains(cursorParent)        ||
                             cursorElement == childElement             ||
                             cursorParent == childElement                 )     ) {
                            it->update(weakThis().toStrongRef(), childIndex, true);
                        }
                    }
                }

                success = true;
            }
        }

        return success;
    }


    bool ElementWithFixedChildren::removeChild(
            ElementPointer         childToRemove,
            CursorStateCollection* cursorStateCollection
        ) {
        long index = currentChildren.indexOf(childToRemove);
        return removeChild(static_cast<unsigned long>(index), cursorStateCollection);
    }


    void ElementWithFixedChildren::removeChildren(CursorStateCollection* cursorStateCollection) {
        updateCursorsFromDescendantsToThis(cursorStateCollection);

        for (int childIndex=0 ; childIndex<currentChildren.size() ; ++childIndex) {
            ElementPointer childElement = currentChildren[childIndex];
            if (childElement != nullptr) {
                childElement->removeChildren(nullptr);

                if (visual() != nullptr) {
                    visual()->aboutToRemoveChild(childIndex, childElement);
                }

                childElement->aboutToUngraftFromTree();

                childElement->currentParent.clear();
                currentChildren[childIndex].reset();
            }
        }
    }


    bool ElementWithFixedChildren::setChild(
            unsigned long          index,
            ElementPointer         childElement,
            CursorStateCollection* cursorStateCollection
        ) {
        assert(currentWeakThis); // This assert will fire if Element::setWeakThis was never called.

        bool success;
        if (static_cast<unsigned long>(currentChildren.size()) > index) {
            ElementPointer oldChildElement = currentChildren.at(index);
            if (childElement != oldChildElement) {
                if (!childElement.isNull()) {
                    if (!oldChildElement.isNull()) {
                        if (visual() != nullptr) {
                            visual()->aboutToRemoveChild(index, oldChildElement);
                        }

                        oldChildElement->aboutToUngraftFromTree();
                        oldChildElement->currentParent.clear();
                    }

                    ElementPointer oldParent = childElement->currentParent.toStrongRef();
                    if (oldParent) {
                        oldParent->removeChild(childElement, nullptr);
                    }

                    currentChildren[index] = childElement;
                    childElement->currentParent = currentWeakThis;

                    childElement->graftedToTree();

                    if (cursorStateCollection != nullptr) {
                        for (  CursorStateCollection::iterator it = cursorStateCollection->begin(),
                                                               end = cursorStateCollection->end()
                             ; it != end
                             ; ++it
                            ) {
                            if (it->isValid()) {
                                ElementPointer cursorElement = it->element();
                                if (cursorElement == oldChildElement ||
                                    (cursorElement.isNull()     &&
                                     it->parent() == weakThis() &&
                                     it->childIndex() == index     )     ) {
                                    if (!childElement.isNull()) {
                                        it->updateToFirstPositionIn(childElement, true);
                                    } else {
                                        it->update(weakThis().toStrongRef(), index, true);
                                    }
                                }
                            }
                        }
                    }

                    if (visual() != nullptr) {
                        visual()->childSet(index, childElement);
                    }

                    success = true;
                } else if (!oldChildElement.isNull()) {
                    success = removeChild(index, cursorStateCollection);
                } else {
                    success = true;
                }
            } else {
                success = true;
            }
        } else {
            success = false;
        }

        return success;
    }


    ElementPointer ElementWithFixedChildren::moveChild(
            unsigned               oldChildIndex,
            unsigned               newChildIndex,
            CursorStateCollection* cursorStateCollection
        ) {
        unsigned long currentNumberChildren = numberChildren();
        assert(currentNumberChildren > oldChildIndex);

        ElementPointer displacedChild = child(newChildIndex);

        if (newChildIndex >= currentNumberChildren) {
            currentNumberChildren = newChildIndex + 1;
            setNumberChildren(currentNumberChildren, cursorStateCollection);
        }

        if (oldChildIndex != newChildIndex) {
            ElementPointer movedChild = child(oldChildIndex);

            if (cursorStateCollection != nullptr) {
                for (  CursorStateCollection::iterator it  = cursorStateCollection->begin(),
                                                       end = cursorStateCollection->end()
                     ; it != end
                     ; ++it
                    ) {
                    if (it->isValid()) {
                        ElementPointer cursorElement = it->element();

                        if (cursorElement.isNull() && it->parent() == weakThis()) {
                            unsigned long cursorChildIndex = it->childIndex();
                            if (cursorChildIndex == oldChildIndex || cursorChildIndex == newChildIndex) {
                                it->update(weakThis().toStrongRef(), newChildIndex, true);
                            }
                        }
                    }
                }
            }

            setChild(oldChildIndex, nullptr, nullptr);
            setChild(newChildIndex, movedChild, nullptr);
        }

        return displacedChild;
    }


    ElementPointer ElementWithFixedChildren::child(unsigned long index) const {
        ElementPointer childElement;

        if (static_cast<unsigned long>(currentChildren.size()) > index) {
            childElement = currentChildren.at(index);
        }

        return childElement;
    }


    ElementPointerList ElementWithFixedChildren::children() const {
        return currentChildren;
    }


    unsigned long ElementWithFixedChildren::indexOfChild(ElementPointer childElement) const {
        unsigned long numberChildren = static_cast<unsigned long>(currentChildren.size());
        unsigned long index          = 0;

        while (index < numberChildren && currentChildren[index] != childElement) {
            ++index;
        }

        return index >= numberChildren ? invalidChildIndex : index;
    }


    void ElementWithFixedChildren::setNumberChildren(
            unsigned               newNumberChildren,
            CursorStateCollection* cursorStateCollection
        ) {
        unsigned currentNumberChildren = static_cast<unsigned>(currentChildren.size());

        if (currentNumberChildren < newNumberChildren) {
            unsigned childrenToAdd = newNumberChildren - currentNumberChildren;
            for (unsigned i=0 ; i<childrenToAdd ; ++i) {
                currentChildren.append(nullptr);
            }
        } else if (currentNumberChildren > newNumberChildren) {
            if (cursorStateCollection != nullptr) {
                ElementPointerSet removedChildren;
                ElementPointer    bestFinalPosition;
                if (newNumberChildren == 0) {
                    removedChildren = ElementPointerSet(currentChildren.begin(), currentChildren.end());
                    currentChildren.clear();

                    bestFinalPosition = weakThis().toStrongRef();
                } else {
                    while (static_cast<unsigned>(currentChildren.size()) > newNumberChildren) {
                        removedChildren.insert(currentChildren.takeLast());
                    }

                    bestFinalPosition = currentChildren.last();
                }

                for (  CursorStateCollection::iterator it  = cursorStateCollection->begin(),
                                                       end = cursorStateCollection->end()
                     ; it != end
                     ; ++it
                    ) {
                    if (it->isValid()) {
                        ElementPointer cursorElement = it->element();
                        if (removedChildren.contains(cursorElement)) {
                            if (bestFinalPosition.isNull()) {
                                it->update(weakThis().toStrongRef(), newNumberChildren - 1, true);
                            } else {
                                it->updateToLastPositionIn(bestFinalPosition, true);
                            }
                        }
                    }
                }
            } else {
                currentChildren.erase(currentChildren.begin() + newNumberChildren, currentChildren.end());
            }
        }
    }


    void ElementWithFixedChildren::graftedToTree() {
        ElementPointer p = parent();
        assert(p);

        p->descendantAdded(currentWeakThis.toStrongRef());

        for (auto it=currentChildren.begin(),end=currentChildren.end() ; it!=end ; ++it) {
            ElementPointer childElement = *it;
            if (childElement) {
                childElement->graftedToTree();
            }
        }

        Element::graftedToTree();
    }


    void ElementWithFixedChildren::aboutToUngraftFromTree() {
        Element::aboutToUngraftFromTree();

        for (auto it=currentChildren.begin(),end=currentChildren.end() ; it!=end ; ++it) {
            ElementPointer childElement = *it;
            if (childElement) {
                childElement->aboutToUngraftFromTree();
            }
        }

        parent()->descendantRemoved(currentWeakThis.toStrongRef());
    }


    void ElementWithFixedChildren::writeChildren(
            QSharedPointer<XmlWriter>       writer,
            QSharedPointer<FormatOrganizer> formatIdentifiers,
            ProgramFile&                    programFile,
            const XmlAttributes&            inheritedAttributes
        ) const {
        for (int index=0 ; index<currentChildren.size() ; ++index) {
            ElementPointer childElement = currentChildren[index];
            if (childElement) {
                XmlAttributes childAttributes = inheritedAttributes;
                childAttributes.append("index", index);

                childElement->writeXml(writer, formatIdentifiers, programFile, childAttributes);
            }
        }
    }


    void ElementWithFixedChildren::readChild(
            QSharedPointer<XmlReader>  reader,
            const QString&             tagName,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            const XmlAttributes&       attributes,
            unsigned                   xmlVersion
        ) {
        unsigned long  childIndex = 0;
        ElementPointer childElement;

        if (attributes.hasAttribute("index")) {
            bool ok;
            childIndex = attributes.value<int>("index", &ok);
            if (!ok) {
                QString value = attributes.value<QString>("index");
                reader->raiseError(tr("Tag \"%1\" invalid child fixed index \"%2\"").arg(tagName).arg(value));
            }
        } else {
            reader->raiseError(tr("Tag \"%1\" missing fixed child index").arg(tagName));
        }

        if (!reader->hasError() && childIndex >= static_cast<unsigned long>(currentChildren.size())) {
            reader->raiseError(tr("Tag \"%1\", invalid fixed child index \"%1\"").arg(tagName).arg(childIndex));
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
            setChild(childIndex, childElement, nullptr);
        }
    }


    bool ElementWithFixedChildren::cloneChildren(QSharedPointer<ElementWithFixedChildren> element) const {
        bool success = true;

        unsigned countChildren = currentChildren.size();
        unsigned childIndex    = 0;

        element->setNumberChildren(countChildren, nullptr);

        while (success && childIndex < countChildren) {
            ElementPointer child = currentChildren.at(childIndex);

            if (child.isNull()) {
                element->removeChild(childIndex, nullptr);
            } else {
                ElementPointer clonedChild = child->clone(true);
                if (!clonedChild.isNull()) {
                    element->setChild(childIndex, clonedChild, nullptr);
                } else {
                    success = false;
                }
            }

            ++childIndex;
        }

        return success;
    }
}
