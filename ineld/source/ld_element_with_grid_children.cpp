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
* This file implements the \ref Ld::ElementWithGroupedChildren class.
***********************************************************************************************************************/

#include <QList>
#include <QSharedPointer>

#include "ld_element_structures.h"
#include "ld_format_organizer.h"
#include "ld_xml_writer.h"
#include "ld_xml_attributes.h"
#include "ld_program_file.h"
#include "ld_cursor_state_collection.h"
#include "ld_visual_with_grid_children.h"
#include "ld_element_with_fixed_children.h"
#include "ld_element_with_grid_children.h"

namespace Ld {
    const unsigned long ElementWithGridChildren::defaultNumberRows    = 1;
    const unsigned long ElementWithGridChildren::defaultNumberColumns = 1;

    ElementWithGridChildren::ElementWithGridChildren() {
        currentNumberRows    = defaultNumberRows;
        currentNumberColumns = defaultNumberColumns;

        setNumberChildren(currentNumberRows * currentNumberColumns, nullptr);
    }


    ElementWithGridChildren::~ElementWithGridChildren() {}


    Element::ChildPlacement ElementWithGridChildren::childPlacement() const {
        return Element::ChildPlacement::GRID;
    }


    void ElementWithGridChildren::setVisual(VisualWithGridChildren* newVisual) {
        ElementWithFixedChildren::setVisual(newVisual);
    }


    VisualWithGridChildren* ElementWithGridChildren::visual() const {
        return dynamic_cast<VisualWithGridChildren*>(ElementWithFixedChildren::visual());
    }


    unsigned long ElementWithGridChildren::numberRows() const {
        return currentNumberRows;
    }


    unsigned long ElementWithGridChildren::numberColumns() const {
        return currentNumberColumns;
    }


    void ElementWithGridChildren::setNumberRows(
            unsigned long          newNumberRows,
            CursorStateCollection* cursorStateCollection
        ) {
        assert(numberChildren() == currentNumberRows * currentNumberColumns);

        unsigned long oldNumberRows = currentNumberRows;

        currentNumberRows = newNumberRows;
        setNumberChildren(currentNumberColumns * newNumberRows, cursorStateCollection);

        VisualWithGridChildren* currentVisual = visual();
        if (currentVisual != nullptr) {
            currentVisual->geometryChanged(
                oldNumberRows,
                currentNumberColumns,
                newNumberRows,
                currentNumberColumns
            );
        }
    }


    void ElementWithGridChildren::setNumberColumns(
            unsigned long          newNumberColumns,
            CursorStateCollection* cursorStateCollection
        ) {
        assert(numberChildren() == currentNumberRows * currentNumberColumns);

        unsigned long oldNumberColumns = currentNumberColumns;

        if (currentNumberColumns > 0) {
            if (newNumberColumns > currentNumberColumns) {
                // Adding columns

                setNumberChildren(currentNumberRows * newNumberColumns, nullptr);
                for (unsigned long rowIndex=currentNumberRows-1 ; rowIndex > 0 ; --rowIndex) {
                    unsigned long columnIndex = currentNumberColumns;
                    do {
                        --columnIndex;

                        unsigned oldChildIndex = rowIndex * currentNumberColumns + columnIndex;
                        unsigned newChildIndex = rowIndex * newNumberColumns + columnIndex;

                        moveChild(oldChildIndex, newChildIndex, cursorStateCollection);
                    } while (columnIndex > 0);
                }
            } else if (newNumberColumns < currentNumberColumns) {
                // Removing columns

                for (unsigned long rowIndex=1 ; rowIndex<currentNumberRows ; ++rowIndex) {
                    for (unsigned long columnIndex=0 ; columnIndex<newNumberColumns ; ++columnIndex) {
                        unsigned oldChildIndex = rowIndex * currentNumberColumns + columnIndex;
                        unsigned newChildIndex = rowIndex * newNumberColumns + columnIndex;

                        moveChild(oldChildIndex, newChildIndex, cursorStateCollection);
                    }
                }

                setNumberChildren(currentNumberRows * newNumberColumns, nullptr);
            }
        } else {
            // Nothing to relocate.  Simply set the new size.

            setNumberChildren(currentNumberRows * newNumberColumns, nullptr);
        }

        currentNumberColumns = newNumberColumns;

        VisualWithGridChildren* currentVisual = visual();
        if (currentVisual != nullptr) {
            currentVisual->geometryChanged(
                currentNumberRows,
                oldNumberColumns,
                currentNumberRows,
                newNumberColumns
            );
        }
    }


    bool ElementWithGridChildren::removeChildAt(
            unsigned long          rowIndex,
            unsigned long          columnIndex,
            CursorStateCollection* cursorStateCollection
        ) {
        unsigned long childIndex = rowIndex * currentNumberColumns + columnIndex;
        return ElementWithFixedChildren::removeChild(childIndex, cursorStateCollection);
    }


