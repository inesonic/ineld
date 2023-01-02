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
* This header defines the \ref Ld::CppLnFunctionTranslationEngine class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CPP_LN_FUNCTION_TRANSLATION_ENGINE_H
#define LD_CPP_LN_FUNCTION_TRANSLATION_ENGINE_H

#include <QSharedPointer>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_function_translation_engine.h"
#include "ld_cpp_function_translation_engine.h"

namespace Ld {
    class FunctionElement;
    class FunctionData;
    class CppCodeGenerationEngine;

    /**
     * C++ natural log function translator.  You can use this class to translate the natural log function.  This
     * function will call several variants depending on the parameter in the natural log function.
     */
    class LD_PUBLIC_API CppLnFunctionTranslationEngine:public CppFunctionTranslationEngine {
        public:
            /**
             * Static translation engine instance.
             */
            static const CppLnFunctionTranslationEngine instance;

            CppLnFunctionTranslationEngine();

            ~CppLnFunctionTranslationEngine() override;

            /**
             * Method that is called prior to actually performing any translation to identify any and all required
             * headers and required static or dynamic libraries.  The default implementation simply returns.
             *
             * \param[in]     functionData     The function data defining the standard function.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.  The default implementation always returns
             *         true.
             */
            bool identifyDependenciesAndExplicitTypes(
                const FunctionData&             functionData,
                QSharedPointer<FunctionElement> element,
                CppCodeGenerationEngine&        generationEngine
            ) const override;

            /**
             * Method that is called during translation to insert the actual implementation.  The default
             * version will define the function using the internal name defined in the supplied \ref Ld::FunctionData
             * instance.
             *
             * This method is used for both the main thread implementation and the method implementation.
             *
             * \param[in]     functionData     The function data defining the standard function.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.  The default implementation always returns
             *         true.
             */
            bool implementation(
                const FunctionData&             functionData,
                QSharedPointer<FunctionElement> element,
                CppCodeGenerationEngine&        generationEngine
            ) const override;
    };
};

#endif
