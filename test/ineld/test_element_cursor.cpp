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
* This file implements tests of the Ld::ElementCursor class.  Also tests core functions of the
* Ld::Element class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QList>
#include <QSet>
#include <QtTest/QtTest>

#include <ld_handle.h>
#include <ld_element_structures.h>
#include <ld_element.h>
#include <ld_paragraph_element.h>
#include <ld_paragraph_format.h>
#include <ld_text_element.h>
#include <ld_character_format.h>
#include <ld_element_cursor.h>

#include "test_element_position.h"
#include "test_element_cursor.h"

/***********************************************************************************************************************
 * TestElementCursor:
 */

TestElementCursor::TestElementCursor() {}


TestElementCursor::~TestElementCursor() {}


void TestElementCursor::initTestCase() {
    Ld::Handle::initialize(0x12345678ABCDEF01);
    Ld::Element::registerCreator("PositionTest", &PositionTestElement::creator);

    Ld::Element::registerCreator(Ld::ParagraphElement::elementName, Ld::ParagraphElement::creator);
    Ld::Element::registerCreator(Ld::TextElement::elementName, Ld::TextElement::creator);

    Ld::Format::registerCreator(Ld::ParagraphFormat::formatName, Ld::ParagraphFormat::creator);
    Ld::Format::registerCreator(Ld::CharacterFormat::formatName, Ld::CharacterFormat::creator);
}


void TestElementCursor::testConstructorsDestructors() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(1));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());

    rootElement->setChild(0, element1, nullptr);

    element1->setNumberTextRegions(1);
    element1->setText("Test", 0);

    Ld::ElementCursor cursor1;
    QCOMPARE(cursor1.isValid(), false);

    Ld::ElementCursor cursor2(element1);
    QCOMPARE(cursor2.isValid(), true);

    QCOMPARE(cursor2.element(), element1);
    QCOMPARE(cursor2.parent(), rootElement);
    QCOMPARE(cursor2.childIndex(), 0U);
    QCOMPARE(cursor2.regionIndex(), Ld::Element::invalidRegionIndex);
    QCOMPARE(cursor2.textIndex(), Ld::Element::invalidTextIndex);
    QCOMPARE(cursor2.positionType(), Ld::ElementCursor::PositionType::ELEMENT);

    Ld::ElementCursor cursor3(rootElement, 0);
    QCOMPARE(cursor3.isValid(), true);

    QCOMPARE(cursor3.element(), element1);
    QCOMPARE(cursor3.parent(), rootElement);
    QCOMPARE(cursor3.childIndex(), 0U);
    QCOMPARE(cursor2.regionIndex(), Ld::Element::invalidRegionIndex);
    QCOMPARE(cursor2.textIndex(), Ld::Element::invalidTextIndex);
    QCOMPARE(cursor3.positionType(), Ld::ElementCursor::PositionType::ELEMENT);

    Ld::ElementCursor cursor4(1, 0, element1);
    QCOMPARE(cursor4.isValid(), true);

    QCOMPARE(cursor4.element(), element1);
    QCOMPARE(cursor4.parent(), rootElement);
    QCOMPARE(cursor4.childIndex(), 0U);
    QCOMPARE(cursor4.regionIndex(), 0U);
    QCOMPARE(cursor4.textIndex(), 1U);
    QCOMPARE(cursor4.positionType(), Ld::ElementCursor::PositionType::TEXT);

    Ld::ElementCursor cursor5(1, 0, rootElement, 0);
    QCOMPARE(cursor5.isValid(), true);

    QCOMPARE(cursor5.element(), element1);
    QCOMPARE(cursor5.parent(), rootElement);
    QCOMPARE(cursor5.childIndex(), 0U);
    QCOMPARE(cursor5.regionIndex(), 0U);
    QCOMPARE(cursor5.textIndex(), 1U);
    QCOMPARE(cursor5.positionType(), Ld::ElementCursor::PositionType::TEXT);

    Ld::ElementCursor cursor6(cursor5);
    QCOMPARE(cursor6.isValid(), true);

    QCOMPARE(cursor6.element(), element1);
    QCOMPARE(cursor6.parent(), rootElement);
    QCOMPARE(cursor6.childIndex(), 0U);
    QCOMPARE(cursor6.regionIndex(), 0U);
    QCOMPARE(cursor6.textIndex(), 1U);
    QCOMPARE(cursor6.positionType(), Ld::ElementCursor::PositionType::TEXT);
}


void TestElementCursor::testValidInvalidMethods() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(1));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());

    rootElement->setChild(0, element1, nullptr);

    element1->setNumberTextRegions(1);
    element1->setText("Test", 0);

    Ld::ElementCursor cursor;
    QCOMPARE(cursor.isValid(), false);
    QCOMPARE(cursor.isInvalid(), true);

    cursor.setElement(element1);
    QCOMPARE(cursor.isValid(), true);
    QCOMPARE(cursor.isInvalid(), false);

    element1->setText(QString("1234"));
    cursor.setPositionInElement(4, 0);
    QCOMPARE(cursor.isValid(), true);
    QCOMPARE(cursor.isInvalid(), false);

    element1->setText(QString("123"));
    QCOMPARE(cursor.isValid(), false);
    QCOMPARE(cursor.isInvalid(), true);
}


void TestElementCursor::testPositionTypeMethod() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(1));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());

    rootElement->setChild(0, element1, nullptr);

    element1->setNumberTextRegions(1);
    element1->setText("Test", 0);

    Ld::ElementCursor cursor;
    QCOMPARE(cursor.isValid(), false);
    QCOMPARE(cursor.positionType(), Ld::ElementCursor::PositionType::INVALID);

    cursor.setElement(element1);
    QCOMPARE(cursor.isValid(), true);
    QCOMPARE(cursor.positionType(), Ld::ElementCursor::PositionType::ELEMENT);

    element1->setText(QString("1234"));
    cursor.setPositionInElement(4, 0);
    QCOMPARE(cursor.isValid(), true);
    QCOMPARE(cursor.positionType(), Ld::ElementCursor::PositionType::TEXT);

    element1->setText(QString("123"));
    QCOMPARE(cursor.isValid(), false);
    QCOMPARE(cursor.positionType(), Ld::ElementCursor::PositionType::INVALID);
}


