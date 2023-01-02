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
* This file implements a single function that can be called configure special functions.
***********************************************************************************************************************/

#include <QCoreApplication>

#include "ld_configure_helpers.h"
#include "ld_data_type.h"
#include "ld_variable_name.h"
#include "ld_function_data.h"
#include "ld_function_database.h"
#include "ld_configure_special_functions.h"

#define TR(_x) QCoreApplication::translate("ConfigureSpecialFunctions", (_x))

namespace Ld {
    void configureSpecialFunctions() {
        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::gamma"),
                QString(),
                VariableName(TR("%1").arg(QChar(0x0393))),
                false,
                TR("gammafn"),
                TR("Gamma function"),
                TR("Special"),
                TR("gammafn"),
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
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("s"),
                    DataType::ValueType::BOOLEAN, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("s"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::BOOLEAN, TR("s"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::BOOLEAN, TR("s"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::BOOLEAN, TR("s"),
                    DataType::ValueType::VARIANT, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::BOOLEAN, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::VARIANT, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::BOOLEAN, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::VARIANT, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::BOOLEAN, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::VARIANT, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::BOOLEAN, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::VARIANT, TR("z")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::lnFactorial"),
                QString(),
                VariableName(TR("lnFactorial")),
                false,
                TR("lnfactorial"),
                TR("Natural log of factorial function"),
                TR("Special"),
                TR("lnfactorial"),
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
                    DataType::ValueType::REAL,
                    DataType::ValueType::COMPLEX, TR("value")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::VARIANT, TR("value")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::lnGamma"),
                QString(),
                VariableName(TR("lnGamma")),
                false,
                TR("lngamma"),
                TR("Natural log of gamma function"),
                TR("Special"),
                TR("lngamma"),
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
                QString("M::unsignedSterlingNumber1"),
                QString(),
                VariableName(TR("s")),
                false,
                TR("sterling1fn"),
                TR("Unsigned sterling number of the first kind"),
                TR("Special"),
                TR("sterling1fn"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("n"),
                    DataType::ValueType::INTEGER, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("n"),
                    DataType::ValueType::REAL, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("n"),
                    DataType::ValueType::COMPLEX, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("n"),
                    DataType::ValueType::VARIANT, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("n"),
                    DataType::ValueType::INTEGER, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("n"),
                    DataType::ValueType::REAL, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("n"),
                    DataType::ValueType::COMPLEX, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("n"),
                    DataType::ValueType::VARIANT, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::COMPLEX, TR("n"),
                    DataType::ValueType::INTEGER, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::COMPLEX, TR("n"),
                    DataType::ValueType::REAL, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::COMPLEX, TR("n"),
                    DataType::ValueType::COMPLEX, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::COMPLEX, TR("n"),
                    DataType::ValueType::VARIANT, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::VARIANT, TR("n"),
                    DataType::ValueType::INTEGER, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::VARIANT, TR("n"),
                    DataType::ValueType::REAL, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::VARIANT, TR("n"),
                    DataType::ValueType::COMPLEX, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::VARIANT, TR("n"),
                    DataType::ValueType::VARIANT, TR("k")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::sterlingNumber2"),
                QString(),
                VariableName(TR("S")),
                false,
                TR("sterling2fn"),
                TR("Sterling number of the second kind"),
                TR("Special"),
                TR("sterling2fn"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("n"),
                    DataType::ValueType::INTEGER, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("n"),
                    DataType::ValueType::REAL, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("n"),
                    DataType::ValueType::COMPLEX, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("n"),
                    DataType::ValueType::VARIANT, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("n"),
                    DataType::ValueType::INTEGER, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("n"),
                    DataType::ValueType::REAL, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("n"),
                    DataType::ValueType::COMPLEX, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("n"),
                    DataType::ValueType::VARIANT, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::COMPLEX, TR("n"),
                    DataType::ValueType::INTEGER, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::COMPLEX, TR("n"),
                    DataType::ValueType::REAL, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::COMPLEX, TR("n"),
                    DataType::ValueType::COMPLEX, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::COMPLEX, TR("n"),
                    DataType::ValueType::VARIANT, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::VARIANT, TR("n"),
                    DataType::ValueType::INTEGER, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::VARIANT, TR("n"),
                    DataType::ValueType::REAL, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::VARIANT, TR("n"),
                    DataType::ValueType::COMPLEX, TR("k")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::VARIANT, TR("n"),
                    DataType::ValueType::VARIANT, TR("k")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::lowerGamma"),
                QString(),
                VariableName(TR("%1").arg(QChar(0x03B3))),
                false,
                TR("lowergamma"),
                TR("Lower incomplete gamma function"),
                TR("Special"),
                TR("lowergamma"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::VARIANT, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::VARIANT, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::VARIANT, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::VARIANT, TR("z")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::upperGamma"),
                QString(),
                VariableName(TR("%1").arg(QChar(0x0393)), TR("U")),
                false,
                TR("uppergamma"),
                TR("Upper incomplete gamma function"),
                TR("Special"),
                TR("uppergamma"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::VARIANT, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::VARIANT, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::VARIANT, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::VARIANT, TR("z")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::normalizedLowerGamma"),
                QString(),
                VariableName(TR("P")),
                false,
                TR("pgamma"),
                TR("Normalized lower gamma function"),
                TR("Special"),
                TR("pgamma"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::VARIANT, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::VARIANT, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::VARIANT, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::VARIANT, TR("z")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::normalizedUpperGamma"),
                QString(),
                VariableName(TR("Q")),
                false,
                TR("qgamma"),
                TR("Normalized upper gamma function"),
                TR("Special"),
                TR("qgamma"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::VARIANT, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::VARIANT, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::VARIANT, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::VARIANT, TR("z")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::inverseLowerGamma"),
                QString(),
                VariableName(TR("%1Inv").arg(QChar(0x03B3))),
                false,
                TR("lowergammainv"),
                TR("Inverse lower gamma"),
                TR("Special"),
                TR("lowergammainv"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::INTEGER, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::REAL, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::COMPLEX, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::VARIANT, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::INTEGER, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::REAL, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::COMPLEX, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::VARIANT, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::INTEGER, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::REAL, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::COMPLEX, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::VARIANT, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::INTEGER, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::REAL, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::COMPLEX, TR("y")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::VARIANT, TR("y")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::beta"),
                QString(),
                VariableName(TR("%1").arg(QChar(0x0392))),
                false,
                TR("betafn"),
                TR("Beta function"),
                TR("Special"),
                TR("betafn"),
                false,
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::INTEGER, TR("s"),
                    DataType::ValueType::VARIANT, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::REAL,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::REAL, TR("s"),
                    DataType::ValueType::VARIANT, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("s"),
                    DataType::ValueType::VARIANT, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("s"),
                    DataType::ValueType::VARIANT, TR("z")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::reimannZeta"),
                QString(),
                VariableName(TR("%1").arg(QChar(0x03B6))),
                false,
                TR("zetafn"),
                TR("Reimann Zeta function"),
                TR("Special"),
                TR("zetafn"),
                true,
                FunctionVariant(DataType::ValueType::REAL, DataType::ValueType::INTEGER, TR("s")),
                FunctionVariant(DataType::ValueType::REAL, DataType::ValueType::REAL, TR("s")),
                FunctionVariant(DataType::ValueType::COMPLEX, DataType::ValueType::COMPLEX, TR("s"))
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::erf"),
                QString(),
                VariableName(TR("erf")),
                false,
                TR("erf"),
                TR("Error function"),
                TR("Special"),
                TR("erf"),
                false,
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
                QString("M::erfInv"),
                QString(),
                VariableName(TR("erfInv")),
                false,
                TR("erfinv"),
                TR("Inverse error function"),
                TR("Special"),
                TR("erfinv"),
                false,
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
                QString("M::erfc"),
                QString(),
                VariableName(TR("erfc")),
                false,
                TR("erfc"),
                TR("Complementary error function"),
                TR("Special"),
                TR("erfc"),
                false,
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
                QString("M::lambertW"),
                QString(),
                VariableName(TR("W").arg(QChar(0x0392))),
                true,
                TR("lambertw"),
                TR("Lambert W function"),
                TR("Special"),
                TR("lambertw"),
                false,
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::INTEGER, TR("k"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::INTEGER, TR("k"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::INTEGER, TR("k"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::INTEGER, TR("k"),
                    DataType::ValueType::VARIANT, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::REAL, TR("k"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::REAL, TR("k"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::REAL, TR("k"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::REAL, TR("k"),
                    DataType::ValueType::VARIANT, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("k"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("k"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("k"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("k"),
                    DataType::ValueType::VARIANT, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("k"),
                    DataType::ValueType::INTEGER, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("k"),
                    DataType::ValueType::REAL, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("k"),
                    DataType::ValueType::COMPLEX, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("k"),
                    DataType::ValueType::VARIANT, TR("z")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::INTEGER, TR("k"),
                    DataType::ValueType::INTEGER, TR("z"),
                    DataType::ValueType::REAL, TR("relative error")
                ),
                FunctionVariant(
                   DataType::ValueType::COMPLEX,
                    DataType::ValueType::INTEGER, TR("k"),
                    DataType::ValueType::REAL, TR("z"),
                    DataType::ValueType::REAL, TR("relative error")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::INTEGER, TR("k"),
                    DataType::ValueType::COMPLEX, TR("z"),
                    DataType::ValueType::REAL, TR("relative error")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::INTEGER, TR("k"),
                    DataType::ValueType::VARIANT, TR("z"),
                    DataType::ValueType::REAL, TR("relative error")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::REAL, TR("k"),
                    DataType::ValueType::INTEGER, TR("z"),
                    DataType::ValueType::REAL, TR("relative error")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::REAL, TR("k"),
                    DataType::ValueType::REAL, TR("z"),
                    DataType::ValueType::REAL, TR("relative error")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::REAL, TR("k"),
                    DataType::ValueType::COMPLEX, TR("z"),
                    DataType::ValueType::REAL, TR("relative error")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::REAL, TR("k"),
                    DataType::ValueType::VARIANT, TR("z"),
                    DataType::ValueType::REAL, TR("relative error")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("k"),
                    DataType::ValueType::INTEGER, TR("z"),
                    DataType::ValueType::REAL, TR("relative error")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("k"),
                    DataType::ValueType::REAL, TR("z"),
                    DataType::ValueType::REAL, TR("relative error")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("k"),
                    DataType::ValueType::COMPLEX, TR("z"),
                    DataType::ValueType::REAL, TR("relative error")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::COMPLEX, TR("k"),
                    DataType::ValueType::VARIANT, TR("z"),
                    DataType::ValueType::REAL, TR("relative error")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("k"),
                    DataType::ValueType::INTEGER, TR("z"),
                    DataType::ValueType::REAL, TR("relative error")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("k"),
                    DataType::ValueType::REAL, TR("z"),
                    DataType::ValueType::REAL, TR("relative error")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("k"),
                    DataType::ValueType::COMPLEX, TR("z"),
                    DataType::ValueType::REAL, TR("relative error")
                ),
                FunctionVariant(
                    DataType::ValueType::COMPLEX,
                    DataType::ValueType::VARIANT, TR("k"),
                    DataType::ValueType::VARIANT, TR("z"),
                    DataType::ValueType::REAL, TR("relative error")
                )
            )
        );
    }
}
