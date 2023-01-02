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
* This file implements tests of the Ld::PlotFormatBase class.
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
#include <ld_plot_format_base.h>

#include "test_plot_format_base.h"

TestPlotFormatBase::TestPlotFormatBase() {}


TestPlotFormatBase::~TestPlotFormatBase() {}


void TestPlotFormatBase::initTestCase() {
    Ld::Handle::initialize(0x123456789ABCDEFULL);
}


void TestPlotFormatBase::testConstructorsAndDestructors() {
    Ld::PlotFormatBase fmt1;

    QCOMPARE(fmt1.typeName(), QString("PlotFormatBase"));
    QCOMPARE(fmt1.isValid(), false);
    QCOMPARE(fmt1.chartType(), QString());
    QCOMPARE(fmt1.chartWidth(), Ld::PlotFormatBase::defaultChartWidth);
    QCOMPARE(fmt1.chartHeight(), Ld::PlotFormatBase::defaultChartHeight);
    QCOMPARE(fmt1.borderLineStyle(), Ld::ChartLineStyle());
    QCOMPARE(fmt1.plotAreaLeftMargin(), Ld::PlotFormatBase::defaultChartLeftMargin);
    QCOMPARE(fmt1.plotAreaRightMargin(), Ld::PlotFormatBase::defaultChartRightMargin);
    QCOMPARE(fmt1.plotAreaTopMargin(), Ld::PlotFormatBase::defaultChartTopMargin);
    QCOMPARE(fmt1.plotAreaBottomMargin(), Ld::PlotFormatBase::defaultChartBottomMargin);
    QCOMPARE(fmt1.plotAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotBackgroundAreaColor);
    QCOMPARE(fmt1.plotDrawingAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotDrawingAreaBackgroundColor);
    QCOMPARE(fmt1.titleFontFormat(), Ld::PlotFormatBase::defaultPlotTitleFontFormat);
    QCOMPARE(fmt1.legendLocation(), Ld::PlotFormatBase::defaultLegendLocation);
    QCOMPARE(fmt1.drawingAreaOutlineStyle(), Ld::PlotFormatBase::defaultPlotDrawingAreaOutputStyle);

    fmt1.setChartWidth(5.0F);
    fmt1.setChartHeight(4.0F);
    fmt1.setChartType(QString("Fnord"));
    fmt1.setBorderLineStyle(Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 1.5, QColor(Qt::red)));
    fmt1.setPlotAreaLeftMargin(5.0F);
    fmt1.setPlotAreaRightMargin(6.0F);
    fmt1.setPlotAreaTopMargin(7.0F);
    fmt1.setPlotAreaBottomMargin(8.0F);
    fmt1.setPlotAreaBackgroundColor(QColor(Qt::green));
    fmt1.setPlotDrawingAreaBackgroundColor(QColor(Qt::cyan));
    fmt1.setTitleFontFormat(Ld::FontFormat("Courier", 24, Ld::FontFormat::Weight::BLACK, true));
    fmt1.setLegendLocation(Ld::PlotFormatBase::LegendLocation::LEFT);
    fmt1.setDrawingAreaOutlineStyle(Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASHED_LINE, 3, QColor(Qt::red)));

    Ld::PlotFormatBase fmt2(fmt1);

    QCOMPARE(fmt1.typeName(), QString("PlotFormatBase"));
    QCOMPARE(fmt1.isValid(), true);
    QCOMPARE(fmt1.chartType(), QString("Fnord"));
    QCOMPARE(fmt1.chartWidth(), 5.0F);
    QCOMPARE(fmt1.chartHeight(), 4.0F);
    QCOMPARE(
        fmt1.borderLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 1.5, QColor(Qt::red)))
    );
    QCOMPARE(fmt1.plotAreaLeftMargin(), 5.0F);
    QCOMPARE(fmt1.plotAreaRightMargin(), 6.0F);
    QCOMPARE(fmt1.plotAreaTopMargin(), 7.0F);
    QCOMPARE(fmt1.plotAreaBottomMargin(), 8.0F);
    QCOMPARE(fmt1.plotAreaBackgroundColor(), QColor(Qt::green));
    QCOMPARE(fmt1.plotDrawingAreaBackgroundColor(), QColor(Qt::cyan));
    QCOMPARE(fmt1.titleFontFormat(), Ld::FontFormat("Courier", 24, Ld::FontFormat::Weight::BLACK, true));
    QCOMPARE(fmt1.legendLocation(), Ld::PlotFormatBase::LegendLocation::LEFT);
    QCOMPARE(
        fmt1.drawingAreaOutlineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASHED_LINE, 3, QColor(Qt::red))
    );
}


void TestPlotFormatBase::testCapabilities() {
    Ld::PlotFormatBase fmt;
    Ld::Format::Capabilities capabilities = fmt.capabilities();

    QCOMPARE(capabilities.size(), 3);
    QCOMPARE(capabilities.contains(Ld::Format::formatName), true);
    QCOMPARE(capabilities.contains(Ld::ChartFormat::formatName), true);
    QCOMPARE(capabilities.contains(Ld::PlotFormatBase::formatName), true);
}


