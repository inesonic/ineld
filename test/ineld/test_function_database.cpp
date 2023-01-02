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
* This file implements tests for the \ref Ld::FunctionDatabase class.  Also provides some testing of the
* \ref Ld::Format class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QSharedPointer>

#include <cassert>

#include <ld_handle.h>
#include <ld_data_type.h>
#include <ld_capabilities.h>
#include <ld_variable_name.h>
#include <ld_function_variant.h>
#include <ld_function_data.h>
#include <ld_function_database.h>

#include "test_function_database.h"


TestFunctionDatabase::TestFunctionDatabase() {}


TestFunctionDatabase::~TestFunctionDatabase() {}


void TestFunctionDatabase::initTestCase() {
    Ld::Handle::initialize(0x12345678ABCDEF0);
}


void TestFunctionDatabase::testAddCategories() {
    Ld::FunctionDatabase::reset();

    Ld::FunctionDatabase db;
    db.addCategories(QList<QString>() << "category 1" << "category 2");
    db.buildSearchDatabase();

    QList<QString> categories = db.categories();
    QCOMPARE(categories, QList<QString>() << "category 1" << "category 2");

    db.registerFunction(
        Ld::FunctionData(
            Ld::FunctionData::Type::BUILT_IN,
            QString("foo"),
            Ld::VariableName("foo", "1"),
            false,
            QString("foo1"),
            QString("Generates wrong answers"),
            QString("category 3"),
            QString("No help at all"),
            false,
            Ld::DataType::ValueType::BOOLEAN,
            Ld::DataType::ValueType::BOOLEAN, "p1"
        )
    );

    db.buildSearchDatabase();

    categories = db.categories();
    QCOMPARE(categories, QList<QString>() << "category 1" << "category 2" << "category 3");
}


void TestFunctionDatabase::testPersistentRegistrationAndLookupMethods() {
    Ld::FunctionDatabase::reset();

    Ld::FunctionDatabase db;

    bool success = db.registerFunction(
        Ld::FunctionData(
            Ld::FunctionData::Type::BUILT_IN,
            QString("foo1"),
            Ld::VariableName("foo", "1"),
            false,
            QString("foo1"),
            QString("Generates wrong answers"),
            QString("category 1"),
            QString("No help at all"),
            false,
            Ld::DataType::ValueType::BOOLEAN,
            Ld::DataType::ValueType::BOOLEAN, "p1"
        )
    );
    QCOMPARE(success, true);

    success = db.registerFunction(
        Ld::FunctionData(
            Ld::FunctionData::Type::BUILT_IN,
            QString("foo1"),
            Ld::VariableName("foo", "2"),
            false,
            QString("foo2"),
            QString("Generates very wrong answers"),
            QString("category 2"),
            QString("Even less help"),
            false,
            Ld::DataType::ValueType::BOOLEAN,
            Ld::DataType::ValueType::BOOLEAN, "p1"
        )
    );
    QCOMPARE(success, false);

    success = db.registerFunction(
        Ld::FunctionData(
            Ld::FunctionData::Type::BUILT_IN,
            QString("foo2"),
            Ld::VariableName("foo", "1"),
            false,
            QString("foo2"),
            QString("Generates very wrong answers"),
            QString("category 3"),
            QString("Even less help"),
            false,
            Ld::DataType::ValueType::BOOLEAN,
            Ld::DataType::ValueType::BOOLEAN, "p1"
        )
    );
    QCOMPARE(success, false);

    success = db.registerFunction(
        Ld::FunctionData(
            Ld::FunctionData::Type::BUILT_IN,
            QString("foo2"),
            Ld::VariableName("foo", "2"),
            false,
            QString("foo2"),
            QString("Generates marginal answers"),
            QString("category 2"),
            QString("Even less help"),
            false,
            Ld::DataType::ValueType::BOOLEAN,
            Ld::DataType::ValueType::BOOLEAN, "p1"
        )
    );
    QCOMPARE(success, true);

    Ld::FunctionData f1a = db.function(QString("foo1"));
    QCOMPARE(
        f1a,
        Ld::FunctionData(
            Ld::FunctionData::Type::BUILT_IN,
            QString("foo1"),
            Ld::VariableName("foo", "1"),
            false,
            QString("foo1"),
            QString("Generates wrong answers"),
            QString("category 1"),
            QString("No help at all"),
            false,
            Ld::DataType::ValueType::BOOLEAN,
            Ld::DataType::ValueType::BOOLEAN, "p1"
        )
    );

    Ld::FunctionData f1b = db.function(Ld::VariableName("foo", "1"));
    QCOMPARE(
        f1b,
        Ld::FunctionData(
            Ld::FunctionData::Type::BUILT_IN,
            QString("foo1"),
            Ld::VariableName("foo", "1"),
            false,
            QString("foo1"),
            QString("Generates wrong answers"),
            QString("category 1"),
            QString("No help at all"),
            false,
            Ld::DataType::ValueType::BOOLEAN,
            Ld::DataType::ValueType::BOOLEAN, "p1"
        )
    );

    QList<Ld::FunctionData> functions = db.functions();
    QCOMPARE(functions.size(), 2);
    QCOMPARE(
        functions.at(0),
        Ld::FunctionData(
            Ld::FunctionData::Type::BUILT_IN,
            QString("foo1"),
            Ld::VariableName("foo", "1"),
            false,
            QString("foo1"),
            QString("Generates wrong answers"),
            QString("category 1"),
            QString("No help at all"),
            false,
            Ld::DataType::ValueType::BOOLEAN,
            Ld::DataType::ValueType::BOOLEAN, "p1"
        )
    );
    QCOMPARE(
        functions.at(1),
        Ld::FunctionData(
            Ld::FunctionData::Type::BUILT_IN,
            QString("foo2"),
            Ld::VariableName("foo", "2"),
            false,
            QString("foo2"),
            QString("Generates marginal answers"),
            QString("category 2"),
            QString("Even less help"),
            false,
            Ld::DataType::ValueType::BOOLEAN,
            Ld::DataType::ValueType::BOOLEAN, "p1"
        )
    );

    QCOMPARE(
        db.function(Ld::VariableName("foo", "1")),
        Ld::FunctionData(
            Ld::FunctionData::Type::BUILT_IN,
            QString("foo1"),
            Ld::VariableName("foo", "1"),
            false,
            QString("foo1"),
            QString("Generates wrong answers"),
            QString("category 1"),
            QString("No help at all"),
            false,
            Ld::DataType::ValueType::BOOLEAN,
            Ld::DataType::ValueType::BOOLEAN, "p1"
        )
    );
    QCOMPARE(
        db.function(Ld::VariableName("foo", "2")),
        Ld::FunctionData(
            Ld::FunctionData::Type::BUILT_IN,
            QString("foo2"),
            Ld::VariableName("foo", "2"),
            false,
            QString("foo2"),
            QString("Generates marginal answers"),
            QString("category 2"),
            QString("Even less help"),
            false,
            Ld::DataType::ValueType::BOOLEAN,
            Ld::DataType::ValueType::BOOLEAN, "p1"
        )
    );

    QCOMPARE(db.function(Ld::VariableName("foo", "3")).isInvalid(), true);
}


