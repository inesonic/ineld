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
* This file implements tests for the \ref Ld::CppCodeGeneratorDiagnostic class.  Also tests the underlying
* \ref Ld::Diagnostic class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QSharedPointer>
#include <QtTest/QtTest>

#include <ld_root_element.h>
#include <ld_cpp_translation_phase.h>
#include <ld_cpp_context.h>
#include <ld_cpp_code_generator_diagnostic.h>

#include "test_cpp_code_generator_diagnostic.h"

TestCppCodeGeneratorDiagnostic::TestCppCodeGeneratorDiagnostic() {}


TestCppCodeGeneratorDiagnostic::~TestCppCodeGeneratorDiagnostic() {}


void TestCppCodeGeneratorDiagnostic::testConstructorsAndDestructors() {
    Ld::CppTranslationPhase translationPhase;

    QSharedPointer<Ld::RootElement> element(new Ld::RootElement);
    element->setWeakThis(element.toWeakRef());

    Ld::CppCodeGeneratorDiagnostic diagnostic1(
        element,
        Ld::CppCodeGeneratorDiagnostic::Type::WARNING,
        translationPhase,
        Ld::CppCodeGeneratorDiagnostic::Code::COMPILER_DIAGNOSTIC
    );

    QVERIFY(diagnostic1.element() == element);
    QVERIFY(diagnostic1.diagnosticType() == Ld::CppCodeGeneratorDiagnostic::Type::WARNING);
    QVERIFY(diagnostic1.translationPhase() == translationPhase);
    QVERIFY(diagnostic1.diagnosticCode() == Ld::CppCodeGeneratorDiagnostic::Code::COMPILER_DIAGNOSTIC);
    QVERIFY(diagnostic1.backendMessage().isEmpty());
    QVERIFY(diagnostic1.context().isNull());
    QVERIFY(diagnostic1.compilerCode() == Ld::CppCodeGeneratorDiagnostic::CompilerCode::NO_ERROR);
    QVERIFY(diagnostic1.byteOffset() == Ld::CppCodeGeneratorDiagnostic::invalidByteOffset);
    QVERIFY(diagnostic1.lineNumber() == Ld::CppCodeGeneratorDiagnostic::invalidLineNumber);
    QVERIFY(diagnostic1.columnNumber() == Ld::CppCodeGeneratorDiagnostic::invalidColumnNumber);

    Ld::CppCodeGeneratorDiagnostic diagnostic2(
        element,
        Ld::CppCodeGeneratorDiagnostic::Type::WARNING,
        translationPhase,
        Ld::CppCodeGeneratorDiagnostic::Code::COMPILER_DIAGNOSTIC,
        "backend message"
    );

    QVERIFY(diagnostic2.element() == element);
    QVERIFY(diagnostic2.diagnosticType() == Ld::CppCodeGeneratorDiagnostic::Type::WARNING);
    QVERIFY(diagnostic2.translationPhase() == translationPhase);
    QVERIFY(diagnostic2.diagnosticCode() == Ld::CppCodeGeneratorDiagnostic::Code::COMPILER_DIAGNOSTIC);
    QVERIFY(diagnostic2.backendMessage() == QString("backend message"));
    QVERIFY(diagnostic2.context().isNull());
    QVERIFY(diagnostic2.compilerCode() == Ld::CppCodeGeneratorDiagnostic::CompilerCode::NO_ERROR);
    QVERIFY(diagnostic2.byteOffset() == Ld::CppCodeGeneratorDiagnostic::invalidByteOffset);
    QVERIFY(diagnostic2.lineNumber() == Ld::CppCodeGeneratorDiagnostic::invalidLineNumber);
    QVERIFY(diagnostic2.columnNumber() == Ld::CppCodeGeneratorDiagnostic::invalidColumnNumber);

    QSharedPointer<Ld::CppContext> context(new Ld::CppContext("obj", "out", nullptr));

    Ld::CppCodeGeneratorDiagnostic diagnostic3(
        element,
        Ld::CppCodeGeneratorDiagnostic::Type::WARNING,
        translationPhase,
        Ld::CppCodeGeneratorDiagnostic::Code::COMPILER_DIAGNOSTIC,
        "backend message",
        context
    );

    QVERIFY(diagnostic3.element() == element);
    QVERIFY(diagnostic3.diagnosticType() == Ld::CppCodeGeneratorDiagnostic::Type::WARNING);
    QVERIFY(diagnostic3.translationPhase() == translationPhase);
    QVERIFY(diagnostic3.diagnosticCode() == Ld::CppCodeGeneratorDiagnostic::Code::COMPILER_DIAGNOSTIC);
    QVERIFY(diagnostic3.backendMessage() == QString("backend message"));
    QVERIFY(diagnostic3.context() == context);
    QVERIFY(diagnostic3.compilerCode() == Ld::CppCodeGeneratorDiagnostic::CompilerCode::NO_ERROR);
    QVERIFY(diagnostic3.byteOffset() == Ld::CppCodeGeneratorDiagnostic::invalidByteOffset);
    QVERIFY(diagnostic3.lineNumber() == Ld::CppCodeGeneratorDiagnostic::invalidLineNumber);
    QVERIFY(diagnostic3.columnNumber() == Ld::CppCodeGeneratorDiagnostic::invalidColumnNumber);

    Ld::CppCodeGeneratorDiagnostic diagnostic4(
        element,
        Ld::CppCodeGeneratorDiagnostic::Type::WARNING,
        translationPhase,
        Ld::CppCodeGeneratorDiagnostic::Code::COMPILER_DIAGNOSTIC,
        "backend message",
        context,
        Ld::CppCodeGeneratorDiagnostic::CompilerCode::WARNING_NO_UNLOCK
    );

    QVERIFY(diagnostic4.element() == element);
    QVERIFY(diagnostic4.diagnosticType() == Ld::CppCodeGeneratorDiagnostic::Type::WARNING);
    QVERIFY(diagnostic4.translationPhase() == translationPhase);
    QVERIFY(diagnostic4.diagnosticCode() == Ld::CppCodeGeneratorDiagnostic::Code::COMPILER_DIAGNOSTIC);
    QVERIFY(diagnostic4.backendMessage() == QString("backend message"));
    QVERIFY(diagnostic4.context() == context);
    QVERIFY(diagnostic4.compilerCode() == Ld::CppCodeGeneratorDiagnostic::CompilerCode::WARNING_NO_UNLOCK);
    QVERIFY(diagnostic4.byteOffset() == Ld::CppCodeGeneratorDiagnostic::invalidByteOffset);
    QVERIFY(diagnostic4.lineNumber() == Ld::CppCodeGeneratorDiagnostic::invalidLineNumber);
    QVERIFY(diagnostic4.columnNumber() == Ld::CppCodeGeneratorDiagnostic::invalidColumnNumber);

    Ld::CppCodeGeneratorDiagnostic diagnostic5(
        element,
        Ld::CppCodeGeneratorDiagnostic::Type::WARNING,
        translationPhase,
        Ld::CppCodeGeneratorDiagnostic::Code::COMPILER_DIAGNOSTIC,
        "backend message",
        context,
        Ld::CppCodeGeneratorDiagnostic::CompilerCode::WARNING_NO_UNLOCK,
        10,
        20,
        30
    );

    QVERIFY(diagnostic5.element() == element);
    QVERIFY(diagnostic5.diagnosticType() == Ld::CppCodeGeneratorDiagnostic::Type::WARNING);
    QVERIFY(diagnostic5.translationPhase() == translationPhase);
    QVERIFY(diagnostic5.diagnosticCode() == Ld::CppCodeGeneratorDiagnostic::Code::COMPILER_DIAGNOSTIC);
    QVERIFY(diagnostic5.backendMessage() == QString("backend message"));
    QVERIFY(diagnostic5.context() == context);
    QVERIFY(diagnostic5.compilerCode() == Ld::CppCodeGeneratorDiagnostic::CompilerCode::WARNING_NO_UNLOCK);
    QVERIFY(diagnostic5.byteOffset() == 10);
    QVERIFY(diagnostic5.lineNumber() == 20);
    QVERIFY(diagnostic5.columnNumber() == 30);

    Ld::CppCodeGeneratorDiagnostic diagnostic6 = diagnostic5;

    QVERIFY(diagnostic6.element() == element);
    QVERIFY(diagnostic6.diagnosticType() == Ld::CppCodeGeneratorDiagnostic::Type::WARNING);
    QVERIFY(diagnostic6.translationPhase() == translationPhase);
    QVERIFY(diagnostic6.diagnosticCode() == Ld::CppCodeGeneratorDiagnostic::Code::COMPILER_DIAGNOSTIC);
    QVERIFY(diagnostic6.backendMessage() == QString("backend message"));
    QVERIFY(diagnostic6.context() == context);
    QVERIFY(diagnostic6.compilerCode() == Ld::CppCodeGeneratorDiagnostic::CompilerCode::WARNING_NO_UNLOCK);
    QVERIFY(diagnostic6.byteOffset() == 10);
    QVERIFY(diagnostic6.lineNumber() == 20);
    QVERIFY(diagnostic6.columnNumber() == 30);
}


