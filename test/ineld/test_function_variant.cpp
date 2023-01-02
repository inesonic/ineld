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
* This file implements tests for the \ref Ld::FunctionVariant class.  Also provides some testing of the
* \ref Ld::Format class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QSharedPointer>

#include <cassert>

#include <ld_handle.h>
#include <ld_data_type.h>
#include <ld_capabilities.h>
#include <ld_function_variant.h>

#include "test_function_variant.h"


TestFunctionVariant::TestFunctionVariant() {}


TestFunctionVariant::~TestFunctionVariant() {}


void TestFunctionVariant::initTestCase() {
    Ld::Handle::initialize(0x12345678ABCDEF0);
}


void TestFunctionVariant::testConstructorsAndDestructors() {
    Ld::FunctionVariant v1;
    QCOMPARE(v1.isValid(), false);

    Ld::FunctionVariant v2(
        Ld::DataType::ValueType::INTEGER,
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::REAL << Ld::DataType::ValueType::COMPLEX,
        QList<QString>() << "real" << "complex"
    );

    QCOMPARE(v2.isValid(), true);
    QCOMPARE(v2.returnValueType(), Ld::DataType::ValueType::INTEGER);
    QCOMPARE(
        v2.parameterTypes(),
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::REAL << Ld::DataType::ValueType::COMPLEX
    );
    QCOMPARE(
        v2.parameterDescriptions(),
        QList<QString>() << "real" << "complex"
    );

    Ld::FunctionVariant v3(v2);

    QCOMPARE(v3.isValid(), true);
    QCOMPARE(v3.returnValueType(), Ld::DataType::ValueType::INTEGER);
    QCOMPARE(
        v3.parameterTypes(),
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::REAL << Ld::DataType::ValueType::COMPLEX
    );
    QCOMPARE(
        v3.parameterDescriptions(),
        QList<QString>() << "real" << "complex"
    );

    Ld::FunctionVariant v4(
        Ld::DataType::ValueType::COMPLEX,
        Ld::DataType::ValueType::MATRIX_REAL, QString("parameter 1"),
        Ld::DataType::ValueType::REAL,        QString("parameter 2"),
        Ld::DataType::ValueType::INTEGER,     QString("parameter 3")
    );

    QCOMPARE(v4.isValid(), true);
    QCOMPARE(v4.returnValueType(), Ld::DataType::ValueType::COMPLEX);
    QCOMPARE(
        v4.parameterTypes(),
        QList<Ld::DataType::ValueType>()
            << Ld::DataType::ValueType::MATRIX_REAL
            << Ld::DataType::ValueType::REAL
            << Ld::DataType::ValueType::INTEGER
    );
    QCOMPARE(
        v4.parameterDescriptions(),
        QList<QString>() << QString("parameter 1") << QString("parameter 2") << QString("parameter 3")
    );

    // Code below is used to confirm that the internal data sharing functions work.

    Ld::FunctionVariant v5(
        Ld::DataType::ValueType::INTEGER,
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::REAL << Ld::DataType::ValueType::COMPLEX,
        QList<QString>() << "real" << "complex"
    );

    QCOMPARE(v5.isValid(), true);
    QCOMPARE(v5.returnValueType(), Ld::DataType::ValueType::INTEGER);
    QCOMPARE(
        v5.parameterTypes(),
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::REAL << Ld::DataType::ValueType::COMPLEX
    );
    QCOMPARE(
        v5.parameterDescriptions(),
        QList<QString>() << "real" << "complex"
    );
}


void TestFunctionVariant::testValidInvalidMethods() {
    Ld::FunctionVariant v1;
    QCOMPARE(v1.isValid(), false);
    QCOMPARE(v1.isInvalid(), true);

    Ld::FunctionVariant v2(
        Ld::DataType::ValueType::INTEGER,
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::REAL << Ld::DataType::ValueType::COMPLEX,
        QList<QString>() << "real" << "complex"
    );

    QCOMPARE(v2.isValid(), true);
    QCOMPARE(v2.isInvalid(), false);
}


void TestFunctionVariant::testAccessorMethods() {
    Ld::FunctionVariant v1(
        Ld::DataType::ValueType::REAL,
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::INTEGER << Ld::DataType::ValueType::BOOLEAN,
        QList<QString>() << "integer" << "boolean"
    );

    QCOMPARE(v1.returnValueType(), Ld::DataType::ValueType::REAL);
    QCOMPARE(
        v1.parameterTypes(),
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::INTEGER << Ld::DataType::ValueType::BOOLEAN
    );
    QCOMPARE(
        v1.parameterDescriptions(),
        QList<QString>() << "integer" << "boolean"
    );
    QCOMPARE(v1.variantCapabilities(), Ld::Capabilities::allRealRValues);
    QCOMPARE(
        v1.parameterCapabilities(),
        QList<Ld::Capabilities>() << Ld::Capabilities::allIntegerRValues << Ld::Capabilities::allBooleanRValues
    );
}


