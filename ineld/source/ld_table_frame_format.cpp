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
* This file implements the \ref Ld::TableFrameFormat class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>
#include <QList>
#include <QSet>

#include <algorithm>

#include <util_units.h>
#include <util_color_functions.h>
#include <util_hash_functions.h>

#include "ld_format_structures.h"
#include "ld_table_column_width.h"
#include "ld_table_line_settings.h"
#include "ld_xml_attributes.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_program_file.h"
#include "ld_justified_block_format.h"
#include "ld_table_frame_format.h"

/***********************************************************************************************************************
 * Ld::TableFrameFormat::Aggregation
 */

namespace Ld {
    TableFrameFormat::Aggregation::Aggregation() {}


    TableFrameFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* TableFrameFormat::Aggregation::creator(const QString&) {
        return new TableFrameFormat::Aggregation;
    }


    QString TableFrameFormat::Aggregation::typeName() const {
        return TableFrameFormat::formatName;
    }


    void TableFrameFormat::Aggregation::clear() {
        JustifiedBlockFormat::Aggregation::clear();

        currentDefaultColumnWidths.clear();
        currentColumnWidthsByColumn.clear();
        currentColumnWidthsByGroup.clear();
        currentDefaultColumnLineSettings.clear();
        currentColumnLineSettingsByColumn.clear();
        currentColumnLineSettingsByGroup.clear();
        currentDefaultRowLineSettings.clear();
        currentRowLineSettingsByRow.clear();
        currentRowLineSettingsByGroup.clear();
        currentDefaultCellColors.clear();
        currentColumnColorsByColumn.clear();
        currentColumnColorsByGroup.clear();
        currentRowColorsByRow.clear();
        currentRowColorsByGroup.clear();
        currentLeftGutterValues.clear();
        currentRightGutterValues.clear();
        currenTopGutterValues.clear();
        currentBottomGutterValues.clear();
    }


    bool TableFrameFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<TableFrameFormat> tableFrameFormat = formatInstance.dynamicCast<TableFrameFormat>();
        if (!tableFrameFormat.isNull()) {
            success = JustifiedBlockFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentDefaultColumnWidths       << tableFrameFormat->currentDefaultColumnWidth;
                currentColumnWidthsByGroup       << tableFrameFormat->currentTableColumnWidths;
                currentDefaultColumnLineSettings << tableFrameFormat->currentDefaultColumnLineSetting;
                currentColumnLineSettingsByGroup << tableFrameFormat->currentColumnLineSettings;
                currentDefaultRowLineSettings    << tableFrameFormat->currentDefaultRowLineSetting;
                currentRowLineSettingsByGroup    << tableFrameFormat->currentRowLineSettings;
                currentDefaultCellColors         << tableFrameFormat->currentDefaultColor;
                currentColumnColorsByGroup       << tableFrameFormat->currentCellColorByColumn;
                currentRowColorsByGroup          << tableFrameFormat->currentCellColorByRow;
                currentLeftGutterValues          << tableFrameFormat->currentLeftGutter;
                currentRightGutterValues         << tableFrameFormat->currentRightGutter;
                currenTopGutterValues            << tableFrameFormat->currentTopGutter;
                currentBottomGutterValues        << tableFrameFormat->currentBottomGutter;
                currentCellColorsByGroup         << tableFrameFormat->currentCellColorsByPosition;

                for (  WidthsByColumn::const_iterator
                           widthIterator    = tableFrameFormat->currentTableColumnWidths.constBegin(),
                           widthEndIterator = tableFrameFormat->currentTableColumnWidths.constEnd()
                     ; widthIterator != widthEndIterator
                     ; ++widthIterator
                    ) {
                    unsigned                columnIndex = widthIterator.key();
                    const TableColumnWidth& width       = widthIterator.value();

                    currentColumnWidthsByColumn[columnIndex].insert(width);
                }

                for (  LineSettingsByIndex::const_iterator
                           lineSettingsIterator    = tableFrameFormat->currentColumnLineSettings.constBegin(),
                           lineSettingsEndIterator = tableFrameFormat->currentColumnLineSettings.constEnd()
                     ; lineSettingsIterator != lineSettingsEndIterator
                     ; ++lineSettingsIterator
                    ) {
                    unsigned                 lineIndex   = lineSettingsIterator.key();
                    const TableLineSettings& lineSetting = lineSettingsIterator.value();

                    currentColumnLineSettingsByColumn[lineIndex].insert(lineSetting);
                }

                for (  LineSettingsByIndex::const_iterator
                           lineSettingsIterator    = tableFrameFormat->currentRowLineSettings.constBegin(),
                           lineSettingsEndIterator = tableFrameFormat->currentRowLineSettings.constEnd()
                     ; lineSettingsIterator != lineSettingsEndIterator
                     ; ++lineSettingsIterator
                    ) {
                    unsigned                 lineIndex   = lineSettingsIterator.key();
                    const TableLineSettings& lineSetting = lineSettingsIterator.value();

                    currentRowLineSettingsByRow[lineIndex].insert(lineSetting);
                }

                for (  ColorByRowColumnIndex::const_iterator
                           colorByColumnIterator    = tableFrameFormat->currentCellColorByColumn.constBegin(),
                           colorByColumnEndIterator = tableFrameFormat->currentCellColorByColumn.constEnd()
                     ; colorByColumnIterator != colorByColumnEndIterator
                     ; ++colorByColumnIterator
                    ) {
                    unsigned      columnIndex = colorByColumnIterator.key();
                    const QColor& color       = colorByColumnIterator.value();

                    currentColumnColorsByColumn[columnIndex].insert(color);
                }

                for (  ColorByRowColumnIndex::const_iterator
                           colorByRowIterator    = tableFrameFormat->currentCellColorByRow.constBegin(),
                           colorByRowEndIterator = tableFrameFormat->currentCellColorByRow.constEnd()
                     ; colorByRowIterator != colorByRowEndIterator
                     ; ++colorByRowIterator
                    ) {
                    unsigned      rowIndex = colorByRowIterator.key();
                    const QColor& color    = colorByRowIterator.value();

                    currentRowColorsByRow[rowIndex].insert(color);
                }

                for (  ColorByPosition::const_iterator
                           cellColorIterator    = tableFrameFormat->currentCellColorsByPosition.constBegin(),
                           cellColorEndIterator = tableFrameFormat->currentCellColorsByPosition.constEnd()
                     ; cellColorIterator != cellColorEndIterator
                     ; ++cellColorIterator
                    ) {
                    const CellPosition& position = cellColorIterator.key();
                    const QColor&       color    = cellColorIterator.value();

                    currentCellColorsByPosition[position].insert(color);
                }
            }
        }

        return success;
    }


    QSet<TableColumnWidth> TableFrameFormat::Aggregation::defaultColumnWidths() const {
        return currentDefaultColumnWidths;
    }


    QMap<unsigned, QSet<TableColumnWidth>> TableFrameFormat::Aggregation::columnWidthsByColumn() const {
        return currentColumnWidthsByColumn;
    }


    QSet<TableFrameFormat::WidthsByColumn> TableFrameFormat::Aggregation::columnWidthsByGroup() const {
        return currentColumnWidthsByGroup;
    }


    QSet<TableLineSettings> TableFrameFormat::Aggregation::defaultColumnLineSettings() const {
        return currentDefaultColumnLineSettings;
    }


    QMap<unsigned, QSet<TableLineSettings>> TableFrameFormat::Aggregation::columnLineSettingsByIndex() const {
        return currentColumnLineSettingsByColumn;
    }


    QSet<TableFrameFormat::LineSettingsByIndex> TableFrameFormat::Aggregation::columnLineSettingsByGroup() const {
        return currentColumnLineSettingsByGroup;
    }


    QSet<TableLineSettings> TableFrameFormat::Aggregation::defaultRowLineSettings() const {
        return currentDefaultRowLineSettings;
    }


    QMap<unsigned, QSet<TableLineSettings>> TableFrameFormat::Aggregation::rowLineSettingsByIndex() const {
        return currentRowLineSettingsByRow;
    }


    QSet<TableFrameFormat::LineSettingsByIndex> TableFrameFormat::Aggregation::rowLineSettingsByGroup() const {
        return currentRowLineSettingsByGroup;
    }


    QSet<QColor> TableFrameFormat::Aggregation::defaultCellColors() const {
        return currentDefaultCellColors;
    }


    QMap<unsigned, QSet<QColor>> TableFrameFormat::Aggregation::columnColorsByIndex() const {
        return currentColumnColorsByColumn;
    }


    QSet<TableFrameFormat::ColorByRowColumnIndex> TableFrameFormat::Aggregation::columnColorsByGroup() const {
        return currentColumnColorsByGroup;
    }


    QMap<unsigned, QSet<QColor>> TableFrameFormat::Aggregation::rowColorsByIndex() const {
        return currentRowColorsByRow;
    }


    QSet<TableFrameFormat::ColorByRowColumnIndex> TableFrameFormat::Aggregation::rowColorsByGroup() const {
        return currentRowColorsByGroup;
    }


    QMap<TableFrameFormat::CellPosition, QSet<QColor>> TableFrameFormat::Aggregation::cellColorsByPosition() const {
        return currentCellColorsByPosition;
    }


    QSet<TableFrameFormat::ColorByPosition> TableFrameFormat::Aggregation::cellColorsByGroup() const {
        return currentCellColorsByGroup;
    }


    QSet<float> TableFrameFormat::Aggregation::leftGutterValues() const {
        return currentLeftGutterValues;
    }


    QSet<float> TableFrameFormat::Aggregation::rightGutterValues() const {
        return currentRightGutterValues;
    }


    QSet<float> TableFrameFormat::Aggregation::topGutterValues() const {
        return currenTopGutterValues;
    }


    QSet<float> TableFrameFormat::Aggregation::bottomGutterValues() const {
        return currentBottomGutterValues;
    }
}

