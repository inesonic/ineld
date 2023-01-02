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
* This file implements tests of the Ld::elementWithPositionalChildren class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QSet>
#include <QList>
#include <QByteArray>
#include <QSharedPointer>
#include <QtTest/QtTest>

#include <ld_element_structures.h>
#include <ld_data_type.h>
#include <ld_capabilities.h>
#include <ld_xml_writer.h>
#include <ld_xml_reader.h>
#include <ld_xml_attributes.h>
#include <ld_element.h>
#include <ld_element_cursor.h>
#include <ld_cursor.h>
#include <ld_cursor_state_collection.h>
#include <ld_visual_with_positional_children.h>
#include <ld_element_with_positional_children.h>

#include "test_element_with_positional_children.h"

/***********************************************************************************************************************
 * PositionalElement:
 */

class PositionalElement:public Ld::ElementWithPositionalChildren {
    public:
        PositionalElement(const QString& identifer = QString());

        ~PositionalElement() override;

        static QSharedPointer<PositionalElement> create(const QString& identifier = QString());

        static QSharedPointer<PositionalElement> create(
            Ld::VisualWithPositionalChildren* visual,
            const QString&                          identifier = QString()
        );

        QString typeName() const final;

        QString plugInName() const final;

        QString description() const final;

        Ld::DataType::ValueType valueType() const final;

        Ld::Capabilities parentRequires(unsigned long index) const final;

        Ld::Capabilities childProvidesCapabilities() const final;

        QString identifier() const;

        unsigned minimumNumberTextRegions() const final;

        unsigned maximumNumberTextRegions() const final;

        bool setNumberTextRegions(unsigned newNumberTextRegions) final;

        unsigned numberTextRegions() const final;

        bool setText(const QString& newText, unsigned regionNumber = 0) final;

        QString text(unsigned regionNumber = 0) const final;

        long numberDescendants() const;

        Ld::ElementPointerSet descendants() const;

        static long numberAllocatedInstances();

        static void clearNumberAllocatedInstances();

        static Ld::Element* creator(const QString& typeName);

    protected:
        void descendantAdded(Ld::ElementPointer descendantElement) override;

        void descendantRemoved(Ld::ElementPointer descendantElement) override;

    private:
        static long                  currentNumberAllocatedInstances;
        long                         currentNumberDescendants;
        QSet<Ld::ElementWeakPointer> currentDescendants;
        QString                      elementIdentifier;
        QList<QString>               currentText;
};


long PositionalElement::currentNumberAllocatedInstances = 0;

PositionalElement::PositionalElement(const QString& identifier) {
    elementIdentifier = identifier; // Assists with debugging.
    currentNumberDescendants = 0;

    ++currentNumberAllocatedInstances;
}


PositionalElement::~PositionalElement() {
    --currentNumberAllocatedInstances;
}


QSharedPointer<PositionalElement> PositionalElement::create(const QString& identifier) {
    QSharedPointer<PositionalElement> element = QSharedPointer<PositionalElement>(new PositionalElement(identifier));
    element->setWeakThis(element.toWeakRef());

    return element;
}


QSharedPointer<PositionalElement> PositionalElement::create(
        Ld::VisualWithPositionalChildren* visual,
        const QString&                          identifier
    ) {
    QSharedPointer<PositionalElement> element = PositionalElement::create(identifier);
    element->setVisual(visual);

    return element;
}


QString PositionalElement::typeName() const {
    return QString("PositionalElement");
}


QString PositionalElement::plugInName() const {
    return QString();
}


QString PositionalElement::description() const {
    return QString();
}


Ld::DataType::ValueType PositionalElement::valueType() const {
    return Ld::DataType::ValueType::NONE;
}


Ld::Capabilities PositionalElement::parentRequires(unsigned long) const {
    return Ld::Capabilities();
}


Ld::Capabilities PositionalElement::childProvidesCapabilities() const {
    return Ld::Capabilities();
}


QString PositionalElement::identifier() const {
    return elementIdentifier;
}


unsigned PositionalElement::minimumNumberTextRegions() const {
    return 0;
}


unsigned PositionalElement::maximumNumberTextRegions() const {
    return static_cast<unsigned>(-1);
}


bool PositionalElement::setNumberTextRegions(unsigned newNumberTextRegions) {
    unsigned currentNumberRegions = static_cast<unsigned>(currentText.size());
    if (newNumberTextRegions > currentNumberRegions) {
        unsigned regionsToAdd = newNumberTextRegions - currentNumberRegions;
        do {
            currentText.append(QString());
            --regionsToAdd;
        } while (regionsToAdd);
    } else if (newNumberTextRegions < currentNumberRegions) {
        currentText.erase(currentText.begin() + newNumberTextRegions, currentText.end());
    }

    return true;
}


