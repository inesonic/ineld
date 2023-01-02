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
* This header defines the \ref Ld::PlotFormatBase class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_PLOT_FORMAT_BASE_H
#define LD_PLOT_FORMAT_BASE_H

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
#include "ld_font_format.h"
#include "ld_chart_format.h"

namespace Ld {
    /**
     * Base class for plots with a distinct plot area.
     */
    class LD_PUBLIC_API PlotFormatBase:public virtual ChartFormat {
        public:
            /**
             * Enumeration of supported legend locations.
             */
            enum class LegendLocation {
                /**
                 * Indicates that no legend should be displayed.
                 */
                NO_LEGEND,

                /**
                 * Indicates the legend should be on the top, under the title.
                 */
                TOP,

                /**
                 * Indicates the legend should be on the left.
                 */
                LEFT,

                /**
                 * Indicates the legency should be on the right.
                 */
                RIGHT,

                /**
                 * Indicates the legend should be on the bottom.
                 */
                BOTTOM
            };

            /**
             * Class you can use to track an aggregation of multiple \ref Ld::PlotFormatBase instances.
             */
            class LD_PUBLIC_API Aggregation:public virtual ChartFormat::Aggregation {
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
                     * Method you can use to obtain all the plot area left margins, in points.
                     *
                     * \return Returns a set holding all the plot area left margins.
                     */
                    const QSet<float>& plotAreaLeftMargins() const;

                    /**
                     * Method you can use to obtain all the plot area right margins, in points.
                     *
                     * \return Returns a set holding all the plot area right margins.
                     */
                    const QSet<float>& plotAreaRightMargins() const;

                    /**
                     * Method you can use to obtain all the plot area top margins, in points.
                     *
                     * \return Returns a set holding all the plot area top margins.
                     */
                    const QSet<float>& plotAreaTopMargins() const;

                    /**
                     * Method you can use to obtain all the plot area bottom margins, in points.
                     *
                     * \return Returns a set holding all the plot area bottom margins.
                     */
                    const QSet<float>& plotAreaBottomMargins() const;

                    /**
                     * Method you can use to obtain all the background colors for the plot areas.
                     *
                     * \return Returns a set holding the background colors for the entire plot areas.
                     */
                    const QSet<QColor>& plotBackgroundAreaColors() const;

                    /**
                     * Method you can use to obtain the background color for the plot drawing area.
                     *
                     * \return Returns a set holding the background color for the plot drawing area.
                     */
                    const QSet<QColor>& plotDrawingAreaBackgroundColors() const;

                    /**
                     * Method you can use to obtain the title text fonts.
                     *
                     * \return Returns a set containing all the title text fonts.
                     */
                    const QSet<FontFormat>& titleFontFormats() const;

                    /**
                     * Method you can use to obtain the plot legend locations.
                     *
                     * \return Returns a set indicating the locations for plot legends.
                     */
                    const QSet<LegendLocation>& legendLocations() const;

                    /**
                     * Method you can use to obtain the font formats used for the legends.
                     *
                     * \return Returns a set of legend font formats.
                     */
                    const QSet<FontFormat>& legendFontFormats() const;

                    /**
                     * Method you can use to obtain a set of legend border styles.
                     *
                     * \return Returns a set of legend border styles that are currently being used.
                     */
                    const QSet<ChartLineStyle>& legendBorderlineStyles() const;

                    /**
                     * Method you can use to obtain a set of currently used legend background colors.
                     *
                     * \return Returns a set of legend background colors.
                     */
                    const QSet<QColor>& legendBackgroundColors() const;

                    /**
                     * Method you can use to obtain all the plot drawing area border line styles.
                     *
                     * \return Returns a set indicating all the plot drawing area border line styles.
                     */
                    const QSet<ChartLineStyle>& plotDrawingAreaOutlineStyles() const;

                private:
                    /**
                     * Set of plot left margin values.
                     */
                    QSet<float> currentPlotAreaLeftMargins;

                    /**
                     * Set of plot right margins.
                     *
                     * \return Returns a set holding all the plot area right margins.
                     */
                    QSet<float> currentPlotAreaRightMargins;

                    /**
                     * Set of plot top margins.
                     */
                    QSet<float> currentPlotAreaTopMargins;

                    /**
                     * Set of plot bottom margins.
                     */
                    QSet<float> currentPlotAreaBottomMargins;

                    /**
                     * Set of plot background colors.
                     */
                    QSet<QColor> currentPlotBackgroundAreaColors;

                    /**
                     * Set of plot drawing area background colors.
                     */
                    QSet<QColor> currentPlotDrawingAreaBackgroundColors;

                    /**
                     * Set of plot title fonts.
                     */
                    QSet<FontFormat> currentPlotTitleFontFormats;

                    /**
                     * Set of plot legend locations.
                     */
                    QSet<LegendLocation> currentLegendLocations;

