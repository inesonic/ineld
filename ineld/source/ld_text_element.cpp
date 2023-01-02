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
* This file implements the \ref Ld::TextElement class.
***********************************************************************************************************************/

#include <QList>
#include <QSharedPointer>

#include <cassert>

#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_data_type.h"
#include "ld_program_file.h"
#include "ld_format_organizer.h"
#include "ld_visual_with_no_children.h"
#include "ld_format.h"
#include "ld_format_container.h"
#include "ld_character_format.h"
#include "ld_capabilities.h"
#include "ld_xml_writer.h"
#include "ld_xml_attributes.h"
#include "ld_program_file.h"
#include "ld_element_cursor.h"
#include "ld_cursor.h"
#include "ld_cursor_state_collection.h"
#include "ld_element_with_no_children.h"
#include "ld_root_element.h"
#include "ld_text_visual.h"
#include "ld_text_element.h"

namespace Ld {
    const QString      TextElement::elementName("Text");
    const Capabilities TextElement::childProvides = Capabilities::textAnnotations;

    TextElement::TextElement() {}


    TextElement::~TextElement() {}


    Element* TextElement::creator(const QString&) {
        return new TextElement();
    }


    QString TextElement::typeName() const {
        return elementName;
    }


    QString TextElement::plugInName() const {
        return QString();
    }


    QString TextElement::description() const {
        return tr("\"%1\"").arg(currentText);
    }


    DataType::ValueType TextElement::valueType() const {
        return DataType::ValueType::NONE;
    }


    Capabilities TextElement::childProvidesCapabilities() const {
        return childProvides;
    }


    Capabilities TextElement::textCapabilities(unsigned long, unsigned) const {
        return (
              Capabilities::allSymbols
            | Capabilities::nonTextAnnotations
            | Capabilities::newLine
            | Capabilities::expressions
        );
    }


    void TextElement::setVisual(TextVisual *newVisual) {
        ElementWithNoChildren::setVisual(newVisual);
    }


    TextVisual* TextElement::visual() const {
        return dynamic_cast<TextVisual*>(ElementWithNoChildren::visual());
    }


    bool TextElement::aggregateTextDuringSearchAllowed(ElementPointer otherElement, unsigned, unsigned) const {
        return (otherElement->typeName() == elementName && parent() == otherElement->parent());
    }


    unsigned TextElement::minimumNumberTextRegions() const {
        return 1;
    };


    unsigned TextElement::maximumNumberTextRegions() const {
        return 1;
    }


    bool TextElement::setNumberTextRegions(unsigned newNumberTextRegions) {
        return newNumberTextRegions == 1 ? true : false;
    }


    unsigned TextElement::numberTextRegions() const {
        return 1;
    }


    bool TextElement::setText(const QString& newText, unsigned regionNumber) {
        bool success;
        if (regionNumber == 0) {
            success = true;

            if (newText != currentText) {
                currentText = newText;

                TextVisual* textVisual = visual();
                if (textVisual != nullptr) {
                    elementDataChanged();
                    textVisual->textChanged(currentText);
                }
            }
        } else {
            success = false;
        }

        return success;
    }


    QString TextElement::text(unsigned regionNumber) const {
        return regionNumber == 0 ? currentText : QString();
    }


    Element::CursorParkMode TextElement::cursorParkMode() const {
        return CursorParkMode::PARK_ON_TEXT_ONLY;
    }


    TextElement::ExportImageCapability TextElement::exportImageCapability() const {
        return ExportImageCapability::NONE;
    }


    void TextElement::updateAfterGraft() {
        if (format().isNull()) {
            ElementPointer rootElement = root();
            assert(!rootElement.isNull());

            if (rootElement->typeName() == Ld::RootElement::elementName) {
                QSharedPointer<Ld::RootElement> root = rootElement.dynamicCast<Ld::RootElement>();
                assert(!root.isNull());

                setFormat(root->defaultTextFormat());
            }
        }
    }


    QString TextElement::writeData(QSharedPointer<const FormatOrganizer>, ProgramFile&) const {
        return currentText;
    }


    void TextElement::readData(
            QSharedPointer<XmlReader>,
            const QString&             text,
            const FormatsByIdentifier&,
            ProgramFile&,
            unsigned
        ) {
        currentText = text;
    }
}
