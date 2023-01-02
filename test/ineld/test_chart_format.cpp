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
* This file implements tests of the Ld::ChartFormat class.
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
#include <ld_chart_format.h>

#include "test_chart_format.h"

TestChartFormat::TestChartFormat() {}


TestChartFormat::~TestChartFormat() {}


void TestChartFormat::initTestCase() {
    Ld::Handle::initialize(0x123456789ABCDEFULL);
}


void TestChartFormat::testConstructorsAndDestructors() {
    Ld::ChartFormat fmt1;

    QCOMPARE(fmt1.typeName(), QString("ChartFormat"));
    QCOMPARE(fmt1.isValid(), false);
    QCOMPARE(fmt1.chartType(), QString());
    QCOMPARE(fmt1.chartWidth(), Ld::ChartFormat::defaultChartWidth);
    QCOMPARE(fmt1.chartHeight(), Ld::ChartFormat::defaultChartHeight);
    QCOMPARE(fmt1.borderLineStyle(), Ld::ChartLineStyle());

    fmt1.setChartWidth(5.0F);
    fmt1.setChartHeight(4.0F);
    fmt1.setChartType(QString("Fnord"));
    fmt1.setBorderLineStyle(Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 1.5, QColor(Qt::red)));

    Ld::ChartFormat fmt2(fmt1);

    QCOMPARE(fmt1.typeName(), QString("ChartFormat"));
    QCOMPARE(fmt1.isValid(), true);
    QCOMPARE(fmt1.chartType(), QString("Fnord"));
    QCOMPARE(fmt1.chartWidth(), 5.0F);
    QCOMPARE(fmt1.chartHeight(), 4.0F);
    QCOMPARE(
        fmt1.borderLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 1.5, QColor(Qt::red)))
    );
}


void TestChartFormat::testAccessors() {
    Ld::ChartFormat fmt;

    QCOMPARE(fmt.isValid(), false);
    QCOMPARE(fmt.chartType(), QString());
    QCOMPARE(fmt.chartWidth(), Ld::ChartFormat::defaultChartWidth);
    QCOMPARE(fmt.chartHeight(), Ld::ChartFormat::defaultChartHeight);
    QCOMPARE(fmt.borderLineStyle(), Ld::ChartLineStyle());

    fmt.setChartType(QString("xy"));

    QCOMPARE(fmt.isValid(), true);
    QCOMPARE(fmt.chartType(), QString("xy"));
    QCOMPARE(fmt.chartWidth(), Ld::ChartFormat::defaultChartWidth);
    QCOMPARE(fmt.chartHeight(), Ld::ChartFormat::defaultChartHeight);
    QCOMPARE(fmt.borderLineStyle(), Ld::ChartLineStyle());

    fmt.setChartWidth(-1.0F);

    QCOMPARE(fmt.isValid(), false);
    QCOMPARE(fmt.chartType(), QString("xy"));
    QCOMPARE(fmt.chartWidth(), -1.0F);
    QCOMPARE(fmt.chartHeight(), Ld::ChartFormat::defaultChartHeight);
    QCOMPARE(fmt.borderLineStyle(), Ld::ChartLineStyle());

    fmt.setChartWidth(1.0F);

    QCOMPARE(fmt.isValid(), true);
    QCOMPARE(fmt.chartType(), QString("xy"));
    QCOMPARE(fmt.chartWidth(), 1.0F);
    QCOMPARE(fmt.chartHeight(), Ld::ChartFormat::defaultChartHeight);
    QCOMPARE(fmt.borderLineStyle(), Ld::ChartLineStyle());

    fmt.setChartHeight(-1.0F);

    QCOMPARE(fmt.isValid(), false);
    QCOMPARE(fmt.chartType(), QString("xy"));
    QCOMPARE(fmt.chartWidth(), 1.0F);
    QCOMPARE(fmt.chartHeight(), -1.0F);
    QCOMPARE(fmt.borderLineStyle(), Ld::ChartLineStyle());

    fmt.setChartHeight(1.0F);

    QCOMPARE(fmt.isValid(), true);
    QCOMPARE(fmt.chartType(), QString("xy"));
    QCOMPARE(fmt.chartWidth(), 1.0F);
    QCOMPARE(fmt.chartHeight(), 1.0F);
    QCOMPARE(fmt.borderLineStyle(), Ld::ChartLineStyle());

    fmt.setBorderLineStyle(Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2.5F, QColor(Qt::red)));

    QCOMPARE(fmt.isValid(), true);
    QCOMPARE(fmt.chartType(), QString("xy"));
    QCOMPARE(fmt.chartWidth(), 1.0F);
    QCOMPARE(fmt.chartHeight(), 1.0F);
    QCOMPARE(
        fmt.borderLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2.5F, QColor(Qt::red))
    );
}


