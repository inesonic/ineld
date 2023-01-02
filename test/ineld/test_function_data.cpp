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
* This file implements tests for the \ref Ld::FunctionData class.  Also provides some testing of the
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

#include "test_function_data.h"


TestFunctionData::TestFunctionData() {}


TestFunctionData::~TestFunctionData() {}


void TestFunctionData::initTestCase() {
    Ld::Handle::initialize(0x12345678ABCDEF0);
}


void TestFunctionData::testConstructorsAndDestructors() {
    Ld::FunctionData d1;
    QCOMPARE(d1.isValid(), false);

    Ld::FunctionData d2(
        Ld::FunctionData::Type::USER_DEFINED,
        QString("foo"),
        QString(),
        Ld::VariableName(QString("Foo")),
        false,
        QString("fubar"),
        QString("Fubar function"),
        QString("Wrong Answers"),
        QString("Help me !"),
        false,
        Ld::FunctionVariant(
            Ld::DataType::ValueType::REAL,
            Ld::DataType::ValueType::COMPLEX, QString("angle")
        )
    );

    QCOMPARE(d2.isValid(), true);
    QCOMPARE(d2.functionType(), Ld::FunctionData::Type::USER_DEFINED);
    QCOMPARE(d2.internalName(), QString("foo"));
    QCOMPARE(d2.primaryLibraryName(), QString());
    QCOMPARE(d2.userVisibleName(), Ld::VariableName(QString("Foo")));
    QCOMPARE(d2.includeSubscriptParameter(), false);
    QCOMPARE(d2.functionCommand(), QString("fubar"));
    QCOMPARE(d2.description(), QString("Fubar function"));
    QCOMPARE(d2.category(), QString("Wrong Answers"));
    QCOMPARE(d2.helpData(), QString("Help me !"));
    QCOMPARE(d2.requiresPerThreadInstance(), false);
    QCOMPARE(d2.variants().size(), 1);
    QCOMPARE(d2.variants().at(0).returnValueType(), Ld::DataType::ValueType::REAL);
    QCOMPARE(d2.variants().at(0).parameterTypes().size(), 1);
    QCOMPARE(d2.variants().at(0).parameterTypes().at(0), Ld::DataType::ValueType::COMPLEX);
    QCOMPARE(d2.variants().at(0).parameterDescriptions().at(0), QString("angle"));

    Ld::FunctionData d3(
        Ld::FunctionData::Type::USER_DEFINED,
        QString("foo"),
        QString("fnord"),
        Ld::VariableName(QString("Foo")),
        true,
        QString("fubar"),
        QString("Fubar function"),
        QString("Wrong Answers"),
        QString("Help me !"),
        false,
        Ld::FunctionVariant(
            Ld::DataType::ValueType::REAL,
            Ld::DataType::ValueType::COMPLEX, QString("angle")
        )
    );

    QCOMPARE(d3.isValid(), true);
    QCOMPARE(d3.functionType(), Ld::FunctionData::Type::USER_DEFINED);
    QCOMPARE(d3.internalName(), QString("foo"));
    QCOMPARE(d3.primaryLibraryName(), QString("fnord"));
    QCOMPARE(d3.userVisibleName(), Ld::VariableName(QString("Foo")));
    QCOMPARE(d3.includeSubscriptParameter(), true);
    QCOMPARE(d3.functionCommand(), QString("fubar"));
    QCOMPARE(d3.description(), QString("Fubar function"));
    QCOMPARE(d3.category(), QString("Wrong Answers"));
    QCOMPARE(d3.helpData(), QString("Help me !"));
    QCOMPARE(d3.requiresPerThreadInstance(), false);
    QCOMPARE(d3.variants().size(), 1);
    QCOMPARE(d3.variants().at(0).returnValueType(), Ld::DataType::ValueType::REAL);
    QCOMPARE(d3.variants().at(0).parameterTypes().size(), 1);
    QCOMPARE(d3.variants().at(0).parameterTypes().at(0), Ld::DataType::ValueType::COMPLEX);
    QCOMPARE(d3.variants().at(0).parameterDescriptions().at(0), QString("angle"));

    Ld::FunctionData d4(
        Ld::FunctionData::Type::BUILT_IN,
        QString("bar"),
        Ld::VariableName(QString("Bar")),
        false,
        QString("foobar"),
        QString("Fubar Squared function"),
        QString("More Wrong Answers"),
        QString("I need help !"),
        false,
        Ld::DataType::ValueType::COMPLEX,
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::REAL,
        QList<QString>() << QString("theta")
    );

    QCOMPARE(d4.isValid(), true);
    QCOMPARE(d4.functionType(), Ld::FunctionData::Type::BUILT_IN);
    QCOMPARE(d4.internalName(), QString("bar"));
    QCOMPARE(d4.primaryLibraryName(), QString());
    QCOMPARE(d4.userVisibleName(), Ld::VariableName(QString("Bar")));
    QCOMPARE(d4.includeSubscriptParameter(), false);
    QCOMPARE(d4.functionCommand(), QString("foobar"));
    QCOMPARE(d4.description(), QString("Fubar Squared function"));
    QCOMPARE(d4.category(), QString("More Wrong Answers"));
    QCOMPARE(d4.helpData(), QString("I need help !"));
    QCOMPARE(d4.requiresPerThreadInstance(), false);
    QCOMPARE(d4.variants().size(), 1);
    QCOMPARE(d4.variants().at(0).returnValueType(), Ld::DataType::ValueType::COMPLEX);
    QCOMPARE(d4.variants().at(0).parameterTypes().size(), 1);
    QCOMPARE(d4.variants().at(0).parameterTypes().at(0), Ld::DataType::ValueType::REAL);
    QCOMPARE(d4.variants().at(0).parameterDescriptions().at(0), QString("theta"));

    Ld::FunctionData d5(
        Ld::FunctionData::Type::BUILT_IN,
        QString("bar"),
        QString("fnord"),
        Ld::VariableName(QString("Bar")),
        false,
        QString("foobar"),
        QString("Fubar Squared function"),
        QString("More Wrong Answers"),
        QString("No help here"),
        false,
        Ld::DataType::ValueType::COMPLEX,
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::REAL,
        QList<QString>() << QString("theta")
    );

    QCOMPARE(d5.isValid(), true);
    QCOMPARE(d5.functionType(), Ld::FunctionData::Type::BUILT_IN);
    QCOMPARE(d5.internalName(), QString("bar"));
    QCOMPARE(d5.primaryLibraryName(), QString("fnord"));
    QCOMPARE(d5.userVisibleName(), Ld::VariableName(QString("Bar")));
    QCOMPARE(d5.includeSubscriptParameter(), false);
    QCOMPARE(d5.functionCommand(), QString("foobar"));
    QCOMPARE(d5.description(), QString("Fubar Squared function"));
    QCOMPARE(d5.category(), QString("More Wrong Answers"));
    QCOMPARE(d5.helpData(), QString("No help here"));
    QCOMPARE(d5.requiresPerThreadInstance(), false);
    QCOMPARE(d5.variants().size(), 1);
    QCOMPARE(d5.variants().at(0).returnValueType(), Ld::DataType::ValueType::COMPLEX);
    QCOMPARE(d5.variants().at(0).parameterTypes().size(), 1);
    QCOMPARE(d5.variants().at(0).parameterTypes().at(0), Ld::DataType::ValueType::REAL);
    QCOMPARE(d5.variants().at(0).parameterDescriptions().at(0), QString("theta"));

    Ld::FunctionData d6(
        Ld::FunctionData::Type::USER_DEFINED,
        QString("foo"),
        Ld::VariableName(QString("Foo")),
        false,
        QString("fubar"),
        QString("Fubar function"),
        QString("Wrong Answers"),
        QString("Helpless"),
        false,
        Ld::FunctionVariant(
            Ld::DataType::ValueType::REAL,
            Ld::DataType::ValueType::COMPLEX, QString("angle")
        ),
        Ld::FunctionVariant(
            Ld::DataType::ValueType::COMPLEX,
            Ld::DataType::ValueType::INTEGER, QString("elgna"),
            Ld::DataType::ValueType::SET, QString("theta")
        )
    );

    QCOMPARE(d6.isValid(), true);
    QCOMPARE(d6.functionType(), Ld::FunctionData::Type::USER_DEFINED);
    QCOMPARE(d6.internalName(), QString("foo"));
    QCOMPARE(d6.primaryLibraryName(), QString());
    QCOMPARE(d6.userVisibleName(), Ld::VariableName(QString("Foo")));
    QCOMPARE(d6.includeSubscriptParameter(), false);
    QCOMPARE(d6.functionCommand(), QString("fubar"));
    QCOMPARE(d6.description(), QString("Fubar function"));
    QCOMPARE(d6.category(), QString("Wrong Answers"));
    QCOMPARE(d6.helpData(), QString("Helpless"));
    QCOMPARE(d6.requiresPerThreadInstance(), false);
    QCOMPARE(d6.variants().size(), 2);
    QCOMPARE(d6.variants().at(0).returnValueType(), Ld::DataType::ValueType::REAL);
    QCOMPARE(d6.variants().at(0).parameterTypes().size(), 1);
    QCOMPARE(d6.variants().at(0).parameterTypes().at(0), Ld::DataType::ValueType::COMPLEX);
    QCOMPARE(d6.variants().at(0).parameterDescriptions().at(0), QString("angle"));
    QCOMPARE(d6.variants().at(1).returnValueType(), Ld::DataType::ValueType::COMPLEX);
    QCOMPARE(d6.variants().at(1).parameterTypes().size(), 2);
    QCOMPARE(d6.variants().at(1).parameterTypes().at(0), Ld::DataType::ValueType::INTEGER);
    QCOMPARE(d6.variants().at(1).parameterDescriptions().at(0), QString("elgna"));
    QCOMPARE(d6.variants().at(1).parameterTypes().at(1), Ld::DataType::ValueType::SET);
    QCOMPARE(d6.variants().at(1).parameterDescriptions().at(1), QString("theta"));

    Ld::FunctionData d7(
        Ld::FunctionData::Type::USER_DEFINED,
        QString("foo"),
        QString("fnord"),
        Ld::VariableName(QString("Foo")),
        true,
        QString("fubar"),
        QString("Fubar function"),
        QString("Wrong Answers"),
        QString("And hopeless"),
        false,
        Ld::FunctionVariant(
            Ld::DataType::ValueType::REAL,
            Ld::DataType::ValueType::COMPLEX, QString("angle")
        ),
        Ld::FunctionVariant(
            Ld::DataType::ValueType::COMPLEX,
            Ld::DataType::ValueType::INTEGER, QString("elgna"),
            Ld::DataType::ValueType::SET, QString("theta")
        )
    );

    QCOMPARE(d7.isValid(), true);
    QCOMPARE(d7.functionType(), Ld::FunctionData::Type::USER_DEFINED);
    QCOMPARE(d7.internalName(), QString("foo"));
    QCOMPARE(d7.primaryLibraryName(), QString("fnord"));
    QCOMPARE(d7.userVisibleName(), Ld::VariableName(QString("Foo")));
    QCOMPARE(d7.includeSubscriptParameter(), true);
    QCOMPARE(d7.functionCommand(), QString("fubar"));
    QCOMPARE(d7.description(), QString("Fubar function"));
    QCOMPARE(d7.category(), QString("Wrong Answers"));
    QCOMPARE(d7.helpData(), QString("And hopeless"));
    QCOMPARE(d7.requiresPerThreadInstance(), false);
    QCOMPARE(d7.variants().size(), 2);
    QCOMPARE(d7.variants().at(0).returnValueType(), Ld::DataType::ValueType::REAL);
    QCOMPARE(d7.variants().at(0).parameterTypes().size(), 1);
    QCOMPARE(d7.variants().at(0).parameterTypes().at(0), Ld::DataType::ValueType::COMPLEX);
    QCOMPARE(d7.variants().at(0).parameterDescriptions().at(0), QString("angle"));
    QCOMPARE(d7.variants().at(1).returnValueType(), Ld::DataType::ValueType::COMPLEX);
    QCOMPARE(d7.variants().at(1).parameterTypes().size(), 2);
    QCOMPARE(d7.variants().at(1).parameterTypes().at(0), Ld::DataType::ValueType::INTEGER);
    QCOMPARE(d7.variants().at(1).parameterDescriptions().at(0), QString("elgna"));
    QCOMPARE(d7.variants().at(1).parameterTypes().at(1), Ld::DataType::ValueType::SET);
    QCOMPARE(d7.variants().at(1).parameterDescriptions().at(1), QString("theta"));

    Ld::FunctionData d8(
        Ld::FunctionData::Type::BUILT_IN,
        QString("bar"),
        Ld::VariableName(QString("Bar")),
        false,
        QString("foobar"),
        QString("Fubar Squared function"),
        QString("More Wrong Answers"),
        QString("Bad information"),
        false,
        Ld::DataType::ValueType::COMPLEX,
        Ld::DataType::ValueType::REAL, QString("theta"),
        Ld::DataType::ValueType::INTEGER, QString("angle")
    );

    QCOMPARE(d8.isValid(), true);
    QCOMPARE(d8.functionType(), Ld::FunctionData::Type::BUILT_IN);
    QCOMPARE(d8.internalName(), QString("bar"));
    QCOMPARE(d8.primaryLibraryName(), QString());
    QCOMPARE(d8.userVisibleName(), Ld::VariableName(QString("Bar")));
    QCOMPARE(d8.includeSubscriptParameter(), false);
    QCOMPARE(d8.functionCommand(), QString("foobar"));
    QCOMPARE(d8.description(), QString("Fubar Squared function"));
    QCOMPARE(d8.category(), QString("More Wrong Answers"));
    QCOMPARE(d8.helpData(), QString("Bad information"));
    QCOMPARE(d8.requiresPerThreadInstance(), false);
    QCOMPARE(d8.variants().size(), 1);
    QCOMPARE(d8.variants().at(0).returnValueType(), Ld::DataType::ValueType::COMPLEX);
    QCOMPARE(d8.variants().at(0).parameterTypes().size(), 2);
    QCOMPARE(d8.variants().at(0).parameterTypes().at(0), Ld::DataType::ValueType::REAL);
    QCOMPARE(d8.variants().at(0).parameterDescriptions().at(0), QString("theta"));
    QCOMPARE(d8.variants().at(0).parameterTypes().at(1), Ld::DataType::ValueType::INTEGER);
    QCOMPARE(d8.variants().at(0).parameterDescriptions().at(1), QString("angle"));

    Ld::FunctionData d9(
        Ld::FunctionData::Type::BUILT_IN,
        QString("bar"),
        QString("fnord"),
        Ld::VariableName(QString("Bar")),
        true,
        QString("foobar"),
        QString("Fubar Squared function"),
        QString("More Wrong Answers"),
        QString("Bad advise"),
        false,
        Ld::DataType::ValueType::COMPLEX,
        Ld::DataType::ValueType::REAL, QString("theta"),
        Ld::DataType::ValueType::INTEGER, QString("angle")
    );

    QCOMPARE(d9.isValid(), true);
    QCOMPARE(d9.functionType(), Ld::FunctionData::Type::BUILT_IN);
    QCOMPARE(d9.internalName(), QString("bar"));
    QCOMPARE(d9.primaryLibraryName(), QString("fnord"));
    QCOMPARE(d9.userVisibleName(), Ld::VariableName(QString("Bar")));
    QCOMPARE(d9.includeSubscriptParameter(), true);
    QCOMPARE(d9.functionCommand(), QString("foobar"));
    QCOMPARE(d9.description(), QString("Fubar Squared function"));
    QCOMPARE(d9.category(), QString("More Wrong Answers"));
    QCOMPARE(d9.helpData(), QString("Bad advise"));
    QCOMPARE(d9.requiresPerThreadInstance(), false);
    QCOMPARE(d9.variants().size(), 1);
    QCOMPARE(d9.variants().at(0).returnValueType(), Ld::DataType::ValueType::COMPLEX);
    QCOMPARE(d9.variants().at(0).parameterTypes().size(), 2);
    QCOMPARE(d9.variants().at(0).parameterTypes().at(0), Ld::DataType::ValueType::REAL);
    QCOMPARE(d9.variants().at(0).parameterDescriptions().at(0), QString("theta"));
    QCOMPARE(d9.variants().at(0).parameterTypes().at(1), Ld::DataType::ValueType::INTEGER);
    QCOMPARE(d9.variants().at(0).parameterDescriptions().at(1), QString("angle"));

    Ld::FunctionData d10(d9);

    QCOMPARE(d10.isValid(), true);
    QCOMPARE(d10.functionType(), Ld::FunctionData::Type::BUILT_IN);
    QCOMPARE(d10.internalName(), QString("bar"));
    QCOMPARE(d10.primaryLibraryName(), QString("fnord"));
    QCOMPARE(d10.userVisibleName(), Ld::VariableName(QString("Bar")));
    QCOMPARE(d10.includeSubscriptParameter(), true);
    QCOMPARE(d10.functionCommand(), QString("foobar"));
    QCOMPARE(d10.description(), QString("Fubar Squared function"));
    QCOMPARE(d10.category(), QString("More Wrong Answers"));
    QCOMPARE(d10.helpData(), QString("Bad advise"));
    QCOMPARE(d10.requiresPerThreadInstance(), false);
    QCOMPARE(d10.variants().size(), 1);
    QCOMPARE(d10.variants().at(0).returnValueType(), Ld::DataType::ValueType::COMPLEX);
    QCOMPARE(d10.variants().at(0).parameterTypes().size(), 2);
    QCOMPARE(d10.variants().at(0).parameterTypes().at(0), Ld::DataType::ValueType::REAL);
    QCOMPARE(d10.variants().at(0).parameterDescriptions().at(0), QString("theta"));
    QCOMPARE(d10.variants().at(0).parameterTypes().at(1), Ld::DataType::ValueType::INTEGER);
    QCOMPARE(d10.variants().at(0).parameterDescriptions().at(1), QString("angle"));
}


