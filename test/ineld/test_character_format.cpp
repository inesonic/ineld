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
* This file implements tests of the Ld::CharacterFormat class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QGuiApplication> // QFont support requires a QGuiApplication instance to be instantiated.
#include <QFont>
#include <QString>
#include <QColor>
#include <QtTest/QtTest>

#include <cmath>

#include <cstring>
#include <cstdlib>

#include <ld_format_structures.h>
#include <ld_character_format.h>

#include "test_character_format.h"

TestCharacterFormat::TestCharacterFormat() {}


TestCharacterFormat::~TestCharacterFormat() {}


void TestCharacterFormat::initTestCase() {
}


void TestCharacterFormat::testConstructorsAndDestructors() {
    Ld::CharacterFormat font1;

    QCOMPARE(font1.family(), QString(Ld::CharacterFormat::defaultFontFamily));
    QCOMPARE(font1.fontSize(), Ld::CharacterFormat::defaultFontSizePoints);
    QCOMPARE(font1.fontWeight(), Ld::CharacterFormat::defaultFontWeight);
    QCOMPARE(font1.position(), Ld::CharacterFormat::Position::NORMAL);
    QCOMPARE(font1.italics(), false);
    QCOMPARE(font1.underline(), false);
    QCOMPARE(font1.overline(), false);
    QCOMPARE(font1.strikeout(), false);
    QCOMPARE(font1.letterSpacing(), Ld::CharacterFormat::defaultLetterSpacing);
    QCOMPARE(font1.fontColor(), Ld::CharacterFormat::defaultFontColor);
    QCOMPARE(font1.fontBackgroundColor(), Ld::CharacterFormat::defaultFontBackgroundColor);

    Ld::CharacterFormat font2(QString("Times Roman"), 26, Ld::CharacterFormat::Weight::BLACK, true);

    QCOMPARE(font2.family(), QString("Times Roman"));
    QCOMPARE(font2.fontSize(), static_cast<unsigned>(26));
    QCOMPARE(font2.fontWeight(), Ld::CharacterFormat::Weight::BLACK);
    QCOMPARE(font2.position(), Ld::CharacterFormat::Position::NORMAL);
    QCOMPARE(font2.italics(), true);
    QCOMPARE(font2.underline(), false);
    QCOMPARE(font2.overline(), false);
    QCOMPARE(font2.strikeout(), false);
    QCOMPARE(font2.letterSpacing(), Ld::CharacterFormat::defaultLetterSpacing);
    QCOMPARE(font2.fontColor(), Ld::CharacterFormat::defaultFontColor);
    QCOMPARE(font2.fontBackgroundColor(), Ld::CharacterFormat::defaultFontBackgroundColor);

    Ld::CharacterFormat font3("Courier", 24, Ld::CharacterFormat::Weight::LIGHT, true);

    QCOMPARE(font3.family(), QString("Courier"));
    QCOMPARE(font3.fontSize(), static_cast<unsigned>(24));
    QCOMPARE(font3.fontWeight(), Ld::CharacterFormat::Weight::LIGHT);
    QCOMPARE(font3.position(), Ld::CharacterFormat::Position::NORMAL);
    QCOMPARE(font3.italics(), true);
    QCOMPARE(font3.underline(), false);
    QCOMPARE(font3.overline(), false);
    QCOMPARE(font3.strikeout(), false);
    QCOMPARE(font3.letterSpacing(), Ld::CharacterFormat::defaultLetterSpacing);
    QCOMPARE(font3.fontColor(), Ld::CharacterFormat::defaultFontColor);
    QCOMPARE(font3.fontBackgroundColor(), Ld::CharacterFormat::defaultFontBackgroundColor);

    Ld::CharacterFormat font4(font3);

    QCOMPARE(font4.family(), QString("Courier"));
    QCOMPARE(font4.fontSize(), static_cast<unsigned>(24));
    QCOMPARE(font4.fontWeight(), Ld::CharacterFormat::Weight::LIGHT);
    QCOMPARE(font4.position(), Ld::CharacterFormat::Position::NORMAL);
    QCOMPARE(font4.italics(), true);
    QCOMPARE(font4.underline(), false);
    QCOMPARE(font4.overline(), false);
    QCOMPARE(font4.strikeout(), false);
    QCOMPARE(font4.letterSpacing(), Ld::CharacterFormat::defaultLetterSpacing);
    QCOMPARE(font4.fontColor(), Ld::CharacterFormat::defaultFontColor);
    QCOMPARE(font4.fontBackgroundColor(), Ld::CharacterFormat::defaultFontBackgroundColor);
}


