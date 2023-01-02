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
* This file implements tests of the Ld::PayloadElement class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QSharedPointer>
#include <QByteArray>
#include <QtTest/QtTest>

#include <ld_element_structures.h>
#include <ld_data_type.h>
#include <ld_root_element.h>
#include <ld_plug_in_manager.h>
#include <ld_character_format.h>
#include <ld_page_format.h>
#include <ld_payload_visual.h>
#include <ld_payload_element.h>

#include "test_payload_element.h"

/***********************************************************************************************************************
 * TestPayloadVisual:
 */

class TestPayloadVisual:public Ld::PayloadVisual {
    public:
        TestPayloadVisual();

        ~TestPayloadVisual() override;

        QString typeName() const override;

        QString plugInName() const override;

        bool elementDataChangedCalled();

        bool payloadChangedCalled();

        bool payloadCouldNotBeLoadedCalled();

        void clear();

    protected:
        void elementDataChanged() override;

        void payloadChanged() override;

        void payloadCouldNotBeLoaded() override;

    private:
        bool elementDataChangedWasCalled;
        bool payloadChangedWasCalled;
        bool payloadCouldNotBeLoadedWasCalled;
};


TestPayloadVisual::TestPayloadVisual() {
    clear();
}


TestPayloadVisual::~TestPayloadVisual() {}


QString TestPayloadVisual::typeName() const {
    return QString();
}


QString TestPayloadVisual::plugInName() const {
    return QString();
}


bool TestPayloadVisual::elementDataChangedCalled() {
    return elementDataChangedWasCalled;
}


bool TestPayloadVisual::payloadChangedCalled() {
    return payloadChangedWasCalled;
}


bool TestPayloadVisual::payloadCouldNotBeLoadedCalled() {
    return payloadCouldNotBeLoadedWasCalled;
}


void TestPayloadVisual::clear() {
    elementDataChangedWasCalled      = false;
    payloadChangedWasCalled          = false;
    payloadCouldNotBeLoadedWasCalled = false;
}


void TestPayloadVisual::elementDataChanged() {
    elementDataChangedWasCalled = true;
}


void TestPayloadVisual::payloadChanged() {
    payloadChangedWasCalled = true;
}


void TestPayloadVisual::payloadCouldNotBeLoaded() {
    payloadCouldNotBeLoadedWasCalled = true;
}

/***********************************************************************************************************************
 * TestChildlessPayloadElement:
 */

class TestChildlessPayloadElement:public Ld::PayloadElement {
    public:
        static const QString elementName;

        TestChildlessPayloadElement();

        ~TestChildlessPayloadElement() override;

        static Ld::Element* creator(const QString& typeName);

        QString typeName() const final;

        QString plugInName() const final;

        QString description() const final;

        Ld::DataType::ValueType valueType() const final;

        Ld::Capabilities parentRequires(unsigned long index) const final;

        Ld::Capabilities childProvidesCapabilities() const final;
};


const QString TestChildlessPayloadElement::elementName("TestChildlessPayloadElement");

TestChildlessPayloadElement::TestChildlessPayloadElement() {}


TestChildlessPayloadElement::~TestChildlessPayloadElement() {}


Ld::Element* TestChildlessPayloadElement::creator(const QString&) {
    return new TestChildlessPayloadElement;
}


QString TestChildlessPayloadElement::typeName() const {
    return elementName;
}


QString TestChildlessPayloadElement::plugInName() const {
    return QString();
}


QString TestChildlessPayloadElement::description() const {
    return QString();
}


Ld::DataType::ValueType TestChildlessPayloadElement::valueType() const {
    return Ld::DataType::ValueType::NONE;
}


Ld::Capabilities TestChildlessPayloadElement::parentRequires(unsigned long) const {
    return Ld::Capabilities();
}


Ld::Capabilities TestChildlessPayloadElement::childProvidesCapabilities() const {
    return Ld::Capabilities();
}

/***********************************************************************************************************************
 * TestPayloadElement:
 */

TestPayloadElement::TestPayloadElement() {}


TestPayloadElement::~TestPayloadElement() {}


void TestPayloadElement::initTestCase() {
    Ld::Handle::initialize(0x12345678UL);
    Ld::Element::registerCreator(Ld::RootElement::elementName,  &Ld::RootElement::creator);
    Ld::Element::registerCreator(TestChildlessPayloadElement::elementName, &TestChildlessPayloadElement::creator);
}


