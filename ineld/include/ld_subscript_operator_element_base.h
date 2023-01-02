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
* This header defines the \ref Ld::SubscriptOperatorElementBase class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_SUBSCRIPT_OPERATOR_ELEMENT_BASE_H
#define LD_SUBSCRIPT_OPERATOR_ELEMENT_BASE_H

#include <QString>
#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_data_type.h"
#include "ld_capabilities.h"
#include "ld_operator_element_base.h"

namespace Ld {
    class SubscriptIndexOperatorVisual;

    /**
     * Base class for subscripting operators.
     */
    class LD_PUBLIC_API SubscriptOperatorElementBase:public OperatorElementBase {
        Q_DECLARE_TR_FUNCTIONS(Ld::SubscriptOperatorElementBase)

        public:
            /**
             * Static value indicating the capabilities provides by this element.
             */
            static const Capabilities childProvides;

            /**
             * Constructor
             *
             * \param[in] numberSubscripts The number of subscript terms.
             */
            SubscriptOperatorElementBase(unsigned numberSubscripts);

            ~SubscriptOperatorElementBase() override;

            /**
             * Method you can use to determine the value type this element represents.  Elements typically either
             * either report a fixed value or calculate the value based on their children.
             *
             * \return Returns the value type this element would represent.  This version always returns
             *         Ld::Element::ValueType::COMPLEX.
             */
            DataType::ValueType valueType() const override;

            /**
             * Method you can call to determine the requirements imposed by this element on a specific child element.
             * Note that no testing is performed by elements to confirm that requirements are met.  The information is
             * expected to be used by the user interface.
             *
             * \param[in] childIndex The zero based child index.
             *
             * \return Returns the requirements imposed by this parent on the child element.
             */
            Capabilities parentRequires(unsigned long childIndex) const override;

            /**
             * Method you can call to determine what capabilities this element will provide.  Note that this object
             * also provides a static constant value named "childProvides" returning the same information.
             *
             * \return Returns the requirements met by this element.
             */
            Capabilities childProvidesCapabilities() const final;

        protected:
            /**
             * Method you can use to determine the intrinsic precedence for this element.
             *
             * \return Returns the element's intrinsic precedence.
             */
            Precedence intrinsicPrecedence() const override;
    };
};

#endif
