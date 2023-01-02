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
* This header defines the \ref Ld::PlotFormat class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_PLOT_FORMAT_H
#define LD_PLOT_FORMAT_H

#include <QString>
#include <QSharedPointer>
#include <QSet>
#include <QMap>

#include "ld_common.h"
#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_font_format.h"
#include "ld_chart_format.h"
#include "ld_plot_series.h"
#include "ld_chart_axis_format.h"
#include "ld_plot_format_base.h"

namespace Ld {
    /**
     * Class used to track the look and feel of plots.
     */
    class LD_PUBLIC_API PlotFormat:public virtual PlotFormatBase {
        public:
            /**
             * Enumeration of supported axis locations.
             */
            typedef PlotSeries::AxisLocation AxisLocation;

            /**
             * The default baseline axis value.
             */
            static const AxisLocation defaultBaselineAxis;

            /**
             * Class you can use to track an aggregation of multiple \ref Ld::PlotFormat instances.
             */
            class LD_PUBLIC_API Aggregation:public virtual PlotFormatBase::Aggregation {
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
                     * Method you can use to obtain a map of all the plot axis values by location.
                     *
                     * \return axisLocation The axis location of interest.
                     *
                     * \return Returns a set holding all the axis settings for the given location.
                     */
                    QSet<ChartAxisFormat> axisFormats(AxisLocation axisLocation) const;

                    /**
                     * Method you can use to determine if plots use the first series for data labels.
                     *
                     * \return Returns a boolean aggregation indicating how the first data series is interpreted.
                     */
                    const BooleanAggregation& firstSeriesIsDataLabels() const;

                    /**
                     * Method you can use to obtain the currently selected baseline axis values.
                     *
                     * \return Returns a set indicating the selected baseline axis values.
                     */
                    const QSet<AxisLocation>& baselineAxis() const;

                private:
                    /**
                     * Map of sets for each axis location.
                     */
                    QMap<AxisLocation, QSet<ChartAxisFormat>> currentAxisFormatsByLocation;

                    /**
                     * Boolean aggregation indicating if the first series are used as data labels.
                     */
                    BooleanAggregation currentFirstSeriesIsDataLabels;

                    /**
                     * Set of selected baseline axis locations.
                     */
                    QSet<AxisLocation> currentBaselineAxis;
            };

            /**
             * The typename for the format.
             */
            static const QString formatName;

            PlotFormat();

            /**
             * Copy constructor.  Note that the copy constructor does not copy elements from the underlying
             * \ref Ld::Format class.  All format parameters are copied by the format are not tied to any element.
             *
             * \param[in] other The instance to be copied.
             */
            PlotFormat(const PlotFormat& other);

            ~PlotFormat() override;

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
             * \return Returns the format identifying name, in this case, "PlotFormat".
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
             * Method you can use to clear all the axis format data.
             */
            void clearAxisFormat();

            /**
             * Method you can use to update the axis format for a given axis.
             *
             * \param[in] axisLocation The location of interest.
             *
             * \param[in] axisFormat   The format at the specified location.
             */
            void setAxisFormat(AxisLocation axisLocation, const ChartAxisFormat& axisFormat);

            /**
             * Method you can use to remove an axis format for a given axis.
             *
             * \param[in] axisLocation The location of interest.
             *
             * \return Returns true on success.  Returns false if the axis format is not defined.
             */
            bool removeAxisFormat(AxisLocation axisLocation);

            /**
             * Method you can use to determine if an axis format has been defined for a location.
             *
             * \param[in] axisLocation The axis location to be queried.
             *
             * \return Returns true if a location has been defined.  REturns false if a location has not been defined.
             */
            bool axisFormatDefined(AxisLocation axisLocation) const;

            /**
             * Method you can use to obtain a list of all the currently defined axis format locations.
             *
             * \return Returns a list of every defined axis format location.
             */
            QList<AxisLocation> definedAxisLocations() const;

