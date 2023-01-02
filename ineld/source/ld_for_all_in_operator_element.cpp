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
* This file implements the \ref Ld::ForAllInOperatorElement class.
***********************************************************************************************************************/

#include <QCoreApplication>
#include <QString>
#include <QSharedPointer>

#include "ld_capabilities.h"
#include "ld_data_type.h"
#include "ld_for_all_in_operator_visual.h"
#include "ld_binary_operator_element_base.h"
#include "ld_for_all_in_operator_element.h"

namespace Ld {
    const QString      ForAllInOperatorElement::elementName("ForAllInOperator");
    const Capabilities ForAllInOperatorElement::childProvides = Capabilities::iterationOperator;

    ForAllInOperatorElement::ForAllInOperatorElement() {
        currentBreakpointSet = false;
    }


    ForAllInOperatorElement::~ForAllInOperatorElement() {}


    Element* ForAllInOperatorElement::creator(const QString&) {
        return new ForAllInOperatorElement();
    }


    QString ForAllInOperatorElement::typeName() const {
        return elementName;
    }


    QString ForAllInOperatorElement::plugInName() const {
        return QString();
    }


    QString ForAllInOperatorElement::description() const {
        return tr("for each in/for each over operator");
    }


    DataType::ValueType ForAllInOperatorElement::valueType() const {
        return DataType::ValueType::NONE;
    }


    Capabilities ForAllInOperatorElement::parentRequires(unsigned long index) const {
        Capabilities result;

        switch (index) {
            case 0: { result = Capabilities::variables;                                              break; }
            case 1: { result = Capabilities::allIterableAndRangeRValues;                             break; }
            case 2: { result = Capabilities::expressions | Capabilities::compoundStatementOperator;  break; }

            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    Capabilities ForAllInOperatorElement::childProvidesCapabilities() const {
        return childProvides;
    }


    bool ForAllInOperatorElement::isTopLevelOperator() const {
        return true;
    }


    bool ForAllInOperatorElement::supportsInstructionBreakpoints() const {
        return true;
    }


    bool ForAllInOperatorElement::setInstructionBreakpoint(bool nowSet) {
        if (currentBreakpointSet != nowSet) {
            currentBreakpointSet = nowSet;
            instructionBreakpointStatusChanged(nowSet);
        }

        return true;
    }


    bool ForAllInOperatorElement::instructionBreakpointSet() const {
        return currentBreakpointSet;
    }


    bool ForAllInOperatorElement::isSetIterable() const {
        bool result;

        ElementPointer child1 = child(1);
        if (child1.isNull()) {
            result = false;
        } else {
            DataType::ValueType valueType  = child1->valueType();

            if        (valueType == DataType::ValueType::SET) {
                result = true;
            } else if (valueType == DataType::ValueType::VARIANT        ||
                       valueType == DataType::ValueType::TUPLE          ||
                       valueType == DataType::ValueType::MATRIX_BOOLEAN ||
                       valueType == DataType::ValueType::MATRIX_INTEGER ||
                       valueType == DataType::ValueType::MATRIX_REAL    ||
                       valueType == DataType::ValueType::MATRIX_COMPLEX    ) {
                result = false;
            } else {
                Capabilities childCapabilities = child1->childProvidesCapabilities();
                result = childCapabilities.intersects(Capabilities::allSetRValues);
            }
        }

        return result;
    }


    void ForAllInOperatorElement::setVisual(ForAllInOperatorVisual *newVisual) {
        TrinaryOperatorElementBase::setVisual(newVisual);
    }


    ForAllInOperatorVisual* ForAllInOperatorElement::visual() const {
        return dynamic_cast<ForAllInOperatorVisual*>(TrinaryOperatorElementBase::visual());
    }


    bool ForAllInOperatorElement::isFunctionLogicalStop(unsigned long functionChildIndex) const {
        return (functionChildIndex > 0);
    }


    Element::Precedence ForAllInOperatorElement::intrinsicPrecedence() const {
        return defaultPrecedence;
    }


    void ForAllInOperatorElement::writeAddAttributes(
        XmlAttributes&                  attributes,
        QSharedPointer<FormatOrganizer> formats,
        ProgramFile&                    programFile
    ) const {
        TrinaryOperatorElementBase::writeAddAttributes(attributes, formats, programFile);
        if (currentBreakpointSet) {
            attributes.append("ibkp", true);
        }
    }


    void ForAllInOperatorElement::readAttributes(
        QSharedPointer<XmlReader>  reader,
        const XmlAttributes&       attributes,
        const FormatsByIdentifier& formats,
        ProgramFile&               programFile,
        unsigned                   xmlVersion
    ) {
        TrinaryOperatorElementBase::readAttributes(reader, attributes, formats, programFile, xmlVersion);

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
