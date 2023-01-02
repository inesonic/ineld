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
* This header defines the \ref Ld::PlotElement class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_PLOT_ELEMENT_H
#define LD_PLOT_ELEMENT_H

#include <QString>
#include <QList>
#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QSharedPointer>

#include <util_hash_functions.h>

#include "ld_common.h"
#include "ld_data_type.h"
#include "ld_calculated_value.h"
#include "ld_variable_name.h"
#include "ld_capabilities.h"
#include "ld_special_symbol_element_base.h"

namespace Ld {
    class PlotVisual;

    /**
     * Element that provides support for plots and graphs.
     *
     * Text regions are used to store details regarding the plots with locations hard coded or calculated
     * algorithmically.
     */
    class LD_PUBLIC_API PlotElement:public ElementWithNoChildren {
        Q_DECLARE_TR_FUNCTIONS(Ld::PlotElement)

        public:
            /**
             * The typename for the text element.
             */
            static const QString elementName;

            /**
             * Static value indicating the capabilities provides by this element.
             */
            static const Capabilities childProvides;

            /**
             * Enumeration of supported axis locations.
             *
             * Note that the GUI depends on horizontal axis having a lower value than vertical axis.
             */
            enum class AxisLocation {
                /**
                 * Indicates an axis on the bottom.  Also indicates the X axis in a rotated 3D projection or the
                 * latitude in a polar projection.
                 */
                BOTTOM_X_A_GM = 0,

                /**
                 * Indicatea an axis on the top.  Also indicates the Y axis in a rotated 3D projection or the
                 * radius in a polar projection.
                 */
                TOP_X_A_GM = 1,

                /**
                 * Indicates an axis on the left side.  Also indicates the Y axis in a rotated 3D projection or the
                 * radius in a polar projection.
                 */
                LEFT_Y_R_RC = 2,

                /**
                 * Indicates an axis on the right.  Also indicates the Y axis in a rotated 3D projection or the
                 * radius in a polar projection.
                 */
                RIGHT_Y_R_RY = 3,

                /**
                 * Indicates the Z axis in a rotated 3D projection.  Also indicates the longtitude in a polar
                 * projection.
                 */
                Z_B_BK = 4,

                /**
                 * Number of supported plot axis locations.
                 */
                NUMBER_AXIS_LOCATIONS = 5
            };

            /**
             * Convenience method that calculates the calculated value number for a given source.
             *
             * \param[in] seriesIndex                The zero based series index.
             *
             * \param[in] sourceIndex                The zero based index of the data source.
             *
             * \param[in] numberSourcesPerDataSeries The number of data sources per data series.
             *
             * \return Returns the desired region index.
             */
            static inline unsigned calculatedValueNumber(
                    unsigned seriesIndex,
                    unsigned sourceIndex,
                    unsigned numberSourcesPerDataSeries
                ) {
                return seriesIndex * numberSourcesPerDataSeries + sourceIndex;
            }

            PlotElement();

            ~PlotElement() override;

            /**
             * Factory method that can be used to create a default instance of a plot element.
             *
             * \param[in] typeName The name used to reference the creator function.
             *
             * \return Returns a newly created, default, text element instance.
             */
            static Element* creator(const QString& typeName);

            /**
             * Method that returns the type name associated with this object.
             *
             * \return Returns the typename associated with this object.  The value must match the type used to invoke
             *         the static \ref Element::create and \ref Element::registerCreator methods.
             */
            QString typeName() const override;

            /**
             * Method that returns the name of the plug-in that provides this element.
             *
             * \return Returns the name of the plug-in that provides this element. The name must match the name provided
             *         in the plug-in structures.  Elements defined as part of the ineld library should return an empty
             *         string.
             */
            QString plugInName() const override;

            /**
             * Method that returns a description of this element.
             *
             * \return Returns a description of this element suitable for use in a GUI.
             */
            QString description() const override;

