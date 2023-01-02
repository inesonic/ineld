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
* This header defines the \ref Ld::Environment class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_ENVIRONMENT_H
#define LD_ENVIRONMENT_H

#include <QString>
#include <QList>
#include <QScopedPointer>

#include "ld_common.h"

namespace Ld {
    class EnvironmentPrivate;

    /**
     * Static class used to track details of the run-time environment.  You can use this class to identify the location
     * of files used by the application.
     *
     * The class will locate everything based on the location of the executable.  You can optionally override any given
     * location using the environment variables listed below:
     *
     * \rst:leading-asterisk
     * +------------------------------------------------+--------------------------------------------------------------+
     * | Environment Variable                           | Function                                                     |
     * +================================================+==============================================================+
     * | INESONIC_APPLICATION_ROOT                      | You can use this environment variable to specify the top     |
     * |                                                | level directory to use for application files.                |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_USE_HEADER_FILES                      | If set to "true", "yes", "1" (case insensitive) then the     |
     * |                                                | application will use C++ source header files rather than PCH |
     * |                                                | files.                                                       |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_SUPPORT_EXECUTABLE_DIRECTORY          | You can use this environment variable to override the        |
     * |                                                | location of support executables such as the linker.          |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_COMPILER_ROOT                         | You can use this environment variable to set the root        |
     * |                                                | directory for the compiler back-end.                         |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_GCC_PREFIX                            | You can use this environment variable to force the compiler  |
     * |                                                | backend to use a specific GCC instance for its run-time      |
     * |                                                | libraries.  The environment variable should point to the GCC |
     * |                                                | prefix directory and assume a standard GCC install.          |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_COMPILER_RESOURCE_DIRECTORY           | You can use this environment variable to set the compiler's  |
     * |                                                | resource directory.                                          |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_HEADER_SEARCH_PATHS                   | You can use this environment variable to override the header |
     * |                                                | search paths used by the tool with your own list.  On MacOS  |
     * |                                                | and Linux, use a colon as a separated.  On Windows, use a    |
     * |                                                | semicolon as a separator.                                    |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_PCH_SEARCH_PATHS                      | You can use this environment variable to override the PCH    |
     * |                                                | search paths used by the tool with your own list.  On MacOS  |
     * |                                                | and Linux, use a colon as a separator.  On Windows, use a    |
     * |                                                | semicolon as a separator.                                    |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_LIBRARY_SEARCH_PATHS                  | You can use this environment variable to override the        |
     * |                                                | library search paths used by the tool with your own list.    |
     * |                                                | On MacOS and Linux, use a colon as a separator.  On Windows, |
     * |                                                | use a semicolon as a separator.                              |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_STANDARD_HEADER_FILES                 | You can use this environment variable to override the list   |
     * |                                                | of headers referenced by the compiler.  On MacOS and Linux,  |
     * |                                                | use a colon as a separator.  On Windows, use a semicolon as  |
     * |                                                | a separator.                                                 |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_STANDARD_PCH_FILES                    | You can use this environment variable to override the list   |
     * |                                                | of PCH files referenced by the compiler.  On MacOS and       |
     * |                                                | Linux, use a colon as separator.  On Windows, use a          |
     * |                                                | semicolon as a separator.                                    |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_STANDARD_STATIC_LIBRARIES             | You can use this environment variable to override the list   |
     * |                                                | of standard libraries referenced by the compiler and         |
     * |                                                | statically linked against the customer's model.  On MacOS    |
     * |                                                | and Linux, use a colon as a separator.  On Windows, use a    |
     * |                                                | semicolon as a separator.                                    |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_STANDARD_DYNAMIC_LIBRARIES            | You can use this environment variable to override the list   |
     * |                                                | of standard libraries referenced by the compiler and         |
     * |                                                | dynamically linked against the customer's model.  On MacOS   |
     * |                                                | and Linux, use a colon as a separator.  On Windows, use a    |
     * |                                                | semicolon as a separator.                                    |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_RUNTIME_LIBRARY_SEARCH_PATHS          | You can use this environment variable to override the        |
     * |                                                | default run-time library search paths imposed by the linker. |
     * |                                                | This environment variable is ignored on Windows.             |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_ADDITIONAL_LIBRARY_SEARCH_PATHS       | You can use this environment variable to specify additional  |
     * |                                                | paths to search for libraries.                               |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_C_RUNTIME_LIBRARY                     | You can use this environment variable to point to a specific |
     * |                                                | version of the ANSI-C runtime.  Note that this variable is   |
     * |                                                | only available on Linux.                                     |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_DL_LIBRARY                            | You can use this environment variable to point to a specific |
     * |                                                | version of the Linux DL (dynamic loader) library.            |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_FONT_SEARCH_PATHS                     | You can use this environment variable to specify the         |
     * |                                                | locations of application fonts.                              |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_EXAMPLES_DIRECTORY                    | You can use this environment variable to specify the path to |
     * |                                                | the examples top level directory.                            |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_PLUG_IN_ROOT_DIRECTORY                | You can use this environment variable to override the        |
     * |                                                | default location to search for plug-ins.                     |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_USER_PLUG_IN_ROOT_DIRECTORY           | You can use this environment variable to override the        |
     * |                                                | default location to search for user created plug-ins.        |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_PLUG_IN_FILES                         | You can use this environment variable to override the        |
     * |                                                | default list of plug-in dynamic libraries/bundles.           |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_PLUG_IN_DIRECTORIES                   | You can use this environment variable to specify the top     |
     * |                                                | level directories for each plug-in.                          |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_USER_PLUG_IN_DIRECTORIES              | You can use this environment variable to specify the top     |
     * |                                                | level directories for each user created plug-in.             |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_USER_PLUG_IN_DIRECTORIES              | You can use this environment variable to specify the top     |
     * |                                                | level directories for each user defined plug-in.             |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_PLUG_IN_HEADER_SEARCH_PATHS           | You can use this environemnt variable to specify the header  |
     * |                                                | search paths for plug-ins.                                   |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_PLUG_IN_PCH_SEARCH_PATHS              | You can use this environment variable to specify the PCH     |
     * |                                                | file search paths for plug-ins.                              |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_PLUG_IN_LIBRARY_SEARCH_PATHS          | You can use this environment variable to specify the         |
     * |                                                | libraries to search for plug-in run-time libraries.          |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_PLUG_IN_RUN_TIME_LIBRARY_SEARCH_PATHS | You can use this environment variable to specify the         |
     * |                                                | search paths to insert during link for the library search    |
     * |                                                | paths.                                                       |
     * +------------------------------------------------+--------------------------------------------------------------+
     * | INESONIC_PLUG_IN_FONT_SEARCH_PATHS             | You can use this environment variable to specify the         |
     * |                                                | locations of application fonts.                              |
     * +------------------------------------------------+--------------------------------------------------------------+
     *
     * \endrst
     *
     * Note that, by convention, the compiler system root will be set to the same path used for the .PCH files.
     */
    class LD_PUBLIC_API Environment {
        public:
            /**
             * Environment variable used to indicate if header or .PCH files should be used.
             */
            static const char environmentVariableUseHeaderFiles[];

