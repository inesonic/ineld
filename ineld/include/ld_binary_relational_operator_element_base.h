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
* This header defines the \ref Ld::BinaryRelationalOperatorElementBase class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_BINARY_RELATIONAL_OPERATOR_ELEMENT_BASE_H
#define LD_BINARY_RELATIONAL_OPERATOR_ELEMENT_BASE_H

#include "ld_common.h"
#include "ld_data_type.h"
#include "ld_binary_operator_element_base.h"

namespace Ld {
    /**
     * Trivial base class for relational operator elements with two children.
     */
    class LD_PUBLIC_API BinaryRelationalOperatorElementBase:public BinaryOperatorElementBase {
        public:
            BinaryRelationalOperatorElementBase();

            ~BinaryRelationalOperatorElementBase() override;

            /**
             * Method you can use to determine the value type this element represents.  Elements typically either
             * either report a fixed value or calculate the value based on their children.
             *
             * \return Returns the value type this element would represent.  This version will always return
             *         Ld::DataType::ValueType::BOOLEAN.
             */
            DataType::ValueType valueType() const override;

        protected:
            /**
             * Method you can overload to report the intrinsic precedence of the operator.  The intrinsic precedence is
             * the translation precedence without parenthesis.
             *
             * \return Returns the intrinsic precedence of the operator.  THe default implementation returns the
             *         precedence Element::relationalOperatorPrecedence.
             */
            Precedence intrinsicPrecedence() const override;
    };
};

#endif
