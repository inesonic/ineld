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
* This file implements the \ref Ld::CppCodeGeneratorDiagnostic::Private class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>

#include <cassert>

#include "ld_cpp_translation_phase.h"
#include "ld_cpp_code_generator_diagnostic.h"
#include "ld_cpp_code_generator_diagnostic_private.h"

namespace Ld {
    CppCodeGeneratorDiagnostic::Private::Private(
            const CppTranslationPhase&               translationPhase,
            CppCodeGeneratorDiagnostic::Code         diagnosticCode,
            const QString&                           backendMessage,
            QSharedPointer<CppContext>               context,
            CppCodeGeneratorDiagnostic::CompilerCode compilerDiagnosticCode,
            unsigned long                            byteOffset,
            unsigned long                            lineNumber,
            unsigned long                            columnNumber
        ) {
        currentTranslationPhase       = translationPhase;
        currentDiagnosticCode         = diagnosticCode;
        currentBackendMessage         = backendMessage;
        currentContext                = context;
        currentCompilerDiagnosticCode = compilerDiagnosticCode;
        currentByteOffset             = byteOffset;
        currentLineNumber             = lineNumber;
        currentColumnNumber           = columnNumber;
    }


    CppCodeGeneratorDiagnostic::Private::~Private() {}


    const CppTranslationPhase& CppCodeGeneratorDiagnostic::Private::translationPhase() const {
        return currentTranslationPhase;
    }


    CppCodeGeneratorDiagnostic::Code CppCodeGeneratorDiagnostic::Private::diagnosticCode() const {
        return currentDiagnosticCode;
    }


    QSharedPointer<CppContext> CppCodeGeneratorDiagnostic::Private::context() const {
        return currentContext;
    }


    unsigned long CppCodeGeneratorDiagnostic::Private::byteOffset() const {
        return currentByteOffset;
    }


    unsigned long CppCodeGeneratorDiagnostic::Private::lineNumber() const {
        return currentLineNumber;
    }


    unsigned long CppCodeGeneratorDiagnostic::Private::columnNumber() const {
        return currentColumnNumber;
    }


    CppCodeGeneratorDiagnostic::CompilerCode CppCodeGeneratorDiagnostic::Private::compilerCode() const {
        return currentCompilerDiagnosticCode;
    }


    QString CppCodeGeneratorDiagnostic::Private::backendMessage() const {
        return currentBackendMessage;
    }
}