void TestCharacterFormat::testAccessors() {
    Ld::CharacterFormat font;

    QCOMPARE(font.family(), QString(Ld::CharacterFormat::defaultFontFamily));
    QCOMPARE(font.fontSize(), Ld::CharacterFormat::defaultFontSizePoints);
    QCOMPARE(font.fontWeight(), Ld::CharacterFormat::defaultFontWeight);
    QCOMPARE(font.position(), Ld::CharacterFormat::Position::NORMAL);
    QCOMPARE(font.italics(), false);
    QCOMPARE(font.underline(), false);
    QCOMPARE(font.overline(), false);
    QCOMPARE(font.strikeout(), false);
    QCOMPARE(font.letterSpacing(), Ld::CharacterFormat::defaultLetterSpacing);
    QCOMPARE(font.fontColor(), Ld::CharacterFormat::defaultFontColor);
    QCOMPARE(font.fontBackgroundColor(), Ld::CharacterFormat::defaultFontBackgroundColor);

    font.setFamily("Times Roman");
    font.setFontSize(26);
    font.setFontWeight(Ld::CharacterFormat::Weight::BOLD);
    font.setPosition(Ld::CharacterFormat::Position::SUBSCRIPT);
    font.setFontColor(QColor(255,0,0));
    font.setFontBackgroundColor(QColor(255,255,0));

    QCOMPARE(font.family(), QString("Times Roman"));
    QCOMPARE(font.fontSize(), static_cast<unsigned>(26));
    QCOMPARE(font.fontWeight(), Ld::CharacterFormat::Weight::BOLD);
    QCOMPARE(font.position(), Ld::CharacterFormat::Position::SUBSCRIPT);
    QCOMPARE(font.fontColor(), QColor(255,0,0));
    QCOMPARE(font.fontBackgroundColor(), QColor(255,255,0));

    font.setFamily(QString("Courier"));
    QCOMPARE(font.family(), QString("Courier"));

    font.setItalics(true);

    QCOMPARE(font.italics(), true);
    QCOMPARE(font.underline(), false);
    QCOMPARE(font.overline(), false);
    QCOMPARE(font.strikeout(), false);

    font.setItalics(false);
    font.setUnderline(true);

    QCOMPARE(font.italics(), false);
    QCOMPARE(font.underline(), true);
    QCOMPARE(font.overline(), false);
    QCOMPARE(font.strikeout(), false);

    font.setUnderline(false);
    font.setOverline(true);

    QCOMPARE(font.italics(), false);
    QCOMPARE(font.underline(), false);
    QCOMPARE(font.overline(), true);
    QCOMPARE(font.strikeout(), false);

    font.setOverline(false);
    font.setStrikeout(true);

    QCOMPARE(font.italics(), false);
    QCOMPARE(font.underline(), false);
    QCOMPARE(font.overline(), false);
    QCOMPARE(font.strikeout(), true);

    font.setStrikeout(false);

    QCOMPARE(font.italics(), false);
    QCOMPARE(font.underline(), false);
    QCOMPARE(font.overline(), false);
    QCOMPARE(font.strikeout(), false);

    font.setLetterSpacing(0.1F);
    QVERIFY(fabs(font.letterSpacing() - 0.1) < 0.1 * 1E-7); // Allow for a little error. in the stored value.
}


