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
* This header defines the \ref Ld::ChartFormat class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CHART_FORMAT_H
#define LD_CHART_FORMAT_H

#include <QString>
#include <QSharedPointer>
#include <QSet>

#include "ld_common.h"
#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_chart_line_style.h"
#include "ld_format.h"

namespace Ld {
    /**
     * Base class that provides format information common to all charts and graphs.
     */
    class LD_PUBLIC_API ChartFormat:public virtual Format {
        public:
            /**
             * Class you can use to track an aggregation of multiple \ref Ld::ChartFormat instances.
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
                     * Method you can use to obtain all the chart types.
                     *
                     * \return Returns a set containing all the chart types.
                     */
                    const QSet<QString>& chartTypes() const;

                    /**
                     * Method you can use to obtain all the chart widths, in points.
                     *
                     * \return Returns a set containing all the chart widths, in points.
                     */
                    const QSet<float>& chartWidths() const;

                    /**
                     * Method you can use to obtain all the chart heights, in points.
                     *
                     * \return Returns a set containing all the chart heights, in points.
                     */
                    const QSet<float>& chartHeights() const;

                    /**
                     * Method you can use to obtain all the chart border line styles.
                     *
                     * \return Returns a set containing all the chart border line styles.
                     */
                    const QSet<ChartLineStyle>& borderLineStyles() const;

                private:
                    /**
                     * Set of chart types.
                     */
                    QSet<QString> currentChartTypes;

                    /**
                     * Set of chart width values.
                     */
                    QSet<float> currentChartWidths;

                    /**
                     * Set of chart height values.
                     */
                    QSet<float> currentChartHeights;

                    /**
                     * Set of chart border line styles.
                     */
                    QSet<ChartLineStyle> currentBorderLineStyles;
            };

            /**
             * The typename for the format.
             */
            static const QString formatName;

            /**
             * The default chart width.
             */
            static const float defaultChartWidth;

            /**
             * The default chart height.
             */
            static const float defaultChartHeight;

            ChartFormat();

            /**
             * Copy constructor.  Note that the copy constructor does not copy elements from the underlying
             * \ref Ld::Format class.  All format parameters are copied by the format are not tied to any element.
             *
             * \param[in] other The instance to be copied.
             */
            ChartFormat(const ChartFormat& other);

            ~ChartFormat() override;

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
             * \return Returns the format identifying name, in this case, "ChartFormat".
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
             * Method you can use to set the chart type.
             *
             * \param[in] newChartType An name for the chart type.
             */
            void setChartType(const QString& newChartType);

            /**
             * Method you can use to obtain the chart type.
             *
             * \return Returns an integer value representing the chart type.
             */
            const QString& chartType() const;

            /**
             * Method you can use to set the outside width of the chart, in points.
             *
             * \param[in] newWidth The new chart width, in points.
             */
            void setChartWidth(float newWidth);

            /**
             * Method you can use to obtain the current outside width of the chart, in points.
             *
             * \return Returns the current outside width of the chart, in points.
             */
            float chartWidth() const;

            /**
             * Method you can use to set the outside height of the chart, in points.
             *
             * \param[in] newHeight The new chart height, in points.
             */
            void setChartHeight(float newHeight);

            /**
             * Method you can use to obtain the current outside height of the chart, in points.
             *
             * \return Returns the current outside height of the chart, in points.
             */
            float chartHeight() const;

            /**
             * Method you can use to update the chart border line style.
             *
             * \param[in] newBorderLineStyle The new chart border line style.
             */
            void setBorderLineStyle(const ChartLineStyle& newBorderLineStyle);