            /**
             * Method you can use to obtain the axis format for a given location.
             *
             * \param[in] axisLocation The location of interest.
             *
             * \return Returns the format for the specified area.  A default constructed value will be returned if the
             *         location currently does not have a format associated with it.
             */
            ChartAxisFormat axisFormat(AxisLocation axisLocation) const;

            /**
             * Method you can use to remove all the plot series.
             */
            void clearPlotSeries();

            /**
             * Method you can use to determine the current number of plot series.
             *
             * \return Returns the current number of plot series.
             */
            unsigned numberPlotSeries() const;

            /**
             * Method you can use to determine if a plot series is a default plot series.
             *
             * \param[in] seriesIndex The zero based index of the desired plot series.
             *
             * \return Returns true if the plot series is default.  Returns false if the plot series is not default.
             */
            bool isDefaultPlotSeries(unsigned seriesIndex) const;

            /**
             * Method you can use to obtain a plot series.
             *
             * \param[in] seriesIndex The zero based index to the desired plot series.
             *
             * \return Returns a constant reference to the requested plot series.
             */
            const PlotSeries& plotSeries(unsigned seriesIndex) const;

            /**
             * Method you can use to update a plot series.
             *
             * \param[in] seriesIndex The zero based index of the plot series of interest.  If the index is invalid,
             *                        default plot series will be added until the specified series index is reached.
             *
             * \param[in] plotSeries  The new plot series.
             */
            void setPlotSeries(unsigned seriesIndex, const PlotSeries& plotSeries);

            /**
             * Method you can use to determine if plot uses the first series for data labels.
             *
             * \return Returns true if the first data series is used for data labels.
             */
            bool firstSeriesIsDataLabels() const;

            /**
             * Method you can use to determine if plot uses the first series as a series.
             *
             * \return Returns true if the first data series is used as a data series.
             */
            bool firstSeriesIsDataSeries() const;

            /**
             * Method you can use to specify if the first data series is used for data labels.
             *
             * \param[in] nowFirstSeriesIsDataLabels If true, the first data series should be used as labels for all
             *                                       other series data.  If false, the first data series is simply a
             *                                       data series.
             */
            void setFirstSeriesIsDataLabels(bool nowFirstSeriesIsDataLabels = true);

            /**
             * Method you can use to specify if the first data series is used as a seroes/
             *
             * \param[in] nowFirstSeriesIsDataSeries If true, the first data series should be used as a data series.
             *                                       If false, the first data series is used as a source for labels
             *                                       for the other data series.
             */
            void setFirstSeriesIsDataSeries(bool nowFirstSeriesIsDataSeries = true);

            /**
             * Method you can use to determine the selected baseline axis for bar charts and similar types of plots.
             *
             * \return Returns the selected baseline axis.
             */
            AxisLocation baselineAxis() const;

            /**
             * Method you can use to set the desired baseline axis for the plot.
             *
             * \param[in] newBaselineAxis The newly selected baseline axis.
             */
            void setBaselineAxis(AxisLocation newBaselineAxis);

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
             * Method you can overload to receive notification that the XML description for this format is about to
             * be read.  You can use this call to reset any child format information to a default state.
             *
             * If you overload this method, be sure to call the base class version.
             */
            void aboutToReadXml() override;

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
             * Method used to update a plot series.
             *
             * \param[in] seriesIndex The zero based index of the plot series of interest.  If the index is invalid,
             *                        default plot series will be added until the specified series index is reached.
             *
             * \param[in] plotSeries  The new plot series.
             */
            void setPlotSeriesHelper(unsigned seriesIndex, const PlotSeries& plotSeries);

            /**
             * The axis formats by location.
             */
            QMap<AxisLocation, ChartAxisFormat> currentAxisFormatsByLocation;

            /**
             * The list of defined plot series.
             */
            QList<PlotSeries> currentPlotSeries;

            /**
             * Flag indicating if the first data series should be interpreted as the source of data labels.
             */
            bool currentFirstSeriesIsDataLabels;

            /**
             * The currently selected baseline axis.  This value is only used for some types of plots.
             */
            AxisLocation currentBaselineAxis;
    };
}

#endif