void TestFunctionVariant::testAssignmentOperator() {
    Ld::FunctionVariant v1;
    QCOMPARE(v1.isValid(), false);

    Ld::FunctionVariant v2(
        Ld::DataType::ValueType::INTEGER,
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::REAL << Ld::DataType::ValueType::COMPLEX,
        QList<QString>() << "real" << "complex"
    );

    QCOMPARE(v2.isValid(), true);
    QCOMPARE(v2.returnValueType(), Ld::DataType::ValueType::INTEGER);
    QCOMPARE(
        v2.parameterTypes(),
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::REAL << Ld::DataType::ValueType::COMPLEX
    );
    QCOMPARE(
        v2.parameterDescriptions(),
        QList<QString>() << "real" << "complex"
    );

    v1 = v2;

    QCOMPARE(v1.isValid(), true);
    QCOMPARE(v1.returnValueType(), Ld::DataType::ValueType::INTEGER);
    QCOMPARE(
        v1.parameterTypes(),
        QList<Ld::DataType::ValueType>() << Ld::DataType::ValueType::REAL << Ld::DataType::ValueType::COMPLEX
    );
    QCOMPARE(
        v1.parameterDescriptions(),
        QList<QString>() << "real" << "complex"
    );
}


void TestFunctionVariant::testComparisonMethods() {
    Ld::FunctionVariant::clearDatabase();

    Ld::FunctionVariant v1(
        Ld::DataType::ValueType::INTEGER,
        QList<Ld::DataType::ValueType>()
            << Ld::DataType::ValueType::INTEGER
            << Ld::DataType::ValueType::REAL,
        QList<QString>() << "p1" << "p2"
    );

    Ld::FunctionVariant v2(
        Ld::DataType::ValueType::INTEGER,
        QList<Ld::DataType::ValueType>()
            << Ld::DataType::ValueType::INTEGER
            << Ld::DataType::ValueType::REAL,
        QList<QString>() << "p1" << "p2"
    );

    Ld::FunctionVariant v3(
        Ld::DataType::ValueType::REAL,
        QList<Ld::DataType::ValueType>()
            << Ld::DataType::ValueType::INTEGER
            << Ld::DataType::ValueType::REAL,
        QList<QString>() << "p1" << "p2"
    );

    Ld::FunctionVariant v4(
        Ld::DataType::ValueType::REAL,
        QList<Ld::DataType::ValueType>()
            << Ld::DataType::ValueType::INTEGER
            << Ld::DataType::ValueType::COMPLEX,
        QList<QString>() << "p1" << "p2"
    );

    Ld::FunctionVariant v5(
        Ld::DataType::ValueType::REAL,
        QList<Ld::DataType::ValueType>()
            << Ld::DataType::ValueType::INTEGER
            << Ld::DataType::ValueType::COMPLEX
            << Ld::DataType::ValueType::BOOLEAN,
        QList<QString>() << "p1" << "p2" << "p3"
    );

    Ld::FunctionVariant v6(
        Ld::DataType::ValueType::REAL,
        QList<Ld::DataType::ValueType>()
            << Ld::DataType::ValueType::INTEGER
            << Ld::DataType::ValueType::COMPLEX
            << Ld::DataType::ValueType::BOOLEAN,
        QList<QString>() << "p1" << "p2" << "p4"
    );

    QCOMPARE(v1 == v2, true);
    QCOMPARE(v1 != v2, false);
    QCOMPARE(v1 < v2, false);
    QCOMPARE(v1 > v2, false);
    QCOMPARE(v1 <= v2, true);
    QCOMPARE(v1 >= v2, true);

    QList<Ld::FunctionVariant> variants;
    variants << v2 << v3 << v4 << v5 << v6;

    for (unsigned i1=0 ; i1<5 ; ++i1) {
        const Ld::FunctionVariant& m1 = variants.at(i1);
        for (unsigned i2=0 ; i2<5 ; ++i2) {
            const Ld::FunctionVariant& m2 = variants.at(i2);

            if (i1 < i2) {
                QCOMPARE(m1 == m2, false);
                QCOMPARE(m1 != m2, true);
                QCOMPARE(m1 < m2, true);
                QCOMPARE(m1 > m2, false);
                QCOMPARE(m1 <= m2, true);
                QCOMPARE(m1 >= m2, false);
            } else if (i1 == i2) {
                QCOMPARE(m1 == m2, true);
                QCOMPARE(m1 != m2, false);
                QCOMPARE(m1 < m2, false);
                QCOMPARE(m1 > m2, false);
                QCOMPARE(m1 <= m2, true);
                QCOMPARE(m1 >= m2, true);
            } else /* if (i1 > i2) */ {
                QCOMPARE(m1 == m2, false);
                QCOMPARE(m1 != m2, true);
                QCOMPARE(m1 < m2, false);
                QCOMPARE(m1 > m2, true);
                QCOMPARE(m1 <= m2, false);
                QCOMPARE(m1 >= m2, true);
            }
        }
    }
}