            /**
             * Method you can use to determine the value type this element represents.  Elements typically either
             * either report a fixed value or calculate the value based on their children.
             *
             * \return Returns the value type this element would represent.  A value of ValueType::NONE indicates
             *         either that the element does not return a value or the value type could not be determined at
             *         this time.
             */
            DataType::ValueType valueType() const override;

            /**
             * Method you can call to determine what capabilities this element will provide.  Note that this object
             * also provides a static constant value named "childProvides" returning the same information.
             *
             * \return Returns the requirements met by this element.
             */
            Capabilities childProvidesCapabilities() const final;

            /**
             * Method you can use to change the object used to manage visual representation of this element.  Note that
             * using this method will also cause the \ref Visual::element method to point back to this
             * \ref Ld::PlotElement instance.
             *
             * If the newly added \ref Ld::PlotVisual already points to a different element, then
             * those connections will be broken in order to maintain a 1:1 relationship between any
             * \ref Ld::PlotVisual instance and \ref Ld::PlotElement instance.
             *
             * If this element currently has an associated visual, then that visual be will disconnected from this
             * element.  The disconnected visual will *not* be deleted.
             *
             * \param[in] newVisual A pointer to the new visual object.  Note that this element will take ownership of
             *                      the visual.
             */
            void setVisual(PlotVisual* newVisual);

            /**
             * Method you can use to obtain the visual representation of this element.
             *
             * \return Returns a pointer to the visual representation of this element.  A null pointer is returned if
             *         no visual representation has been defined.
             */
            PlotVisual* visual() const;

            /**
             * Method you can use to determine if this element supports image export capability.
             *
             * \return Returns the export image capability for this element.  This implementatin returns
             *         \ref Ld::Element::ExportImageCapability::THROUGH_VISUAL_EPHEMERAL.
             */
            ExportImageCapability exportImageCapability() const override;

            /**
             * Method you can use to set the plot title.
             *
             * \param[in] newPlotTitle The new plot title.
             */
            void setPlotTitle(const QString& newPlotTitle);

            /**
             * Method you can use to obtain the current plot title.
             *
             * \return Returns the current plot title.
             */
            QString plotTitle() const;

            /**
             * Method you can use to set the plot sub-title.  This parameter is provided but currently unused.
             *
             * \param[in] newPlotSubTitle The new plot sub-title.
             */
            void setPlotSubTitle(const QString& newPlotSubTitle);

            /**
             * Method you can use to obtain the current plot sub-title.
             *
             * \return Returns the current plot sub-title.
             */
            QString plotSubTitle() const;

            /**
             * Method you can use to set the label for a particular axis.
             *
             * \param[in] axisLocation The axis of interest.
             *
             * \param[in] axisTitle    The title for the axis.
             */
            void setPlotAxisTitle(AxisLocation axisLocation, const QString& axisTitle);

            /**
             * Method you can use to obtain the title for a given axis.
             *
             * \param[in] axisLocation The axis of interest.
             *
             * \return Returns the axis title for the axis.  An empty string is returned if the axis has not been
             *         defined.
             */
            QString axisTitle(AxisLocation axisLocation) const;

            /**
             * Method you can use to determine which axis titles are currently defined.
             *
             * \return Returns a list of axis locations indicating the defined axis titles.
             */
            QList<AxisLocation> definedAxisTitles() const;

            /**
             * Method you can use to set the legend title for a given data series.
             *
             * \param[in] seriesIndex The zero based index of the data series.
             *
             * \param[in] legendTitle The title to apply to the series.
             */
            void setLegendTitle(unsigned seriesIndex, const QString& legendTitle);

            /**
             * Method you can use to obtain the legend title for a given data series.
             *
             * \param[in] seriesIndex The zero based index of the data series.
             *
             * \return Returns the title for the series.
             */
            QString legendTitle(unsigned seriesIndex) const;

