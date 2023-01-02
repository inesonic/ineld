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
* This header defines the \ref Ld::HtmlTrinaryOperatorTranslatorBase class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_HTML_TRINARY_OPERATOR_TRANSLATOR_BASE_H
#define LD_HTML_TRINARY_OPERATOR_TRANSLATOR_BASE_H

#include <QString>
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_html_operator_translator_base.h"

namespace Ld {
    class HtmlCodeGenerationEngine;

    /**
     * Base class for HTML translators used to render trinary operators.
     */
    class LD_PUBLIC_API HtmlTrinaryOperatorTranslatorBase:public HtmlOperatorTranslatorBase {
        public:
            ~HtmlTrinaryOperatorTranslatorBase() override;

            /**
             * Method that is called to insert body content.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            bool body(ElementPointer element, HtmlCodeGenerationEngine& generationEngine) override;

        protected:
            /**
             * Virtual method you can overload to build the trinary operator's HTML MathML content.  The method should
             * build the operator contents, less parenthesis.
             *
             * The default implementation does nothing.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.  The default implemetation always returns
             *         true.
             */
            virtual bool buildMathMlOperator(ElementPointer element, HtmlCodeGenerationEngine& generationEngine);

            /**
             * Virtual method you can overload to build the trinary operator's HTML LaTeX content.  The method should
             * build the operator contents, less parenthesis.
             *
             * The default implementation does nothing.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.  The default implemetation always returns
             *         true.
             */
            virtual bool buildLaTeXOperator(ElementPointer element, HtmlCodeGenerationEngine& generationEngine);
    };
};

#endif
