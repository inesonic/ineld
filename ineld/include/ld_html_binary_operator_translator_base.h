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
* This header defines the \ref Ld::HtmlBinaryOperatorTranslatorBase class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_HTML_BINARY_OPERATOR_TRANSLATOR_BASE_H
#define LD_HTML_BINARY_OPERATOR_TRANSLATOR_BASE_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_html_operator_translator_base.h"

namespace Ld {
    class HtmlCodeGenerationEngine;
    class OperatorFormat;

    /**
     * Base class for HTML translators used to render binary operators.
     */
    class LD_PUBLIC_API HtmlBinaryOperatorTranslatorBase:public HtmlOperatorTranslatorBase {
        Q_DECLARE_TR_FUNCTIONS(Ld::HtmlBinaryOperatorTranslatorBase)

        public:
            ~HtmlBinaryOperatorTranslatorBase() override;

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
             * Pure virtual method you should overload to provide the MathML operator character.
             *
             * \param[in] operatorFormat The format instance tied to this operator.
             *
             * \return returns the MathML operator character string.
             */
            virtual QString mathMlOperator(FormatPointer operatorFormat) const = 0;

            /**
             * Pure virtual method you should overload to provide the LaTeX operator command.
             *
             * \param[in] operatorFormat The format instance tied to this operator.
             *
             * \return returns the LaTeX operator string.
             */
            virtual QString latexOperator(FormatPointer operatorFormat) const = 0;
    };
};

#endif
