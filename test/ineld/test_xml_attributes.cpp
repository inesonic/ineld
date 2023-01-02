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
* This file implements tests of \ref Ld::XmlAttributes class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QtTest/QtTest>

#include <cstdint>
#include <limits>
#include <cmath>

#include <ld_handle.h>
#include <ld_xml_attributes.h>

#include "test_xml_attributes.h"


TestXmlAttributes::TestXmlAttributes() {}


TestXmlAttributes::~TestXmlAttributes() {}


void TestXmlAttributes::testConstructors() {
    Ld::XmlAttributes attributes1;
    QVERIFY(attributes1.size() == 0);

    attributes1.append("foo", "bar");
    QVERIFY(attributes1.size() == 1);
    QVERIFY(attributes1.hasAttribute("foo"));

    Ld::XmlAttributes attributes2 = attributes1;
    QVERIFY(attributes2.size() == 1);
    QVERIFY(attributes2.hasAttribute("foo"));

    Ld::XmlAttributes attributes3;
    QVERIFY(attributes3.size() == 0);

    attributes3 = attributes1;
    QVERIFY(attributes3.size() == 1);
    QVERIFY(attributes3.hasAttribute("foo"));
}


void TestXmlAttributes::testSigned8BitConversions() {
    Ld::XmlAttributes attributes;

    // Test against the maximum value

    typedef std::int8_t TestType;
    TestType minimumValue = std::numeric_limits<TestType>::min();
    TestType maximumValue = std::numeric_limits<TestType>::max();

    TestType valueIn = maximumValue;
    attributes.append("v1", valueIn);

    bool success;
    TestType valueOut = attributes.value<TestType>("v1", &success);
    QVERIFY(success);
    QVERIFY(valueOut == valueIn);

    // Test against the minimum value

    valueIn = minimumValue;
    attributes.append("v2", valueIn);

    valueOut = attributes.value<TestType>("v2", &success);
    QVERIFY(success);
    QVERIFY(valueOut == valueIn);

    // Test against an out-of-bounds value

    attributes.append("v3", INT64_MIN);
    valueOut = attributes.value<TestType>("v3", &success);
    QVERIFY(!success);

    // Test against a value that is invalid (e.g. non-number)

    attributes.append("v4", "This should fail badly.");
    valueOut = attributes.value<TestType>("v4", &success);
    QVERIFY(!success);

    // Test version that takes a default to confirm success.

    valueOut = attributes.value<TestType>("v2", static_cast<TestType>(1), &success);
    QVERIFY(success);
    QVERIFY(valueOut == valueIn);

    valueOut = attributes.value<TestType>("v5", static_cast<TestType>(1), &success);
    QVERIFY(success);
    QVERIFY(valueOut == 1);
}


void TestXmlAttributes::testSigned16BitConversions() {
    Ld::XmlAttributes attributes;

    // Test against the maximum value

    typedef std::int16_t TestType;
    TestType minimumValue = std::numeric_limits<TestType>::min();
    TestType maximumValue = std::numeric_limits<TestType>::max();

    TestType valueIn = maximumValue;
    attributes.append("v1", valueIn);

    bool success;
    TestType valueOut = attributes.value<TestType>("v1", &success);
    QVERIFY(success);
    QVERIFY(valueOut == valueIn);

    // Test against the minimum value

    valueIn = minimumValue;
    attributes.append("v2", valueIn);

    valueOut = attributes.value<TestType>("v2", &success);
    QVERIFY(success);
    QVERIFY(valueOut == valueIn);

    // Test against an out-of-bounds value

    attributes.append("v3", INT64_MIN);
    valueOut = attributes.value<TestType>("v3", &success);
    QVERIFY(!success);

    // Test against a value that is invalid (e.g. non-number)

    attributes.append("v4", "This should fail badly.");
    valueOut = attributes.value<TestType>("v4", &success);
    QVERIFY(!success);

    // Test version that takes a default to confirm success.

    valueOut = attributes.value<TestType>("v2", static_cast<TestType>(1), &success);
    QVERIFY(success);
    QVERIFY(valueOut == valueIn);

    valueOut = attributes.value<TestType>("v5", static_cast<TestType>(1), &success);
    QVERIFY(success);
    QVERIFY(valueOut == 1);
}


