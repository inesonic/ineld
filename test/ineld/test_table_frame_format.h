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
* This header provides tests for the Ld::TableFrameFormat class.
***********************************************************************************************************************/

#ifndef TEST_TABLE_FRAME_FORMAT_H
#define TEST_TABLE_FRAME_FORMAT_H

#include <QObject>
#include <QtTest/QtTest>

class TestTableFrameFormat:public QObject {
    Q_OBJECT

    public:
        TestTableFrameFormat();

        ~TestTableFrameFormat() override;

    private slots:
        void initTestCase();

        void testConstructorsAndDestructors();

        void testCommonAccessorsAndFactories();

        void testColumnWidthMethods();

        void testColumnLineMethod();

        void testRowLineMethod();

        void testColorMethod();

        void testColumnRemovedMethod();

        void testRowRemovedMethod();

        void testColumnsInsertedMethod();

        void testRowsInsertedMethod();

        void testMergeCellsMethod();

        void testToStringMethod();

        void testToCssMethod();

        void testToCssEntryMethod();

        // Add tests for the aggregation class.

    private:
        static const unsigned maximumTestColumn = 1000;
        static const unsigned maximumTestRow    = 1000;
};

#endif
