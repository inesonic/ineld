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
* This header defines the \ref Ld::FontFormat class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_FONT_FORMAT_H
#define LD_FONT_FORMAT_H

#include <QtGlobal>
#include <QString>
#include <QSharedPointer>
#include <QColor>
#include <QFont>
#include <QMap>

#include <util_hash_functions.h>

#include "ld_common.h"
#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_attributes.h"
#include "ld_format.h"

namespace Ld {
    /**
     * Class used to track information about the font used by elements in a program.  This class is designed to also
     * be used as a sub-format for other complex format types.
     */
    class LD_PUBLIC_API FontFormat:public virtual Format {
        friend LD_PUBLIC_API Util::HashResult qHash(const FontFormat& fontFormat, Util::HashSeed seed);

        public:
            /**
             * Enumeration of supported font weights.  Note that the weights align with the vaues used by the Qt QFont
             * class and roughly align with the values used by CSS.
             */
            enum class Weight : quint8 {
                /**
                 * Indicates an extra-light, thin font.
                 */
                THIN = 0,

                /**
                 * Indicates an extra light font.
                 */
                EXTRA_LIGHT = 12,

                /**
                 * A light font.
                 */
                LIGHT = 25,

                /**
                 * A normal font.
                 */
                NORMAL = 50,

                /**
                 * A medium weight font.
                 */
                MEDIUM = 57,

                /**
                 * Boldfaced font.
                 */
                DEMI_BOLD = 63,

                /**
                 * Boldfaced font.
                 */
                BOLD = 75,

                /**
                 * An extra bold-faced font.
                 */
                EXTRA_BOLD = 81,

                /**
                 * A black font.
                 */
                BLACK = 87,

                /**
                 * Value used to indicate an invalid font weight.
                 */
                INVALID = 255
            };

            /**
             * Value that indicates the minimum allowed font size, in points.
             */
            static const unsigned minimumFontSize;

            /**
             * Value that indicates the maximum allowed font size, in points.
             */
            static const unsigned maximumFontSize;

            /**
             * Value that indicates the minimum allowed letter spacing value.
             */
            static const float minimumLetterSpacing;

            /**
             * Value that indicates the maximum allowed letter spacing value.
             */
            static const float maximumLetterSpacing;

            /**
             * Class you can use to track an aggregation of multiple \ref Ld::FontFormat instances.
             */
            class LD_PUBLIC_API Aggregation:public virtual Format::Aggregation {
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
                    QString typeName() const override;

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
                     * Method you can use to obtain all the font families in the aggregation.
                     *
                     * \return Returns a set of font family names.
                     */
                    const QSet<QString>& families() const;

                    /**
                     * Method you can use to obtain all the font sizes in the aggregation.
                     *
                     * \return Returns a set of font sizes in the aggregation.
                     */
                    const QSet<unsigned>& sizes() const;

                    /**
                     * Method you can use to obtain all the font weights in the aggregation.
                     *
                     * \return Returns all the font weights in the aggregation.
                     */
                    const QSet<FontFormat::Weight>& weights() const;

                    /**
                     * Method you can use to determine if fonts are italiced or not.
                     *
                     * \return Returns the state of italic values.
                     */
                    const Format::BooleanAggregation& italics() const;

                    /**
                     * Method you can use to determine if fonts are underlined or not.
                     *
                     * \return Returns the state of the underline values.
                     */
                    const Format::BooleanAggregation& underline() const;

                    /**
                     * Method you can use to determine if fonts are overlined or not.
                     *
                     * \return Returns the state of the overline values.
                     */
                    const Format::BooleanAggregation& overline() const;

                    /**
                     * Method you can use to determine if fonts are striked out.
                     *
                     * \return Returns the state of the strikeout values.
                     */
                    const Format::BooleanAggregation& strikeOut() const;

                    /**
                     * Method you can use to determine the letter spacing values.
                     *
                     * \return Returns a set of unique letter spacing values.
                     */
                    const QSet<float>& letterSpacing() const;

                    /**
                     * Method you can use to determine the supported Qt fonts.
                     *
                     * \return Returns a set of supported letter fonts.
                     */
                    const QSet<QFont>& qtFonts() const;