void TestElementCursor::testAccessors() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(1));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());

    rootElement->setChild(0, element1, nullptr);

    element1->setNumberTextRegions(1);
    element1->setText("Test", 0);

    Ld::ElementCursor cursor;
    QCOMPARE(cursor.isValid(), false);

    QVERIFY(cursor.element().isNull());
    QVERIFY(cursor.parent().isNull());
    QCOMPARE(cursor.childIndex(), Ld::Element::invalidChildIndex);
    QCOMPARE(cursor.regionIndex(), Ld::Element::invalidRegionIndex);
    QCOMPARE(cursor.textIndex(), Ld::Element::invalidTextIndex);
    QCOMPARE(cursor.positionType(), Ld::ElementCursor::PositionType::INVALID);

    cursor.setElement(element1);
    QCOMPARE(cursor.isValid(), true);

    QCOMPARE(cursor.element(), element1);
    QCOMPARE(cursor.parent(), rootElement);
    QCOMPARE(cursor.childIndex(), 0U);
    QCOMPARE(cursor.regionIndex(), Ld::Element::invalidRegionIndex);
    QCOMPARE(cursor.textIndex(), Ld::Element::invalidTextIndex);
    QCOMPARE(cursor.positionType(), Ld::ElementCursor::PositionType::ELEMENT);

    cursor.setElementUnderParent(rootElement, 0);

    QCOMPARE(cursor.element(), element1);
    QCOMPARE(cursor.parent(), rootElement);
    QCOMPARE(cursor.childIndex(), 0U);
    QCOMPARE(cursor.regionIndex(), Ld::Element::invalidRegionIndex);
    QCOMPARE(cursor.textIndex(), Ld::Element::invalidTextIndex);
    QCOMPARE(cursor.positionType(), Ld::ElementCursor::PositionType::ELEMENT);

    cursor.setPositionInElement(1, 0);

    QCOMPARE(cursor.element(), element1);
    QCOMPARE(cursor.parent(), rootElement);
    QCOMPARE(cursor.childIndex(), 0U);
    QCOMPARE(cursor.regionIndex(), 0U);
    QCOMPARE(cursor.textIndex(), 1U);
    QCOMPARE(cursor.positionType(), Ld::ElementCursor::PositionType::TEXT);

    cursor.setWholeElement();
    QCOMPARE(cursor.isValid(), true);

    QCOMPARE(cursor.element(), element1);
    QCOMPARE(cursor.parent(), rootElement);
    QCOMPARE(cursor.childIndex(), 0U);
    QCOMPARE(cursor.regionIndex(), Ld::Element::invalidRegionIndex);
    QCOMPARE(cursor.textIndex(), Ld::Element::invalidTextIndex);
    QCOMPARE(cursor.positionType(), Ld::ElementCursor::PositionType::ELEMENT);
}


void TestElementCursor::testAssignmentOperator() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(1));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());

    rootElement->setChild(0, element1, nullptr);

    element1->setNumberTextRegions(1);
    element1->setText("Test", 0);

    Ld::ElementCursor cursor1;
    QCOMPARE(cursor1.isValid(), false);

    Ld::ElementCursor cursor2(element1);
    cursor2.setPositionInElement(1, 0);
    QCOMPARE(cursor2.isValid(), true);

    QCOMPARE(cursor2.element(), element1);
    QCOMPARE(cursor2.parent(), rootElement);
    QCOMPARE(cursor2.childIndex(), 0U);
    QCOMPARE(cursor2.regionIndex(), 0U);
    QCOMPARE(cursor2.textIndex(), 1U);
    QCOMPARE(cursor2.positionType(), Ld::ElementCursor::PositionType::TEXT);

    cursor1 = cursor2;

    QCOMPARE(cursor1.element(), element1);
    QCOMPARE(cursor1.parent(), rootElement);
    QCOMPARE(cursor1.childIndex(), 0U);
    QCOMPARE(cursor1.regionIndex(), 0U);
    QCOMPARE(cursor1.textIndex(), 1U);
    QCOMPARE(cursor1.positionType(), Ld::ElementCursor::PositionType::TEXT);
}


