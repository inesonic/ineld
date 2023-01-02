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
* This file implements the \ref Ld::MatrixComplexDataTypeDecoder class.
***********************************************************************************************************************/

#include <QCoreApplication>
#include <QString>
#include <QSharedPointer>

#include <cassert>

#include <util_string.h>

#include <model_intrinsic_types.h>
#include <model_complex.h>
#include <model_matrix_complex.h>

#include <ld_format_structures.h>
#include <ld_matrix_data_type_format.h>
#include <ld_complex_data_type_format.h>
#include <ld_value_field_format.h>
#include <ld_matrix_complex_data_type_format.h>
#include "ld_data_type_decoder.h"
#include "ld_matrix_data_type_decoder.h"
#include "ld_matrix_complex_data_type_decoder.h"

/***********************************************************************************************************************
 * MatrixComplexDataTypeDecoder::Context:
 */

namespace Ld {
    MatrixComplexDataTypeDecoder::Context::Context(
            const Model::MatrixComplex& matrix,
            FormatPointer            format
        ):MatrixDataTypeDecoder::Context(
            format
        ),currentMatrix(
            matrix
        ) {
        configure(matrix.numberRows(), matrix.numberColumns());

        if (!format.isNull()                                                             &&
            (format->capabilities().contains(Ld::ComplexDataTypeFormat::formatName) ||
             format->capabilities().contains(Ld::ValueFieldFormat::formatName)         )    ) {
            QSharedPointer<ComplexDataTypeFormat> complexFormat = format.dynamicCast<ComplexDataTypeFormat>();
            currentRealNumberStyle = complexFormat->realNumberStyle();
            currentPrecision       = complexFormat->precision();
            currentUpperCase       = complexFormat->upperCase();
            currentImaginaryUnit   = complexFormat->imaginaryUnit();
            currentMultiplier      = complexFormat->multiplier();
        } else {
            currentRealNumberStyle = MatrixComplexDataTypeFormat::defaultRealNumberStyle;
            currentPrecision       = MatrixComplexDataTypeFormat::defaultPrecision;
            currentUpperCase       = MatrixComplexDataTypeFormat::defaultUpperCase;
            currentImaginaryUnit   = MatrixComplexDataTypeFormat::defaultImaginaryUnit;
            currentMultiplier      = MatrixComplexDataTypeFormat::defaultMultiplier;
        }
    }


    MatrixComplexDataTypeDecoder::Context::~Context() {}


    const Model::Matrix& MatrixComplexDataTypeDecoder::Context::matrix() const {
        return currentMatrix;
    }


    unsigned long MatrixComplexDataTypeDecoder::Context::numberRows() const {
        return currentMatrix.numberRows();
    }


    unsigned long MatrixComplexDataTypeDecoder::Context::numberColumns() const {
        return currentMatrix.numberColumns();
    }


    Util::RealNumberStyle MatrixComplexDataTypeDecoder::Context::realNumberStyle() const {
        return currentRealNumberStyle;
    }


    unsigned MatrixComplexDataTypeDecoder::Context::precision() const {
        return currentPrecision;
    }


    bool MatrixComplexDataTypeDecoder::Context::upperCase() const {
        return currentUpperCase;
    }


    ComplexDataTypeFormat::ImaginaryUnit MatrixComplexDataTypeDecoder::Context::imaginaryUnit() const {
        return currentImaginaryUnit;
    }


    Model::Real MatrixComplexDataTypeDecoder::Context::multiplier() const {
        return currentMultiplier;
    }


    bool MatrixComplexDataTypeDecoder::Context::canSuperscript() const {
        return (
               currentRealNumberStyle == MatrixComplexDataTypeFormat::RealNumberStyle::CONCISE
            || currentRealNumberStyle == MatrixComplexDataTypeFormat::RealNumberStyle::SCIENTIFIC
            || currentRealNumberStyle == MatrixComplexDataTypeFormat::RealNumberStyle::ENGINEERING
        );
    }


    bool MatrixComplexDataTypeDecoder::Context::canSubscript() const {
        return false;
    }


    QString MatrixComplexDataTypeDecoder::Context::toString(unsigned long row, unsigned long column) {
        Model::Complex c = currentMatrix(row, column) * currentMultiplier;
        Model::Real    r = c.real();
        Model::Real    i = c.imag();

        QString result;
        if (i == 0) {
            result = Util::longDoubleToUnicodeString(r, currentRealNumberStyle, currentPrecision, currentUpperCase);
        } else {
            if (r == 0) {
                result = Util::longDoubleToUnicodeString(
                    i,
                    currentRealNumberStyle,
                    currentPrecision,
                    currentUpperCase
                );
            } else {
                if (i < 0) {
                    result = (
                          Util::longDoubleToUnicodeString(
                              r,
                              currentRealNumberStyle,
                              currentPrecision,
                              currentUpperCase
                          )
                        + Util::longDoubleToUnicodeString(
                              i,
                              currentRealNumberStyle,
                              currentPrecision,
                              currentUpperCase
                          )
                   );
                } else {
                    result = (
                          Util::longDoubleToUnicodeString(
                              r,
                              currentRealNumberStyle,
                              currentPrecision,
                              currentUpperCase
                          )
                        + "+"
                        + Util::longDoubleToUnicodeString(
                              i,
                              currentRealNumberStyle,
                              currentPrecision,
                              currentUpperCase
                          )
                    );
                }
            }

            switch (currentImaginaryUnit) {
                case ComplexDataTypeFormat::ImaginaryUnit::I: { result += tr("i");    break; }
                case ComplexDataTypeFormat::ImaginaryUnit::J: { result += tr("j");    break; }
                default: {
                    assert(false);
                    break;
                }
            }
        }

        return result;
    }
}

/***********************************************************************************************************************
 * MatrixComplexDataTypeDecoder:
 */

namespace Ld {
    MatrixComplexDataTypeDecoder::MatrixComplexDataTypeDecoder() {}


    MatrixComplexDataTypeDecoder::~MatrixComplexDataTypeDecoder() {}


    QString MatrixComplexDataTypeDecoder::toString(const Model::Variant& value, Ld::FormatPointer format) const {
        QString result;

        bool                 ok;
        Model::MatrixComplex matrix = value.toMatrixComplex(&ok);
        if (ok) {
            Context context(matrix, format);
            result = translateToString(context);
        }

        return result;
    }


    QString MatrixComplexDataTypeDecoder::toDescription(const Model::Variant& value) const {
        QString              result;

        bool                 ok;
        Model::MatrixComplex m = value.toMatrixComplex(&ok);

        if (ok) {
            unsigned long numberRows    = static_cast<unsigned long>(m.numberRows());
            unsigned long numberColumns = static_cast<unsigned long>(m.numberColumns());

            if (numberRows == 0 || numberColumns == 0) {
                result = tr("Complex matrix (empty)");
            } else if (numberRows == 1) {
                if (numberColumns == 1) {
                    result = tr("Complex matrix (single coefficient");
                } else {
                    result = tr("Complex row matrix (%1 coefficients)").arg(numberColumns);
                }
            } else {
                if (numberColumns == 1) {
                    result = tr("Complex column matrix (%1 coefficients)").arg(numberRows);
                } else {
                    result = tr("Complex matrix (%1%2%3)").arg(numberRows).arg(QChar(0x00D7)).arg(numberColumns);
                }
            }
        } else {
            result = tr("Complex matrix (*** Could not decode ***)");
        }

        return result;
    }
}
