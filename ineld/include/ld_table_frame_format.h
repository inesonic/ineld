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
* This header defines the \ref Ld::TableFrameFormat class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_TABLE_FRAME_FORMAT_H
#define LD_TABLE_FRAME_FORMAT_H

#include <QString>
#include <QSharedPointer>
#include <QList>
#include <QMap>
#include <QSet>
#include <QColor>

#include <cstdint>

#include <util_hash_functions.h>

#include "ld_common.h"
#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_attributes.h"
#include "ld_table_frame_element.h"
#include "ld_table_column_width.h"
#include "ld_table_line_settings.h"
#include "ld_justified_block_format.h"

namespace Ld {
    class FormatOrganizer;
    class XmlWriter;
    class XmlAttributes;
    class XmlReader;

    /**
     * Class that extends the \ref Ld::BlockFormat class to configure it for table formatting.  The class provides
     * outside margins for the table, left/right justification, as well as column widths and number of columns.
     */
    class LD_PUBLIC_API TableFrameFormat:public virtual JustifiedBlockFormat {
        public:
            /**
             * Type used to represent a cell position.
             */
            typedef TableFrameElement::CellPosition CellPosition;

            /**
             * Type used to represent a map of table column widths by column.
             */
            typedef QMap<unsigned, TableColumnWidth> WidthsByColumn;

            /**
             * Type used to represent a map of line settings by line index.
             */
            typedef QMap<unsigned, TableLineSettings> LineSettingsByIndex;

            /**
             * Type used to represent a map of cell colors by position.
             */
            typedef QMap<CellPosition, QColor> ColorByPosition;

            /**
             * Type used to represent a map of cell colors by row or column.
             */
            typedef QMap<unsigned, QColor> ColorByRowColumnIndex;

            /**
             * Class you can use to track an aggregation of multiple \ref Ld::TableFrameFormat instances.
             */
            class LD_PUBLIC_API Aggregation:public virtual JustifiedBlockFormat::Aggregation {
                public:
                    Aggregation();

                    ~Aggregation() override;

                    /**
                     * Creator function for this aggregator.
                     *
                     * \param[in] formatName The name of the format to create an aggregator for.
                     *
                     * \return Returns a pointer to the newly created aggregator.
                     */
                    static Format::Aggregation* creator(const QString& formatName);

                    /**
                     * Method that returns the type name of the format tracked by this aggregation.
                     *
                     * \return Returns the format name for formats tied to this aggregation.
                     */
                    QString typeName() const override;

                    /**
                     * Method you can use to clear the aggregation.  Calling the base class instance will clear the
                     * underlying format database.
                     */
                    void clear() override;

                    /**
                     * Virtual method you can call to add a format instance to the aggregation.  Derived class
                     * derived class instances should call this method to add the format to the aggregation.
                     *
                     * \param[in] formatInstance  The format instance to be added.
                     *
                     * \param[in] includeExisting If true, this format will be included in the aggregation even if it
                     *                            has already been accounted for.
                     *
                     * \return Returns true if the format was added to the database.  Returns false if the format is
                     *         already contained in the underlying database.
                     */
                    bool addFormat(FormatPointer formatInstance, bool includeExisting = false) override;

                    /**
                     * Method you can use to obtain a set containing the default column widths.
                     *
                     * \return Returns a set containing all the default column widths.
                     */
                    QSet<TableColumnWidth> defaultColumnWidths() const;

                    /**
                     * Method you can use to obtain a map of sets of column widths for each column.
                     *
                     * \return Returns a map of sets of column widths.
                     */
                    QMap<unsigned, QSet<TableColumnWidth>> columnWidthsByColumn() const;

                    /**
                     * Method you can use to obtain a set of maps of column widths for each column.
                     *
                     * \return Returns a set of maps of column widths.
                     */
                    QSet<QMap<unsigned, TableColumnWidth>> columnWidthsByGroup() const;

                    /**
                     * Method you can use to obtain a set of the default column line settings.
                     *
                     * \return Returns a set of the default column line settings.
                     */
                    QSet<TableLineSettings> defaultColumnLineSettings() const;

                    /**
                     * Method you can use to obtain a map of sets of column line settings.
                     *
                     * \return Returns a map of sets of column line settings by line index.
                     */
                    QMap<unsigned, QSet<TableLineSettings>> columnLineSettingsByIndex() const;

                    /**
                     * Method you can use to obtain a set of maps of column line settings.
                     *
                     * \return Returns a set of maps of column line settings.
                     */
                    QSet<LineSettingsByIndex> columnLineSettingsByGroup() const;

