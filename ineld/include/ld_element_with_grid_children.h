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
* This header defines the \ref Ld::ElementWithGridChildren class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_ELEMENT_WITH_GRID_CHILDREN_H
#define LD_ELEMENT_WITH_GRID_CHILDREN_H

#include <QList>
#include <QSharedPointer>
#include <QList>
#include <QMap>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_format_organizer.h"
#include "ld_xml_writer.h"
#include "ld_xml_attributes.h"
#include "ld_element_with_fixed_children.h"

namespace Ld {
    class VisualWithGridChildren;

    /**
     * Base class for elements that support a fixed number of children that are arranged in a grid.
     */
    class LD_PUBLIC_API ElementWithGridChildren:public ElementWithFixedChildren {
        public:
            ElementWithGridChildren();

            ~ElementWithGridChildren() override;

            /**
             * The default number of rows.
             */
            static const unsigned long defaultNumberRows;

            /**
             * The default number of columns.
             */
            static const unsigned long defaultNumberColumns;

            /**
             * Method that can be called to determine how children are managed and tracked by this element.
             *
             * \return Returns a value indicating how children are tracked by this element.  This version always returns
             *         \ref Ld::Element::ChildPlacement::GROUPED.
             */
            Element::ChildPlacement childPlacement() const final;

            /**
             * Method you can use to change the object used to manage visual representation of this element.  Note that
             * using this method will also cause the \ref Visual::element method to point back to this \ref Element
             * instance.
             *
             * If the newly added \ref Visual already points to a different element, then those connections will be
             * broken in order to maintain a 1:1 relationship between any \ref Visual instance and \ref Element
             * instance.
             *
             * If this element currently has an associated visual, then that visual will disconnected from this
             * element and will be deleted.
             *
             * \param[in] newVisual A pointer to the new visual object.
             */
            void setVisual(VisualWithGridChildren* newVisual);

            /**
             * Method you can use to obtain the visual representation of this element.
             *
             * \return Returns a pointer to the visual representation of this element.  A null pointer is returned if
             *         no visual representation has been defined.
             */
            VisualWithGridChildren* visual() const;

            /**
             * Method you can use to determine the current number of rows.
             *
             * \return Returns the current number of rows.
             */
            unsigned long numberRows() const;

            /**
             * Method you can use to determine the current number of columns.
             *
             * \return Returns the current number of columns.
             */
            unsigned long numberColumns() const;

            /**
             * Method you can use to set the number of rows.  If the number of rows are reduced, children may be
             * removed.
             *
             * \param[in]     newNumberRows         The new number of rows.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             */
            void setNumberRows(unsigned long newNumberRows, CursorStateCollection* cursorStateCollection);

            /**
             * Method you can use to set the number of columns.  If the number of rows are reduced, children may be
             * removed.
             *
             * \param[in]     newNumberColumns      The new number of columns.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             */
            void setNumberColumns(unsigned long newNumberColumns, CursorStateCollection* cursorStateCollection);

            /**
             * Method that is called to remove a child from this class by index.
             *
             * \param[in]     rowIndex              The zero based row index to the child to remove.
             *
             * \param[in]     columnIndex           The zero based column index to the child to remove.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             *
             * \return Returns true on success.  Returns false if index is invalid or no child exists at the specified
             *         index.
             */
            bool removeChildAt(
                unsigned long          rowIndex,
                unsigned long          columnIndex,
                CursorStateCollection* cursorStateCollection
            );

            /**
             * Method you can use to insert a row before a given position.
             *
             * \param[in]     rowIndex              The row to follow the newly inserted row.  A row index beyond the
             *                                      number of rows will cause rows to be appended.  A row index of 0
             *                                      will cause rows to be prepended.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             *
             * \param[in]     defaultChild          An optional pointer to a child to be cloned into the newly inserted
             *                                      row.
             */
            void insertRowBefore(
                unsigned long          rowIndex,
                CursorStateCollection* cursorStateCollection,
                ElementPointer         defaultChild = ElementPointer()
            );

            /**
             * Method you can use to insert a row after a given position.
             *
             * \param[in]     rowIndex              The row to precede the newly inserted row.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             *
             * \param[in]     defaultChild          An optional pointer to a child to be cloned into the newly inserted
             *                                      row.
             */
            void insertRowAfter(
                unsigned long          rowIndex,
                CursorStateCollection* cursorStateCollection,
                ElementPointer         defaultChild = ElementPointer()
            );

