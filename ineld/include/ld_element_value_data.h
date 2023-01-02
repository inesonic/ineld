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
* This header defines the \ref Ld::ElementValueData class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_ELEMENT_VALUE_DATA_H
#define LD_ELEMENT_VALUE_DATA_H

#include <QString>

#include <util_hash_functions.h>

#include "ld_common.h"
#include "ld_element_structures.h"

namespace Ld {
    /**
     * Class used to store and track an element/value index pair.
     */
    class LD_PUBLIC_API ElementValueData {
        public:
            ElementValueData();

            /**
             * Constructor
             *
             * \param[in] element    The element to be tracked.
             *
             * \param[in] valueIndex The value index to be tracked with the element.
             */
            explicit ElementValueData(ElementPointer element, unsigned valueIndex = 0);

            /**
             * Copy constructor.
             *
             * \param[in] other The instance to be copied.
             */
            ElementValueData(const ElementValueData& other);

            ~ElementValueData();

            /**
             * Method you can use to set the tracked element.
             *
             * \param[in] element A shared pointer to the tracked element.
             */
            void setElement(ElementPointer element);

            /**
             * Method you can use to obtain a pointer to the desired element.
             *
             * \return Returns a pointer to the desired element.  Note that a null pointer may be returned if the
             *         desired element was deleted.
             */
            ElementPointer element() const;

            /**
             * Method you can use to set the value index.
             *
             * \param[in] newValueIndex The new value index.
             */
            void setValueIndex(unsigned newValueIndex);

            /**
             * Methid you can use to obtain the value index.
             *
             * \return Returns the value index.
             */
            unsigned valueIndex() const;

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            ElementValueData& operator=(const ElementValueData& other);

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instances are equal.  Returns false if the instances are not equal.
             */
            bool operator==(const ElementValueData& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instances are not equal.  Returns false if the instances are equal.
             */
            bool operator!=(const ElementValueData& other) const;

        private:
            /**
             * Weak pointer to the underlying instance.
             */
            ElementWeakPointer currentElement;

            /**
             * The value index.
             */
            unsigned currentValueIndex;
    };

    /**
     * Hashing function for use with the \ref Ld::VariableName class.  This function exists to support the Qt QSet and
     * QHash classes.
     *
     * \param[in] entry The entry to calculate a hash for.
     *
     * \param[in] seed  An optional seed value.
     *
     * \return Returns a hash for the provided seed.
     */
    LD_PUBLIC_API Util::HashResult qHash(const ElementValueData& entry, Util::HashSeed seed = 0);
};

#endif