void TestCppCodeGeneratorDiagnostic::testAccessors() {
    Ld::CppTranslationPhase translationPhase1;

    QSharedPointer<Ld::RootElement> element1(new Ld::RootElement);
    element1->setWeakThis(element1.toWeakRef());

    QSharedPointer<Ld::CppContext> context1(new Ld::CppContext("object_file", "output_file", nullptr));

    Ld::CppCodeGeneratorDiagnostic diagnostic1(
        element1,
        Ld::CppCodeGeneratorDiagnostic::Type::WARNING,
        translationPhase1,
        Ld::CppCodeGeneratorDiagnostic::Code::COMPILER_DIAGNOSTIC,
        "backend message 1",
        context1,
        Ld::CppCodeGeneratorDiagnostic::CompilerCode::WARNING_NULLABILITY_LOST,
        1,
        2,
        3
    );

    QVERIFY(diagnostic1.element() == element1);
    QVERIFY(diagnostic1.diagnosticType() == Ld::CppCodeGeneratorDiagnostic::Type::WARNING);
    QVERIFY(diagnostic1.translationPhase() == translationPhase1);
    QVERIFY(diagnostic1.diagnosticCode() == Ld::CppCodeGeneratorDiagnostic::Code::COMPILER_DIAGNOSTIC);
    QVERIFY(diagnostic1.backendMessage() == QString("backend message 1"));
    QVERIFY(diagnostic1.context() == context1);
    QVERIFY(diagnostic1.compilerCode() == Ld::CppCodeGeneratorDiagnostic::CompilerCode::WARNING_NULLABILITY_LOST);
    QVERIFY(diagnostic1.byteOffset() == 1);
    QVERIFY(diagnostic1.lineNumber() == 2);
    QVERIFY(diagnostic1.columnNumber() == 3);

    Ld::CppTranslationPhase translationPhase2 = translationPhase1;

    QSharedPointer<Ld::RootElement> element2(new Ld::RootElement);
    element2->setWeakThis(element2.toWeakRef());

    QSharedPointer<Ld::CppContext> context2(new Ld::CppContext("object_file 2", "output_file 2", nullptr));

    translationPhase2.nextPhase();

    Ld::CppCodeGeneratorDiagnostic diagnostic2(
        element2,
        Ld::CppCodeGeneratorDiagnostic::Type::FATAL_ERROR,
        translationPhase2,
        Ld::CppCodeGeneratorDiagnostic::Code::COMPILER_DIAGNOSTIC,
        "backend message 2",
        context2,
        Ld::CppCodeGeneratorDiagnostic::CompilerCode::WARNING_NULLABILITY_MISSING,
        10,
        20,
        30
    );

    QVERIFY(diagnostic2.element() == element2);
    QVERIFY(diagnostic2.diagnosticType() == Ld::CppCodeGeneratorDiagnostic::Type::FATAL_ERROR);
    QVERIFY(diagnostic2.translationPhase() == translationPhase2);
    QVERIFY(diagnostic2.diagnosticCode() == Ld::CppCodeGeneratorDiagnostic::Code::COMPILER_DIAGNOSTIC);
    QVERIFY(diagnostic2.backendMessage() == QString("backend message 2"));
    QVERIFY(diagnostic2.context() == context2);
    QVERIFY(   diagnostic2.compilerCode()
            == Ld::CppCodeGeneratorDiagnostic::CompilerCode::WARNING_NULLABILITY_MISSING
           );
    QVERIFY(diagnostic2.byteOffset() == 10);
    QVERIFY(diagnostic2.lineNumber() == 20);
    QVERIFY(diagnostic2.columnNumber() == 30);

    Ld::CppCodeGeneratorDiagnostic diagnostic3(
        element1,
        Ld::CppCodeGeneratorDiagnostic::Type::FATAL_ERROR,
        translationPhase1,
        Ld::CppCodeGeneratorDiagnostic::Code::LINKER_DIAGNOSTIC,
        "backend message 3"
    );

    QVERIFY(diagnostic3.diagnosticCode() == Ld::CppCodeGeneratorDiagnostic::Code::LINKER_DIAGNOSTIC);
}


