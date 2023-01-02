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
* This file implements tests of the Ld::ElementPosition class.  Also tests core functions of the
* Ld::Element class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QList>
#include <QtTest/QtTest>

#include <ld_handle.h>
#include <ld_element_structures.h>
#include <ld_capabilities.h>
#include <ld_element.h>
#include <ld_element_with_fixed_children.h>
#include <ld_element_position.h>

#include "test_element_position.h"

/***********************************************************************************************************************
 * PositionTestElement:
 */

PositionTestElement::PositionTestElement(unsigned numberChildren, const QString& identifier) {
    setNumberChildren(numberChildren, nullptr);
    elementIdentifier = identifier;
}


PositionTestElement::~PositionTestElement() {}


Ld::Element* PositionTestElement::creator(const QString&) {
    return new PositionTestElement;
}


QString PositionTestElement::typeName() const {
    return QString("PositionTest");
}


QString PositionTestElement::plugInName() const {
    return QString();
}


QString PositionTestElement::description() const {
    return QString();
}


Ld::DataType::ValueType PositionTestElement::valueType() const {
    return Ld::DataType::ValueType::NONE;
}


Ld::ElementPointer PositionTestElement::clone(bool includeChildren) const {
    Ld::ElementPointer element = Ld::ElementWithFixedChildren::clone(includeChildren);
    element.dynamicCast<PositionTestElement>()->elementIdentifier = elementIdentifier;

    return element;
}


Ld::ElementPointer PositionTestElement::clone(
        unsigned long            startingTextIndex,
        unsigned                 startingRegionIndex,
        unsigned long            endingTextIndex,
        unsigned                 endingRegionIndex,
        bool                     includeChildren
    ) const {
    Ld::ElementPointer element = Ld::ElementWithFixedChildren::clone(
        startingTextIndex,
        startingRegionIndex,
        endingTextIndex,
        endingRegionIndex,
        includeChildren
    );

    element.dynamicCast<PositionTestElement>()->elementIdentifier = elementIdentifier;

    return element;
}


Ld::Capabilities PositionTestElement::parentRequires(unsigned long) const {
    return Ld::Capabilities();
}


Ld::Capabilities PositionTestElement::childProvidesCapabilities() const {
    return Ld::Capabilities();
}


QString PositionTestElement::identifier() const {
    return elementIdentifier;
}


unsigned PositionTestElement::minimumNumberTextRegions() const {
    return 0;
}


unsigned PositionTestElement::maximumNumberTextRegions() const {
    return static_cast<signed>((static_cast<unsigned>(-1) >> 1)); // Maximum signed integer value.
}


bool PositionTestElement::setNumberTextRegions(unsigned newNumberTextRegions) {
    if (newNumberTextRegions < static_cast<unsigned>(textRegions.size())) {
        textRegions.erase(textRegions.begin() + newNumberTextRegions, textRegions.end());
    } else {
        while (newNumberTextRegions > static_cast<unsigned>(textRegions.size())) {
            textRegions.append(QString());
        }
    }

    return true;
}


unsigned PositionTestElement::numberTextRegions() const {
    return static_cast<unsigned>(textRegions.size());
}


bool PositionTestElement::setText(const QString& newText, unsigned regionNumber) {
    bool result;

    if (regionNumber < static_cast<unsigned>(textRegions.size())) {
        textRegions[regionNumber] = newText;
        result = true;
    } else {
        result = false;
    }

    return result;
}


QString PositionTestElement::text(unsigned regionNumber) const {
    QString result;

    if (regionNumber < static_cast<unsigned>(textRegions.size())) {
        result = textRegions[regionNumber];
    }

    return result;
}

/***********************************************************************************************************************
 * TestElementPosition:
 */

TestElementPosition::TestElementPosition() {}


TestElementPosition::~TestElementPosition() {}


void TestElementPosition::initTestCase() {
    Ld::Handle::initialize(0x12345678ABCDEF01);
}


