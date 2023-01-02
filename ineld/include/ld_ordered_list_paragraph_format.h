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
* This header defines the \ref Ld::OrderedListParagraphFormat class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_ORDERED_LIST_PARAGRAPH_FORMAT_H
#define LD_ORDERED_LIST_PARAGRAPH_FORMAT_H

#include <QString>
#include <QSharedPointer>
#include <QSet>

#include <cstdint>

#include <util_hash_functions.h>

#include "ld_common.h"
#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_attributes.h"
#include "ld_list_paragraph_format_base.h"

namespace Ld {
    /**
     * Class that extends the \ref Ld::ListParagraphFormatBase class to support ordered lists.
     */
    class LD_PUBLIC_API OrderedListParagraphFormat:public virtual ListParagraphFormatBase {
        public:
            /**
             * Enumeration of list numbering styles.
             */
            enum class Style {
                /**
                 * Indicates an invalid style.
                 */
                INVALID,

                /**
                 * Indicates simple numerical numbering: 1, 2, 3, 4, 5, etc.
                 */
                NUMBER,

                /**
                 * Indicates use of lowercase letters for numbering: a, b, c, d, ..., aa, ab, ac, ...
                 */
                LOWER_CASE,

                /**
                 * Indicates use of uppercase letters for numbering: A, B, C, D, ..., AA, AB, AC, ...
                 */
                UPPER_CASE,

                /**
                 * Indicates use of upper case roman numerals for numbering:
                 *     I, II, III, IV, V, VI, VII, VIII, IX, X, XI, ...
                 */
                ROMAN_UPPER_CASE,

                /**
                 * Indicates use of lower case roman numerals for numbering:
                 *     i, ii, iii, iv, v, vi, vii, viii, ix, x, xi, ...
                 */
                ROMAN_LOWER_CASE
            };

            /**
             * Class you can use to track an aggregation of multiple \ref Ld::OrderedListParagraphFormat instances.
             */
            class LD_PUBLIC_API Aggregation:public virtual ListParagraphFormatBase::Aggregation {
                public:
                    Aggregation();

                    ~Aggregation() override;

                    /**
                     * Creator function for this aggregator.
                     *
                     * \param[in] formatName The name of the format to create an aggregator for.
                     *
                     * \return Returns a pointer to the newly created aggregator.
                     */
                    static Format::Aggregation* creator(const QString& formatName);

                    /**
                     * Method that returns the type name of the format tracked by this aggregation.
                     *
                     * \return Returns the format name for formats tied to this aggregation.
                     */
                    QString typeName() const final;

                    /**
                     * Method you can use to clear the aggregation.  Calling the base class instance will clear the
                     * underlying format database.
                     */
                    void clear() override;

                    /**
                     * Virtual method you can call to add a format instance to the aggregation.  Derived class
                     * derived class instances should call this method to add the format to the aggregation.
                     *
                     * \param[in] formatInstance  The format instance to be added.
                     *
                     * \param[in] includeExisting If true, this format will be included in the aggregation even if it
                     *                            has already been accounted for.
                     *
                     * \return Returns true if the format was added to the database.  Returns false if the format is
                     *         already contained in the underlying database.
                     */
                    bool addFormat(FormatPointer formatInstance, bool includeExisting = false) override;

                    /**
                     * Method you can use to determine the number styles in the aggregation.
                     *
                     * \return Returns a set of number styles in the aggregation.
                     */
                    QSet<Style> styles() const;

                    /**
                     * Method you can use to determine the numerical values at the top of the list.
                     *
                     * \return Returns the numerical values at the top of the list.
                     */
                    QSet<unsigned long> startingNumbers() const;

                    /**
                     * Method you can use to determine the prefixes used for the list(s).
                     *
                     * \return Returns a set of number prefixes.
                     */
                    QSet<QString> prefixes() const;

                    /**
                     * Method you can use to determine the suffixes used for the lists.
                     *
                     * \return Returns a set of number suffixes.
                     */
                    QSet<QString> suffixes() const;

                private:
                    /**
                     * The current list of unique number styles.
                     */
                    QSet<Style> currentStyles;

                    /**
                     * The set of starting list numbers.
                     */
                    QSet<unsigned long> currentStartingNumbers;

                    /**
                     * The set of number prefixes.
                     */
                    QSet<QString> currentPrefixes;

                    /**
                     * The set of number suffixes.
                     */
                    QSet<QString> currentSuffixes;
            };

            /**
             * The typename for the format.
             */
            static const QString formatName;

            /**
             * The default left indentation setting.
             */
            static const Style defaultNumberStyle;

            /**
             * The default prefix.
             */
            static const QString defaultPrefix;

            /**
             * The default suffix.
             */
            static const QString defaultSuffix;

            OrderedListParagraphFormat();

            /**
             * Copy constructor.  Note that the copy constructor does not copy elements from the underlying
             * \ref Ld::Format class.  All format parameters are copied by the format are not tied to any element.
             *
             * \param[in] other The instance to be copied.
             */
            OrderedListParagraphFormat(const OrderedListParagraphFormat& other);

            ~OrderedListParagraphFormat() override;

