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
* This file implements tests of the Ld::Operation class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QList>
#include <QtTest/QtTest>

#include <ld_element_structures.h>
#include <ld_handle.h>
#include <ld_assignment_operator_element.h>
#include <ld_operation.h>

#include "test_operation.h"


TestOperation::TestOperation() {}

TestOperation::~TestOperation() {}

void TestOperation::initTestCase() {
    Ld::Handle::initialize(0x123456789ABCDEF0ULL);
}


void TestOperation::testConstructorsAndAssignmentMethods() {
    Ld::Operation operation1;

    QCOMPARE(operation1.isValid(), false);
    QCOMPARE(operation1.isInvalid(), true);

    Ld::ElementPointer element(new Ld::AssignmentOperatorElement);

    Ld::Operation operation2(element, 1);

    QCOMPARE(operation2.isValid(), true);
    QCOMPARE(operation2.isInvalid(), false);

    QCOMPARE(operation2.element(), element);
    QCOMPARE(operation2.handle(), 1U);

    operation1 = operation2;

    QCOMPARE(operation1.isValid(), true);
    QCOMPARE(operation1.isInvalid(), false);

    QCOMPARE(operation1.element(), element);
    QCOMPARE(operation1.handle(), 1U);
}


void TestOperation::testAccessors() {
    Ld::Operation operation1;

    QCOMPARE(operation1.isValid(), false);
    QCOMPARE(operation1.isInvalid(), true);

    QCOMPARE(operation1.element(), Ld::ElementPointer());
    QCOMPARE(operation1.handle(), Ld::Operation::invalidHandle);

    Ld::ElementPointer element(new Ld::AssignmentOperatorElement);
    operation1.setElement(element);

    QCOMPARE(operation1.isValid(), false);
    QCOMPARE(operation1.isInvalid(), true);

    QCOMPARE(operation1.element(), element);
    QCOMPARE(operation1.handle(), Ld::Operation::invalidHandle);

    operation1.setHandle(2);

    QCOMPARE(operation1.isValid(), true);
    QCOMPARE(operation1.isInvalid(), false);

    QCOMPARE(operation1.element(), element);
    QCOMPARE(operation1.handle(), 2U);

    Ld::Operation operation2;
    QCOMPARE(operation2.isValid(), false);

    operation2.setHandle(1);
    QCOMPARE(operation2.isValid(), false);

    operation2.setElement(element);
    QCOMPARE(operation2.isValid(), true);
}
