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
* This file implements the \ref Ld::SubfscriptRowColumnOperatorElement class.
***********************************************************************************************************************/

#include <QCoreApplication>
#include <QString>
#include <QSharedPointer>

#include "ld_capabilities.h"
#include "ld_data_type.h"
#include "ld_range_2_element.h"
#include "ld_range_3_element.h"
#include "ld_subscript_row_column_operator_visual.h"
#include "ld_subscript_operator_element_base.h"
#include "ld_subscript_row_column_operator_element.h"

namespace Ld {
    const QString SubscriptRowColumnOperatorElement::elementName("SubscriptRowColumnOperator");
    const DataType::ValueType SubscriptRowColumnOperatorElement::allowedConversions
        [DataType::numberValueTypes]
        [DataType::numberValueTypes]
        [DataType::numberValueTypes] = {
        /* Base: DataType::ValueType::NONE */
        {
            /* S1:DataType::ValueType::NONE */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::VARIANT */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::BOOLEAN */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::INTEGER */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::REAL */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::COMPLEX */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::SET */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::TUPLE */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_BOOLEAN */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_INTEGER */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_REAL */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_COMPLEX */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            }
        },
        /* Base: DataType::ValueType::VARIANT */
        {
            /* S1:DataType::ValueType::NONE */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::VARIANT */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::REAL */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::SET */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::VARIANT         /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::BOOLEAN */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::INTEGER */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::REAL */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::SET */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::VARIANT         /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::REAL */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::REAL */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::SET */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::VARIANT         /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::COMPLEX */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::REAL */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::SET */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::VARIANT         /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::SET */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::REAL */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::SET */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::VARIANT         /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::TUPLE */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::REAL */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::SET */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::VARIANT         /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_BOOLEAN */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::REAL */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::SET */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::VARIANT         /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_INTEGER */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::REAL */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::SET */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::VARIANT         /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_REAL */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::REAL */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::SET */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::VARIANT         /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_COMPLEX */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::REAL */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::SET */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::VARIANT         /* S2: DataType::ValueType::MATRIX_COMPLEX */
            }
        },
        /* Base: DataType::ValueType::BOOLEAN */
        {
            /* S1:DataType::ValueType::NONE */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::VARIANT */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::BOOLEAN */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::INTEGER */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::REAL */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::COMPLEX */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::SET */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::TUPLE */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_BOOLEAN */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_INTEGER */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_REAL */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_COMPLEX */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            }
        },
        /* Base: DataType::ValueType::INTEGER */
        {
            /* S1:DataType::ValueType::NONE */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::VARIANT */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::BOOLEAN */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::INTEGER */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::REAL */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::COMPLEX */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::SET */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::TUPLE */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_BOOLEAN */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_INTEGER */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_REAL */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_COMPLEX */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            }
        },
        /* Base: DataType::ValueType::REAL */
        {
            /* S1:DataType::ValueType::NONE */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::VARIANT */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::BOOLEAN */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::INTEGER */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::REAL */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::COMPLEX */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::SET */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::TUPLE */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_BOOLEAN */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_INTEGER */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_REAL */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_COMPLEX */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            }
        },
        /* Base: DataType::ValueType::COMPLEX */
        {
            /* S1:DataType::ValueType::NONE */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::VARIANT */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::BOOLEAN */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::INTEGER */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::REAL */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::COMPLEX */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::SET */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::TUPLE */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_BOOLEAN */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_INTEGER */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_REAL */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_COMPLEX */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            }
        },
        /* Base: DataType::ValueType::SET */
        {
            /* S1:DataType::ValueType::NONE */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::VARIANT */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::BOOLEAN */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::INTEGER */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::REAL */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::COMPLEX */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::SET */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::TUPLE */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_BOOLEAN */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_INTEGER */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_REAL */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_COMPLEX */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            }
        },
        /* Base: DataType::ValueType::TUPLE */
        {
            /* S1:DataType::ValueType::NONE */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::VARIANT */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::BOOLEAN */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::INTEGER */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::REAL */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::COMPLEX */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::SET */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::TUPLE */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_BOOLEAN */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_INTEGER */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_REAL */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_COMPLEX */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
        },
        /* Base: DataType::ValueType::MATRIX_BOOLEAN */
        {
            /* S1:DataType::ValueType::NONE */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::VARIANT */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::REAL */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::SET */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::VARIANT         /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::BOOLEAN */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::INTEGER */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::BOOLEAN,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::BOOLEAN,        /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::BOOLEAN,        /* S2: DataType::ValueType::REAL */
                DataType::ValueType::BOOLEAN,        /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_BOOLEAN  /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::REAL */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::BOOLEAN,        /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::BOOLEAN,        /* S2: DataType::ValueType::REAL */
                DataType::ValueType::BOOLEAN,        /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_BOOLEAN  /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::COMPLEX */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::BOOLEAN,        /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::BOOLEAN,        /* S2: DataType::ValueType::REAL */
                DataType::ValueType::BOOLEAN,        /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_BOOLEAN  /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::SET */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::REAL */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_BOOLEAN  /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::TUPLE */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::REAL */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_BOOLEAN  /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_BOOLEAN */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_INTEGER */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::REAL */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_BOOLEAN  /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_REAL */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::REAL */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_BOOLEAN  /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_COMPLEX */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::REAL */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_BOOLEAN, /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_BOOLEAN  /* S2: DataType::ValueType::MATRIX_COMPLEX */
            }
        },
        /* Base: DataType::ValueType::MATRIX_INTEGER */
        {
            /* S1:DataType::ValueType::NONE */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::VARIANT */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::REAL */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::SET */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::VARIANT         /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::BOOLEAN */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::INTEGER */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::INTEGER,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::INTEGER,        /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::INTEGER,        /* S2: DataType::ValueType::REAL */
                DataType::ValueType::INTEGER,        /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_INTEGER  /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::REAL */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::INTEGER,        /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::INTEGER,        /* S2: DataType::ValueType::REAL */
                DataType::ValueType::INTEGER,        /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_INTEGER  /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::COMPLEX */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::INTEGER,        /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::INTEGER,        /* S2: DataType::ValueType::REAL */
                DataType::ValueType::INTEGER,        /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_INTEGER  /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::SET */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::REAL */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_INTEGER  /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::TUPLE */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::REAL */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_INTEGER  /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_BOOLEAN */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_INTEGER */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::REAL */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_INTEGER  /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_REAL */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::REAL */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_INTEGER  /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_COMPLEX */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::REAL */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_INTEGER, /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_INTEGER  /* S2: DataType::ValueType::MATRIX_COMPLEX */
            }
        },
        /* Base: DataType::ValueType::MATRIX_REAL */
        {
            /* S1:DataType::ValueType::NONE */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::VARIANT */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::REAL */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::SET */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::VARIANT         /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::BOOLEAN */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::INTEGER */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::REAL,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::REAL,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::REAL,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::REAL,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_REAL     /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::REAL */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::REAL,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::REAL,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::REAL,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_REAL     /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::COMPLEX */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::REAL,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::REAL,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::REAL,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_REAL     /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::SET */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::REAL */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_REAL     /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::TUPLE */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::REAL */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_REAL     /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_BOOLEAN */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_INTEGER */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::REAL */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_REAL     /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_REAL */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::REAL */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_REAL     /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_COMPLEX */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::REAL */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_REAL,    /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_REAL     /* S2: DataType::ValueType::MATRIX_COMPLEX */
            }
        },
        /* Base: DataType::ValueType::MATRIX_COMPLEX */
        {
            /* S1:DataType::ValueType::NONE */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::VARIANT */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::REAL */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::SET */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::VARIANT         /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::BOOLEAN */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::INTEGER */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::COMPLEX,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::COMPLEX,        /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::COMPLEX,        /* S2: DataType::ValueType::REAL */
                DataType::ValueType::COMPLEX,        /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_COMPLEX  /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::REAL */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::COMPLEX,        /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::COMPLEX,        /* S2: DataType::ValueType::REAL */
                DataType::ValueType::COMPLEX,        /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_COMPLEX  /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::COMPLEX */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::COMPLEX,        /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::COMPLEX,        /* S2: DataType::ValueType::REAL */
                DataType::ValueType::COMPLEX,        /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_COMPLEX  /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::SET */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::REAL */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_COMPLEX  /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::TUPLE */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::REAL */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_COMPLEX  /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_BOOLEAN */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::REAL */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::SET */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::NONE            /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_INTEGER */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::REAL */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_COMPLEX  /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_REAL */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::REAL */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_COMPLEX  /* S2: DataType::ValueType::MATRIX_COMPLEX */
            },
            /* S1: DataType::ValueType::MATRIX_COMPLEX */
            {
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::NONE */
                DataType::ValueType::VARIANT,        /* S2: DataType::ValueType::VARIANT */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::BOOLEAN */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::INTEGER */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::REAL */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::COMPLEX */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::SET */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::TUPLE */
                DataType::ValueType::NONE,           /* S2: DataType::ValueType::MATRIX_BOOLEAN */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::MATRIX_INTEGER */
                DataType::ValueType::MATRIX_COMPLEX, /* S2: DataType::ValueType::MATRIX_REAL */
                DataType::ValueType::MATRIX_COMPLEX  /* S2: DataType::ValueType::MATRIX_COMPLEX */
            }
        }
    };


    SubscriptRowColumnOperatorElement::SubscriptRowColumnOperatorElement():SubscriptOperatorElementBase(2) {}


    SubscriptRowColumnOperatorElement::~SubscriptRowColumnOperatorElement() {}


    Element* SubscriptRowColumnOperatorElement::creator(const QString&) {
        return new SubscriptRowColumnOperatorElement();
    }


    QString SubscriptRowColumnOperatorElement::typeName() const {
        return elementName;
    }


    QString SubscriptRowColumnOperatorElement::plugInName() const {
        return QString();
    }


    QString SubscriptRowColumnOperatorElement::description() const {
        return tr("subscript operator");
    }


    DataType::ValueType SubscriptRowColumnOperatorElement::valueType() const {
        DataType::ValueType result;

        ElementPointer child0 = child(0);
        ElementPointer child1 = child(1);
        ElementPointer child2 = child(2);

        if (!child0.isNull() && !child1.isNull()) {
            DataType::ValueType c0ValueType = child0->valueType();

            DataType::ValueType c1ValueType;
            QString             c1TypeName = child1->typeName();
            if (c1TypeName == Range2Element::elementName || c1TypeName == Range3Element::elementName) {
                c1ValueType = DataType::ValueType::TUPLE;
            } else {
                c1ValueType = child1->valueType();
            }

            DataType::ValueType c2ValueType;
            QString             c2TypeName = child2->typeName();
            if (c2TypeName == Range2Element::elementName || c2TypeName == Range3Element::elementName) {
                c2ValueType = DataType::ValueType::TUPLE;
            } else {
                c2ValueType = child2->valueType();
            }

            result = allowedConversions[static_cast<unsigned char>(c0ValueType)]
                                       [static_cast<unsigned char>(c1ValueType)]
                                       [static_cast<unsigned char>(c2ValueType)];
        } else {
            result = DataType::ValueType::NONE;
        }

        return result;
    }


    void SubscriptRowColumnOperatorElement::setVisual(SubscriptRowColumnOperatorVisual *newVisual) {
        SubscriptOperatorElementBase::setVisual(newVisual);
    }


    SubscriptRowColumnOperatorVisual* SubscriptRowColumnOperatorElement::visual() const {
        return dynamic_cast<SubscriptRowColumnOperatorVisual*>(SubscriptOperatorElementBase::visual());
    }
}
