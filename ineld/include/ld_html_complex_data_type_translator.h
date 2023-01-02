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
* This header defines the \ref Ld::HtmlComplexDataTypeTranslator class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_HTML_COMPLEX_DATA_TYPE_TRANSLATOR_H
#define LD_HTML_COMPLEX_DATA_TYPE_TRANSLATOR_H

#include <QString>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_html_code_generation_engine.h"
#include "ld_identifier_container.h"
#include "ld_html_data_type_translator.h"

namespace Ld {

    /**
     * HTML data type translator for complex values.
     */
    class LD_PUBLIC_API HtmlComplexDataTypeTranslator:public HtmlDataTypeTranslator {
        public:
            /**
             * Static instance.
             */
            static const HtmlComplexDataTypeTranslator instance;

            HtmlComplexDataTypeTranslator();

            ~HtmlComplexDataTypeTranslator() override;

            /**
             * Method that returns the name of the element this translator operates on.
             *
             * \return Returns the name of the element this translator operates on.
             */
            QString elementName() const override;

            /**
             * Overloaded method that creates a MathML representation of the value.
             *
             * \param[in]     variant          The variant to be translated.
             *
             * \param[in]     element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            bool toMathML(
                const Model::Variant&     variant,
                ElementPointer            element,
                HtmlCodeGenerationEngine& generationEngine
            ) const override;

            /**
             * Overloaded method that creates a LaTeX representation of the value.
             *
             * \param[in]     variant          The variant to be translated.
             *
             * \param[in]     element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            bool toLaTeX(
                const Model::Variant&     variant,
                ElementPointer            element,
                HtmlCodeGenerationEngine& generationEngine
            ) const final;

            /**
             * Overloaded method that creates a traditional HTML representation of the value.
             *
             * \param[in]     variant          The variant to be translated.
             *
             * \param[in]     element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            bool toHtml(
                const Model::Variant&     variant,
                ElementPointer            element,
                HtmlCodeGenerationEngine& generationEngine
            ) const override;
    };
};

#endif