void TestElementCursor::testMovementByElementMethods() {
    /* Tree structure:
     *
     *                   R
     *                  / \
     *                /     \
     *               /       \
     *              1         2
     *             / \       /|\
     *            /   \     / | \
     *           3     4   5  6  (null)
     *                / \
     *               /   \
     *              7     8
     */

    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(2, "R"));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement(2, "1"));
    element1->setWeakThis(element1.toWeakRef());
    rootElement->setChild(0, element1, nullptr);

    QSharedPointer<PositionTestElement> element2(new PositionTestElement(3, "2"));
    element2->setWeakThis(element2.toWeakRef());
    rootElement->setChild(1, element2, nullptr);

    QSharedPointer<PositionTestElement> element3(new PositionTestElement(0, "3"));
    element3->setWeakThis(element3.toWeakRef());
    element1->setChild(0, element3, nullptr);

    QSharedPointer<PositionTestElement> element4(new PositionTestElement(2, "4"));
    element4->setWeakThis(element4.toWeakRef());
    element1->setChild(1, element4, nullptr);

    QSharedPointer<PositionTestElement> element5(new PositionTestElement(0, "5"));
    element5->setWeakThis(element5.toWeakRef());
    element2->setChild(0, element5, nullptr);

    QSharedPointer<PositionTestElement> element6(new PositionTestElement(0, "6"));
    element6->setWeakThis(element6.toWeakRef());
    element2->setChild(1, element6, nullptr);

    QSharedPointer<PositionTestElement> element7(new PositionTestElement(0, "7"));
    element7->setWeakThis(element7.toWeakRef());
    element4->setChild(0, element7, nullptr);

    QSharedPointer<PositionTestElement> element8(new PositionTestElement(0, "8"));
    element8->setWeakThis(element8.toWeakRef());
    element4->setChild(1, element8, nullptr);

    Ld::ElementCursor cursor(element1);

    QList<QString> traversedNodes;
    bool isLast = false;
    while (!isLast) {
        Ld::ElementPointer currentNode = cursor.element();

        QString identifier;
        if (currentNode.isNull()) {
            identifier = QString("(null)");
        } else {
            identifier = currentNode.dynamicCast<PositionTestElement>()->identifier();
        }

        traversedNodes.append(identifier);
        isLast = cursor.moveForwardByElement();
    }

    QCOMPARE(traversedNodes.size(), 9);
    QCOMPARE(traversedNodes.at(0), "1");
    QCOMPARE(traversedNodes.at(1), "3");
    QCOMPARE(traversedNodes.at(2), "4");
    QCOMPARE(traversedNodes.at(3), "7");
    QCOMPARE(traversedNodes.at(4), "8");
    QCOMPARE(traversedNodes.at(5), "2");
    QCOMPARE(traversedNodes.at(6), "5");
    QCOMPARE(traversedNodes.at(7), "6");
    QCOMPARE(traversedNodes.at(8), "(null)");

    traversedNodes.clear();

    bool isFirst = false;
    while (!isFirst) {
        Ld::ElementPointer currentNode = cursor.element();

        QString identifier;
        if (currentNode.isNull()) {
            identifier = QString("(null)");
        } else {
            identifier = currentNode.dynamicCast<PositionTestElement>()->identifier();
        }

        traversedNodes.append(identifier);
        isFirst = cursor.moveBackwardByElement();
    }

    QCOMPARE(traversedNodes.size(), 9);
    QCOMPARE(traversedNodes.at(0), "(null)");
    QCOMPARE(traversedNodes.at(1), "6");
    QCOMPARE(traversedNodes.at(2), "5");
    QCOMPARE(traversedNodes.at(3), "2");
    QCOMPARE(traversedNodes.at(4), "8");
    QCOMPARE(traversedNodes.at(5), "7");
    QCOMPARE(traversedNodes.at(6), "4");
    QCOMPARE(traversedNodes.at(7), "3");
    QCOMPARE(traversedNodes.at(8), "1");
}


void TestElementCursor::testMovementByRegionMethods() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(4, "R"));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement(0, "1"));
    element1->setWeakThis(element1.toWeakRef());
    rootElement->setChild(0, element1, nullptr);
    element1->setNumberTextRegions(3);
    element1->setText("1A", 0);
    element1->setText("1B", 1);
    element1->setText("1C", 2);

    QSharedPointer<PositionTestElement> element2(new PositionTestElement(0, "2"));
    element2->setWeakThis(element2.toWeakRef());
    rootElement->setChild(1, element2, nullptr);
    element2->setNumberTextRegions(3);
    element2->setText("2A", 0);
    element2->setText("2B", 1);
    element2->setText("2C", 2);

    QSharedPointer<PositionTestElement> element3(new PositionTestElement(0, "3"));
    element3->setWeakThis(element3.toWeakRef());
    rootElement->setChild(2, element3, nullptr);
    element3->setNumberTextRegions(3);
    element3->setText("3A", 0);
    element3->setText("3B", 1);
    element3->setText("3C", 2);

    Ld::ElementCursor cursor(1, 0, element1); // Start cursor in first region.

    QList<QString> traversedRegions;
    bool isLast = false;
    bool isFirst = true;

    while (!isLast) {
        Ld::ElementPointer currentNode = cursor.element();
        unsigned           regionIndex = cursor.regionIndex();
        unsigned long      textIndex   = cursor.textIndex();

        if (!currentNode.isNull()) {
            QSharedPointer<PositionTestElement> currentElement = currentNode.dynamicCast<PositionTestElement>();
            QVERIFY(!currentElement.isNull());

            if (isFirst) {
                QCOMPARE(textIndex, 1U);
                isFirst = false;
            } else {
                QCOMPARE(textIndex, 0U);
            }

            QVERIFY(regionIndex < currentElement->numberTextRegions());
            QString regionText = currentElement->text(regionIndex);
            traversedRegions.append(regionText);
        } else {
            traversedRegions.append(QString("(null)"));
        }

        isLast = cursor.moveForwardByRegion();
    }

    QCOMPARE(traversedRegions.size(), 10);
    QCOMPARE(traversedRegions.at(0), "1A");
    QCOMPARE(traversedRegions.at(1), "1B");
    QCOMPARE(traversedRegions.at(2), "1C");
    QCOMPARE(traversedRegions.at(3), "2A");
    QCOMPARE(traversedRegions.at(4), "2B");
    QCOMPARE(traversedRegions.at(5), "2C");
    QCOMPARE(traversedRegions.at(6), "3A");
    QCOMPARE(traversedRegions.at(7), "3B");
    QCOMPARE(traversedRegions.at(8), "3C");
    QCOMPARE(traversedRegions.at(9), "(null)");

    traversedRegions.clear();

    while (!isFirst) {
        Ld::ElementPointer currentNode = cursor.element();
        unsigned           regionIndex = cursor.regionIndex();
        unsigned long      textIndex   = cursor.textIndex();

        if (!currentNode.isNull()) {
            QSharedPointer<PositionTestElement> currentElement = currentNode.dynamicCast<PositionTestElement>();
            QVERIFY(!currentElement.isNull());

            QVERIFY(regionIndex < currentElement->numberTextRegions());

            QString regionText = currentElement->text(regionIndex);
            QCOMPARE(textIndex, static_cast<unsigned long>(regionText.length()));

            traversedRegions.append(regionText);
        } else {
            traversedRegions.append(QString("(null)"));
        }

        isFirst = cursor.moveBackwardByRegion();
    }

    QCOMPARE(traversedRegions.size(), 10);
    QCOMPARE(traversedRegions.at(0), "(null)");
    QCOMPARE(traversedRegions.at(1), "3C");
    QCOMPARE(traversedRegions.at(2), "3B");
    QCOMPARE(traversedRegions.at(3), "3A");
    QCOMPARE(traversedRegions.at(4), "2C");
    QCOMPARE(traversedRegions.at(5), "2B");
    QCOMPARE(traversedRegions.at(6), "2A");
    QCOMPARE(traversedRegions.at(7), "1C");
    QCOMPARE(traversedRegions.at(8), "1B");
    QCOMPARE(traversedRegions.at(9), "1A");
 }