void TestXmlAttributes::testSigned32BitConversions() {
    Ld::XmlAttributes attributes;

    // Test against the maximum value

    typedef std::int32_t TestType;
    TestType minimumValue = std::numeric_limits<TestType>::min();
    TestType maximumValue = std::numeric_limits<TestType>::max();

    TestType valueIn = maximumValue;
    attributes.append("v1", valueIn);

    bool success;
    TestType valueOut = attributes.value<TestType>("v1", &success);
    QVERIFY(success);
    QVERIFY(valueOut == valueIn);

    // Test against the minimum value

    valueIn = minimumValue;
    attributes.append("v2", valueIn);

    valueOut = attributes.value<TestType>("v2", &success);
    QVERIFY(success);
    QVERIFY(valueOut == valueIn);

    // Test against an out-of-bounds value

    attributes.append("v3", INT64_MIN);
    valueOut = attributes.value<TestType>("v3", &success);
    QVERIFY(!success);

    // Test against a value that is invalid (e.g. non-number)

    attributes.append("v4", "This should fail badly.");
    valueOut = attributes.value<TestType>("v4", &success);
    QVERIFY(!success);

    // Test version that takes a default to confirm success.

    valueOut = attributes.value<TestType>("v2", static_cast<TestType>(1), &success);
    QVERIFY(success);
    QVERIFY(valueOut == valueIn);

    valueOut = attributes.value<TestType>("v5", static_cast<TestType>(1), &success);
    QVERIFY(success);
    QVERIFY(valueOut == 1);
}


void TestXmlAttributes::testSigned64BitConversions() {
    Ld::XmlAttributes attributes;

    // Test against the maximum value

    typedef std::int64_t TestType;
    TestType minimumValue = std::numeric_limits<TestType>::min();
    TestType maximumValue = std::numeric_limits<TestType>::max();

    TestType valueIn = maximumValue;
    attributes.append("v1", valueIn);

    bool success;
    TestType valueOut = attributes.value<TestType>("v1", &success);
    QVERIFY(success);
    QVERIFY(valueOut == valueIn);

    // Test against the minimum value

    valueIn = minimumValue;
    attributes.append("v2", valueIn);

    valueOut = attributes.value<TestType>("v2", &success);
    QVERIFY(success);
    QVERIFY(valueOut == valueIn);

    // Test against an out-of-bounds value

    attributes.append("v3", UINT64_MAX);
    valueOut = attributes.value<TestType>("v3", &success);
    QVERIFY(!success);

    // Test against a value that is invalid (e.g. non-number)

    attributes.append("v4", "This should fail badly.");
    valueOut = attributes.value<TestType>("v4", &success);
    QVERIFY(!success);

    // Test version that takes a default to confirm success.

    valueOut = attributes.value<TestType>("v2", static_cast<TestType>(1), &success);
    QVERIFY(success);
    QVERIFY(valueOut == valueIn);

    valueOut = attributes.value<TestType>("v5", static_cast<TestType>(1), &success);
    QVERIFY(success);
    QVERIFY(valueOut == 1);
}


void TestXmlAttributes::testUnsigned8BitConversions() {
    Ld::XmlAttributes attributes;

    // Test against the maximum value

    typedef std::uint8_t TestType;
    TestType maximumValue = std::numeric_limits<TestType>::max();

    TestType valueIn = maximumValue;
    attributes.append("v1", valueIn);

    bool success;
    TestType valueOut = attributes.value<TestType>("v1", &success);
    QVERIFY(success);
    QVERIFY(valueOut == valueIn);

    // Test against 0 value

    valueIn = 0;
    attributes.append("v2", valueIn);

    valueOut = attributes.value<TestType>("v2", &success);
    QVERIFY(success);
    QVERIFY(valueOut == valueIn);

    // Test against an out-of-bounds value

    attributes.append("v3", INT64_MIN);
    valueOut = attributes.value<TestType>("v3", &success);
    QVERIFY(!success);

    // Test against a value that is invalid (e.g. non-number)

    attributes.append("v4", "This should fail badly.");
    valueOut = attributes.value<TestType>("v4", &success);
    QVERIFY(!success);

    // Test version that takes a default to confirm success.

    valueOut = attributes.value<TestType>("v2", static_cast<TestType>(1), &success);
    QVERIFY(success);
    QVERIFY(valueOut == valueIn);

    valueOut = attributes.value<TestType>("v5", static_cast<TestType>(1), &success);
    QVERIFY(success);
    QVERIFY(valueOut == 1);
}