void TestPayloadElement::testAccessors() {
    QSharedPointer<Ld::RootElement> rootElement = Ld::Element::create(Ld::RootElement::elementName)
                                                  .dynamicCast<Ld::RootElement>();
    bool success = rootElement->openNew();
    QVERIFY(success);

    QSharedPointer<TestChildlessPayloadElement> payloadElement = Ld::Element::create(
        TestChildlessPayloadElement::elementName
    ).dynamicCast<TestChildlessPayloadElement>();
    rootElement->append(payloadElement, nullptr);

    QString testString1(
        "The sun was shining on the sea,\n"
        "Shining with all his might:\n"
        "He did his very best to make\n"
        "The billows smooth and bright —\n"
        "And this was odd, because it was\n"
        "The middle of the night.\n"
    );

    QByteArray testData1 = testString1.toLocal8Bit();

    payloadElement->updatePayload(testData1);

    QSharedPointer<TestChildlessPayloadElement> clonedElement =
        payloadElement->clone(true).dynamicCast<TestChildlessPayloadElement>();

    QByteArray clonedData1;
    success = clonedElement->getPayload(clonedData1);
    QVERIFY(success);

    QCOMPARE(testData1, clonedData1);

    QString testString2(
        "The moon was shining sulkily,\n"
        "Because she thought the sun\n"
        "Had got no business to be there\n"
        "After the day was done —\n"
        "\"It's very rude of him,\" she said,\n"
        "To come and spoil the fun."
    );
    QByteArray testData2 = testString2.toLocal8Bit();

    success = payloadElement->updatePayload(testData2);
    QVERIFY(success);

    QByteArray clonedData2;
    success = clonedElement->getPayload(clonedData2);
    QVERIFY(success);

    QCOMPARE(testData1, clonedData2);

    QByteArray testData3;
    success = payloadElement->getPayload(testData3);
    QVERIFY(success);

    QCOMPARE(testData2, testData3);

    success = rootElement->saveAs("test_container.aion");
    QVERIFY(success);

    success = rootElement->close();
    QVERIFY(success);
}


void TestPayloadElement::testDataCloning() {
    QSharedPointer<Ld::RootElement> rootElement1 = Ld::Element::create(Ld::RootElement::elementName)
                                                  .dynamicCast<Ld::RootElement>();
    bool success = rootElement1->openNew();
    QVERIFY(success);

    QSharedPointer<TestChildlessPayloadElement> payloadElement1 = Ld::Element::create(
        TestChildlessPayloadElement::elementName
    ).dynamicCast<TestChildlessPayloadElement>();
    rootElement1->append(payloadElement1, nullptr);

    QString testString1(
        "The sea was we as wet could be,\n"
        "The sands were dry as dry.\n"
        "You could not see a cloud, because\n"
        "No cloud was in the sky:\n"
        "No birs were flying overhead --\n"
        "There were no birds to fly\n"
    );

    QByteArray testData1 = testString1.toLocal8Bit();
    success = payloadElement1->updatePayload(testData1);
    QVERIFY(success);

    QCOMPARE(payloadElement1->storageMode(), Ld::PayloadElement::StorageMode::STORED_IN_FILE);

    QSharedPointer<TestChildlessPayloadElement> payloadElement2 = payloadElement1->clone(true)
                                                                  .dynamicCast<TestChildlessPayloadElement>();

    QByteArray testData2;
    success = payloadElement2->getPayload(testData2);
    QVERIFY(success);

    QCOMPARE(testData1, testData2);

    QCOMPARE(payloadElement2->storageMode(), Ld::PayloadElement::StorageMode::STORED_IN_FILE);

    success = rootElement1->close();
    QVERIFY(success);

    QCOMPARE(payloadElement1->storageMode(), Ld::PayloadElement::StorageMode::STORED_IN_MEMORY);
    QCOMPARE(payloadElement2->storageMode(), Ld::PayloadElement::StorageMode::STORED_IN_MEMORY);

    QByteArray testData3;
    success = payloadElement1->getPayload(testData3);
    QVERIFY(success);

    QCOMPARE(testData1, testData3);

    QByteArray testData4;
    success = payloadElement2->getPayload(testData4);
    QVERIFY(success);

    QCOMPARE(testData1, testData4);

    QSharedPointer<Ld::RootElement> rootElement2 = Ld::Element::create(Ld::RootElement::elementName)
                                                  .dynamicCast<Ld::RootElement>();
    success = rootElement2->openNew();

    QSharedPointer<TestChildlessPayloadElement> payloadElement3 = payloadElement2->clone(true)
                                                                  .dynamicCast<TestChildlessPayloadElement>();

    QByteArray testData5;
    success = payloadElement3->getPayload(testData5);
    QVERIFY(success);

    QCOMPARE(testData1, testData5);

    QCOMPARE(payloadElement3->storageMode(), Ld::PayloadElement::StorageMode::STORED_IN_MEMORY);

    rootElement2->append(payloadElement3, nullptr);

    QByteArray testData6;
    success = payloadElement3->getPayload(testData6);
    QVERIFY(success);

    QCOMPARE(testData1, testData6);

    QCOMPARE(payloadElement3->storageMode(), Ld::PayloadElement::StorageMode::STORED_IN_FILE);
//    QCOMPARE(payloadElement2->storageMode(), Ld::PayloadElement::StorageMode::STORED_IN_FILE);

    success = rootElement2->close();
    QVERIFY(success);
}


