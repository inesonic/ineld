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
* This header defines the \ref DevelopmentEnvironmentPrivate class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef DEVELOPMENT_ENVIRONMENT_PRIVATE_H
#define DEVELOPMENT_ENVIRONMENT_PRIVATE_H

#include <QString>
#include <QList>

#include "ld_common.h"
#include "ld_environment.h"
#include "ld_environment_private.h"

/**
 * Base class used to track files in the development environment.  This class exists mostly to facilitate testing and
 * code development.
 */
class LD_PUBLIC_API DevelopmentEnvironmentPrivate:public Ld::EnvironmentPrivate {
    public:
        /**
         * Constructor
         *
         * \param[in] executableFilePath The full path to the executable.  All paths are relative to this file.
         *
         * \param[in] useHeaderFiles     If true, the environment will be configured to use header files.  If false,
         *                               the environment will be configured to use PCH files.
         */
        DevelopmentEnvironmentPrivate(const QString& executableFilePath, bool useHeaderFiles);

        ~DevelopmentEnvironmentPrivate() override;

        /**
         * Method that returns the expected path to additional executable tools used by this applcation.  Examples
         * might include the linker.
         *
         * \return Returns the location of support executables.
         */
        QString supportExecutableDirectory() const final;

        /**
         * Method that returns the compiler's root directory.
         *
         * \return Returns the compiler's root directory.
         */
        QString compilerRoot() const final;

        /**
         * Method that returns the compiler's resource directory.
         *
         * \return Returns the compiler's resource directory.
         */
        QString compilerResourceDirectory() const final;

        /**
         * Method that returns a list of C++ source header search paths.
         *
         * \return Returns a list of C++ source header search paths.  An empty list will be returned if the
         *         environment is configured to use .PCH files.
         */
        QList<QString> headerSearchPaths() const final;

        /**
         * Method that returns a list of .PCH header search paths.
         *
         * \return Returns a list of .PCH search paths.  An empty list will be returned if the environment is
         *         configured to use C++ header source files.
         */
        QList<QString> pchSearchPaths() const final;

        /**
         * Method that returns the run-time library search paths.
         *
         * \param[in] overrideRuntime Optional path to the CRT to be used.  This value is only used on Windows and will
         *                            be ignored on other platforms.
         *
         * \return Returns a list of paths to the different run-time library search paths.
         */
        QList<QString> librarySearchPaths(const QString& overrideRuntime) const final;

        /**
         * Method that returns a list of header files to always be included.  This method is used to specify
         * what headers should be included for the run-time.
         *
         * \return Returns a list of run-time headers.
         */
        QList<QString> standardHeaderFiles() const final;

        /**
         * Method that returns a list of .PCH files to always be included.  This method is used to specify what .PCH
         * files should be included for the run-time.
         *
         * \return Returns a list of run-time headers.
         */
        QList<QString> standardPchFiles() const final;

        /**
         * Method that returns a list of standard libraries to always be statically linked against the customer
         * model.
         *
         * \param[in] overrideRuntime Optional path to the CRT to be used.  This value is only used on Windows and will
         *                            be ignored on other platforms.
         *
         * \return Returns a list of static libraries to be included.
         */
        QList<QString> standardStaticLibraries(const QString& overrideRuntime) const final;

        /**
         * Method that returns a list of standard libraries to always be dynamically linked against the customer
         * model.
         *
         * \return Returns a list of dynamic libraries to be included.
         */
        QList<QString> standardDynamicLibraries() const final;

        /**
         * Method that returns a list of run-time library search paths to be included.
         *
         * \return Returns a list of run-time library search paths.
         */
        QList<QString> runTimeLibrarySearchPaths() const final;

        /**
         * Method that returns a list of application font directories.
         *
         * \return Returns a list of application font directories.
         */
        QList<QString> fontSearchPaths() const final;

        /**
         * Method that returns the path to the examples directory.
         *
         * \return Returns the path to the examples directory.
         */
        QString examplesDirectory() const final;

        /**
         * Method that returns the top level directory to search for plug-ins.
         *
         * \return Returns the top level directory to search for plug-ins.
         */
        QString plugInRootDirectory() const final;

        /**
         * Method that returns a list of plug-in top level directories.
         *
         * \return Returns a list of plug-in top level directories containing plug-ins.
         */
        QList<QString> plugInDirectories() const final;

        /**
         * Method that returns a list of plug-in dynamic load libraries/bundles.
         *
         * \return Returns a list of plug-in dynamic load libraries or bundles.
         */
        QList<QString> plugInFiles() const final;

        /**
         * Method that returns a list of plug-in header search paths.
         *
         * \return Returns a list of plug-in header search paths.
         */
        QList<QString> plugInHeaderSearchPaths() const final;

        /**
         * Method that returns a list of plug-in .PCH file search paths.
         *
         * \return Returns a list of plug-in .PCH search paths.
         */
        QList<QString> plugInPchSearchPaths() const final;

        /**
         * Method that returns a list of plug-in run-time library search paths.
         *
         * \return Returns a list of plug-in run-time library search paths.
         */
        QList<QString> plugInLibrarySearchPaths() const final;

        /**
         * Method that returns a list of run-time library search paths to be included to support plug-ins.
         *
         * \return Returns a list of run-time library search paths for plug-ins.
         */
        QList<QString> plugInRunTimeLibrarySearchPaths() const final;

        /**
         * Method that returns a list of application plug-in run-time font directories.
         *
         * \return Returns a list of application plug-in run-time font directories.
         */
        QList<QString> plugInFontSearchPaths() const final;

    protected:
        /**
         * Method you can call to get the source root directory.
         *
         * \return Returns the source tree root directory.
         */
        QString sourceRoot() const;
};

#endif
