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
* This file implements tests of the Ld::BlockFormat class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QPageLayout>
#include <QMarginsF>
#include <QList>
#include <QtTest/QtTest>

#include <ld_format_structures.h>
#include <ld_block_format.h>

#include "test_block_format.h"

TestBlockFormat::TestBlockFormat() {}


TestBlockFormat::~TestBlockFormat() {}


void TestBlockFormat::initTestCase() {}


void TestBlockFormat::testConstructorsAndDestructors() {
    Ld::BlockFormat format1;
    QVERIFY(format1.isValid());

    format1.setLeftIndentation(72.0);
    format1.setRightIndentation(73.0);
    format1.setTopSpacing(6.0);
    format1.setBottomSpacing(12.0);

    Ld::BlockFormat format2(format1);
    QVERIFY(format2.isValid());
    QVERIFY(format2.leftIndentation() == 72.0);
    QVERIFY(format2.rightIndentation() == 73.0);
    QVERIFY(format2.topSpacing() == 6.0);
    QVERIFY(format2.bottomSpacing() == 12.0);

    QSharedPointer<Ld::BlockFormat> format3 = format2.clone().dynamicCast<Ld::BlockFormat>();

    QVERIFY(format3->isValid());
    QVERIFY(format3->leftIndentation() == 72.0);
    QVERIFY(format3->rightIndentation() == 73.0);
    QVERIFY(format3->topSpacing() == 6.0);
    QVERIFY(format3->bottomSpacing() == 12.0);
}


void TestBlockFormat::testValidInvalidMethods() {
    Ld::BlockFormat format;
    QVERIFY(format.isValid());
    QVERIFY(!format.isInvalid());

    format.setLeftIndentation(72.0);
    format.setRightIndentation(73.0);
    format.setTopSpacing(6.0);
    format.setBottomSpacing(12.0);

    QVERIFY(format.isValid());
    QVERIFY(!format.isInvalid());
}


void TestBlockFormat::testAccessors() {
    Ld::BlockFormat format;

    format.setLeftIndentation(72.0);
    format.setRightIndentation(73.0);
    format.setTopSpacing(6.0);
    format.setBottomSpacing(12.0);

    QVERIFY(format.leftIndentation() == 72.0);
    QVERIFY(format.rightIndentation() == 73.0);
    QVERIFY(format.topSpacing() == 6.0);
    QVERIFY(format.bottomSpacing() == 12.0);
}


void TestBlockFormat::testToStringMethod() {
    Ld::BlockFormat format;

    format.setLeftIndentation(72.0);
    format.setRightIndentation(73.0);
    format.setTopSpacing(6.0);
    format.setBottomSpacing(12.0);

    QString description = format.toString();
    QVERIFY(description == QString("BlockFormat,72,73,6,12"));
}


void TestBlockFormat::testFormatAggregator() {
    QSharedPointer<Ld::BlockFormat> format1(new Ld::BlockFormat);
    format1->setTopSpacing(10);
    format1->setBottomSpacing(11);
    format1->setLeftIndentation(12);
    format1->setRightIndentation(13);

    Ld::FormatPointerSet formats;

    Ld::BlockFormat::Aggregation aggregation;
    {
        bool success = aggregation.addFormat(format1);
        QVERIFY(success);

        QSharedPointer<Ld::BlockFormat> format2(new Ld::BlockFormat);
        format2->setTopSpacing(20);
        format2->setBottomSpacing(21);
        format2->setLeftIndentation(22);
        format2->setRightIndentation(23);

        success = aggregation.addFormat(format2);
        QVERIFY(success);

        QSharedPointer<Ld::BlockFormat> format3(new Ld::BlockFormat);
        format3->setTopSpacing(30);
        format3->setBottomSpacing(31);
        format3->setLeftIndentation(32);
        format3->setRightIndentation(33);

        success = aggregation.addFormat(format3);
        QVERIFY(success);

        formats = aggregation.formats();
        QCOMPARE(formats.size(), 3);
        QVERIFY(formats.contains(format1));
        QVERIFY(formats.contains(format2));
        QVERIFY(formats.contains(format3));

        QCOMPARE(aggregation.topSpacings().size(), 3);
        QVERIFY(aggregation.topSpacings().contains(10));
        QVERIFY(aggregation.topSpacings().contains(20));
        QVERIFY(aggregation.topSpacings().contains(30));

        QCOMPARE(aggregation.bottomSpacings().size(), 3);
        QVERIFY(aggregation.bottomSpacings().contains(11));
        QVERIFY(aggregation.bottomSpacings().contains(21));
        QVERIFY(aggregation.bottomSpacings().contains(31));

        QCOMPARE(aggregation.leftIndentations().size(), 3);
        QVERIFY(aggregation.leftIndentations().contains(12));
        QVERIFY(aggregation.leftIndentations().contains(22));
        QVERIFY(aggregation.leftIndentations().contains(32));

        QCOMPARE(aggregation.rightIndentations().size(), 3);
        QVERIFY(aggregation.rightIndentations().contains(13));
        QVERIFY(aggregation.rightIndentations().contains(23));
        QVERIFY(aggregation.rightIndentations().contains(33));

        success = aggregation.removeFormat(format3);
        QVERIFY(success);

        success = aggregation.removeFormat(format3);
        QVERIFY(!success);

        formats = aggregation.formats();
        QCOMPARE(formats.size(), 2);
        QVERIFY(formats.contains(format1));
        QVERIFY(formats.contains(format2));

        formats.clear();
    }

    formats = aggregation.formats();
    QCOMPARE(formats.size(), 1);
    QVERIFY(formats.contains(format1));
}
