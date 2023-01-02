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
* This header defines the \ref Ld::LaTeXMatrixDataTypeTranslator class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_LATEX_MATRIX_DATA_TYPE_TRANSLATOR_H
#define LD_LATEX_MATRIX_DATA_TYPE_TRANSLATOR_H

#include <QString>

#include "ld_common.h"
#include "ld_format_structures.h"
#include "ld_matrix_data_type_decoder.h"
#include "ld_latex_data_type_translator.h"

namespace Model {
    class Matrix;
}

namespace Ld {
    /**
     * Base class for HTML matrix data type translators.
     */
    class LD_PUBLIC_API LaTeXMatrixDataTypeTranslator:public LaTeXDataTypeTranslator {
        public:
            /**
             * Pure virtual base class for contexts used by various LaTeX matrix translator.
             */
            class Context:public virtual MatrixDataTypeDecoder::Context {
                public:
                    /**
                     * Constructor
                     *
                     * \param[in] matrix The matrix to be translated to HTML.
                     *
                     * \param[in] format The format used to decode the matrix.
                     */
                    Context(const Model::Matrix& matrix, FormatPointer format);

                    virtual ~Context();

                    /**
                     * Method that generates MathML for a single coefficient.
                     *
                     * \param[in] row              The row containing the coefficient to be translated.
                     *
                     * \param[in] column           The column containing the coefficient to be translated.
                     *
                     * \param[in] element          A pointer to the element being translated.
                     *
                     * \param[in] generationEngine The generation engine driving the conversion.
                     *
                     * \return Returns true on success, returns false on error.
                     */
                    virtual bool toLaTeX(
                        unsigned long              row,
                        unsigned long              column,
                        ElementPointer             element,
                        LaTeXCodeGenerationEngine& generationEngine
                    ) = 0;
            };

            LaTeXMatrixDataTypeTranslator();

            ~LaTeXMatrixDataTypeTranslator() override;

            /**
             * Method that returns the name of the element this translator operates on.
             *
             * \return Returns the name of the element this translator operates on.  This version returns an empty
             *         string.
             */
            QString elementName() const override;

        protected:
            /**
             * Method that creates a LaTeX representation of the value.
             *
             * \param[in,out] context          The context used to drive the translation.
             *
             * \param[in]     element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            bool translateToLaTeX(
                Context&                   context,
                ElementPointer             element,
                LaTeXCodeGenerationEngine& generationEngine
            ) const;
    };
};

#endif