    void ElementWithGridChildren::insertRowBefore(
            unsigned long          rowIndex,
            CursorStateCollection* cursorStateCollection,
            ElementPointer         defaultChild
        ) {
        assert(numberChildren() == currentNumberRows * currentNumberColumns);

        if (currentNumberColumns > 0) {
            unsigned long newNumberChildren = numberChildren() + currentNumberColumns;
            setNumberChildren(newNumberChildren, nullptr);

            if (currentNumberRows > 0 && rowIndex < currentNumberRows) {
                unsigned long moveRowIndex = currentNumberRows;
                do {
                    --moveRowIndex;

                    unsigned long oldChildIndex = moveRowIndex * currentNumberColumns;
                    unsigned long newChildIndex = oldChildIndex + currentNumberColumns;

                    for (unsigned long columnIndex=0 ; columnIndex<currentNumberColumns ; ++columnIndex) {
                        moveChild(oldChildIndex, newChildIndex, cursorStateCollection);
                        ++oldChildIndex;
                        ++newChildIndex;
                    }
                } while (moveRowIndex > rowIndex);
            }

            if (rowIndex > currentNumberRows) {
                rowIndex = currentNumberRows;
            }

            if (!defaultChild.isNull()) {
                unsigned long childIndex = currentNumberColumns * rowIndex;
                for (unsigned long columnIndex=0 ; columnIndex<currentNumberColumns ; ++ columnIndex) {
                    setChild(childIndex, defaultChild->clone(true), nullptr);
                    ++childIndex;
                }
            }
        }

        Ld::VisualWithGridChildren* currentVisual = visual();
        if (currentVisual != nullptr) {
            currentVisual->rowInserted(rowIndex);
        }

        ++currentNumberRows;
    }


    void ElementWithGridChildren::insertRowAfter(
            unsigned long          rowIndex,
            CursorStateCollection* cursorStateCollection,
            ElementPointer         defaultChild
        ) {
        insertRowBefore(rowIndex + 1, cursorStateCollection, defaultChild);
    }


    void ElementWithGridChildren::insertColumnBefore(
            unsigned long          columnIndex,
            CursorStateCollection* cursorStateCollection,
            ElementPointer         defaultChild
        ) {
        assert(numberChildren() == currentNumberRows * currentNumberColumns);

        unsigned newNumberColumns = currentNumberColumns + 1;

        if (currentNumberRows > 0) {
            unsigned long newNumberChildren = currentNumberRows * newNumberColumns;
            setNumberChildren(newNumberChildren, nullptr);

            if (currentNumberColumns > 0) {
                unsigned long moveRowIndex = currentNumberRows;
                do {
                    --moveRowIndex;

                    unsigned long moveColumnIndex = currentNumberColumns;
                    do {
                        --moveColumnIndex;
                        unsigned long oldChildIndex = moveRowIndex * currentNumberColumns + moveColumnIndex;
                        unsigned long newChildIndex;
                        if (moveColumnIndex >= columnIndex) {
                            newChildIndex = moveRowIndex * newNumberColumns + moveColumnIndex + 1;
                        } else {
                            newChildIndex = moveRowIndex * newNumberColumns + moveColumnIndex;
                        }

                        moveChild(oldChildIndex, newChildIndex, cursorStateCollection);
                    } while (moveColumnIndex > 0);
                } while (moveRowIndex > 0);
            }

            if (columnIndex > currentNumberColumns) {
                columnIndex = currentNumberColumns;
            }

            if (!defaultChild.isNull()) {
                unsigned long childIndex = columnIndex;
                for (unsigned long rowIndex=0 ; rowIndex<currentNumberRows ; ++ rowIndex) {
                    setChild(childIndex, defaultChild->clone(true), nullptr);
                    childIndex += newNumberColumns;
                }
            }
        }

        Ld::VisualWithGridChildren* currentVisual = visual();
        if (currentVisual != nullptr) {
            currentVisual->columnInserted(columnIndex);
        }

        currentNumberColumns = newNumberColumns;
    }


    void ElementWithGridChildren::insertColumnAfter(
            unsigned long          columnIndex,
            CursorStateCollection* cursorStateCollection,
            ElementPointer         defaultChild
        ) {
        insertColumnBefore(columnIndex + 1, cursorStateCollection, defaultChild);
    }


    bool ElementWithGridChildren::removeRow(unsigned long rowIndex, CursorStateCollection* cursorStateCollection) {
        assert(numberChildren() == currentNumberRows * currentNumberColumns);

        bool success;

        if (rowIndex < currentNumberRows) {
            Ld::VisualWithGridChildren* currentVisual = visual();
            if (currentVisual != nullptr) {
                currentVisual->aboutToRemoveRow(rowIndex);
            }

            unsigned long newChildIndex         = currentNumberColumns * rowIndex;
            unsigned long oldChildIndex         = newChildIndex + currentNumberColumns;
            unsigned long currentNumberChildren = numberChildren();
            while (oldChildIndex < currentNumberChildren) {
                moveChild(oldChildIndex, newChildIndex, cursorStateCollection);
                ++oldChildIndex;
                ++newChildIndex;
            }

            unsigned long newNumberRows = currentNumberRows - 1;
            setNumberRows(newNumberRows, rowIndex == newNumberRows ? cursorStateCollection : nullptr);

            success = true;
        } else {
            success = false;
        }

        return success;
    }


