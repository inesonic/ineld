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
* This file implements the \ref Ld::BraceConditionalOperatorElement class.
***********************************************************************************************************************/

#include <QCoreApplication>
#include <QString>
#include <QSharedPointer>

#include "ld_element_structures.h"
#include "ld_capabilities.h"
#include "ld_data_type.h"
#include "ld_cursor_state_collection.h"
#include "ld_brace_conditional_operator_visual.h"
#include "ld_operator_element_base.h"
#include "ld_brace_conditional_operator_element.h"

namespace Ld {
    const QString      BraceConditionalOperatorElement::elementName("BraceConditionalOperator");
    const Capabilities BraceConditionalOperatorElement::childProvides = (
          Capabilities::allOperators
        | Capabilities::textAnnotations
    );
    const unsigned     BraceConditionalOperatorElement::defaultNumberChildren = 3;

    BraceConditionalOperatorElement::BraceConditionalOperatorElement() {
        setNumberChildren(defaultNumberChildren, nullptr);
    }


    BraceConditionalOperatorElement::~BraceConditionalOperatorElement() {}


    Element* BraceConditionalOperatorElement::creator(const QString&) {
        return new BraceConditionalOperatorElement();
    }


    QString BraceConditionalOperatorElement::typeName() const {
        return elementName;
    }


    QString BraceConditionalOperatorElement::plugInName() const {
        return QString();
    }


    QString BraceConditionalOperatorElement::description() const {
        return tr("brace conditional value operator");
    }


    DataType::ValueType BraceConditionalOperatorElement::valueType() const {
        ElementPointer      child     = elseValueElement();
        DataType::ValueType valueType = child.isNull() ? DataType::ValueType::NONE : child->valueType();

        unsigned long numberExplicitConditions = BraceConditionalOperatorElement::numberExplicitConditions();
        for (unsigned conditionIndex=0 ; conditionIndex<numberExplicitConditions ; ++conditionIndex) {
            child = valueElement(conditionIndex);
            if (!child.isNull()) {
                valueType = DataType::bestUpcast(child->valueType(), valueType);
            }
        }

        return valueType;
    }


    Capabilities BraceConditionalOperatorElement::parentRequires(unsigned long index) const {
        Capabilities result;

        if (hasExplicitElseCondition() || index + 1 < numberChildren()) {
            result = (index % 2 == 0) ? Capabilities::allScalarRValues : Capabilities::allRValues;
        } else {
            result = Capabilities::allRValues;
        }

        return result;
    }


    Capabilities BraceConditionalOperatorElement::childProvidesCapabilities() const {
        return childProvides;
    }


    void BraceConditionalOperatorElement::setNumberConditions(
            unsigned long          numberExplicitConditions,
            bool                   includeElseCondition,
            ElementPointer         defaultChild,
            CursorStateCollection* cursorStateCollection
        ) {
        assert(numberExplicitConditions >= 1);

        ElementPointer currentElseCondition = elseConditionElement();
        ElementPointer currentElseValue     = elseValueElement();
        unsigned long  newNumberChildren    = 2 * numberExplicitConditions + (includeElseCondition ? 2 : 1);

        setNumberChildren(newNumberChildren, cursorStateCollection);

        if (includeElseCondition) {
            setChild(newNumberChildren - 2, currentElseValue, cursorStateCollection);
            setChild(newNumberChildren - 1, currentElseCondition, cursorStateCollection);
        } else {
            setChild(newNumberChildren - 1, currentElseValue, cursorStateCollection);
        }

        if (!defaultChild.isNull()) {
            for (unsigned long childIndex=0 ; childIndex<newNumberChildren ; ++childIndex) {
                if (child(childIndex).isNull()) {
                    setChild(childIndex, defaultChild->clone(true), cursorStateCollection);
                }
            }
        }
    }


    unsigned long BraceConditionalOperatorElement::numberExplicitConditions() const {
        return (numberChildren() - 1) / 2;
    }


    bool BraceConditionalOperatorElement::hasExplicitElseCondition() const {
        return numberChildren() % 2 == 0;
    }


    ElementPointer BraceConditionalOperatorElement::conditionElement(unsigned long conditionNumber) const {
        return child(2 * conditionNumber + 1);
    }


    ElementPointer BraceConditionalOperatorElement::valueElement(unsigned long conditionNumber) const {
        return child(2 * conditionNumber + 0);
    }


    ElementPointer BraceConditionalOperatorElement::elseConditionElement() const {
        ElementPointer result;
        unsigned long  numberChildren = BraceConditionalOperatorElement::numberChildren();

        if (numberChildren % 2 == 0) {
            result = child(numberChildren - 1);
        }

        return result;
    }


    ElementPointer BraceConditionalOperatorElement::elseValueElement() const {
        ElementPointer result;
        unsigned long  numberChildren = BraceConditionalOperatorElement::numberChildren();

        if (numberChildren % 2 == 0) {
            result = child(numberChildren - 2);
        } else {
            result = child(numberChildren - 1);
        }

        return result;
    }


    void BraceConditionalOperatorElement::setVisual(BraceConditionalOperatorVisual *newVisual) {
        OperatorElementBase::setVisual(newVisual);
    }


    BraceConditionalOperatorVisual* BraceConditionalOperatorElement::visual() const {
        return dynamic_cast<BraceConditionalOperatorVisual*>(OperatorElementBase::visual());
    }


    void BraceConditionalOperatorElement::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        OperatorElementBase::writeAddAttributes(attributes, formats, programFile);

        if (numberChildren() != defaultNumberChildren) {
            attributes.append("number_entries", numberChildren());
        }
    }


    void BraceConditionalOperatorElement::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        OperatorElementBase::readAttributes(reader, attributes, formats, programFile, xmlVersion);

        if (!reader->hasError()) {
            bool ok;
            unsigned numberEntries = attributes.value<unsigned>("number_entries", defaultNumberChildren, &ok);

            if (!ok || numberEntries < 3) {
                QString tagName     = reader->qualifiedName().toString();
                QString valueString = attributes.value<QString>("number_entries");
                reader->raiseError(tr("Tag \"%1\" has invalid number entries \"%1\"").arg(tagName).arg(valueString));
            } else {
                setNumberChildren(numberEntries, nullptr);
            }
        }
    }


    Element::Precedence BraceConditionalOperatorElement::intrinsicPrecedence() const {
        return defaultPrecedence;
    }
}
