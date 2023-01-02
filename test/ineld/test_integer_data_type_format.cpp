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
* This file implements tests of the Ld::IntegerDataTypeFormat class.  We also use this module to test the
* \ref Ld::NumericDataTypeFormat and \ref Ld::DataTypeFormat classes.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QPageLayout>
#include <QMarginsF>
#include <QList>
#include <QtTest/QtTest>

#include <ld_format_structures.h>
#include <ld_integer_data_type_format.h>

#include "test_integer_data_type_format.h"

TestIntegerDataTypeFormat::TestIntegerDataTypeFormat() {}


TestIntegerDataTypeFormat::~TestIntegerDataTypeFormat() {}


void TestIntegerDataTypeFormat::initTestCase() {}


void TestIntegerDataTypeFormat::testConstructorsAndDestructors() {
    Ld::IntegerDataTypeFormat format1;
    QVERIFY(format1.isValid());

    format1.setWidth(5);
    format1.setLowerCase();
    format1.setBase(11);
    format1.setIntegerNumberStyle(Ld::IntegerDataTypeFormat::IntegerNumberStyle::VERILOG_STYLE);

    Ld::IntegerDataTypeFormat format2(format1);
    QVERIFY(format2.isValid());
    QCOMPARE(format2.width(), 5U);
    QCOMPARE(format2.lowerCase(), true);
    QCOMPARE(format2.base(), 11U);
    QCOMPARE(format2.integerNumberStyle(), Ld::IntegerDataTypeFormat::IntegerNumberStyle::VERILOG_STYLE);

    QSharedPointer<Ld::IntegerDataTypeFormat> format3 = format2.clone().dynamicCast<Ld::IntegerDataTypeFormat>();
    QVERIFY(format3->isValid());
    QCOMPARE(format3->width(), 5U);
    QCOMPARE(format3->lowerCase(), true);
    QCOMPARE(format3->base(), 11U);
    QCOMPARE(format3->integerNumberStyle(), Ld::IntegerDataTypeFormat::IntegerNumberStyle::VERILOG_STYLE);
}


void TestIntegerDataTypeFormat::testValidInvalidMethods() {
    Ld::IntegerDataTypeFormat format;
    QVERIFY(format.isValid());
    QVERIFY(!format.isInvalid());

    format.setBase(0);

    QVERIFY(!format.isValid());
    QVERIFY(format.isInvalid());

    format.setBase(1);

    QVERIFY(!format.isValid());
    QVERIFY(format.isInvalid());

    format.setBase(2);

    QVERIFY(format.isValid());
    QVERIFY(!format.isInvalid());

    format.setIntegerNumberStyle(Ld::IntegerDataTypeFormat::IntegerNumberStyle::INVALID);

    QVERIFY(!format.isValid());
    QVERIFY(format.isInvalid());
}


void TestIntegerDataTypeFormat::testAccessors() {
    Ld::IntegerDataTypeFormat format;

    format.setBase(5);
    format.setIntegerNumberStyle(Ld::IntegerDataTypeFormat::IntegerNumberStyle::C_STYLE);
    format.setWidth(6);
    format.setUpperCase();

    QCOMPARE(format.base(), 5U);
    QCOMPARE(format.integerNumberStyle(), Ld::IntegerDataTypeFormat::IntegerNumberStyle::C_STYLE);
    QCOMPARE(format.width(), 6U);
    QCOMPARE(format.upperCase(), true);
    QCOMPARE(format.lowerCase(), false);


    format.setBase(7);
    format.setIntegerNumberStyle(Ld::IntegerDataTypeFormat::IntegerNumberStyle::SUBSCRIPT_STYLE);
    format.setWidth(8);
    format.setLowerCase();

    QCOMPARE(format.base(), 7U);
    QCOMPARE(format.integerNumberStyle(), Ld::IntegerDataTypeFormat::IntegerNumberStyle::SUBSCRIPT_STYLE);
    QCOMPARE(format.width(), 8U);
    QCOMPARE(format.upperCase(), false);
    QCOMPARE(format.lowerCase(), true);

    format.setLowerCase(false);
    QCOMPARE(format.upperCase(), true);
    QCOMPARE(format.lowerCase(), false);

    format.setUpperCase(false);
    QCOMPARE(format.upperCase(), false);
    QCOMPARE(format.lowerCase(), true);
}


