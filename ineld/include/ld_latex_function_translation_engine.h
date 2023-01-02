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
* This header defines the \ref Ld::LaTeXFunctionTranslationEngine class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_LATEX_FUNCTION_TRANSLATION_ENGINE_H
#define LD_LATEX_FUNCTION_TRANSLATION_ENGINE_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_latex_operator_translator_helper.h"
#include "ld_function_translation_engine.h"

namespace Ld {
    class FunctionData;
    class FunctionElement;
    class LaTeXCodeGenerationEngine;

    /**
     * Virtual base class for LaTeX function translation engines.  This class exists to allow functions to modify how
     * they're translated by the LaTeX translation engine.
     */
    class LD_PUBLIC_API LaTeXFunctionTranslationEngine:public FunctionTranslationEngine,
                                                       public LaTeXOperatorTranslatorHelper {
        Q_DECLARE_TR_FUNCTIONS(Ld::LaTeXFunctionTranslationEngine)

        public:
            /**
             * Static translation engine instance.
             */
            static const LaTeXFunctionTranslationEngine instance;

            LaTeXFunctionTranslationEngine();

            ~LaTeXFunctionTranslationEngine() override;

            /**
             * Method that is called during the \ref Ld::LaTeXTranslationPhase::Phase::IDENTIFY_DEPENDENCIES phase to
             * collect a list of packages needed by this element translator.  You can also use this phase to define
             * any special custom commands.
             *
             * The default implementation simply returns.
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
            virtual bool identifyDependencies(
                const FunctionData&             functionData,
                QSharedPointer<FunctionElement> element,
                LaTeXCodeGenerationEngine&      generationEngine
            ) const;

            /**
             * Method that is called to insert the LaTeX body content.
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
                LaTeXCodeGenerationEngine&      generationEngine
            ) const;
    };
};

#endif
