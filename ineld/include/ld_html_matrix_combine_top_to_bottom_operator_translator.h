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
* This file defines the \ref Ld::HtmlMatrixCombineTopToBottomOperatorTranslator class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_HTML_MATRIX_COMBINE_TOP_TO_BOTTOM_OPERATOR_TRANSLATOR_H
#define LD_HTML_MATRIX_COMBINE_TOP_TO_BOTTOM_OPERATOR_TRANSLATOR_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>

#include "ld_common.h"
#include "ld_format_structures.h"
#include "ld_html_matrix_operator_translator_base.h"

namespace Ld {
    class HtmlCodeGenerationEngine;

    /**
     * HTML translator class for the \ref Ld::MatrixCombineTopToBottomOperatorElement class.
     */
    class LD_PUBLIC_API HtmlMatrixCombineTopToBottomOperatorTranslator:public HtmlMatrixOperatorTranslatorBase {
        Q_DECLARE_TR_FUNCTIONS(Ld::HtmlMatrixCombineTopToBottomOperatorTranslator)

        public:
            ~HtmlMatrixCombineTopToBottomOperatorTranslator() override;

            /**
             * Method that returns the name of the element this translator operates on.4
             *
             * \return Returns the name of the element this translator operates on.
             */
            QString elementName() const override;

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
    };
};

#endif
