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
* This file implements tests of the Ld::UnorderedListParagraphFormat class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QtTest/QtTest>

#include <ld_unordered_list_paragraph_format.h>

#include "test_unordered_list_paragraph_format.h"

TestUnorderedListParagraphFormat::TestUnorderedListParagraphFormat() {}


TestUnorderedListParagraphFormat::~TestUnorderedListParagraphFormat() {}


void TestUnorderedListParagraphFormat::initTestCase() {}


void TestUnorderedListParagraphFormat::testConstructorsAndDestructors() {
    Ld::UnorderedListParagraphFormat format1;
    QVERIFY(format1.isValid());
    QCOMPARE(format1.leftIndentation(), Ld::UnorderedListParagraphFormat::defaultLeftIndentation);
    QCOMPARE(format1.rightIndentation(), Ld::UnorderedListParagraphFormat::defaultRightIndentation);
    QCOMPARE(format1.topSpacing(), Ld::UnorderedListParagraphFormat::defaultTopSpacing);
    QCOMPARE(format1.bottomSpacing(), Ld::UnorderedListParagraphFormat::defaultBottomSpacing);
    QCOMPARE(format1.justificationMode(), Ld::UnorderedListParagraphFormat::defaultJustification);
    QCOMPARE(format1.lineSpacing(), Ld::UnorderedListParagraphFormat::defaultLineSpacing);
    QCOMPARE(format1.listIndentation(), Ld::UnorderedListParagraphFormat::defaultListIndentation);
    QCOMPARE(format1.family(), QString(Ld::UnorderedListParagraphFormat::defaultFontFamily));
    QCOMPARE(format1.fontSize(), Ld::UnorderedListParagraphFormat::defaultFontSizePoints);
    QCOMPARE(format1.fontWeight(), Ld::UnorderedListParagraphFormat::defaultFontWeight);
    QCOMPARE(format1.italics(), false);
    QCOMPARE(format1.underline(), false);
    QCOMPARE(format1.overline(), false);
    QCOMPARE(format1.strikeout(), false);
    QCOMPARE(format1.letterSpacing(), Ld::UnorderedListParagraphFormat::defaultLetterSpacing);
    QCOMPARE(format1.fontColor(), Ld::UnorderedListParagraphFormat::defaultFontColor);
    QCOMPARE(format1.fontBackgroundColor(), Ld::UnorderedListParagraphFormat::defaultFontBackgroundColor);
    QCOMPARE(format1.bulletStyle(), Ld::UnorderedListParagraphFormat::defaultBulletStyle);

    format1.setLeftIndentation(72.0);
    format1.setRightIndentation(73.0);
    format1.setTopSpacing(6.0);
    format1.setBottomSpacing(12.0);
    format1.setJustification(Ld::UnorderedListParagraphFormat::Justification::CENTER);
    format1.clearFirstLineLeftIndentation();
    format1.setLineSpacing(2.5);
    format1.setListIndentation(19.0);
    format1.setFamily("Times Roman");
    format1.setFontSize(14.0);
    format1.setFontWeight(Ld::UnorderedListParagraphFormat::Weight::BLACK);
    format1.setBulletStyle(QString("&"));

    Ld::UnorderedListParagraphFormat format2(format1);
    QVERIFY(format2.isValid());
    QCOMPARE(format2.leftIndentation(), 72.0);
    QCOMPARE(format2.rightIndentation(), 73.0);
    QCOMPARE(format2.topSpacing(), 6.0);
    QCOMPARE(format2.bottomSpacing(), 12.0);
    QCOMPARE(format2.justificationMode(), Ld::UnorderedListParagraphFormat::Justification::CENTER);
    QCOMPARE(format2.firstLineLeftIndentation(), 72.0);
    QCOMPARE(format2.lineSpacing(), 2.5);
    QCOMPARE(format2.listIndentation(), 19.0);
    QCOMPARE(format2.family(), QString("Times Roman"));
    QCOMPARE(format2.fontSize(), 14.0);
    QCOMPARE(format2.fontWeight(), Ld::UnorderedListParagraphFormat::Weight::BLACK);
    QCOMPARE(format2.bulletStyle(), QString("&"));

    QSharedPointer<Ld::UnorderedListParagraphFormat> format3 =
        format2.clone().dynamicCast<Ld::UnorderedListParagraphFormat>();

    QVERIFY(format3->isValid());
    QCOMPARE(format3->leftIndentation(), 72.0);
    QCOMPARE(format3->rightIndentation(), 73.0);
    QCOMPARE(format3->topSpacing(), 6.0);
    QCOMPARE(format3->bottomSpacing(), 12.0);
    QCOMPARE(format3->justificationMode(), Ld::UnorderedListParagraphFormat::Justification::CENTER);
    QCOMPARE(format3->firstLineLeftIndentation(), 72.0);
    QCOMPARE(format3->lineSpacing(), 2.5);
    QCOMPARE(format3->listIndentation(), 19.0);
    QCOMPARE(format3->family(), QString("Times Roman"));
    QCOMPARE(format3->fontSize(), 14.0);
    QCOMPARE(format3->fontWeight(), Ld::UnorderedListParagraphFormat::Weight::BLACK);
    QCOMPARE(format3->bulletStyle(), QString("&"));
}


