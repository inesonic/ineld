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
* This header defines the \ref Ld::OperatorElementBase class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_OPERATOR_ELEMENT_BASE_H
#define LD_OPERATOR_ELEMENT_BASE_H

#include <QSharedPointer>

#include "ld_common.h"
#include "ld_capabilities.h"
#include "ld_element_with_fixed_children.h"

namespace Ld {
    /**
     * Base class for all operators with a fixed number of children.
     */
    class LD_PUBLIC_API OperatorElementBase:public ElementWithFixedChildren {
        public:
            OperatorElementBase();

            ~OperatorElementBase() override;

            /**
             * Method you can use to determine the precedence for this element when comparing against a child.  This
             * version returns the value reported by \ref Ld::OperatorElementBase::intrinsicPrecedence.
             *
             * \param[in] childIndex The index of the child we're comparing against.  This method will return a
             *                       reasonable default value if an invalid child index is supplied.
             */
            Precedence parentPrecedence(unsigned long childIndex) const override;

            /**
             * Method you can use to determine the precedence for this element.  This version will either report the
             * intrinsic precedence of the operator or the \ref Ld::Element::groupingOperatorPrecedence.
             *
             * \return Returns the element's precedence.  The default implementation returns
             *         \ref Ld::Element::defaultPrecedence.
             */
            Precedence childPrecedence() const override;

            /**
             * Method you can use to determine the value type this element represents.  Elements typically either
             * either report a fixed value or calculate the value based on their children.
             *
             * \return Returns the value type this element would represent.  This version will calculate the best
             *         recommended upcast between all the values reported by the children and use that as the return
             *         value.
             */
            DataType::ValueType valueType() const override;

        protected:
            /**
             * Method you can overload to report the intrinsic precedence of the operator.  The intrinsic precedence is
             * the translation precedence without parenthesis.
             *
             * \return Returns the intrinsic precedence of the operator.  THe default implementation returns the
             *         default precedence.
             */
            virtual Precedence intrinsicPrecedence() const;

            /**
             * Method that is called when this element is grafted to a new tree to perform any additional
             * initialization that may be needed.  This method is called by \ref Ld::Element::graftedToTree just before
             * triggering \ref Ld::Visual::graftedToTree.
             *
             * This version will setup the text element's format if the format is not defined and the root is
             * of type \ref Ld::RootElement.
             */
            void updateAfterGraft() override;
    };
};

#endif
