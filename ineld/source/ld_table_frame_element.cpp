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
* This file implements the \ref Ld::TableFrameElement class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>

#include <algorithm>

#include <util_bit_array.h>

#include "ld_data_type.h"
#include "ld_table_frame_format.h"
#include "ld_capabilities.h"
#include "ld_xml_element.h"
#include "ld_element.h"
#include "ld_table_frame_visual.h"
#include "ld_element_with_positional_children.h"
#include "ld_element_with_grouped_children.h"
#include "ld_table_frame_element.h"

/***********************************************************************************************************************
 * Ld::TableFrameElement::CellPosition
 */

namespace Ld {
    TableFrameElement::CellPosition::CellPosition() {
        currentRow    = TableFrameElement::invalidRow;
        currentColumn = TableFrameElement::invalidColumn;
    }


    TableFrameElement::CellPosition::CellPosition(unsigned rowIndex, unsigned columnIndex) {
        currentRow    = rowIndex;
        currentColumn = columnIndex;
    }


    TableFrameElement::CellPosition::CellPosition(const TableFrameElement::CellPosition& other) {
        currentRow    = other.currentRow;
        currentColumn = other.currentColumn;
    }


    TableFrameElement::CellPosition::~CellPosition() {}


    bool TableFrameElement::CellPosition::isValid() const {
        return currentRow != TableFrameElement::invalidRow && currentColumn != TableFrameElement::invalidColumn;
    }


    bool TableFrameElement::CellPosition::isInvalid() const {
        return !isValid();
    }


    unsigned TableFrameElement::CellPosition::rowIndex() const {
        return currentRow;
    }


    void TableFrameElement::CellPosition::setRowIndex(unsigned newRowIndex) {
        currentRow = newRowIndex;
    }


    unsigned TableFrameElement::CellPosition::columnIndex() const {
        return currentColumn;
    }


    void TableFrameElement::CellPosition::setColumnIndex(unsigned newColumnIndex) {
        currentColumn = newColumnIndex;
    }


    TableFrameElement::CellPosition& TableFrameElement::CellPosition::operator=(
            const TableFrameElement::CellPosition& other
        ) {
        currentRow    = other.currentRow;
        currentColumn = other.currentColumn;

        return *this;
    }


    bool TableFrameElement::CellPosition::operator==(const TableFrameElement::CellPosition& other) const {
        return other.currentRow == currentRow && other.currentColumn == currentColumn;
    }


    bool TableFrameElement::CellPosition::operator!=(const TableFrameElement::CellPosition& other) const {
        return other.currentRow != currentRow || other.currentColumn != currentColumn;
    }


    bool TableFrameElement::CellPosition::operator<(const TableFrameElement::CellPosition& other) const {
        bool isLess;

        if (currentRow < other.currentRow) {
            isLess = true;
        } else if (currentRow == other.currentRow && currentColumn < other.currentColumn) {
            isLess = true;
        } else {
            isLess = false;
        }

        return isLess;
    }


    bool TableFrameElement::CellPosition::operator>(const TableFrameElement::CellPosition& other) const {
        bool isMore;

        if (currentRow > other.currentRow) {
            isMore = true;
        } else if (currentRow == other.currentRow && currentColumn > other.currentColumn) {
            isMore = true;
        } else {
            isMore = false;
        }

        return isMore;
    }


    bool TableFrameElement::CellPosition::operator<=(const TableFrameElement::CellPosition& other) const {
        bool isLess;

        if (currentRow < other.currentRow) {
            isLess = true;
        } else if (currentRow == other.currentRow && currentColumn <= other.currentColumn) {
            isLess = true;
        } else {
            isLess = false;
        }

        return isLess;
    }


    bool TableFrameElement::CellPosition::operator>=(const TableFrameElement::CellPosition& other) const {
        bool isMore;

        if (currentRow > other.currentRow) {
            isMore = true;
        } else if (currentRow == other.currentRow && currentColumn >= other.currentColumn) {
            isMore = true;
        } else {
            isMore = false;
        }

        return isMore;
    }
}

/***********************************************************************************************************************
 * Ld::TableFrameElement::CellPosition
 */

namespace Ld {
    TableFrameElement::CellData::CellData(
            unsigned           rowSpan,
            unsigned           columnSpan,
            unsigned           groupIndex,
            ElementPointerList children
        ) {
        currentRowSpan    = rowSpan;
        currentColumnSpan = columnSpan;
        currentGroupIndex = groupIndex;
        currentChildren   = children;
    }


    TableFrameElement::CellData::CellData() {
        currentRowSpan    = 0;
        currentColumnSpan = 0;
        currentGroupIndex = invalidGroup;
        currentChildren.clear();
    }


    TableFrameElement::CellData::CellData(const TableFrameElement::CellData& other) {
        currentRowSpan    = other.currentRowSpan;
        currentColumnSpan = other.currentColumnSpan;
        currentGroupIndex = other.currentGroupIndex;
        currentChildren   = other.currentChildren;
    }


    TableFrameElement::CellData::~CellData() {}


    unsigned TableFrameElement::CellData::rowSpan() const {
        return currentRowSpan;
    }


    unsigned TableFrameElement::CellData::columnSpan() const {
        return currentColumnSpan;
    }


    unsigned TableFrameElement::CellData::groupIndex() const {
        return currentGroupIndex;
    }


    const ElementPointerList& TableFrameElement::CellData::children() const {
        return currentChildren;
    }


    TableFrameElement::CellData& TableFrameElement::CellData::operator=(const TableFrameElement::CellData& other) {
        currentRowSpan    = other.currentRowSpan;
        currentColumnSpan = other.currentColumnSpan;
        currentGroupIndex = other.currentGroupIndex;
        currentChildren   = other.currentChildren;

        return *this;
    }
}

/***********************************************************************************************************************
 * Ld::TableFrameElement
 */

namespace Ld {
    const QString      TableFrameElement::elementName("TableFrame");
    const Capabilities TableFrameElement::childProvides = Capabilities::frame;
    const unsigned     TableFrameElement::invalidRow    = static_cast<unsigned>(-1);
    const unsigned     TableFrameElement::invalidColumn = static_cast<unsigned>(-1);