void TestUnorderedListParagraphFormat::testAccessors() {
    Ld::UnorderedListParagraphFormat format;

    format.setLeftIndentation(72.0);
    format.setRightIndentation(73.0);
    format.setTopSpacing(6.0);
    format.setBottomSpacing(12.0);
    format.setJustification(Ld::UnorderedListParagraphFormat::Justification::RIGHT);
    format.setFirstLineLeftIndentation(71.0);
    format.setLineSpacing(2.5);
    format.setListIndentation(19.0);
    format.setFamily("Courier");
    format.setFontSize(14.0);
    format.setFontWeight(Ld::UnorderedListParagraphFormat::Weight::LIGHT);
    format.setBulletStyle("^");

    QCOMPARE(format.leftIndentation(), 72.0);
    QCOMPARE(format.rightIndentation(), 73.0);
    QCOMPARE(format.topSpacing(), 6.0);
    QCOMPARE(format.bottomSpacing(), 12.0);
    QCOMPARE(format.justificationMode(), Ld::UnorderedListParagraphFormat::Justification::RIGHT);
    QCOMPARE(format.firstLineLeftIndentation(), 71.0);
    QCOMPARE(format.lineSpacing(), 2.5);
    QCOMPARE(format.listIndentation(), 19.0);
    QCOMPARE(format.family(), QString("Courier"));
    QCOMPARE(format.fontSize(), 14.0);
    QCOMPARE(format.fontWeight(), Ld::UnorderedListParagraphFormat::Weight::LIGHT);
    QCOMPARE(format.bulletStyle(), QString("^"));
}


void TestUnorderedListParagraphFormat::testToStringMethod() {
    Ld::UnorderedListParagraphFormat format;

    format.setLeftIndentation(72.0);
    format.setRightIndentation(73.0);
    format.setTopSpacing(6.0);
    format.setBottomSpacing(12.0);
    format.setJustification(Ld::UnorderedListParagraphFormat::Justification::RIGHT);
    format.setFirstLineLeftIndentation(71.0);
    format.setLineSpacing(2.5);
    format.setListIndentation(19.0);
    format.setFamily("Courier");
    format.setFontSize(14.0);
    format.setFontWeight(Ld::UnorderedListParagraphFormat::Weight::LIGHT);
    format.setBulletStyle("!");

    QString description = format.toString();
    QCOMPARE(
        description,
        QString("UnorderedListParagraphFormat,72,73,6,12,RIGHT,71,2.5,Courier,14,0.25,,0,-,-,19,'!'")
    );
}


void TestUnorderedListParagraphFormat::testFormatAggregator() {
    Ld::UnorderedListParagraphFormat::Aggregation aggregation;

    QSharedPointer<Ld::UnorderedListParagraphFormat> format1(new Ld::UnorderedListParagraphFormat);
    format1->setLeftIndentation(10);
    format1->setFamily("Helvetica");
    format1->setListIndentation(20);
    format1->setBulletStyle("@");

    QSharedPointer<Ld::UnorderedListParagraphFormat> format2(new Ld::UnorderedListParagraphFormat);
    format2->setLeftIndentation(11);
    format2->setFamily("Helvetica");
    format2->setListIndentation(30);
    format2->setBulletStyle("#");

    QSharedPointer<Ld::UnorderedListParagraphFormat> format3(new Ld::UnorderedListParagraphFormat);
    format3->setLeftIndentation(11);
    format3->setFamily("Courier");
    format3->setListIndentation(20);
    format3->setBulletStyle("@");

    bool success = aggregation.addFormat(format1);
    QVERIFY(success);

    success = aggregation.addFormat(format2);
    QVERIFY(success);

    success = aggregation.addFormat(format3);
    QVERIFY(success);

    QCOMPARE(aggregation.leftIndentations().size(), 2);
    QCOMPARE(aggregation.families().size(), 2);
    QCOMPARE(aggregation.listIndentations().size(), 2);
    QCOMPARE(aggregation.bulletStyles().size(), 2);

    QVERIFY(aggregation.leftIndentations().contains(10));
    QVERIFY(aggregation.leftIndentations().contains(11));

    QVERIFY(aggregation.families().contains(QString("Helvetica")));
    QVERIFY(aggregation.families().contains(QString("Courier")));

    QVERIFY(aggregation.listIndentations().contains(20));
    QVERIFY(aggregation.listIndentations().contains(30));

    QVERIFY(aggregation.bulletStyles().contains(QString("@")));
    QVERIFY(aggregation.bulletStyles().contains(QString("#")));
}
