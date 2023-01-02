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
* This file implements tests of the Ld::TableLineSettings class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QtTest/QtTest>

#include <ld_table_line_settings.h>

#include "test_table_line_settings.h"

TestTableLineSettings::TestTableLineSettings() {}


TestTableLineSettings::~TestTableLineSettings() {}


void TestTableLineSettings::initTestCase() {}


void TestTableLineSettings::testConstructors() {
    Ld::TableLineSettings lineSettings1;
    QVERIFY(lineSettings1.isInvalid());

    Ld::TableLineSettings lineSettings2(Ld::TableLineSettings::Style::SINGLE, 1);
    QVERIFY(lineSettings2.isValid());
    QCOMPARE(lineSettings2.lineStyle(), Ld::TableLineSettings::Style::SINGLE);
    QCOMPARE(lineSettings2.width(), 1);

    Ld::TableLineSettings lineSettings3(lineSettings2);
    QVERIFY(lineSettings3.isValid());
    QCOMPARE(lineSettings3.lineStyle(), Ld::TableLineSettings::Style::SINGLE);
    QCOMPARE(lineSettings3.width(), 1);
}


void TestTableLineSettings::testAccessors() {
    Ld::TableLineSettings lineSettings;
    QCOMPARE(lineSettings.isValid(), false);
    QCOMPARE(lineSettings.isInvalid(), true);

    lineSettings.setLineStyle(Ld::TableLineSettings::Style::SINGLE);
    lineSettings.setWidth(1.0);

    QCOMPARE(lineSettings.lineStyle(), Ld::TableLineSettings::Style::SINGLE);
    QCOMPARE(lineSettings.width(), 1.0);

    bool success = lineSettings.setLine(Ld::TableLineSettings::Style::DOUBLE, 0);
    QVERIFY(!success);

    success = lineSettings.setLine(Ld::TableLineSettings::Style::INVALID, 1);
    QVERIFY(!success);

    success = lineSettings.setLine(Ld::TableLineSettings::Style::SINGLE, 0);
    QVERIFY(!success);

    success = lineSettings.setLine(Ld::TableLineSettings::Style::DOUBLE, 0.5);
    QVERIFY(success);

    QCOMPARE(lineSettings.lineStyle(), Ld::TableLineSettings::Style::DOUBLE);
    QCOMPARE(lineSettings.width(), 0.5);
}


void TestTableLineSettings::testAssignmentOperator() {
    Ld::TableLineSettings lineSettings1;
    QVERIFY(lineSettings1.isInvalid());

    Ld::TableLineSettings lineSettings2(Ld::TableLineSettings::Style::SINGLE, 1);
    QVERIFY(lineSettings2.isValid());
    QCOMPARE(lineSettings2.lineStyle(), Ld::TableLineSettings::Style::SINGLE);
    QCOMPARE(lineSettings2.width(), 1);

    lineSettings1 = lineSettings2;

    QVERIFY(lineSettings1.isValid());
    QCOMPARE(lineSettings1.lineStyle(), Ld::TableLineSettings::Style::SINGLE);
    QCOMPARE(lineSettings1.width(), 1);

    lineSettings2.setLine(Ld::TableLineSettings::Style::DOUBLE, 0.5);

    QVERIFY(lineSettings2.isValid());
    QCOMPARE(lineSettings2.lineStyle(), Ld::TableLineSettings::Style::DOUBLE);
    QCOMPARE(lineSettings2.width(), 0.5);

    QVERIFY(lineSettings1.isValid());
    QCOMPARE(lineSettings1.lineStyle(), Ld::TableLineSettings::Style::SINGLE);
    QCOMPARE(lineSettings1.width(), 1);
}