void TestCharacterFormat::testQFontSupport() {
    Ld::CharacterFormat font;
    QFont          qFont = font.toQFont();

    QVERIFY(qFont.family() == font.family());
    QVERIFY(qFont.pointSize() == static_cast<int>(font.fontSize()));
    QVERIFY(qFont.weight() == static_cast<int>(font.fontWeight()));
    QVERIFY(qFont.italic() == font.italics());
    QVERIFY(qFont.underline() == font.underline());
    QVERIFY(qFont.overline() == font.overline());
    QVERIFY(qFont.strikeOut() == font.strikeout());
    QVERIFY(qFont.letterSpacing() == (Ld::CharacterFormat::defaultLetterSpacing + 1.0) * 100.0);

    font.setFamily("Times Roman");
    qFont = font.toQFont();

    QVERIFY(qFont.family() == font.family());
    QVERIFY(qFont.pointSize() == static_cast<int>(font.fontSize()));
    QVERIFY(qFont.weight() == static_cast<int>(font.fontWeight()));
    QVERIFY(qFont.italic() == font.italics());
    QVERIFY(qFont.underline() == font.underline());
    QVERIFY(qFont.overline() == font.overline());
    QVERIFY(qFont.strikeOut() == font.strikeout());
    QVERIFY(qFont.letterSpacing() == (Ld::CharacterFormat::defaultLetterSpacing + 1.0) * 100.0);

    font.setFontSize(font.fontSize() + 2);
    qFont = font.toQFont();

    QVERIFY(qFont.family() == font.family());
    QVERIFY(qFont.pointSize() == static_cast<int>(font.fontSize()));
    QVERIFY(qFont.weight() == static_cast<int>(font.fontWeight()));
    QVERIFY(qFont.italic() == font.italics());
    QVERIFY(qFont.underline() == font.underline());
    QVERIFY(qFont.overline() == font.overline());
    QVERIFY(qFont.strikeOut() == font.strikeout());
    QVERIFY(qFont.letterSpacing() == (Ld::CharacterFormat::defaultLetterSpacing + 1.0) * 100.0);

    font.setFontWeight(Ld::CharacterFormat::Weight::BLACK);
    qFont = font.toQFont();

    QVERIFY(qFont.family() == font.family());
    QVERIFY(qFont.pointSize() == static_cast<int>(font.fontSize()));
    QVERIFY(qFont.weight() == static_cast<int>(font.fontWeight()));
    QVERIFY(qFont.italic() == font.italics());
    QVERIFY(qFont.underline() == font.underline());
    QVERIFY(qFont.overline() == font.overline());
    QVERIFY(qFont.strikeOut() == font.strikeout());
    QVERIFY(qFont.letterSpacing() == (Ld::CharacterFormat::defaultLetterSpacing + 1.0) * 100.0);

    font.setItalics(true);
    qFont = font.toQFont();

    QVERIFY(qFont.family() == font.family());
    QVERIFY(qFont.pointSize() == static_cast<int>(font.fontSize()));
    QVERIFY(qFont.weight() == static_cast<int>(font.fontWeight()));
    QVERIFY(qFont.italic() == font.italics());
    QVERIFY(qFont.underline() == font.underline());
    QVERIFY(qFont.overline() == font.overline());
    QVERIFY(qFont.strikeOut() == font.strikeout());
    QVERIFY(qFont.letterSpacing() == (Ld::CharacterFormat::defaultLetterSpacing + 1.0) * 100.0);

    font.setUnderline(true);
    qFont = font.toQFont();

    QVERIFY(qFont.family() == font.family());
    QVERIFY(qFont.pointSize() == static_cast<int>(font.fontSize()));
    QVERIFY(qFont.weight() == static_cast<int>(font.fontWeight()));
    QVERIFY(qFont.italic() == font.italics());
    QVERIFY(qFont.underline() == font.underline());
    QVERIFY(qFont.overline() == font.overline());
    QVERIFY(qFont.strikeOut() == font.strikeout());
    QVERIFY(qFont.letterSpacing() == (Ld::CharacterFormat::defaultLetterSpacing + 1.0) * 100.0);

    font.setOverline(true);
    qFont = font.toQFont();

    QVERIFY(qFont.family() == font.family());
    QVERIFY(qFont.pointSize() == static_cast<int>(font.fontSize()));
    QVERIFY(qFont.weight() == static_cast<int>(font.fontWeight()));
    QVERIFY(qFont.italic() == font.italics());
    QVERIFY(qFont.underline() == font.underline());
    QVERIFY(qFont.overline() == font.overline());
    QVERIFY(qFont.strikeOut() == font.strikeout());
    QVERIFY(qFont.letterSpacing() == (Ld::CharacterFormat::defaultLetterSpacing + 1.0) * 100.0);

    font.setStrikeout(true);
    qFont = font.toQFont();

    QVERIFY(qFont.family() == font.family());
    QVERIFY(qFont.pointSize() == static_cast<int>(font.fontSize()));
    QVERIFY(qFont.weight() == static_cast<int>(font.fontWeight()));
    QVERIFY(qFont.italic() == font.italics());
    QVERIFY(qFont.underline() == font.underline());
    QVERIFY(qFont.overline() == font.overline());
    QVERIFY(qFont.strikeOut() == font.strikeout());
    QVERIFY(qFont.letterSpacing() == (Ld::CharacterFormat::defaultLetterSpacing + 1.0) * 100.0);

    float newLetterSpacing = 0.1 + Ld::CharacterFormat::defaultLetterSpacing;
    font.setLetterSpacing(newLetterSpacing);
    qFont = font.toQFont();

    QVERIFY(qFont.family() == font.family());
    QVERIFY(qFont.pointSize() == static_cast<int>(font.fontSize()));
    QVERIFY(qFont.weight() == static_cast<int>(font.fontWeight()));
    QVERIFY(qFont.italic() == font.italics());
    QVERIFY(qFont.underline() == font.underline());
    QVERIFY(qFont.overline() == font.overline());
    QVERIFY(qFont.strikeOut() == font.strikeout());
    QVERIFY(fabs(qFont.letterSpacing() - (1.0 + newLetterSpacing) * 100.0) < (1.0 + newLetterSpacing) * 100.0 * 1.0E-7);
}