    TableFrameElement::TableFrameElement() {
        currentNumberColumns = 1;

        groupForCell.clear();
        groupForCell.append(0);
    }


    TableFrameElement::~TableFrameElement() {}


    Element* TableFrameElement::creator(const QString&) {
        return new TableFrameElement();
    }


    ElementPointer TableFrameElement::clone(bool includeChildren) const {
        ElementPointer element = ElementWithGroupedChildren::clone(includeChildren);

        QSharedPointer<TableFrameElement> tableElement = element.dynamicCast<TableFrameElement>();
        tableElement->currentNumberColumns = currentNumberColumns;
        tableElement->groupForCell         = groupForCell;

        return element;
    }


    ElementPointer TableFrameElement::clone(
        unsigned long        startingTextIndex,
        unsigned             startingRegionIndex,
        unsigned long        endingTextIndex,
        unsigned             endingRegionIndex,
        bool                 includeChildren
    ) const {
        ElementPointer element = ElementWithGroupedChildren::clone(
            startingTextIndex,
            startingRegionIndex,
            endingTextIndex,
            endingRegionIndex,
            includeChildren
        );

        if (!element.isNull()) {
            QSharedPointer<TableFrameElement> tableElement = element.dynamicCast<TableFrameElement>();
            tableElement->currentNumberColumns = currentNumberColumns;
            tableElement->groupForCell         = groupForCell;
        }

        return element;
    }


    QString TableFrameElement::typeName() const {
        return elementName;
    }


    QString TableFrameElement::plugInName() const {
        return QString();
    }


    QString TableFrameElement::description() const {
        return tr("table");
    }


    DataType::ValueType TableFrameElement::valueType() const {
        return DataType::ValueType::NONE;
    }


    Capabilities TableFrameElement::parentRequires(unsigned long) const {
        return Capabilities::frame;
    }


    Capabilities TableFrameElement::childProvidesCapabilities() const {
        return childProvides;
    }


    void TableFrameElement::setVisual(TableFrameVisual* newVisual) {
        ElementWithGroupedChildren::setVisual(newVisual);
    }


    TableFrameVisual* TableFrameElement::visual() const {
        return dynamic_cast<TableFrameVisual*>(ElementWithGroupedChildren::visual());
    }


    void TableFrameElement::setFormat(QSharedPointer<TableFrameFormat> newFormat) {
        ElementWithGroupedChildren::setFormat(newFormat);
    }


    void TableFrameElement::setFormat(TableFrameFormat* newFormat) {
        ElementWithGroupedChildren::setFormat(newFormat);
    }


    QSharedPointer<TableFrameFormat> TableFrameElement::format() const {
        return ElementWithGroupedChildren::format().dynamicCast<TableFrameFormat>();
    }


    Element::CursorParkMode TableFrameElement::cursorParkMode() const {
        return CursorParkMode::CAN_NOT_PARK;
    }


    TableFrameElement::ExportImageCapability TableFrameElement::exportImageCapability() const {
        return ExportImageCapability::NONE;
    }


    unsigned TableFrameElement::numberColumns() const {
        return currentNumberColumns;
    }


    unsigned TableFrameElement::numberRows() const {
        return static_cast<unsigned>(groupForCell.size()) / currentNumberColumns;
    }


    unsigned long TableFrameElement::numberChildrenInCell(unsigned rowIndex, unsigned columnIndex) const {
        unsigned long result;

        unsigned groupIndex = groupAt(rowIndex, columnIndex);
        if (groupIndex != invalidGroup) {
            result = numberChildrenInGroup(groupIndex);
        } else {
            result = invalidChildIndex;
        }

        return result;
    }


    ElementPointer TableFrameElement::childInCell(
            unsigned rowIndex,
            unsigned columnIndex,
            unsigned childIndexInCell
        ) const {
        ElementPointer result;

        unsigned groupIndex = groupAt(rowIndex, columnIndex);
        if (groupIndex != invalidGroup) {
            result = childInGroup(groupIndex, childIndexInCell);
        }

        return result;
    }


    ElementPointerList TableFrameElement::childrenInCell(unsigned rowIndex, unsigned columnIndex) const {
        ElementPointerList result;

        unsigned groupIndex = groupAt(rowIndex, columnIndex);
        if (groupIndex != invalidGroup) {
            result = childrenInGroup(groupIndex);
        }

        return result;
    }


    unsigned TableFrameElement::groupAt(unsigned rowIndex, unsigned columnIndex) const {
        unsigned result;

        if (rowIndex != invalidRow && columnIndex != invalidColumn && columnIndex < currentNumberColumns) {
            unsigned cellIndex = rowIndex * currentNumberColumns + columnIndex;
            if (cellIndex < static_cast<unsigned>(groupForCell.size())) {
                result = groupForCell.at(cellIndex);
            } else {
                result = invalidGroup;
            }
        } else {
            result = invalidGroup;
        }

        return result;
    }


    bool TableFrameElement::removeColumn(
            unsigned               columnNumber,
            bool                   updateFormat,
            CursorStateCollection* cursorStateCollection
        ) {
        bool     success;

        if (currentNumberColumns > 1 && (columnNumber != invalidColumn || columnNumber < currentNumberColumns)) {
            if (visual() != nullptr) {
                visual()->aboutToRemoveColumn(columnNumber, updateFormat);
            }

            unsigned cellIndex = columnNumber;
            while (cellIndex < static_cast<unsigned>(groupForCell.size())) {
                groupForCell.removeAt(cellIndex);
                cellIndex += currentNumberColumns - 1;
            }

            purgeChildrenFromUnusedGroups(cursorStateCollection);
            reorderGroups(cursorStateCollection);
            --currentNumberColumns;

            if (updateFormat) {
                QSharedPointer<TableFrameFormat> format = TableFrameElement::format();
                if (!format.isNull()) {
                    format->columnRemoved(columnNumber);
                }
            }

            success = true;

            if (visual() != nullptr) {
                visual()->columnRemoved(columnNumber, updateFormat);
            }
        } else {
            success = false;
        }

        return success;
    }