void TestFunctionData::testValidInvalidMethods() {
    Ld::FunctionData d1;
    QCOMPARE(d1.isValid(), false);
    QCOMPARE(d1.isInvalid(), true);

    Ld::FunctionData d2(
        Ld::FunctionData::Type::USER_DEFINED,
        QString("foo"),
        Ld::VariableName(QString("Foo")),
        false,
        QString("fubar"),
        QString("Fubar function"),
        QString("Wrong Answers"),
        QString("Bad information"),
        false,
        Ld::FunctionVariant(
            Ld::DataType::ValueType::REAL,
            Ld::DataType::ValueType::COMPLEX, QString("angle")
        )
    );

    QCOMPARE(d2.isValid(), true);
    QCOMPARE(d2.isInvalid(), false);
}


void TestFunctionData::testAccessorMethods() {
    // Accessors are tested by the other methods.
}


void TestFunctionData::testAssignmentOperator() {
    Ld::FunctionData d1;
    QCOMPARE(d1.isValid(), false);

    Ld::FunctionData d2(
        Ld::FunctionData::Type::USER_DEFINED,
        QString("foo"),
        Ld::VariableName(QString("Foo")),
        true,
        QString("fubar"),
        QString("Fubar function"),
        QString("Wrong Answers"),
        QString("Bad information"),
        false,
        Ld::FunctionVariant(
            Ld::DataType::ValueType::REAL,
            Ld::DataType::ValueType::COMPLEX, QString("angle")
        )
    );

    QCOMPARE(d2.isValid(), true);
    QCOMPARE(d2.functionType(), Ld::FunctionData::Type::USER_DEFINED);
    QCOMPARE(d2.internalName(), QString("foo"));
    QCOMPARE(d2.userVisibleName(), Ld::VariableName(QString("Foo")));
    QCOMPARE(d2.includeSubscriptParameter(), true);
    QCOMPARE(d2.functionCommand(), QString("fubar"));
    QCOMPARE(d2.description(), QString("Fubar function"));
    QCOMPARE(d2.category(), QString("Wrong Answers"));
    QCOMPARE(d2.helpData(), QString("Bad information"));
    QCOMPARE(d2.requiresPerThreadInstance(), false);
    QCOMPARE(d2.variants().size(), 1);
    QCOMPARE(d2.variants().at(0).returnValueType(), Ld::DataType::ValueType::REAL);
    QCOMPARE(d2.variants().at(0).parameterTypes().size(), 1);
    QCOMPARE(d2.variants().at(0).parameterTypes().at(0), Ld::DataType::ValueType::COMPLEX);
    QCOMPARE(d2.variants().at(0).parameterDescriptions().at(0), QString("angle"));

    d1 = d2;

    QCOMPARE(d1.isValid(), true);
    QCOMPARE(d1.functionType(), Ld::FunctionData::Type::USER_DEFINED);
    QCOMPARE(d1.internalName(), QString("foo"));
    QCOMPARE(d1.userVisibleName(), Ld::VariableName(QString("Foo")));
    QCOMPARE(d1.includeSubscriptParameter(), true);
    QCOMPARE(d1.functionCommand(), QString("fubar"));
    QCOMPARE(d1.description(), QString("Fubar function"));
    QCOMPARE(d1.category(), QString("Wrong Answers"));
    QCOMPARE(d1.helpData(), QString("Bad information"));
    QCOMPARE(d1.requiresPerThreadInstance(), false);
    QCOMPARE(d1.variants().size(), 1);
    QCOMPARE(d1.variants().at(0).returnValueType(), Ld::DataType::ValueType::REAL);
    QCOMPARE(d1.variants().at(0).parameterTypes().size(), 1);
    QCOMPARE(d1.variants().at(0).parameterTypes().at(0), Ld::DataType::ValueType::COMPLEX);
    QCOMPARE(d1.variants().at(0).parameterDescriptions().at(0), QString("angle"));
}