void TestCppCodeGeneratorDiagnostic::testAssignmentOperator() {
    Ld::CppTranslationPhase translationPhase1;

    QSharedPointer<Ld::RootElement> element1(new Ld::RootElement);
    element1->setWeakThis(element1.toWeakRef());

    QSharedPointer<Ld::CppContext> context1(new Ld::CppContext("object_file", "output_file", nullptr));

    Ld::CppCodeGeneratorDiagnostic diagnostic1(
        element1,
        Ld::CppCodeGeneratorDiagnostic::Type::WARNING,
        translationPhase1,
        Ld::CppCodeGeneratorDiagnostic::Code::COMPILER_DIAGNOSTIC,
        "backend message 1",
        context1,
        Ld::CppCodeGeneratorDiagnostic::CompilerCode::WARNING_NULLABILITY_LOST,
        1,
        2,
        3
    );

    QVERIFY(diagnostic1.element() == element1);
    QVERIFY(diagnostic1.diagnosticType() == Ld::CppCodeGeneratorDiagnostic::Type::WARNING);
    QVERIFY(diagnostic1.translationPhase() == translationPhase1);
    QVERIFY(diagnostic1.diagnosticCode() == Ld::CppCodeGeneratorDiagnostic::Code::COMPILER_DIAGNOSTIC);
    QVERIFY(diagnostic1.backendMessage() == QString("backend message 1"));
    QVERIFY(diagnostic1.context() == context1);
    QVERIFY(diagnostic1.compilerCode() == Ld::CppCodeGeneratorDiagnostic::CompilerCode::WARNING_NULLABILITY_LOST);
    QVERIFY(diagnostic1.byteOffset() == 1);
    QVERIFY(diagnostic1.lineNumber() == 2);
    QVERIFY(diagnostic1.columnNumber() == 3);

    Ld::CppTranslationPhase translationPhase2 = translationPhase1;

    QSharedPointer<Ld::RootElement> element2(new Ld::RootElement);
    element2->setWeakThis(element2.toWeakRef());

    QSharedPointer<Ld::CppContext> context2(new Ld::CppContext("object_file 2", "output_file 2", nullptr));

    translationPhase2.nextPhase();

    Ld::CppCodeGeneratorDiagnostic diagnostic2(
        element2,
        Ld::CppCodeGeneratorDiagnostic::Type::FATAL_ERROR,
        translationPhase2,
        Ld::CppCodeGeneratorDiagnostic::Code::LINKER_DIAGNOSTIC,
        "backend message 2",
        context2,
        Ld::CppCodeGeneratorDiagnostic::CompilerCode::WARNING_NULLABILITY_MISSING,
        10,
        20,
        30
    );

    QVERIFY(diagnostic2.element() == element2);
    QVERIFY(diagnostic2.diagnosticType() == Ld::CppCodeGeneratorDiagnostic::Type::FATAL_ERROR);
    QVERIFY(diagnostic2.translationPhase() == translationPhase2);
    QVERIFY(diagnostic2.diagnosticCode() == Ld::CppCodeGeneratorDiagnostic::Code::LINKER_DIAGNOSTIC);
    QVERIFY(diagnostic2.backendMessage() == QString("backend message 2"));
    QVERIFY(diagnostic2.context() == context2);
    QVERIFY(   diagnostic2.compilerCode()
            == Ld::CppCodeGeneratorDiagnostic::CompilerCode::WARNING_NULLABILITY_MISSING
           );
    QVERIFY(diagnostic2.byteOffset() == 10);
    QVERIFY(diagnostic2.lineNumber() == 20);
    QVERIFY(diagnostic2.columnNumber() == 30);

    diagnostic2 = diagnostic1;

    QVERIFY(diagnostic2.element() == element1);
    QVERIFY(diagnostic2.diagnosticType() == Ld::CppCodeGeneratorDiagnostic::Type::WARNING);
    QVERIFY(diagnostic2.translationPhase() == translationPhase1);
    QVERIFY(diagnostic2.diagnosticCode() == Ld::CppCodeGeneratorDiagnostic::Code::COMPILER_DIAGNOSTIC);
    QVERIFY(diagnostic2.backendMessage() == QString("backend message 1"));
    QVERIFY(diagnostic2.context() == context1);
    QVERIFY(diagnostic2.compilerCode() == Ld::CppCodeGeneratorDiagnostic::CompilerCode::WARNING_NULLABILITY_LOST);
    QVERIFY(diagnostic2.byteOffset() == 1);
    QVERIFY(diagnostic2.lineNumber() == 2);
    QVERIFY(diagnostic2.columnNumber() == 3);
}
