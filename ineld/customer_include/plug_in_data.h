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
* This header provides key defines used to write plug-ins.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef PLUG_IN_DATA_H
#define PLUG_IN_DATA_H

#include "user_function_definition.h"
#include "run_time_library_definition.h"

#if (defined(_WIN32) || defined(_WIN64))

    /**
     * Macro that conditionally includes platform specific stuff needed to make an entity visible.  Resolves to
     * extern "C" __declspec(dllexport) on Windows and just extern "C" on other platforms.
     */
    #define PLUG_IN_EXPORT __declspec(dllexport)

#elif (defined(__linux__) || defined(__APPLE__))

    /**
     * Macro that conditionally includes platform specific stuff needed to make an entity visible.  Resolves to
     * extern "C" __declspec(dllexport) on Windows and just extern "C" on other platforms.
     */
    #define PLUG_IN_EXPORT

#else

    #error Unknown platform.

#endif

/**
 * Define providing the expected name for the plug-in data instance.
 */
#define PLUG_IN_DATA __plugInData

/**
 * Structure that is used to register a non-Inesonic the plug-in with the system.  The system will search for an
 * instance of this structure named "__plugInData" when the plug-in is loaded and will use its contents to properly
 * configure the plug-in.  The structure is expected to be defined statically and must persist for the entire lifetime
 * of the plug-in.
 *
 * Unless otherwise specified, all provided strings should be UTF-8 encoded.
 */
typedef struct PlugInData {
    /**
     * The name of the plug-in.  You must include a unique name for the plug-in.
     */
    const char* name;

    /**
     * The plug-in author or author company, e.g. "Inesonic, LLC" or "John Doe".  You must set this value to a non-empty
     * string or the plug-in will not load.
     */
    const char* author;

    /**
     * The company that developed this plug-in.  You must set this value to a non-empty string or the plug-in will not
     * load.
     */
    const char* company;

    /**
     * The plug-in license.  You can set this to a null pointer to cause the standard Inesonic, LLC license to be used.
     */
    const char* license;

    /**
     * A brief description of the plug-in.  You must set a brief description or the plug-in will not load.
     */
    const char* briefDescription;

    /**
     * A detailed description of the plug-in.  You can set this to a null pointer to omit a detailed description.
     */
    const char* detailedDescription;

    /**
     * The plug-in version string.  This value is informational only, you can set this to any value you wish although
     * you must set a non-empty value.
     */
    const char* version;

    /**
     * A value specifying the number of functions to be instantiated by this plug-in.
     */
    unsigned numberFunctionDefinitions;

    /**
     * A pointer to an array of function definitions.
     */
    const UserFunctionDefinition* userFunctionDefinitions;

    /**
     * A pointer to an array of run-time library definitions.  The field is ignored if it is a null pointer.
     */
    const RunTimeLibraryDefinition* runTimeLibraryDefinition;
} PlugInData;

#endif
