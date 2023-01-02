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
* This file implements tests for the \ref Ld::FormatOrganizer class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QMap>
#include <QtTest/QtTest>

#include <cassert>

#include <ld_format.h>
#include <ld_character_format.h>
#include <ld_page_format.h>
#include <ld_text_block_format.h>

#include <ld_format_container.h>
#include <ld_format_organizer.h>

#include "test_format_organizer.h"

TestFormatOrganizer::TestFormatOrganizer() {}

TestFormatOrganizer::~TestFormatOrganizer() {}

void TestFormatOrganizer::initTestCase() {}

void TestFormatOrganizer::testIdentifiersByFormat() {
    Ld::FormatOrganizer organizer = initializeOrganizer();

    Ld::FormatIdentifiersByFormat identifiersByFormat = organizer.identifiersByFormat();

    Ld::CharacterFormat* f1 = new Ld::CharacterFormat("Helvetica");
    Ld::CharacterFormat* f2 = new Ld::CharacterFormat("Arial");
    Ld::CharacterFormat* f3 = new Ld::CharacterFormat("Courier");
    Ld::PageFormat*      f4 = new Ld::PageFormat(8.5 * 72.0, 11.0 * 72.0);

    f4->setTopMargin(72.0);
    f4->setRightMargin(72.0);
    f4->setBottomMargin(72.0);
    f4->setLeftMargin(72.0);

    Ld::FormatContainer format1(f1);
    Ld::FormatContainer format2(f2);
    Ld::FormatContainer format3(f3);
    Ld::FormatContainer format4(f4);

    delete f1;
    delete f2;
    delete f3;
    delete f4;

    QVERIFY(identifiersByFormat.count() == 4);
    QVERIFY(identifiersByFormat.contains(format1));
    QVERIFY(identifiersByFormat.contains(format2));
    QVERIFY(identifiersByFormat.contains(format3));
    QVERIFY(identifiersByFormat.contains(format4));

    QVERIFY(identifiersByFormat.value(format1) == 0); // Helvetica char format was most commonly used.
    QVERIFY(identifiersByFormat.value(format2) == 1); // Arial char format was next most commonly used.
    QVERIFY(identifiersByFormat.value(format3) == 2 || identifiersByFormat.value(format3) == 3);
    QVERIFY(identifiersByFormat.value(format4) == 2 || identifiersByFormat.value(format4) == 3);
    QVERIFY(identifiersByFormat.value(format3) != identifiersByFormat.value(format4));
}


void TestFormatOrganizer::testFormatsByIdentifier() {
    Ld::FormatOrganizer organizer = initializeOrganizer();

    Ld::FormatsByIdentifier formatsByIdentifier = organizer.formatsByIdentifier();

    Ld::CharacterFormat* f1 = new Ld::CharacterFormat("Helvetica");
    Ld::CharacterFormat* f2 = new Ld::CharacterFormat("Arial");
    Ld::CharacterFormat* f3 = new Ld::CharacterFormat("Courier");
    Ld::PageFormat*      f4 = new Ld::PageFormat(8.5 * 72.0, 11.0 * 72.0);

    f4->setTopMargin(72.0);
    f4->setRightMargin(72.0);
    f4->setBottomMargin(72.0);
    f4->setLeftMargin(72.0);

    Ld::FormatContainer format1(f1);
    Ld::FormatContainer format2(f2);
    Ld::FormatContainer format3(f3);
    Ld::FormatContainer format4(f4);

    delete f1;
    delete f2;
    delete f3;
    delete f4;

    QVERIFY(formatsByIdentifier.count() == 4);
    QVERIFY(formatsByIdentifier.value(0) == format1);
    QVERIFY(formatsByIdentifier.value(1) == format2);
    QVERIFY(formatsByIdentifier.value(2) == format3 || formatsByIdentifier.value(2) == format4);
    QVERIFY(formatsByIdentifier.value(3) == format3 || formatsByIdentifier.value(3) == format4);
    QVERIFY(formatsByIdentifier.value(2) != formatsByIdentifier.value(3));
}


