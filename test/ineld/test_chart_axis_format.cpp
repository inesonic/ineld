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
* This file implements tests of the Ld::ChartAxisFormat class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QSet>
#include <QColor>
#include <QtTest/QtTest>

#include <util_hash_functions.h>

#include <ld_handle.h>
#include <ld_format.h>
#include <ld_font_format.h>
#include <ld_chart_line_style.h>
#include <ld_chart_axis_format.h>

#include "test_chart_axis_format.h"

TestChartAxisFormat::TestChartAxisFormat() {}


TestChartAxisFormat::~TestChartAxisFormat() {}


void TestChartAxisFormat::initTestCase() {
    Ld::Handle::initialize(0x123456789ABCDEFULL);
}


void TestChartAxisFormat::testConstructorsAndDestructors() {
    Ld::ChartAxisFormat cls1;
    QCOMPARE(cls1.majorTickLineStyle(), Ld::ChartAxisFormat::defaultMajorTickLineStyle);
    QCOMPARE(cls1.minorTickLineStyle(), Ld::ChartAxisFormat::defaultMinorTickLineStyle);
    QCOMPARE(cls1.axisScale(), Ld::ChartAxisFormat::defaultAxisScale);
    QCOMPARE(cls1.minimumValue(), Ld::ChartAxisFormat::defaultMinimumValue);
    QCOMPARE(cls1.maximumValue(), Ld::ChartAxisFormat::defaultMaximumValue);
    QCOMPARE(cls1.majorStepSize(), Ld::ChartAxisFormat::defaultMajorStepSize);
    QCOMPARE(cls1.minorStepSize(), Ld::ChartAxisFormat::defaultMinorStepSize);
    QCOMPARE(cls1.majorTickStyle(), Ld::ChartAxisFormat::defaultMajorTickStyle);
    QCOMPARE(cls1.minorTickStyle(), Ld::ChartAxisFormat::defaultMinorTickStyle);
    QCOMPARE(cls1.titleFont(), Ld::ChartAxisFormat::defaultTitleFontFormat);
    QCOMPARE(cls1.numberFont(), Ld::ChartAxisFormat::defaultNumberFontFormat);

    Ld::ChartAxisFormat cls2(
        Ld::ChartAxisFormat::AxisScale::LOG,
        -2.0,
        +2.0,
        0.2,
        0.1,
        Ld::ChartAxisFormat::TickStyle::SPANNING,
        Ld::ChartAxisFormat::TickStyle::LINE_AND_TICK,
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 0, QColor(Qt::GlobalColor::red)),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::NO_LINE, 0, QColor(Qt::GlobalColor::green)),
        Ld::FontFormat("Helvetica", 10, Ld::FontFormat::Weight::DEMI_BOLD, false),
        Ld::FontFormat("Courier", 24, Ld::FontFormat::Weight::EXTRA_LIGHT, false)
    );

    QCOMPARE(
        cls2.majorTickLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 0, QColor(Qt::GlobalColor::red))
    );
    QCOMPARE(
        cls2.majorTickLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::NO_LINE, 0, QColor(Qt::GlobalColor::green))
    );
    QCOMPARE(cls2.axisScale(), Ld::ChartAxisFormat::AxisScale::LOG);
    QCOMPARE(cls2.minimumValue(), -2.0);
    QCOMPARE(cls2.maximumValue(), +2.0);
    QCOMPARE(cls2.majorStepSize(), 0.2);
    QCOMPARE(cls2.minorStepSize(), 0.1);
    QCOMPARE(cls2.majorTickStyle(), Ld::ChartAxisFormat::TickStyle::SPANNING);
    QCOMPARE(cls2.minorTickStyle(), Ld::ChartAxisFormat::TickStyle::LINE_AND_TICK);
    QCOMPARE(cls2.titleFont(), Ld::FontFormat("Helvetica", 10, Ld::FontFormat::Weight::DEMI_BOLD, false));
    QCOMPARE(cls2.numberFont(), Ld::FontFormat("Courier", 24, Ld::FontFormat::Weight::EXTRA_LIGHT, false));

    Ld::ChartAxisFormat cls3 = cls2;
    QCOMPARE(
        cls3.majorTickLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 0, QColor(Qt::GlobalColor::red))
    );
    QCOMPARE(
        cls3.majorTickLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::NO_LINE, 0, QColor(Qt::GlobalColor::green))
    );
    QCOMPARE(cls3.axisScale(), Ld::ChartAxisFormat::AxisScale::LOG);
    QCOMPARE(cls3.minimumValue(), -2.0);
    QCOMPARE(cls3.maximumValue(), +2.0);
    QCOMPARE(cls3.majorStepSize(), 0.2);
    QCOMPARE(cls3.minorStepSize(), 0.1);
    QCOMPARE(cls3.majorTickStyle(), Ld::ChartAxisFormat::TickStyle::SPANNING);
    QCOMPARE(cls3.minorTickStyle(), Ld::ChartAxisFormat::TickStyle::LINE_AND_TICK);
    QCOMPARE(cls3.titleFont(), Ld::FontFormat("Helvetica", 10, Ld::FontFormat::Weight::DEMI_BOLD, false));
    QCOMPARE(cls3.numberFont(), Ld::FontFormat("Courier", 24, Ld::FontFormat::Weight::EXTRA_LIGHT, false));
}


