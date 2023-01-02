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
* This file implements helper functions to deal with complex vlaues.
***********************************************************************************************************************/

#include <QString>

#include <model_intrinsic_types.h>
#include <model_complex.h>
#include <model_variant.h>

#include <util_string.h>

#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_complex_data_type_format.h"
#include "ld_value_field_format.h"
#include "ld_complex_data_type_translator_helpers.h"

namespace Ld {
    bool generateComplexValueStrings(
            const Model::Variant& variant,
            Ld::ElementPointer    element,
            QString&              realBaselineString,
            QString&              realSuperscriptString,
            QString&              imaginaryBaselineString,
            QString&              imaginarySuperscriptString,
            QString&              imaginaryUnitString
        ) {
        bool           success;
        Model::Complex value = variant.toComplex(&success);

        if (success) {
            bool                                       upperCase;
            unsigned                                   precision;
            double                                     multiplier;
            Ld::ComplexDataTypeFormat::RealNumberStyle realNumberStyle;
            Ld::ComplexDataTypeFormat::ImaginaryUnit   imaginaryUnit;

            Ld::FormatPointer format = element->format();
            if (!format.isNull()                                                             &&
                (format->capabilities().contains(Ld::ComplexDataTypeFormat::formatName) ||
                 format->capabilities().contains(Ld::ValueFieldFormat::formatName)         )    ) {
                QSharedPointer<Ld::ComplexDataTypeFormat>
                    complexFormat = format.dynamicCast<Ld::ComplexDataTypeFormat>();

                upperCase       = complexFormat->upperCase();
                precision       = complexFormat->precision();
                multiplier      = complexFormat->multiplier();
                realNumberStyle = complexFormat->realNumberStyle();
                imaginaryUnit   = complexFormat->imaginaryUnit();
            } else {
                upperCase       = Ld::ComplexDataTypeFormat::defaultUpperCase;
                precision       = Ld::ComplexDataTypeFormat::defaultPrecision;
                multiplier      = Ld::ComplexDataTypeFormat::defaultMultiplier;
                realNumberStyle = Ld::ComplexDataTypeFormat::defaultRealNumberStyle;
                imaginaryUnit   = Ld::ComplexDataTypeFormat::defaultImaginaryUnit;
            }

            generateComplexValueStrings(
                value * multiplier,
                realNumberStyle,
                precision,
                upperCase,
                imaginaryUnit,
                realBaselineString,
                realSuperscriptString,
                imaginaryBaselineString,
                imaginarySuperscriptString,
                imaginaryUnitString
            );
        }

        return success;
    }


    void generateComplexValueStrings(
            const Model::Complex&                      value,
            Ld::ComplexDataTypeFormat::RealNumberStyle realNumberStyle,
            unsigned                                   precision,
            bool                                       upperCase,
            Ld::ComplexDataTypeFormat::ImaginaryUnit   imaginaryUnit,
            QString&                                   realBaselineString,
            QString&                                   realSuperscriptString,
            QString&                                   imaginaryBaselineString,
            QString&                                   imaginarySuperscriptString,
            QString&                                   imaginaryUnitString
        ) {
        Model::Real realValue      = value.real();
        Model::Real imaginaryValue = value.imag();

        if (realValue != Model::Real(0) || imaginaryValue == Model::Real(0)) {
            Util::longDoubleToMantissaAndExponentStrings(
                realValue,
                realNumberStyle,
                precision,
                realBaselineString,
                realSuperscriptString
            );
        }

        if (imaginaryValue != Model::Real(0)) {
            Util::longDoubleToMantissaAndExponentStrings(
                imaginaryValue,
                realNumberStyle,
                precision,
                imaginaryBaselineString,
                imaginarySuperscriptString
            );
        }

        if (realNumberStyle == Ld::ComplexDataTypeFormat::RealNumberStyle::COMPUTER_SCIENTIFIC ||
            realNumberStyle == Ld::ComplexDataTypeFormat::RealNumberStyle::COMPUTER_CONCISE       ) {
            QString exponentSymbol = upperCase ? QString("E") : QString("e");

            if (!realSuperscriptString.isEmpty()) {
                realBaselineString = QString("%1%2%3")
                                     .arg(realBaselineString)
                                     .arg(exponentSymbol)
                                     .arg(realSuperscriptString);
                realSuperscriptString.clear();
            }

            if (!imaginarySuperscriptString.isEmpty()) {
                imaginaryBaselineString = QString("%1%2%3")
                                          .arg(imaginaryBaselineString)
                                          .arg(exponentSymbol)
                                          .arg(imaginarySuperscriptString);
                imaginarySuperscriptString.clear();
            }
        }

        imaginaryUnitString =   imaginaryUnit == Ld::ComplexDataTypeFormat::ImaginaryUnit::I
                              ? QString("i")
                              : QString("j");
    }
}
