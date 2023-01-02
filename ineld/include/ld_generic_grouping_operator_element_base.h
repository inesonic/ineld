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
* This header defines the \ref Ld::GenericGroupingOperatorElementBase class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_GENERIC_GROUPING_OPERATOR_ELEMENT_BASE_H
#define LD_GENERIC_GROUPING_OPERATOR_ELEMENT_BASE_H

#include "ld_common.h"
#include "ld_grouping_operator_element_base.h"

namespace Ld {
    /**
     * Base class for generic grouping operators such as parenthesis that do not change the value of their contents.
     */
    class LD_PUBLIC_API GenericGroupingOperatorElementBase:public GroupingOperatorElementBase {
        public:
            /**
             * Static value indicating the capabilities provides by this element.
             */
            static const Capabilities childProvides;

            GenericGroupingOperatorElementBase();

            ~GenericGroupingOperatorElementBase() override;

            /**
             * Method you can call to determine the requirements imposed by this element on a specific child element.
             * Note that no testing is performed by elements to confirm that requirements are met.  The information is
             * expected to be used by the user interface.
             *
             * \param[in] childIndex The zero based child index.
             *
             * \return Returns the requirements imposed by this parent on the child element.  The default
             *         implementation returns Ld::Capabilities::rValue.
             */
            Capabilities parentRequires(unsigned long childIndex) const override;

            /**
             * Method you can call to determine what capabilities this element will provide.  Note that this object
             * also provides a static constant value named "childProvides" returning the same information.
             *
             * \return Returns the requirements met by this element.  The default implementation returns
             *         Ld::Capabilities::operator
             */
            Capabilities childProvidesCapabilities() const override;

            /**
             * Method you can use to obtain the parenthesis style for use by this element.  This version indicates a
             * parenthesis is always required.  If the format doesn't indicate a parenthesis style or indicates the
             * default parenthesis style, then the Ld::GenericGroupingElementBase::defaultParenthesis method is used
             * to render the parenthesis.
             *
             * \return Returns the required parenthesis style.  The value
             *         Ld::Element::ParenthesisStyle::INVALID_OR_NONE indicates no parenthesis are required.
             */
            ParenthesisStyle parenthesisStyle() const override;

        protected:
            /**
             * Method you can overload to indicate the default parenthesis style for this operator.
             *
             * \return Returns the default parenthesis style for this operator.
             */
            virtual ParenthesisStyle defaultParenthesisStyle() const = 0;
    };
};

#endif