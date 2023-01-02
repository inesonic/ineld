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
* This header defines the \ref Ld::ImageFormat class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_IMAGE_FORMAT_H
#define LD_IMAGE_FORMAT_H

#include <QString>
#include <QSharedPointer>
#include <QSet>

#include <util_hash_functions.h>

#include "ld_common.h"
#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_format.h"

namespace Ld {
    /**
     * Class that provides format information for images on a page.  The class defines methods to control the image
     * size and aspect ratio.
     */
    class LD_PUBLIC_API ImageFormat:public virtual Format {
        public:
            /**
             * Class that can be used to store and manage information about an axis setting.
             */
            class LD_PUBLIC_API Axis {
                public:
                    Axis();

                    /**
                     * Constructor
                     *
                     * \param[in] value       The value to apply to this axis.
                     *
                     * \param[in] scalingMode The scaling mode to apply to this axis.
                     */
                    Axis(float value, ImageScalingMode scalingMode);

                    /**
                     * Copy constructor
                     *
                     * \param[in] other The instance to be copied.
                     */
                    Axis(const Axis& other);

                    ~Axis();

                    /**
                     * Method you can use to set the new axis value.
                     *
                     * \param[in] newValue The new value to apply to this axis.
                     */
                    void setValue(float newValue);

                    /**
                     * Method you can use to explicitly obtain the value.
                     *
                     * \return Returns the current value.
                     */
                    float value() const;

                    /**
                     * Method you can use to set the scaling mode.
                     *
                     * \param[in] newScalingMode The new scaling mode to be applied.
                     */
                    void setScalingMode(ImageScalingMode newScalingMode);

                    /**
                     * Method you can use to obtain the current scaling mode.
                     */
                    ImageScalingMode scalingMode() const;

                    /**
                     * Cast operator.
                     *
                     * \return Returns the value associated with this \ref Axis instance.
                     */
                    operator float() const;

                    /**
                     * Assignment operator.
                     *
                     * \param[in] other The instance to be copied.
                     */
                    Axis& operator=(const Axis& other);

                    /**
                     * Comparison operator.
                     *
                     * \param[in] other The instance to be copied.
                     */
                    bool operator==(const Axis& other) const;

                    /**
                     * Comparison operator.
                     *
                     * \param[in] other The instance to be compared against this instance.
                     */
                    bool operator!=(const Axis& other) const;

                private:
                    /**
                     * The current value.
                     */
                    float currentValue;

                    /**
                     * The current scaling mode.
                     */
                    ImageScalingMode currentScalingMode;
            };

            /**
             * Class you can use to track an aggregation of multiple \ref Ld::ImageFormat instances.
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
                     * Method you can use to obtain all the horizontal axis settings.
                     *
                     * \return Returns a set of horizontal axis settings.
                     */
                    QSet<Axis> horizontalAxis() const;

                    /**
                     * Method you can use to obtain all the vertical axis settings.
                     *
                     * \return Returns a set of vertical axis settings.
                     */
                    QSet<Axis> verticalAxis() const;

                    /**
                     * Method you can use to obtain all of the current horizontal scaling modes being used.
                     *
                     * \return Returns a set containing all of the horizontal scaling modes that are in use.
                     */
                    QSet<ImageScalingMode> horizontalScalingModes() const;

                    /**
                     * Method you can use to obtain all of the horizontal scaling values.
                     *
                     * \return Returns a set containing all of the horizontal scaling values.
                     */
                    QSet<float> horizontalValues() const;

                    /**
                     * Method you can use to obtain all of the current vertical scaling modes being used.
                     *
                     * \return Returns a set containing all of the vertical scaling modes that are in use.
                     */
                    QSet<ImageScalingMode> verticalScalingModes() const;

                    /**
                     * Method you can use to obtain all of the vertical scaling values.
                     *
                     * \return Returns a set containing all of the vertical scaling values.
                     */
                    QSet<float> verticalValues() const;

                    /**
                     * Method you can use to obtain all the rotation settings.
                     *
                     * \return Returns a set containing all the rotation settings.
                     */
                    QSet<Rotation> rotations() const;

                private:
                    /**
                     * Set of horizontal axis settings.
                     */
                    QSet<Axis> currentHorizontalAxis;