    bool TableFrameElement::removeRow(
            unsigned               rowNumber,
            bool                   updateFormat,
            CursorStateCollection* cursorStateCollection
        ) {
        bool success;

        if (static_cast<unsigned>(groupForCell.size()) > currentNumberColumns) {
            if (visual() != nullptr) {
                visual()->aboutToRemoveRow(rowNumber, updateFormat);
            }

            unsigned cellIndex = currentNumberColumns * rowNumber;
            if (cellIndex < static_cast<unsigned>(groupForCell.size())) {
                for (unsigned i=0 ; i<currentNumberColumns ; ++i) {
                    groupForCell.removeAt(cellIndex);
                }

                purgeChildrenFromUnusedGroups(cursorStateCollection);
                reorderGroups(cursorStateCollection);

                if (updateFormat) {
                    QSharedPointer<TableFrameFormat> format = TableFrameElement::format();
                    if (!format.isNull()) {
                        format->rowRemoved(rowNumber);
                    }
                }

                success = true;

                if (visual() != nullptr) {
                    visual()->rowRemoved(rowNumber, updateFormat);
                }
            } else {
                success = false;
            }
        } else {
            success = false;
        }

        return success;
    }


    void TableFrameElement::insertColumnsBefore(
            unsigned           columnNumber,
            unsigned           numberColumns,
            bool               updateFormat,
            ElementPointerList children
        ) {
        if (columnNumber == invalidColumn || columnNumber > currentNumberColumns) {
            columnNumber = currentNumberColumns;
        }

        QList<unsigned> newGroupForCell;
        unsigned        numberRows = TableFrameElement::numberRows();

        for (unsigned rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
            unsigned sourceColumnIndex = 0;
            unsigned leftSiblingGroup  = invalidGroup;
            while (sourceColumnIndex < columnNumber) {
                leftSiblingGroup = groupAt(rowIndex, sourceColumnIndex);
                newGroupForCell.append(leftSiblingGroup);
                ++sourceColumnIndex;
            }

            unsigned rightSiblingGroup;
            if (columnNumber < currentNumberColumns) {
                rightSiblingGroup = groupAt(rowIndex, columnNumber);
            } else {
                rightSiblingGroup = invalidGroup;
            }

            if (leftSiblingGroup == invalidGroup || leftSiblingGroup != rightSiblingGroup) {
                for (unsigned newColumnNumber=0 ; newColumnNumber<numberColumns ; ++newColumnNumber) {
                    newGroupForCell.append(invalidGroup);
                }
            } else {
                for (unsigned newColumnNumber=0 ; newColumnNumber<numberColumns ; ++newColumnNumber) {
                    newGroupForCell.append(leftSiblingGroup);
                }
            }

            while (sourceColumnIndex < currentNumberColumns) {
                unsigned groupIndex = groupAt(rowIndex, sourceColumnIndex);
                newGroupForCell.append(groupIndex);
                ++sourceColumnIndex;
            }
        }

        QMap<unsigned, unsigned> remappedGroups;
        unsigned                 newGroupIndex = 0;

        unsigned numberCellIndexes = static_cast<unsigned>(newGroupForCell.size());
        for (unsigned cellIndex=0 ; cellIndex<numberCellIndexes ; ++cellIndex) {
            unsigned replacementGroupIndex;
            unsigned oldGroupIndex = newGroupForCell.at(cellIndex);
            if (oldGroupIndex == invalidGroup) {
                insertGroupsBefore(newGroupIndex, 1);
                addClonesToGroups(newGroupIndex, 1, children);

                replacementGroupIndex = newGroupIndex;
                ++newGroupIndex;
            } else if (remappedGroups.contains(oldGroupIndex)) {
                replacementGroupIndex = remappedGroups.value(oldGroupIndex);
            } else {
                replacementGroupIndex = newGroupIndex;
                remappedGroups.insert(oldGroupIndex, newGroupIndex);
                ++newGroupIndex;
            }

            newGroupForCell[cellIndex] = replacementGroupIndex;
        }

        groupForCell = newGroupForCell;
        currentNumberColumns += numberColumns;

        if (updateFormat) {
            QSharedPointer<TableFrameFormat> format = TableFrameElement::format();
            if (!format.isNull()) {
                format->columnsInserted(columnNumber, numberColumns);
            }
        }

        if (visual() != nullptr) {
            visual()->columnsInserted(columnNumber, numberColumns, updateFormat);
        }
    }


    void TableFrameElement::insertColumnsAfter(
            unsigned           columnNumber,
            unsigned           numberColumns,
            bool               updateFormat,
            ElementPointerList children
        ) {
        insertColumnsBefore(columnNumber + 1, numberColumns, updateFormat, children);
    }