void TestPlotFormatBase::testAccessors() {
    Ld::PlotFormatBase fmt;

    QCOMPARE(fmt.isValid(), false);
    QCOMPARE(fmt.chartType(), QString());
    QCOMPARE(fmt.chartWidth(), Ld::PlotFormatBase::defaultChartWidth);
    QCOMPARE(fmt.chartHeight(), Ld::PlotFormatBase::defaultChartHeight);
    QCOMPARE(fmt.borderLineStyle(), Ld::ChartLineStyle());
    QCOMPARE(fmt.plotAreaLeftMargin(), Ld::PlotFormatBase::defaultChartLeftMargin);
    QCOMPARE(fmt.plotAreaRightMargin(), Ld::PlotFormatBase::defaultChartRightMargin);
    QCOMPARE(fmt.plotAreaTopMargin(), Ld::PlotFormatBase::defaultChartTopMargin);
    QCOMPARE(fmt.plotAreaBottomMargin(), Ld::PlotFormatBase::defaultChartBottomMargin);
    QCOMPARE(fmt.plotAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotBackgroundAreaColor);
    QCOMPARE(fmt.plotDrawingAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotDrawingAreaBackgroundColor);
    QCOMPARE(fmt.titleFontFormat(), Ld::PlotFormatBase::defaultPlotTitleFontFormat);
    QCOMPARE(fmt.legendLocation(), Ld::PlotFormatBase::defaultLegendLocation);
    QCOMPARE(fmt.drawingAreaOutlineStyle(), Ld::PlotFormatBase::defaultPlotDrawingAreaOutputStyle);

    fmt.setChartType(QString("xy"));

    QCOMPARE(fmt.isValid(), true);
    QCOMPARE(fmt.chartType(), QString("xy"));
    QCOMPARE(fmt.chartWidth(), Ld::PlotFormatBase::defaultChartWidth);
    QCOMPARE(fmt.chartHeight(), Ld::PlotFormatBase::defaultChartHeight);
    QCOMPARE(fmt.borderLineStyle(), Ld::ChartLineStyle());
    QCOMPARE(fmt.plotAreaLeftMargin(), Ld::PlotFormatBase::defaultChartLeftMargin);
    QCOMPARE(fmt.plotAreaRightMargin(), Ld::PlotFormatBase::defaultChartRightMargin);
    QCOMPARE(fmt.plotAreaTopMargin(), Ld::PlotFormatBase::defaultChartTopMargin);
    QCOMPARE(fmt.plotAreaBottomMargin(), Ld::PlotFormatBase::defaultChartBottomMargin);
    QCOMPARE(fmt.plotAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotBackgroundAreaColor);
    QCOMPARE(fmt.plotDrawingAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotDrawingAreaBackgroundColor);
    QCOMPARE(fmt.titleFontFormat(), Ld::PlotFormatBase::defaultPlotTitleFontFormat);
    QCOMPARE(fmt.legendLocation(), Ld::PlotFormatBase::defaultLegendLocation);
    QCOMPARE(fmt.drawingAreaOutlineStyle(), Ld::PlotFormatBase::defaultPlotDrawingAreaOutputStyle);

    fmt.setChartWidth(-1.0F);

    QCOMPARE(fmt.isValid(), false);
    QCOMPARE(fmt.chartType(), QString("xy"));
    QCOMPARE(fmt.chartWidth(), -1.0F);
    QCOMPARE(fmt.chartHeight(), Ld::PlotFormatBase::defaultChartHeight);
    QCOMPARE(fmt.borderLineStyle(), Ld::ChartLineStyle());
    QCOMPARE(fmt.plotAreaLeftMargin(), Ld::PlotFormatBase::defaultChartLeftMargin);
    QCOMPARE(fmt.plotAreaRightMargin(), Ld::PlotFormatBase::defaultChartRightMargin);
    QCOMPARE(fmt.plotAreaTopMargin(), Ld::PlotFormatBase::defaultChartTopMargin);
    QCOMPARE(fmt.plotAreaBottomMargin(), Ld::PlotFormatBase::defaultChartBottomMargin);
    QCOMPARE(fmt.plotAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotBackgroundAreaColor);
    QCOMPARE(fmt.plotDrawingAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotDrawingAreaBackgroundColor);
    QCOMPARE(fmt.titleFontFormat(), Ld::PlotFormatBase::defaultPlotTitleFontFormat);
    QCOMPARE(fmt.legendLocation(), Ld::PlotFormatBase::defaultLegendLocation);
    QCOMPARE(fmt.drawingAreaOutlineStyle(), Ld::PlotFormatBase::defaultPlotDrawingAreaOutputStyle);

    fmt.setChartWidth(1.0F);

    QCOMPARE(fmt.isValid(), true);
    QCOMPARE(fmt.chartType(), QString("xy"));
    QCOMPARE(fmt.chartWidth(), 1.0F);
    QCOMPARE(fmt.chartHeight(), Ld::PlotFormatBase::defaultChartHeight);
    QCOMPARE(fmt.borderLineStyle(), Ld::ChartLineStyle());
    QCOMPARE(fmt.plotAreaLeftMargin(), Ld::PlotFormatBase::defaultChartLeftMargin);
    QCOMPARE(fmt.plotAreaRightMargin(), Ld::PlotFormatBase::defaultChartRightMargin);
    QCOMPARE(fmt.plotAreaTopMargin(), Ld::PlotFormatBase::defaultChartTopMargin);
    QCOMPARE(fmt.plotAreaBottomMargin(), Ld::PlotFormatBase::defaultChartBottomMargin);
    QCOMPARE(fmt.plotAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotBackgroundAreaColor);
    QCOMPARE(fmt.plotDrawingAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotDrawingAreaBackgroundColor);
    QCOMPARE(fmt.titleFontFormat(), Ld::PlotFormatBase::defaultPlotTitleFontFormat);
    QCOMPARE(fmt.legendLocation(), Ld::PlotFormatBase::defaultLegendLocation);
    QCOMPARE(fmt.drawingAreaOutlineStyle(), Ld::PlotFormatBase::defaultPlotDrawingAreaOutputStyle);

    fmt.setChartHeight(-1.0F);

    QCOMPARE(fmt.isValid(), false);
    QCOMPARE(fmt.chartType(), QString("xy"));
    QCOMPARE(fmt.chartWidth(), 1.0F);
    QCOMPARE(fmt.chartHeight(), -1.0F);
    QCOMPARE(fmt.borderLineStyle(), Ld::ChartLineStyle());
    QCOMPARE(fmt.plotAreaLeftMargin(), Ld::PlotFormatBase::defaultChartLeftMargin);
    QCOMPARE(fmt.plotAreaRightMargin(), Ld::PlotFormatBase::defaultChartRightMargin);
    QCOMPARE(fmt.plotAreaTopMargin(), Ld::PlotFormatBase::defaultChartTopMargin);
    QCOMPARE(fmt.plotAreaBottomMargin(), Ld::PlotFormatBase::defaultChartBottomMargin);
    QCOMPARE(fmt.plotAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotBackgroundAreaColor);
    QCOMPARE(fmt.plotDrawingAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotDrawingAreaBackgroundColor);
    QCOMPARE(fmt.titleFontFormat(), Ld::PlotFormatBase::defaultPlotTitleFontFormat);
    QCOMPARE(fmt.legendLocation(), Ld::PlotFormatBase::defaultLegendLocation);
    QCOMPARE(fmt.drawingAreaOutlineStyle(), Ld::PlotFormatBase::defaultPlotDrawingAreaOutputStyle);

    fmt.setChartHeight(1.0F);

    QCOMPARE(fmt.isValid(), true);
    QCOMPARE(fmt.chartType(), QString("xy"));
    QCOMPARE(fmt.chartWidth(), 1.0F);
    QCOMPARE(fmt.chartHeight(), 1.0F);
    QCOMPARE(fmt.borderLineStyle(), Ld::ChartLineStyle());
    QCOMPARE(fmt.plotAreaLeftMargin(), Ld::PlotFormatBase::defaultChartLeftMargin);
    QCOMPARE(fmt.plotAreaRightMargin(), Ld::PlotFormatBase::defaultChartRightMargin);
    QCOMPARE(fmt.plotAreaTopMargin(), Ld::PlotFormatBase::defaultChartTopMargin);
    QCOMPARE(fmt.plotAreaBottomMargin(), Ld::PlotFormatBase::defaultChartBottomMargin);
    QCOMPARE(fmt.plotAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotBackgroundAreaColor);
    QCOMPARE(fmt.plotDrawingAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotDrawingAreaBackgroundColor);
    QCOMPARE(fmt.titleFontFormat(), Ld::PlotFormatBase::defaultPlotTitleFontFormat);
    QCOMPARE(fmt.legendLocation(), Ld::PlotFormatBase::defaultLegendLocation);
    QCOMPARE(fmt.drawingAreaOutlineStyle(), Ld::PlotFormatBase::defaultPlotDrawingAreaOutputStyle);

    fmt.setBorderLineStyle(Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2.5F, QColor(Qt::red)));

    QCOMPARE(fmt.isValid(), true);
    QCOMPARE(fmt.chartType(), QString("xy"));
    QCOMPARE(fmt.chartWidth(), 1.0F);
    QCOMPARE(fmt.chartHeight(), 1.0F);
    QCOMPARE(
        fmt.borderLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2.5F, QColor(Qt::red))
    );
    QCOMPARE(fmt.plotAreaLeftMargin(), Ld::PlotFormatBase::defaultChartLeftMargin);
    QCOMPARE(fmt.plotAreaRightMargin(), Ld::PlotFormatBase::defaultChartRightMargin);
    QCOMPARE(fmt.plotAreaTopMargin(), Ld::PlotFormatBase::defaultChartTopMargin);
    QCOMPARE(fmt.plotAreaBottomMargin(), Ld::PlotFormatBase::defaultChartBottomMargin);
    QCOMPARE(fmt.plotAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotBackgroundAreaColor);
    QCOMPARE(fmt.plotDrawingAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotDrawingAreaBackgroundColor);
    QCOMPARE(fmt.titleFontFormat(), Ld::PlotFormatBase::defaultPlotTitleFontFormat);
    QCOMPARE(fmt.legendLocation(), Ld::PlotFormatBase::defaultLegendLocation);
    QCOMPARE(fmt.drawingAreaOutlineStyle(), Ld::PlotFormatBase::defaultPlotDrawingAreaOutputStyle);

    fmt.setPlotAreaLeftMargin(-4.0F);

    QCOMPARE(fmt.isValid(), false);
    QCOMPARE(fmt.chartType(), QString("xy"));
    QCOMPARE(fmt.chartWidth(), 1.0F);
    QCOMPARE(fmt.chartHeight(), 1.0F);
    QCOMPARE(
        fmt.borderLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2.5F, QColor(Qt::red))
    );
    QCOMPARE(fmt.plotAreaLeftMargin(), -4.0F);
    QCOMPARE(fmt.plotAreaRightMargin(), Ld::PlotFormatBase::defaultChartRightMargin);
    QCOMPARE(fmt.plotAreaTopMargin(), Ld::PlotFormatBase::defaultChartTopMargin);
    QCOMPARE(fmt.plotAreaBottomMargin(), Ld::PlotFormatBase::defaultChartBottomMargin);
    QCOMPARE(fmt.plotAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotBackgroundAreaColor);
    QCOMPARE(fmt.plotDrawingAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotDrawingAreaBackgroundColor);
    QCOMPARE(fmt.titleFontFormat(), Ld::PlotFormatBase::defaultPlotTitleFontFormat);
    QCOMPARE(fmt.legendLocation(), Ld::PlotFormatBase::defaultLegendLocation);
    QCOMPARE(fmt.drawingAreaOutlineStyle(), Ld::PlotFormatBase::defaultPlotDrawingAreaOutputStyle);

    fmt.setPlotAreaLeftMargin(4.0F);

    QCOMPARE(fmt.isValid(), true);
    QCOMPARE(fmt.chartType(), QString("xy"));
    QCOMPARE(fmt.chartWidth(), 1.0F);
    QCOMPARE(fmt.chartHeight(), 1.0F);
    QCOMPARE(
        fmt.borderLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2.5F, QColor(Qt::red))
    );
    QCOMPARE(fmt.plotAreaLeftMargin(), 4.0F);
    QCOMPARE(fmt.plotAreaRightMargin(), Ld::PlotFormatBase::defaultChartRightMargin);
    QCOMPARE(fmt.plotAreaTopMargin(), Ld::PlotFormatBase::defaultChartTopMargin);
    QCOMPARE(fmt.plotAreaBottomMargin(), Ld::PlotFormatBase::defaultChartBottomMargin);
    QCOMPARE(fmt.plotAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotBackgroundAreaColor);
    QCOMPARE(fmt.plotDrawingAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotDrawingAreaBackgroundColor);
    QCOMPARE(fmt.titleFontFormat(), Ld::PlotFormatBase::defaultPlotTitleFontFormat);
    QCOMPARE(fmt.legendLocation(), Ld::PlotFormatBase::defaultLegendLocation);
    QCOMPARE(fmt.drawingAreaOutlineStyle(), Ld::PlotFormatBase::defaultPlotDrawingAreaOutputStyle);

    fmt.setPlotAreaRightMargin(-5.0F);

    QCOMPARE(fmt.isValid(), false);
    QCOMPARE(fmt.chartType(), QString("xy"));
    QCOMPARE(fmt.chartWidth(), 1.0F);
    QCOMPARE(fmt.chartHeight(), 1.0F);
    QCOMPARE(
        fmt.borderLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2.5F, QColor(Qt::red))
    );
    QCOMPARE(fmt.plotAreaLeftMargin(), 4.0F);
    QCOMPARE(fmt.plotAreaRightMargin(), -5.0F);
    QCOMPARE(fmt.plotAreaTopMargin(), Ld::PlotFormatBase::defaultChartTopMargin);
    QCOMPARE(fmt.plotAreaBottomMargin(), Ld::PlotFormatBase::defaultChartBottomMargin);
    QCOMPARE(fmt.plotAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotBackgroundAreaColor);
    QCOMPARE(fmt.plotDrawingAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotDrawingAreaBackgroundColor);
    QCOMPARE(fmt.titleFontFormat(), Ld::PlotFormatBase::defaultPlotTitleFontFormat);
    QCOMPARE(fmt.legendLocation(), Ld::PlotFormatBase::defaultLegendLocation);
    QCOMPARE(fmt.drawingAreaOutlineStyle(), Ld::PlotFormatBase::defaultPlotDrawingAreaOutputStyle);

    fmt.setPlotAreaRightMargin(5.0F);

    QCOMPARE(fmt.isValid(), true);
    QCOMPARE(fmt.chartType(), QString("xy"));
    QCOMPARE(fmt.chartWidth(), 1.0F);
    QCOMPARE(fmt.chartHeight(), 1.0F);
    QCOMPARE(
        fmt.borderLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2.5F, QColor(Qt::red))
    );
    QCOMPARE(fmt.plotAreaLeftMargin(), 4.0F);
    QCOMPARE(fmt.plotAreaRightMargin(), 5.0F);
    QCOMPARE(fmt.plotAreaTopMargin(), Ld::PlotFormatBase::defaultChartTopMargin);
    QCOMPARE(fmt.plotAreaBottomMargin(), Ld::PlotFormatBase::defaultChartBottomMargin);
    QCOMPARE(fmt.plotAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotBackgroundAreaColor);
    QCOMPARE(fmt.plotDrawingAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotDrawingAreaBackgroundColor);
    QCOMPARE(fmt.titleFontFormat(), Ld::PlotFormatBase::defaultPlotTitleFontFormat);
    QCOMPARE(fmt.legendLocation(), Ld::PlotFormatBase::defaultLegendLocation);
    QCOMPARE(fmt.drawingAreaOutlineStyle(), Ld::PlotFormatBase::defaultPlotDrawingAreaOutputStyle);

    fmt.setPlotAreaTopMargin(-6.0F);

    QCOMPARE(fmt.isValid(), false);
    QCOMPARE(fmt.chartType(), QString("xy"));
    QCOMPARE(fmt.chartWidth(), 1.0F);
    QCOMPARE(fmt.chartHeight(), 1.0F);
    QCOMPARE(
        fmt.borderLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2.5F, QColor(Qt::red))
    );
    QCOMPARE(fmt.plotAreaLeftMargin(), 4.0F);
    QCOMPARE(fmt.plotAreaRightMargin(), 5.0F);
    QCOMPARE(fmt.plotAreaTopMargin(), -6.0F);
    QCOMPARE(fmt.plotAreaBottomMargin(), Ld::PlotFormatBase::defaultChartBottomMargin);
    QCOMPARE(fmt.plotAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotBackgroundAreaColor);
    QCOMPARE(fmt.plotDrawingAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotDrawingAreaBackgroundColor);
    QCOMPARE(fmt.titleFontFormat(), Ld::PlotFormatBase::defaultPlotTitleFontFormat);
    QCOMPARE(fmt.legendLocation(), Ld::PlotFormatBase::defaultLegendLocation);
    QCOMPARE(fmt.drawingAreaOutlineStyle(), Ld::PlotFormatBase::defaultPlotDrawingAreaOutputStyle);

    fmt.setPlotAreaTopMargin(6.0F);

    QCOMPARE(fmt.isValid(), true);
    QCOMPARE(fmt.chartType(), QString("xy"));
    QCOMPARE(fmt.chartWidth(), 1.0F);
    QCOMPARE(fmt.chartHeight(), 1.0F);
    QCOMPARE(
        fmt.borderLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2.5F, QColor(Qt::red))
    );
    QCOMPARE(fmt.plotAreaLeftMargin(), 4.0F);
    QCOMPARE(fmt.plotAreaRightMargin(), 5.0F);
    QCOMPARE(fmt.plotAreaTopMargin(), 6.0F);
    QCOMPARE(fmt.plotAreaBottomMargin(), Ld::PlotFormatBase::defaultChartBottomMargin);
    QCOMPARE(fmt.plotAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotBackgroundAreaColor);
    QCOMPARE(fmt.plotDrawingAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotDrawingAreaBackgroundColor);
    QCOMPARE(fmt.titleFontFormat(), Ld::PlotFormatBase::defaultPlotTitleFontFormat);
    QCOMPARE(fmt.legendLocation(), Ld::PlotFormatBase::defaultLegendLocation);
    QCOMPARE(fmt.drawingAreaOutlineStyle(), Ld::PlotFormatBase::defaultPlotDrawingAreaOutputStyle);

    fmt.setPlotAreaBottomMargin(-7.0F);

    QCOMPARE(fmt.isValid(), false);
    QCOMPARE(fmt.chartType(), QString("xy"));
    QCOMPARE(fmt.chartWidth(), 1.0F);
    QCOMPARE(fmt.chartHeight(), 1.0F);
    QCOMPARE(
        fmt.borderLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2.5F, QColor(Qt::red))
    );
    QCOMPARE(fmt.plotAreaLeftMargin(), 4.0F);
    QCOMPARE(fmt.plotAreaRightMargin(), 5.0F);
    QCOMPARE(fmt.plotAreaTopMargin(), 6.0F);
    QCOMPARE(fmt.plotAreaBottomMargin(), -7.0F);
    QCOMPARE(fmt.plotAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotBackgroundAreaColor);
    QCOMPARE(fmt.plotDrawingAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotDrawingAreaBackgroundColor);
    QCOMPARE(fmt.titleFontFormat(), Ld::PlotFormatBase::defaultPlotTitleFontFormat);
    QCOMPARE(fmt.legendLocation(), Ld::PlotFormatBase::defaultLegendLocation);
    QCOMPARE(fmt.drawingAreaOutlineStyle(), Ld::PlotFormatBase::defaultPlotDrawingAreaOutputStyle);

    fmt.setPlotAreaBottomMargin(7.0F);

    QCOMPARE(fmt.isValid(), true);
    QCOMPARE(fmt.chartType(), QString("xy"));
    QCOMPARE(fmt.chartWidth(), 1.0F);
    QCOMPARE(fmt.chartHeight(), 1.0F);
    QCOMPARE(
        fmt.borderLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2.5F, QColor(Qt::red))
    );
    QCOMPARE(fmt.plotAreaLeftMargin(), 4.0F);
    QCOMPARE(fmt.plotAreaRightMargin(), 5.0F);
    QCOMPARE(fmt.plotAreaTopMargin(), 6.0F);
    QCOMPARE(fmt.plotAreaBottomMargin(), 7.0F);
    QCOMPARE(fmt.plotAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotBackgroundAreaColor);
    QCOMPARE(fmt.plotDrawingAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotDrawingAreaBackgroundColor);
    QCOMPARE(fmt.titleFontFormat(), Ld::PlotFormatBase::defaultPlotTitleFontFormat);
    QCOMPARE(fmt.legendLocation(), Ld::PlotFormatBase::defaultLegendLocation);
    QCOMPARE(fmt.drawingAreaOutlineStyle(), Ld::PlotFormatBase::defaultPlotDrawingAreaOutputStyle);

    fmt.setPlotAreaBackgroundColor(QColor(Qt::yellow));

    QCOMPARE(fmt.isValid(), true);
    QCOMPARE(fmt.chartType(), QString("xy"));
    QCOMPARE(fmt.chartWidth(), 1.0F);
    QCOMPARE(fmt.chartHeight(), 1.0F);
    QCOMPARE(
        fmt.borderLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2.5F, QColor(Qt::red))
    );
    QCOMPARE(fmt.plotAreaLeftMargin(), 4.0F);
    QCOMPARE(fmt.plotAreaRightMargin(), 5.0F);
    QCOMPARE(fmt.plotAreaTopMargin(), 6.0F);
    QCOMPARE(fmt.plotAreaBottomMargin(), 7.0F);
    QCOMPARE(fmt.plotAreaBackgroundColor(), QColor(Qt::yellow));
    QCOMPARE(fmt.plotDrawingAreaBackgroundColor(), Ld::PlotFormatBase::defaultPlotDrawingAreaBackgroundColor);
    QCOMPARE(fmt.titleFontFormat(), Ld::PlotFormatBase::defaultPlotTitleFontFormat);
    QCOMPARE(fmt.legendLocation(), Ld::PlotFormatBase::defaultLegendLocation);
    QCOMPARE(fmt.drawingAreaOutlineStyle(), Ld::PlotFormatBase::defaultPlotDrawingAreaOutputStyle);

    fmt.setPlotDrawingAreaBackgroundColor(QColor(Qt::green));

    QCOMPARE(fmt.isValid(), true);
    QCOMPARE(fmt.chartType(), QString("xy"));
    QCOMPARE(fmt.chartWidth(), 1.0F);
    QCOMPARE(fmt.chartHeight(), 1.0F);
    QCOMPARE(
        fmt.borderLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2.5F, QColor(Qt::red))
    );
    QCOMPARE(fmt.plotAreaLeftMargin(), 4.0F);
    QCOMPARE(fmt.plotAreaRightMargin(), 5.0F);
    QCOMPARE(fmt.plotAreaTopMargin(), 6.0F);
    QCOMPARE(fmt.plotAreaBottomMargin(), 7.0F);
    QCOMPARE(fmt.plotAreaBackgroundColor(), QColor(Qt::yellow));
    QCOMPARE(fmt.plotDrawingAreaBackgroundColor(), QColor(Qt::green));
    QCOMPARE(fmt.titleFontFormat(), Ld::PlotFormatBase::defaultPlotTitleFontFormat);
    QCOMPARE(fmt.legendLocation(), Ld::PlotFormatBase::defaultLegendLocation);
    QCOMPARE(fmt.drawingAreaOutlineStyle(), Ld::PlotFormatBase::defaultPlotDrawingAreaOutputStyle);

    fmt.setTitleFontFormat(Ld::FontFormat("Courier", 24, Ld::FontFormat::Weight::LIGHT, false));

    QCOMPARE(fmt.isValid(), true);
    QCOMPARE(fmt.chartType(), QString("xy"));
    QCOMPARE(fmt.chartWidth(), 1.0F);
    QCOMPARE(fmt.chartHeight(), 1.0F);
    QCOMPARE(
        fmt.borderLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2.5F, QColor(Qt::red))
    );
    QCOMPARE(fmt.plotAreaLeftMargin(), 4.0F);
    QCOMPARE(fmt.plotAreaRightMargin(), 5.0F);
    QCOMPARE(fmt.plotAreaTopMargin(), 6.0F);
    QCOMPARE(fmt.plotAreaBottomMargin(), 7.0F);
    QCOMPARE(fmt.plotAreaBackgroundColor(), QColor(Qt::yellow));
    QCOMPARE(fmt.plotDrawingAreaBackgroundColor(), QColor(Qt::green));
    QCOMPARE(fmt.titleFontFormat(), Ld::FontFormat("Courier", 24, Ld::FontFormat::Weight::LIGHT, false));
    QCOMPARE(fmt.legendLocation(), Ld::PlotFormatBase::defaultLegendLocation);
    QCOMPARE(fmt.drawingAreaOutlineStyle(), Ld::PlotFormatBase::defaultPlotDrawingAreaOutputStyle);

    fmt.setLegendLocation(Ld::PlotFormatBase::LegendLocation::LEFT);

    QCOMPARE(fmt.isValid(), true);
    QCOMPARE(fmt.chartType(), QString("xy"));
    QCOMPARE(fmt.chartWidth(), 1.0F);
    QCOMPARE(fmt.chartHeight(), 1.0F);
    QCOMPARE(
        fmt.borderLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2.5F, QColor(Qt::red))
    );
    QCOMPARE(fmt.plotAreaLeftMargin(), 4.0F);
    QCOMPARE(fmt.plotAreaRightMargin(), 5.0F);
    QCOMPARE(fmt.plotAreaTopMargin(), 6.0F);
    QCOMPARE(fmt.plotAreaBottomMargin(), 7.0F);
    QCOMPARE(fmt.plotAreaBackgroundColor(), QColor(Qt::yellow));
    QCOMPARE(fmt.plotDrawingAreaBackgroundColor(), QColor(Qt::green));
    QCOMPARE(fmt.titleFontFormat(), Ld::FontFormat("Courier", 24, Ld::FontFormat::Weight::LIGHT, false));
    QCOMPARE(fmt.legendLocation(), Ld::PlotFormatBase::LegendLocation::LEFT);
    QCOMPARE(fmt.drawingAreaOutlineStyle(), Ld::PlotFormatBase::defaultPlotDrawingAreaOutputStyle);

    fmt.setDrawingAreaOutlineStyle(Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 4, QColor(Qt::red)));

    QCOMPARE(fmt.isValid(), true);
    QCOMPARE(fmt.chartType(), QString("xy"));
    QCOMPARE(fmt.chartWidth(), 1.0F);
    QCOMPARE(fmt.chartHeight(), 1.0F);
    QCOMPARE(
        fmt.borderLineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2.5F, QColor(Qt::red))
    );
    QCOMPARE(fmt.plotAreaLeftMargin(), 4.0F);
    QCOMPARE(fmt.plotAreaRightMargin(), 5.0F);
    QCOMPARE(fmt.plotAreaTopMargin(), 6.0F);
    QCOMPARE(fmt.plotAreaBottomMargin(), 7.0F);
    QCOMPARE(fmt.plotAreaBackgroundColor(), QColor(Qt::yellow));
    QCOMPARE(fmt.plotDrawingAreaBackgroundColor(), QColor(Qt::green));
    QCOMPARE(fmt.titleFontFormat(), Ld::FontFormat("Courier", 24, Ld::FontFormat::Weight::LIGHT, false));
    QCOMPARE(fmt.legendLocation(), Ld::PlotFormatBase::LegendLocation::LEFT);
    QCOMPARE(
        fmt.drawingAreaOutlineStyle(),
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 4, QColor(Qt::red))
    );
}