void TestFunctionData::testBestFitVariant() {
    Ld::FunctionData d(
        Ld::FunctionData::Type::USER_DEFINED,
        QString("foo"),
        Ld::VariableName(QString("Foo")),
        false,
        QString("fubar"),
        QString("Fubar function"),
        QString("Wrong Answers"),
        QString("Bad information"),
        false,
        Ld::FunctionVariant(
            Ld::DataType::ValueType::REAL,
            Ld::DataType::ValueType::COMPLEX, QString("p1"),
            Ld::DataType::ValueType::INTEGER, QString("p2")
        ),
        Ld::FunctionVariant(
            Ld::DataType::ValueType::REAL,
            Ld::DataType::ValueType::REAL, QString("p1"),
            Ld::DataType::ValueType::INTEGER, QString("p2")
        ),
        Ld::FunctionVariant(
            Ld::DataType::ValueType::COMPLEX,
            Ld::DataType::ValueType::COMPLEX, QString("p1"),
            Ld::DataType::ValueType::REAL, QString("p2")
        ),
        Ld::FunctionVariant(
            Ld::DataType::ValueType::SET,
            Ld::DataType::ValueType::SET, QString("p1"),
            Ld::DataType::ValueType::SET, QString("p2")
        )
    );

    Ld::FunctionVariant v1 = d.bestFitVariant(
        Ld::Capabilities::realOperators,
        QList<Ld::DataType::ValueType>()
            << Ld::DataType::ValueType::NONE
            << Ld::DataType::ValueType::INTEGER
    );

    QCOMPARE(
        v1,
        Ld::FunctionVariant(
            Ld::DataType::ValueType::REAL,
            Ld::DataType::ValueType::REAL, QString("p1"),
            Ld::DataType::ValueType::INTEGER, QString("p2")
        )
    );

    Ld::FunctionVariant v2 = d.bestFitVariant(
        Ld::Capabilities::allScalarRValues,
        QList<Ld::DataType::ValueType>()
            << Ld::DataType::ValueType::COMPLEX
    );

    QCOMPARE(
        v2,
        Ld::FunctionVariant(
            Ld::DataType::ValueType::REAL,
            Ld::DataType::ValueType::COMPLEX, QString("p1"),
            Ld::DataType::ValueType::INTEGER, QString("p2")
        )
    );

    Ld::FunctionVariant v3 = d.bestFitVariant(
        Ld::Capabilities::allRealRValues,
        QList<Ld::DataType::ValueType>()
    );

    QCOMPARE(
        v3,
        Ld::FunctionVariant(
            Ld::DataType::ValueType::REAL,
            Ld::DataType::ValueType::REAL, QString("p1"),
            Ld::DataType::ValueType::INTEGER, QString("p2")
        )
    );

    Ld::FunctionVariant v4 = d.bestFitVariant(
        Ld::Capabilities::setOperators,
        QList<Ld::DataType::ValueType>()
    );

    QCOMPARE(
        v4,
        Ld::FunctionVariant(
            Ld::DataType::ValueType::SET,
            Ld::DataType::ValueType::SET, QString("p1"),
            Ld::DataType::ValueType::SET, QString("p2")
        )
    );

    Ld::FunctionVariant v5 = d.bestFitVariant(
        Ld::Capabilities::setOperators,
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::REAL
    );

    QCOMPARE(v5.isInvalid(), true);
}


