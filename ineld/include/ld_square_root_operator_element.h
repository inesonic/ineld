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
* This header defines the \ref Ld::SquareRootOperatorElement class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_SQUARE_ROOT_OPERATOR_ELEMENT_H
#define LD_SQUARE_ROOT_OPERATOR_ELEMENT_H

#include <QString>
#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_capabilities.h"
#include "ld_unary_numeric_operator_element_base.h"

namespace Ld {
    class SquareRootOperatorVisual;

    /**
     * Element that provides support for square root operations.
     */
    class LD_PUBLIC_API SquareRootOperatorElement:public UnaryNumericOperatorElementBase {
        Q_DECLARE_TR_FUNCTIONS(Ld::SquareRootOperatorElement)

        public:
            /**
             * The typename for the text element.
             */
            static const QString elementName;

            SquareRootOperatorElement();

            ~SquareRootOperatorElement() override;

            /**
             * Factory method that can be used to create a default instance of this element.
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
             * Method you can use to determine the precedence for this element when comparing against the parent.
             *
             * \return Returns the element's precedence when comparing against the element's parent.
             */
            Precedence childPrecedence() const override;

            /**
             * Method you can use to determine the precedence for this element when comparing against a child.
             *
             * \param[in] childIndex The index of the child we're comparing against.
             */
            Precedence parentPrecedence(unsigned long childIndex) const override;

            /**
             * Method you can use to determine the value type this element represents.  Elements typically either
             * either report a fixed value or calculate the value based on their children.
             *
             * \return Returns the value type this element would represent.  This version always returns
             *         DataType::ValueType::COMPLEX.
             */
            DataType::ValueType valueType() const override;

            /**
             * Method you can use to change the object used to manage visual representation of this element.  Note that
             * using this method will also cause the \ref Visual::element method to point back to this
             * \ref Ld::SquareRootOperatorElement instance.
             *
             * If the newly added \ref SquareRootOperatorVisual already points to a different element, then those
             * connections will be broken in order to maintain a 1:1 relationship between any
             * \ref SquareRootOperatorVisual instance and \ref SquareRootOperatorElement instance.
             *
             * If this element currently has an associated visual, then that visual be will disconnected from this
             * element.  The disconnected visual will *not* be deleted.
             *
             * \param[in] newVisual A pointer to the new visual object.  Note that this element will take ownership of
             *                      the visual.
             */
            void setVisual(SquareRootOperatorVisual* newVisual);

            /**
             * Method you can use to obtain the visual representation of this element.
             *
             * \return Returns a pointer to the visual representation of this element.  A null pointer is returned if
             *         no visual representation has been defined.
             */
            SquareRootOperatorVisual* visual() const;
    };
};

#endif