unsigned PositionalElement::numberTextRegions() const {
    return static_cast<unsigned>(currentText.size());
}


bool PositionalElement::setText(const QString& newText, unsigned regionNumber) {
    Q_ASSERT(regionNumber < numberTextRegions());
    currentText[regionNumber] = newText;

    return true;
}


QString PositionalElement::text(unsigned regionNumber) const {
    Q_ASSERT(regionNumber < numberTextRegions());
    return currentText.at(regionNumber);
}


long PositionalElement::numberDescendants() const {
    return currentNumberDescendants;
}


Ld::ElementPointerSet PositionalElement::descendants() const {
    Ld::ElementPointerSet result;

    for (  QSet<Ld::ElementWeakPointer>::const_iterator it  = currentDescendants.constBegin(),
                                                        end = currentDescendants.constEnd()
         ; it != end
         ; ++it
        ) {
        Ld::ElementPointer element = it->toStrongRef();
        if (element) {
            result.insert(element);
        }
    }

    return result;
}


long PositionalElement::numberAllocatedInstances() {
    return currentNumberAllocatedInstances;
}


void PositionalElement::clearNumberAllocatedInstances() {
    currentNumberAllocatedInstances = 0;
}


Ld::Element* PositionalElement::creator(const QString& typeName) {
    Element* element = new PositionalElement;
    Q_ASSERT(element->typeName() == typeName);

    return element;
}


void PositionalElement::descendantAdded(Ld::ElementPointer descendantElement) {
    Ld::Element::descendantAdded(descendantElement);
    ++currentNumberDescendants;
    currentDescendants.insert(descendantElement.toWeakRef());
}


void PositionalElement::descendantRemoved(Ld::ElementPointer descendantElement) {
    Ld::Element::descendantRemoved(descendantElement);
    --currentNumberDescendants;
    currentDescendants.remove(descendantElement.toWeakRef());
}

/***********************************************************************************************************************
 * PositionalVisual:
 */

class PositionalVisual:public Ld::VisualWithPositionalChildren {
    public:
        enum class LastCall: unsigned char {
            NONE = 0,
            ABOUT_TO_REMOVE_CHILD = 1,
            CHILD_INSERTED_BEFORE = 2,
            CHILD_INSERTED_AFTER = 3
        };

        PositionalVisual(const QString& identifer = QString());

        ~PositionalVisual() override;

        QString typeName() const final;

        QString plugInName() const final;

        QString identifier() const;

        static long numberAllocatedInstances();

        static void clearNumberAllocatedInstances();

        void clearCallData();

        LastCall lastCall() const;

        unsigned long lastChildIndex() const;

        Ld::ElementPointer lastElement() const;

        static Ld::Visual* creator(const QString& typeName);

    protected:
        void aboutToRemoveChild(unsigned long childIndex, Ld::ElementPointer childElement) final;

        void childInsertedBefore(unsigned long childIndex, Ld::ElementPointer childElement) final;

        void childInsertedAfter(unsigned long childIndex, Ld::ElementPointer childElement) final;

    private:
        static long            currentNumberAllocatedInstances;

        QString                elementIdentifier;

        LastCall               currentLastCall;

        unsigned long          currentLastIndex;

        Ld::ElementWeakPointer currentLastElement;
};

long PositionalVisual::currentNumberAllocatedInstances = 0;

PositionalVisual::PositionalVisual(const QString& identifier) {
    elementIdentifier  = identifier; // Assists with debugging.
    clearCallData();

    ++currentNumberAllocatedInstances;
}


PositionalVisual::~PositionalVisual() {
    --currentNumberAllocatedInstances;
}


QString PositionalVisual::typeName() const {
    return QString("PositionalVisual");
}


QString PositionalVisual::plugInName() const {
    return QString();
}


QString PositionalVisual::identifier() const {
    return elementIdentifier;
}


long PositionalVisual::numberAllocatedInstances() {
    return currentNumberAllocatedInstances;
}


void PositionalVisual::clearNumberAllocatedInstances() {
    currentNumberAllocatedInstances = 0;
}


void PositionalVisual::clearCallData() {
    currentLastCall    = LastCall::NONE;
    currentLastIndex   = static_cast<unsigned long>(-1);
    currentLastElement = nullptr;
}


PositionalVisual::LastCall PositionalVisual::lastCall() const {
    return currentLastCall;
}


unsigned long PositionalVisual::lastChildIndex() const {
    return currentLastIndex;
}


