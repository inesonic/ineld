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
* This file implements tests of the Ld::TableColumnWidth class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QtTest/QtTest>

#include <ld_table_column_width.h>

#include "test_table_column_width.h"

TestTableColumnWidth::TestTableColumnWidth() {}


TestTableColumnWidth::~TestTableColumnWidth() {}


void TestTableColumnWidth::initTestCase() {}


void TestTableColumnWidth::testConstructors() {
    Ld::TableColumnWidth columnWidth1;
    QVERIFY(columnWidth1.isInvalid());

    Ld::TableColumnWidth columnWidth2(Ld::TableColumnWidth::WidthType::FIXED, 1);
    QVERIFY(columnWidth2.isValid());
    QCOMPARE(columnWidth2.widthType(), Ld::TableColumnWidth::WidthType::FIXED);
    QCOMPARE(columnWidth2.width(), 1);

    Ld::TableColumnWidth columnWidth3(columnWidth2);
    QVERIFY(columnWidth3.isValid());
    QCOMPARE(columnWidth3.widthType(), Ld::TableColumnWidth::WidthType::FIXED);
    QCOMPARE(columnWidth3.width(), 1);
}


void TestTableColumnWidth::testAccessors() {
    Ld::TableColumnWidth columnWidth;
    QCOMPARE(columnWidth.isValid(), false);
    QCOMPARE(columnWidth.isInvalid(), true);

    columnWidth.setWidthType(Ld::TableColumnWidth::WidthType::FIXED);
    columnWidth.setWidth(1.0);

    QCOMPARE(columnWidth.widthType(), Ld::TableColumnWidth::WidthType::FIXED);
    QCOMPARE(columnWidth.width(), 1.0);

    bool success = columnWidth.setWidth(Ld::TableColumnWidth::WidthType::PROPORTIONAL, 2.0);
    QVERIFY(!success);

    success = columnWidth.setWidth(Ld::TableColumnWidth::WidthType::PROPORTIONAL, 0);
    QVERIFY(!success);

    success = columnWidth.setWidth(Ld::TableColumnWidth::WidthType::FIXED, 0);
    QVERIFY(!success);

    success = columnWidth.setWidth(Ld::TableColumnWidth::WidthType::PROPORTIONAL, 0.5);
    QVERIFY(success);

    QCOMPARE(columnWidth.widthType(), Ld::TableColumnWidth::WidthType::PROPORTIONAL);
    QCOMPARE(columnWidth.width(), 0.5);
}


void TestTableColumnWidth::testAssignmentOperator() {
    Ld::TableColumnWidth columnWidth1;
    QVERIFY(columnWidth1.isInvalid());

    Ld::TableColumnWidth columnWidth2(Ld::TableColumnWidth::WidthType::FIXED, 1);
    QVERIFY(columnWidth2.isValid());
    QCOMPARE(columnWidth2.widthType(), Ld::TableColumnWidth::WidthType::FIXED);
    QCOMPARE(columnWidth2.width(), 1);

    columnWidth1 = columnWidth2;

    QVERIFY(columnWidth1.isValid());
    QCOMPARE(columnWidth1.widthType(), Ld::TableColumnWidth::WidthType::FIXED);
    QCOMPARE(columnWidth1.width(), 1);

    columnWidth2.setWidth(Ld::TableColumnWidth::WidthType::PROPORTIONAL, 0.5);

    QVERIFY(columnWidth2.isValid());
    QCOMPARE(columnWidth2.widthType(), Ld::TableColumnWidth::WidthType::PROPORTIONAL);
    QCOMPARE(columnWidth2.width(), 0.5);

    QVERIFY(columnWidth1.isValid());
    QCOMPARE(columnWidth1.widthType(), Ld::TableColumnWidth::WidthType::FIXED);
    QCOMPARE(columnWidth1.width(), 1);
}