void TestElementPosition::testConstructorsDestructors() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(1));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());

    rootElement->setChild(0, element1, nullptr);

    Ld::ElementPosition position1;
    QCOMPARE(position1.isValid(), false);

    Ld::ElementPosition position2(element1);
    QCOMPARE(position2.isValid(), true);

    QCOMPARE(position2.element(), element1);
    QCOMPARE(position2.parent(), rootElement);
    QCOMPARE(position2.childIndex(), 0U);

    Ld::ElementPosition position3(rootElement, 0);

    QCOMPARE(position3.element(), element1);
    QCOMPARE(position3.parent(), rootElement);
    QCOMPARE(position3.childIndex(), 0U);

    Ld::ElementPosition position4(position2);

    QCOMPARE(position4.element(), element1);
    QCOMPARE(position4.parent(), rootElement);
    QCOMPARE(position4.childIndex(), 0U);
}


void TestElementPosition::testValidInvalidMethods() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(1));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());

    rootElement->setChild(0, element1, nullptr);

    Ld::ElementPosition position1;
    QCOMPARE(position1.isValid(), false);
    QCOMPARE(position1.isInvalid(), true);

    Ld::ElementPosition position2(element1);
    QCOMPARE(position2.isValid(), true);
    QCOMPARE(position2.isInvalid(), false);
}


void TestElementPosition::testAccessors() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(2));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());

    rootElement->setChild(0, element1, nullptr);

    QSharedPointer<PositionTestElement> element2(new PositionTestElement);
    element2->setWeakThis(element2.toWeakRef());

    rootElement->setChild(1, element2, nullptr);

    Ld::ElementPosition position(element1);
    QCOMPARE(position.isValid(), true);
    QCOMPARE(position.element(), element1);
    QCOMPARE(position.parent(), rootElement);
    QCOMPARE(position.childIndex(), 0U);

    position.setElement(element2);
    QCOMPARE(position.isValid(), true);
    QCOMPARE(position.element(), element2);
    QCOMPARE(position.parent(), rootElement);
    QCOMPARE(position.childIndex(), 1U);

    position.setElementUnderParent(rootElement, 0);
    QCOMPARE(position.isValid(), true);
    QCOMPARE(position.element(), element1);
    QCOMPARE(position.parent(), rootElement);
    QCOMPARE(position.childIndex(), 0U);
}


void TestElementPosition::testAssignmentOperator() {
    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(2));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement);
    element1->setWeakThis(element1.toWeakRef());

    rootElement->setChild(0, element1, nullptr);

    QSharedPointer<PositionTestElement> element2(new PositionTestElement);
    element2->setWeakThis(element2.toWeakRef());

    rootElement->setChild(1, element2, nullptr);

    Ld::ElementPosition position1;
    QCOMPARE(position1.isValid(), false);

    Ld::ElementPosition position2(element1);
    QCOMPARE(position2.isValid(), true);

    QCOMPARE(position2.element(), element1);
    QCOMPARE(position2.parent(), rootElement);
    QCOMPARE(position2.childIndex(), 0U);

    position1 = position2;

    QCOMPARE(position1.element(), element1);
    QCOMPARE(position1.parent(), rootElement);
    QCOMPARE(position1.childIndex(), 0U);

    position2.setElement(element2);
    QCOMPARE(position2.element(), element2);
    QCOMPARE(position2.parent(), rootElement);
    QCOMPARE(position2.childIndex(), 1U);

    QCOMPARE(position1.element(), element1);
    QCOMPARE(position1.parent(), rootElement);
    QCOMPARE(position1.childIndex(), 0U);
}