/***********************************************************************************************************************
 * Ld::TableFrameFormat
 */

namespace Ld {
    const QString                         TableFrameFormat::formatName("TableFrameFormat");
    const float                           TableFrameFormat::defaultLeftIndentation = 0;
    const float                           TableFrameFormat::defaultRightIndentation = 0;
    const float                           TableFrameFormat::defaultTopSpacing = 6;
    const float                           TableFrameFormat::defaultBottomSpacing = 6;

    const TableFrameFormat::Justification TableFrameFormat::defaultJustification =
        TableFrameFormat::Justification::CENTER;

    const TableColumnWidth                TableFrameFormat::initialDefaultColumnWidth(
        TableColumnWidth::WidthType::PROPORTIONAL,
        0.125
    );

    const TableLineSettings               TableFrameFormat::defaultLineSetting(TableLineSettings::Style::SINGLE, 1.0F);

    const float TableFrameFormat::defaultLeftGutter   = static_cast<float>(0.04 * Util::Units::pointsPerInch);
    const float TableFrameFormat::defaultRightGutter  = static_cast<float>(0.04 * Util::Units::pointsPerInch);
    const float TableFrameFormat::defaultTopGutter    = static_cast<float>(0.04 * Util::Units::pointsPerInch);
    const float TableFrameFormat::defaultBottomGutter = static_cast<float>(0.04 * Util::Units::pointsPerInch);

    TableFrameFormat::TableFrameFormat() {
        setSupportedJustificationModes(
            Justification::LEFT,
            Justification::RIGHT,
            Justification::CENTER
        );

        setJustification(defaultJustification);
        setLeftIndentation(defaultLeftIndentation);
        setRightIndentation(defaultRightIndentation);
        setTopSpacing(defaultTopSpacing);
        setBottomSpacing(defaultBottomSpacing);

        currentDefaultColumnWidth       = initialDefaultColumnWidth;
        currentDefaultColumnLineSetting = defaultLineSetting;
        currentDefaultRowLineSetting    = defaultLineSetting;
        currentDefaultColor             = QColor();
        currentLeftGutter               = defaultLeftGutter;
        currentRightGutter              = defaultRightGutter;
        currentTopGutter                = defaultTopGutter;
        currentBottomGutter             = defaultBottomGutter;
    }


    TableFrameFormat::TableFrameFormat(
            const TableFrameFormat& other
        ):Format(
            other
        ),BlockFormat(
            other
        ),JustifiedBlockFormat(
            other
        ) {
        currentDefaultColumnWidth       = other.currentDefaultColumnWidth;
        currentTableColumnWidths        = other.currentTableColumnWidths;
        currentDefaultColumnLineSetting = other.currentDefaultColumnLineSetting;
        currentColumnLineSettings       = other.currentColumnLineSettings;
        currentDefaultRowLineSetting    = other.currentDefaultRowLineSetting;
        currentRowLineSettings          = other.currentRowLineSettings;
        currentDefaultColor             = other.currentDefaultColor;
        currentCellColorsByPosition     = other.currentCellColorsByPosition;
        currentCellColorByRow           = other.currentCellColorByRow;
        currentCellColorByColumn        = other.currentCellColorByColumn;
        currentLeftGutter               = other.currentLeftGutter;
        currentRightGutter              = other.currentRightGutter;
        currentTopGutter                = other.currentTopGutter;
        currentBottomGutter             = other.currentBottomGutter;
    }


    TableFrameFormat::~TableFrameFormat() {}


    FormatPointer TableFrameFormat::clone() const {
        return FormatPointer(new TableFrameFormat(*this));
    }


    Format* TableFrameFormat::creator(const QString&) {
        return new TableFrameFormat();
    }


    QString TableFrameFormat::typeName() const {
        return formatName;
    }


    bool TableFrameFormat::isValid() const {
        bool valid = JustifiedBlockFormat::isValid();
        return valid;
    }


    Format::Capabilities TableFrameFormat::capabilities() const {
        return JustifiedBlockFormat::capabilities() << formatName;
    }


    TableColumnWidth TableFrameFormat::columnWidth(unsigned columnIndex) const {
        return currentTableColumnWidths.value(columnIndex, currentDefaultColumnWidth);
    }


    void TableFrameFormat::setColumnWidth(unsigned columnIndex, const TableColumnWidth& newColumnWidth) {
        if (newColumnWidth == currentDefaultColumnWidth) {
            setColumnToDefaultWidth(columnIndex);
        } else {
            currentTableColumnWidths.insert(columnIndex, newColumnWidth);
            reportFormatUpdated();
        }
    }


    bool TableFrameFormat::columnWidthMapsToDefault(unsigned columnIndex) const {
        return !currentTableColumnWidths.contains(columnIndex);
    }


    void TableFrameFormat::setColumnToDefaultWidth(unsigned columnIndex) {
        if (currentTableColumnWidths.contains(columnIndex)) {
            currentTableColumnWidths.remove(columnIndex);
            reportFormatUpdated();
        }
    }


    const TableColumnWidth& TableFrameFormat::defaultColumnWidth() const {
        return currentDefaultColumnWidth;
    }


    void TableFrameFormat::setDefaultColumnWidth(const TableColumnWidth& newColumnWidth) {
        if (currentDefaultColumnWidth != newColumnWidth) {
            currentDefaultColumnWidth = newColumnWidth;
            reportFormatUpdated();
        }
    }


    TableLineSettings TableFrameFormat::columnLineSetting(unsigned lineIndex) const {
        return currentColumnLineSettings.value(lineIndex, currentDefaultColumnLineSetting);
    }


    void TableFrameFormat::setColumnLineSetting(unsigned lineIndex, const TableLineSettings& lineSetting) {
        if (lineSetting == currentDefaultColumnLineSetting) {
            setColumnLineSettingToDefault(lineIndex);
        } else {
            currentColumnLineSettings.insert(lineIndex, lineSetting);
            reportFormatUpdated();
        }
    }


    bool TableFrameFormat::columnLineSettingMapsToDefault(unsigned lineIndex) const {
        return !currentColumnLineSettings.contains(lineIndex);
    }


    void TableFrameFormat::setColumnLineSettingToDefault(unsigned lineIndex) {
        if (currentColumnLineSettings.contains(lineIndex)) {
            currentColumnLineSettings.remove(lineIndex);
            reportFormatUpdated();
        }
    }


    const TableLineSettings& TableFrameFormat::defaultColumnLineSetting() const {
        return currentDefaultColumnLineSetting;
    }


    void TableFrameFormat::setDefaultColumnLineSetting(const TableLineSettings& lineSetting) {
        currentDefaultColumnLineSetting = lineSetting;
        reportFormatUpdated();
    }


    TableLineSettings TableFrameFormat::rowLineSetting(unsigned lineIndex) const {
        return currentRowLineSettings.value(lineIndex, currentDefaultRowLineSetting);
    }


    void TableFrameFormat::setRowLineSetting(unsigned lineIndex, const TableLineSettings& lineSetting) {
        if (lineSetting == currentDefaultRowLineSetting) {
            setRowLineSettingToDefault(lineIndex);
        } else {
            currentRowLineSettings.insert(lineIndex, lineSetting);
            reportFormatUpdated();
        }
    }


    bool TableFrameFormat::rowLineSettingMapsToDefault(unsigned lineIndex) const {
        return !currentRowLineSettings.contains(lineIndex);
    }


    void TableFrameFormat::setRowLineSettingToDefault(unsigned lineIndex) {
        if (currentRowLineSettings.contains(lineIndex)) {
            currentRowLineSettings.remove(lineIndex);
            reportFormatUpdated();
        }
    }


    const TableLineSettings& TableFrameFormat::defaultRowLineSetting() const {
        return currentDefaultRowLineSetting;
    }


    void TableFrameFormat::setDefaultRowLineSetting(const TableLineSettings& lineSetting) {
        currentDefaultRowLineSetting = lineSetting;
        reportFormatUpdated();
    }


    QColor TableFrameFormat::blendedColor(unsigned rowIndex, unsigned columnIndex) const {
        const QColor& cellColor   = currentCellColorsByPosition.value(CellPosition(rowIndex, columnIndex));
        const QColor& rowColor    = currentCellColorByRow.value(rowIndex);
        const QColor& columnColor = currentCellColorByColumn.value(columnIndex);

        return Util::blend(currentDefaultColor, cellColor, rowColor, columnColor);
    }


    QColor TableFrameFormat::cellColor(unsigned rowIndex, unsigned columnIndex) const {
        return currentCellColorsByPosition.value(CellPosition(rowIndex, columnIndex));
    }


    void TableFrameFormat::setCellColor(unsigned rowIndex, unsigned columnIndex, const QColor& cellColor) {
        if (!cellColor.isValid()) {
            setCellColorToDefault(rowIndex, columnIndex);
        } else {
            currentCellColorsByPosition.insert(CellPosition(rowIndex, columnIndex), cellColor);
            reportFormatUpdated();
        }
    }


    bool TableFrameFormat::cellColorMapsToDefault(unsigned rowIndex, unsigned columnIndex) const {
        return !currentCellColorsByPosition.contains(CellPosition(rowIndex, columnIndex));
    }


    void TableFrameFormat::setCellColorToDefault(unsigned rowIndex, unsigned columnIndex) {
        CellPosition position(rowIndex, columnIndex);
        if (currentCellColorsByPosition.contains(position)) {
            currentCellColorsByPosition.remove(position);
            reportFormatUpdated();
        }
    }


    QColor TableFrameFormat::columnColor(unsigned columnIndex) const {
        return currentCellColorByColumn.value(columnIndex);
    }


    void TableFrameFormat::setColumnColor(unsigned columnIndex, const QColor& columnColor) {
        if (!columnColor.isValid()) {
            setColumnColorToDefault(columnIndex);
        } else {
            currentCellColorByColumn.insert(columnIndex, columnColor);
            reportFormatUpdated();
        }
    }


