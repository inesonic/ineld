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
* This file implements tests of the Ld::elementWithNoChildren class.  Also tests core functions of the \ref Ld::Element
* class.
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
#include <ld_element_cursor.h>
#include <ld_cursor.h>
#include <ld_cursor_state_collection.h>
#include <ld_root_element.h>
#include <ld_capabilities.h>
#include <ld_element.h>
#include <ld_program_file.h>
#include <ld_xml_writer.h>
#include <ld_xml_reader.h>
#include <ld_xml_attributes.h>
#include <ld_format.h>
#include <ld_character_format.h>
#include <ld_format_organizer.h>
#include <ld_visual_with_no_children.h>
#include <ld_element_with_no_children.h>

#include "test_element_with_no_children.h"

/***********************************************************************************************************************
 * ChildlessElement:
 */

class ChildlessElement:public Ld::ElementWithNoChildren {
    public:
        ChildlessElement(const QString& identifier = QString());

        ~ChildlessElement() override;

        static QSharedPointer<ChildlessElement> create(const QString& identifier = QString());

        static QSharedPointer<ChildlessElement> create(
            Ld::VisualWithNoChildren* visual,
            const QString&            identifier = QString()
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

        static long numberAllocatedInstances();

        static void clearNumberAllocatedInstances();

        static Ld::Element* creator(const QString& typeName);

    private:
        static long    currentNumberAllocatedInstances;
        QString        elementIdentifier;
        QList<QString> currentText;
};


long ChildlessElement::currentNumberAllocatedInstances = 0;

ChildlessElement::ChildlessElement(const QString& identifier) {
    elementIdentifier = identifier; // Assists with debugging.
    ++currentNumberAllocatedInstances;
}


ChildlessElement::~ChildlessElement() {
    --currentNumberAllocatedInstances;
}


QSharedPointer<ChildlessElement> ChildlessElement::create(const QString& identifier) {
    QSharedPointer<ChildlessElement> element = QSharedPointer<ChildlessElement>(new ChildlessElement(identifier));
    element->setWeakThis(element.toWeakRef());

    return element;
}


QSharedPointer<ChildlessElement> ChildlessElement::create(
        Ld::VisualWithNoChildren* visual,
        const QString&            identifier
    ) {
    QSharedPointer<ChildlessElement> element = ChildlessElement::create(identifier);
    element->setVisual(visual);

    return element;
}


QString ChildlessElement::typeName() const {
    return QString("ChildlessElement");
}


QString ChildlessElement::plugInName() const {
    return QString();
}


QString ChildlessElement::description() const {
    return QString();
}


Ld::DataType::ValueType ChildlessElement::valueType() const {
    return Ld::DataType::ValueType::NONE;
}


Ld::Capabilities ChildlessElement::parentRequires(unsigned long) const {
    return Ld::Capabilities();
}


Ld::Capabilities ChildlessElement::childProvidesCapabilities() const {
    return Ld::Capabilities();
}


QString ChildlessElement::identifier() const {
    return elementIdentifier;
}


unsigned ChildlessElement::minimumNumberTextRegions() const {
    return 0;
}


unsigned ChildlessElement::maximumNumberTextRegions() const {
    return static_cast<unsigned>(-1);
}


bool ChildlessElement::setNumberTextRegions(unsigned newNumberTextRegions) {
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


unsigned ChildlessElement::numberTextRegions() const {
    return static_cast<unsigned>(currentText.size());
}


bool ChildlessElement::setText(const QString& newText, unsigned regionNumber) {
    Q_ASSERT(regionNumber < numberTextRegions());
    currentText[regionNumber] = newText;

    return true;
}


QString ChildlessElement::text(unsigned regionNumber) const {
    Q_ASSERT(regionNumber < numberTextRegions());
    return currentText.at(regionNumber);
}


long ChildlessElement::numberAllocatedInstances() {
    return currentNumberAllocatedInstances;
}


void ChildlessElement::clearNumberAllocatedInstances() {
    currentNumberAllocatedInstances = 0;
}


Ld::Element* ChildlessElement::creator(const QString& typeName) {
    ChildlessElement* element = new ChildlessElement;
    Q_ASSERT(element->typeName() == typeName);

    return element;
}

/***********************************************************************************************************************
 * ChildlessVisual:
 */

class ChildlessVisual:public Ld::VisualWithNoChildren {
    public:
        enum class LastCall: unsigned char {
            NONE = 0,
            ABOUT_TO_REMOVE_CHILD = 1,
            CHILD_SET = 2
        };

        ChildlessVisual(const QString& identifer = QString());

        ~ChildlessVisual() override;

        QString typeName() const final;

        QString plugInName() const final;

        QString identifier() const;

        static long numberAllocatedInstances();

        static void clearNumberAllocatedInstances();

        static Ld::Visual* creator(const QString& typeName);

    private:
        static long currentNumberAllocatedInstances;
        QString     elementIdentifier;
};

long ChildlessVisual::currentNumberAllocatedInstances = 0;

ChildlessVisual::ChildlessVisual(const QString& identifier) {
    elementIdentifier  = identifier; // Assists with debugging.
    ++currentNumberAllocatedInstances;
}


ChildlessVisual::~ChildlessVisual() {
    --currentNumberAllocatedInstances;
}


QString ChildlessVisual::typeName() const {
    return QString("ChildlessVisual");
}


QString ChildlessVisual::plugInName() const{
    return QString();
}


QString ChildlessVisual::identifier() const {
    return elementIdentifier;
}


long ChildlessVisual::numberAllocatedInstances() {
    return currentNumberAllocatedInstances;
}


void ChildlessVisual::clearNumberAllocatedInstances() {
    currentNumberAllocatedInstances = 0;
}


Ld::Visual* ChildlessVisual::creator(const QString& typeName) {
    Visual* visual = new ChildlessVisual;
    Q_ASSERT(visual->typeName() == typeName);

    return visual;
}

/***********************************************************************************************************************
 * TestElementWithNoChildren:
 */

TestElementWithNoChildren::TestElementWithNoChildren() {}


TestElementWithNoChildren::~TestElementWithNoChildren() {}


void TestElementWithNoChildren::initTestCase() {
    Ld::Handle::initialize(0x12345678UL);

    Ld::Element::registerCreator("ChildlessElement", &ChildlessElement::creator);
    Ld::Format::registerCreator(Ld::CharacterFormat::formatName, &Ld::CharacterFormat::creator);
}


void TestElementWithNoChildren::testHandleGeneration() {
    ChildlessElement element1;
    ChildlessElement element2;

    QVERIFY2(element1.handle() != element2.handle(),"Elements don't have unique handles.");
}


void TestElementWithNoChildren::testTypeName() {
    ChildlessElement element;
    QCOMPARE(element.typeName(),QString("ChildlessElement"));
}


void TestElementWithNoChildren::testCloneMethods() {
    QSharedPointer<ChildlessElement> element = ChildlessElement::create();
    element->setNumberTextRegions(3);
    element->setText(QString("foo"), 0);
    element->setText(QString("bar"), 1);
    element->setText(QString("bang"), 2);
    element->setFormat(Ld::Format::create(Ld::CharacterFormat::formatName));

    QSharedPointer<ChildlessElement> cloned1 = element->clone(true).dynamicCast<ChildlessElement>();

    QVERIFY(cloned1->typeName() == QString("ChildlessElement"));
    QVERIFY(cloned1->format()->typeName() == Ld::CharacterFormat::formatName);
    QVERIFY(cloned1->numberTextRegions() == 3);
    QVERIFY(cloned1->text(0) == QString("foo"));
    QVERIFY(cloned1->text(1) == QString("bar"));
    QVERIFY(cloned1->text(2) == QString("bang"));

    QSharedPointer<ChildlessElement> cloned2 = element->clone(1, 1, 2, 2, true).dynamicCast<ChildlessElement>();

    QVERIFY(cloned2->typeName() == QString("ChildlessElement"));
    QVERIFY(cloned2->format()->typeName() == Ld::CharacterFormat::formatName);
    QVERIFY(cloned2->numberTextRegions() == 3);
    QVERIFY(cloned2->text(0).isEmpty());
    QVERIFY(cloned2->text(1) == QString("ar"));
    QVERIFY(cloned2->text(2) == QString("ba"));
}


void TestElementWithNoChildren::testChildPlacement() {
    ChildlessElement element;
    QCOMPARE(element.childPlacement(), Ld::Element::ChildPlacement::NONE);
}


void TestElementWithNoChildren::testParentChildTreeFunctions() {
    QSharedPointer<ChildlessElement> element = ChildlessElement::create();

    QCOMPARE(element->numberChildren(), 0UL);
    QVERIFY(element->children().isEmpty());
    QVERIFY(element->child(0).isNull());
}


void TestElementWithNoChildren::testVisualTracking() {
    ChildlessVisual::clearNumberAllocatedInstances();

    ChildlessVisual* visual = new ChildlessVisual;

    {
        // Verify assignment of visual in constructor.

        QSharedPointer<ChildlessElement> element = ChildlessElement::create(visual);

        QVERIFY2(element->visual() == visual, "Element did not return visual after assigning in the constructor.");

        // Confirm deleting the visual automatically removes it from the element.
        delete visual;
        QVERIFY2(element->visual() == nullptr, "Element holds pointer to visual after delete.");

        visual = new ChildlessVisual();
        element->setVisual(visual);

        QVERIFY2(element->visual() == visual, "Element did not return visual after assigning using setVisual.");
        QVERIFY2(ChildlessVisual::numberAllocatedInstances() == 1, "Incorrect number of allocated Visual instances.");
    }

    // Confirm deleting the element also deletes the visual.
    QVERIFY2(ChildlessVisual::numberAllocatedInstances() == 0, "Deleting element did not delete visual.");
}


void TestElementWithNoChildren::testCreatorFunctions() {
    bool success = Ld::Element::registerCreator("ChildlessElement", ChildlessElement::creator);
//    QVERIFY2(success, "Element::registerCreator failed."); // We now register the creator in initTest.

    success = Ld::Element::registerCreator("ChildlessElement", ChildlessElement::creator);
    QVERIFY2(!success, "Element::registerCreator allowed duplicate registration.");

    Ld::ElementPointer element = Ld::Element::create("ChildlessElement");
    QVERIFY2(element, "Creator method returned a null pointer.");
    QVERIFY2(element->typeName() == "ChildlessElement", "Creator method returned the wrong element.");

    success = Ld::Visual::registerCreator("ChildlessVisual", ChildlessVisual::creator);
    QVERIFY2(success, "Visual::registerCreator failed.");

    success = Ld::Visual::registerCreator("ChildlessVisual", ChildlessVisual::creator);
    QVERIFY2(!success, "Visual::registerCreator allowed duplicate registration.");

    Ld::Visual* visual = Ld::Visual::create("ChildlessVisual");
    QVERIFY2(visual != nullptr, "Creator method returned a null pointer.");
    QVERIFY2(visual->typeName() == "ChildlessVisual", "Creator method returned the wrong visual.");

    delete visual;
}


void TestElementWithNoChildren::testRemoveChildrenMethod() {
    ChildlessElement::clearNumberAllocatedInstances();
    QSharedPointer<ChildlessElement> element = ChildlessElement::create(QString("p"));
    bool success = element->removeChild(element, nullptr);
    QVERIFY(success == false);
}


void TestElementWithNoChildren::testIndexOfChildMethod() {
    QSharedPointer<ChildlessElement> element = ChildlessElement::create();

    unsigned long childIndex = element->indexOfChild(element);
    QCOMPARE(childIndex, Ld::Element::invalidChildIndex);
}


void TestElementWithNoChildren::testInsertTextMethod() {
    QSharedPointer<ChildlessElement> element = ChildlessElement::create();
    element->setNumberTextRegions(1);

    element->setText(QString("Eh"));
    bool success = element->insertText(QString("art"), 1, 0);
    QVERIFY(success);

    QCOMPARE(element->text(), QString("Earth"));

    element->setText(QString("Heart"));
    success = element->insertText(QString("burn"), Ld::Element::invalidTextIndex, Ld::Element::invalidRegionIndex);
    QVERIFY(success);

    QCOMPARE(element->text(), QString("Heartburn"));

    element->setText(QString("limited"));
    element->insertText(QString("Un"), 0, Ld::Element::invalidRegionIndex);

    QCOMPARE(element->text(), QString("Unlimited"));
}


void TestElementWithNoChildren::testRemoveTextMethod() {
    QSharedPointer<ChildlessElement> element = ChildlessElement::create();

    element->setNumberTextRegions(4);
    element->setText(QString("foo"), 0);
    element->setText(QString("bar"), 1);
    element->setText(QString("bang"), 2);
    element->setText(QString("boing"), 3);

    element->removeText(1, 1, 1, 3, nullptr);

    QVERIFY(element->numberTextRegions() == 4);
    QVERIFY(element->text(0) == QString("foo"));
    QVERIFY(element->text(1) == QString("b"));
    QVERIFY(element->text(2).isEmpty());
    QVERIFY(element->text(3) == QString("oing"));

    element->setText(QString("bar"), 1);
    element->setText(QString("bang"), 2);
    element->setText(QString("boing"), 3);

    element->removeText(1, 2, 3, 2, nullptr);

    QVERIFY(element->numberTextRegions() == 4);
    QVERIFY(element->text(0) == QString("foo"));
    QVERIFY(element->text(1) == QString("bar"));
    QVERIFY(element->text(2) == QString("bg"));
    QVERIFY(element->text(3) == QString("boing"));
}


void TestElementWithNoChildren::testInsertTextCursorUpdates() {
    QSharedPointer<Ld::RootElement>  rootElement(new Ld::RootElement);
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<ChildlessElement> textElement = ChildlessElement::create();
    textElement->setWeakThis(textElement.toWeakRef());
    textElement->setNumberTextRegions(1);

    rootElement->append(textElement, nullptr);

    textElement->setText(QString("Eh"));

    Ld::CursorPointer cursor1(new Ld::Cursor);
    cursor1->updateFromElementCursor(Ld::ElementCursor(0, 0, textElement));

    Ld::CursorPointer cursor2(new Ld::Cursor);
    cursor2->updateFromElementCursor(Ld::ElementCursor(1, 0, textElement));

    Ld::CursorPointer cursor3(new Ld::Cursor);
    cursor3->updateFromElementCursor(Ld::ElementCursor(2, 0, textElement));

    Ld::CursorStateCollection collection;
    collection.insert(cursor1);
    collection.insert(cursor2);
    collection.insert(cursor3);

    bool success = textElement->insertText(QString("art"), 1, 0, &collection);
    QVERIFY(success);

    collection.updateCursorState(false);

    QCOMPARE(textElement->text(), QString("Earth"));
    QCOMPARE(cursor1->elementCursor(), Ld::ElementCursor(0, 0, textElement));
    QCOMPARE(cursor2->elementCursor(), Ld::ElementCursor(1, 0, textElement));
    QCOMPARE(cursor3->elementCursor(), Ld::ElementCursor(5, 0, textElement));

    success = textElement->insertText(
        QString("bound"),
        Ld::Element::invalidTextIndex,
        Ld::Element::invalidRegionIndex,
        &collection,
        false
    );
    QVERIFY(success);

    collection.updateCursorState(false);

    QCOMPARE(textElement->text(), QString("Earthbound"));
    QCOMPARE(cursor1->elementCursor(), Ld::ElementCursor( 0, 0, textElement));
    QCOMPARE(cursor2->elementCursor(), Ld::ElementCursor( 1, 0, textElement));
    QCOMPARE(cursor3->elementCursor(), Ld::ElementCursor(10, 0, textElement));
}


void TestElementWithNoChildren::testRemoveTextCursorUpdates() {
    QSharedPointer<Ld::RootElement>  rootElement(new Ld::RootElement);
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<ChildlessElement> textElement = ChildlessElement::create();
    textElement->setWeakThis(textElement.toWeakRef());
    textElement->setNumberTextRegions(2);

    rootElement->append(textElement, nullptr);

    //                            0        1         2         3
    //                            1234567890123456789012345678901
    textElement->setText(QString("For he on honey-dew hath fed,"), 0);
    textElement->setText(QString("And drunk the milk of Paradise."), 1);

    Ld::CursorPointer cursor1(new Ld::Cursor);
    cursor1->updateFromElementCursor(Ld::ElementCursor(0, 0, textElement));

    Ld::CursorPointer cursor2(new Ld::Cursor);
    cursor2->updateFromElementCursor(Ld::ElementCursor(26, 0, textElement));

    Ld::CursorPointer cursor3(new Ld::Cursor);
    cursor3->updateFromElementCursor(Ld::ElementCursor(3, 1, textElement));

    Ld::CursorPointer cursor4(new Ld::Cursor);
    cursor4->updateFromElementCursor(Ld::ElementCursor(15, 1, textElement));

    Ld::CursorStateCollection collection;
    collection.insert(cursor1);
    collection.insert(cursor2);
    collection.insert(cursor3);
    collection.insert(cursor4);

    bool success = textElement->removeText(21, 0, 4, 1, &collection);
    QVERIFY(success);

    collection.updateCursorState(false);

    QCOMPARE(cursor1->elementCursor(), Ld::ElementCursor( 0, 0, textElement));
    QCOMPARE(cursor2->elementCursor(), Ld::ElementCursor(21, 0, textElement));
    QCOMPARE(cursor3->elementCursor(), Ld::ElementCursor( 0, 1, textElement));
    QCOMPARE(cursor4->elementCursor(), Ld::ElementCursor(11, 1, textElement));
}
