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
* This file implements tests of the Ld::elementWithFixedChildren class.  Also tests core functions of the
* Ld::Element class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QSet>
#include <QList>
#include <QByteArray>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QtTest/QtTest>

#include <ld_element_structures.h>
#include <ld_data_type.h>
#include <ld_capabilities.h>
#include <ld_element.h>
#include <ld_element_cursor.h>
#include <ld_cursor.h>
#include <ld_cursor_state_collection.h>
#include <ld_program_file.h>
#include <ld_xml_writer.h>
#include <ld_xml_reader.h>
#include <ld_xml_attributes.h>
#include <ld_program_file.h>
#include <ld_format_organizer.h>
#include <ld_visual_with_fixed_children.h>
#include <ld_element_with_fixed_children.h>

#include "test_element_with_fixed_children.h"

/***********************************************************************************************************************
 * FixedElement:
 */

class FixedElement:public Ld::ElementWithFixedChildren {
    public:
        static constexpr unsigned forcedNumberChildren = 3;

        FixedElement(const QString& identifier = QString());

        ~FixedElement() override;

        static Ld::Element* creatorFunction(const QString& typeName);

        static QSharedPointer<FixedElement> create(const QString& identifier = QString());

        static QSharedPointer<FixedElement> create(
            Ld::VisualWithFixedChildren* visual,
            const QString&               identifier = QString()
        );

        QString typeName() const final;

        QString plugInName() const final;

        QString description() const final;