                    /**
                     * Method you can use to obtain a set of the default row line settings.
                     *
                     * \return Returns a set of the default row line settings.
                     */
                    QSet<TableLineSettings> defaultRowLineSettings() const;

                    /**
                     * Method you can use to obtain a map of sets of row line settings.
                     *
                     * \return Returns a map of sets of row line settings by line index.
                     */
                    QMap<unsigned, QSet<TableLineSettings>> rowLineSettingsByIndex() const;

                    /**
                     * Method you can use to obtain a set of maps of row line settings.
                     *
                     * \return Returns a set of maps of row line settings.
                     */
                    QSet<LineSettingsByIndex> rowLineSettingsByGroup() const;

                    /**
                     * Method you can use to obtain a set containing the default cell color.
                     *
                     * \return Returns a set holding all the default cell colors.
                     */
                    QSet<QColor> defaultCellColors() const;

                    /**
                     * Method you can use to obtain a map of sets of column colors by column.
                     *
                     * \return Returns a map of sets of column colors by column.
                     */
                    QMap<unsigned, QSet<QColor>> columnColorsByIndex() const;

                    /**
                     * Method you can use to obtain a set of maps of column colors by column.
                     *
                     * \return Returns a set of maps of column colors by column.
                     */
                    QSet<ColorByRowColumnIndex> columnColorsByGroup() const;

                    /**
                     * Method you can use to obtain a map of sets of row colors by row.
                     *
                     * \return Returns a map of sets of row colors by row.
                     */
                    QMap<unsigned, QSet<QColor>> rowColorsByIndex() const;

                    /**
                     * Method you can use to obtain a set of maps of row colors by row.
                     *
                     * \return Returns a set of maps of row colors by row.
                     */
                    QSet<ColorByRowColumnIndex> rowColorsByGroup() const;

                    /**
                     * Method you can use to obtain a map of sets of colors by cell position.
                     *
                     * \return Returns a map of sets of colors by cell position.
                     */
                    QMap<CellPosition, QSet<QColor>> cellColorsByPosition() const;

                    /**
                     * Method you can use to obtain a set of maps of colors by cell position.
                     *
                     * \return Returns a set of maps of sets of colors by cell position.
                     */
                    QSet<ColorByPosition> cellColorsByGroup() const;

                    /**
                     * Method you can use to obtain all the left gutter values.
                     *
                     * \return Returns a set of all the left gutter values.
                     */
                    QSet<float> leftGutterValues() const;

                    /**
                     * Method you can use to obtain all the right gutter values.
                     *
                     * \return Returns a set of all the right gutter values.
                     */
                    QSet<float> rightGutterValues() const;

                    /**
                     * Method you can use to obtain all the top gutter values.
                     *
                     * \return Returns a set of all the top gutter values.
                     */
                    QSet<float> topGutterValues() const;

                    /**
                     * Method you can use to obtain all the bottom gutter values.
                     *
                     * \return Returns a set of all the bottom gutter values.
                     */
                    QSet<float> bottomGutterValues() const;

                private:
                    /**
                     * Set of default column width values.
                     */
                    QSet<TableColumnWidth> currentDefaultColumnWidths;

                    /**
                     * Sets of column widths by column.
                     */
                    QMap<unsigned, QSet<TableColumnWidth>> currentColumnWidthsByColumn;

                    /**
                     * Sets of groups of column widths by column.
                     */
                    QSet<WidthsByColumn> currentColumnWidthsByGroup;

                    /**
                     * Sets of default line settings.
                     */
                    QSet<TableLineSettings> currentDefaultColumnLineSettings;

                    /**
                     * Sets of column line settings by column.
                     */
                    QMap<unsigned, QSet<TableLineSettings>> currentColumnLineSettingsByColumn;

                    /**
                     * Sets of groups of column line settings by column.
                     */
                    QSet<LineSettingsByIndex> currentColumnLineSettingsByGroup;

                    /**
                     * Sets of default line settings.
                     */
                    QSet<TableLineSettings> currentDefaultRowLineSettings;

                    /**
                     * Sets of row line settings by row.
                     */
                    QMap<unsigned, QSet<TableLineSettings>> currentRowLineSettingsByRow;

                    /**
                     * Sets of groups of row line settings by row.
                     */
                    QSet<LineSettingsByIndex> currentRowLineSettingsByGroup;

                    /**
                     * Set of default column color.
                     */
                    QSet<QColor> currentDefaultCellColors;

                    /**
                     * Sets of column colors by column index.
                     */
                    QMap<unsigned, QSet<QColor>> currentColumnColorsByColumn;

