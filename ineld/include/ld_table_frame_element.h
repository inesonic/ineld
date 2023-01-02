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
* This header defines the \ref Ld::TableFrameElement class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_TABLE_FRAME_ELEMENT_H
#define LD_TABLE_FRAME_ELEMENT_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QSharedPointer>
#include <QMap>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_data_type.h"
#include "ld_capabilities.h"
#include "ld_element_with_grouped_children.h"

namespace Ld {
    class TableFrameVisual;
    class TableFrameFormat;

    /**
     * Class that encapsulates a table containing one or more cells.  Children are organized in groups which can be
     * accessed either by group number or by row/column.
     */
    class LD_PUBLIC_API TableFrameElement:public ElementWithGroupedChildren  {
        Q_DECLARE_TR_FUNCTIONS(TableFrameElement)

        public:
            /**
             * Class you can use to represent a cell position.
             */
            class LD_PUBLIC_API CellPosition {
                public:
                    CellPosition();

                    /**
                     * Constructor
                     *
                     * \param[in] rowIndex    The zero based row index.
                     *
                     * \param[in] columnIndex The zero based column index.
                     */
                    CellPosition(unsigned rowIndex, unsigned columnIndex);

                    /**
                     * Copy constructor
                     *
                     * \param[in] other The instance to be copied.
                     */
                    CellPosition(const CellPosition& other);

                    ~CellPosition();

                    /**
                     * Method you can use to determine if this position is valid.
                     *
                     * \return Returns true if the position is valid.  Returns false if the position is invalid.
                     */
                    bool isValid() const;

                    /**
                     * Method you can use to determine if this position is invalid.
                     *
                     * \return Returns true if the position is invalid.  Returns false if the position is valid.
                     */
                    bool isInvalid() const;

                    /**
                     * Method you can use to obtain the row index.
                     *
                     * \return Returns the row index.
                     */
                    unsigned rowIndex() const;

                    /**
                     * Method you can use to set the row index.
                     *
                     * \param[in] newRowIndex The new row index.
                     */
                    void setRowIndex(unsigned newRowIndex);

                    /**
                     * Method you can use to obtain the column index.
                     *
                     * \return Returns the column index.
                     */
                    unsigned columnIndex() const;

                    /**
                     * Method you can use to set the column index.
                     *
                     * \param[in] newColumnIndex The new column index.
                     */
                    void setColumnIndex(unsigned newColumnIndex);

                    /**
                     * Assignment operator.
                     *
                     * \param[in] other The instance to be copied.
                     *
                     * \return Returns a reference to this object.
                     */
                    CellPosition& operator=(const CellPosition& other);

                    /**
                     * Comparison operator
                     *
                     * \param[in] other The instance to be compared against this instance.
                     *
                     * \return Returns true if the two instances are equal.  Returns false if the two instances are
                     *         not equal.
                     */
                    bool operator==(const CellPosition& other) const;

                    /**
                     * Comparison operator
                     *
                     * \param[in] other The instance to be compared against this instance.
                     *
                     * \return Returns true if the two instances are not equal.  Returns false if the two instances are
                     *         equal.
                     */
                    bool operator!=(const CellPosition& other) const;

                    /**
                     * Comparison operator
                     *
                     * \param[in] other The instance to be compared against this instance.
                     *
                     * \return Returns true if this instance precedes the other instance.  Returns false if this
                     *         is equal to or follows the other instance.
                     */
                    bool operator<(const CellPosition& other) const;

                    /**
                     * Comparison operator
                     *
                     * \param[in] other The instance to be compared against this instance.
                     *
                     * \return Returns true if this instance follows the other instance.  Returns false if this
                     *         is equal to or precedes the other instance.
                     */
                    bool operator>(const CellPosition& other) const;

                    /**
                     * Comparison operator
                     *
                     * \param[in] other The instance to be compared against this instance.
                     *
                     * \return Returns true if this instance precedes or is equal to the other instance.  Returns false
                     *         if this instance follows the other instance.
                     */
                    bool operator<=(const CellPosition& other) const;

                    /**
                     * Comparison operator
                     *
                     * \param[in] other The instance to be compared against this instance.
                     *
                     * \return Returns true if this instance follows or equals the other instance.  Returns false if
                     *         this instance precedes the other instance.
                     */
                    bool operator>=(const CellPosition& other) const;

                private:
                    /**
                     * The current row index.
                     */
                    unsigned currentRow;

