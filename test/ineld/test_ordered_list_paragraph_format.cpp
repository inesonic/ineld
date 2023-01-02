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
* This file implements tests of the Ld::OrderedListParagraphFormat class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QtTest/QtTest>

#include <ld_ordered_list_paragraph_format.h>

#include "test_ordered_list_paragraph_format.h"

TestOrderedListParagraphFormat::TestOrderedListParagraphFormat() {}


TestOrderedListParagraphFormat::~TestOrderedListParagraphFormat() {}


void TestOrderedListParagraphFormat::initTestCase() {}


void TestOrderedListParagraphFormat::testConstructorsAndDestructors() {
    Ld::OrderedListParagraphFormat format1;
    QVERIFY(format1.isValid());
    QCOMPARE(format1.leftIndentation(), Ld::OrderedListParagraphFormat::defaultLeftIndentation);
    QCOMPARE(format1.rightIndentation(), Ld::OrderedListParagraphFormat::defaultRightIndentation);
    QCOMPARE(format1.topSpacing(), Ld::OrderedListParagraphFormat::defaultTopSpacing);
    QCOMPARE(format1.bottomSpacing(), Ld::OrderedListParagraphFormat::defaultBottomSpacing);
    QCOMPARE(format1.justificationMode(), Ld::OrderedListParagraphFormat::defaultJustification);
    QCOMPARE(format1.lineSpacing(), Ld::OrderedListParagraphFormat::defaultLineSpacing);
    QCOMPARE(format1.listIndentation(), Ld::OrderedListParagraphFormat::defaultListIndentation);
    QCOMPARE(format1.family(), QString(Ld::OrderedListParagraphFormat::defaultFontFamily));
    QCOMPARE(format1.fontSize(), Ld::OrderedListParagraphFormat::defaultFontSizePoints);
    QCOMPARE(format1.fontWeight(), Ld::OrderedListParagraphFormat::defaultFontWeight);
    QCOMPARE(format1.italics(), false);
    QCOMPARE(format1.underline(), false);
    QCOMPARE(format1.overline(), false);
    QCOMPARE(format1.strikeout(), false);
    QCOMPARE(format1.letterSpacing(), Ld::OrderedListParagraphFormat::defaultLetterSpacing);
    QCOMPARE(format1.fontColor(), Ld::OrderedListParagraphFormat::defaultFontColor);
    QCOMPARE(format1.fontBackgroundColor(), Ld::OrderedListParagraphFormat::defaultFontBackgroundColor);
    QCOMPARE(format1.startingNumber(), 1U);
    QCOMPARE(format1.style(), Ld::OrderedListParagraphFormat::defaultNumberStyle);

    format1.setLeftIndentation(72.0);
    format1.setRightIndentation(73.0);
    format1.setTopSpacing(6.0);
    format1.setBottomSpacing(12.0);
    format1.setJustification(Ld::OrderedListParagraphFormat::Justification::CENTER);
    format1.clearFirstLineLeftIndentation();
    format1.setLineSpacing(2.5);
    format1.setListIndentation(19.0);
    format1.setFamily("Times Roman");
    format1.setFontSize(14.0);
    format1.setFontWeight(Ld::OrderedListParagraphFormat::Weight::BLACK);
    format1.setStartingNumber(2);
    format1.setStyle(Ld::OrderedListParagraphFormat::Style::ROMAN_LOWER_CASE);

    Ld::OrderedListParagraphFormat format2(format1);
    QVERIFY(format2.isValid());
    QCOMPARE(format2.leftIndentation(), 72.0);
    QCOMPARE(format2.rightIndentation(), 73.0);
    QCOMPARE(format2.topSpacing(), 6.0);
    QCOMPARE(format2.bottomSpacing(), 12.0);
    QCOMPARE(format2.justificationMode(), Ld::OrderedListParagraphFormat::Justification::CENTER);
    QCOMPARE(format2.firstLineLeftIndentation(), 72.0);
    QCOMPARE(format2.lineSpacing(), 2.5);
    QCOMPARE(format2.listIndentation(), 19.0);
    QCOMPARE(format2.family(), QString("Times Roman"));
    QCOMPARE(format2.fontSize(), 14.0);
    QCOMPARE(format2.fontWeight(), Ld::OrderedListParagraphFormat::Weight::BLACK);
    QCOMPARE(format2.startingNumber(), 2U);
    QCOMPARE(format2.style(), Ld::OrderedListParagraphFormat::Style::ROMAN_LOWER_CASE);

    QSharedPointer<Ld::OrderedListParagraphFormat> format3 =
        format2.clone().dynamicCast<Ld::OrderedListParagraphFormat>();

    QVERIFY(format3->isValid());
    QCOMPARE(format3->leftIndentation(), 72.0);
    QCOMPARE(format3->rightIndentation(), 73.0);
    QCOMPARE(format3->topSpacing(), 6.0);
    QCOMPARE(format3->bottomSpacing(), 12.0);
    QCOMPARE(format3->justificationMode(), Ld::OrderedListParagraphFormat::Justification::CENTER);
    QCOMPARE(format3->firstLineLeftIndentation(), 72.0);
    QCOMPARE(format3->lineSpacing(), 2.5);
    QCOMPARE(format3->listIndentation(), 19.0);
    QCOMPARE(format3->family(), QString("Times Roman"));
    QCOMPARE(format3->fontSize(), 14.0);
    QCOMPARE(format3->fontWeight(), Ld::OrderedListParagraphFormat::Weight::BLACK);
    QCOMPARE(format3->startingNumber(), 2U);
    QCOMPARE(format3->style(), Ld::OrderedListParagraphFormat::Style::ROMAN_LOWER_CASE);
}


