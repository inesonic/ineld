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
* This header defines the \ref Ld::LiteralElement class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_LITERAL_ELEMENT_H
#define LD_LITERAL_ELEMENT_H

#include <QString>
#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QSharedPointer>
#include <QSet>
#include <QList>
#include <QRegularExpression>

#include <model_variant.h>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_data_type.h"
#include "ld_capabilities.h"
#include "ld_xml_writer.h"
#include "ld_xml_attributes.h"
#include "ld_element_with_no_children.h"

namespace Ld {
    class LiteralVisual;
    class ElementCursor;
    class Cursor;
    class CursorStateCollection;

    /**
     * Element that is used to represent a numeric literal.
     *
     * Numeric literals support several common formats:
     *
     * [+-]?(0x|0b)?[0-9]*\.?[0-9]*([+-]
     * +1.23
     * -1.23
     *
     */
    class LD_PUBLIC_API LiteralElement:public ElementWithNoChildren {
        Q_DECLARE_TR_FUNCTIONS(Ld::LiteralElement)

        public:
            /**
             * Type used to represent different sections of a literal.
             */
            enum class SectionType {
                /**
                 * Indicates an invalid/unknown section.
                 */
                INVALID,

                /**
                 * Indicates a string.  A value of this type precludes any other section.
                 */
                STRING_CONTENT,

                /**
                 * Indicates a sign for the mantissa.
                 */
                MANTISSA_SIGN,

                /**
                 * Indicates a sign for the exponent.
                 */
                EXPONENT_SIGN,

                /**
                 * Indicates a mantissa.
                 */
                MANTISSA_VALUE,

                /**
                 * Indicates an exponent value.
                 */
                EXPONENT_VALUE,

                /**
                 * Indicates the exponent symbol.
                 */
                EXPONENT_SYMBOL,

                /**
                 * Indicates an imaginary symbol indicator.
                 */
                IMAGINARY_SYMBOL
            };

            /**
             * Class used to track a section of a literal.
             */
            class LD_PUBLIC_API Section:public QString {
                public:
                    Section();

                    /**
                     * Constructor
                     *
                     * \param[in] text        The text to track as part of this section.
                     *
                     * \param[in] sectionType The section type to be tracked.
                     */
                    Section(const QString& text, SectionType sectionType);

                    /**
                     * Copy constructor
                     *
                     * \param[in] other The instance to be copied.
                     */
                    Section(const Section& other);

                    /**
                     * Copy constructor (move semantics)
                     *
                     * \param[in] other The instance to be moved.
                     */
                    Section(const Section&& other);

                    ~Section();

                    /**
                     * Method you can use to determine the tracked section type.
                     *
                     * \return Returns the tracked section type.
                     */
                    SectionType sectionType() const;

                    /**
                     * Assignment operator
                     *
                     * \param[in] other The instance to be assigned to this instance.
                     *
                     * \return Returns a reference to this instance.
                     */
                    Section& operator=(const Section& other);

                private:
                    /**
                     * The tracked section type.
                     */
                    SectionType currentSectionType;
            };

            /**
             * Type used to represent a list of sections.
             */
            typedef QList<Section> SectionList;

            /**
             * The typename for the text element.
             */
            static const QString elementName;

            /**
             * Static value indicating the capabilities provides by this element.
             */
            static const Capabilities childProvides;

            LiteralElement();

            ~LiteralElement() override;

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
             * \return Returns the value type this element would represent.  This version determines the value type by
             *         examining the current text content.
             */
            DataType::ValueType valueType() const override;

            /**
             * Method you can call to determine what capabilities this element will provide.  Note that this object
             * also provides a static constant value named "childProvides" returning the same information.
             *
             * \return Returns the requirements met by this element.
             */
            Capabilities childProvidesCapabilities() const override;

            /**
             * Method you can use to determine the precedence for this element.
             *
             * \return Returns the element's precedence.
             */
            Precedence childPrecedence() const override;

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
            Capabilities textCapabilities(unsigned long textOffset, unsigned regionIndex) const override;

            /**
             * Method you can use to change the object used to manage visual representation of this element.  Note that
             * using this method will also cause the \ref Visual::element method to point back to this
             * \ref LiteralElement instance.
             *
             * If the newly added \ref LiteralVisual already points to a different element, then those connections
             * will be broken in order to maintain a 1:1 relationship between any \ref LiteralVisual instance and
             * \ref LiteralElement instance.
             *
             * If this element currently has an associated visual, then that visual be will disconnected from this
             * element.  The disconnected visual will *not* be deleted.
             *
             * \param[in] newVisual A pointer to the new visual object.  Note that this element will take ownership of
             *                      the visual.
             */
            void setVisual(LiteralVisual* newVisual);

            /**
             * Method you can use to obtain the visual representation of this element.
             *
             * \return Returns a pointer to the visual representation of this element.  A null pointer is returned if
             *         no visual representation has been defined.
             */
            LiteralVisual* visual() const;

            /**
             * Method that can be used to determine the minimum number of distinct text regions used by this element.
             *
             * \return Returns the minimum number of text regions.
             */
            unsigned minimumNumberTextRegions() const override;

            /**
             * Method that can be used to determine the maximum number of distinct text regions used by this element.
             *
             * \return Returns the maximum number of text regions.
             */
            unsigned maximumNumberTextRegions() const override;

            /**
             * Method you can use to set the number of expected text regions.
             *
             * \param[in] newNumberTextRegions The new number of text regions.
             *
             * \return Returns true on success, returns false if the number of text regions is invalid.
             */
            bool setNumberTextRegions(unsigned newNumberTextRegions) override;

