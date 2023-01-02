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
* This file implements the \ref Ld::LogicalConditionalOperatorElement class.
***********************************************************************************************************************/

#include <QCoreApplication>
#include <QString>
#include <QSharedPointer>

#include "ld_capabilities.h"
#include "ld_data_type.h"
#include "ld_logical_conditional_operator_visual.h"
#include "ld_binary_operator_element_base.h"
#include "ld_logical_conditional_operator_element.h"

namespace Ld {
    const QString      LogicalConditionalOperatorElement::elementName("LogicalConditionalOperator");
    const Capabilities LogicalConditionalOperatorElement::childProvides = (
          Capabilities::conditionalOperator
        | Capabilities::booleanOperators
    );

    LogicalConditionalOperatorElement::LogicalConditionalOperatorElement() {
        currentBreakpointSet = false;
    }


    LogicalConditionalOperatorElement::~LogicalConditionalOperatorElement() {}


    Element* LogicalConditionalOperatorElement::creator(const QString&) {
        return new LogicalConditionalOperatorElement();
    }


    QString LogicalConditionalOperatorElement::typeName() const {
        return elementName;
    }


    QString LogicalConditionalOperatorElement::plugInName() const {
        return QString();
    }


    QString LogicalConditionalOperatorElement::description() const {
        return tr("logical conditional operator");
    }


    DataType::ValueType LogicalConditionalOperatorElement::valueType() const {
        return isRelationalOperator() ? DataType::ValueType::BOOLEAN : DataType::ValueType::NONE;
    }


    Capabilities LogicalConditionalOperatorElement::parentRequires(unsigned long index) const {
        Capabilities result;

        if (index == 0) {
            result = Capabilities::allScalarRValues;
        } else {
            if (isRelationalOperator()) {
                result = Capabilities::allScalarRValues;
            } else {
                result = Capabilities::expressions;
            }
        }

        return result;
    }


    Capabilities LogicalConditionalOperatorElement::childProvidesCapabilities() const {
        return childProvides;
    }


    bool LogicalConditionalOperatorElement::isTopLevelOperator() const {
        return !isRelationalOperator();
    }


    bool LogicalConditionalOperatorElement::supportsInstructionBreakpoints() const {
        return !isRelationalOperator();
    }


    bool LogicalConditionalOperatorElement::setInstructionBreakpoint(bool nowSet) {
        if (currentBreakpointSet != nowSet) {
            currentBreakpointSet = nowSet;
            instructionBreakpointStatusChanged(nowSet);
        }

        return true;
    }


    bool LogicalConditionalOperatorElement::instructionBreakpointSet() const {
        return currentBreakpointSet;
    }


    void LogicalConditionalOperatorElement::setVisual(LogicalConditionalOperatorVisual *newVisual) {
        BinaryOperatorElementBase::setVisual(newVisual);
    }


    LogicalConditionalOperatorVisual* LogicalConditionalOperatorElement::visual() const {
        return dynamic_cast<LogicalConditionalOperatorVisual*>(BinaryOperatorElementBase::visual());
    }


    Element::Precedence  LogicalConditionalOperatorElement::intrinsicPrecedence() const {
        return logicalSpecialOperatorPrecedence;
    }


    void LogicalConditionalOperatorElement::writeAddAttributes(
        XmlAttributes&                  attributes,
        QSharedPointer<FormatOrganizer> formats,
        ProgramFile&                    programFile
    ) const {
        BinaryOperatorElementBase::writeAddAttributes(attributes, formats, programFile);
        if (currentBreakpointSet) {
            attributes.append("ibkp", true);
        }
    }


    void LogicalConditionalOperatorElement::readAttributes(
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


    bool LogicalConditionalOperatorElement::isRelationalOperator() const {
        bool result = true;

        ElementPointer parentElement = parent();
        if (!parentElement.isNull()) {
            unsigned long indexUnderParent = parentElement->indexOfChild(weakThis().toStrongRef());
            result = parentElement->parentRequires(indexUnderParent).intersects(Capabilities::booleanOperators);
        }

        return result;
    }
}