            /**
             * Method you can use to obtain a list of all the legend titles by series index.
             *
             * \return Returns a list of legend titles by series indexes.
             */
            const QList<QString>& legendTitles() const;

            /**
             * Method you can use to set the name of a data source used by a series.
             *
             * \param[in] seriesIndex  The zero based series index of interest.
             *
             * \param[in] sourceIndex  The zero based index of the source within the series.
             *
             * \param[in] axisLocation The axis location of interest.
             *
             * \param[in] variableName The name of the variable.
             */
            void setDataSource(
                unsigned            seriesIndex,
                unsigned            sourceIndex,
                AxisLocation        axisLocation,
                const VariableName& variableName
            );

            /**
             * Method you can use to obtain the name of a data source used by a series.
             *
             * \param[in] seriesIndex  The zero based series index of interest.
             *
             * \param[in] sourceIndex  The zero based index of the source within the series.
             *
             * \return Returns the name of the data source.  Returns an empty variable name if the series index is
             *         not defined or the axis location is not defined.
             */
            VariableName dataSource(unsigned seriesIndex, unsigned sourceIndex) const;

            /**
             * Method you can use to determine the number of data series.
             *
             * \return Returns the number of data series.
             */
            unsigned numberDataSeries() const;

            /**
             * Method you can use to determine the number of data sources within given series.
             *
             * \param[in] seriesIndex The zero based series index.
             *
             * \return Returns the number of data sources associated with the series.  A value of 0 is returned if the
             *         series index is invalid.
             */
            unsigned numberDataSources(unsigned seriesIndex) const;

            /**
             * Method that returns the axis locations for a given data source.
             *
             * \param[in] seriesIndex The zero based series index of interest.
             *
             * \param[in] sourceIndex The zero based index of the source within the series.
             *
             * \return Returns the axis location for the specified source.
             */
            AxisLocation axisLocationForSource(unsigned seriesIndex, unsigned sourceIndex) const;

            /**
             * Method that clears the axis titles.
             */
            void clearAxisTitles();

            /**
             * Method you can use to clear all the legend titles.
             */
            void clearLegendTitles();

            /**
             * Method that clears the series data, specifically the legend titles and data sources.
             */
            void clearSeriesData();

            /**
             * Method you can use to clear all the data sources.
             */
            void clearDataSources();

            /**
             * Method that clears the data sources for a specific data series.
             *
             * \param[in] seriesIndex The series index of interest.
             */
            void clearDataSources(unsigned seriesIndex);

            /**
             * Method you can overload to indicate if this element requires calculated data to fully render content.
             *
             * \return Returns true if this element requires calculated data to fully render its content.  Returns
             *         false if this element does not require calculated data to fully render content.  This version
             *         returns true.
             */
            bool requiresCalculatedDataToRender() const override;

            /**
             * Method you can overload to determine the number of distinct calculated values supported by this
             * element.
             *
             * \return Returns the number of calculated values supported by this element.
             */
            unsigned numberCalculatedValues() const override;

            /**
             * Method you can overload to receive notification of calculated values associated with this element.  The
             * default method simply reports the data to any visual tied to this element.
             *
             * \param[in] valueIndex      A zero based index used to indicate which calculated value to update.
             *
             * \param[in] calculatedValue The calculated value.
             */
            void setCalculatedValue(unsigned valueIndex, const CalculatedValue& calculatedValue) override;

            /**
             * Method you can overload to receive notification when the calculated value should be cleared.  The
             * default method simply reports the clearing event to the visual.
             */
            void clearCalculatedValue() override;

            /**
             * Method you can overload to obtain the last reported calculated value.
             *
             * \param[in] valueIndex A zero based index used to reference the desired calculated value.
             *
             * \return Returns the last reported calculated value.  An invalid calculated value is returned if the
             *         index is invalid.
             */
            CalculatedValue calculatedValue(unsigned valueIndex = 0) const override;

