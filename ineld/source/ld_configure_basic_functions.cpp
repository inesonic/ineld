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
* This header provides a single function that can be called configure basic functions.
***********************************************************************************************************************/

#include <QCoreApplication>

#include "ld_data_type.h"
#include "ld_variable_name.h"
#include "ld_function_data.h"
#include "ld_function_database.h"
#include "ld_cpp_code_generator.h"
#include "ld_cpp_ln_function_translation_engine.h"
#include "ld_cpp_log_function_translation_engine.h"
#include "ld_configure_helpers.h"

#define TR(_x) QCoreApplication::translate("ConfigureBasicFunctions", (_x))

namespace Ld {
    void configureBasicFunctions() {
        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::real"),
                QString(),
                VariableName(TR("Re")),
                false,
                TR("re"),
                TR("Real portion of a value.  Also supports matrices."),
                TR("Basic"),
                TR("re"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::COMPLEX, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::INTEGER, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::BOOLEAN, TR("value")
                ),
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
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::VARIANT,
                    DataType::ValueType::VARIANT, TR("value")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::imag"),
                QString(),
                VariableName(TR("Im")),
                false,
                TR("im"),
                TR("Imaginary portion of a value.  Also supports matrices."),
                TR("Basic"),
                TR("im"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::COMPLEX, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::INTEGER, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::BOOLEAN, TR("value")
                ),
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
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::VARIANT,
                    DataType::ValueType::VARIANT, TR("value")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::systemTime"),
                QString(),
                VariableName(TR("SystemTime")),
                false,
                TR("systemtime"),
                TR("Determines the system time in mSec."),
                TR("Special"),
                TR("systemtime"),
                false,
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::BOOLEAN, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::INTEGER, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::REAL, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::COMPLEX, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::SET, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::TUPLE, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::MATRIX_BOOLEAN, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::MATRIX_INTEGER, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::MATRIX_REAL, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::MATRIX_COMPLEX, TR("any value"))
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::isNaN"),
                QString(),
                VariableName(TR("IsNaN")),
                false,
                TR("isnan"),
                TR("Determines if a value is NaN."),
                TR("Basic"),
                TR("isnan"),
                false,
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::BOOLEAN, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::VARIANT, TR("value")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::sine"),
                QString(),
                VariableName(TR("sin")),
                false,
                TR("sin"),
                TR("Sine function"),
                TR("Basic"),
                TR("sin"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("angle")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("angle")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("angle")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("angle")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("angle")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::cosine"),
                QString(),
                VariableName(TR("cos")),
                false,
                TR("cos"),
                TR("Cosine function"),
                TR("Basic"),
                TR("cos"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("angle")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("angle")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("angle")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("angle")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("angle")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::tangent"),
                QString(),
                VariableName(TR("tan")),
                false,
                TR("tan"),
                TR("Tangent function"),
                TR("Basic"),
                TR("tan"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("angle")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("angle")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("angle")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("angle")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("angle")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::secant"),
                QString(),
                VariableName(TR("sec")),
                false,
                TR("sec"),
                TR("Secant function"),
                TR("Basic"),
                TR("sec"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("angle")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("angle")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("angle")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("angle")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("angle")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::cosecant"),
                QString(),
                VariableName(TR("csc")),
                false,
                TR("csc"),
                TR("Cosecant function"),
                TR("Basic"),
                TR("csc"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("angle")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("angle")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("angle")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("angle")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("angle")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::cotangent"),
                QString(),
                VariableName(TR("cot")),
                false,
                TR("cot"),
                TR("Cotangent function"),
                TR("Basic"),
                TR("cot"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("angle")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("angle")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("angle")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("angle")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("angle")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::arcsine"),
                QString(),
                VariableName(TR("arcsin")),
                false,
                TR("arcsin"),
                TR("Arcsine function"),
                TR("Basic"),
                TR("arcsin"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("value")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::arccosine"),
                QString(),
                VariableName(TR("arccos")),
                false,
                TR("arccos"),
                TR("Arccosine function"),
                TR("Basic"),
                TR("arccos"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("value")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::arctangent"),
                QString(),
                VariableName(TR("arctan")),
                false,
                TR("arctan"),
                TR("Arctangent function"),
                TR("Basic"),
                TR("arctan"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("value")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::arcsecant"),
                QString(),
                VariableName(TR("arcsec")),
                false,
                TR("arcsec"),
                TR("Arcsecant function"),
                TR("Basic"),
                TR("arcsec"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("value")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::arccosecant"),
                QString(),
                VariableName(TR("arccsc")),
                false,
                TR("arccsc"),
                TR("Arccosecant function"),
                TR("Basic"),
                TR("arccsc"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("value")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::arccotangent"),
                QString(),
                VariableName(TR("arccot")),
                false,
                TR("arccot"),
                TR("Arccotangent function"),
                TR("Basic"),
                TR("arccot"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("value")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::hyperbolicSine"),
                QString(),
                VariableName(TR("sinh")),
                false,
                TR("sinh"),
                TR("Hyperbolic sine function"),
                TR("Basic"),
                TR("sinh"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("hyperbolic angle")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("hyperbolic angle")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("hyperbolic angle")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("hyperbolic angle")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("hyperbolic angle")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::hyperbolicCosine"),
                QString(),
                VariableName(TR("cosh")),
                false,
                TR("cosh"),
                TR("Hyperbolic cosine function"),
                TR("Basic"),
                TR("cosh"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("hyperbolic angle")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("hyperbolic angle")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("hyperbolic angle")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("hyperbolic angle")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("hyperbolic angle")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::hyperbolicTangent"),
                QString(),
                VariableName(TR("tanh")),
                false,
                TR("tanh"),
                TR("Hyperbolic tangent function"),
                TR("Basic"),
                TR("tanh"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("hyperbolic angle")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("hyperbolic angle")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("hyperbolic angle")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("hyperbolic angle")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("hyperbolic angle")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::hyperbolicSecant"),
                QString(),
                VariableName(TR("sech")),
                false,
                TR("sech"),
                TR("Hyperbolic secant function"),
                TR("Basic"),
                TR("sech"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("hyperbolic angle")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("hyperbolic angle")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("hyperbolic angle")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("hyperbolic angle")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("hyperbolic angle")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::hyperbolicCosecant"),
                QString(),
                VariableName(TR("csch")),
                false,
                TR("csch"),
                TR("Hyperbolic cosecant function"),
                TR("Basic"),
                TR("csch"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("hyperbolic angle")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("hyperbolic angle")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("hyperbolic angle")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("hyperbolic angle")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("hyperbolic angle")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::hyperbolicCotangent"),
                QString(),
                VariableName(TR("coth")),
                false,
                TR("coth"),
                TR("Hyperbolic cotangent function"),
                TR("Basic"),
                TR("coth"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("hyperbolic angle")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("hyperbolic angle")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("hyperbolic angle")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("hyperbolic angle")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("hyperbolic angle")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::hyperbolicArsine"),
                QString(),
                VariableName(TR("arsinh")),
                false,
                TR("arsinh"),
                TR("Area hyperbolic sine function"),
                TR("Basic"),
                TR("arsinh"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("value")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::hyperbolicArcosine"),
                QString(),
                VariableName(TR("arcosh")),
                false,
                TR("arcosh"),
                TR("Area hyperbolic cosine function"),
                TR("Basic"),
                TR("arcosh"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("value")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::hyperbolicArtangent"),
                QString(),
                VariableName(TR("artanh")),
                false,
                TR("artanh"),
                TR("Area hyperbolic tangent function"),
                TR("Basic"),
                TR("artanh"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("value")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::hyperbolicArsecant"),
                QString(),
                VariableName(TR("arsech")),
                false,
                TR("arsech"),
                TR("Area hyperbolic secant function"),
                TR("Basic"),
                TR("arsech"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("value")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::hyperbolicArcosecant"),
                QString(),
                VariableName(TR("arcsch")),
                false,
                TR("arcsch"),
                TR("Area hyperbolic cosecant function"),
                TR("Basic"),
                TR("arcsch"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("value")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::hyperbolicArcotangent"),
                QString(),
                VariableName(TR("arcoth")),
                false,
                TR("arcoth"),
                TR("Area hyperbolic cotangent function"),
                TR("Basic"),
                TR("arcoth"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("value")
                )
            )
        );

        FunctionData lnFunctionData = FunctionData(
            FunctionData::Type::BUILT_IN,
            QString("M::ln"),
            QString(),
            VariableName(TR("ln")),
            false,
            TR("ln"),
            TR("Natural log function"),
            TR("Basic"),
            TR("ln"),
            false,
            FunctionVariant(
                DataType::ValueType::COMPLEX,
                DataType::ValueType::INTEGER, TR("value")
            ),
            FunctionVariant(
                DataType::ValueType::COMPLEX,
                DataType::ValueType::REAL, TR("value")
            ),
            FunctionVariant(
                DataType::ValueType::COMPLEX,
                DataType::ValueType::COMPLEX, TR("value")
            )
        );
        lnFunctionData.registerTranslator(
            CppCodeGenerator::codeGeneratorName,
            &CppLnFunctionTranslationEngine::instance
        );
        FunctionDatabase::registerFunction(lnFunctionData);

        FunctionData logFunctionData = FunctionData(
            FunctionData::Type::BUILT_IN,
            QString("M::log"),
            QString(),
            VariableName(TR("log")),
            true,
            TR("log"),
            TR("Log base b function"),
            TR("Basic"),
            TR("log"),
            false,
            FunctionVariant(
                DataType::ValueType::COMPLEX,
                DataType::ValueType::INTEGER, TR("base"),
                DataType::ValueType::INTEGER, TR("value")
            ),
            FunctionVariant(
                DataType::ValueType::COMPLEX,
                DataType::ValueType::INTEGER, TR("base"),
                DataType::ValueType::REAL, TR("value")
            ),
            FunctionVariant(
                DataType::ValueType::COMPLEX,
                DataType::ValueType::INTEGER, TR("base"),
                DataType::ValueType::COMPLEX, TR("value")
            ),
            FunctionVariant(
                DataType::ValueType::COMPLEX,
                DataType::ValueType::REAL, TR("base"),
                DataType::ValueType::INTEGER, TR("value")
            ),
            FunctionVariant(
                DataType::ValueType::COMPLEX,
                DataType::ValueType::REAL, TR("base"),
                DataType::ValueType::REAL, TR("value")
            ),
            FunctionVariant(
                DataType::ValueType::COMPLEX,
                DataType::ValueType::REAL, TR("base"),
                DataType::ValueType::COMPLEX, TR("value")
            ),
            FunctionVariant(
                DataType::ValueType::COMPLEX,
                DataType::ValueType::COMPLEX, TR("base"),
                DataType::ValueType::INTEGER, TR("value")
            ),
            FunctionVariant(
                DataType::ValueType::COMPLEX,
                DataType::ValueType::COMPLEX, TR("base"),
                DataType::ValueType::REAL, TR("value")
            ),
            FunctionVariant(
                DataType::ValueType::COMPLEX,
                DataType::ValueType::COMPLEX, TR("base"),
                DataType::ValueType::COMPLEX, TR("value")
            ),
            FunctionVariant(
                DataType::ValueType::COMPLEX,
                DataType::ValueType::VARIANT, TR("base"),
                DataType::ValueType::INTEGER, TR("value")
            ),
            FunctionVariant(
                DataType::ValueType::COMPLEX,
                DataType::ValueType::VARIANT, TR("base"),
                DataType::ValueType::REAL, TR("value")
            ),
            FunctionVariant(
                DataType::ValueType::COMPLEX,
                DataType::ValueType::VARIANT, TR("base"),
                DataType::ValueType::COMPLEX, TR("value")
            ),
            FunctionVariant(
                DataType::ValueType::COMPLEX,
                DataType::ValueType::INTEGER, TR("base"),
                DataType::ValueType::VARIANT, TR("value")
            ),
            FunctionVariant(
                DataType::ValueType::COMPLEX,
                DataType::ValueType::REAL, TR("base"),
                DataType::ValueType::VARIANT, TR("value")
            ),
            FunctionVariant(
                DataType::ValueType::COMPLEX,
                DataType::ValueType::COMPLEX, TR("base"),
                DataType::ValueType::VARIANT, TR("value")
            ),
            FunctionVariant(
                DataType::ValueType::COMPLEX,
                DataType::ValueType::VARIANT, TR("base"),
                DataType::ValueType::VARIANT, TR("value")
            )
        );

        logFunctionData.registerTranslator(
            CppCodeGenerator::codeGeneratorName,
            &CppLogFunctionTranslationEngine::instance
        );
        FunctionDatabase::registerFunction(logFunctionData);
    }
}