            /**
             * Environment variable used to specify the application root directory.
             */
            static const char environmentVariableApplicationRoot[];

            /**
             * Environment variable used to set the executable directory path.
             */
            static const char environmentVariableSupportExecutableDirectory[];

            /**
             * Environment variable used to specify the compiler root directory.
             */
            static const char environmentVariableCompilerRoot[];

            /**
             * Environment variable used to specify the optional GCC prefix directory.
             */
            static const char environmentVariableGccPrefix[];

            /**
             * Environment variable used to specify the resource directory.
             */
            static const char environmentVariableCompilerResourceDirectory[];

            /**
             * Environment variable used to set the head search paths.
             */
            static const char environmentVariableHeaderSearchPaths[];

            /**
             * Environment variable used to set the PCH search paths.
             */
            static const char environmentVariablePchSearchPaths[];

            /**
             * Environment variable used to set the library search paths.
             */
            static const char environmentVariableLibrarySearchPaths[];

            /**
             * Environment variable used to specify a list of header files.
             */
            static const char environmentVariableStandardHeaderFiles[];

            /**
             * Environment variable used to specify a list of .PCH files.
             */
            static const char environmentVariableStandardPchFiles[];

            /**
             * Environment variable used to specify a list of standard static libraries.
             */
            static const char environmentVariableStandardStaticLibraries[];

            /**
             * Environment variable used to specify a list of standard dynamic libraries.
             */
            static const char environmentVariableStandardDynamicLibraries[];

            /**
             * Environment variable used to specify a list of run-time library search paths.
             */
            static const char environmentVariableRunTimeLibrarySearchPaths[];

            /**
             * Environment variable used to specify additional library search paths.
             */
            static const char environmentVariableAdditionalLibrarySearchPaths[];

            /**
             * Environment variable used to set the font search paths.
             */
            static const char environmentVariableFontSearchPaths[];