void TestChartAxisFormat::testAccessors() {
    Ld::ChartAxisFormat cls;

    QCOMPARE(cls.majorTickLineStyle(), Ld::ChartAxisFormat::defaultMajorTickLineStyle);
    QCOMPARE(cls.minorTickLineStyle(), Ld::ChartAxisFormat::defaultMinorTickLineStyle);
    QCOMPARE(cls.axisScale(), Ld::ChartAxisFormat::defaultAxisScale);
    QCOMPARE(cls.minimumValue(), Ld::ChartAxisFormat::defaultMinimumValue);
    QCOMPARE(cls.maximumValue(), Ld::ChartAxisFormat::defaultMaximumValue);
    QCOMPARE(cls.majorStepSize(), Ld::ChartAxisFormat::defaultMajorStepSize);
    QCOMPARE(cls.minorStepSize(), Ld::ChartAxisFormat::defaultMinorStepSize);
    QCOMPARE(cls.majorTickStyle(), Ld::ChartAxisFormat::defaultMajorTickStyle);
    QCOMPARE(cls.minorTickStyle(), Ld::ChartAxisFormat::defaultMinorTickStyle);
    QCOMPARE(cls.titleFont(), Ld::ChartAxisFormat::defaultTitleFontFormat);
    QCOMPARE(cls.numberFont(), Ld::ChartAxisFormat::defaultNumberFontFormat);

    cls.setMajorTickLineStyle(
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASH_DOTTED_DOTTED_LINE, 0, QColor(Qt::GlobalColor::red))
    );

    QCOMPARE(
        cls.majorTickLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASH_DOTTED_DOTTED_LINE, 0, QColor(Qt::GlobalColor::red))
    );
    QCOMPARE(cls.minorTickLineStyle(), Ld::ChartAxisFormat::defaultMinorTickLineStyle);
    QCOMPARE(cls.axisScale(), Ld::ChartAxisFormat::defaultAxisScale);
    QCOMPARE(cls.minimumValue(), Ld::ChartAxisFormat::defaultMinimumValue);
    QCOMPARE(cls.maximumValue(), Ld::ChartAxisFormat::defaultMaximumValue);
    QCOMPARE(cls.majorStepSize(), Ld::ChartAxisFormat::defaultMajorStepSize);
    QCOMPARE(cls.minorStepSize(), Ld::ChartAxisFormat::defaultMinorStepSize);
    QCOMPARE(cls.majorTickStyle(), Ld::ChartAxisFormat::defaultMajorTickStyle);
    QCOMPARE(cls.minorTickStyle(), Ld::ChartAxisFormat::defaultMinorTickStyle);
    QCOMPARE(cls.titleFont(), Ld::ChartAxisFormat::defaultTitleFontFormat);
    QCOMPARE(cls.numberFont(), Ld::ChartAxisFormat::defaultNumberFontFormat);

    cls.setMinorTickLineStyle(
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASH_DOTTED_LINE, 1, QColor(Qt::GlobalColor::blue))
    );

    QCOMPARE(
        cls.majorTickLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASH_DOTTED_DOTTED_LINE, 0, QColor(Qt::GlobalColor::red))
    );
    QCOMPARE(
        cls.minorTickLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASH_DOTTED_LINE, 1, QColor(Qt::GlobalColor::blue))
    );
    QCOMPARE(cls.axisScale(), Ld::ChartAxisFormat::defaultAxisScale);
    QCOMPARE(cls.minimumValue(), Ld::ChartAxisFormat::defaultMinimumValue);
    QCOMPARE(cls.maximumValue(), Ld::ChartAxisFormat::defaultMaximumValue);
    QCOMPARE(cls.majorStepSize(), Ld::ChartAxisFormat::defaultMajorStepSize);
    QCOMPARE(cls.minorStepSize(), Ld::ChartAxisFormat::defaultMinorStepSize);
    QCOMPARE(cls.majorTickStyle(), Ld::ChartAxisFormat::defaultMajorTickStyle);
    QCOMPARE(cls.minorTickStyle(), Ld::ChartAxisFormat::defaultMinorTickStyle);
    QCOMPARE(cls.titleFont(), Ld::ChartAxisFormat::defaultTitleFontFormat);
    QCOMPARE(cls.numberFont(), Ld::ChartAxisFormat::defaultNumberFontFormat);

    cls.setAxisScale(Ld::ChartAxisFormat::AxisScale::DB_10_LOG);

    QCOMPARE(
        cls.majorTickLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASH_DOTTED_DOTTED_LINE, 0, QColor(Qt::GlobalColor::red))
    );
    QCOMPARE(
        cls.minorTickLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASH_DOTTED_LINE, 1, QColor(Qt::GlobalColor::blue))
    );
    QCOMPARE(cls.axisScale(), Ld::ChartAxisFormat::AxisScale::DB_10_LOG);
    QCOMPARE(cls.minimumValue(), Ld::ChartAxisFormat::defaultMinimumValue);
    QCOMPARE(cls.maximumValue(), Ld::ChartAxisFormat::defaultMaximumValue);
    QCOMPARE(cls.majorStepSize(), Ld::ChartAxisFormat::defaultMajorStepSize);
    QCOMPARE(cls.minorStepSize(), Ld::ChartAxisFormat::defaultMinorStepSize);
    QCOMPARE(cls.majorTickStyle(), Ld::ChartAxisFormat::defaultMajorTickStyle);
    QCOMPARE(cls.minorTickStyle(), Ld::ChartAxisFormat::defaultMinorTickStyle);
    QCOMPARE(cls.titleFont(), Ld::ChartAxisFormat::defaultTitleFontFormat);
    QCOMPARE(cls.numberFont(), Ld::ChartAxisFormat::defaultNumberFontFormat);

    cls.setMinimumValue(-1.0);

    QCOMPARE(
        cls.majorTickLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASH_DOTTED_DOTTED_LINE, 0, QColor(Qt::GlobalColor::red))
    );
    QCOMPARE(
        cls.minorTickLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASH_DOTTED_LINE, 1, QColor(Qt::GlobalColor::blue))
    );
    QCOMPARE(cls.axisScale(), Ld::ChartAxisFormat::AxisScale::DB_10_LOG);
    QCOMPARE(cls.minimumValue(), -1.0);
    QCOMPARE(cls.maximumValue(), Ld::ChartAxisFormat::defaultMaximumValue);
    QCOMPARE(cls.majorStepSize(), Ld::ChartAxisFormat::defaultMajorStepSize);
    QCOMPARE(cls.minorStepSize(), Ld::ChartAxisFormat::defaultMinorStepSize);
    QCOMPARE(cls.majorTickStyle(), Ld::ChartAxisFormat::defaultMajorTickStyle);
    QCOMPARE(cls.minorTickStyle(), Ld::ChartAxisFormat::defaultMinorTickStyle);
    QCOMPARE(cls.titleFont(), Ld::ChartAxisFormat::defaultTitleFontFormat);
    QCOMPARE(cls.numberFont(), Ld::ChartAxisFormat::defaultNumberFontFormat);

    cls.setMaximumValue(+2.0);

    QCOMPARE(
        cls.majorTickLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASH_DOTTED_DOTTED_LINE, 0, QColor(Qt::GlobalColor::red))
    );
    QCOMPARE(
        cls.minorTickLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASH_DOTTED_LINE, 1, QColor(Qt::GlobalColor::blue))
    );
    QCOMPARE(cls.axisScale(), Ld::ChartAxisFormat::AxisScale::DB_10_LOG);
    QCOMPARE(cls.minimumValue(), -1.0);
    QCOMPARE(cls.maximumValue(), +2.0);
    QCOMPARE(cls.majorStepSize(), Ld::ChartAxisFormat::defaultMajorStepSize);
    QCOMPARE(cls.minorStepSize(), Ld::ChartAxisFormat::defaultMinorStepSize);
    QCOMPARE(cls.majorTickStyle(), Ld::ChartAxisFormat::defaultMajorTickStyle);
    QCOMPARE(cls.minorTickStyle(), Ld::ChartAxisFormat::defaultMinorTickStyle);
    QCOMPARE(cls.titleFont(), Ld::ChartAxisFormat::defaultTitleFontFormat);
    QCOMPARE(cls.numberFont(), Ld::ChartAxisFormat::defaultNumberFontFormat);

    cls.setMajorStepSize(0.2);

    QCOMPARE(
        cls.majorTickLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASH_DOTTED_DOTTED_LINE, 0, QColor(Qt::GlobalColor::red))
    );
    QCOMPARE(
        cls.minorTickLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASH_DOTTED_LINE, 1, QColor(Qt::GlobalColor::blue))
    );
    QCOMPARE(cls.axisScale(), Ld::ChartAxisFormat::AxisScale::DB_10_LOG);
    QCOMPARE(cls.minimumValue(), -1.0);
    QCOMPARE(cls.maximumValue(), +2.0);
    QCOMPARE(cls.majorStepSize(), 0.2);
    QCOMPARE(cls.minorStepSize(), Ld::ChartAxisFormat::defaultMinorStepSize);
    QCOMPARE(cls.majorTickStyle(), Ld::ChartAxisFormat::defaultMajorTickStyle);
    QCOMPARE(cls.minorTickStyle(), Ld::ChartAxisFormat::defaultMinorTickStyle);
    QCOMPARE(cls.titleFont(), Ld::ChartAxisFormat::defaultTitleFontFormat);
    QCOMPARE(cls.numberFont(), Ld::ChartAxisFormat::defaultNumberFontFormat);

    cls.setMinorStepSize(0.1);

    QCOMPARE(
        cls.majorTickLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASH_DOTTED_DOTTED_LINE, 0, QColor(Qt::GlobalColor::red))
    );
    QCOMPARE(
        cls.minorTickLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASH_DOTTED_LINE, 1, QColor(Qt::GlobalColor::blue))
    );
    QCOMPARE(cls.axisScale(), Ld::ChartAxisFormat::AxisScale::DB_10_LOG);
    QCOMPARE(cls.minimumValue(), -1.0);
    QCOMPARE(cls.maximumValue(), +2.0);
    QCOMPARE(cls.majorStepSize(), 0.2);
    QCOMPARE(cls.minorStepSize(), 0.1);
    QCOMPARE(cls.majorTickStyle(), Ld::ChartAxisFormat::defaultMajorTickStyle);
    QCOMPARE(cls.minorTickStyle(), Ld::ChartAxisFormat::defaultMinorTickStyle);
    QCOMPARE(cls.titleFont(), Ld::ChartAxisFormat::defaultTitleFontFormat);
    QCOMPARE(cls.numberFont(), Ld::ChartAxisFormat::defaultNumberFontFormat);

    cls.setMajorTickStyle(Ld::ChartAxisFormat::TickStyle::LINE_AND_TICK);

    QCOMPARE(
        cls.majorTickLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASH_DOTTED_DOTTED_LINE, 0, QColor(Qt::GlobalColor::red))
    );
    QCOMPARE(
        cls.minorTickLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASH_DOTTED_LINE, 1, QColor(Qt::GlobalColor::blue))
    );
    QCOMPARE(cls.axisScale(), Ld::ChartAxisFormat::AxisScale::DB_10_LOG);
    QCOMPARE(cls.minimumValue(), -1.0);
    QCOMPARE(cls.maximumValue(), +2.0);
    QCOMPARE(cls.majorStepSize(), 0.2);
    QCOMPARE(cls.minorStepSize(), 0.1);
    QCOMPARE(cls.majorTickStyle(), Ld::ChartAxisFormat::TickStyle::LINE_AND_TICK);
    QCOMPARE(cls.minorTickStyle(), Ld::ChartAxisFormat::defaultMinorTickStyle);
    QCOMPARE(cls.titleFont(), Ld::ChartAxisFormat::defaultTitleFontFormat);
    QCOMPARE(cls.numberFont(), Ld::ChartAxisFormat::defaultNumberFontFormat);

    cls.setMinorTickStyle(Ld::ChartAxisFormat::TickStyle::OUTER);

    QCOMPARE(
        cls.majorTickLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASH_DOTTED_DOTTED_LINE, 0, QColor(Qt::GlobalColor::red))
    );
    QCOMPARE(
        cls.minorTickLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASH_DOTTED_LINE, 1, QColor(Qt::GlobalColor::blue))
    );
    QCOMPARE(cls.axisScale(), Ld::ChartAxisFormat::AxisScale::DB_10_LOG);
    QCOMPARE(cls.minimumValue(), -1.0);
    QCOMPARE(cls.maximumValue(), +2.0);
    QCOMPARE(cls.majorStepSize(), 0.2);
    QCOMPARE(cls.minorStepSize(), 0.1);
    QCOMPARE(cls.majorTickStyle(), Ld::ChartAxisFormat::TickStyle::LINE_AND_TICK);
    QCOMPARE(cls.minorTickStyle(), Ld::ChartAxisFormat::TickStyle::OUTER);
    QCOMPARE(cls.titleFont(), Ld::ChartAxisFormat::defaultTitleFontFormat);
    QCOMPARE(cls.numberFont(), Ld::ChartAxisFormat::defaultNumberFontFormat);

    cls.setTitleFont(Ld::FontFormat("Helvetica", 10, Ld::FontFormat::Weight::BOLD, true));

    QCOMPARE(
        cls.majorTickLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASH_DOTTED_DOTTED_LINE, 0, QColor(Qt::GlobalColor::red))
    );
    QCOMPARE(
        cls.minorTickLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASH_DOTTED_LINE, 1, QColor(Qt::GlobalColor::blue))
    );
    QCOMPARE(cls.axisScale(), Ld::ChartAxisFormat::AxisScale::DB_10_LOG);
    QCOMPARE(cls.minimumValue(), -1.0);
    QCOMPARE(cls.maximumValue(), +2.0);
    QCOMPARE(cls.majorStepSize(), 0.2);
    QCOMPARE(cls.minorStepSize(), 0.1);
    QCOMPARE(cls.majorTickStyle(), Ld::ChartAxisFormat::TickStyle::LINE_AND_TICK);
    QCOMPARE(cls.minorTickStyle(), Ld::ChartAxisFormat::TickStyle::OUTER);
    QCOMPARE(cls.titleFont(), Ld::FontFormat("Helvetica", 10, Ld::FontFormat::Weight::BOLD, true));
    QCOMPARE(cls.numberFont(), Ld::ChartAxisFormat::defaultNumberFontFormat);

    cls.setNumberFont(Ld::FontFormat("Arial", 17, Ld::FontFormat::Weight::LIGHT, false));

    QCOMPARE(
        cls.majorTickLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASH_DOTTED_DOTTED_LINE, 0, QColor(Qt::GlobalColor::red))
    );
    QCOMPARE(
        cls.minorTickLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASH_DOTTED_LINE, 1, QColor(Qt::GlobalColor::blue))
    );
    QCOMPARE(cls.axisScale(), Ld::ChartAxisFormat::AxisScale::DB_10_LOG);
    QCOMPARE(cls.minimumValue(), -1.0);
    QCOMPARE(cls.maximumValue(), +2.0);
    QCOMPARE(cls.majorStepSize(), 0.2);
    QCOMPARE(cls.minorStepSize(), 0.1);
    QCOMPARE(cls.majorTickStyle(), Ld::ChartAxisFormat::TickStyle::LINE_AND_TICK);
    QCOMPARE(cls.minorTickStyle(), Ld::ChartAxisFormat::TickStyle::OUTER);
    QCOMPARE(cls.titleFont(), Ld::FontFormat("Helvetica", 10, Ld::FontFormat::Weight::BOLD, true));
    QCOMPARE(cls.numberFont(), Ld::FontFormat("Arial", 17, Ld::FontFormat::Weight::LIGHT, false));
}


