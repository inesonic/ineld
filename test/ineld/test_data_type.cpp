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
* This file implements tests of the Ld::DataType class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QList>
#include <QtTest/QtTest>

#include <model_api_types.h>

#include <ld_element_structures.h>
#include <ld_element.h>
#include <ld_data_type.h>

#include "test_element_position.h"
#include "test_data_type.h"

TestDataType::TestDataType() {}

TestDataType::~TestDataType() {}

void TestDataType::initTestCase() {
    Ld::Handle::initialize(0x123456789ABCDEF0ULL);
    Ld::Element::registerCreator("PositionTest", PositionTestElement::creator);
}

void TestDataType::testRegistrationFunction() {
    Ld::DataType::registerType(Model::ValueType::REAL,    "Double precision", "DoublePrecisionElement");
    Ld::DataType::registerType(Model::ValueType::INTEGER, "Integer",          "PositionTest");
    Ld::DataType::registerType(Model::ValueType::BOOLEAN, "Boolean",          "BooleanElement");

    QList<Ld::DataType> dataTypes = Ld::DataType::allTypes();

    QCOMPARE(dataTypes.size(), 3);
    QCOMPARE(dataTypes[0].valueType(), Model::ValueType::REAL);
    QCOMPARE(dataTypes[1].valueType(), Model::ValueType::INTEGER);
    QCOMPARE(dataTypes[2].valueType(), Model::ValueType::BOOLEAN);

    QCOMPARE(dataTypes[1].description(), QString("Integer"));
    QCOMPARE(dataTypes[1].typeSymbolElementName(), QString("PositionTest"));
}


void TestDataType::testConstructorsAndAssignments() {
    Ld::DataType dataType1;

    QCOMPARE(dataType1.isValid(),   false);
    QCOMPARE(dataType1.isInvalid(), true);

    QList<Ld::DataType> dataTypes = Ld::DataType::allTypes();
    QCOMPARE(dataTypes.size(), 3);

    Ld::DataType dataType2(dataTypes[1]);

    QCOMPARE(dataType2.isValid(),   true);
    QCOMPARE(dataType2.isInvalid(), false);

    QCOMPARE(dataType2.valueType(), Model::ValueType::INTEGER);
    QCOMPARE(dataType2.description(), QString("Integer"));
    QCOMPARE(dataType2.typeSymbolElementName(), QString("PositionTest"));

    dataType1 = dataType2;

    QCOMPARE(dataType1.isValid(),   true);
    QCOMPARE(dataType1.isInvalid(), false);

    QCOMPARE(dataType1.valueType(), Model::ValueType::INTEGER);
    QCOMPARE(dataType1.description(), QString("Integer"));
    QCOMPARE(dataType1.typeSymbolElementName(), QString("PositionTest"));
}


void TestDataType::testStaticMethods() {
    QList<Ld::DataType> dataTypes = Ld::DataType::allTypes();
    QCOMPARE(dataTypes.size(), 3);
    QCOMPARE(dataTypes[0].valueType(), Model::ValueType::REAL);
    QCOMPARE(dataTypes[1].valueType(), Model::ValueType::INTEGER);
    QCOMPARE(dataTypes[2].valueType(), Model::ValueType::BOOLEAN);

    Ld::DataType dataType = Ld::DataType::fromValueType(Model::ValueType::REAL);

    QCOMPARE(dataType.isValid(), true);

    QCOMPARE(dataType.valueType(), Model::ValueType::REAL);
    QCOMPARE(dataType.description(), QString("Double precision"));
    QCOMPARE(dataType.typeSymbolElementName(), QString("DoublePrecisionElement"));

    dataType = Ld::DataType::fromSymbol("BooleanElement");

    QCOMPARE(dataType.isValid(), true);

    QCOMPARE(dataType.valueType(), Model::ValueType::BOOLEAN);
    QCOMPARE(dataType.description(), QString("Boolean"));
    QCOMPARE(dataType.typeSymbolElementName(), QString("BooleanElement"));

    Ld::ElementPointer positionTestElement = Ld::Element::create("PositionTest");

    dataType = Ld::DataType::fromSymbol(positionTestElement);

    QCOMPARE(dataType.isValid(), true);

    QCOMPARE(dataType.valueType(), Model::ValueType::INTEGER);
    QCOMPARE(dataType.description(), QString("Integer"));
    QCOMPARE(dataType.typeSymbolElementName(), QString("PositionTest"));

    Ld::ElementPointer element = dataType.typeSymbolElement();
    QCOMPARE(element->typeName(), QString("PositionTest"));
}


void TestDataType::testAccessors() {
    // Tested by other methods in this module.
}


void TestDataType::testEqualityChecks() {
    Ld::DataType dataType1;
    Ld::DataType dataType2;

    QList<Ld::DataType> dataTypes = Ld::DataType::allTypes();

    QCOMPARE(dataType1 == dataType2, true);
    QCOMPARE(dataType1 != dataType2, false);

    QCOMPARE(dataType1 == dataTypes[0], false);
    QCOMPARE(dataType1 != dataTypes[0], true);

    dataType1 = dataTypes[0];

    QCOMPARE(dataType1 == dataTypes[0], true);
    QCOMPARE(dataType1 != dataTypes[0], false);

    QCOMPARE(dataType1 == dataTypes[1], false);
    QCOMPARE(dataType1 != dataTypes[1], true);
}
