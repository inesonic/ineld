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
* This file implements tests of the Ld::PageFormat class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QPageLayout>
#include <QMarginsF>
#include <QList>
#include <QtTest/QtTest>

#include <util_page_size.h>

#include <ld_format_structures.h>
#include <ld_page_format.h>

#include "test_page_format.h"

TestPageFormat::TestPageFormat() {}


TestPageFormat::~TestPageFormat() {}


void TestPageFormat::initTestCase() {}


void TestPageFormat::testConstructorsAndDestructors() {
    Ld::PageFormat layout1;
    QVERIFY(layout1.isValid());
    QVERIFY(layout1.isPageInvalid() == false);
    QVERIFY(layout1.areMarginsInvalid() == false);

    Ld::PageFormat layout2(8.5*72.0, 11.0*72.0);
    QVERIFY(layout2.isInvalid() == false);
    QVERIFY(layout2.isPageValid());
    QVERIFY(layout2.areMarginsInvalid() == false);

    Ld::PageFormat layout3(QString("US Letter"));
    QVERIFY(layout3.isInvalid() == false);
    QVERIFY(layout3.isPageValid());
    QVERIFY(layout3.areMarginsInvalid() == false);

    Ld::PageFormat layout4("US Letter");
    QVERIFY(layout4.isInvalid() == false);
    QVERIFY(layout4.isPageValid());
    QVERIFY(layout4.areMarginsInvalid() == false);

    Ld::PageFormat layout5(layout4);
    QVERIFY(layout3.isInvalid() == false);
    QVERIFY(layout3.isPageValid());
    QVERIFY(layout3.areMarginsInvalid() == false);

    Util::PageSize pageSize("US Ledger");
    Ld::PageFormat layout6(pageSize);
    QVERIFY(layout3.isInvalid() == false);
    QVERIFY(layout3.isPageValid());
    QVERIFY(layout3.areMarginsInvalid() == false);

    QPageLayout qPageLayout(
        QPageSize(QPageSize::Ledger),
        QPageLayout::Landscape,
        QMarginsF(72.0,72.0,72.0,72.0),
        QPageLayout::Point
    );
    Ld::PageFormat layout7(qPageLayout);
    QVERIFY(layout7.isValid());
    QVERIFY(layout7.isPageValid());
    QVERIFY(layout7.areMarginsValid());
}


void TestPageFormat::testValidInvalidMethods() {
    Ld::PageFormat layout;

    QVERIFY(layout.isValid() == true);
    QVERIFY(layout.isInvalid() == false);
    QVERIFY(layout.isPageInvalid() == false);
    QVERIFY(layout.isPageValid() == true);
    QVERIFY(layout.areMarginsInvalid() == false);
    QVERIFY(layout.areMarginsValid() == true);

    layout.setLeftMargin(-1);

    QVERIFY(layout.isInvalid() == true);
    QVERIFY(layout.isValid() == false);
    QVERIFY(layout.isPageInvalid() == false);
    QVERIFY(layout.isPageValid() == true);
    QVERIFY(layout.areMarginsInvalid() == true);
    QVERIFY(layout.areMarginsValid() == false);

    layout.setLeftMargin(72.0);

    QVERIFY(layout.isValid() == true);
    QVERIFY(layout.isInvalid() == false);
    QVERIFY(layout.isPageInvalid() == false);
    QVERIFY(layout.isPageValid() == true);
    QVERIFY(layout.areMarginsInvalid() == false);
    QVERIFY(layout.areMarginsValid() == true);

    layout.setRightMargin(-1);

    QVERIFY(layout.isInvalid() == true);
    QVERIFY(layout.isValid() == false);
    QVERIFY(layout.isPageInvalid() == false);
    QVERIFY(layout.isPageValid() == true);
    QVERIFY(layout.areMarginsInvalid() == true);
    QVERIFY(layout.areMarginsValid() == false);

    layout.setRightMargin(72.0);

    QVERIFY(layout.isValid() == true);
    QVERIFY(layout.isInvalid() == false);
    QVERIFY(layout.isPageInvalid() == false);
    QVERIFY(layout.isPageValid() == true);
    QVERIFY(layout.areMarginsInvalid() == false);
    QVERIFY(layout.areMarginsValid() == true);

    layout.setTopMargin(-1);

    QVERIFY(layout.isInvalid() == true);
    QVERIFY(layout.isValid() == false);
    QVERIFY(layout.isPageInvalid() == false);
    QVERIFY(layout.isPageValid() == true);
    QVERIFY(layout.areMarginsInvalid() == true);
    QVERIFY(layout.areMarginsValid() == false);

    layout.setTopMargin(72.0);

    QVERIFY(layout.isValid() == true);
    QVERIFY(layout.isInvalid() == false);
    QVERIFY(layout.isPageInvalid() == false);
    QVERIFY(layout.isPageValid() == true);
    QVERIFY(layout.areMarginsInvalid() == false);
    QVERIFY(layout.areMarginsValid() == true);

    layout.setBottomMargin(-1);

    QVERIFY(layout.isInvalid() == true);
    QVERIFY(layout.isValid() == false);
    QVERIFY(layout.isPageInvalid() == false);
    QVERIFY(layout.isPageValid() == true);
    QVERIFY(layout.areMarginsInvalid() == true); // Need a valid page size to test margins.
    QVERIFY(layout.areMarginsValid() == false);

    layout.setBottomMargin(72.0);

    QVERIFY(layout.isValid() == true);
    QVERIFY(layout.isInvalid() == false);
    QVERIFY(layout.isPageInvalid() == false);
    QVERIFY(layout.isPageValid() == true);
    QVERIFY(layout.areMarginsInvalid() == false);
    QVERIFY(layout.areMarginsValid() == true);

    layout.setPageSize(Util::PageSize());

    QVERIFY(layout.isValid() == false);
    QVERIFY(layout.isInvalid() == true);
    QVERIFY(layout.isPageInvalid() == true);
    QVERIFY(layout.isPageValid() == false);
    QVERIFY(layout.areMarginsInvalid() == true);
    QVERIFY(layout.areMarginsValid() == false);
}


