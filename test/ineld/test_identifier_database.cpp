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
* This file implements tests of the Ld::IdentifierDatabase class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QString>
#include <QStringList>
#include <QSharedPointer>
#include <QList>
#include <QtTest/QtTest>

#include <ld_data_type.h>
#include <ld_identifier.h>
#include <ld_identifier_container.h>
#include <ld_identifier_database.h>

#include "test_element_position.h"
#include "test_identifier_database.h"

TestIdentifierDatabase::TestIdentifierDatabase() {}


TestIdentifierDatabase::~TestIdentifierDatabase() {}


void TestIdentifierDatabase::initTestCase() {
    Ld::Handle::initialize(0x123456789ABCDEF0ULL);
    Ld::DataType::registerType(Model::ValueType::INTEGER, "Data Type 1", "Foo1Element");
    Ld::DataType::registerType(Model::ValueType::REAL, "Data Type 2", "Foo2Element");
}


void TestIdentifierDatabase::testConstructorsAndAssignmentMethods() {
    Ld::IdentifierDatabase db1;
    QCOMPARE(db1.isEmpty(), true);

    db1.insert(new Ld::Identifier("foo"));
    QCOMPARE(db1.isEmpty(), false);

    Ld::IdentifierDatabase db2(db1);
    QCOMPARE(db2.isEmpty(), false);
    QCOMPARE(db2.identifiers().first().text1(), QString("foo"));

    Ld::IdentifierDatabase db3;
    QCOMPARE(db2.isEmpty(), false);

    db3 = db1;
    QCOMPARE(db3.isEmpty(), false);
    QCOMPARE(db3.identifiers().first().text1(), QString("foo"));
}


void TestIdentifierDatabase::testInsertFunctions() {
    Ld::IdentifierDatabase db;

    Ld::Identifier*                identifier1 = new Ld::Identifier("foo");
    QSharedPointer<Ld::Identifier> identifier2(new Ld::Identifier("bar"));
    Ld::IdentifierContainer        identifier3(new Ld::Identifier("bang"));

    db.insert(identifier1);
    db.insert(identifier2);
    db.insert(identifier3);

    QList<Ld::IdentifierContainer> identifiers = db.identifiers();
    QCOMPARE(identifiers.size(), 3);
    QCOMPARE(db.size(), 3U);

    for (unsigned index=0 ; index<3 ; ++index) {
        Ld::IdentifierContainer identifier = identifiers.at(index);
        QCOMPARE(identifier.handle(), index);
    }

    QCOMPARE(identifiers.at(0).text1(), QString("foo"));
    QCOMPARE(identifiers.at(1).text1(), QString("bar"));
    QCOMPARE(identifiers.at(2).text1(), QString("bang"));
}


void TestIdentifierDatabase::testClearFunction() {
    Ld::IdentifierDatabase db;

    db.insert(new Ld::Identifier("foo", "bar"));
    db.insert(new Ld::Identifier("burp", "boop"));
    db.insert(new Ld::Identifier("bang", "snort"));
    db.insert(new Ld::Identifier("fnord"));
    db.insert(new Ld::Identifier("foop"));

    QList<Ld::IdentifierContainer> identifiers = db.identifiers();
    QCOMPARE(identifiers.size(), 5);
    QCOMPARE(db.size(),       5U);
    QCOMPARE(db.isEmpty(),    false);
    QCOMPARE(db.isNotEmpty(), true);

    db.clear();

    identifiers = db.identifiers();
    QCOMPARE(identifiers.size(), 0);
    QCOMPARE(db.size(),       0U);
    QCOMPARE(db.isEmpty(),    true);
    QCOMPARE(db.isNotEmpty(), false);
}


void TestIdentifierDatabase::testLookupFunctions() {
    Ld::IdentifierDatabase db;

    Ld::Identifier::reset();

    db.insert(new Ld::Identifier("foo", "bar"));
    db.insert(new Ld::Identifier("burp", "boop"));
    db.insert(new Ld::Identifier("bang", "snort"));
    db.insert(new Ld::Identifier("fnord"));
    db.insert(new Ld::Identifier("foop"));

    QList<Ld::IdentifierContainer> identifiers = db.identifiers();
    QCOMPARE(identifiers.size(), 5);
    QCOMPARE(db.size(), 5U);

    for (unsigned index=0 ; index<5 ; ++index) {
        Ld::IdentifierContainer identifier = identifiers.at(index);
        QCOMPARE(identifier.handle(), index);
    }

    QCOMPARE(identifiers.at(0).text1(), QString("foo"));
    QCOMPARE(identifiers.at(1).text1(), QString("burp"));
    QCOMPARE(identifiers.at(2).text1(), QString("bang"));
    QCOMPARE(identifiers.at(3).text1(), QString("fnord"));
    QCOMPARE(identifiers.at(4).text1(), QString("foop"));

    Ld::IdentifierContainer container = db.entryByName("burp", "boop", Ld::ElementPointer());
    QCOMPARE(container.text1(), "burp");
    QCOMPARE(container.text2(), "boop");
    QCOMPARE(container.handle(), static_cast<Ld::Identifier::Handle>(2));

    container = db.entryByHandle(3);
    QCOMPARE(container.text1(), "fnord");
    QCOMPARE(container.text2().isEmpty(), true);
    QCOMPARE(container.internalName(), "Kfnord");
    QCOMPARE(container.handle(), static_cast<Ld::Identifier::Handle>(4));
}
