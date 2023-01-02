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
* This file provides a very minimalist example plug-in for test purposes.
***********************************************************************************************************************/

#include <iostream>
#include <cstdint>

#include <model_api_types.h>

#include <plug_in_data.h>

static const UserFunctionParameter functionParameters1[] = {
    { Model::ValueType::COMPLEX, "bender" },
    { Model::ValueType::INTEGER, "joey mousepad" }
};


static const UserFunctionVariant functionVariant1[] = {
    { Model::ValueType::TUPLE, sizeof(functionParameters1) / sizeof(UserFunctionParameter), functionParameters1 }
};


static const UserFunctionDefinition functionDefinitions[] = {
    {
        "donbot",
        "robot_mafia",
        "don",
        "bot",
        "donbot",
        "Dangerous if crossed",
        "Little Bitaly",
        true, // For gambling
        false,
        sizeof(functionVariant1) / sizeof(UserFunctionVariant),
        functionVariant1,
    }
};

extern "C" PLUG_IN_EXPORT PlugInData __plugInData;
PlugInData __plugInData = {
    "example_plug_in",
    "Francis X. Clampazzo",
    "Robot Mafia, Inc.",
    nullptr,
    "This is the brief description.",
    "This is the detailed description",
    "1.0",
    sizeof(functionDefinitions) / sizeof(UserFunctionDefinition),
    functionDefinitions,
    nullptr
};
