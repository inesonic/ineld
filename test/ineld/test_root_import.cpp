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
* This file implements tests of the Ld::RootImport class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QFileInfo>
#include <QtTest/QtTest>

#include <ld_handle.h>
#include <ld_root_element.h>
#include <ld_root_import.h>

#include "test_root_import.h"

TestRootImport::TestRootImport() {}


TestRootImport::~TestRootImport() {}


void TestRootImport::initTestCase() {
    Ld::Handle::initialize(0x123456789ABCDEF0ULL);
}


void TestRootImport::testConstructorsDestructors() {
    QSharedPointer<Ld::RootElement> rootElement(new Ld::RootElement);
    rootElement->setWeakThis(rootElement.toWeakRef());

    Ld::RootImport import1;

    QVERIFY(import1.rootElement().isNull());
    QVERIFY(import1.isRelativeImport());
    QVERIFY(import1.isInvalid());

    Ld::RootImport import2(rootElement);

    QVERIFY(import2.rootElement() == rootElement);
    QVERIFY(import2.isRelativeImport());
    QVERIFY(import2.isValid());

    Ld::RootImport import3(rootElement, false);

    QVERIFY(import3.rootElement() == rootElement);
    QVERIFY(import3.isAbsoluteImport());

    Ld::RootImport import4(rootElement, true);

    QVERIFY(import4.rootElement() == rootElement);
    QVERIFY(import4.isRelativeImport());

    Ld::RootImport import5 = import3;

    QVERIFY(import3.rootElement() == rootElement);
    QVERIFY(import3.isAbsoluteImport());
}


void TestRootImport::testElementAccessors() {
    QSharedPointer<Ld::RootElement> rootElement(new Ld::RootElement);
    rootElement->setWeakThis(rootElement.toWeakRef());

    Ld::RootImport import;

    QVERIFY(import.rootElement().isNull());
    QVERIFY( import.isRelativeImport());
    QVERIFY(!import.isAbsoluteImport());
    QVERIFY(!import.isValid());
    QVERIFY( import.isInvalid());

    import.setRootElement(rootElement);
    QVERIFY(import.rootElement() == rootElement);
    QVERIFY(import.isValid());
    QVERIFY(!import.isInvalid());

    import.setAbsoluteImport();
    QVERIFY(!import.isRelativeImport());
    QVERIFY( import.isAbsoluteImport());

    import.setAbsoluteImport(false);
    QVERIFY( import.isRelativeImport());
    QVERIFY(!import.isAbsoluteImport());

    import.setAbsoluteImport(true);
    QVERIFY(!import.isRelativeImport());
    QVERIFY( import.isAbsoluteImport());

    import.setRelativeImport();
    QVERIFY(import.isRelativeImport());
    QVERIFY(!import.isAbsoluteImport());

    import.setRelativeImport(false);
    QVERIFY(!import.isRelativeImport());
    QVERIFY( import.isAbsoluteImport());

    import.setRelativeImport(true);
    QVERIFY( import.isRelativeImport());
    QVERIFY(!import.isAbsoluteImport());

    import.setAbsoluteImport();

    Ld::RootImport import2(QString("fubar"));
    QVERIFY(!import2.isValid());
    QVERIFY( import2.isInvalid());

    QVERIFY(import2.rootElement().isNull());
    QVERIFY(import2.isRelativeImport());

    import2 = import;

    QVERIFY(import2.rootElement() == rootElement);
    QVERIFY(import2.isAbsoluteImport());
    QVERIFY( import2.isValid());
    QVERIFY(!import2.isInvalid());
}


void TestRootImport::testAbsolutePaths() {
    QSharedPointer<Ld::RootElement> rootElement(new Ld::RootElement);
    rootElement->setWeakThis(rootElement.toWeakRef());

    bool success = rootElement->openNew();
    QVERIFY(success);

    success = rootElement->saveAs("test.ms");
    QVERIFY(success);

    Ld::RootImport import(rootElement, false);
    QString absolutePath = import.absolutePath();

    QFileInfo fileInformation1(absolutePath);
    QVERIFY(fileInformation1.isAbsolute());
    QVERIFY(fileInformation1.fileName() == QString("test.ms"));

    absolutePath = import.path(QString(".."));

    QFileInfo fileInformation2(absolutePath);
    QVERIFY(fileInformation2.isAbsolute());
    QVERIFY(fileInformation2.fileName() == QString("test.ms"));

    success = rootElement->close();
    QVERIFY(success);
}


void TestRootImport::testRelativePaths() {
    QSharedPointer<Ld::RootElement> rootElement(new Ld::RootElement);
    rootElement->setWeakThis(rootElement.toWeakRef());

    bool success = rootElement->openNew();
    QVERIFY(success);

    success = rootElement->saveAs("test.ms");
    QVERIFY(success);

    Ld::RootImport import(rootElement, true);
    QString relativePath = import.relativePath(QString(".."));

    QFileInfo fileInformation1(relativePath);
    QVERIFY(!fileInformation1.isAbsolute());
    QVERIFY( fileInformation1.fileName() == QString("test.ms"));

    relativePath = import.path(QString(".."));

    QFileInfo fileInformation2(relativePath);
    QVERIFY(!fileInformation2.isAbsolute());
    QVERIFY( fileInformation2.fileName() == QString("test.ms"));

    success = rootElement->close();
    QVERIFY(success);
}
