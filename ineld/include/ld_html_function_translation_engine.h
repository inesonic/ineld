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
* This header defines the \ref Ld::HtmlFunctionTranslationEngine class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_HTML_FUNCTION_TRANSLATION_ENGINE_H
#define LD_HTML_FUNCTION_TRANSLATION_ENGINE_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_html_translator_helper.h"
#include "ld_html_operator_translator_helper.h"
#include "ld_function_translation_engine.h"

namespace Ld {
    class FunctionElement;
    class FunctionData;
    class HtmlCodeGenerationEngine;

    /**
     * Virtual base class for HTML function translators.  You can use this class as a common base class for HTML
     * function translators used to translate function calls into HTML.
     *
     * Derived classes are expected to be defined as static const instances so all methods are defined as static.
     */
    class LD_PUBLIC_API HtmlFunctionTranslationEngine:public FunctionTranslationEngine,
                                                      public HtmlTranslatorHelper,
                                                      public HtmlOperatorTranslatorHelper {
        Q_DECLARE_TR_FUNCTIONS(Ld::HtmlFunctionTranslationEngine)

        public:
            /**
             * Static translation engine instance.
             */
            static const HtmlFunctionTranslationEngine instance;

            HtmlFunctionTranslationEngine();

            ~HtmlFunctionTranslationEngine() override;

            /**
             * Method that is called to insert header content.  The default implementation does nothing.
             *
             * \param[in]     functionData     The function data defining the standard function.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.  The default implementaiton returns true.
             */
            virtual bool header(
                const FunctionData&             functionData,
                QSharedPointer<FunctionElement> element,
                HtmlCodeGenerationEngine&       generationEngine
            ) const;

            /**
             * Method that is called to insert body content.  The default implementation renders the function using
             * the same algorithm as user defined functions.
             *
             * \param[in]     functionData     The function data defining the standard function.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            virtual bool body(
                const FunctionData&             functionData,
                QSharedPointer<FunctionElement> element,
                HtmlCodeGenerationEngine&       generationEngine
            ) const;
    };
};

#endif
