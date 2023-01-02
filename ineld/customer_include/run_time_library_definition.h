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
* This header provides key defines used to describe user third party run-time libraries.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef RUN_TIME_LIBRARY_DEFINITION_H
#define RUN_TIME_LIBRARY_DEFINITION_H

/**
 * Enumeration of library types.
 */
enum class LibraryType {
    /**
     * Indicates a customer implemented dynamic library.
     */
    CUSTOMER_DYNAMIC_LIBRARY,

    /**
     * Indicates a customer implemented static library.
     */
    CUSTOMER_STATIC_LIBRARY,

    /**
     * Indicates an Inesonic dynamic library.
     */
    VENDOR_DYNAMIC_LIBRARY,

    /**
     * Indicates an Inesonic dynamic library.
     */
    VENDOR_STATIC_LIBRARY
};

/**
 * Structure that defines a single run-time library.  The last entry in the list should populate all fields with null
 * pointers.
 *
 * Unless otherwise specified, all provided strings should be UTF-8 encoded.
 */
struct RunTimeLibraryDefinition {
    /**
     * A pointer to a nul terminated string holding the library name.  The library name should exclude the file
     * extension, any path information, and should exclude any leading "lib" prefix.
     */
    const char* libraryName;

    /**
     * A flag indicating if this library is a static library or dynamic library.
     */
    LibraryType libraryType;
};

#endif
