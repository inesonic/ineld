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
* This file implements tests for the \ref Ld::AggregationsByCapability class.  Also provides some testing of the
* \ref Ld::Format class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QSharedPointer>
#include <QtTest/QtTest>

#include <cassert>

#include <ld_element_structures.h>
#include <ld_element.h>
#include <ld_paragraph_element.h>
#include <ld_format.h>
#include <ld_format.h>
#include <ld_block_format.h>
#include <ld_justified_block_format.h>
#include <ld_text_block_format.h>
#include <ld_aggregations_by_capability.h>

#include "test_aggregations_by_capability.h"


TestAggregationsByCapability::TestAggregationsByCapability() {}


TestAggregationsByCapability::~TestAggregationsByCapability() {}


void TestAggregationsByCapability::initTestCase() {
    Ld::Handle::initialize(0x12345678ABCDEF0);

    Ld::Format::registerCreator(Ld::BlockFormat::formatName, Ld::BlockFormat::creator);
    Ld::Format::registerCreator(Ld::JustifiedBlockFormat::formatName, Ld::JustifiedBlockFormat::creator);
    Ld::Format::registerCreator(Ld::TextBlockFormat::formatName, Ld::TextBlockFormat::creator);

    Ld::Format::Aggregation::registerCreator(Ld::BlockFormat::formatName, Ld::BlockFormat::Aggregation::creator);

    Ld::Format::Aggregation::registerCreator(
        Ld::JustifiedBlockFormat::formatName,
        Ld::JustifiedBlockFormat::Aggregation::creator
    );

    Ld::Format::Aggregation::registerCreator(
        Ld::TextBlockFormat::formatName,
        Ld::TextBlockFormat::Aggregation::creator
    );
}


void TestAggregationsByCapability::testConstructorsAndDestructors() {
    Ld::AggregationsByCapability aggregationsByCapability1;
    QVERIFY(aggregationsByCapability1.capabilities().isEmpty());

    aggregationsByCapability1.addFormat(Ld::Format::create(Ld::BlockFormat::formatName));
    QCOMPARE(aggregationsByCapability1.capabilities().count(), 1);
    QVERIFY(aggregationsByCapability1.contains(Ld::BlockFormat::formatName));

    Ld::AggregationsByCapability aggregationsByCapability2(aggregationsByCapability1);
    QCOMPARE(aggregationsByCapability2.capabilities().count(), 1);
    QVERIFY(aggregationsByCapability2.contains(Ld::BlockFormat::formatName));
}


void TestAggregationsByCapability::testCapabilitiesMethod() {
    Ld::AggregationsByCapability aggregationsByCapability;
    QVERIFY(aggregationsByCapability.capabilities().isEmpty());

    aggregationsByCapability.addFormat(Ld::Format::create(Ld::BlockFormat::formatName));
    QCOMPARE(aggregationsByCapability.capabilities().count(), 1);
    QVERIFY(aggregationsByCapability.contains(Ld::BlockFormat::formatName));

    aggregationsByCapability.addFormat(Ld::Format::create(Ld::TextBlockFormat::formatName));
    QCOMPARE(aggregationsByCapability.capabilities().count(), 3);
    QVERIFY(aggregationsByCapability.contains(Ld::BlockFormat::formatName));
    QVERIFY(aggregationsByCapability.contains(Ld::JustifiedBlockFormat::formatName));
    QVERIFY(aggregationsByCapability.contains(Ld::TextBlockFormat::formatName));
}


void TestAggregationsByCapability::testContainsMethod() {
    Ld::AggregationsByCapability aggregationsByCapability;

    QCOMPARE(aggregationsByCapability.contains(Ld::BlockFormat::formatName), false);
    QCOMPARE(aggregationsByCapability.contains(Ld::JustifiedBlockFormat::formatName), false);
    QCOMPARE(aggregationsByCapability.contains(Ld::TextBlockFormat::formatName), false);
    QCOMPARE(aggregationsByCapability.contains(QString("Format")), false);

    aggregationsByCapability.addFormat(Ld::Format::create(Ld::TextBlockFormat::formatName));

    QCOMPARE(aggregationsByCapability.contains(Ld::BlockFormat::formatName), true);
    QCOMPARE(aggregationsByCapability.contains(Ld::JustifiedBlockFormat::formatName), true);
    QCOMPARE(aggregationsByCapability.contains(Ld::TextBlockFormat::formatName), true);
    QCOMPARE(aggregationsByCapability.contains(QString("Format")), false);
}


