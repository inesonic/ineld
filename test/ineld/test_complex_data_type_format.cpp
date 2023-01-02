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
* This file implements tests of the \ref Ld::ComplexDataTypeFormat and \ref Ld::RealDataTypeFormat classes.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QPageLayout>
#include <QMarginsF>
#include <QList>
#include <QtTest/QtTest>

#include <ld_format_structures.h>
#include <ld_complex_data_type_format.h>

#include "test_complex_data_type_format.h"

TestComplexDataTypeFormat::TestComplexDataTypeFormat() {}


TestComplexDataTypeFormat::~TestComplexDataTypeFormat() {}


void TestComplexDataTypeFormat::initTestCase() {}


void TestComplexDataTypeFormat::testConstructorsAndDestructors() {
    Ld::ComplexDataTypeFormat format1;
    QVERIFY(format1.isValid());

    format1.setLowerCase();
    format1.setPrecision(2);
    format1.setRealNumberStyle(Ld::ComplexDataTypeFormat::RealNumberStyle::SCIENTIFIC);
    format1.setImaginaryUnit(Ld::ComplexDataTypeFormat::ImaginaryUnit::J);

    Ld::ComplexDataTypeFormat format2(format1);
    QVERIFY(format2.isValid());
    QCOMPARE(format2.lowerCase(), true);
    QCOMPARE(format2.precision(), 2U);
    QCOMPARE(format2.realNumberStyle(), Ld::ComplexDataTypeFormat::RealNumberStyle::SCIENTIFIC);
    QCOMPARE(format2.imaginaryUnit(), Ld::ComplexDataTypeFormat::ImaginaryUnit::J);

    QSharedPointer<Ld::ComplexDataTypeFormat> format3 = format2.clone().dynamicCast<Ld::ComplexDataTypeFormat>();
    QVERIFY(format3->isValid());
    QCOMPARE(format3->lowerCase(), true);
    QCOMPARE(format3->precision(), 2U);
    QCOMPARE(format3->realNumberStyle(), Ld::ComplexDataTypeFormat::RealNumberStyle::SCIENTIFIC);
    QCOMPARE(format3->imaginaryUnit(), Ld::ComplexDataTypeFormat::ImaginaryUnit::J);
}


void TestComplexDataTypeFormat::testValidInvalidMethods() {
    Ld::ComplexDataTypeFormat format;
    QVERIFY(format.isValid());
    QVERIFY(!format.isInvalid());

    format.setRealNumberStyle(Ld::ComplexDataTypeFormat::RealNumberStyle::INVALID);

    QVERIFY(!format.isValid());
    QVERIFY(format.isInvalid());

    format.setRealNumberStyle(Ld::ComplexDataTypeFormat::RealNumberStyle::SCIENTIFIC);

    QVERIFY(format.isValid());
    QVERIFY(!format.isInvalid());

    format.setMultiplier(0.0F);

    QVERIFY(!format.isValid());
    QVERIFY(format.isInvalid());

    format.setMultiplier(1.0F);

    QVERIFY(format.isValid());
    QVERIFY(!format.isInvalid());

    format.setImaginaryUnit(Ld::ComplexDataTypeFormat::ImaginaryUnit::INVALID);

    QVERIFY(!format.isValid());
    QVERIFY(format.isInvalid());

    format.setImaginaryUnit(Ld::ComplexDataTypeFormat::ImaginaryUnit::I);

    QVERIFY(format.isValid());
    QVERIFY(!format.isInvalid());
}


void TestComplexDataTypeFormat::testAccessors() {
    Ld::ComplexDataTypeFormat format;

    format.setLowerCase();
    format.setPrecision(2);
    format.setRealNumberStyle(Ld::ComplexDataTypeFormat::RealNumberStyle::SCIENTIFIC);
    format.setImaginaryUnit(Ld::ComplexDataTypeFormat::ImaginaryUnit::J);

    QVERIFY(format.isValid());
    QCOMPARE(format.lowerCase(), true);
    QCOMPARE(format.precision(), 2U);
    QCOMPARE(format.realNumberStyle(), Ld::ComplexDataTypeFormat::RealNumberStyle::SCIENTIFIC);
    QCOMPARE(format.imaginaryUnit(), Ld::ComplexDataTypeFormat::ImaginaryUnit::J);



    format.setUpperCase();
    format.setPrecision(3);
    format.setRealNumberStyle(Ld::ComplexDataTypeFormat::RealNumberStyle::ENGINEERING);
    format.setImaginaryUnit(Ld::ComplexDataTypeFormat::ImaginaryUnit::I);

    QVERIFY(format.isValid());
    QCOMPARE(format.lowerCase(), false);
    QCOMPARE(format.precision(), 3U);
    QCOMPARE(format.realNumberStyle(), Ld::ComplexDataTypeFormat::RealNumberStyle::ENGINEERING);
    QCOMPARE(format.imaginaryUnit(), Ld::ComplexDataTypeFormat::ImaginaryUnit::I);
}


