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
* This file implements the \ref Ld::LaTeXTableCellElementTranslator class.
***********************************************************************************************************************/

#include <QString>

#include "ld_table_cell_element.h"
#include "ld_table_cell_format.h"
#include "ld_text_export_context.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_latex_translator.h"
#include "ld_latex_table_cell_element_translator.h"

namespace Ld {
    LaTeXTableCellElementTranslator::~LaTeXTableCellElementTranslator() {}


    QString LaTeXTableCellElementTranslator::elementName() const {
        return TableCellElement::elementName;
    }


    bool LaTeXTableCellElementTranslator::body(ElementPointer element, LaTeXCodeGenerationEngine& engine) {
        QSharedPointer<TableCellElement> tableCellElement = element.dynamicCast<TableCellElement>();
        TextExportContext&               context          = engine.context();

        unsigned columnSpan = tableCellElement->columnSpan();
        unsigned rowSpan    = tableCellElement->rowSpan();

        QColor cellColor;
        FormatPointer tableCellFormat = tableCellElement->format();
        if (!tableCellFormat.isNull()                                             &&
            tableCellFormat->capabilities().contains(TableCellFormat::formatName)    ) {
            cellColor = tableCellFormat.dynamicCast<TableCellFormat>()->backgroundColor();
        }

        unsigned nestedCommands = 0;
        if (columnSpan > 1) {
            context << QString("\\multicolumn{%1}{|l|}{").arg(columnSpan);

            ++nestedCommands;
        }

        if (rowSpan > 1) {
            context << QString("\\multirow{%1}{*}{").arg(rowSpan);

            ++nestedCommands;
        }

        if (cellColor.isValid()) {
            context << QString("\\cellcolor[cmyk]{%1,%2,%3,%4} ")
                       .arg(cellColor.cyanF())
                       .arg(cellColor.magentaF())
                       .arg(cellColor.yellowF())
                       .arg(cellColor.blackF());
        }

        unsigned long numberChildren = tableCellElement->numberChildren();

        if (numberChildren > 1) {
            context << "\\begin{tabular}{@{}l@{}}\n";
        }

        for (unsigned long index=0 ; index<numberChildren ; ++index) {
            ElementPointer childElement = tableCellElement->child(index);

            if (context.columnNumber() > 0) {
                context << '\n';
            }

            if (index > 0) {
                context << "\\\\\n";
            }

            engine.translateChild(childElement);
        }

        if (context.columnNumber() > 0) {
            context << '\n';
        }

        if (numberChildren > 1) {
            context << "\\end{tabular}\n";
        }

        if (nestedCommands > 0) {
            context << QString("}").repeated(nestedCommands);
        }

        return true;
    }
}