void TestXmlAttributes::testUnsigned16BitConversions() {
    Ld::XmlAttributes attributes;

    // Test against the maximum value

    typedef std::uint16_t TestType;
    TestType maximumValue = std::numeric_limits<TestType>::max();

    TestType valueIn = maximumValue;
    attributes.append("v1", valueIn);

    bool success;
    TestType valueOut = attributes.value<TestType>("v1", &success);
    QVERIFY(success);
    QVERIFY(valueOut == valueIn);

    // Test against 0 value

    valueIn = 0;
    attributes.append("v2", valueIn);

    valueOut = attributes.value<TestType>("v2", &success);
    QVERIFY(success);
    QVERIFY(valueOut == valueIn);

    // Test against an out-of-bounds value

    attributes.append("v3", INT64_MIN);
    valueOut = attributes.value<TestType>("v3", &success);
    QVERIFY(!success);

    // Test against a value that is invalid (e.g. non-number)

    attributes.append("v4", "This should fail badly.");
    valueOut = attributes.value<TestType>("v4", &success);
    QVERIFY(!success);

    // Test version that takes a default to confirm success.

    valueOut = attributes.value<TestType>("v2", static_cast<TestType>(1), &success);
    QVERIFY(success);
    QVERIFY(valueOut == valueIn);

    valueOut = attributes.value<TestType>("v5", static_cast<TestType>(1), &success);
    QVERIFY(success);
    QVERIFY(valueOut == 1);
}


void TestXmlAttributes::testUnsigned32BitConversions() {
    Ld::XmlAttributes attributes;

    // Test against the maximum value

    typedef std::uint32_t TestType;
    TestType maximumValue = std::numeric_limits<TestType>::max();

    TestType valueIn = maximumValue;
    attributes.append("v1", valueIn);

    bool success;
    TestType valueOut = attributes.value<TestType>("v1", &success);
    QVERIFY(success);
    QVERIFY(valueOut == valueIn);

    // Test against 0 value

    valueIn = 0;
    attributes.append("v2", valueIn);

    valueOut = attributes.value<TestType>("v2", &success);
    QVERIFY(success);
    QVERIFY(valueOut == valueIn);

    // Test against an out-of-bounds value

    attributes.append("v3", INT64_MIN);
    valueOut = attributes.value<TestType>("v3", &success);
    QVERIFY(!success);

    // Test against a value that is invalid (e.g. non-number)

    attributes.append("v4", "This should fail badly.");
    valueOut = attributes.value<TestType>("v4", &success);
    QVERIFY(!success);

    // Test version that takes a default to confirm success.

    valueOut = attributes.value<TestType>("v2", static_cast<TestType>(1), &success);
    QVERIFY(success);
    QVERIFY(valueOut == valueIn);

    valueOut = attributes.value<TestType>("v5", static_cast<TestType>(1), &success);
    QVERIFY(success);
    QVERIFY(valueOut == 1);
}


void TestXmlAttributes::testUnsigned64BitConversions() {
    Ld::XmlAttributes attributes;

    // Test against the maximum value

    typedef std::uint64_t TestType;
    TestType maximumValue = std::numeric_limits<TestType>::max();

    TestType valueIn = maximumValue;
    attributes.append("v1", valueIn);

    bool success;
    TestType valueOut = attributes.value<TestType>("v1", &success);
    QVERIFY(success);
    QVERIFY(valueOut == valueIn);

    // Test against 0 value

    valueIn = 0;
    attributes.append("v2", valueIn);

    valueOut = attributes.value<TestType>("v2", &success);
    QVERIFY(success);
    QVERIFY(valueOut == valueIn);

    // Test against an out-of-bounds value

    attributes.append("v3", INT64_MIN);
    valueOut = attributes.value<TestType>("v3", &success);
    QVERIFY(!success);

    // Test against a value that is invalid (e.g. non-number)

    attributes.append("v4", "This should fail badly.");
    valueOut = attributes.value<TestType>("v4", &success);
    QVERIFY(!success);

    // Test version that takes a default to confirm success.

    valueOut = attributes.value<TestType>("v2", static_cast<TestType>(1), &success);
    QVERIFY(success);
    QVERIFY(valueOut == valueIn);

    valueOut = attributes.value<TestType>("v5", static_cast<TestType>(1), &success);
    QVERIFY(success);
    QVERIFY(valueOut == 1);
}


