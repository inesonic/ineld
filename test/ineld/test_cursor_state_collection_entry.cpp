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
* This file implements tests of the Ld::CursorStateCollectionEntry class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QSharedPointer>
#include <QtTest/QtTest>

#include <ld_element_structures.h>
#include <ld_element.h>
#include <ld_element.h>
#include <ld_element_cursor.h>
#include <ld_cursor_state_collection_entry.h>

#include "test_element_position.h"
#include "test_cursor_state_collection_entry.h"

TestCursorStateCollectionEntry::TestCursorStateCollectionEntry() {}


TestCursorStateCollectionEntry::~TestCursorStateCollectionEntry() {}


void TestCursorStateCollectionEntry::initTestCase() {
    Ld::Handle::initialize(0x12345678ABCDEF01);
    Ld::Element::registerCreator("PositionTest", &PositionTestElement::creator);
}


void TestCursorStateCollectionEntry::testConstructorsDestructors() {
    Ld::CursorStateCollectionEntry entry1;
    QCOMPARE(entry1.isValid(), false);

    Ld::ElementCursor elementCursor2;
    Ld::CursorStateCollectionEntry entry2(elementCursor2);
    QCOMPARE(entry2.toElementCursor(), elementCursor2);

    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(1));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());
    element1->setNumberTextRegions(1);
    element1->setText("Test", 0);

    rootElement->setChild(0, element1, nullptr);

    Ld::ElementCursor elementCursor3(element1);
    Ld::CursorStateCollectionEntry entry3(elementCursor3);
    QCOMPARE(entry3.toElementCursor(), elementCursor3);

    Ld::ElementCursor elementCursor4(rootElement, 0);
    Ld::CursorStateCollectionEntry entry4(elementCursor4);
    QCOMPARE(entry4.toElementCursor(), elementCursor4);

    Ld::ElementCursor elementCursor5(1, 0, element1);
    Ld::CursorStateCollectionEntry entry5(elementCursor5);
    QCOMPARE(entry5.toElementCursor(), elementCursor5);

    Ld::CursorStateCollectionEntry entry6(entry5);
    QCOMPARE(entry6.toElementCursor(), elementCursor5);
}


void TestCursorStateCollectionEntry::testValidInvalidMethods() {
    Ld::CursorStateCollectionEntry entry1;
    QCOMPARE(entry1.isValid(), false);
    QCOMPARE(entry1.isInvalid(), true);

    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(1));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());
    element1->setNumberTextRegions(1);
    element1->setText("Test", 0);

    rootElement->setChild(0, element1, nullptr);

    Ld::ElementCursor elementCursor2(element1);
    Ld::CursorStateCollectionEntry entry2(elementCursor2);
    QCOMPARE(entry2.isValid(), true);
    QCOMPARE(entry2.isInvalid(), false);

    Ld::ElementCursor elementCursor3(rootElement, 0);
    Ld::CursorStateCollectionEntry entry3(elementCursor3);
    QCOMPARE(entry3.isValid(), true);
    QCOMPARE(entry3.isInvalid(), false);

    Ld::ElementCursor elementCursor4(1, 0, element1);
    Ld::CursorStateCollectionEntry entry4(elementCursor4);
    QCOMPARE(entry4.isValid(), true);
    QCOMPARE(entry4.isInvalid(), false);
}


void TestCursorStateCollectionEntry::testInvalidCursorTracking() {
    Ld::CursorStateCollectionEntry entry;

    QCOMPARE(entry.isValid(), false);
    QCOMPARE(entry.isInvalid(), true);
    QCOMPARE(entry.isWholeElement(), false);
    QCOMPARE(entry.isTextInElement(), false);
    QCOMPARE(entry.positionType(), Ld::CursorStateCollectionEntry::PositionType::INVALID);

    QVERIFY(entry.element().isNull());
    QVERIFY(entry.parent().isNull());
    QCOMPARE(entry.childIndex(), Ld::Element::invalidChildIndex);
    QCOMPARE(entry.textIndex(), Ld::Element::invalidTextIndex);
    QCOMPARE(entry.regionIndex(), Ld::Element::invalidRegionIndex);

    QVERIFY(entry.toElementCursor().isInvalid());
}


