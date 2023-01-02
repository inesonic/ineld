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
* This header defines the \ref DeploymentMainApplicationEnvironmentPrivate class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef DEPLOYMENT_MAIN_APPLICATION_ENVIRONMENT_PRIVATE_H
#define DEPLOYMENT_MAIN_APPLICATION_ENVIRONMENT_PRIVATE_H

#include <QString>
#include <QList>

#include "ld_common.h"
#include "ld_environment.h"
#include "deployment_environment_private.h"

/**
 * Base class used to track files for the main application in the deployed environment.
 */
class LD_PUBLIC_API DeploymentMainApplicationEnvironmentPrivate:public DeploymentEnvironmentPrivate {
    public:
        /**
         * Constructor
         *
         * \param[in] executableFilePath The full path to the executable.  All paths are relative to this file.
         *
         * \param[in] useHeaderFiles     If true, the environment will be configured to use header files.  If false,
         *                               the environment will be configured to use PCH files.
         */
        DeploymentMainApplicationEnvironmentPrivate(const QString& executableFilePath, bool useHeaderFiles);

        ~DeploymentMainApplicationEnvironmentPrivate() override;

        /**
         * Method that indicates the type of supplied executable.
         *
         * \returns the type of supported executable.
         */
        Ld::Environment::Type executableType() const final;

        /**
         * Method you can call to get the application root root directory.
         *
         * \return Returns the build root directory.
         */
        QString applicationRoot() const final;
};

#endif