void TestPageFormat::testAccessors() {
    Ld::PageFormat layout;

    QVERIFY(layout.leftMargin() == 72);
    QVERIFY(layout.rightMargin() == 72);
    QVERIFY(layout.topMargin() == 72);
    QVERIFY(layout.bottomMargin() == 72);
    QVERIFY(layout.landscape() == false);
    QVERIFY(layout.portrait() == true);

    layout.setLeftMargin(72.0);
    layout.setRightMargin(73.0);
    layout.setTopMargin(74.0);
    layout.setBottomMargin(75.0);

    QVERIFY(layout.leftMargin() == 72.0);
    QVERIFY(layout.rightMargin() == 73.0);
    QVERIFY(layout.topMargin() == 74.0);
    QVERIFY(layout.bottomMargin() == 75.0);

    layout.setPageSize(Util::PageSize("US Letter"));
    QVERIFY(layout.pageHeight() == 11.0 * 72.0);
    QVERIFY(layout.pageWidth() == 8.5 * 72.0);

    QVERIFY(layout.horizontalExtents() == 8.5 * 72.0);
    QVERIFY(layout.verticalExtents() == 11.0 * 72.0);
    QVERIFY(layout.horzontalActiveArea() == 8.5 * 72.0 - 72.0 - 73.0);
    QVERIFY(layout.verticalActiveArea() == 11.0 * 72.0 - 74.0 - 75.0);

    layout.setLandscape();

    QVERIFY(layout.landscape() == true);
    QVERIFY(layout.portrait() == false);

    QVERIFY(layout.horizontalExtents() == 11.0 * 72.0);
    QVERIFY(layout.verticalExtents() == 8.5 * 72.0);
    QVERIFY(layout.horzontalActiveArea() == 11.0 * 72.0 - 72.0 - 73.0);
    QVERIFY(layout.verticalActiveArea() == 8.5 * 72.0 - 74.0 - 75.0);

    layout.setPortrait();

    QVERIFY(layout.landscape() == false);
    QVERIFY(layout.portrait() == true);

    layout.setLandscape(true);

    QVERIFY(layout.landscape() == true);
    QVERIFY(layout.portrait() == false);

    layout.setLandscape(false);

    QVERIFY(layout.landscape() == false);
    QVERIFY(layout.portrait() == true);

    layout.setPortrait(false);

    QVERIFY(layout.landscape() == true);
    QVERIFY(layout.portrait() == false);

    layout.setPortrait(true);

    QVERIFY(layout.landscape() == false);
    QVERIFY(layout.portrait() == true);
}


void TestPageFormat::testStringConversions() {
    Ld::PageFormat layout1("US Letter");
    Ld::PageFormat layout2;

    QString layoutString = layout2.toString();
    QVERIFY(layoutString == QString("PageFormat,US Letter,72,72,72,72,portrait"));

    bool ok = false;
    layout2 = Ld::PageFormat::fromString(QString("Invalid"), &ok);
    QVERIFY(ok);
    QVERIFY(layout2.isInvalid());

    layout1.setLeftMargin(72.0);
    layout1.setLeftMargin(72.0);
    layout1.setRightMargin(73.0);
    layout1.setTopMargin(74.0);
    layout1.setBottomMargin(75.0);
    layout1.setPortrait();

    QVERIFY(layout1.toString() != layout2.toString());

    layoutString = layout1.toString();
    ok           = false;

    layout2 = Ld::PageFormat::fromString(layoutString, &ok);
    QVERIFY(ok);
    QVERIFY(layout1.toString() == layout2.toString());

    layout2 = Ld::PageFormat::fromString(QString("Fubar"), &ok);
    QVERIFY(!ok);
    QVERIFY(layout2.isInvalid());

    layout2 = Ld::PageFormat::fromString(QString("Invalid"), &ok);
    QVERIFY(ok);
    QVERIFY(layout2.isInvalid());
}