void TestCursorStateCollectionEntry::testWholeElementCursorTracking() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(1));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());
    element1->setNumberTextRegions(1);
    element1->setText("Test", 0);

    rootElement->setChild(0, element1, nullptr);

    Ld::ElementCursor elementCursor(element1);
    Ld::CursorStateCollectionEntry entry(elementCursor);

    QCOMPARE(entry.isValid(), true);
    QCOMPARE(entry.isInvalid(), false);
    QCOMPARE(entry.isWholeElement(), true);
    QCOMPARE(entry.isTextInElement(), false);
    QCOMPARE(entry.positionType(), Ld::CursorStateCollectionEntry::PositionType::ELEMENT);

    QCOMPARE(entry.element(), element1);
    QCOMPARE(entry.parent(), rootElement);
    QCOMPARE(entry.childIndex(), 0U);
    QCOMPARE(entry.textIndex(), Ld::Element::invalidTextIndex);
    QCOMPARE(entry.regionIndex(), Ld::Element::invalidRegionIndex);

    QCOMPARE(entry.toElementCursor(), elementCursor);
}


void TestCursorStateCollectionEntry::testNullEntryCursorTracking() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(2));
    rootElement->setWeakThis(rootElement.toWeakRef());

    Ld::ElementCursor elementCursor(rootElement, 1);
    Ld::CursorStateCollectionEntry entry(elementCursor);

    QCOMPARE(entry.isValid(), true);
    QCOMPARE(entry.isInvalid(), false);
    QCOMPARE(entry.isWholeElement(), true);
    QCOMPARE(entry.isTextInElement(), false);
    QCOMPARE(entry.positionType(), Ld::CursorStateCollectionEntry::PositionType::ELEMENT);

    QVERIFY(entry.element().isNull());
    QCOMPARE(entry.parent(), rootElement);
    QCOMPARE(entry.childIndex(), 1U);
    QCOMPARE(entry.textIndex(), Ld::Element::invalidTextIndex);
    QCOMPARE(entry.regionIndex(), Ld::Element::invalidRegionIndex);

    QCOMPARE(entry.toElementCursor(), elementCursor);
}


void TestCursorStateCollectionEntry::testTextLocationCursorTracking() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(1));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());
    element1->setNumberTextRegions(1);
    element1->setText("Test", 0);

    rootElement->setChild(0, element1, nullptr);

    Ld::ElementCursor elementCursor(1, 0, element1);
    Ld::CursorStateCollectionEntry entry(elementCursor);

    QCOMPARE(entry.isValid(), true);
    QCOMPARE(entry.isInvalid(), false);
    QCOMPARE(entry.isWholeElement(), false);
    QCOMPARE(entry.isTextInElement(), true);
    QCOMPARE(entry.positionType(), Ld::CursorStateCollectionEntry::PositionType::TEXT);

    QCOMPARE(entry.element(), element1);
    QCOMPARE(entry.parent(), rootElement);
    QCOMPARE(entry.childIndex(), 0U);
    QCOMPARE(entry.textIndex(), 1U);
    QCOMPARE(entry.regionIndex(), 0U);

    QCOMPARE(entry.toElementCursor(), elementCursor);
}


void TestCursorStateCollectionEntry::testInvalidateMethod() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(1));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());
    element1->setNumberTextRegions(1);
    element1->setText("Test", 0);

    rootElement->setChild(0, element1, nullptr);

    Ld::ElementCursor elementCursor(1, 0, element1);
    Ld::CursorStateCollectionEntry entry(elementCursor);

    entry.invalidate();

    QCOMPARE(entry.isValid(), false);
    QCOMPARE(entry.isInvalid(), true);
    QCOMPARE(entry.isWholeElement(), false);
    QCOMPARE(entry.isTextInElement(), false);
    QCOMPARE(entry.positionType(), Ld::CursorStateCollectionEntry::PositionType::INVALID);

    QVERIFY(entry.element().isNull());
    QVERIFY(entry.parent().isNull());
    QCOMPARE(entry.childIndex(), Ld::Element::invalidChildIndex);
    QCOMPARE(entry.textIndex(), Ld::Element::invalidTextIndex);
    QCOMPARE(entry.regionIndex(), Ld::Element::invalidRegionIndex);

    QVERIFY(entry.toElementCursor().isInvalid());
}