void TestChartFormat::testToStringMethod() {
    Ld::ChartFormat fmt;

    fmt.setChartType(QString("xy"));
    fmt.setChartWidth(1.0F);
    fmt.setChartHeight(2.0F);
    fmt.setBorderLineStyle(Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2.5F, QColor(Qt::red)));

    QString s = fmt.toString();
    QCOMPARE(s, QString("ChartFormat,xy,1,2,DOTTED_LINE,2.5,#ff0000"));
}


void TestChartFormat::testFormatAggregator() {
    QSharedPointer<Ld::ChartFormat> fmt1(new Ld::ChartFormat);
    QSharedPointer<Ld::ChartFormat> fmt2(new Ld::ChartFormat);
    QSharedPointer<Ld::ChartFormat> fmt3(new Ld::ChartFormat);

    fmt1->setChartType(QString("xy"));
    fmt1->setChartWidth(1.0F);
    fmt1->setChartHeight(2.0F);
    fmt1->setBorderLineStyle(Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2.5F, QColor(Qt::red)));

    fmt2->setChartType(QString("xy"));
    fmt2->setChartWidth(4.0F);
    fmt2->setChartHeight(2.0F);
    fmt2->setBorderLineStyle(Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASHED_LINE, 2.5F, QColor(Qt::red)));

    fmt3->setChartType(QString("bar"));
    fmt3->setChartWidth(4.0F);
    fmt3->setChartHeight(5.0F);
    fmt3->setBorderLineStyle(Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2.5F, QColor(Qt::red)));

    Ld::ChartFormat::Aggregation aggregation;

    aggregation.addFormat(fmt1);
    aggregation.addFormat(fmt2);
    aggregation.addFormat(fmt3);

    const QSet<QString>&            chartTypes       = aggregation.chartTypes();
    const QSet<float>&              chartWidths      = aggregation.chartWidths();
    const QSet<float>&              chartHeights     = aggregation.chartHeights();
    const QSet<Ld::ChartLineStyle>& borderLineStyles = aggregation.borderLineStyles();

    QCOMPARE(chartTypes.size(), 2);
    QCOMPARE(chartWidths.size(), 2);
    QCOMPARE(chartHeights.size(), 2);
    QCOMPARE(borderLineStyles.size(), 2);

    QCOMPARE(chartTypes.contains(QString("xy")), true);
    QCOMPARE(chartTypes.contains(QString("bar")), true);

    QCOMPARE(chartWidths.contains(1.0F), true);
    QCOMPARE(chartWidths.contains(4.0F), true);

    QCOMPARE(chartHeights.contains(2.0F), true);
    QCOMPARE(chartHeights.contains(5.0F), true);

    QCOMPARE(
        borderLineStyles.contains(
            Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2.5F, QColor(Qt::red))
        ),
        true
    );
    QCOMPARE(
        borderLineStyles.contains(
            Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASHED_LINE, 2.5F, QColor(Qt::red))
        ),
        true
    );
}