    void TableFrameElement::insertRowsBefore(
            unsigned           rowNumber,
            unsigned           numberRows,
            bool               updateFormat,
            ElementPointerList children
        ) {
        unsigned numberTableRows = TableFrameElement::numberRows();
        if (rowNumber >= numberTableRows) {
            unsigned groupIndex   = *std::max_element(groupForCell.begin(), groupForCell.end()) + 1;
            unsigned numberGroups = numberRows * currentNumberColumns;

            insertGroupsBefore(groupIndex, numberGroups);
            addClonesToGroups(groupIndex, numberGroups, children);

            for (unsigned groupCount=0 ; groupCount<numberGroups ; ++groupCount) {
                groupForCell.append(groupIndex + groupCount);
            }
        } else {
            QList<unsigned> columnGroups;

            if (rowNumber > 0 && rowNumber < (numberTableRows - 1)) {
                for (unsigned columnIndex=0 ; columnIndex<currentNumberColumns ; ++columnIndex) {
                    unsigned groupAbove = groupAt(rowNumber - 1, columnIndex);
                    unsigned groupBelow = groupAt(rowNumber, columnIndex);

                    if (groupAbove == groupBelow) {
                        columnGroups.append(groupAbove);
                    } else {
                        columnGroups.append(invalidGroup);
                    }
                }
            } else {
                for (unsigned columnIndex=0 ; columnIndex<currentNumberColumns ; ++columnIndex) {
                    columnGroups.append(invalidGroup);
                }
            }

            unsigned        insertCellIndex = rowNumber * currentNumberColumns;
            QList<unsigned> newGroupForCell = groupForCell.mid(0, insertCellIndex);
            QList<unsigned> trailingCells   = groupForCell.mid(insertCellIndex);

            unsigned  baseGroupIndex;
            if (newGroupForCell.isEmpty()) {
                baseGroupIndex = 0;
            } else {
                baseGroupIndex = *std::max_element(newGroupForCell.begin(), newGroupForCell.end()) + 1;
            }

            unsigned numberGroupsAdded = 0;

            for (unsigned rowCount=0 ; rowCount<numberRows ; ++rowCount) {
                for (unsigned columnIndex=0 ; columnIndex<currentNumberColumns ; ++columnIndex) {
                    unsigned newGroupIndex = columnGroups[columnIndex];
                    if (newGroupIndex == invalidGroup) {
                        newGroupIndex = baseGroupIndex + numberGroupsAdded;

                        insertGroupsBefore(newGroupIndex, 1);
                        addClonesToGroups(newGroupIndex, 1, children);

                        ++numberGroupsAdded;
                    }

                    newGroupForCell.append(newGroupIndex);
                }
            }

            for (  QList<unsigned>::const_iterator trailingIterator    = trailingCells.constBegin(),
                                                   trailingEndIterator = trailingCells.constEnd()
                 ; trailingIterator != trailingEndIterator
                 ; ++trailingIterator
                ) {
                unsigned trailingGroup = *trailingIterator;
                if (trailingGroup >= baseGroupIndex) {
                    newGroupForCell.append(trailingGroup + numberGroupsAdded);
                } else {
                    newGroupForCell.append(trailingGroup);
                }
            }

            groupForCell = newGroupForCell;
        }

        if (updateFormat) {
            QSharedPointer<TableFrameFormat> format = TableFrameElement::format();
            if (!format.isNull()) {
                format->rowsInserted(rowNumber, numberRows);
            }
        }

        if (visual() != nullptr) {
            visual()->rowsInserted(rowNumber, numberRows, updateFormat);
        }
    }


    void TableFrameElement::insertRowsAfter(
            unsigned           rowNumber,
            unsigned           numberRows,
            bool               updateFormat,
            ElementPointerList children
        ) {
        insertRowsBefore(rowNumber + 1, numberRows, updateFormat, children);
    }


    bool TableFrameElement::isMerged(unsigned rowIndex, unsigned columnIndex) const {
        unsigned numberRows      = TableFrameElement::numberRows();
        unsigned groupAtPosition = groupAt(rowIndex, columnIndex);

        return (
               (columnIndex > 0 && groupAt(rowIndex, columnIndex - 1) == groupAtPosition)
            || (columnIndex < (currentNumberColumns - 1) && groupAt(rowIndex, columnIndex + 1) == groupAtPosition)
            || (rowIndex > 0 && groupAt(rowIndex - 1, columnIndex) == groupAtPosition)
            || (rowIndex < (numberRows - 1) && groupAt(rowIndex + 1, columnIndex) == groupAtPosition)
        );
    }


    bool TableFrameElement::mergeCells(
            unsigned               rowIndex,
            unsigned               columnIndex,
            unsigned               numberCellsToMergeRight,
            unsigned               numberCellsToMergeDown,
            bool                   updateFormat,
            CursorStateCollection* cursorStateCollection
        ) {
        bool              success      = true;
        unsigned          groupIndex   = groupAt(rowIndex, columnIndex);
        unsigned          endingColumn = columnIndex;
        unsigned          endingRow    = rowIndex;
        TableFrameVisual* visual       = TableFrameElement::visual();

        while ((endingColumn + 1) < currentNumberColumns && groupAt(rowIndex, endingColumn + 1) == groupIndex) {
            ++endingColumn;
        }

        if (currentNumberColumns - endingColumn - 1 >= numberCellsToMergeRight) {
            endingColumn += numberCellsToMergeRight;
        } else {
            success = false;
        }

        if (success) {
            unsigned numberRows = TableFrameElement::numberRows();
            while ((endingRow + 1) < numberRows && groupAt(endingRow + 1, columnIndex) == groupIndex) {
                ++endingRow;
            }

            if (numberRows - endingRow - 1 >= numberCellsToMergeDown) {
                endingRow += numberCellsToMergeDown;
            } else {
                success = false;
            }
        }

        if (success) {
            if (visual != nullptr) {
                visual->aboutToMergeCells(
                    rowIndex,
                    columnIndex,
                    numberCellsToMergeRight,
                    numberCellsToMergeDown,
                    updateFormat
                );
            }

            for (unsigned row=rowIndex ; row<=endingRow ; ++row) {
                for (unsigned column=columnIndex ; column<=endingColumn ; ++column) {
                    unsigned cellIndex = row * currentNumberColumns + column;
                    unsigned removedGroupIndex = groupForCell.at(cellIndex);

                    if (removedGroupIndex != groupIndex) {
                        ElementPointerList children = childrenInGroup(removedGroupIndex);

                        ElementPointerList::const_iterator it  = children.constBegin();
                        ElementPointerList::const_iterator end = children.constEnd();

                        while (success && it != end) {
                            ElementPointer element = *it;
                            appendToGroup(groupIndex, element, cursorStateCollection);
                            ++it;
                        }

                        if (success) {
                            groupForCell[cellIndex] = groupIndex;
                        }
                    }
                }
            }

            if (updateFormat) {
                QSharedPointer<TableFrameFormat> format = TableFrameElement::format();
                if (!format.isNull()) {
                    format->mergeCells(rowIndex, columnIndex, numberCellsToMergeRight, numberCellsToMergeDown);
                }
            }

            if (visual != nullptr) {
                visual->cellsMerged(rowIndex, columnIndex, updateFormat);
            }
        }

        if (success) {
            reorderGroups(cursorStateCollection);
        }

        return success;
    }


