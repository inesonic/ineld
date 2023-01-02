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
* This file implements the \ref Ld::HtmlTableFrameElementTranslator class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>
#include <QList>
#include <QBitArray>

#include <util_color_functions.h>

#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_table_frame_element.h"
#include "ld_table_column_width.h"
#include "ld_table_frame_format.h"
#include "ld_table_line_settings.h"
#include "ld_xml_export_context.h"
#include "ld_code_generation_engine.h"
#include "ld_html_code_generator_diagnostic.h"
#include "ld_html_code_generation_engine.h"
#include "ld_html_translator.h"
#include "ld_html_table_frame_element_translator.h"

namespace Ld {
    HtmlTableFrameElementTranslator::~HtmlTableFrameElementTranslator() {}


    QString HtmlTableFrameElementTranslator::elementName() const {
        return Ld::TableFrameElement::elementName;
    }


    bool HtmlTableFrameElementTranslator::body(ElementPointer element, HtmlCodeGenerationEngine& engine) {
        XmlExportContext& context = engine.context();

        QSharedPointer<TableFrameElement> tableFrameElement  = element.dynamicCast<TableFrameElement>();
        QSharedPointer<TableFrameFormat>  tableFrameFormat   = element->format().dynamicCast<TableFrameFormat>();

        if (engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML5_WITH_CSS) {
            QString  className     = engine.cssClassForFormat(tableFrameFormat);
            unsigned numberColumns = tableFrameElement->numberColumns();
            unsigned numberRows    = tableFrameElement->numberRows();

            QList<TableColumnWidth> columnWidths;
            for (unsigned columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                columnWidths.append(tableFrameFormat->columnWidth(columnIndex));
            }

            context.writeStartElement("table");
            context.writeAttribute("class", className);

            // We use an empty header to fix column widths.
            context.writeStartElement("thead");
            context.writeStartElement("tr");
            for (unsigned columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                QString columnClassName;
                if (tableFrameFormat->columnWidthMapsToDefault(columnIndex)) {
                    columnClassName = QString("%1_default_column_width").arg(className);
                } else {
                    columnClassName = QString("%1_column_%2_width").arg(className).arg(columnIndex + 1);
                }
                context.writeStartElement("th");
                context.writeAttribute("class", columnClassName);
                context.writeEndElement(); // End th
            }
            context.writeEndElement(); // End tr
            context.writeEndElement(); // End thead

            QSet<unsigned> processedGroups;
            for (unsigned rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                context.writeStartElement("tr");

                for (unsigned columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                    unsigned groupIndex = tableFrameElement->groupAt(rowIndex, columnIndex);
                    if (!processedGroups.contains(groupIndex)) {
                        processedGroups.insert(groupIndex);

                        unsigned bottomRow   = tableFrameElement->bottomRow(rowIndex, columnIndex);
                        unsigned rightColumn = tableFrameElement->rightColumn(rowIndex, columnIndex);
                        unsigned rowSpan     = bottomRow - rowIndex + 1;
                        unsigned columnSpan  = rightColumn - columnIndex + 1;

                        context.writeStartElement("td");
                        if (rowSpan != 1) {
                            context.writeAttribute("rowspan", QString::number(rowSpan));
                        }

                        if (columnSpan != 1) {
                            context.writeAttribute("colspan", QString::number(columnSpan));
                        }

                        QString styles;
                        if (!tableFrameFormat->rowColorMapsToDefault(rowIndex)) {
                            if (!tableFrameFormat->columnColorMapsToDefault(columnIndex)) {
                                styles = QString("%1_cell_%2_%3_color")
                                         .arg(className)
                                         .arg(rowIndex + 1)
                                         .arg(columnIndex + 1);
                            } else {
                                if (!tableFrameFormat->cellColorMapsToDefault(rowIndex, columnIndex)) {
                                    styles = QString("%1_cell_%2_%3_color")
                                             .arg(className)
                                             .arg(rowIndex + 1)
                                             .arg(columnIndex + 1);
                                } else {
                                    styles = QString("%1_row_%2_color").arg(className).arg(rowIndex + 1);
                                }
                            }
                        } else {
                            if (!tableFrameFormat->columnColorMapsToDefault(columnIndex)) {
                                if (!tableFrameFormat->cellColorMapsToDefault(rowIndex, columnIndex)) {
                                    styles = QString("%1_cell_%2_%3_color")
                                             .arg(className)
                                             .arg(rowIndex + 1)
                                             .arg(columnIndex + 1);
                                } else {
                                    styles = QString("%1_column_%2_color").arg(className).arg(columnIndex + 1);
                                }
                            } else {
                                if (!tableFrameFormat->cellColorMapsToDefault(rowIndex, columnIndex)) {
                                    styles = QString("%1_cell_%2_%3_color")
                                             .arg(className)
                                             .arg(rowIndex + 1)
                                             .arg(columnIndex + 1);
                                }
                            }
                        }

                        unsigned bottomRowIndex = rowIndex + rowSpan - 1;
                        if (bottomRowIndex == 0) {
                            if (tableFrameFormat->rowLineSettingMapsToDefault(bottomRowIndex)) {
                                styles += QString(" %1_row_default_top_line_style").arg(className);
                            } else {
                                styles += QString(" %1_row_%2_top_line_style\n")
                                          .arg(className)
                                          .arg(bottomRowIndex + 1);
                            }
                        }

                        if (tableFrameFormat->rowLineSettingMapsToDefault(bottomRowIndex + 1)) {
                            styles += QString(" %1_row_default_bottom_line_style").arg(className);
                        } else {
                            styles += QString(" %1_row_%2_bottom_line_style").arg(className).arg(bottomRowIndex + 1);
                        }

                        unsigned rightColumnIndex = columnIndex + columnSpan - 1;
                        if (rightColumnIndex == 0) {
                            if (tableFrameFormat->columnLineSettingMapsToDefault(rightColumnIndex)) {
                                styles += QString(" %1_column_default_left_line_style").arg(className);
                            } else {
                                styles += QString(" %1_column_%2_top_left_style\n")
                                          .arg(className)
                                          .arg(rightColumnIndex + 1);
                            }
                        }

                        if (tableFrameFormat->columnLineSettingMapsToDefault(rightColumnIndex + 1)) {
                            styles += QString(" %1_column_default_right_line_style").arg(className);
                        } else {
                            styles += QString(" %1_column_%2_right_line_style")
                                      .arg(className)
                                      .arg(rightColumnIndex + 1);
                        }

                        context.writeAttribute("class", styles);

                        ElementPointerList children = tableFrameElement->childrenInGroup(groupIndex);
                        for (  ElementPointerList::const_iterator childIterator    = children.constBegin(),
                                                                  childEndIterator = children.constEnd()
                             ; childIterator != childEndIterator
                             ; ++childIterator
                            ) {
                            ElementPointer child = *childIterator;
                            engine.translateChild(child);
                        }

                        context.writeEndElement(); // End td
                    }
                }

                context.writeEndElement(); // End tr
            }

            context.writeEndElement(); // End table;
        } else if (engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML4_WITHOUT_CSS) {
            unsigned numberColumns = tableFrameElement->numberColumns();
            unsigned numberRows    = tableFrameElement->numberRows();

            QList<TableColumnWidth> columnWidths;
            for (unsigned columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                columnWidths.append(tableFrameFormat->columnWidth(columnIndex));
            }

            context.writeStartElement("table");
            context.writeAttribute("border", "1");
            context.writeAttribute("frame", "box");
            context.writeAttribute("rules", "all");

            QSet<unsigned> processedGroups;
            for (unsigned rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                context.writeStartElement("tr");

                for (unsigned columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                    unsigned groupIndex = tableFrameElement->groupAt(rowIndex, columnIndex);
                    if (!processedGroups.contains(groupIndex)) {
                        processedGroups.insert(groupIndex);

                        unsigned bottomRow   = tableFrameElement->bottomRow(rowIndex, columnIndex);
                        unsigned rightColumn = tableFrameElement->rightColumn(rowIndex, columnIndex);
                        unsigned rowSpan     = bottomRow - rowIndex + 1;
                        unsigned columnSpan  = rightColumn - columnIndex + 1;

                        context.writeStartElement("td");
                        if (rowSpan != 1) {
                            context.writeAttribute("rowspan", QString::number(rowSpan));
                        }

                        if (columnSpan != 1) {
                            context.writeAttribute("colspan", QString::number(columnSpan));
                        }

                        QColor cellColor = tableFrameFormat->blendedColor(rowIndex, columnIndex);
                        if (cellColor.isValid()) {
                            QColor htmlCellColor = Util::paintersAlgorithm(QColor(Qt::white), cellColor);
                            context.writeAttribute("bgcolor", htmlCellColor.name());
                        }

                        ElementPointerList children = tableFrameElement->childrenInGroup(groupIndex);
                        for (  ElementPointerList::const_iterator childIterator    = children.constBegin(),
                                                                  childEndIterator = children.constEnd()
                             ; childIterator != childEndIterator
                             ; ++childIterator
                            ) {
                            ElementPointer child = *childIterator;
                            engine.translateChild(child);
                        }

                        context.writeEndElement(); // End td
                    }
                }

                context.writeEndElement(); // End tr
            }

            context.writeEndElement(); // End table;
        }

        return true;
    }
}
