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
* This header defines the \ref Ld::VisualWithGridChildren class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_VISUAL_WITH_GRID_CHILDREN_H
#define LD_VISUAL_WITH_GRID_CHILDREN_H

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_element.h"
#include "ld_visual_with_fixed_children.h"

namespace Ld {
    class ElementWithGridChildren;

    /**
     * Pure virtual base class for visuals with a variable number of elements that occupy fixed relative positions in
     * one or more group.  THis class is intended to be used in tandem with the \ref ElementWithGroupedChildren class
     * as a base class for visuals.
     */
    class LD_PUBLIC_API VisualWithGridChildren:public virtual VisualWithFixedChildren {
        friend class ElementWithGridChildren;

        public:
            VisualWithGridChildren();

            ~VisualWithGridChildren() override;

            /**
             * Method that can be called to determine how children are managed and tracked by this visual.
             *
             * \return Returns a value indicating how children are tracked by this element.  This version returns
             *         \ref Ld::Element::ChildPlacement::GROUPED.
             */
            Element::ChildPlacement childPlacement() const override;

            /**
             * Method you can use to obtain the \ref ElementWithPositionalChildren class associated with this visual.
             *
             * \return Returns a pointer to the associated \ref ElementWithPositionalChildren instance.
             */
            QSharedPointer<ElementWithGridChildren> element() const;

        protected:
            /**
             * Pure virtual method that is called after the grid dimensions are changed.  This method is not triggered
             * when rows and columns are inserted or removed.
             *
             * \param[in] oldNumberRows    The old number of rows.
             *
             * \param[in] oldNumberColumns The old number of columns.
             *
             * \param[in] newNumberRows    The new number of rows.
             *
             * \param[in] newNumberColumns The new number of columns.
             */
            virtual void geometryChanged(
                unsigned long oldNumberRows,
                unsigned long oldNumberColumns,
                unsigned long newNumberRows,
                unsigned long newNumberColumns
            ) = 0;

            /**
             * Pure virtual method that is called just before a row is removed.
             *
             * \param[in] rowIndex The row index of the row that is about to be removed.
             */
            virtual void aboutToRemoveRow(unsigned long rowIndex) = 0;

            /**
             * Pure virtual method that is called just before a column is removed.
             *
             * \param[in] columnIndex The column index of the column that is about to be removed.
             */
            virtual void aboutToRemoveColumn(unsigned long columnIndex) = 0;

            /**
             * Pure virtual method that is called after a new row is inserted.
             *
             * \param[in] rowIndex The row index of the newly inserted row.
             */
            virtual void rowInserted(unsigned long rowIndex) = 0;

            /**
             * Pure virtual method that is called after a new column is inserted.
             *
             * \param[in] columnIndex The column index of the newly inserted column.
             */
            virtual void columnInserted(unsigned long columnIndex) = 0;
    };
};

#endif
