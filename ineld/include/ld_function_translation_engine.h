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
* This header defines the \ref Ld::FunctionTranslationEngine class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_FUNCTION_TRANSLATION_ENGINE_H
#define LD_FUNCTION_TRANSLATION_ENGINE_H

#include "ld_common.h"

namespace Ld {
    /**
     * Virtual base class for all types of function translation engines.  This class exists to allow functions to
     * modify how they're translated by the various translation engines.
     */
    class LD_PUBLIC_API FunctionTranslationEngine {
        public:
            FunctionTranslationEngine();

            virtual ~FunctionTranslationEngine();
    };
};

#endif
