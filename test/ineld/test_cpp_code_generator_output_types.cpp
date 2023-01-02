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
* This file implements tests for the \ref Ld::CppCodeGeneratorOutputType class.  Also tests the underlying
* \ref Ld::CodeGeneratorOutputType and \ref Ld::CodeGeneratorOutputTypeContainer classes.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QtTest/QtTest>

#include <ld_code_generator_output_type_container.h>
#include <ld_cpp_code_generator_output_types.h>

#include "test_cpp_code_generator_output_types.h"

TestCppCodeGeneratorOutputTypes::TestCppCodeGeneratorOutputTypes() {}


TestCppCodeGeneratorOutputTypes::~TestCppCodeGeneratorOutputTypes() {}


void TestCppCodeGeneratorOutputTypes::testConstructorsAndDestructors() {
    Ld::CodeGeneratorOutputTypeContainer outputType1(new Ld::CppLoadableModuleOutputType());
    QVERIFY(outputType1.name() == "CppLoadableModuleOutputType");

    Ld::CodeGeneratorOutputTypeContainer outputType2 = outputType1;
    QVERIFY(outputType2.name() == "CppLoadableModuleOutputType");

    QSharedPointer<Ld::CodeGeneratorOutputType> outputType3(new Ld::CppLoadableModuleOutputType());
    Ld::CodeGeneratorOutputTypeContainer outputType4(outputType3);
    QVERIFY(outputType4.name() == "CppLoadableModuleOutputType");
}


void TestCppCodeGeneratorOutputTypes::testAccessors() {
    Ld::CppLoadableModuleOutputType outputType1;

    QVERIFY(outputType1.name() == "CppLoadableModuleOutputType");
    QVERIFY(outputType1.identifier() == 0);  // Order that we call this method between classes matters.
    QVERIFY(outputType1.shortDescription() == tr("Application loadable module."));
    QVERIFY(outputType1.description() == tr("A dynamic library that can be loaded and executed by the application."));
    QVERIFY(outputType1.defaultOutputType() == true);
    QVERIFY(outputType1.userReadable() == false);
    QVERIFY(outputType1.applicationLoadable() == true);
    QVERIFY(outputType1.exportable().contains(Ld::CodeGeneratorOutputType::ExportMode::NO_EXPORT));
    QVERIFY(outputType1.exportable().contains(Ld::CodeGeneratorOutputType::ExportMode::EXPORT_AS_FILE));
    QVERIFY(outputType1.defaultExportMode() == Ld::CodeGeneratorOutputType::ExportMode::NO_EXPORT);
    QVERIFY(outputType1.requiresIntermediateRepresentation() == true);

    Ld::CppLoadableModuleOutputType outputType2;
    QVERIFY(outputType2.identifier() == 0);  // Order that we call this method between classes matters.

    Ld::CodeGeneratorOutputTypeContainer outputType3(new Ld::CppLoadableModuleOutputType());
    QVERIFY(outputType3.name() == "CppLoadableModuleOutputType");
    QVERIFY(outputType3.identifier() == 0);  // Order that we call this method between classes matters.
    QVERIFY(outputType3.shortDescription() == tr("Application loadable module."));
    QVERIFY(outputType3.description() == tr("A dynamic library that can be loaded and executed by the application."));
    QVERIFY(outputType3.defaultOutputType() == true);
    QVERIFY(outputType3.userReadable() == false);
    QVERIFY(outputType3.applicationLoadable() == true);
    QVERIFY(outputType3.exportable().contains(Ld::CodeGeneratorOutputType::ExportMode::NO_EXPORT));
    QVERIFY(outputType3.exportable().contains(Ld::CodeGeneratorOutputType::ExportMode::EXPORT_AS_FILE));
    QVERIFY(outputType3.defaultExportMode() == Ld::CodeGeneratorOutputType::ExportMode::NO_EXPORT);
    QVERIFY(outputType3.requiresIntermediateRepresentation() == true);
}


void TestCppCodeGeneratorOutputTypes::testAssignmentOperator() {
    Ld::CodeGeneratorOutputTypeContainer outputType1(new Ld::CppLoadableModuleOutputType());
    QVERIFY( outputType1.isDefined());
    QVERIFY(!outputType1.isUndefined());
    QVERIFY( outputType1.name() == "CppLoadableModuleOutputType");

    Ld::CodeGeneratorOutputTypeContainer outputType2;
    QVERIFY(!outputType2.isDefined());
    QVERIFY( outputType2.isUndefined());
    QVERIFY( outputType2.name().isEmpty());

    outputType2 = outputType1;
    QVERIFY( outputType2.isDefined());
    QVERIFY(!outputType2.isUndefined());
    QVERIFY( outputType2.name() == "CppLoadableModuleOutputType");
}


void TestCppCodeGeneratorOutputTypes::testComparisonOperators() {
    Ld::CppLoadableModuleOutputType      outputType1;
    Ld::CppLoadableModuleOutputType      outputType2;
    Ld::CodeGeneratorOutputTypeContainer outputType3(new Ld::CppLoadableModuleOutputType());
    Ld::CodeGeneratorOutputTypeContainer outputType4;

    QVERIFY((outputType1 == outputType2) == true);
    QVERIFY((outputType1 != outputType2) == false);

    QVERIFY((outputType1 == outputType2) == true);
    QVERIFY((outputType1 != outputType2) == false);
    QVERIFY((outputType1 == outputType3) == true);
    QVERIFY((outputType1 != outputType3) == false);
    QVERIFY((outputType1 == outputType4) == false);
    QVERIFY((outputType1 != outputType4) == true);

    Ld::CodeGeneratorOutputTypeContainer outputType5 = outputType3;

    QVERIFY((outputType5 == outputType4) == false);
    QVERIFY((outputType5 != outputType4) == true);
    QVERIFY((outputType5 == outputType1) == true);
    QVERIFY((outputType5 != outputType1) == false);
}
