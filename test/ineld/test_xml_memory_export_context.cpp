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
* This file implements tests of \ref Ld::XmlMemoryExportContext class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QtTest/QtTest>

#include <cstdint>
#include <limits>
#include <cmath>

#include <ld_handle.h>
#include <ld_xml_attributes.h>
#include <ld_xml_memory_export_context.h>

#include "test_xml_memory_export_context.h"


TestXmlMemoryExportContext::TestXmlMemoryExportContext() {}


TestXmlMemoryExportContext::~TestXmlMemoryExportContext() {}


void TestXmlMemoryExportContext::testConstructors() {
    Ld::XmlMemoryExportContext context("index.html");
    QVERIFY(context.payloads().contains("index.html"));

    context.writeStartDocument();

    QString received(context.payload("index.html"));
    QCOMPARE(
        received,
        QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>")
    );
}


void TestXmlMemoryExportContext::testXmlEngine() {
    // Basically tested by the testConstructors test.
}


void TestXmlMemoryExportContext::testPayloads() {
    Ld::XmlMemoryExportContext context("index.html");
    QVERIFY(context.payloads().contains("index.html"));

    context.writeStartDocument();

    QByteArray byteArray("Foo !");
    context.addPayload("foo", byteArray);

    QCOMPARE(context.payloads().count(), 2);
    QVERIFY(context.payloads().contains("index.html"));
    QVERIFY(context.payloads().contains("foo"));

    QString received1(context.payload("index.html"));
    QCOMPARE(received1, QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>"));

    QString received2(context.payload("foo"));
    QCOMPARE(received2, QString("Foo !"));
}
