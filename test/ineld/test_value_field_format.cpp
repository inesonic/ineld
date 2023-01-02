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
* This file implements tests of the \ref Ld::ValueFieldFormat and \ref Ld::UnifiedDataTypeFormat classes.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QPageLayout>
#include <QMarginsF>
#include <QList>
#include <QtTest/QtTest>

#include <ld_format_structures.h>
#include <ld_variable_name.h>
#include <ld_value_field_format.h>

#include "test_value_field_format.h"

TestValueFieldFormat::TestValueFieldFormat() {}


TestValueFieldFormat::~TestValueFieldFormat() {}


void TestValueFieldFormat::initTestCase() {}


void TestValueFieldFormat::testConstructorsAndDestructors() {
    Ld::ValueFieldFormat format1;
    format1.setVariableName("foo", "bar");

    QVERIFY(!format1.isValid());

    format1.setType(Ld::ValueFieldFormat::Type::COMPLEX);
    QVERIFY(format1.isValid());

    format1.setFamily("Arial");
    format1.setWidth(5);
    format1.setLowerCase();
    format1.setBase(16);
    format1.setIntegerNumberStyle(Ld::ValueFieldFormat::IntegerNumberStyle::C_STYLE);
    format1.setPrecision(2);
    format1.setRealNumberStyle(Ld::ValueFieldFormat::RealNumberStyle::SCIENTIFIC);
    format1.setMultiplier(2.0);
    format1.setImaginaryUnit(Ld::ValueFieldFormat::ImaginaryUnit::J);
    format1.setDisplayMode(Ld::TupleDataTypeFormat::DisplayMode::STRING_BARE);
    format1.setLeadingMemberCount(7);
    format1.setTrailingMemberCount(4);
    format1.setHiddenMemberCountHidden();

    Ld::ValueFieldFormat format2(format1);
    QVERIFY(format2.isValid());

    QCOMPARE(format2.family(), QString("Arial"));
    QCOMPARE(format2.width(), 5U);
    QCOMPARE(format2.lowerCase(), true);
    QCOMPARE(format2.base(), 16U);
    QCOMPARE(format2.integerNumberStyle(), Ld::ValueFieldFormat::IntegerNumberStyle::C_STYLE);
    QCOMPARE(format2.precision(), 2U);
    QCOMPARE(format2.realNumberStyle(), Ld::ValueFieldFormat::RealNumberStyle::SCIENTIFIC);
    QCOMPARE(format2.multiplier(), 2.0);
    QCOMPARE(format2.imaginaryUnit(), Ld::ValueFieldFormat::ImaginaryUnit::J);
    QCOMPARE(format2.displayMode(), Ld::TupleDataTypeFormat::DisplayMode::STRING_BARE);
    QCOMPARE(format2.leadingMemberCount(), 7U);
    QCOMPARE(format2.trailingMemberCount(), 4U);
    QCOMPARE(format2.hideHiddenMemberCount(), true);

    QSharedPointer<Ld::ValueFieldFormat> format3 = format2.clone().dynamicCast<Ld::ValueFieldFormat>();
    QVERIFY(format3->isValid());

    QCOMPARE(format3->family(), QString("Arial"));
    QCOMPARE(format3->width(), 5U);
    QCOMPARE(format3->lowerCase(), true);
    QCOMPARE(format3->base(), 16U);
    QCOMPARE(format3->integerNumberStyle(), Ld::ValueFieldFormat::IntegerNumberStyle::C_STYLE);
    QCOMPARE(format3->precision(), 2U);
    QCOMPARE(format3->realNumberStyle(), Ld::ValueFieldFormat::RealNumberStyle::SCIENTIFIC);
    QCOMPARE(format3->multiplier(), 2.0);
    QCOMPARE(format3->imaginaryUnit(), Ld::ValueFieldFormat::ImaginaryUnit::J);
    QCOMPARE(format3->displayMode(), Ld::TupleDataTypeFormat::DisplayMode::STRING_BARE);
    QCOMPARE(format3->leadingMemberCount(), 7U);
    QCOMPARE(format3->trailingMemberCount(), 4U);
    QCOMPARE(format3->hideHiddenMemberCount(), true);
}