void TestCursorStateCollectionEntry::testUpdateFromCursorMethod() {
    Ld::CursorStateCollectionEntry entry;

    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(2));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());
    element1->setNumberTextRegions(1);
    element1->setText("Test", 0);

    rootElement->setChild(0, element1, nullptr);

    Ld::ElementCursor elementCursor1;
    Ld::ElementCursor elementCursor2(element1);
    Ld::ElementCursor elementCursor3(rootElement, 1);
    Ld::ElementCursor elementCursor4(1, 0, element1);

    // Invalid cursor

    entry.update(elementCursor1);

    QCOMPARE(entry.isValid(), false);
    QCOMPARE(entry.isInvalid(), true);
    QCOMPARE(entry.isWholeElement(), false);
    QCOMPARE(entry.isTextInElement(), false);
    QCOMPARE(entry.positionType(), Ld::CursorStateCollectionEntry::PositionType::INVALID);

    QVERIFY(entry.element().isNull());
    QVERIFY(entry.parent().isNull());
    QCOMPARE(entry.childIndex(), Ld::Element::invalidChildIndex);
    QCOMPARE(entry.textIndex(), Ld::Element::invalidTextIndex);
    QCOMPARE(entry.regionIndex(), Ld::Element::invalidRegionIndex);

    QVERIFY(entry.toElementCursor().isInvalid());

    // Whole element cursor

    entry.invalidate();
    entry.update(elementCursor2);

    QCOMPARE(entry.isValid(), true);
    QCOMPARE(entry.isInvalid(), false);
    QCOMPARE(entry.isWholeElement(), true);
    QCOMPARE(entry.isTextInElement(), false);
    QCOMPARE(entry.positionType(), Ld::CursorStateCollectionEntry::PositionType::ELEMENT);

    QCOMPARE(entry.element(), element1);
    QCOMPARE(entry.parent(), rootElement);
    QCOMPARE(entry.childIndex(), 0U);
    QCOMPARE(entry.textIndex(), Ld::Element::invalidTextIndex);
    QCOMPARE(entry.regionIndex(), Ld::Element::invalidRegionIndex);

    QCOMPARE(entry.toElementCursor(), elementCursor2);

    // Null element cursor

    entry.invalidate();
    entry.update(elementCursor3);

    QCOMPARE(entry.isValid(), true);
    QCOMPARE(entry.isInvalid(), false);
    QCOMPARE(entry.isWholeElement(), true);
    QCOMPARE(entry.isTextInElement(), false);
    QCOMPARE(entry.positionType(), Ld::CursorStateCollectionEntry::PositionType::ELEMENT);

    QVERIFY(entry.element().isNull());
    QCOMPARE(entry.parent(), rootElement);
    QCOMPARE(entry.childIndex(), 1U);
    QCOMPARE(entry.textIndex(), Ld::Element::invalidTextIndex);
    QCOMPARE(entry.regionIndex(), Ld::Element::invalidRegionIndex);

    QCOMPARE(entry.toElementCursor(), elementCursor3);

    // Text location cursor

    entry.invalidate();
    entry.update(elementCursor4);

    QCOMPARE(entry.isValid(), true);
    QCOMPARE(entry.isInvalid(), false);
    QCOMPARE(entry.isWholeElement(), false);
    QCOMPARE(entry.isTextInElement(), true);
    QCOMPARE(entry.positionType(), Ld::CursorStateCollectionEntry::PositionType::TEXT);

    QCOMPARE(entry.element(), element1);
    QCOMPARE(entry.parent(), rootElement);
    QCOMPARE(entry.childIndex(), 0U);
    QCOMPARE(entry.textIndex(), 1U);
    QCOMPARE(entry.regionIndex(), 0U);

    QCOMPARE(entry.toElementCursor(), elementCursor4);
}


void TestCursorStateCollectionEntry::testUpdateWholeElementMethod() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(2));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());
    element1->setNumberTextRegions(1);
    element1->setText("Test", 0);

    rootElement->setChild(0, element1, nullptr);

    Ld::CursorStateCollectionEntry entry;
    QCOMPARE(entry.isInvalid(), true);

    entry.update(element1, true);

    QCOMPARE(entry.isValid(), true);
    QCOMPARE(entry.isInvalid(), false);
    QCOMPARE(entry.isWholeElement(), true);
    QCOMPARE(entry.isTextInElement(), false);
    QCOMPARE(entry.positionType(), Ld::CursorStateCollectionEntry::PositionType::ELEMENT);

    QCOMPARE(entry.element(), element1);
    QCOMPARE(entry.parent(), rootElement);
    QCOMPARE(entry.childIndex(), 0U);
    QCOMPARE(entry.textIndex(), Ld::Element::invalidTextIndex);
    QCOMPARE(entry.regionIndex(), Ld::Element::invalidRegionIndex);

    QCOMPARE(entry.toElementCursor(), Ld::ElementCursor(element1));
}