void TestPayloadElement::testVisual() {
    // NOTE: This does not test the PayloadVisual::payloadCouldNotBeLoaded method.

    QSharedPointer<Ld::RootElement> rootElement = Ld::Element::create(Ld::RootElement::elementName)
                                                  .dynamicCast<Ld::RootElement>();
    bool success = rootElement->openNew();
    QVERIFY(success);

    QSharedPointer<TestChildlessPayloadElement> payloadElement = Ld::Element::create(
        TestChildlessPayloadElement::elementName
    ).dynamicCast<TestChildlessPayloadElement>();
    rootElement->append(payloadElement, nullptr);

    TestPayloadVisual* payloadVisual = new TestPayloadVisual;
    payloadElement->setVisual(payloadVisual);

    payloadVisual->clear();

    QVERIFY(!payloadVisual->elementDataChangedCalled());
    QVERIFY(!payloadVisual->payloadChangedCalled());

    QString testString1(
        "The sea was we as wet could be,\n"
        "The sands were dry as dry.\n"
        "You could not see a cloud, because\n"
        "No cloud was in the sky:\n"
        "No birs were flying overhead --\n"
        "There were no birds to fly\n"
    );

    QByteArray testData1 = testString1.toLocal8Bit();

    payloadElement->updatePayload(testData1);

    QVERIFY(payloadVisual->elementDataChangedCalled());
    QVERIFY(payloadVisual->payloadChangedCalled());

    success = rootElement->close();
    QVERIFY(success);
}


void TestPayloadElement::testSaveLoadMethods() {
    QString testString1(
        "The Walrus and the Carpenter\n"
        "Were walking close at hand;\n"
        "They wept like anything to see\n"
        "Such quantities of sand\n"
        "If this were only cleared away,\n"
        "They said, it would be grand !\n"
    );

    QByteArray testData1 = testString1.toLocal8Bit();

    {
        QSharedPointer<Ld::RootElement> rootElement = Ld::Element::create(Ld::RootElement::elementName)
                                                      .dynamicCast<Ld::RootElement>();
        bool success = rootElement->openNew();
        QVERIFY(success);

        QSharedPointer<TestChildlessPayloadElement> payloadElement = Ld::Element::create(
            TestChildlessPayloadElement::elementName
        ).dynamicCast<TestChildlessPayloadElement>();
        rootElement->append(payloadElement, nullptr);

        success = payloadElement->updatePayload(testData1);
        QVERIFY(success);

        success = rootElement->saveAs("test_container.aion");
        QVERIFY(success);

        success = rootElement->close();
        QVERIFY(success);
    }

    {
        QSharedPointer<Ld::RootElement> rootElement = Ld::Element::create(Ld::RootElement::elementName)
                                                      .dynamicCast<Ld::RootElement>();

        Ld::PlugInsByName plugInsByName;
        bool success = rootElement->openExisting("test_container.aion", false, plugInsByName);
        QVERIFY(success);

        QCOMPARE(rootElement->numberChildren(), 1U);
        Ld::ElementPointer element = rootElement->child(0);
        QCOMPARE(element->typeName(), TestChildlessPayloadElement::elementName);

        QSharedPointer<TestChildlessPayloadElement> testElement = element.dynamicCast<TestChildlessPayloadElement>();

        QByteArray testData2;
        success = testElement->getPayload(testData2);
        QVERIFY(success);

        QCOMPARE(testData1, testData2);

        success = rootElement->close();
        QVERIFY(success);
    }
}