    bool TableFrameFormat::columnColorMapsToDefault(unsigned columnIndex) const {
        return !currentCellColorByColumn.contains(columnIndex);
    }


    void TableFrameFormat::setColumnColorToDefault(unsigned columnIndex) {
        if (currentCellColorByColumn.contains(columnIndex)) {
            currentCellColorByColumn.remove(columnIndex);
            reportFormatUpdated();
        }
    }


    QColor TableFrameFormat::rowColor(unsigned rowIndex) const {
        return currentCellColorByRow.value(rowIndex);
    }


    void TableFrameFormat::setRowColor(unsigned rowIndex, const QColor& rowColor) {
        if (!rowColor.isValid()) {
            setRowColorToDefault(rowIndex);
        } else {
            currentCellColorByRow.insert(rowIndex, rowColor);
            reportFormatUpdated();
        }
    }


    bool TableFrameFormat::rowColorMapsToDefault(unsigned rowIndex) const {
        return !currentCellColorByRow.contains(rowIndex);
    }


    void TableFrameFormat::setRowColorToDefault(unsigned rowIndex) {
        if (currentCellColorByRow.contains(rowIndex)) {
            currentCellColorByRow.remove(rowIndex);
            reportFormatUpdated();
        }
    }


    QColor TableFrameFormat::defaultColor() const {
        return currentDefaultColor;
    }


    void TableFrameFormat::setDefaultColor(const QColor& defaultColor) {
        currentDefaultColor = defaultColor;
        reportFormatUpdated();
    }


    float TableFrameFormat::leftGutter() const {
        return currentLeftGutter;
    }


    void TableFrameFormat::setLeftGutter(float newGutterValue) {
        if (newGutterValue != currentLeftGutter) {
            currentLeftGutter = newGutterValue;
            reportFormatUpdated();
        }
    }


    float TableFrameFormat::rightGutter() const {
        return currentRightGutter;
    }


    void TableFrameFormat::setRightGutter(float newGutterValue) {
        if (newGutterValue != currentRightGutter) {
            currentRightGutter = newGutterValue;
            reportFormatUpdated();
        }
    }


    float TableFrameFormat::topGutter() const {
        return currentTopGutter;
    }


    void TableFrameFormat::setTopGutter(float newGutterValue) {
        if (newGutterValue != currentTopGutter) {
            currentTopGutter = newGutterValue;
            reportFormatUpdated();
        }
    }


    float TableFrameFormat::bottomGutter() const {
        return currentBottomGutter;
    }


    void TableFrameFormat::setBottomGutter(float newGutterValue) {
        if (newGutterValue != currentBottomGutter) {
            currentBottomGutter = newGutterValue;
            reportFormatUpdated();
        }
    }


    void TableFrameFormat::columnRemoved(unsigned columnNumber) {
        WidthsByColumn        newColumnWidths;
        LineSettingsByIndex   newColumnLineSettings;
        ColorByRowColumnIndex newCellColorsByColumn;
        ColorByPosition       newCellColorsByPosition;

        for (  WidthsByColumn::const_iterator widthIterator    = currentTableColumnWidths.constBegin(),
                                              widthEndIterator = currentTableColumnWidths.constEnd()
             ; widthIterator != widthEndIterator
             ; ++widthIterator
            ) {
            if (widthIterator.key() < columnNumber) {
                newColumnWidths.insert(widthIterator.key(), widthIterator.value());
            } else if (widthIterator.key() > columnNumber) {
                newColumnWidths.insert(widthIterator.key() - 1, widthIterator.value());
            }
        }

        for (  LineSettingsByIndex::const_iterator lineSettingsIterator    = currentColumnLineSettings.constBegin(),
                                                   lineSettingsEndIterator = currentColumnLineSettings.constEnd()
             ; lineSettingsIterator != lineSettingsEndIterator
             ; ++lineSettingsIterator
            ) {
            if (lineSettingsIterator.key() < columnNumber + 1) {
                newColumnLineSettings.insert(lineSettingsIterator.key(), lineSettingsIterator.value());
            } else if (lineSettingsIterator.key() > columnNumber + 1) {
                newColumnLineSettings.insert(lineSettingsIterator.key() - 1, lineSettingsIterator.value());
            }
        }

        for (  ColorByRowColumnIndex::const_iterator colorByColumnIterator    = currentCellColorByColumn.constBegin(),
                                                     colorByColumnEndIterator = currentCellColorByColumn.constEnd()
             ; colorByColumnIterator != colorByColumnEndIterator
             ; ++colorByColumnIterator
            ) {
            if (colorByColumnIterator.key() < columnNumber) {
                newCellColorsByColumn.insert(colorByColumnIterator.key(), colorByColumnIterator.value());
            } else if (colorByColumnIterator.key() > columnNumber) {
                newCellColorsByColumn.insert(colorByColumnIterator.key() - 1, colorByColumnIterator.value());
            }
        }

        for (  ColorByPosition::const_iterator cellColorIterator    = currentCellColorsByPosition.constBegin(),
                                               cellColorEndIterator = currentCellColorsByPosition.constEnd()
             ; cellColorIterator != cellColorEndIterator
             ; ++cellColorIterator
            ) {
            const CellPosition& position = cellColorIterator.key();

            if (position.columnIndex() < columnNumber) {
                newCellColorsByPosition.insert(position, cellColorIterator.value());
            } else if (position.columnIndex() > columnNumber) {
                CellPosition newPosition(position.rowIndex(), position.columnIndex() - 1);
                newCellColorsByPosition.insert(newPosition, cellColorIterator.value());
            }
        }

        currentTableColumnWidths    = newColumnWidths;
        currentColumnLineSettings   = newColumnLineSettings;
        currentCellColorByColumn    = newCellColorsByColumn;
        currentCellColorsByPosition = newCellColorsByPosition;
    }


    void TableFrameFormat::rowRemoved(unsigned rowNumber) {
        LineSettingsByIndex   newRowLineSettings;
        ColorByRowColumnIndex newCellColorsByRow;
        ColorByPosition       newCellColorsByPosition;

        for (  LineSettingsByIndex::const_iterator lineSettingsIterator    = currentRowLineSettings.constBegin(),
                                                   lineSettingsEndIterator = currentRowLineSettings.constEnd()
             ; lineSettingsIterator != lineSettingsEndIterator
             ; ++lineSettingsIterator
            ) {
            if (lineSettingsIterator.key() < rowNumber + 1) {
                newRowLineSettings.insert(lineSettingsIterator.key(), lineSettingsIterator.value());
            } else if (lineSettingsIterator.key() > rowNumber + 1) {
                newRowLineSettings.insert(lineSettingsIterator.key() - 1, lineSettingsIterator.value());
            }
        }

        for (  ColorByRowColumnIndex::const_iterator colorByRowIterator    = currentCellColorByRow.constBegin(),
                                                     colorByRowEndIterator = currentCellColorByRow.constEnd()
             ; colorByRowIterator != colorByRowEndIterator
             ; ++colorByRowIterator
            ) {
            if (colorByRowIterator.key() < rowNumber) {
                newCellColorsByRow.insert(colorByRowIterator.key(), colorByRowIterator.value());
            } else if (colorByRowIterator.key() > rowNumber) {
                newCellColorsByRow.insert(colorByRowIterator.key() - 1, colorByRowIterator.value());
            }
        }

        for (  ColorByPosition::const_iterator cellColorIterator    = currentCellColorsByPosition.constBegin(),
                                               cellColorEndIterator = currentCellColorsByPosition.constEnd()
             ; cellColorIterator != cellColorEndIterator
             ; ++cellColorIterator
            ) {
            const CellPosition& position = cellColorIterator.key();

            if (position.rowIndex() < rowNumber) {
                newCellColorsByPosition.insert(position, cellColorIterator.value());
            } else if (position.rowIndex() > rowNumber) {
                CellPosition newPosition(position.rowIndex() - 1, position.columnIndex());
                newCellColorsByPosition.insert(newPosition, cellColorIterator.value());
            }
        }

        currentRowLineSettings      = newRowLineSettings;
        currentCellColorByRow       = newCellColorsByRow;
        currentCellColorsByPosition = newCellColorsByPosition;
    }


    void TableFrameFormat::columnsInserted(unsigned columnNumber, unsigned numberColumns) {
        WidthsByColumn        newColumnWidths;
        LineSettingsByIndex   newColumnLineSettings;
        ColorByRowColumnIndex newCellColorsByColumn;
        ColorByPosition       newCellColorsByPosition;

        for (  WidthsByColumn::const_iterator widthIterator    = currentTableColumnWidths.constBegin(),
                                              widthEndIterator = currentTableColumnWidths.constEnd()
             ; widthIterator != widthEndIterator
             ; ++widthIterator
            ) {
            if (widthIterator.key() < columnNumber) {
                newColumnWidths.insert(widthIterator.key(), widthIterator.value());
            } else {
                newColumnWidths.insert(widthIterator.key() + numberColumns, widthIterator.value());
            }
        }

        for (  LineSettingsByIndex::const_iterator lineSettingsIterator    = currentColumnLineSettings.constBegin(),
                                                   lineSettingsEndIterator = currentColumnLineSettings.constEnd()
             ; lineSettingsIterator != lineSettingsEndIterator
             ; ++lineSettingsIterator
            ) {
            if (lineSettingsIterator.key() <= columnNumber + 1) {
                newColumnLineSettings.insert(lineSettingsIterator.key(), lineSettingsIterator.value());
            } else {
                newColumnLineSettings.insert(lineSettingsIterator.key() + numberColumns, lineSettingsIterator.value());
            }
        }

        for (  ColorByRowColumnIndex::const_iterator colorByColumnIterator    = currentCellColorByColumn.constBegin(),
                                                     colorByColumnEndIterator = currentCellColorByColumn.constEnd()
             ; colorByColumnIterator != colorByColumnEndIterator
             ; ++colorByColumnIterator
            ) {
            if (colorByColumnIterator.key() < columnNumber) {
                newCellColorsByColumn.insert(colorByColumnIterator.key(), colorByColumnIterator.value());
            } else {
                newCellColorsByColumn.insert(
                    colorByColumnIterator.key() + numberColumns,
                    colorByColumnIterator.value()
                );
            }
        }

        for (  ColorByPosition::const_iterator cellColorIterator    = currentCellColorsByPosition.constBegin(),
                                               cellColorEndIterator = currentCellColorsByPosition.constEnd()
             ; cellColorIterator != cellColorEndIterator
             ; ++cellColorIterator
            ) {
            const CellPosition& position = cellColorIterator.key();

            if (position.columnIndex() < columnNumber) {
                newCellColorsByPosition.insert(position, cellColorIterator.value());
            } else {
                CellPosition newPosition(position.rowIndex(), position.columnIndex() + numberColumns);
                newCellColorsByPosition.insert(newPosition, cellColorIterator.value());
            }
        }

        currentTableColumnWidths    = newColumnWidths;
        currentColumnLineSettings   = newColumnLineSettings;
        currentCellColorByColumn    = newCellColorsByColumn;
        currentCellColorsByPosition = newCellColorsByPosition;
    }