void TestComplexDataTypeFormat::testToStringMethod() {
    Ld::ComplexDataTypeFormat format;

    format.setFamily("Courier");
    format.setFontSize(12.0);
    format.setFontWeight(Ld::ComplexDataTypeFormat::Weight::BLACK);
    format.setItalics(true);
    format.setUnderline(true);
    format.setOverline(true);
    format.setStrikeout(true);
    format.setLetterSpacing(0.1F);
    format.setFontColor(QColor(0, 255, 0, 255));
    format.setFontBackgroundColor(QColor(255, 0, 0, 0));
    format.setLowerCase();
    format.setRealNumberStyle(Ld::ComplexDataTypeFormat::RealNumberStyle::FLOATING_POINT);
    format.setPrecision(7);
    format.setMultiplier(2.5F);
    format.setImaginaryUnit(Ld::ComplexDataTypeFormat::ImaginaryUnit::J);

    QString description = format.toString();
    QCOMPARE(
        description,
        QString("ComplexDataTypeFormat,Courier,12,0.87,IUOS,0.1,#00ff00,#ff0000,LOWER,FLOATING_POINT,7,2.5,J")
    );
}


void TestComplexDataTypeFormat::testFormatAggregator() {
    QSharedPointer<Ld::ComplexDataTypeFormat> format1(new Ld::ComplexDataTypeFormat);
    format1->setFamily("Helvetica");
    format1->setRealNumberStyle(Ld::ComplexDataTypeFormat::RealNumberStyle::CONCISE);
    format1->setPrecision(6);
    format1->setUpperCase(true);
    format1->setMultiplier(1.0F);
    format1->setImaginaryUnit(Ld::ComplexDataTypeFormat::ImaginaryUnit::I);

    Ld::ComplexDataTypeFormat::Aggregation aggregation;
    bool success = aggregation.addFormat(format1);
    QVERIFY(success);

    QSharedPointer<Ld::ComplexDataTypeFormat> format2(new Ld::ComplexDataTypeFormat);
    format2->setFamily("Courier");
    format2->setRealNumberStyle(Ld::ComplexDataTypeFormat::RealNumberStyle::FLOATING_POINT);
    format2->setPrecision(6);
    format2->setUpperCase(true);
    format2->setMultiplier(1.0F);
    format2->setImaginaryUnit(Ld::ComplexDataTypeFormat::ImaginaryUnit::I);

    success = aggregation.addFormat(format2);
    QVERIFY(success);

    QSharedPointer<Ld::ComplexDataTypeFormat> format3(new Ld::ComplexDataTypeFormat);
    format3->setFamily("Courier");
    format3->setRealNumberStyle(Ld::ComplexDataTypeFormat::RealNumberStyle::FLOATING_POINT);
    format3->setPrecision(5);
    format3->setUpperCase(false);
    format3->setMultiplier(10.0F);
    format3->setImaginaryUnit(Ld::ComplexDataTypeFormat::ImaginaryUnit::J);

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

    QCOMPARE(aggregation.realNumberStyles().size(), 2);
    QVERIFY(aggregation.realNumberStyles().contains(Ld::ComplexDataTypeFormat::RealNumberStyle::CONCISE));
    QVERIFY(aggregation.realNumberStyles().contains(Ld::ComplexDataTypeFormat::RealNumberStyle::FLOATING_POINT));

    QCOMPARE(aggregation.precisions().size(), 2);
    QVERIFY(aggregation.precisions().contains(6));
    QVERIFY(aggregation.precisions().contains(5));

    QCOMPARE(aggregation.upperCase().includesTrue(), true);
    QCOMPARE(aggregation.upperCase().allTrue(), false);

    QCOMPARE(aggregation.multipliers().size(), 2);
    QVERIFY(aggregation.multipliers().contains(1.0F));
    QVERIFY(aggregation.multipliers().contains(10.0F));

    QCOMPARE(aggregation.imaginaryUnits().size(), 2);
    QVERIFY(aggregation.imaginaryUnits().contains(Ld::ComplexDataTypeFormat::ImaginaryUnit::I));
    QVERIFY(aggregation.imaginaryUnits().contains(Ld::ComplexDataTypeFormat::ImaginaryUnit::J));
}
