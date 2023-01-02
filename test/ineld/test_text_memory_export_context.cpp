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
* This file implements tests of \ref Ld::TextMemoryExportContext class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QtTest/QtTest>

#include <ld_text_memory_export_context.h>

#include "test_text_memory_export_context.h"


TestTextMemoryExportContext::TestTextMemoryExportContext() {}


TestTextMemoryExportContext::~TestTextMemoryExportContext() {}


void TestTextMemoryExportContext::testConstructors() {
    Ld::TextMemoryExportContext context("document.tex");
    QVERIFY(context.payloads().contains("document.tex"));

    context << "foo";

    QString received(context.payload("document.tex"));
    QCOMPARE(received, QString("foo"));
}


void TestTextMemoryExportContext::testTextExport() {
    Ld::TextMemoryExportContext context("document.tex");

    context << "foo";
    context << 'b';
    context << QString("ar");

    QString received(context.payload("document.tex"));
    QCOMPARE(received, QString("foobar"));
}


void TestTextMemoryExportContext::testPayloads() {
    Ld::TextMemoryExportContext context("document.tex");
    QVERIFY(context.payloads().contains("document.tex"));

    context << "foo";

    QByteArray byteArray("preamble\n");
    context.addPayload("preamble.tex", byteArray);

    QCOMPARE(context.payloads().count(), 2);
    QVERIFY(context.payloads().contains("document.tex"));
    QVERIFY(context.payloads().contains("preamble.tex"));

    QString received1(context.payload("document.tex"));
    QCOMPARE(received1, QString("foo"));

    QString received2(context.payload("preamble.tex"));
    QCOMPARE(received2, QString("preamble\n"));
}
