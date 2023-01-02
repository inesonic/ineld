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
* This header defines the \ref Ld::EnvironmentPrivate class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_ENVIRONMENT_PRIVATE_H
#define LD_ENVIRONMENT_PRIVATE_H

#include <QString>
#include <QList>

#include "ld_common.h"
#include "ld_environment.h"

namespace Ld {
    /**
     * Pure virtual base class used by \ref Ld::Environment to track and manage details of the run-time environment.
     */
    class LD_PUBLIC_API EnvironmentPrivate {
        public:
            /**
             * Constructor
             *
             * \param[in] executableFilePath The full path to the executable.  All paths are relative to this file.
             *
             * \param[in] useHeaderFiles     If true, the environment will be configured to use header files.  If false,
             *                               the environment will be configured to use .PCH files.
             */
            EnvironmentPrivate(const QString& executableFilePath, bool useHeaderFiles);

            virtual ~EnvironmentPrivate();

            /**
             * Method you can use to determine if C++ source header files should be used for the run-time.
             *
             * \return Returns true if C++ source headers should be used for the run-time.  Returns false if .PCH files
             *         should be used.
             */
            bool useHeaderFiles() const;

            /**
             * Method that returns the location of the application executable.
             *
             * \return Returns the path to the directory containing the application executable.
             */
            QString executableDirectory() const;

            /**
             * Method you can call to get the application root directory.  On development builds, this will be the build
             * root directory.  On deployment builds, this will be the application root directory.
             *
             * \return Returns the build root directory.
             */
            virtual QString applicationRoot() const = 0;

            /**
             * Method that indicates the type of supplied executable.
             *
             * \returns the type of supported executable.
             */
            virtual Environment::Type executableType() const = 0;

            /**
             * Method that returns the expected path to additional executable tools used by this applcation.  Examples
             * might include the linker.
             *
             * \return Returns the location of support executables.
             */
            virtual QString supportExecutableDirectory() const = 0;

            /**
             * Method that returns the compiler's root directory.
             *
             * \return Returns the compiler's root directory.
             */
            virtual QString compilerRoot() const = 0;

            /**
             * Method that returns the GCC prefix directory to be used on this platform.
             *
             * \return Returns the GCC prefix directory to be used on this platform.  An empty string is returned if
             *         GCC resources should not be used.
             */
            virtual QString gccPrefix() const;

            /**
             * Method that returns the compiler's resource directory.
             *
             * \return Returns the compiler's resource directory.
             */
            virtual QString compilerResourceDirectory() const = 0;

            /**
             * Method that returns a list of C++ source header search paths.
             *
             * \return Returns a list of C++ source header search paths.  An empty list will be returned if the
             *         environment is configured to use .PCH files.
             */
            virtual QList<QString> headerSearchPaths() const = 0;

            /**
             * Method that returns a list of .PCH header search paths.
             *
             * \return Returns a list of .PCH search paths.  An empty list will be returned if the environment is
             *         configured to use C++ header source files.
             */
            virtual QList<QString> pchSearchPaths() const = 0;

            /**
             * Method that returns the run-time library search paths.
             *
             * \param[in] overrideRuntime Optional path to the CRT to be used.  This value is only used on Windows
             *                            and will be ignored on other platforms.
             *
             * \return Returns a list of paths to the different run-time library search paths.
             */
            virtual QList<QString> librarySearchPaths(const QString& overrideRuntime) const = 0;

            /**
             * Method that returns a list of header files to always be included.  This method is used to specify
             * what headers should be included for the run-time.
             *
             * \return Returns a list of run-time headers.
             */
            virtual QList<QString> standardHeaderFiles() const = 0;

            /**
             * Method that returns a list of .PCH files to always be included.  This method is used to specify what .PCH
             * files should be included for the run-time.
             *
             * \return Returns a list of run-time headers.
             */
            virtual QList<QString> standardPchFiles() const = 0;

