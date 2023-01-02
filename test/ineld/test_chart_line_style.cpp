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
* This file implements tests of the Ld::ChartLineStyle class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QSet>
#include <QColor>
#include <QtTest/QtTest>

#include <util_hash_functions.h>

#include <ld_handle.h>
#include <ld_format.h>
#include <ld_chart_line_style.h>

#include "test_chart_line_style.h"

TestChartLineStyle::TestChartLineStyle() {}


TestChartLineStyle::~TestChartLineStyle() {}


void TestChartLineStyle::initTestCase() {
    Ld::Handle::initialize(0x123456789ABCDEFULL);
}


void TestChartLineStyle::testConstructorsAndDestructors() {
    Ld::ChartLineStyle cls1;
    QCOMPARE(cls1.lineStyle(), Ld::ChartLineStyle::defaultLineStyle);
    QCOMPARE(cls1.lineWidth(), Ld::ChartLineStyle::defaultLineWidth);
    QCOMPARE(cls1.lineColor(), Ld::ChartLineStyle::defaultLineColor);

    Ld::ChartLineStyle cls2(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 1.5F, QColor(Qt::red));
    QCOMPARE(cls2.lineStyle(), Ld::ChartLineStyle::LineStyle::DOTTED_LINE);
    QCOMPARE(cls2.lineWidth(), 1.5F);
    QCOMPARE(cls2.lineColor(), QColor(Qt::red));

    Ld::ChartLineStyle cls3 = cls2;
    QCOMPARE(cls3.lineStyle(), Ld::ChartLineStyle::LineStyle::DOTTED_LINE);
    QCOMPARE(cls3.lineWidth(), 1.5F);
    QCOMPARE(cls3.lineColor(), QColor(Qt::red));
}


void TestChartLineStyle::testAccessors() {
    Ld::ChartLineStyle cls;

    QCOMPARE(cls.lineStyle(), Ld::ChartLineStyle::defaultLineStyle);
    QCOMPARE(cls.lineWidth(), Ld::ChartLineStyle::defaultLineWidth);
    QCOMPARE(cls.lineColor(), Ld::ChartLineStyle::defaultLineColor);

    cls.setLineStyle(Ld::ChartLineStyle::LineStyle::DASH_DOTTED_DOTTED_LINE);

    QCOMPARE(cls.lineStyle(), Ld::ChartLineStyle::LineStyle::DASH_DOTTED_DOTTED_LINE);
    QCOMPARE(cls.lineWidth(), Ld::ChartLineStyle::defaultLineWidth);
    QCOMPARE(cls.lineColor(), Ld::ChartLineStyle::defaultLineColor);

    cls.setLineWidth(2.5F);

    QCOMPARE(cls.lineStyle(), Ld::ChartLineStyle::LineStyle::DASH_DOTTED_DOTTED_LINE);
    QCOMPARE(cls.lineWidth(), 2.5F);
    QCOMPARE(cls.lineColor(), Ld::ChartLineStyle::defaultLineColor);

    cls.setLineColor(QColor(Qt::blue));

    QCOMPARE(cls.lineStyle(), Ld::ChartLineStyle::LineStyle::DASH_DOTTED_DOTTED_LINE);
    QCOMPARE(cls.lineWidth(), 2.5F);
    QCOMPARE(cls.lineColor(), QColor(Qt::blue));
}


void TestChartLineStyle::testToStringMethod() {
    Ld::ChartLineStyle cls2(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 1.5F, QColor(Qt::red));

    QString v = cls2.toString();
    QCOMPARE(v, QString("DOTTED_LINE,1.5,#ff0000"));
}


void TestChartLineStyle::testFormatAggregator() {
    Ld::FormatPointer cls1(
        new Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 1.5F, QColor(Qt::red))
    );

    Ld::FormatPointer cls2(
        new Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASHED_LINE, 2.5F, QColor(Qt::red))
    );

    Ld::FormatPointer cls3(
        new Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASHED_LINE, 1.5F, QColor(Qt::blue))
    );

    Ld::ChartLineStyle::Aggregation aggregation;

    QCOMPARE(aggregation.lineStyles().isEmpty(), true);
    QCOMPARE(aggregation.lineWidths().isEmpty(), true);
    QCOMPARE(aggregation.lineColors().isEmpty(), true);

    aggregation.addFormat(cls1);
    aggregation.addFormat(cls2);
    aggregation.addFormat(cls3);

    const QSet<Ld::ChartLineStyle::LineStyle>& lineStyles = aggregation.lineStyles();
    const QSet<float>&                         lineWidths = aggregation.lineWidths();
    const QSet<QColor>&                        lineColors = aggregation.lineColors();

    QCOMPARE(lineStyles.size(), 2);
    QCOMPARE(lineWidths.size(), 2);
    QCOMPARE(lineColors.size(), 2);

    QCOMPARE(lineStyles.contains(Ld::ChartLineStyle::LineStyle::DOTTED_LINE), true);
    QCOMPARE(lineStyles.contains(Ld::ChartLineStyle::LineStyle::DASHED_LINE), true);

    QCOMPARE(lineWidths.contains(1.5F), true);
    QCOMPARE(lineWidths.contains(2.5F), true);

    QCOMPARE(lineColors.contains(QColor(Qt::red)), true);
    QCOMPARE(lineColors.contains(QColor(Qt::blue)), true);
}