                    /**
                     * The current column index.
                     */
                    unsigned currentColumn;
            };

            /**
             * Class you can use to represent data on a cell.
             */
            class LD_PUBLIC_API CellData {
                friend class TableFrameElement;

                private:
                    /**
                     * Constructor
                     *
                     * \param[in] rowSpan     The row span for this cell.
                     *
                     * \param[in] columnSpan  The column span for this cell.
                     *
                     * \param[in] groupIndex  The group index for this cell.
                     *
                     * \param[in] children    An optional list of children in this cell.
                     */
                    CellData(
                        unsigned           rowSpan,
                        unsigned           columnSpan,
                        unsigned           groupIndex,
                        ElementPointerList children = ElementPointerList()
                    );

                public:
                    CellData();

                    /**
                     * Copy constructor
                     *
                     * \param[in] other The instance to be copied.
                     */
                    CellData(const CellData& other);

                    ~CellData();

                    /**
                     * Method you can use to obtain the row span for the cell.
                     *
                     * \return Returns the row span for the cell.
                     */
                    unsigned rowSpan() const;

                    /**
                     * Method you can use to obtain the column span for the cell.
                     *
                     * \return Returns the column span for the cell.
                     */
                    unsigned columnSpan() const;

                    /**
                     * Method you can use to obtain the group index for this cell.
                     *
                     * \return Returns the group index.
                     */
                    unsigned groupIndex() const;

                    /**
                     * Method you can use to obtain the list of children.
                     *
                     * \return Returns the list of children.
                     */
                    const ElementPointerList& children() const;

                    /**
                     * Assignment operator.
                     *
                     * \param[in] other The instance to be copied.
                     *
                     * \return Returns a reference to this object.
                     */
                    CellData& operator=(const CellData& other);

                private:
                    /**
                     * The current row span.
                     */
                    unsigned currentRowSpan;

                    /**
                     * The current column span.
                     */
                    unsigned currentColumnSpan;

                    /**
                     * The current group index.
                     */
                    unsigned currentGroupIndex;

                    /**
                     * The current child list.
                     */
                    ElementPointerList currentChildren;
            };

            /**
             * Type used to represent a map of cell data fields by cell position.
             */
            typedef QMap<CellPosition, CellData> CellDataByPosition;

            /**
             * The typename for the paragraph element.
             */
            static const QString elementName;

            /**
             * Static value indicating the capabilities provides by this element.
             */
            static const Capabilities childProvides;

            /**
             * Static value used to indicate and invalid row.
             */
            static const unsigned invalidRow;

            /**
             * Static value used to indicate and invalid column.
             */
            static const unsigned invalidColumn;

            TableFrameElement();

            ~TableFrameElement() override;

            /**
             * Factory method that can be used to create a default instance of a paragraph element.
             *
             * \param[in] typeName The name used to reference the creator function.
             *
             * \return Returns a newly created, default, paragraph element instance.
             */
            static Element* creator(const QString& typeName);

            /**
             * Method you can use to create a clone of this element.  This version will create a clone by:
             *
             * \param[in] includeChildren If true, clones of every child will be included in generated clone.  If
             *                            false, children will be excluded from the clone.  This parameter is included
             *                            for compatibility and is always ignored.
             *
             * \return Returns a new instance of this element with clones of each child included.  Note that a null
             *         pointer may be returned in the unlikely event of an error.
             */
            ElementPointer clone(bool includeChildren) const override;

            /**
             * Method you can use to create a clone of this element based on a starting and ending cursor position.
             * This version is intended to be used to clone elements containing text in one or more regions and will
             * create a clone by:
             *
             * \param[in] startingTextIndex   The starting text index of the first specified region to include in the
             *                                clone.
             *
             * \param[in] startingRegionIndex The startng text region to include in the clone.
             *
             * \param[in] endingTextIndex     The ending text index of the last specified region to include in the
             *                                clone.  A value of \ref Ld::Element::invalidTextIndex indicates all the
             *                                text in the ending region should be included.
             *
             * \param[in] endingRegionIndex   The ending text region to include in the clone.  A value of
             *                                \ref Ld::Element::invalidRegionIndex indicates the close should include
             *                                the last region.
             *
             * \param[in] includeChildren     If true, clones of every child will be included in generated clone.  If
             *                                false, children will be excluded from the clone.
             *
             * \return Returns a new instance of the element.  The clone may optionally include children if this
             *         element supports children.  A null pointer will be returned if the specified ranges are invalid.
             */
            ElementPointer clone(
                unsigned long        startingTextIndex,
                unsigned             startingRegionIndex,
                unsigned long        endingTextIndex,
                unsigned             endingRegionIndex,
                bool                 includeChildren
            ) const override;

