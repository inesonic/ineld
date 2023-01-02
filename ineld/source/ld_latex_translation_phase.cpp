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
* This header defines the \ref Ld::LaTeXTranslationPhase class.
***********************************************************************************************************************/

#include <QString>

#include "ld_translation_phase.h"
#include "ld_latex_translation_phase.h"

static constexpr unsigned phaseCount = 3;

static const struct PhaseData {
    const char*                           name;
    Ld::TranslationPhase::TranslationMode mode;
} phaseData[phaseCount] = {
    { "IDENTIFY_DEPENDENCIES", Ld::TranslationPhase::TranslationMode::IGNORE_HEIRARCHY },
    { "PREAMBLE",              Ld::TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION },
    { "BODY",                  Ld::TranslationPhase::TranslationMode::HONOR_HEIRARCHY }
};

namespace Ld {
    LaTeXTranslationPhase::~LaTeXTranslationPhase() {}


    QString LaTeXTranslationPhase::phaseName(unsigned number) const {
        QString result;
        if (number < phaseCount) {
            result = QString::fromLocal8Bit(phaseData[number].name);
        } else {
            result = QString();
        }

        return result;
    }


    TranslationPhase::TranslationMode LaTeXTranslationPhase::translationMode(unsigned number) const {
        TranslationPhase::TranslationMode result;
        if (number < phaseCount) {
            result = phaseData[number].mode;
        } else {
            result = TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION;
        }

        return result;
    }


    unsigned LaTeXTranslationPhase::numberPhases() const {
        return phaseCount;
    }


    LaTeXTranslationPhase::Phase LaTeXTranslationPhase::phase() const {
        return static_cast<Phase>(phaseNumber());
    }
}
