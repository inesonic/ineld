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
* This header defines the \ref Ld::Format class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_FORMAT_H
#define LD_FORMAT_H

#include <QString>
#include <QMap>
#include <QList>
#include <QSet>
#include <QHash>
#include <QSharedPointer>
#include <QWeakPointer>

#include <cstdint>

#include <util_hash_functions.h>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_xml_element.h"
#include "ld_element.h"

namespace Ld {
    class Element;

    /**
     * Pure virtual base class for all classes used to hold visual format information for an element.  The class exists
     * to allow an extensible mechanism for tracking, loading, and saving format information.
     */
    class LD_PUBLIC_API Format:public XmlElement {
        friend class Element;

        public:
            /**
             * Small helper class used to track aggregations of boolean values.
             */
            class LD_PUBLIC_API BooleanAggregation {
                public:
                    /**
                     * Enumeration of numeric values.
                     */
                    enum EnumeratedValue {
                        /**
                         * Indicates an invalid result.  This value is returned if the aggregation is empty.
                         */
                        INVALID = 0,

                        /**
                         * Indicates all provided values were "true"
                         */
                        ALL_TRUE = 1,

                        /**
                         * Indicates all provided values were "false"
                         */
                        ALL_FALSE = 2,

                        /**
                         * Indicates both values were provided.
                         */
                        BOTH = 3
                    };

                    inline BooleanAggregation() {
                        currentValue = EnumeratedValue::INVALID;
                    }

                    /**
                     * Copy constructor
                     *
                     * \param[in] other The value to be copied.
                     */
                    inline BooleanAggregation(const BooleanAggregation& other) {
                        currentValue = other.currentValue;
                    }

                    /**
                     * Method you can call to clear the aggregation.
                     */
                    inline void clear() {
                        currentValue = EnumeratedValue::INVALID;
                    }

                    /**
                     * Method you can use to determine if all provided values were true.
                     *
                     * \return Returns true if all provided values were true.
                     */
                    inline bool allTrue() const {
                        return currentValue == EnumeratedValue::ALL_TRUE;
                    }

                    /**
                     * Method you can use to determine if all provided values were false.
                     *
                     * \return Returns true if all provided values were true.
                     */
                    inline bool allFalse() const {
                        return currentValue == EnumeratedValue::ALL_FALSE;
                    }

                    /**
                     * Method you can use to determine if both true and false values were provided.
                     */
                    inline bool bothTrueAndFalse() const {
                        return currentValue == EnumeratedValue::BOTH;
                    }

                    /**
                     * Method you can use to determine if any of the provided values were true.
                     *
                     * \return Returns true if any of the values were true.
                     */
                    inline bool includesTrue() const {
                        return allTrue() || bothTrueAndFalse();
                    }

                    /**
                     * Method you can use to determine if any of the provided values were false.
                     *
                     * \return Returns true if any of the values were false.
                     */
                    inline bool includesFalse() const {
                        return allFalse() || bothTrueAndFalse();
                    }

                    /**
                     * Method you can use to obtain an enumerated value for this aggregation.
                     *
                     * \return Returns an enumerated value for this aggregation.
                     */
                    inline EnumeratedValue value() const {
                        return currentValue;
                    }

                    /**
                     * Method you can call to aggregate a new value into this aggregation.
                     *
                     * \param[in] newValue The new value to be aggregated.
                     */
                    void include(bool newValue);

                    /**
                     * Assignment operator
                     *
                     * \param[in] other The value to be copied.
                     *
                     * \return Returns a reference to this instance.
                     */
                    inline BooleanAggregation& operator=(const BooleanAggregation& other) {
                        currentValue = other.currentValue;
                        return *this;
                    }

                    /**
                     * Comparison operator
                     *
                     * \param[in] other The value to be compared against.
                     *
                     * \return Returns true if the values are equal.  Returns false if the values are different.
                     */
                    inline bool operator==(const BooleanAggregation& other) const {
                        return currentValue == other.currentValue;
                    }