            /**
             * Method that returns the type name associated with this object.
             *
             * \return Returns the typename associated with this object.  The value must match the type used to invoke
             *         the static \ref Element::create and \ref Element::registerCreator methods.
             */
            QString typeName() const override;

            /**
             * Method that returns the name of the plug-in that provides this element.
             *
             * \return Returns the name of the plug-in that provides this element. The name must match the name provided
             *         in the plug-in structures.  Elements defined as part of the ineld library should return an empty
             *         string.
             */
            QString plugInName() const override;

            /**
             * Method that returns a description of this element.
             *
             * \return Returns a description of this element suitable for use in a GUI.
             */
            QString description() const override;

            /**
             * Method you can use to determine the value type this element represents.  Elements typically either
             * either report a fixed value or calculate the value based on their children.
             *
             * \return Returns the value type this element would represent.  A value of ValueType::NONE indicates
             *         either that the element does not return a value or the value type could not be determined at
             *         this time.
             */
            DataType::ValueType valueType() const override;

            /**
             * Method you can call to determine the requirements imposed by this element on a specific child element.
             * Note that no testing is performed by elements to confirm that requirements are met.  The information is
             * expected to be used by the user interface.
             *
             * \param[in] childIndex The zero based child index.
             *
             * \return Returns the requirements imposed by this parent on the child element.
             */
            Capabilities parentRequires(unsigned long childIndex) const override;

            /**
             * Method you can call to determine what capabilities this element will provide.  Note that this object
             * also provides a static constant value named "childProvides" returning the same information.
             *
             * \return Returns the requirements met by this element.
             */
            Capabilities childProvidesCapabilities() const override;

            /**
             * Method you can use to change the object used to manage visual representation of this element.  Note that
             * using this method will also cause the \ref TableVisual::element method to point back to this root element
             * instance.
             *
             * If the newly added \ref TableVisual already points to a different element, then those connections will be
             * broken in order to maintain a 1:1 relationship between any \ref Visual instance and \ref Element
             * instance.
             *
             * If this element currently has an associated visual, then that visual will disconnected from this
             * element and will be deleted.
             *
             * \param[in] newVisual A pointer to the new visual object.
             */
            void setVisual(TableFrameVisual* newVisual);

            /**
             * Method you can use to obtain the visual representation of this element.
             *
             * \return Returns a pointer to the visual representation of this element.  A null pointer is returned if
             *         no visual representation has been defined.
             */
            TableFrameVisual* visual() const;

            /**
             * Method you can use to tie a format to this element used when rendering the element in the visual.
             *
             * \param[in] newFormat A shared pointer to the format to tie to this element.
             */
            void setFormat(QSharedPointer<TableFrameFormat> newFormat);

            /**
             * Convenience method you can use to tie a format to this element used when rendering the element in the
             * visual.
             *
             * \param[in] newFormat The format to tie to this element.  Note that the element will assign the format to
             *                      a shared pointer, thus taking shared ownership of it.
             */
            void setFormat(TableFrameFormat* newFormat);

            /**
             * Method you can use to obtain the current format tied to this element.
             *
             * \return Returns a shared pointer to the desired format.
             */
            QSharedPointer<TableFrameFormat> format() const;

            /**
             * Method that indicates if the \ref Ld::Cursor can place the cursor on this element.
             *
             * \return Returns the allowed cursor park mode.
             */
            CursorParkMode cursorParkMode() const override;

            /**
             * Method you can use to determine if this element supports image export capability.
             *
             * \return Returns the export image capability for this element.  This implementatin returns
             *         \ref Ld::Element::ExportImageCapability::NONE.
             */
            ExportImageCapability exportImageCapability() const override;

            /**
             * Method you can use to determine the number of columns in the table.
             *
             * \return Returns the current number of columns.
             */
            unsigned numberColumns() const;

            /**
             * Method you can use to determine the number of rows in the table.
             *
             * \return Returns the current number of rows.
             */
            unsigned numberRows() const;

