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
* This header defines the \ref Ld::Handle class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_HANDLE_H
#define LD_HANDLE_H

#include <QString>

#include <cstdint>
#include <string>

#include <util_hash_functions.h>

#include "ld_common.h"

namespace Ld {
    /**
     * Class that provides support for guaranteed unique identifier handles.  Note that the current design will *mostly*
     * guarantee unique handles across machines.  Assuming the seed provided in the \ref Handle::initialize method is
     * unique for each machine, there is as 1 in 33554432 chance that two instances of the program started at exactly
     * the same time will have a colliding handle.  Programs would then need to be shared in the same compilation.  The
     * probability of this occurring is essentially 0.
     *
     * The class has been largely written without Qt dependencies; however, several functions and methods have been
     * includes to make integration with Qt reasonably seamless.
     */
    class LD_PUBLIC_API Handle {
        public:
            /**
             * Constructor.  Creates an invalid handle value.
             */
            Handle();

            /**
             * Copy constructor.
             *
             * \param[in] other Instance to be copied.
             */
            Handle(const Handle& other);

            ~Handle();

            /**
             * Method that can be called to determine if this handle is valid.
             *
             * \return Returns true if the handle is valid.  Returns false if the handle is invalid.
             */
            bool isValid() const;

            /**
             * Method that can be called to determine if this handle is invalid.
             *
             * \return Returns true if the handle is invalid.  Returns false if the handle is valid.
             */
            bool isInvalid() const;

            /**
             * Method that must be called prior to allocating a handle to set a unique system identifier.
             *
             * \param[in] uniqueSystemId A value that must be unique for this system.
             */
            static void initialize(std::uint64_t uniqueSystemId);

            /**
             * Method that creates a new world-unique handle value.
             *
             * \return Returns a new, world unique, handle value.
             */
            static Handle create();

            /**
             * Method that converts a handle to a string.
             *
             * \return Returns a string representation of a handle.
             */
            std::string toString() const;

            /**
             * Method that converts a handle to a QString.
             *
             * \return Returns a string representation of a handle.
             */
            QString toQString() const;

            /**
             * Method that converts the handle to case insensitive string.  The string is suitable for use in
             * filenames.
             *
             * \return Returns a case insensitive string.
             */
            std::string toCaseInsensitiveString() const;

            /**
             * Method that converts the handle to case insensitive string.  The string is suitable for use in
             * filenames.
             *
             * \return Returns a case insensitive string.
             */
            QString toCaseInsensitiveQString() const;

            /**
             * Method that creates a handle from a string.
             *
             * \param[in]  string A textual representation of the handle.
             *
             * \return Returns a handle represented by the string.  An invalid handle will be returned if the provided
             *         string is invalid.
             */
            static Handle fromString(const std::string& string);

            /**
             * Method that creates a handle from a string.
             *
             * \param[in]  string A textual representation of the handle.
             *
             * \return Returns a handle represented by the string.  An invalid handle will be returned if the provided
             *         string is invalid.
             */
            static Handle fromString(const QString& string);

            /**
             * Assignment operator.  You can use this value to assign one handle value to another.
             *
             * \param[in] other The handle value to be assigned.
             *
             * \return Returns a reference to this handle.
             */
            Handle& operator=(const Handle& other);

            /**
             * Comparison operator.  You can use this value to determine if two handles are identical.
             *
             * \param[in] other The handle value to be compared against.
             *
             * \return Returns true if the handles are identical.  Returns false if the handles are different.
             */
            bool operator==(const Handle& other) const;

            /**
             * Comparison operator.  You can use this value to determine if two handles are different.
             *
             * \param[in] other The handle value to be compared against.
             *
             * \return Returns true if the handles are different.  Returns false if the handles are identical.
             */
            bool operator!=(const Handle& other) const;

            /**
             * Comparison operator.  You can use this value to order handles in sorted lists.
             *
             * \param[in] other The handle value to be compared against.
             *
             * \return Returns true if this handle should precede the other handle.  Returns false if the handles are
             *         identical or if this handle should follow the other handle.
             */
            bool operator<(const Handle& other) const;

            /**
             * Comparison operator.  You can use this value to order handles in sorted lists.
             *
             * \param[in] other The handle value to be compared against.
             *
             * \return Returns true if this handle should follow the other handle.  Returns false if the handles are
             *         identical or if this handle should precede the other handle.
             */
            bool operator>(const Handle& other) const;

            /**
             * Comparison operator.  You can use this value to order handles in sorted lists.
             *
             * \param[in] other The handle value to be compared against.
             *
             * \return Returns true if this handle should precede or is equal to the other handle.  Returns false if
             *         this handle should follow the other handle.
             */
            bool operator<=(const Handle& other) const;

            /**
             * Comparison operator.  You can use this value to order handles in sorted lists.
             *
             * \param[in] other The handle value to be compared against.
             *
             * \return Returns true if this handle should follow or is equal to the other handle.  Returns false if
             *         this handle should precede the other handle.
             */
            bool operator>=(const Handle& other) const;

            /**
             * Method you can use to calculate a numeric hash for this Handle suitable for use in hash based lookup
             * algorithms.  The method uses an optimized version of the Murmur 3 hash algorithm.
             *
             * \param[in] seed An optional seed that can be applied.
             *
             * \return Returns a hash calculated from this handle.
             */
            Util::HashResult hash(Util::HashSeed seed = 1) const;

        private:
            /**
             * Method that performs one iteration of the Murmur 3 hash algorithm.
             *
             * \param[in] keyValue    The key value to use for the calculation.
             *
             * \param[in] currentHash The current hash value.
             *
             * \return Returns the updated hash value.
             */
            static std::uint32_t murmurIteration(std::uint32_t keyValue, std::uint32_t currentHash);

            /**
             * Location used to store the next handle value.
             */
            static std::uint32_t nextLower;

            /**
             * Location used to store the upper 32-bits to assign to newly created handles.
             */
            static std::uint64_t nextUpper;

            /**
             * Holds the handle lower value.
             */
            std::uint32_t lower;

            /**
             * Holds the handle upper value.
             */
            std::uint64_t upper;
    };

    /**
     * Hashing function for use with the Handle class.  This function exists to support the Qt QSet and QHash
     * classes.
     *
     * \see Handle::calculateHash
     *
     * \param[in] handle The handle to calculate a hash for.
     *
     * \param[in] seed   An optional seed value.
     *
     * \return Returns a hash for the provided seed.
     */
    LD_PUBLIC_API Util::HashResult qHash(const Handle& handle, Util::HashSeed seed = 1);
};

#endif
