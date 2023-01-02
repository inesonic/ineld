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
* This header defines the \ref Ld::PowerOperatorElement class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_POWER_OPERATOR_ELEMENT_H
#define LD_POWER_OPERATOR_ELEMENT_H

#include <QString>
#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_data_type.h"
#include "ld_capabilities.h"
#include "ld_binary_numeric_operator_element_base.h"

namespace Ld {
    class PowerOperatorVisual;

    /**
     * Element that provides a generic power operator.
     */
    class LD_PUBLIC_API PowerOperatorElement:public BinaryNumericOperatorElementBase {
        Q_DECLARE_TR_FUNCTIONS(Ld::PowerOperatorElement)

        public:
            /**
             * The typename for the text element.
             */
            static const QString elementName;

            PowerOperatorElement();

            ~PowerOperatorElement() override;

            /**
             * Factory method that can be used to create a default instance of a text element.
             *
             * \param[in] typeName The name used to reference the creator function.
             *
             * \return Returns a newly created, default, text element instance.
             */
            static Element* creator(const QString& typeName);

            /**
             * Method that returns the type name associated with this object.
             *
             * \return Returns the typename associated with this object.  The value must match the type used to invoke
             *         the static \ref Element::create and \ref Element::registerCreator methods.
             */
            QString typeName() const override;

            /**
             * Method that returns the name of the plug-in that provides this element.
             *
             * \return Returns the name of the plug-in that provides this element. The name must match the name provided
             *         in the plug-in structures.  Elements defined as part of the ineld library should return an empty
             *         string.
             */
            QString plugInName() const override;

            /**
             * Method that returns a description of this element.
             *
             * \return Returns a description of this element suitable for use in a GUI.
             */
            QString description() const override;

            /**
             * Method you can use to determine the precedence for this element when comparing against a child.  This
             * version returns \ref Ld::Element::powerOperatorPrecedence for the base and
             * \ref Ld::Element::powerOperatorExponentPrecedence for the exponent.
             *
             * \param[in] childIndex The index of the child we're comparing against.  This method will return a
             *                       reasonable default value if an invalid child index is supplied.
             */
            Precedence parentPrecedence(unsigned long childIndex) const override;

            /**
             * Method you can use to determine the value type this element represents.  Elements typically either
             * either report a fixed value or calculate the value based on their children.
             *
             * \return Returns the value type this element would represent.
             */
            DataType::ValueType valueType() const override;

            /**
             * Method you can use to determine if the exponent should be rendered.  There are a few children that
             * will render the exponent for this operator.
             *
             * Note that this method exists for a future enhancement is is currently not used.
             *
             * \return Returns true if the exponent should be rendered.  Returns false if the exponent should not be
             *         rendered.
             */
            bool renderExponent() const;

            /**
             * Method you can use to change the object used to manage visual representation of this element.  Note that
             * using this method will also cause the \ref Visual::element method to point back to this
             * \ref PowerOperatorElement instance.
             *
             * If the newly added \ref PowerOperatorVisual already points to a different element, then those
             * connections will be broken in order to maintain a 1:1 relationship between any
             * \ref PowerOperatorVisual instance and \ref PowerOperatorElement instance.
             *
             * If this element currently has an associated visual, then that visual be will disconnected from this
             * element.  The disconnected visual will *not* be deleted.
             *
             * \param[in] newVisual A pointer to the new visual object.  Note that this element will take ownership of
             *                      the visual.
             */
            void setVisual(PowerOperatorVisual* newVisual);

            /**
             * Method you can use to obtain the visual representation of this element.
             *
             * \return Returns a pointer to the visual representation of this element.  A null pointer is returned if
             *         no visual representation has been defined.
             */
            PowerOperatorVisual* visual() const;

            /**
             * Method you can use to determine the intrinsic precedence for this element.
             *
             * \return Returns the element's intrinsic precedence.
             */
            Precedence intrinsicPrecedence() const override;

        private:
            /**
             * Table of data conversion rules.
             */
            static const DataType::ValueType allowedConversions[DataType::numberValueTypes][DataType::numberValueTypes];
    };
};

#endif