                    /**
                     * Sets of groups of column colors.
                     */
                    QSet<ColorByRowColumnIndex> currentColumnColorsByGroup;

                    /**
                     * Sets of row colors by row index.
                     */
                    QMap<unsigned, QSet<QColor>> currentRowColorsByRow;

                    /**
                     * Sets of groups of row colors.
                     */
                    QSet<ColorByRowColumnIndex> currentRowColorsByGroup;

                    /**
                     * Map of sets of cell colors by position.
                     */
                    QMap<CellPosition, QSet<QColor>> currentCellColorsByPosition;

                    /**
                     * Set of maps of cell colors by position.
                     */
                    QSet<ColorByPosition> currentCellColorsByGroup;

                    /**
                     * Set of left gutter values.
                     */
                    QSet<float> currentLeftGutterValues;

                    /**
                     * Set of right gutter values.
                     */
                    QSet<float> currentRightGutterValues;

                    /**
                     * Set of top gutter values.
                     */
                    QSet<float> currenTopGutterValues;

                    /**
                     * Set of bottom gutter values.
                     */
                    QSet<float> currentBottomGutterValues;
            };

            /**
             * The typename for the format.
             */
            static const QString formatName;

            /**
             * The default left indentation setting.
             */
            static const float defaultLeftIndentation;

            /**
             * The default right indentation setting.
             */
            static const float defaultRightIndentation;

            /**
             * The default top block spacing.
             */
            static const float defaultTopSpacing;

            /**
             * The default bottom block spacing.
             */
            static const float defaultBottomSpacing;

            /**
             * The default justification mode.
             */
            static const Justification defaultJustification;

            /**
             * The default column width setting.
             */
            static const TableColumnWidth initialDefaultColumnWidth;

            /**
             * The default row/column line style.
             */
            static const TableLineSettings defaultLineSetting;

            /**
             * The default left gutter value.
             */
            static const float defaultLeftGutter;

            /**
             * The default right gutter value.
             */
            static const float defaultRightGutter;

            /**
             * The default top gutter value.
             */
            static const float defaultTopGutter;

            /**
             * The default bottom gutter value.
             */
            static const float defaultBottomGutter;

            TableFrameFormat();

            /**
             * Copy constructor.  Note that the copy constructor does not copy elements from the underlying
             * \ref Ld::Format class.  All format parameters are copied by the format are not tied to any element.
             *
             * \param[in] other The instance to be copied.
             */
            TableFrameFormat(const TableFrameFormat& other);

            ~TableFrameFormat() override;

            /**
             * Overloaded method that creates a clone of this class instance.  Note that the clone method does not copy
             * elements from the underlying \ref Ld::Format class.  All format parameters are copied by the format are
             * not tied to any element.
             *
             * \return Returns a deep copy of this class instance.
             */
            FormatPointer clone() const final;

            /**
             * Factory method that can be used to create a default instance of this format.
             *
             * \param[in] formatType The name used to reference this creator function.
             *
             * \return Returns a newly created, default, format instance.
             */
            static Format* creator(const QString& formatType);

            /**
             * Overloaded method that returns a name for this format.  The value is used to track and categorize
             * different types of format data.
             *
             * \return Returns the format identifying name, in this case, "BlockFormat".
             */
            QString typeName() const final;

            /**
             * Method that indicates if this format contains valid information.
             *
             * \return Returns true if the format is valid.  Returns false if the format is invalid.
             */
            bool isValid() const final;

            /**
             * Method you can call to determine the capabilities of this format.  The capabilities is a set containing
             * all the classes that make up the format instance.
             *
             * \return Returns a \ref Ld::Format::Capabilities instance containing the names of all the classes that
             *         define this format.
             */
            Capabilities capabilities() const final;

            /**
             * Method you can use to obtain the column width setting for a specific column.
             *
             * \param[in] columnIndex The column index of issue.
             *
             * \return Returns the column width at the specified column
             */
            TableColumnWidth columnWidth(unsigned columnIndex) const;

            /**
             * Method you can use to set the column width setting for a specific column.
             *
             * \param[in] columnIndex    The column index of issue.
             *
             * \param[in] newColumnWidth The new column width setting.
             */
            void setColumnWidth(unsigned columnIndex, const TableColumnWidth& newColumnWidth);

            /**
             * Method you can use to determine if a column maps to the current default column width.
             *
             * \param[in] columnIndex The column index at issue.
             *
             * \return Return true if the column width will map to the default value.  Returns false if the column
             *         width is set specifically for this column width.
             */
            bool columnWidthMapsToDefault(unsigned columnIndex) const;