void TestElementCursor::testMovementByWhitespaceMethods() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(4, "R"));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement(0, "1"));
    element1->setWeakThis(element1.toWeakRef());
    rootElement->setChild(0, element1, nullptr);
    element1->setNumberTextRegions(3);
    element1->setText("Now is", 0);
    element1->setText("the", 1);
    element1->setText("time for", 2);

    QSharedPointer<PositionTestElement> element2(new PositionTestElement(0, "2"));
    element2->setWeakThis(element2.toWeakRef());
    rootElement->setChild(1, element2, nullptr);
    element2->setNumberTextRegions(3);
    element2->setText("all good", 0);
    element2->setText("men", 1);
    element2->setText("to come to", 2);

    QSharedPointer<PositionTestElement> element3(new PositionTestElement(0, "3"));
    element3->setWeakThis(element3.toWeakRef());
    rootElement->setChild(2, element3, nullptr);
    element3->setNumberTextRegions(3);
    element3->setText("the aid", 0);
    element3->setText("of their", 1);
    element3->setText("country.", 2);

    Ld::ElementCursor cursor(0, 0, element1); // Start cursor in first region.

    QList<QString> traversedText;
    bool isLast = false;

    while (!isLast) {
        Ld::ElementPointer currentNode = cursor.element();
        unsigned           regionIndex = cursor.regionIndex();
        unsigned long      textIndex   = cursor.textIndex();

        if (!currentNode.isNull()) {
            QSharedPointer<PositionTestElement> currentElement = currentNode.dynamicCast<PositionTestElement>();
            QVERIFY(!currentElement.isNull());

            QVERIFY(regionIndex < currentElement->numberTextRegions());
            QString regionText = currentElement->text(regionIndex);

            QVERIFY(textIndex <= static_cast<unsigned long>(regionText.length()));
            QString textAtCursor = regionText.mid(textIndex).trimmed();

            traversedText.append(textAtCursor);
        } else {
            traversedText.append(QString("(null)"));
        }

        isLast = cursor.moveForwardByWhitespace();
    }

    QCOMPARE(traversedText.size(), 26);
    QCOMPARE(traversedText.at( 0), "Now is");
    QCOMPARE(traversedText.at( 1), "is");
    QCOMPARE(traversedText.at( 2), "");
    QCOMPARE(traversedText.at( 3), "the");
    QCOMPARE(traversedText.at( 4), "");
    QCOMPARE(traversedText.at( 5), "time for");
    QCOMPARE(traversedText.at( 6), "for");
    QCOMPARE(traversedText.at( 7), "");
    QCOMPARE(traversedText.at( 8), "all good");
    QCOMPARE(traversedText.at( 9), "good");
    QCOMPARE(traversedText.at(10), "");
    QCOMPARE(traversedText.at(11), "men");
    QCOMPARE(traversedText.at(12), "");
    QCOMPARE(traversedText.at(13), "to come to");
    QCOMPARE(traversedText.at(14), "come to");
    QCOMPARE(traversedText.at(15), "to");
    QCOMPARE(traversedText.at(16), "");
    QCOMPARE(traversedText.at(17), "the aid");
    QCOMPARE(traversedText.at(18), "aid");
    QCOMPARE(traversedText.at(19), "");
    QCOMPARE(traversedText.at(20), "of their");
    QCOMPARE(traversedText.at(21), "their");
    QCOMPARE(traversedText.at(22), "");
    QCOMPARE(traversedText.at(23), "country.");
    QCOMPARE(traversedText.at(24), "");
    QCOMPARE(traversedText.at(25), "(null)");

    traversedText.clear();

    bool isFirst = false;

    while (!isFirst) {
        Ld::ElementPointer currentNode = cursor.element();
        unsigned           regionIndex = cursor.regionIndex();
        unsigned long      textIndex   = cursor.textIndex();

        if (!currentNode.isNull()) {
            QSharedPointer<PositionTestElement> currentElement = currentNode.dynamicCast<PositionTestElement>();
            QVERIFY(!currentElement.isNull());

            QVERIFY(regionIndex < currentElement->numberTextRegions());
            QString regionText = currentElement->text(regionIndex);

            QVERIFY(textIndex <= static_cast<unsigned long>(regionText.length()));
            QString textAtCursor = regionText.mid(textIndex).trimmed();

            traversedText.append(textAtCursor);
        } else {
            traversedText.append(QString("(null)"));
        }

        isFirst = cursor.moveBackwardByWhitespace();
    }

    QCOMPARE(traversedText.size(), 26);
    QCOMPARE(traversedText.at( 0), "(null)");
    QCOMPARE(traversedText.at( 1), "");
    QCOMPARE(traversedText.at( 2), "country.");
    QCOMPARE(traversedText.at( 3), "");
    QCOMPARE(traversedText.at( 4), "their");
    QCOMPARE(traversedText.at( 5), "of their");
    QCOMPARE(traversedText.at( 6), "");
    QCOMPARE(traversedText.at( 7), "aid");
    QCOMPARE(traversedText.at( 8), "the aid");
    QCOMPARE(traversedText.at( 9), "");
    QCOMPARE(traversedText.at(10), "to");
    QCOMPARE(traversedText.at(11), "come to");
    QCOMPARE(traversedText.at(12), "to come to");
    QCOMPARE(traversedText.at(13), "");
    QCOMPARE(traversedText.at(14), "men");
    QCOMPARE(traversedText.at(15), "");
    QCOMPARE(traversedText.at(16), "good");
    QCOMPARE(traversedText.at(17), "all good");
    QCOMPARE(traversedText.at(18), "");
    QCOMPARE(traversedText.at(19), "for");
    QCOMPARE(traversedText.at(20), "time for");
    QCOMPARE(traversedText.at(21), "");
    QCOMPARE(traversedText.at(22), "the");
    QCOMPARE(traversedText.at(23), "");
    QCOMPARE(traversedText.at(24), "is");
    QCOMPARE(traversedText.at(25), "Now is");
}