Ld::ElementPointer PositionalVisual::lastElement() const {
    return currentLastElement.toStrongRef();
}


void PositionalVisual::aboutToRemoveChild(unsigned long childIndex, Ld::ElementPointer childElement) {
    currentLastCall    = LastCall::ABOUT_TO_REMOVE_CHILD;
    currentLastIndex   = childIndex;
    currentLastElement = childElement.toWeakRef();
}


void PositionalVisual::childInsertedBefore(unsigned long childIndex, Ld::ElementPointer childElement) {
    currentLastCall    = LastCall::CHILD_INSERTED_BEFORE;
    currentLastIndex   = childIndex;
    currentLastElement = childElement.toWeakRef();
}


void PositionalVisual::childInsertedAfter(unsigned long childIndex, Ld::ElementPointer childElement) {
    currentLastCall    = LastCall::CHILD_INSERTED_AFTER;
    currentLastIndex   = childIndex;
    currentLastElement = childElement.toWeakRef();
}


Ld::Visual* PositionalVisual::creator(const QString& typeName) {
    Visual* visual = new PositionalVisual;
    Q_ASSERT(visual->typeName() == typeName);

    return visual;
}

/***********************************************************************************************************************
 * TestElementWithPositionalChildren:
 */

TestElementWithPositionalChildren::TestElementWithPositionalChildren() {}


TestElementWithPositionalChildren::~TestElementWithPositionalChildren() {}


void TestElementWithPositionalChildren::initTestCase() {
    Ld::Handle::initialize(0x12345678UL);
    Ld::Element::registerCreator("PositionalElement", &PositionalElement::creator);
}


void TestElementWithPositionalChildren::testHandleGeneration() {
    PositionalElement element1;
    PositionalElement element2;

    QVERIFY2(element1.handle() != element2.handle(),"Elements don't have unique handles.");
}


void TestElementWithPositionalChildren::testTypeName() {
    PositionalElement element;
    QCOMPARE(element.typeName(),QString("PositionalElement"));
}


void TestElementWithPositionalChildren::testCloneMethods() {
    QSharedPointer<PositionalElement> parent = PositionalElement::create();
    parent->setNumberTextRegions(1);
    parent->setText("p");

    QSharedPointer<PositionalElement> child1 = PositionalElement::create();
    child1->setNumberTextRegions(1);
    child1->setText("c1");

    QSharedPointer<PositionalElement> child2 = PositionalElement::create();
    child2->setNumberTextRegions(2);
    child2->setText("c2");

    parent->append(child1, nullptr);
    parent->append(child2, nullptr);

    QSharedPointer<PositionalElement> cloneParent = parent->clone(true).dynamicCast<PositionalElement>();
    QVERIFY(cloneParent->numberTextRegions() == 1);
    QVERIFY(cloneParent->text(0) == QString("p"));

    QSharedPointer<PositionalElement> cloneChild1 = parent->child(0).dynamicCast<PositionalElement>();
    QVERIFY(cloneChild1->numberTextRegions() == 1);
    QVERIFY(cloneChild1->text(0) == QString("c1"));

    QSharedPointer<PositionalElement> cloneChild2 = parent->child(1).dynamicCast<PositionalElement>();
    QVERIFY(cloneChild2->numberTextRegions() == 2);
    QVERIFY(cloneChild2->text(0) == QString("c2"));
}


void TestElementWithPositionalChildren::testChildPlacement() {
    PositionalElement element;
    QCOMPARE(element.childPlacement(), Ld::Element::ChildPlacement::POSITIONAL);
}