    void TableFrameFormat::rowsInserted(unsigned rowNumber, unsigned numberRows) {
        LineSettingsByIndex   newRowLineSettings;
        ColorByRowColumnIndex newCellColorsByRow;
        ColorByPosition       newCellColorsByPosition;

        for (  LineSettingsByIndex::const_iterator lineSettingsIterator    = currentRowLineSettings.constBegin(),
                                                   lineSettingsEndIterator = currentRowLineSettings.constEnd()
             ; lineSettingsIterator != lineSettingsEndIterator
             ; ++lineSettingsIterator
            ) {
            if (lineSettingsIterator.key() <= rowNumber + 1) {
                newRowLineSettings.insert(lineSettingsIterator.key(), lineSettingsIterator.value());
            } else {
                newRowLineSettings.insert(lineSettingsIterator.key() + numberRows, lineSettingsIterator.value());
            }
        }

        for (  ColorByRowColumnIndex::const_iterator colorByRowIterator    = currentCellColorByRow.constBegin(),
                                                     colorByRowEndIterator = currentCellColorByRow.constEnd()
             ; colorByRowIterator != colorByRowEndIterator
             ; ++colorByRowIterator
            ) {
            if (colorByRowIterator.key() < rowNumber) {
                newCellColorsByRow.insert(colorByRowIterator.key(), colorByRowIterator.value());
            } else {
                newCellColorsByRow.insert(colorByRowIterator.key() + numberRows, colorByRowIterator.value());
            }
        }

        for (  ColorByPosition::const_iterator cellColorIterator    = currentCellColorsByPosition.constBegin(),
                                               cellColorEndIterator = currentCellColorsByPosition.constEnd()
             ; cellColorIterator != cellColorEndIterator
             ; ++cellColorIterator
            ) {
            const CellPosition& position = cellColorIterator.key();

            if (position.rowIndex() < rowNumber) {
                newCellColorsByPosition.insert(position, cellColorIterator.value());
            } else {
                CellPosition newPosition(position.rowIndex() + numberRows, position.columnIndex());
                newCellColorsByPosition.insert(newPosition, cellColorIterator.value());
            }
        }

        currentRowLineSettings      = newRowLineSettings;
        currentCellColorByRow       = newCellColorsByRow;
        currentCellColorsByPosition = newCellColorsByPosition;
    }


    void TableFrameFormat::mergeCells(
            unsigned rowIndex,
            unsigned columnIndex,
            unsigned numberCellsToMergeRight,
            unsigned numberCellsToMergeDown
        ) {
        unsigned topRowIndex      = rowIndex;
        unsigned bottomRowIndex   = rowIndex + numberCellsToMergeDown;
        unsigned leftColumnIndex  = columnIndex;
        unsigned rightColumnIndex = columnIndex + numberCellsToMergeRight;

        ColorByPosition::iterator it = currentCellColorsByPosition.begin();
        while (it != currentCellColorsByPosition.end()) {
            const CellPosition& position    = it.key();
            unsigned            rowIndex    = position.rowIndex();
            unsigned            columnIndex = position.columnIndex();

            if (rowIndex >= topRowIndex              &&
                rowIndex <= bottomRowIndex           &&
                columnIndex >= leftColumnIndex       &&
                columnIndex <= rightColumnIndex      &&
                (rowIndex != topRowIndex        ||
                 columnIndex != leftColumnIndex    )    ) {
                it = currentCellColorsByPosition.erase(it);
            } else {
                ++it;
            }
        }
    }


    QString TableFrameFormat::toString() const {
        QString formatString = JustifiedBlockFormat::toString();

        formatString += QString(",%1").arg(toString(currentDefaultColumnWidth));
        formatString += QString(",%1").arg(toString(currentDefaultColumnLineSetting));
        formatString += QString(",%1").arg(toString(currentDefaultRowLineSetting));
        formatString += QString(",%1").arg(currentDefaultColor.name(QColor::NameFormat::HexArgb));
        formatString += QString(",%1").arg(currentLeftGutter);
        formatString += QString(",%1").arg(currentRightGutter);
        formatString += QString(",%1").arg(currentTopGutter);
        formatString += QString(",%1").arg(currentBottomGutter);

        for (  WidthsByColumn::const_iterator widthsIterator    = currentTableColumnWidths.constBegin(),
                                              widthsEndIterator = currentTableColumnWidths.constEnd()
             ; widthsIterator != widthsEndIterator
             ; ++widthsIterator
            ) {
            formatString += QString(",W(%1,%2)").arg(widthsIterator.key()).arg(toString(widthsIterator.value()));
        }

        for (  LineSettingsByIndex::const_iterator columnLineIterator    = currentColumnLineSettings.constBegin(),
                                                   columnLineEndIterator = currentColumnLineSettings.constEnd()
             ; columnLineIterator != columnLineEndIterator
             ; ++columnLineIterator
            ) {
            formatString += QString(",CL(%1,%2)")
                            .arg(columnLineIterator.key())
                            .arg(toString(columnLineIterator.value()));
        }

        for (  LineSettingsByIndex::const_iterator rowLineIterator    = currentRowLineSettings.constBegin(),
                                                   rowLineEndIterator = currentRowLineSettings.constEnd()
             ; rowLineIterator != rowLineEndIterator
             ; ++rowLineIterator
            ) {
            formatString += QString(",RL(%1,%2)").arg(rowLineIterator.key()).arg(toString(rowLineIterator.value()));
        }

        for (  ColorByPosition::const_iterator cellColorIterator    = currentCellColorsByPosition.constBegin(),
                                               cellColorEndIterator = currentCellColorsByPosition.constEnd()
             ; cellColorIterator != cellColorEndIterator
             ; ++cellColorIterator
            ) {
            const CellPosition& position = cellColorIterator.key();
            formatString += QString(",cC(%1,%2,%3)")
                            .arg(position.rowIndex())
                            .arg(position.columnIndex())
                            .arg(cellColorIterator.value().name(QColor::NameFormat::HexArgb));
        }

        for (  ColorByRowColumnIndex::const_iterator columnColorIterator    = currentCellColorByColumn.constBegin(),
                                                     columnColorEndIterator = currentCellColorByColumn.constEnd()
             ; columnColorIterator != columnColorEndIterator
             ; ++columnColorIterator
            ) {
            formatString += QString(",CC(%1,%2)")
                            .arg(columnColorIterator.key())
                            .arg(columnColorIterator.value().name(QColor::NameFormat::HexArgb));
        }

        for (  ColorByRowColumnIndex::const_iterator rowColorIterator    = currentCellColorByRow.constBegin(),
                                                     rowColorEndIterator = currentCellColorByRow.constEnd()
             ; rowColorIterator != rowColorEndIterator
             ; ++rowColorIterator
            ) {
            formatString += QString(",RC(%1,%2)")
                            .arg(rowColorIterator.key())
                            .arg(rowColorIterator.value().name(QColor::NameFormat::HexArgb));
        }

        return formatString;
    }


    QString TableFrameFormat::toCss() const {
        QString result = "border-collapse: collapse; table-layout: fixed; ";

        switch (justificationMode()) {
            case Justification::LEFT: {
                result += QString("margin-left: %1pt; margin-right: auto; ").arg(leftIndentation());
                break;
            }

            case Justification::RIGHT: {
                result += QString("margin-left: auto: margin-right: %1pt; ").arg(rightIndentation());
                break;
            }

            case Justification::CENTER:  {
                result += "margin-left: auto; margin-right: auto; ";
                break;
            }

            case Justification::JUSTIFY: {
                result += QString("margin-left: %1pt; margin-right: %1pt; ")
                          .arg(leftIndentation())
                          .arg(rightIndentation());
                break;
            }

            default: {
                result += QString("margin-left: %1pt; margin-right: auto; ").arg(leftIndentation());
                break;
            }
        }

        if (currentDefaultColor.isValid()) {
            int red;
            int green;
            int blue;
            int alpha;
            currentDefaultColor.getRgb(&red, &green, &blue, &alpha);

            result += QString("background-color: rgba(%1, %2, %3, %4); ")
                      .arg(red)
                      .arg(green)
                      .arg(blue)
                      .arg(alpha);
        }

        return result;
    }


