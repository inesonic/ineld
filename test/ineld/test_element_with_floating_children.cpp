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
* This file implements tests of the Ld::elementWithFloatingChildren class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QSet>
#include <QList>
#include <QMap>
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
#include <ld_location.h>
#include <ld_xml_writer.h>
#include <ld_xml_reader.h>
#include <ld_xml_attributes.h>
#include <ld_format_organizer.h>
#include <ld_visual_with_floating_children.h>
#include <ld_element_with_floating_children.h>

#include "test_element_with_floating_children.h"

/***********************************************************************************************************************
 * FloatingElement:
 */

class FloatingElement:public Ld::ElementWithFloatingChildren {
    public:
        FloatingElement(const QString& identifer = QString());

        ~FloatingElement() override;

        static QSharedPointer<FloatingElement> create(const QString& identifier = QString());

        static QSharedPointer<FloatingElement> create(
            Ld::VisualWithFloatingChildren* visual,
            const QString&                        identifier = QString()
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


long FloatingElement::currentNumberAllocatedInstances = 0;

FloatingElement::FloatingElement(const QString& identifier) {
    elementIdentifier = identifier; // Assists with debugging.
    currentNumberDescendants = 0;

    ++currentNumberAllocatedInstances;
}


FloatingElement::~FloatingElement() {
    --currentNumberAllocatedInstances;
}


QSharedPointer<FloatingElement> FloatingElement::create(const QString& identifier) {
    QSharedPointer<FloatingElement> element = QSharedPointer<FloatingElement>(new FloatingElement(identifier));
    element->setWeakThis(element.toWeakRef());

    return element;
}


QSharedPointer<FloatingElement> FloatingElement::create(
        Ld::VisualWithFloatingChildren* visual,
        const QString&                        identifier
    ) {
    QSharedPointer<FloatingElement> element = FloatingElement::create(identifier);
    element->setVisual(visual);

    return element;
}


QString FloatingElement::typeName() const {
    return QString("FloatingElement");
}


QString FloatingElement::plugInName() const {
    return QString();
}


QString FloatingElement::description() const {
    return QString();
}


Ld::DataType::ValueType FloatingElement::valueType() const {
    return Ld::DataType::ValueType::NONE;
}


Ld::Capabilities FloatingElement::parentRequires(unsigned long) const {
    return Ld::Capabilities();
}


Ld::Capabilities FloatingElement::childProvidesCapabilities() const {
    return Ld::Capabilities();
}


QString FloatingElement::identifier() const {
    return elementIdentifier;
}


unsigned FloatingElement::minimumNumberTextRegions() const {
    return 0;
}


unsigned FloatingElement::maximumNumberTextRegions() const {
    return static_cast<unsigned>(-1);
}


bool FloatingElement::setNumberTextRegions(unsigned newNumberTextRegions) {
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


unsigned FloatingElement::numberTextRegions() const {
    return static_cast<unsigned>(currentText.size());
}


bool FloatingElement::setText(const QString& newText, unsigned regionNumber) {
    Q_ASSERT(regionNumber < numberTextRegions());
    currentText[regionNumber] = newText;

    return true;
}


QString FloatingElement::text(unsigned regionNumber) const {
    Q_ASSERT(regionNumber < numberTextRegions());
    return currentText.at(regionNumber);
}


long FloatingElement::numberDescendants() const {
    return currentNumberDescendants;
}


Ld::ElementPointerSet FloatingElement::descendants() const {
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


long FloatingElement::numberAllocatedInstances() {
    return currentNumberAllocatedInstances;
}


void FloatingElement::clearNumberAllocatedInstances() {
    currentNumberAllocatedInstances = 0;
}


Ld::Element* FloatingElement::creator(const QString& typeName) {
    Element* element = new FloatingElement;
    Q_ASSERT(element->typeName() == typeName);

    return element;
}


void FloatingElement::descendantAdded(Ld::ElementPointer descendantElement) {
    Ld::Element::descendantAdded(descendantElement);
    ++currentNumberDescendants;
    currentDescendants.insert(descendantElement);
}


void FloatingElement::descendantRemoved(Ld::ElementPointer descendantElement) {
    Ld::Element::descendantRemoved(descendantElement);
    --currentNumberDescendants;
    currentDescendants.remove(descendantElement);
}

/***********************************************************************************************************************
 * FloatingVisual:
 */

class FloatingVisual:public Ld::VisualWithFloatingChildren {
    public:
        enum class LastCall: unsigned char {
            NONE = 0,
            ABOUT_TO_REMOVE_CHILD = 1,
            CHILD_INSERTED = 2,
            CHILD_RELOCATED = 3
        };

        FloatingVisual(const QString& identifer = QString());

        ~FloatingVisual() override;

        QString typeName() const final;

        QString plugInName() const final;

        QString identifier() const;

        static long numberAllocatedInstances();

        static void clearNumberAllocatedInstances();

        void clearCallData();

        LastCall lastCall() const;

        Ld::Location lastLocation() const;

        Ld::Location lastOldLocation() const;

        Ld::ElementPointer lastElement() const;

        static Ld::Visual* creator(const QString& typeName);

    protected:
        void aboutToRemoveChild(const Ld::Location& location, Ld::ElementPointer childElement) final;

        void childInserted(Ld::ElementPointer childElement, const Ld::Location& location) final;

        void childRelocated(
            Ld::ElementPointer  childElement,
            const Ld::Location& oldLocation,
            const Ld::Location& newLocation
        ) final;

    private:
        static long            currentNumberAllocatedInstances;

        QString                elementIdentifier;

        LastCall               currentLastCall;

        Ld::Location           currentLastLocation;

        Ld::Location           currentLastOldLocation;

        Ld::ElementWeakPointer currentLastElement;
};

long FloatingVisual::currentNumberAllocatedInstances = 0;

FloatingVisual::FloatingVisual(const QString& identifier) {
    elementIdentifier  = identifier; // Assists with debugging.
    clearCallData();

    ++currentNumberAllocatedInstances;
}


FloatingVisual::~FloatingVisual() {
    --currentNumberAllocatedInstances;
}


QString FloatingVisual::typeName() const {
    return QString("FloatingVisual");
}


QString FloatingVisual::plugInName() const{
    return QString();
}


QString FloatingVisual::identifier() const {
    return elementIdentifier;
}


long FloatingVisual::numberAllocatedInstances() {
    return currentNumberAllocatedInstances;
}


void FloatingVisual::clearNumberAllocatedInstances() {
    currentNumberAllocatedInstances = 0;
}


void FloatingVisual::clearCallData() {
    currentLastCall        = LastCall::NONE;
    currentLastLocation    = Ld::Location();
    currentLastOldLocation = Ld::Location();
    currentLastElement     = nullptr;
}


FloatingVisual::LastCall FloatingVisual::lastCall() const {
    return currentLastCall;
}


Ld::Location FloatingVisual::lastLocation() const {
    return currentLastLocation;
}


Ld::Location FloatingVisual::lastOldLocation() const {
    return currentLastOldLocation;
}


Ld::ElementPointer FloatingVisual::lastElement() const {
    return currentLastElement.toStrongRef();
}


void FloatingVisual::aboutToRemoveChild(const Ld::Location& location, Ld::ElementPointer childElement) {
    currentLastCall     = LastCall::ABOUT_TO_REMOVE_CHILD;
    currentLastLocation = location;
    currentLastElement  = childElement.toWeakRef();
}


void FloatingVisual::childInserted(Ld::ElementPointer childElement, const Ld::Location& location) {
    currentLastCall     = LastCall::CHILD_INSERTED;
    currentLastLocation = location;
    currentLastElement  = childElement.toWeakRef();
}


void FloatingVisual::childRelocated(
        Ld::ElementPointer  childElement,
        const Ld::Location& oldLocation,
        const Ld::Location& newLocation
    ) {
    currentLastCall        = LastCall::CHILD_RELOCATED;
    currentLastLocation    = newLocation;
    currentLastOldLocation = oldLocation;
    currentLastElement     = childElement.toWeakRef();
}


Ld::Visual* FloatingVisual::creator(const QString& typeName) {
    Visual* visual = new FloatingVisual;
    Q_ASSERT(visual->typeName() == typeName);

    return visual;
}

/***********************************************************************************************************************
 * TestElementWithFloatingChildren:
 */

TestElementWithFloatingChildren::TestElementWithFloatingChildren() {}


TestElementWithFloatingChildren::~TestElementWithFloatingChildren() {}


void TestElementWithFloatingChildren::initTestCase() {
    Ld::Handle::initialize(0x12345678UL);
    Ld::Location::setLeftToRightOrder(); // Make certain we're back to left to right ordering.
    Ld::Element::registerCreator("FloatingElement", &FloatingElement::creator);
}


void TestElementWithFloatingChildren::testHandleGeneration() {
    FloatingElement element1;
    FloatingElement element2;

    QVERIFY2(element1.handle() != element2.handle(),"Elements don't have unique handles.");
}


void TestElementWithFloatingChildren::testTypeName() {
    FloatingElement element;
    QCOMPARE(element.typeName(),QString("FloatingElement"));
}


void TestElementWithFloatingChildren::testCloneMethods() {
    QSharedPointer<FloatingElement> parent = FloatingElement::create();
    parent->setNumberTextRegions(1);
    parent->setText(QString("p"));

    QSharedPointer<FloatingElement> child0 = FloatingElement::create();
    child0->setNumberTextRegions(1);
    child0->setText(QString("c0"));
    parent->insertChild(child0, 0, 0, nullptr);

    QSharedPointer<FloatingElement> child1 = FloatingElement::create();
    child1->setNumberTextRegions(1);
    child1->setText(QString("c1"));
    parent->insertChild(child1, 1, 1, nullptr);

    QSharedPointer<FloatingElement> clonedParent = parent->clone(true).dynamicCast<FloatingElement>();

    QVERIFY(clonedParent->numberTextRegions() == 1);
    QVERIFY(clonedParent->text() == QString("p"));

    QMap<Ld::Location, Ld::ElementPointer> childrenByLocation = clonedParent->childrenByLocation();
    QVERIFY(childrenByLocation.size() == 2);

    QMap<Ld::Location, Ld::ElementPointer>::const_iterator it = childrenByLocation.constBegin();

    Ld::Location                    location1    = it.key();
    QSharedPointer<FloatingElement> clonedChild1 = it.value().dynamicCast<FloatingElement>();

    ++it;

    Ld::Location                    location2    = it.key();
    QSharedPointer<FloatingElement> clonedChild2 = it.value().dynamicCast<FloatingElement>();

    QVERIFY(location1 == Ld::Location(0, 0));
    QVERIFY(clonedChild1->numberTextRegions() == 1);
    QVERIFY(clonedChild1->text() == QString("c0"));

    QVERIFY(location2 == Ld::Location(1, 1));
    QVERIFY(clonedChild2->numberTextRegions() == 1);
    QVERIFY(clonedChild2->text() == QString("c1"));
}


void TestElementWithFloatingChildren::testChildPlacement() {
    FloatingElement element;
    QCOMPARE(element.childPlacement(), Ld::Element::ChildPlacement::FLOATING);
}


void TestElementWithFloatingChildren::testParentChildTreeFunctions() {
    QSharedPointer<FloatingElement> parent1 = FloatingElement::create();
    QSharedPointer<FloatingElement> parent2 = FloatingElement::create();

    QSharedPointer<FloatingElement> child1 = FloatingElement::create();
    QSharedPointer<FloatingElement> child2 = FloatingElement::create();
    QSharedPointer<FloatingElement> child3 = FloatingElement::create();
    QSharedPointer<FloatingElement> child4 = FloatingElement::create();

    // Verify default parent() return values.

    QVERIFY2(!parent1->parent(), "parent() not null (parent1).");
    QVERIFY2(!parent2->parent(), "parent() not null (parent2).");
    QVERIFY2(!child1->parent(), "parent() not null (child1).");
    QVERIFY2(!child2->parent(), "parent() not null (child2).");
    QVERIFY2(!child3->parent(), "parent() not null (child3).");
    QVERIFY2(!child4->parent(), "parent() not null (child4).");

    // Verify default value returned by numberChildren method

    QVERIFY2(parent1->numberChildren() == 0, "Unexpected number of children (parent1).");
    QVERIFY2(parent2->numberChildren() == 0, "Unexpected number of children (parent2).");

    // Insert children in a square pattern and confirm the number of children, reported order, and locations using each
    // available method.
    //
    //     Location Pattern:      Insertion Order:       Expected Order:
    //        4   1                 1 2 3 4                4 1 3 2
    //        3   2

    parent1->insertChild(child1, Ld::Location(1, 0), nullptr);
    parent1->insertChild(child2, Ld::Location(1, 1), nullptr);
    parent1->insertChild(child3, 0, 1, nullptr);
    parent1->insertChild(child4, 0, 0, nullptr);

    QVERIFY2(parent1->numberChildren() == 4, "Unexpected number of children after insertion.");

    QVERIFY(parent1->child(0) == child4);
    QVERIFY(parent1->child(1) == child1);
    QVERIFY(parent1->child(2) == child3);
    QVERIFY(parent1->child(3) == child2);

    Ld::ElementPointerList childList = parent1->children();
    QVERIFY(childList.size() == 4);
    QVERIFY(childList.at(0) == child4);
    QVERIFY(childList.at(1) == child1);
    QVERIFY(childList.at(2) == child3);
    QVERIFY(childList.at(3) == child2);

    QMap<Ld::Location, Ld::ElementPointer> childMap = parent1->childrenByLocation();
    QMap<Ld::Location, Ld::ElementPointer>::iterator childMapIterator = childMap.begin();

    QVERIFY(childMap.size() == 4);
    QVERIFY((childMapIterator+0).key() == Ld::Location(0, 0) && (childMapIterator+0).value() == child4);
    QVERIFY((childMapIterator+1).key() == Ld::Location(1, 0) && (childMapIterator+1).value() == child1);
    QVERIFY((childMapIterator+2).key() == Ld::Location(0, 1) && (childMapIterator+2).value() == child3);
    QVERIFY((childMapIterator+3).key() == Ld::Location(1, 1) && (childMapIterator+3).value() == child2);

    QVERIFY(parent1->childLocation(static_cast<unsigned long>(0)) == Ld::Location(0, 0));
    QVERIFY(parent1->childLocation(1) == Ld::Location(1, 0));
    QVERIFY(parent1->childLocation(2) == Ld::Location(0, 1));
    QVERIFY(parent1->childLocation(3) == Ld::Location(1, 1));

    QVERIFY(parent1->childLocation(child1) == Ld::Location(1, 0));
    QVERIFY(parent1->childLocation(child2) == Ld::Location(1, 1));
    QVERIFY(parent1->childLocation(child3) == Ld::Location(0, 1));
    QVERIFY(parent1->childLocation(child4) == Ld::Location(0, 0));

    // Confirm that each child knows who its parent is.

    QVERIFY(child1->parent() == parent1);
    QVERIFY(child2->parent() == parent1);
    QVERIFY(child3->parent() == parent1);
    QVERIFY(child4->parent() == parent1);

    // Remove a child and confirm the reported children are correct.  Also confirm parent/child relationships.

    bool success = parent1->removeChild(child1, nullptr);
    QVERIFY2(success, "removeChild failed (1).");
    QVERIFY(parent1->numberChildren() == 3);
    QVERIFY(parent1->child(0) == child4);
    QVERIFY(parent1->child(1) == child3);
    QVERIFY(parent1->child(2) == child2);

    QVERIFY(!child1->parent());
    QVERIFY(child2->parent() == parent1);
    QVERIFY(child3->parent() == parent1);
    QVERIFY(child4->parent() == parent1);

    // Move children 4 and 2 by different methods and confirm order is properly updated.  Locations are now:
    //
    //     Location pattern:   Expected order:
    //         2  4              2 4 3
    //         3

    success = parent1->relocateChild(child4, Ld::Location(1,0), nullptr);
    QVERIFY2(success, "relocateChild failed (1).");

    success = parent1->relocateChild(child2, 0, 0, nullptr);
    QVERIFY2(success, "relocateChild failed (2).");

    QVERIFY(parent1->numberChildren() == 3);
    QVERIFY(parent1->child(0) == child2);
    QVERIFY(parent1->child(1) == child4);
    QVERIFY(parent1->child(2) == child3);

    QVERIFY(child2->parent() == parent1);
    QVERIFY(child3->parent() == parent1);
    QVERIFY(child4->parent() == parent1);

    // Attempt to remove and relocate the missing child1 and confirm failure.

    success = parent1->removeChild(child1, nullptr);
    QVERIFY2(!success, "removeChild reported success with unparented child.");

    success = parent1->relocateChild(child1, 1, 1, nullptr);
    QVERIFY2(!success, "relocateChild reported success with unparented child.");

    // Attempt to relocate children between parents and make certain all the relationships still hold.
    //
    //   parent1:                                   parent2:
    //     Location pattern:   Expected order:        Location pattern:
    //         2                   2 4 3                    4
    //         3

    parent2->insertChild(child4, 1, 1, nullptr);

    QVERIFY(parent1->numberChildren() == 2);
    QVERIFY(parent1->child(0) == child2);
    QVERIFY(parent1->child(1) == child3);

    QVERIFY(parent2->numberChildren() == 1);
    QVERIFY(parent2->child(0) == child4);

    QVERIFY(child2->parent() == parent1);
    QVERIFY(child3->parent() == parent1);
    QVERIFY(child4->parent() == parent2);

    // Remove a child and make certain that the parents are informed.
    parent1->removeChild(child2, nullptr);

    QVERIFY(parent1->numberChildren() == 1);
    QVERIFY(parent1->child(0) == child3);
}


void TestElementWithFloatingChildren::testConstructorsDestructors() {
    FloatingElement::clearNumberAllocatedInstances();

    {
        QSharedPointer<FloatingElement> parent = FloatingElement::create(QString("p"));

        QSharedPointer<FloatingElement> child0 = FloatingElement::create(QString("c0"));
        parent->insertChild(child0, 0, 0, nullptr);

        QSharedPointer<FloatingElement> child1 = FloatingElement::create(QString("c1"));
        parent->insertChild(child1, 0, 1, nullptr);

        QSharedPointer<FloatingElement> child2 = FloatingElement::create(QString("c2"));
        parent->insertChild(child2, 0, 2, nullptr);

        child0->insertChild(FloatingElement::create(QString("c00")), 1, 0, nullptr);
        child0->insertChild(FloatingElement::create(QString("c02")), 2, 0, nullptr);

        child1->insertChild(FloatingElement::create(QString("c11")), 1, 1, nullptr);
        child1->insertChild(FloatingElement::create(QString("c12")), 2, 1, nullptr);

        child2->insertChild(FloatingElement::create(QString("c20")), 1, 2, nullptr);
        child2->insertChild(FloatingElement::create(QString("c21")), 2, 2, nullptr);

        QCOMPARE(FloatingElement::numberAllocatedInstances(), static_cast<long>(10));
    }

    QCOMPARE(FloatingElement::numberAllocatedInstances(), static_cast<long>(0));
}


void TestElementWithFloatingChildren::testRootSearch() {
    QSharedPointer<FloatingElement> parent = FloatingElement::create(QString("p"));

    QSharedPointer<FloatingElement> child0 = FloatingElement::create(QString("c0"));
    parent->insertChild(child0, 0, 0, nullptr);

    QSharedPointer<FloatingElement> child1 = FloatingElement::create(QString("c1"));
    parent->insertChild(child1, 0, 1, nullptr);

    QSharedPointer<FloatingElement> child2 = FloatingElement::create(QString("c2"));
    parent->insertChild(child2, 0, 2, nullptr);

    QSharedPointer<FloatingElement> child00 = FloatingElement::create(QString("c00"));
    QSharedPointer<FloatingElement> child01 = FloatingElement::create(QString("c01"));
    child0->insertChild(child00, 1, 0, nullptr);
    child0->insertChild(child01, 2, 0, nullptr);

    QSharedPointer<FloatingElement> child10 = FloatingElement::create(QString("c10"));
    QSharedPointer<FloatingElement> child11 = FloatingElement::create(QString("c11"));
    child1->insertChild(child10, 1, 1, nullptr);
    child1->insertChild(child11, 2, 1, nullptr);

    QSharedPointer<FloatingElement> child20 = FloatingElement::create(QString("c20"));
    QSharedPointer<FloatingElement> child21 = FloatingElement::create(QString("c21"));
    child2->insertChild(child20, 1, 2, nullptr);
    child2->insertChild(child21, 2, 2, nullptr);

    QList<QSharedPointer<FloatingElement>> elements;
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


void TestElementWithFloatingChildren::testDescendantTracking() {
    QSharedPointer<FloatingElement> parent = FloatingElement::create(QString("p"));

    QSharedPointer<FloatingElement> child0 = FloatingElement::create(QString("c0"));
    parent->insertChild(child0, 0, 0, nullptr);

    QSharedPointer<FloatingElement> child1 = FloatingElement::create(QString("c1"));
    parent->insertChild(child1, 0, 1, nullptr);

    QSharedPointer<FloatingElement> child2 = FloatingElement::create(QString("c2"));
    parent->insertChild(child2, 0, 2, nullptr);

    QSharedPointer<FloatingElement> child00 = FloatingElement::create(QString("c00"));
    QSharedPointer<FloatingElement> child01 = FloatingElement::create(QString("c01"));
    child0->insertChild(child00, 1, 0, nullptr);
    child0->insertChild(child01, 2, 0, nullptr);

    QSharedPointer<FloatingElement> child10 = FloatingElement::create(QString("c10"));
    QSharedPointer<FloatingElement> child11 = FloatingElement::create(QString("c11"));
    child1->insertChild(child10, 1, 1, nullptr);
    child1->insertChild(child11, 2, 1, nullptr);

    QSharedPointer<FloatingElement> child20 = FloatingElement::create(QString("c20"));
    QSharedPointer<FloatingElement> child21 = FloatingElement::create(QString("c21"));
    child2->insertChild(child20, 1, 2, nullptr);
    child2->insertChild(child21, 2, 2, nullptr);

    // Check if we received the correct number of notifications when children are added.

    QVERIFY(parent->numberDescendants() == 9);

    // Check that notification occurs when children are more gently removed.

    bool success = parent->removeChild(child0, nullptr);
    QVERIFY2(success, "removeChild failed unexpectedly.");

    QVERIFY(parent->numberDescendants() == 6);
}


void TestElementWithFloatingChildren::testVisualTracking() {
    FloatingVisual::clearNumberAllocatedInstances();

    // Verify assignment of visual in constructor.

    FloatingVisual* visual = new FloatingVisual;
    QSharedPointer<FloatingElement> element = FloatingElement::create(visual);

    QVERIFY2(element->visual() == visual, "Element did not return visual after assigning in the constructor.");

    // Confirm deleting the visual automatically removes it from the element.
    delete visual;
    QVERIFY2(element->visual() == nullptr, "Element holds pointer to visual after delete.");

    visual = new FloatingVisual();
    {
        QSharedPointer<FloatingElement> element2 = FloatingElement::create();
        element2->setVisual(visual);

        QVERIFY2(element2->visual() == visual, "Element did not return visual after assigning using setVisual.");
        QVERIFY2(FloatingVisual::numberAllocatedInstances() == 1, "Incorrect number of allocated Visual instances.");
    } // End of scope for element2

    QVERIFY2(FloatingVisual::numberAllocatedInstances() == 0, "Deleting element did not delete visual.");

    visual = new FloatingVisual;
    QSharedPointer<FloatingElement> parent = FloatingElement::create(visual);

    QVERIFY2(visual->lastCall() == FloatingVisual::LastCall::NONE, "Unexpected call state.");

    QSharedPointer<FloatingElement> child = FloatingElement::create();
    parent->insertChild(child, 1, 1, nullptr);

    QVERIFY2(
        visual->lastCall() == FloatingVisual::LastCall::CHILD_INSERTED,
        "Visual did not receive notification of child insertion."
    );
    QVERIFY2(visual->lastLocation() == Ld::Location(1,1), "Visual received the wrong location.");
    QVERIFY2(visual->lastElement() == child, "Visual received pointer to wrong child element.");

    visual->clearCallData();

    parent->relocateChild(child, 2, 2, nullptr);

    QVERIFY2(
        visual->lastCall() == FloatingVisual::LastCall::CHILD_RELOCATED,
        "Visual did not receive notification of child relocation."
    );
    QVERIFY2(
        visual->lastLocation() == Ld::Location(2, 2),
        "Visual received the wrong child destination location."
    );
    QVERIFY2(
        visual->lastOldLocation() == Ld::Location(1, 1),
        "Visual received the wrong child start location."
    );
    QVERIFY2(visual->lastElement() == child, "Visual received pointer to wrong child element.");

    visual->clearCallData();

    parent->removeChild(child, nullptr);

    QVERIFY2(
        visual->lastCall() == FloatingVisual::LastCall::ABOUT_TO_REMOVE_CHILD,
        "Visual did not receive notification of child removal."
    );
    QVERIFY2(
        visual->lastLocation() == Ld::Location(2,2),
        "Visual received the wrong child location during removal."
    );
    QVERIFY2(visual->lastElement() == child, "Visual received pointer to wrong child element during removal.");
}


void TestElementWithFloatingChildren::testCreatorFunctions() {
    bool success = Ld::Element::registerCreator("FloatingElement", FloatingElement::creator);
//    QVERIFY2(success, "Element::registerCreator failed."); // Creater already registered elsewhere.

    success = Ld::Element::registerCreator("FloatingElement", FloatingElement::creator);
    QVERIFY2(!success, "Element::registerCreator allowed duplicate registration.");

    Ld::ElementPointer element = Ld::Element::create("FloatingElement");
    QVERIFY2(element, "Creator method returned a null pointer.");
    QVERIFY2(element->typeName() == "FloatingElement", "Creator method returned the wrong element.");

    success = Ld::Visual::registerCreator("FloatingVisual", FloatingVisual::creator);
    QVERIFY2(success, "Visual::registerCreator failed.");

    success = Ld::Visual::registerCreator("FloatingVisual", FloatingVisual::creator);
    QVERIFY2(!success, "Visual::registerCreator allowed duplicate registration.");

    Ld::Visual* visual = Ld::Visual::create("FloatingVisual");
    QVERIFY2(visual != nullptr, "Creator method returned a null pointer.");
    QVERIFY2(visual->typeName() == "FloatingVisual", "Creator method returned the wrong visual.");

    delete visual;
}


void TestElementWithFloatingChildren::testRemoveChildrenMethod() {
    FloatingElement::clearNumberAllocatedInstances();
    QSharedPointer<FloatingElement> parent = FloatingElement::create(QString("p"));

    {
        QSharedPointer<FloatingElement> child0 = FloatingElement::create(QString("c0"));
        parent->insertChild(child0, 0, 0, nullptr);

        QSharedPointer<FloatingElement> child1 = FloatingElement::create(QString("c1"));
        parent->insertChild(child1, 0, 1, nullptr);

        QSharedPointer<FloatingElement> child2 = FloatingElement::create(QString("c2"));
        parent->insertChild(child2, 1, 0, nullptr);

        QVERIFY(FloatingElement::numberAllocatedInstances() == 4);

        parent->removeChildren(nullptr);

        QVERIFY(parent->numberChildren() == 0);
    }

    QVERIFY(FloatingElement::numberAllocatedInstances() == 1);
}


void TestElementWithFloatingChildren::testIndexOfChildMethod() {
    QSharedPointer<FloatingElement> parent = FloatingElement::create();

    QSharedPointer<FloatingElement> child0 = FloatingElement::create();
    QSharedPointer<FloatingElement> child1 = FloatingElement::create();
    QSharedPointer<FloatingElement> child2 = FloatingElement::create();
    QSharedPointer<FloatingElement> child3 = FloatingElement::create();

    parent->insertChild(child0, 0, 0, nullptr);
    parent->insertChild(child1, 1, 0, nullptr);
    parent->insertChild(child2, 0, 1, nullptr);

    unsigned long index = parent->indexOfChild(child0);
    QCOMPARE(index, 0U);

    index = parent->indexOfChild(child1);
    QCOMPARE(index, 1U);

    index = parent->indexOfChild(child2);
    QCOMPARE(index, 2U);

    index = parent->indexOfChild(child3);
    QCOMPARE(index, Ld::Element::invalidChildIndex);
}


void TestElementWithFloatingChildren::testRemoveChildCursorUpdates() {
    QSharedPointer<FloatingElement> rootElement   = FloatingElement::create();
    QSharedPointer<FloatingElement> parentElement = FloatingElement::create();
    QSharedPointer<FloatingElement> childElement1 = FloatingElement::create();
    QSharedPointer<FloatingElement> childElement2 = FloatingElement::create();
    QSharedPointer<FloatingElement> childElement3 = FloatingElement::create();

    rootElement->insertChild(parentElement, 0, 0, nullptr);

    parentElement->insertChild(childElement1, 1, 0, nullptr);
    parentElement->insertChild(childElement2, 2, 0, nullptr);
    parentElement->insertChild(childElement3, 3, 0, nullptr);

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


void TestElementWithFloatingChildren::testRemoveChildrenCursorUpdates() {
    QSharedPointer<FloatingElement> rootElement   = FloatingElement::create();
    QSharedPointer<FloatingElement> parentElement = FloatingElement::create();
    QSharedPointer<FloatingElement> childElement = FloatingElement::create();

    rootElement->insertChild(parentElement, 0, 0, nullptr);
    parentElement->insertChild(childElement, 0, 0, nullptr);

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