void TestElementCursor::testMovementByCharacterMethods() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(3, "R"));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement(0, "1"));
    element1->setWeakThis(element1.toWeakRef());
    rootElement->setChild(0, element1, nullptr);
    element1->setNumberTextRegions(2);
    element1->setText("Work out your", 0);
    element1->setText("own salvation", 1);

    QSharedPointer<PositionTestElement> element2(new PositionTestElement(0, "2"));
    element2->setWeakThis(element2.toWeakRef());
    rootElement->setChild(1, element2, nullptr);
    element2->setNumberTextRegions(2);
    element2->setText("with", 0);
    element2->setText("diligence", 1);

    Ld::ElementCursor cursor(0, 0, element1); // Start cursor in first region.

    QList<QString> traversed;
    bool isLast = false;

    while (!isLast) {
        Ld::ElementPointer currentNode = cursor.element();
        unsigned           regionIndex = cursor.regionIndex();
        unsigned long      textIndex   = cursor.textIndex();

        if (!currentNode.isNull()) {
            QSharedPointer<PositionTestElement> currentElement = currentNode.dynamicCast<PositionTestElement>();
            QVERIFY(!currentElement.isNull());

            QVERIFY(regionIndex < currentElement->numberTextRegions());
            QString regionText = currentElement->text(regionIndex);

            QVERIFY(textIndex <= static_cast<unsigned long>(regionText.length()));
            QString textAtCursor = regionText.mid(textIndex, 1);

            traversed.append(textAtCursor);
        } else {
            traversed.append(QString("(null)"));
        }

        isLast = cursor.moveForwardByCharacter();
    }

    QCOMPARE(traversed.size(), 44);
    QCOMPARE(traversed.at( 0), "W");
    QCOMPARE(traversed.at( 1), "o");
    QCOMPARE(traversed.at( 2), "r");
    QCOMPARE(traversed.at( 3), "k");
    QCOMPARE(traversed.at( 4), " ");
    QCOMPARE(traversed.at( 5), "o");
    QCOMPARE(traversed.at( 6), "u");
    QCOMPARE(traversed.at( 7), "t");
    QCOMPARE(traversed.at( 8), " ");
    QCOMPARE(traversed.at( 9), "y");
    QCOMPARE(traversed.at(10), "o");
    QCOMPARE(traversed.at(11), "u");
    QCOMPARE(traversed.at(12), "r");
    QCOMPARE(traversed.at(13), "");
    QCOMPARE(traversed.at(14), "o");
    QCOMPARE(traversed.at(15), "w");
    QCOMPARE(traversed.at(16), "n");
    QCOMPARE(traversed.at(17), " ");
    QCOMPARE(traversed.at(18), "s");
    QCOMPARE(traversed.at(19), "a");
    QCOMPARE(traversed.at(20), "l");
    QCOMPARE(traversed.at(21), "v");
    QCOMPARE(traversed.at(22), "a");
    QCOMPARE(traversed.at(23), "t");
    QCOMPARE(traversed.at(24), "i");
    QCOMPARE(traversed.at(25), "o");
    QCOMPARE(traversed.at(26), "n");
    QCOMPARE(traversed.at(27), "");
    QCOMPARE(traversed.at(28), "w");
    QCOMPARE(traversed.at(29), "i");
    QCOMPARE(traversed.at(30), "t");
    QCOMPARE(traversed.at(31), "h");
    QCOMPARE(traversed.at(32), "");
    QCOMPARE(traversed.at(33), "d");
    QCOMPARE(traversed.at(34), "i");
    QCOMPARE(traversed.at(35), "l");
    QCOMPARE(traversed.at(36), "i");
    QCOMPARE(traversed.at(37), "g");
    QCOMPARE(traversed.at(38), "e");
    QCOMPARE(traversed.at(39), "n");
    QCOMPARE(traversed.at(40), "c");
    QCOMPARE(traversed.at(41), "e");
    QCOMPARE(traversed.at(42), "");
    QCOMPARE(traversed.at(43), "(null)");

    traversed.clear();

    bool isFirst = false;
    while (!isFirst) {
        Ld::ElementPointer currentNode = cursor.element();
        unsigned           regionIndex = cursor.regionIndex();
        unsigned long      textIndex   = cursor.textIndex();

        if (!currentNode.isNull()) {
            QSharedPointer<PositionTestElement> currentElement = currentNode.dynamicCast<PositionTestElement>();
            QVERIFY(!currentElement.isNull());

            QVERIFY(regionIndex < currentElement->numberTextRegions());
            QString regionText = currentElement->text(regionIndex);

            QVERIFY(textIndex <= static_cast<unsigned long>(regionText.length()));
            QString textAtCursor = regionText.mid(textIndex, 1);

            traversed.append(textAtCursor);
        } else {
            traversed.append(QString("(null)"));
        }

        isFirst = cursor.moveBackwardByCharacter();
    }

    QCOMPARE(traversed.size(), 44);
    QCOMPARE(traversed.at( 0), "(null)");
    QCOMPARE(traversed.at( 1), "");
    QCOMPARE(traversed.at( 2), "e");
    QCOMPARE(traversed.at( 3), "c");
    QCOMPARE(traversed.at( 4), "n");
    QCOMPARE(traversed.at( 5), "e");
    QCOMPARE(traversed.at( 6), "g");
    QCOMPARE(traversed.at( 7), "i");
    QCOMPARE(traversed.at( 8), "l");
    QCOMPARE(traversed.at( 9), "i");
    QCOMPARE(traversed.at(10), "d");
    QCOMPARE(traversed.at(11), "");
    QCOMPARE(traversed.at(12), "h");
    QCOMPARE(traversed.at(13), "t");
    QCOMPARE(traversed.at(14), "i");
    QCOMPARE(traversed.at(15), "w");
    QCOMPARE(traversed.at(16), "");
    QCOMPARE(traversed.at(17), "n");
    QCOMPARE(traversed.at(18), "o");
    QCOMPARE(traversed.at(19), "i");
    QCOMPARE(traversed.at(20), "t");
    QCOMPARE(traversed.at(21), "a");
    QCOMPARE(traversed.at(22), "v");
    QCOMPARE(traversed.at(23), "l");
    QCOMPARE(traversed.at(24), "a");
    QCOMPARE(traversed.at(25), "s");
    QCOMPARE(traversed.at(26), " ");
    QCOMPARE(traversed.at(27), "n");
    QCOMPARE(traversed.at(28), "w");
    QCOMPARE(traversed.at(29), "o");
    QCOMPARE(traversed.at(30), "");
    QCOMPARE(traversed.at(31), "r");
    QCOMPARE(traversed.at(32), "u");
    QCOMPARE(traversed.at(33), "o");
    QCOMPARE(traversed.at(34), "y");
    QCOMPARE(traversed.at(35), " ");
    QCOMPARE(traversed.at(36), "t");
    QCOMPARE(traversed.at(37), "u");
    QCOMPARE(traversed.at(38), "o");
    QCOMPARE(traversed.at(39), " ");
    QCOMPARE(traversed.at(40), "k");
    QCOMPARE(traversed.at(41), "r");
    QCOMPARE(traversed.at(42), "o");
    QCOMPARE(traversed.at(43), "W");
}