            /**
             * Environment variable used to set the path to the examples directory.
             */
            static const char environmentVariableExamplesDirectory[];

            /**
             * Environment variable used to set the plug-in to-level directory.
             */
            static const char environmentVariablePlugInRootDirectory[];

            /**
             * Environment variable used to set the user created plug-in to-level directory.
             */
            static const char environmentVariableUserPlugInRootDirectory[];

            /**
             * Environment variable used to set the list of plug-in directories.
             */
            static const char environmentVariablePlugInDirectories[];

            /**
             * Environment variable used to set the list of plug-in directories.
             */
            static const char environmentVariableUserPlugInDirectories[];

            /**
             * Environment variable used to set the list of plug-in libraries/bundles.
             */
            static const char environmentVariablePlugInFiles[];

            /**
             * Environment variable used to set the plug-in header search paths.
             */
            static const char environmentVariablePlugInHeaderSearchPaths[];

            /**
             * Environment variable used to set the plug-in .PCH file search paths.
             */
            static const char environmentVariablePlugInPchSearchPaths[];

            /**
             * Environment variable used to set the plug-in library search paths.
             */
            static const char environmentVariablePlugInLibrarySearchPaths[];

            /**
             * Environment variable used to set the plug-in run-time library search paths.
             */
            static const char environmentVariablePlugInRunTimeLibrarySearchPaths[];

            /**
             * Environment variable used to set the font search paths.
             */
            static const char environmentVariablePlugInFontSearchPaths[];

            /**
             * Enumeration of the types of supported tools.
             */
            enum class Type {
                /**
                 * Use this value to indicate that this environment is for the main application during development.
                 */
                MAIN_APPLICATION_DEVELOPMENT,

                /**
                 * Use this value to indicate that this environment is for the main application during deployment.
                 */
                MAIN_APPLICATION_DEPLOYMENT,

                /**
                 * Use this value to indicate that this environment is for the test environment during development.
                 */
                TEST_DEVELOPMENT
            };

            /**
             * Method that must be called to configure the environment.  Be sure to call this method before using any of
             * the supplied static methods.
             *
             * \param[in] executableFilePath The full path to the executable.  All paths are relative to this file.
             *
             * \param[in] executableType     The type of executable.
             *
             * \param[in] useHeaderFiles     If true, the environment will be configured to use header files.  If false,
             *                               the environment will be configured to use .PCH files.
             */
            static void configure(const QString& executableFilePath, Type executableType, bool useHeaderFiles = false);

            /**
             * Method you can use to determine if C++ source header files should be used for the run-time.
             *
             * \return Returns true if C++ source headers should be used for the run-time.  Returns false if .PCH files
             *         should be used.
             */
            static bool useHeaderFiles();

            /**
             * Method you can use to determine if .PCH files should be used for the run-time.
             *
             * \return Returns true if .PCH file should be used for the run-time.  Returns false if C++ header source
             *         files should be used for the run-time.
             */
            static bool usePchFiles();

            /**
             * Method that indicates the type of supplied executable.
             *
             * \returns the type of supported executable.
             */
            static Type executableType();

            /**
             * Method you can call to get the application root directory.  On development builds, this will be the build
             * root directory.  On deployment builds, this will be the application root directory.
             *
             * \return Returns the build root directory.
             */
            static QString applicationRoot();

            /**
             * Method that returns the location of the application executable.
             *
             * \return Returns the path to the directory containing the application executable.
             */
            static QString executableDirectory();

            /**
             * Method that returns the expected path to additional executable tools used by this applcation.  Examples
             * might include the linker.
             *
             * \return Returns the location of support executables.
             */
            static QString supportExecutableDirectory();

            /**
             * Method that returns the compiler's root directory.
             *
             * \return Returns the compiler's root directory.
             */
            static QString compilerRoot();

            /**
             * Method that returns the GCC prefix directory to be used on this platform.
             *
             * \return Returns the GCC prefix directory to be used on this platform.  An empty string is returned if
             *         GCC resources should not be used.
             */
            static QString gccPrefix();

            /**
             * Method that returns the compiler's resource directory.
             *
             * \return Returns the compiler's resource directory.
             */
            static QString compilerResourceDirectory();

            /**
             * Method that returns a list of C++ source header search paths.
             *
             * \return Returns a list of C++ source header search paths.  An empty list will be returned if the
             *         environment is configured to use .PCH files.
             */
            static QList<QString> headerSearchPaths();