void TestCursorStateCollectionEntry::testUpdateNullEntryMethod() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(2));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());
    element1->setNumberTextRegions(1);
    element1->setText("Test", 0);

    rootElement->setChild(0, element1, nullptr);

    Ld::CursorStateCollectionEntry entry;
    QCOMPARE(entry.isInvalid(), true);

    entry.update(rootElement, 1);

    QCOMPARE(entry.isValid(), true);
    QCOMPARE(entry.isInvalid(), false);
    QCOMPARE(entry.isWholeElement(), true);
    QCOMPARE(entry.isTextInElement(), false);
    QCOMPARE(entry.positionType(), Ld::CursorStateCollectionEntry::PositionType::ELEMENT);

    QVERIFY(entry.element().isNull());
    QCOMPARE(entry.parent(), rootElement);
    QCOMPARE(entry.childIndex(), 1U);
    QCOMPARE(entry.textIndex(), Ld::Element::invalidTextIndex);
    QCOMPARE(entry.regionIndex(), Ld::Element::invalidRegionIndex);

    QCOMPARE(entry.toElementCursor(), Ld::ElementCursor(rootElement, 1));
}


void TestCursorStateCollectionEntry::testUpdateTextLocationRegion0Method() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(2));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());
    element1->setNumberTextRegions(1);
    element1->setText("Test", 0);

    rootElement->setChild(0, element1, nullptr);

    Ld::CursorStateCollectionEntry entry;
    QCOMPARE(entry.isInvalid(), true);

    entry.update(1, element1);

    QCOMPARE(entry.isValid(), true);
    QCOMPARE(entry.isInvalid(), false);
    QCOMPARE(entry.isWholeElement(), false);
    QCOMPARE(entry.isTextInElement(), true);
    QCOMPARE(entry.positionType(), Ld::CursorStateCollectionEntry::PositionType::TEXT);

    QCOMPARE(entry.element(), element1);
    QCOMPARE(entry.parent(), rootElement);
    QCOMPARE(entry.childIndex(), 0U);
    QCOMPARE(entry.textIndex(), 1U);
    QCOMPARE(entry.regionIndex(), 0U);

    QCOMPARE(entry.toElementCursor(), Ld::ElementCursor(1, 0, element1));
}


void TestCursorStateCollectionEntry::testUpdateTextLocationMethod() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(2));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());
    element1->setNumberTextRegions(1);
    element1->setText("Test", 0);

    rootElement->setChild(0, element1, nullptr);

    Ld::CursorStateCollectionEntry entry;
    QCOMPARE(entry.isInvalid(), true);

    entry.update(2, 0, element1);

    QCOMPARE(entry.isValid(), true);
    QCOMPARE(entry.isInvalid(), false);
    QCOMPARE(entry.isWholeElement(), false);
    QCOMPARE(entry.isTextInElement(), true);
    QCOMPARE(entry.positionType(), Ld::CursorStateCollectionEntry::PositionType::TEXT);

    QCOMPARE(entry.element(), element1);
    QCOMPARE(entry.parent(), rootElement);
    QCOMPARE(entry.childIndex(), 0U);
    QCOMPARE(entry.textIndex(), 2U);
    QCOMPARE(entry.regionIndex(), 0U);

    QCOMPARE(entry.toElementCursor(), Ld::ElementCursor(2, 0, element1));
}


void TestCursorStateCollectionEntry::testUpdateToFirstPositionInMethod() {
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

    Ld::CursorStateCollectionEntry entry1;
    entry1.updateToFirstPositionIn(element1, true);
    QCOMPARE(entry1.toElementCursor(), Ld::ElementCursor(0, 0, element1));

    Ld::CursorStateCollectionEntry entry2;
    entry2.updateToFirstPositionIn(element2, true);
    QCOMPARE(entry2.toElementCursor(), Ld::ElementCursor(element2));
}


void TestCursorStateCollectionEntry::testUpdateToLastPositionInMethod() {
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

    Ld::CursorStateCollectionEntry entry1;
    entry1.updateToLastPositionIn(element1, true);
    QCOMPARE(entry1.toElementCursor(), Ld::ElementCursor(4, 0, element1));

    Ld::CursorStateCollectionEntry entry2;
    entry2.updateToLastPositionIn(element2, true);
    QCOMPARE(entry2.toElementCursor(), Ld::ElementCursor(element2));
}