                    /**
                     * Comparison operator
                     *
                     * \param[in] other The value to be compared against.
                     *
                     * \return Returns true if the values are different.  Returns false if the values are equal.
                     */
                    inline bool operator!=(const BooleanAggregation& other) const {
                        return currentValue != other.currentValue;
                    }

                    /**
                     * Comparison operator
                     *
                     * \param[in] other The value to be compared against.
                     *
                     * \return Returns true if the values are equal.  Returns false if the values are different.
                     */
                    inline bool operator==(EnumeratedValue other) const {
                        return currentValue == other;
                    }

                    /**
                     * Comparison operator
                     *
                     * \param[in] other The value to be compared against.
                     *
                     * \return Returns true if the values are different.  Returns false if the values are equal.
                     */
                    inline bool operator!=(EnumeratedValue other) const {
                        return currentValue != other;
                    }

                    /**
                     * Casting operator.
                     */
                    inline operator EnumeratedValue() {
                        return currentValue;
                    }

                    /**
                     * Aggregation operator.
                     *
                     * \param[in] newValue the boolean value to aggregate into this aggregation.
                     *
                     * \return Returns a reference to this instance.
                     */
                    inline BooleanAggregation& operator<<(bool newValue) {
                        include(newValue);
                        return *this;
                    }

                private:
                    /**
                     * The underlying enumerated value.
                     */
                    EnumeratedValue currentValue;
            };

            /**
             * Pure virtual base class used to hold aggregations of multiple format instances.  The class exists to
             * facilitate GUI interfaces that allow operations across multiple format instances.
             *
             * The class is derived from an ElementWeakPointerSet so that you can also use an aggregation to store
             * \ref Ld::Element instances.
             */
            class LD_PUBLIC_API Aggregation:public ElementWeakPointerSet {
                friend class Format;

                public:
                    /**
                     * Type used to represent an aggregator creator function.
                     *
                     * \param[in] formatName The name of the aggregator to create.  creator methods can use this to
                     *                       facility having a single creator method generate multiple aggregators.
                     */
                    typedef Aggregation* (*CreatorFunction)(const QString& formatName);

                    Aggregation();

                    virtual ~Aggregation();

                    /**
                     * Method you can call to register a creator function with the aggregation factory.
                     *
                     * \param[in] formatName The name of the format to tie the creator method to.
                     *
                     * \param[in] creatorFunction The creator function to tie to this format.
                     *
                     * \return Returns true on success, returns false if a creator function has already been registered
                     *         using the supplied format name.
                     */
                    static bool registerCreator(const QString& formatName, CreatorFunction creatorFunction);

                    /**
                     * Method you can call to create an aggregator by name.
                     *
                     * \param[in] formatName The name of the format to create an aggregator for.
                     */
                    static QSharedPointer<Aggregation> create(const QString& formatName);

                    /**
                     * Method that returns the type name of the format tracked by this aggregation.
                     *
                     * \return Returns the format name for formats tied to this aggregation.
                     */
                    virtual QString typeName() const = 0;

                    /**
                     * Method you can use to clear the aggregation.  Calling the base class instance will clear the
                     * underlying format database.
                     *
                     * This method will also clear the associated set of elements.
                     */
                    virtual void clear();

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
                    virtual bool addFormat(FormatPointer formatInstance, bool includeExisting = false);

                    /**
                     * Method you can call to indicate that a format has been removed.  Calling this method will cause
                     * this instance to regenerate its contents using the \ref Aggregation::clear and
                     * \ref Aggregation::addFormat methods.
                     *
                     * \param[in] formatInstance The format instance to be added.
                     *
                     * \return Returns true if the format was removed from the database and the aggregation was
                     *         regenerated.  Returns false if the format was not found in the database.
                     */
                    bool removeFormat(FormatPointer formatInstance);

