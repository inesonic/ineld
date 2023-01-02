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
* This file implements the \ref Ld::CppContext class.
***********************************************************************************************************************/

#include <QString>
#include <QByteArray>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QSet>

#include <cbe_cpp_compiler_context.h>
#include <cbe_linker_context.h>
#include <cbe_cpp_compiler_diagnostic.h>

#include "ld_element_structures.h"
#include "ld_element.h"
#include "ld_cpp_code_generation_engine.h"
#include "ld_cpp_context.h"

namespace Ld {
    CppContext::CppContext(
            const QString&           objectFile,
            const QString&           outputFile,
            CppCodeGenerationEngine* engine
        ):Cbe::CppCompilerContext(
            objectFile
        ),Cbe::LinkerContext(
            outputFile,
            objectFile
        ) {
        currentEngine  = engine;
        lastByteOffset = -1;
    }


    CppContext::~CppContext() {}


    void CppContext::startedNewStatement() {
        lastByteOffset = byteOffset();
    }


    void CppContext::startNewStatement() {
        if (lastByteOffset >= 0 && byteOffset() != static_cast<unsigned long>(lastByteOffset)) {
            append(";\n");
        }

        lastByteOffset = byteOffset();
    }


    void CppContext::startElement(ElementPointer element) {
        elementStartLocations.insert(byteOffset(), element.toWeakRef());
    }


    ElementPointer CppContext::elementAt(unsigned long byteOffset) const {
        QMap<unsigned long, ElementWeakPointer>::const_iterator it = elementStartLocations.lowerBound(byteOffset);
        ElementPointer elementOfInterest;

        if (it != elementStartLocations.constEnd()) {
            elementOfInterest = it.value().toStrongRef();
        }

        return elementOfInterest;
    }


    void CppContext::handleCompilerDiagnostic(const Cbe::CppCompilerDiagnostic& diagnostic) {
        currentEngine->handleCompilerDiagnostic(diagnostic);
    }


    void CppContext::compilerFinished(bool success) {
        currentEngine->compilerFinished(success);
    }


    void CppContext::handleLinkerDiagnostic(const QString& diagnosticMessage) {
        currentEngine->handleLinkerDiagnostic(diagnosticMessage);
    }


    void CppContext::linkerFinished(bool success) {
        currentEngine->linkerFinished(success);
    }
}
