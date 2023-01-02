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
* This file implements the \ref Ld::MatrixBooleanDataTypeDecoder class.
***********************************************************************************************************************/

#include <QCoreApplication>
#include <QList>
#include <QString>
#include <QSharedPointer>

#include <cassert>

#include <util_string.h>

#include <model_matrix_boolean.h>

#include <model_intrinsic_types.h>
#include <model_matrix_boolean.h>

#include <ld_format_structures.h>
#include <ld_matrix_data_type_format.h>
#include <ld_boolean_data_type_format.h>
#include <ld_matrix_boolean_data_type_format.h>
#include <ld_value_field_format.h>
#include "ld_data_type_decoder.h"
#include "ld_matrix_data_type_decoder.h"
#include "ld_matrix_boolean_data_type_decoder.h"

/***********************************************************************************************************************
 * MatrixBooleanDataTypeDecoder::Context:
 */

namespace Ld {
    MatrixBooleanDataTypeDecoder::Context::Context(
            const Model::MatrixBoolean& matrix,
            FormatPointer               format
        ):MatrixDataTypeDecoder::Context(
            format
        ),currentMatrix(
            matrix
        ) {
        configure(matrix.numberRows(), matrix.numberColumns());

        if (!format.isNull()                                                             &&
            (format->capabilities().contains(Ld::BooleanDataTypeFormat::formatName) ||
             format->capabilities().contains(Ld::ValueFieldFormat::formatName)         )    ) {
            QSharedPointer<BooleanDataTypeFormat> booleanFormat = format.dynamicCast<BooleanDataTypeFormat>();
            currentBooleanStyle = booleanFormat->booleanStyle();
        } else {
            currentBooleanStyle = MatrixBooleanDataTypeFormat::defaultBooleanStyle;
        }
    }


    MatrixBooleanDataTypeDecoder::Context::~Context() {}


    const Model::Matrix& MatrixBooleanDataTypeDecoder::Context::matrix() const {
        return currentMatrix;
    }


    unsigned long MatrixBooleanDataTypeDecoder::Context::numberRows() const {
        return currentMatrix.numberRows();
    }


    unsigned long MatrixBooleanDataTypeDecoder::Context::numberColumns() const {
        return currentMatrix.numberColumns();
    }


    Util::BooleanStyle MatrixBooleanDataTypeDecoder::Context::booleanStyle() const {
        return currentBooleanStyle;
    }


    bool MatrixBooleanDataTypeDecoder::Context::canSuperscript() const {
        return false;
    }


    bool MatrixBooleanDataTypeDecoder::Context::canSubscript() const {
        return false;
    }


    QString MatrixBooleanDataTypeDecoder::Context::toString(unsigned long row, unsigned long column) {
        Model::Boolean b = currentMatrix(row, column);
        return Util::booleanToUnicodeString(b, currentBooleanStyle);
    }
}

/***********************************************************************************************************************
 * MatrixBooleanDataTypeDecoder:
 */

namespace Ld {
    MatrixBooleanDataTypeDecoder::MatrixBooleanDataTypeDecoder() {}


    MatrixBooleanDataTypeDecoder::~MatrixBooleanDataTypeDecoder() {}


    QString MatrixBooleanDataTypeDecoder::toString(const Model::Variant& value, Ld::FormatPointer format) const {
        QString result;

        bool                 ok;
        Model::MatrixBoolean matrix = value.toMatrixBoolean(&ok);
        if (ok) {
            Context context(matrix, format);
            result = translateToString(context);
        }

        return result;
    }


    QString MatrixBooleanDataTypeDecoder::toDescription(const Model::Variant& value) const {
        QString              result;

        bool                 ok;
        Model::MatrixBoolean m = value.toMatrixBoolean(&ok);

        if (ok) {
            unsigned long numberRows    = static_cast<unsigned long>(m.numberRows());
            unsigned long numberColumns = static_cast<unsigned long>(m.numberColumns());

            if (numberRows == 0 || numberColumns == 0) {
                result = tr("Boolean matrix (empty)");
            } else if (numberRows == 1) {
                if (numberColumns == 1) {
                    result = tr("Boolean matrix (single coefficient");
                } else {
                    result = tr("Boolean row matrix (%1 coefficients)").arg(numberColumns);
                }
            } else {
                if (numberColumns == 1) {
                    result = tr("Boolean column matrix (%1 coefficients)").arg(numberRows);
                } else {
                    result = tr("Boolean matrix (%1%2%3)").arg(numberRows).arg(QChar(0x00D7)).arg(numberColumns);
                }
            }
        } else {
            result = tr("Boolean matrix (*** Could not decode ***)");
        }

        return result;
    }
}