            /**
             * Method you can use to obtain the current number of text regions.
             *
             * \return Returns the current number of text regions.
             */
            unsigned numberTextRegions() const override;

            /**
             * Method that sets the text associated with a text region of this element.
             *
             * \param[in] newText      The new text to associate with this element and region.
             *
             * \param[in] regionNumber The zero based region number.
             *
             * \return Returns true if the region is valid.  Returns false if the region is invalid.
             */
            bool setText(const QString& newText, unsigned regionNumber = 0) override;

            /**
             * Method you can use to obtain the text associated with a text region of this element.
             *
             * \param[in] regionNumber The zero based region number.
             *
             * \return Returns the text currently associated with this element's region.  Returns empty text if the
             *         region is invalid.
             */
            QString text(unsigned regionNumber = 0) const override;

            /**
             * Method that indicates if the \ref Ld::Cursor can place the cursor on this element.
             *
             * \return Returns the allowed cursor park mode.  The default implementation returns
             *         \ref CursorParkMode::PARK_ON_TEXT_ONLY.
             */
            CursorParkMode cursorParkMode() const override;

            /**
             * Method that converts a text string to a numeric literal value.
             *
             * \param[in]  text The text to be converted.
             *
             * \return Returns an Model::Variant holding the converted value.
             *         A value type of Model::ValueType::NONE will be returned if the provided text is
             *         invalid.
             */
            static Model::Variant convert(const QString& text);

            /**
             * Method that determines if we can convert a string to a valid literal value.
             *
             * \param[in]  text The text to be converted.
             *
             * \return Returns true if the string can be converted.  Returns false if the string can-not be converted.
             */
            static bool canConvert(const QString& text);

            /**
             * Method that converts a text string into a group of distinct sections.
             *
             * \param[in] text The text to be converted.
             *
             * \return Returns a list of section instances.
             */
            static SectionList section(const QString& text);

            /**
             * Method that converts the current text string to real and complex values.
             *
             * \param[out] real      Variable that will be populated with the real portion of the result.
             *
             * \param[out] imaginary Variable that will be populated with the imaginary portion of the result.
             *
             * \return Returns an Model::Variant holding the converted value.
             *         A value type of DataType::ValueType::NONE will be returned if the provided text is invalid.
             */
            Model::Variant convert() const;

            /**
             * Method that converts the current text string into a group of distinct sections.
             *
             * \param[in] text The text to be converted.
             *
             * \return Returns a list of section instances.
             */
            SectionList section() const;

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

            /**
             * Method you can overload to include PCDATA into the XML description of this element.  Returning an empty
             * string will cause PCDATA to be omitted.
             *
             * \param[in]     formats     A \ref FormatOrganizer instance used to track all known formats in the
             *                            program.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             *
             * \return Returns the PCDATA to be included in the written XML description.  Note that the XML writer will
             *         automatically translate special characters such as ">" to their XML escaped sequences (e.g.
             *         &gt;).
             */
            virtual QString writeData(
                QSharedPointer<const FormatOrganizer> formats,
                ProgramFile&                          programFile
            ) const override;

            /**
             * Method you can overload to parse incoming PCDATA tied to the XML description for this object.  This
             * method will only be called if non-empty PCDATA is provided.
             *
             * Errors can be reported directly to the reader using the raiseError method.
             *
             * \param[in]     reader       The reader used to parse the incoming XML stream.
             *
             * \param[in]     text         The PCDATA to be parsed.  XML tokens such as &gt; are pre-processed and
             *                             converted prior to the call to this method.
             *
             * \param[in]     formats      A map of format instances by format identifier.  You can use this parameter
             *                             to either tie a format to an object or add a new format to the list of known
             *                             formats.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             *
             * \param[in]     xmlVersion  The XML major version code.  The value can be used to modify how the incoming
             *                            XML stream is parsed.
             */
            void readData(
                QSharedPointer<XmlReader>  reader,
                const QString&             text,
                const FormatsByIdentifier& formats,
                ProgramFile&               programFile,
                unsigned                   xmlVersion
            ) override;

        private:
            /**
             * Method that returns a number regular expression pattern for real numbers.  This method will provide a
             * string that is suitable to the current locale.
             *
             * \param[in] includeIntegerValues If true, the regular expression will include support for decimal
             *                                 integer values.  If false, the regular expression will exclude support
             *                                 for decimal integer values.
             *
             * \return Returns a regular expression pattern suitable for finding real values.
             */
            static QString realRegularExpressionPatternString(bool includeIntegerValues = false);

            /**
             * Method that converts a string to a real value in the current locale.
             *
             * \param[in]     text The text to be converted.
             *
             * \param[in,out] ok   An optional pointer to a boolean value that will hold true on success, false on
             *                     error.
             *
             * \return Returns the resulting long double result.
             */
            static long double toLongDouble(const QString& text, bool* ok = nullptr);

            /**
             * Method that prepares the integer regular expression pattern.
             */
            static void prepareIntegerRegex();

            /**
             * Method that prepares the real regular expression pattern.
             */
            static void prepareRealRegex();

            /**
             * Method that prepares the complex regular expression pattern.
             */
            static void prepareComplexRegex();

            /**
             * Regular expression used to parse integer literal binary, decimal, or hexidecimal values.
             */
            static QRegularExpression integerRegex;

            /**
             * Regular expression used to parse literal real values.
             */
            static QRegularExpression realRegex;

            /**
             * Regular expression used to parse literal complex values.
             */
            static QRegularExpression complexRegex;

            /**
             * The text held by this element.
             */
            QString currentText;
    };
};

#endif