void TestElementWithPositionalChildren::testParentChildTreeFunctions() {
    QSharedPointer<PositionalElement> parent1 = PositionalElement::create();
    QSharedPointer<PositionalElement> parent2 = PositionalElement::create();

    QSharedPointer<PositionalElement> child1 = PositionalElement::create();
    QSharedPointer<PositionalElement> child2 = PositionalElement::create();
    QSharedPointer<PositionalElement> child3 = PositionalElement::create();

    // Verify default parent() return values.

    QVERIFY2(!parent1->parent(), "parent() not null (parent1).");
    QVERIFY2(!parent2->parent(), "parent() not null (parent2).");
    QVERIFY2(!child1->parent(), "parent() not null (child1).");
    QVERIFY2(!child2->parent(), "parent() not null (child2).");
    QVERIFY2(!child3->parent(), "parent() not null (child3).");

    // Verify default value returned by numberChildren method

    QVERIFY2(parent1->numberChildren() == 0, "Unexpected number of children (parent1).");
    QVERIFY2(parent2->numberChildren() == 0, "Unexpected number of children (parent2).");

    // Append and prepend children and verify number of children as well as order..

    parent1->append(child1, nullptr);
    parent1->prepend(child2, nullptr);
    parent1->append(child3, nullptr);

    QVERIFY2(parent1->numberChildren() == 3, "Unexpected number of children after append/prepend.");

    QVERIFY2(parent1->child(0) == child2, "child2 not first after append/prepend.");
    QVERIFY2(parent1->child(1) == child1, "child1 not first after append/prepend.");
    QVERIFY2(parent1->child(2) == child3, "child3 not first after append/prepend.");

    // Confirm children point to their parent.

    QVERIFY2(child1->parent() == parent1, "child1 does not point to parent.");
    QVERIFY2(child2->parent() == parent1, "child2 does not point to parent.");
    QVERIFY2(child3->parent() == parent1, "child3 does not point to parent.");

    // Verify children list is generated correctly.

    Ld::ElementPointerList parent1Children = parent1->children();
    QVERIFY2(parent1Children.size() == 3, "Unexpected children list length.");

    QVERIFY2(parent1Children.at(0) == child2, "child2 not first in list after append/prepend.");
    QVERIFY2(parent1Children.at(1) == child1, "child1 not first in list after append/prepend.");
    QVERIFY2(parent1Children.at(2) == child3, "child3 not first in list after append/prepend.");

    // Remove child from the parent and confirm that they are, in fact, decoupled.

    bool success = parent1->removeChild(child1, nullptr);
    QVERIFY2(success, "removeChild(Element*) failed during valid first removal.");

    QVERIFY2(parent1->numberChildren() == 2, "Incorrect number of children after first removal.");
    QVERIFY2(parent1->child(0) == child2, "Children of parent invalid after first removal (1).");
    QVERIFY2(parent1->child(1) == child3, "Children of parent invalid after first removal (2).");
    QVERIFY2(!child1->parent(), "Removed child still points to parent after first removal.");

    // Attempt to remove missing child and confirm failure.

    success = parent1->removeChild(child1, nullptr);
    QVERIFY2(!success, "removeChild(Element*) reported success for invalid child.");

    // Insert child into list using insertBefore and confirm ordering.

    parent1->insertBefore(1, child1, nullptr);

    QVERIFY2(parent1->numberChildren() == 3, "Incorrect number of children after inserBefore.");
    QVERIFY2(parent1->child(0) == child2, "Children of parent invalid after insertBefore (1).");
    QVERIFY2(parent1->child(1) == child1, "Children of parent invalid after insertBefore (2).");
    QVERIFY2(parent1->child(2) == child3, "Children of parent invalid after insertBefore (3).");

    // Remove child by index and confirm removal.

    success = parent1->removeChild(1, nullptr);
    QVERIFY2(success, "removeChild(unsigned long) failed during valid second removal.");

    QVERIFY2(parent1->numberChildren() == 2, "Incorrect number of children after second removal.");
    QVERIFY2(parent1->child(0) == child2, "Children of parent invalid after second removal (1).");
    QVERIFY2(parent1->child(1) == child3, "Children of parent invalid after second removal (2).");
    QVERIFY2(!child1->parent(), "Removed child still points to parent after second removal.");

    // Insert child into list using insertAfter and confirm ordering.

    parent1->insertAfter(0, child1, nullptr);

    QVERIFY2(parent1->numberChildren() == 3, "Incorrect number of children after inserBefore.");
    QVERIFY2(parent1->child(0) == child2, "Children of parent invalid after insertBefore (1).");
    QVERIFY2(parent1->child(1) == child1, "Children of parent invalid after insertBefore (2).");
    QVERIFY2(parent1->child(2) == child3, "Children of parent invalid after insertBefore (3).");

    // Move the child from one parent to another using insertBefore and confirm both parents are all children are
    // correct.

    parent2->insertBefore(0, child1, nullptr);

    QVERIFY2(parent1->numberChildren() == 2, "Incorrect number of children in parent1 after move via inserBefore.");
    QVERIFY2(parent1->child(0) == child2, "Children of parent1 invalid after move via insertBefore (2).");
    QVERIFY2(parent1->child(1) == child3, "Children of parent1 invalid after move via insertBefore (3).");

    QVERIFY2(parent2->numberChildren() == 1, "Incorrect number of children in parent2 after move via insertBefore.");
    QVERIFY2(parent2->child(0) == child1, "Children of parent2 invalid after move via insertBefore (1).");

    QVERIFY2(child1->parent() == parent2, "child1's parent invalid after move via insertBefore.");
    QVERIFY2(child2->parent() == parent1, "child2's parent invalid after move via insertBefore.");
    QVERIFY2(child3->parent() == parent1, "child3's parent invalid after move via insertBefore.");

    // Move the child from one parent to another using insertAfter and confirm both parents are all children are
    // correct.

    parent2->insertAfter(0, child2, nullptr);

    QVERIFY2(parent1->numberChildren() == 1, "Incorrect number of children in parent1 after move via inserBefore.");
    QVERIFY2(parent1->child(0) == child3, "Children of parent1 invalid after move via insertAfter (3).");

    QVERIFY2(parent2->numberChildren() == 2, "Incorrect number of children in parent2 after move via insertAfter.");
    QVERIFY2(parent2->child(0) == child1, "Children of parent2 invalid after move via insertAfter (1).");
    QVERIFY2(parent2->child(1) == child2, "Children of parent2 invalid after move via insertAfter (2).");

    QVERIFY2(child1->parent() == parent2, "child1's parent invalid after move via insertAfter.");
    QVERIFY2(child2->parent() == parent2, "child2's parent invalid after move via insertAfter.");
    QVERIFY2(child3->parent() == parent1, "child3's parent invalid after move via insertAfter.");
}


