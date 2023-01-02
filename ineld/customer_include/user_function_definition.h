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
* This header provides key defines used to register a new function.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef USER_FUNCTION_DEFINITION_H
#define USER_FUNCTION_DEFINITION_H

#include "model_api_types.h"

/**
 * Structure you can use to define a specific parameter for the function.
 *
 * Unless otherwise specified, all provided strings should be UTF-8 encoded.
 */
struct UserFunctionParameter {
    /**
     * The type of the parameter.
     */
    Model::ValueType valueType;

    /**
     * A short description you can assign to the parameter.
     */
    const char* description;
};

/**
 * Structure you can use to define a specific variant of a function.  Function variants can have a different parameter
 * list.
 */
struct UserFunctionVariant {
    /**
     * The function return value type.
     */
    Model::ValueType returnValueType;

    /**
     * The number of parameters for this function.
     */
    unsigned numberParameters;

    /**
     * Pointer to an array of accepted function parameter types.
     */
    const UserFunctionParameter* parameters;
};

/**
 * Structure you can use to define a new function.
 *
 * Unless otherwise specified, all provided strings should be UTF-8 encoded.
 */
struct UserFunctionDefinition {
    /**
     * The runtime function name.  The name will be mangled (C++ name manging) for each supported variant.  The value
     * must not be an empty string.
     */
    const char* internalName;

    /**
     * The name of the run-time library supplying this function.  The value must not be null or an empty string.
     */
    const char* runtimeLibraryName;

    /**
     * A UTF-8 string holding the function's name as seen in the application.  The value must not be an empty string.
     */
    const char* visibleName;

    /**
     * A UTF-8 string holding the function's subscript as seen in the application.  An empty string or null pointer
     * indicates no subscript.
     */
    const char* visibleNameSubscript;

    /**
     * A UTF-8 string holding the command to assign to the function.  The command must be unique.  An empty string or
     * null pointer indicates no command will be defined for this function.
     */
    const char* command;

    /**
     * A UTF-8 string holding a description for this function.  A null pointer will cause the description
     * "User defined function" to be used.
     */
    const char* description;

    /**
     * A UTF-8 string holding a category to assign this function under.  Category names are open-ended.  An empty
     * string or null pointer will place the function under the category "User Defined"
     */
    const char* category;

    /**
     * A flag indicating that the first parameter to this function should be an instance of Model::Rng.  Note that the
     * Model::Rng instance is supplied as the first parameter into the function and is hidden from the user in the
     * document view of the user's model or program.
     */
    bool requiresRNG;

    /**
     * A flag indicating if the first parameter should be subscripted by default.  Example functions with a subscripted
     * parameter would be \f$ log _ b x \f$.
     */
    bool subscriptedFirstParameter;

    /**
     * A value indicating the number of supported variants.  This value must be non-zero.
     */
    unsigned numberVariants;

    /**
     * Array of supported variants.
     */
    const UserFunctionVariant* variants;
};

#endif