void TestChartAxisFormat::testToStringMethod() {
    Ld::ChartAxisFormat cls2(
        Ld::ChartAxisFormat::AxisScale::LOG,
        -2.0,
        +2.0,
        0.2,
        0.1,
        Ld::ChartAxisFormat::TickStyle::SPANNING,
        Ld::ChartAxisFormat::TickStyle::LINE_AND_TICK,
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2, QColor(Qt::GlobalColor::red)),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::NO_LINE, 3, QColor(Qt::GlobalColor::green)),
        Ld::FontFormat("Helvetica", 10, Ld::FontFormat::Weight::DEMI_BOLD, false),
        Ld::FontFormat("Courier", 24, Ld::FontFormat::Weight::EXTRA_LIGHT, false)
    );

    QString measured = cls2.toString();
    QString expected(
        "LOG,-2,2,0.2,0.1,"
        "DOTTED_LINE,2,#ff0000,"
        "NO_LINE,3,#00ff00,"
        "Helvetica,10,0.63,,0,-,-,"
        "Courier,24,0.12,,0,-,-,"
        "SPANNING,LINE_AND_TICK"
    );

    QCOMPARE(measured, expected);
}


void TestChartAxisFormat::testFormatAggregator() {
    Ld::FormatPointer cls1(
        new Ld::ChartAxisFormat(
            Ld::ChartAxisFormat::AxisScale::LINEAR,
            -1.0,
            +2.0,
            0.1,
            0.05,
            Ld::ChartAxisFormat::TickStyle::INNER,
            Ld::ChartAxisFormat::TickStyle::NONE,
            Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2, QColor(Qt::GlobalColor::red)),
            Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::NO_LINE, 3, QColor(Qt::GlobalColor::green)),
            Ld::FontFormat("Helvetica", 24, Ld::FontFormat::Weight::NORMAL, false),
            Ld::FontFormat("Courier", 12, Ld::FontFormat::Weight::MEDIUM, true)
        )
    );

    Ld::FormatPointer cls2(
        new Ld::ChartAxisFormat(
            Ld::ChartAxisFormat::AxisScale::LINEAR,
            -2.0,
            +2.0,
            0.2,
            0.1,
            Ld::ChartAxisFormat::TickStyle::OUTER,
            Ld::ChartAxisFormat::TickStyle::NONE,
            Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2, QColor(Qt::GlobalColor::blue)),
            Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::SOLID_LINE, 3, QColor(Qt::GlobalColor::green)),
            Ld::FontFormat("Arial", 24, Ld::FontFormat::Weight::NORMAL, false),
            Ld::FontFormat("Courier", 12, Ld::FontFormat::Weight::MEDIUM, true)
        )
    );

    Ld::FormatPointer cls3(
        new Ld::ChartAxisFormat(
            Ld::ChartAxisFormat::AxisScale::LOG,
            -1.0,
            +1.0,
            0.2,
            0.1,
            Ld::ChartAxisFormat::TickStyle::INNER,
            Ld::ChartAxisFormat::TickStyle::OUTER,
            Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2, QColor(Qt::GlobalColor::blue)),
            Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::SOLID_LINE, 3, QColor(Qt::GlobalColor::green)),
            Ld::FontFormat("Helvetica", 24, Ld::FontFormat::Weight::NORMAL, false),
            Ld::FontFormat("Times Roman", 12, Ld::FontFormat::Weight::MEDIUM, true)
        )
    );

    Ld::ChartAxisFormat::Aggregation aggregation;

    aggregation.addFormat(cls1);
    aggregation.addFormat(cls2);
    aggregation.addFormat(cls3);

    const QSet<Ld::ChartAxisFormat::AxisScale>& axisScales        = aggregation.axisScales();
    const QSet<double>&                         minimumValues     = aggregation.minimumValues();
    const QSet<double>&                         maximumValues     = aggregation.maximumValues();
    const QSet<double>&                         majorStepSizes    = aggregation.majorStepSizes();
    const QSet<double>&                         minorStepSizes    = aggregation.minorStepSizes();
    const QSet<Ld::FontFormat>&                 titleFontFormats  = aggregation.titleFontFormats();
    const QSet<Ld::FontFormat>&                 numberFontFormats = aggregation.numberFontFormats();
    const QSet<Ld::ChartAxisFormat::TickStyle>& majorTickStyles   = aggregation.majorTickStyles();
    const QSet<Ld::ChartAxisFormat::TickStyle>& minorTickStyles   = aggregation.minorTickStyles();

    QCOMPARE(axisScales.size(), 2);
    QCOMPARE(minimumValues.size(), 2);
    QCOMPARE(maximumValues.size(), 2);
    QCOMPARE(majorStepSizes.size(), 2);
    QCOMPARE(minorStepSizes.size(), 2);
    QCOMPARE(titleFontFormats.size(), 2);
    QCOMPARE(numberFontFormats.size(), 2);
    QCOMPARE(majorTickStyles.size(), 2);
    QCOMPARE(minorTickStyles.size(), 2);

    QCOMPARE(axisScales.contains(Ld::ChartAxisFormat::AxisScale::LINEAR), true);
    QCOMPARE(axisScales.contains(Ld::ChartAxisFormat::AxisScale::LOG), true);

    QCOMPARE(minimumValues.contains(-1.0), true);
    QCOMPARE(minimumValues.contains(-2.0), true);

    QCOMPARE(maximumValues.contains(+2.0), true);
    QCOMPARE(maximumValues.contains(+1.0), true);

    QCOMPARE(majorStepSizes.contains(0.1), true);
    QCOMPARE(majorStepSizes.contains(0.2), true);

    QCOMPARE(minorStepSizes.contains(0.05), true);
    QCOMPARE(minorStepSizes.contains(0.1), true);

    QCOMPARE(titleFontFormats.contains(Ld::FontFormat("Helvetica", 24, Ld::FontFormat::Weight::NORMAL, false)), true);
    QCOMPARE(titleFontFormats.contains(Ld::FontFormat("Arial", 24, Ld::FontFormat::Weight::NORMAL, false)), true);

    QCOMPARE(numberFontFormats.contains(Ld::FontFormat("Courier", 12, Ld::FontFormat::Weight::MEDIUM, true)), true);
    QCOMPARE(numberFontFormats.contains(Ld::FontFormat("Times Roman", 12, Ld::FontFormat::Weight::MEDIUM, true)), true);

    QCOMPARE(majorTickStyles.contains(Ld::ChartAxisFormat::TickStyle::INNER), true);
    QCOMPARE(majorTickStyles.contains(Ld::ChartAxisFormat::TickStyle::OUTER), true);

    QCOMPARE(minorTickStyles.contains(Ld::ChartAxisFormat::TickStyle::NONE), true);
    QCOMPARE(minorTickStyles.contains(Ld::ChartAxisFormat::TickStyle::OUTER), true);
}