            /**
             * Method you can use to force a column to map to the default column width.
             *
             * \param[in] columnIndex The column index at issue.
             */
            void setColumnToDefaultWidth(unsigned columnIndex);

            /**
             * Method you can use to obtain the default column width setting.
             *
             * \param[in] columnIndex The column index of issue.
             */
            const TableColumnWidth& defaultColumnWidth() const;

            /**
             * Method you can use to set the defaut column width setting.
             *
             * \param[in] newDefaultColumnWidth The new column width setting.
             */
            void setDefaultColumnWidth(const TableColumnWidth& newDefaultColumnWidth);

            /**
             * Method you can use to obtain the column line specification for a given line.  There will always be one
             * more column line specification than the number of columns.
             *
             * \param[in] lineIndex The zero based line index.  Line 0 represents the left border line.  The last line
             *                      represents the right border line.
             *
             * \return Returns the column line specification.
             */
            TableLineSettings columnLineSetting(unsigned lineIndex) const;

            /**
             * Method you can use to change the column line specification for a given line.  There will always be one
             * more column line specification than the number of columns.
             *
             * \param[in] lineIndex   The zero based line index.  Line 0 represents the left border line.  The last
             *                        line represents the right border line.
             *
             * \param[in] lineSetting The line setting to apply to this line.
             */
            void setColumnLineSetting(unsigned lineIndex, const TableLineSettings& lineSetting);

            /**
             * Method you can use to determine if a column line setting maps to the current default column line
             * setting.
             *
             * \param[in] lineIndex The column line index at issue.
             *
             * \return Return true if the column line setting will map to the default value.  Returns false if the
             *         column line setting is set specifically for this column.
             */
            bool columnLineSettingMapsToDefault(unsigned lineIndex) const;

            /**
             * Method you can use to force a column line setting to map to the default column line setting.
             *
             * \param[in] columnIndex The column line index at issue.
             */
            void setColumnLineSettingToDefault(unsigned columnIndex);

            /**
             * Method you can use to obtain the default column line specification.
             *
             * \return Returns the default column line specification.
             */
            const TableLineSettings& defaultColumnLineSetting() const;

            /**
             * Method you can use to change the default column line specification for a given line.
             *
             * \param[in] lineSetting The line setting to apply to this line.
             */
            void setDefaultColumnLineSetting(const TableLineSettings& lineSetting);

            /**
             * Method you can use to obtain the row line specification for a given line.  There will always be one
             * more row line specification than the number of rows.
             *
             * \param[in] lineIndex The zero based line index.  Line 0 represents the left border line.  The last line
             *                      represents the right border line.
             *
             * \return Returns the row line specification.
             */
            TableLineSettings rowLineSetting(unsigned lineIndex) const;

            /**
             * Method you can use to change the row line specification for a given line.  There will always be one
             * more row line specification than the number of rows.
             *
             * \param[in] lineIndex   The zero based line index.  Line 0 represents the left border line.  The last
             *                        line represents the right border line.
             *
             * \param[in] lineSetting The line setting to apply to this line.
             */
            void setRowLineSetting(unsigned lineIndex, const TableLineSettings& lineSetting);

            /**
             * Method you can use to determine if a row line setting maps to the current default row line setting.
             *
             * \param[in] lineIndex The row line index at issue.
             *
             * \return Return true if the row line setting will map to the default value.  Returns false if the row
             *         line setting is set specifically for this row.
             */
            bool rowLineSettingMapsToDefault(unsigned lineIndex) const;

            /**
             * Method you can use to force a row line setting to map to the default row line setting.
             *
             * \param[in] rowIndex The row line index at issue.
             */
            void setRowLineSettingToDefault(unsigned rowIndex);

            /**
             * Method you can use to obtain the default row line specification.
             *
             * \return Returns the default row line specification.
             */
            const TableLineSettings& defaultRowLineSetting() const;

            /**
             * Method you can use to change the default row line specification for a given line.
             *
             * \param[in] lineSetting The line setting to apply to this line.
             */
            void setDefaultRowLineSetting(const TableLineSettings& lineSetting);

            /**
             * Method you can use to obtain the color to apply to a specific cell.
             *
             * \param[in] rowIndex    The zero based index of the row of the cell.
             *
             * \param[in] columnIndex The zero based column index of the cell.
             *
             * \return Returns the color to apply to the specific cell.
             */
            QColor blendedColor(unsigned rowIndex, unsigned columnIndex) const;

