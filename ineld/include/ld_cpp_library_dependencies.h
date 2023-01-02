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
* This header defines the \ref Ld::CppLibraryDependencies class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CPP_LIBRARY_DEPENDENCIES_H
#define LD_CPP_LIBRARY_DEPENDENCIES_H

#include <QList>
#include <QSet>
#include <QString>

#include "ld_common.h"
#include "ld_cpp_library_information.h"

namespace Ld {
    /**
     * Class that tracks dependencies between libraries.  You can use this class as a list of dependent libraries
     * tied to a specific, specified, library.
     */
    class LD_PUBLIC_API CppLibraryDependencies:public QList<CppLibraryInformation> {
        public:
            /**
             * Method you can use to register a new dependency.
             *
             * \param[in] newDependencies The new library dependency information.
             *
             * \return Returns true on success, returns false if a payload with the requested name already exists.
             */
            static bool registerDependencies(const CppLibraryDependencies& newDependencies);

            /**
             * Method you can use to obtain an instance of this class by library name.
             *
             * \param[in] libraryName The library that we want dependencies for.
             */
            static CppLibraryDependencies dependencies(const QString& libraryName);

            CppLibraryDependencies();

            /**
             * Constructor.
             *
             * \param[in] libraryName  The library we're defining dependencies for.  The name should exclude the path,
             *                         any leading "lib" and the extension.
             *
             * \param[in] dependencies The list of library dependencies.
             */
            CppLibraryDependencies(
                const QString&                      libraryName,
                const QList<CppLibraryInformation>& dependencies
            );

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            CppLibraryDependencies(const CppLibraryDependencies& other);

            /**
             * Move constructor
             *
             * \param[in] other The instance to be copied.
             */
            CppLibraryDependencies(CppLibraryDependencies&& other);

            /**
             * Method you can use to obtain the library associated with the dependencies.
             *
             * \return Returns the library name associated with the dependencies.
             */
            const QString& libraryName() const;

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to be copied to this instance.
             */
            CppLibraryDependencies& operator=(const CppLibraryDependencies& other);

        private:
            /**
             * The library name.
             */
            QString currentLibraryName;
    };
}

#endif