            /**
             * Method that returns a list of standard libraries to always be statically linked against the customer
             * model.
             *
             * \param[in] overrideRuntime Optional path to the CRT to be used.  This value is only used on Windows
             *                            and will be ignored on other platforms.
             *
             * \return Returns a list of static libraries to be included.
             */
            virtual QList<QString> standardStaticLibraries(const QString& overrideRuntime) const = 0;

            /**
             * Method that returns a list of standard libraries to always be dynamically linked against the customer
             * model.
             *
             * \return Returns a list of dynamic libraries to be included.
             */
            virtual QList<QString> standardDynamicLibraries() const = 0;

            /**
             * Method that returns a list of run-time library search paths to be included.
             *
             * \return Returns a list of run-time library search paths.
             */
            virtual QList<QString> runTimeLibrarySearchPaths() const = 0;

            /**
             * Method that returns a list of application font directories.
             *
             * \return Returns a list of application font directories.
             */
            virtual QList<QString> fontSearchPaths() const = 0;

            /**
             * Method that returns the path to the examples directory.
             *
             * \return Returns the path to the examples directory.
             */
            virtual QString examplesDirectory() const = 0;

            /**
             * Method that returns the top level directory to search for plug-ins.
             *
             * \return Returns the top level directory to search for plug-ins.
             */
            virtual QString plugInRootDirectory() const = 0;

            /**
             * Method that returns the top level directory to search for user created plug-ins.
             *
             * \return Returns the top level directory to search for plug-ins.
             */
            virtual QString userPlugInRootDirectory() const;

            /**
             * Method that returns a list of plug-in top level directories.
             *
             * \return Returns a list of plug-in top level directories containing plug-ins.
             */
            virtual QList<QString> plugInDirectories() const = 0;

            /**
             * Method that returns a list of user created plug-in top level directories.
             *
             * \return Returns a list of plug-in top level directories containing plug-ins.
             */
            virtual QList<QString> userPlugInDirectories() const;

            /**
             * Method that returns a list of plug-in dynamic load libraries/bundles.
             *
             * \return Returns a list of plug-in dynamic load libraries or bundles.
             */
            virtual QList<QString> plugInFiles() const = 0;

            /**
             * Method that returns a list of plug-in header search paths.
             *
             * \return Returns a list of plug-in header search paths.
             */
            virtual QList<QString> plugInHeaderSearchPaths() const = 0;

            /**
             * Method that returns a list of plug-in .PCH file search paths.
             *
             * \return Returns a list of plug-in .PCH search paths.
             */
            virtual QList<QString> plugInPchSearchPaths() const = 0;

            /**
             * Method that returns a list of plug-in run-time library search paths.
             *
             * \return Returns a list of plug-in run-time library search paths.
             */
            virtual QList<QString> plugInLibrarySearchPaths() const = 0;

            /**
             * Method that returns a list of run-time library search paths to be included to support plug-ins.
             *
             * \return Returns a list of run-time library search paths for plug-ins.
             */
            virtual QList<QString> plugInRunTimeLibrarySearchPaths() const = 0;

            /**
             * Method that returns a list of plug-in run-time font search paths.
             *
             * \return Returns a list of plug-in run-time font search paths.
             */
            virtual QList<QString> plugInFontSearchPaths() const = 0;

            /**
             * Method that returns the target tuple to use for this platform.
             *
             * \return Returns the target tuple to be used for this platform.  Some platforms may return an empty
             *         string indicating that the compiler should pick the target tuple for the platform.
             */
            virtual QString targetTriple();

        protected:
            /**
             * Function that adds a path only if the specified file exists within the path.
             *
             * \param[in,out] stringList The list to receive the requested file.
             *
             * \param[in]     path       The path to be checked.
             *
             * \param[in]     filename   The file to be checked.
             *
             * \return Returns true if the file was added.  Returns false if the file was not added.
             */
            static bool addPathIfExists(QList<QString>& stringList, const QString& path, const QString& filename);

        private:
            /**
             * The current path to the executable.
             */
            QString currentExecutableDirectory;

            /**
             * Holds true if header files should be used.
             */
            bool currentUseHeaderFiles;
    };
};

#endif