void TestAggregationsByCapability::testAggregationMethods() {
    Ld::AggregationsByCapability aggregationsByCapability;
    aggregationsByCapability.addFormat(Ld::Format::create(Ld::TextBlockFormat::formatName));

    const Ld::BlockFormat::Aggregation aggregation1 =
        aggregationsByCapability.aggregation<Ld::BlockFormat::Aggregation>(
            Ld::BlockFormat::formatName
        );
    QCOMPARE(aggregation1.typeName(), Ld::BlockFormat::formatName);

    const Ld::JustifiedBlockFormat::Aggregation aggregation2 =
        aggregationsByCapability.aggregation<Ld::JustifiedBlockFormat::Aggregation>(
            Ld::JustifiedBlockFormat::formatName
        );
    QCOMPARE(aggregation2.typeName(), Ld::JustifiedBlockFormat::formatName);
}


void TestAggregationsByCapability::testAddFormatMethods() {
    // The Ld::AggregationsByCapability::addFormat(Ld::FormatPointer) method is already tested above.

    Ld::ElementPointer testElement(new Ld::ParagraphElement);
    testElement->setWeakThis(testElement.toWeakRef());

    testElement->setFormat(Ld::Format::create(Ld::TextBlockFormat::formatName));

    Ld::AggregationsByCapability aggregationsByCapability;
    aggregationsByCapability.addFormat(testElement);

    QCOMPARE(aggregationsByCapability.contains(Ld::BlockFormat::formatName), true);
    QCOMPARE(aggregationsByCapability.contains(Ld::JustifiedBlockFormat::formatName), true);
    QCOMPARE(aggregationsByCapability.contains(Ld::TextBlockFormat::formatName), true);
    QCOMPARE(aggregationsByCapability.contains(QString("Format")), false);

    const Ld::BlockFormat::Aggregation& aggregation =
        aggregationsByCapability.aggregation<Ld::BlockFormat::Aggregation>(Ld::BlockFormat::formatName);

    const QSet<Ld::ElementWeakPointer>& elements = aggregation;

    QCOMPARE(elements.count(), 1);
    QVERIFY(elements.contains(testElement.toWeakRef()));
}


void TestAggregationsByCapability::testAssignmentOperator() {
    Ld::AggregationsByCapability aggregationsByCapability1;
    QVERIFY(aggregationsByCapability1.capabilities().isEmpty());

    aggregationsByCapability1.addFormat(Ld::Format::create(Ld::BlockFormat::formatName));
    QCOMPARE(aggregationsByCapability1.capabilities().count(), 1);
    QVERIFY(aggregationsByCapability1.contains(Ld::BlockFormat::formatName));

    Ld::AggregationsByCapability aggregationsByCapability2;
    QVERIFY(aggregationsByCapability2.capabilities().isEmpty());

    aggregationsByCapability2 = aggregationsByCapability1;
    QCOMPARE(aggregationsByCapability2.capabilities().count(), 1);
    QVERIFY(aggregationsByCapability2.contains(Ld::BlockFormat::formatName));

    aggregationsByCapability1.addFormat(Ld::Format::create(Ld::TextBlockFormat::formatName));
    QCOMPARE(aggregationsByCapability1.capabilities().count(), 3);
    QVERIFY(aggregationsByCapability1.contains(Ld::BlockFormat::formatName));
    QVERIFY(aggregationsByCapability1.contains(Ld::JustifiedBlockFormat::formatName));
    QVERIFY(aggregationsByCapability1.contains(Ld::TextBlockFormat::formatName));

    QCOMPARE(aggregationsByCapability2.capabilities().count(), 1);
    QVERIFY(aggregationsByCapability2.contains(Ld::BlockFormat::formatName));
}
