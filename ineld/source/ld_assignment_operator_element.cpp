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
* This file implements the \ref Ld::AssignmentOperatorElement class.
***********************************************************************************************************************/

#include <QCoreApplication>
#include <QString>
#include <QSharedPointer>

#include "ld_capabilities.h"
#include "ld_assignment_operator_visual.h"
#include "ld_binary_operator_element_base.h"
#include "ld_assignment_operator_element.h"

namespace Ld {
    const QString      AssignmentOperatorElement::elementName("AssignmentOperator");
    const Capabilities AssignmentOperatorElement::childProvides = (
          Capabilities::assignment
        | Capabilities::booleanOperators
    );

    AssignmentOperatorElement::AssignmentOperatorElement() {
        currentBreakpointSet = false;
    }


    AssignmentOperatorElement::~AssignmentOperatorElement() {}


    Element* AssignmentOperatorElement::creator(const QString&) {
        return new AssignmentOperatorElement();
    }


    QString AssignmentOperatorElement::typeName() const {
        return elementName;
    }


    QString AssignmentOperatorElement::plugInName() const {
        return QString();
    }


    QString AssignmentOperatorElement::description() const {
        QString result;

        if (isRelationalOperator()) {
            result = tr("equal to relational operator");
        } else {
            result = tr("assignment operator");
        }

        return result;
    }


    DataType::ValueType AssignmentOperatorElement::valueType() const {
        return isRelationalOperator() ? BinaryRelationalOperatorElementBase::valueType() : DataType::ValueType::NONE;
    }


    Capabilities AssignmentOperatorElement::parentRequires(unsigned long index) const {
        Capabilities result;

        if (isRelationalOperator()) {
            result = Capabilities::allRValues;
        } else {
            if (index == 0) {
                result = Capabilities::allAssignables;
            } else {
                result = Capabilities::allRValues | Capabilities::compoundStatementOperator;
            }
        }

        return result;
    }


    Capabilities AssignmentOperatorElement::childProvidesCapabilities() const {
        return childProvides;
    }


    void AssignmentOperatorElement::setVisual(AssignmentOperatorVisual *newVisual) {
        BinaryOperatorElementBase::setVisual(newVisual);
    }


    AssignmentOperatorVisual* AssignmentOperatorElement::visual() const {
        return dynamic_cast<AssignmentOperatorVisual*>(BinaryOperatorElementBase::visual());
    }


    bool AssignmentOperatorElement::isTopLevelOperator() const {
        return !isRelationalOperator();
    }


    bool AssignmentOperatorElement::supportsInstructionBreakpoints() const {
        return !isRelationalOperator();
    }


    bool AssignmentOperatorElement::setInstructionBreakpoint(bool nowSet) {
        if (currentBreakpointSet != nowSet) {
            currentBreakpointSet = nowSet;
            instructionBreakpointStatusChanged(nowSet);
        }

        return true;
    }


    bool AssignmentOperatorElement::instructionBreakpointSet() const {
        return currentBreakpointSet;
    }


    Element::Precedence AssignmentOperatorElement::intrinsicPrecedence() const {
        Element::Precedence result;

        if (isRelationalOperator()) {
            result = BinaryRelationalOperatorElementBase::intrinsicPrecedence();
        } else {
            result = assignmentOperatorPrecedence;
        }

        return result;
    }


    void AssignmentOperatorElement::writeAddAttributes(
        XmlAttributes&                  attributes,
        QSharedPointer<FormatOrganizer> formats,
        ProgramFile&                    programFile
    ) const {
        BinaryOperatorElementBase::writeAddAttributes(attributes, formats, programFile);
        if (currentBreakpointSet) {
            attributes.append("ibkp", true);
        }
    }


    void AssignmentOperatorElement::readAttributes(
        QSharedPointer<XmlReader>  reader,
        const XmlAttributes&       attributes,
        const FormatsByIdentifier& formats,
        ProgramFile&               programFile,
        unsigned                   xmlVersion
    ) {
        BinaryOperatorElementBase::readAttributes(reader, attributes, formats, programFile, xmlVersion);

        if (!reader->hasError()) {
            if (attributes.hasAttribute("ibkp")) {
                bool ok;
                bool value = attributes.value<bool>("ibkp", &ok);
                if (ok) {
                    currentBreakpointSet = value;
                } else {
                    reader->raiseError(
                        tr("Tag \"%1\" has invalid breakpoint setting").arg(reader->qualifiedName().toString())
                    );
                }
            }
        }
    }


    bool AssignmentOperatorElement::isRelationalOperator() const {
        bool result = true;

        ElementPointer parentElement = parent();
        if (!parentElement.isNull()) {
            unsigned long indexUnderParent = parentElement->indexOfChild(weakThis().toStrongRef());
            result = parentElement->parentRequires(indexUnderParent).intersects(Capabilities::booleanOperators);
        }

        return result;
    }
}
