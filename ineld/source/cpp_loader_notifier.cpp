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
* This file implements the \ref CppLoaderNotifier class.
***********************************************************************************************************************/

#include <QString>
#include <QSet>

#include <cbe_loader_notifier.h>

#include "ld_cpp_code_generator.h"
#include "cpp_loader_notifier.h"

namespace Ld {
    CppLoaderNotifier::CppLoaderNotifier(CppCodeGenerator* generator) {
        currentGenerator = generator;
    }


    CppLoaderNotifier::~CppLoaderNotifier() {}


    void CppLoaderNotifier::libraryLoadFailed(const QString& libraryFilename, const QString& errorReason) {
        currentGenerator->executionErrorDetected(
            nullptr,
            tr("Unable to load %1\n%2").arg(libraryFilename).arg(errorReason),
            Ld::CodeGeneratorOutputTypeContainer()
        );
    }


    void CppLoaderNotifier::libraryUnloadFailed(const QString& libraryFilename) {
        currentGenerator->executionErrorDetected(
            nullptr,
            tr("Unable to unload %1").arg(libraryFilename),
            Ld::CodeGeneratorOutputTypeContainer()
        );
    }
};