    bool TableFrameElement::unmergeCells(
            unsigned           rowIndex,
            unsigned           columnIndex,
            ElementPointerList children
        ) {
        bool success;
        if (isMerged(rowIndex, columnIndex)) {
            if (visual() != nullptr) {
                visual()->aboutToUnmergeCells(rowIndex, columnIndex);
            }

            unsigned topRow            = TableFrameElement::topRow(rowIndex, columnIndex);
            unsigned leftColumn        = TableFrameElement::leftColumn(rowIndex, columnIndex);
            unsigned startingCellIndex = topRow * currentNumberColumns + leftColumn;
            unsigned groupIndex        = groupForCell.at(startingCellIndex);

            unsigned cellIndex = startingCellIndex + 1;
            unsigned nextGroup = groupIndex + 1;

            while (cellIndex < static_cast<unsigned>(groupForCell.size())) {
                unsigned oldGroupIndex = groupForCell.at(cellIndex);
                if (oldGroupIndex == groupIndex) {
                    insertGroupsBefore(nextGroup, 1);
                    addClonesToGroups(nextGroup, 1, children);
                }

                groupForCell[cellIndex] = nextGroup;
                ++nextGroup;
                ++cellIndex;
            }

            if (visual() != nullptr) {
                visual()->cellsUnmerged(rowIndex, columnIndex);
            }
            success = true;
        } else {
            success = false;
        }

        return success;
    }


    TableFrameElement::CellDataByPosition TableFrameElement::cellDataByPosition(bool includeChildren) const {
        unsigned numberGroups = TableFrameElement::numberGroups();
        unsigned numberRows   = TableFrameElement::numberRows();

        Util::BitArray includedGroups;
        includedGroups.resize(numberGroups);

        CellDataByPosition result;

        unsigned cellIndex = 0;
        for (unsigned rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
            for (unsigned columnIndex=0 ; columnIndex<currentNumberColumns ; ++columnIndex) {
                unsigned groupIndex = groupForCell.at(cellIndex);

                if (includedGroups.isClear(groupIndex)) {
                    unsigned     rowSpan = bottomRow(rowIndex, columnIndex)   - topRow(rowIndex, columnIndex)     + 1;
                    unsigned     colSpan = rightColumn(rowIndex, columnIndex) - leftColumn(rowIndex, columnIndex) + 1;

                    ElementPointerList children;
                    if (includeChildren) {
                        children = childrenInGroup(groupIndex);
                    }

                    result.insert(
                        CellPosition(rowIndex, columnIndex),
                        CellData(rowSpan, colSpan, groupIndex, children)
                    );

                    includedGroups.setBit(groupIndex);
                }

                ++cellIndex;
            }
        }

        return result;
    }


    TableFrameElement::CellPosition TableFrameElement::cellContainingChild(
            Ld::ElementPointer child,
            unsigned long*     childIndexInCell
        ) const {
        CellPosition result;

        unsigned long childIndex = indexOfChild(child);
        if (childIndex != invalidChildIndex) {
            unsigned groupIndex = groupContainingChild(childIndex, childIndexInCell);
            assert(groupIndex != invalidGroup);

            // Note: A binary search would be problematic here due to merged cells.  For this reason, we stick with a
            //       linear search.  Note that a binary search *could* likely be used to guess an lower bounds to
            //       search from.  For now we'll just keep things simple and stick with a linear algorithm.

            unsigned numberCellIndexes = static_cast<unsigned>(groupForCell.size());
            unsigned cellIndex         = 0;

            while (cellIndex < numberCellIndexes && groupForCell.at(cellIndex) < groupIndex) {
                ++cellIndex;
            }

            if (cellIndex < numberCellIndexes) {
                unsigned rowIndex    = cellIndex / currentNumberColumns;
                unsigned columnIndex = cellIndex % currentNumberColumns;

                result.setRowIndex(rowIndex);
                result.setColumnIndex(columnIndex);
            }
        }

        return result;
    }


    unsigned TableFrameElement::topRow(unsigned rowIndex, unsigned columnIndex) const {
        unsigned result;

        unsigned groupIndex = groupAt(rowIndex, columnIndex);
        if (groupIndex != invalidGroup) {
            if (rowIndex == 0) {
                result = rowIndex;
            } else {
                unsigned previousRowIndex = rowIndex;
                do {
                    result = previousRowIndex;
                    -- previousRowIndex;
                } while (result > 0 && groupAt(previousRowIndex, columnIndex) == groupIndex);
            }
        } else {
            result = invalidRow;
        }

        return result;
    }


    unsigned TableFrameElement::leftColumn(unsigned rowIndex, unsigned columnIndex) const {
        unsigned result;

        unsigned groupIndex = groupAt(rowIndex, columnIndex);
        if (groupIndex != invalidGroup) {
            if (columnIndex == 0) {
                result = columnIndex;
            } else {
                unsigned previousColumnIndex = columnIndex;
                do {
                    result = previousColumnIndex;
                    -- previousColumnIndex;
                } while (result > 0 && groupAt(rowIndex, previousColumnIndex) == groupIndex);
            }
        } else {
            result = invalidColumn;
        }

        return result;
    }


    unsigned TableFrameElement::bottomRow(unsigned rowIndex, unsigned columnIndex) const {
        unsigned result;

        unsigned groupIndex = groupAt(rowIndex, columnIndex);
        if (groupIndex != invalidGroup) {
            unsigned nextRowIndex = rowIndex;
            do {
                result = nextRowIndex;
                ++nextRowIndex;
            } while (groupAt(nextRowIndex, columnIndex) == groupIndex);
        } else {
            result = invalidRow;
        }

        return result;
    }


    unsigned TableFrameElement::rightColumn(unsigned rowIndex, unsigned columnIndex) const {
        unsigned result;

        unsigned groupIndex = groupAt(rowIndex, columnIndex);
        if (groupIndex != invalidGroup) {
            unsigned nextColumnIndex = columnIndex;
            do {
                result = nextColumnIndex;
                ++nextColumnIndex;
            } while (groupAt(rowIndex, nextColumnIndex) == groupIndex);
        } else {
            result = invalidRow;
        }

        return result;
    }