void TestElementWithPositionalChildren::testConstructorsDestructors() {
    {
        PositionalElement::clearNumberAllocatedInstances();

        QSharedPointer<PositionalElement> parent = PositionalElement::create(QString("p"));

        QSharedPointer<PositionalElement> child0 = PositionalElement::create(QString("c0"));
        parent->append(child0, nullptr);

        QSharedPointer<PositionalElement> child1 = PositionalElement::create(QString("c1"));
        parent->append(child1, nullptr);

        QSharedPointer<PositionalElement> child2 = PositionalElement::create(QString("c2"));
        parent->append(child2, nullptr);

        child0->append(PositionalElement::create(QString("c00")), nullptr);
        child0->append(PositionalElement::create(QString("c02")), nullptr);

        child1->append(PositionalElement::create(QString("c11")), nullptr);
        child1->append(PositionalElement::create(QString("c12")), nullptr);

        child2->append(PositionalElement::create(QString("c20")), nullptr);
        child2->append(PositionalElement::create(QString("c21")), nullptr);

        QCOMPARE(PositionalElement::numberAllocatedInstances(), static_cast<long>(10));

        // Verify that deleting a child removes it from it's parent. Verify that children are also removed.
        parent->removeChild(child0, nullptr);
        QVERIFY2(parent->numberChildren() == 2, "Parent has incorrect number of children after one child deleted.");
        QVERIFY2(parent->child(0) == child1, "Parent contains incorrect child1 (or ordered incorrectly).");
        QVERIFY2(parent->child(1) == child2, "Parent contains incorrect child2 (or ordered incorrectly).");
    }

    QCOMPARE(PositionalElement::numberAllocatedInstances(), static_cast<long>(0));
}


void TestElementWithPositionalChildren::testRootSearch() {
    QSharedPointer<PositionalElement> parent = PositionalElement::create(QString("p"));

    QSharedPointer<PositionalElement> child0 = PositionalElement::create(QString("c0"));
    parent->append(child0, nullptr);

    QSharedPointer<PositionalElement> child1 = PositionalElement::create(QString("c1"));
    parent->append(child1, nullptr);

    QSharedPointer<PositionalElement> child2 = PositionalElement::create(QString("c2"));
    parent->append(child2, nullptr);

    QSharedPointer<PositionalElement> child00 = PositionalElement::create(QString("c00"));
    QSharedPointer<PositionalElement> child01 = PositionalElement::create(QString("c01"));
    child0->append(child00, nullptr);
    child0->append(child01, nullptr);

    QSharedPointer<PositionalElement> child10 = PositionalElement::create(QString("c10"));
    QSharedPointer<PositionalElement> child11 = PositionalElement::create(QString("c11"));
    child1->append(child10, nullptr);
    child1->append(child11, nullptr);

    QSharedPointer<PositionalElement> child20 = PositionalElement::create(QString("c20"));
    QSharedPointer<PositionalElement> child21 = PositionalElement::create(QString("c21"));
    child2->append(child20, nullptr);
    child2->append(child21, nullptr);

    QList<QSharedPointer<PositionalElement>> elements;
    elements.append(parent);
    elements.append(child0);
    elements.append(child1);
    elements.append(child2);
    elements.append(child00);
    elements.append(child01);
    elements.append(child10);
    elements.append(child11);
    elements.append(child20);
    elements.append(child21);

    for (auto it=elements.begin(),end=elements.end() ; it!=end ; ++it) {
        QVERIFY2(
            (*it)->root() == parent,
            tr("Element %1 could not locate root element").arg((*it)->identifier()).toLatin1().constData()
        );
    }
}


