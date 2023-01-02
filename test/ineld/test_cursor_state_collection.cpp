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
* This file implements tests of the Ld::CursorStateCollection class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QSharedPointer>
#include <QtTest/QtTest>

#include <ld_element_structures.h>
#include <ld_element.h>
#include <ld_element.h>
#include <ld_element_cursor.h>
#include <ld_cursor.h>
#include <ld_cursor_collection.h>
#include <ld_cursor_weak_collection.h>
#include <ld_cursor_state_collection.h>

#include "test_element_position.h"
#include "test_cursor_state_collection.h"

TestCursorStateCollection::TestCursorStateCollection() {}


TestCursorStateCollection::~TestCursorStateCollection() {}


void TestCursorStateCollection::initTestCase() {
    Ld::Handle::initialize(0x12345678ABCDEF01);
    Ld::Element::registerCreator("PositionTest", &PositionTestElement::creator);
}


void TestCursorStateCollection::testConstructorsDestructors() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(2));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());
    element1->setNumberTextRegions(1);
    element1->setText("Test", 0);

    QSharedPointer<PositionTestElement> element2(new PositionTestElement);
    element2->setWeakThis(element2.toWeakRef());
    element2->setNumberTextRegions(0);

    rootElement->setChild(0, element1, nullptr);
    rootElement->setChild(1, element2, nullptr);

    Ld::ElementCursor elementCursor1(element1);
    Ld::ElementCursor elementCursor2(element2);
    Ld::ElementCursor elementCursor3(1, 0, element1);

    Ld::CursorPointer cursor1(new Ld::Cursor);
    cursor1->updateFromElementCursor(elementCursor1);

    Ld::CursorPointer cursor2(new Ld::Cursor);
    cursor2->updateFromElementCursor(elementCursor2);

    Ld::CursorPointer cursor3(new Ld::Cursor);
    cursor3->updateFromElementCursor(elementCursor3);

    Ld::CursorCollection collection1;
    collection1 << cursor1;
    collection1 << cursor2;
    collection1 << cursor3;

    Ld::CursorWeakCollection collection2;
    collection2 << cursor1.toWeakRef();
    collection2 << cursor2.toWeakRef();
    collection2 << cursor3.toWeakRef();

    Ld::CursorStateCollection collection3;
    QVERIFY(collection3.isEmpty());
    QVERIFY(Ld::CursorCollection(collection3).isEmpty());

    Ld::CursorStateCollection collection4(collection1);
    QVERIFY(!collection4.isEmpty());
    QCOMPARE(collection4.size(), 2 * 3);
    QCOMPARE(Ld::CursorCollection(collection4).size(), 3);
    QVERIFY(Ld::CursorCollection(collection4).contains(cursor1));
    QVERIFY(Ld::CursorCollection(collection4).contains(cursor2));
    QVERIFY(Ld::CursorCollection(collection4).contains(cursor3));

    Ld::CursorStateCollection collection5(collection2);
    QVERIFY(!collection5.isEmpty());
    QCOMPARE(collection5.size(), 2 * 3);
    QCOMPARE(Ld::CursorCollection(collection5).size(), 3);
    QVERIFY(Ld::CursorCollection(collection5).contains(cursor1));
    QVERIFY(Ld::CursorCollection(collection5).contains(cursor2));
    QVERIFY(Ld::CursorCollection(collection5).contains(cursor3));
}


void TestCursorStateCollection::testInsertAndClearMethod() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(2));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());
    element1->setNumberTextRegions(1);
    element1->setText("Test", 0);

    rootElement->setChild(0, element1, nullptr);

    Ld::ElementCursor elementCursor1(element1);

    Ld::CursorPointer cursor1(new Ld::Cursor);
    cursor1->updateFromElementCursor(elementCursor1);

    Ld::CursorStateCollection collection;
    collection.insert(cursor1);
    QCOMPARE(collection.size(), 2 * 1);
    QCOMPARE(Ld::CursorCollection(collection).size(), 1);
    QVERIFY(Ld::CursorCollection(collection).contains(cursor1));

    collection.clear();
    QVERIFY(collection.isEmpty());
    QVERIFY(Ld::CursorCollection(collection).isEmpty());
}