            /**
             * Method you can use to obtain the color specified at a specific cell.
             *
             * \param[in] rowIndex    The zero based index of the row of the cell.
             *
             * \param[in] columnIndex The zero based column index of the cell.
             *
             * \return Returns the color to apply to the specific cell.
             */
            QColor cellColor(unsigned rowIndex, unsigned columnIndex) const;

            /**
             * Method you can use to set the color to apply to a specific cell.
             *
             * \param[in] rowIndex    The zero based row index of the cell.
             *
             * \param[in] columnIndex The zero based column index of the cell.
             *
             * \param[in] cellColor   The color to apply to the cell.
             */
            void setCellColor(unsigned rowIndex, unsigned columnIndex, const QColor& cellColor);

            /**
             * Method you can use to determine if the cell color maps to the default.
             *
             * \param[in] rowIndex    The zero based row index of the cell.
             *
             * \param[in] columnIndex The zero based column index of the cell.
             *
             * \return Returns true if the cell color maps to the default color.  Returns false if the cell color
             *         is cell specific.
             */
            bool cellColorMapsToDefault(unsigned rowIndex, unsigned columnIndex) const;

            /**
             * Method you can use to force the cell color to map to the default color.
             *
             * \param[in] rowIndex    The zero based row index of the cell.
             *
             * \param[in] columnIndex The zero based column index of the cell.
             */
            void setCellColorToDefault(unsigned rowIndex, unsigned columnIndex);

            /**
             * Method you can use to obtain the color being applied to a specific column.
             *
             * \param[in] columnIndex The zero based column index in question.
             *
             * \return Returns the color applied across the column.
             */
            QColor columnColor(unsigned columnIndex) const;

            /**
             * Method you can use to set the color being applied to a specific column.
             *
             * \param[in] columnIndex The zero based column index in question.
             *
             * \param[in] columnColor The color to apply to call cells in this column.
             */
            void setColumnColor(unsigned columnIndex, const QColor& columnColor);

            /**
             * Method you can use to determine if the column color maps to the default.
             *
             * \param[in] columnIndex The zero based column index of the column.
             *
             * \return Returns true if the column color maps to the default color.  Returns false if the column color
             *         is column specific.
             */
            bool columnColorMapsToDefault(unsigned columnIndex) const;

            /**
             * Method you can use to force the column color to map to the default color.
             *
             * \param[in] columnIndex The zero based column index of the column.
             */
            void setColumnColorToDefault(unsigned columnIndex);

            /**
             * Method you can use to obtain the color being applied to a specific row.
             *
             * \param[in] rowIndex The zero based row index in question.
             *
             * \return Returns the color applied across the row.
             */
            QColor rowColor(unsigned rowIndex) const;

            /**
             * Method you can use to set the color being applied to a specific row.
             *
             * \param[in] rowIndex The zero based row index in question.
             *
             * \param[in] rowColor The color to apply to call cells in this row.
             */
            void setRowColor(unsigned rowIndex, const QColor& rowColor);

            /**
             * Method you can use to determine if the row color maps to the default.
             *
             * \param[in] rowIndex The zero based row index of the row.
             *
             * \return Returns true if the row color maps to the default color.  Returns false if the row color
             *         is row specific.
             */
            bool rowColorMapsToDefault(unsigned rowIndex) const;

            /**
             * Method you can use to force the row color to map to the default color.
             *
             * \param[in] rowIndex The zero based row index of the row.
             */
            void setRowColorToDefault(unsigned rowIndex);

            /**
             * Method you can use to obtain the default color applied to every cell.
             *
             * \return Returns the default color applied to each cell.
             */
            QColor defaultColor() const;

            /**
             * Method you can use to set the default color being applied to every cell.
             *
             * \param[in] defaultColor The color to apply to call cells in this default.
             */
            void setDefaultColor(const QColor& defaultColor);

            /**
             * Method you can use to obtain the left cell gutter.
             *
             * \return returns the current left cell gutter value, in points.
             */
            float leftGutter() const;

            /**
             * Method you can use to set the left cell gutter.
             *
             * \param[in] newGutterValue The new gutter value, in points.
             */
            void setLeftGutter(float newGutterValue);

            /**
             * Method you can use to obtain the right cell gutter.
             *
             * \return returns the current right cell gutter value, in points.
             */
            float rightGutter() const;

            /**
             * Method you can use to set the right cell gutter.
             *
             * \param[in] newGutterValue The new gutter value, in points.
             */
            void setRightGutter(float newGutterValue);

            /**
             * Method you can use to obtain the top cell gutter.
             *
             * \return returns the current top cell gutter value, in points.
             */
            float topGutter() const;

