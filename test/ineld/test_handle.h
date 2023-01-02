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
* This header provides tests for the Ld::Handle class.  Also tests core functions of the
* Ld::Element class.
***********************************************************************************************************************/

#ifndef TEST_HANDLE_H
#define TEST_HANDLE_H

#include <QObject>
#include <QtTest/QtTest>

class TestHandle:public QObject {
    Q_OBJECT

    public:
        TestHandle();

        ~TestHandle() override;

    private slots:
        void initTestCase();

        void testCreateFunction();

        void testAssignmentAndCopyConstructors();

        void testComparisonOperators();

        void testStringFunctions();

        void testHash();

    private:
        /**
         * Number of test iterations to perform to confirm that all element handles are, in fact, unique.
         */
        static constexpr unsigned long numberCreateTestIterations = 1000000;

        /**
         * Number of test iterations to perform on the hash method.
         */
        static constexpr unsigned long numberHashTestIterations = 5000;

        /**
         * The expected maximum average correlation across all the points.
         */
        static constexpr double correlationThreshold = 1.0E-4;

        /**
         * The worst case correlation we expect across all the points.
         */
        static constexpr double maximumAllowedCorrelation = 0.03;
};

#endif