void TestCharacterFormat::testToStringMethod() {
    Ld::CharacterFormat font;

    font.setFamily("Courier");
    font.setFontSize(12.0);
    font.setFontWeight(Ld::CharacterFormat::Weight::BLACK);
    font.setPosition(Ld::CharacterFormat::Position::SUBSCRIPT);
    font.setItalics(true);
    font.setUnderline(true);
    font.setOverline(true);
    font.setStrikeout(true);
    font.setLetterSpacing(0.1F);
    font.setFontColor(QColor(0, 255, 0, 255));
    font.setFontBackgroundColor(QColor(255, 0, 0, 0)); // Default background is 255, 255, 255, 0

    QString description = font.toString();
    QCOMPARE(description, QString("CharacterFormat,Courier,12,0.87,IUOS,0.1,#00ff00,#ff0000,SUBSCRIPT"));
}


void TestCharacterFormat::testFormatAggregator() {
    QSharedPointer<Ld::CharacterFormat> format1(new Ld::CharacterFormat("Courier", 10));
    format1->setItalics(true);
    format1->setUnderline(true);
    format1->setOverline(false);
    format1->setStrikeout(false);
    format1->setLetterSpacing(0.2F);
    format1->setFontColor(QColor(Qt::black));
    format1->setFontBackgroundColor(QColor(Qt::red));

    Ld::FormatPointerSet formats;

    Ld::CharacterFormat::Aggregation aggregation;
    {
        bool success = aggregation.addFormat(format1);
        QVERIFY(success);

        QSharedPointer<Ld::CharacterFormat> format2(new Ld::CharacterFormat("Helvetica", 12));
        format2->setItalics(false);
        format2->setUnderline(true);
        format2->setOverline(true);
        format2->setStrikeout(false);
        format2->setLetterSpacing(0);
        format2->setFontColor(QColor(Qt::black));
        format2->setFontBackgroundColor(QColor(Qt::red));

        success = aggregation.addFormat(format2);
        QVERIFY(success);

        QSharedPointer<Ld::CharacterFormat> format3(new Ld::CharacterFormat("Helvetica", 10));
        format3->setItalics(false);
        format3->setUnderline(true);
        format3->setOverline(true);
        format3->setStrikeout(false);
        format3->setLetterSpacing(0.2F);
        format3->setFontColor(QColor(Qt::blue));
        format3->setFontBackgroundColor(QColor(Qt::yellow));

        success = aggregation.addFormat(format3);
        QVERIFY(success);

        formats = aggregation.formats();
        QCOMPARE(formats.size(), 3);
        QVERIFY(formats.contains(format1));
        QVERIFY(formats.contains(format2));
        QVERIFY(formats.contains(format3));

        QCOMPARE(aggregation.families().size(), 2);
        QVERIFY(aggregation.families().contains(QString("Courier")));
        QVERIFY(aggregation.families().contains(QString("Helvetica")));

        QCOMPARE(aggregation.sizes().size(), 2);
        QVERIFY(aggregation.sizes().contains(10));
        QVERIFY(aggregation.sizes().contains(12));

        QVERIFY(aggregation.italics().bothTrueAndFalse());
        QVERIFY(aggregation.underline().allTrue());
        QVERIFY(aggregation.overline().bothTrueAndFalse());
        QVERIFY(aggregation.strikeOut().allFalse());

        QCOMPARE(aggregation.letterSpacing().size(), 2);
        QVERIFY(aggregation.letterSpacing().contains(0.2F));
        QVERIFY(aggregation.letterSpacing().contains(0));

        QCOMPARE(aggregation.fontColor().size(), 2);
        QVERIFY(aggregation.fontColor().contains(QColor(Qt::black)));
        QVERIFY(aggregation.fontColor().contains(QColor(Qt::blue)));

        QCOMPARE(aggregation.fontBackgroundColor().size(), 2);
        QVERIFY(aggregation.fontBackgroundColor().contains(QColor(Qt::red)));
        QVERIFY(aggregation.fontBackgroundColor().contains(QColor(Qt::yellow)));

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
