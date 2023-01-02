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
* This file implements the \ref Ld::LaTeXMatrixDataTypeTranslator class.
***********************************************************************************************************************/

#include <QString>

#include <model_matrix.h>

#include "ld_text_export_context.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_matrix_data_type_format.h"
#include "ld_latex_data_type_translator.h"
#include "ld_latex_matrix_data_type_translator.h"

/***********************************************************************************************************************
 * LaTeXMatrixDataTypeTranslator::Context:
 */

namespace Ld {
    LaTeXMatrixDataTypeTranslator::Context::Context(
            const Model::Matrix& /* matrix */,
            FormatPointer        /* format */
        ) {}


    LaTeXMatrixDataTypeTranslator::Context::~Context() {}
}

/***********************************************************************************************************************
 * LaTeXMatrixDataTypeTranslator:
 */

namespace Ld {
    LaTeXMatrixDataTypeTranslator::LaTeXMatrixDataTypeTranslator() {}


    LaTeXMatrixDataTypeTranslator::~LaTeXMatrixDataTypeTranslator() {}


    QString LaTeXMatrixDataTypeTranslator::elementName() const {
        return QString();
    }


    bool LaTeXMatrixDataTypeTranslator::translateToLaTeX(
            Context&                   context,
            ElementPointer             element,
            LaTeXCodeGenerationEngine& engine
        ) const {
        bool success = true;

        unsigned long numberRows            = context.numberRows();
        unsigned long numberColumns         = context.numberColumns();
        unsigned long numberLeadingRows     = context.numberLeadingRows();
        unsigned long numberTrailingRows    = context.numberTrailingRows();
        unsigned long firstTrailingRow      = context.firstTrailingRow();
        unsigned long numberLeadingColumns  = context.numberLeadingColumns();
        unsigned long numberTrailingColumns = context.numberTrailingColumns();
        unsigned long firstTrailingColumn   = context.firstTrailingColumn();

        unsigned long totalNumberColumns = numberLeadingColumns + numberTrailingColumns;
        if (numberTrailingRows > 0 || numberLeadingColumns < numberColumns) {
            ++totalNumberColumns;
        }

        TextExportContext& textContext = engine.context();

        textContext << "\\left [ \\begin{array}";
        textContext << QString("{%1}").arg(QString("c").repeated(totalNumberColumns));
        textContext << "\n";

        bool firstRow = true;
        for (unsigned long row=1 ; row<=numberLeadingRows ; ++row) {
            if (!firstRow) {
                textContext << "\\\\\n";
            } else {
                firstRow = false;
            }

            bool firstInRow = true;

            for (unsigned long column=1 ; column<=numberLeadingColumns ; ++column) {
                if (!firstInRow) {
                    textContext << " & ";
                } else {
                    firstInRow = false;
                }

                success = success && context.toLaTeX(row, column, element, engine);
            }

            if (numberTrailingColumns > 0 || numberLeadingColumns < numberColumns) {
                if (!firstInRow) {
                    textContext << " & ";
                } else {
                    firstInRow = false;
                }

                textContext << "\\cdots";

                for (unsigned long column=firstTrailingColumn ; column<=numberColumns ; ++column) {
                    if (!firstInRow) {
                        textContext << " & ";
                    } else {
                        firstInRow = false;
                    }

                    success = success && context.toLaTeX(row, column, element, engine);
                }
            }
        }

        if (numberTrailingRows > 0 || numberLeadingRows < numberRows) {
            if (!firstRow) {
                textContext << "\\\\\n";
            } else {
                firstRow = false;
            }

            for (unsigned long column=0 ; column<totalNumberColumns ; ++column) {
                if (column > 0) {
                    textContext << " & ";
                }

                textContext << "\\vdots";
            }

            for (unsigned long row=firstTrailingRow ; row<=numberRows ; ++row) {
                textContext << "\\\\";

                bool firstInRow = true;

                for (unsigned long column=1 ; column<=numberLeadingColumns ; ++column) {
                    if (!firstInRow) {
                        textContext << " & ";
                    } else {
                        firstInRow = false;
                    }

                    success = success && context.toLaTeX(row, column, element, engine);
                }

                if (numberTrailingColumns > 0 || numberLeadingColumns < numberColumns) {
                    if (!firstInRow) {
                        textContext << " & ";
                    } else {
                        firstInRow = false;
                    }

                    textContext << "\\cdots";

                    for (unsigned long column=firstTrailingColumn ; column<=numberColumns ; ++column) {
                        if (!firstInRow) {
                            textContext << " & ";
                        } else {
                            firstInRow = false;
                        }

                        success = success && context.toLaTeX(row, column, element, engine);
                    }
                }
            }
        }

        textContext << "\n\\end{array} \\right ]";

        return success;
    }
}