    TableFrameElement::CellPosition TableFrameElement::firstNonEmptyCell() const {
        CellPosition result;

        Util::BitArray processedGroups(numberGroups(), false);
        unsigned       numberRows = TableFrameElement::numberRows();
        unsigned       rowIndex   = 0;

        while (rowIndex < numberRows && result.isInvalid()) {
            unsigned columnIndex = 0;
            while (columnIndex < currentNumberColumns && result.isInvalid()) {
                unsigned groupIndex = groupAt(rowIndex, columnIndex);
                if (processedGroups.isClear(groupIndex)) {
                    processedGroups.setBit(groupIndex);
                    unsigned long numberChildrenInGroup = TableFrameElement::numberChildrenInGroup(groupIndex);
                    if (numberChildrenInGroup != 0) {
                        result.setRowIndex(rowIndex);
                        result.setColumnIndex(columnIndex);
                    }
                }

                ++columnIndex;
            }

            ++rowIndex;
        }

        return result;
    }


    TableFrameElement::CellPosition TableFrameElement::lastNonEmptyCell() const {
        CellPosition result;

        unsigned numberRows = TableFrameElement::numberRows();
        unsigned rowIndex   = numberRows;

        do {
            --rowIndex;
            unsigned columnIndex = currentNumberColumns;
            do {
                columnIndex = leftColumn(rowIndex, columnIndex - 1);

                unsigned      groupIndex            = groupAt(rowIndex, columnIndex);
                unsigned long numberChildrenInGroup = TableFrameElement::numberChildrenInGroup(groupIndex);

                if (numberChildrenInGroup != 0 && rowIndex == topRow(rowIndex, columnIndex)) {
                    result.setRowIndex(rowIndex);
                    result.setColumnIndex(columnIndex);
                }
            } while (columnIndex > 0 && result.isInvalid());
        } while (rowIndex > 0 && result.isInvalid());

        return result;
    }


    unsigned TableFrameElement::firstNonEmptyRow() const {
        return firstNonEmptyCell().rowIndex();
    }


    unsigned TableFrameElement::lastNonEmptyRow() const {
        return lastNonEmptyCell().rowIndex();
    }


    unsigned TableFrameElement::firstNonEmptyColumn() const {
        unsigned result = invalidColumn;

        Util::BitArray processedGroups(numberGroups(), false);
        unsigned       numberRows  = TableFrameElement::numberRows();
        unsigned       columnIndex = 0;

        while (columnIndex < currentNumberColumns && result == invalidColumn) {
            unsigned rowIndex = 0;
            while (rowIndex < numberRows && result == invalidColumn) {
                unsigned groupIndex = groupAt(rowIndex, columnIndex);
                if (processedGroups.isClear(groupIndex)) {
                    processedGroups.setBit(groupIndex);
                    unsigned long numberChildrenInGroup = TableFrameElement::numberChildrenInGroup(groupIndex);
                    if (numberChildrenInGroup != 0) {
                        result = columnIndex;
                    }
                }

                ++rowIndex;
            }

            ++columnIndex;
        }

        return result;
    }


    unsigned TableFrameElement::lastNonEmptyColumn() const {
        unsigned result = invalidColumn;

        unsigned numberRows  = TableFrameElement::numberRows();
        unsigned columnIndex = currentNumberColumns;

        do {
            --columnIndex;

            unsigned rowIndex = numberRows;
            do {
                rowIndex = topRow(rowIndex - 1, columnIndex);
                unsigned      groupIndex            = groupAt(rowIndex, columnIndex);
                unsigned long numberChildrenInGroup = TableFrameElement::numberChildrenInGroup(groupIndex);
                if (numberChildrenInGroup != 0 && columnIndex == leftColumn(rowIndex, columnIndex)) {
                    result = columnIndex;
                }
            } while (rowIndex > 0 && result == invalidColumn);
        } while (columnIndex > 0 && result == invalidColumn);

        return result;
    }


    TableFrameElement::CellPosition TableFrameElement::firstEmptyCell() const {
        CellPosition result;

        unsigned numberRows = TableFrameElement::numberRows();
        unsigned rowIndex   = 0;
        while (rowIndex < numberRows && result.isInvalid()) {
            unsigned columnIndex = 0;
            while (columnIndex < currentNumberColumns && result.isInvalid()) {
                unsigned      groupIndex            = groupAt(rowIndex, columnIndex);
                unsigned long numberChildrenInGroup = TableFrameElement::numberChildrenInGroup(groupIndex);
                if (numberChildrenInGroup == 0) {
                    result.setRowIndex(rowIndex);
                    result.setColumnIndex(columnIndex);
                }

                ++columnIndex;
            }

            ++rowIndex;
        }

        return result;
    }


    void TableFrameElement::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        attributes.append("number_rows", numberRows());
        attributes.append("number_columns", currentNumberColumns);

