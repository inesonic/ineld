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
* This file implements tests of the \ref Ld::ListDataTypeFormat class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QPageLayout>
#include <QMarginsF>
#include <QList>
#include <QtTest/QtTest>

#include <ld_format_structures.h>
#include <ld_list_data_type_format.h>

#include "test_list_data_type_format.h"

TestListDataTypeFormat::TestListDataTypeFormat() {}


TestListDataTypeFormat::~TestListDataTypeFormat() {}


void TestListDataTypeFormat::initTestCase() {}


void TestListDataTypeFormat::testConstructorsAndDestructors() {
    Ld::ListDataTypeFormat format1;
    QVERIFY(format1.isValid());

    format1.setLeadingMemberCount(7);
    format1.setTrailingMemberCount(4);
    format1.setHiddenMemberCountHidden();

    Ld::ListDataTypeFormat format2(format1);
    QVERIFY(format2.isValid());
    QCOMPARE(format2.leadingMemberCount(), 7U);
    QCOMPARE(format2.trailingMemberCount(), 4U);
    QCOMPARE(format2.hideHiddenMemberCount(), true);

    QSharedPointer<Ld::ListDataTypeFormat> format3 = format2.clone().dynamicCast<Ld::ListDataTypeFormat>();
    QVERIFY(format3->isValid());
    QCOMPARE(format3->leadingMemberCount(), 7U);
    QCOMPARE(format3->trailingMemberCount(), 4U);
    QCOMPARE(format3->hideHiddenMemberCount(), true);
}


void TestListDataTypeFormat::testValidInvalidMethods() {
    Ld::ListDataTypeFormat format;
    QVERIFY(format.isValid());
    QVERIFY(!format.isInvalid());
}


void TestListDataTypeFormat::testAccessors() {
    Ld::ListDataTypeFormat format;

    format.setLeadingMemberCount(7);
    format.setTrailingMemberCount(4);
    format.setHiddenMemberCountHidden();

    QVERIFY(format.isValid());
    QCOMPARE(format.leadingMemberCount(), 7U);
    QCOMPARE(format.trailingMemberCount(), 4U);
    QCOMPARE(format.hideHiddenMemberCount(), true);
    QCOMPARE(format.displayHiddenMemberCount(), false);


    format.setLeadingMemberCount(8);
    format.setTrailingMemberCount(3);
    format.setHiddenMemberCountVisible();

    QVERIFY(format.isValid());
    QCOMPARE(format.leadingMemberCount(), 8U);
    QCOMPARE(format.trailingMemberCount(), 3U);
    QCOMPARE(format.hideHiddenMemberCount(), false);
    QCOMPARE(format.displayHiddenMemberCount(), true);
}


void TestListDataTypeFormat::testToStringMethod() {
    Ld::ListDataTypeFormat format;

    format.setFamily("Courier");
    format.setFontSize(12.0);
    format.setFontWeight(Ld::ListDataTypeFormat::Weight::BLACK);
    format.setItalics(true);
    format.setUnderline(true);
    format.setOverline(true);
    format.setStrikeout(true);
    format.setLetterSpacing(0.1F);
    format.setFontColor(QColor(0, 255, 0, 255));
    format.setFontBackgroundColor(QColor(255, 0, 0, 0));
    format.setLeadingMemberCount(8);
    format.setTrailingMemberCount(3);
    format.setHiddenMemberCountVisible();

    QString description = format.toString();
    QCOMPARE(
        description,
        QString("ListDataTypeFormat,Courier,12,0.87,IUOS,0.1,#00ff00,#ff0000,8,3,SHOW")
    );
}


void TestListDataTypeFormat::testFormatAggregator() {
    QSharedPointer<Ld::ListDataTypeFormat> format1(new Ld::ListDataTypeFormat);
    format1->setFamily("Helvetica");
    format1->setLeadingMemberCount(8);
    format1->setTrailingMemberCount(3);
    format1->setHiddenMemberCountVisible();

    Ld::ListDataTypeFormat::Aggregation aggregation;
    bool success = aggregation.addFormat(format1);
    QVERIFY(success);

    QSharedPointer<Ld::ListDataTypeFormat> format2(new Ld::ListDataTypeFormat);
    format2->setFamily("Courier");
    format2->setLeadingMemberCount(9);
    format2->setTrailingMemberCount(5);
    format2->setHiddenMemberCountVisible();

    success = aggregation.addFormat(format2);
    QVERIFY(success);

    QSharedPointer<Ld::ListDataTypeFormat> format3(new Ld::ListDataTypeFormat);
    format3->setFamily("Courier");
    format3->setLeadingMemberCount(9);
    format3->setTrailingMemberCount(3);
    format3->setHiddenMemberCountHidden();

    success = aggregation.addFormat(format3);
    QVERIFY(success);

    Ld::FormatPointerSet formats = aggregation.formats();
    QCOMPARE(formats.size(), 3);
    QVERIFY(formats.contains(format1));
    QVERIFY(formats.contains(format2));
    QVERIFY(formats.contains(format3));

    QCOMPARE(aggregation.families().size(), 2);
    QVERIFY(aggregation.families().contains(QString("Courier")));
    QVERIFY(aggregation.families().contains(QString("Helvetica")));

    QCOMPARE(aggregation.leadingMemberCounts().size(), 2);
    QVERIFY(aggregation.leadingMemberCounts().contains(8U));
    QVERIFY(aggregation.leadingMemberCounts().contains(9U));

    QCOMPARE(aggregation.trailingMemberCounts().size(), 2);
    QVERIFY(aggregation.trailingMemberCounts().contains(3));
    QVERIFY(aggregation.trailingMemberCounts().contains(5));

    QCOMPARE(aggregation.showHiddenMemberCounts().bothTrueAndFalse(), true);
}
