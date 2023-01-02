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
* This file implements the \ref Ld::NumericGroupingOperatorElementBase class.
***********************************************************************************************************************/

#include "ld_data_type.h"
#include "ld_grouping_operator_element_base.h"
#include "ld_numeric_grouping_operator_element_base.h"

namespace Ld {
    const Capabilities NumericGroupingOperatorElementBase::childProvides = Capabilities::numericOperators;
    const DataType::ValueType NumericGroupingOperatorElementBase::allowedConversions[DataType::numberValueTypes] = {
        DataType::ValueType::NONE,           /* DataType::ValueType::NONE */
        DataType::ValueType::REAL,           /* DataType::ValueType::VARIANT */
        DataType::ValueType::INTEGER,        /* DataType::ValueType::BOOLEAN */
        DataType::ValueType::INTEGER,        /* DataType::ValueType::INTEGER */
        DataType::ValueType::REAL,           /* DataType::ValueType::REAL */
        DataType::ValueType::REAL,           /* DataType::ValueType::COMPLEX */
        DataType::ValueType::NONE,           /* DataType::ValueType::SET */
        DataType::ValueType::NONE,           /* DataType::ValueType::TUPLE */
        DataType::ValueType::NONE,           /* DataType::ValueType::MATRIX_BOOLEAN */
        DataType::ValueType::NONE,           /* DataType::ValueType::MATRIX_INTEGER */
        DataType::ValueType::NONE,           /* DataType::ValueType::MATRIX_REAL */
        DataType::ValueType::NONE            /* DataType::ValueType::MATRIX_COMPLEX */
    };

    NumericGroupingOperatorElementBase::NumericGroupingOperatorElementBase() {}


    NumericGroupingOperatorElementBase::~NumericGroupingOperatorElementBase() {}


    Capabilities NumericGroupingOperatorElementBase::parentRequires(unsigned long /* index */) const {
        return Capabilities::allScalarRValues;
    }


    Capabilities NumericGroupingOperatorElementBase::childProvidesCapabilities() const {
        return childProvides;
    }


    DataType::ValueType NumericGroupingOperatorElementBase::valueType() const {
        ElementPointer child = UnaryOperatorElementBase::child(0);
        return   child.isNull()
               ? DataType::ValueType::NONE
               : allowedConversions[static_cast<unsigned char>(child->valueType())];
    }
}
