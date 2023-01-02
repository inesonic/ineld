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
* This file implements the \ref Ld::LaTeXTableFrameElementTranslator class.
***********************************************************************************************************************/

#include <QString>
#include <QList>
#include <QBitArray>

#include <algorithm>

#include <util_bit_array.h>
#include <util_color_functions.h>

#include "ld_element.h"
#include "ld_element_structures.h"
#include "ld_format.h"
#include "ld_format_structures.h"
#include "ld_table_column_width.h"
#include "ld_table_frame_format.h"
#include "ld_table_frame_element.h"
#include "ld_text_export_context.h"
#include "ld_latex_code_generator_diagnostic.h"
#include "ld_latex_translation_phase.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_latex_translator.h"
#include "ld_latex_table_frame_element_translator.h"

namespace Ld {
    LaTeXTableFrameElementTranslator::~LaTeXTableFrameElementTranslator() {}


    QString LaTeXTableFrameElementTranslator::elementName() const {
        return TableFrameElement::elementName;
    }


    bool LaTeXTableFrameElementTranslator::identifyDependencies(ElementPointer, LaTeXCodeGenerationEngine& engine) {
        engine.addPackage("xtab");
        engine.addPackage("calc");
        engine.addPackage("ragged2e");
        engine.addPackage("colortbl");
        engine.addPackage("placeins");
        engine.addPackage("multirow");

        engine.addPreamble(
            "\\let\\inenl\\newlength\n"
            "\\let\\inesl\\setlength",
            LaTeXCodeGenerationEngine::minimumPackageRelativeOrdering - 1000
        );

        engine.addPreamble(
            "\\inenl{\\ineclinea}\n"
            "\\newcommand{\\inecline}[2]{\n"
            "  \\noalign{\\global\\inesl{\\ineclinea}{\\arrayrulewidth}}\n"
            "  \\noalign{\\global\\inesl{\\arrayrulewidth}{#1}}\\cline{#2}\n"
            "  \\noalign{\\global\\inesl{\\arrayrulewidth}{\\ineclinea}}\n"
            "}\n"
            "\\newcommand{\\inevline}[1]{\\vrule width #1}\n"
            "\\newcommand{\\inehline}[1]{\\noalign{\\hrule height #1}}",
            LaTeXCodeGenerationEngine::nominalNewCommandRelativeOrdering
        );

        return true;
    }