            /**
             * Method you can use to insert a column before a given position.
             *
             * \param[in]     columnIndex           The column to follow the newly inserted column.  A column index
             *                                      beyond the number of columns will cause columns to be appended.  A
             *                                      column index of 0 will cause columns to be prepended.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             *
             * \param[in]     defaultChild          An optional pointer to a child to be cloned into the newly inserted
             *                                      column.
             */
            void insertColumnBefore(
                unsigned long          columnIndex,
                CursorStateCollection* cursorStateCollection,
                ElementPointer         defaultChild = ElementPointer()
            );

            /**
             * Method you can use to insert a column after a given position.
             *
             * \param[in]     columnIndex           The column to precede the newly inserted column.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             *
             * \param[in]     defaultChild          An optional pointer to a child to be cloned into the newly inserted
             *                                      column.
             */
            void insertColumnAfter(
                unsigned long          columnIndex,
                CursorStateCollection* cursorStateCollection,
                ElementPointer         defaultChild = ElementPointer()
            );

            /**
             * Method you can use to remove a row at a given position.
             *
             * \param[in]     rowIndex              The row index of the row to be removed.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             *
             * \return Returns true on success, returns false if the row index is invalid.
             */
            bool removeRow(unsigned long rowIndex, CursorStateCollection* cursorStateCollection);

            /**
             * Method you can use to remove a column at a given position.
             *
             * \param[in]     columnIndex           The row index of the row to be removed.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             *
             * \return Returns true on success, returns false if the row index is invalid.
             */
            bool removeColumn(unsigned long columnIndex, CursorStateCollection* cursorStateCollection);

            /**
             * Method you can use to change the child at a given position.
             *
             * \param[in]     rowIndex              The row index indicating where the child should be placed.
             *
             * \param[in]     columnIndex           The column index indicating where the child should be placed.
             *
             * \param[in]     childElement          The child element to be placed.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             *
             * \return Returns true on success, returns false if the location is invalid.
             */
            bool setChild(
                unsigned long          rowIndex,
                unsigned long          columnIndex,
                ElementPointer         childElement,
                CursorStateCollection* cursorStateCollection
            );

            /**
             * Method you can use to obtain the child at a given position.
             *
             * \param[in] rowIndex    The row index indicating where the child should be placed.
             *
             * \param[in] columnIndex The column index indicating where the child should be placed.
             *
             * \return Returns a pointer to the desired child.
             */
            ElementPointer child(unsigned long rowIndex, unsigned long columnIndex) const;

            /**
             * Method that can be used to change the child at a given position.  If a child is already defined for that
             * position, the child will be destroyed.
             *
             * \param[in] childIndex                The zero based index where the desired child should be placed.
             *
             * \param[in] childElement              The child element to be placed.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             *
             * \return Returns true on success, returns false if the index is invalid.
             */
            inline bool setChild(
                    unsigned long          childIndex,
                    ElementPointer         childElement,
                    CursorStateCollection* cursorStateCollection
                ) {
                return ElementWithFixedChildren::setChild(childIndex, childElement, cursorStateCollection);
            }

            /**
             * Method that can be used to obtain a child using its index.  Note that the index for each child may change
             * dynamically as children are added and removed.
             *
             * \param[in] childIndex The zero based index of the desired child.
             *
             * \return Returns a pointer to the desired child.
             */
            inline ElementPointer child(unsigned long childIndex) const override {
                return ElementWithFixedChildren::child(childIndex);
            };

            /**
             * Method that can be used to obtain the row containing a child.
             *
             * \param[in] childIndex The zero based index of the child.
             *
             * \return Returns the row index of the requested child.
             */
            unsigned long rowIndexOfChild(unsigned long childIndex) const;

            /**
             * Method that can be used to obtain the row containing a child.
             *
             * \param[in] child The child to determine the row index for.
             *
             * \return Returns the row index of the requested child.
             */
            unsigned long rowIndexOfChild(ElementPointer child) const;

            /**
             * Method that can be used to obtain the column containing a child.
             *
             * \param[in] childIndex The zero based index of the child.
             *
             * \return Returns the column index of the requested child.
             */
            unsigned long columnIndexOfChild(unsigned long childIndex) const;

            /**
             * Method that can be used to obtain the column containing a child.
             *
             * \param[in] child The child to determine the column index for.
             *
             * \return Returns the column index of the requested child.
             */
            unsigned long columnIndexOfChild(ElementPointer child) const;

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
             * Method used by the clone methods to clone the child elements.
             *
             * \param[in] element Shared pointer to the element to receive the clones.
             *
             * \return Returns true on success, returns false on failure.
             */
            bool cloneChildren(QSharedPointer<ElementWithFixedChildren> element) const override;

        private:
            /**
             * Value indicating the number of rows in the grid.
             */
            unsigned long currentNumberRows;

            /**
             * Value indicating the number of columns in the grid.
             */
            unsigned long currentNumberColumns;
    };
};

#endif