void TestXmlAttributes::testFloatConversions() {
    Ld::XmlAttributes attributes;

    // Test against the maximum value

    typedef float TestType;
    TestType minimumValue = std::numeric_limits<TestType>::min();
    TestType maximumValue = std::numeric_limits<TestType>::max();

    TestType valueIn = maximumValue;
    attributes.append("v1", valueIn);

    bool success;
    TestType valueOut = attributes.value<TestType>("v1", &success);
    QVERIFY(success);
    QVERIFY(std::fabs((valueOut - valueIn) / valueIn) < 1.0E-6); // String conversions will introduce tiny errors.

    // Test against the minimum value

    valueIn = minimumValue;
    attributes.append("v2", valueIn);

    valueOut = attributes.value<TestType>("v2", &success);
    QVERIFY(success);
    QVERIFY(std::fabs((valueOut - valueIn) / valueIn) < 1.0E-6);

    // Test against an out-of-bounds value

    attributes.append("v3", std::numeric_limits<double>::max());
    valueOut = attributes.value<TestType>("v3", &success);
    QVERIFY(!success);

    // Test against a value that is invalid (e.g. non-number)

    attributes.append("v4", "This should fail badly.");
    valueOut = attributes.value<TestType>("v4", &success);
    QVERIFY(!success);

    // Test version that takes a default to confirm success.

    valueOut = attributes.value<TestType>("v2", static_cast<TestType>(1), &success);
    QVERIFY(success);
    QVERIFY(std::fabs((valueOut - valueIn) / valueIn) < 1.0E-6);

    valueOut = attributes.value<TestType>("v5", static_cast<TestType>(1), &success);
    QVERIFY(success);
    QVERIFY(valueOut == 1);
}


void TestXmlAttributes::testDoubleConversions() {
    Ld::XmlAttributes attributes;

    // Test against the maximum value

    typedef double TestType;
    TestType minimumValue = std::numeric_limits<TestType>::min();
    TestType maximumValue = std::numeric_limits<TestType>::max();

    TestType valueIn = maximumValue;
    attributes.append("v1", valueIn);

    bool success;
    TestType valueOut = attributes.value<TestType>("v1", &success);
    QVERIFY(success);
    QVERIFY(std::fabs((valueOut - valueIn) / valueIn) < 1.0E-13); // String conversions will introduce tiny errors.

    // Test against the minimum value

    valueIn = minimumValue;
    attributes.append("v2", valueIn);

    valueOut = attributes.value<TestType>("v2", &success);
    QVERIFY(success);
    QVERIFY(std::fabs((valueOut - valueIn) / valueIn) < 1.0E-13); // String conversions will introduce tiny errors.

    // Test against a value that is invalid (e.g. non-number)

    attributes.append("v4", "This should fail badly.");
    valueOut = attributes.value<TestType>("v4", &success);
    QVERIFY(!success);

    // Test version that takes a default to confirm success.

    valueOut = attributes.value<TestType>("v2", static_cast<TestType>(1), &success);
    QVERIFY(success);
    QVERIFY(std::fabs((valueOut - valueIn) / valueIn) < 1.0E-13); // String conversions will introduce tiny errors.

    valueOut = attributes.value<TestType>("v5", static_cast<TestType>(1), &success);
    QVERIFY(success);
    QVERIFY(valueOut == 1);
}


void TestXmlAttributes::testStringConversions() {
    Ld::XmlAttributes attributes;

    QString valueIn("Test Value");
    attributes.append("v1", valueIn);

    QString qStringValueOut = attributes.value<QString>("v1");
    QVERIFY(qStringValueOut == valueIn);

    std::string stringValueOut = attributes.value<std::string>("v1");
    QVERIFY(QString::fromStdString(stringValueOut) == valueIn);

    const char* charValueOut = attributes.value<const char*>("v1");
    QVERIFY(QString(charValueOut) == valueIn);

    attributes.append("v3", "Yet Another Test Value");
    QVERIFY(attributes.value<QString>("v3") == QString("Yet Another Test Value"));
}


void TestXmlAttributes::testHandleConversions() {
    Ld::XmlAttributes attributes;

    Ld::Handle::initialize(0x3456789ABULL);
    Ld::Handle handleIn = Ld::Handle::create();

    attributes.append("v1", handleIn);

    Ld::Handle handleOut = Ld::Handle::create();

    QVERIFY(handleIn != handleOut);

    bool success;
    handleOut = attributes.value<Ld::Handle>("v1", &success);
    QVERIFY(success);
    QVERIFY(handleOut == handleIn);

    attributes.append("v2", "An invalid handle value.");
    handleOut = attributes.value<Ld::Handle>("v2", &success);
    QVERIFY(!success);
}
