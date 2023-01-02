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
* This file implements the \ref Ld::CppLibraryInformation class.
***********************************************************************************************************************/

#include <QString>
#include <QHash>

#include <util_hash_functions.h>

#include "ld_cpp_library_information.h"

namespace Ld {
    CppLibraryInformation::CppLibraryInformation() {
        currentLibraryType = LibraryType::INVALID;
    }


    CppLibraryInformation::CppLibraryInformation(
            const QString&                     libraryName,
            CppLibraryInformation::LibraryType libraryType
        ):currentLibraryName(
            libraryName
        ),currentLibraryType(
            libraryType
        ) {}


    CppLibraryInformation::CppLibraryInformation(
            const CppLibraryInformation& other
        ):currentLibraryName(
            other.currentLibraryName
        ),currentLibraryType(
            other.currentLibraryType
        ) {}


    CppLibraryInformation::~CppLibraryInformation() {}


    const QString& CppLibraryInformation::libraryName() const {
        return currentLibraryName;
    }


    CppLibraryInformation::LibraryType CppLibraryInformation::libraryType() const {
        return currentLibraryType;
    }

    CppLibraryInformation& CppLibraryInformation::operator=(const CppLibraryInformation& other) {
        currentLibraryName = other.currentLibraryName;
        currentLibraryType = other.currentLibraryType;

        return *this;
    }


    bool CppLibraryInformation::operator==(const CppLibraryInformation& other) const {
        return currentLibraryName == other.currentLibraryName && currentLibraryType == other.currentLibraryType;
    }


    bool CppLibraryInformation::operator!=(const CppLibraryInformation& other) const {
        return !operator==(other);
    }


    Util::HashResult qHash(const CppLibraryInformation& value, Util::HashSeed seed) {
        return ::qHash(value.libraryName(), seed) ^ ::qHash(static_cast<unsigned>(value.libraryType()), seed);
    }
}
