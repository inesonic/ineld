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
* This header defines the \ref Ld::Operation class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_OPERATION_H
#define LD_OPERATION_H

#include <QString>
#include <QList>
#include <QSet>
#include <QHash>

#include <util_hash_functions.h>

#include "ld_element_structures.h"
#include "ld_common.h"
#include "ld_data_type.h"
#include "ld_element_structures.h"

namespace Ld {
    /**
     * Class used to store information about a program operation.
     *
     * You can use this trivial class to track the relationship between program operations and the elements that define
     * those operations.
     */
    class LD_PUBLIC_API Operation {
        public:
            /**
             * Type used to numerically identify the operation.
             */
            typedef unsigned long Handle;

            /**
             * Value used to indicate an invalid handle.
             */
            static const Handle invalidHandle;

            Operation();

            /**
             * Constructor
             *
             * \param[in] element Element that defines this operation.
             *
             * \param[in] handle  The numeric handle used to reference this operation in the program.
             */
            Operation(ElementPointer element, Handle handle);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            Operation(const Operation& other);

            ~Operation();

            /**
             * Method that indicates if the operation is valid.
             *
             * \return Returns true if the entry is valid.  Returns false if the entry is invalid.
             */
            bool isValid() const;

            /**
             * Method that indicates if the operation is invalid.
             *
             * \return Returns true if the entry is invalid.  Returns false if the entry is valid.
             */
            bool isInvalid() const;

            /**
             * Method you can use to set the handle associated with this operation.
             *
             * \param[in] newHandle The new handle to assign to this operation.
             */
            void setHandle(Handle newHandle);

            /**
             * Method you can use to obtain the handle associated with this operation.
             *
             * \return Returns the handle tied to this operation.
             */
            Handle handle() const;

            /**
             * Method you can use to set the element associated with this operation.
             *
             * \param[in] newElement The element tied to the the operation.
             */
            void setElement(ElementPointer newElement);

            /**
             * Method you can use to obtain the element associated with the operation.
             *
             * \return Returns a pointer to the element.  A null pointer may be returned if the element no longer
             *         exists.
             */
            ElementPointer element() const;

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be copied
             *
             * \return Returns a reference to this object.
             */
            Operation& operator=(const Operation& other);

        private:
            /**
             * The operation handle.
             */
            Handle currentHandle;

            /**
             * Weak pointer to the element tied to the operation.
             */
            ElementWeakPointer currentElement;
    };

    /**
     * Hashing function for use with the \ref Ld::Operation class.  This function exists to support the Qt QSet and
     * QHash classes.
     *
     * \param[in] entry The entry to calculate a hash for.
     *
     * \param[in] seed  An optional seed value.
     *
     * \return Returns a hash for the provided seed.
     */
    LD_PUBLIC_API Util::HashResult qHash(const Operation& entry, Util::HashSeed seed = 0);
};

#endif
