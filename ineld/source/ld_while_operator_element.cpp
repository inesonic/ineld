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
* This file implements the \ref Ld::WhileOperatorElement class.
***********************************************************************************************************************/

#include <QCoreApplication>
#include <QString>
#include <QSharedPointer>

#include "ld_capabilities.h"
#include "ld_data_type.h"
#include "ld_while_operator_visual.h"
#include "ld_binary_operator_element_base.h"
#include "ld_while_operator_element.h"

namespace Ld {
    const QString      WhileOperatorElement::elementName("WhileOperator");
    const Capabilities WhileOperatorElement::childProvides = Capabilities::iterationOperator;

    WhileOperatorElement::WhileOperatorElement() {
        currentBreakpointSet = false;
    }


    WhileOperatorElement::~WhileOperatorElement() {}


    Element* WhileOperatorElement::creator(const QString&) {
        return new WhileOperatorElement();
    }


    QString WhileOperatorElement::typeName() const {
        return elementName;
    }


    QString WhileOperatorElement::plugInName() const {
        return QString();
    }


    QString WhileOperatorElement::description() const {
        return tr("while looping operator");
    }


    DataType::ValueType WhileOperatorElement::valueType() const {
        return DataType::ValueType::NONE;
    }


    Capabilities WhileOperatorElement::parentRequires(unsigned long index) const {
        Capabilities result;

        if (index == 0) {
            result = Capabilities::allScalarRValues;
        } else {
            result = Capabilities::expressions | Capabilities::compoundStatementOperator;
        }

        return result;
    }


    Capabilities WhileOperatorElement::childProvidesCapabilities() const {
        return childProvides;
    }


    bool WhileOperatorElement::isTopLevelOperator() const {
        return true;
    }


    bool WhileOperatorElement::supportsInstructionBreakpoints() const {
        return true;
    }


    bool WhileOperatorElement::setInstructionBreakpoint(bool nowSet) {
        if (currentBreakpointSet != nowSet) {
            currentBreakpointSet = nowSet;
            instructionBreakpointStatusChanged(nowSet);
        }

        return true;
    }


    bool WhileOperatorElement::instructionBreakpointSet() const {
        return currentBreakpointSet;
    }


    void WhileOperatorElement::setVisual(WhileOperatorVisual *newVisual) {
        BinaryOperatorElementBase::setVisual(newVisual);
    }


    WhileOperatorVisual* WhileOperatorElement::visual() const {
        return dynamic_cast<WhileOperatorVisual*>(BinaryOperatorElementBase::visual());
    }


    bool WhileOperatorElement::isFunctionLogicalStop(unsigned long /* functionChildIndex */) const {
        return true;
    }


    Element::Precedence WhileOperatorElement::intrinsicPrecedence() const {
        return defaultPrecedence;
    }


    void WhileOperatorElement::writeAddAttributes(
        XmlAttributes&                  attributes,
        QSharedPointer<FormatOrganizer> formats,
        ProgramFile&                    programFile
    ) const {
        BinaryOperatorElementBase::writeAddAttributes(attributes, formats, programFile);
        if (currentBreakpointSet) {
            attributes.append("ibkp", true);
        }
    }


    void WhileOperatorElement::readAttributes(
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