void TestCursorStateCollectionEntry::testAssignmentOperator() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(2));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());
    element1->setNumberTextRegions(1);
    element1->setText("Test", 0);

    rootElement->setChild(0, element1, nullptr);

    Ld::ElementCursor elementCursor1;
    Ld::ElementCursor elementCursor2(element1);
    Ld::ElementCursor elementCursor3(rootElement, 1);
    Ld::ElementCursor elementCursor4(1, 0, element1);

    Ld::CursorStateCollectionEntry entry1(elementCursor1);
    QCOMPARE(entry1.toElementCursor(), elementCursor1);

    Ld::CursorStateCollectionEntry entry2(elementCursor2);
    QCOMPARE(entry2.toElementCursor(), elementCursor2);

    entry1 = entry2;
    QCOMPARE(entry1.toElementCursor(), elementCursor2);

    entry1.update(elementCursor3);
    QCOMPARE(entry1.toElementCursor(), elementCursor3);
    QCOMPARE(entry2.toElementCursor(), elementCursor2);

    entry1 = entry2;
    QCOMPARE(entry1.toElementCursor(), elementCursor2);

    entry2.update(elementCursor4);
    QCOMPARE(entry1.toElementCursor(), elementCursor2);
    QCOMPARE(entry2.toElementCursor(), elementCursor4);
}


void TestCursorStateCollectionEntry::testComparisonOperators() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(2));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());
    element1->setNumberTextRegions(1);
    element1->setText("Test", 0);

    rootElement->setChild(0, element1, nullptr);

    Ld::ElementCursor elementCursor1;
    Ld::ElementCursor elementCursor2(element1);
    Ld::ElementCursor elementCursor3(rootElement, 1);
    Ld::ElementCursor elementCursor4(1, 0, element1);

    Ld::CursorStateCollectionEntry entry1a(elementCursor1);
    Ld::CursorStateCollectionEntry entry1b(elementCursor1);

    Ld::CursorStateCollectionEntry entry2a(elementCursor2);
    Ld::CursorStateCollectionEntry entry2b(elementCursor2);

    Ld::CursorStateCollectionEntry entry3a(elementCursor3);
    Ld::CursorStateCollectionEntry entry3b(elementCursor3);

    Ld::CursorStateCollectionEntry entry4a(elementCursor4);
    Ld::CursorStateCollectionEntry entry4b(elementCursor4);

    QCOMPARE(entry1a == entry1b, true);
    QCOMPARE(entry1a == entry2a, false);
    QCOMPARE(entry1a == entry3a, false);
    QCOMPARE(entry1a == entry4a, false);

    QCOMPARE(entry1a != entry1b, false);
    QCOMPARE(entry1a != entry2a, true);
    QCOMPARE(entry1a != entry3a, true);
    QCOMPARE(entry1a != entry4a, true);


    QCOMPARE(entry2a == entry1a, false);
    QCOMPARE(entry2a == entry2b, true);
    QCOMPARE(entry2a == entry3a, false);
    QCOMPARE(entry2a == entry4a, false);

    QCOMPARE(entry2a != entry1a, true);
    QCOMPARE(entry2a != entry2b, false);
    QCOMPARE(entry2a != entry3a, true);
    QCOMPARE(entry2a != entry4a, true);


    QCOMPARE(entry3a == entry1a, false);
    QCOMPARE(entry3a == entry2a, false);
    QCOMPARE(entry3a == entry3b, true);
    QCOMPARE(entry3a == entry4a, false);

    QCOMPARE(entry3a != entry1a, true);
    QCOMPARE(entry3a != entry2a, true);
    QCOMPARE(entry3a != entry3b, false);
    QCOMPARE(entry3a != entry4a, true);


    QCOMPARE(entry4a == entry1a, false);
    QCOMPARE(entry4a == entry2a, false);
    QCOMPARE(entry4a == entry3a, false);
    QCOMPARE(entry4a == entry4b, true);

    QCOMPARE(entry4a != entry1a, true);
    QCOMPARE(entry4a != entry2a, true);
    QCOMPARE(entry4a != entry3a, true);
    QCOMPARE(entry4a != entry4b, false);
}
