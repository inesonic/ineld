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
* This file implements the \ref Ld::ElementWithNoChildren class.
***********************************************************************************************************************/

#include <QList>
#include <QSharedPointer>

#include "ld_element_structures.h"
#include "ld_format.h"
#include "ld_program_file.h"
#include "ld_format_organizer.h"
#include "ld_cursor_state_collection.h"
#include "ld_visual_with_no_children.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_attributes.h"
#include "ld_program_file.h"
#include "ld_element.h"
#include "ld_element_with_no_children.h"

namespace Ld {
    ElementWithNoChildren::ElementWithNoChildren() {}


    ElementWithNoChildren::~ElementWithNoChildren() {}


    ElementPointer ElementWithNoChildren::clone(bool) const {
        return cloneThisElementOnly();
    }


    ElementPointer ElementWithNoChildren::clone(
            unsigned long startingTextIndex,
            unsigned      startingRegionIndex,
            unsigned long endingTextIndex,
            unsigned      endingRegionIndex,
            bool
        ) const {
        return cloneThisElementOnly(startingTextIndex, startingRegionIndex, endingTextIndex, endingRegionIndex);
    }


    Element::ChildPlacement ElementWithNoChildren::childPlacement() const {
        return Element::ChildPlacement::NONE;
    }


    void ElementWithNoChildren::setVisual(VisualWithNoChildren* newVisual) {
        Element::setVisual(newVisual);
    }


    VisualWithNoChildren* ElementWithNoChildren::visual() const {
        return dynamic_cast<VisualWithNoChildren*>(Element::visual());
    }


    Capabilities ElementWithNoChildren::parentRequires(unsigned long) const {
        return Capabilities();
    }


    unsigned long ElementWithNoChildren::numberChildren() const {
        return 0;
    }


    bool ElementWithNoChildren::removeChild(ElementPointer, CursorStateCollection*) {
        return false;
    }


    void ElementWithNoChildren::removeChildren(CursorStateCollection*) {}


    ElementPointer ElementWithNoChildren::child(unsigned long) const {
        return ElementPointer();
    }


    ElementPointerList ElementWithNoChildren::children() const {
        return ElementPointerList();
    }


    unsigned long ElementWithNoChildren::indexOfChild(ElementPointer) const {
        return invalidChildIndex;
    }


    void ElementWithNoChildren::graftedToTree() {
        ElementPointer p = parent();
        assert(p);

        p->descendantAdded(currentWeakThis.toStrongRef());

        Element::graftedToTree();
    }


    void ElementWithNoChildren::aboutToUngraftFromTree() {
        Element::aboutToUngraftFromTree();
        parent()->descendantRemoved(currentWeakThis.toStrongRef());
    }


    void ElementWithNoChildren::writeChildren(
            QSharedPointer<XmlWriter>,
            QSharedPointer<FormatOrganizer>,
            ProgramFile&,
            const XmlAttributes&
        ) const {}


    void ElementWithNoChildren::readChild(
            QSharedPointer<XmlReader>  reader,
            const QString&             tagName,
            const FormatsByIdentifier&,
            ProgramFile&,
            const XmlAttributes&,
            unsigned
        ) {
        reader->raiseError(tr("Tag \"%1\" can not be under \"%2\"").arg(tagName).arg(typeName()));
    }
}
