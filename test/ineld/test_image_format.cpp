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
* This file implements tests of the Ld::ImageFormat class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QSet>
#include <QtTest/QtTest>

#include <ld_handle.h>
#include <ld_format.h>
#include <ld_image_format.h>

#include "test_image_format.h"

TestImageFormat::TestImageFormat() {}


TestImageFormat::~TestImageFormat() {}


void TestImageFormat::initTestCase() {
    Ld::Handle::initialize(0x123456789ABCDEFULL);
    Ld::Format::registerCreator(Ld::ImageFormat::formatName, Ld::ImageFormat::creator);
}


void TestImageFormat::testAxisConstructorsAndDestructors() {
    Ld::ImageFormat::Axis axis1;
    QCOMPARE(axis1.scalingMode(), Ld::ImageFormat::defaultScalingMode);
    QCOMPARE(axis1.value(), Ld::ImageFormat::defaultAxisScalingValue);

    Ld::ImageFormat::Axis axis2(1.0F, Ld::ImageFormat::ImageScalingMode::FRACTIONAL);
    QCOMPARE(axis2.scalingMode(), Ld::ImageFormat::ImageScalingMode::FRACTIONAL);
    QCOMPARE(axis2.value(), 1.0F);

    Ld::ImageFormat::Axis axis3 = axis2;
    QCOMPARE(axis3.scalingMode(), Ld::ImageFormat::ImageScalingMode::FRACTIONAL);
    QCOMPARE(axis3.value(), 1.0F);
}


void TestImageFormat::testAxisAccessors() {
    Ld::ImageFormat::Axis axis;
    axis.setScalingMode(Ld::ImageFormat::ImageScalingMode::ASPECT_RATIO);
    axis.setValue(5.0F);

    QCOMPARE(axis.scalingMode(), Ld::ImageFormat::ImageScalingMode::ASPECT_RATIO);
    QCOMPARE(axis.value(), 5.0F);

    axis.setScalingMode(Ld::ImageFormat::ImageScalingMode::FIXED_POINTS);
    QCOMPARE(axis.scalingMode(), Ld::ImageFormat::ImageScalingMode::FIXED_POINTS);
    QCOMPARE(axis.value(), 5.0F);

    axis.setValue(2.0F);
    QCOMPARE(axis.scalingMode(), Ld::ImageFormat::ImageScalingMode::FIXED_POINTS);
    QCOMPARE(axis.value(), 2.0F);

    float value = axis; // Invokes cast
    QCOMPARE(value, 2.0F);
}


void TestImageFormat::testAxisAssignmentOperator() {
    Ld::ImageFormat::Axis axis1(1.0F, Ld::ImageFormat::ImageScalingMode::FRACTIONAL);

    QCOMPARE(axis1.value(), 1.0F);
    QCOMPARE(axis1.scalingMode(), Ld::ImageFormat::ImageScalingMode::FRACTIONAL);

    Ld::ImageFormat::Axis axis2(2.0F, Ld::ImageFormat::ImageScalingMode::ASPECT_RATIO);

    QCOMPARE(axis2.value(), 2.0F);
    QCOMPARE(axis2.scalingMode(), Ld::ImageFormat::ImageScalingMode::ASPECT_RATIO);

    axis2 = axis1;

    QCOMPARE(axis1.value(), 1.0F);
    QCOMPARE(axis1.scalingMode(), Ld::ImageFormat::ImageScalingMode::FRACTIONAL);

    QCOMPARE(axis2.value(), 1.0F);
    QCOMPARE(axis2.scalingMode(), Ld::ImageFormat::ImageScalingMode::FRACTIONAL);

    axis1.setScalingMode(Ld::ImageFormat::ImageScalingMode::FIXED_POINTS);
    axis1.setValue(144.0F);

    QCOMPARE(axis1.value(), 144.0F);
    QCOMPARE(axis1.scalingMode(), Ld::ImageFormat::ImageScalingMode::FIXED_POINTS);

    QCOMPARE(axis2.value(), 1.0F);
    QCOMPARE(axis2.scalingMode(), Ld::ImageFormat::ImageScalingMode::FRACTIONAL);
}


void TestImageFormat::testAxisComparisonOperators() {
    Ld::ImageFormat::Axis axis1(1.0F, Ld::ImageFormat::ImageScalingMode::FRACTIONAL);
    Ld::ImageFormat::Axis axis2(1.0F, Ld::ImageFormat::ImageScalingMode::ASPECT_RATIO);
    Ld::ImageFormat::Axis axis3(1.5F, Ld::ImageFormat::ImageScalingMode::FRACTIONAL);

    QCOMPARE((axis1 == axis1), true);
    QCOMPARE((axis1 == axis2), false);
    QCOMPARE((axis1 == axis3), false);

    QCOMPARE((axis1 != axis1), false);
    QCOMPARE((axis1 != axis2), true);
    QCOMPARE((axis1 != axis3), true);

    QCOMPARE((axis1 < axis1), false);
    QCOMPARE((axis1 < axis2), false);
    QCOMPARE((axis1 < axis3), true);
    QCOMPARE((axis3 < axis1), false);

    QCOMPARE((axis1 > axis1), false);
    QCOMPARE((axis1 > axis2), false);
    QCOMPARE((axis1 > axis3), false);
    QCOMPARE((axis3 > axis1), true);

    QCOMPARE((axis1 <= axis1), true);
    QCOMPARE((axis1 <= axis2), true);
    QCOMPARE((axis1 <= axis3), true);
    QCOMPARE((axis3 <= axis1), false);

    QCOMPARE((axis1 >= axis1), true);
    QCOMPARE((axis1 >= axis2), true);
    QCOMPARE((axis1 >= axis3), false);
    QCOMPARE((axis3 >= axis1), true);
}