void TestFunctionData::testAllCompatibleVariants() {
    Ld::FunctionData d(
        Ld::FunctionData::Type::USER_DEFINED,
        QString("foo"),
        Ld::VariableName(QString("Foo")),
        false,
        QString("fubar"),
        QString("Fubar function"),
        QString("Wrong Answers"),
        QString("Bad information"),
        false,
        Ld::FunctionVariant(
            Ld::DataType::ValueType::REAL,
            Ld::DataType::ValueType::COMPLEX, QString("p1"),
            Ld::DataType::ValueType::INTEGER, QString("p2")
        ),
        Ld::FunctionVariant(
            Ld::DataType::ValueType::REAL,
            Ld::DataType::ValueType::REAL, QString("p1"),
            Ld::DataType::ValueType::INTEGER, QString("p2")
        ),
        Ld::FunctionVariant(
            Ld::DataType::ValueType::COMPLEX,
            Ld::DataType::ValueType::COMPLEX, QString("p1"),
            Ld::DataType::ValueType::REAL, QString("p2")
        ),
        Ld::FunctionVariant(
            Ld::DataType::ValueType::SET,
            Ld::DataType::ValueType::SET, QString("p1"),
            Ld::DataType::ValueType::SET, QString("p2")
        )
    );

    QList<Ld::FunctionVariant> m1 = d.allCompatibleVariants(
        Ld::Capabilities::allScalarRValues,
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::COMPLEX
    );

    QCOMPARE(m1.size(), 2);
    QCOMPARE(
        m1.at(0),
        Ld::FunctionVariant(
            Ld::DataType::ValueType::REAL,
            Ld::DataType::ValueType::COMPLEX, QString("p1"),
            Ld::DataType::ValueType::INTEGER, QString("p2")
        )
    );
    QCOMPARE(
        m1.at(1),
        Ld::FunctionVariant(
            Ld::DataType::ValueType::COMPLEX,
            Ld::DataType::ValueType::COMPLEX, QString("p1"),
            Ld::DataType::ValueType::REAL, QString("p2")
        )
    );

    QList<Ld::FunctionVariant> m2 = d.allCompatibleVariants(
        Ld::Capabilities::realOperators,
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::NONE << Ld::DataType::ValueType::INTEGER
    );

    QCOMPARE(m2.size(), 2);
    QCOMPARE(
        m2.at(0),
        Ld::FunctionVariant(
            Ld::DataType::ValueType::REAL,
            Ld::DataType::ValueType::REAL, QString("p1"),
            Ld::DataType::ValueType::INTEGER, QString("p2")
        )
    );
    QCOMPARE(
        m2.at(1),
        Ld::FunctionVariant(
            Ld::DataType::ValueType::REAL,
            Ld::DataType::ValueType::COMPLEX, QString("p1"),
            Ld::DataType::ValueType::INTEGER, QString("p2")
        )
    );

    QList<Ld::FunctionVariant> m3 = d.allCompatibleVariants(
        Ld::Capabilities::complexOperators,
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::COMPLEX
    );

    QCOMPARE(m3.size(), 1);
    QCOMPARE(
        m3.at(0),
        Ld::FunctionVariant(
            Ld::DataType::ValueType::COMPLEX,
            Ld::DataType::ValueType::COMPLEX, QString("p1"),
            Ld::DataType::ValueType::REAL, QString("p2")
        )
    );

    QList<Ld::FunctionVariant> m4 = d.allCompatibleVariants(
        Ld::Capabilities::tupleOperators,
        QList<Ld::DataType::ValueType>()
    );

    QCOMPARE(m4.size(), 0);

    QList<Ld::FunctionVariant> m5 = d.allCompatibleVariants(
        Ld::Capabilities::allRValues,
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::MATRIX_REAL
    );

    QCOMPARE(m5.size(), 0);
}


