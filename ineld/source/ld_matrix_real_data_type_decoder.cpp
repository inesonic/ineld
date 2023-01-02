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
* This file implements the \ref Ld::MatrixRealDataTypeDecoder class.
***********************************************************************************************************************/

#include <QCoreApplication>
#include <QString>
#include <QSharedPointer>

#include <cassert>

#include <util_string.h>

#include <model_matrix_real.h>

#include <model_intrinsic_types.h>
#include <model_matrix_real.h>

#include <ld_format_structures.h>
#include <ld_matrix_data_type_format.h>
#include <ld_real_data_type_format.h>
#include <ld_value_field_format.h>
#include <ld_matrix_real_data_type_format.h>
#include "ld_data_type_decoder.h"
#include "ld_matrix_data_type_decoder.h"
#include "ld_matrix_real_data_type_decoder.h"

/***********************************************************************************************************************
 * MatrixRealDataTypeDecoder::Context:
 */

namespace Ld {
    MatrixRealDataTypeDecoder::Context::Context(
            const Model::MatrixReal& matrix,
            FormatPointer            format
        ):MatrixDataTypeDecoder::Context(
            format
        ),currentMatrix(
            matrix
        ) {
        configure(matrix.numberRows(), matrix.numberColumns());

        if (!format.isNull()                                                          &&
            (format->capabilities().contains(Ld::RealDataTypeFormat::formatName) ||
             format->capabilities().contains(Ld::ValueFieldFormat::formatName)      )    ) {
            QSharedPointer<RealDataTypeFormat> realFormat = format.dynamicCast<RealDataTypeFormat>();
            currentRealNumberStyle = realFormat->realNumberStyle();
            currentPrecision       = realFormat->precision();
            currentUpperCase       = realFormat->upperCase();
            currentMultiplier      = realFormat->multiplier();
        } else {
            currentRealNumberStyle = MatrixRealDataTypeFormat::defaultRealNumberStyle;
            currentPrecision       = MatrixRealDataTypeFormat::defaultPrecision;
            currentUpperCase       = MatrixRealDataTypeFormat::defaultUpperCase;
            currentMultiplier      = MatrixRealDataTypeFormat::defaultMultiplier;
        }
    }


    MatrixRealDataTypeDecoder::Context::~Context() {}


    const Model::Matrix& MatrixRealDataTypeDecoder::Context::matrix() const {
        return currentMatrix;
    }


    unsigned long MatrixRealDataTypeDecoder::Context::numberRows() const {
        return currentMatrix.numberRows();
    }


    unsigned long MatrixRealDataTypeDecoder::Context::numberColumns() const {
        return currentMatrix.numberColumns();
    }


    Util::RealNumberStyle MatrixRealDataTypeDecoder::Context::realNumberStyle() const {
        return currentRealNumberStyle;
    }


    unsigned MatrixRealDataTypeDecoder::Context::precision() const {
        return currentPrecision;
    }


    bool MatrixRealDataTypeDecoder::Context::upperCase() const {
        return currentUpperCase;
    }


    Model::Real MatrixRealDataTypeDecoder::Context::multiplier() const {
        return currentMultiplier;
    }


    bool MatrixRealDataTypeDecoder::Context::canSuperscript() const {
        return (
               currentRealNumberStyle == MatrixRealDataTypeFormat::RealNumberStyle::CONCISE
            || currentRealNumberStyle == MatrixRealDataTypeFormat::RealNumberStyle::SCIENTIFIC
            || currentRealNumberStyle == MatrixRealDataTypeFormat::RealNumberStyle::ENGINEERING
        );
    }


    bool MatrixRealDataTypeDecoder::Context::canSubscript() const {
        return false;
    }


    QString MatrixRealDataTypeDecoder::Context::toString(unsigned long row, unsigned long column) {
        Model::Real r = currentMatrix(row, column) * currentMultiplier;
        return Util::longDoubleToUnicodeString(r, currentRealNumberStyle, currentPrecision, currentUpperCase);
    }
}

/***********************************************************************************************************************
 * MatrixRealDataTypeDecoder:
 */

namespace Ld {
    MatrixRealDataTypeDecoder::MatrixRealDataTypeDecoder() {}


    MatrixRealDataTypeDecoder::~MatrixRealDataTypeDecoder() {}


    QString MatrixRealDataTypeDecoder::toString(const Model::Variant& value, Ld::FormatPointer format) const {
        QString result;

        bool              ok;
        Model::MatrixReal matrix = value.toMatrixReal(&ok);
        if (ok) {
            Context context(matrix, format);
            result = translateToString(context);
        }

        return result;
    }


    QString MatrixRealDataTypeDecoder::toDescription(const Model::Variant& value) const {
        QString           result;

        bool              ok;
        Model::MatrixReal m = value.toMatrixReal(&ok);

        if (ok) {
            unsigned long numberRows    = static_cast<unsigned long>(m.numberRows());
            unsigned long numberColumns = static_cast<unsigned long>(m.numberColumns());

            if (numberRows == 0 || numberColumns == 0) {
                result = tr("Real matrix (empty)");
            } else if (numberRows == 1) {
                if (numberColumns == 1) {
                    result = tr("Real matrix (single coefficient");
                } else {
                    result = tr("Real row matrix (%1 coefficients)").arg(numberColumns);
                }
            } else {
                if (numberColumns == 1) {
                    result = tr("Real column matrix (%1 coefficients)").arg(numberRows);
                } else {
                    result = tr("Real matrix (%1%2%3)").arg(numberRows).arg(QChar(0x00D7)).arg(numberColumns);
                }
            }
        } else {
            result = tr("Real matrix (*** Could not decode ***)");
        }

        return result;
    }
}
