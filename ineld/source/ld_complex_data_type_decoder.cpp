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
* This file implements the \ref Ld::ComplexDataTypeDecoder class.
***********************************************************************************************************************/

#include <QString>

#include <model_intrinsic_types.h>
#include <model_complex.h>
#include <model_variant.h>

#include "ld_format_structures.h"
#include "ld_complex_data_type_format.h"
#include "ld_real_data_type_decoder_base.h"
#include "ld_complex_data_type_decoder.h"

namespace Ld {
    ComplexDataTypeDecoder::ComplexDataTypeDecoder() {}


    ComplexDataTypeDecoder::~ComplexDataTypeDecoder() {}


    QString ComplexDataTypeDecoder::toString(const Model::Variant& value, Ld::FormatPointer format) const {
        QString  result;

        QSharedPointer<Ld::ComplexDataTypeFormat> complexFormat = format.dynamicCast<Ld::ComplexDataTypeFormat>();
        bool           ok;
        Model::Complex complexValue = value.toComplex(&ok);

        if (ok) {
            Model::Real r = complexValue.real();
            Model::Real i = complexValue.imag();

            if (r == Model::Real(0) && i == Model::Real(0)) {
                result = tr("0");
            } else {
                if (r != Model::Real(0)) {
                    result = realToString(r, complexFormat);
                }

                if (i != Model::Real(0)) {
                    if (i > 0) {
                        result += tr("+") + realToString(i, complexFormat);
                    } else {
                        result += realToString(i, complexFormat);
                    }

                    ComplexDataTypeFormat::ImaginaryUnit imaginaryUnit;
                    if (complexFormat.isNull()) {
                        imaginaryUnit = ComplexDataTypeFormat::ImaginaryUnit::I;
                    } else {
                        imaginaryUnit = complexFormat->imaginaryUnit();
                    }

                    switch (imaginaryUnit) {
                        case ComplexDataTypeFormat::ImaginaryUnit::INVALID:
                        case ComplexDataTypeFormat::ImaginaryUnit::I: {
                            result += tr("i");
                            break;
                        }

                        case ComplexDataTypeFormat::ImaginaryUnit::J: {
                            result += tr("j");
                            break;
                        }

                        default: {
                            Q_ASSERT(false);
                            break;
                        }
                    }
                }
            }
        } else {
            result = tr("???");
        }

        return result;
    }


    QString ComplexDataTypeDecoder::toDescription(const Model::Variant& /* value */) const {
        return tr("Complex (double precision)");
    }
}


