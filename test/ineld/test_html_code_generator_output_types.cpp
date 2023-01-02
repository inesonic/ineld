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
* This file implements tests for the \ref Ld::HtmlOutputType class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QtTest/QtTest>

#include <ld_code_generator_output_type_container.h>
#include <ld_html_code_generator_output_types.h>

#include "test_html_code_generator_output_types.h"

TestHtmlCodeGeneratorOutputTypes::TestHtmlCodeGeneratorOutputTypes() {}


TestHtmlCodeGeneratorOutputTypes::~TestHtmlCodeGeneratorOutputTypes() {}


void TestHtmlCodeGeneratorOutputTypes::testConstructorsAndDestructors() {
    Ld::CodeGeneratorOutputTypeContainer outputType1(new Ld::HtmlOutputType());
    QVERIFY(outputType1.name() == "HtmlOutputType");

    Ld::CodeGeneratorOutputTypeContainer outputType2 = outputType1;
    QVERIFY(outputType2.name() == "HtmlOutputType");

    QSharedPointer<Ld::CodeGeneratorOutputType> outputType3(new Ld::HtmlOutputType());
    Ld::CodeGeneratorOutputTypeContainer outputType4(outputType3);
    QVERIFY(outputType4.name() == "HtmlOutputType");
}


void TestHtmlCodeGeneratorOutputTypes::testAccessors() {
    Ld::HtmlOutputType outputType1;

    QVERIFY(outputType1.name() == "HtmlOutputType");
    QVERIFY(outputType1.identifier() == 1); // Order that we call this method between classes matters.
    QVERIFY(outputType1.shortDescription() == tr("HTML"));
    QVERIFY(outputType1.description() == tr("Hypertext markup that can be viewed in a browser"));
    QVERIFY(outputType1.defaultOutputType() == true);
    QVERIFY(outputType1.userReadable() == false);
    QVERIFY(outputType1.applicationLoadable() == false);
    QVERIFY(outputType1.exportable().contains(Ld::HtmlOutputType::ExportMode::EXPORT_AS_DIRECTORY));
    QVERIFY(outputType1.exportable().contains(Ld::HtmlOutputType::ExportMode::EXPORT_IN_MEMORY));
    QVERIFY(outputType1.exportable().contains(Ld::HtmlOutputType::ExportMode::EXPORT_AS_MIXED_TEMPORARY_OBJECT));
    QVERIFY(outputType1.requiresIntermediateRepresentation() == false);

    Ld::HtmlOutputType outputType2;
    QVERIFY(outputType2.identifier() == 1);

    Ld::CodeGeneratorOutputTypeContainer outputType3(new Ld::HtmlOutputType());
    QVERIFY(outputType3.name() == "HtmlOutputType");
    QVERIFY(outputType3.identifier() == 1);
    QVERIFY(outputType3.shortDescription() == tr("HTML"));
    QVERIFY(outputType3.description() == tr("Hypertext markup that can be viewed in a browser"));
    QVERIFY(outputType3.defaultOutputType() == true);
    QVERIFY(outputType3.userReadable() == false);
    QVERIFY(outputType3.applicationLoadable() == false);
    QVERIFY(outputType3.exportable().contains(Ld::HtmlOutputType::ExportMode::EXPORT_AS_DIRECTORY));
    QVERIFY(outputType3.exportable().contains(Ld::HtmlOutputType::ExportMode::EXPORT_IN_MEMORY));
    QVERIFY(outputType3.exportable().contains(Ld::HtmlOutputType::ExportMode::EXPORT_AS_MIXED_TEMPORARY_OBJECT));
    QVERIFY(outputType3.requiresIntermediateRepresentation() == false);
}