void TestElementWithPositionalChildren::testSiblingMethods() {
    QSharedPointer<PositionalElement> parent = PositionalElement::create(QString("p"));

    QSharedPointer<PositionalElement> child0 = PositionalElement::create(QString("c0"));
    parent->append(child0, nullptr);

    QCOMPARE(child0->isOnlyChild(), true);

    QSharedPointer<PositionalElement> child1 = PositionalElement::create(QString("c1"));
    parent->append(child1, nullptr);

    QCOMPARE(child1->isOnlyChild(), false);

    QSharedPointer<PositionalElement> child2 = PositionalElement::create(QString("c2"));
    parent->append(child2, nullptr);

    QCOMPARE(child0->isOnlyChild(), false);
    QCOMPARE(child0->isFirstChild(), true);
    QCOMPARE(child0->isLastChild(), false);

    QCOMPARE(child1->isOnlyChild(), false);
    QCOMPARE(child1->isFirstChild(), false);
    QCOMPARE(child1->isLastChild(), false);

    QCOMPARE(child2->isOnlyChild(), false);
    QCOMPARE(child2->isFirstChild(), false);
    QCOMPARE(child2->isLastChild(), true);

    bool isFirst;
    bool isLast;

    Ld::ElementPointer element = child0->previousSibling(&isFirst);
    QVERIFY(element.isNull());
    QCOMPARE(isFirst, true);

    element = child2->nextSibling(&isLast);
    QVERIFY(element.isNull());
    QCOMPARE(isLast, true);

    element = child2->previousSibling(&isFirst);
    QCOMPARE(element, child1);
    QCOMPARE(isFirst, false);

    element = child0->nextSibling(&isLast);
    QCOMPARE(element, child1);
    QCOMPARE(isLast, false);
}


void TestElementWithPositionalChildren::testDescendantTracking() {
    QSharedPointer<PositionalElement> parent = PositionalElement::create(QString("p"));

    QSharedPointer<PositionalElement> child0 = PositionalElement::create(QString("c0"));
    parent->append(child0, nullptr);

    QSharedPointer<PositionalElement> child1 = PositionalElement::create(QString("c1"));
    parent->append(child1, nullptr);

    QSharedPointer<PositionalElement> child2 = PositionalElement::create(QString("c2"));
    parent->append(child2, nullptr);

    QSharedPointer<PositionalElement> child00 = PositionalElement::create(QString("c00"));
    QSharedPointer<PositionalElement> child01 = PositionalElement::create(QString("c01"));
    child0->append(child00, nullptr);
    child0->append(child01, nullptr);

    QSharedPointer<PositionalElement> child10 = PositionalElement::create(QString("c10"));
    QSharedPointer<PositionalElement> child11 = PositionalElement::create(QString("c11"));
    child1->append(child10, nullptr);
    child1->append(child11, nullptr);

    QSharedPointer<PositionalElement> child20 = PositionalElement::create(QString("c20"));
    QSharedPointer<PositionalElement> child21 = PositionalElement::create(QString("c21"));
    child2->append(child20, nullptr);
    child2->append(child21, nullptr);

    // Check if we received the correct number of notifications when children are added.

    QVERIFY(parent->numberDescendants() == 9);

    // Check that notification occurs when children are more gently removed.

    bool success = parent->removeChild(child0, nullptr);
    QVERIFY2(success, "removeChild failed unexpectedly.");

    QVERIFY(parent->numberDescendants() == 6);
}


