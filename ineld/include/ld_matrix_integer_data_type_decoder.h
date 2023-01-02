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
* This header defines the \ref Ld::MatrixIntegerDataTypeDecoder class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_MATRIX_INTEGER_DATA_TYPE_DECODER_H
#define LD_MATRIX_INTEGER_DATA_TYPE_DECODER_H

#include <QCoreApplication>
#include <QList>
#include <QString>

#include <util_string.h>

#include <model_matrix_integer.h>

#include "ld_common.h"
#include "ld_matrix_data_type_decoder.h"

namespace Ld {
    /**
     * Integer matrix data type decoder.
     */
    class LD_PUBLIC_API MatrixIntegerDataTypeDecoder:public virtual MatrixDataTypeDecoder {
        Q_DECLARE_TR_FUNCTIONS(Ld::MatrixIntegerDataTypeDecoder)

        public:
            /**
             * Base class that provides stateful information during decode operations.
             */
            class LD_PUBLIC_API Context:public virtual MatrixDataTypeDecoder::Context {
                public:
                    /**
                     * Constructor
                     *
                     * \param[in] matrix The matrix.  This class will take ownership of the matrix and destroy it at
                     *                   the end of scope.
                     *
                     * \param[in] format The format used to decode the matrix.
                     */
                    Context(const Model::MatrixInteger& matrix, FormatPointer format);

                    ~Context() override;

                    /**
                     * Method you can use to obtain a pointer to the underlying matrix instance.
                     *
                     * \return Returns a pointer to the underlying matrix instance.
                     */
                    const Model::Matrix& matrix() const final;

                    /**
                     * Method you can use to obtain the number of rows.
                     *
                     * \return Returns the number of matrix rows.
                     */
                    unsigned long numberRows() const final;

                    /**
                     * Method you can use to obtain the number of columns.
                     *
                     * \return Returns the number of matrix columns.
                     */
                    unsigned long numberColumns() const final;

                    /**
                     * Method you can call to determine the desired integer style.
                     *
                     * \return Returns the desired integer style.
                     */
                    Util::IntegerNumberStyle integerNumberStyle() const;

                    /**
                     * Method you can call to determine the desired number of digits.
                     *
                     * \return Returns the desired number of digits.
                     */
                    unsigned numberDigits() const;

                    /**
                     * Method you can call to determine the desired integer base.
                     *
                     * \return Returns the desired integer bas.e
                     */
                    unsigned base() const;

                    /**
                     * Method you can call to determine if alphabetic digits should be presented in upper case.
                     *
                     * \return Returns true if alphabetic digits should be displayed in upper case.
                     */
                    bool upperCase() const;

                    /**
                     * Method you can can use to determine if values of this type and format can require superscript.
                     *
                     * \return Returns true if values can be superscripted.  Returns false if values will never
                     *         include superscripts.
                     */
                    bool canSuperscript() const override;

                    /**
                     * Method you can can use to determine if values of this type and format can require subscript.
                     *
                     * \return Returns true if values can be subscripted.  Returns false if values will never
                     *         include subscripts.
                     */
                    bool canSubscript() const override;

                    /**
                     * Method you can overload to render a single matrix element.
                     *
                     * \param[in] row    The matrix row of the coefficient to be rendered.
                     *
                     * \param[in] column The matrix column of the coefficient to be rendered.
                     *
                     * \return Returns a string representation of the coefficient.
                     */
                    QString toString(unsigned long row, unsigned long column) override;

                private:
                    /**
                     * The matrix tracked by this instance.
                     */
                    Model::MatrixInteger currentMatrix;

                    /**
                     * The desired integer style.
                     */
                    Util::IntegerNumberStyle currentIntegerNumberStyle;

                    /**
                     * The desired number of digits.
                     */
                    unsigned currentNumberDigits;

                    /**
                     * The desired radix.
                     */
                    unsigned currentBase;

                    /**
                     * Flag indicating if we should use upper-case characters.
                     */
                    bool currentUpperCase;
            };

            MatrixIntegerDataTypeDecoder();

            ~MatrixIntegerDataTypeDecoder() override;

            /**
             * Method you can overload to create a textual representation of a value of this data-type.
             *
             * \param[in] value   A variant type containing the value to be translated.
             *
             * \param[in] format  Format information used to control how the data-type is translated.  Note that the
             *                    translation engine must be able to perform a reasonable translation even if a format
             *                    has not been supplied.
             *
             * \return Returns the value translated to a string.  An empty string is returned if the translation could
             *         not be performed.
             */
            QString toString(const Model::Variant& value, Ld::FormatPointer format) const override;

            /**
             * Method you can overload to create a brief textual description of a value of this data type.
             *
             * \param[in] value A variant type containing the value to generate the description for.
             *
             * \return Returns a brief (one line) description.  The description will exclude the actual value.
             */
            QString toDescription(const Model::Variant& value) const override;
    };
};

#endif