        ElementWithPositionalChildren::writeAddAttributes(attributes, formats, programFile);
    }


    void TableFrameElement::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        unsigned newNumberColumns = invalidColumn;
        unsigned newNumberRows    = invalidRow;

        ElementWithPositionalChildren::readAttributes(reader, attributes, formats, programFile, xmlVersion);

        if (!reader->hasError()) {
            if (attributes.hasAttribute("number_columns")) {
                bool ok;
                newNumberColumns = attributes.value<unsigned>("number_columns", &ok);
                if (!ok || newNumberColumns == 0) {
                    QString tagName = reader->qualifiedName().toString();
                    QString value   = attributes.value<QString>("number_columns");

                    reader->raiseError(tr("Tag \"%1\" has invalid number columns \"%1\"").arg(tagName).arg(value));
                }
            } else {
                QString tagName = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\" is missing number_columns attribute").arg(tagName));
            }
        }

        if (!reader->hasError()) {
            if (attributes.hasAttribute("number_rows")) {
                bool ok;
                newNumberRows = attributes.value<unsigned>("number_rows", &ok);
                if (!ok || newNumberRows == 0) {
                    QString tagName = reader->qualifiedName().toString();
                    QString value   = attributes.value<QString>("number_rows");

                    reader->raiseError(tr("Tag \"%1\" has invalid number rows \"%1\"").arg(tagName).arg(value));
                }
            } else {
                QString tagName = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\" is missing number_rows attribute").arg(tagName));
            }
        }

        if (!reader->hasError()) {
            currentNumberColumns = newNumberColumns;
            groupForCell.clear();
            unsigned numberCellLocations = newNumberColumns * newNumberRows;
            for (unsigned index=0; index<numberCellLocations ; ++index) {
                groupForCell.append(invalidGroup);
            }
        }
    }


    void TableFrameElement::writeChildren(
            QSharedPointer<XmlWriter>       writer,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile,
            const XmlAttributes&            inheritedAttributes
        ) const {
        unsigned numberGroups = TableFrameElement::numberGroups();
        if (numberGroups > 0) {
            Util::BitArray processedGroups;
            processedGroups.resize(numberGroups);
            processedGroups.clearBits(0, numberGroups - 1);

            unsigned numberRows = TableFrameElement::numberRows();
            for (unsigned rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                for (unsigned columnIndex=0 ; columnIndex<currentNumberColumns ; ++columnIndex) {
                    unsigned groupIndex = groupAt(rowIndex, columnIndex);

                    if (processedGroups.isClear(groupIndex)) {
                        processedGroups.setBit(groupIndex);

                        unsigned topRowIndex      = topRow(rowIndex, columnIndex);
                        unsigned bottomRowIndex   = bottomRow(rowIndex, columnIndex);
                        unsigned leftColumnIndex  = leftColumn(rowIndex, columnIndex);
                        unsigned rightColumnIndex = rightColumn(rowIndex, columnIndex);

                        writer->writeStartElement("TableCell");

                        XmlAttributes cellAttributes;
                        cellAttributes.append("row", topRowIndex);
                        cellAttributes.append("column", leftColumnIndex);

                        if (bottomRowIndex != topRowIndex) {
                            cellAttributes.append("row_span", bottomRowIndex - topRowIndex + 1);
                        }

                        if (rightColumnIndex != leftColumnIndex) {
                            cellAttributes.append("column_span", rightColumnIndex - leftColumnIndex + 1);
                        }

                        writer->writeAttributes(cellAttributes);

                        ElementPointerList children       = ElementWithGroupedChildren::childrenInGroup(groupIndex);
                        unsigned           numberChildren = static_cast<unsigned>(children.size());
                        for (unsigned long childIndex=0 ; childIndex<numberChildren; ++childIndex) {
                            ElementPointer childElement = children.at(childIndex);

                            XmlAttributes childAttributes = inheritedAttributes;
                            childAttributes.append("index", childIndex);

                            childElement->writeXml(writer, formats, programFile, childAttributes);
                        }

                        writer->writeEndElement();
                    }
                }
            }
        }
    }


    void TableFrameElement::readChild(
            QSharedPointer<XmlReader>  reader,
            const QString&             tagName,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            const XmlAttributes&       attributes,
            unsigned                   xmlVersion
        ) {
        if (tagName != "TableCell") {
            reader->raiseError(tr("Tag \"%1\" unexpected tag.").arg(tagName));
        } else {
            unsigned topRowIndex      = invalidRow;
            unsigned bottomRowIndex   = invalidRow;
            unsigned leftColumnIndex  = invalidColumn;
            unsigned rightColumnIndex = invalidColumn;

            unsigned numberRows    = TableFrameElement::numberRows();
            unsigned numberColumns = TableFrameElement::numberColumns();

            if (attributes.hasAttribute("row")) {
                bool ok;
                topRowIndex = attributes.value<unsigned>("row", &ok);
                if (!ok || topRowIndex >= numberRows) {
                    QString value = attributes.value<QString>("row");
                    reader->raiseError(tr("Tag \"%1\", invalid row \"%1\"").arg(tagName).arg(value));
                }
            } else {
                reader->raiseError(tr("Tag \"%1\", missing row attribute").arg(tagName));
            }

            if (attributes.hasAttribute("column")) {
                bool ok;
                leftColumnIndex = attributes.value<unsigned>("column", &ok);
                if (!ok || leftColumnIndex >= numberColumns) {
                    QString value = attributes.value<QString>("column");
                    reader->raiseError(tr("Tag \"%1\", invalid column \"%1\"").arg(tagName).arg(value));
                }
            } else {
                reader->raiseError(tr("Tag \"%1\", missing column attribute").arg(tagName));
            }


            if (attributes.hasAttribute("row_span")) {
                bool     ok;
                unsigned rowSpan = attributes.value<unsigned>("row_span", &ok);
                if (!ok || rowSpan == 0 || topRowIndex + rowSpan > numberRows) {
                    QString value = attributes.value<QString>("row_span");
                    reader->raiseError(tr("Tag \"%1\", invalid row span \"%1\"").arg(tagName).arg(value));
                } else {
                    bottomRowIndex = topRowIndex + rowSpan - 1;
                }
            } else {
                bottomRowIndex = topRowIndex;
            }

            if (attributes.hasAttribute("column_span")) {
                bool     ok;
                unsigned columnSpan = attributes.value<unsigned>("column_span", &ok);
                if (!ok || columnSpan == 0 || leftColumnIndex + columnSpan > numberColumns) {
                    QString value = attributes.value<QString>("column_span");
                    reader->raiseError(tr("Tag \"%1\", invalid column span \"%1\"").arg(tagName).arg(value));
                } else {
                    rightColumnIndex = leftColumnIndex + columnSpan - 1;
                }
            } else {
                rightColumnIndex = leftColumnIndex;
            }

            unsigned newGroupIndex;
            if (topRowIndex == 0 && leftColumnIndex == 0) {
                newGroupIndex = 0;
            } else {
                newGroupIndex = numberGroups();
                insertGroupsAfter(newGroupIndex, 1);
            }

            unsigned rowIndex = topRowIndex;
            while (!reader->hasError() && rowIndex <= bottomRowIndex) {
                unsigned columnIndex = leftColumnIndex;
                while (!reader->hasError() && columnIndex <= rightColumnIndex) {
                    unsigned cellIndex = rowIndex * currentNumberColumns + columnIndex;
                    assert(cellIndex < static_cast<unsigned>(groupForCell.size()));

                    if (groupForCell[cellIndex] != invalidGroup) {
                        reader->raiseError(
                            tr("Tag \"%1\", overlapping cells at %2,%3, cell %4,%5 - %6,%7")
                            .arg(tagName)
                            .arg(rowIndex)
                            .arg(columnIndex)
                            .arg(topRowIndex)
                            .arg(leftColumnIndex)
                            .arg(bottomRowIndex)
                            .arg(rightColumnIndex)
                        );
                    } else {
                        groupForCell[cellIndex] = newGroupIndex;
                        ++columnIndex;
                    }
                }

                ++rowIndex;
            }

            if (!reader->atEnd() && !reader->hasError()) {
                XmlReader::TokenType tokenType;

                do {
                    tokenType = reader->readNext();

                    if (tokenType == XmlReader::Characters) {
                        QString pcData = reader->text().toString();
                        readData(reader, pcData, formats, programFile, xmlVersion);
                    } else if (tokenType == XmlReader::StartElement) {
                        QString       childToken      = reader->qualifiedName().toString();
                        XmlAttributes childAttributes = reader->attributes();
                        ElementPointer newChild = create(childToken);
                        if (newChild.isNull()) {
                            reader->raiseError(tr("Tag \"%1\" invalid name").arg(childToken));
                        } else {
                            newChild->readXml(reader, formats, programFile, childAttributes, xmlVersion);

                            if (!reader->hasError()) {
                                appendToGroup(newGroupIndex, newChild, nullptr);
                            }
                        }
                    } else if (tokenType == XmlReader::EndElement) {
                        QString endTag = reader->qualifiedName().toString();
                        if (endTag != "TableCell") {
                            reader->raiseError(tr("Tag \"%1\", unexpected end").arg(endTag));
                        }
                    } else {
                        reader->raiseError(tr("Invalid primitive"));
                    }
                } while (!reader->hasError() && tokenType != XmlReader::EndElement);
            }
        }
    }


    void TableFrameElement::purgeChildrenFromUnusedGroups(CursorStateCollection* cursorStateCollection) {
        unsigned numberGroups = TableFrameElement::numberGroups();
        Util::BitArray assignedGroups;
        assignedGroups.resize(numberGroups);

        unsigned numberCells = static_cast<unsigned>(groupForCell.size());
        for (unsigned cellIndex=0 ; cellIndex<numberCells ; ++cellIndex) {
            unsigned groupIndex = groupForCell.at(cellIndex);
            assignedGroups.setBit(groupIndex);
        }

        Util::BitArray::Index nextPurgeableGroup = assignedGroups.firstClearedBit();
        while (nextPurgeableGroup != Util::BitArray::invalidIndex) {
            unsigned groupIndex = static_cast<unsigned>(nextPurgeableGroup);
            removeChildrenFromGroup(groupIndex, cursorStateCollection);
            nextPurgeableGroup = assignedGroups.firstClearedBit(nextPurgeableGroup + 1);
        }
    }


    void TableFrameElement::reorderGroups(CursorStateCollection* cursorStateCollection) {
        QMap<unsigned, unsigned>           remappedGroups;
        QMap<unsigned, ElementPointerList> childrenByGroup;
        unsigned                           numberCellIndexes = static_cast<unsigned>(groupForCell.size());

        for (unsigned cellIndex=0 ; cellIndex<numberCellIndexes ; ++cellIndex) {
            unsigned oldGroupIndex = groupForCell.at(cellIndex);
            if (!remappedGroups.contains(oldGroupIndex)) {
                unsigned newGroupIndex = static_cast<unsigned>(remappedGroups.size());
                remappedGroups.insert(oldGroupIndex, newGroupIndex);
                if (newGroupIndex != oldGroupIndex) {
                    ElementPointerList children = childrenInGroup(oldGroupIndex);
                    childrenByGroup.insert(newGroupIndex, children);
                }
            }
        }

        for (unsigned cellIndex=0 ; cellIndex<numberCellIndexes ; ++cellIndex) {
            unsigned oldGroupIndex = groupForCell.at(cellIndex);
            unsigned newGroupIndex = remappedGroups.value(oldGroupIndex);

            if (oldGroupIndex != newGroupIndex) {
                groupForCell[cellIndex] = newGroupIndex;
            }
        }

        for (  QMap<unsigned, ElementPointerList>::const_iterator childrenIterator    = childrenByGroup.constBegin(),
                                                                  childrenEndIterator = childrenByGroup.constEnd()
             ; childrenIterator != childrenEndIterator
             ; ++childrenIterator
            ) {
            unsigned                  newGroupIndex           = childrenIterator.key();
            const ElementPointerList& children                = childrenIterator.value();
            unsigned                  numberChildrenThisGroup = static_cast<unsigned>(children.size());

            for (unsigned childIndexInGroup=0 ; childIndexInGroup<numberChildrenThisGroup ; ++childIndexInGroup) {
                ElementPointer child = children.at(childIndexInGroup);
                appendToGroup(newGroupIndex, child, nullptr);
            }
        }

        unsigned requiredNumberGroups = static_cast<unsigned>(remappedGroups.size());
        if (requiredNumberGroups < numberGroups()) {
            removeGroups(requiredNumberGroups, numberGroups() - requiredNumberGroups, cursorStateCollection);
        }
    }


    void TableFrameElement::addClonesToGroups(
            unsigned           startingGroupIndex,
            unsigned           numberGroups,
            ElementPointerList children
        ) {
        for (unsigned groupOffset=0 ; groupOffset<numberGroups ; ++groupOffset) {
            unsigned groupIndex = startingGroupIndex + groupOffset;
            for (ElementPointerList::const_iterator it=children.constBegin(),end=children.constEnd() ; it!=end ; ++it) {
                ElementPointer element = *it;
                ElementPointer cloned  = element->clone(true);

                insertIntoGroupAfter(groupIndex, invalidChildIndex, cloned, nullptr);
            }
        }
    }


    ElementPointerList TableFrameElement::cloneChildren(const ElementPointerList& elements) {
        ElementPointerList result;
        for (ElementPointerList::const_iterator it=elements.constBegin(),end=elements.constEnd() ; it!=end ; ++it) {
            ElementPointer element = *it;
            result << element->clone(true);
        }

        return result;
    }
}
