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
* This header defines the \ref Ld::FormatContainer class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_FORMAT_CONTAINER_H
#define LD_FORMAT_CONTAINER_H

#include <QSharedPointer>

#include "ld_common.h"
#include "ld_format_structures.h"

namespace Ld {
    class Format;

    /**
     * Class that provides a container for pointers to classes derived from \ref Ld::Format.  The class will clone the
     * provided format when the class instance is created or when a new \ref Ld::Format instance is provided and will
     * delete the created, cloned, instance when the \ref Ld::FormatContainer is destroyed.
     */
    class LD_PUBLIC_API FormatContainer {
        public:
            FormatContainer();

            /**
             * Constructor, initializes this object from a \ref Ld::Format instance.
             *
             * \param[in] formatInstance The format instance to be held in this container.
             */
            FormatContainer(const Format* formatInstance);

            /**
             * Constructor, initializes this object from a \ref Ld::Format instance.
             *
             * \param[in] formatInstance The format instance to be held in this container.
             */
            FormatContainer(FormatPointer formatInstance);

            /**
             * Copy constructor.
             *
             * \param[in] other The instance to be copied.
             */
            FormatContainer(const FormatContainer& other);

            ~FormatContainer();

            /**
             * Method that can be called to explicitly set the underlying \ref Ld::Format instance.  This method will
             * also reset the number of references back to zero.
             *
             * \param[in] formatInstance A pointer to the underlying format instance.  This instance will be cloned and
             *                           thus will not track changes to the provided format instance.
             */
            void setFormat(Format* formatInstance);

            /**
             * Method that can be called to explicitly set the underlying \ref Ld::Format instance.  This method will
             * also reset the number of references back to zero.
             *
             * \param[in] formatInstance A pointer to the underlying format instance.  Note that the shared pointer is
             *                           stored directly in the container without cloning.
             */
            void setFormat(FormatPointer formatInstance);

            /**
             * Method that can be called to obtain the underlying \ref Ld::Format instance.  Do not change the provided
             * format instance.
             *
             * \return Returns a shared pointer to the underlying format instance.
             */
            FormatPointer format() const;

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to assign to this instance.
             */
            FormatContainer& operator=(const FormatContainer& other);

            /**
             * Casting operator.  Allows access to the underlying \ref Ld::Format instance.
             *
             * \return Returns a reference to a \ref Ld::Format instance.
             */
            Format& operator*();

            /**
             * Casting operator.  Allows access to the underlying \ref Ld::Format instance.
             *
             * \return Returns a pointer to a \ref Ld::Format instance.
             */
            const Format& operator*() const;

            /**
             * Comparison operator.  Determines if this instance is the same as another instance.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if this instance are the same.  Returns false if the instances are different.
             */
            bool operator==(const FormatContainer& other) const;

            /**
             * Comparison operator.  Determines if this instance is different from another instance.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if this instance are different.  Returns false if the instances are the same.
             */
            bool operator!=(const FormatContainer& other) const;

            /**
             * Comparison operator.  Determines if this instance should precede another instance in the self-sorting
             * data structure.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if this instance should precede the other instance.  Returns false if this instance
             *         should follow the other instance.
             */
            bool operator<(const FormatContainer& other) const;

        private:
            /**
             * A pointer to the underlying format.
             */
            FormatPointer currentFormatInstance;
    };

    /**
     * Type you can use to uniquely identify a format by a numeric value.
     */
    typedef unsigned long FormatIdentifier;

    /**
     * Type you can use to tie a unique format to an identifier.
     */
    typedef QMap<FormatContainer, FormatIdentifier> FormatIdentifiersByFormat;

    /**
     * Type you can use to tie an identifier to a format.
     */
    typedef QMap<FormatIdentifier, FormatContainer> FormatsByIdentifier;

    /**
     * Type you can use to categorize and track formats by type and frequency of occurrance.
     */
    typedef QMap<QString, QMap<FormatIdentifier, FormatContainer>> FormatParedos;

    /**
     * Value used to indicate an invalid format identifier value.
     */
    static constexpr FormatIdentifier invalidFormatIdentifier = static_cast<FormatIdentifier>(-1);

}

#endif