void TestValueFieldFormat::testCapabilities() {
    Ld::ValueFieldFormat format;

    format.setType(Ld::ValueFieldFormat::Type::INTEGER);

    Ld::Format::Capabilities capabilities = format.capabilities();
    QVERIFY( capabilities.contains("Format"));
    QVERIFY( capabilities.contains(Ld::FontFormat::formatName));
    QVERIFY( capabilities.contains(Ld::DataTypeFormat::formatName));
    QVERIFY( capabilities.contains(Ld::NumericDataTypeFormat::formatName));
    QVERIFY( capabilities.contains(Ld::IntegerDataTypeFormat::formatName));
    QVERIFY(!capabilities.contains(Ld::RealDataTypeFormat::formatName));
    QVERIFY(!capabilities.contains(Ld::ComplexDataTypeFormat::formatName));
    QVERIFY(!capabilities.contains(Ld::ListDataTypeFormat::formatName));
    QVERIFY(!capabilities.contains(Ld::SetDataTypeFormat::formatName));
    QVERIFY(!capabilities.contains(Ld::TupleDataTypeFormat::formatName));
    QVERIFY( capabilities.contains(Ld::UnifiedDataTypeFormat::formatName));
    QVERIFY( capabilities.contains(Ld::ValueFieldFormat::formatName));
    QCOMPARE(format.typeName(), Ld::ValueFieldFormat::formatName);

    format.setType(Ld::ValueFieldFormat::Type::REAL);

    capabilities = format.capabilities();
    QVERIFY( capabilities.contains("Format"));
    QVERIFY( capabilities.contains(Ld::FontFormat::formatName));
    QVERIFY( capabilities.contains(Ld::DataTypeFormat::formatName));
    QVERIFY( capabilities.contains(Ld::NumericDataTypeFormat::formatName));
    QVERIFY(!capabilities.contains(Ld::IntegerDataTypeFormat::formatName));
    QVERIFY( capabilities.contains(Ld::RealDataTypeFormat::formatName));
    QVERIFY(!capabilities.contains(Ld::ComplexDataTypeFormat::formatName));
    QVERIFY(!capabilities.contains(Ld::ListDataTypeFormat::formatName));
    QVERIFY(!capabilities.contains(Ld::SetDataTypeFormat::formatName));
    QVERIFY(!capabilities.contains(Ld::TupleDataTypeFormat::formatName));
    QVERIFY( capabilities.contains(Ld::UnifiedDataTypeFormat::formatName));
    QVERIFY( capabilities.contains(Ld::ValueFieldFormat::formatName));
    QCOMPARE(format.typeName(), Ld::ValueFieldFormat::formatName);

    format.setType(Ld::ValueFieldFormat::Type::COMPLEX);

    capabilities = format.capabilities();
    QVERIFY( capabilities.contains("Format"));
    QVERIFY( capabilities.contains(Ld::FontFormat::formatName));
    QVERIFY( capabilities.contains(Ld::DataTypeFormat::formatName));
    QVERIFY( capabilities.contains(Ld::NumericDataTypeFormat::formatName));
    QVERIFY(!capabilities.contains(Ld::IntegerDataTypeFormat::formatName));
    QVERIFY( capabilities.contains(Ld::RealDataTypeFormat::formatName));
    QVERIFY( capabilities.contains(Ld::ComplexDataTypeFormat::formatName));
    QVERIFY(!capabilities.contains(Ld::ListDataTypeFormat::formatName));
    QVERIFY(!capabilities.contains(Ld::SetDataTypeFormat::formatName));
    QVERIFY(!capabilities.contains(Ld::TupleDataTypeFormat::formatName));
    QVERIFY( capabilities.contains(Ld::UnifiedDataTypeFormat::formatName));
    QVERIFY( capabilities.contains(Ld::ValueFieldFormat::formatName));
    QCOMPARE(format.typeName(), Ld::ValueFieldFormat::formatName);
}


