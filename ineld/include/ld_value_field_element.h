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
* This header defines the \ref Ld::ValueFieldElement class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_VALUE_FIELD_ELEMENT_H
#define LD_VALUE_FIELD_ELEMENT_H

#include <QString>
#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QSharedPointer>

#include <model_variant.h>

#include "ld_common.h"
#include "ld_capabilities.h"
#include "ld_data_type.h"
#include "ld_calculated_value.h"
#include "ld_element_with_no_children.h"

namespace Ld {
    class ValueFieldVisual;

    /**
     * Element that is used to field containing the value from a variable.
     */
    class LD_PUBLIC_API ValueFieldElement:public ElementWithNoChildren {
        Q_DECLARE_TR_FUNCTIONS(Ld::ValueFieldElement)

        public:
            /**
             * The typename for the text element.
             */
            static const QString elementName;

            /**
             * Static value indicating the capabilities provides by this element.
             */
            static const Capabilities childProvides;

            ValueFieldElement();

            ~ValueFieldElement() override;

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
             * Method you can use to determine the value type this element represents.  Elements typically either
             * either report a fixed value or calculate the value based on their children.
             *
             * \return Returns the value type this element would represent.  A value of ValueType::NONE indicates
             *         either that the element does not return a value or the value type could not be determined at
             *         this time.
             */
            DataType::ValueType valueType() const override;

            /**
             * Method you can call to determine what capabilities this element will provide.  Note that this object
             * also provides a static constant value named "childProvides" returning the same information.
             *
             * \return Returns the requirements met by this element.
             */
            Capabilities childProvidesCapabilities() const final;

            /**
             * Method you can use to change the object used to manage visual representation of this element.  Note that
             * using this method will also cause the \ref Visual::element method to point back to this
             * \ref ValueFieldElement instance.
             *
             * If the newly added \ref ValueFieldVisual already points to a different element, then those connections
             * will be broken in order to maintain a 1:1 relationship between any \ref ValueFieldVisual instance and
             * \ref ValueFieldElement instance.
             *
             * If this element currently has an associated visual, then that visual be will disconnected from this
             * element.  The disconnected visual will *not* be deleted.
             *
             * \param[in] newVisual A pointer to the new visual object.  Note that this element will take ownership of
             *                      the visual.
             */
            void setVisual(ValueFieldVisual* newVisual);

            /**
             * Method you can use to obtain the visual representation of this element.
             *
             * \return Returns a pointer to the visual representation of this element.  A null pointer is returned if
             *         no visual representation has been defined.
             */
            ValueFieldVisual* visual() const;

            /**
             * Method you can use to determine if this element supports image export capability.
             *
             * \return Returns the export image capability for this element.  This implementatin returns
             *         \ref Ld::Element::ExportImageCapability::THROUGH_VISUAL_EPHEMERAL.
             */
            ExportImageCapability exportImageCapability() const override;

            /**
             * Method you can overload to indicate if this element requires calculated data to fully render content.
             *
             * \return Returns true if this element requires calculated data to fully render its content.  Returns
             *         false if this element does not require calculated data to fully render content.  This version
             *         returns true.
             */
            bool requiresCalculatedDataToRender() const override;

            /**
             * Method you can overload to determine the number of distinct calculated values supported by this
             * element.
             *
             * \return Returns the number of calculated values supported by this element.
             */
            unsigned numberCalculatedValues() const override;

            /**
             * Method you can overload to receive notification of calculated values associated with this element.  The
             * default method simply reports the data to any visual tied to this element.
             *
             * \param[in] valueIndex      A zero based index used to indicate which calculated value to update.
             *
             * \param[in] calculatedValue The calculated value.
             */
            void setCalculatedValue(unsigned valueIndex, const CalculatedValue& calculatedValue) override;

            /**
             * Method you can overload to receive notification when the calculated value should be cleared.
             */
            void clearCalculatedValue() override;

            /**
             * Method you can overload to obtain the last reported calculated value.
             *
             * \param[in] valueIndex A zero based index used to reference the desired calculated value.
             *
             * \return Returns the last reported calculated value.  An invalid calculated value is returned if the
             *         index is invalid.  The default implementation always returns an invalid calculated value.
             */
            CalculatedValue calculatedValue(unsigned valueIndex = 0) const override;

        private:
            /**
             * The current calculated value for this field.
             */
            CalculatedValue currentCalculatedValue;
    };
};

#endif
