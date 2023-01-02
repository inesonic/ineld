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
* This file implements the \ref Ld::IfOperatorElement class.
***********************************************************************************************************************/

#include <QCoreApplication>
#include <QString>
#include <QSharedPointer>

#include "ld_capabilities.h"
#include "ld_data_type.h"
#include "ld_if_operator_visual.h"
#include "ld_binary_operator_element_base.h"
#include "ld_if_operator_element.h"

namespace Ld {
    const QString      IfOperatorElement::elementName("IfOperator");
    const Capabilities IfOperatorElement::childProvides = Capabilities::conditionalOperator;

    IfOperatorElement::IfOperatorElement() {
        currentBreakpointSet = false;
    }


    IfOperatorElement::~IfOperatorElement() {}


    Element* IfOperatorElement::creator(const QString&) {
        return new IfOperatorElement();
    }


    QString IfOperatorElement::typeName() const {
        return elementName;
    }


    QString IfOperatorElement::plugInName() const {
        return QString();
    }


    QString IfOperatorElement::description() const {
        return tr("if conditional operator");
    }


    DataType::ValueType IfOperatorElement::valueType() const {
        return DataType::ValueType::NONE;
    }


    Capabilities IfOperatorElement::parentRequires(unsigned long index) const {
        Capabilities result;

        if (index == 0) {
            result = Capabilities::allScalarRValues;
        } else {
            result = Capabilities::expressions | Capabilities::compoundStatementOperator;
        }

        return result;
    }


    Capabilities IfOperatorElement::childProvidesCapabilities() const {
        return childProvides;
    }


    bool IfOperatorElement::isTopLevelOperator() const {
        return true;
    }


    bool IfOperatorElement::supportsInstructionBreakpoints() const {
        return true;
    }


    bool IfOperatorElement::setInstructionBreakpoint(bool nowSet) {
        if (currentBreakpointSet != nowSet) {
            currentBreakpointSet = nowSet;
            instructionBreakpointStatusChanged(nowSet);
        }

        return true;
    }


    bool IfOperatorElement::instructionBreakpointSet() const {
        return currentBreakpointSet;
    }


    void IfOperatorElement::setVisual(IfOperatorVisual *newVisual) {
        BinaryOperatorElementBase::setVisual(newVisual);
    }


    IfOperatorVisual* IfOperatorElement::visual() const {
        return dynamic_cast<IfOperatorVisual*>(BinaryOperatorElementBase::visual());
    }


    bool IfOperatorElement::isFunctionLogicalStop(unsigned long /* functionChildIndex */) const {
        return true;
    }


    Element::Precedence IfOperatorElement::intrinsicPrecedence() const {
        return defaultPrecedence;
    }


    void IfOperatorElement::writeAddAttributes(
        XmlAttributes&                  attributes,
        QSharedPointer<FormatOrganizer> formats,
        ProgramFile&                    programFile
    ) const {
        BinaryOperatorElementBase::writeAddAttributes(attributes, formats, programFile);
        if (currentBreakpointSet) {
            attributes.append("ibkp", true);
        }
    }


    void IfOperatorElement::readAttributes(
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
}
