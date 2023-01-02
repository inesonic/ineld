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
* This file implements tests of the Ld::IdentifierContainer class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QString>
#include <QStringList>
#include <QList>
#include <QtTest/QtTest>

#include <model_api_types.h>

#include <ld_data_type.h>
#include <ld_identifier.h>
#include <ld_identifier_container.h>

#include "test_identifier_container.h"

TestIdentifierContainer::TestIdentifierContainer() {}

TestIdentifierContainer::~TestIdentifierContainer() {}

void TestIdentifierContainer::initTestCase() {
    Ld::DataType::registerType(Model::ValueType::INTEGER, "Data Type 1", "Foo1Element");
    Ld::DataType::registerType(Model::ValueType::REAL,    "Data Type 2", "Foo2Element");
}


void TestIdentifierContainer::testConstructorsAndAssignmentMethods() {
    Ld::IdentifierContainer container1;
    Ld::IdentifierContainer container2(new Ld::Identifier("foo", "bar"));

    QSharedPointer<Ld::Identifier> identifier3(new Ld::Identifier("bang", "snort"));
    Ld::IdentifierContainer container3(identifier3);

    QCOMPARE(container1.isValid(), false);
    QCOMPARE(container2.isValid(), true);
    QCOMPARE(container3.isValid(), true);

    QCOMPARE(container2.text1(), QString("foo"));
    QCOMPARE(container2.text2(), QString("bar"));

    QCOMPARE(container3.text1(), QString("bang"));
    QCOMPARE(container3.text2(), QString("snort"));

    container1 = container2;

    QCOMPARE(container1.isValid(), true);
    QCOMPARE(container1.text1(), QString("foo"));
    QCOMPARE(container1.text2(), QString("bar"));
}


void TestIdentifierContainer::testAccessors() {
    Ld::IdentifierContainer container1;

    Ld::Identifier* ident2 = new Ld::Identifier("foo", "bar", Ld::DataType::fromValueType(Model::ValueType::INTEGER));
    QSharedPointer<Ld::Identifier> identifier2(ident2);
    Ld::IdentifierContainer        container2(identifier2);

    QCOMPARE(container1.isValid(),   false);
    QCOMPARE(container1.isInvalid(), true);

    QCOMPARE(container2.isValid(),   true);
    QCOMPARE(container2.isInvalid(), false);

    QCOMPARE(container2.text1(),        QString("foo"));
    QCOMPARE(container2.text2(),        QString("bar"));
    QCOMPARE(container2.dataType(),     Ld::DataType::fromValueType(Model::ValueType::INTEGER));
    QCOMPARE(container2.internalName(), QString("Kfoo_Sbar"));

    QCOMPARE(container2.pointer(), identifier2);
}


void TestIdentifierContainer::testPointerMethods() {
    Ld::IdentifierContainer container1;

    Ld::Identifier* ident2 = new Ld::Identifier("foo", "bar", Ld::DataType::fromValueType(Model::ValueType::INTEGER));
    QSharedPointer<Ld::Identifier> identifier2(ident2);
    Ld::IdentifierContainer        container2(identifier2);

    QCOMPARE(container1.pointer().isNull(), true);
    QCOMPARE(container1.operator->(), nullptr);
}


void TestIdentifierContainer::testEqualityChecks() {
    Ld::IdentifierContainer container1;
    Ld::IdentifierContainer container2(new Ld::Identifier(QString("foo")));
    Ld::IdentifierContainer container3(new Ld::Identifier(QString("foo")));
    Ld::IdentifierContainer container4(new Ld::Identifier(QString("foo"), QString("bar")));

    QCOMPARE(container1 == container1, false);
    QCOMPARE(container1 != container1, true);

    QCOMPARE(container1 == container2, false);
    QCOMPARE(container1 != container2, true);

    QCOMPARE(container2 == container3, true);
    QCOMPARE(container2 != container3, false);

    QCOMPARE(container1 == container4, false);
    QCOMPARE(container1 != container4, true);

    QCOMPARE(container2 == container4, false);
    QCOMPARE(container2 != container4, true);
}