            /**
             * Method you can use to obtain the number of children in a given cell.
             *
             * \param[in] rowIndex The zero based row index of the cell.
             *
             * \param[in] columnIndex The zero based column index of the cell.
             *
             * \return Returns the number of children occupying the cell at the specified location.  An invalid cell
             *         location will return a value of \ref Ld::Element::invalidChildIndex.
             */
            unsigned long numberChildrenInCell(unsigned rowIndex, unsigned columnIndex) const;

            /**
             * Method you can use to obtain a given child in a given cell.
             *
             * \param[in] rowIndex         The zero based row index of the cell.
             *
             * \param[in] columnIndex      The zero based column index of the cell.
             *
             * \param[in] childIndexInCell The zero based child index in the specified cell.
             *
             * \return Returns the number of children occupying the cell at the specified location.  A null pointer
             *         is returned if the location is invalid.
             */
            ElementPointer childInCell(unsigned rowIndex, unsigned columnIndex, unsigned childIndexInCell) const;

            /**
             * Method you can use to obtain all the children in a given cell.
             *
             * \param[in] rowIndex    The zero based row index of the cell.
             *
             * \param[in] columnIndex The zero based column index of the cell.
             *
             * \return Returns an ordered list of child element pointers.  An empty list is returned if the location is
             *         invalid.
             */
            ElementPointerList childrenInCell(unsigned rowIndex, unsigned columnIndex) const;

            /**
             * Method that returns the group at a specific row/column.
             *
             * \param[in] rowIndex    The zero based row index.
             *
             * \param[in] columnIndex The zero based column index.
             *
             * \return Returns the zero based group at the requested position.  An invalid group number is returned if
             *         the location is invalid.
             */
            unsigned groupAt(unsigned rowIndex, unsigned columnIndex) const;

            /**
             * Method you can use to delete a single column.
             *
             * \param[in]     columnNumber          The zero based column number to be deleted.
             *
             * \param[in]     updateFormat          If true, the format will be updated with the operation.  If false,
             *                                      the format will be left unchanged by this operation.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             *
             * \return Returns true on success, returns false if the indicated column is the only remaining column.
             */
            bool removeColumn(unsigned columnNumber, bool updateFormat, CursorStateCollection* cursorStateCollection);

            /**
             * Method you can use to delete a single row.
             *
             * \param[in]     rowNumber             The zero based row number to be deleted.
             *
             * \param[in]     updateFormat          If true, the format will be updated with the operation.  If false,
             *                                      the format will be left unchanged by this operation.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             *
             * \return Returns true on success, returns false if the indicated column is the only remaining column.
             */
            bool removeRow(unsigned rowNumber, bool updateFormat, CursorStateCollection* cursorStateCollection);

            /**
             * Method you can use to insert one or more columns into the table.
             *
             * \param[in] columnNumber  The column to follow the newly inserted columns.  Specifying an invalid column
             *                          number will cause the newly added columns to be appended to the right side of
             *                          the table.
             *
             * \param[in] numberColumns The number of columns to be added.
             *
             * \param[in] updateFormat  If true, the format will be updated with the operation.  If false, the format
             *                          will be left unchanged by this operation.
             *
             * \param[in] children      A list of children to be cloned and added to each column.
             */
            void insertColumnsBefore(
                unsigned           columnNumber,
                unsigned           numberColumns,
                bool               updateFormat,
                ElementPointerList children = ElementPointerList()
            );

            /**
             * Method you can use to insert one or more columns into the table.
             *
             * \param[in] columnNumber  The column to precede the newly inserted columns.  Specifying an invalid column
             *                          number will cause the newly added columns to be appended to the right side of
             *                          the table.
             *
             * \param[in] numberColumns The number of columns to be added.
             *
             * \param[in] updateFormat  If true, the format will be updated with the operation.  If false, the format
             *                          will be left unchanged by this operation.
             *
             * \param[in] children      A list of children to be cloned and added to each column.
             */
            void insertColumnsAfter(
                unsigned           columnNumber,
                unsigned           numberColumns,
                bool               updateFormat,
                ElementPointerList children = ElementPointerList()
            );

