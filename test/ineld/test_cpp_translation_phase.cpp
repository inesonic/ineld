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
* This file implements tests for the \ref Ld::CppTranslationPhase class.  The module also tests the underlying
* \ref Ld::TranslationPhase class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QSharedPointer>
#include <QtTest/QtTest>

#include <ld_translation_phase.h>
#include <ld_cpp_translation_phase.h>

#include "test_cpp_translation_phase.h"

TestCppTranslationPhase::TestCppTranslationPhase() {}


TestCppTranslationPhase::~TestCppTranslationPhase() {}


void TestCppTranslationPhase::testConstructorsAndDestructors() {
    Ld::CppTranslationPhase translationPhase1;
    QCOMPARE(translationPhase1.phaseNumber(), 0U);

    translationPhase1.nextPhase();
    QCOMPARE(translationPhase1.phaseNumber(), 1U);

    Ld::CppTranslationPhase translationPhase2 = translationPhase1;
    QCOMPARE(translationPhase2.phaseNumber(), 1U);
}


void TestCppTranslationPhase::testPhases() {
    Ld::CppTranslationPhase translationPhase;

    QCOMPARE(translationPhase.numberPhases(), 18U);

    QCOMPARE(translationPhase.phaseNumber(), 0U);
    QCOMPARE(translationPhase.phase(), Ld::CppTranslationPhase::Phase::IDENTIFY_DEPENDENCIES_AND_EXPLICIT_TYPES);
    QCOMPARE(translationPhase.lastPhase(), false);
    QCOMPARE(translationPhase.currentPhaseName(), QString("IDENTIFY_DEPENDENCIES"));
    QCOMPARE(translationPhase.currentTranslationMode(), Ld::TranslationPhase::TranslationMode::IGNORE_HEIRARCHY);

    translationPhase.nextPhase();

    QCOMPARE(translationPhase.phaseNumber(), 1U);
    QCOMPARE(translationPhase.phase(), Ld::CppTranslationPhase::Phase::IDENTIFY_INFERRED_TYPES);
    QCOMPARE(translationPhase.lastPhase(), false);
    QCOMPARE(translationPhase.currentPhaseName(), QString("FILE_HEADER"));
    QCOMPARE(translationPhase.currentTranslationMode(), Ld::TranslationPhase::TranslationMode::HONOR_HEIRARCHY);

    translationPhase.nextPhase();

    QCOMPARE(translationPhase.phaseNumber(), 2U);
    QCOMPARE(translationPhase.phase(), Ld::CppTranslationPhase::Phase::DECLARATIONS);
    QCOMPARE(translationPhase.lastPhase(), false);
    QCOMPARE(translationPhase.currentPhaseName(), QString("DECLARATIONS"));
    QCOMPARE(translationPhase.currentTranslationMode(), Ld::TranslationPhase::TranslationMode::IGNORE_HEIRARCHY);

    translationPhase.nextPhase();

    QCOMPARE(translationPhase.phaseNumber(), 3U);
    QCOMPARE(translationPhase.phase(), Ld::CppTranslationPhase::Phase::MODEL_CLASS_DECLARATION);
    QCOMPARE(translationPhase.lastPhase(), false);
    QCOMPARE(translationPhase.currentPhaseName(), QString("MODEL_CLASS_DECLARATION"));
    QCOMPARE(   translationPhase.currentTranslationMode()
           , Ld::TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION
           );

    translationPhase.nextPhase();

    QCOMPARE(translationPhase.phaseNumber(), 4U);
    QCOMPARE(translationPhase.phase(), Ld::CppTranslationPhase::Phase::METHOD_DECLARATIONS);
    QCOMPARE(translationPhase.lastPhase(), false);
    QCOMPARE(translationPhase.currentPhaseName(), QString("METHOD_DECLARATIONS"));
    QCOMPARE(translationPhase.currentTranslationMode(), Ld::TranslationPhase::TranslationMode::HONOR_HEIRARCHY);

    translationPhase.nextPhase();

    QCOMPARE(translationPhase.phaseNumber(), 5U);
    QCOMPARE(translationPhase.phase(), Ld::CppTranslationPhase::Phase::CLASS_SCOPE_VARIABLES);
    QCOMPARE(translationPhase.lastPhase(), false);
    QCOMPARE(translationPhase.currentPhaseName(), QString("CLASS_SCOPE_VARIABLES"));
    QCOMPARE(
        translationPhase.currentTranslationMode(),
        Ld::TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION
    );

    translationPhase.nextPhase();

    QCOMPARE(translationPhase.phaseNumber(), 6U);
    QCOMPARE(translationPhase.phase(), Ld::CppTranslationPhase::Phase::END_MODEL_CLASS_DECLARATION);
    QCOMPARE(translationPhase.lastPhase(), false);
    QCOMPARE(translationPhase.currentPhaseName(), QString("END_MODEL_CLASS_DECLARATION"));
    QCOMPARE(
        translationPhase.currentTranslationMode(),
        Ld::TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION
    );

    translationPhase.nextPhase();

    QCOMPARE(translationPhase.phaseNumber(), 7U);
    QCOMPARE(translationPhase.phase(), Ld::CppTranslationPhase::Phase::THREAD_DEFINITION);
    QCOMPARE(translationPhase.lastPhase(), false);
    QCOMPARE(translationPhase.currentPhaseName(), QString("THREAD_DEFINITION"));
    QCOMPARE(
        translationPhase.currentTranslationMode(),
        Ld::TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION
    );

    translationPhase.nextPhase();

    QCOMPARE(translationPhase.phaseNumber(), 8U);
    QCOMPARE(translationPhase.phase(), Ld::CppTranslationPhase::Phase::THREAD_LOCALS);
    QCOMPARE(translationPhase.lastPhase(), false);
    QCOMPARE(translationPhase.currentPhaseName(), QString("THREAD_LOCALS"));
    QCOMPARE(
        translationPhase.currentTranslationMode(),
        Ld::TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION
    );

    translationPhase.nextPhase();

    QCOMPARE(translationPhase.phaseNumber(), 9U);
    QCOMPARE(translationPhase.phase(), Ld::CppTranslationPhase::Phase::THREAD_IMPLEMENTATION);
    QCOMPARE(translationPhase.lastPhase(), false);
    QCOMPARE(translationPhase.currentPhaseName(), QString("THREAD_IMPLEMENTATION"));
    QCOMPARE(translationPhase.currentTranslationMode(), Ld::TranslationPhase::TranslationMode::HONOR_HEIRARCHY);

    translationPhase.nextPhase();

    QCOMPARE(translationPhase.phaseNumber(), 10U);
    QCOMPARE(translationPhase.phase(), Ld::CppTranslationPhase::Phase::THREAD_POST_PROCESSING);
    QCOMPARE(translationPhase.lastPhase(), false);
    QCOMPARE(translationPhase.currentPhaseName(), QString("THREAD_POST_PROCESSING"));
    QCOMPARE(translationPhase.currentTranslationMode(), Ld::TranslationPhase::TranslationMode::IGNORE_HEIRARCHY);

    translationPhase.nextPhase();

    QCOMPARE(translationPhase.phaseNumber(), 11U);
    QCOMPARE(translationPhase.phase(), Ld::CppTranslationPhase::Phase::THREAD_CLEANUP);
    QCOMPARE(translationPhase.lastPhase(), false);
    QCOMPARE(translationPhase.currentPhaseName(), QString("THREAD_CLEANUP"));
    QCOMPARE(translationPhase.currentTranslationMode(), Ld::TranslationPhase::TranslationMode::IGNORE_HEIRARCHY);

    translationPhase.nextPhase();

    QCOMPARE(translationPhase.phaseNumber(), 12U);
    QCOMPARE(translationPhase.phase(), Ld::CppTranslationPhase::Phase::THREAD_END);
    QCOMPARE(translationPhase.lastPhase(), false);
    QCOMPARE(translationPhase.currentPhaseName(), QString("THREAD_END"));
    QCOMPARE(   translationPhase.currentTranslationMode()
           , Ld::TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION
           );

    translationPhase.nextPhase();

    QCOMPARE(translationPhase.phaseNumber(), 13U);
    QCOMPARE(translationPhase.phase(), Ld::CppTranslationPhase::Phase::METHOD_DEFINITIONS);
    QCOMPARE(translationPhase.lastPhase(), false);
    QCOMPARE(translationPhase.currentPhaseName(), QString("METHOD_DEFINITIONS"));
    QCOMPARE(translationPhase.currentTranslationMode(), Ld::TranslationPhase::TranslationMode::HONOR_HEIRARCHY);

    translationPhase.nextPhase();

    QCOMPARE(translationPhase.phaseNumber(), 14U);
    QCOMPARE(translationPhase.phase(), Ld::CppTranslationPhase::Phase::BOOKKEEPING_DEFINITION);
    QCOMPARE(translationPhase.lastPhase(), false);
    QCOMPARE(translationPhase.currentPhaseName(), QString("BOOKKEEPING_DEFINITION"));
    QCOMPARE(
        translationPhase.currentTranslationMode(),
        Ld::TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION
    );

    translationPhase.nextPhase();

    QCOMPARE(translationPhase.phaseNumber(), 15U);
    QCOMPARE(translationPhase.phase(), Ld::CppTranslationPhase::Phase::MODEL_ALLOCATOR);
    QCOMPARE(translationPhase.lastPhase(), false);
    QCOMPARE(translationPhase.currentPhaseName(), QString("MODEL_ALLOCATOR"));
    QCOMPARE(
        translationPhase.currentTranslationMode(),
        Ld::TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION
    );

    translationPhase.nextPhase();

    QCOMPARE(translationPhase.phaseNumber(), 16U);
    QCOMPARE(translationPhase.phase(), Ld::CppTranslationPhase::Phase::CONVERT_IR_TO_OBJECT);
    QCOMPARE(translationPhase.lastPhase(), false);
    QCOMPARE(translationPhase.currentPhaseName(), QString("CONVERT_IR_TO_OBJECT"));
    QCOMPARE(
        translationPhase.currentTranslationMode(),
        Ld::TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION
    );

    translationPhase.nextPhase();

    QCOMPARE(translationPhase.phaseNumber(), 17U);
    QCOMPARE(translationPhase.phase(), Ld::CppTranslationPhase::Phase::LINK);
    QCOMPARE(translationPhase.lastPhase(), true);
    QCOMPARE(translationPhase.currentPhaseName(), QString("LINK"));
    QCOMPARE(
        translationPhase.currentTranslationMode(),
        Ld::TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION
    );
}


void TestCppTranslationPhase::testAssignmentOperator() {
    Ld::CppTranslationPhase translationPhase1;
    translationPhase1.nextPhase();
    QCOMPARE(translationPhase1.phaseNumber(), 1U);

    Ld::CppTranslationPhase translationPhase2;
    QCOMPARE(translationPhase2.phaseNumber(), 0U);

    translationPhase2 = translationPhase1;
    QCOMPARE(translationPhase2.phaseNumber(), 1U);
}


void TestCppTranslationPhase::testComparisonOperators() {
    Ld::CppTranslationPhase translationPhase1;
    translationPhase1.nextPhase();
    QCOMPARE(translationPhase1.phaseNumber(), 1U);

    Ld::CppTranslationPhase translationPhase2;
    QCOMPARE(translationPhase2.phaseNumber(), 0U);

    Ld::CppTranslationPhase translationPhase3;
    translationPhase3.nextPhase();
    QCOMPARE(translationPhase3.phaseNumber(), 1U);

    QCOMPARE((translationPhase1 != translationPhase2), true);
    QCOMPARE((translationPhase1 == translationPhase2), false);

    QCOMPARE((translationPhase1 != translationPhase3), false);
    QCOMPARE((translationPhase1 == translationPhase3), true);
}
