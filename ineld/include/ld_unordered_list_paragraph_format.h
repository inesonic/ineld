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
* This header defines the \ref Ld::UnorderedListParagraphFormat class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_UNORDERED_LIST_PARAGRAPH_FORMAT_H
#define LD_UNORDERED_LIST_PARAGRAPH_FORMAT_H

#include <QString>
#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QSharedPointer>
#include <QSet>

#include <cstdint>

#include "ld_common.h"
#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_attributes.h"
#include "ld_list_paragraph_format_base.h"

namespace Ld {
    /**
     * Class that extends the \ref Ld::ListParagraphFormatBase class to support unordered lists.
     */
    class LD_PUBLIC_API UnorderedListParagraphFormat:public virtual ListParagraphFormatBase {
        Q_DECLARE_TR_FUNCTIONS(Ld::UnorderedListParagraphFormat)

        public:
            /**
             * Class you can use to track an aggregation of multiple \ref Ld::UnorderedListParagraphFormat instances.
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
                     * Method you can use to obtain every unique list indentation setting in the aggregation.
                     *
                     * \return Returns a set of list indentation setting values.
                     */
                    QSet<QString> bulletStyles() const;

                private:
                    /**
                     * The current list of unique list indentation values.
                     */
                    QSet<QString> currentBulletStyles;
            };

            /**
             * Class you can use to obtain information about a standardized bullet.
             */
            class LD_PUBLIC_API StandardBullet {
                friend class UnorderedListParagraphFormat;

                public:
                    /**
                     * Type used to represent a unicode value.
                     */
                    typedef std::uint16_t UnicodeValue;

                    /**
                     * Enumeration of bullet style categories.
                     */
                    enum class Category {
                        /**
                         * Indicates an invalid bullet type.
                         */
                        INVALID,

                        /**
                         * Indicates a filled disc.
                         */
                        FILLED_DISC,

                        /**
                         * Indicated an circle or unfilled disc.
                         */
                        UNFILLED_DISC,

                        /**
                         * Indicates a filled square.
                         */
                        FILLED_SQUARE,

                        /**
                         * Indicatea an unfilled square.
                         */
                        UNFILLED_SQUARE,

                        /**
                         * Indicates a filled triangle.
                         */
                        FILLED_TRIANGLE,

                        /**
                         * Indicates an unfilled triangle.
                         */
                        UNFILLED_TRIANGLE,

                        /**
                         * Indicates a filled diamond.
                         */
                        FILLED_DIAMOND,

                        /**
                         * Indicates an unfilled diamond.
                         */
                        UNFILLED_DIAMOND,

                        /**
                         * Indicates a filled star.
                         */
                        FILLED_STAR,

                        /**
                         * Indicates an unfilled star.
                         */
                        UNFILLED_STAR,

                        /**
                         * Indicates a hyphen.
                         */
                        HYPHEN,

                        /**
                         * Indicates an asterisk.
                         */
                        ASTERISK,

                        /**
                         * Indicates a check mark.
                         */
                        CHECKMARK,

                        /**
                         * Indicates another type of symbol.
                         */
                        OTHER
                    };

                    /**
                     * Enumeration of HTML default supported bullet styles.
                     */
                    enum class HtmlCategory {
                        /**
                         * Indicates an invalid value.
                         */
                        INVALID,

                        /**
                         * Indicates an HTML disc bullet.
                         */
                        DISC,

                        /**
                         * Indicates an HTML square bullet.
                         */
                        SQUARE,

                        /**
                         * Indicates an HTML circle bullet.
                         */
                        CIRCLE
                    };

                    StandardBullet();

                    /**
                     * Copy constructor.
                     *
                     * \param[in] other The instance to be copied.
                     */
                    StandardBullet(const StandardBullet& other);

                    ~StandardBullet();

                    /**
                     * Method you can use to determine if this instance is valid.
                     *
                     * \return Returns true if the instance is valid.  Returns false if the instance is invalid.
                     */
                    bool isValid() const;

                    /**
                     * Method you can use to determine if this instance is invalid.
                     *
                     * \return REturns true if the instance is invalid.  Returns false if the instance is valid.
                     */
                    bool isInvalid() const;

                    /**
                     * Method you can use to obtain the unicode value for this bullet.
                     *
                     * \return Returns the unicode value of this bullet.
                     */
                    UnicodeValue unicodeValue() const;

                    /**
                     * Method you can use to obtain a string representation of the bullet.
                     *
                     * \return Returns a string representation of the bullet.
                     */
                    QString bullet() const;