    QString TableFrameFormat::toCssEntry(const QString& className, const QString& tag, const QString& id) const {
        QString result;

        if (!className.isEmpty()) {
            result = JustifiedBlockFormat::toCssEntry(className, tag, id);

            if (!id.isEmpty()) {
                result += QString("%1.%2_default_column_width#%3 { %4}\n")
                          .arg(tag)
                          .arg(className)
                          .arg(id)
                          .arg(currentDefaultColumnWidth.toCss());
            } else {
                result += QString("%1.%2_default_column_width { %3}\n")
                          .arg(tag)
                          .arg(className)
                          .arg(currentDefaultColumnWidth.toCss());
            }

            for (  WidthsByColumn::const_iterator widthsIterator = currentTableColumnWidths.constBegin(),
                                                  widthsEndIterator = currentTableColumnWidths.constEnd()
                 ; widthsIterator != widthsEndIterator
                 ; ++widthsIterator
                ) {
                unsigned                columnIndex = widthsIterator.key();
                const TableColumnWidth& width       = widthsIterator.value();

                if (!id.isEmpty()) {
                    result += QString("%1.%2_column_%3_width#%4 { %5}\n")
                              .arg(tag)
                              .arg(className)
                              .arg(columnIndex + 1)
                              .arg(id)
                              .arg(width.toCss());
                } else {
                    result += QString("%1.%2_column_%3_width { %4}\n")
                              .arg(tag)
                              .arg(className)
                              .arg(columnIndex + 1)
                              .arg(width.toCss());
                }
            }

            QColor white(Qt::white);

            for (  ColorByRowColumnIndex::const_iterator colorRowIterator    = currentCellColorByRow.constBegin(),
                                                         colorRowEndIterator = currentCellColorByRow.constEnd()
                 ; colorRowIterator != colorRowEndIterator
                 ; ++colorRowIterator
                ) {
                unsigned rowIndex = colorRowIterator.key();
                QColor   rowColor = Util::paintersAlgorithm(
                    white,
                    Util::blend(currentDefaultColor, colorRowIterator.value())
                );

                if (!id.isEmpty()) {
                    result += QString("%1.%2_row_%3_color#%4 { background-color: %5; }\n")
                              .arg(tag)
                              .arg(className)
                              .arg(rowIndex + 1)
                              .arg(id)
                              .arg(Util::toRgba(rowColor));
                } else {
                    result += QString("%1.%2_row_%3_color { background-color: %4; }\n")
                              .arg(tag)
                              .arg(className)
                              .arg(rowIndex + 1)
                              .arg(Util::toRgba(rowColor));
                }
            }

            for (  ColorByRowColumnIndex::const_iterator colorColumnIterator    = currentCellColorByColumn.constBegin(),
                                                         colorColumnEndIterator = currentCellColorByColumn.constEnd()
                 ; colorColumnIterator != colorColumnEndIterator
                 ; ++colorColumnIterator
                ) {
                unsigned columnIndex = colorColumnIterator.key();
                QColor   columnColor = Util::paintersAlgorithm(
                    white,
                    Util::blend(currentDefaultColor, colorColumnIterator.value())
                );

                if (!id.isEmpty()) {
                    result += QString("%1.%2_column_%3_color#%4 { background-color: %5; }\n")
                              .arg(tag)
                              .arg(className)
                              .arg(columnIndex + 1)
                              .arg(id)
                              .arg(Util::toRgba(columnColor));
                } else {
                    result += QString("%1.%2_column_%3_color { background-color: %4; }\n")
                              .arg(tag)
                              .arg(className)
                              .arg(columnIndex + 1)
                              .arg(Util::toRgba(columnColor));
                }
            }

            for (  ColorByRowColumnIndex::const_iterator colorRowIterator    = currentCellColorByRow.constBegin(),
                                                         colorRowEndIterator = currentCellColorByRow.constEnd()
                 ; colorRowIterator != colorRowEndIterator
                 ; ++colorRowIterator
                ) {
                unsigned rowIndex = colorRowIterator.key();
                QColor   rowColor = Util::blend(currentDefaultColor, colorRowIterator.value());

                for (  ColorByRowColumnIndex::const_iterator
                           colorColumnIterator    = currentCellColorByColumn.constBegin(),
                           colorColumnEndIterator = currentCellColorByColumn.constEnd()
                     ; colorColumnIterator != colorColumnEndIterator
                     ; ++colorColumnIterator
                ) {
                    unsigned columnIndex = colorColumnIterator.key();
                    QColor   columnColor = Util::blend(currentDefaultColor, colorColumnIterator.value());

                    if (!currentCellColorsByPosition.contains(CellPosition(rowIndex, columnIndex))) {
                        QColor cellColor = Util::paintersAlgorithm(
                            white,
                            Util::blend(currentDefaultColor, rowColor, columnColor)
                        );

                        if (!id.isEmpty()) {
                            result += QString("%1.%2_cell_%3_%4_color#%5 { background-color: %6; }\n")
                                      .arg(tag)
                                      .arg(className)
                                      .arg(rowIndex + 1)
                                      .arg(columnIndex + 1)
                                      .arg(id)
                                      .arg(Util::toRgba(cellColor));
                        } else {
                            result += QString("%1.%2_cell_%3_%4_color { background-color: %5; }\n")
                                      .arg(tag)
                                      .arg(className)
                                      .arg(rowIndex + 1)
                                      .arg(columnIndex + 1)
                                      .arg(Util::toRgba(cellColor));
                        }
                    }
                }
            }

            for (  ColorByPosition::const_iterator cellColorIterator    = currentCellColorsByPosition.constBegin(),
                                                   cellColorEndIterator = currentCellColorsByPosition.constEnd()
                 ; cellColorIterator != cellColorEndIterator
                 ; ++cellColorIterator
                ) {
                const CellPosition& position  = cellColorIterator.key();
                QColor              cellColor = Util::paintersAlgorithm(
                    white,
                    blendedColor(position.rowIndex(), position.columnIndex())
                );

                if (!id.isEmpty()) {
                    result += QString("%1.%2_cell_%3_%4_color#%5 { background-color: %6; }\n")
                              .arg(tag)
                              .arg(className)
                              .arg(position.rowIndex() + 1)
                              .arg(position.columnIndex() + 1)
                              .arg(id)
                              .arg(Util::toRgba(cellColor));
                } else {
                    result += QString("%1.%2_cell_%3_%4_color { background-color: %5; }\n")
                              .arg(tag)
                              .arg(className)
                              .arg(position.rowIndex() + 1)
                              .arg(position.columnIndex() + 1)
                              .arg(Util::toRgba(cellColor));
                }
            }

            if (!id.isEmpty()) {
                result += QString("%1.%2_column_default_left_line_style#%3 { %4}\n")
                          .arg(tag)
                          .arg(className)
                          .arg(id)
                          .arg(currentDefaultColumnLineSetting.toCss(TableLineSettings::Position::LEFT_POSITION));
                result += QString("%1.%2_column_default_right_line_style#%3 { %4}\n")
                          .arg(tag)
                          .arg(className)
                          .arg(id)
                          .arg(currentDefaultColumnLineSetting.toCss(TableLineSettings::Position::RIGHT_POSITION));
            } else {
                result += QString("%1.%2_column_default_left_line_style { %3}\n")
                          .arg(tag)
                          .arg(className)
                          .arg(currentDefaultColumnLineSetting.toCss(TableLineSettings::Position::LEFT_POSITION));
                result += QString("%1.%2_column_default_right_line_style { %3}\n")
                          .arg(tag)
                          .arg(className)
                          .arg(currentDefaultColumnLineSetting.toCss(TableLineSettings::Position::RIGHT_POSITION));
            }

            for (  LineSettingsByIndex::const_iterator columnLineIterator    = currentColumnLineSettings.constBegin(),
                                                       columnLineEndIterator = currentColumnLineSettings.constEnd()
                 ; columnLineIterator != columnLineEndIterator
                 ; ++columnLineIterator
                ) {
                unsigned                 columnLineIndex = columnLineIterator.key();
                const TableLineSettings& lineSetting     = columnLineIterator.value();

                if (!id.isEmpty()) {
                    if (columnLineIndex == 0) {
                        result += QString("%1.%2_column_%3_left_line_style#%4 { %5}\n")
                                  .arg(tag)
                                  .arg(className)
                                  .arg(columnLineIndex + 1)
                                  .arg(id)
                                  .arg(lineSetting.toCss(TableLineSettings::Position::LEFT_POSITION));
                    } else {
                        result += QString("%1.%2_column_%3_right_line_style#%4 { %5}\n")
                                  .arg(tag)
                                  .arg(className)
                                  .arg(columnLineIndex)
                                  .arg(id)
                                  .arg(lineSetting.toCss(TableLineSettings::Position::RIGHT_POSITION));
                    }
                } else {
                    if (columnLineIndex == 0) {
                        result += QString("%1.%2_column_%3_left_line_style { %5}\n")
                                  .arg(tag)
                                  .arg(className)
                                  .arg(columnLineIndex + 1)
                                  .arg(lineSetting.toCss(TableLineSettings::Position::LEFT_POSITION));
                    } else {
                        result += QString("%1.%2_column_%3_right_line_style { %5}\n")
                                  .arg(tag)
                                  .arg(className)
                                  .arg(columnLineIndex)
                                  .arg(lineSetting.toCss(TableLineSettings::Position::RIGHT_POSITION));
                    }
                }
            }

            if (!id.isEmpty()) {
                result += QString("%1.%2_row_default_top_line_style#%3 { %4}\n")
                          .arg(tag)
                          .arg(className)
                          .arg(id)
                          .arg(currentDefaultRowLineSetting.toCss(TableLineSettings::Position::TOP_POSITION));
                result += QString("%1.%2_row_default_bottom_line_style#%3 { %4}\n")
                          .arg(tag)
                          .arg(className)
                          .arg(id)
                          .arg(currentDefaultRowLineSetting.toCss(TableLineSettings::Position::BOTTOM_POSITION));
            } else {
                result += QString("%1.%2_row_default_top_line_style { %3}\n")
                          .arg(tag)
                          .arg(className)
                          .arg(currentDefaultRowLineSetting.toCss(TableLineSettings::Position::TOP_POSITION));
                result += QString("%1.%2_row_default_bottom_line_style { %3}\n")
                          .arg(tag)
                          .arg(className)
                          .arg(currentDefaultRowLineSetting.toCss(TableLineSettings::Position::BOTTOM_POSITION));
            }

            for (  LineSettingsByIndex::const_iterator rowLineIterator    = currentRowLineSettings.constBegin(),
                                                       rowLineEndIterator = currentRowLineSettings.constEnd()
                 ; rowLineIterator != rowLineEndIterator
                 ; ++rowLineIterator
                ) {
                unsigned                 rowLineIndex = rowLineIterator.key();
                const TableLineSettings& lineSetting  = rowLineIterator.value();

                if (!id.isEmpty()) {
                    if (rowLineIndex == 0) {
                        result += QString("%1.%2_row_%3_top_line_style#%4 { %5}\n")
                                  .arg(tag)
                                  .arg(className)
                                  .arg(rowLineIndex + 1)
                                  .arg(id)
                                  .arg(lineSetting.toCss(TableLineSettings::Position::TOP_POSITION));
                    } else {
                        result += QString("%1.%2_row_%3_bottom_line_style#%4 { %5}\n")
                                  .arg(tag)
                                  .arg(className)
                                  .arg(rowLineIndex)
                                  .arg(id)
                                  .arg(lineSetting.toCss(TableLineSettings::Position::BOTTOM_POSITION));
                    }
                } else {
                    if (rowLineIndex == 0) {
                        result += QString("%1.%2_row_%3_top_line_style { %5}\n")
                                  .arg(tag)
                                  .arg(className)
                                  .arg(rowLineIndex + 1)
                                  .arg(lineSetting.toCss(TableLineSettings::Position::TOP_POSITION));
                    } else {
                        result += QString("%1.%2_row_%3_bottom_line_style { %5}\n")
                                  .arg(tag)
                                  .arg(className)
                                  .arg(rowLineIndex)
                                  .arg(lineSetting.toCss(TableLineSettings::Position::BOTTOM_POSITION));
                    }
                }
            }
        }

        return result;
    }