void TestValueFieldFormat::testValidInvalidMethods() {
    Ld::ValueFieldFormat format;
    format.setVariableName("foo", "bar");

    format.setType(Ld::ValueFieldFormat::Type::INTEGER);

    QVERIFY( format.isValid());
    QVERIFY(!format.isInvalid());

    format.setType(Ld::ValueFieldFormat::Type::NONE);

    QVERIFY(!format.isValid());
    QVERIFY( format.isInvalid());

    format.setType(Ld::ValueFieldFormat::Type::INTEGER);

    QVERIFY( format.isValid());
    QVERIFY(!format.isInvalid());

    format.setIntegerNumberStyle(Ld::ValueFieldFormat::IntegerNumberStyle::INVALID);

    QVERIFY(!format.isValid());
    QVERIFY( format.isInvalid());

    format.setType(Ld::ValueFieldFormat::Type::REAL);

    QVERIFY( format.isValid());
    QVERIFY(!format.isInvalid());

    format.setRealNumberStyle(Ld::ValueFieldFormat::RealNumberStyle::INVALID);

    QVERIFY(!format.isValid());
    QVERIFY( format.isInvalid());

    format.setRealNumberStyle(Ld::ValueFieldFormat::RealNumberStyle::ENGINEERING);

    QVERIFY( format.isValid());
    QVERIFY(!format.isInvalid());

    format.setType(Ld::ValueFieldFormat::Type::COMPLEX);

    QVERIFY( format.isValid());
    QVERIFY(!format.isInvalid());

    format.setImaginaryUnit(Ld::ValueFieldFormat::ImaginaryUnit::INVALID);

    QVERIFY(!format.isValid());
    QVERIFY( format.isInvalid());

    format.setType(Ld::ValueFieldFormat::Type::REAL);

    QVERIFY( format.isValid());
    QVERIFY(!format.isInvalid());

    format.setVariableName(QString(), QString());

    QVERIFY(!format.isValid());
    QVERIFY( format.isInvalid());

    format.setVariableName("foo", "bar");
    format.setType(Ld::ValueFieldFormat::Type::SET);

    QVERIFY( format.isValid());
    QVERIFY(!format.isInvalid());

    format.setDisplayMode(Ld::ValueFieldFormat::DisplayMode::INVALID);

    QVERIFY(!format.isValid());
    QVERIFY( format.isInvalid());

    format.setType(Ld::ValueFieldFormat::Type::REAL);

    QVERIFY( format.isValid());
    QVERIFY(!format.isInvalid());

    format.setType(Ld::ValueFieldFormat::Type::TUPLE);

    QVERIFY(!format.isValid());
    QVERIFY( format.isInvalid());

    format.setDisplayMode(Ld::ValueFieldFormat::DisplayMode::NORMAL);

    QVERIFY( format.isValid());
    QVERIFY(!format.isInvalid());
}


void TestValueFieldFormat::testAccessors() {
    // Tested by other methods (and other tests).
}


void TestValueFieldFormat::testToStringMethod() {
    Ld::ValueFieldFormat format;
    format.setVariableName("foo", "bar");

    format.setFamily("Courier");
    format.setFontSize(12.0);
    format.setFontWeight(Ld::ValueFieldFormat::Weight::BLACK);
    format.setItalics(true);
    format.setUnderline(true);
    format.setOverline(true);
    format.setStrikeout(true);
    format.setLetterSpacing(0.1F);
    format.setFontColor(QColor(0, 255, 0, 255));
    format.setFontBackgroundColor(QColor(255, 0, 0, 0));
    format.setWidth(5);
    format.setLowerCase();
    format.setBase(16);
    format.setIntegerNumberStyle(Ld::ValueFieldFormat::IntegerNumberStyle::C_STYLE);
    format.setPrecision(2);
    format.setRealNumberStyle(Ld::ValueFieldFormat::RealNumberStyle::SCIENTIFIC);
    format.setMultiplier(2.0);
    format.setImaginaryUnit(Ld::ValueFieldFormat::ImaginaryUnit::J);
    format.setDisplayMode(Ld::TupleDataTypeFormat::DisplayMode::STRING_BARE);
    format.setLeadingMemberCount(7);
    format.setTrailingMemberCount(4);
    format.setHiddenMemberCountHidden();

    format.setType(Ld::ValueFieldFormat::Type::INTEGER);
    QString description = format.toString();
    QCOMPARE(
        description,
        QString(
            "UnifiedDataTypeFormat,IntegerDataTypeFormat,Courier,12,0.87,IUOS,0.1,#00ff00,#ff0000,"
            "LOWER_CASE_TRUE_FALSE,LOWER,5,C_STYLE,16,LOWER,SCIENTIFIC,2,2,J,VERTICAL,7,4,HIDE,"
            "foo,bar"
        )
    );

    format.setType(Ld::ValueFieldFormat::Type::REAL);
    description = format.toString();
    QCOMPARE(
        description,
        QString(
            "UnifiedDataTypeFormat,RealDataTypeFormat,Courier,12,0.87,IUOS,0.1,#00ff00,#ff0000,"
            "LOWER_CASE_TRUE_FALSE,LOWER,5,C_STYLE,16,LOWER,SCIENTIFIC,2,2,J,VERTICAL,7,4,HIDE,"
            "foo,bar"
        )
    );

    format.setType(Ld::ValueFieldFormat::Type::COMPLEX);
    description = format.toString();
    QCOMPARE(
        description,
        QString(
            "UnifiedDataTypeFormat,ComplexDataTypeFormat,Courier,12,0.87,IUOS,0.1,#00ff00,#ff0000,"
            "LOWER_CASE_TRUE_FALSE,LOWER,5,C_STYLE,16,LOWER,SCIENTIFIC,2,2,J,VERTICAL,7,4,HIDE,"
            "foo,bar"
        )
    );
}


