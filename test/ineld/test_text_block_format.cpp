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
* This file implements tests of the Ld::TextBlockFormat class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QtTest/QtTest>

#include <ld_text_block_format.h>

#include "test_text_block_format.h"

TestTextBlockFormat::TestTextBlockFormat() {}


TestTextBlockFormat::~TestTextBlockFormat() {}


void TestTextBlockFormat::initTestCase() {}


void TestTextBlockFormat::testConstructorsAndDestructors() {
    Ld::TextBlockFormat format1;
    QVERIFY(format1.isValid());

    format1.setLeftIndentation(72.0);
    format1.setRightIndentation(73.0);
    format1.setTopSpacing(6.0);
    format1.setBottomSpacing(12.0);
    format1.setJustification(Ld::TextBlockFormat::Justification::LEFT);
    format1.clearFirstLineLeftIndentation();
    format1.setLineSpacing(2.5);

    Ld::TextBlockFormat format2(format1);
    QVERIFY(format2.isValid());
    QVERIFY(format2.leftIndentation() == 72.0);
    QVERIFY(format2.rightIndentation() == 73.0);
    QVERIFY(format2.topSpacing() == 6.0);
    QVERIFY(format2.bottomSpacing() == 12.0);
    QVERIFY(format2.justificationMode() == Ld::TextBlockFormat::Justification::LEFT);
    QVERIFY(format2.firstLineLeftIndentation() == 72.0);
    QVERIFY(format2.lineSpacing() == 2.5);

    QSharedPointer<Ld::TextBlockFormat> format3 = format2.clone().dynamicCast<Ld::TextBlockFormat>();

    QVERIFY(format3->isValid());
    QVERIFY(format3->leftIndentation() == 72.0);
    QVERIFY(format3->rightIndentation() == 73.0);
    QVERIFY(format3->topSpacing() == 6.0);
    QVERIFY(format3->bottomSpacing() == 12.0);
    QVERIFY(format3->justificationMode() == Ld::TextBlockFormat::Justification::LEFT);
    QVERIFY(format3->firstLineLeftIndentation() == 72.0);
    QVERIFY(format3->lineSpacing() == 2.5);
}


void TestTextBlockFormat::testValidInvalidMethods() {
    Ld::TextBlockFormat format;
    QVERIFY(format.isValid());
    QVERIFY(!format.isInvalid());

    format.setLeftIndentation(72.0);
    format.setRightIndentation(73.0);
    format.setTopSpacing(6.0);
    format.setBottomSpacing(12.0);
    format.setJustification(Ld::TextBlockFormat::Justification::LEFT);

    QVERIFY(!format.isInvalid());
    QVERIFY(format.isValid());

    format.setFirstLineLeftIndentation(144.0);

    QVERIFY(!format.isInvalid());
    QVERIFY(format.isValid());

    format.clearFirstLineLeftIndentation();

    QVERIFY(!format.isInvalid());
    QVERIFY(format.isValid());
}


void TestTextBlockFormat::testAccessors() {
    Ld::TextBlockFormat format;

    format.setLeftIndentation(72.0);
    format.setRightIndentation(73.0);
    format.setTopSpacing(6.0);
    format.setBottomSpacing(12.0);

    QVERIFY(format.leftIndentation() == 72.0);
    QVERIFY(format.rightIndentation() == 73.0);
    QVERIFY(format.topSpacing() == 6.0);
    QVERIFY(format.bottomSpacing() == 12.0);

    bool success = format.setJustification(Ld::TextBlockFormat::Justification::LEFT);
    QVERIFY(success);
    QVERIFY(format.justificationMode() == Ld::TextBlockFormat::Justification::LEFT);

    format.setFirstLineLeftIndentation(144.0);
    QVERIFY(format.firstLineLeftIndentation() == 144.0);

    format.clearFirstLineLeftIndentation();
    QVERIFY(format.firstLineLeftIndentation() == 72.0);

    format.setLineSpacing(2.5);
    QVERIFY(format.lineSpacing() == 2.5);
}


void TestTextBlockFormat::testToStringMethod() {
    Ld::TextBlockFormat format;

    format.setLeftIndentation(72.0);
    format.setRightIndentation(73.0);
    format.setTopSpacing(6.0);
    format.setBottomSpacing(12.0);
    format.setJustification(Ld::TextBlockFormat::Justification::RIGHT);
    format.setFirstLineLeftIndentation(144.0);
    format.setLineSpacing(2.5);

    QString description = format.toString();
    QVERIFY(description == QString("TextBlockFormat,72,73,6,12,RIGHT,144,2.5"));
}


void TestTextBlockFormat::testFormatAggregator() {
    Ld::TextBlockFormat::Aggregation aggregation;

    QSharedPointer<Ld::TextBlockFormat> format1(new Ld::TextBlockFormat);
    format1->setFirstLineLeftIndentation(10);
    format1->setLineSpacing(1.5);

    QSharedPointer<Ld::TextBlockFormat> format2(new Ld::TextBlockFormat);
    format2->setFirstLineLeftIndentation(20);
    format2->setLineSpacing(2.0);

    bool success = aggregation.addFormat(format1);
    QVERIFY(success);

    success = aggregation.addFormat(format2);
    QVERIFY(success);

    QCOMPARE(aggregation.firstLineLeftIndentations().size(), 2);
    QVERIFY(aggregation.firstLineLeftIndentations().contains(10));
    QVERIFY(aggregation.firstLineLeftIndentations().contains(20));

    QCOMPARE(aggregation.lineSpacings().size(), 2);
    QVERIFY(aggregation.lineSpacings().contains(1.5));
    QVERIFY(aggregation.lineSpacings().contains(2.0));
}