                    /**
                     * Method you can use to obtain all the format instances tracked by this aggregation.
                     *
                     * \return Returns a set containing every format instance tracked by the aggregation.
                     */
                    FormatPointerSet formats() const;

                private:
                    /**
                     * Method that is called when a format's values are changed.  The method will cause the entire
                     * aggregation's values to be reprocessed.
                     */
                    void formatChanged();

                    /**
                     * Collection of aggregated format instances.
                     */
                    FormatWeakPointerSet currentFormats;

                    /**
                     * A hash used to map aggregators to formats.
                     */
                    static QHash<QString, CreatorFunction> creatorFunctions;
            };

            friend class Format::Aggregation;

            /**
             * The typename for the format.
             */
            static const QString formatName;

            /**
             * Enumeration indicating supported rotations for formats that control images.  Values can be cast to
             * integer and multiplied by 90 to get the angle of rotation, in degrees.  Enumeration is placed here so
             * that it can be available to multiple format classes.
             */
            enum class Rotation : std::uint8_t {
                /**
                 * Indicates no rotation.
                 */
                NO_ROTATION = 0,

                /**
                 * Indicates counter-clockwise 90 degree rotation.
                 */
                ROTATE_CCW_90 = 1,

                /**
                 * Indicates flipping or 180 degree rotation.
                 */
                FLIP = 2,

                /**
                 * Indicates clockwise 90 degrees (270 degrees).
                 */
                ROTATE_CW_90 = 3,
            };

            /**
             * Enumeration indicating how an images sould be scaled.
             */
            enum class ImageScalingMode : std::uint8_t {
                /**
                 * Value used to indicate an invalid setting.
                 */
                INVALID = 0,

                /**
                 * Value indicates that this axis value should be ignored.
                 */
                IGNORED = 1,

                /**
                 * Value indicates that this axis value is a size in points.
                 */
                FIXED_POINTS = 2,

                /**
                 * Value indicates that this axis value is a fractional scaling value.  A value of 0.5 indicates 1/2
                 * original size on this axis.  A value of 1.5 indicates this axis should be scaled by an additional
                 * 50% in size.
                 */
                FRACTIONAL = 3,

                /**
                 * Value indicates that the scaling should be based on the other axis to maintain the aspect
                 * ratio.  It is an error for both axis to use this scaling mode.
                 */
                ASPECT_RATIO = 4
            };

            /**
             * Type used to define function pointers to default class creation functions.  In most cases, a creator
             * function can simply instantiate a new, default, instance of an object on the heap.
             *
             * The \ref XmlElement::readXml method will be called on default instances if those instances need to be
             * populated from an XML description.
             *
             * \param[in] typeName The name used to reference this creator function.  A single creator function can be
             *                     used to create multiple object types using this value to identify what type of
             *                     format to be created.
             *
             * \return Returns a pointer to the newly created format instance.
             */
            typedef Format* (*CreatorFunction)(const QString& typeName);

            /**
             * Type used to indicate the capabilities of a format.  The type is a list that will represent the order of
             * inheritance of classes within a format.  Names are expected to match the typenames reported for each
             * format.
             */
            typedef QList<QString> Capabilities;

            /**
             * Static method you can use to create an format given a name.
             *
             * \param[in] typeName The name used to reference this format type.
             *
             * \return Returns a pointer to a class derived from \ref Format.
             */
            static FormatPointer create(const QString& typeName);

            /**
             * Template method you can use to more easily create format instances.
             *
             * \param[in] T The format class type to be created.
             */
            template<class T> LD_PUBLIC_TEMPLATE_METHOD static QSharedPointer<T> createFormat() {
                FormatPointer newFormat = create(T::formatName);
                return newFormat.dynamicCast<T>();
            }

