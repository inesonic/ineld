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
* This file implements the \ref Ld::CppLibraryDepencencies class.
***********************************************************************************************************************/

#include <QList>
#include <QHash>
#include <QString>

#include "ld_common.h"
#include "ld_cpp_library_information.h"
#include "ld_cpp_library_dependencies.h"

namespace Ld {
    static QHash<QString, CppLibraryDependencies> dependenciesByLibrary;

    bool CppLibraryDependencies::registerDependencies(const CppLibraryDependencies& newDependencies) {
        bool success;

        if (!dependenciesByLibrary.contains(newDependencies.libraryName())) {
            dependenciesByLibrary.insert(newDependencies.libraryName(), newDependencies);
            success = true;
        } else {
            success = false;
        }

        return success;
    }


    CppLibraryDependencies CppLibraryDependencies::dependencies(const QString& libraryName) {
        return dependenciesByLibrary.value(libraryName);
    }


    CppLibraryDependencies::CppLibraryDependencies() {}


    CppLibraryDependencies::CppLibraryDependencies(
            const QString&                      libraryName,
            const QList<CppLibraryInformation>& dependencies
        ):QList<CppLibraryInformation>(
            dependencies
        ),currentLibraryName(
            libraryName
        ) {}


    CppLibraryDependencies::CppLibraryDependencies(
            const CppLibraryDependencies& other
        ):QList<CppLibraryInformation>(
            other
        ),currentLibraryName(
            other.currentLibraryName
        ) {}


    CppLibraryDependencies::CppLibraryDependencies(
            CppLibraryDependencies&& other
        ):QList<CppLibraryInformation>(
            other
        ),currentLibraryName(
            other.currentLibraryName
        ) {}


    const QString& CppLibraryDependencies::libraryName() const {
        return currentLibraryName;
    }


    CppLibraryDependencies& CppLibraryDependencies::operator=(const CppLibraryDependencies& other) {
        QList<CppLibraryInformation>::operator=(other);
        currentLibraryName = other.currentLibraryName;

        return *this;
    }
}
