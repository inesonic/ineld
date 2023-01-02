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
* This file implements tests of the Ld::BooleanDataTypeFormat class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QPageLayout>
#include <QMarginsF>
#include <QList>
#include <QtTest/QtTest>

#include <ld_format_structures.h>
#include <ld_boolean_data_type_format.h>

#include "test_boolean_data_type_format.h"

TestBooleanDataTypeFormat::TestBooleanDataTypeFormat() {}


TestBooleanDataTypeFormat::~TestBooleanDataTypeFormat() {}


void TestBooleanDataTypeFormat::initTestCase() {}


void TestBooleanDataTypeFormat::testConstructorsAndDestructors() {
    Ld::BooleanDataTypeFormat format1;
    QVERIFY(format1.isValid());

    format1.setBooleanStyle(Ld::BooleanDataTypeFormat::BooleanStyle::NUMERIC_1_0);

    Ld::BooleanDataTypeFormat format2(format1);
    QVERIFY(format2.isValid());
    QCOMPARE(format2.booleanStyle(), Ld::BooleanDataTypeFormat::BooleanStyle::NUMERIC_1_0);

    QSharedPointer<Ld::BooleanDataTypeFormat> format3 = format2.clone().dynamicCast<Ld::BooleanDataTypeFormat>();
    QVERIFY(format3->isValid());
    QCOMPARE(format3->booleanStyle(), Ld::BooleanDataTypeFormat::BooleanStyle::NUMERIC_1_0);
}


void TestBooleanDataTypeFormat::testValidInvalidMethods() {
    Ld::BooleanDataTypeFormat format;
    QVERIFY(format.isValid());
    QVERIFY(!format.isInvalid());

    format.setBooleanStyle(Ld::BooleanDataTypeFormat::BooleanStyle::INVALID);

    QVERIFY(!format.isValid());
    QVERIFY(format.isInvalid());
}


void TestBooleanDataTypeFormat::testAccessors() {
    Ld::BooleanDataTypeFormat format;

    format.setBooleanStyle(Ld::BooleanDataTypeFormat::BooleanStyle::LOWER_CASE_TRUE_FALSE);
    QCOMPARE(format.booleanStyle(), Ld::BooleanDataTypeFormat::BooleanStyle::LOWER_CASE_TRUE_FALSE);

    format.setBooleanStyle(Ld::BooleanDataTypeFormat::BooleanStyle::TITLE_CASE_TRUE_FALSE);
    QCOMPARE(format.booleanStyle(), Ld::BooleanDataTypeFormat::BooleanStyle::TITLE_CASE_TRUE_FALSE);

    format.setBooleanStyle(Ld::BooleanDataTypeFormat::BooleanStyle::UPPER_CASE_TRUE_FALSE);
    QCOMPARE(format.booleanStyle(), Ld::BooleanDataTypeFormat::BooleanStyle::UPPER_CASE_TRUE_FALSE);

    format.setBooleanStyle(Ld::BooleanDataTypeFormat::BooleanStyle::NUMERIC_1_0);
    QCOMPARE(format.booleanStyle(), Ld::BooleanDataTypeFormat::BooleanStyle::NUMERIC_1_0);
}


void TestBooleanDataTypeFormat::testToStringMethod() {
    Ld::BooleanDataTypeFormat format;

    format.setFamily("Courier");
    format.setFontSize(12.0);
    format.setFontWeight(Ld::BooleanDataTypeFormat::Weight::BLACK);
    format.setItalics(true);
    format.setUnderline(true);
    format.setOverline(true);
    format.setStrikeout(true);
    format.setLetterSpacing(0.1F);
    format.setFontColor(QColor(0, 255, 0, 255));
    format.setFontBackgroundColor(QColor(255, 0, 0, 0));
    format.setBooleanStyle(Ld::BooleanDataTypeFormat::BooleanStyle::UPPER_CASE_TRUE_FALSE);

    QString description = format.toString();
    QCOMPARE(
        description,
        QString("BooleanDataTypeFormat,Courier,12,0.87,IUOS,0.1,#00ff00,#ff0000,UPPER_CASE_TRUE_FALSE")
    );

    format.setBooleanStyle(Ld::BooleanDataTypeFormat::BooleanStyle::TITLE_CASE_TRUE_FALSE);
    description = format.toString();
    QCOMPARE(
        description,
        QString("BooleanDataTypeFormat,Courier,12,0.87,IUOS,0.1,#00ff00,#ff0000,TITLE_CASE_TRUE_FALSE")
    );

    format.setBooleanStyle(Ld::BooleanDataTypeFormat::BooleanStyle::LOWER_CASE_TRUE_FALSE);
    description = format.toString();
    QCOMPARE(
        description,
        QString("BooleanDataTypeFormat,Courier,12,0.87,IUOS,0.1,#00ff00,#ff0000,LOWER_CASE_TRUE_FALSE")
    );

    format.setBooleanStyle(Ld::BooleanDataTypeFormat::BooleanStyle::NUMERIC_1_0);
    description = format.toString();
    QCOMPARE(
        description,
        QString("BooleanDataTypeFormat,Courier,12,0.87,IUOS,0.1,#00ff00,#ff0000,NUMERIC_1_0")
    );
}


void TestBooleanDataTypeFormat::testFormatAggregator() {
    QSharedPointer<Ld::BooleanDataTypeFormat> format1(new Ld::BooleanDataTypeFormat);
    format1->setFamily("Courier");
    format1->setBooleanStyle(Ld::BooleanDataTypeFormat::BooleanStyle::NUMERIC_1_0);

    Ld::BooleanDataTypeFormat::Aggregation aggregation;
    {
        bool success = aggregation.addFormat(format1);
        QVERIFY(success);

        QSharedPointer<Ld::BooleanDataTypeFormat> format2(new Ld::BooleanDataTypeFormat);
        format2->setFamily("Helvetica");
        format2->setBooleanStyle(Ld::BooleanDataTypeFormat::BooleanStyle::UPPER_CASE_TRUE_FALSE);

        success = aggregation.addFormat(format2);
        QVERIFY(success);

        QSharedPointer<Ld::BooleanDataTypeFormat> format3(new Ld::BooleanDataTypeFormat);
        format3->setFamily("Courier");
        format3->setBooleanStyle(Ld::BooleanDataTypeFormat::BooleanStyle::NUMERIC_1_0);

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

        QCOMPARE(aggregation.booleanStyles().size(), 2);
        QVERIFY(aggregation.booleanStyles().contains(Ld::BooleanDataTypeFormat::BooleanStyle::NUMERIC_1_0));
        QVERIFY(aggregation.booleanStyles().contains(Ld::BooleanDataTypeFormat::BooleanStyle::UPPER_CASE_TRUE_FALSE));
    }
}
