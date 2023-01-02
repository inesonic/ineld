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
* This file implements tests of the Ld::TextElement class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QtTest/QtTest>

#include <ld_element_structures.h>
#include <ld_element_cursor.h>
#include <ld_cursor.h>
#include <ld_cursor_state_collection.h>
#include <ld_root_element.h>
#include <ld_plug_in_manager.h>
#include <ld_character_format.h>
#include <ld_page_format.h>
#include <ld_text_visual.h>
#include <ld_text_element.h>

#include "test_text_element.h"

/***********************************************************************************************************************
 * TestTextVisual:
 */

class TestTextVisual:public Ld::TextVisual {
    public:
        TestTextVisual();

        ~TestTextVisual() override;

        bool textChangedCalled() const;

        bool elementDataChangedCalled() const;

        QString reportedText() const;

        void clear();

    protected:
        void elementDataChanged() override;

        void textChanged(const QString& newText) override;

    private:
        bool    elementDataChangedWasCalled;
        bool    textChangedWasCalled;
        QString currentReportedText;
};


TestTextVisual::TestTextVisual() {
    clear();
}


TestTextVisual::~TestTextVisual() {}


bool TestTextVisual::textChangedCalled() const {
    return textChangedWasCalled;
}


bool TestTextVisual::elementDataChangedCalled() const {
    return elementDataChangedWasCalled;
}


QString TestTextVisual::reportedText() const {
    return currentReportedText;
}


void TestTextVisual::clear() {
    elementDataChangedWasCalled = false;
    textChangedWasCalled        = false;

    currentReportedText.clear();
}


void TestTextVisual::elementDataChanged() {
    elementDataChangedWasCalled = true;
}


void TestTextVisual::textChanged(const QString& newText) {
    textChangedWasCalled = true;
    currentReportedText  = newText;
}

/***********************************************************************************************************************
 * TestTextElement:
 */

TestTextElement::TestTextElement() {}


TestTextElement::~TestTextElement() {}


void TestTextElement::initTestCase() {
    Ld::Element::registerCreator("Root", Ld::RootElement::creator);
    Ld::Element::registerCreator("Text", Ld::TextElement::creator);
    Ld::Format::registerCreator("CharacterFormat", Ld::CharacterFormat::creator);
    Ld::Format::registerCreator("PageFormat", Ld::PageFormat::creator);
}


void TestTextElement::testTypeName() {
    QSharedPointer<Ld::TextElement> element(new Ld::TextElement());
    element->setWeakThis(element.toWeakRef());

    QVERIFY(element->typeName() == "Text");
    QVERIFY(element->plugInName().isEmpty());
}


void TestTextElement::testAccessors() {
    QSharedPointer<Ld::TextElement> element(new Ld::TextElement());
    element->setWeakThis(element.toWeakRef());

    TestTextVisual* visual = new TestTextVisual();
    element->setVisual(visual);

    QVERIFY(element->text().isEmpty());

    QVERIFY(visual->textChangedCalled() == false);
    QVERIFY(visual->elementDataChangedCalled() == false);
    QVERIFY(visual->reportedText().isEmpty());

    element->setText("Some text to try.");
    QVERIFY(element->text() == QString("Some text to try."));

    QVERIFY(visual->textChangedCalled() == true);
    QVERIFY(visual->elementDataChangedCalled() == true);
    QVERIFY(visual->reportedText() == QString("Some text to try."));
}


void TestTextElement::testSaveLoadMethods() {
    QSharedPointer<Ld::RootElement> rootElement1(new Ld::RootElement);
    rootElement1->setWeakThis(rootElement1.toWeakRef());

    bool success = rootElement1->openNew();
    QVERIFY(success);

    QSharedPointer<Ld::TextElement> textElement1(new Ld::TextElement);
    textElement1->setWeakThis(textElement1.toWeakRef());

    textElement1->setText("Some text to be saved !");
    textElement1->setFormat(new Ld::CharacterFormat("Courier", 10));

    rootElement1->append(textElement1, nullptr);

    success = rootElement1->saveAs("test_container.dat");
    QVERIFY(success);

    success = rootElement1->close();
    QVERIFY(success);

    QSharedPointer<Ld::RootElement> rootElement2(new Ld::RootElement);
    rootElement2->setWeakThis(rootElement2.toWeakRef());

    Ld::PlugInsByName plugInsByName;
    success = rootElement2->openExisting("test_container.dat", true, plugInsByName);
    QVERIFY(success);

    QVERIFY(rootElement2->numberChildren() == 1);
    Ld::ElementPointer childElement = rootElement2->child(0);
    QVERIFY(childElement->typeName() == QString("Text"));

    QSharedPointer<Ld::TextElement> textElement2 = childElement.dynamicCast<Ld::TextElement>();
    QVERIFY(!textElement2.isNull());

    QVERIFY(textElement2->text() == QString("Some text to be saved !"));

    QSharedPointer<Ld::CharacterFormat> format = textElement2->format().dynamicCast<Ld::CharacterFormat>();
    QVERIFY(!format.isNull());

    QVERIFY(format->family() == QString("Courier"));
    QVERIFY(format->fontSize() == 10);

    success = rootElement2->close();
    QVERIFY(success);
}
