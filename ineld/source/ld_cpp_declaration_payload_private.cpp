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
* This file implements the \ref Ld::CppDeclarationPayload::Private class.
***********************************************************************************************************************/

#include <QSharedPointer>
#include <QString>
#include <QByteArray>
#include <QHash>

#include <cstdint>

#include "ld_cpp_declaration_payload.h"
#include "ld_cpp_declaration_payload_private.h"

namespace Ld {
    CppDeclarationPayload::Private::Private(
            const QString&       library,
            const unsigned char* payload,
            unsigned long        length,
            bool                 compressed
        ):currentLibraryName(
            library
        ),currentRawPayload(
            payload
        ),currentPayloadLength(
            length
        ),currentIsCompressed(
            compressed
        ) {}


    const QString& CppDeclarationPayload::Private::library() const {
        return currentLibraryName;
    }


    const unsigned char* CppDeclarationPayload::Private::rawPayload() const {
        return currentRawPayload;
    }


    unsigned long CppDeclarationPayload::Private::length() const {
        return currentPayloadLength;
    }


    bool CppDeclarationPayload::Private::isCompressed() const {
        return currentIsCompressed;
    }


    QByteArray CppDeclarationPayload::Private::payload() const {
        return   currentIsCompressed
               ? qUncompress(reinterpret_cast<const uchar*>(currentRawPayload), currentPayloadLength)
               : QByteArray(reinterpret_cast<const char*>(currentRawPayload), currentPayloadLength);
    }
}
