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
* This file implements tests for the \ref Ld::FormatContainer class.  Also provides some testing of the
* \ref Ld::Format class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QSharedPointer>
#include <QMap>
#include <QtTest/QtTest>

#include <cassert>

#include <ld_format_structures.h>
#include <ld_xml_writer.h>
#include <ld_xml_reader.h>
#include <ld_xml_attributes.h>
#include <ld_format.h>
#include <ld_format_container.h>

#include "test_format_container.h"

/***********************************************************************************************************************
 * Class TestFormat:
 */

class TestFormat:public Ld::Format {
    public:
        TestFormat(unsigned value = 0);

        TestFormat(const TestFormat& other);

        ~TestFormat() override;

        Ld::FormatPointer clone() const final;

        static Ld::Format* creator(const QString& typeName);

        QString typeName() const final;

        bool isValid() const final;

        QString toString() const override;

        QString toCss() const override;

        static Ld::Format* defaultCreator(const QString& formatType);

        static Ld::Format* xmlCreator(
            const QString&                      formatType,
            QSharedPointer<Ld::XmlReader> reader,
            const Ld::XmlAttributes&      xmlAttributes
        );

        void writeAddImmediateAttributes(
            Ld::XmlAttributes&                  attributes,
            QSharedPointer<Ld::FormatOrganizer> formats,
            Ld::ProgramFile&                    programFile
        ) const override;

        void readImmediateAttributes(
            QSharedPointer<Ld::XmlReader>  reader,
            const Ld::XmlAttributes&       attributes,
            const Ld::FormatsByIdentifier& formats,
            Ld::ProgramFile&               programFile,
            unsigned                       xmlVersion
        ) override;

    private:
        unsigned currentValue;
};


TestFormat::TestFormat(unsigned value) {
    currentValue = value;
}


TestFormat::TestFormat(const TestFormat& other):Format(other) {
    currentValue = other.currentValue;
}


TestFormat::~TestFormat() {}


Ld::FormatPointer TestFormat::clone() const {
    return Ld::FormatPointer(new TestFormat(*this));
}


Ld::Format* TestFormat::creator(const QString&) {
    return new TestFormat();
}


QString TestFormat::typeName() const {
    return QString("TestFormat");
}


bool TestFormat::isValid() const {
    return true;
}


QString TestFormat::toString() const {
    return QString("TestFormat,%1").arg(currentValue);
}


QString TestFormat::toCss() const {
    return QString();
}


Ld::Format* TestFormat::defaultCreator(const QString& formatType) {
    Ld::Format* newFormat;

    if (formatType == QString("TestFormat")) {
        newFormat = new TestFormat(0);
    } else {
        newFormat = nullptr;
    }

    return newFormat;
}


Ld::Format* TestFormat::xmlCreator(
        const QString&,QSharedPointer<Ld::XmlReader>,
        const Ld::XmlAttributes&
    ) {
    // We test this capability in test_program_load_save rather than here.
    return nullptr;
}


void TestFormat::writeAddImmediateAttributes(
        Ld::XmlAttributes&,
        QSharedPointer<Ld::FormatOrganizer>,
        Ld::ProgramFile&
    ) const {
    // We test this capability in test_program_load_save rather than here.
}


void TestFormat::readImmediateAttributes(
        QSharedPointer<Ld::XmlReader>,
        const Ld::XmlAttributes&,
        const Ld::FormatsByIdentifier&,
        Ld::ProgramFile&,
        unsigned
    ) {
    // We test this capability in test_program_load_save rather than here.
}

/***********************************************************************************************************************
 * Class TestFormatContainer:
 */

TestFormatContainer::TestFormatContainer() {}


TestFormatContainer::~TestFormatContainer() {}


void TestFormatContainer::initTestCase() {}


void TestFormatContainer::testConstructorsAndDestructors() {
    Ld::FormatContainer container1;
    QVERIFY(container1.format() == nullptr);

    TestFormat* testFormat = new TestFormat(1);
    Ld::FormatContainer container2(testFormat);
    QVERIFY(container2.format()->toString() == testFormat->toString());
}


void TestFormatContainer::testFormatCreatorMethods() {
    bool success = Ld::Format::registerCreator("TestFormat", TestFormat::creator);
    QVERIFY2(success, "Format::registerCreator failed.");

    success = Ld::Format::registerCreator("TestFormat", TestFormat::creator);
    QVERIFY2(!success, "Format::registerCreator allowed duplicate registration.");

    Ld::FormatPointer newFormat = Ld::Format::create("TestFormat");
    QVERIFY2(newFormat != nullptr, "Creator method returned a null pointer.");
    if (newFormat != nullptr) {
        QVERIFY2(newFormat->typeName() == "TestFormat", "Creator method returned the wrong format.");
    }
}


void TestFormatContainer::testAccessors() {
    Ld::FormatContainer container;
    QVERIFY(container.format() == nullptr);

    TestFormat* testFormat = new TestFormat(1);
    container.setFormat(testFormat);

    QVERIFY(container.format()->toString() == testFormat->toString());
    delete testFormat;
}


void TestFormatContainer::testAssignmentsAndCasts() {
    Ld::FormatContainer container1;
    Ld::FormatContainer container2;

    TestFormat* testFormat1 = new TestFormat(1);

    container1 = testFormat1;
    QVERIFY(container1.format()->toString() == testFormat1->toString());

    Ld::Format& testFormat2 = *container1;
    QVERIFY(testFormat1->toString() == testFormat2.toString());

    container2 = container1;
    QVERIFY(container2.format()->toString() == testFormat1->toString());
}


void TestFormatContainer::testSorting() {
    Ld::FormatContainer container1(new TestFormat(1));
    Ld::FormatContainer container2(new TestFormat(2));
    Ld::FormatContainer container3(new TestFormat(2));

    QVERIFY(container1 < container2);
    QVERIFY((container2 < container3) == false);
}