                    /**
                     * Set of legend font formats.
                     */
                    QSet<FontFormat> currentLegendFontFormats;

                    /**
                     * Set of legend border line styles.
                     */
                    QSet<ChartLineStyle> currentLegendBorderlineStyles;

                    /**
                     * Set of legend background colors.
                     */
                    QSet<QColor> currentLegendBackgroundColors;

                    /**
                     * Set of plot drawing area border line styles.
                     */
                    QSet<ChartLineStyle> currentPlotDrawingAreaOutlineStyles;
            };

            /**
             * The typename for the format.
             */
            static const QString formatName;

            /**
             * The default chart top margin.
             */
            static const float defaultChartTopMargin;

            /**
             * The default chart left margin.
             */
            static const float defaultChartLeftMargin;

            /**
             * The default chart right margin.
             */
            static const float defaultChartRightMargin;

            /**
             * The default chart bottom margin.
             */
            static const float defaultChartBottomMargin;

            /**
             * The default chart background color.
             */
            static const QColor defaultPlotBackgroundAreaColor;

            /**
             * The default chart drawing area color.
             */
            static const QColor defaultPlotDrawingAreaBackgroundColor;

            /**
             * The default plot title font.
             */
            static const FontFormat defaultPlotTitleFontFormat;

            /**
             * The default plot legend location.
             */
            static const LegendLocation defaultLegendLocation;

            /**
             * The default legend font format.
             */
            static const FontFormat defaultLegendFontFormat;

            /**
             * The default legend border line style.
             */
            static const ChartLineStyle defaultLegendBorderlineStyle;

            /**
             * The default legend background color.
             */
            static const QColor defaultLegendBackgroundColor;

            /**
             * The default plot drawing area outline style.
             */
            static const ChartLineStyle defaultPlotDrawingAreaOutputStyle;

            PlotFormatBase();

            /**
             * Copy constructor.  Note that the copy constructor does not copy elements from the underlying
             * \ref Ld::Format class.  All format parameters are copied by the format are not tied to any element.
             *
             * \param[in] other The instance to be copied.
             */
            PlotFormatBase(const PlotFormatBase& other);

            ~PlotFormatBase() override;

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
             * \return Returns the format identifying name, in this case, "PlotFormatBase".
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
             * Method you can use to set the plot area left margin, in points.
             *
             * \param[in] newLeftMargin The new plot area left margin.
             */
            void setPlotAreaLeftMargin(float newLeftMargin);

            /**
             * Method you can use to obtain the current plot area left margin, in points.
             *
             * \return Returns the plot area left margin, in points.
             */
            float plotAreaLeftMargin() const;

            /**
             * Method you can use to set the plot area right margin, in points.
             *
             * \param[in] newRightMargin The new plot area right margin.
             */
            void setPlotAreaRightMargin(float newRightMargin);

            /**
             * Method you can use to obtain the current plot area right margin, in points.
             *
             * \return Returns the plot area right margin, in points.
             */
            float plotAreaRightMargin() const;

            /**
             * Method you can use to set the plot area top margin, in points.
             *
             * \param[in] newTopMargin The new plot area top margin.
             */
            void setPlotAreaTopMargin(float newTopMargin);

            /**
             * Method you can use to obtain the current plot area top margin, in points.
             *
             * \return Returns the plot area top margin, in points.
             */
            float plotAreaTopMargin() const;

            /**
             * Method you can use to set the plot area bottom margin, in points.
             *
             * \param[in] newBottomMargin The new plot area bottom margin.
             */
            void setPlotAreaBottomMargin(float newBottomMargin);

            /**
             * Method you can use to obtain the current plot area bottom margin, in points.
             *
             * \return Returns the plot area bottom margin, in points.
             */
            float plotAreaBottomMargin() const;

            /**
             * Method you can use to set the plot area (outside) background color.
             *
             * \param[in] newBackgroundColor The new plot area background color.
             */
            void setPlotAreaBackgroundColor(const QColor& newBackgroundColor);

            /**
             * Method you can use to obtain the current plot area background color.
             *
             * \return Returns the current plot area background color.
             */
            const QColor& plotAreaBackgroundColor() const;

            /**
             * Method you can use to set the plot drawing area (inside) background color.
             *
             * \param[in] newBackgroundColor The new plot drawing area background color.
             */
            void setPlotDrawingAreaBackgroundColor(const QColor& newBackgroundColor);

            /**
             * Method you can use to obtain the current plot drawing area background color.
             *
             * \return Returns the current plot drawing area background color.
             */
            const QColor& plotDrawingAreaBackgroundColor() const;

            /**
             * Method you can use to set the title font.
             *
             * \param[in] newTitleFont The new title font.
             */
            void setTitleFontFormat(const FontFormat& newTitleFont);

