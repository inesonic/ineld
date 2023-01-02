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
* This header defines helper functions to deal with complex values.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_COMPLEX_DATA_TYPE_TRANSLATOR_HELPERS_H
#define LD_COMPLEX_DATA_TYPE_TRANSLATOR_HELPERS_H

#include <QString>

#include "ld_element_structures.h"
#include "ld_complex_data_type_format.h"

namespace Model {
    class Variant;
    class Complex;
}

namespace Ld {
    /**
     * Method that generates strings for the real and imaginary baseline and superscript.  Method also generates a
     * string for the imaginary unit.
     *
     * \param[in]     variant                    The variant to be translated.
     *
     * \param[in]     element                    A pointer to the element to be translated.
     *
     * \param[out]    realBaselineString         String holding the real value baseline text.
     *
     * \param[out]    realSuperscriptString      String holding the superscript for the real value.
     *
     * \param[out]    imaginaryBaselineString    String holding the imaginary baseline text.
     *
     * \param[out]    imaginarySuperscriptString String holding the superscript for the imaginary value.
     *
     * \param[out]    imaginaryUnitString        String holding the imaginary unit.
     *
     * \return Returns true on success, returns false on error.
     */
    LD_PUBLIC_API bool generateComplexValueStrings(
        const Model::Variant& variant,
        Ld::ElementPointer    element,
        QString&              realBaselineString,
        QString&              realSuperscriptString,
        QString&              imaginaryBaselineString,
        QString&              imaginarySuperscriptString,
        QString&              imaginaryUnitString
    );

    /**
     * Method that generates strings for the real and imaginary baseline and superscript.  Method also generates a
     * string for the imaginary unit.
     *
     * \param[in]     value                      The value to be translated.
     *
     * \param[in]     realNumberStyle            The real number style to apply to the converted value.
     *
     * \param[in]     precision                  The precision to apply for the number.
     *
     * \param[in]     upperCase                  If true, characters other than "i" or "j" should be displayed in upper
     *                                           case.
     *
     * \param[in]     imaginaryUnit              The imaginary unit.
     *
     * \param[out]    realBaselineString         String holding the real value baseline text.
     *
     * \param[out]    realSuperscriptString      String holding the superscript for the real value.
     *
     * \param[out]    imaginaryBaselineString    String holding the imaginary baseline text.
     *
     * \param[out]    imaginarySuperscriptString String holding the superscript for the imaginary value.
     *
     * \param[out]    imaginaryUnitString        String holding the imaginary unit.
     */
    LD_PUBLIC_API void generateComplexValueStrings(
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
    );
}

#endif
