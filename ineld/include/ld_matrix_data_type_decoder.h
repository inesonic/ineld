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
* This header defines the \ref Ld::MatrixDataTypeDecoder class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_MATRIX_DATA_TYPE_DECODER_H
#define LD_MATRIX_DATA_TYPE_DECODER_H

#include <QCoreApplication>

#include "ld_common.h"
#include "ld_format_structures.h"
#include "ld_data_type_decoder.h"

namespace Model {
    class Matrix;
}

namespace Ld {
    /**
     * Base class you can use for all matrix data type decoders.  You can use this class as a common decoder for all
     * matrix decoders.
     */
    class LD_PUBLIC_API MatrixDataTypeDecoder:public virtual DataTypeDecoder {
        Q_DECLARE_TR_FUNCTIONS(Ld::MatrixDataTypeDecoder)

        public:
            /**
             * Base class that provides stateful information during decode operations.
             */
            class LD_PUBLIC_API Context {
                public:
                    /**
                     * Constructor
                     *
                     * \param[in] format The format used to decode the matrix.
                     */
                    Context(FormatPointer format);

                    virtual ~Context();

                    /**
                     * Method you can use to obtain a pointer to the underlying matrix instance.
                     *
                     * \return Returns a pointer to the underlying matrix instance.
                     */
                    virtual const Model::Matrix& matrix() const = 0;

                    /**
                     * Method you can use to obtain a pointer to the format instance.
                     *
                     * \return Returns a pointer to the format.
                     */
                    FormatPointer format() const;

                    /**
                     * Method you can use to obtain the number of rows.
                     *
                     * \return Returns the number of matrix rows.
                     */
                    virtual unsigned long numberRows() const = 0;

                    /**
                     * Method you can use to obtain the number of columns.
                     *
                     * \return Returns the number of matrix columns.
                     */
                    virtual unsigned long numberColumns() const = 0;

                    /**
                     * Method you can use to obtain the number of leading rows.
                     *
                     * \return Returns the number of leading rows.
                     */
                    unsigned long numberLeadingRows() const;

                    /**
                     * Method you can use to obtain the number of trailing rows.
                     *
                     * \return Returns the number of trailing rows.
                     */
                    unsigned long numberTrailingRows() const;

                    /**
                     * Method yo can use to obtain the first trailing row.
                     *
                     * \return Returns the row number of the first trailing row.
                     */
                    unsigned long firstTrailingRow() const;

                    /**
                     * Method you can use to obtain the number of leading columns.
                     *
                     * \return Returns the number of leading columns.
                     */
                    unsigned long numberLeadingColumns() const;

                    /**
                     * Method you can use to obtain the number of trailing columns.
                     *
                     * \return Returns the number of trailing columns.
                     */
                    unsigned long numberTrailingColumns() const;

                    /**
                     * Method you can use to obtain the first trailing column.
                     *
                     * \return Returns the column number of the first trailing column.
                     */
                    unsigned long firstTrailingColumn() const;

                    /**
                     * Method you can can use to determine if values of this type and format can require superscript.
                     *
                     * \return Returns true if values can be superscripted.  Returns false if values will never
                     *         include superscripts.
                     */
                    virtual bool canSuperscript() const = 0;

                    /**
                     * Method you can can use to determine if values of this type and format can require subscript.
                     *
                     * \return Returns true if values can be subscripted.  Returns false if values will never
                     *         include subscripts.
                     */
                    virtual bool canSubscript() const = 0;

                    /**
                     * Method you can overload to render a single matrix element.
                     *
                     * \param[in] row    The matrix row of the coefficient to be rendered.
                     *
                     * \param[in] column The matrix column of the coefficient to be rendered.
                     *
                     * \return Returns a string representation of the coefficient.
                     */
                    virtual QString toString(unsigned long row, unsigned long column) = 0;

                protected:
                    /**
                     * Method you can use to configure most of the base class variables.
                     *
                     * \param[in] numberRows    The number of matrix rows.
                     *
                     * \param[in] numberColumns The number of matrix columns.
                     */
                    void configure(unsigned long numberRows, unsigned long numberColumns);

                private:
                    /**
                     * Pointer to the format instance.
                     */
                    FormatPointer currentFormat;

                    /**
                     * The current number of leading rows.
                     */
                    unsigned long currentNumberLeadingRows;

                    /**
                     * The current number of trailing rows.
                     */
                    unsigned long currentNumberTrailingRows;

                    /**
                     * The first row in the trailing set of rows.
                     */
                    unsigned long currentFirstTrailingRow;

                    /**
                     * The current number of leading columns.
                     */
                    unsigned long currentNumberLeadingColumns;

                    /**
                     * The current number of trailing columns.
                     */
                    unsigned long currentNumberTrailingColumns;

                    /**
                     * The first row in the trailing set of columns.
                     */
                    unsigned long currentFirstTrailingColumn;
            };

            MatrixDataTypeDecoder();

            ~MatrixDataTypeDecoder() override;

        protected:
            /**
             * Method you can call to render the matrix.
             *
             * \param[in,out] context The context tied to the matrix to be rendered.
             *
             * \return Returns a string representation of the matrix.
             */
            QString translateToString(Context& context) const;
    };
};

#endif