                    /**
                     * Method you can use to obtain a description of this bullet.
                     *
                     * \return Returns a description of the bullet.  Descriptions will be in all lower case.
                     */
                    QString description() const;

                    /**
                     * Method you can use to obtain a category for this bullet.
                     *
                     * \return Returns the bullet's category type.
                     */
                    Category category() const;

                    /**
                     * Method you can use to obtain the best fit HTML 4.01 bullet style.
                     *
                     * \returns the HTML 4.01 compatible bullet style.
                     */
                    HtmlCategory htmlCategory() const;

                    /**
                     * Assignment operator
                     *
                     * \param[in] other The instance to assign to this instance.
                     *
                     * \return Returns a reference to this object.
                     */
                    StandardBullet& operator=(const StandardBullet& other);

                    /**
                     * Comparison operator
                     *
                     * \param[in] other The instance to compare against this instance.
                     *
                     * \return Returns true if the instances are equal.  Returns false if the instances are different.
                     */
                    bool operator==(const StandardBullet& other) const;

                    /**
                     * Comparison operator
                     *
                     * \param[in] other The instance to compare against this instance.
                     *
                     * \return Returns true if the instances are different.  Returns false if the instances are equal.
                     */
                    bool operator!=(const StandardBullet& other) const;

                private:
                    /**
                     * Private constructor used to create new instances of this class.
                     *
                     * \param[in] unicode      The unicode to report for the bullet.
                     *
                     * \param[in] description  A description of the bullet.
                     *
                     * \param[in] category     The bullet category.
                     *
                     * \param[in] htmlCategory The bullet's HTML category.
                     */
                    StandardBullet(
                        UnicodeValue   unicode,
                        const QString& description,
                        Category       category,
                        HtmlCategory   htmlCategory
                    );

                    /**
                     * The unicode value.  A value of 0 indicates that this instance is invalid.
                     */
                    UnicodeValue currentUnicodeValue;

                    /**
                     * The description of this bullet.
                     */
                    QString currentDescription;

                    /**
                     * The bullet category.
                     */
                    Category currentCategory;

                    /**
                     * The bullet's HTML category.
                     */
                    HtmlCategory currentHtmlCategory;
            };

            /**
             * The typename for the format.
             */
            static const QString formatName;

            /**
             * The default bullet.
             */
            static const QString defaultBulletStyle;

            /**
             * The fallback bullet to be used when the default bullet does not appear to exist.
             */
            static const QString fallbackBulletStyle;

            UnorderedListParagraphFormat();

            /**
             * Copy constructor.  Note that the copy constructor does not copy elements from the underlying
             * \ref Ld::Format class.  All format parameters are copied by the format are not tied to any element.
             *
             * \param[in] other The instance to be copied.
             */
            UnorderedListParagraphFormat(const UnorderedListParagraphFormat& other);

            ~UnorderedListParagraphFormat() override;

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
             * Method you can use to set the bullet list style.
             *
             * \param[in] newBulletStyle A string holding the new bullet to use for this paragraph.
             */
            void setBulletStyle(const QString& newBulletStyle);

            /**
             * Method you can use to obtain the current bullet list style.
             *
             * \return Returns a string holding the desired bullet.
             */
            QString bulletStyle() const;

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

            /**
             * Method you can call to generate a CSS entry for this format.  You can optionally overload this class
             * if you wish to modify how CSS entries are created.  Output is suitable for direct use in CSS style
             * sheets.
             *
             * This version will include an additional CSS entry with the :before pseudo-class to define the correct
             * bullet character and font information.
             *
             * \param[in] className The class name to be used.  The class name will be omitted if an empty string is
             *                      provided.
             *
             * \param[in] tag       An optional tag to apply to the CSS entry.  The tag will be omitted if an empty
             *                      string is provided.
             *
             * \param[in] id        An optional ID to apply to the CSS entry.  The ID will be omitted if an empty
             *                      string is provided.
             *
             * \return Returns a string suitable for use as a CSS entry.
             */
            QString toCssEntry(
                const QString& className,
                const QString& tag = QString(),
                const QString& id = QString()
            ) const override;

            /**
             * Convenience method you can use to obtain a list of standard bullet characters.
             *
             * \return Returns a list of standard bullet types.
             */
            static const QList<StandardBullet>& standardBullets();

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
             * A pre-defined list of standard bullet characters.
             */
            static QList<StandardBullet> currentStandardBullets;

            /**
             * The current bullet style.
             */
            QString currentBulletStyle;
    };
}

#endif
