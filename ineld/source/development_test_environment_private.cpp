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
* This file implements the \ref DevelopmentTestEnvironmentPrivate class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QFileInfo>
#include <QString>

#include "ld_environment.h"
#include "development_environment_private.h"
#include "development_test_environment_private.h"

DevelopmentTestEnvironmentPrivate::DevelopmentTestEnvironmentPrivate(
        const QString& executableFilePath,
        bool           useHeaderFiles
    ):DevelopmentEnvironmentPrivate(
        executableFilePath,
        useHeaderFiles
    ) {}


DevelopmentTestEnvironmentPrivate::~DevelopmentTestEnvironmentPrivate() {}


Ld::Environment::Type DevelopmentTestEnvironmentPrivate::executableType() const {
    return Ld::Environment::Type::TEST_DEVELOPMENT;
}


QString DevelopmentTestEnvironmentPrivate::applicationRoot() const {
    QString result;

    #if (defined(Q_OS_LINUX))

        result = QFileInfo(executableDirectory() + "/../../../").canonicalPath();

    #elif (defined(Q_OS_WIN))

        result = QFileInfo(executableDirectory() + "/../../../").canonicalPath();

    #elif (defined(Q_OS_DARWIN))

        result = QFileInfo(executableDirectory() + "/../../../../../../").canonicalPath();

    #else

        #error Unknown platform.

    #endif

    return result;
}