    bool ElementWithGridChildren::removeColumn(
            unsigned long          columnIndex,
            CursorStateCollection* cursorStateCollection
        ) {
        assert(numberChildren() == currentNumberRows * currentNumberColumns);

        bool success;

        if (columnIndex < currentNumberColumns) {
            Ld::VisualWithGridChildren* currentVisual = visual();
            if (currentVisual != nullptr) {
                currentVisual->aboutToRemoveColumn(columnIndex);
            }

            if ((columnIndex + 1) < currentNumberColumns) {
                for (unsigned long moveRow=0 ; moveRow<currentNumberRows ; ++moveRow) {
                    for (unsigned long moveColumn=columnIndex+1 ; moveColumn<currentNumberColumns ; ++moveColumn) {
                        unsigned long oldChildIndex = moveRow * currentNumberColumns + moveColumn;
                        unsigned long newChildIndex = oldChildIndex - 1;

                        moveChild(oldChildIndex, newChildIndex, cursorStateCollection);
                    }
                }

                setNumberColumns(currentNumberColumns - 1, nullptr);
            } else {
                setNumberColumns(currentNumberColumns - 1, cursorStateCollection);
            }

            success = true;
        } else {
            success = false;
        }

        return success;
    }


    bool ElementWithGridChildren::setChild(
            unsigned long          rowIndex,
            unsigned long          columnIndex,
            ElementPointer         childElement,
            CursorStateCollection* cursorStateCollection
        ) {
        unsigned long childIndex = rowIndex * currentNumberColumns + columnIndex;
        return setChild(childIndex, childElement, cursorStateCollection);
    }


    ElementPointer ElementWithGridChildren::child(unsigned long rowIndex, unsigned long columnIndex) const {
        unsigned long childIndex = rowIndex * currentNumberColumns + columnIndex;
        return child(childIndex);
    }


    unsigned long ElementWithGridChildren::rowIndexOfChild(unsigned long index) const {
        unsigned long result;

        if (index < numberChildren()) {
            result = index / currentNumberColumns;
        } else {
            result = invalidChildIndex;
        }

        return result;
    }


    unsigned long ElementWithGridChildren::rowIndexOfChild(ElementPointer child) const {
        return rowIndexOfChild(indexOfChild(child));
    }


    unsigned long ElementWithGridChildren::columnIndexOfChild(unsigned long index) const {
        unsigned long result;

        if (index < numberChildren()) {
            result = index % currentNumberColumns;
        } else {
            result = invalidChildIndex;
        }

        return result;
    }


    unsigned long ElementWithGridChildren::columnIndexOfChild(ElementPointer child) const {
        return columnIndexOfChild(indexOfChild(child));
    }


    void ElementWithGridChildren::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        ElementWithFixedChildren::writeAddAttributes(attributes, formats, programFile);
        if (currentNumberRows != defaultNumberRows) {
            attributes.append("number_rows", currentNumberRows);
        }

        if (currentNumberColumns != defaultNumberColumns) {
            attributes.append("number_columns", currentNumberColumns);
        }
    }


    void ElementWithGridChildren::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        ElementWithFixedChildren::readAttributes(reader, attributes, formats, programFile, xmlVersion);

        if (attributes.hasAttribute("number_rows")) {
            bool          ok;
            unsigned long newNumberRows = attributes.value<unsigned long>("number_rows", &ok);
            if (!ok) {
                QString tagName = reader->qualifiedName().toString();
                QString value  = attributes.value<QString>("number_rows");
                reader->raiseError(tr("Tag \"%1\", invalid number rows \"%1\"").arg(tagName, value));
            } else {
                currentNumberRows = newNumberRows;
            }
        } else {
            currentNumberRows = defaultNumberRows;
        }

        if (!reader->hasError()) {
            if (attributes.hasAttribute("number_columns")) {
                bool          ok;
                unsigned long newNumberColumns = attributes.value<unsigned long>("number_columns", &ok);
                if (!ok) {
                    QString tagName = reader->qualifiedName().toString();
                    QString value  = attributes.value<QString>("number_columns");
                    reader->raiseError(tr("Tag \"%1\", invalid number columns \"%1\"").arg(tagName, value));
                } else {
                    currentNumberColumns = newNumberColumns;
                }
            } else {
                currentNumberColumns = defaultNumberColumns;
            }
        }

        if (!reader->hasError()) {
            setNumberChildren(currentNumberRows * currentNumberColumns, nullptr);
        }
    }


    bool ElementWithGridChildren::cloneChildren(QSharedPointer<ElementWithFixedChildren> element) const {
        bool success = ElementWithFixedChildren::cloneChildren(element);

        if (success && element->childPlacement() == ChildPlacement::GRID) {
            QSharedPointer<ElementWithGridChildren> gridElement = element.dynamicCast<ElementWithGridChildren>();
            gridElement->currentNumberColumns = currentNumberColumns;
            gridElement->currentNumberRows    = currentNumberRows;
        }

        return success;
    }
}
