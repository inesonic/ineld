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
* This file implements the \ref Ld::RealDataTypeDecoderBase class.
***********************************************************************************************************************/

#include <QString>

#include <cmath>
#include <sstream>
#include <ios>
#include <iomanip>
#include <limits>

#include <util_string.h>

#include <model_intrinsic_types.h>

#include "ld_format_structures.h"
#include "ld_real_data_type_format.h"
#include "ld_data_type_decoder.h"
#include "ld_real_data_type_decoder_base.h"

namespace Ld {
    RealDataTypeDecoderBase::RealDataTypeDecoderBase() {}


    RealDataTypeDecoderBase::~RealDataTypeDecoderBase() {}


    QString RealDataTypeDecoderBase::realToString(Model::Real value, QSharedPointer<Ld::RealDataTypeFormat> format) {
        QString result;

        bool                                upperCase;
        RealDataTypeFormat::RealNumberStyle style;
        unsigned                            precision;

        if (format.isNull() || !format->capabilities().contains(Ld::RealDataTypeFormat::formatName)) {
            upperCase  = RealDataTypeFormat::defaultUpperCase;
            style      = RealDataTypeFormat::defaultRealNumberStyle;
            precision  = RealDataTypeFormat::defaultPrecision;
        } else {
            upperCase  = format->upperCase();
            style      = format->realNumberStyle();
            precision  = format->precision();
        }

        result = Util::longDoubleToUnicodeString(value, style, precision, upperCase);

        return result;
    }
}