void TestElementWithPositionalChildren::testVisualTracking() {
    PositionalVisual::clearNumberAllocatedInstances();

    // Verify assignment of visual in constructor.

    PositionalVisual* visual = new PositionalVisual;
    QSharedPointer<PositionalElement> element = PositionalElement::create(visual);

    QVERIFY2(element->visual() == visual, "Element did not return visual after assigning in the constructor.");

    // Confirm deleting the visual automatically removes it from the element.
    delete visual;
    QVERIFY2(element->visual() == nullptr, "Element holds pointer to visual after delete.");

    visual = new PositionalVisual();
    {
        QSharedPointer<PositionalElement> element2 = PositionalElement::create();
        element2->setVisual(visual);
        QVERIFY2(element2->visual() == visual, "Element did not return visual after assigning using setVisual.");
        QVERIFY2(PositionalVisual::numberAllocatedInstances() == 1, "Incorrect number of allocated Visual instances.");
    }

    // Confirm deleting the element also deletes the visual.
    QVERIFY2(PositionalVisual::numberAllocatedInstances() == 0, "Deleting element did not delete visual.");

    visual = new PositionalVisual;
    QSharedPointer<PositionalElement> parent = PositionalElement::create(visual);

    QVERIFY2(visual->lastCall() == PositionalVisual::LastCall::NONE, "Unexpected call state.");

    QSharedPointer<PositionalElement> child = PositionalElement::create();
    parent->insertAfter(0, child, nullptr);

    QVERIFY2(
        visual->lastCall() == PositionalVisual::LastCall::CHILD_INSERTED_AFTER,
        "Visual did not receive notification of child set."
    );
    QVERIFY2(visual->lastChildIndex() == 0, "Visual received the wrong child index.");
    QVERIFY2(visual->lastElement() == child, "Visual received pointer to wrong child element.");

    visual->clearCallData();

    parent->removeChild(child, nullptr);

    QVERIFY2(
        visual->lastCall() == PositionalVisual::LastCall::ABOUT_TO_REMOVE_CHILD,
        "Visual did not receive notification of child removal."
    );
    QVERIFY2(visual->lastChildIndex() == 0, "Visual received the wrong child index during removal.");
    QVERIFY2(visual->lastElement() == child, "Visual received pointer to wrong child element during removal.");

    visual->clearCallData();

    parent->insertBefore(0, child, nullptr);

    QVERIFY2(
        visual->lastCall() == PositionalVisual::LastCall::CHILD_INSERTED_BEFORE,
        "Visual did not receive notification of child set."
    );
    QVERIFY2(visual->lastChildIndex() == 0, "Visual received the wrong child index.");
    QVERIFY2(visual->lastElement() == child, "Visual received pointer to wrong child element.");
}


void TestElementWithPositionalChildren::testCreatorFunctions() {
    Ld::Element::registerCreator("PositionalElement", PositionalElement::creator);

    bool success = Ld::Element::registerCreator("PositionalElement", PositionalElement::creator);
    QVERIFY2(!success, "Element::registerCreator allowed duplicate registration.");

    Ld::ElementPointer element = Ld::Element::create("PositionalElement");
    QVERIFY2(element, "Creator method returned a null pointer.");
    QVERIFY2(element->typeName() == "PositionalElement", "Creator method returned the wrong element.");

    success = Ld::Visual::registerCreator("PositionalVisual", PositionalVisual::creator);
    QVERIFY2(success, "Visual::registerCreator failed.");

    success = Ld::Visual::registerCreator("PositionalVisual", PositionalVisual::creator);
    QVERIFY2(!success, "Visual::registerCreator allowed duplicate registration.");

    Ld::Visual* visual = Ld::Visual::create("PositionalVisual");
    QVERIFY2(visual != nullptr, "Creator method returned a null pointer.");
    QVERIFY2(visual->typeName() == "PositionalVisual", "Creator method returned the wrong visual.");

    delete visual;
}


void TestElementWithPositionalChildren::testRemoveChildrenMethod() {
    PositionalElement::clearNumberAllocatedInstances();
    QSharedPointer<PositionalElement> parent = PositionalElement::create(QString("p"));

    {
        QSharedPointer<PositionalElement> child0 = PositionalElement::create(QString("c0"));
        parent->append(child0, nullptr);

        QSharedPointer<PositionalElement> child1 = PositionalElement::create(QString("c1"));
        parent->append(child1, nullptr);

        QSharedPointer<PositionalElement> child2 = PositionalElement::create(QString("c2"));
        parent->append(child2, nullptr);

        QVERIFY(PositionalElement::numberAllocatedInstances() == 4);

        parent->removeChildren(nullptr);

        QVERIFY(parent->numberChildren() == 0);
    }

    QVERIFY(PositionalElement::numberAllocatedInstances() == 1);
}


void TestElementWithPositionalChildren::testIndexOfChildMethod() {
    QSharedPointer<PositionalElement> parentElement = PositionalElement::create();
    QSharedPointer<PositionalElement> childElement1 = PositionalElement::create();
    QSharedPointer<PositionalElement> childElement2 = PositionalElement::create();
    QSharedPointer<PositionalElement> childElement3 = PositionalElement::create();
    QSharedPointer<PositionalElement> childElement4 = PositionalElement::create();

    parentElement->append(childElement1, nullptr);
    parentElement->append(childElement2, nullptr);
    parentElement->append(childElement3, nullptr);

    unsigned long childIndex = parentElement->indexOfChild(childElement1);
    QCOMPARE(childIndex, 0U);

    childIndex = parentElement->indexOfChild(childElement2);
    QCOMPARE(childIndex, 1U);

    childIndex = parentElement->indexOfChild(childElement3);
    QCOMPARE(childIndex, 2U);

    childIndex = parentElement->indexOfChild(childElement4);
    QCOMPARE(childIndex, Ld::Element::invalidChildIndex);
}


