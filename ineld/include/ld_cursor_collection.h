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
* This header defines the \ref Ld::CursorCollection class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CURSOR_COLLECTION_H
#define LD_CURSOR_COLLECTION_H

#include <QSharedPointer>
#include <QWeakPointer>
#include <QSet>

#include "ld_common.h"
#include "ld_cursor.h"

namespace Ld {
    class CursorWeakCollection;
    class CursorStateCollection;

    /**
     * This class provides support for a container used to hold a group of cursor instances.  The class provides
     * mechanisms to capture cursor state information and restore cursor state information.
     */
    class LD_PUBLIC_API CursorCollection:public QSet<CursorPointer> {
        public:
            CursorCollection();

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            CursorCollection(const CursorCollection& other);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            CursorCollection(const CursorWeakCollection& other);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            CursorCollection(const CursorStateCollection& other);

            ~CursorCollection();

            /**
             * Assignment operator
             *
             * \param[in] other The instance to assign to this instance.
             */
            CursorCollection& operator=(const CursorCollection& other);

            /**
             * Assignment operator
             *
             * \param[in] other The instance to assign to this instance.
             */
            CursorCollection& operator=(const CursorStateCollection& other);

            /**
             * Assignment operator
             *
             * \param[in] other The instance to assign to this instance.
             */
            CursorCollection& operator=(const CursorWeakCollection& other);
    };
}

#endif
