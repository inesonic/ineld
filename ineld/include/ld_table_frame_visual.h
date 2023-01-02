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
* This header defines the \ref Ld::TableFrameVisual class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_TABLE_FRAME_VISUAL_H
#define LD_TABLE_FRAME_VISUAL_H

#include <QtGlobal>
#include <QSharedPointer>
#include <QString>

#include "ld_common.h"
#include "ld_visual_with_grouped_children.h"

namespace Ld {
    class Element;
    class TableFrameElement;
    class TableFrameFormat;

    /**
     * Pure virtual base class for visuals associated with a \ref Ld::TableFrameElement class.  This class extends the
     * base \ref Ld::VisualWithPositionalChildren to provide notification of layout changes.
     */
    class LD_PUBLIC_API TableFrameVisual:public virtual VisualWithGroupedChildren {
        friend class TableFrameElement;

        public:
            TableFrameVisual();

            ~TableFrameVisual() override;

            /**
             * Method you can use to obtain the \ref ElementWithPositionalChildren class associated with this visual.
             *
             * \return Returns a pointer to the associated \ref ElementWithPositionalChildren instance.
             */
            QSharedPointer<TableFrameElement> element() const;

            /**
             * Method that returns the type name associated with this visual.
             *
             * \return Returns the typename associated with this visual.  The value must match the type used to invoke
             *         the static \ref Visual::create and \ref Visual::registerCreator methods.
             */
            QString typeName() const final;

            /**
             * Method that returns the name of the plug-in that provides this visual.
             *
             * \return Returns the name of the plug-in that provides this visual. The name must match the name provided
             *         in the plug-in structures.  Visuals defined as part of the ineld library should return an empty
             *         string.
             */
            QString plugInName() const final;

        protected:
            /**
             * Method that is called when one or more rows are added to the associated \ref Ld::TableFrameElement.
             *
             * \param[in] rowNumber  The zero based number of the first newly inserted row.
             *
             * \param[in] numberRows The number of newly inserted rows.
             *
             * \param[in] formatUpdated If true, the format has been updated.  If false, the format was not changed.
             */
            virtual void rowsInserted(unsigned rowNumber, unsigned numberRows, bool formatUpdated) = 0;

            /**
             * Method that is called when one or more columns are added to the associated \ref Ld::TableFrameElement.
             *
             * \param[in] columnNumber  The zero based number of the first newly inserted columns.
             *
             * \param[in] numberColumns The number of newly inserted columns.
             *
             * \param[in] formatUpdated If true, the format has been updated.  If false, the format was not changed.
             */
            virtual void columnsInserted(unsigned columnNumber, unsigned numberColumns, bool formatUpdated) = 0;

            /**
             * Method that is called when a row is about to be removed from the associated \ref Ld::TableFrameElement.
             *
             * \param[in] rowNumber           The zero based number of row we are about to remove.
             *
             * \param[in] formatWillBeUpdated If true, the format will also be updated.  If false, the format will be
             *                                left unchanged.
             */
            virtual void aboutToRemoveRow(unsigned rowNumber, bool formatWillBeUpdated) = 0;

            /**
             * Method that is called when a row has been removed from the associated \ref Ld::TableFrameElement
             *
             * \param[in] rowNumber     The zero based number of the row we just removed.
             *
             * \param[in] formatUpdated If true, the format has been updated.  If false, the format was not changed.
             */
            virtual void rowRemoved(unsigned rowNumber, bool formatUpdated) = 0;

            /**
             * Method that is called when a column is about to be removed from the associated
             * \ref Ld::TableFrameElement.
             *
             * \param[in] columnNumber        The zero based number of column we are about to remove.
             *
             * \param[in] formatWillBeUpdated If true, the format will also be updated.  If false, the format will be
             *                                left unchanged.
             */
            virtual void aboutToRemoveColumn(unsigned columnNumber, bool formatWillBeUpdated) = 0;

            /**
             * Method that is called when a column has been removed from the associated \ref Ld::TableFrameElement
             *
             * \param[in] columnNumber  The zero based number of the column we just removed.
             *
             * \param[in] formatUpdated If true, the format has been updated.  If false, the format was not changed.
             */
            virtual void columnRemoved(unsigned columnNumber, bool formatUpdated) = 0;

            /**
             * Method that is called when we are about to merge cells in the associated \ref Ld::TableFrameElement.
             *
             * \param[in] rowIndex                The zero based row index of the top-most cell we are merging.
             *
             * \param[in] columnIndex             The zero based column index of the left-most cell we are merging.
             *
             * \param[in] numberCellsToMergeRight The number of additional cells to the right of the cell that should
             *                                    be merged into the specified cell.
             *
             * \param[in] numberCellsToMergeDown  The number of additional cells below the cell that should be merged
             *                                    into the specified cell.
             *
             * \param[in] formatWillBeUpdated     If true, the format will also be updated.  If false, the format will
             *                                    be left unchanged.
             */
            virtual void aboutToMergeCells(
                unsigned               rowIndex,
                unsigned               columnIndex,
                unsigned               numberCellsToMergeRight,
                unsigned               numberCellsToMergeDown,
                bool                   formatWillBeUpdated
            ) = 0;

            /**
             * Method that is called after cells are merged in the associated \ref Ld::TableFrameElement.
             *
             * \param[in] rowIndex      The zero based row index of the top-most cell we merged.
             *
             * \param[in] columnIndex   The zero based column index of the left-most cell we merged.
             *
             * \param[in] formatUpdated If true, the format has been updated.  If false, the format was not changed.
             */
            virtual void cellsMerged(unsigned rowIndex, unsigned columnIndex, bool formatUpdated) = 0;

            /**
             * Method that is called when we are about to unmerge cells in the \ref Ld::TableFrameElement class.
             *
             * \param[in] rowIndex            The zero based row index of the left-most merged cell.
             *
             * \param[in] columnIndex         The zero based column index of the left-most merged cell.
             */
            virtual void aboutToUnmergeCells(unsigned rowIndex, unsigned columnIndex) = 0;

            /**
             * Method that is called when we have completed an unmerge operation.
             *
             * \param[in] rowIndex      The zero based row index of the left-most merged cell.
             *
             * \param[in] columnIndex   The zero based column index of the left-most merged cell.
             */
            virtual void cellsUnmerged(unsigned rowIndex, unsigned columnIndex) = 0;
    };
};

#endif
