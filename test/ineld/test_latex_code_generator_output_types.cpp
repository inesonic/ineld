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
* This file implements tests for the \ref Ld::LaTeXOutputType class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QtTest/QtTest>

#include <ld_code_generator_output_type_container.h>
#include <ld_latex_code_generator_output_types.h>

#include "test_latex_code_generator_output_types.h"

TestLaTeXCodeGeneratorOutputTypes::TestLaTeXCodeGeneratorOutputTypes() {}


TestLaTeXCodeGeneratorOutputTypes::~TestLaTeXCodeGeneratorOutputTypes() {}


void TestLaTeXCodeGeneratorOutputTypes::testConstructorsAndDestructors() {
    Ld::CodeGeneratorOutputTypeContainer outputType1(new Ld::LaTeXOutputType());
    QVERIFY(outputType1.name() == "LaTeXOutputType");

    Ld::CodeGeneratorOutputTypeContainer outputType2 = outputType1;
    QVERIFY(outputType2.name() == "LaTeXOutputType");

    QSharedPointer<Ld::CodeGeneratorOutputType> outputType3(new Ld::LaTeXOutputType());
    Ld::CodeGeneratorOutputTypeContainer outputType4(outputType3);
    QVERIFY(outputType4.name() == "LaTeXOutputType");
}


void TestLaTeXCodeGeneratorOutputTypes::testAccessors() {
    Ld::LaTeXOutputType outputType1;

    QVERIFY(outputType1.name() == "LaTeXOutputType");
    QVERIFY(outputType1.identifier() == 2); // Order that we call this method between classes matters.
    QVERIFY(outputType1.shortDescription() == tr("LaTeX"));
    QVERIFY(outputType1.description() == tr("LaTeX markup"));
    QVERIFY(outputType1.defaultOutputType() == true);
    QVERIFY(outputType1.userReadable() == true);
    QVERIFY(outputType1.applicationLoadable() == false);
    QVERIFY(outputType1.exportable().contains(Ld::LaTeXOutputType::ExportMode::EXPORT_AS_DIRECTORY));
    QVERIFY(outputType1.exportable().contains(Ld::LaTeXOutputType::ExportMode::EXPORT_IN_MEMORY));
    QVERIFY(outputType1.requiresIntermediateRepresentation() == false);

    Ld::LaTeXOutputType outputType2;
    QVERIFY(outputType2.identifier() == 2);

    Ld::CodeGeneratorOutputTypeContainer outputType3(new Ld::LaTeXOutputType());
    QVERIFY(outputType3.name() == "LaTeXOutputType");
    QVERIFY(outputType3.identifier() == 2);
    QVERIFY(outputType3.shortDescription() == tr("LaTeX"));
    QVERIFY(outputType3.description() == tr("LaTeX markup"));
    QVERIFY(outputType3.defaultOutputType() == true);
    QVERIFY(outputType3.userReadable() == true);
    QVERIFY(outputType3.applicationLoadable() == false);
    QVERIFY(outputType3.exportable().contains(Ld::LaTeXOutputType::ExportMode::EXPORT_AS_DIRECTORY));
    QVERIFY(outputType3.exportable().contains(Ld::LaTeXOutputType::ExportMode::EXPORT_IN_MEMORY));
    QVERIFY(outputType3.requiresIntermediateRepresentation() == false);
}
