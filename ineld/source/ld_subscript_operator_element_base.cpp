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
* This file implements the \ref Ld::SubscriptOperatorElementBase class.
***********************************************************************************************************************/

#include <QCoreApplication>
#include <QString>
#include <QSharedPointer>

#include "ld_capabilities.h"
#include "ld_data_type.h"
#include "ld_range_2_element.h"
#include "ld_range_3_element.h"
#include "ld_subscript_index_operator_visual.h"
#include "ld_element_with_fixed_children.h"
#include "ld_subscript_operator_element_base.h"

namespace Ld {
    const Capabilities SubscriptOperatorElementBase::childProvides = Capabilities::subscripts;

    SubscriptOperatorElementBase::SubscriptOperatorElementBase(unsigned numberSubscripts) {
        setNumberChildren(numberSubscripts + 1, nullptr);
    }


    SubscriptOperatorElementBase::~SubscriptOperatorElementBase() {}


    DataType::ValueType SubscriptOperatorElementBase::valueType() const {
        DataType::ValueType result;

        ElementPointer child0 = child(0);
        ElementPointer child1 = child(1);

        QString             child1TypeName  = child1->typeName();
        DataType::ValueType child0ValueType = child0->valueType();

        if (child1TypeName == Ld::Range2Element::elementName || child1TypeName == Ld::Range3Element::elementName) {
            result = child0ValueType;
        } else {
            DataType::ValueType child1ValueType = child1->valueType();
            if (child1ValueType == DataType::ValueType::SET ||
                child1ValueType == DataType::ValueType::TUPLE ||
                child1ValueType == DataType::ValueType::MATRIX_BOOLEAN ||
                child1ValueType == DataType::ValueType::MATRIX_INTEGER ||
                child1ValueType == DataType::ValueType::MATRIX_REAL    ||
                child1ValueType == DataType::ValueType::MATRIX_COMPLEX    ) {
                result = child0ValueType;
            } else if (child1ValueType == DataType::ValueType::VARIANT) {
                result = DataType::ValueType::VARIANT;
            } else {
                if (child0ValueType == DataType::ValueType::TUPLE) {
                    result = DataType::ValueType::VARIANT;
                } else {
                    result = DataType::baseTypeFromMatrixType(child0ValueType);
                }
            }
        }

        return result;
    }


    Capabilities SubscriptOperatorElementBase::parentRequires(unsigned long index) const {
        Capabilities result;

        if (index == 0) {
            ElementPointer parent = SubscriptOperatorElementBase::parent();
            if (!parent.isNull()) {
                unsigned long indexOfThisElement = parent->indexOfChild(weakThis().toStrongRef());
                result = parent->parentRequires(indexOfThisElement);
            } else {
                result = Capabilities::allLValues;
            }
        } else {
            Ld::ElementPointer parentElement = parent();
            if (!parentElement.isNull()) {
                if (parentElement->childProvidesCapabilities().intersects(Ld::Capabilities::expressions) &&
                    parentElement->indexOfChild(weakThis().toStrongRef()) == 0                              ) {
                    result = Capabilities::allIntegerRValues;
                } else {
                    result = Capabilities::allIntegerRValues | Capabilities::allIterableAndRangeRValues;
                }
            } else {
                result = Capabilities::allIntegerRValues;
            }
        }

        return result;
    }


    Capabilities SubscriptOperatorElementBase::childProvidesCapabilities() const {
        return childProvides;
    }


    Element::Precedence SubscriptOperatorElementBase::intrinsicPrecedence() const {
        return subscriptOperatorPrecedence;
    }
}