            /**
             * Method you can use to convert an axis location to a string.
             *
             * \param[in] axisLocation The axis location to convert to a string.
             *
             * \param[in] pascalCase   If true, the name will be presented in PascalCase.  If false, the name will
             *                         be presented in upper-case with underscores.
             *
             * \return Returns a string representation of the axis location.
             */
            static QString toString(AxisLocation axisLocation, bool pascalCase = false);

            /**
             * Method you can use to convert a string into an axis lcoation.
             *
             * \param[in]  str The string to be converted.
             *
             * \param[out] ok  Pointer to a location that will hold true on success, false if the string can not be
             *                 converted.
             *
             * \return Returns the axis location.
             */
            static AxisLocation toAxisLocation(const QString& str, bool* ok = nullptr);

        protected:
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
             * Trivial class used to store information about a specific data source.
             */
            class SourceData{
                public:
                    SourceData();

                    /**
                     * Constructor
                     *
                     * \param[in] variableName The name of the variable associated with this source.
                     *
                     * \param[in] axisLocation The location of the axis to use for this source.
                     */
                    SourceData(const VariableName& variableName, AxisLocation axisLocation);

                    /**
                     * Copy constructor
                     *
                     * \param[in] other The instance to assign to this instance.
                     */
                    SourceData(const SourceData& other);

                    ~SourceData();

                    /**
                     * Method you can use to set the source variable name.
                     *
                     * \param[in] newVariableName The new source variable name.
                     */
                    void setVariableName(const VariableName& newVariableName);

                    /**
                     * Method you can use to set the variable name, text 1.
                     *
                     * \param[in] newText1 The new variable name, text 1.
                     */
                    void setText1(const QString& newText1);

                    /**
                     * Method you can use to set the variable name, text 2.
                     *
                     * \param[in] newText2 The new variable name, text2.
                     */
                    void setText2(const QString& newText2);

                    /**
                     * Method you can use obtain the current variable name.
                     *
                     * \return Returns the current variable name.
                     */
                    const VariableName& variableName() const;

                    /**
                     * Method you can use to obtain the current variable name, text 1.
                     *
                     * \return Returns the current variable name, text 1.
                     */
                    QString text1() const;

                    /**
                     * Method you can use to obtain the current variable name, text 2.
                     *
                     * \return Returns the current variable name, text 2.
                     */
                    QString text2() const;

                    /**
                     * Method you can use to change the current axis location.
                     *
                     * \param[in] newAxisLocation The new axis location.
                     */
                    void setAxisLocation(AxisLocation newAxisLocation);

                    /**
                     * Method you can use to obtain the current axis location.
                     *
                     * \return Returns the current axis location.
                     */
                    AxisLocation axisLocation() const;

                    /**
                     * Assignment operator
                     *
                     * \param[in] other The instance to assign to this instance.
                     *
                     * \return Returns a reference to this instance.
                     */
                    SourceData& operator=(const SourceData& other);

                private:
                    /**
                     * The current variable name.
                     */
                    VariableName currentVariableName;

                    /**
                     * The current axis location.
                     */
                    AxisLocation currentAxisLocation;
            };