            /**
             * Method you can use to obtain the current chart border line style.
             *
             * \return Returns the border line style for the chart.
             */
            const ChartLineStyle& borderLineStyle() const;

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
             * Method that you can use to update the state of this object instance based on an incoming XML stream.  The
             * method will generally be called on an instance immediately after construction by the creator factor
             * function and will be called during parsing of the XML stream immediately after encountering the XML start
             * tag.  You can overload this method if you wish to modify how the XML stream is parsed for this object.
             *
             * This version simply calls \ref Ld::ChartFormat::aboutToReadXml followed by a call to the base class
             * version.
             *
             * \param[in]     reader      The reader used to parse the incoming XML stream.
             *
             * \param[in]     formats     A map of format instances by format identifier.  You can use this parameter
             *                            to either tie a format to an object.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             *
             * \param[in]     attributes  Attributes tied to the XML start tag.
             *
             * \param[in]     xmlVersion  The XML major version code.  The value can be used to modify how the incoming XML
             *                            stream is parsed.
             */
            void readXml(
                QSharedPointer<XmlReader>  reader,
                const FormatsByIdentifier& formats,
                ProgramFile&               programFile,
                const XmlAttributes&       attributes,
                unsigned                   xmlVersion = invalidXmlVersion
            ) override;

        protected:
            /**
             * Method you can overload to receive notification that the XML description for this format is about to
             * be read.  You can use this call to reset any child format information to a default state.
             *
             * If you overload this method, be sure to call the base class version.
             */
            virtual void aboutToReadXml();

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

            /**
             * Method that can be overloaded to write one or more child format tags to the program file.  Overload this
             * method if the format needs to write a child tag.  Be sure to call the base class versions.
             *
             * \param[in]     writer      The writer used to write the XML description of the program.
             *
             * \param[in]     formats     A \ref FormatOrganizer instance used to track all known formats in the
             *                            program.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             */
            virtual void writeChildFormats(
                QSharedPointer<XmlWriter>       writer,
                QSharedPointer<FormatOrganizer> formats,
                ProgramFile&                    programFile
            ) const;

            /**
             * Method that you can use to read and process a child format tag.  You need only process tag attributes
             * in this method.  The calling method will search for the end of the tag.
             *
             * \param[in]     reader      The reader used to parse the incoming XML stream.
             *
             * \param[in]     tagName     The name of the spotted XML tag.
             *
             * \param[in]     formats     A map of format instances by format identifier.  You can use this parameter
             *                            to either tie a format to an object or add a new format to the list of known
             *                            formats.
             *
             * \param[in]     attributes  The XML attributes to be parsed.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             *
             * \param[in]     xmlVersion  The XML major version code.  The value can be used to modify how the incoming
             *                            XML stream is parsed.
             *
             * \return Returns true if the tag was recognized.  Returns false if the tag was not recognized.
             */
            virtual bool readChildFormat(
                QSharedPointer<XmlReader>  reader,
                const QString&             tagName,
                const FormatsByIdentifier& formats,
                const XmlAttributes&       attributes,
                ProgramFile&               programFile,
                unsigned                   xmlVersion
            );

            /**
             * Method that can be overloaded to write child format tags to the program file.  This version will call
             * the \ref Ld::ChartFormat::writeChildFormats method.
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

            /**
             * Method that you can use to read and process a child XML tag.  The method is called right after the XML
             * tag is detected in the XML stream.
             *
             * Errors can be reported directly to the reader using the raiseError method.  This method will call
             * the \ref Ld::ChartFormat::readChildFormat.
             *
             * \param[in]     reader      The reader used to parse the incoming XML stream.
             *
             * \param[in]     tagName     The name of the spotted XML tag.
             *
             * \param[in]     formats     A map of format instances by format identifier.  You can use this parameter
             *                            to either tie a format to an object or add a new format to the list of known
             *                            formats.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             *
             * \param[in]     attributes  Attributes tied to the XML start tag.
             *
             * \param[in]     xmlVersion  The XML major version code.  The value can be used to modify how the incoming
             *                            XML stream is parsed.
             */
            void readChild(
                QSharedPointer<XmlReader>  reader,
                const QString&             tagName,
                const FormatsByIdentifier& formats,
                ProgramFile&               programFile,
                const XmlAttributes&       attributes,
                unsigned                   xmlVersion
            ) override;

        private:
            /**
             * The current chart type.
             */
            QString currentChartType;

            /**
             * The current chart width, in points.
             */
            float currentChartWidth;

            /**
             * The current chart height, in points.
             */
            float currentChartHeight;

            /**
             * The current border line style.
             */
            ChartLineStyle currentBorderLineStyle;
    };
}

#endif
