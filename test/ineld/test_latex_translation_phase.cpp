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
* This file implements tests for the \ref Ld::LaTeXTranslationPhase class.  The module also tests the underlying
* \ref Ld::TranslationPhase class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QSharedPointer>
#include <QtTest/QtTest>

#include <ld_translation_phase.h>
#include <ld_latex_translation_phase.h>

#include "test_latex_translation_phase.h"

TestLaTeXTranslationPhase::TestLaTeXTranslationPhase() {}


TestLaTeXTranslationPhase::~TestLaTeXTranslationPhase() {}


void TestLaTeXTranslationPhase::testConstructorsAndDestructors() {
    Ld::LaTeXTranslationPhase translationPhase1;
    QVERIFY(translationPhase1.phaseNumber() == 0);

    translationPhase1.nextPhase();
    QVERIFY(translationPhase1.phaseNumber() == 1);

    Ld::LaTeXTranslationPhase translationPhase2 = translationPhase1;
    QVERIFY(translationPhase2.phaseNumber() == 1);
}


void TestLaTeXTranslationPhase::testPhases() {
    Ld::LaTeXTranslationPhase translationPhase;

    QCOMPARE(translationPhase.numberPhases(), 3U);

    QCOMPARE(translationPhase.phaseNumber(), 0U);
    QCOMPARE(translationPhase.phase(), Ld::LaTeXTranslationPhase::Phase::IDENTIFY_DEPENDENCIES);
    QCOMPARE(translationPhase.lastPhase(), false);
    QCOMPARE(translationPhase.currentPhaseName(), QString("IDENTIFY_DEPENDENCIES"));
    QCOMPARE(translationPhase.currentTranslationMode(), Ld::TranslationPhase::TranslationMode::IGNORE_HEIRARCHY);

    translationPhase.nextPhase();

    QCOMPARE(translationPhase.phaseNumber(), 1U);
    QCOMPARE(translationPhase.phase(), Ld::LaTeXTranslationPhase::Phase::PREAMBLE);
    QCOMPARE(translationPhase.lastPhase(), false);
    QCOMPARE(translationPhase.currentPhaseName(), QString("PREAMBLE"));
    QCOMPARE(
        translationPhase.currentTranslationMode(),
        Ld::TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION
    );

    translationPhase.nextPhase();

    QCOMPARE(translationPhase.phaseNumber(), 2U);
    QCOMPARE(translationPhase.phase(), Ld::LaTeXTranslationPhase::Phase::BODY);
    QCOMPARE(translationPhase.lastPhase(), true);
    QCOMPARE(translationPhase.currentPhaseName(), QString("BODY"));
    QCOMPARE(translationPhase.currentTranslationMode(), Ld::TranslationPhase::TranslationMode::HONOR_HEIRARCHY);
}


void TestLaTeXTranslationPhase::testAssignmentOperator() {
    Ld::LaTeXTranslationPhase translationPhase1;
    translationPhase1.nextPhase();
    QCOMPARE(translationPhase1.phaseNumber(), 1U);

    Ld::LaTeXTranslationPhase translationPhase2;
    QCOMPARE(translationPhase2.phaseNumber(), 0U);

    translationPhase2 = translationPhase1;
    QCOMPARE(translationPhase2.phaseNumber(), 1U);
}


void TestLaTeXTranslationPhase::testComparisonOperators() {
    Ld::LaTeXTranslationPhase translationPhase1;
    translationPhase1.nextPhase();
    QCOMPARE(translationPhase1.phaseNumber(), 1U);

    Ld::LaTeXTranslationPhase translationPhase2;
    QCOMPARE(translationPhase2.phaseNumber(), 0U);

    Ld::LaTeXTranslationPhase translationPhase3;
    translationPhase3.nextPhase();
    QCOMPARE(translationPhase3.phaseNumber(), 1U);

    QCOMPARE((translationPhase1 != translationPhase2), true);
    QCOMPARE((translationPhase1 == translationPhase2), false);

    QCOMPARE((translationPhase1 != translationPhase3), false);
    QCOMPARE((translationPhase1 == translationPhase3), true);
}