    void TableFrameFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (currentDefaultColumnWidth.widthType() != initialDefaultColumnWidth.widthType()) {
            attributes.append("column_width_style", toString(currentDefaultColumnWidth.widthType()));
            attributes.append("column_width", currentDefaultColumnWidth.width());
        } else if (currentDefaultColumnWidth.width() != initialDefaultColumnWidth.width()) {
            attributes.append("column_width", currentDefaultColumnWidth.width());
        }

        if (currentDefaultColumnLineSetting.lineStyle() != defaultLineSetting.lineStyle()) {
            attributes.append("column_line_style", toString(currentDefaultColumnLineSetting.lineStyle()));
            if (currentDefaultColumnLineSetting.lineStyle() == Ld::TableLineSettings::Style::SINGLE ||
                currentDefaultColumnLineSetting.lineStyle() == Ld::TableLineSettings::Style::DOUBLE    ) {
                attributes.append("column_line_width", currentDefaultColumnLineSetting.width());
            }
        } else if (currentDefaultColumnLineSetting.width() != defaultLineSetting.width()                     &&
                   (currentDefaultColumnLineSetting.lineStyle() == Ld::TableLineSettings::Style::SINGLE ||
                    currentDefaultColumnLineSetting.lineStyle() == Ld::TableLineSettings::Style::DOUBLE    )    ) {
            attributes.append("column_line_width", currentDefaultColumnLineSetting.width());
        }

        if (currentDefaultRowLineSetting.lineStyle() != defaultLineSetting.lineStyle()) {
            attributes.append("row_line_style", toString(currentDefaultRowLineSetting.lineStyle()));
            if (currentDefaultRowLineSetting.lineStyle() == Ld::TableLineSettings::Style::SINGLE ||
                currentDefaultRowLineSetting.lineStyle() == Ld::TableLineSettings::Style::DOUBLE    ) {
                attributes.append("row_line_width", currentDefaultRowLineSetting.width());
            }
        } else if (currentDefaultRowLineSetting.width() != defaultLineSetting.width()                     &&
                   (currentDefaultRowLineSetting.lineStyle() == Ld::TableLineSettings::Style::SINGLE ||
                    currentDefaultRowLineSetting.lineStyle() == Ld::TableLineSettings::Style::DOUBLE    )    ) {
            attributes.append("row_line_width", currentDefaultRowLineSetting.width());
        }

        if (currentDefaultColor.isValid()) {
            attributes.append("cell_color", currentDefaultColor);
        }

        if (currentLeftGutter != defaultLeftGutter) {
            attributes.append("left_gutter", currentLeftGutter);
        }

        if (currentRightGutter != defaultRightGutter) {
            attributes.append("right_gutter", currentRightGutter);
        }

        if (currentTopGutter != defaultTopGutter) {
            attributes.append("top_gutter", currentTopGutter);
        }