                    /**
                     * Set of vertical axis settings.
                     */
                    QSet<Axis> currentVerticalAxis;

                    /**
                     * Set of horizontal scaling modes.
                     */
                    QSet<ImageScalingMode> currentHorizontalScalingModes;

                    /**
                     * Set of horizontal values.
                     */
                    QSet<float> currentHorizontalValues;

                    /**
                     * Set of vertical scaling modes.
                     */
                    QSet<ImageScalingMode> currentVerticalScalingModes;

                    /**
                     * Set of vertical values.
                     */
                    QSet<float> currentVerticalValues;

                    /**
                     * Set of rotation values.
                     */
                    QSet<Rotation> currentRotations;
            };

            /**
             * The typename for the format.
             */
            static const QString formatName;

            /**
             * Value indicating the default scaling mode to apply to each axis.
             */
            static const ImageScalingMode defaultScalingMode;

            /**
             * Value indicating the default axis value.
             */
            static const float defaultAxisScalingValue;

            /**
             * Value indicating the default rotation.
             */
            static const Rotation defaultRotation;

            ImageFormat();

            /**
             * Copy constructor.  Note that the copy constructor does not copy elements from the underlying
             * \ref Ld::Format class.  All format parameters are copied by the format are not tied to any element.
             *
             * \param[in] other The instance to be copied.
             */
            ImageFormat(const ImageFormat& other);

            ~ImageFormat() override;

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
             * \return Returns the format identifying name, in this case, "ImageFormat".
             */
            QString typeName() const override;

            /**
             * Virtual method that indicates if this format contains valid information.
             *
             * \return Returns true if the format is valid.  Returns false if the format is invalid.
             */
            bool isValid() const override;

            /**
             * Method you can call to determine the capabilities of this format.  The capabilities is a set containing
             * all the classes that make up the format instance.
             *
             * \return Returns a \ref Ld::Format::Capabilities instance containing the names of all the classes that
             *         define this format.
             */
            Capabilities capabilities() const override;

            /**
             * Method you can use to set the horizontal axis settings.
             *
             * \param[in] newHorizontalAxis settings.
             */
            void setHorizontalAxis(const Axis& newHorizontalAxis);

            /**
             * Convenience method you can use to set the horizontal axis settings.
             *
             * \param[in] newValue       The value driving the scaling.
             *
             * \param[in] newScalingMode The new scaling mode for the horizontal axis.
             */
            void setHorizontalAxis(float newValue, ImageScalingMode newScalingMode);

            /**
             * Method you can use to obtain the current horizontal axis settings.
             *
             * \return Returns the horizontal axis settings.
             */
            const Axis& horizontalAxis() const;

            /**
             * Method you can use to set the vertical axis settings.
             *
             * \param[in] newVerticalAxis settings.
             */
            void setVerticalAxis(const Axis& newVerticalAxis);

            /**
             * Convenience method you can use to set the vertical axis settings.
             *
             * \param[in] newValue       The value driving the scaling.
             *
             * \param[in] newScalingMode The new scaling mode for the vertical axis,
             */
            void setVerticalAxis(float newValue, ImageScalingMode newScalingMode);

            /**
             * Method you can use to obtain the current vertical axis settings.
             *
             * \return Returns the vertical axis settings.
             */
            const Axis& verticalAxis() const;

            /**
             * Method you can use to set the current rotation value.
             *
             * \param[in] newRotation The new rotation value, in degrees.
             */
            void setRotation(Rotation newRotation);

            /**
             * Method you can use to obtain the current rotation value.
             *
             * \return Returns the current rotation value.
             */
            Rotation rotation() const;

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
             * The current horizontal axis value.
             */
            Axis currentHorizontalAxis;

            /**
             * The current vertical axis value.
             */
            Axis currentVerticalAxis;

            /**
             * The current rotation value.
             */
            Rotation currentRotation;
    };

    /**
     * Hash function for the \ref Ld::ImageFormat::Axis class.
     *
     * \param[in] axis The axis data instance to be hashed.
     *
     * \param[in] seed An optional seed to apply to the hash operation.
     *
     * \return Returns a hash for the provided axis.
     */
    LD_PUBLIC_API Util::HashResult qHash(const ImageFormat::Axis& axis, Util::HashSeed seed = 0);
}

#endif