void TestImageFormat::testConstructorsAndDestructors() {
    Ld::ImageFormat format1;

    Ld::ImageFormat::Axis defaultAxis;
    QCOMPARE(format1.horizontalAxis(), defaultAxis);
    QCOMPARE(format1.verticalAxis(), defaultAxis);
    QCOMPARE(format1.rotation(), Ld::ImageFormat::defaultRotation);

    format1.setHorizontalAxis(144.0F, Ld::ImageFormat::ImageScalingMode::FIXED_POINTS);
    format1.setVerticalAxis(1.0F, Ld::ImageFormat::ImageScalingMode::ASPECT_RATIO);
    format1.setRotation(Ld::ImageFormat::Rotation::ROTATE_CCW_90);

    QCOMPARE(format1.horizontalAxis(), Ld::ImageFormat::Axis(144.0F, Ld::ImageFormat::ImageScalingMode::FIXED_POINTS));
    QCOMPARE(format1.verticalAxis(), Ld::ImageFormat::Axis(1.0F, Ld::ImageFormat::ImageScalingMode::ASPECT_RATIO));
    QCOMPARE(format1.rotation(), Ld::ImageFormat::Rotation::ROTATE_CCW_90);

    Ld::ImageFormat format2 = format1;

    QCOMPARE(format2.horizontalAxis(), Ld::ImageFormat::Axis(144.0F, Ld::ImageFormat::ImageScalingMode::FIXED_POINTS));
    QCOMPARE(format2.verticalAxis(), Ld::ImageFormat::Axis(1.0F, Ld::ImageFormat::ImageScalingMode::ASPECT_RATIO));
    QCOMPARE(format1.rotation(), Ld::ImageFormat::Rotation::ROTATE_CCW_90);
}


void TestImageFormat::testAccessors() {
    Ld::ImageFormat format;

    QCOMPARE(format.typeName(), Ld::ImageFormat::formatName);
    QCOMPARE(format.isValid(), false);
    QCOMPARE(format.isInvalid(), true);

    Ld::Format::Capabilities capabilities = format.capabilities();
    QVERIFY(capabilities.contains(QString("Format")));
    QVERIFY(capabilities.contains(Ld::ImageFormat::formatName));

    format.setHorizontalAxis(144.0F, Ld::ImageFormat::ImageScalingMode::FIXED_POINTS);

    QCOMPARE(format.isValid(), false);
    QCOMPARE(format.isInvalid(), true);

    format.setVerticalAxis(1.0F, Ld::ImageFormat::ImageScalingMode::ASPECT_RATIO);

    QCOMPARE(format.isValid(), true);
    QCOMPARE(format.isInvalid(), false);

    QCOMPARE(format.horizontalAxis(), Ld::ImageFormat::Axis(144.0F, Ld::ImageFormat::ImageScalingMode::FIXED_POINTS));
    QCOMPARE(format.verticalAxis(), Ld::ImageFormat::Axis(1.0F, Ld::ImageFormat::ImageScalingMode::ASPECT_RATIO));

    format.setHorizontalAxis(Ld::ImageFormat::Axis(2.0F, Ld::ImageFormat::ImageScalingMode::FRACTIONAL));
    format.setVerticalAxis(Ld::ImageFormat::Axis(1.5F, Ld::ImageFormat::ImageScalingMode::FRACTIONAL));

    QCOMPARE(format.horizontalAxis(), Ld::ImageFormat::Axis(2.0F, Ld::ImageFormat::ImageScalingMode::FRACTIONAL));
    QCOMPARE(format.verticalAxis(), Ld::ImageFormat::Axis(1.5F, Ld::ImageFormat::ImageScalingMode::FRACTIONAL));

    format.setRotation(Ld::ImageFormat::Rotation::ROTATE_CCW_90);
    QCOMPARE(format.rotation(), Ld::ImageFormat::Rotation::ROTATE_CCW_90);

    format.setRotation(Ld::ImageFormat::Rotation::ROTATE_CW_90);
    QCOMPARE(format.rotation(), Ld::ImageFormat::Rotation::ROTATE_CW_90);
}


