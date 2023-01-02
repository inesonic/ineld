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
* This file implements the \ref Ld::Diagnostic class.
***********************************************************************************************************************/

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QHash>
#include <QSharedPointer>

#include <util_hash_functions.h>

#include "ld_element_structures.h"
#include "ld_translation_phase.h"
#include "ld_code_generator.h"
#include "ld_diagnostic_private.h"
#include "ld_diagnostic.h"

namespace Ld {
    Diagnostic::Diagnostic() {}


    Diagnostic::Diagnostic(
            ElementPointer element,
            const QString& codeGeneratorName,
            Type           diagnosticType
        ):impl(new Diagnostic::Private(
            element,
            codeGeneratorName,
            diagnosticType
        )) {}


    Diagnostic::Diagnostic(const Diagnostic& other) {
        impl = other.impl;
    }


    Diagnostic::~Diagnostic() {}


    ElementPointer Diagnostic::element() const {
        return impl.isNull() ? nullptr : impl->element();
    }


    QString Diagnostic::codeGeneratorName() const {
        return impl.isNull() ? QString() : impl->codeGeneratorName();
    }



    Diagnostic::Type Diagnostic::diagnosticType() const {
        return impl.isNull() ? Type::NONE : impl->diagnosticType();
    }


    QSharedPointer<CodeGenerator> Diagnostic::codeGenerator() const {
        return   impl.isNull()
               ? QSharedPointer<CodeGenerator>()
               : CodeGenerator::codeGenerator(impl->codeGeneratorName());
    }


    Diagnostic& Diagnostic::operator=(const Diagnostic& other) {
        impl = other.impl;
        return *this;
    }


    QString Diagnostic::diagnosticTypeMessage() const {
        QString message;

        switch (diagnosticType()) {
            case Type::NONE:           { message = tr("No diagnostic");           break; }
            case Type::INFORMATION:    { message = tr("Information");             break; }
            case Type::PAUSED:         { message = tr("Paused");                  break; }
            case Type::FATAL_ERROR:    { message = tr("Error");                   break; }
            case Type::RUNTIME_ERROR:  { message = tr("Runtime Error");           break; }
            case Type::WARNING:        { message = tr("Warning");                 break; }
            case Type::INTERNAL_ERROR: { message = tr("Internal Error");          break; }
            default:                   { message = tr("Unknown diagnostic type"); break; }
        }

        return message;
    }


    Util::HashResult qHash(Diagnostic::Type value, Util::HashSeed seed) {
        return ::qHash(static_cast<int>(value), seed);
    }
}