void TestElementPosition::testMovementMethods() {
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

    Ld::ElementPosition position(element1);

    QList<QString> traversedNodes;
    bool isLast = false;
    while (!isLast) {
        Ld::ElementPointer currentNode = position.element();

        QString identifier;
        if (currentNode.isNull()) {
            identifier = QString("(null)");
        } else {
            identifier = currentNode.dynamicCast<PositionTestElement>()->identifier();
        }

        traversedNodes.append(identifier);
        isLast = position.moveForwardByElement();
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
        Ld::ElementPointer currentNode = position.element();

        QString identifier;
        if (currentNode.isNull()) {
            identifier = QString("(null)");
        } else {
            identifier = currentNode.dynamicCast<PositionTestElement>()->identifier();
        }

        traversedNodes.append(identifier);
        isFirst = position.moveBackwardByElement();
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

    position.moveToFirstElement();
    QCOMPARE(position.isValid(), true);
    QCOMPARE(position.element().dynamicCast<PositionTestElement>(), element1);

    position.moveToLastElement();
    QCOMPARE(position.isValid(), true);
    QVERIFY(position.element().dynamicCast<PositionTestElement>().isNull());
    QCOMPARE(position.parent().dynamicCast<PositionTestElement>(), element2);
    QCOMPARE(position.childIndex(), 2U);
}


void TestElementPosition::testComparisonOperators() {
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

    Ld::ElementPosition invalidPosition;

    Ld::ElementPosition position1(element1);
    Ld::ElementPosition position2(element2);
    Ld::ElementPosition position3(element3);
    Ld::ElementPosition position5(element5);
    Ld::ElementPosition position7(element7);

    QCOMPARE(position1 < invalidPosition, false);
    QCOMPARE(invalidPosition < position1, false);

    QCOMPARE(position1 < position2, true);
    QCOMPARE(position1 < position1, false);
    QCOMPARE(position2 < position1, false);
    QCOMPARE(position1 < position3, true);
    QCOMPARE(position3 < position1, false);
    QCOMPARE(position7 < position5, true);
    QCOMPARE(position5 < position7, false);

    QCOMPARE(position1 == invalidPosition, false);
    QCOMPARE(position1 == position2, false);
    QCOMPARE(position1 == position1, true);

    QCOMPARE(position1 != invalidPosition, true);
    QCOMPARE(position1 != position2, true);
    QCOMPARE(position1 != position1, false);

    QCOMPARE(position1 > invalidPosition, false);
    QCOMPARE(invalidPosition > position1, false);

    QCOMPARE(position1 > position2, false);
    QCOMPARE(position1 > position1, false);
    QCOMPARE(position2 > position1, true);
    QCOMPARE(position1 > position3, false);
    QCOMPARE(position3 > position1, true);
    QCOMPARE(position7 > position5, false);
    QCOMPARE(position5 > position7, true);
}


void TestElementPosition::testElementStack() {
    /* Tree structure:
     *
     *                   R
     *                  / \
     *                /     \
     *               /       \
     *              1         2
     *             / \
     *            /   \
     *           3     4
     *                / \
     *               /   \
     *              5     6
     */

    QSharedPointer<PositionTestElement> rootElement(new PositionTestElement(2, "R"));
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<PositionTestElement> element1(new PositionTestElement(2, "1"));
    element1->setWeakThis(element1.toWeakRef());
    rootElement->setChild(0, element1, nullptr);

    QSharedPointer<PositionTestElement> element2(new PositionTestElement(0, "2"));
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
    element4->setChild(0, element5, nullptr);

    QSharedPointer<PositionTestElement> element6(new PositionTestElement(0, "6"));
    element6->setWeakThis(element6.toWeakRef());
    element4->setChild(1, element6, nullptr);

    Ld::ElementPosition    invalidPosition;
    Ld::ElementPointerList elementStack = invalidPosition.elementStack();
    QVERIFY(elementStack.isEmpty());

    Ld::ElementPosition position1(element1);
    elementStack = position1.elementStack();
    QCOMPARE(elementStack.size(), 2);
    QCOMPARE(elementStack.at(0), rootElement);
    QCOMPARE(elementStack.at(1), element1);

    Ld::ElementPosition position5(element5);
    elementStack = position5.elementStack();
    QCOMPARE(elementStack.size(), 4);
    QCOMPARE(elementStack.at(0), rootElement);
    QCOMPARE(elementStack.at(1), element1);
    QCOMPARE(elementStack.at(2), element4);
    QCOMPARE(elementStack.at(3), element5);
}
