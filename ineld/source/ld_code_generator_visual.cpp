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
* This file implements the \ref Ld::CodeGeneratorVisual class.
***********************************************************************************************************************/

#include <QSharedPointer>

#include "ld_element_structures.h"
#include "ld_code_generator.h"
#include "ld_code_generator_output_type.h"
#include "ld_diagnostic.h"
#include "ld_diagnostic_structures.h"
#include "ld_translation_phase.h"
#include "ld_code_generator_visual.h"

namespace Ld {
    CodeGeneratorVisual::CodeGeneratorVisual() {
        currentCodeGenerator = nullptr;
    }


    CodeGeneratorVisual::~CodeGeneratorVisual() {
        if (currentCodeGenerator != nullptr) {
            currentCodeGenerator->currentVisual = nullptr;
        }
    }


    CodeGenerator* CodeGeneratorVisual::codeGenerator() const {
        return currentCodeGenerator;
    }


    void CodeGeneratorVisual::translationStarted(
        QSharedPointer<RootElement>,
        const CodeGeneratorOutputTypeContainer&,
        unsigned long
    ) {}


    void CodeGeneratorVisual::translationPhaseStarted(
            QSharedPointer<RootElement>,
            const CodeGeneratorOutputTypeContainer&,
            const TranslationPhase&
        ) {}


    void CodeGeneratorVisual::translationStepCompleted(unsigned long) {}


    void CodeGeneratorVisual::translationCompleted(
            bool,
            QSharedPointer<RootElement>,
            const CodeGeneratorOutputTypeContainer&
        ) {}


    void CodeGeneratorVisual::translationAborted(
            QSharedPointer<RootElement>,
            const CodeGeneratorOutputTypeContainer&
        ) {}


    void CodeGeneratorVisual::translationErrorDetected(DiagnosticPointer) {}
}
