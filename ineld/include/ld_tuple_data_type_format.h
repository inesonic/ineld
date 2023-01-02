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
* This header defines the \ref Ld::TupleDataTypeFormat class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_TUPLE_DATA_TYPE_FORMAT_H
#define LD_TUPLE_DATA_TYPE_FORMAT_H

#include <QString>
#include <QSet>

#include <util_hash_functions.h>

#include "ld_common.h"
#include "ld_format_structures.h"
#include "ld_list_data_type_format.h"

namespace Ld {
    /**
     * Class that provides format information to render tuple data type values.
     */
    class LD_PUBLIC_API TupleDataTypeFormat:public virtual ListDataTypeFormat {
        public:
            /**
             * Enumeration of supported display modes.  Note that the display modes are currently supported by the
             * format but *not* by the GUI or any of the translators.  Currently only wrapped mode is supported.
             */
            enum class DisplayMode {
                /**
                 * Indicates an invalid display mode.
                 */
                INVALID,

                /**
                 * Indicates the tuple should be displayed in reading format, wrapping across lines.  Elements should
                 * be displayed as distinct.
                 */
                NORMAL,

                /**
                 * Indicates the tuple should be displayed as a string.  All members must be valid Unicode integers.
                 * The string will be displayed without any form of quotation.
                 */
                STRING_BARE,

                /**
                 * Indicates the tuple should be displayed as a string.  All members must be valid Unicode integers.
                 * The string will be displayed in angle brackets.
                 */
                STRING_ANGLE_BRACKETS,

                /**
                 * Indicates the tuple should be displayed as a string.  All members must be valid Unicode integers.
                 * The string will be displayed in single quotes.
                 */
                STRING_SINGLE_QUOTES,

                /**
                 * Indicates the tuple should be displayed as a string.  All members must be valid Unicode integers.
                 * The string will be displayed in double quotes.
                 */
                STRING_DOUBLE_QUOTES
            };

            /**
             * Class you can use to track an aggregation of multiple \ref Ld::TupleDataTypeFormat instances.
             */
            class LD_PUBLIC_API Aggregation:public virtual ListDataTypeFormat::Aggregation {
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
                     * \param[in] formatInstance The format instance to be added.
                     *
                     * \param[in] includeExisting If true, this format will be included in the aggregation even if it
                     *                            has already been accounted for.
                     *
                     * \return Returns true if the format was added to the database.  Returns false if the format is
                     *         already contained in the underlying database.
                     */
                    bool addFormat(FormatPointer formatInstance, bool includeExisting = false) override;

                    /**
                     * Method you can use to obtain the current display modes.
                     *
                     * \return Returns a set of currently requested display modes.
                     */
                    const QSet<DisplayMode>& displayModes() const;

                private:
                    /**
                     * The supported tuple display modes.
                     */
                    QSet<DisplayMode> currentDisplayModes;
            };

            /**
             * The typename for the format.
             */
            static const QString formatName;

            /**
             * The default display modes.
             */
            static const DisplayMode defaultDisplayMode;

            TupleDataTypeFormat();

            /**
             * Copy constructor.  Note that the copy constructor does not copy elements from the underlying
             * \ref Ld::Format class.  All format parameters are copied by the format are not tied to any element.
             *
             * \param[in] other The instance to be copied.
             */
            TupleDataTypeFormat(const TupleDataTypeFormat& other);

            ~TupleDataTypeFormat() override;

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
             * \return Returns the format identifying name, in this case, "TupleDataTypeFormat".
             */
            QString typeName() const override;

            /**
             * Pure virtual method that indicates if this format contains valid information.
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
             * Method you can use to set the desired display mode.
             *
             * \param[in] newDisplayMode The newly desired display mode.
             */
            void setDisplayMode(DisplayMode newDisplayMode);

            /**
             * Method you can use to obtain the current display mode.
             *
             * \return Returns the current display mode.
             */
            DisplayMode displayMode() const;

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
             * multiple virtual inheritance, the Format:writeAddAttributes method should be coded to call the
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
             * Method that converts a display mode value to a string.
             *
             * \param[in] displayMode The display mode to be converted.
             *
             * \return Returns a string representation of the display mode.
             */
            static QString toString(DisplayMode displayMode);

            /**
             * Method that converts a string to a display mode value.
             *
             * \param[in]  str The string to convert.
             *
             * \param[out] ok  A boolean variable that will be populated with true on success, false on error.
             *
             * \return Returns the translated display mode.
             */
            static DisplayMode toDisplayMode(const QString& str, bool* ok = nullptr);

            /**
             * The current display mode.
             */
            DisplayMode currentDisplayMode;
    };

    /**
     * Hash function for the \ref Ld::TupleDataTypeFormat::DisplayMode enumeration.
     *
     * \param[in] displayMode The display mode to be hashed.
     *
     * \param[in] seed        An optional seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    LD_PUBLIC_API Util::HashResult qHash(TupleDataTypeFormat::DisplayMode displayMode, Util::HashSeed seed = 0);
}

#endif
