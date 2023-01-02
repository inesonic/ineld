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
* This file implements the \ref Ld::LaTeXMatrixOperatorTranslator class.
***********************************************************************************************************************/

#include <QString>

#include "ld_format_structures.h"
#include "ld_text_export_context.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_matrix_operator_element.h"
#include "ld_latex_operator_translator_base.h"
#include "ld_latex_matrix_operator_translator.h"

namespace Ld {
    LaTeXMatrixOperatorTranslator::~LaTeXMatrixOperatorTranslator() {}


    QString LaTeXMatrixOperatorTranslator::elementName() const {
        return MatrixOperatorElement::elementName;
    }


    bool LaTeXMatrixOperatorTranslator::body(ElementPointer element, LaTeXCodeGenerationEngine& engine) {
        TextExportContext& context = engine.context();

        QString leftParenthesis;
        QString rightParenthesis;
        bool    requiresParenthesis = includeParenthesis(element, leftParenthesis, rightParenthesis);

        if (requiresParenthesis) {
            context << tr(" %1 ").arg(leftParenthesis);
        }

        QSharedPointer<MatrixOperatorElement> matrixElement = element.dynamicCast<MatrixOperatorElement>();
        assert(!matrixElement.isNull());

        unsigned long numberRows    = matrixElement->numberRows();
        unsigned long numberColumns = matrixElement->numberColumns();
        unsigned long lastColumn    = numberColumns - 1;

        context << QString("\\begin{array}{%1}\n").arg(QString("c").repeated(numberColumns));

        for (unsigned long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
            if (rowIndex != 0) {
                context << " \\\\\n  ";
            } else {
                context << "  ";
            }

            for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                Ld::ElementPointer child = matrixElement->child(rowIndex, columnIndex);
                context << "{ ";
                engine.translateChild(child);

                if (columnIndex != lastColumn) {
                    context << " } & ";
                } else {
                    context << " }";
                }
            }
        }

        context << "\n\\end{array}";

        if (requiresParenthesis) {
            context << QString(" %1 ").arg(rightParenthesis);
        }

        return true;
    }
}
