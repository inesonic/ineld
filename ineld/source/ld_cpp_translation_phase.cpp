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
* This header defines the \ref Ld::CppTranslationPhase class.
***********************************************************************************************************************/

#include <QString>

#include <functional>

#include "ld_translation_phase.h"
#include "ld_cpp_translation_phase.h"

static constexpr unsigned dynamicLibraryPhaseCount = 18;
static constexpr unsigned objectFilePhaseCount     = 17;

static const struct PhaseData {
    const char*                           name;
    Ld::TranslationPhase::TranslationMode mode;
} phaseData[dynamicLibraryPhaseCount] = {
    { "IDENTIFY_DEPENDENCIES_AND_EXPLICIT_TYPES", Ld::TranslationPhase::TranslationMode::HONOR_HEIRARCHY            },
    { "IDENTIFY_IMPLICIT_TYPES",                  Ld::TranslationPhase::TranslationMode::HONOR_HEIRARCHY            },
    { "DECLARATIONS",                             Ld::TranslationPhase::TranslationMode::IGNORE_HEIRARCHY           },
    { "MODEL_CLASS_DECLARATION",                  Ld::TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION },
    { "METHOD_DECLARATIONS",                      Ld::TranslationPhase::TranslationMode::HONOR_HEIRARCHY            },
    { "CLASS_SCOPE_VARIABLES",                    Ld::TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION },
    { "END_MODEL_CLASS_DECLARATION",              Ld::TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION },
    { "THREAD_DEFINITION",                        Ld::TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION },
    { "THREAD_LOCALS",                            Ld::TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION },
    { "THREAD_IMPLEMENTATION",                    Ld::TranslationPhase::TranslationMode::HONOR_HEIRARCHY            },
    { "THREAD_POST_PROCESSING",                   Ld::TranslationPhase::TranslationMode::IGNORE_HEIRARCHY           },
    { "THREAD_CLEANUP",                           Ld::TranslationPhase::TranslationMode::IGNORE_HEIRARCHY           },
    { "THREAD_END",                               Ld::TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION },
    { "METHOD_DEFINITIONS",                       Ld::TranslationPhase::TranslationMode::HONOR_HEIRARCHY            },
    { "BOOKKEEPING_DEFINITION",                   Ld::TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION },
    { "MODEL_ALLOCATOR",                          Ld::TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION },
    { "CONVERT_IR_TO_OBJECT",                     Ld::TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION },
    { "LINK",                                     Ld::TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION }
};

namespace Ld {
    CppTranslationPhase::CppTranslationPhase(bool generateDynamicLibrary) {
        currentNumberPhases = generateDynamicLibrary ? dynamicLibraryPhaseCount : objectFilePhaseCount;
    }


    CppTranslationPhase::~CppTranslationPhase() {}


    QString CppTranslationPhase::phaseName(unsigned number) const {
        QString result;
        if (number < currentNumberPhases) {
            result = QString::fromLocal8Bit(phaseData[number].name);
        } else {
            result = QString();
        }

        return result;
    }


    TranslationPhase::TranslationMode CppTranslationPhase::translationMode(unsigned number) const {
        TranslationPhase::TranslationMode result;
        if (number < currentNumberPhases) {
            result = phaseData[number].mode;
        } else {
            result = TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION;
        }

        return result;
    }


    unsigned CppTranslationPhase::numberPhases() const {
        return currentNumberPhases;
    }


    CppTranslationPhase::Phase CppTranslationPhase::phase() const {
        return static_cast<Phase>(phaseNumber());
    }


    unsigned CppTranslationPhase::numberThreads() const {
        // Eventually we can set and return the number of threads.  Value can also be used to adjust the translation
        // phases that are performed.

        return 1;
    }

    unsigned CppTranslationPhase::threadId() const {
        // Eventually we can loop through a subset of the translation phases for each thread that we need to build
        // code for.  For now, everything is singly threaded so we don't loop and always return zero here.

        return 0;
    }
}
