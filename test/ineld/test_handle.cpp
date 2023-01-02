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
* This file implements tests of the Ld::Handle class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QObject>
#include <QSet>
#include <QVector>
#include <QtTest/QtTest>

#include <string>

#include <ld_handle.h>

#include "test_handle.h"

TestHandle::TestHandle() {}


TestHandle::~TestHandle() {}


void TestHandle::initTestCase() {
    Ld::Handle::initialize(0x123456789ABCDEF0ULL);
}


void TestHandle::testCreateFunction() {
    QSet<Ld::Handle> handles;

    for (unsigned i=0 ; i<numberCreateTestIterations ; ++i) {
        Ld::Handle handle(Ld::Handle::create());
        QVERIFY2(!handles.contains(handle),"Duplicate handle detected");

        handles.insert(handle);
    }
}


void TestHandle::testAssignmentAndCopyConstructors() {
    Ld::Handle handle1(Ld::Handle::create());
    Ld::Handle handle2(handle1);
    Ld::Handle handle3;
    Ld::Handle handle4;

    QVERIFY2(handle1 == handle2, "Copy constructor failed or operator== failed.");
    QVERIFY2(handle1 != handle3, "Copy constructor failed or operator!= failed.");
    QVERIFY2(handle1.isValid(), "isValid method failed or copy constructor failed.");
    QVERIFY2(handle3.isInvalid(), "isInvalid method failed or constructor failed.");

    handle3 = handle1;
    QVERIFY2(handle3 == handle1, "Assighment operator failed.");
    QVERIFY2(handle3 != handle4, "Assignment operator failed.");

    handle2 = handle4;
    QVERIFY2(handle2 == handle4, "Assighment operator failed.");
    QVERIFY2(handle2 != handle1, "Assignment operator failed.");
}


void TestHandle::testComparisonOperators() {
    Ld::Handle handle1(Ld::Handle::create());
    Ld::Handle handle2(Ld::Handle::create());
    Ld::Handle handle3;

    handle3 = handle1;

    QVERIFY2(handle1 < handle2, "operator< failed (less than).");
    QVERIFY2((handle2 < handle1) == false, "operator< failed (false condition).");
    QVERIFY2(handle2 > handle1, "operator> failed (greater than).");
    QVERIFY2((handle1 > handle2) == false, "operator> failed (false condition).");

    QVERIFY2(handle1 <= handle2, "operator<= failed (less-than).");
    QVERIFY2((handle2 <= handle1) == false, "operator<= failed (false condition).");
    QVERIFY2(handle1 <= handle3, "operator<= failed (equality).");

    QVERIFY2(handle2 >= handle1, "operator>= failed (greater-than).");
    QVERIFY2((handle1 >= handle2) == false, "operator>= failed (false condition).");
    QVERIFY2(handle1 >= handle3, "operator>= failed (equality).");

    QVERIFY2(handle1 == handle3, "operator== failed (equality).");
    QVERIFY2((handle1 == handle2) == false, "operator== failed (false condition).");

    QVERIFY2(handle1 != handle2, "operator!= failed (inequality).");
    QVERIFY2((handle1 != handle3) == false, "operator!= failed (false condition).");
}
\

void TestHandle::testStringFunctions() {
    Ld::Handle handle1(Ld::Handle::create());

    std::string s = handle1.toString();
    Ld::Handle handle2(Ld::Handle::fromString(s));

    QVERIFY2(handle1 == handle2, "String functions failed");
}


void TestHandle::testHash() {
    QVector<double> points;
    unsigned long    i;
    unsigned long    j;
    for (i=0 ; i<numberHashTestIterations ; ++i) {
        Ld::Handle handle(Ld::Handle::create());
        points.append((1.0 * handle.hash())/static_cast<quint32>(-1) - 0.5);
    }

    QVector<double> autocorrelation;
    for (i=0 ; i<numberHashTestIterations ; ++i) {
        double x = 0;
        for (j=0 ; j<numberHashTestIterations ; ++j) {
            unsigned long k = (j-i) % numberHashTestIterations;
            x += points[j] * points[k];
        }

        autocorrelation.append(x / numberHashTestIterations);
    }

    double averageCorrelation = 0;
    double maximumCorrelation = 0;

    // We'll always have correlation at 0 (point will correlate with itself).
    for (i=1 ; i<numberHashTestIterations ; ++i) {
        averageCorrelation += autocorrelation[i];
        if (autocorrelation[i] > maximumCorrelation) {
            maximumCorrelation = autocorrelation[i];
        }
    }

    averageCorrelation /= numberHashTestIterations;

    QVERIFY2(averageCorrelation < correlationThreshold, "average correlation exceeded threshold.");
    QVERIFY2(maximumCorrelation < maximumAllowedCorrelation, "maximum correlation exceeded threshold.");
}
