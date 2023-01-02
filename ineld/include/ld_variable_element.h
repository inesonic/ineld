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
* This header defines the \ref Ld::VariableElement class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_VARIABLE_ELEMENT_H
#define LD_VARIABLE_ELEMENT_H

#include <QString>
#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QSharedPointer>
#include <QSet>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_data_type.h"
#include "ld_capabilities.h"
#include "ld_calculated_value.h"
#include "ld_identifier_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_attributes.h"
#include "ld_element_with_no_children.h"

namespace Ld {
    class VariableVisual;
    class ElementCursor;
    class Cursor;
    class CursorStateCollection;
    class Identifier;

    /**
     * Element that is used to represent a variable element.
     */
    class LD_PUBLIC_API VariableElement:public ElementWithNoChildren {
        Q_DECLARE_TR_FUNCTIONS(Ld::VariableElement)

        public:
            /**
             * The typename for the text element.
             */
            static const QString elementName;

            /**
             * Static value indicating the capabilities provides by this element.
             */
            static const Capabilities childProvides;

            VariableElement();

            ~VariableElement() override;

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
             * \return Returns the value type this element would represent.  This version obtains the type currently
             *         reported by the \ref Ld::RootElement.
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
             * Method you can call to determine what capabilities this element can support intrinisically.  This method
             * is used when the cursor is in text mode to determine what additional types of children or special
             * objects can be inserted.
             *
             * Generally this call indicates what types of special characters can be inserted although it can also be
             * used to indicate additional elements that can be processed while the cursor is pointing to text in this
             * element.
             *
             * \param[in] textOffset  The zero based offset into the text region.
             *
             * \param[in] regionIndex The zero based region index.
             *
             * \return Returns the capabilities this element can support while the cursor is pointing to text.
             */
            Capabilities textCapabilities(unsigned long textOffset, unsigned regionIndex) const final;

            /**
             * Method you can use to change the object used to manage visual representation of this element.  Note that
             * using this method will also cause the \ref Visual::element method to point back to this
             * \ref VariableElement instance.
             *
             * If the newly added \ref VariableVisual already points to a different element, then those connections
             * will be broken in order to maintain a 1:1 relationship between any \ref VariableVisual instance and
             * \ref VariableElement instance.
             *
             * If this element currently has an associated visual, then that visual be will disconnected from this
             * element.  The disconnected visual will *not* be deleted.
             *
             * \param[in] newVisual A pointer to the new visual object.  Note that this element will take ownership of
             *                      the visual.
             */
            void setVisual(VariableVisual* newVisual);

            /**
             * Method you can use to obtain the visual representation of this element.
             *
             * \return Returns a pointer to the visual representation of this element.  A null pointer is returned if
             *         no visual representation has been defined.
             */
            VariableVisual* visual() const;

            /**
             * Method that can be used to determine the minimum number of distinct text regions used by this element.
             *
             * \return Returns the minimum number of text regions.
             */
            unsigned minimumNumberTextRegions() const final;

            /**
             * Method that can be used to determine the maximum number of distinct text regions used by this element.
             *
             * \return Returns the maximum number of text regions.
             */
            unsigned maximumNumberTextRegions() const final;

            /**
             * Method you can use to set the number of expected text regions.
             *
             * \param[in] newNumberTextRegions The new number of text regions.
             *
             * \return Returns true on success, returns false if the number of text regions is invalid.
             */
            bool setNumberTextRegions(unsigned newNumberTextRegions) final;

            /**
             * Method you can use to obtain the current number of text regions.
             *
             * \return Returns the current number of text regions.
             */
            unsigned numberTextRegions() const final;

            /**
             * Method that sets the text associated with a text region of this element.
             *
             * \param[in] newText      The new text to associate with this element and region.
             *
             * \param[in] regionNumber The zero based region number.
             *
             * \return Returns true if the region is valid.  Returns false if the region is invalid.
             */
            bool setText(const QString& newText, unsigned regionNumber = 0) final;

            /**
             * Method you can use to obtain the text associated with a text region of this element.
             *
             * \param[in] regionNumber The zero based region number.
             *
             * \return Returns the text currently associated with this element's region.  Returns empty text if the
             *         region is invalid.
             */
            QString text(unsigned regionNumber = 0) const final;

            /**
             * Method that indicates if the \ref Ld::Cursor can place the cursor on this element.
             *
             * \return Returns the allowed cursor park mode.  The default implementation returns
             *         \ref CursorParkMode::PARK_ON_TEXT_ONLY.
             */
            CursorParkMode cursorParkMode() const override;

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

            /**
             * Method you can use to set the identifier tracking this variable.
             *
             * \param[in] identifier The identifier tracking this variable.
             */
            void setIdentifier(const IdentifierContainer& identifier);

            /**
             * Method you can use to obtain the identifier tracking this variable.
             *
             * \return Returns the identifier tracking this variable.
             */
            const IdentifierContainer& identifier() const;

        protected:
            /**
             * Method that is called when this element is grafted to a new tree to perform any additional
             * initialization that may be needed.  This method is called by \ref Ld::Element::graftedToTree just before
             * triggering \ref Ld::Visual::graftedToTree.
             *
             * This version will setup the text element's format if the format is not defined and the root is
             * of type \ref Ld::RootElement.
             */
            void updateAfterGraft() final;

            /**
             * Method you can optionally overload to add additional attributes to the XML description of this element.
             *
             * \param[in]     attributes  A reference to the list of attributes to be modified.
             *
             * \param[in]     formats     A \ref FormatOrganizer instance used to track all known formats in the
             *                            program.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             */
            void writeAddAttributes(
                XmlAttributes&                  attributes,
                QSharedPointer<FormatOrganizer> formats,
                ProgramFile&                    programFile
            ) const override;

            /**
             * Method you can overload to parse incoming XML attributes.  This method will only be called if there are
             * provided attributes.
             *
             * Errors are reported directly to the reader using the raiseError method.
             *
             * \param[in]     reader      The reader used to parse the incoming XML stream.
             *
             * \param[in]     attributes  The XML attributes to be parsed.
             *
             * \param[in]     formats     A map of format instances by format identifier.  You can use this parameter
             *                            to either tie a format to an object or add a new format to the list of known
             *                            formats.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             *
             * \param[in]     xmlVersion  The XML major version code.  The value can be used to modify how the incoming
             *                            XML stream is parsed.
             */
            void readAttributes(
                QSharedPointer<XmlReader>  reader,
                const XmlAttributes&       attributes,
                const FormatsByIdentifier& formats,
                ProgramFile&               programFile,
                unsigned                   xmlVersion
            ) override;

        private:
            /**
             * The text held by this element, regions 1 and 2.
             */
            QString currentText[2];

            /**
             * A weak pointer to the \ref Identifier tracking this variable.
             */
            IdentifierContainer currentIdentifier;

            /**
             * The current calculated value for this field.
             */
            CalculatedValue currentCalculatedValue;
    };
};

#endif