            /**
             * Method that returns a list of .PCH header search paths.
             *
             * \return Returns a list of .PCH search paths.  An empty list will be returned if the environment is
             *         configured to use C++ header source files.
             */
            static QList<QString> pchSearchPaths();

            /**
             * Method that returns the run-time library search paths.
             *
             * \param[in] overrideRuntime Optional path to the CRT to be used.  This value is only used on Windows
             *                            and will be ignored on other platforms.
             *
             * \return Returns a list of paths to the different run-time library search paths.
             */
            static QList<QString> librarySearchPaths(const QString& overrideRuntime = QString());

            /**
             * Method that returns a list of header files to always be included.  This method is used to specify
             * what headers should be included for the run-time.
             *
             * \return Returns a list of run-time headers.
             */
            static QList<QString> standardHeaderFiles();

            /**
             * Method that returns a list of .PCH files to always be included.  This method is used to specify what .PCH
             * files should be included for the run-time.
             *
             * \return Returns a list of run-time headers.
             */
            static QList<QString> standardPchFiles();

            /**
             * Method that returns a list of standard static libraries to always be included.
             *
             * \param[in] overrideRuntime Optional path to the CRT to be used.  This value is only used on Windows
             *                            and will be ignored on other platforms.
             *
             * \return Returns a list of libraries to be included.
             */
            static QList<QString> standardStaticLibraries(const QString& overrideRuntime = QString());

            /**
             * Method that returns a list of standard dynamic libraries to always be included.
             *
             * \return Returns a list of libraries to be included.
             */
            static QList<QString> standardDynamicLibraries();

            /**
             * Method that returns a list of run-time library search paths to be included.
             *
             * \return Returns a list of run-time library search paths.
             */
            static QList<QString> runTimeLibrarySearchPaths();

            /**
             * Method that returns a list of application font directories.
             *
             * \return Returns a list of application font directories.
             */
            static QList<QString> fontSearchPaths();

            /**
             * Method that returns the path to the examples directory.
             *
             * \return Returns the path to the examples directory.
             */
            static QString examplesDirectory();

            /**
             * Method that returns the top level directory to search for plug-ins.
             *
             * \return Returns the top level directory to search for plug-ins.
             */
            static QString plugInRootDirectory();

            /**
             * Method that returns the top level directory to search for user created plug-ins.
             *
             * \return Returns the top level directory to search for plug-ins.
             */
            static QString userPlugInRootDirectory();

            /**
             * Method that returns a list of plug-in top level directories.
             *
             * \return Returns a list of plug-in top level directories containing plug-ins.
             */
            static QList<QString> plugInDirectories();

            /**
             * Method that returns a list of user created plug-in top level directories.
             *
             * \return Returns a list of plug-in top level directories containing user created plug-ins.
             */
            static QList<QString> userPlugInDirectories();

            /**
             * Method that returns a list of plug-in dynamic load libraries/bundles.
             *
             * \return Returns a list of plug-in dynamic load libraries or bundles.
             */
            static QList<QString> plugInFiles();

            /**
             * Method that returns a list of plug-in header search paths.
             *
             * \return Returns a list of plug-in header search paths.
             */
            static QList<QString> plugInHeaderSearchPaths();

            /**
             * Method that returns a list of plug-in .PCH file search paths.
             *
             * \return Returns a list of plug-in .PCH search paths.
             */
            static QList<QString> plugInPchSearchPaths();

            /**
             * Method that returns a list of plug-in run-time library search paths.
             *
             * \return Returns a list of plug-in run-time library search paths.
             */
            static QList<QString> plugInLibrarySearchPaths();

            /**
             * Method that returns a list of run-time library search paths to be included to support plug-ins.
             *
             * \return Returns a list of run-time library search paths for plug-ins.
             */
            static QList<QString> plugInRunTimeLibrarySearchPaths();

            /**
             * Method that returns a list of plug-in run-time font search paths.
             *
             * \return Returns a list of plug-in run-time font search paths.
             */
            static QList<QString> plugInFontSearchPaths();

            /**
             * Method that returns the target tuple to use for this platform.
             *
             * \return Returns the target tuple to be used for this platform.  Some platforms may return an empty
             *         string indicating that the compiler should pick the target tuple for the platform.
             */
            static QString targetTriple();

        private:
            /**
             * Method you can call to parse a list environment variable based on the platform specific separator.
             *
             * \param[in] environmentVariable The environment variable to be parsed and split.
             *
             * \return Returns the resulting list.
             */
            static QList<QString> splitEnvironmentVariable(const char* environmentVariable);

            static QScopedPointer<EnvironmentPrivate> impl;
    };
};

#endif