                    /**
                     * Method you can use to determine the font color values.
                     *
                     * \return Returns a set of unique font color values.
                     */
                    const QSet<QColor>& fontColor() const;

                    /**
                     * Method you can use to determines the font background values.
                     *
                     * \return Returns a set of unique font background color values.
                     */
                    const QSet<QColor>& fontBackgroundColor() const;

                private:
                    /**
                     * Set of font families.
                     */
                    QSet<QString> currentFontFamilies;

                    /**
                     * Set of font sizes.
                     */
                    QSet<unsigned> currentFontSizes;

                    /**
                     * Set of font weights.
                     */
                    QSet<FontFormat::Weight> currentWeights;

                    /**
                     * Current italics state.
                     */
                    Format::BooleanAggregation currentItalics;

                    /**
                     * Current underline state.
                     */
                    Format::BooleanAggregation currentUnderline;

                    /**
                     * Current overline state.
                     */
                    Format::BooleanAggregation currentOverline;

                    /**
                     * Current strikeout state.
                     */
                    Format::BooleanAggregation currentStrikeOut;

                    /**
                     * Set of letter spacing values.
                     */
                    QSet<float> currentLetterSpacing;

                    /**
                     * Set of Qt font instances represented by the aggregation.
                     */
                    QSet<QFont> currentQtFonts;

                    /**
                     * Set of font color values.
                     */
                    QSet<QColor> currentFontColor;

                    /**
                     * Set of font background color values.
                     */
                    QSet<QColor> currentFontBackgroundColor;
            };

            /**
             * The typename for the format.
             */
            static const QString formatName;

            /**
             * The default font family name.
             */
            static const char defaultFontFamily[];

            /**
             * The default font size, in points.
             */
            static const unsigned defaultFontSizePoints;

            /**
             * The default font weight.
             */
            static const FontFormat::Weight defaultFontWeight;

            /**
             * The default letter spacing value.
             */
            static const float defaultLetterSpacing;

            /**
             * The default font color.
             */
            static const QColor defaultFontColor;

            /**
             * The default font background color.
             */
            static const QColor defaultFontBackgroundColor;

            FontFormat();

            /**
             * Constructor
             *
             * \param[in] family    The desired font family.
             *
             * \param[in] pointSize The desired font size, in points.  A negative value will cause the system default
             *                      font size to be used.
             *
             * \param[in] weight    The desired font weight.
             *
             * \param[in] italic    If true, the font will be italics, if false the font will not be italics.
             */
            FontFormat(
                const QString& family,
                unsigned       pointSize = defaultFontSizePoints,
                Weight         weight = Weight::NORMAL,
                bool           italic = false
            );

            /**
             * Constructor
             *
             * \param[in] family    The desired font family.
             *
             * \param[in] pointSize The desired font size, in points.  A negative value will cause the system default
             *                      font size to be used.
             *
             * \param[in] weight    The desired font weight.
             *
             * \param[in] italic    If true, the font will be italics, if false the font will not be italics.
             */
            FontFormat(
                const char* family,
                unsigned    pointSize = defaultFontSizePoints,
                Weight      weight = Weight::NORMAL,
                bool        italic = false
            );

            /**
             * Copy constructor.  Note that the copy constructor does not copy elements from the underlying
             * \ref Ld::Format class.  All format parameters are copied by the format are not tied to any element.
             *
             * \param[in] other The instance to be copied.
             */
            FontFormat(const FontFormat& other);

            ~FontFormat();

            /**
             * Overloaded method that creates a clone of this class instance.  Note that the clone method does not copy
             * elements from the underlying \ref Ld::Format class.  All format parameters are copied by the format are
             * not tied to any element.
             *
             * \return Returns a deep copy of this class instance.
             */
            FormatPointer clone() const override;

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
             * \return Returns the format identifying name, in this case, "FontFormat".
             */
            QString typeName() const override;

            /**
             * Overloaded method that returns true to indicate that this format is valid.
             *
             * \return Returns true if the format is valid.  Returns false if the format is invalid.  This version
             *         always returns true.
             */
            bool isValid() const override;

            /**
             * Overloaded method that returns true if this format is set to the default values.
             *
             * \return Returns true if the format holds default values.  Returns false if the format does not hold
             *         default values.
             */
            bool isDefault() const;