            /**
             * Template method you can use to more easily create format aggregation instances.
             *
             * \param[in] T The format class type to be created.
             */
            template<class T> LD_PUBLIC_TEMPLATE_METHOD static
                QSharedPointer<typename T::Aggregation> createAggregation() {
                    QSharedPointer<Format::Aggregation> newAggregation = Aggregation::create(T::formatName);
                    return newAggregation.dynamicCast<T::Aggregation>();
                }

            /**
             * Static method you can use to register methods used to create default objects by type name.
             *
             * \param[in] typeName        The name used to reference this format type.
             *
             * \param[in] creatorFunction Pointer to the function that should be called to create a default instance of
             *                            the correct object type.
             *
             * \return Returns true on success, returns false if an object creator function has already been registered
             *         for the specified object.
             */
            static bool registerCreator(const QString& typeName, CreatorFunction creatorFunction);

            Format();

            /**
             * Copy constructor.  Note that the list of elements tied to the format is *not* copied.
             *
             * \param[in] other The instance to be copied.
             */
            Format(const Format& other);

            ~Format() override;

            /**
             * Pure virtual method that creates a clone of this class instance.
             *
             * \return Returns a deep copy of this class instance.
             */
            virtual FormatPointer clone() const = 0;

            /**
             * Method that provides the format to be used by the right element of a split.  You can overload this
             * method if you want to modify how formats are applied during a split.
             *
             * A call to this method indicates that an element referencing this format instance is being split.
             *
             * \param[in] element The element that is being split.
             *
             * \return Returns a shared pointer to the format that should be used on the right element after a split.
             *         A null pointer is returned if this format instance should be used on both sides.  The default
             *         implementation returns a null pointer.
             */
            virtual FormatPointer formatForSplit(ElementPointer element);

            /**
             * Pure virtual method that indicates if this format contains valid information.
             *
             * \return Returns true if the format is valid.  Returns false if the format is invalid.
             */
            virtual bool isValid() const = 0;

            /**
             * Method that indicates if this format contains invalid information.  The method obtains its value by
             * calling the \ref Ld::Format::isValid method and inverting the result.
             *
             * \return Returns true if the format is invalid.  Returns false if the format is valid.
             */
            bool isInvalid() const;

            /**
             * Method you can call to determine the capabilities of this format.  The capabilities is a set containing
             * all the classes that make up the format instance.
             *
             * \return Returns a \ref Ld::Format::Capabilities instance containing the names of all the classes that
             *         define this format.  The default version will return a set containing the string "Format".
             */
            virtual Capabilities capabilities() const;

            /**
             * Pure virtual method that returns a description of this format as a string.  The string format should be
             * in the form of a comma separated group of fields with the first field being the type-name of the format.
             *
             * While not always supported, the string should be such that the format can be constructed later from the
             * string.
             *
             * \return Returns a description of this format as a string.
             */
            virtual QString toString() const = 0;

            /**
             * Pure virtual method that returns CSS content that can be used to emulate this format.  The returned
             * string should be on a single line and should not include leading or trailing braces.
             *
             * \return Returns CSS content comparable to this format.
             */
            virtual QString toCss() const = 0;

            /**
             * Method you can call to generate a CSS entry for this format.  You can optionally overload this class
             * if you wish to modify how CSS entries are created.  Output is suitable for direct use in CSS style
             * sheets.
             *
             * The default implementation will use the \ref Ld::Format::toCss method to obtain the CSS content and will
             * insert the CSS contents within braces after the class name and optional ID.  The method will also append
             * a newline.
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
            virtual QString toCssEntry(
                const QString& className,
                const QString& tag = QString(),
                const QString& id = QString()
            ) const;

            /**
             * Method that provides a list of pointers to every element associated with this format.
             *
             * \return Returns a list of shared pointers to \ref Ld::Element instances.
             */
            ElementPointerList elements() const;

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            Format& operator=(const Format& other) {
                XmlElement::operator=(other);

                creators = other.creators;
                currentElements = other.currentElements;

                return *this;
            }