            /**
             * Method you can use to insert one or more rows into the table.
             *
             * \param[in] rowNumber    The row to follow the newly inserted rows.  Specifying an invalid row number
             *                         will cause the newly added rows to be appended to the right side of the table.
             *
             * \param[in] numberRows   The number of rows to be added.
             *
             * \param[in] updateFormat If true, the format will be updated with the operation.  If false, the format
             *                         will be left unchanged by this operation.
             *
             * \param[in] children     A list of children to be cloned and added to each row.
             */
            void insertRowsBefore(
                unsigned           rowNumber,
                unsigned           numberRows,
                bool               updateFormat,
                ElementPointerList children = ElementPointerList()
            );

            /**
             * Method you can use to insert one or more rows into the table.
             *
             * \param[in] rowNumber    The row to precede the newly inserted rows.  Specifying an invalid row number
             *                         will cause the newly added rows to be appended to the right side of the table.
             *
             * \param[in] numberRows   The number of rows to be added.
             *
             * \param[in] updateFormat If true, the format will be updated with the operation.  If false, the format
             *                         will be left unchanged by this operation.
             *
             * \param[in] children     A list of children to be cloned and added to each row.
             */
            void insertRowsAfter(
                unsigned           rowNumber,
                unsigned           numberRows,
                bool               updateFormat,
                ElementPointerList children = ElementPointerList()
            );

            /**
             * Method you can use to determine if the cell at a given position has been merged with other cells.
             *
             * \param[in]     rowIndex    The row index to test.
             *
             * \param[in]     columnIndex The column index to test.
             *
             * \return Returns true if the cell is merged.  Returns false if the cell is not merged.
             */
            bool isMerged(unsigned rowIndex, unsigned columnIndex) const;

            /**
             * Method you can use to merge a cell with other adjascent cells.  Cell contents will be combined in left
             * to right, top to bottom order.
             *
             * \param[in]     rowIndex                The row of the left-most cell to merge.
             *
             * \param[in]     columnIndex             The column of the left-most cell to merge.
             *
             * \param[in]     numberCellsToMergeRight The number of additional cells to the right of this cell to be
             *                                        merged in.
             *
             * \param[in]     numberCellsToMergeDown  The number of additional cells below this cell to be merged in.
             *
             * \param[in]     updateFormat            If true, the format will be updated with the operation.  If false,
             *                                        the format will be left unchanged by this operation.
             *
             * \param[in,out] cursorStateCollection   A pointer to a collection of cursor instances to be adjusted by
             *                                        this method.  No adjustments are performed if a null pointer is
             *                                        provided.
             *
             * \return Returns true on success, returns false on error.
             */
            bool mergeCells(
                unsigned               rowIndex,
                unsigned               columnIndex,
                unsigned               numberCellsToMergeRight,
                unsigned               numberCellsToMergeDown,
                bool                   updateFormat,
                CursorStateCollection* cursorStateCollection
            );

            /**
             * Method you can use to unmerge cells that were previously merged.  The original cell contents will be
             * placed in the top-left most cell.
             *
             * \param[in] rowIndex     The row index of the top-most cell in the merge.
             *
             * \param[in] columnIndex  The column index of the left-most cell in the merge.
             *
             * \param[in] children     A list of children to be cloned and added to each newly added cell.
             *
             * \return Returns true on success, returns false on error.
             */
            bool unmergeCells(unsigned rowIndex, unsigned columnIndex, ElementPointerList children);

            /**
             * Method you can use to obtain cell data by cell position.
             *
             * \param[in] includeChildren If true, children for each cell will be included.
             *
             * \return Returns a map of cell data by cell position.
             */
            CellDataByPosition cellDataByPosition(bool includeChildren = true) const;

            /**
             * Method you can use to obtain the location of an element in the table.
             *
             * \param[in]  child            A shared pointer to the child element to locate.
             *
             * \param[out] childIndexInCell An index that can optionally be populated with the index of the child
             *                              relative to the start of the cell.
             *
             * \return Returns the cell position of the requested child.  An invalid position is returned if the
             *         element is not a child of the table.
             */
            CellPosition cellContainingChild(Ld::ElementPointer child, unsigned long* childIndexInCell = nullptr) const;

            /**
             * Method you can use to locate the top row of a cell.
             *
             * \param[in] rowIndex    The zero based row index.
             *
             * \param[in] columnIndex The zero based columnIndex.
             *
             * \return Returns the top row of a cell. A value of \ref Ld::TableFrameElement::invalidRow is returned
             *         if the cell location is invalid.
             */
            unsigned topRow(unsigned rowIndex, unsigned columnIndex) const;

