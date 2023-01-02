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
* This file implements tests of the \ref Ld::MatrixDataTypeFormat class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QPageLayout>
#include <QMarginsF>
#include <QList>
#include <QtTest/QtTest>

#include <ld_format_structures.h>
#include <ld_matrix_data_type_format.h>

#include "test_matrix_data_type_format.h"

TestMatrixDataTypeFormat::TestMatrixDataTypeFormat() {}


TestMatrixDataTypeFormat::~TestMatrixDataTypeFormat() {}


void TestMatrixDataTypeFormat::initTestCase() {}


void TestMatrixDataTypeFormat::testConstructorsAndDestructors() {
    Ld::MatrixDataTypeFormat format1;
    QVERIFY(format1.isValid());

    format1.setLeadingRows(1);
    format1.setTrailingRows(2);
    format1.setLeadingColumns(3);
    format1.setTrailingColumns(4);

    Ld::MatrixDataTypeFormat format2(format1);
    QVERIFY(format2.isValid());
    QCOMPARE(format2.leadingRows(), 1U);
    QCOMPARE(format2.trailingRows(), 2U);
    QCOMPARE(format2.leadingColumns(), 3U);
    QCOMPARE(format2.trailingColumns(), 4U);

    QSharedPointer<Ld::MatrixDataTypeFormat> format3 = format2.clone().dynamicCast<Ld::MatrixDataTypeFormat>();
    QVERIFY(format3->isValid());
    QCOMPARE(format3->leadingRows(), 1U);
    QCOMPARE(format3->trailingRows(), 2U);
    QCOMPARE(format3->leadingColumns(), 3U);
    QCOMPARE(format3->trailingColumns(), 4U);
}


void TestMatrixDataTypeFormat::testValidInvalidMethods() {
    // There are no good ways to invalidate the format -- No test needed.
}


void TestMatrixDataTypeFormat::testAccessors() {
    Ld::MatrixDataTypeFormat format;

    format.setLeadingRows(2);
    format.setTrailingRows(3);
    format.setLeadingColumns(4);
    format.setTrailingColumns(5);

    QVERIFY(format.isValid());
    QCOMPARE(format.leadingRows(), 2U);
    QCOMPARE(format.trailingRows(), 3U);
    QCOMPARE(format.leadingColumns(), 4U);
    QCOMPARE(format.trailingColumns(), 5U);


    format.setLeadingRows(6);
    format.setTrailingRows(7);
    format.setLeadingColumns(8);
    format.setTrailingColumns(9);

    QVERIFY(format.isValid());
    QCOMPARE(format.leadingRows(), 6U);
    QCOMPARE(format.trailingRows(), 7U);
    QCOMPARE(format.leadingColumns(), 8U);
    QCOMPARE(format.trailingColumns(), 9U);
}


void TestMatrixDataTypeFormat::testToStringMethod() {
    Ld::MatrixDataTypeFormat format;

    format.setFamily("Courier");
    format.setFontSize(12.0);
    format.setFontWeight(Ld::MatrixDataTypeFormat::Weight::BLACK);
    format.setItalics(true);
    format.setUnderline(true);
    format.setOverline(true);
    format.setStrikeout(true);
    format.setLetterSpacing(0.1F);
    format.setFontColor(QColor(0, 255, 0, 255));
    format.setFontBackgroundColor(QColor(255, 0, 0, 0));
    format.setLeadingRows(6);
    format.setTrailingRows(7);
    format.setLeadingColumns(8);
    format.setTrailingColumns(9);

    QString description = format.toString();
    QCOMPARE(
        description,
        QString("MatrixDataTypeFormat,Courier,12,0.87,IUOS,0.1,#00ff00,#ff0000,6,7,8,9")
    );
}


void TestMatrixDataTypeFormat::testFormatAggregator() {
    QSharedPointer<Ld::MatrixDataTypeFormat> format1(new Ld::MatrixDataTypeFormat);
    format1->setFamily("Helvetica");
    format1->setLeadingRows(3);
    format1->setTrailingRows(9);
    format1->setLeadingColumns(4);
    format1->setTrailingColumns(8);

    Ld::MatrixDataTypeFormat::Aggregation aggregation;
    bool success = aggregation.addFormat(format1);
    QVERIFY(success);

    QSharedPointer<Ld::MatrixDataTypeFormat> format2(new Ld::MatrixDataTypeFormat);
    format2->setFamily("Courier");
    format2->setLeadingRows(3);
    format2->setTrailingRows(7);
    format2->setLeadingColumns(4);
    format2->setTrailingColumns(2);

    success = aggregation.addFormat(format2);
    QVERIFY(success);

    QSharedPointer<Ld::MatrixDataTypeFormat> format3(new Ld::MatrixDataTypeFormat);
    format3->setFamily("Courier");
    format3->setLeadingRows(5);
    format3->setTrailingRows(9);
    format3->setLeadingColumns(6);
    format3->setTrailingColumns(8);

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

    QCOMPARE(aggregation.leadingRowsCounts().size(), 2);
    QVERIFY(aggregation.leadingRowsCounts().contains(3));
    QVERIFY(aggregation.leadingRowsCounts().contains(5));

    QCOMPARE(aggregation.trailingRowsCounts().size(), 2);
    QVERIFY(aggregation.trailingRowsCounts().contains(9));
    QVERIFY(aggregation.trailingRowsCounts().contains(7));

    QCOMPARE(aggregation.leadingColumnsCounts().size(), 2);
    QVERIFY(aggregation.leadingColumnsCounts().contains(4));
    QVERIFY(aggregation.leadingColumnsCounts().contains(6));

    QCOMPARE(aggregation.trailingColumnsCounts().size(), 2);
    QVERIFY(aggregation.trailingColumnsCounts().contains(2));
    QVERIFY(aggregation.trailingColumnsCounts().contains(8));
}
