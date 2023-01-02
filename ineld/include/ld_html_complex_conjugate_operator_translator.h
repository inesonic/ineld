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
* This header defines the \ref Ld::HtmlComplexConjugateOperatorTranslator class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_HTML_COMPLEX_CONJUGATE_OPERATOR_TRANSLATOR_H
#define LD_HTML_COMPLEX_CONJUGATE_OPERATOR_TRANSLATOR_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS
#include <QString>

#include "ld_common.h"
#include "ld_format_structures.h"
#include "ld_html_unary_superscript_operator_translator_base.h"

namespace Ld {
    class HtmlCodeGenerationEngine;
    class OperatorFormat;

    /**
     * HTML translator class for the \ref Ld::ComplexConjugateOperatorElement class.
     */
    class LD_PUBLIC_API HtmlComplexConjugateOperatorTranslator:public HtmlUnarySuperscriptOperatorTranslatorBase {
        Q_DECLARE_TR_FUNCTIONS(Ld::HtmlComplexConjugateOperatorTranslator)

        public:
            ~HtmlComplexConjugateOperatorTranslator() override;

            /**
             * Method that returns the name of the element this translator operates on.
             *
             * \return Returns the name of the element this translator operates on.
             */
            QString elementName() const override;

        protected:
            /**
             * Method that provides the MathML operator character.
             *
             * \param[in] operatorFormat The format instance tied to this operator.
             *
             * \return returns the MathML operator character string.
             */
            QString mathMlOperator(FormatPointer operatorFormat) const override;

            /**
             * Method that provides the LaTeX operator command.
             *
             * \param[in] operatorFormat The format instance tied to this operator.
             *
             * \return returns the LaTeX operator string.
             */
            QString latexOperator(FormatPointer operatorFormat) const override;
    };
};

#endif
