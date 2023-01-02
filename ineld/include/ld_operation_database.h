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
* This header defines the \ref Ld::OperationDatabase class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_OPERATION_DATABASE_H
#define LD_OPERATION_DATABASE_H

#include <QList>
#include <QHash>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_operation.h"

namespace Ld {
    /**
     * Class that maintains a database of operations.  You can use this class to track the relationship between
     * the elements associated with an operation and the handle used to reference an operation.
     */
    class LD_PUBLIC_API OperationDatabase {
        public:
            OperationDatabase();

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            OperationDatabase(const OperationDatabase& other);

            ~OperationDatabase();

            /**
             * Method you can use to determine if the database is empty.
             *
             * \return Returns true if the database is empty.  Returns false if the database is not empty.
             */
            bool isEmpty() const;

            /**
             * Method you can use to determine if the database is empty.
             *
             * \return Returns true if the database is not empty.  Returns false if the database is empty.
             */
            bool isNotEmpty() const;

            /**
             * Method you can use to determine the size of the database.
             *
             * \return Returns the size of the database, in entries.
             */
            Operation::Handle size() const;

            /**
             * Method you can use to clear the database contents.
             */
            void clear();

            /**
             * Method you can use to determine if the database contains an operation for an element.
             *
             * \param[in] element Pointer to the element to query.
             *
             * \return Returns true if the database contains an operation for the specified element.  Returns false
             *         if the database does not have an operation for the specified element.
             */
            bool contains(ElementPointer element) const;

            /**
             * Method you can use to determine if the database contains an operation for a given handle.
             *
             * \param[in] handle The handle to be queried.
             *
             * \return Returns true if the database contains an operation for the specified handle.  Returns false
             *         if the database does not have an operation for the specified handle.
             */
            bool contains(Operation::Handle handle) const;

            /**
             * Method you can use to create a new operation.
             *
             * \param[in] element The element to tie to the operation.
             *
             * \return Returns the newly created operation instance.
             */
            Operation createOperation(ElementPointer element);

            /**
             * Method you can use to obtain the \ref Ld::Operation data for an element.
             *
             * \param[in] element Pointer to the element to query.
             *
             * \return Returns the operation instance associated with the element.  An invalid operation instance is
             *         returned if there is no operation associated with the element.
             */
            Operation fromElement(ElementPointer element) const;

            /**
             * Method you can use to obtain the \ref Ld::Operation data for a handle.
             *
             * \param[in] handle The operation handle to query.
             *
             * \return Returns the operation instance associated with the handle.  An invalid operation instance is
             *         returned if there is no operation associated with the handle.
             */
            Operation fromHandle(Operation::Handle handle) const;

            /**
             * Method you can use to obtain a list of operations in handle order.
             *
             * \return Returns a list of all operations in handle order.
             */
            const QList<Operation>& operations() const;

            /**
             * Assignment operator
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            OperationDatabase& operator=(const OperationDatabase& other) {
                operationsByHandle = other.operationsByHandle;
                operationsByElement = other.operationsByElement;

                return *this;
            }

            /**
             * Assignment operator (move semantics)
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            OperationDatabase& operator=(OperationDatabase&& other) {
                operationsByHandle = other.operationsByHandle;
                operationsByElement = other.operationsByElement;

                return *this;
            }

        private:
            /**
             * A list of all operations in handle order.
             */
            QList<Operation> operationsByHandle;

            /**
             * A hash used to locate operations by element.
             */
            QHash<ElementWeakPointer, Operation> operationsByElement;
    };
};

#endif