void TestFunctionData::testChildCapabilities() {
    Ld::FunctionData d(
        Ld::FunctionData::Type::USER_DEFINED,
        QString("foo"),
        Ld::VariableName(QString("Foo")),
        false,
        QString("fubar"),
        QString("Fubar function"),
        QString("Wrong Answers"),
        QString("Bad information"),
        false,
        Ld::FunctionVariant(
            Ld::DataType::ValueType::REAL,
            Ld::DataType::ValueType::COMPLEX, QString("p1"),
            Ld::DataType::ValueType::INTEGER, QString("p2")
        ),
        Ld::FunctionVariant(
            Ld::DataType::ValueType::INTEGER,
            Ld::DataType::ValueType::REAL, QString("p1"),
            Ld::DataType::ValueType::INTEGER, QString("p2")
        )
    );

    Ld::Capabilities m = d.childCapabilities();
    QCOMPARE(m, Ld::Capabilities::allRealRValues | Ld::Capabilities::allIntegerRValues);
}


void TestFunctionData::testParentRequires1() {
    Ld::FunctionData d(
        Ld::FunctionData::Type::USER_DEFINED,
        QString("foo"),
        Ld::VariableName(QString("Foo")),
        false,
        QString("fubar"),
        QString("Fubar function"),
        QString("Wrong Answers"),
        QString("Bad information"),
        false,
        Ld::FunctionVariant(
            Ld::DataType::ValueType::REAL,
            Ld::DataType::ValueType::COMPLEX, QString("p1"),
            Ld::DataType::ValueType::INTEGER, QString("p2")
        ),
        Ld::FunctionVariant(
            Ld::DataType::ValueType::REAL,
            Ld::DataType::ValueType::REAL, QString("p1"),
            Ld::DataType::ValueType::INTEGER, QString("p2")
        ),
        Ld::FunctionVariant(
            Ld::DataType::ValueType::COMPLEX,
            Ld::DataType::ValueType::COMPLEX, QString("p1"),
            Ld::DataType::ValueType::REAL, QString("p2")
        ),
        Ld::FunctionVariant(
            Ld::DataType::ValueType::SET,
            Ld::DataType::ValueType::SET, QString("p1"),
            Ld::DataType::ValueType::SET, QString("p2")
        )
    );

    Ld::Capabilities m1 = d.parentRequires(
        0,
        Ld::Capabilities::allScalarRValues,
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::COMPLEX
    );

    QCOMPARE(m1, Ld::Capabilities::allComplexRValues);

    Ld::Capabilities m2 = d.parentRequires(
        0,
        Ld::Capabilities::realOperators,
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::NONE << Ld::DataType::ValueType::INTEGER
    );

    QCOMPARE(m2, Ld::Capabilities::allRealRValues | Ld::Capabilities::allComplexRValues);

    Ld::Capabilities m3 = d.parentRequires(
        1,
        Ld::Capabilities::complexOperators,
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::COMPLEX
    );

    QCOMPARE(m3, Ld::Capabilities::allRealRValues);

    Ld::Capabilities m4 = d.parentRequires(
        0,
        Ld::Capabilities::tupleOperators,
        QList<Ld::DataType::ValueType>()
    );

    QCOMPARE(m4, Ld::Capabilities());
}


