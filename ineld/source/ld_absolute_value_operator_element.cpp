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
* This file implements the \ref Ld::AbsoluteValueOperatorElement class.
***********************************************************************************************************************/

#include <QCoreApplication>
#include <QString>
#include <QSharedPointer>

#include <cassert>

#include "ld_capabilities.h"
#include "ld_data_type.h"
#include "ld_absolute_value_operator_visual.h"
#include "ld_unary_operator_element_base.h"
#include "ld_absolute_value_operator_element.h"

namespace Ld {
    static const DataType::ValueType outputTypesByInputType[DataType::numberValueTypes] = {
        DataType::ValueType::NONE,    // Ld::DataType::ValueType::NONE
        DataType::ValueType::VARIANT, // Ld::DataType::ValueType::VARIANT
        DataType::ValueType::INTEGER, // Ld::DataType::ValueType::BOOLEAN,
        DataType::ValueType::INTEGER, // Ld::DataType::ValueType::INTEGER
        DataType::ValueType::REAL,    // Ld::DataType::ValueType::REAL
        DataType::ValueType::REAL,    // Ld::DataType::ValueType::COMPLEX
        DataType::ValueType::INTEGER, // Ld::DataType::ValueType::SET
        DataType::ValueType::NONE,    // Ld::DataType::ValueType::TUPLE
        DataType::ValueType::INTEGER, // Ld::DataType::ValueType::MATRIX_BOOLEAN
        DataType::ValueType::INTEGER, // Ld::DataType::ValueType::MATRIX_INTEGER
        DataType::ValueType::REAL,    // Ld::DataType::ValueType::MATRIX_REAL
        DataType::ValueType::COMPLEX  // Ld::DataType::ValueType::MATRIX_COMPLEX
    };

    const QString      AbsoluteValueOperatorElement::elementName("AbsoluteValueOperator");
    const Capabilities AbsoluteValueOperatorElement::childProvides = Capabilities::numericOperators;

    AbsoluteValueOperatorElement::AbsoluteValueOperatorElement() {}


    AbsoluteValueOperatorElement::~AbsoluteValueOperatorElement() {}


    Element* AbsoluteValueOperatorElement::creator(const QString&) {
        return new AbsoluteValueOperatorElement();
    }


    QString AbsoluteValueOperatorElement::typeName() const {
        return elementName;
    }


    QString AbsoluteValueOperatorElement::plugInName() const {
        return QString();
    }


    QString AbsoluteValueOperatorElement::description() const {
        return tr("absolute value, magnitude, determinate, and set cardinality operator");
    }


    DataType::ValueType AbsoluteValueOperatorElement::valueType() const {
        ElementPointer      child          = AbsoluteValueOperatorElement::child(0);
        DataType::ValueType childValueType = child.isNull() ? DataType::ValueType::NONE : child->valueType();

        return outputTypesByInputType[static_cast<unsigned char>(childValueType)];
    }


    Capabilities AbsoluteValueOperatorElement::parentRequires(unsigned long /* index */) const {
        return Capabilities::allScalarFiniteSetAndMatrixRValues;
    }


    Capabilities AbsoluteValueOperatorElement::childProvidesCapabilities() const {
        return childProvides;
    }


    void AbsoluteValueOperatorElement::setVisual(AbsoluteValueOperatorVisual *newVisual) {
        UnaryOperatorElementBase::setVisual(newVisual);
    }


    AbsoluteValueOperatorVisual* AbsoluteValueOperatorElement::visual() const {
        return dynamic_cast<AbsoluteValueOperatorVisual*>(UnaryOperatorElementBase::visual());
    }
}
