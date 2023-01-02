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
* This file implements the \ref Ld::FormatContainer class.
***********************************************************************************************************************/

#include <cassert>

#include "ld_format_structures.h"
#include "ld_format.h"
#include "ld_format_container.h"

namespace Ld {
    FormatContainer::FormatContainer() {
        currentFormatInstance = FormatPointer();
    }


    FormatContainer::FormatContainer(const Format* formatInstance) {
        currentFormatInstance = FormatPointer(formatInstance->clone());
    }


    FormatContainer::FormatContainer(FormatPointer formatInstance) {
        currentFormatInstance = FormatPointer(formatInstance->clone());
    }


    FormatContainer::FormatContainer(const FormatContainer& other) {
        currentFormatInstance = other.currentFormatInstance;
    }


    FormatContainer::~FormatContainer() {}



    void FormatContainer::setFormat(Format* formatInstance) {
        currentFormatInstance = FormatPointer(formatInstance->clone());
    }


    void FormatContainer::setFormat(FormatPointer formatInstance) {
        currentFormatInstance = formatInstance;
    }


    FormatPointer FormatContainer::format() const {
        return currentFormatInstance;
    }


    FormatContainer& FormatContainer::operator=(const FormatContainer& other) {
        currentFormatInstance = other.currentFormatInstance;
        return *this;
    }


    Format& FormatContainer::operator*() {
        return *currentFormatInstance;
    }


    const Format& FormatContainer::operator*() const {
        return *currentFormatInstance;
    }


    bool FormatContainer::operator==(const FormatContainer& other) const {
        assert(currentFormatInstance);
        assert(other.currentFormatInstance);

        return currentFormatInstance->toString() == other.currentFormatInstance->toString();
    }


    bool FormatContainer::operator!=(const FormatContainer& other) const {
        assert(currentFormatInstance);
        assert(other.currentFormatInstance);

        return currentFormatInstance->toString() != other.currentFormatInstance->toString();
    }


    bool FormatContainer::operator<(const FormatContainer& other) const {
        assert(currentFormatInstance);
        assert(other.currentFormatInstance);

        return currentFormatInstance->toString() < other.currentFormatInstance->toString();
    }
}