            /**
             * Method you can use to set the top cell gutter.
             *
             * \param[in] newGutterValue The new gutter value, in points.
             */
            void setTopGutter(float newGutterValue);

            /**
             * Method you can use to obtain the bottom cell gutter.
             *
             * \return returns the current bottom cell gutter value, in points.
             */
            float bottomGutter() const;

            /**
             * Method you can use to set the bottom cell gutter.
             *
             * \param[in] newGutterValue The new gutter value, in points.
             */
            void setBottomGutter(float newGutterValue);

            /**
             * Method you can use adjust parameters when a column is deleted.
             *
             * \param[in] columnNumber The zero based column number to be deleted.
             */
            void columnRemoved(unsigned columnNumber);

            /**
             * Method you can use to adjust parameters when a row is deleted.
             *
             * \param[in] rowNumber The zero based row number to be deleted.
             */
            void rowRemoved(unsigned rowNumber);

            /**
             * Method you can use to adjust parameters after one or more columns have been inserted into the table.
             *
             * \param[in] columnNumber The column number of the left-most newly inserted column.
             *
             * \param[in] numberColumns The number of columns that were inserted.
             */
            void columnsInserted(unsigned columnNumber, unsigned numberColumns = 1);

            /**
             * Method you can use to adjust parameters after one or more rows have been inserted.
             *
             * \param[in] rowNumber  The row number of the top-most newly inserted row.
             *
             * \param[in] numberRows The number of rows to be added.
             */
            void rowsInserted(unsigned rowNumber, unsigned numberRows = 1);

            /**
             * Method you can use to adjust parameters when a cell is merged.
             *
             * \param[in] rowIndex                The row of the left-most cell to merge.
             *
             * \param[in] columnIndex             The column of the left-most cell to merge.
             *
             * \param[in] numberCellsToMergeRight The number of additional cells to the right of this cell to be merged
             *                                    in.
             *
             * \param[in] numberCellsToMergeDown  The number of additional cells below this cell to be merged in.
             */
            void mergeCells(
                unsigned rowIndex,
                unsigned columnIndex,
                unsigned numberCellsToMergeRight,
                unsigned numberCellsToMergeDown
            );

            /**
             * Method that returns a description of this format as a string.  The string format should be in the form of
             * a comma separated group of fields with the first field being the type-name of the format.
             *
             * While not always supported, the string should be such that the format can be constructed later from the
             * string.
             *
             * \return Returns a description of this format as a string.
             */
            QString toString() const override;

            /**
             * Method that returns CSS content that can be used to emulate this format.  The returned string contains
             * no newline and does not include leading or trailing braces.
             *
             * \return Returns CSS content comparable to this format.
             */
            QString toCss() const override;

            /**
             * Method you can call to generate a CSS entry for this format.  You can optionally overload this class
             * if you wish to modify how CSS entries are created.  Output is suitable for direct use in CSS style
             * sheets.
             *
             * This version will include an additional CSS entry with the :before pseudo-class to define the correct
             * bullet character and font information.
             *
             * \param[in] className The class name to be used.  The class name will be omitted if an empty string is
             *                      provided.
             *
             * \param[in] tag       An optional tag to apply to the CSS entry.  The tag will be omitted if an empty
             *                      string is provided.
             *
             * \param[in] id        An optional ID to apply to the CSS entry.  The ID will be omitted if an empty
             *                      string is provided.
             *
             * \return Returns a string suitable for use as a CSS entry.
             */
            QString toCssEntry(
                const QString& className,
                const QString& tag = QString(),
                const QString& id = QString()
            ) const override;

        protected:
            /**
             * Method you can optionally overload to add additional attributes to the XML description of this element.
             * This method should write attributes for this class only and not the class's children.  To cleanly handle
             * multiple virtual inheritance, the \ref Ld::Format::writeAddAttributes method should be coded to call the
             * appropriate child class methods before calling this method.
             *
             * \param[in]     attributes  A reference to the list of attributes to be modified.
             *
             * \param[in]     formats     A \ref FormatOrganizer instance used to track all known formats in the
             *                            program.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             */
            void writeAddImmediateAttributes(
                XmlAttributes&                  attributes,
                QSharedPointer<FormatOrganizer> formats,
                ProgramFile&                    programFile
            ) const override;

            /**
             * Method you can optionally overload to add additional attributes to the XML description of this element.
             * The default implementation simply returns without modifying the attribute list.
             *
             * \param[in]     attributes  A reference to the list of attributes to be modified.
             *
             * \param[in]     formats     A \ref FormatOrganizer instance used to track all known formats in the
             *                            program.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             */
            void writeAddAttributes(
                XmlAttributes&                  attributes,
                QSharedPointer<FormatOrganizer> formats,
                ProgramFile&                    programFile
            ) const override;

