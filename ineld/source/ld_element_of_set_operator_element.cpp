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
* This file implements the \ref Ld::ElementOfSetOperatorElement class.
***********************************************************************************************************************/

#include <QCoreApplication>
#include <QString>
#include <QSharedPointer>

#include "ld_capabilities.h"
#include "ld_element_of_set_operator_visual.h"
#include "ld_binary_relational_operator_element_base.h"
#include "ld_element_of_set_operator_element.h"

namespace Ld {
    const QString      ElementOfSetOperatorElement::elementName("ElementOfSet");
    const Capabilities ElementOfSetOperatorElement::childProvides = (
          Capabilities::typeDeclaration
        | Capabilities::booleanOperators
    );

    ElementOfSetOperatorElement::ElementOfSetOperatorElement() {}


    ElementOfSetOperatorElement::~ElementOfSetOperatorElement() {}


    Element* ElementOfSetOperatorElement::creator(const QString&) {
        return new ElementOfSetOperatorElement();
    }


    QString ElementOfSetOperatorElement::typeName() const {
        return elementName;
    }


    QString ElementOfSetOperatorElement::plugInName() const {
        return QString();
    }


    QString ElementOfSetOperatorElement::description() const {
        return tr("element of operator");
    }


    DataType::ValueType ElementOfSetOperatorElement::valueType() const {
        return isRelationalOperator() ? BinaryRelationalOperatorElementBase::valueType() : DataType::ValueType::NONE;
    }


    Capabilities ElementOfSetOperatorElement::parentRequires(unsigned long index) const {
        Capabilities result;

        if (isRelationalOperator()) {
            if (index == 0) {
                result = Capabilities::allRValues;
            } else {
                result = Capabilities::allSetRValues;
            }
        } else {
            if (index == 0) {
                result = Capabilities::allValueAssignables;
            } else {
                result = Capabilities::dataTypes;
            }
        }

        return result;
    }


    Capabilities ElementOfSetOperatorElement::childProvidesCapabilities() const {
        return childProvides;
    }


    bool ElementOfSetOperatorElement::isTopLevelOperator() const {
        return !isRelationalOperator();
    }


    void ElementOfSetOperatorElement::setVisual(ElementOfSetOperatorVisual *newVisual) {
        BinaryOperatorElementBase::setVisual(newVisual);
    }


    ElementOfSetOperatorVisual* ElementOfSetOperatorElement::visual() const {
        return dynamic_cast<ElementOfSetOperatorVisual*>(BinaryOperatorElementBase::visual());
    }


    Element::Precedence ElementOfSetOperatorElement::intrinsicPrecedence() const {
        Element::Precedence result;

        if (isRelationalOperator()) {
            result = BinaryRelationalOperatorElementBase::intrinsicPrecedence();
        } else {
            result = assignmentOperatorPrecedence;
        }

        return result;
    }


    bool ElementOfSetOperatorElement::isRelationalOperator() const {
        bool result = true;

        ElementPointer parentElement = parent();
        if (!parentElement.isNull()) {
            unsigned long indexUnderParent = parentElement->indexOfChild(weakThis().toStrongRef());
            result = !parentElement->parentRequires(indexUnderParent).intersects(Capabilities::expressions);
        }

        return result;
    }
}