            /**
             * Method you can use to locate the left column of a cell.
             *
             * \param[in] rowIndex    The zero based row index.
             *
             * \param[in] columnIndex The zero based columnIndex.
             *
             * \return Returns the left column of a cell. A value of \ref Ld::TableFrameElement::invalidColumn is
             *         returned if the cell location is invalid.
             */
            unsigned leftColumn(unsigned rowIndex, unsigned columnIndex) const;

            /**
             * Method you can use to determine the bottom row of a cell.
             *
             * \param[in] rowIndex    The zero based row index.
             *
             * \param[in] columnIndex The zero based columnIndex.
             *
             * \return Returns the bottom row of a cell.  A value of \\ref Ld::TableFrameElement::invalidRow is
             *         returned if the cell location is invalid.
             */
            unsigned bottomRow(unsigned rowIndex, unsigned columnIndex) const;

            /**
             * Method you can use to determine the right column of a cell.
             *
             * \param[in] rowIndex    The zero based row index.
             *
             * \param[in] columnIndex The zero based columnIndex.
             *
             * \return Returns the right column of the specified cell.  The value
             *         \ref Ld::TableFrameElement::invalidColumn is returned if the cell location is invalid.
             */
            unsigned rightColumn(unsigned rowIndex, unsigned columnIndex) const;

            /**
             * Method you can use to identify the location of the first non-empty cell.  Cells are tracked in reading
             * order.
             *
             * \return Returns the cell position of the first non empty cell.  An invalid cell position is returned if
             *         all cells contain content.
             */
            CellPosition firstNonEmptyCell() const;

            /**
             * Method you can use to identify the location of the last non-empty cell. Cells are tracked in reading
             * order.
             *
             * \return Returns the cell position of the last non-empty cell. An invalid cell position is returned if
             *         all cells contain content.
             */
            CellPosition lastNonEmptyCell() const;

            /**
             * Method you can use to identify the location of the first non-empty row.
             *
             * \return Returns the row index of the first non-empty row.  An invalid row index is returned if there
             *         are no non-empty rows.
             */
            unsigned firstNonEmptyRow() const;

            /**
             * Method you can use to identify the location of the last non-empty row.
             *
             * \return Returns the row index of the last non-empty row.
             */
            unsigned lastNonEmptyRow() const;

            /**
             * Method you can use to identify the location of the first non-empty column.
             *
             * \return Returns the column index of the first non-empty column.  An invalid column index is returned if
             *         there are no empty columns.
             */
            unsigned firstNonEmptyColumn() const;

            /**
             * Method you can use to identify the location of the last non-empty column.
             *
             * \return Returns the column index of the last non-empty column.  An invalid column index is returned if
             *         there are no empty columns.
             */
            unsigned lastNonEmptyColumn() const;

            /**
             * Method that identifies the first empty cell in the table.
             *
             * \return Returns the position of the first empty cell.
             */
            CellPosition firstEmptyCell() const;

        protected:
            /**
             * Method you can optionally overload to add additional attributes to the XML description of this element.
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
             * Method that writes child elements to the program file.
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
             * Method that reads and processes the XML description of a child, inserting the child into the element
             * tree.
             *
             * Errors are reported directly to the reader using the raiseError method.
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
             * Method that is used to remove children from unused groups.  This method does not remove the groups, only
             * the children from the groups.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             */
            void purgeChildrenFromUnusedGroups(CursorStateCollection* cursorStateCollection);

            /**
             * Method that is used to remove currently unused groups from the table.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             */
            void reorderGroups(CursorStateCollection* cursorStateCollection);

            /**
             * Method that clones children into a range of groups.
             *
             * \param[in] startingGroupIndex The starting group index.
             *
             * \param[in] numberGroups       The number of groups to receive the cloned children.
             *
             * \param[in] children           The children to be cloned into the groups.
             */
            void addClonesToGroups(unsigned startingGroupIndex, unsigned numberGroups, ElementPointerList children);

            /**
             * Method that clones an entire list of children.
             *
             * \param[in] elements A list of elements to be cloned.
             *
             * \return Returns a list of clones.
             */
            static ElementPointerList cloneChildren(const ElementPointerList& elements);

            /**
             * The current number of columns in this table.
             */
            unsigned currentNumberColumns;

            /**
             * List used to facilitate translation of grid positions into group numbers.
             *
             * The index is calculated by rowNumber * currentNumberColumns + columnNumber;
             */
            QList<unsigned> groupForCell;
    };
};

#endif