            /**
             * Overloaded method that returns true if this format is not set to the default values.
             *
             * \return Returns true if the format holds non-default values.  Returns false if the format holds default
             *         values.
             */
            bool isNotDefault() const;

            /**
             * Method you can call to determine the capabilities of this format.  The capabilities is a set containing
             * all the classes that make up the format instance.
             *
             * \return Returns a \ref Ld::Format::Capabilities instance containing the names of all the classes that
             *         define this format.
             */
            Capabilities capabilities() const override;

            /**
             * Method you can use to change the font family.
             *
             * \param[in] newFamily The new font family to be used.
             */
            void setFamily(const QString& newFamily);

            /**
             * Convenience method you can use to change the font family.
             *
             * \param[in] newFamily The new font family to be used.
             */
            void setFamily(const char* newFamily);

            /**
             * Method you can use to obtain the font family.
             *
             * \return Returns the font family.
             */
            QString family() const;

            /**
             * Method you can use to set the font size, in points.
             *
             * \param[in] newPointSize The new font size, in points.
             */
            void setFontSize(unsigned newPointSize);

            /**
             * Method you can use to obtain the font size, in points.
             *
             * \return Returns the font size, in points.
             */
            unsigned fontSize() const;

            /**
             * Method you can use to set the font weight.
             *
             * \param[in] newWeight The new font weight.
             */
            void setFontWeight(Weight newWeight);

            /**
             * Method you can use to obtain the font weight.
             *
             * \return Returns the font weight.
             */
            Weight fontWeight() const;

            /**
             * Method you can use to italicize a font.
             *
             * \param[in] nowItalics If true, the font will be italicized.  If false, the font will not be italicized.
             */
            void setItalics(bool nowItalics = true);

            /**
             * Method you can use to determine if the font is italicized.
             *
             * \return Returns true if the font is italicized.  Returns false if the font is not italicized.
             */
            bool italics() const;

            /**
             * Method you can use to underline a font.
             *
             * \param[in] nowUnderlined If true, the font will be underlined.  If false, the font will not be
             *                          underlined.
             */
            void setUnderline(bool nowUnderlined = true);

            /**
             * Method you can use to determine if the font is underlined.
             *
             * \return Returns true if the font is underlined.  Returns false if the font is not underlined.
             */
            bool underline() const;

            /**
             * Method you can use to overline a font.
             *
             * \param[in] nowOverlined If true, the font will be overlined.  If false, the font will not be overlined.
             */
            void setOverline(bool nowOverlined = true);

            /**
             * Method you can use to determine if the font is overlined.
             *
             * \return Returns true if the font is overlined.  Returns false if the font is not overlined.
             */
            bool overline() const;

            /**
             * Method you can use to strikeout a font.
             *
             * \param[in] nowStrikeout If true, the font will be struck out.  If false, the font will not be struck out.
             */
            void setStrikeout(bool nowStrikeout = true);

            /**
             * Method you can use to determine if the font should be struck-out.
             *
             * \return Returns true if the font should be struck-out.  Returns false if the font should not be struct
             *         out.
             */
            bool strikeout() const;

            /**
             * Method you can use to set additional spacing between letters in the text.
             *
             * \param[in] newSpacing The new spacing factor.  A value of 0 indicates no additional spacing.  A value of
             *                       1 adds a full character of additional spacing.  You will normally want small values
             *                       such as 0, or 0.1 for this parameter.
             */
            void setLetterSpacing(float newSpacing);

            /**
             * Method you can use to determine the current additional font spacing.
             *
             * \return Returns the current font spacing.  A value of 0 indicates no additional font spacing.
             */
            float letterSpacing() const;

            /**
             * Method you can use to clear all formatting flags such as underline, italics, strikethrough, etc. for this
             * character format.
             */
            void clearFormatting();

            /**
             * Method you can use to set the font color.
             *
             * \param[in] newFontColor The new font color to assign.  An invalid color indicates that the default
             *                         color should be used.
             */
            void setFontColor(const QColor& newFontColor);

            /**
             * Method you can use to obtain the font color data.
             *
             * \return The font color to be used.  An invalid color is returned if the default color should be used.
             */
            QColor fontColor() const;

