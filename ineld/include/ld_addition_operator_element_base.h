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
* This header defines the \ref Ld::AdditionOperatorElementBase class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_ADDITION_OPERATOR_ELEMENT_BASE_H
#define LD_ADDITION_OPERATOR_ELEMENT_BASE_H

#include "ld_common.h"
#include "ld_data_type.h"
#include "ld_binary_numeric_and_matrix_operator_element_base.h"

namespace Ld {
    /**
     * Common base class for the addition and subtraction operators.
     */
    class LD_PUBLIC_API AdditionOperatorElementBase:public BinaryNumericAndMatrixOperatorElementBase {
        public:
            /**
             * The typename for the text element.
             */
            static const QString elementName;

            AdditionOperatorElementBase();

            ~AdditionOperatorElementBase() override;

            /**
             * Method you can use to determine the value type this element represents.  Elements typically either
             * either report a fixed value or calculate the value based on their children.
             *
             * \return Returns the value type this element would represent.  This version will calculate the best
             *         recommended upcast for matrix and scalar/basic multiplication values.
             */
            DataType::ValueType valueType() const override;

        protected:
            /**
             * Table of allowed operation types and results.
             */
            static const DataType::ValueType allowedConversions
                [static_cast<unsigned>(DataType::ValueType::NUMBER_TYPES)]
                [static_cast<unsigned>(DataType::ValueType::NUMBER_TYPES)];

            /**
             * Method you can use to determine the intrinsic precedence for this element.
             *
             * \return Returns the element's intrinsic precedence.
             */
            Precedence intrinsicPrecedence() const override;
    };
};

#endif
