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
* This file implements the \ref Ld::VariableElement class.
***********************************************************************************************************************/

#include <QList>
#include <QSharedPointer>

#include <cassert>

#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_data_type.h"
#include "ld_root_element.h"
#include "ld_identifier.h"
#include "ld_identifier_container.h"
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
#include "ld_variable_visual.h"
#include "ld_variable_element.h"

namespace Ld {
    const QString      VariableElement::elementName("Variable");
    const Capabilities VariableElement::childProvides = Capabilities::variables;

    VariableElement::VariableElement() {}


    VariableElement::~VariableElement() {}


    Element* VariableElement::creator(const QString&) {
        return new VariableElement();
    }


    QString VariableElement::typeName() const {
        return elementName;
    }


    QString VariableElement::plugInName() const {
        return QString();
    }


    QString VariableElement::description() const {
        return tr("variable");
    }


    DataType::ValueType VariableElement::valueType() const {
        DataType::ValueType result;

        IdentifierContainer identifier = VariableElement::identifier();
        if (identifier.isValid()) {
            DataType dataType = identifier.dataType();
            if (dataType.isInvalid()) {
                dataType = DataType::defaultDataType();
            }

            result = dataType.valueType();
        } else {
            result = DataType::ValueType::NONE;
        }

        return result;
    }


    Capabilities VariableElement::childProvidesCapabilities() const {
        return childProvides;
    }


    Capabilities VariableElement::textCapabilities(unsigned long textOffset, unsigned regionIndex) const {
        return   regionIndex == 0 && textOffset == 0
               ? Capabilities::allAlphabeticSymbols
               : Capabilities::allAlphanumericSymbols;
    }


    void VariableElement::setVisual(VariableVisual *newVisual) {
        ElementWithNoChildren::setVisual(newVisual);
    }


    VariableVisual* VariableElement::visual() const {
        return dynamic_cast<VariableVisual*>(ElementWithNoChildren::visual());
    }


    unsigned VariableElement::minimumNumberTextRegions() const {
        return 2;
    };


    unsigned VariableElement::maximumNumberTextRegions() const {
        return 2;
    }


    bool VariableElement::setNumberTextRegions(unsigned newNumberTextRegions) {
        return newNumberTextRegions == 2 ? true : false;
    }


    unsigned VariableElement::numberTextRegions() const {
        return 2;
    }


    bool VariableElement::setText(const QString& newText, unsigned regionNumber) {
        bool success;
        if (regionNumber <= 1) {
            success = true;

            if (newText != currentText[regionNumber]) {
                currentText[regionNumber] = newText;

                VariableVisual* variableVisual = visual();
                if (variableVisual != nullptr) {
                    elementDataChanged();
                    variableVisual->textChanged(newText, regionNumber);
                }
            }
        } else {
            success = false;
        }

        return success;
    }


    QString VariableElement::text(unsigned regionNumber) const {
        return regionNumber <= 1 ? currentText[regionNumber] : QString();
    }


    Element::CursorParkMode VariableElement::cursorParkMode() const {
        return CursorParkMode::PARK_ON_TEXT_AND_ELEMENT;
    }


    unsigned VariableElement::numberCalculatedValues() const {
        return 1;
    }


    void VariableElement::setCalculatedValue(unsigned int valueIndex, const CalculatedValue &calculatedValue) {
        if (valueIndex == 0) {
            currentCalculatedValue = calculatedValue;
        }

        ElementWithNoChildren::setCalculatedValue(valueIndex, calculatedValue);
    }


    void VariableElement::clearCalculatedValue() {
        currentCalculatedValue = CalculatedValue();
        ElementWithNoChildren::clearCalculatedValue();
    }


    CalculatedValue VariableElement::calculatedValue(unsigned int valueIndex) const {
        return valueIndex == 0 ? currentCalculatedValue : CalculatedValue();
    }


    void VariableElement::setIdentifier(const IdentifierContainer& identifier) {
        currentIdentifier = identifier.pointer().toWeakRef();
    }


    const IdentifierContainer& VariableElement::identifier() const {
        return currentIdentifier;
    }


    void VariableElement::updateAfterGraft() {
        if (format().isNull()) {
            ElementPointer rootElement = root();
            assert(!rootElement.isNull());

            if (rootElement->typeName() == Ld::RootElement::elementName) {
                QSharedPointer<Ld::RootElement> root = rootElement.dynamicCast<Ld::RootElement>();
                assert(!root.isNull());

                setFormat(root->defaultMathIdentifierFormat());
            }
        }
    }


    void VariableElement::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        ElementWithNoChildren::writeAddAttributes(attributes, formats, programFile);

        attributes.append("t1", currentText[0]);

        if (!currentText[1].isEmpty()) {
            attributes.append("t2", currentText[1]);
        }
    }


    void VariableElement::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        QString t1;
        QString t2;

        ElementWithNoChildren::readAttributes(reader, attributes, formats, programFile, xmlVersion);

        if (!reader->hasError()) {
            if (attributes.hasAttribute("t1")) {
                t1 = attributes.value<QString>("t1");
                if (t1.isEmpty()) {
                    QString tagName = reader->qualifiedName().toString();
                    reader->raiseError(tr("Tag \"%1\" has invalid attribute \"%1\"").arg(tagName).arg(t1));
                }
            } else {
                QString tagName = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\" is missing attribute t1").arg(tagName));
            }
        }

        if (!reader->hasError() && attributes.hasAttribute("t2")) {
            t2 = attributes.value<QString>("t2");
        }

        if (!reader->hasError()) {
            currentText[0] = t1;
            currentText[1] = t2;
        }
    }
}