void TestPageFormat::testQPageLayoutMethods() {
    QPageLayout qPageLayout(
        QPageSize(QPageSize::Ledger),
        QPageLayout::Landscape,
        QMarginsF(72.0,74.0,73.0,75.0),
        QPageLayout::Point
    );
    Ld::PageFormat layout(qPageLayout);

    QVERIFY(layout.isValid());
    QVERIFY(layout.isPageValid());
    QVERIFY(layout.areMarginsValid());

    QVERIFY(layout.leftMargin() == 72.0);
    QVERIFY(layout.rightMargin() == 73.0);
    QVERIFY(layout.topMargin() == 74.0);
    QVERIFY(layout.bottomMargin() == 75.0);
    QVERIFY(layout.landscape());

    QPageLayout qPageLayout2 = layout.toQPageLayout();

    QVERIFY(qPageLayout2.isValid());
    QVERIFY(qPageLayout2.margins(QPageLayout::Point).left() == 72.0);
    QVERIFY(qPageLayout2.margins(QPageLayout::Point).right() == 73.0);
    QVERIFY(qPageLayout2.margins(QPageLayout::Point).top() == 74.0);
    QVERIFY(qPageLayout2.margins(QPageLayout::Point).bottom() == 75.0);
    QVERIFY(qPageLayout2.orientation() == QPageLayout::Landscape);
}


void TestPageFormat::testFormatAggregator() {
    QSharedPointer<Ld::PageFormat> format1(new Ld::PageFormat("US Letter"));
    format1->setLandscape();
    format1->setLeftMargin(10);
    format1->setRightMargin(11);
    format1->setTopMargin(12);
    format1->setBottomMargin(13);

    Ld::FormatPointerSet formats;

    Ld::PageFormat::Aggregation aggregation;
    {
        bool success = aggregation.addFormat(format1);
        QVERIFY(success);

        QSharedPointer<Ld::PageFormat> format2(new Ld::PageFormat("US Ledger"));
        format2->setLandscape();
        format2->setLeftMargin(20);
        format2->setRightMargin(21);
        format2->setTopMargin(22);
        format2->setBottomMargin(23);

        success = aggregation.addFormat(format2);
        QVERIFY(success);

        QSharedPointer<Ld::PageFormat> format3(new Ld::PageFormat("US Legal"));
        format3->setLandscape();
        format3->setLeftMargin(30);
        format3->setRightMargin(31);
        format3->setTopMargin(32);
        format3->setBottomMargin(33);

        success = aggregation.addFormat(format3);
        QVERIFY(success);

        formats = aggregation.formats();
        QCOMPARE(formats.size(), 3);
        QVERIFY(formats.contains(format1));
        QVERIFY(formats.contains(format2));
        QVERIFY(formats.contains(format3));

        QCOMPARE(aggregation.pageSizes().size(), 3);
        QVERIFY(aggregation.pageSizes().contains(Util::PageSize("US Letter")));
        QVERIFY(aggregation.pageSizes().contains(Util::PageSize("US Ledger")));
        QVERIFY(aggregation.pageSizes().contains(Util::PageSize("US Legal")));

        QVERIFY(aggregation.landscape().allTrue());
        QVERIFY(aggregation.portrait().allFalse());

        QCOMPARE(aggregation.leftMargins().size(), 3);
        QVERIFY(aggregation.leftMargins().contains(10));
        QVERIFY(aggregation.leftMargins().contains(20));
        QVERIFY(aggregation.leftMargins().contains(30));

        QCOMPARE(aggregation.rightMargins().size(), 3);
        QVERIFY(aggregation.rightMargins().contains(11));
        QVERIFY(aggregation.rightMargins().contains(21));
        QVERIFY(aggregation.rightMargins().contains(31));

        QCOMPARE(aggregation.topMargins().size(), 3);
        QVERIFY(aggregation.topMargins().contains(12));
        QVERIFY(aggregation.topMargins().contains(22));
        QVERIFY(aggregation.topMargins().contains(32));

        QCOMPARE(aggregation.bottomMargins().size(), 3);
        QVERIFY(aggregation.bottomMargins().contains(13));
        QVERIFY(aggregation.bottomMargins().contains(23));
        QVERIFY(aggregation.bottomMargins().contains(33));

        success = aggregation.removeFormat(format3);
        QVERIFY(success);

        success = aggregation.removeFormat(format3);
        QVERIFY(!success);

        formats = aggregation.formats();
        QCOMPARE(formats.size(), 2);
        QVERIFY(formats.contains(format1));
        QVERIFY(formats.contains(format2));

        formats.clear();
    }

    formats = aggregation.formats();
    QCOMPARE(formats.size(), 1);
    QVERIFY(formats.contains(format1));
}