            /**
             * Assignment operator (move semantics)
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            Format& operator=(Format&& other) {
                XmlElement::operator=(other);

                creators = other.creators;
                currentElements = other.currentElements;

                return *this;
            }

        protected:
            /**
             * Method that converts a scaling mode to a string.
             *
             * \param[in] mode The mode to be converted.
             *
             * \return Returns the scaling mode converted to a string.
             */
            static QString toString(ImageScalingMode mode);

            /**
             * Method that converts a string to a scaling mode.
             *
             * \param[in]  text The text string to be converted.
             *
             * \param[out] ok   A pointer to a boolean value that will be populated with true if the conversion was
             *                  successful.  A false value will be applied if the conversion is not successful.
             *
             * \return Returns the translated mode.
             */
            static ImageScalingMode toScalingMode(const QString& text, bool* ok = nullptr);

            /**
             * Method that can be called from derived classes to message the \ref Ld::Element class tied to this format
             * that the format has been updated.  The method will also generate a message to the \ref Ld::Visual class
             * tied to the \ref Ld::Element class.
             */
            void reportFormatUpdated();

            /**
             * Method you can optionally overload to add additional attributes to the XML description of this element.
             * This method should write attributes for this class only and not the class's children.  To cleanly handle
             * multiple virtual inheritance, the Ld::Format::writeAddAttributes method should be coded to call the
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
            virtual void writeAddImmediateAttributes(
                XmlAttributes&                  attributes,
                QSharedPointer<FormatOrganizer> formats,
                ProgramFile&                    programFile
            ) const = 0;

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
            virtual void readImmediateAttributes(
                QSharedPointer<XmlReader>  reader,
                const XmlAttributes&       attributes,
                const FormatsByIdentifier& formats,
                ProgramFile&               programFile,
                unsigned                   xmlVersion
            ) = 0;

            /**
             * Method that can be overloaded to write child format tags to the program file.  The default implementation
             * simply returns as formats should generally not employ child tags.
             *
             * \param[in]     writer              The writer used to write the XML description of the program.
             *
             * \param[in]     formats             A \ref FormatOrganizer instance used to track all known formats in
             *                                    the program.
             *
             * \param[in,out] programFile         The program file instance that can be used to store (or retrieve)
             *                                    additional side-band information.
             *
             * \param[in]     inheritedAttributes Attributes provided by the calling class to this class.  Provided
             *                                    attributes should be provided with child's XML element.  This
             *                                    parameter exists to allow the parent to include positional
             *                                    information as attributes of each child.
             */
            void writeChildren(
                QSharedPointer<XmlWriter>       writer,
                QSharedPointer<FormatOrganizer> formats,
                ProgramFile&                    programFile,
                const XmlAttributes&            inheritedAttributes
            ) const override;

        private:
            /**
             * Dictionary used to identify empty format creators based on a format type name.
             */
            static QMap<QString, CreatorFunction> creators;

            /**
             * The set of elements referencing this element.
             */
            ElementWeakPointerSet currentElements;
    };

    /**
     * Hash function for the \ref Ld::ImageFormat::Rotation enumeration.
     *
     * \param[in] rotation The rotation to be hashed.
     *
     * \param[in] seed     An optional seed for the hash operation.
     *
     * \return Returns a hash for the scaling mode.
     */
    LD_PUBLIC_API Util::HashResult qHash(const Format::Rotation rotation, Util::HashSeed seed = 0);

    /**
     * Hash function for the \ref Ld::ImageFormat::ImageScalingMode enumeration.
     *
     * \param[in] scalingMode The scaling mode value to be hashed.
     *
     * \param[in] seed        An optional seed for the hash operation.
     *
     * \return Returns a hash for the scaling mode.
     */
    LD_PUBLIC_API Util::HashResult qHash(const Format::ImageScalingMode scalingMode, Util::HashSeed seed = 0);
}

#endif