void TestValueFieldFormat::testFormatAggregator() {
    QSharedPointer<Ld::ValueFieldFormat> format1(new Ld::ValueFieldFormat);
    format1->setType(Ld::ValueFieldFormat::Type::INTEGER);
    format1->setVariableName("foo", "bar");

    format1->setFamily("Courier");
    format1->setFontSize(12.0);
    format1->setFontWeight(Ld::ValueFieldFormat::Weight::NORMAL);
    format1->setItalics(true);
    format1->setUnderline(false);
    format1->setOverline(false);
    format1->setStrikeout(false);
    format1->setLetterSpacing(0.0F);
    format1->setFontColor(QColor(0, 255, 0, 255));
    format1->setFontBackgroundColor(QColor(255, 0, 255, 0));
    format1->setWidth(5);
    format1->setLowerCase();
    format1->setBase(10);
    format1->setIntegerNumberStyle(Ld::ValueFieldFormat::IntegerNumberStyle::VERILOG_STYLE);
    format1->setPrecision(2);
    format1->setRealNumberStyle(Ld::ValueFieldFormat::RealNumberStyle::SCIENTIFIC);
    format1->setMultiplier(2.0);
    format1->setImaginaryUnit(Ld::ValueFieldFormat::ImaginaryUnit::I);
    format1->setDisplayMode(Ld::TupleDataTypeFormat::DisplayMode::NORMAL);
    format1->setLeadingMemberCount(8);
    format1->setTrailingMemberCount(3);
    format1->setHiddenMemberCountVisible();

    Ld::ValueFieldFormat::Aggregation aggregation;
    bool success = aggregation.addFormat(format1);
    QVERIFY(success);

    QSharedPointer<Ld::ValueFieldFormat> format2(new Ld::ValueFieldFormat);
    format2->setType(Ld::ValueFieldFormat::Type::REAL);
    format2->setVariableName("foo", "bang");

    format2->setFamily("Helvetica");
    format2->setFontSize(11.0);
    format2->setFontWeight(Ld::ValueFieldFormat::Weight::BLACK);
    format2->setItalics(true);
    format2->setUnderline(false);
    format2->setOverline(true);
    format2->setStrikeout(true);
    format2->setLetterSpacing(0.1F);
    format2->setFontColor(QColor(255, 255, 0, 255));
    format2->setFontBackgroundColor(QColor(255, 255, 0, 0));
    format2->setWidth(6);
    format2->setUpperCase();
    format2->setBase(16);
    format2->setIntegerNumberStyle(Ld::ValueFieldFormat::IntegerNumberStyle::C_STYLE);
    format2->setPrecision(3);
    format2->setRealNumberStyle(Ld::ValueFieldFormat::RealNumberStyle::SCIENTIFIC);
    format2->setMultiplier(1.0);
    format2->setImaginaryUnit(Ld::ValueFieldFormat::ImaginaryUnit::I);
    format2->setDisplayMode(Ld::TupleDataTypeFormat::DisplayMode::STRING_BARE);
    format2->setLeadingMemberCount(9);
    format2->setTrailingMemberCount(5);
    format2->setHiddenMemberCountVisible();

    success = aggregation.addFormat(format2);
    QVERIFY(success);

    QSharedPointer<Ld::ValueFieldFormat> format3(new Ld::ValueFieldFormat);
    format3->setType(Ld::ValueFieldFormat::Type::COMPLEX);
    format3->setVariableName("snort", "bang");

    format3->setFamily("Courier");
    format3->setFontSize(11.0);
    format3->setFontWeight(Ld::ValueFieldFormat::Weight::NORMAL);
    format3->setItalics(true);
    format3->setUnderline(false);
    format3->setOverline(false);
    format3->setStrikeout(false);
    format3->setLetterSpacing(0.0F);
    format3->setFontColor(QColor(0, 255, 255, 255));
    format3->setFontBackgroundColor(QColor(255, 0, 0, 0));
    format3->setWidth(5);
    format3->setLowerCase();
    format3->setBase(10);
    format3->setIntegerNumberStyle(Ld::ValueFieldFormat::IntegerNumberStyle::C_STYLE);
    format3->setPrecision(3);
    format3->setRealNumberStyle(Ld::ValueFieldFormat::RealNumberStyle::ENGINEERING);
    format3->setMultiplier(1.0);
    format3->setImaginaryUnit(Ld::ValueFieldFormat::ImaginaryUnit::J);
    format3->setDisplayMode(Ld::TupleDataTypeFormat::DisplayMode::NORMAL);
    format3->setLeadingMemberCount(9);
    format3->setTrailingMemberCount(3);
    format3->setHiddenMemberCountHidden();

    success = aggregation.addFormat(format3);
    QVERIFY(success);

    QSharedPointer<Ld::ValueFieldFormat> format4(new Ld::ValueFieldFormat);
    format4->setType(Ld::ValueFieldFormat::Type::SET);
    format4->setVariableName("snort", "bang");

    format4->setFamily("Courier");
    format4->setFontSize(11.0);
    format4->setFontWeight(Ld::ValueFieldFormat::Weight::NORMAL);
    format4->setItalics(true);
    format4->setUnderline(false);
    format4->setOverline(false);
    format4->setStrikeout(false);
    format4->setLetterSpacing(0.0F);
    format4->setFontColor(QColor(0, 255, 255, 255));
    format4->setFontBackgroundColor(QColor(255, 0, 0, 0));
    format4->setWidth(5);
    format4->setLowerCase();
    format4->setBase(10);
    format4->setIntegerNumberStyle(Ld::ValueFieldFormat::IntegerNumberStyle::C_STYLE);
    format4->setPrecision(3);
    format4->setRealNumberStyle(Ld::ValueFieldFormat::RealNumberStyle::ENGINEERING);
    format4->setMultiplier(1.0);
    format4->setImaginaryUnit(Ld::ValueFieldFormat::ImaginaryUnit::J);
    format4->setDisplayMode(Ld::TupleDataTypeFormat::DisplayMode::NORMAL);
    format4->setLeadingMemberCount(9);
    format4->setTrailingMemberCount(3);
    format4->setHiddenMemberCountHidden();

    success = aggregation.addFormat(format4);
    QVERIFY(success);

    Ld::FormatPointerSet formats = aggregation.formats();
    QCOMPARE(formats.size(), 4);
    QVERIFY(formats.contains(format1));
    QVERIFY(formats.contains(format2));
    QVERIFY(formats.contains(format3));
    QVERIFY(formats.contains(format4));

    QCOMPARE(aggregation.families().size(), 2);
    QVERIFY(aggregation.families().contains(QString("Courier")));
    QVERIFY(aggregation.families().contains(QString("Helvetica")));

    QCOMPARE(aggregation.sizes().size(), 2);
    QVERIFY(aggregation.sizes().contains(12.0));
    QVERIFY(aggregation.sizes().contains(11.0));

    QCOMPARE(aggregation.weights().size(), 2);
    QVERIFY(aggregation.weights().contains(Ld::ValueFieldFormat::Weight::BLACK));
    QVERIFY(aggregation.weights().contains(Ld::ValueFieldFormat::Weight::NORMAL));

    QVERIFY(aggregation.italics().allTrue());
    QVERIFY(aggregation.underline().allFalse());
    QVERIFY(aggregation.overline().bothTrueAndFalse());
    QVERIFY(aggregation.strikeOut().bothTrueAndFalse());

    QCOMPARE(aggregation.letterSpacing().size(), 2);
    QVERIFY(aggregation.letterSpacing().contains(0.1F));
    QVERIFY(aggregation.letterSpacing().contains(0.0F));

    QCOMPARE(aggregation.fontColor().size(), 3);
    QVERIFY(aggregation.fontColor().contains(QColor(0, 255, 255, 255)));
    QVERIFY(aggregation.fontColor().contains(QColor(255, 255, 0, 255)));
    QVERIFY(aggregation.fontColor().contains(QColor(0, 255, 0, 255)));

    QCOMPARE(aggregation.fontBackgroundColor().size(), 3);
    QVERIFY(aggregation.fontBackgroundColor().contains(QColor(255, 255, 0, 0)));
    QVERIFY(aggregation.fontBackgroundColor().contains(QColor(255, 0, 0, 0)));
    QVERIFY(aggregation.fontBackgroundColor().contains(QColor(255, 0, 255, 0)));

    QCOMPARE(aggregation.widths().size(), 2);
    QVERIFY(aggregation.widths().contains(6));
    QVERIFY(aggregation.widths().contains(5));

    QVERIFY(aggregation.upperCase().bothTrueAndFalse());

    QCOMPARE(aggregation.bases().size(), 2);
    QVERIFY(aggregation.bases().contains(10));
    QVERIFY(aggregation.bases().contains(16));

    QCOMPARE(aggregation.integerNumberStyles().size(), 2);
    QVERIFY(aggregation.integerNumberStyles().contains(Ld::ValueFieldFormat::IntegerNumberStyle::C_STYLE));
    QVERIFY(aggregation.integerNumberStyles().contains(Ld::ValueFieldFormat::IntegerNumberStyle::VERILOG_STYLE));

    QCOMPARE(aggregation.precisions().size(), 2);
    QVERIFY(aggregation.precisions().contains(3));
    QVERIFY(aggregation.precisions().contains(2));

    QCOMPARE(aggregation.realNumberStyles().size(), 2);
    QVERIFY(aggregation.realNumberStyles().contains(Ld::ValueFieldFormat::RealNumberStyle::SCIENTIFIC));
    QVERIFY(aggregation.realNumberStyles().contains(Ld::ValueFieldFormat::RealNumberStyle::ENGINEERING));

    QCOMPARE(aggregation.multipliers().size(), 2);
    QVERIFY(aggregation.multipliers().contains(1.0));
    QVERIFY(aggregation.multipliers().contains(2.0));

    QCOMPARE(aggregation.imaginaryUnits().size(), 2);
    QVERIFY(aggregation.imaginaryUnits().contains(Ld::ValueFieldFormat::ImaginaryUnit::I));
    QVERIFY(aggregation.imaginaryUnits().contains(Ld::ValueFieldFormat::ImaginaryUnit::J));

    QCOMPARE(aggregation.typeNames().size(), 4);
    QVERIFY(aggregation.typeNames().contains(Ld::IntegerDataTypeFormat::formatName));
    QVERIFY(aggregation.typeNames().contains(Ld::RealDataTypeFormat::formatName));
    QVERIFY(aggregation.typeNames().contains(Ld::ComplexDataTypeFormat::formatName));
    QVERIFY(aggregation.typeNames().contains(Ld::SetDataTypeFormat::formatName));

    QCOMPARE(aggregation.displayModes().size(), 2);
    QVERIFY(aggregation.displayModes().contains(Ld::TupleDataTypeFormat::DisplayMode::NORMAL));
    QVERIFY(aggregation.displayModes().contains(Ld::TupleDataTypeFormat::DisplayMode::STRING_BARE));

    QCOMPARE(aggregation.leadingMemberCounts().size(), 2);
    QVERIFY(aggregation.leadingMemberCounts().contains(8U));
    QVERIFY(aggregation.leadingMemberCounts().contains(9U));

    QCOMPARE(aggregation.trailingMemberCounts().size(), 2);
    QVERIFY(aggregation.trailingMemberCounts().contains(3));
    QVERIFY(aggregation.trailingMemberCounts().contains(5));

    QCOMPARE(aggregation.showHiddenMemberCounts().bothTrueAndFalse(), true);
    QCOMPARE(aggregation.variableNames().size(), 3);
    QVERIFY(aggregation.variableNames().contains(Ld::VariableName("foo", "bar")));
    QVERIFY(aggregation.variableNames().contains(Ld::VariableName("foo", "bang")));
    QVERIFY(aggregation.variableNames().contains(Ld::VariableName("snort", "bang")));

    QCOMPARE(aggregation.text1().size(), 2);
    QVERIFY(aggregation.text1().contains("foo"));
    QVERIFY(aggregation.text1().contains("snort"));

    QCOMPARE(aggregation.text2().size(), 2);
    QVERIFY(aggregation.text2().contains("bar"));
    QVERIFY(aggregation.text2().contains("bang"));
}
