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
* This file implements tests of the Ld::OperationDatabase class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QString>
#include <QStringList>
#include <QSharedPointer>
#include <QList>
#include <QtTest/QtTest>

#include <ld_handle.h>
#include <ld_element_structures.h>
#include <ld_assignment_operator_element.h>
#include <ld_operation.h>
#include <ld_operation_database.h>

#include "test_operation_database.h"

TestOperationDatabase::TestOperationDatabase() {}


TestOperationDatabase::~TestOperationDatabase() {}


void TestOperationDatabase::initTestCase() {
    Ld::Handle::initialize(0x123456789ABCDEF0ULL);
}


void TestOperationDatabase::testConstructorsAndAssignmentMethods() {
    Ld::OperationDatabase db1;
    QCOMPARE(db1.isEmpty(), true);
    QCOMPARE(db1.isNotEmpty(), false);
    QCOMPARE(db1.size(), 0U);

    Ld::ElementPointer element(new Ld::AssignmentOperatorElement);
    Ld::Operation operation1 = db1.createOperation(element);

    QCOMPARE(db1.isEmpty(), false);
    QCOMPARE(db1.isNotEmpty(), true);
    QCOMPARE(db1.size(), 1U);

    Ld::OperationDatabase db2(db1);

    QCOMPARE(db2.isEmpty(), false);
    QCOMPARE(db2.isNotEmpty(), true);
    QCOMPARE(db2.size(), 1U);

    Ld::Operation operation2 = db2.fromHandle(0);
    QCOMPARE(operation1.handle(), operation2.handle());
    QCOMPARE(operation1.element(), operation2.element());

    Ld::OperationDatabase db3;
    QCOMPARE(db3.isEmpty(), true);

    db3 = db1;

    QCOMPARE(db3.isEmpty(), false);
    QCOMPARE(db3.isNotEmpty(), true);
    QCOMPARE(db3.size(), 1U);

    operation2 = db3.fromHandle(0);
    QCOMPARE(operation1.handle(), operation2.handle());
    QCOMPARE(operation1.element(), operation2.element());
}


void TestOperationDatabase::testCreateFunction() {
    Ld::OperationDatabase db;

    Ld::ElementPointer element1(new Ld::AssignmentOperatorElement);
    Ld::ElementPointer element2(new Ld::AssignmentOperatorElement);
    Ld::ElementPointer element3(new Ld::AssignmentOperatorElement);

    Ld::Operation operation1 = db.createOperation(element1);
    Ld::Operation operation2 = db.createOperation(element2);
    Ld::Operation operation3 = db.createOperation(element3);

    QCOMPARE(db.isEmpty(), false);
    QCOMPARE(db.size(), 3U);

    QCOMPARE(operation1.handle(), 0U);
    QCOMPARE(operation2.handle(), 1U);
    QCOMPARE(operation3.handle(), 2U);

    QCOMPARE(operation1.element(), element1);
    QCOMPARE(operation2.element(), element2);
    QCOMPARE(operation3.element(), element3);
}


void TestOperationDatabase::testClearFunction() {
    Ld::OperationDatabase db;

    Ld::ElementPointer element1(new Ld::AssignmentOperatorElement);
    Ld::ElementPointer element2(new Ld::AssignmentOperatorElement);
    Ld::ElementPointer element3(new Ld::AssignmentOperatorElement);

    Ld::Operation operation1 = db.createOperation(element1);
    Ld::Operation operation2 = db.createOperation(element2);
    Ld::Operation operation3 = db.createOperation(element3);

    QCOMPARE(db.isEmpty(), false);
    QCOMPARE(db.size(), 3U);

    db.clear();

    QCOMPARE(db.isEmpty(), true);
    QCOMPARE(db.size(), 0U);
}


void TestOperationDatabase::testLookupFunctions() {
    Ld::OperationDatabase db;

    Ld::ElementPointer element1(new Ld::AssignmentOperatorElement);
    Ld::ElementPointer element2(new Ld::AssignmentOperatorElement);
    Ld::ElementPointer element3(new Ld::AssignmentOperatorElement);
    Ld::ElementPointer element4(new Ld::AssignmentOperatorElement);

    Ld::Operation operation1 = db.createOperation(element1);
    Ld::Operation operation2 = db.createOperation(element2);
    Ld::Operation operation3 = db.createOperation(element3);

    QCOMPARE(db.isEmpty(), false);
    QCOMPARE(db.size(), 3U);

    QCOMPARE(db.contains(element1), true);
    QCOMPARE(db.contains(element4), false);

    QCOMPARE(db.contains(1U), true);
    QCOMPARE(db.contains(3U), false);

    QCOMPARE(db.fromElement(element1).handle(), 0U);
    QCOMPARE(db.fromElement(element2).handle(), 1U);
    QCOMPARE(db.fromElement(element4).isInvalid(), true);

    QCOMPARE(db.fromHandle(0U).handle(), 0U);
    QCOMPARE(db.fromHandle(2U).handle(), 2U);

    QCOMPARE(db.fromHandle(0U).element(), element1);
    QCOMPARE(db.fromHandle(2U).element(), element3);

    QCOMPARE(db.fromHandle(3U).isInvalid(), true);
}
