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
* This header defines the \ref Ld::CppLibraryInformation class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CPP_LIBRARY_INFORMATION_H
#define LD_CPP_LIBRARY_INFORMATION_H

#include <QString>

#include <util_hash_functions.h>

#include "ld_common.h"

namespace Ld {
    /**
     * Class that tracks information about a library.
     */
    class LD_PUBLIC_API CppLibraryInformation {
        public:
            /**
             * Enumeration of supported library types.
             */
            enum class LibraryType {
                /**
                 * Indicates an invalid library type.
                 */
                INVALID,

                /**
                 * Indicates a static library.
                 */
                STATIC,

                /**
                 * Indicates a dynamic library.
                 */
                DYNAMIC
            };

            CppLibraryInformation();

            /**
             * Constructor
             *
             * \param[in] libraryName The name of the library.  The name should exclude the path, leading "lib" and
             *                        should exclude the extension.
             *
             * \param[in] libraryType The type of the library.
             */
            CppLibraryInformation(const QString& libraryName, LibraryType libraryType);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            CppLibraryInformation(const CppLibraryInformation& other);

            ~CppLibraryInformation();

            /**
             * Method you can use to obtain the library name.
             *
             * \return Returns the library name.
             */
            const QString& libraryName() const;

            /**
             * Method you can use to obtain the library type.
             *
             * \return Returns the library type.
             */
            LibraryType libraryType() const;

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this object.
             */
            CppLibraryInformation& operator=(const CppLibraryInformation& other);

            /**
             * Comparison operator
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instances are equal.  Returns false if the instances are not equal.
             */
            bool operator==(const CppLibraryInformation& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instances are not equal.  Returns false if the instances are equal.
             */
            bool operator!=(const CppLibraryInformation& other) const;

        private:
            /**
             * The current library name.
             */
            QString currentLibraryName;

            /**
             * The current library type.
             */
            LibraryType currentLibraryType;
    };

    /**
     * Function used to hash a \ref Ld::CppLibraryInformation instance.
     *
     * \param[in] value The value to be hashed.
     *
     * \param[in] seed  An optional seed.
     *
     * \return Returns a hash of the provided \ref Ld::CppLibraryInformation instance.
     */
    Util::HashResult qHash(const CppLibraryInformation& value, Util::HashSeed seed = 0);
}

#endif
