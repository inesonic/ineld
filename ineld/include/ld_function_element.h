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
* This header defines the \ref Ld::FunctionElement class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_FUNCTION_ELEMENT_H
#define LD_FUNCTION_ELEMENT_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_data_type.h"
#include "ld_capabilities.h"
#include "ld_identifier_container.h"
#include "ld_list_element_base.h"

namespace Ld {
    class FunctionVisual;

    /**
     * Class that manages a function declaration or function class.  Use depends on whether the element is on the left
     * or right side of an assignment.
     *
     * Several key notes about this operator:
     *
     *     * Parenthesis represent the contents of the function, not parenthesis around the function itself.
     *     * Because of the above constraint, this operator must interact with the power operator during rendering
     *       so that the power is placed after the function name, not after the parameter(s).
     *     * If desired, you can explicitly insert parenthesis around a function using an appropriate grouping
     *       operator.
     */
    class LD_PUBLIC_API FunctionElement:public ListElementBase {
        public:
            /**
             * The typename for the paragraph element.
             */
            static const QString elementName;

            /**
             * Static value indicating the capabilities provides by this element.
             */
            static const Capabilities childProvides;

            FunctionElement();

            ~FunctionElement() override;

            /**
             * Factory method that can be used to create a default instance of a paragraph element.
             *
             * \param[in] typeName The name used to reference the creator function.
             *
             * \return Returns a newly created, default, paragraph element instance.
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
             * \return Returns the value type this element would represent.
             */
            DataType::ValueType valueType() const override;

            /**
             * Method you can use to obtain the parenthesis style for use by this element around the parameters.  The
             * rules for this element are:
             *
             *     * Parenthesis will be indicated if:
             *         - There is more than one non-placeholder child
             *         - The element is not the last element under an assignment and the function does not have
             *           surrounding parenthesis.
             *         - The format forces parenthesis.
             *
             * If parenthesis are required:
             *
             *     * If the format derives from \ref Ld::ParenthesisFormat, the parenthesis style will be indicated
             *       by the format.
             *     * Otherwise: The parenthesis style will be set to \ref Ld::Element::ParenthesisStyle::DEFAULT
             *
             * \return Returns the required parenthesis style.  The value
             *         \ref Ld::Element::ParenthesisStyle::INVALID_OR_NONE indicates no parenthesis are required.
             */
            ParenthesisStyle parameterParenthesisStyle() const;

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
            Capabilities childProvidesCapabilities() const override;

            /**
             * Method you can use to determine the precedence for this element when comparing against the parent.
             *
             * \return Returns the element's precedence when comparing against the element's parent.  This version
             *         returns \ref Ld::Element::functionOperatorPrecedence.
             */
            Precedence childPrecedence() const override;

            /**
             * Method you can use to determine the precedence for this element when comparing against a child.  The
             * default implementation returns the value reported by \ref Ld::Element::childPrecedence.
             *
             * \param[in] childIndex The index of the child we're comparing against.  This version will return a
             *                       precedence of \ref Ld::Element::defaultPrecedence to keep children from forcibly
             *                       rendering parenthesis.
             */
            Precedence parentPrecedence(unsigned long childIndex) const override;

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
             * Method you can use to change the object used to manage visual representation of this element.  Note that
             * using this method will also cause the \ref Visual::element method to point back to this
             * \ref FunctionElement instance.
             *
             * If the newly added \ref FunctionVisual already points to a different element, then those connections
             * will be broken in order to maintain a 1:1 relationship between any \ref FunctionVisual instance and
             * \ref FunctionElement instance.
             *
             * If this element currently has an associated visual, then that visual be will disconnected from this
             * element.  The disconnected visual will *not* be deleted.
             *
             * \param[in] newVisual A pointer to the new visual object.  Note that this element will take ownership of
             *                      the visual.
             */
            void setVisual(FunctionVisual* newVisual);

            /**
             * Method you can use to obtain the visual representation of this element.
             *
             * \return Returns a pointer to the visual representation of this element.  A null pointer is returned if
             *         no visual representation has been defined.
             */
            FunctionVisual* visual() const;

            /**
             * Method that generates a list of all non-placeholder elements.
             *
             * \return Returns an element list holding the non-placeholder elements.
             */
            ElementPointerList parameterElements() const;

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
             * Method that is called to determine if parenthesis are required based on the element position.
             *
             * \param[in] firstParameterIsSubscripted If true, the first parameter is subscripted.  If false, the
             *                                        first parameter is not subscripted.
             *
             * \return Returns true if parenthesis are required.  Returns false if parenthesis are not required.
             */
            bool parenthesisRequiredBasedOnPosition(bool firstParameterIsSubscripted) const;

            /**
             * Method that generates a list of all non-placeholder value types.
             *
             * \return Returns a list of all the value types for every parameter.
             */
            QList<DataType::ValueType> parameterValueTypes() const;

            /**
             * The text held by this element, regions 1 and 2.
             */
            QString currentText[2];

            /**
             * The current identifier for this function.
             */
            IdentifierContainer currentIdentifier;
    };
};

#endif