            /**
             * Method that writes column data as child elements for this format.
             *
             * \param[in]     writer              The writer used to write the XML description of the program.
             *
             * \param[in]     formats             A \ref FormatOrganizer instance used to track all known formats in
             *                                    the program.
             *
             * \param[in,out] programFile         The program file instance that can be used to store (or retrieve)
             *                                    additional side-band information.
             *
             * \param[in]     inheritedAttributes Attributes provided by the calling class to this class.  Provided
             *                                    attributes should be provided with child's XML element.  This
             *                                    parameter exists to allow the parent to include positional
             *                                    information as attributes of each child.
             */
            void writeChildren(
                QSharedPointer<XmlWriter>       writer,
                QSharedPointer<FormatOrganizer> formats,
                ProgramFile&                    programFile,
                const XmlAttributes&            inheritedAttributes
            ) const override;

            /**
             * Method you can overload to parse incoming XML attributes.  This method will only be called if there are
             * provided attributes.  To cleanly handle multiple virtual inheritance, the
             * \ref Ld::Format::readAttributes method should be coded to call the appropriate child class methods
             * before calling this method.
             *
             * Errors are reported directly to the reader using the raiseError method.
             *
             * \param[in]     reader      The reader used to parse the incoming XML stream.
             *
             * \param[in]     attributes  The XML attributes to be parsed.
             *
             * \param[in]     formats     A map of format instances by format identifier.  You can use this parameter
             *                            to either tie a format to an object or add a new format to the list of known
             *                            formats.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             *
             * \param[in]     xmlVersion  The XML major version code.  The value can be used to modify how the incoming
             *                            XML stream is parsed.
             */
            void readImmediateAttributes(
                QSharedPointer<XmlReader>  reader,
                const XmlAttributes&       attributes,
                const FormatsByIdentifier& formats,
                ProgramFile&               programFile,
                unsigned                   xmlVersion
            ) override;

            /**
             * Method you can overload to parse incoming XML attributes.  This method will only be called if there are
             * provided attributes.
             *
             * Errors are reported directly to the reader using the raiseError method.
             *
             * \param[in]     reader      The reader used to parse the incoming XML stream.
             *
             * \param[in]     attributes  The XML attributes to be parsed.
             *
             * \param[in]     formats     A map of format instances by format identifier.  You can use this parameter
             *                            to either tie a format to an object or add a new format to the list of known
             *                            formats.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             *
             * \param[in]     xmlVersion  The XML major version code.  The value can be used to modify how the incoming
             *                            XML stream is parsed.
             */
            void readAttributes(
                QSharedPointer<XmlReader>  reader,
                const XmlAttributes&       attributes,
                const FormatsByIdentifier& formats,
                ProgramFile&               programFile,
                unsigned                   xmlVersion
            ) override;

            /**
             * Method that you can use to read and process a child XML tag.  The method is called right after the XML
             * tag is detected in the XML stream.
             *
             * Errors can be reported directly to the reader using the raiseError method.
             *
             * \param[in]     reader      The reader used to parse the incoming XML stream.
             *
             * \param[in]     tagName     The name of the spotted XML tag.
             *
             * \param[in]     formats     A map of format instances by format identifier.  You can use this parameter
             *                            to either tie a format to an object or add a new format to the list of known
             *                            formats.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             *
             * \param[in]     attributes  Attributes tied to the XML start tag.
             *
             * \param[in]     xmlVersion  The XML major version code.  The value can be used to modify how the incoming
             *                            XML stream is parsed.
             */
            void readChild(
                QSharedPointer<XmlReader>  reader,
                const QString&             tagName,
                const FormatsByIdentifier& formats,
                ProgramFile&               programFile,
                const XmlAttributes&       attributes,
                unsigned                   xmlVersion
            ) override;

        private:
            /**
             * Method that converts a \ref Ld::TableColumnWidth value to a string.
             *
             * \param[in] tableColumnWidth The table column width to convert.
             *
             * \return Returns the table column width value as a string.
             */
            static QString toString(const TableColumnWidth& tableColumnWidth);

            /**
             * Method that convers a \ref Ld::TableLineSettings value to a string.
             *
             * \param[in] tableLineSetting The table line setting to be converted.
             *
             * \return Returns the table line setting value as a string.
             */
            static QString toString(const TableLineSettings& tableLineSetting);

