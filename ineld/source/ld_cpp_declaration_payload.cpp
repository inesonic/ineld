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
* This file implements the \ref Ld::CppDeclarationPayload class.
***********************************************************************************************************************/

#include <QSharedPointer>
#include <QString>
#include <QByteArray>
#include <QHash>

#include <cstdint>

#include "ld_cpp_declaration_payload.h"
#include "ld_cpp_declaration_payload_private.h"

namespace Ld {
    static QHash<QString, CppDeclarationPayload> payloadsByLibrary;
    static const QString                         dummyString;

    bool CppDeclarationPayload::registerPayload(const CppDeclarationPayload& newPayload) {
        bool result;

        if (!payloadsByLibrary.contains(newPayload.library())) {
            payloadsByLibrary.insert(newPayload.library(), newPayload);
            result = true;
        } else {
            result = false;
        }

        return result;
    }


    CppDeclarationPayload CppDeclarationPayload::payload(const QString& libraryName) {
        return payloadsByLibrary.value(libraryName);
    }


    CppDeclarationPayload::CppDeclarationPayload():impl() {}


    CppDeclarationPayload::CppDeclarationPayload(
            const QString&       library,
            const unsigned char* payload,
            unsigned long        length,
            bool                 compressed
        ):impl(new Private(
            library,
            payload,
            length,
            compressed
        )) {}


    CppDeclarationPayload::CppDeclarationPayload(
            const CppDeclarationPayload& other
        ):impl(
            other.impl
        ) {}


    bool CppDeclarationPayload::isValid() const {
        return !impl.isNull();
    }


    bool CppDeclarationPayload::isInvalid() const {
        return impl.isNull();
    }


    const QString& CppDeclarationPayload::library() const {
        return !impl.isNull() ? impl->library() : dummyString;
    }


    const unsigned char* CppDeclarationPayload::rawPayload() const {
        return !impl.isNull() ? impl->rawPayload() : nullptr;
    }


    unsigned long CppDeclarationPayload::length() const {
        return !impl.isNull() ? impl->length() : 0;
    }


    bool CppDeclarationPayload::isCompressed() const {
        return !impl.isNull() ? impl->isCompressed() : false;
    }


    QByteArray CppDeclarationPayload::payload() const {
        return !impl.isNull() ? impl->payload() : QByteArray();
    }


    CppDeclarationPayload& CppDeclarationPayload::operator=(const CppDeclarationPayload& other) {
        impl = other.impl;
        return *this;
    }
}