void TestFunctionData::testParentRequires2() {
    Ld::FunctionData d(
        Ld::FunctionData::Type::USER_DEFINED,
        QString("foo"),
        Ld::VariableName(QString("Foo")),
        false,
        QString("fubar"),
        QString("Fubar function"),
        QString("Wrong Answers"),
        QString("Bad information"),
        false,
        Ld::FunctionVariant(
            Ld::DataType::ValueType::REAL,
            Ld::DataType::ValueType::COMPLEX, QString("p1"),
            Ld::DataType::ValueType::INTEGER, QString("p2")
        ),
        Ld::FunctionVariant(
            Ld::DataType::ValueType::REAL,
            Ld::DataType::ValueType::REAL, QString("p1"),
            Ld::DataType::ValueType::INTEGER, QString("p2")
        ),
        Ld::FunctionVariant(
            Ld::DataType::ValueType::COMPLEX,
            Ld::DataType::ValueType::COMPLEX, QString("p1"),
            Ld::DataType::ValueType::REAL, QString("p2")
        ),
        Ld::FunctionVariant(
            Ld::DataType::ValueType::SET,
            Ld::DataType::ValueType::SET, QString("p1"),
            Ld::DataType::ValueType::SET, QString("p2")
        )
    );

    QList<Ld::Capabilities> m1 = d.parentRequires(
        Ld::Capabilities::allScalarRValues,
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::COMPLEX
    );

    QCOMPARE(
        m1,
        QList<Ld::Capabilities>()
            << Ld::Capabilities::allComplexRValues
            << (Ld::Capabilities::allIntegerRValues | Ld::Capabilities::allRealRValues)
    );

    QList<Ld::Capabilities> m2 = d.parentRequires(
        Ld::Capabilities::realOperators,
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::NONE << Ld::DataType::ValueType::INTEGER
    );

    QCOMPARE(
        m2,
        QList<Ld::Capabilities>()
            << (Ld::Capabilities::allRealRValues | Ld::Capabilities::allComplexRValues)
            << Ld::Capabilities::allIntegerRValues
    );

    QList<Ld::Capabilities> m3 = d.parentRequires(
        Ld::Capabilities::complexOperators,
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::COMPLEX
    );

    QCOMPARE(
        m3,
        QList<Ld::Capabilities>()
            << Ld::Capabilities::allComplexRValues
            << Ld::Capabilities::allRealRValues
    );

    QList<Ld::Capabilities> m4 = d.parentRequires(
        Ld::Capabilities::tupleOperators,
        QList<Ld::DataType::ValueType>()
    );

    QCOMPARE(m4.size(), 0);
}