void TestIntegerDataTypeFormat::testToStringMethod() {
    Ld::IntegerDataTypeFormat format;

    format.setFamily("Courier");
    format.setFontSize(12.0);
    format.setFontWeight(Ld::IntegerDataTypeFormat::Weight::BLACK);
    format.setItalics(true);
    format.setUnderline(true);
    format.setOverline(true);
    format.setStrikeout(true);
    format.setLetterSpacing(0.1F);
    format.setFontColor(QColor(0, 255, 0, 255));
    format.setFontBackgroundColor(QColor(255, 0, 0, 0));
    format.setBase(12);
    format.setIntegerNumberStyle(Ld::IntegerDataTypeFormat::IntegerNumberStyle::C_STYLE);
    format.setWidth(13);
    format.setLowerCase();

    QString description = format.toString();
    QCOMPARE(
        description,
        QString("IntegerDataTypeFormat,Courier,12,0.87,IUOS,0.1,#00ff00,#ff0000,LOWER,13,C_STYLE,12")
    );
}


void TestIntegerDataTypeFormat::testFormatAggregator() {
    QSharedPointer<Ld::IntegerDataTypeFormat> format1(new Ld::IntegerDataTypeFormat);
    format1->setFamily("Courier");
    format1->setBase(10);
    format1->setIntegerNumberStyle(Ld::IntegerDataTypeFormat::IntegerNumberStyle::NONE);
    format1->setWidth(4);
    format1->setUpperCase(true);

    Ld::IntegerDataTypeFormat::Aggregation aggregation;
    {
        bool success = aggregation.addFormat(format1);
        QVERIFY(success);

        QSharedPointer<Ld::IntegerDataTypeFormat> format2(new Ld::IntegerDataTypeFormat);
        format2->setFamily("Helvetica");
        format2->setBase(11);
        format2->setIntegerNumberStyle(Ld::IntegerDataTypeFormat::IntegerNumberStyle::NONE);
        format2->setWidth(5);
        format2->setUpperCase(false);

        success = aggregation.addFormat(format2);
        QVERIFY(success);

        QSharedPointer<Ld::IntegerDataTypeFormat> format3(new Ld::IntegerDataTypeFormat);
        format3->setFamily("Courier");
        format3->setBase(10);
        format3->setIntegerNumberStyle(Ld::IntegerDataTypeFormat::IntegerNumberStyle::C_STYLE);
        format3->setWidth(5);
        format3->setUpperCase(false);

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

        QCOMPARE(aggregation.bases().size(), 2);
        QVERIFY(aggregation.bases().contains(10));
        QVERIFY(aggregation.bases().contains(11));

        QCOMPARE(aggregation.widths().size(), 2);
        QVERIFY(aggregation.widths().contains(4));
        QVERIFY(aggregation.widths().contains(5));

        QCOMPARE(aggregation.integerNumberStyles().size(), 2);
        QVERIFY(aggregation.integerNumberStyles().contains(Ld::IntegerDataTypeFormat::IntegerNumberStyle::NONE));
        QVERIFY(aggregation.integerNumberStyles().contains(Ld::IntegerDataTypeFormat::IntegerNumberStyle::C_STYLE));

        QCOMPARE(aggregation.upperCase().includesTrue(), true);
        QCOMPARE(aggregation.upperCase().allTrue(), false);

        QCOMPARE(aggregation.lowerCase().includesTrue(), true);
        QCOMPARE(aggregation.lowerCase().allTrue(), false);
    }
}
