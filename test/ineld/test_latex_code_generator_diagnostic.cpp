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
* This file implements tests for the \ref Ld::LaTeXCodeGeneratorDiagnostic class.  Also tests the underlying
* \ref Ld::Diagnostic class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QSharedPointer>
#include <QtTest/QtTest>

#include <ld_root_element.h>
#include <ld_latex_translation_phase.h>
#include <ld_latex_code_generator_diagnostic.h>

#include "test_latex_code_generator_diagnostic.h"

TestLaTeXCodeGeneratorDiagnostic::TestLaTeXCodeGeneratorDiagnostic() {}


TestLaTeXCodeGeneratorDiagnostic::~TestLaTeXCodeGeneratorDiagnostic() {}


void TestLaTeXCodeGeneratorDiagnostic::testConstructorsAndDestructors() {
    Ld::LaTeXTranslationPhase translationPhase;

    QSharedPointer<Ld::RootElement> element(new Ld::RootElement);
    element->setWeakThis(element.toWeakRef());

    Ld::LaTeXCodeGeneratorDiagnostic diagnostic1(
        element,
        Ld::LaTeXCodeGeneratorDiagnostic::Type::WARNING,
        translationPhase,
        Ld::LaTeXCodeGeneratorDiagnostic::Code::MISSING_TRANSLATOR
    );

    QVERIFY(diagnostic1.element() == element);
    QVERIFY(diagnostic1.diagnosticType() == Ld::LaTeXCodeGeneratorDiagnostic::Type::WARNING);
    QVERIFY(diagnostic1.translationPhase() == translationPhase);
    QVERIFY(diagnostic1.diagnosticCode() == Ld::LaTeXCodeGeneratorDiagnostic::Code::MISSING_TRANSLATOR);
    QVERIFY(diagnostic1.context().isNull());

    Ld::LaTeXCodeGeneratorDiagnostic diagnostic2(
        element,
        Ld::LaTeXCodeGeneratorDiagnostic::Type::WARNING,
        translationPhase,
        Ld::LaTeXCodeGeneratorDiagnostic::Code::UNEXPECTED_TRANSLATION_PHASE
    );

    QVERIFY(diagnostic2.element() == element);
    QVERIFY(diagnostic2.diagnosticType() == Ld::LaTeXCodeGeneratorDiagnostic::Type::WARNING);
    QVERIFY(diagnostic2.translationPhase() == translationPhase);
    QVERIFY(diagnostic2.diagnosticCode() == Ld::LaTeXCodeGeneratorDiagnostic::Code::UNEXPECTED_TRANSLATION_PHASE);

    Ld::LaTeXCodeGeneratorDiagnostic diagnostic3 = diagnostic2;

    QVERIFY(diagnostic3.element() == element);
    QVERIFY(diagnostic3.diagnosticType() == Ld::LaTeXCodeGeneratorDiagnostic::Type::WARNING);
    QVERIFY(diagnostic3.translationPhase() == translationPhase);
    QVERIFY(diagnostic3.diagnosticCode() == Ld::LaTeXCodeGeneratorDiagnostic::Code::UNEXPECTED_TRANSLATION_PHASE);
}


void TestLaTeXCodeGeneratorDiagnostic::testAccessors() {
    // Verified by testConstructorsAndDestructors
}


void TestLaTeXCodeGeneratorDiagnostic::testAssignmentOperator() {
    Ld::LaTeXTranslationPhase translationPhase;

    QSharedPointer<Ld::RootElement> element(new Ld::RootElement);
    element->setWeakThis(element.toWeakRef());

    Ld::LaTeXCodeGeneratorDiagnostic diagnostic1(
        element,
        Ld::LaTeXCodeGeneratorDiagnostic::Type::WARNING,
        translationPhase,
        Ld::LaTeXCodeGeneratorDiagnostic::Code::MISSING_TRANSLATOR
    );

    QVERIFY(diagnostic1.element() == element);
    QVERIFY(diagnostic1.diagnosticType() == Ld::LaTeXCodeGeneratorDiagnostic::Type::WARNING);
    QVERIFY(diagnostic1.translationPhase() == translationPhase);
    QVERIFY(diagnostic1.diagnosticCode() == Ld::LaTeXCodeGeneratorDiagnostic::Code::MISSING_TRANSLATOR);
    QVERIFY(diagnostic1.context().isNull());

    Ld::LaTeXCodeGeneratorDiagnostic diagnostic2(
        element,
        Ld::LaTeXCodeGeneratorDiagnostic::Type::WARNING,
        translationPhase,
        Ld::LaTeXCodeGeneratorDiagnostic::Code::UNEXPECTED_TRANSLATION_PHASE
    );

    QVERIFY(diagnostic2.element() == element);
    QVERIFY(diagnostic2.diagnosticType() == Ld::LaTeXCodeGeneratorDiagnostic::Type::WARNING);
    QVERIFY(diagnostic2.translationPhase() == translationPhase);
    QVERIFY(diagnostic2.diagnosticCode() == Ld::LaTeXCodeGeneratorDiagnostic::Code::UNEXPECTED_TRANSLATION_PHASE);

    diagnostic1 = diagnostic2;

    QVERIFY(diagnostic1.element() == element);
    QVERIFY(diagnostic1.diagnosticType() == Ld::LaTeXCodeGeneratorDiagnostic::Type::WARNING);
    QVERIFY(diagnostic1.translationPhase() == translationPhase);
    QVERIFY(diagnostic1.diagnosticCode() == Ld::LaTeXCodeGeneratorDiagnostic::Code::UNEXPECTED_TRANSLATION_PHASE);
}