        Ld::DataType::ValueType valueType() const override;

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


long FixedElement::currentNumberAllocatedInstances = 0;

FixedElement::FixedElement(const QString& identifier) {
    elementIdentifier = identifier; // Assists with debugging.
    currentNumberDescendants = 0;

    ++currentNumberAllocatedInstances;
    setNumberChildren(forcedNumberChildren, nullptr);
}


FixedElement::~FixedElement() {
    --currentNumberAllocatedInstances;
}


Ld::Element* FixedElement::creatorFunction(const QString&) {
    return new FixedElement;
}


QSharedPointer<FixedElement> FixedElement::create(const QString& identifier) {
    QSharedPointer<FixedElement> element = QSharedPointer<FixedElement>(new FixedElement(identifier));
    element->setWeakThis(element.toWeakRef());

    return element;
}


QSharedPointer<FixedElement> FixedElement::create(
        Ld::VisualWithFixedChildren* visual,
        const QString&                     identifier
    ) {
    QSharedPointer<FixedElement> element = FixedElement::create(identifier);
    element->setVisual(visual);

    return element;
}


QString FixedElement::typeName() const {
    return QString("FixedElement");
}


QString FixedElement::plugInName() const {
    return QString();
}


QString FixedElement::description() const {
    return QString();
}


Ld::DataType::ValueType FixedElement::valueType() const {
    return Ld::DataType::ValueType::NONE;
}


Ld::Capabilities FixedElement::parentRequires(unsigned long) const {
    return Ld::Capabilities();
}


Ld::Capabilities FixedElement::childProvidesCapabilities() const {
    return Ld::Capabilities();
}


QString FixedElement::identifier() const {
    return elementIdentifier;
}


unsigned FixedElement::minimumNumberTextRegions() const {
    return 0;
}


unsigned FixedElement::maximumNumberTextRegions() const {
    return static_cast<unsigned>(-1);
}


bool FixedElement::setNumberTextRegions(unsigned newNumberTextRegions) {
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


unsigned FixedElement::numberTextRegions() const {
    return static_cast<unsigned>(currentText.size());
}


bool FixedElement::setText(const QString& newText, unsigned regionNumber) {
    Q_ASSERT(regionNumber < numberTextRegions());
    currentText[regionNumber] = newText;

    return true;
}


QString FixedElement::text(unsigned regionNumber) const {
    Q_ASSERT(regionNumber < numberTextRegions());
    return currentText.at(regionNumber);
}


long FixedElement::numberDescendants() const {
    return currentNumberDescendants;
}


Ld::ElementPointerSet FixedElement::descendants() const {
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


long FixedElement::numberAllocatedInstances() {
    return currentNumberAllocatedInstances;
}


void FixedElement::clearNumberAllocatedInstances() {
    currentNumberAllocatedInstances = 0;
}


Ld::Element* FixedElement::creator(const QString& typeName) {
    FixedElement* element = new FixedElement;
    Q_ASSERT(element->typeName() == typeName);

    return element;
}


void FixedElement::descendantAdded(Ld::ElementPointer descendantElement) {
    Ld::Element::descendantAdded(descendantElement);
    ++currentNumberDescendants;
    currentDescendants.insert(descendantElement.toWeakRef());
}


void FixedElement::descendantRemoved(Ld::ElementPointer descendantElement) {
    Ld::Element::descendantRemoved(descendantElement);
    --currentNumberDescendants;
    currentDescendants.remove(descendantElement.toWeakRef());
}

/***********************************************************************************************************************
 * FixedVisual:
 */

class FixedVisual:public Ld::VisualWithFixedChildren {
    public:
        enum class LastCall: unsigned char {
            NONE = 0,
            ABOUT_TO_REMOVE_CHILD = 1,
            CHILD_SET = 2
        };

        FixedVisual(const QString& identifer = QString());

        ~FixedVisual() override;

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

        void childSet(unsigned long childIndex, Ld::ElementPointer childElement) final;

    private:
        static long            currentNumberAllocatedInstances;

        QString                elementIdentifier;

        LastCall               currentLastCall;

        unsigned long          currentLastIndex;

        Ld::ElementWeakPointer currentLastElement;
};

long FixedVisual::currentNumberAllocatedInstances = 0;

FixedVisual::FixedVisual(const QString& identifier) {
    elementIdentifier  = identifier; // Assists with debugging.
    clearCallData();

    ++currentNumberAllocatedInstances;
}


FixedVisual::~FixedVisual() {
    --currentNumberAllocatedInstances;
}


QString FixedVisual::typeName() const {
    return QString("FixedVisual");
}


QString FixedVisual::plugInName() const{
    return QString();
}


QString FixedVisual::identifier() const {
    return elementIdentifier;
}


long FixedVisual::numberAllocatedInstances() {
    return currentNumberAllocatedInstances;
}


void FixedVisual::clearNumberAllocatedInstances() {
    currentNumberAllocatedInstances = 0;
}


void FixedVisual::clearCallData() {
    currentLastCall    = LastCall::NONE;
    currentLastIndex   = static_cast<unsigned long>(-1);
    currentLastElement = nullptr;
}


FixedVisual::LastCall FixedVisual::lastCall() const {
    return currentLastCall;
}


unsigned long FixedVisual::lastChildIndex() const {
    return currentLastIndex;
}


Ld::ElementPointer FixedVisual::lastElement() const {
    return currentLastElement;
}


void FixedVisual::aboutToRemoveChild(unsigned long childIndex, Ld::ElementPointer childElement) {
    currentLastCall    = LastCall::ABOUT_TO_REMOVE_CHILD;
    currentLastIndex   = childIndex;
    currentLastElement = childElement;
}


void FixedVisual::childSet(unsigned long childIndex, Ld::ElementPointer childElement) {
    currentLastCall    = LastCall::CHILD_SET;
    currentLastIndex   = childIndex;
    currentLastElement = childElement;
}


Ld::Visual* FixedVisual::creator(const QString& typeName) {
    Visual* visual = new FixedVisual;
    Q_ASSERT(visual->typeName() == typeName);

    return visual;
}

/***********************************************************************************************************************
 * TestElementWithFixedChildren:
 */

TestElementWithFixedChildren::TestElementWithFixedChildren() {}


TestElementWithFixedChildren::~TestElementWithFixedChildren() {}


void TestElementWithFixedChildren::initTestCase() {
    Ld::Handle::initialize(0x12345678UL);
    Ld::Element::registerCreator("FixedElement", &FixedElement::creatorFunction);
}


void TestElementWithFixedChildren::testHandleGeneration() {
    FixedElement element1;
    FixedElement element2;

    QVERIFY2(element1.handle() != element2.handle(),"Elements don't have unique handles.");
}


void TestElementWithFixedChildren::testTypeName() {
    FixedElement element;
    QCOMPARE(element.typeName(),QString("FixedElement"));
}


void TestElementWithFixedChildren::testCloneMethods() {
    QSharedPointer<FixedElement> parent = FixedElement::create();
    parent->setNumberTextRegions(1);
    parent->setText("p");

    QSharedPointer<FixedElement> child1 = FixedElement::create();
    child1->setNumberTextRegions(1);
    child1->setText("c1");

    QSharedPointer<FixedElement> child2 = FixedElement::create();
    child2->setNumberTextRegions(2);
    child2->setText("c2");

    parent->setChild(0, child1, nullptr);
    parent->setChild(1, child2, nullptr);

    QSharedPointer<FixedElement> cloneParent = parent->clone(true).dynamicCast<FixedElement>();
    QVERIFY(cloneParent->numberTextRegions() == 1);
    QVERIFY(cloneParent->text(0) == QString("p"));

    QSharedPointer<FixedElement> cloneChild1 = parent->child(0).dynamicCast<FixedElement>();
    QVERIFY(cloneChild1->numberTextRegions() == 1);
    QVERIFY(cloneChild1->text(0) == QString("c1"));

    QSharedPointer<FixedElement> cloneChild2 = parent->child(1).dynamicCast<FixedElement>();
    QVERIFY(cloneChild2->numberTextRegions() == 2);
    QVERIFY(cloneChild2->text(0) == QString("c2"));
}


void TestElementWithFixedChildren::testChildPlacement() {
    FixedElement element;
    QCOMPARE(element.childPlacement(), Ld::Element::ChildPlacement::FIXED);
}


void TestElementWithFixedChildren::testParentChildTreeFunctions() {
    QSharedPointer<FixedElement> parent1 = FixedElement::create();
    QSharedPointer<FixedElement> parent2 = FixedElement::create();

    QSharedPointer<FixedElement> child1 = FixedElement::create();
    QSharedPointer<FixedElement> child2 = FixedElement::create();
    QSharedPointer<FixedElement> child3 = FixedElement::create();

    // Verify default parent() return values.

    QVERIFY2(!parent1->parent(), "parent() not null (parent1).");
    QVERIFY2(!parent2->parent(), "parent() not null (parent2).");
    QVERIFY2(!child1->parent(), "parent() not null (child1).");
    QVERIFY2(!child2->parent(), "parent() not null (child2).");
    QVERIFY2(!child3->parent(), "parent() not null (child3).");

    // Verify value returned by numberChildren method

    QVERIFY2(
        parent1->numberChildren() == FixedElement::forcedNumberChildren,
        "Unexpected number of child slots (parent1)."
    );
    QVERIFY2(
        parent2->numberChildren() == FixedElement::forcedNumberChildren,
        "Unexpected number of child slots (parent2)."
    );

    // Verify children method returns correctly sized list and that all elements are null prior to a child being
    // inserted.

    Ld::ElementPointerList parent1Children = parent1->children();
    QVERIFY2(parent1Children.size() == FixedElement::forcedNumberChildren, "children() method returned wroung size.");

    int i;
    for (i=0 ; i<parent1Children.size() ; ++i) {
        QVERIFY2(!parent1Children[i], "Unexpected child list value.");
    }

    // Insert two children and verify that their parent is correct.

    bool success = parent1->setChild(0, child1, nullptr);
    QVERIFY2(child1->parent() == parent1, "child1 does not report the correct parent after being placed under parent1.");
    QVERIFY2(success, "setChild reported failure (1).");

    success = parent1->setChild(1, child2, nullptr);
    QVERIFY2(child2->parent() == parent1, "child2 does not report the correct parent after being placed under parent1.");
    QVERIFY2(success, "setChild reported failure (1).");

    // Verify children method returns a correctly formed list.

    parent1Children = parent1->children();
    QVERIFY2(parent1Children[0] == child1, "child1 not in position 0 (list).");
    QVERIFY2(parent1Children[1] == child2, "child2 not in position 1 (list).");
    QVERIFY2(!parent1Children[2], "position 2 not null (list).");

    // Verify the child method returns correct values.

    QVERIFY2(parent1->child(0) == child1, "child1 not in position 0 (child method).");
    QVERIFY2(parent1->child(1) == child2, "child2 not in position 1 (child method).");
    QVERIFY2(!parent1->child(2), "position 2 not null (child method).");

    // Remove a child from the parent by index and confirm that they in fact are decoupled.

    unsigned long index = 0;
    success = parent1->removeChild(index, nullptr);
    QVERIFY2(success, "removeChild(unsigned long) reported failure (1).");

    QVERIFY2(!child1->parent(), "removed child1 still points to parent.");
    QVERIFY2(!parent1->children().contains(child1), "child1 still tied to parent.");

    // Remove a child from the parent by pointer and confirm that they in fact are decoupled.

    success = parent1->setChild(0, child1, nullptr);
    QVERIFY2(success, "setChild reported failure (2).");

    success = parent1->removeChild(child2, nullptr);
    QVERIFY2(success, "removeChild(Element*) reported failure (1).");

    QVERIFY2(!child2->parent(), "removed child2 still points to parent.");
    QVERIFY2(!parent1->children().contains(child2), "child2 still tied to parent.");

    // Set child under a new parent and confirm child is moved correctly.

    success = parent1->setChild(1, child2, nullptr);
    QVERIFY2(success, "setChild reported failure (3).");

    success = parent1->setChild(2, child3, nullptr);
    QVERIFY2(success, "setChild reported failure (4).");

    success = parent2->setChild(1, child2, nullptr);
    QVERIFY2(success, "setChild reported failure (5).");

    QVERIFY2(child1->parent() == parent1, "child1 parent incorrect after child reparented.");
    QVERIFY2(child2->parent() == parent2, "child2 parent incorrect after child reparented.");
    QVERIFY2(child3->parent() == parent1, "child3 parent incorrect after child reparented.");

    QVERIFY2(parent1->child(0) == child1, "parent1 does not hold child1 in the correct position.");
    QVERIFY2(!parent1->child(1), "parent1 position 1 value is not null.");
    QVERIFY2(parent1->child(2) == child3, "parent1 does not hold child3 in the correct position.");

    QVERIFY2(!parent2->child(0), "parent2 position 0 value is not null.");
    QVERIFY2(parent2->child(1) == child2, "parent2 does not hold child2 in the correct position.");
    QVERIFY2(!parent2->child(2), "parent2 position 2 value is not null.");

    // Verify methods gracefully fail.

    success = parent1->setChild(FixedElement::forcedNumberChildren, child2, nullptr);
    QVERIFY2(!success, "setChild accepted out-of-bounds index.");

    success = parent1->removeChild(FixedElement::forcedNumberChildren, nullptr);
    QVERIFY2(!success, "removeChild(unsigned long) accepted out-of-bounds index.");

    success = parent1->removeChild(1, nullptr);
    QVERIFY2(!success, "removeChild(unsigned long) accepted index to empty slot.");

    success = parent1->removeChild(child2, nullptr);
    QVERIFY2(!success, "removeChild(Element*) accepted pointer to child not under parent.");
}


void TestElementWithFixedChildren::testConstructorsDestructors() {
    FixedElement::clearNumberAllocatedInstances();

    {
        QSharedPointer<FixedElement> parent = FixedElement::create(QString("p"));

        QSharedPointer<FixedElement> child0 = FixedElement::create(QString("c0"));
        parent->setChild(0, child0, nullptr);

        QSharedPointer<FixedElement> child1 = FixedElement::create(QString("c1"));
        parent->setChild(1, child1, nullptr);

        QSharedPointer<FixedElement> child2 = FixedElement::create(QString("c2"));
        parent->setChild(2, child2, nullptr);

        child0->setChild(0, FixedElement::create(QString("c00")), nullptr);
        child0->setChild(2, FixedElement::create(QString("c01")), nullptr);

        child1->setChild(1, FixedElement::create(QString("c10")), nullptr);
        child1->setChild(2, FixedElement::create(QString("c11")), nullptr);

        child2->setChild(0, FixedElement::create(QString("c20")), nullptr);
        child2->setChild(1, FixedElement::create(QString("c21")), nullptr);

        QCOMPARE(FixedElement::numberAllocatedInstances(), static_cast<long>(10));
    } // Forces all instances to be deleted.

    QCOMPARE(FixedElement::numberAllocatedInstances(), static_cast<long>(0));
}


void TestElementWithFixedChildren::testRootSearch() {
    QSharedPointer<FixedElement> parent = FixedElement::create(QString("p"));

    QSharedPointer<FixedElement> child0 = FixedElement::create(QString("c0"));
    parent->setChild(0, child0, nullptr);

    QSharedPointer<FixedElement> child1 = FixedElement::create(QString("c1"));
    parent->setChild(1, child1, nullptr);

    QSharedPointer<FixedElement> child2 = FixedElement::create(QString("c2"));
    parent->setChild(2, child2, nullptr);

    QSharedPointer<FixedElement> child00 = FixedElement::create(QString("c00"));
    QSharedPointer<FixedElement> child01 = FixedElement::create(QString("c01"));
    child0->setChild(0, child00, nullptr);
    child0->setChild(2, child01, nullptr);

    QSharedPointer<FixedElement> child10 = FixedElement::create(QString("c10"));
    QSharedPointer<FixedElement> child11 = FixedElement::create(QString("c11"));
    child1->setChild(1, child10, nullptr);
    child1->setChild(2, child11, nullptr);

    QSharedPointer<FixedElement> child20 = FixedElement::create(QString("c20"));
    QSharedPointer<FixedElement> child21 = FixedElement::create(QString("c21"));
    child2->setChild(0, child20, nullptr);
    child2->setChild(1, child21, nullptr);

    QList<QSharedPointer<FixedElement>> elements;
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


void TestElementWithFixedChildren::testDescendantTracking() {
    QSharedPointer<FixedElement> parent = FixedElement::create(QString("p"));

    QSharedPointer<FixedElement> child0 = FixedElement::create(QString("c0"));
    parent->setChild(0, child0, nullptr);

    QSharedPointer<FixedElement> child1 = FixedElement::create(QString("c1"));
    parent->setChild(1, child1, nullptr);

    QSharedPointer<FixedElement> child2 = FixedElement::create(QString("c2"));
    parent->setChild(2, child2, nullptr);

    QSharedPointer<FixedElement> child00 = FixedElement::create(QString("c00"));
    QSharedPointer<FixedElement> child01 = FixedElement::create(QString("c01"));
    child0->setChild(0, child00, nullptr);
    child0->setChild(2, child01, nullptr);

    QSharedPointer<FixedElement> child10 = FixedElement::create(QString("c10"));
    QSharedPointer<FixedElement> child11 = FixedElement::create(QString("c11"));
    child1->setChild(1, child10, nullptr);
    child1->setChild(2, child11, nullptr);

    QSharedPointer<FixedElement> child20 = FixedElement::create(QString("c20"));
    QSharedPointer<FixedElement> child21 = FixedElement::create(QString("c21"));
    child2->setChild(0, child20, nullptr);
    child2->setChild(1, child21, nullptr);

    // Check if we received the correct number of notifications when children are added.

    QVERIFY(parent->numberDescendants() == 9);

    bool success = child0->removeChild(child00, nullptr);
    QVERIFY2(success, "removeChild failed unexpectedly.");

    QVERIFY(parent->numberDescendants() == 8);

    success = parent->removeChild(child1, nullptr);
    QVERIFY2(success, "removeChild failed unexpectedly.");

    QVERIFY(parent->numberDescendants() == 5);
}


void TestElementWithFixedChildren::testNumberChildren() {
    FixedElement element;
    unsigned long expectedNumberChildren = FixedElement::forcedNumberChildren;
    QCOMPARE(element.numberChildren(), expectedNumberChildren);
}


void TestElementWithFixedChildren::testVisualTracking() {
    FixedVisual::clearNumberAllocatedInstances();

    FixedVisual* visual = new FixedVisual;

    {
        // Verify assignment of visual in constructor.

        QSharedPointer<FixedElement> element = FixedElement::create(visual);

        QVERIFY2(element->visual() == visual, "Element did not return visual after assigning in the constructor.");

        // Confirm deleting the visual automatically removes it from the element.
        delete visual;
        QVERIFY2(element->visual() == nullptr, "Element holds pointer to visual after delete.");

        visual = new FixedVisual();
        element->setVisual(visual);

        QVERIFY2(element->visual() == visual, "Element did not return visual after assigning using setVisual.");
        QVERIFY2(FixedVisual::numberAllocatedInstances() == 1, "Incorrect number of allocated Visual instances.");
    }

    // Confirm deleting the element also deletes the visual.
    QVERIFY2(FixedVisual::numberAllocatedInstances() == 0, "Deleting element did not delete visual.");

    visual = new FixedVisual;
    QSharedPointer<FixedElement> parent = FixedElement::create(visual);

    QVERIFY2(visual->lastCall() == FixedVisual::LastCall::NONE, "Unexpected call state.");

    QSharedPointer<FixedElement> child = FixedElement::create();
    parent->setChild(1, child, nullptr);

    QVERIFY2(visual->lastCall() == FixedVisual::LastCall::CHILD_SET, "Visual did not receive notification of child set.");
    QVERIFY2(visual->lastChildIndex() == 1, "Visual received the wrong child index.");
    QVERIFY2(visual->lastElement() == child, "Visual received pointer to wrong child element.");

    visual->clearCallData();

    parent->removeChild(1, nullptr);

    QVERIFY2(
        visual->lastCall() == FixedVisual::LastCall::ABOUT_TO_REMOVE_CHILD,
        "Visual did not receive notification of child removal."
    );
    QVERIFY2(visual->lastChildIndex() == 1, "Visual received the wrong child index during removal.");
    QVERIFY2(visual->lastElement() == child, "Visual received pointer to wrong child element during removal.");
}


void TestElementWithFixedChildren::testCreatorFunctions() {
    Ld::Element::registerCreator("FixedElement", FixedElement::creator);

    bool success = Ld::Element::registerCreator("FixedElement", FixedElement::creator);
    QVERIFY2(!success, "Element::registerCreator allowed duplicate registration.");

    Ld::ElementPointer element = Ld::Element::create("FixedElement");
    QVERIFY2(element, "Creator method returned a null pointer.");
    QVERIFY2(element->typeName() == "FixedElement", "Creator method returned the wrong element.");

    success = Ld::Visual::registerCreator("FixedVisual", FixedVisual::creator);
    QVERIFY2(success, "Visual::registerCreator failed.");

    success = Ld::Visual::registerCreator("FixedVisual", FixedVisual::creator);
    QVERIFY2(!success, "Visual::registerCreator allowed duplicate registration.");

    Ld::Visual* visual = Ld::Visual::create("FixedVisual");
    QVERIFY2(visual != nullptr, "Creator method returned a null pointer.");
    QVERIFY2(visual->typeName() == "FixedVisual", "Creator method returned the wrong visual.");

    delete visual;
}


void TestElementWithFixedChildren::testProgramWriteReadFunctions() {
    QSharedPointer<FixedElement> parent = FixedElement::create(QString("p"));

    QSharedPointer<FixedElement> child0 = FixedElement::create(QString("c0"));
    parent->setChild(0, child0, nullptr);

    QSharedPointer<FixedElement> child1 = FixedElement::create(QString("c1"));
    parent->setChild(1, child1, nullptr);

    QSharedPointer<FixedElement> child2 = FixedElement::create(QString("c2"));
    parent->setChild(2, child2, nullptr);

    Ld::ProgramFile programFile;
    bool success = programFile.openNew();
    QVERIFY(success);

    success = programFile.initializeXmlWriter();
    QVERIFY(success);

    programFile.writer()->setAutoFormatting(true); // Makes the XML easier to read.
    programFile.writer()->writeStartDocument();

    QSharedPointer<Ld::FormatOrganizer> formats(new Ld::FormatOrganizer());
    Ld::XmlAttributes                   inheritedAttributes;

    parent->writeXml(programFile.writer(), formats, programFile, inheritedAttributes);

    programFile.writer()->writeEndDocument();
    programFile.releaseXmlWriter();

    success = programFile.saveAs("test_container.dat");
    QVERIFY(success);

    success = programFile.close();
    QVERIFY(success);
}


void TestElementWithFixedChildren::testRemoveChildrenMethod() {
    FixedElement::clearNumberAllocatedInstances();
    QSharedPointer<FixedElement> parent = FixedElement::create(QString("p"));

    {
        QSharedPointer<FixedElement> child0 = FixedElement::create(QString("c0"));
        parent->setChild(0, child0, nullptr);

        QSharedPointer<FixedElement> child1 = FixedElement::create(QString("c1"));
        parent->setChild(1, child1, nullptr);

        QSharedPointer<FixedElement> child2 = FixedElement::create(QString("c2"));
        parent->setChild(2, child2, nullptr);

        parent->removeChildren(nullptr);

        for (unsigned i=0 ; i<parent->numberChildren() ; ++i) {
            QVERIFY(!parent->child(i));
        }
    }

    QVERIFY(FixedElement::numberAllocatedInstances() == 1);
}


void TestElementWithFixedChildren::testIndexOfChildMethod() {
    QSharedPointer<FixedElement> parentElement = FixedElement::create();
    QSharedPointer<FixedElement> childElement1 = FixedElement::create();
    QSharedPointer<FixedElement> childElement2 = FixedElement::create();
    QSharedPointer<FixedElement> childElement3 = FixedElement::create();
    QSharedPointer<FixedElement> childElement4 = FixedElement::create();

    parentElement->setChild(0, childElement1, nullptr);
    parentElement->setChild(1, childElement2, nullptr);
    parentElement->setChild(2, childElement3, nullptr);

    unsigned long childIndex = parentElement->indexOfChild(childElement1);
    QCOMPARE(childIndex, 0U);

    childIndex = parentElement->indexOfChild(childElement2);
    QCOMPARE(childIndex, 1U);

    childIndex = parentElement->indexOfChild(childElement3);
    QCOMPARE(childIndex, 2U);

    childIndex = parentElement->indexOfChild(childElement4);
    QCOMPARE(childIndex, Ld::Element::invalidChildIndex);
}


void TestElementWithFixedChildren::testRemoveChildCursorUpdates() {
    QSharedPointer<FixedElement> rootElement   = FixedElement::create();
    QSharedPointer<FixedElement> parentElement = FixedElement::create();
    QSharedPointer<FixedElement> childElement1 = FixedElement::create();
    QSharedPointer<FixedElement> childElement2 = FixedElement::create();
    QSharedPointer<FixedElement> childElement3 = FixedElement::create();

    rootElement->setChild(0, parentElement, nullptr);

    parentElement->setChild(0, childElement1, nullptr);
    parentElement->setChild(1, childElement2, nullptr);
    parentElement->setChild(2, childElement3, nullptr);

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

    parentElement->removeChild(1, &collection);
    collection.updateCursorState(false);

    QCOMPARE(cursor1->elementCursor(), Ld::ElementCursor(childElement1));
    QCOMPARE(cursor2->elementCursor(), Ld::ElementCursor(parentElement, 1));
    QCOMPARE(cursor3->elementCursor(), Ld::ElementCursor(childElement3));

    parentElement->removeChild(2, &collection);
    collection.updateCursorState(false);

    QCOMPARE(cursor1->elementCursor(), Ld::ElementCursor(childElement1));
    QCOMPARE(cursor2->elementCursor(), Ld::ElementCursor(parentElement, 1));
    QCOMPARE(cursor3->elementCursor(), Ld::ElementCursor(parentElement, 2));

    rootElement->removeChild(0, &collection);
    collection.updateCursorState(false);

    QCOMPARE(cursor1->elementCursor(), Ld::ElementCursor(rootElement, 0));
    QCOMPARE(cursor2->elementCursor(), Ld::ElementCursor(rootElement, 0));
    QCOMPARE(cursor3->elementCursor(), Ld::ElementCursor(rootElement, 0));
}


void TestElementWithFixedChildren::testRemoveChildrenCursorUpdates() {
    QSharedPointer<FixedElement> rootElement   = FixedElement::create();
    QSharedPointer<FixedElement> parentElement = FixedElement::create();
    QSharedPointer<FixedElement> childElement  = FixedElement::create();

    rootElement->setChild(0, parentElement, nullptr);
    parentElement->setChild(0, childElement, nullptr);

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


void TestElementWithFixedChildren::testSetChildCursorUpdates() {
    QSharedPointer<FixedElement> rootElement   = FixedElement::create();
    QSharedPointer<FixedElement> parentElement = FixedElement::create();
    QSharedPointer<FixedElement> childElement1 = FixedElement::create();
    QSharedPointer<FixedElement> childElement2 = FixedElement::create();

    rootElement->setChild(0, parentElement, nullptr);
    parentElement->setChild(0, childElement1, nullptr);

    Ld::CursorPointer cursor(new Ld::Cursor);
    cursor->updateFromElementCursor(Ld::ElementCursor(childElement1));

    Ld::CursorStateCollection collection;
    collection.insert(cursor);

    parentElement->setChild(0, childElement2, &collection);
    collection.updateCursorState(false);

    QCOMPARE(cursor->elementCursor(), Ld::ElementCursor(childElement2));

    parentElement->setChild(0, Ld::ElementPointer(), &collection);
    collection.updateCursorState(false);

    QCOMPARE(cursor->elementCursor(), Ld::ElementCursor(parentElement, 0));
}
