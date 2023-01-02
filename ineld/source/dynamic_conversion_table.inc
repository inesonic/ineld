/*-*-c++-*-*************************************************************************************************************
* Copyright 2016 Inesonic, LLC.
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
* This file provides a table indicating the language's ability to perform dynamic conversion.
***********************************************************************************************************************/

#include "ld_data_type.h"

static constexpr unsigned numberTypes = static_cast<unsigned>(Ld::DataType::ValueType::NUMBER_TYPES);

#define NEVER     (Ld::DataType::DynamicConversion::NEVER)
#define SOMETIMES (Ld::DataType::DynamicConversion::SOMETIMES)
#define ALWAYS    (Ld::DataType::DynamicConversion::ALWAYS)

//                                                            fromType     toType
static Ld::DataType::DynamicConversion dynamicConversionTable[numberTypes][numberTypes] = {
    // fromType  toType -> { NONE,      VARIANT, BOOLEAN,   INTEGER,   REAL,      COMPLEX,   SET        TUPLE      MATRIX_BOOLEAN, MATRIX_INTEGER, MATRIX_REAL, MATRIX_COMPLEX },
    /* NONE */             { NEVER,     NEVER,   NEVER,     NEVER,     NEVER,     NEVER,     NEVER,     NEVER,     NEVER,          NEVER,          NEVER,       NEVER          },
    /* VARIANT */          { NEVER,     ALWAYS,  SOMETIMES, SOMETIMES, SOMETIMES, SOMETIMES, SOMETIMES, SOMETIMES, SOMETIMES,      SOMETIMES,      SOMETIMES,   SOMETIMES      },
    /* BOOLEAN */          { NEVER,     ALWAYS,  ALWAYS,    ALWAYS,    ALWAYS,    ALWAYS,    NEVER,     NEVER,     NEVER,          NEVER,          NEVER,       NEVER          },
    /* INTEGER */          { NEVER,     ALWAYS,  ALWAYS,    ALWAYS,    ALWAYS,    ALWAYS,    NEVER,     NEVER,     NEVER,          NEVER,          NEVER,       NEVER          },
    /* REAL */             { NEVER,     ALWAYS,  ALWAYS,    SOMETIMES, ALWAYS,    ALWAYS,    NEVER,     NEVER,     NEVER,          NEVER,          NEVER,       NEVER          },
    /* COMPLEX */          { NEVER,     ALWAYS,  ALWAYS,    SOMETIMES, SOMETIMES, ALWAYS,    NEVER,     NEVER,     NEVER,          NEVER,          NEVER,       NEVER          },
    /* SET */              { NEVER,     ALWAYS,  NEVER,     NEVER,     NEVER,     NEVER,     ALWAYS,    NEVER,     NEVER,          NEVER,          NEVER,       NEVER          },
    /* TUPLE */            { NEVER,     ALWAYS,  NEVER,     NEVER,     NEVER,     NEVER,     NEVER,     ALWAYS,    NEVER,          NEVER,          NEVER,       NEVER          },
    /* MATRIX_BOOLEAN */   { NEVER,     ALWAYS,  NEVER,     NEVER,     NEVER,     NEVER,     NEVER,     NEVER,     ALWAYS,         ALWAYS,         ALWAYS,      ALWAYS         },
    /* MATRIX_INTEGER */   { NEVER,     ALWAYS,  NEVER,     NEVER,     NEVER,     NEVER,     NEVER,     NEVER,     ALWAYS,         ALWAYS,         ALWAYS,      ALWAYS         },
    /* MATRIX_REAL */      { NEVER,     ALWAYS,  NEVER,     NEVER,     NEVER,     NEVER,     NEVER,     NEVER,     ALWAYS,         SOMETIMES,      ALWAYS,      ALWAYS         },
    /* MATRIX_COMPLEX */   { NEVER,     ALWAYS,  NEVER,     NEVER,     NEVER,     NEVER,     NEVER,     NEVER,     ALWAYS,         SOMETIMES,      SOMETIMES,   ALWAYS         }
};

#undef NEVER
#undef SOMETIMES
#undef ALWAYS