void TestFormatOrganizer::testFormatParedo() {
    Ld::FormatOrganizer organizer = initializeOrganizer();

    Ld::FormatParedos paredos = organizer.formatParedos();

    Ld::CharacterFormat* f1 = new Ld::CharacterFormat("Helvetica");
    Ld::CharacterFormat* f2 = new Ld::CharacterFormat("Arial");
    Ld::CharacterFormat* f3 = new Ld::CharacterFormat("Courier");
    Ld::PageFormat*      f4 = new Ld::PageFormat(8.5 * 72.0, 11.0 * 72.0);

    f4->setTopMargin(72.0);
    f4->setRightMargin(72.0);
    f4->setBottomMargin(72.0);
    f4->setLeftMargin(72.0);

    Ld::FormatContainer format1(f1);
    Ld::FormatContainer format2(f2);
    Ld::FormatContainer format3(f3);
    Ld::FormatContainer format4(f4);

    delete f1;
    delete f2;
    delete f3;
    delete f4;

    QVERIFY(paredos.count() == 2); // We have 2 types of formats.
    QVERIFY(paredos.contains(QString("CharacterFormat")));
    QVERIFY(paredos.contains(QString("PageFormat")));

    typedef QMap<Ld::FormatIdentifier, Ld::FormatContainer> FormatParedo;
    FormatParedo paredo1 = paredos.value(QString("CharacterFormat"));
    FormatParedo paredo2 = paredos.value(QString("PageFormat"));

    QVERIFY(paredo1.count() == 3);
    QVERIFY(paredo2.count() == 1);
    QVERIFY(paredo1.contains(0));
    QVERIFY(paredo1.contains(1));
    QVERIFY(paredo1.contains(2) || paredo1.contains(3));

    if (paredo1.contains(2)) {
        QVERIFY(paredo2.contains(3));
    } else {
        QVERIFY(paredo2.contains(2));
    }

    QVERIFY(paredo1.value(0) == format1);
    QVERIFY(paredo1.value(1) == format2);

    if (paredo1.contains(2)) {
        QVERIFY(paredo1.value(2) == format3 || paredo1.value(2) == format4);
        QVERIFY(paredo2.value(3) == format3 || paredo2.value(3) == format4);
        QVERIFY(paredo1.value(2) != paredo2.value(3));
    } else {
        QVERIFY(paredo1.value(3) == format3 || paredo1.value(3) == format4);
        QVERIFY(paredo2.value(2) == format3 || paredo2.value(2) == format4);
        QVERIFY(paredo1.value(3) != paredo2.value(2));
    }
}


Ld::FormatOrganizer TestFormatOrganizer::initializeOrganizer() {
    Ld::FormatOrganizer organizer;

    Ld::CharacterFormat* f1 = new Ld::CharacterFormat("Helvetica");
    Ld::CharacterFormat* f2 = new Ld::CharacterFormat("Arial");
    Ld::CharacterFormat* f3 = new Ld::CharacterFormat("Helvetica");
    Ld::PageFormat*      f4 = new Ld::PageFormat(8.5 * 72.0, 11.0 * 72.0);
    Ld::CharacterFormat* f5 = new Ld::CharacterFormat("Helvetica");
    Ld::CharacterFormat* f6 = new Ld::CharacterFormat("Arial");
    Ld::CharacterFormat* f7 = new Ld::CharacterFormat("Courier");

    f4->setTopMargin(72.0);
    f4->setRightMargin(72.0);
    f4->setBottomMargin(72.0);
    f4->setLeftMargin(72.0);

    Ld::FormatContainer container1(f1);
    Ld::FormatContainer container2(f2);
    Ld::FormatContainer container3(f3);
    Ld::FormatContainer container4(f4);
    Ld::FormatContainer container5(f5);
    Ld::FormatContainer container6(f6);
    Ld::FormatContainer container7(f7);

    delete f1;
    delete f2;
    delete f3;
    delete f4;
    delete f5;
    delete f6;
    delete f7;

    organizer.addFormat(container1);
    organizer.addFormat(container2);
    organizer.addFormat(container3);
    organizer.addFormat(container4);
    organizer.addFormat(container5);
    organizer.addFormat(container6);
    organizer.addFormat(container7);

    return organizer;
}