            /**
             * Method you can use to obtain the current title font.
             *
             * \return Returns the current title font.
             */
            const FontFormat& titleFontFormat() const;

            /**
             * Method you can use to set the legend location.
             *
             * \param[in] newLegendLocation The new legend location.
             */
            void setLegendLocation(LegendLocation newLegendLocation);

            /**
             * Method you can use to obtain the legend location.
             *
             * \return Returns the current legend location.
             */
            LegendLocation legendLocation() const;

            /**
             * Method you can use to set the legend text font.
             *
             * \param[in] newFontFormat The new legend font format.
             */
            void setLegendFontFormat(const FontFormat& newFontFormat);

            /**
             * Method you can use to obtain the currently selected legend text font.
             *
             * \return Returns the current legend text font.
             */
            const FontFormat& legendFontFormat() const;

            /**
             * Method you can use to set the legend border line style.
             *
             * \param[in] newLineStyle The new legend border line style.
             */
            void setLegendBorderLineStyle(const ChartLineStyle& newLineStyle);

            /**
             * Method you can use to determine the current legend border line style.
             *
             * \return Returns the current legend border line style.
             */
            const ChartLineStyle& legendBorderlineStyle() const;

            /**
             * Method you can uset to set the legend area background color.
             *
             * \param[in] newBackgroundColor The new legend area background color.
             */
            void setLegendBackgroundColor(const QColor& newBackgroundColor);

            /**
             * Method you can use to obtain the current legend background color.
             *
             * \return Returns the current legend background color.
             */
            const QColor& legendBackgroundColor() const;

            /**
             * Method you can use to set the plot drawing area border line style.
             *
             * \param[in] newLineStyle The drawing area line style.
             */
            void setDrawingAreaOutlineStyle(const ChartLineStyle& newLineStyle);

            /**
             * Method you can use to obtain the plot drawing area border style.
             *
             * \return Returns the chart drawing area border style.
             */
            const ChartLineStyle& drawingAreaOutlineStyle() const;

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
             * Method you can use to convert a legend location value to a string.
             *
             * \param[in] legendLocation The legend location to convert to a string.
             *
             * \return Returns a string representation of the legend location.
             */
            static QString toString(LegendLocation legendLocation);

            /**
             * Method you can use to convert a string into a legend lcoation.
             *
             * \param[in]  str The string to be converted.
             *
             * \param[out] ok  Pointer to a location that will hold true on success, false if the string can not be
             *                 converted.
             *
             * \return Returns the legend location.
             */
            static LegendLocation toLegendLocation(const QString& str, bool* ok = nullptr);

            /**
             * Method you can overload to receive notification that the XML description for this format is about to
             * be read.  You can use this call to reset any child format information to a default state.
             *
             * If you overload this method, be sure to call the base class version.
             */
            void aboutToReadXml() override;

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
            void writeChildFormats(
                QSharedPointer<XmlWriter>       writer,
                QSharedPointer<FormatOrganizer> formats,
                ProgramFile&                    programFile
            ) const override;

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
            bool readChildFormat(
                QSharedPointer<XmlReader>  reader,
                const QString&             tagName,
                const FormatsByIdentifier& formats,
                const XmlAttributes&       attributes,
                ProgramFile&               programFile,
                unsigned                   xmlVersion
            ) override;

        private:
            /**
             * The plot left margin value.
             */
            float currentPlotAreaLeftMargin;

            /**
             * The plot right margin value.
             */
            float currentPlotAreaRightMargin;

            /**
             * Plot top margin value.
             */
            float currentPlotAreaTopMargin;

            /**
             * Plot bottom margin value.
             */
            float currentPlotAreaBottomMargin;

            /**
             * Plot background color.
             */
            QColor currentPlotBackgroundAreaColor;

            /**
             * Plot drawing area background color.
             */
            QColor currentPlotDrawingAreaBackgroundColor;

            /**
             * Plot title font.
             */
            FontFormat currentPlotTitleFontFormat;

            /**
             * Plot legend location.
             */
            LegendLocation currentLegendLocation;

            /**
             * Legend font formats.
             */
            FontFormat currentLegendFontFormat;

            /**
             * Legend border line style.
             */
            ChartLineStyle currentLegendBorderlineStyle;

            /**
             * Legend background colors.
             */
            QColor currentLegendBackgroundColor;

            /**
             * Plot drawing area border line style.
             */
            ChartLineStyle currentPlotDrawingAreaOutlineStyle;
    };

    /**
     * Hash function for the \ref Ld::PlotFormatBase::LegendLocation enumeration.
     *
     * \param[in] value The value to be hashed.
     *
     * \param[in] seed  An optional seed to apply to the hash operation.
     *
     * \return Returns a hash for the provided line style.
     */
    LD_PUBLIC_API Util::HashResult qHash(PlotFormatBase::LegendLocation value, Util::HashSeed seed = 0);
}

#endif
