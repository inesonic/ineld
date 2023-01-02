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
* This file implements a single function that can be called configure set functions.
***********************************************************************************************************************/

#include <QCoreApplication>

#include "ld_configure_helpers.h"
#include "ld_data_type.h"
#include "ld_variable_name.h"
#include "ld_function_data.h"
#include "ld_function_database.h"
#include "ld_configure_set_functions.h"

#define TR(_x) QCoreApplication::translate("ConfigureSetFunctions", (_x))

namespace Ld {
    void configureSetFunctions() {
        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::size"),
                QString(),
                VariableName("SizeOf"),
                false,
                TR("sizeof"),
                TR("Determine the size of a matrix, set, or tuple"),
                TR("Set/Tuple"),
                TR("sizeof"),
                false,
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::SET, TR("set")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::TUPLE, TR("tuple")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::VARIANT, TR("value")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::alphabet"),
                QString(),
                VariableName("Alph"),
                false,
                TR("alphabet"),
                TR("Determine the unique members of a set, tuple, or matrix"),
                TR("Set/Tuple"),
                TR("alphabet"),
                false,
                FunctionVariant(
                    DataType::ValueType::SET,
                    DataType::ValueType::SET, TR("set")
                ),
                FunctionVariant(
                    DataType::ValueType::SET,
                    DataType::ValueType::TUPLE, TR("tuple")
                ),
                FunctionVariant(
                    DataType::ValueType::SET,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::SET,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::SET,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::SET,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::SET,
                    DataType::ValueType::VARIANT, TR("value")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::toTupleString"),
                QString(),
                VariableName("ToString"),
                false,
                TR("tostring"),
                TR("Converts a scalar to a string representation"),
                TR("Set/Tuple"),
                TR("tostring"),
                false,
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::BOOLEAN, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::INTEGER, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::REAL, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::COMPLEX, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::BOOLEAN, TR("value"),
                    DataType::ValueType::TUPLE, TR("format")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::INTEGER, TR("value"),
                    DataType::ValueType::TUPLE, TR("format")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::REAL, TR("value"),
                    DataType::ValueType::TUPLE, TR("format")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::COMPLEX, TR("value"),
                    DataType::ValueType::TUPLE, TR("format")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::BOOLEAN, TR("value"),
                    DataType::ValueType::VARIANT, TR("format")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::INTEGER, TR("value"),
                    DataType::ValueType::VARIANT, TR("format")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::REAL, TR("value"),
                    DataType::ValueType::VARIANT, TR("format")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::COMPLEX, TR("value"),
                    DataType::ValueType::VARIANT, TR("format")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("value"),
                    DataType::ValueType::TUPLE, TR("format")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("value"),
                    DataType::ValueType::VARIANT, TR("format")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::find"),
                QString(),
                VariableName("Find"),
                false,
                TR("find"),
                TR("Finds a sub-tuple within a tuple.  Also works for strings."),
                TR("Set/Tuple"),
                TR("find"),
                false,
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("sub")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("sub"),
                    DataType::ValueType::BOOLEAN, TR("starting at")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("sub"),
                    DataType::ValueType::INTEGER, TR("starting at")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("sub"),
                    DataType::ValueType::REAL, TR("starting at")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("sub"),
                    DataType::ValueType::COMPLEX, TR("starting at")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("sub"),
                    DataType::ValueType::VARIANT, TR("starting at")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("sub")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("sub"),
                    DataType::ValueType::BOOLEAN, TR("starting at")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("sub"),
                    DataType::ValueType::INTEGER, TR("starting at")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("sub"),
                    DataType::ValueType::REAL, TR("starting at")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("sub"),
                    DataType::ValueType::COMPLEX, TR("starting at")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("sub"),
                    DataType::ValueType::VARIANT, TR("starting at")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("sub")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("sub"),
                    DataType::ValueType::BOOLEAN, TR("starting at")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("sub"),
                    DataType::ValueType::INTEGER, TR("starting at")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("sub"),
                    DataType::ValueType::REAL, TR("starting at")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("sub"),
                    DataType::ValueType::COMPLEX, TR("starting at")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("sub"),
                    DataType::ValueType::VARIANT, TR("starting at")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("sub")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("sub"),
                    DataType::ValueType::BOOLEAN, TR("starting at")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("sub"),
                    DataType::ValueType::INTEGER, TR("starting at")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("sub"),
                    DataType::ValueType::REAL, TR("starting at")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("sub"),
                    DataType::ValueType::COMPLEX, TR("starting at")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("sub"),
                    DataType::ValueType::VARIANT, TR("starting at")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::split"),
                QString(),
                VariableName("Split"),
                false,
                TR("split"),
                TR("Splits a tuple into multiple smaller sections.  Also works for strings."),
                TR("Set/Tuple"),
                TR("split"),
                false,
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators"),
                    DataType::ValueType::BOOLEAN, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators"),
                    DataType::ValueType::BOOLEAN, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators"),
                    DataType::ValueType::BOOLEAN, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators"),
                    DataType::ValueType::BOOLEAN, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators"),
                    DataType::ValueType::INTEGER, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators"),
                    DataType::ValueType::INTEGER, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators"),
                    DataType::ValueType::INTEGER, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators"),
                    DataType::ValueType::INTEGER, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators"),
                    DataType::ValueType::REAL, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators"),
                    DataType::ValueType::REAL, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators"),
                    DataType::ValueType::REAL, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators"),
                    DataType::ValueType::REAL, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators"),
                    DataType::ValueType::COMPLEX, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators"),
                    DataType::ValueType::COMPLEX, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators"),
                    DataType::ValueType::COMPLEX, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators"),
                    DataType::ValueType::COMPLEX, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::SET, TR("split at")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators"),
                    DataType::ValueType::BOOLEAN, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators"),
                    DataType::ValueType::BOOLEAN, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators"),
                    DataType::ValueType::BOOLEAN, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators"),
                    DataType::ValueType::BOOLEAN, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators"),
                    DataType::ValueType::INTEGER, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators"),
                    DataType::ValueType::INTEGER, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators"),
                    DataType::ValueType::INTEGER, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators"),
                    DataType::ValueType::INTEGER, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators"),
                    DataType::ValueType::REAL, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators"),
                    DataType::ValueType::REAL, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators"),
                    DataType::ValueType::REAL, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators"),
                    DataType::ValueType::REAL, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators"),
                    DataType::ValueType::COMPLEX, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators"),
                    DataType::ValueType::COMPLEX, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators"),
                    DataType::ValueType::COMPLEX, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators"),
                    DataType::ValueType::COMPLEX, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators"),
                    DataType::ValueType::BOOLEAN, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators"),
                    DataType::ValueType::BOOLEAN, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators"),
                    DataType::ValueType::BOOLEAN, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators"),
                    DataType::ValueType::BOOLEAN, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators"),
                    DataType::ValueType::INTEGER, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators"),
                    DataType::ValueType::INTEGER, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators"),
                    DataType::ValueType::INTEGER, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators"),
                    DataType::ValueType::INTEGER, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators"),
                    DataType::ValueType::REAL, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators"),
                    DataType::ValueType::REAL, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators"),
                    DataType::ValueType::REAL, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators"),
                    DataType::ValueType::REAL, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators"),
                    DataType::ValueType::COMPLEX, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators"),
                    DataType::ValueType::COMPLEX, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators"),
                    DataType::ValueType::COMPLEX, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::TUPLE, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators"),
                    DataType::ValueType::COMPLEX, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::SET, TR("split at")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators"),
                    DataType::ValueType::BOOLEAN, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators"),
                    DataType::ValueType::BOOLEAN, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators"),
                    DataType::ValueType::BOOLEAN, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators"),
                    DataType::ValueType::BOOLEAN, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators"),
                    DataType::ValueType::INTEGER, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators"),
                    DataType::ValueType::INTEGER, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators"),
                    DataType::ValueType::INTEGER, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators"),
                    DataType::ValueType::INTEGER, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators"),
                    DataType::ValueType::REAL, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators"),
                    DataType::ValueType::REAL, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators"),
                    DataType::ValueType::REAL, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators"),
                    DataType::ValueType::REAL, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators"),
                    DataType::ValueType::COMPLEX, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators"),
                    DataType::ValueType::COMPLEX, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators"),
                    DataType::ValueType::COMPLEX, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::SET, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators"),
                    DataType::ValueType::COMPLEX, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators"),
                    DataType::ValueType::BOOLEAN, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators"),
                    DataType::ValueType::BOOLEAN, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators"),
                    DataType::ValueType::BOOLEAN, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators"),
                    DataType::ValueType::BOOLEAN, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators"),
                    DataType::ValueType::INTEGER, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators"),
                    DataType::ValueType::INTEGER, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators"),
                    DataType::ValueType::INTEGER, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators"),
                    DataType::ValueType::INTEGER, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators"),
                    DataType::ValueType::REAL, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators"),
                    DataType::ValueType::REAL, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators"),
                    DataType::ValueType::REAL, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators"),
                    DataType::ValueType::REAL, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators"),
                    DataType::ValueType::COMPLEX, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators"),
                    DataType::ValueType::COMPLEX, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators"),
                    DataType::ValueType::COMPLEX, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::TUPLE, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators"),
                    DataType::ValueType::COMPLEX, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators"),
                    DataType::ValueType::BOOLEAN, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators"),
                    DataType::ValueType::BOOLEAN, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators"),
                    DataType::ValueType::BOOLEAN, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators"),
                    DataType::ValueType::BOOLEAN, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators"),
                    DataType::ValueType::INTEGER, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators"),
                    DataType::ValueType::INTEGER, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators"),
                    DataType::ValueType::INTEGER, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators"),
                    DataType::ValueType::INTEGER, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators"),
                    DataType::ValueType::REAL, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators"),
                    DataType::ValueType::REAL, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators"),
                    DataType::ValueType::REAL, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators"),
                    DataType::ValueType::REAL, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::BOOLEAN, TR("keep separators"),
                    DataType::ValueType::COMPLEX, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::INTEGER, TR("keep separators"),
                    DataType::ValueType::COMPLEX, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::REAL, TR("keep separators"),
                    DataType::ValueType::COMPLEX, TR("remove empty")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("tuple"),
                    DataType::ValueType::VARIANT, TR("split at"),
                    DataType::ValueType::COMPLEX, TR("keep separators"),
                    DataType::ValueType::COMPLEX, TR("remove empty")
                )
            )
        );
    }
}
