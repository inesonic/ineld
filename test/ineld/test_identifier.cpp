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
* This file implements tests of the Ld::Identifier class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QString>
#include <QStringList>
#include <QList>
#include <QtTest/QtTest>

#include <ld_data_type.h>
#include <ld_identifier.h>

#include "test_element_position.h"
#include "test_identifier.h"


TestIdentifier::TestIdentifier() {}

TestIdentifier::~TestIdentifier() {}

void TestIdentifier::initTestCase() {
    Ld::Handle::initialize(0x123456789ABCDEF0ULL);
    Ld::DataType::registerType(Model::ValueType::INTEGER, "Data Type 1", "Foo1Element");
    Ld::DataType::registerType(Model::ValueType::REAL,    "Data Type 2", "Foo2Element");
}


void TestIdentifier::testConstructorsAndAssignmentMethods() {
    Ld::Identifier identifier1;

    QCOMPARE(identifier1.isValid(), false);
    QCOMPARE(identifier1.isInvalid(), true);

    Ld::Identifier identifier2("foo", Ld::DataType::fromValueType(Model::ValueType::INTEGER));

    QCOMPARE(identifier2.isValid(), true);
    QCOMPARE(identifier2.isInvalid(), false);

    QCOMPARE(identifier2.text1(), QString("foo"));
    QCOMPARE(identifier2.text2().isEmpty(), true);
    QCOMPARE(identifier2.dataType(), Ld::DataType::fromValueType(Model::ValueType::INTEGER));

    Ld::Identifier identifier3("foo", "bar", Ld::DataType::fromValueType(Model::ValueType::REAL));

    QCOMPARE(identifier3.isValid(), true);
    QCOMPARE(identifier3.isInvalid(), false);

    QCOMPARE(identifier3.text1(), QString("foo"));
    QCOMPARE(identifier3.text2(), QString("bar"));
    QCOMPARE(identifier3.dataType(), Ld::DataType::fromValueType(Model::ValueType::REAL));

    identifier1 = identifier3;

    QCOMPARE(identifier1.isValid(), true);
    QCOMPARE(identifier1.isInvalid(), false);

    QCOMPARE(identifier1.text1(), QString("foo"));
    QCOMPARE(identifier1.text2(), QString("bar"));
    QCOMPARE(identifier1.dataType(), Ld::DataType::fromValueType(Model::ValueType::REAL));
}


void TestIdentifier::testAccessors() {
    // Tested by other methods in this module.
}


void TestIdentifier::testInternalName() {
    Ld::Identifier identifier1("foo");
    QCOMPARE(identifier1.internalName(), QString("_b"));

    Ld::Identifier identifier2("bar");
    QCOMPARE(identifier2.internalName(), QString("_c"));

    Ld::Identifier identifier3(QString("foo"), QString("bar"));
    QCOMPARE(identifier3.internalName(), QString("_d"));
}


void TestIdentifier::testEqualityChecks() {
    Ld::Identifier identifier1;
    Ld::Identifier identifier2(QString("foo"));
    Ld::Identifier identifier3(QString("foo"), QString("bar"));

    QCOMPARE(identifier1 == identifier1, true);
    QCOMPARE(identifier1 != identifier1, false);

    QCOMPARE(identifier1 == identifier2, false);
    QCOMPARE(identifier1 != identifier2, true);

    QCOMPARE(identifier1 == identifier3, false);
    QCOMPARE(identifier1 != identifier3, true);

    QCOMPARE(identifier2 == identifier3, false);
    QCOMPARE(identifier2 != identifier3, true);
}