void TestElementCursor::testMovementToBeginningEndMethods() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(3, "R"));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement(0, "1"));
    element1->setWeakThis(element1.toWeakRef());
    rootElement->setChild(0, element1, nullptr);
    element1->setNumberTextRegions(3);
    element1->setText("1A", 0);
    element1->setText("1B", 1);
    element1->setText("1C", 2);

    QSharedPointer<PositionTestElement> element2(new PositionTestElement(0, "2"));
    element2->setWeakThis(element2.toWeakRef());
    rootElement->setChild(1, element2, nullptr);
    element2->setNumberTextRegions(3);
    element2->setText("2A", 0);
    element2->setText("2B", 1);
    element2->setText("2C", 2);

    QSharedPointer<PositionTestElement> element3(new PositionTestElement(0, "3"));
    element3->setWeakThis(element3.toWeakRef());
    rootElement->setChild(2, element3, nullptr);
    element3->setNumberTextRegions(3);
    element3->setText("3A", 0);
    element3->setText("3B", 1);
    element3->setText("3C", 2);

    Ld::ElementCursor cursor(1, 0, element2); // Start cursor in first region.

    cursor.moveToFirstElement();
    QCOMPARE(cursor.element().dynamicCast<PositionTestElement>(), element1);
    QCOMPARE(cursor.isWholeElement(), true);

    cursor.moveToLastElement();
    QCOMPARE(cursor.element().dynamicCast<PositionTestElement>(), element3);
    QCOMPARE(cursor.isWholeElement(), true);

    cursor.moveToBeginning();
    QCOMPARE(cursor.element().dynamicCast<PositionTestElement>(), element1);
    QCOMPARE(cursor.isWholeElement(), false);
    QCOMPARE(cursor.regionIndex(), 0U);
    QCOMPARE(cursor.textIndex(), 0U);

    cursor.moveToEnd();
    QCOMPARE(cursor.element().dynamicCast<PositionTestElement>(), element3);
    QCOMPARE(cursor.isWholeElement(), false);
    QCOMPARE(cursor.regionIndex(), 2U);
    QCOMPARE(cursor.textIndex(), 2U);
}



void TestElementCursor::testComparisonOperators() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(4, "R"));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement(0, "1"));
    element1->setWeakThis(element1.toWeakRef());
    rootElement->setChild(0, element1, nullptr);
    element1->setNumberTextRegions(3);
    element1->setText("1A", 0);
    element1->setText("1B", 1);
    element1->setText("1C", 2);

    QSharedPointer<PositionTestElement> element2(new PositionTestElement(0, "2"));
    element2->setWeakThis(element2.toWeakRef());
    rootElement->setChild(1, element2, nullptr);
    element2->setNumberTextRegions(3);
    element2->setText("2A", 0);
    element2->setText("2B", 1);
    element2->setText("2C", 2);

    QSharedPointer<PositionTestElement> element3(new PositionTestElement(0, "3"));
    element3->setWeakThis(element3.toWeakRef());
    rootElement->setChild(2, element3, nullptr);
    element3->setNumberTextRegions(3);
    element3->setText("3A", 0);
    element3->setText("3B", 1);
    element3->setText("3C", 2);

    Ld::ElementCursor cursor100(0, 0, element1);
    Ld::ElementCursor cursor110(1, 0, element1);
    Ld::ElementCursor cursor200(0, 0, element2);
    Ld::ElementCursor cursor201(0, 1, element2);

    QCOMPARE(cursor100 < cursor100, false);
    QCOMPARE(cursor100 < cursor110, true);
    QCOMPARE(cursor110 < cursor100, false);
    QCOMPARE(cursor100 < cursor200, true);
    QCOMPARE(cursor200 < cursor100, false);
    QCOMPARE(cursor200 < cursor201, true);
    QCOMPARE(cursor201 < cursor200, false);

    QCOMPARE(cursor100 == cursor100, true);
    QCOMPARE(cursor100 == cursor110, false);
    QCOMPARE(cursor100 == cursor200, false);
    QCOMPARE(cursor200 == cursor201, false);

    QCOMPARE(cursor100 != cursor100, false);
    QCOMPARE(cursor100 != cursor110, true);
    QCOMPARE(cursor100 != cursor200, true);
    QCOMPARE(cursor200 != cursor201, true);

    QCOMPARE(cursor100 > cursor100, false);
    QCOMPARE(cursor110 > cursor100, true);
    QCOMPARE(cursor100 > cursor110, false);
    QCOMPARE(cursor200 > cursor100, true);
    QCOMPARE(cursor100 > cursor200, false);
    QCOMPARE(cursor201 > cursor200, true);
    QCOMPARE(cursor200 > cursor201, false);

    QCOMPARE(cursor100 <= cursor100, true);
    QCOMPARE(cursor100 <= cursor110, true);
    QCOMPARE(cursor110 <= cursor100, false);
    QCOMPARE(cursor100 <= cursor200, true);
    QCOMPARE(cursor200 <= cursor100, false);
    QCOMPARE(cursor200 <= cursor201, true);
    QCOMPARE(cursor201 <= cursor200, false);

    QCOMPARE(cursor100 >= cursor100, true);
    QCOMPARE(cursor110 >= cursor100, true);
    QCOMPARE(cursor100 >= cursor110, false);
    QCOMPARE(cursor200 >= cursor100, true);
    QCOMPARE(cursor100 >= cursor200, false);
    QCOMPARE(cursor201 >= cursor200, true);
    QCOMPARE(cursor200 >= cursor201, false);
}