            /**
             * Method that processes a "TitleText" tag.
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
            void readTitleTextTag(
                QSharedPointer<XmlReader>  reader,
                const QString&             tagName,
                const FormatsByIdentifier& formats,
                ProgramFile&               programFile,
                const XmlAttributes&       attributes,
                unsigned                   xmlVersion
            );

            /**
             * Method that processes a "SubTitleText" tag.
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
            void readSubTitleTextTag(
                QSharedPointer<XmlReader>  reader,
                const QString&             tagName,
                const FormatsByIdentifier& formats,
                ProgramFile&               programFile,
                const XmlAttributes&       attributes,
                unsigned                   xmlVersion
            );

            /**
             * Method that processes a "AxisTitleText" tag.
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
            void readAxisTitleTextTag(
                QSharedPointer<XmlReader>  reader,
                const QString&             tagName,
                const FormatsByIdentifier& formats,
                ProgramFile&               programFile,
                const XmlAttributes&       attributes,
                unsigned                   xmlVersion
            );

            /**
             * Method that processes a "SeriesLabelText" tag.
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
            void readSeriesLabelTextTag(
                QSharedPointer<XmlReader>  reader,
                const QString&             tagName,
                const FormatsByIdentifier& formats,
                ProgramFile&               programFile,
                const XmlAttributes&       attributes,
                unsigned                   xmlVersion
            );

            /**
             * Method that processes a "DataSourcet" tag.
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
            void readDataSourcetTag(
                QSharedPointer<XmlReader>  reader,
                const QString&             tagName,
                const FormatsByIdentifier& formats,
                ProgramFile&               programFile,
                const XmlAttributes&       attributes,
                unsigned                   xmlVersion
            );

            /**
             * Method that reads a tag that should contain a string of characters.
             *
             * \param[in] reader  The reader used to parse the incoming XML.
             *
             * \param[in] tagName The tag we're processing.  Used for error handling.
             *
             * \return Returns the captured string.
             */
            static QString readTextTag(QSharedPointer<XmlReader> reader, const QString& tagName);

            /**
             * Helper method that sets the legend title.
             *
             * \param[in] seriesIndex The zero based index of the data series.
             *
             * \param[in] legendTitle The title to apply to the series.
             *
             * \return Returns true if the title was changed.  Returns false if the title was not changed.
             */
            bool setLegendTitleHelper(unsigned seriesIndex, const QString& legendTitle);

            /**
             * Helper method you can use to set the name of a data source used by a series.
             *
             * \param[in] seriesIndex  The zero based series index of interest.
             *
             * \param[in] sourceIndex  The zero based index of the source within the series.
             *
             * \param[in] axisLocation The axis location of interest.
             *
             * \param[in] variableName The name of the variable.
             *
             * \return Returns true if the data source information was changed.  Returns false if the data source
             *         information was not changed.
             */
            bool setDataSourceHelper(
                unsigned            seriesIndex,
                unsigned            sourceIndex,
                AxisLocation        axisLocation,
                const VariableName& variableName
            );

            /**
             * Method used to add a new data source to a series.
             *
             * \param[in] sourceList   The list of sources of interest.
             *
             * \param[in] sourceIndex  The zero based index of the source.
             *
             * \param[in] variableName The name of the data source.
             *
             * \param[in] axisLocation The axis associated with the data source.
             */
            bool addDataSourceHelper(
                QList<SourceData>&  sourceList,
                unsigned            sourceIndex,
                const VariableName& variableName,
                AxisLocation        axisLocation
            );

            /**
             * The current plot title text.
             */
            QString currentTitle;

            /**
             * The current plot subtitle.
             */
            QString currentSubTitle;

            /**
             * Map of axis titles by axis location.
             */
            QMap<AxisLocation, QString> currentAxisTitlesByLocation;

            /**
             * List of legend titles by series index.
             */
            QList<QString> currentLegendTitlesBySeriesIndex;

            /**
             * List of data sources by source index, by series index.
             */
            QList<QList<SourceData>> currentSourceDataBySourceBySeries;

            /**
             * A list containing all the calculated values reported to the plot.
             */
            QList<CalculatedValue> currentCalculatedValues;
    };

    /**
     * Hash function for the \ref Ld::PlotFormat::LegendLocation enumeration.
     *
     * \param[in] value The value to be hashed.
     *
     * \param[in] seed  An optional seed to apply to the hash operation.
     *
     * \return Returns a hash for the provided line style.
     */
    LD_PUBLIC_API Util::HashResult qHash(PlotElement::AxisLocation value, Util::HashSeed seed = 0);
};

#endif