void TestPlotFormatBase::testToStringMethod() {
    Ld::PlotFormatBase fmt;

    fmt.setChartType(QString("xy"));
    fmt.setChartWidth(1.0F);
    fmt.setChartHeight(2.0F);
    fmt.setBorderLineStyle(Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2.5F, QColor(Qt::red)));
    fmt.setPlotAreaLeftMargin(5.0F);
    fmt.setPlotAreaRightMargin(6.0F);
    fmt.setPlotAreaTopMargin(7.0F);
    fmt.setPlotAreaBottomMargin(8.0F);
    fmt.setPlotAreaBackgroundColor(QColor(Qt::green));
    fmt.setPlotDrawingAreaBackgroundColor(QColor(Qt::cyan));
    fmt.setTitleFontFormat(Ld::FontFormat("Courier", 24, Ld::FontFormat::Weight::BLACK, true));
    fmt.setLegendLocation(Ld::PlotFormatBase::LegendLocation::LEFT);
    fmt.setDrawingAreaOutlineStyle(Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASHED_LINE, 3, QColor(Qt::red)));

    QString s = fmt.toString();
    QCOMPARE(
        s,
        QString(
            "PlotFormatBase,xy,1,2,DOTTED_LINE,2.5,#ff0000,5,6,7,8,#00ff00,#00ffff,"
            "Courier,24,0.87,I,0,-,-,"
            "LEFT,"
            "DASHED_LINE,3,#ff0000"
        )
    );
}