void TestFunctionData::testComparisonMethods() {
    Ld::FunctionData d0(
        Ld::FunctionData::Type::BUILT_IN,
        QString("!bar"),
        Ld::VariableName(QString("Foo"), QString("bar")),
        false,
        QString("!foobar"),
        QString("!Fubar Squared function!"),
        QString("!More Wrong Answers!"),
        QString("Bad information"),
        true,
        Ld::DataType::ValueType::SET,
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::INTEGER,
        QList<QString>() << QString("phi")
    );

    Ld::FunctionData d1(
        Ld::FunctionData::Type::BUILT_IN,
        QString("bar"),
        Ld::VariableName(QString("Foo"), QString("bar")),
        false,
        QString("foobar"),
        QString("Fubar Squared function"),
        QString("More Wrong Answers"),
        QString("Bad information"),
        false,
        Ld::DataType::ValueType::COMPLEX,
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::REAL,
        QList<QString>() << QString("theta")
    );

    Ld::FunctionData d2(
        Ld::FunctionData::Type::BUILT_IN,
        QString("bar"),
        Ld::VariableName(QString("foo")),
        false,
        QString("foobar"),
        QString("Fubar Squared function"),
        QString("More Wrong Answers"),
        QString("Bad information"),
        false,
        Ld::DataType::ValueType::COMPLEX,
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::REAL,
        QList<QString>() << QString("theta")
    );

    Ld::FunctionData d3(
        Ld::FunctionData::Type::BUILT_IN,
        QString("bar"),
        Ld::VariableName(QString("foo"), QString("bang")),
        false,
        QString("foobar"),
        QString("Fubar Squared function"),
        QString("More Wrong Answers"),
        QString("Bad information"),
        false,
        Ld::DataType::ValueType::COMPLEX,
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::REAL,
        QList<QString>() << QString("theta")
    );

    Ld::FunctionData d4(
        Ld::FunctionData::Type::BUILT_IN,
        QString("bar"),
        Ld::VariableName(QString("foo"), QString("bar")),
        false,
        QString("foobar"),
        QString("Fubar Squared function"),
        QString("More Wrong Answers"),
        QString("Bad information"),
        false,
        Ld::DataType::ValueType::COMPLEX,
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::REAL,
        QList<QString>() << QString("theta")
    );

    QCOMPARE(d0 == d1, true);
    QCOMPARE(d0 != d1, false);
    QCOMPARE(d0 < d1, false);
    QCOMPARE(d0 > d1, false);
    QCOMPARE(d0 <= d1, true);
    QCOMPARE(d0 >= d1, true);

    QCOMPARE(d1 == d2, false);
    QCOMPARE(d1 != d2, true);
    QCOMPARE(d1 < d2, true);
    QCOMPARE(d1 > d2, false);
    QCOMPARE(d1 <= d2, true);
    QCOMPARE(d1 >= d2, false);

    QCOMPARE(d1 == d3, false);
    QCOMPARE(d1 != d3, true);
    QCOMPARE(d1 < d3, true);
    QCOMPARE(d1 > d3, false);
    QCOMPARE(d1 <= d3, true);
    QCOMPARE(d1 >= d3, false);

    QCOMPARE(d1 == d4, false);
    QCOMPARE(d1 != d4, true);
    QCOMPARE(d1 < d4, true);
    QCOMPARE(d1 > d4, false);
    QCOMPARE(d1 <= d4, true);
    QCOMPARE(d1 >= d4, false);


    QCOMPARE(d4 == d1, false);
    QCOMPARE(d4 != d1, true);
    QCOMPARE(d4 < d1, false);
    QCOMPARE(d4 > d1, true);
    QCOMPARE(d4 <= d1, false);
    QCOMPARE(d4 >= d1, true);

    QCOMPARE(d4 == d2, false);
    QCOMPARE(d4 != d2, true);
    QCOMPARE(d4 < d2, false);
    QCOMPARE(d4 > d2, true);
    QCOMPARE(d4 <= d2, false);
    QCOMPARE(d4 >= d2, true);

    QCOMPARE(d4 == d3, false);
    QCOMPARE(d4 != d3, true);
    QCOMPARE(d4 < d3, false);
    QCOMPARE(d4 > d3, true);
    QCOMPARE(d4 <= d3, false);
    QCOMPARE(d4 >= d3, true);
}