void TestElementWithPositionalChildren::testRemoveChildCursorUpdates() {
    QSharedPointer<PositionalElement> rootElement   = PositionalElement::create();
    QSharedPointer<PositionalElement> parentElement = PositionalElement::create();
    QSharedPointer<PositionalElement> childElement1 = PositionalElement::create();
    QSharedPointer<PositionalElement> childElement2 = PositionalElement::create();
    QSharedPointer<PositionalElement> childElement3 = PositionalElement::create();

    rootElement->append(parentElement, nullptr);

    parentElement->append(childElement1, nullptr);
    parentElement->append(childElement2, nullptr);
    parentElement->append(childElement3, nullptr);

    Ld::CursorPointer cursor1(new Ld::Cursor);
    Ld::CursorPointer cursor2(new Ld::Cursor);
    Ld::CursorPointer cursor3(new Ld::Cursor);

    cursor1->updateFromElementCursor(Ld::ElementCursor(childElement1));
    cursor2->updateFromElementCursor(Ld::ElementCursor(childElement2));
    cursor3->updateFromElementCursor(Ld::ElementCursor(childElement3));

    Ld::CursorStateCollection collection;
    collection.insert(cursor1);
    collection.insert(cursor2);
    collection.insert(cursor3);

    parentElement->removeChild(childElement2, &collection);
    collection.updateCursorState(false);

    QCOMPARE(cursor1->elementCursor(), Ld::ElementCursor(childElement1));
    QCOMPARE(cursor2->elementCursor(), Ld::ElementCursor(childElement3));
    QCOMPARE(cursor3->elementCursor(), Ld::ElementCursor(childElement3));

    parentElement->removeChild(childElement3, &collection);
    collection.updateCursorState(false);

    QCOMPARE(cursor1->elementCursor(), Ld::ElementCursor(childElement1));
    QCOMPARE(cursor2->elementCursor(), Ld::ElementCursor(childElement1));
    QCOMPARE(cursor3->elementCursor(), Ld::ElementCursor(childElement1));

    parentElement->removeChild(childElement1, &collection);
    collection.updateCursorState(false);

    QCOMPARE(cursor1->elementCursor(), Ld::ElementCursor(parentElement));
    QCOMPARE(cursor2->elementCursor(), Ld::ElementCursor(parentElement));
    QCOMPARE(cursor3->elementCursor(), Ld::ElementCursor(parentElement));
}


void TestElementWithPositionalChildren::testRemoveChildrenCursorUpdates() {
    QSharedPointer<PositionalElement> rootElement   = PositionalElement::create();
    QSharedPointer<PositionalElement> parentElement = PositionalElement::create();
    QSharedPointer<PositionalElement> childElement = PositionalElement::create();

    rootElement->append(parentElement, nullptr);
    parentElement->append(childElement, nullptr);

    Ld::CursorPointer cursor(new Ld::Cursor);

    cursor->updateFromElementCursor(Ld::ElementCursor(childElement));

    Ld::CursorStateCollection collection;
    collection.insert(cursor);

    parentElement->removeChildren(&collection);
    collection.updateCursorState(false);

    QCOMPARE(cursor->elementCursor(), Ld::ElementCursor(parentElement));

    rootElement->removeChildren(&collection);
    collection.updateCursorState(false);

    QCOMPARE(cursor->elementCursor(), Ld::ElementCursor());
}


void TestElementWithPositionalChildren::testInsertBeforeCursorUpdates() {
    QSharedPointer<PositionalElement> rootElement   = PositionalElement::create();
    QSharedPointer<PositionalElement> parentElement = PositionalElement::create();
    QSharedPointer<PositionalElement> childElement1 = PositionalElement::create();
    QSharedPointer<PositionalElement> childElement2 = PositionalElement::create();

    rootElement->append(parentElement, nullptr);
    parentElement->append(childElement1, nullptr);

    Ld::CursorPointer cursor(new Ld::Cursor);
    cursor->updateFromElementCursor(Ld::ElementCursor(parentElement, 0));

    Ld::CursorStateCollection collection;
    collection.insert(cursor);

    parentElement->insertBefore(0, childElement2, &collection);
    collection.updateCursorState(false);

    QCOMPARE(cursor->elementCursor(), Ld::ElementCursor(parentElement, 1));
}
