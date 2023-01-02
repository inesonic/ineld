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
* This file implements the \ref Ld::OperationDatabase class.
***********************************************************************************************************************/

#include <QHash>

#include <cassert>

#include "ld_element_structures.h"
#include "ld_operation.h"
#include "ld_operation_database.h"

namespace Ld {
    OperationDatabase::OperationDatabase() {}


    OperationDatabase::OperationDatabase(const OperationDatabase& other) {
        operationsByHandle  = other.operationsByHandle;
        operationsByElement = other.operationsByElement;
    }


    OperationDatabase::~OperationDatabase() {}


    bool OperationDatabase::isEmpty() const {
        return operationsByHandle.isEmpty();
    }


    bool OperationDatabase::isNotEmpty() const {
        return !isEmpty();
    }


    Operation::Handle OperationDatabase::size() const {
        return static_cast<Operation::Handle>(operationsByHandle.size());
    }


    void OperationDatabase::clear() {
        operationsByHandle.clear();
        operationsByElement.clear();
    }


    bool OperationDatabase::contains(ElementPointer element) const {
        return operationsByElement.contains(element.toWeakRef());
    }


    bool OperationDatabase::contains(Operation::Handle handle) const {
        return handle < size();
    }


    Operation OperationDatabase::createOperation(ElementPointer element) {
        Operation result;

        if (!operationsByElement.contains(element.toWeakRef())) {
            result.setElement(element);
            result.setHandle(size());

            operationsByHandle.append(result);
            operationsByElement.insert(element.toWeakRef(), result);
        }

        return result;
    }


    Operation OperationDatabase::fromElement(ElementPointer element) const {
        return operationsByElement.value(element);
    }


    Operation OperationDatabase::fromHandle(Operation::Handle handle) const {
        Operation result;

        if (handle < size()) {
            result = operationsByHandle.at(handle);
        }

        return result;
    }


    const QList<Operation>& OperationDatabase::operations() const {
        return operationsByHandle;
    }
}
