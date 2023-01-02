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
* This file implements the \ref Ld::GenericGroupingOperatorElementBase class.
***********************************************************************************************************************/

#include "ld_format_structures.h"
#include "ld_parenthesis_format.h"
#include "ld_grouping_operator_element_base.h"
#include "ld_generic_grouping_operator_element_base.h"

namespace Ld {
    const Capabilities GenericGroupingOperatorElementBase::childProvides = Capabilities::allOperators;

    GenericGroupingOperatorElementBase::GenericGroupingOperatorElementBase() {}


    GenericGroupingOperatorElementBase::~GenericGroupingOperatorElementBase() {}


    Capabilities GenericGroupingOperatorElementBase::parentRequires(unsigned long /* index */) const {
        return Capabilities::allRValues;
    }


    Capabilities GenericGroupingOperatorElementBase::childProvidesCapabilities() const {
        return childProvides;
    }


    Element::ParenthesisStyle GenericGroupingOperatorElementBase::parenthesisStyle() const {
        ParenthesisStyle result = ParenthesisStyle::DEFAULT;

        FormatPointer format = GenericGroupingOperatorElementBase::format();
        if (format->capabilities().contains(ParenthesisFormat::formatName)) {
            QSharedPointer<ParenthesisFormat> parenthesisFormat = format.dynamicCast<ParenthesisFormat>();
            assert(!parenthesisFormat.isNull());

            result = parenthesisFormat->parenthesisStyle();
        }

        if (result == ParenthesisStyle::DEFAULT || result == ParenthesisStyle::INVALID_OR_NONE) {
            result = defaultParenthesisStyle();
        }

        return result;
    }
}