void TestElementCursor::testElementsInRangeMethod() {
    /* Tree structure:
     *
     *                   R
     *                  / \
     *                /     \
     *               /       \
     *              1         2
     *             / \       /|\
     *            /   \     / | \
     *           3     4   5  6  (null)
     *                / \
     *               /   \
     *              7     8
     *
     * Element 3 test element's text:
     *     Region 1: foo
     *     Region 2: bar
     *
     * Element 5 test element's text:
     *     Region 1: bang
     *     Region 2: boing
     */

    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(2, "R"));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement(2, "1"));
    element1->setWeakThis(element1.toWeakRef());
    rootElement->setChild(0, element1, nullptr);

    QSharedPointer<PositionTestElement> element2(new PositionTestElement(3, "2"));
    element2->setWeakThis(element2.toWeakRef());
    rootElement->setChild(1, element2, nullptr);

    QSharedPointer<PositionTestElement> element3(new PositionTestElement(0, "3"));
    element3->setWeakThis(element3.toWeakRef());
    element1->setChild(0, element3, nullptr);

    element3->setNumberTextRegions(2);
    element3->setText("foo", 0);
    element3->setText("bar", 1);

    QSharedPointer<PositionTestElement> element4(new PositionTestElement(2, "4"));
    element4->setWeakThis(element4.toWeakRef());
    element1->setChild(1, element4, nullptr);

    QSharedPointer<PositionTestElement> element5(new PositionTestElement(0, "5"));
    element5->setWeakThis(element5.toWeakRef());
    element2->setChild(0, element5, nullptr);

    element5->setNumberTextRegions(2);
    element5->setText("bang", 0);
    element5->setText("boing", 1);

    QSharedPointer<PositionTestElement> element6(new PositionTestElement(0, "6"));
    element6->setWeakThis(element6.toWeakRef());
    element2->setChild(1, element6, nullptr);

    QSharedPointer<PositionTestElement> element7(new PositionTestElement(0, "7"));
    element7->setWeakThis(element7.toWeakRef());
    element4->setChild(0, element7, nullptr);

    QSharedPointer<PositionTestElement> element8(new PositionTestElement(0, "8"));
    element8->setWeakThis(element8.toWeakRef());
    element4->setChild(1, element8, nullptr);

    Ld::ElementCursor cursor1(1, 2, element3);
    Ld::ElementCursor cursor2(element5);

    Ld::ElementPointerSet elementsInRange = Ld::ElementCursor::elementsInRange(cursor1, cursor2);

    QCOMPARE(elementsInRange.size(), 6);
    QVERIFY(elementsInRange.contains(element3));
    QVERIFY(elementsInRange.contains(element4));
    QVERIFY(elementsInRange.contains(element7));
    QVERIFY(elementsInRange.contains(element8));
    QVERIFY(elementsInRange.contains(element2));
    QVERIFY(elementsInRange.contains(element5));

    cursor1 = Ld::ElementCursor(element2, 2);
    elementsInRange = Ld::ElementCursor::elementsInRange(cursor1, cursor2);

    QCOMPARE(elementsInRange.size(), 2);
    QVERIFY(elementsInRange.contains(element5));
    QVERIFY(elementsInRange.contains(element6));
}