void TestOrderedListParagraphFormat::testAccessors() {
    Ld::OrderedListParagraphFormat format;

    format.setLeftIndentation(72.0);
    format.setRightIndentation(73.0);
    format.setTopSpacing(6.0);
    format.setBottomSpacing(12.0);
    format.setJustification(Ld::OrderedListParagraphFormat::Justification::RIGHT);
    format.setFirstLineLeftIndentation(71.0);
    format.setLineSpacing(2.5);
    format.setListIndentation(19.0);
    format.setFamily("Courier");
    format.setFontSize(14.0);
    format.setFontWeight(Ld::OrderedListParagraphFormat::Weight::LIGHT);
    format.setStyle(Ld::OrderedListParagraphFormat::Style::ROMAN_UPPER_CASE);
    format.setStartingNumber(5);

    QCOMPARE(format.leftIndentation(), 72.0);
    QCOMPARE(format.rightIndentation(), 73.0);
    QCOMPARE(format.topSpacing(), 6.0);
    QCOMPARE(format.bottomSpacing(), 12.0);
    QCOMPARE(format.justificationMode(), Ld::OrderedListParagraphFormat::Justification::RIGHT);
    QCOMPARE(format.firstLineLeftIndentation(), 71.0);
    QCOMPARE(format.lineSpacing(), 2.5);
    QCOMPARE(format.listIndentation(), 19.0);
    QCOMPARE(format.family(), QString("Courier"));
    QCOMPARE(format.fontSize(), 14.0);
    QCOMPARE(format.fontWeight(), Ld::OrderedListParagraphFormat::Weight::LIGHT);
    QCOMPARE(format.style(), Ld::OrderedListParagraphFormat::Style::ROMAN_UPPER_CASE);
    QCOMPARE(format.startingNumber(), 5U);
}


void TestOrderedListParagraphFormat::testToStringMethod() {
    Ld::OrderedListParagraphFormat format;

    format.setLeftIndentation(72.0);
    format.setRightIndentation(73.0);
    format.setTopSpacing(6.0);
    format.setBottomSpacing(12.0);
    format.setJustification(Ld::OrderedListParagraphFormat::Justification::RIGHT);
    format.setFirstLineLeftIndentation(71.0);
    format.setLineSpacing(2.5);
    format.setListIndentation(19.0);
    format.setFamily("Courier");
    format.setFontSize(14.0);
    format.setFontWeight(Ld::OrderedListParagraphFormat::Weight::LIGHT);
    format.setStyle(Ld::OrderedListParagraphFormat::Style::UPPER_CASE);
    format.setStartingNumber(4);

    QString description = format.toString();
    QCOMPARE(
        description,
        QString("OrderedListParagraphFormat,72,73,6,12,RIGHT,71,2.5,Courier,14,0.25,,0,-,-,19,UPPER_CASE,4")
    );
}


void TestOrderedListParagraphFormat::testFormatAggregator() {
    Ld::OrderedListParagraphFormat::Aggregation aggregation;

    QSharedPointer<Ld::OrderedListParagraphFormat> format1(new Ld::OrderedListParagraphFormat);
    format1->setLeftIndentation(10);
    format1->setFamily("Helvetica");
    format1->setListIndentation(20);
    format1->setStyle(Ld::OrderedListParagraphFormat::Style::NUMBER);
    format1->setStartingNumber(5);

    QSharedPointer<Ld::OrderedListParagraphFormat> format2(new Ld::OrderedListParagraphFormat);
    format2->setLeftIndentation(11);
    format2->setFamily("Helvetica");
    format2->setListIndentation(30);
    format2->setStyle(Ld::OrderedListParagraphFormat::Style::LOWER_CASE);
    format2->setStartingNumber(5);

    QSharedPointer<Ld::OrderedListParagraphFormat> format3(new Ld::OrderedListParagraphFormat);
    format3->setLeftIndentation(11);
    format3->setFamily("Courier");
    format3->setListIndentation(20);
    format3->setStyle(Ld::OrderedListParagraphFormat::Style::NUMBER);
    format3->setStartingNumber(6);

    bool success = aggregation.addFormat(format1);
    QVERIFY(success);

    success = aggregation.addFormat(format2);
    QVERIFY(success);

    success = aggregation.addFormat(format3);
    QVERIFY(success);

    QCOMPARE(aggregation.leftIndentations().size(), 2);
    QCOMPARE(aggregation.families().size(), 2);
    QCOMPARE(aggregation.listIndentations().size(), 2);
    QCOMPARE(aggregation.styles().size(), 2);

    QVERIFY(aggregation.leftIndentations().contains(10));
    QVERIFY(aggregation.leftIndentations().contains(11));

    QVERIFY(aggregation.families().contains(QString("Helvetica")));
    QVERIFY(aggregation.families().contains(QString("Courier")));

    QVERIFY(aggregation.listIndentations().contains(20));
    QVERIFY(aggregation.listIndentations().contains(30));

    QVERIFY(aggregation.styles().contains(Ld::OrderedListParagraphFormat::Style::NUMBER));
    QVERIFY(aggregation.styles().contains(Ld::OrderedListParagraphFormat::Style::LOWER_CASE));

    QVERIFY(aggregation.startingNumbers().contains(5));
    QVERIFY(aggregation.startingNumbers().contains(6));
}
