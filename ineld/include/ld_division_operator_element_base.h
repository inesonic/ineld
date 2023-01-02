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
* This header defines the \ref Ld::DivisionOperatorElementBase class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_DIVISION_OPERATOR_ELEMENT_BASE_H
#define LD_DIVISION_OPERATOR_ELEMENT_BASE_H

#include <QString>
#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_data_type.h"
#include "ld_capabilities.h"
#include "ld_binary_operator_element_base.h"

namespace Ld {
    class DivisionOperatorVisual;

    /**
     * Base class for elements that provides support for division operations.  The first child represent the numerator
     * and the second child represents the denominator.
     */
    class LD_PUBLIC_API DivisionOperatorElementBase:public BinaryOperatorElementBase {
        public:
            /**
             * Static value indicating the capabilities provides by this element.
             */
            static const Capabilities childProvides;

            DivisionOperatorElementBase();

            ~DivisionOperatorElementBase() override;

            /**
             * Method you can use to determine the precedence for this element when comparing against the parent.
             *
             * \return Returns the element's precedence when comparing against the element's parent.
             */
            Precedence childPrecedence() const override;

            /**
             * Method you can use to determine the precedence for this element when comparing against a child.  The
             * default implementation returns the value reported by \ref Ld::Element::childPrecedence.
             *
             * \param[in] childIndex The index of the child we're comparing against.  This method will return a
             *                       reasonable default value if an invalid child index is supplied.
             */
            Precedence parentPrecedence(unsigned long childIndex) const override;

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
             * Method you can use to determine the value type this element represents.  Elements typically either
             * either report a fixed value or calculate the value based on their children.
             *
             * \return Returns the value type this element would represent.  This version will calculate the best
             *         recommended upcast for matrix and division values.
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
             * Method that determines if this instance is presenting as an obelus or as a fraction.
             *
             * \return Returns true if this instance is presenting as a fraction.  Returns false if this instance is
             *         presenting as an obelus.
             */
            virtual bool presentingAsFraction() const = 0;
    };
};

#endif
