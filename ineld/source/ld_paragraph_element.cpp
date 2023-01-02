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
* This file implements the \ref Ld::ParagraphElement class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>

#include "ld_paragraph_format.h"
#include "ld_list_additional_paragraph_format.h"
#include "ld_ordered_list_paragraph_format.h"
#include "ld_unordered_list_paragraph_format.h"
#include "ld_element.h"
#include "ld_text_element.h"
#include "ld_frame_element.h"
#include "ld_paragraph_element.h"

namespace Ld {
    const QString      ParagraphElement::elementName("Paragraph");
    const Capabilities ParagraphElement::childProvides = Capabilities::frame;

    ParagraphElement::ParagraphElement() {}


    ParagraphElement::~ParagraphElement() {}


    Element* ParagraphElement::creator(const QString&) {
        return new ParagraphElement();
    }


    QString ParagraphElement::typeName() const {
        return elementName;
    }


    QString ParagraphElement::plugInName() const {
        return QString();
    }


    QString ParagraphElement::description() const {
        return tr("paragraph");
    }


    Capabilities ParagraphElement::parentRequires(unsigned long) const {
        return Capabilities::allAnnotations | Capabilities::expressions;
    }


    Capabilities ParagraphElement::childProvidesCapabilities() const {
        return childProvides;
    }


    void ParagraphElement::setFormat(QSharedPointer<ParagraphFormat> newFormat) {
        FrameElement::setFormat(newFormat);
    }


    void ParagraphElement::setFormat(QSharedPointer<ListAdditionalParagraphFormat> newFormat) {
        FrameElement::setFormat(newFormat);
    }


    void ParagraphElement::setFormat(QSharedPointer<OrderedListParagraphFormat> newFormat) {
        FrameElement::setFormat(newFormat);
    }


    void ParagraphElement::setFormat(QSharedPointer<UnorderedListParagraphFormat> newFormat) {
        FrameElement::setFormat(newFormat);
    }


    void ParagraphElement::setFormat(ParagraphFormat* newFormat) {
        FrameElement::setFormat(newFormat);
    }


    QSharedPointer<ParagraphFormat> ParagraphElement::format() const {
        return FrameElement::format().dynamicCast<ParagraphFormat>();
    }


    Element::CursorParkMode ParagraphElement::cursorParkMode() const {
        return CursorParkMode::CAN_NOT_PARK;
    }


    ParagraphElement::ExportImageCapability ParagraphElement::exportImageCapability() const {
        return ExportImageCapability::NONE;
    }


    bool ParagraphElement::isFunctionLogicalStop(unsigned long /* functionChildIndex */) const {
        return true;
    }


    bool ParagraphElement::containsNormalText() const {
        bool          hasNormalText  = false;
        unsigned long numberChildren = ParagraphElement::numberChildren();
        unsigned      childIndex     = 0;

        while (!hasNormalText && childIndex<numberChildren) {
            ElementPointer element = child(childIndex);
            hasNormalText = (
                   element->typeName() == Ld::TextElement::elementName
                && element->numberTextRegions() == 1
                && !element->text().isEmpty()
            );

            ++childIndex;
        }

        return hasNormalText;
    }
}