void TestPlotFormatBase::testFormatAggregator() {
    QSharedPointer<Ld::PlotFormatBase> fmt1(new Ld::PlotFormatBase);
    QSharedPointer<Ld::PlotFormatBase> fmt2(new Ld::PlotFormatBase);
    QSharedPointer<Ld::PlotFormatBase> fmt3(new Ld::PlotFormatBase);

    fmt1->setChartType(QString("xy"));
    fmt1->setChartWidth(1.0F);
    fmt1->setChartHeight(2.0F);
    fmt1->setBorderLineStyle(Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2.5F, QColor(Qt::red)));
    fmt1->setPlotAreaLeftMargin(5.0F);
    fmt1->setPlotAreaRightMargin(16.0F);
    fmt1->setPlotAreaTopMargin(7.0F);
    fmt1->setPlotAreaBottomMargin(18.0F);
    fmt1->setPlotAreaBackgroundColor(QColor(Qt::green));
    fmt1->setPlotDrawingAreaBackgroundColor(QColor(Qt::gray));
    fmt1->setTitleFontFormat(Ld::FontFormat("Courier", 24, Ld::FontFormat::Weight::BLACK, true));
    fmt1->setLegendLocation(Ld::PlotFormatBase::LegendLocation::RIGHT);
    fmt1->setDrawingAreaOutlineStyle(
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 3, QColor(Qt::red))
    );

    fmt2->setChartType(QString("xy"));
    fmt2->setChartWidth(4.0F);
    fmt2->setChartHeight(2.0F);
    fmt2->setBorderLineStyle(Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASHED_LINE, 2.5F, QColor(Qt::red)));
    fmt2->setPlotAreaLeftMargin(5.0F);
    fmt2->setPlotAreaRightMargin(6.0F);
    fmt2->setPlotAreaTopMargin(17.0F);
    fmt2->setPlotAreaBottomMargin(18.0F);
    fmt2->setPlotAreaBackgroundColor(QColor(Qt::blue));
    fmt2->setPlotDrawingAreaBackgroundColor(QColor(Qt::cyan));
    fmt2->setTitleFontFormat(Ld::FontFormat("Courier", 24, Ld::FontFormat::Weight::BLACK, true));
    fmt2->setLegendLocation(Ld::PlotFormatBase::LegendLocation::LEFT);
    fmt2->setDrawingAreaOutlineStyle(
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASHED_LINE, 3, QColor(Qt::red))
    );

    fmt3->setChartType(QString("bar"));
    fmt3->setChartWidth(4.0F);
    fmt3->setChartHeight(5.0F);
    fmt3->setBorderLineStyle(Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 2.5F, QColor(Qt::red)));
    fmt3->setPlotAreaLeftMargin(15.0F);
    fmt3->setPlotAreaRightMargin(16.0F);
    fmt3->setPlotAreaTopMargin(7.0F);
    fmt3->setPlotAreaBottomMargin(8.0F);
    fmt3->setPlotAreaBackgroundColor(QColor(Qt::green));
    fmt3->setPlotDrawingAreaBackgroundColor(QColor(Qt::cyan));
    fmt3->setTitleFontFormat(Ld::FontFormat("Helvetica", 24, Ld::FontFormat::Weight::BLACK, true));
    fmt3->setLegendLocation(Ld::PlotFormatBase::LegendLocation::LEFT);
    fmt3->setDrawingAreaOutlineStyle(
        Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 3, QColor(Qt::red))
    );

    Ld::PlotFormatBase::Aggregation aggregation;

    aggregation.addFormat(fmt1);
    aggregation.addFormat(fmt2);
    aggregation.addFormat(fmt3);

    const QSet<QString>&                            chartTypes        = aggregation.chartTypes();
    const QSet<float>&                              chartWidths       = aggregation.chartWidths();
    const QSet<float>&                              chartHeights      = aggregation.chartHeights();
    const QSet<Ld::ChartLineStyle>&                 borderLineStyles  = aggregation.borderLineStyles();
    const QSet<float>&                              leftMargins       = aggregation.plotAreaLeftMargins();
    const QSet<float>&                              rightMargins      = aggregation.plotAreaRightMargins();
    const QSet<float>&                              topMargins        = aggregation.plotAreaTopMargins();
    const QSet<float>&                              bottomMargins     = aggregation.plotAreaBottomMargins();
    const QSet<QColor>&                             backgroundColors  = aggregation.plotBackgroundAreaColors();
    const QSet<QColor>&                             drawingAreaColors = aggregation.plotDrawingAreaBackgroundColors();
    const QSet<Ld::FontFormat>&                     titleFontFormats  = aggregation.titleFontFormats();
    const QSet<Ld::PlotFormatBase::LegendLocation>& legendLocations   = aggregation.legendLocations();
    const QSet<Ld::ChartLineStyle>&                 outlineStyles     = aggregation.plotDrawingAreaOutlineStyles();

    QCOMPARE(chartTypes.size(), 2);
    QCOMPARE(chartWidths.size(), 2);
    QCOMPARE(chartHeights.size(), 2);
    QCOMPARE(borderLineStyles.size(), 2);
    QCOMPARE(leftMargins.size(), 2);
    QCOMPARE(rightMargins.size(), 2);
    QCOMPARE(topMargins.size(), 2);
    QCOMPARE(bottomMargins.size(), 2);
    QCOMPARE(backgroundColors.size(), 2);
    QCOMPARE(drawingAreaColors.size(), 2);
    QCOMPARE(titleFontFormats.size(), 2);
    QCOMPARE(legendLocations.size(), 2);
    QCOMPARE(outlineStyles.size(), 2);

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

    QCOMPARE(leftMargins.contains( 5.0F), true);
    QCOMPARE(leftMargins.contains(15.0F), true);

    QCOMPARE(rightMargins.contains( 6.0F), true);
    QCOMPARE(rightMargins.contains(16.0F), true);

    QCOMPARE(topMargins.contains( 7.0F), true);
    QCOMPARE(topMargins.contains(17.0F), true);

    QCOMPARE(bottomMargins.contains( 8.0F), true);
    QCOMPARE(bottomMargins.contains(18.0F), true);

    QCOMPARE(backgroundColors.contains(Qt::green), true);
    QCOMPARE(backgroundColors.contains(Qt::blue), true);

    QCOMPARE(drawingAreaColors.contains(Qt::gray), true);
    QCOMPARE(drawingAreaColors.contains(Qt::cyan), true);

    QCOMPARE(titleFontFormats.contains(Ld::FontFormat("Helvetica", 24, Ld::FontFormat::Weight::BLACK, true)), true);
    QCOMPARE(titleFontFormats.contains(Ld::FontFormat("Courier", 24, Ld::FontFormat::Weight::BLACK, true)), true);

    QCOMPARE(legendLocations.contains(Ld::PlotFormatBase::LegendLocation::LEFT), true);
    QCOMPARE(legendLocations.contains(Ld::PlotFormatBase::LegendLocation::RIGHT), true);

    QCOMPARE(
        outlineStyles.contains(Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DASHED_LINE, 3, QColor(Qt::red))),
        true
    );
    QCOMPARE(
        outlineStyles.contains(Ld::ChartLineStyle(Ld::ChartLineStyle::LineStyle::DOTTED_LINE, 3, QColor(Qt::red))),
        true
    );
}