        if (currentBottomGutter != defaultBottomGutter) {
            attributes.append("bottom_gutter", currentBottomGutter);
        }
    }


    void TableFrameFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        BlockFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        JustifiedBlockFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        TableFrameFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void TableFrameFormat::writeChildren(
            QSharedPointer<XmlWriter>       writer,
            QSharedPointer<FormatOrganizer>,
            ProgramFile&,
            const XmlAttributes&
        ) const {
        for (  WidthsByColumn::const_iterator widthIterator    = currentTableColumnWidths.constBegin(),
                                              widthEndIterator = currentTableColumnWidths.constEnd()
             ; widthIterator != widthEndIterator
             ; ++widthIterator
            ) {
            unsigned                columnIndex = widthIterator.key();
            const TableColumnWidth& width       = widthIterator.value();

            XmlAttributes attributes;
            attributes.append("index", columnIndex);
            if (width.widthType() != currentDefaultColumnWidth.widthType()) {
                attributes.append("style", toString(width.widthType()));
                attributes.append("width", width.width());
            } else {
                attributes.append("width", width.width());
            }

            writer->writeStartElement("ColumnWidth");
            writer->writeAttributes(attributes);
            writer->writeEndElement();
        }

        for (  LineSettingsByIndex::const_iterator lineSettingsIterator    = currentColumnLineSettings.constBegin(),
                                                   lineSettingsEndIterator = currentColumnLineSettings.constEnd()
             ; lineSettingsIterator != lineSettingsEndIterator
             ; ++lineSettingsIterator
            ) {
            unsigned          lineIndex   = lineSettingsIterator.key();
            TableLineSettings lineSetting = lineSettingsIterator.value();

            XmlAttributes attributes;
            attributes.append("index", lineIndex);
            if (lineSetting.lineStyle() != currentDefaultColumnLineSetting.lineStyle()) {
                attributes.append("style", toString(lineSetting.lineStyle()));
                if (lineSetting.lineStyle() == Ld::TableLineSettings::Style::SINGLE ||
                    lineSetting.lineStyle() == Ld::TableLineSettings::Style::DOUBLE    ) {
                    attributes.append("width", lineSetting.width());
                }
            } else {
                if (lineSetting.lineStyle() == Ld::TableLineSettings::Style::SINGLE ||
                    lineSetting.lineStyle() == Ld::TableLineSettings::Style::DOUBLE    ) {
                    attributes.append("width", lineSetting.width());
                }
            }

            writer->writeStartElement("VerticalLine");
            writer->writeAttributes(attributes);
            writer->writeEndElement();
        }

        for (  LineSettingsByIndex::const_iterator lineSettingsIterator    = currentRowLineSettings.constBegin(),
                                                   lineSettingsEndIterator = currentRowLineSettings.constEnd()
             ; lineSettingsIterator != lineSettingsEndIterator
             ; ++lineSettingsIterator
            ) {
            unsigned          lineIndex   = lineSettingsIterator.key();
            TableLineSettings lineSetting = lineSettingsIterator.value();

            XmlAttributes attributes;
            attributes.append("index", lineIndex);
            if (lineSetting.lineStyle() != currentDefaultRowLineSetting.lineStyle()) {
                attributes.append("style", toString(lineSetting.lineStyle()));
                if (lineSetting.lineStyle() == Ld::TableLineSettings::Style::SINGLE ||
                    lineSetting.lineStyle() == Ld::TableLineSettings::Style::DOUBLE    ) {
                    attributes.append("width", lineSetting.width());
                }
            } else {
                if (lineSetting.lineStyle() == Ld::TableLineSettings::Style::SINGLE ||
                    lineSetting.lineStyle() == Ld::TableLineSettings::Style::DOUBLE    ) {
                    attributes.append("width", lineSetting.width());
                }
            }

            writer->writeStartElement("HorizontalLine");
            writer->writeAttributes(attributes);
            writer->writeEndElement();
        }

        for (  ColorByRowColumnIndex::const_iterator colorByColumnIterator    = currentCellColorByColumn.constBegin(),
                                                     colorByColumnEndIterator = currentCellColorByColumn.constEnd()
             ; colorByColumnIterator != colorByColumnEndIterator
             ; ++colorByColumnIterator
            ) {
            unsigned columnIndex = colorByColumnIterator.key();
            QColor   columnColor = colorByColumnIterator.value();

            XmlAttributes attributes;
            attributes.append("index", columnIndex);
            attributes.append("color", columnColor);

            writer->writeStartElement("ColumnColor");
            writer->writeAttributes(attributes);
            writer->writeEndElement();
        }

        for (  ColorByRowColumnIndex::const_iterator colorByRowIterator    = currentCellColorByRow.constBegin(),
                                                     colorByRowEndIterator = currentCellColorByRow.constEnd()
             ; colorByRowIterator != colorByRowEndIterator
             ; ++colorByRowIterator
            ) {
            unsigned rowIndex = colorByRowIterator.key();
            QColor   rowColor = colorByRowIterator.value();

            XmlAttributes attributes;
            attributes.append("index", rowIndex);
            attributes.append("color", rowColor);

            writer->writeStartElement("RowColor");
            writer->writeAttributes(attributes);
            writer->writeEndElement();
        }

        for (  ColorByPosition::const_iterator cellColorIterator    = currentCellColorsByPosition.constBegin(),
                                               cellColorEndIterator = currentCellColorsByPosition.constEnd()
             ; cellColorIterator != cellColorEndIterator
             ; ++cellColorIterator
            ) {
            const CellPosition& position  = cellColorIterator.key();
            const QColor&       cellColor = cellColorIterator.value();

            XmlAttributes attributes;
            attributes.append("row", position.rowIndex());
            attributes.append("column", position.columnIndex());
            attributes.append("color", cellColor);

            writer->writeStartElement("CellColor");
            writer->writeAttributes(attributes);
            writer->writeEndElement();
        }
    }


    void TableFrameFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        TableColumnWidth::WidthType newDefaultColumnWidthType = initialDefaultColumnWidth.widthType();
        float                       newDefaultColumnWidth     = initialDefaultColumnWidth.width();

        TableLineSettings::Style    newDefaultColumnLineStyle = defaultLineSetting.lineStyle();
        float                       newDefaultColumnLineWidth = defaultLineSetting.width();

        TableLineSettings::Style    newDefaultRowLineStyle    = defaultLineSetting.lineStyle();
        float                       newDefaultRowLineWidth    = defaultLineSetting.width();

        QColor                      newDefaultColor           = QColor();

        float                       newLeftGutter             = defaultLeftGutter;
        float                       newRightGutter            = defaultRightGutter;
        float                       newTopGutter              = defaultTopGutter;
        float                       newBottomGutter           = defaultBottomGutter;

        if (!reader->hasError() && attributes.hasAttribute("column_width_style")) {
            bool ok;
            QString styleString = attributes.value<QString>("column_width_style");
            newDefaultColumnWidthType = toColumnWidthType(styleString, &ok);
            if (!ok) {
                QString tagName = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid column width type \"%1\"").arg(tagName).arg(styleString));
            }
        }

        if (!reader->hasError() && attributes.hasAttribute("column_width")) {
            bool ok;
            newDefaultColumnWidth = attributes.value<float>("column_width", &ok);
            if (!ok) {
                QString tagName = reader->qualifiedName().toString();
                QString value   = attributes.value<QString>("column_width");
                reader->raiseError(tr("Tag \"%1\", invalid column width \"%1\"").arg(tagName).arg(value));
            }
        }

        if (!reader->hasError() && attributes.hasAttribute("column_line_style")) {
            bool ok;
            QString styleString = attributes.value<QString>("column_line_style");
            newDefaultColumnLineStyle = toLineStyle(styleString, &ok);
            if (!ok) {
                QString tagName = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid column line style \"%1\"").arg(tagName).arg(styleString));
            }
        }

        if (!reader->hasError() && attributes.hasAttribute("column_line_width")) {
            bool ok;
            newDefaultColumnLineWidth = attributes.value<float>("column_line_width", &ok);
            if (!ok) {
                QString tagName = reader->qualifiedName().toString();
                QString value   = attributes.value<QString>("column_line_width");
                reader->raiseError(tr("Tag \"%1\", invalid column line width \"%1\"").arg(tagName).arg(value));
            }
        }

        if (!reader->hasError() && attributes.hasAttribute("row_line_style")) {
            bool ok;
            QString styleString = attributes.value<QString>("row_line_style");
            newDefaultRowLineStyle = toLineStyle(styleString, &ok);
            if (!ok) {
                QString tagName = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid row line style \"%1\"").arg(tagName).arg(styleString));
            }
        }

        if (!reader->hasError() && attributes.hasAttribute("row_line_width")) {
            bool ok;
            newDefaultRowLineWidth = attributes.value<float>("row_line_width", &ok);
            if (!ok) {
                QString tagName = reader->qualifiedName().toString();
                QString value   = attributes.value<QString>("row_line_width");
                reader->raiseError(tr("Tag \"%1\", invalid row line width \"%1\"").arg(tagName).arg(value));
            }
        }

        if (!reader->hasError() && attributes.hasAttribute("cell_color")) {
            bool ok;
            newDefaultColor = attributes.colorValue("cell_color", &ok);
            if (!ok) {
                QString tagName   = reader->qualifiedName().toString();
                QString colorName = attributes.value<QString>("cell_color");
                reader->raiseError(tr("Tag \"%1\", invalid color \"%1\"").arg(tagName).arg(colorName));
            }
        }

        if (!reader->hasError() && attributes.hasAttribute("left_gutter")) {
            bool ok;
            newLeftGutter = attributes.value<float>("left_gutter", &ok);
            if (!ok || newLeftGutter < 0.0F) {
                QString tagName = reader->qualifiedName().toString();
                QString value   = attributes.value<QString>("left_gutter");
                reader->raiseError(tr("Tag \"%1\", invalid left gutter \"%1\"").arg(tagName).arg(value));
            }
        }

        if (!reader->hasError() && attributes.hasAttribute("right_gutter")) {
            bool ok;
            newRightGutter = attributes.value<float>("right_gutter", &ok);
            if (!ok || newRightGutter < 0.0F) {
                QString tagName = reader->qualifiedName().toString();
                QString value   = attributes.value<QString>("right_gutter");
                reader->raiseError(tr("Tag \"%1\", invalid right gutter \"%1\"").arg(tagName).arg(value));
            }
        }

        if (!reader->hasError() && attributes.hasAttribute("top_gutter")) {
            bool ok;
            newTopGutter = attributes.value<float>("top_gutter", &ok);
            if (!ok || newTopGutter < 0.0F) {
                QString tagName = reader->qualifiedName().toString();
                QString value   = attributes.value<QString>("top_gutter");
                reader->raiseError(tr("Tag \"%1\", invalid top gutter \"%1\"").arg(tagName).arg(value));
            }
        }

        if (!reader->hasError() && attributes.hasAttribute("bottom_gutter")) {
            bool ok;
            newBottomGutter = attributes.value<float>("bottom_gutter", &ok);
            if (!ok || newBottomGutter < 0.0F) {
                QString tagName = reader->qualifiedName().toString();
                QString value   = attributes.value<QString>("bottom_gutter");
                reader->raiseError(tr("Tag \"%1\", invalid bottom gutter \"%1\"").arg(tagName).arg(value));
            }
        }

        if (!reader->hasError()) {
            currentDefaultColumnWidth       = TableColumnWidth(newDefaultColumnWidthType, newDefaultColumnWidth);
            currentDefaultColumnLineSetting = TableLineSettings(newDefaultColumnLineStyle, newDefaultColumnLineWidth);
            currentDefaultRowLineSetting    = TableLineSettings(newDefaultRowLineStyle, newDefaultRowLineWidth);
            currentDefaultColor             = newDefaultColor;
            currentLeftGutter               = newLeftGutter;
            currentRightGutter              = newRightGutter;
            currentTopGutter                = newTopGutter;
            currentBottomGutter             = newBottomGutter;
        }
    }


    void TableFrameFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        BlockFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        JustifiedBlockFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        TableFrameFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }


    void TableFrameFormat::readChild(
            QSharedPointer<XmlReader>  reader,
            const QString&             tagName,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            const XmlAttributes&       attributes,
            unsigned                   xmlVersion
        ) {
        if (tagName == QString("ColumnWidth")) {
            TableColumnWidth::WidthType widthType = currentDefaultColumnWidth.widthType();
            float                       width     = currentDefaultColumnWidth.width();
            unsigned                    index;
            bool                        ok;

            if (attributes.hasAttribute("index")) {
                index = attributes.value<unsigned>("index", &ok);
                if (!ok) {
                    QString value = attributes.value<QString>("index");
                    reader->raiseError(tr("Tag \"%1\" invalid index \"%1\"").arg(tagName).arg(value));
                }
            } else {
                reader->raiseError(tr("Tag \"%1\" missing index").arg(tagName));
            }

            if (attributes.hasAttribute("style")) {
                QString text = attributes.value<QString>("style");
                widthType = toColumnWidthType(text, &ok);
                if (!ok) {
                    reader->raiseError(tr("Tag \"%1\" invalid style \"%1\"").arg(tagName).arg(text));
                }
            }

            if (attributes.hasAttribute("width")) {
                width = attributes.value<float>("width", &ok);
                if (!ok || width < 0.0F) {
                    QString value = attributes.value<QString>("width");
                    reader->raiseError(tr("Tag \"%1\" invalid width \"%1\"").arg(tagName).arg(value));
                }
            }

            currentTableColumnWidths.insert(index, TableColumnWidth(widthType, width));
        } else if (tagName == QString("VerticalLine") || tagName == QString("HorizontalLine")) {
            TableLineSettings::Style lineStyle =   tagName == QString("VerticalLine")
                                                 ? currentDefaultColumnLineSetting.lineStyle()
                                                 : currentDefaultRowLineSetting.lineStyle();
            float                    width     = currentDefaultColumnWidth.width();
            unsigned                 index;
            bool                     ok;

            if (attributes.hasAttribute("index")) {
                index = attributes.value<unsigned>("index", &ok);
                if (!ok) {
                    QString value = attributes.value<QString>("index");
                    reader->raiseError(tr("Tag \"%1\" invalid index \"%1\"").arg(tagName).arg(value));
                }
            } else {
                reader->raiseError(tr("Tag \"%1\" missing index").arg(tagName));
            }

            if (attributes.hasAttribute("style")) {
                QString text = attributes.value<QString>("style");
                lineStyle = toLineStyle(text, &ok);
                if (!ok) {
                    reader->raiseError(tr("Tag \"%1\" invalid style \"%1\"").arg(tagName).arg(text));
                }
            }

            if (attributes.hasAttribute("width")) {
                width = attributes.value<float>("width", &ok);
                if (!ok || width < 0.0F) {
                    QString value = attributes.value<QString>("width");
                    reader->raiseError(tr("Tag \"%1\" invalid width \"%1\"").arg(tagName).arg(value));
                }
            }

            if (tagName == QString("VerticalLine")) {
                currentColumnLineSettings.insert(index, TableLineSettings(lineStyle, width));
            } else {
                currentRowLineSettings.insert(index, TableLineSettings(lineStyle, width));
            }
        } else if (tagName == QString("ColumnColor") || tagName == QString("RowColor")) {
            QColor   color;
            unsigned index;
            bool     ok;

            if (attributes.hasAttribute("index")) {
                index = attributes.value<unsigned>("index", &ok);
                if (!ok) {
                    QString value = attributes.value<QString>("index");
                    reader->raiseError(tr("Tag \"%1\" invalid index \"%1\"").arg(tagName).arg(value));
                }
            } else {
                reader->raiseError(tr("Tag \"%1\" missing index").arg(tagName));
            }

            if (attributes.hasAttribute("color")) {
                color = attributes.colorValue("color", &ok);
                if (!ok) {
                    QString text = attributes.value<QString>("color");
                    reader->raiseError(tr("Tag \"%1\" invalid style \"%1\"").arg(tagName).arg(text));
                }
            } else {
                reader->raiseError(tr("Tag \"%1\" missing color").arg(tagName));
            }

            if (tagName == QString("ColumnColor")) {
                currentCellColorByColumn.insert(index, color);
            } else {
                currentCellColorByRow.insert(index, color);
            }
        } else if (tagName == QString("CellColor")) {
            unsigned rowIndex    = 0;
            unsigned columnIndex = 0;
            QColor   color;
            bool     ok;

            if (attributes.hasAttribute("row")) {
                rowIndex = attributes.value<unsigned>("row", &ok);
                if (!ok) {
                    QString value = attributes.value<QString>("row");
                    reader->raiseError(tr("Tag \"%1\" invalid row index \"%1\"").arg(tagName).arg(value));
                }
            } else {
                reader->raiseError(tr("Tag \"%1\" missing row index").arg(tagName));
            }

            if (attributes.hasAttribute("column")) {
                columnIndex = attributes.value<unsigned>("column", &ok);
                if (!ok) {
                    QString value = attributes.value<QString>("column");
                    reader->raiseError(tr("Tag \"%1\" invalid column index \"%1\"").arg(tagName).arg(value));
                }
            } else {
                reader->raiseError(tr("Tag \"%1\" missing column index").arg(tagName));
            }

            if (attributes.hasAttribute("color")) {
                color = attributes.colorValue("color", &ok);
                if (!ok) {
                    QString text = attributes.value<QString>("color");
                    reader->raiseError(tr("Tag \"%1\" invalid style \"%1\"").arg(tagName).arg(text));
                }
            } else {
                reader->raiseError(tr("Tag \"%1\" missing color").arg(tagName));
            }

            currentCellColorsByPosition.insert(CellPosition(rowIndex, columnIndex), color);
        } else {
            reader->raiseError(tr("Tag \"%1\" invalid name").arg(tagName));
        }

        if (!reader->atEnd() && !reader->hasError()) {
            XmlReader::TokenType tokenType;

            do {
                tokenType = reader->readNext();

                if (tokenType == XmlReader::Characters) {
                    QString pcData = reader->text().toString();
                    readData(reader, pcData, formats, programFile, xmlVersion);
                } else if (tokenType != XmlReader::EndElement) {
                    reader->raiseError(tr("Invalid primitive"));
                }
            } while (!reader->hasError() && tokenType != XmlReader::EndElement);
        }
    }


    QString TableFrameFormat::toString(const TableColumnWidth& tableColumnWidth) {
        QString result;
        switch (tableColumnWidth.widthType()) {
            case TableColumnWidth::WidthType::INVALID: {
                result = QString("INVALID()");
                break;
            }

            case TableColumnWidth::WidthType::FIXED: {
                result = QString("FIXED(%1)").arg(tableColumnWidth.width());
                break;
            }

            case TableColumnWidth::WidthType::PROPORTIONAL: {
                result = QString("PROPORTIONAL(%1)").arg(tableColumnWidth.width());
                break;
            }

            default: {
                Q_ASSERT(false);
                break;
            }
        }

        return result;
    }


    QString TableFrameFormat::toString(const TableLineSettings& tableLineSettings) {
        QString result;
        switch (tableLineSettings.lineStyle()) {
            case TableLineSettings::Style::INVALID: {
                result = QString("INVALID()");
                break;
            }

            case TableLineSettings::Style::NO_LINE: {
                result = QString("NO_LINE()");
                break;
            }

            case TableLineSettings::Style::SINGLE: {
                result = QString("SINGLE(%1)").arg(tableLineSettings.width());
                break;
            }

            case TableLineSettings::Style::DOUBLE: {
                result = QString("DOUBLE(%1)").arg(tableLineSettings.width());
                break;
            }

            default: {
                Q_ASSERT(false);
                break;
            }
        }

        return result;
    }


    QString TableFrameFormat::toString(TableColumnWidth::WidthType columnWidthType) {
        QString result;
        switch (columnWidthType) {

            case TableColumnWidth::WidthType::INVALID: {
                result = QString("invalid");
                break;
            }

            case TableColumnWidth::WidthType::FIXED: {
                result = QString("fixed");
                break;
            }

            case TableColumnWidth::WidthType::PROPORTIONAL: {
                result = QString("proportional");
                break;
            }

            default: {
                Q_ASSERT(false);
                break;
            }
        }

        return result;
    }


    QString TableFrameFormat::toString(TableLineSettings::Style lineStyle) {
        QString result;
        switch (lineStyle) {
            case TableLineSettings::Style::INVALID: {
                result = QString("invalid");
                break;
            }

            case TableLineSettings::Style::NO_LINE: {
                result = QString("no_line");
                break;
            }

            case TableLineSettings::Style::SINGLE: {
                result = QString("single");
                break;
            }

            case TableLineSettings::Style::DOUBLE: {
                result = QString("double");
                break;
            }

            default: {
                Q_ASSERT(false);
                break;
            }
        }

        return result;
    }


    TableColumnWidth::WidthType TableFrameFormat::toColumnWidthType(const QString& text, bool* ok) {
        bool                        success;
        TableColumnWidth::WidthType result;
        QString                     cleaned = text.trimmed().toLower();

        if (cleaned == QString("invalid")) {
            result  = TableColumnWidth::WidthType::INVALID;
            success = true;
        } else if (cleaned == QString("fixed")) {
            result  = TableColumnWidth::WidthType::FIXED;
            success = true;
        } else if (cleaned == QString("proportional")) {
            result  = TableColumnWidth::WidthType::PROPORTIONAL;
            success = true;
        } else {
            result  = TableColumnWidth::WidthType::INVALID;
            success = false;
        }

        if (ok != nullptr) {
            *ok = success;
        }

        return result;
    }


    TableLineSettings::Style TableFrameFormat::toLineStyle(const QString& text, bool* ok) {
        bool                     success;
        TableLineSettings::Style result;
        QString                  cleaned = text.trimmed().toLower();

        if (cleaned == QString("invalid")) {
            result  = TableLineSettings::Style::INVALID;
            success = true;
        } else if (cleaned == QString("no_line") || cleaned == QString("no line")) {
            result  = TableLineSettings::Style::NO_LINE;
            success = true;
        } else if (cleaned == QString("single") || cleaned == QString("regular")) {
            result  = TableLineSettings::Style::SINGLE;
            success = true;
        } else if (cleaned == QString("double") || cleaned == QString("dual")) {
            result  = TableLineSettings::Style::DOUBLE;
            success = true;
        } else {
            result  = TableLineSettings::Style::INVALID;
            success = false;
        }

        if (ok != nullptr) {
            *ok = success;
        }

        return result;
    }


    Util::HashResult qHash(const TableFrameFormat::CellPosition& key, Util::HashSeed seed) {
        return ::qHash(key.rowIndex(), seed) ^ ::qHash(key.columnIndex(), seed);
    }


    Util::HashResult qHash(const TableFrameFormat::WidthsByColumn& key, Util::HashSeed seed) {
        unsigned result = 0;

        for (TableFrameFormat::WidthsByColumn::const_iterator it=key.constBegin(),end=key.constEnd() ; it!=end ; ++it) {
            unsigned                columnIndex = it.key();
            const TableColumnWidth& width       = it.value();
            result ^= ::qHash(columnIndex, seed) ^ qHash(width, seed);
        }

        return result;
    }


    Util::HashResult qHash(const TableFrameFormat::LineSettingsByIndex& key, Util::HashSeed seed) {
        unsigned result = 0;

        for (  TableFrameFormat::LineSettingsByIndex::const_iterator it  = key.constBegin(),
                                                                     end = key.constEnd()
             ; it!=end
             ; ++it
            ) {
            unsigned                 columnIndex = it.key();
            const TableLineSettings& width       = it.value();

            result ^= ::qHash(columnIndex, seed) ^ qHash(width, seed);
        }

        return result;
    }


    Util::HashResult qHash(const TableFrameFormat::ColorByPosition& key, Util::HashSeed seed) {
        unsigned result = 0;

        for (  TableFrameFormat::ColorByPosition::const_iterator it  = key.constBegin(),
                                                                 end = key.constEnd()
             ; it!=end
             ; ++it
            ) {
            const TableFrameFormat::CellPosition& position = it.key();
            const QColor&                         color    = it.value();

            result ^= (
                  ::qHash(position.rowIndex(), seed)
                ^ ::qHash(position.columnIndex(), seed)
                ^ ::qHash(color, seed)
            );
        }

        return result;
    }
}


Util::HashResult qHash(const Ld::TableFrameFormat::ColorByRowColumnIndex& key, Util::HashSeed seed) {
    unsigned result = 0;

    for (  Ld::TableFrameFormat::ColorByRowColumnIndex::const_iterator it  = key.constBegin(),
                                                                       end = key.constEnd()
         ; it!=end
         ; ++it
        ) {
        unsigned      columnIndex = it.key();
        const QColor& color       = it.value();

        result ^= qHash(columnIndex, seed) ^ qHash(color, seed);
    }

    return result;
}
