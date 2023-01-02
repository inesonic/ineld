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
* This file implements the \ref Ld::TranslationPhase class.
***********************************************************************************************************************/

#include <QSet>

#include "ld_translation_phase.h"

namespace Ld {
    TranslationPhase::TranslationPhase() {
        currentPhaseNumber = 0;
    }


    TranslationPhase::TranslationPhase(const TranslationPhase& other) {
        currentPhaseNumber = other.currentPhaseNumber;
    }


    TranslationPhase::~TranslationPhase() {}


    QString TranslationPhase::currentPhaseName() const {
        return phaseName(currentPhaseNumber);
    }


    QString TranslationPhase::phaseName(unsigned) const {
        return QString();
    }


    TranslationPhase::TranslationMode TranslationPhase::currentTranslationMode() const {
        return translationMode(currentPhaseNumber);
    }


    TranslationPhase::TranslationMode TranslationPhase::translationMode(unsigned) const {
        return TranslationMode::NO_PER_ELEMENT_TRANSLATION;
    }


    unsigned TranslationPhase::numberPhases() const {
        return 0;
    }


    unsigned TranslationPhase::phaseNumber() const {
        return currentPhaseNumber;
    }


    bool TranslationPhase::lastPhase() const {
        return currentPhaseNumber >= numberPhases() - 1;
    }


    void TranslationPhase::reset() {
        currentPhaseNumber = 0;
    }


    void TranslationPhase::nextPhase() {
        unsigned nextPhaseNumber = currentPhaseNumber + 1;
        if (nextPhaseNumber < numberPhases()) {
            currentPhaseNumber = nextPhaseNumber;
        }
    }


    TranslationPhase& TranslationPhase::operator=(const TranslationPhase& other) {
        currentPhaseNumber = other.currentPhaseNumber;
        return *this;
    }


    bool TranslationPhase::operator==(const TranslationPhase& other) const {
        return currentPhaseNumber == other.currentPhaseNumber;
    }


    bool TranslationPhase::operator!=(const TranslationPhase& other) const {
        return currentPhaseNumber != other.currentPhaseNumber;
    }


    TranslationPhase::operator unsigned() const {
        return currentPhaseNumber;
    }
}
