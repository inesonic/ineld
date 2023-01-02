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
* This header defines the \ref Ld::BraceConditionalFormat class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_BRACE_CONDITIONAL_FORMAT_H
#define LD_BRACE_CONDITIONAL_FORMAT_H

#include <QtGlobal>
#include <QString>
#include <QSharedPointer>
#include <QColor>
#include <QFont>
#include <QMap>
#include <QRegularExpression>

#include <util_hash_functions.h>

#include "ld_common.h"
#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_attributes.h"
#include "ld_font_format.h"

namespace Ld {
    /**
     * Class used to track information about a brace conditional operator.
     */
    class LD_PUBLIC_API BraceConditionalFormat:public virtual FontFormat {
        public:
            /**
             * Method you can use to set an application global brace conditional format.
             *
             * \param[in] newGlobalFormat The The new global brace conditional format.
             */
            static void setApplicationDefaultBraceConditionalFormat(
                QSharedPointer<BraceConditionalFormat> newGlobalFormat
            );

            /**
             * Method you can use to obtain the application global brace conditional format.
             *
             * \return Returns the current global brace conditional format.
             */
            static QSharedPointer<BraceConditionalFormat> applicationDefaultBraceConditionalFormat();

            /**
             * Method you can use to set the application default setting for including/excluding the else clause.
             *
             * \param[in] nowShowElseClause If true, the application default setting will show the else clause.  If
             *                              false, the application global setting will hide the else clause.
             */
            static void setApplicationDefaultShowElseClause(bool nowShowElseClause);

            /**
             * Method you cna use to obtian the application global include/exclude else clause setting.
             *
             * \return If true, the application default setting will show the else clause.  If false, the application
             *         global setting will hide the else clause.
             */
            static bool applicationDefaultShowElseClause();

            /**
             * Class you can use to track an aggregation of multiple \ref Ld::BraceConditionalFormat instances.
             */
            class LD_PUBLIC_API Aggregation:public virtual FontFormat::Aggregation {
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
                     * Method you can use to obtain all the condition preamble text values.
                     *
                     * \return Returns a set of condition preamble text values.
                     */
                    const QSet<QString>& conditionPreambleStrings() const;

                    /**
                     * Method you can use to obtain the else condition text values.
                     *
                     * \return Returns a set of the else condition text values.
                     */
                    const QSet<QString>& elseConditionStrings() const;

                private:
                    /*
                     * Set of condition preamble strings.
                     */
                    QSet<QString> currentConditionPreambleStrings;

                    /**
                     * Set of else condition strings.
                     */
                    QSet<QString> currentElseConditionStrings;
            };

            /**
             * The typename for the format.
             */
            static const QString formatName;

            /**
             * The default condition preamble string.  Note that these strings are not localized.
             */
            static const QString defaultConditionPreambleString;

            /**
             * The default else condition string.  Note that these strings are not localized.
             */
            static const QString defaultElseConditionString;

            BraceConditionalFormat();

            /**
             * Copy constructor.  Note that the copy constructor does not copy elements from the underlying
             * \ref Ld::Format class.  All format parameters are copied by the format are not tied to any element.
             *
             * \param[in] other The instance to be copied.
             */
            BraceConditionalFormat(const BraceConditionalFormat& other);

            ~BraceConditionalFormat() override;

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
             * \return Returns the format identifying name, in this case, "BraceConditionalFormat".
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
             * Method you can call to determine the capabilities of this format.  The capabilities is a set containing
             * all the classes that make up the format instance.
             *
             * \return Returns a \ref Ld::Format::Capabilities instance containing the names of all the classes that
             *         define this format.
             */
            Capabilities capabilities() const override;

            /**
             * Method you can use to set the condition preamble string.
             *
             * \param[in] newConditionPreambleString The new condition preamble string.
             */
            void setConditionPreambleString(const QString& newConditionPreambleString);

            /**
             * Method you can use to obtain the current condition preamble string.
             *
             * \return Returns the condition preamble string.
             */
            QString conditionPreambleString() const;

            /**
             * Method you can use to set the else condition string.
             *
             * \param[in] newElseConditionString The new else condition string.
             */
            void setElseConditionString(const QString& newElseConditionString);

            /**
             * Method you can use to obtain the current else condition string.
             *
             * \return Returns the else condition string.
             */
            QString elseConditionString() const;

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
             * Method that converts a string identifier to a \ref Ld::CharacterFormat instance.
             *
             * \param[in]  identifier The string identifier for the character format.
             *
             * \param[out] ok         Optional pointer to an instance of bool used to determine if the string identifier
             *                        is correct.  The boolean will be set to true if the string is valid and will be
             *                        set to false if the string is invalid.  You can provide a null pointer if you
             *                        don't need to determine validity.
             *
             * \return Returns a \ref Ld::BraceConditionalFormat described by the string.
             */
            static BraceConditionalFormat fromString(const QString& identifier, bool* ok = Q_NULLPTR);

        protected:
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
             * Value holding the application default text identifier font.
             */
            static QSharedPointer<BraceConditionalFormat> currentApplicationGlobalBraceConditionFormat;

            /**
             * Value holding the application default show/hide else clause setting.
             */
            static bool currentApplicationGlobalShowElseClause;

            /**
             * Regular expression used to split the string description.
             */
            static QRegularExpression textSplitRegularExpression;

            /**
             * The current condition preamble string.
             */
            QString currentConditionPreambleString;

            /**
             * The current else condition string.
             */
            QString currentElseConditionString;
    };
};

#endif
