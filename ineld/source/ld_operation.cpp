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
* This file implements the \ref Ld::Operation class.
***********************************************************************************************************************/

#include <QHash>

#include <cassert>

#include <util_hash_functions.h>

#include "ld_element_structures.h"
#include "ld_operation.h"

namespace Ld {
    const Operation::Handle Operation::invalidHandle = static_cast<Operation::Handle>(-1);

    Operation::Operation() {
        currentHandle = invalidHandle;
    }


    Operation::Operation(ElementPointer element, Operation::Handle handle) {
        currentElement = element.toWeakRef();
        currentHandle  = handle;
    }


    Operation::Operation(const Operation& other) {
        currentElement = other.currentElement;
        currentHandle  = other.currentHandle;
    }


    Operation::~Operation() {}


    bool Operation::isValid() const {
        return currentHandle != invalidHandle && !currentElement.isNull();
    }


    bool Operation::isInvalid() const {
        return !isValid();
    }


    void Operation::setHandle(Handle newHandle) {
        currentHandle = newHandle;
    }


    Operation::Handle Operation::handle() const {
        return currentHandle;
    }


    void Operation::setElement(ElementPointer newElement) {
        currentElement = newElement.toWeakRef();
    }


    ElementPointer Operation::element() const {
        return currentElement.toStrongRef();
    }


    Operation& Operation::operator=(const Operation& other) {
        currentElement = other.currentElement;
        currentHandle  = other.currentHandle;

        return *this;
    }


    Util::HashResult qHash(const Operation& entry, Util::HashSeed seed ) {
        return ::qHash(entry.element(), seed) ^ ::qHash(entry.handle(), seed);
    }
}

