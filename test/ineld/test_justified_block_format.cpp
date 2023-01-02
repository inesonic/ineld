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
* This file implements tests of the Ld::JustifiedBlockFormat class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QPageLayout>
#include <QMarginsF>
#include <QList>
#include <QtTest/QtTest>

#include <ld_justified_block_format.h>

#include "test_justified_block_format.h"

TestJustifiedBlockFormat::TestJustifiedBlockFormat() {}


TestJustifiedBlockFormat::~TestJustifiedBlockFormat() {}


void TestJustifiedBlockFormat::initTestCase() {}


void TestJustifiedBlockFormat::testConstructorsAndDestructors() {
    Ld::JustifiedBlockFormat format1;
    QVERIFY(format1.isValid());

    format1.setLeftIndentation(72.0);
    format1.setRightIndentation(73.0);
    format1.setTopSpacing(6.0);
    format1.setBottomSpacing(12.0);
    format1.setJustification(Ld::JustifiedBlockFormat::Justification::LEFT);

    Ld::JustifiedBlockFormat format2(format1);
    QVERIFY(format2.isValid());
    QVERIFY(format2.leftIndentation() == 72.0);
    QVERIFY(format2.rightIndentation() == 73.0);
    QVERIFY(format2.topSpacing() == 6.0);
    QVERIFY(format2.bottomSpacing() == 12.0);
    QVERIFY(format2.justificationMode() == Ld::JustifiedBlockFormat::Justification::LEFT);

    QSharedPointer<Ld::JustifiedBlockFormat> format3 = format2.clone().dynamicCast<Ld::JustifiedBlockFormat>();

    QVERIFY(format3->isValid());
    QVERIFY(format3->leftIndentation() == 72.0);
    QVERIFY(format3->rightIndentation() == 73.0);
    QVERIFY(format3->topSpacing() == 6.0);
    QVERIFY(format3->bottomSpacing() == 12.0);
    QVERIFY(format3->justificationMode() == Ld::JustifiedBlockFormat::Justification::LEFT);
}


void TestJustifiedBlockFormat::testValidInvalidMethods() {
    Ld::JustifiedBlockFormat format;
    QVERIFY(format.isValid());
    QVERIFY(!format.isInvalid());

    format.setLeftIndentation(72.0);
    format.setRightIndentation(73.0);
    format.setTopSpacing(6.0);
    format.setBottomSpacing(12.0);

    QVERIFY(format.isValid());
    QVERIFY(!format.isInvalid());

    format.setJustification(Ld::JustifiedBlockFormat::Justification::LEFT);

    QVERIFY(format.isValid());
    QVERIFY(!format.isInvalid());
}


void TestJustifiedBlockFormat::testAccessors() {
    Ld::JustifiedBlockFormat format;

    format.setLeftIndentation(72.0);
    format.setRightIndentation(73.0);
    format.setTopSpacing(6.0);
    format.setBottomSpacing(12.0);

    QVERIFY(format.leftIndentation() == 72.0);
    QVERIFY(format.rightIndentation() == 73.0);
    QVERIFY(format.topSpacing() == 6.0);
    QVERIFY(format.bottomSpacing() == 12.0);

    bool success = format.setJustification(Ld::JustifiedBlockFormat::Justification::LEFT);
    QVERIFY(success);
    QVERIFY(format.justificationMode() == Ld::JustifiedBlockFormat::Justification::LEFT);

    success = format.setJustification(Ld::JustifiedBlockFormat::Justification::RIGHT);
    QVERIFY(success);
    QVERIFY(format.justificationMode() == Ld::JustifiedBlockFormat::Justification::RIGHT);

    success = format.setJustification(Ld::JustifiedBlockFormat::Justification::CENTER);
    QVERIFY(success);
    QVERIFY(format.justificationMode() == Ld::JustifiedBlockFormat::Justification::CENTER);

    success = format.setJustification(Ld::JustifiedBlockFormat::Justification::JUSTIFY);
    QVERIFY(!success); // JUSTIFIED is disabled by default.
    QVERIFY(format.justificationMode() == Ld::JustifiedBlockFormat::Justification::CENTER);
}


void TestJustifiedBlockFormat::testToStringMethod() {
    Ld::JustifiedBlockFormat format;

    format.setLeftIndentation(72.0);
    format.setRightIndentation(73.0);
    format.setTopSpacing(6.0);
    format.setBottomSpacing(12.0);
    format.setJustification(Ld::JustifiedBlockFormat::Justification::RIGHT);

    QString description = format.toString();
    QVERIFY(description == QString("JustifiedBlockFormat,72,73,6,12,RIGHT"));
}


void TestJustifiedBlockFormat::testFormatAggregator() {
    Ld::JustifiedBlockFormat::Aggregation aggregation;

    QSharedPointer<Ld::JustifiedBlockFormat> format1(new Ld::JustifiedBlockFormat);
    format1->setJustification(Ld::JustifiedBlockFormat::Justification::LEFT);

    QSharedPointer<Ld::JustifiedBlockFormat> format2(new Ld::JustifiedBlockFormat);
    format2->setJustification(Ld::JustifiedBlockFormat::Justification::RIGHT);

    bool success = aggregation.addFormat(format1);
    QVERIFY(success);

    success = aggregation.addFormat(format2);
    QVERIFY(success);

    QCOMPARE(aggregation.justifications().size(), 2);
    QVERIFY(aggregation.justifications().contains(Ld::JustifiedBlockFormat::Justification::LEFT));
    QVERIFY(aggregation.justifications().contains(Ld::JustifiedBlockFormat::Justification::RIGHT));
}
