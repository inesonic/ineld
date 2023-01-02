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
* \filef
*
* This file implements the \ref Ld::MatrixIntegerDataTypeDecoder class.
***********************************************************************************************************************/

#include <QCoreApplication>
#include <QString>
#include <QSharedPointer>

#include <cassert>

#include <util_string.h>

#include <model_matrix_integer.h>

#include <model_intrinsic_types.h>
#include <model_matrix_integer.h>

#include <ld_format_structures.h>
#include <ld_matrix_data_type_format.h>
#include <ld_integer_data_type_format.h>
#include <ld_value_field_format.h>
#include <ld_matrix_integer_data_type_format.h>
#include "ld_data_type_decoder.h"
#include "ld_matrix_data_type_decoder.h"
#include "ld_matrix_integer_data_type_decoder.h"

/***********************************************************************************************************************
 * MatrixIntegerDataTypeDecoder::Context:
 */

namespace Ld {
    MatrixIntegerDataTypeDecoder::Context::Context(
            const Model::MatrixInteger& matrix,
            FormatPointer               format
        ):MatrixDataTypeDecoder::Context(
            format
        ),currentMatrix(
            matrix
        ) {
        configure(matrix.numberRows(), matrix.numberColumns());

        if (!format.isNull()                                                             &&
            (format->capabilities().contains(Ld::IntegerDataTypeFormat::formatName) ||
             format->capabilities().contains(Ld::ValueFieldFormat::formatName)         )    ) {
            QSharedPointer<IntegerDataTypeFormat> integerFormat = format.dynamicCast<IntegerDataTypeFormat>();
            currentIntegerNumberStyle = integerFormat->integerNumberStyle();
            currentNumberDigits       = integerFormat->width();
            currentBase               = integerFormat->base();
            currentUpperCase          = integerFormat->upperCase();
        } else {
            currentIntegerNumberStyle = MatrixIntegerDataTypeFormat::defaultIntegerNumberStyle;
            currentNumberDigits       = MatrixIntegerDataTypeFormat::defaultWidth;
            currentBase               = MatrixIntegerDataTypeFormat::defaultBase;
            currentUpperCase          = MatrixIntegerDataTypeFormat::defaultUpperCase;
        }
    }


    MatrixIntegerDataTypeDecoder::Context::~Context() {}


    const Model::Matrix& MatrixIntegerDataTypeDecoder::Context::matrix() const {
        return currentMatrix;
    }


    unsigned long MatrixIntegerDataTypeDecoder::Context::numberRows() const {
        return currentMatrix.numberRows();
    }


    unsigned long MatrixIntegerDataTypeDecoder::Context::numberColumns() const {
        return currentMatrix.numberColumns();
    }


    Util::IntegerNumberStyle MatrixIntegerDataTypeDecoder::Context::integerNumberStyle() const {
        return currentIntegerNumberStyle;
    }


    unsigned MatrixIntegerDataTypeDecoder::Context::numberDigits() const {
        return currentNumberDigits;
    }


    unsigned MatrixIntegerDataTypeDecoder::Context::base() const {
        return currentBase;
    }


    bool MatrixIntegerDataTypeDecoder::Context::upperCase() const {
        return currentUpperCase;
    }


    bool MatrixIntegerDataTypeDecoder::Context::canSuperscript() const {
        return false;
    }


    bool MatrixIntegerDataTypeDecoder::Context::canSubscript() const {
        return currentIntegerNumberStyle == MatrixIntegerDataTypeFormat::IntegerNumberStyle::SUBSCRIPT_STYLE;
    }


    QString MatrixIntegerDataTypeDecoder::Context::toString(unsigned long row, unsigned long column) {
        Model::Integer i = currentMatrix(row, column);
        return Util::longLongIntegerToUnicodeString(
            i,
            currentIntegerNumberStyle,
            currentNumberDigits,
            currentBase,
            currentUpperCase
        );
    }
}

/***********************************************************************************************************************
 * MatrixIntegerDataTypeDecoder:
 */

namespace Ld {
    MatrixIntegerDataTypeDecoder::MatrixIntegerDataTypeDecoder() {}


    MatrixIntegerDataTypeDecoder::~MatrixIntegerDataTypeDecoder() {}


    QString MatrixIntegerDataTypeDecoder::toString(const Model::Variant& value, Ld::FormatPointer format) const {
        QString result;

        bool                 ok;
        Model::MatrixInteger matrix = value.toMatrixInteger(&ok);
        if (ok) {
            Context context(matrix, format);
            result = translateToString(context);
        }

        return result;
    }


    QString MatrixIntegerDataTypeDecoder::toDescription(const Model::Variant& value) const {
        QString              result;

        bool                 ok;
        Model::MatrixInteger m = value.toMatrixInteger(&ok);

        if (ok) {
            unsigned long numberRows    = static_cast<unsigned long>(m.numberRows());
            unsigned long numberColumns = static_cast<unsigned long>(m.numberColumns());

            if (numberRows == 0 || numberColumns == 0) {
                result = tr("Integer matrix (empty)");
            } else if (numberRows == 1) {
                if (numberColumns == 1) {
                    result = tr("Integer matrix (single coefficient");
                } else {
                    result = tr("Integer row matrix (%1 coefficients)").arg(numberColumns);
                }
            } else {
                if (numberColumns == 1) {
                    result = tr("Integer column matrix (%1 coefficients)").arg(numberRows);
                } else {
                    result = tr("Integer matrix (%1%2%3)").arg(numberRows).arg(QChar(0x00D7)).arg(numberColumns);
                }
            }
        } else {
            result = tr("Integer matrix (*** Could not decode ***)");
        }

        return result;
    }
}