void TestImageFormat::testToStringMethod() {
    Ld::ImageFormat format;
    format.setHorizontalAxis(144.0F, Ld::ImageFormat::ImageScalingMode::FIXED_POINTS);
    format.setVerticalAxis(1.0F, Ld::ImageFormat::ImageScalingMode::ASPECT_RATIO);
    format.setRotation(Ld::ImageFormat::Rotation::NO_ROTATION);

    QString s = format.toString();
    QCOMPARE(s, QString("ImageFormat,FIXED,144,ASPECT_RATIO,1,0"));

    format.setHorizontalAxis(1.0F, Ld::ImageFormat::ImageScalingMode::INVALID);
    format.setVerticalAxis(1.0F, Ld::ImageFormat::ImageScalingMode::IGNORED);
    format.setRotation(Ld::ImageFormat::Rotation::ROTATE_CCW_90);

    s = format.toString();
    QCOMPARE(s, QString("ImageFormat,INVALID,1,IGNORED,1,90"));

    format.setHorizontalAxis(0.5F, Ld::ImageFormat::ImageScalingMode::FRACTIONAL);
    format.setRotation(Ld::ImageFormat::Rotation::FLIP);

    s = format.toString();
    QCOMPARE(s, QString("ImageFormat,FRACTIONAL,0.5,IGNORED,1,180"));
}


void TestImageFormat::testFormatAggregator() {
    typedef Ld::ImageFormat::Axis Axis;

    Ld::ImageFormat::Aggregation aggregation;

    QCOMPARE(aggregation.typeName(), Ld::ImageFormat::formatName);
    QVERIFY(aggregation.horizontalAxis().isEmpty());
    QVERIFY(aggregation.verticalAxis().isEmpty());
    QVERIFY(aggregation.rotations().isEmpty());

    QSharedPointer<Ld::ImageFormat> format1(new Ld::ImageFormat);

    format1->setHorizontalAxis(1.0F, Ld::ImageFormat::ImageScalingMode::FRACTIONAL);
    format1->setVerticalAxis(0.0F, Ld::ImageFormat::ImageScalingMode::IGNORED);

    aggregation.addFormat(format1);

    QCOMPARE(aggregation.horizontalAxis().count(), 1);
    QCOMPARE(aggregation.verticalAxis().count(), 1);
    QCOMPARE(aggregation.rotations().count(), 1);

    QVERIFY(aggregation.horizontalAxis().contains(Axis(1.0F, Ld::ImageFormat::ImageScalingMode::FRACTIONAL)));
    QVERIFY(aggregation.verticalAxis().contains(Axis(0.0F, Ld::ImageFormat::ImageScalingMode::IGNORED)));
    QVERIFY(aggregation.rotations().contains(Ld::ImageFormat::Rotation::NO_ROTATION));

    QSharedPointer<Ld::ImageFormat> format2(new Ld::ImageFormat);

    format2->setHorizontalAxis(72.0F, Ld::ImageFormat::ImageScalingMode::FIXED_POINTS);
    format2->setVerticalAxis(0.0F, Ld::ImageFormat::ImageScalingMode::IGNORED);
    format2->setRotation(Ld::ImageFormat::Rotation::FLIP);

    aggregation.addFormat(format2);

    QCOMPARE(aggregation.horizontalAxis().count(), 2);
    QCOMPARE(aggregation.verticalAxis().count(), 1);
    QCOMPARE(aggregation.rotations().count(), 2);

    QVERIFY(aggregation.horizontalAxis().contains(Axis(72.0F, Ld::ImageFormat::ImageScalingMode::FIXED_POINTS)));
    QVERIFY(aggregation.horizontalAxis().contains(Axis(1.0F, Ld::ImageFormat::ImageScalingMode::FRACTIONAL)));

    QVERIFY(aggregation.verticalAxis().contains(Axis(0.0F, Ld::ImageFormat::ImageScalingMode::IGNORED)));

    QVERIFY(aggregation.rotations().contains(Ld::ImageFormat::Rotation::NO_ROTATION));
    QVERIFY(aggregation.rotations().contains(Ld::ImageFormat::Rotation::FLIP));

    QSharedPointer<Ld::ImageFormat> format3(new Ld::ImageFormat);

    format3->setHorizontalAxis(72.0F, Ld::ImageFormat::ImageScalingMode::FIXED_POINTS);
    format3->setVerticalAxis(1.0F, Ld::ImageFormat::ImageScalingMode::ASPECT_RATIO);

    aggregation.addFormat(format3);

    QCOMPARE(aggregation.horizontalAxis().count(), 2);
    QCOMPARE(aggregation.verticalAxis().count(), 2);

    QVERIFY(aggregation.horizontalAxis().contains(Axis(72.0F, Ld::ImageFormat::ImageScalingMode::FIXED_POINTS)));
    QVERIFY(aggregation.horizontalAxis().contains(Axis(1.0F, Ld::ImageFormat::ImageScalingMode::FRACTIONAL)));

    QVERIFY(aggregation.verticalAxis().contains(Axis(0.0F, Ld::ImageFormat::ImageScalingMode::IGNORED)));
    QVERIFY(aggregation.verticalAxis().contains(Axis(1.0F, Ld::ImageFormat::ImageScalingMode::ASPECT_RATIO)));
}