    bool LaTeXTableFrameElementTranslator::body(ElementPointer element, LaTeXCodeGenerationEngine& engine) {
        TextExportContext&                context       = engine.context();
        FormatPointer                     rawFormat     = element->format();
        QSharedPointer<TableFrameElement> table         = element.dynamicCast<TableFrameElement>();
        unsigned                          numberColumns = table->numberColumns();
        unsigned                          numberRows    = table->numberRows();

        QList<TableLineSettings>                      columnLineSettings;
        QList<TableLineSettings>                      rowLineSettings;
        QList<TableColumnWidth>                       columnWidths;
        QMap<TableFrameElement::CellPosition, QColor> cellColors;
        TableFrameFormat::Justification               tableJustification;
        float                                         totalFixedWidthPoints = 0;

        if (rawFormat->capabilities().contains(TableFrameFormat::formatName)) {
            QSharedPointer<TableFrameFormat> format = rawFormat.dynamicCast<TableFrameFormat>();

            tableJustification = format->justificationMode();

            for (unsigned columnLineIndex=0 ; columnLineIndex<=numberColumns ; ++columnLineIndex) {
                columnLineSettings.append(format->columnLineSetting(columnLineIndex));
            }

            for (unsigned rowLineIndex=0 ; rowLineIndex<=numberRows ; ++rowLineIndex) {
                rowLineSettings.append(format->rowLineSetting(rowLineIndex));
            }

            for (unsigned columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                TableColumnWidth columnWidth = format->columnWidth(columnIndex);
                columnWidths.append(columnWidth);

                if (columnWidth.widthType() == TableColumnWidth::WidthType::FIXED) {
                    totalFixedWidthPoints += columnWidth.width();
                }
            }

            Util::BitArray assignedGroups(table->numberGroups(), false);
            for (unsigned rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                for (unsigned columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                    unsigned groupIndex = table->groupAt(rowIndex, columnIndex);
                    if (assignedGroups.isClear(groupIndex)) {
                        assignedGroups.setBit(groupIndex);

                        QColor blendedColor = format->blendedColor(rowIndex, columnIndex);
                        if (blendedColor.isValid()) {
                            QColor cellColor = Util::paintersAlgorithm(QColor(Qt::white), blendedColor);
                            cellColors.insert(TableFrameElement::CellPosition(rowIndex, columnIndex), cellColor);
                        }
                    }
                }
            }
        } else {
            tableJustification = TableFrameFormat::Justification::CENTER;

            TableLineSettings defaultLineSetting(TableLineSettings::Style::SINGLE, 1.0F);
            for (unsigned columnLineIndex=0 ; columnLineIndex<=numberColumns ; ++columnLineIndex) {
                columnLineSettings.append(defaultLineSetting);
            }

            for (unsigned rowLineIndex=0 ; rowLineIndex<=numberRows ; ++rowLineIndex) {
                rowLineSettings.append(defaultLineSetting);
            }

            TableColumnWidth defaultColumnWidth(TableColumnWidth::WidthType::PROPORTIONAL, 1.0F / numberColumns);
            for (unsigned columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                columnWidths.append(defaultColumnWidth);
            }
        }

        if (context.columnNumber() > 0) {
            context << "\n\n";
        }

        context << "\\FloatBarrier\n";
        context << "\\begin{table}\n";
        switch(tableJustification) {
            case TableFrameFormat::Justification::JUSTIFY:
            case TableFrameFormat::Justification::LEFT: {
                context << "\\RaggedRight\n";
                break;
            }

            case TableFrameFormat::Justification::CENTER: {
                context << "\\Centering\n";
                break;
            }

            case TableFrameFormat::Justification::RIGHT: {
                context << "\\RaggedLeft\n";
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        QString  columnSpecification;
        for (unsigned columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
            // TODO: Vertical line width (define new command and add to the column specification)
            const TableLineSettings& leftLineSetting = columnLineSettings.at(columnIndex);
            const TableColumnWidth&  columnWidth     = columnWidths.at(columnIndex);

            columnSpecification += toColumnSpecification(leftLineSetting);
            columnSpecification += toColumnSpecification(columnWidth, totalFixedWidthPoints);
        }

        const TableLineSettings& rightLineSetting = columnLineSettings.last();
        columnSpecification += toColumnSpecification(rightLineSetting);

        const TableLineSettings& topLineSetting = rowLineSettings.first();
        context << QString("\\begin{xtabular}{%1}").arg(columnSpecification);
        context << toHLine(topLineSetting);
        context << "\\shrinkheight{\\baselineskip}\n";

        QList<Util::BitArray> rowLineLocations;
        Util::BitArray        emptyRowLineLocations(numberColumns, false);
        Util::BitArray        fullRowLineLocations(numberColumns, true);

        Util::BitArray processedGroups(table->numberGroups(), false);
        for (unsigned rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
            unsigned columnIndex = 0;

            while (columnIndex < numberColumns) {
                unsigned rightColumn = table->rightColumn(rowIndex, columnIndex);
                unsigned columnSpan  = rightColumn - columnIndex + 1;

                unsigned groupIndex  = table->groupAt(rowIndex, columnIndex);
                if (processedGroups.isClear(groupIndex)) {
                    processedGroups.setBit(groupIndex);

                    unsigned bottomRow   = table->bottomRow(rowIndex, columnIndex);
                    unsigned rowSpan     = bottomRow - rowIndex + 1;

                    if (rowSpan == 1) {
                        context << QString("\n% Cell at row %1, ").arg(rowIndex + 1);
                    } else {
                        context << QString("\n% Cell at rows %1 through %2, ")
                                   .arg(rowIndex + 1)
                                   .arg(rowIndex + rowSpan);
                    }

                    if (columnSpan == 1) {
                        context << QString("column %1\n").arg(columnIndex + 1);
                    } else {
                        context << QString("columns %1 through %2\n")
                                   .arg(columnIndex + 1)
                                   .arg(columnIndex + columnSpan);
                    }

                    unsigned nestedCommands = 0;

                    if (columnSpan > 1) {
                        const TableLineSettings& rightLineSettings   = columnLineSettings.at(columnIndex + columnSpan);
                        QString                  columnSpecification = QString("l%1")
                                                                       .arg(toColumnSpecification(rightLineSettings));

                        context << QString("\\multicolumn{%1}{%2}{").arg(columnSpan).arg(columnSpecification);
                        ++nestedCommands;
                    }

                    if (rowSpan > 1) {
                        context << QString("\\multirow{%1}{*}{").arg(rowSpan);
                        ++nestedCommands;
                    }

                    if (cellColors.contains(TableFrameElement::CellPosition(rowIndex, columnIndex))) {
                        QColor cellColor = cellColors.value(TableFrameElement::CellPosition(rowIndex, columnIndex));
                        context << QString("\\cellcolor[cmyk]{%1,%2,%3,%4} ")
                                   .arg(cellColor.cyanF())
                                   .arg(cellColor.magentaF())
                                   .arg(cellColor.yellowF())
                                   .arg(cellColor.blackF());
                    }

                    ElementPointerList children       = table->childrenInGroup(groupIndex);
                    unsigned long      numberChildren = static_cast<unsigned long>(children.size());

                    if (numberChildren > 1) {
                        context << "\\begin{tabular}{@{}l@{}}\n";
                    }

                    for (unsigned childIndex=0 ; childIndex<numberChildren ; ++childIndex) {
                        ElementPointer childElement = children.at(childIndex);

                        if (context.columnNumber() > 0) {
                            context << '\n';
                        }

                        if (childIndex > 0) {
                            context << "\\\\\n";
                        }

                        context.setNewlinesSuppressed();
                        engine.translateChild(childElement);
                        context.setNewlinesAllowed();
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

                    while (static_cast<unsigned>(rowLineLocations.size()) < rowSpan) {
                        rowLineLocations.append(emptyRowLineLocations);
                    }

                    Util::BitArray& currentRowLineLocations = rowLineLocations[rowSpan - 1];
                    currentRowLineLocations.setBits(columnIndex, rightColumn);

                    columnIndex += columnSpan;
                    if (columnIndex < numberColumns) {
                        if (context.columnNumber() > 0) {
                            context << '\n';
                        }

                        context << "&\n";
                    }
                } else {
                    columnIndex += columnSpan;
                }
            }

            if (context.columnNumber() > 0) {
                context << '\n';
            }

            const TableLineSettings& rowLineSetting          = rowLineSettings.at(rowIndex + 1);
            Util::BitArray           currentRowLineLocations = rowLineLocations.takeFirst();

            if (currentRowLineLocations == fullRowLineLocations) {
                context << QString("\\\\ %1\n").arg(toHLine(rowLineSetting));
            } else {
                context << "\\\\";
                Util::BitArray::Index startBitIndex = currentRowLineLocations.firstSetBit();
                while (startBitIndex != Util::BitArray::invalidIndex) {
                    Util::BitArray::Index endBitIndex      = currentRowLineLocations.firstClearedBit(startBitIndex + 1);
                    unsigned              startColumnIndex = static_cast<unsigned>(startBitIndex);
                    unsigned              endColumnIndex;

                    if (endBitIndex == Util::BitArray::invalidIndex) {
                        endColumnIndex = numberColumns - 1;
                        startBitIndex  = Util::BitArray::invalidIndex;
                    } else {
                        endColumnIndex = static_cast<unsigned>(endBitIndex - 1);
                        startBitIndex  = currentRowLineLocations.firstSetBit(endBitIndex + 1);
                    }

                    context << QString(" %1").arg(toCLine(rowLineSetting, startColumnIndex, endColumnIndex));
                }
                context << '\n';
            }
        }

        context << "\\end{xtabular}\n";
        context << "\\end{table}\n";
        context << "\\FloatBarrier\n\n";

        return true;
    }


    QString LaTeXTableFrameElementTranslator::toColumnSpecification(
            const TableColumnWidth& columnWidth,
            float                   totalFixedWidthPoints
        ) {
        QString result;

        switch (columnWidth.widthType()) {
            case TableColumnWidth::WidthType::FIXED: {
                result = QString("p{%1pt - 2 \\tabcolsep - \\arrayrulewidth}").arg(columnWidth.width());
                break;
            }

            case TableColumnWidth::WidthType::PROPORTIONAL: {
                result = QString("p{%1 \\textwidth  - 2 \\tabcolsep - \\arrayrulewidth - %2pt}")
                         .arg(columnWidth.width())
                         .arg(totalFixedWidthPoints);
                break;
            }

            case TableColumnWidth::WidthType::INVALID: {
                result= "p";
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    QString LaTeXTableFrameElementTranslator::toColumnSpecification(const TableLineSettings& lineSetting) {
        QString result;

        TableLineSettings::Style lineStyle = lineSetting.lineStyle();
        float                    width     = lineSetting.width();

        if (lineStyle == TableLineSettings::Style::SINGLE) {
            result = QString("!{\\inevline{%1pt}}").arg(width);
        } else if (lineStyle == TableLineSettings::Style::DOUBLE) {
            result = QString("!{\\inevline{%1pt}}").arg(3.0F * width);
        }

        return result;
    }


    QString LaTeXTableFrameElementTranslator::toHLine(const TableLineSettings& lineSetting) {
        QString result;

        TableLineSettings::Style lineStyle = lineSetting.lineStyle();
        float                    width     = lineSetting.width();

        if (lineStyle == TableLineSettings::Style::SINGLE) {
            result = QString("\\inehline{%1pt}").arg(width);
        } else if (lineStyle == TableLineSettings::Style::DOUBLE) {
            result = QString("\\inehline{%1pt}").arg(3.0F * width);
        }

        return result;
    }


    QString LaTeXTableFrameElementTranslator::toCLine(
            const TableLineSettings& lineSetting,
            unsigned                 startColumn,
            unsigned                 endColumn
        ) {
        QString result;

        TableLineSettings::Style lineStyle = lineSetting.lineStyle();
        float                    width     = lineSetting.width();

        if (lineStyle == TableLineSettings::Style::SINGLE) {
            result = QString("\\inecline{%1pt}{%2-%3}").arg(width).arg(startColumn).arg(endColumn);
        } else if (lineStyle == TableLineSettings::Style::DOUBLE) {
            result = QString("\\inecline{%1pt}{%2-%3}").arg(3.0F * width).arg(startColumn).arg(endColumn);
        }

        return result;
    }
}


