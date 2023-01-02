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
* This header defines the \ref Ld::ElementGroup class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_ELEMENT_GROUP_H
#define LD_ELEMENT_GROUP_H

#include <QList>
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_element_structures.h"

namespace Ld {
    /**
     * Class that tracks a collection of adjascent elements as a group.
     */
    class LD_PUBLIC_API ElementGroup:public ElementPointerList {
        public:
            ElementGroup();

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied to this instance.
             */
            ElementGroup(const ElementGroup& other);

            /**
             * Constructor
             *
             * \param[in] list List of elements that defines this group.
             */
            ElementGroup(const ElementPointerList list);

            ~ElementGroup();

            /**
             * Method you can use to obtain the element that immediately precedes this group.
             *
             * \return Returns a shared pointer to the element the immediately precedes this group.  A null pointer is
             *         returned if there is no immediately preceding element.
             */
            ElementPointer precedingElement() const;

            /**
             * Method you can use to obtain the element that immediately follows this group.
             *
             * \return Returns the element that immediately follows the group.  A null pointer is returned if there is
             *         no element that follows.
             */
            ElementPointer followingElement() const;

            /**
             * Static method you can use to build a list of element groups from an arbitrary list of elements.
             *
             * \param[in] elements A list of shared pointers to elements.
             *
             * \return Returns a list containing the supplied elements, grouped.
             */
            static QList<ElementGroup> convertToGroups(const ElementPointerList& elements);

            /**
             * Assignment operator
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            ElementGroup& operator=(const ElementGroup& other);
    };

    /**
     * Type used to represent a list of element groups.
     */
    typedef QList<ElementGroup> ElementGroupList;
};

#endif