void TestFunctionDatabase::testSearchFunction() {
    Ld::FunctionDatabase::reset();
    Ld::FunctionDatabase db;

    bool success = db.registerFunction(
        Ld::FunctionData(
            Ld::FunctionData::Type::BUILT_IN,
            QString("sin"),
            Ld::VariableName("sin"),
            false,
            QString("sin"),
            QString("Sine function"),
            QString("Trigonometry"),
            QString("Sine function"),
            false,
            Ld::DataType::ValueType::BOOLEAN,
            Ld::DataType::ValueType::BOOLEAN, "angle, radians"
        )
    );
    QCOMPARE(success, true);

    success = db.registerFunction(
        Ld::FunctionData(
            Ld::FunctionData::Type::BUILT_IN,
            QString("cos"),
            Ld::VariableName("cos"),
            false,
            QString("cos"),
            QString("Cosine function"),
            QString("Trigonometry"),
            QString("Cosine function"),
            false,
            Ld::DataType::ValueType::BOOLEAN,
            Ld::DataType::ValueType::BOOLEAN, "angle (radians)"
        )
    );
    QCOMPARE(success, true);

    success = db.registerFunction(
        Ld::FunctionData(
            Ld::FunctionData::Type::BUILT_IN,
            QString("tan"),
            Ld::VariableName("tan"),
            false,
            QString("tan"),
            QString("Tangent function"),
            QString("Trigonometry"),
            QString("Tangent function"),
            false,
            Ld::DataType::ValueType::BOOLEAN,
            Ld::DataType::ValueType::BOOLEAN, "angle (radians)"
        )
    );
    QCOMPARE(success, true);

    success = db.registerFunction(
        Ld::FunctionData(
            Ld::FunctionData::Type::BUILT_IN,
            QString("asin"),
            Ld::VariableName("asin"),
            false,
            QString("asin"),
            QString("Inverse sine function"),
            QString("Trigonometry"),
            QString("Inverse sine function"),
            false,
            Ld::DataType::ValueType::BOOLEAN,
            Ld::DataType::ValueType::BOOLEAN, "value"
        )
    );
    QCOMPARE(success, true);

    success = db.registerFunction(
        Ld::FunctionData(
            Ld::FunctionData::Type::BUILT_IN,
            QString("acos"),
            Ld::VariableName("acos"),
            false,
            QString("acos"),
            QString("Inverse cosine function"),
            QString("Trigonometry"),
            QString("Inverse cosine function"),
            false,
            Ld::DataType::ValueType::BOOLEAN,
            Ld::DataType::ValueType::BOOLEAN, "value"
        )
    );
    QCOMPARE(success, true);

    success = db.registerFunction(
        Ld::FunctionData(
            Ld::FunctionData::Type::BUILT_IN,
            QString("atan"),
            Ld::VariableName("atan"),
            false,
            QString("atan"),
            QString("Inverse Tangent function"),
            QString("Trigonometry"),
            QString("Inverse Tangent function"),
            false,
            Ld::DataType::ValueType::BOOLEAN,
            Ld::DataType::ValueType::BOOLEAN, "value"
        )
    );
    QCOMPARE(success, true);

    success = db.registerFunction(
        Ld::FunctionData(
            Ld::FunctionData::Type::BUILT_IN,
            QString("sinh"),
            Ld::VariableName("sinh"),
            false,
            QString("sinh"),
            QString("Hyperbolic sine function"),
            QString("Hyperbolic Trigonometry"),
            QString("Huperbolic sine function"),
            false,
            Ld::DataType::ValueType::BOOLEAN,
            Ld::DataType::ValueType::BOOLEAN, "angle, radians"
        )
    );
    QCOMPARE(success, true);

    success = db.registerFunction(
        Ld::FunctionData(
            Ld::FunctionData::Type::BUILT_IN,
            QString("cosh"),
            Ld::VariableName("cosh"),
            false,
            QString("cosh"),
            QString("Hyperbolic cosine function"),
            QString("Hyperbolic Trigonometry"),
            QString("Hyperbolic cosine function"),
            false,
            Ld::DataType::ValueType::BOOLEAN,
            Ld::DataType::ValueType::BOOLEAN, "angle (radians)"
        )
    );
    QCOMPARE(success, true);

    success = db.registerFunction(
        Ld::FunctionData(
            Ld::FunctionData::Type::BUILT_IN,
            QString("tanh"),
            Ld::VariableName("tanh"),
            false,
            QString("tanh"),
            QString("Hyperbolic tangent function"),
            QString("Hyperbolic Trigonometry"),
            QString("Hyperbolic tangent function"),
            false,
            Ld::DataType::ValueType::BOOLEAN,
            Ld::DataType::ValueType::BOOLEAN, "angle (radians)"
        )
    );
    QCOMPARE(success, true);

    success = db.registerFunction(
        Ld::FunctionData(
            Ld::FunctionData::Type::BUILT_IN,
            QString("asinh"),
            Ld::VariableName("asinh"),
            false,
            QString("asinh"),
            QString("Inverse hyperbolic sine function"),
            QString("Hyperbolic Trigonometry"),
            QString("Inverse hyperbolic sine function"),
            false,
            Ld::DataType::ValueType::BOOLEAN,
            Ld::DataType::ValueType::BOOLEAN, "value"
        )
    );
    QCOMPARE(success, true);

    success = db.registerFunction(
        Ld::FunctionData(
            Ld::FunctionData::Type::BUILT_IN,
            QString("acosh"),
            Ld::VariableName("acosh"),
            false,
            QString("acosh"),
            QString("Inverse hyperbolic cosine function"),
            QString("Hyperbolic Trigonometry"),
            QString("Inverse hyperbolic cosine function"),
            false,
            Ld::DataType::ValueType::BOOLEAN,
            Ld::DataType::ValueType::BOOLEAN, "value"
        )
    );
    QCOMPARE(success, true);

    success = db.registerFunction(
        Ld::FunctionData(
            Ld::FunctionData::Type::BUILT_IN,
            QString("atanh"),
            Ld::VariableName("atanh"),
            false,
            QString("atanh"),
            QString("Inverse hyberbolic tangent function"),
            QString("Hyperbolic Trigonometry"),
            QString("Inverse hyperbolic tangent function"),
            false,
            Ld::DataType::ValueType::BOOLEAN,
            Ld::DataType::ValueType::BOOLEAN, "value"
        )
    );
    QCOMPARE(success, true);

    db.buildSearchDatabase();

    QList<Ld::FunctionData> result = db.search(QList<QString>() << "Hyperbolic Trigonometry");

    QCOMPARE(result.size(), 6);
    QCOMPARE(result.at(0).internalName(), QString("acosh"));
    QCOMPARE(result.at(1).internalName(), QString("asinh"));
    QCOMPARE(result.at(2).internalName(), QString("atanh"));
    QCOMPARE(result.at(3).internalName(), QString("cosh"));
    QCOMPARE(result.at(4).internalName(), QString("sinh"));
    QCOMPARE(result.at(5).internalName(), QString("tanh"));

    result = db.search(QString("sine"));

    QCOMPARE(result.size(), 4);
    QCOMPARE(result.at(0).internalName(), QString("asin"));
    QCOMPARE(result.at(1).internalName(), QString("asinh"));
    QCOMPARE(result.at(2).internalName(), QString("sin"));
    QCOMPARE(result.at(3).internalName(), QString("sinh"));
}