void TestElementCursor::testCopyRangeMethod() {
//    /* Tree structure:
//     *
//     *                   R                    *
//     *                  / \                   *
//     *                /     \                 *
//     *               /       \                *
//     *              1         2               *
//     *             / \       /|\              *
//     *            /   \     / | \             *
//     *           3     4   5  6  (null)       *
//     *                / \                     *
//     *               /   \                    *
//     *              7     8                   *
//     *
//     * Element 3 test element's text:
//     *     Region 1: foo
//     *     Region 2: bar
//     *
//     * Element 5 test element's text:
//     *     Region 1: bang
//     *     Region 2: boing
//     */

//    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(2, "R"));
//    rootElement->setWeakThis(rootElement.toWeakRef());

//    QSharedPointer<PositionTestElement> element1(new PositionTestElement(2, "1"));
//    element1->setWeakThis(element1.toWeakRef());
//    rootElement->setChild(0, element1, nullptr);

//    QSharedPointer<PositionTestElement> element2(new PositionTestElement(3, "2"));
//    element2->setWeakThis(element2.toWeakRef());
//    rootElement->setChild(1, element2, nullptr);

//    QSharedPointer<PositionTestElement> element3(new PositionTestElement(0, "3"));
//    element3->setWeakThis(element3.toWeakRef());
//    element1->setChild(0, element3, nullptr);

//    element3->setNumberTextRegions(2);
//    element3->setText("foo", 0);
//    element3->setText("bar", 1);

//    QSharedPointer<PositionTestElement> element4(new PositionTestElement(2, "4"));
//    element4->setWeakThis(element4.toWeakRef());
//    element1->setChild(1, element4, nullptr);

//    QSharedPointer<PositionTestElement> element5(new PositionTestElement(0, "5"));
//    element5->setWeakThis(element5.toWeakRef());
//    element2->setChild(0, element5, nullptr);

//    element5->setNumberTextRegions(2);
//    element5->setText("bang", 0);
//    element5->setText("boing", 1);

//    QSharedPointer<PositionTestElement> element6(new PositionTestElement(0, "6"));
//    element6->setWeakThis(element6.toWeakRef());
//    element2->setChild(1, element6, nullptr);

//    QSharedPointer<PositionTestElement> element7(new PositionTestElement(0, "7"));
//    element7->setWeakThis(element7.toWeakRef());
//    element4->setChild(0, element7, nullptr);

//    QSharedPointer<PositionTestElement> element8(new PositionTestElement(0, "8"));
//    element8->setWeakThis(element8.toWeakRef());
//    element4->setChild(1, element8, nullptr);

//    Ld::ElementCursor cursor1(2, 1, element3);
//    Ld::ElementCursor cursor2(element5);

//    Ld::SelectionDataPointer copied = Ld::ElementCursor::copyRange(cursor1, cursor2);

//    QCOMPARE(copied->size(), 3U);
//    QCOMPARE(copied->at(0).element().dynamicCast<PositionTestElement>()->identifier(), "3");
//    QCOMPARE(copied->at(1).element().dynamicCast<PositionTestElement>()->identifier(), "4");
//    QCOMPARE(copied->at(2).element().dynamicCast<PositionTestElement>()->identifier(), "2");

//    QCOMPARE(copied->at(0).relativePosition(), 0);
//    QCOMPARE(copied->at(1).relativePosition(), 0);
//    QCOMPARE(copied->at(2).relativePosition(), -1);

//    QCOMPARE(copied->at(0)->numberTextRegions(), 2U);
//    QVERIFY(copied->at(0)->text(0).isEmpty());
//    QCOMPARE(copied->at(0)->text(1), QString("r"));
}


void TestElementCursor::testRemoveRangeMethod() {
//    /* Tree structure:
//     *
//     *                   R                      *
//     *                  / \                     *
//     *                /     \                   *
//     *               /       \                  *
//     *              1         2                 *
//     *             / \       /|\                *
//     *            /   \     / | \               *
//     *           3     4   5  6  (null)         *
//     *                / \                       *
//     *               /   \                      *
//     *              7     8                     *
//     *
//     * Element 3 test element's text:
//     *     Region 1: foo
//     *     Region 2: bar
//     *
//     * Element 5 test element's text:
//     *     Region 1: bang
//     *     Region 2: boing
//     */

//    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(2, "R"));
//    rootElement->setWeakThis(rootElement.toWeakRef());

//    QSharedPointer<PositionTestElement> element1(new PositionTestElement(2, "1"));
//    element1->setWeakThis(element1.toWeakRef());
//    rootElement->setChild(0, element1, nullptr);

//    QSharedPointer<PositionTestElement> element2(new PositionTestElement(3, "2"));
//    element2->setWeakThis(element2.toWeakRef());
//    rootElement->setChild(1, element2, nullptr);

//    QSharedPointer<PositionTestElement> element3(new PositionTestElement(0, "3"));
//    element3->setWeakThis(element3.toWeakRef());
//    element1->setChild(0, element3, nullptr);

//    element3->setNumberTextRegions(2);
//    element3->setText("foo", 0);
//    element3->setText("bar", 1);

//    QSharedPointer<PositionTestElement> element4(new PositionTestElement(2, "4"));
//    element4->setWeakThis(element4.toWeakRef());
//    element1->setChild(1, element4, nullptr);

//    QSharedPointer<PositionTestElement> element5(new PositionTestElement(0, "5"));
//    element5->setWeakThis(element5.toWeakRef());
//    element2->setChild(0, element5, nullptr);

//    element5->setNumberTextRegions(2);
//    element5->setText("bang", 0);
//    element5->setText("boing", 1);

//    QSharedPointer<PositionTestElement> element6(new PositionTestElement(0, "6"));
//    element6->setWeakThis(element6.toWeakRef());
//    element2->setChild(1, element6, nullptr);

//    QSharedPointer<PositionTestElement> element7(new PositionTestElement(0, "7"));
//    element7->setWeakThis(element7.toWeakRef());
//    element4->setChild(0, element7, nullptr);

//    QSharedPointer<PositionTestElement> element8(new PositionTestElement(0, "8"));
//    element8->setWeakThis(element8.toWeakRef());
//    element4->setChild(1, element8, nullptr);

//    Ld::ElementCursor cursor1(2, 1, element3);
//    Ld::ElementCursor cursor2(element5);

//    Ld::ElementCursor::removeRange(cursor1, cursor2, nullptr);

//    /*
//     *
//     *                   R                    *
//     *                  / \                   *
//     *                /     \                 *
//     *               /       \                *
//     *              1         2               *
//     *             /          |\              *
//     *            /           | \             *
//     *           3            6  (null)       *
//     *
//     *
//     *
//     *
//     * Element 3 test element's text:
//     *     Region 1: foo
//     *     Region 2: bar
//     *
//     * Element 5 test element's text:
//     *     Region 1: bang
//     *     Region 2: boing
//     */

//    Ld::ElementPointerList children = rootElement->children();
//    QVERIFY( children.contains(element1));
//    QVERIFY( children.contains(element2));

//    children = element1->children();
//    QVERIFY( children.contains(element3));
//    QVERIFY(!children.contains(element4));

//    children = element2->children();
//    QVERIFY(!children.contains(element5));
//    QVERIFY( children.contains(element6));

//    QCOMPARE(element3->numberTextRegions(), 2U);
//    QCOMPARE(element3->text(0), QString("foo"));
//    QCOMPARE(element3->text(1), QString("ba"));
}


void TestElementCursor::testRemoveRangeMethodCursorUpdates() {
    qDebug() << "Test not implemented.";
}
