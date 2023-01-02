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
* This file implements the \ref Ld::configure function.
***********************************************************************************************************************/

#include <QCoreApplication>

#include "ld_configure_helpers.h"
#include "ld_data_type.h"
#include "ld_variable_name.h"
#include "ld_function_data.h"
#include "ld_function_database.h"
#include "ld_configure_set_functions.h"

#define TR(_x) QCoreApplication::translate("ConfigureSpecialFunctions", (_x))

namespace Ld {
    void configureMatrixFunctions() {
//        FORCE_INLINE MatrixReal inverse(const MatrixBoolean& matrix) {
//        FORCE_INLINE MatrixReal inverse(const MatrixInteger& matrix) {
//        FORCE_INLINE MatrixReal inverse(const MatrixReal& matrix) {
//        FORCE_INLINE MatrixComplex inverse(const MatrixComplex& matrix) {

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::numberRows"),
                QString(),
                VariableName("NumberRows"),
                false,
                TR("nrows"),
                TR("Determine the number of rows in a matrix"),
                TR("Matrix"),
                TR("nrows"),
                false,
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
                    DataType::ValueType::VARIANT, TR("matrix")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::numberColumns"),
                QString(),
                VariableName("NumberColumns"),
                false,
                TR("ncols"),
                TR("Determine the number of columns in a matrix"),
                TR("Matrix"),
                TR("ncols"),
                false,
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
                    DataType::ValueType::VARIANT, TR("matrix")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::identityBoolean"),
                QString(),
                VariableName("I", "B"),
                false,
                TR("idbool"),
                TR("Generates a boolean identity matrix"),
                TR("Matrix"),
                TR("idbool"),
                false,
                FunctionVariant(DataType::ValueType::MATRIX_BOOLEAN, DataType::ValueType::BOOLEAN, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_BOOLEAN, DataType::ValueType::INTEGER, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_BOOLEAN, DataType::ValueType::REAL, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_BOOLEAN, DataType::ValueType::COMPLEX, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_BOOLEAN, DataType::ValueType::VARIANT, TR("side")),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::zeroBoolean"),
                QString(),
                VariableName("Z", "B"),
                false,
                TR("zbool"),
                TR("Generates a boolean zero matrix (all values false)"),
                TR("Matrix"),
                TR("zbool"),
                false,
                FunctionVariant(DataType::ValueType::MATRIX_BOOLEAN, DataType::ValueType::BOOLEAN, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_BOOLEAN, DataType::ValueType::INTEGER, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_BOOLEAN, DataType::ValueType::REAL, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_BOOLEAN, DataType::ValueType::COMPLEX, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_BOOLEAN, DataType::ValueType::VARIANT, TR("side")),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::oneBoolean"),
                QString(),
                VariableName("OnesMatrix", "B"),
                false,
                TR("obool"),
                TR("Generates a boolean matrix with all true values"),
                TR("Matrix"),
                TR("obool"),
                false,
                FunctionVariant(DataType::ValueType::MATRIX_BOOLEAN, DataType::ValueType::BOOLEAN, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_BOOLEAN, DataType::ValueType::INTEGER, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_BOOLEAN, DataType::ValueType::REAL, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_BOOLEAN, DataType::ValueType::COMPLEX, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_BOOLEAN, DataType::ValueType::VARIANT, TR("side")),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::identityInteger"),
                QString(),
                VariableName("I", "Z"),
                false,
                TR("idint"),
                TR("Generates an integer identity matrix"),
                TR("Matrix"),
                TR("idint"),
                false,
                FunctionVariant(DataType::ValueType::MATRIX_INTEGER, DataType::ValueType::BOOLEAN, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_INTEGER, DataType::ValueType::INTEGER, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_INTEGER, DataType::ValueType::REAL, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_INTEGER, DataType::ValueType::COMPLEX, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_INTEGER, DataType::ValueType::VARIANT, TR("side")),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::zeroInteger"),
                QString(),
                VariableName("Z", "Z"),
                false,
                TR("zint"),
                TR("Generates an integer zero matrix"),
                TR("Matrix"),
                TR("zint"),
                false,
                FunctionVariant(DataType::ValueType::MATRIX_INTEGER, DataType::ValueType::BOOLEAN, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_INTEGER, DataType::ValueType::INTEGER, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_INTEGER, DataType::ValueType::REAL, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_INTEGER, DataType::ValueType::COMPLEX, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_INTEGER, DataType::ValueType::VARIANT, TR("side")),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::oneInteger"),
                QString(),
                VariableName("OnesMatrix", "Z"),
                false,
                TR("oint"),
                TR("Generates an integer matrix containing all 1's values"),
                TR("Matrix"),
                TR("oint"),
                false,
                FunctionVariant(DataType::ValueType::MATRIX_INTEGER, DataType::ValueType::BOOLEAN, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_INTEGER, DataType::ValueType::INTEGER, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_INTEGER, DataType::ValueType::REAL, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_INTEGER, DataType::ValueType::COMPLEX, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_INTEGER, DataType::ValueType::VARIANT, TR("side")),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::identityReal"),
                QString(),
                VariableName("I", "R"),
                false,
                TR("idreal"),
                TR("Generates a real identity matrix"),
                TR("Matrix"),
                TR("idreal"),
                false,
                FunctionVariant(DataType::ValueType::MATRIX_REAL, DataType::ValueType::BOOLEAN, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_REAL, DataType::ValueType::INTEGER, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_REAL, DataType::ValueType::REAL, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_REAL, DataType::ValueType::COMPLEX, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_REAL, DataType::ValueType::VARIANT, TR("side")),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::zeroReal"),
                QString(),
                VariableName("Z", "R"),
                false,
                TR("zreal"),
                TR("Generates a real zero matrix"),
                TR("Matrix"),
                TR("zreal"),
                false,
                FunctionVariant(DataType::ValueType::MATRIX_REAL, DataType::ValueType::BOOLEAN, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_REAL, DataType::ValueType::INTEGER, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_REAL, DataType::ValueType::REAL, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_REAL, DataType::ValueType::COMPLEX, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_REAL, DataType::ValueType::VARIANT, TR("side")),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::oneReal"),
                QString(),
                VariableName("OnesMatrix", "R"),
                false,
                TR("oreal"),
                TR("Generates a real matrix containing all 1's values"),
                TR("Matrix"),
                TR("oreal"),
                false,
                FunctionVariant(DataType::ValueType::MATRIX_REAL, DataType::ValueType::BOOLEAN, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_REAL, DataType::ValueType::INTEGER, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_REAL, DataType::ValueType::REAL, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_REAL, DataType::ValueType::COMPLEX, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_REAL, DataType::ValueType::VARIANT, TR("side")),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::identityComplex"),
                QString(),
                VariableName("I", "C"),
                false,
                TR("idcomp"),
                TR("Generates a complex identity matrix"),
                TR("Matrix"),
                TR("idcomp"),
                false,
                FunctionVariant(DataType::ValueType::MATRIX_COMPLEX, DataType::ValueType::BOOLEAN, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_COMPLEX, DataType::ValueType::INTEGER, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_COMPLEX, DataType::ValueType::REAL, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_COMPLEX, DataType::ValueType::COMPLEX, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_COMPLEX, DataType::ValueType::VARIANT, TR("side")),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::zeroComplex"),
                QString(),
                VariableName("Z", "C"),
                false,
                TR("zcomp"),
                TR("Generates a complex zero matrix"),
                TR("Matrix"),
                TR("zcomp"),
                false,
                FunctionVariant(DataType::ValueType::MATRIX_COMPLEX, DataType::ValueType::BOOLEAN, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_COMPLEX, DataType::ValueType::INTEGER, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_COMPLEX, DataType::ValueType::REAL, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_COMPLEX, DataType::ValueType::COMPLEX, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_COMPLEX, DataType::ValueType::VARIANT, TR("side")),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::oneComplex"),
                QString(),
                VariableName("OnesMatrix", "C"),
                false,
                TR("ocomp"),
                TR("Generates a complex matrix containing all 1's values"),
                TR("Matrix"),
                TR("ocomp"),
                false,
                FunctionVariant(DataType::ValueType::MATRIX_COMPLEX, DataType::ValueType::BOOLEAN, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_COMPLEX, DataType::ValueType::INTEGER, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_COMPLEX, DataType::ValueType::REAL, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_COMPLEX, DataType::ValueType::COMPLEX, TR("side")),
                FunctionVariant(DataType::ValueType::MATRIX_COMPLEX, DataType::ValueType::VARIANT, TR("side")),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::BOOLEAN, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::INTEGER, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::REAL, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::COMPLEX, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::BOOLEAN, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::INTEGER, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::REAL, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::COMPLEX, TR("columns")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::VARIANT, TR("rows"),
                    DataType::ValueType::VARIANT, TR("columns")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::diagonalEntries"),
                QString(),
                VariableName("DiagonalEntries"),
                false,
                TR("diagent"),
                TR("Returns a column matrix holding the diagonal entries"),
                TR("Matrix"),
                TR("diagent"),
                false,
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN,
                    TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::MATRIX_INTEGER,
                    TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_REAL,
                    TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_COMPLEX,
                    TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::VARIANT,
                    DataType::ValueType::VARIANT,
                    TR("matrix")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::diagonal"),
                QString(),
                VariableName("Diagonal"),
                false,
                TR("diagonal"),
                TR("Returns a diagonal matrix using the supplied values"),
                TR("Matrix"),
                TR("diagonal"),
                false,
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN,
                    TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::MATRIX_INTEGER,
                    TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_REAL,
                    TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_COMPLEX,
                    TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::VARIANT,
                    DataType::ValueType::VARIANT,
                    TR("matrix")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::columnReverse"),
                QString(),
                VariableName(TR("FlipLR")),
                false,
                TR("fliplr"),
                TR("Flip a matrix from left to right, reversing column order"),
                TR("Matrix"),
                TR("fliplr"),
                false,
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::VARIANT,
                    DataType::ValueType::VARIANT, TR("matrix")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::rowReverse"),
                QString(),
                VariableName(TR("FlipUD")),
                false,
                TR("flipud"),
                TR("Flip a matrix from to to bottom, reversing row order"),
                TR("Matrix"),
                TR("flipud"),
                false,
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::VARIANT,
                    DataType::ValueType::VARIANT, TR("matrix")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::isSymmetric"),
                QString(),
                VariableName(TR("IsSymmetric")),
                false,
                TR("issymmetric"),
                TR("Returns true if a matrix is symmetric"),
                TR("Matrix"),
                TR("issymmetric"),
                false,
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::VARIANT, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::REAL, TR("relative tolerance")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::REAL, TR("relative tolerance")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::REAL, TR("relative tolerance")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix"),
                    DataType::ValueType::REAL, TR("relative tolerance")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::VARIANT, TR("matrix"),
                    DataType::ValueType::REAL, TR("relative tolerance")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::isHermitian"),
                QString(),
                VariableName(TR("IsHermitian")),
                false,
                TR("ishermitian"),
                TR("Returns true if a matrix is Hermitian"),
                TR("Matrix"),
                TR("ishermitian"),
                false,
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::VARIANT, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::REAL, TR("relative tolerance")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::REAL, TR("relative tolerance")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::REAL, TR("relative tolerance")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix"),
                    DataType::ValueType::REAL, TR("relative tolerance")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::VARIANT, TR("matrix"),
                    DataType::ValueType::REAL, TR("relative tolerance")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::isSkewSymmetric"),
                QString(),
                VariableName(TR("IsSkewSymmetric")),
                false,
                TR("isssymmetric"),
                TR("Returns true if a matrix is skew symmetric"),
                TR("Matrix"),
                TR("isssymmetric"),
                false,
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::VARIANT, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::REAL, TR("relative tolerance")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::REAL, TR("relative tolerance")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::REAL, TR("relative tolerance")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix"),
                    DataType::ValueType::REAL, TR("relative tolerance")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::VARIANT, TR("matrix"),
                    DataType::ValueType::REAL, TR("relative tolerance")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::isSkewHermitian"),
                QString(),
                VariableName(TR("IsSkewHermitian")),
                false,
                TR("isshermitian"),
                TR("Returns true if a matrix is skew Hermitian"),
                TR("Matrix"),
                TR("isshermitian"),
                false,
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::VARIANT, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::REAL, TR("relative tolerance")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::REAL, TR("relative tolerance")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::REAL, TR("relative tolerance")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix"),
                    DataType::ValueType::REAL, TR("relative tolerance")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::VARIANT, TR("matrix"),
                    DataType::ValueType::REAL, TR("relative tolerance")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::isNormal"),
                QString(),
                VariableName(TR("IsNormal")),
                false,
                TR("isnormal"),
                TR("Returns true if a matrix is normal"),
                TR("Matrix"),
                TR("isnormal"),
                false,
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::VARIANT, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::REAL, TR("relative tolerance")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::REAL, TR("relative tolerance")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::REAL, TR("relative tolerance")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix"),
                    DataType::ValueType::REAL, TR("relative tolerance")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::VARIANT, TR("matrix"),
                    DataType::ValueType::REAL, TR("relative tolerance")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::rank"),
                QString(),
                VariableName(TR("Rank")),
                false,
                TR("rank"),
                TR("Returns the matrix rank to within a given tolerance"),
                TR("Matrix"),
                TR("rank"),
                false,
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::REAL, TR("tolerance")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::REAL, TR("tolerance")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::REAL, TR("tolerance")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix"),
                    DataType::ValueType::REAL, TR("tolerance")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::VARIANT, TR("matrix"),
                    DataType::ValueType::REAL, TR("tolerance")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::plu"),
                QString(),
                VariableName(TR("PLU")),
                false,
                TR("plu"),
                TR("Performs PLU decomposition"),
                TR("Matrix"),
                TR("plu"),
                false,
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("matrix")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::svd"),
                QString(),
                VariableName(TR("SVD")),
                false,
                TR("svd"),
                TR("Performs singular value decomposition"),
                TR("Matrix"),
                TR("svd"),
                false,
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("matrix")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::qr"),
                QString(),
                VariableName(TR("QR")),
                false,
                TR("qr"),
                TR("Performs QR decomposition"),
                TR("Matrix"),
                TR("qr"),
                false,
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("matrix")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::lq"),
                QString(),
                VariableName(TR("LQ")),
                false,
                TR("lq"),
                TR("Performs LQ decomposition"),
                TR("Matrix"),
                TR("lq"),
                false,
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("matrix")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::cholesky"),
                QString(),
                VariableName(TR("Cholesky")),
                false,
                TR("cholesky"),
                TR("Performs Cholesky decomposition (creating L%1L%2)").arg(QChar(0x00D7)).arg(QChar(0x1D34)),
                TR("Matrix"),
                TR("cholesky"),
                false,
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::VARIANT,
                    DataType::ValueType::VARIANT, TR("matrix")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::upperCholesky"),
                QString(),
                VariableName(TR("UpperCholesky")),
                false,
                TR("ucholesky"),
                TR("Performs upper Cholesky decomposition (creating U%1U%2)").arg(QChar(0x00D7)).arg(QChar(0x1D34)),
                TR("Matrix"),
                TR("ucholesky"),
                false,
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::VARIANT,
                    DataType::ValueType::VARIANT, TR("matrix")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::hessenberg"),
                QString(),
                VariableName(TR("Hessenberg")),
                false,
                TR("hessenberg"),
                TR("Performs upper Hessenberg decomposition"),
                TR("Matrix"),
                TR("hessenberg"),
                false,
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("matrix")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::schur"),
                QString(),
                VariableName(TR("Schur")),
                false,
                TR("schur"),
                TR("Performs Schur decomposition"),
                TR("Matrix"),
                TR("schur"),
                false,
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("matrix")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::eigenvectors"),
                QString(),
                VariableName(TR("Eigenvectors")),
                false,
                TR("eigenvectors"),
                TR("Calculates eigenvectors, eigenvalues, and Schur decomposition"),
                TR("Matrix"),
                TR("eigenvectors"),
                false,
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("matrix")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::dft"),
                QString(),
                VariableName(TR("DFT")),
                false,
                TR("dft"),
                TR("Calculates the DFT in one or two dimensions"),
                TR("Matrix"),
                TR("dft"),
                false,
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::VARIANT, TR("matrix")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::idft"),
                QString(),
                VariableName(TR("InverseDFT")),
                false,
                TR("idft"),
                TR("Calculates the inverse DFT in one or two dimensions"),
                TR("Matrix"),
                TR("idft"),
                false,
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::VARIANT, TR("matrix")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::dct"),
                QString(),
                VariableName(TR("DCT"), TR("2")),
                false,
                TR("dct2"),
                TR("Calculates the DCT, type 2, in one or two dimensions"),
                TR("Matrix"),
                TR("dct2"),
                false,
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::VARIANT, TR("matrix")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::idct"),
                QString(),
                VariableName(TR("DCT"),TR("3")),
                false,
                TR("dct3"),
                TR("Calculates the DCT, type 3, in one or two dimensions"),
                TR("Matrix"),
                TR("dct3"),
                false,
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::VARIANT, TR("matrix")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::hilbertTransform"),
                QString(),
                VariableName(TR("HilbertTransform")),
                false,
                TR("hilbert"),
                TR("Calculates the Hilbert transform of a row/column matrix"),
                TR("Matrix"),
                TR("hilbert"),
                false,
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::VARIANT, TR("matrix")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::conditionNumber"),
                QString(),
                VariableName(TR("ConditionNumber")),
                false,
                TR("condnum"),
                TR("Calculates the approximate condition number of a matrix"),
                TR("Matrix"),
                TR("condnum"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::VARIANT, TR("matrix")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::pNorm"),
                QString(),
                VariableName(TR("pNorm")),
                false,
                TR("pnorm"),
                TR("Calculates the p-norm of a matrix"),
                TR("Matrix"),
                TR("pnorm"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::BOOLEAN, TR("p")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::BOOLEAN, TR("p")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::BOOLEAN, TR("p")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix"),
                    DataType::ValueType::BOOLEAN, TR("p")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::VARIANT, TR("matrix"),
                    DataType::ValueType::BOOLEAN, TR("p")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::INTEGER, TR("p")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::INTEGER, TR("p")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::INTEGER, TR("p")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix"),
                    DataType::ValueType::INTEGER, TR("p")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::VARIANT, TR("matrix"),
                    DataType::ValueType::INTEGER, TR("p")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::REAL, TR("p")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::REAL, TR("p")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::REAL, TR("p")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix"),
                    DataType::ValueType::REAL, TR("p")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::VARIANT, TR("matrix"),
                    DataType::ValueType::REAL, TR("p")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::COMPLEX, TR("p")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::COMPLEX, TR("p")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::COMPLEX, TR("p")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix"),
                    DataType::ValueType::COMPLEX, TR("p")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::VARIANT, TR("matrix"),
                    DataType::ValueType::COMPLEX, TR("p")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::VARIANT, TR("p")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::VARIANT, TR("p")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::VARIANT, TR("p")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix"),
                    DataType::ValueType::VARIANT, TR("p")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::VARIANT, TR("matrix"),
                    DataType::ValueType::VARIANT, TR("p")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::euclideanNorm"),
                QString(),
                VariableName(TR("EuclideanNorm")),
                false,
                TR("enorm"),
                TR("Calculates the Euclidean norm of a matrix"),
                TR("Matrix"),
                TR("enorm"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::VARIANT, TR("matrix")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::oneNorm"),
                QString(),
                VariableName(TR("OneNorm")),
                false,
                TR("onorm"),
                TR("Calculates the one-norm of a matrix"),
                TR("Matrix"),
                TR("onorm"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::VARIANT, TR("matrix")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::infinityNorm"),
                QString(),
                VariableName(TR("InfinityNorm")),
                false,
                TR("inorm"),
                TR("Calculates the infinity-norm of a matrix"),
                TR("Matrix"),
                TR("inorm"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::VARIANT, TR("matrix")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::equilibrate"),
                QString(),
                VariableName(TR("Equilibrate")),
                false,
                TR("equilibrate"),
                TR("Equilibrates a matrix"),
                TR("Matrix"),
                TR("equilibrate"),
                false,
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::VARIANT, TR("matrix")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::solve"),
                QString(),
                VariableName(TR("Solve")),
                false,
                TR("solve"),
                TR("Solves Ax = y for x given matrix A and y"),
                TR("Matrix"),
                TR("solve"),
                false,
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("A"),
                    DataType::ValueType::MATRIX_BOOLEAN, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("A"),
                    DataType::ValueType::MATRIX_INTEGER, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("A"),
                    DataType::ValueType::MATRIX_REAL, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("A"),
                    DataType::ValueType::MATRIX_COMPLEX, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::VARIANT,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("A"),
                    DataType::ValueType::VARIANT, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_INTEGER, TR("A"),
                    DataType::ValueType::MATRIX_BOOLEAN, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_INTEGER, TR("A"),
                    DataType::ValueType::MATRIX_INTEGER, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_INTEGER, TR("A"),
                    DataType::ValueType::MATRIX_REAL, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_INTEGER, TR("A"),
                    DataType::ValueType::MATRIX_COMPLEX, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::VARIANT,
                    DataType::ValueType::MATRIX_INTEGER, TR("A"),
                    DataType::ValueType::VARIANT, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_REAL, TR("A"),
                    DataType::ValueType::MATRIX_BOOLEAN, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_REAL, TR("A"),
                    DataType::ValueType::MATRIX_INTEGER, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_REAL, TR("A"),
                    DataType::ValueType::MATRIX_REAL, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_REAL, TR("A"),
                    DataType::ValueType::MATRIX_COMPLEX, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::VARIANT,
                    DataType::ValueType::MATRIX_REAL, TR("A"),
                    DataType::ValueType::VARIANT, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_REAL, TR("A"),
                    DataType::ValueType::MATRIX_BOOLEAN, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_REAL, TR("A"),
                    DataType::ValueType::MATRIX_INTEGER, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_REAL, TR("A"),
                    DataType::ValueType::MATRIX_REAL, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_REAL, TR("A"),
                    DataType::ValueType::MATRIX_COMPLEX, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::VARIANT,
                    DataType::ValueType::MATRIX_REAL, TR("A"),
                    DataType::ValueType::VARIANT, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_COMPLEX, TR("A"),
                    DataType::ValueType::MATRIX_BOOLEAN, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_COMPLEX, TR("A"),
                    DataType::ValueType::MATRIX_INTEGER, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_COMPLEX, TR("A"),
                    DataType::ValueType::MATRIX_REAL, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_COMPLEX, TR("A"),
                    DataType::ValueType::MATRIX_COMPLEX, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_COMPLEX, TR("A"),
                    DataType::ValueType::VARIANT, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::VARIANT,
                    DataType::ValueType::VARIANT, TR("A"),
                    DataType::ValueType::MATRIX_BOOLEAN, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::VARIANT,
                    DataType::ValueType::VARIANT, TR("A"),
                    DataType::ValueType::MATRIX_INTEGER, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::VARIANT,
                    DataType::ValueType::VARIANT, TR("A"),
                    DataType::ValueType::MATRIX_REAL, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::VARIANT, TR("A"),
                    DataType::ValueType::MATRIX_COMPLEX, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::VARIANT,
                    DataType::ValueType::VARIANT, TR("A"),
                    DataType::ValueType::VARIANT, TR("y")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::leastSquares"),
                QString(),
                VariableName(TR("LeastSquares")),
                false,
                TR("leastsq"),
                TR("Determines least squares solutions to Ax = y when A or y is over or under determined"),
                TR("Matrix"),
                TR("leastsq"),
                false,
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("A"),
                    DataType::ValueType::MATRIX_BOOLEAN, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("A"),
                    DataType::ValueType::MATRIX_INTEGER, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("A"),
                    DataType::ValueType::MATRIX_REAL, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("A"),
                    DataType::ValueType::MATRIX_COMPLEX, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::VARIANT,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("A"),
                    DataType::ValueType::VARIANT, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_INTEGER, TR("A"),
                    DataType::ValueType::MATRIX_BOOLEAN, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_INTEGER, TR("A"),
                    DataType::ValueType::MATRIX_INTEGER, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_INTEGER, TR("A"),
                    DataType::ValueType::MATRIX_REAL, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_INTEGER, TR("A"),
                    DataType::ValueType::MATRIX_COMPLEX, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::VARIANT,
                    DataType::ValueType::MATRIX_INTEGER, TR("A"),
                    DataType::ValueType::VARIANT, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_REAL, TR("A"),
                    DataType::ValueType::MATRIX_BOOLEAN, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_REAL, TR("A"),
                    DataType::ValueType::MATRIX_INTEGER, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_REAL, TR("A"),
                    DataType::ValueType::MATRIX_REAL, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_REAL, TR("A"),
                    DataType::ValueType::MATRIX_COMPLEX, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::VARIANT,
                    DataType::ValueType::MATRIX_REAL, TR("A"),
                    DataType::ValueType::VARIANT, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_REAL, TR("A"),
                    DataType::ValueType::MATRIX_BOOLEAN, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_REAL, TR("A"),
                    DataType::ValueType::MATRIX_INTEGER, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_REAL, TR("A"),
                    DataType::ValueType::MATRIX_REAL, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_REAL, TR("A"),
                    DataType::ValueType::MATRIX_COMPLEX, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::VARIANT,
                    DataType::ValueType::MATRIX_REAL, TR("A"),
                    DataType::ValueType::VARIANT, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_COMPLEX, TR("A"),
                    DataType::ValueType::MATRIX_BOOLEAN, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_COMPLEX, TR("A"),
                    DataType::ValueType::MATRIX_INTEGER, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_COMPLEX, TR("A"),
                    DataType::ValueType::MATRIX_REAL, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_COMPLEX, TR("A"),
                    DataType::ValueType::MATRIX_COMPLEX, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_COMPLEX, TR("A"),
                    DataType::ValueType::VARIANT, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::VARIANT,
                    DataType::ValueType::VARIANT, TR("A"),
                    DataType::ValueType::MATRIX_BOOLEAN, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::VARIANT,
                    DataType::ValueType::VARIANT, TR("A"),
                    DataType::ValueType::MATRIX_INTEGER, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::VARIANT,
                    DataType::ValueType::VARIANT, TR("A"),
                    DataType::ValueType::MATRIX_REAL, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::VARIANT, TR("A"),
                    DataType::ValueType::MATRIX_COMPLEX, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::VARIANT,
                    DataType::ValueType::VARIANT, TR("A"),
                    DataType::ValueType::VARIANT, TR("y")
                )
            )
        );
    }
}