void TestCursorStateCollection::testCaptureCursorStateMethod() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(2));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());
    element1->setNumberTextRegions(1);
    element1->setText("Test", 0);

    QSharedPointer<PositionTestElement> element2(new PositionTestElement);
    element2->setWeakThis(element2.toWeakRef());
    element2->setNumberTextRegions(0);

    rootElement->setChild(0, element1, nullptr);
    rootElement->setChild(1, element2, nullptr);

    Ld::ElementCursor elementCursor1(element1);
    Ld::ElementCursor elementCursor2(element2);
    Ld::ElementCursor elementCursor3(1, 0, element1);

    Ld::CursorPointer cursor1(new Ld::Cursor);
    cursor1->updateFromElementCursor(elementCursor1);

    Ld::CursorStateCollection collection;
    collection.insert(cursor1);

    QCOMPARE(collection.size(), 2 * 1);
    QCOMPARE(collection.at(0).toElementCursor(), elementCursor1);
    QCOMPARE(collection.at(1).toElementCursor(), Ld::ElementCursor());

    cursor1->updateFromElementCursor(elementCursor2);
    cursor1->updateSelectionFromElementCursor(elementCursor3);

    collection.captureCursorState();

    QCOMPARE(collection.size(), 2 * 1);
    QCOMPARE(collection.at(0).toElementCursor(), elementCursor2);
    QCOMPARE(collection.at(1).toElementCursor(), elementCursor3);
}


void TestCursorStateCollection::testUpdateCursorStateMethod() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(2));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());
    element1->setNumberTextRegions(1);
    element1->setText("Test", 0);

    QSharedPointer<PositionTestElement> element2(new PositionTestElement);
    element2->setWeakThis(element2.toWeakRef());
    element2->setNumberTextRegions(0);

    rootElement->setChild(0, element1, nullptr);
    rootElement->setChild(1, element2, nullptr);

    Ld::ElementCursor elementCursor1(element1);
    Ld::ElementCursor elementCursor2(element2);
    Ld::ElementCursor elementCursor3(1, 0, element1);

    Ld::CursorPointer cursor1(new Ld::Cursor);
    cursor1->updateFromElementCursor(elementCursor1);

    Ld::CursorStateCollection collection;
    collection.insert(cursor1);

    QCOMPARE(collection.size(), 2 * 1);
    QCOMPARE(collection.at(0).toElementCursor(), elementCursor1);
    QCOMPARE(collection.at(1).toElementCursor(), Ld::ElementCursor());

    collection[0].update(elementCursor2);
    collection[1].update(elementCursor3);

    collection.updateCursorState(false);

    QCOMPARE(cursor1->elementCursor(), elementCursor2);
    QCOMPARE(cursor1->selectionCursor(), elementCursor3);

    collection[0].update(elementCursor2);
    collection[1].update(elementCursor2);

    collection.updateCursorState(false);

    QCOMPARE(cursor1->elementCursor(), elementCursor2);
    QVERIFY(cursor1->selectionCursor().isInvalid());
}


void TestCursorStateCollection::testAssignmentOperators() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(2));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());
    element1->setNumberTextRegions(1);
    element1->setText("Test", 0);

    QSharedPointer<PositionTestElement> element2(new PositionTestElement);
    element2->setWeakThis(element2.toWeakRef());
    element2->setNumberTextRegions(0);

    rootElement->setChild(0, element1, nullptr);
    rootElement->setChild(1, element2, nullptr);

    Ld::ElementCursor elementCursor1(element1);
    Ld::ElementCursor elementCursor2(element2);

    Ld::CursorPointer cursor1(new Ld::Cursor);
    cursor1->updateFromElementCursor(elementCursor1);

    Ld::CursorPointer cursor2(new Ld::Cursor);
    cursor2->updateFromElementCursor(elementCursor2);

    Ld::CursorStateCollection collection1;
    Ld::CursorStateCollection collection2;
    collection2.insert(cursor1);
    collection2.insert(cursor2);

    QVERIFY(collection1.isEmpty());
    QVERIFY(Ld::CursorCollection(collection1).isEmpty());

    QCOMPARE(collection2.size(), 2 * 2);
    QCOMPARE(Ld::CursorCollection(collection2).size(), 2);

    collection1 = collection2;

    QCOMPARE(collection1.size(), 2 * 2);
    QCOMPARE(Ld::CursorCollection(collection1).size(), 2);
}
