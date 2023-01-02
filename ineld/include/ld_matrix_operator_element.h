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
* This header defines the \ref Ld::MatrixOperatorElement class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_MATRIX_OPERATOR_ELEMENT_H
#define LD_MATRIX_OPERATOR_ELEMENT_H

#include <QString>
#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_data_type.h"
#include "ld_capabilities.h"
#include "ld_element_with_grid_children.h"

namespace Ld {
    class MatrixOperatorVisual;

    /**
     * Element that allows you to explicitly define a matrix
     */
    class LD_PUBLIC_API MatrixOperatorElement:public ElementWithGridChildren {
        Q_DECLARE_TR_FUNCTIONS(Ld::MatrixOperatorElement)

        public:
            /**
             * The typename for the text element.
             */
            static const QString elementName;

            /**
             * Static value indicating the capabilities provides by this element.
             */
            static const Capabilities childProvides;

            MatrixOperatorElement();

            ~MatrixOperatorElement() override;

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
             * version returns the value reported by Ld::OperatorElementBase::intrinsicPrecedence.
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
             * Method you can call to determine the requirements imposed by this element on a specific child element.
             * Note that no testing is performed by elements to confirm that requirements are met.  The information is
             * expected to be used by the user interface.
             *
             * \param[in] childIndex The zero based child index.
             *
             * \return Returns the requirements imposed by this parent on the child element.  The default
             *         implementation returns Ld::Capabilities::rValue.
             */
            virtual Capabilities parentRequires(unsigned long childIndex) const override;

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
             * default parenthesis style, then the Ld::GenericGroupingElementBase::defaultParenthesis method is
             * used to render the parenthesis.
             *
             * \return Returns the required parenthesis style.  The value
             *         \ref Ld::Element::ParenthesisStyle::INVALID_OR_NONE indicates no parenthesis are required.
             */
            ParenthesisStyle parenthesisStyle() const override;

            /**
             * Method you can use to determine the value type this element represents.  Elements typically either
             * either report a fixed value or calculate the value based on their children.
             *
             * \return Returns the value type this element would represent.  This version always returns
             *         Ld::DataType::ValueType::COMPLEX.
             */
            DataType::ValueType valueType() const override;

            /**
             * Method you can call to determine if this element represents a matrix.  This method is intended to
             * be used to indicate how rendering of parenthesis of child elements should be performed.
             *
             * \return Returns true if this element represents an matrix.  Returns false if this element does not
             *         represent a matrix.  This version returns true.
             */
            bool isMatrix() const override;

            /**
             * Method you can call to detemrine if this element provides a logical stop for function parenthesis.  If
             * true, functions will act as if they're top level.  If false, functions will act as if they're within a
             * larger equation and will always place parenthesis around parameters.
             *
             * \param[in] functionChildIndex The index of the function element being queried.
             *
             * \return Returns true if the function can assume it's at a logical stop in the equation such as the last
             *         position in an equation or at a position, such as directly under a matrix, that can be treated
             *         as the last position.  Returns false in all other cases.  This version always returns true.
             */
            bool isFunctionLogicalStop(unsigned long functionChildIndex) const override;

            /**
             * Method you can use to change the object used to manage visual representation of this element.  Note that
             * using this method will also cause the \ref Visual::element method to point back to this
             * \ref MatrixOperatorElement instance.
             *
             * If the newly added \ref MatrixOperatorVisual already points to a different element, then those
             * connections will be broken in order to maintain a 1:1 relationship between any
             * \ref MatrixOperatorVisual instance and \ref MatrixOperatorElement instance.
             *
             * If this element currently has an associated visual, then that visual be will disconnected from this
             * element.  The disconnected visual will *not* be deleted.
             *
             * \param[in] newVisual A pointer to the new visual object.  Note that this element will take ownership of
             *                      the visual.
             */
            void setVisual(MatrixOperatorVisual* newVisual);

            /**
             * Method you can use to obtain the visual representation of this element.
             *
             * \return Returns a pointer to the visual representation of this element.  A null pointer is returned if
             *         no visual representation has been defined.
             */
            MatrixOperatorVisual* visual() const;

        protected:
            /**
             * Method that is called when this element is grafted to a new tree to perform any additional
             * initialization that may be needed.  This method is called by \ref Ld::Element::graftedToTree just before
             * triggering \ref Ld::Visual::graftedToTree.
             *
             * This version will setup the text element's format if the format is not defined and the root is
             * of type \ref Ld::RootElement.
             */
            void updateAfterGraft() override;

        private:
            /**
             * Method used internally to determine if parenthesis are required.
             *
             * \return Returns true if parenthesis are required.  Returns false if parenthesis are not required.
             */
            bool parenthesisRequired() const;
    };
};

#endif