            /**
             * Method that converts the column width type to a string.
             *
             * \param[in] columnWidthType The column width type to convert.
             *
             * \return Returns a string represention of the column width type.
             */
            static QString toString(TableColumnWidth::WidthType columnWidthType);

            /**
             * Method that converts the line style to a string.
             *
             * \param[in] lineStyle The line style to be converted to a string.
             *
             * \return Returns a string represention of the column width type.
             */
            static QString toString(TableLineSettings::Style lineStyle);

            /**
             * Method that converts a string to a column width type.
             *
             * \param[in] text The text to convert.
             *
             * \param[in] ok   An optional pointer to a boolean value that will hold true if the conversion was
             *                 successful.
             *
             * \return Returns the decoded column width type.
             */
            static TableColumnWidth::WidthType toColumnWidthType(const QString& text, bool* ok = nullptr);

            /**
             * Method that converts a string to a line style type.
             *
             * \param[in] text The text to convert.
             *
             * \param[in] ok   An optional pointer to a boolean value that will hold true if the conversion was
             *                 successful.
             *
             * \return Returns the decoded column width type.
             */
            static TableLineSettings::Style toLineStyle(const QString& text, bool* ok = nullptr);

            /**
             * The current default column width.
             */
            TableColumnWidth currentDefaultColumnWidth;

            /**
             * Map of non-default table column widths, by column.
             */
            WidthsByColumn currentTableColumnWidths;

            /**
             * The current default column line setting.
             */
            TableLineSettings currentDefaultColumnLineSetting;

            /**
             * Map of non-default column line settings by line number.
             */
            LineSettingsByIndex currentColumnLineSettings;

            /**
             * The current default row line setting.
             */
            TableLineSettings currentDefaultRowLineSetting;

            /**
             * Map of non-default row line settings by line number.
             */
            LineSettingsByIndex currentRowLineSettings;

            /**
             * The current default cell color.
             */
            QColor currentDefaultColor;

            /**
             * The current map of cell colors, by cell position.
             */
            ColorByPosition currentCellColorsByPosition;

            /**
             * The current map of cell colors by row.
             */
            ColorByRowColumnIndex currentCellColorByRow;

            /**
             * The current map of cell colors by column.
             */
            ColorByRowColumnIndex currentCellColorByColumn;

            /**
             * The current left gutter value.
             */
            float currentLeftGutter;

            /**
             * The current right gutter value.
             */
            float currentRightGutter;

            /**
             * The current top gutter value.
             */
            float currentTopGutter;

            /**
             * The current bottom gutter value.
             */
            float currentBottomGutter;
    };

    /**
     * Function that calculates a hash for a map of \ref Ld::TableFrameFormat::CellPosition class instances.
     *
     * \param[in] key  The key to calculate a hash for.
     *
     * \param[in] seed An optional seed used to add entropy to the hash.
     */
    LD_PUBLIC_API Util::HashResult qHash(const TableFrameFormat::CellPosition& key, Util::HashSeed seed = 0);

    /**
     * Function that calculates a hash for a map of \ref Ld::TableColumnWidth class instances.
     *
     * \param[in] key  The key to calculate a hash for.
     *
     * \param[in] seed An optional seed used to add entropy to the hash.
     */
    LD_PUBLIC_API Util::HashResult qHash(const TableFrameFormat::WidthsByColumn& key, Util::HashSeed seed = 0);

    /**
     * Function that calculates a hash for a map of \ref Ld::TableLineSettings class instances.
     *
     * \param[in] key  The key to calculate a hash for.
     *
     * \param[in] seed An optional seed used to add entropy to the hash.
     */
    LD_PUBLIC_API Util::HashResult qHash(const TableFrameFormat::LineSettingsByIndex& key, Util::HashSeed seed = 0);

    /**
     * Function that calculates a hash for a map of colors by \ref Ld::TableFrameFormat::CellPosition class instances.
     *
     * \param[in] key  The key to calculate a hash for.
     *
     * \param[in] seed An optional seed used to add entropy to the hash.
     */
    LD_PUBLIC_API Util::HashResult qHash(const TableFrameFormat::ColorByPosition& key, Util::HashSeed seed = 0);
}

/**
 * Function that calculates a hash for a map of QColor class instances.
 *
 * \param[in] key  The key to calculate a hash for.
 *
 * \param[in] seed An optional seed used to add entropy to the hash.
 */
LD_PUBLIC_API Util::HashResult qHash(const Ld::TableFrameFormat::ColorByRowColumnIndex& key, Util::HashSeed seed = 0);

#endif