            /**
             * Overloaded method that creates a clone of this class instance.  Note that the clone method does not copy
             * elements from the underlying \ref Ld::Format class.  All format parameters are copied by the format are
             * not tied to any element.
             *
             * \return Returns a deep copy of this class instance.
             */
            FormatPointer clone() const final;

            /**
             * Factory method that can be used to create a default instance of this format.
             *
             * \param[in] formatType The name used to reference this creator function.
             *
             * \return Returns a newly created, default, format instance.
             */
            static Format* creator(const QString& formatType);

            /**
             * Overloaded method that returns a name for this format.  The value is used to track and categorize
             * different types of format data.
             *
             * \return Returns the format identifying name, in this case, "BlockFormat".
             */
            QString typeName() const final;

            /**
             * Method you can call to determine the capabilities of this format.  The capabilities is a set containing
             * all the classes that make up the format instance.
             *
             * \return Returns a \ref Ld::Format::Capabilities instance containing the names of all the classes that
             *         define this format.
             */
            Capabilities capabilities() const final;

            /**
             * Method you can use to set the number style for the list.
             *
             * \param[in] newStyle The new number style to apply to the list.
             */
            void setStyle(Style newStyle);

            /**
             * Method you can use to determine the desired style.
             *
             * \return Returns the indicated number style.
             */
            Style style() const;

            /**
             * Method you can use to set the starting number for the list.
             *
             * \param[in] newStartingNumber The new starting number for the list.  A value of zero should represent the
             *                              first value such as "1", "A", "I" depending on the style.
             */
            void setStartingNumber(unsigned long newStartingNumber);

            /**
             * Method you can use to determine the starting number for the list.
             *
             * \return Returns the starting number for the list. A value of zero represent the first value such as "1",
             *         "A", "I" depending on the style.
             */
            unsigned long startingNumber() const;

            /**
             * Method you can use to set the number prefix.
             *
             * \param[in] newPrefix The new string to use as the number prefix.
             */
            void setPrefix(const QString& newPrefix);

            /**
             * Method you can use to obtain the current number prefix.
             *
             * \return Returns the current number prefix.
             */
            QString prefix() const;

            /**
             * Method you can use to set the number suffix.
             *
             * \param[in] newSuffix The new string to use as the number suffix.
             */
            void setSuffix(const QString& newSuffix);

            /**
             * Method you can use to obtain the current number suffix.
             *
             * \return Returns the current number suffix.
             */
            QString suffix() const;

            /**
             * Method that returns a description of this format as a string.  The string format should be in the form of
             * a comma separated group of fields with the first field being the type-name of the format.
             *
             * While not always supported, the string should be such that the format can be constructed later from the
             * string.
             *
             * \return Returns a description of this format as a string.
             */
            QString toString() const override;

            /**
             * Method that returns CSS content that can be used to emulate this format.  The returned string contains
             * no newline and does not include leading or trailing braces.
             *
             * \return Returns CSS content comparable to this format.
             */
            QString toCss() const override;

        protected:
            /**
             * Method you can optionally overload to add additional attributes to the XML description of this element.
             * This method should write attributes for this class only and not the class's children.  To cleanly handle
             * multiple virtual inheritance, the \ref Ld::Format::writeAddAttributes method should be coded to call the
             * appropriate child class methods before calling this method.
             *
             * \param[in]     attributes  A reference to the list of attributes to be modified.
             *
             * \param[in]     formats     A \ref FormatOrganizer instance used to track all known formats in the
             *                            program.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             */
            void writeAddImmediateAttributes(
                XmlAttributes&                  attributes,
                QSharedPointer<FormatOrganizer> formats,
                ProgramFile&                    programFile
            ) const override;

            /**
             * Method you can overload to parse incoming XML attributes.  This method will only be called if there are
             * provided attributes.  To cleanly handle multiple virtual inheritance, the
             * \ref Ld::Format::readAttributes method should be coded to call the appropriate child class methods
             * before calling this method.
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
            void readImmediateAttributes(
                QSharedPointer<XmlReader>  reader,
                const XmlAttributes&       attributes,
                const FormatsByIdentifier& formats,
                ProgramFile&               programFile,
                unsigned                   xmlVersion
            ) override;

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
             * Method that converts a style to a textual representation.
             *
             * \param[in] style The style to convert.
             *
             * \return Returns the name of the style.
             */
            static QString toString(Style style);

            /**
             * Method that converts a textual name of a style to a style enumeration value.
             *
             * \param[in] textualRepresentation The name of the style in text form.
             *
             * \return Returns the style associated with the text.  A value of
             *         \ref Ld::OrderedListParagraphFormat::Style::INVALID is returned if the textual representation is
             *         invalid.
             */
            static Style toStyle(const QString& textualRepresentation);

            /**
             * The current number style.
             */
            Style currentStyle;

            /**
             * The starting number for the list.
             */
            unsigned long currentStartingNumber;

            /**
             * The number prefix to apply.
             */
            QString currentPrefix;

            /**
             * The number suffix to apply.
             */
            QString currentSuffix;
    };

    /**
     * Hash function for the \ref Ld::OrderedListParagraphFormat::Style enumeration.
     *
     * \param[in] value The value to be hashed.
     *
     * \param[in] seed  An optional seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    LD_PUBLIC_API Util::HashResult qHash(OrderedListParagraphFormat::Style value, Util::HashSeed seed = 0);
}

#endif