            /**
             * Method you can use to set the font background color.
             *
             * \param[in] newFontBackgroundColor The new font background color to assign.  An invalid color indicates
             *                                   that no background should be used.
             */
            void setFontBackgroundColor(const QColor& newFontBackgroundColor);

            /**
             * Method you can use to obtain the font background color data.
             *
             * \return The font color to be used.  An invalid color indicates that no background or a transparent
             *         background should be applied.
             */
            QColor fontBackgroundColor() const;

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
             * Method you can use to convert this \ref Ld::FontFormat instance to a QFont.  Note that an instance of
             * QGuiApplication must have been instantiated before this method is called.
             *
             * \return Returns a QFont instance associated with the font.
             */
            QFont toQFont();

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            FontFormat& operator=(const FontFormat& other);

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instances are equal.  Returns false if the instances are different.
             */
            bool operator==(const FontFormat& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instances are equal.  Returns false if the instances are different.
             */
            bool operator!=(const FontFormat& other) const;

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
             * Class that holds immutable data associated with each character format.
             */
            class ImmutableData;

            /**
             * Class that holds mutable data associated with each character format.
             */
            class MutableData;

            /**
             * Method that performs initialization that is common to all constructors.
             *
             * \param[in] family    The desired font family.
             *
             * \param[in] pointSize The desired font size, in points.  A negative value will cause the system default
             *                      font size to be used.
             *
             * \param[in] weight    The desired font weight.
             *
             * \param[in] italic    If true, the font will be italics, if false the font will not be italics.
             */
            void configure(const QString& family, unsigned pointSize, Weight weight, bool italic);

            /**
             * Method that creates a copy of the character format and then adjusts the current reference count for the
             * character format under the assumption that a different character format will be created and added to the
             * database.
             *
             * \return Returns the immutable character format data.
             */
            Ld::FontFormat::ImmutableData obtainImmutableFontFormatData();

            /**
             * Method that updates the character format data information referenced by this
             * \ref Ld::FontFormat instance.
             *
             * \param[in] newFontFormatData An updated version of the character format data to be managed in the
             *                                   map.
             */
            void updateFontFormatDatabase(Ld::FontFormat::ImmutableData& newFontFormatData);

            /**
             * Global that holds all the known character formats.  Character format data is stored in a map where the
             * immutable data is the key into the map and the modifiable or temporal values are treated as the data.
             */
            static QMap<ImmutableData, MutableData> fontFormats;

            /**
             * Iterator that points to this character format in the set of global character formats.
             */
            QMap<ImmutableData, MutableData>::iterator iterator;
    };

    /**
     * Function that converts a QFont::Weight value to an \ref Ld::FontFormat::Weight value.
     *
     * \param[in] qFontWeight The QFont::Weight value to be converted.
     *
     * \return Returns the \ref Ld::FontFormat::Weight value.
     */
    LD_PUBLIC_API FontFormat::Weight toFontFormatWeight(QFont::Weight qFontWeight);

    /**
     * Function that converts an \ref Ld::FontFormat::Weight value to a QFont::Weight value.
     *
     * \param[in] fontWeight The \ref Ld::FontFormat::Weight value to be converted.
     *
     * \return Returns the translated QFont::Weight value.
     */
    LD_PUBLIC_API QFont::Weight toQFontWeight(FontFormat::Weight fontWeight);

    /**
     * Hash function used to generate hashes of font weight values.
     *
     * \param[in] weight The weight value to calculate a hash for.
     *
     * \param[in] seed   An optional seed you can apply to the hash.
     *
     * \return Returns a hash for the provided weight.
     */
    LD_PUBLIC_API Util::HashResult qHash(FontFormat::Weight weight, Util::HashSeed seed = 0);

    /**
     * Hash function used to generate hashes of font formats values.
     *
     * \param[in] fontFormat The weight value to calculate a hash for.
     *
     * \param[in] seed       An optional seed you can apply to the hash.
     *
     * \return Returns a hash for the provided weight.
     */
    LD_PUBLIC_API Util::HashResult qHash(const FontFormat& fontFormat, Util::HashSeed seed = 0);
};

#endif
