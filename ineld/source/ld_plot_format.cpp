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
* This file implements the \ref Ld::PlotFormat class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>
#include <QHash>
#include <QSet>

#include <cassert>

#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_program_file.h"
#include "ld_plot_element.h"
#include "ld_chart_line_style.h"
#include "ld_format.h"
#include "ld_chart_format.h"
#include "ld_plot_format_base.h"
#include "ld_plot_format.h"

/***********************************************************************************************************************
 * Ld::PlotFormat::Aggregation
 */

namespace Ld {
    PlotFormat::Aggregation::Aggregation() {}


    PlotFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* PlotFormat::Aggregation::creator(const QString&) {
        return new PlotFormat::Aggregation;
    }


    QString PlotFormat::Aggregation::typeName() const {
        return PlotFormat::formatName;
    }


    void PlotFormat::Aggregation::clear() {
        PlotFormatBase::Aggregation::clear();
        currentAxisFormatsByLocation.clear();
    }


    bool PlotFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<PlotFormat> plotFormat = formatInstance.dynamicCast<PlotFormat>();
        if (!plotFormat.isNull()) {
            Q_ASSERT(!plotFormat.isNull());

            success = PlotFormatBase::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                for (  QMap<AxisLocation, ChartAxisFormat>::const_iterator
                           it = plotFormat->currentAxisFormatsByLocation.constBegin(),
                           end = plotFormat->currentAxisFormatsByLocation.constEnd()
                     ; it != end
                     ; ++it
                    ) {
                    AxisLocation           location = it.key();
                    const ChartAxisFormat& format   = it.value();

                    currentAxisFormatsByLocation[location] << format;
                }

                currentFirstSeriesIsDataLabels << plotFormat->firstSeriesIsDataLabels();
                currentBaselineAxis << plotFormat->baselineAxis();
            }
        }

        return success;
    }


    QSet<ChartAxisFormat> PlotFormat::Aggregation::axisFormats(PlotFormat::AxisLocation axisLocation) const {
        return currentAxisFormatsByLocation.value(axisLocation);
    }


    const Format::BooleanAggregation& PlotFormat::Aggregation::firstSeriesIsDataLabels() const {
        return currentFirstSeriesIsDataLabels;
    }


    const QSet<PlotFormat::AxisLocation>& PlotFormat::Aggregation::baselineAxis() const {
        return currentBaselineAxis;
    }
}

/***********************************************************************************************************************
 * Ld::PlotFormat
 */

namespace Ld {
    const PlotFormat::AxisLocation PlotFormat::defaultBaselineAxis = PlotFormat::AxisLocation::BOTTOM_X_A_GM;
    const QString                  PlotFormat::formatName("PlotFormat");

    PlotFormat::PlotFormat() {
        currentFirstSeriesIsDataLabels = false;
        currentBaselineAxis            = defaultBaselineAxis;
    }


    PlotFormat::PlotFormat(
            const PlotFormat& other
        ):Format(
            other
        ),ChartFormat(
            other
        ),PlotFormatBase(
            other
        ),currentAxisFormatsByLocation(
            other.currentAxisFormatsByLocation
        ),currentPlotSeries(
            other.currentPlotSeries
        ) {
        currentFirstSeriesIsDataLabels = other.currentFirstSeriesIsDataLabels;
        currentBaselineAxis            = other.currentBaselineAxis;
    }


    PlotFormat::~PlotFormat() {}


    FormatPointer PlotFormat::clone() const {
        return FormatPointer(new PlotFormat(*this));
    }


    Format* PlotFormat::creator(const QString&) {
        return new PlotFormat();
    }


    QString PlotFormat::typeName() const {
        return formatName;
    }


    bool PlotFormat::isValid() const {
        return PlotFormatBase::isValid();
    }


    Format::Capabilities PlotFormat::capabilities() const {
        return PlotFormatBase::capabilities() << formatName;
    }


    void PlotFormat::clearAxisFormat() {
        currentAxisFormatsByLocation.clear();
        reportFormatUpdated();
    }


    void PlotFormat::setAxisFormat(PlotFormat::AxisLocation axisLocation, const ChartAxisFormat& axisFormat) {
        currentAxisFormatsByLocation[axisLocation] = axisFormat;
    }


    bool PlotFormat::removeAxisFormat(PlotFormat::AxisLocation axisLocation) {
        bool result;

        if (currentAxisFormatsByLocation.contains(axisLocation)) {
            currentAxisFormatsByLocation.remove(axisLocation);
            result = true;
        } else {
            result = false;
        }

        return result;
    }


    bool PlotFormat::axisFormatDefined(AxisLocation axisLocation) const {
        return currentAxisFormatsByLocation.contains(axisLocation);
    }


    QList<PlotFormat::AxisLocation> PlotFormat::definedAxisLocations() const {
        return currentAxisFormatsByLocation.keys();
    }


    ChartAxisFormat PlotFormat::axisFormat(PlotFormat::AxisLocation axisLocation) const {
        return currentAxisFormatsByLocation.value(axisLocation);
    }


    void PlotFormat::clearPlotSeries() {
        currentPlotSeries.clear();
        reportFormatUpdated();
    }


    unsigned PlotFormat::numberPlotSeries() const {
        return static_cast<unsigned>(currentPlotSeries.size());
    }


    bool PlotFormat::isDefaultPlotSeries(unsigned seriesIndex) const {
        bool isDefault;

        if (seriesIndex < static_cast<unsigned>(currentPlotSeries.size())) {
            unsigned          defaultIndex      = seriesIndex % PlotSeries::numberDefaultPlotSeries;
            const PlotSeries& defaultPlotSeries = PlotSeries::defaultPlotSeries[defaultIndex];

            isDefault = currentPlotSeries.at(seriesIndex) == defaultPlotSeries;
        } else {
            isDefault = true;
        }

        return isDefault;
    }


    const PlotSeries& PlotFormat::plotSeries(unsigned seriesIndex) const {
        const PlotSeries* result;

        if (seriesIndex < static_cast<unsigned>(currentPlotSeries.size())) {
            result = &(currentPlotSeries.at(seriesIndex));
        } else {
            unsigned defaultIndex = seriesIndex % PlotSeries::numberDefaultPlotSeries;
            result = PlotSeries::defaultPlotSeries + defaultIndex;
        }

        return *result;
    }


    void PlotFormat::setPlotSeries(unsigned seriesIndex, const PlotSeries& plotSeries) {
        setPlotSeriesHelper(seriesIndex, plotSeries);
        reportFormatUpdated();
    }


    bool PlotFormat::firstSeriesIsDataLabels() const {
        return currentFirstSeriesIsDataLabels;
    }


    bool PlotFormat::firstSeriesIsDataSeries() const {
        return !firstSeriesIsDataLabels();
    }


    void PlotFormat::setFirstSeriesIsDataLabels(bool nowFirstSeriesIsDataLabels) {
        currentFirstSeriesIsDataLabels = nowFirstSeriesIsDataLabels;
        reportFormatUpdated();
    }


    void PlotFormat::setFirstSeriesIsDataSeries(bool nowFirstSeriesIsDataSeries) {
        setFirstSeriesIsDataLabels(!nowFirstSeriesIsDataSeries);
    }


    PlotFormat::AxisLocation PlotFormat::baselineAxis() const {
        return currentBaselineAxis;
    }


    void PlotFormat::setBaselineAxis(PlotFormat::AxisLocation newBaselineAxis) {
        currentBaselineAxis = newBaselineAxis;
        reportFormatUpdated();
    }


    QString PlotFormat::toString() const {
        QString result = PlotFormatBase::toString() + QString(",%1").arg(PlotElement::toString(currentBaselineAxis));

        for (  QMap<AxisLocation, ChartAxisFormat>::const_iterator
                   it = currentAxisFormatsByLocation.constBegin(),
                   end = currentAxisFormatsByLocation.constEnd()
             ; it != end
             ; ++it
            ) {
            AxisLocation           location = it.key();
            const ChartAxisFormat& value    = it.value();

            QString locationString = PlotElement::toString(location, true);
            result += QString(",%1,%2").arg(locationString).arg(value.toString());
        }

        unsigned numberPlotSeries = static_cast<unsigned>(currentPlotSeries.size());
        for (unsigned seriesIndex=0 ; seriesIndex<numberPlotSeries ; ++seriesIndex) {
            if (!isDefaultPlotSeries(seriesIndex)) {
                result += QString(",%1,%2").arg(seriesIndex).arg(currentPlotSeries.at(seriesIndex).toString());
            }
        }

        return result;
    }


    QString PlotFormat::toCss() const {
        return QString();
    }


    void PlotFormat::aboutToReadXml() {
        PlotFormatBase::aboutToReadXml();
        currentAxisFormatsByLocation.clear();
    }


    void PlotFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (currentFirstSeriesIsDataLabels) {
            attributes.append("first_series_is_data_labels", currentFirstSeriesIsDataLabels);
        }

        if (currentBaselineAxis != defaultBaselineAxis) {
            attributes.append("baseline_axis", PlotElement::toString(currentBaselineAxis));
        }
    }


    void PlotFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned
        ) {
        if (attributes.hasAttribute("first_series_is_data_labels")) {
            bool success;
            currentFirstSeriesIsDataLabels = attributes.value<bool>("first_series_is_data_labels", &success);

            if (!success) {
                QString tag   = reader->qualifiedName().toString();
                QString value = attributes.value<QString>("first_series_is_data_labels");
                reader->raiseError(
                    tr("Tag \"%1\", invalid first_series_is_data_labels tag \"%2\".").arg(tag).arg(value)
                );
            }
        } else {
            currentFirstSeriesIsDataLabels = false;
        }

        if (attributes.hasAttribute("baseline_axis")) {
            QString baselineAxisString = attributes.value<QString>("baseline_axis");
            bool    success;

            currentBaselineAxis = PlotElement::toAxisLocation(baselineAxisString, &success);
            if (!success) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid baseline axis \"%2\".").arg(tag).arg(baselineAxisString));
            }
        } else {
            currentBaselineAxis = defaultBaselineAxis;
        }
    }


    void PlotFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        ChartFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        PlotFormatBase::writeAddImmediateAttributes(attributes, formats, programFile);
        PlotFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void PlotFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        ChartFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        PlotFormatBase::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        PlotFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }


    void PlotFormat::writeChildFormats(
            QSharedPointer<XmlWriter>       writer,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        PlotFormatBase::writeChildFormats(writer, formats, programFile);

        for (  QMap<AxisLocation, ChartAxisFormat>::const_iterator
                   it = currentAxisFormatsByLocation.constBegin(),
                   end = currentAxisFormatsByLocation.constEnd()
             ; it != end
             ; ++it
            ) {
            AxisLocation           location = it.key();
            const ChartAxisFormat& value    = it.value();

            QString locationString = PlotElement::toString(location, true);

            XmlAttributes attributes;
            value.writeAddAttributes(attributes, formats, programFile);

            writer->writeStartElement(QString("%1Axis").arg(locationString));
            writer->writeAttributes(attributes);
            writer->writeEndElement();

            const FontFormat& titleFont = value.titleFont();
            const FontFormat& numberFont = value.numberFont();

            if (titleFont.isNotDefault()) {
                XmlAttributes attributes;
                titleFont.writeAddAttributes(attributes, formats, programFile);

                writer->writeStartElement(QString("%1AxisTitleFont").arg(locationString));
                writer->writeAttributes(attributes);
                writer->writeEndElement();
            }

            if (numberFont.isNotDefault()) {
                XmlAttributes attributes;
                numberFont.writeAddAttributes(attributes, formats, programFile);

                writer->writeStartElement(QString("%1AxisNumberFont").arg(locationString));
                writer->writeAttributes(attributes);
                writer->writeEndElement();
            }

            const ChartLineStyle& majorAxisTickLineStyle = value.majorTickLineStyle();
            const ChartLineStyle& minorAxisTickLineStyle = value.minorTickLineStyle();

            if (majorAxisTickLineStyle != ChartAxisFormat::defaultMajorTickLineStyle) {
                XmlAttributes attributes;
                majorAxisTickLineStyle.writeAddAttributes(attributes, formats, programFile);

                writer->writeStartElement(QString("%1AxisMajorLineStyle").arg(locationString));
                writer->writeAttributes(attributes);
                writer->writeEndElement();
            }

            if (minorAxisTickLineStyle != ChartAxisFormat::defaultMinorTickLineStyle) {
                XmlAttributes attributes;
                minorAxisTickLineStyle.writeAddAttributes(attributes, formats, programFile);

                writer->writeStartElement(QString("%1AxisMinorLineStyle").arg(locationString));
                writer->writeAttributes(attributes);
                writer->writeEndElement();
            }
        }

        unsigned numberPlotSeries = static_cast<unsigned>(currentPlotSeries.size());
        for (unsigned seriesIndex=0 ; seriesIndex<numberPlotSeries ; ++seriesIndex) {
            if (!isDefaultPlotSeries(seriesIndex)) {
                const PlotSeries& plotSeries = currentPlotSeries.at(seriesIndex);
                XmlAttributes     attributes;
                plotSeries.writeAddAttributes(attributes, formats, programFile);

                writer->writeStartElement(QString("PlotSeries%1").arg(seriesIndex + 1));
                writer->writeAttributes(attributes);
                writer->writeEndElement();
            }
        }
    }


    bool PlotFormat::readChildFormat(
            QSharedPointer<XmlReader>  reader,
            const QString&             tagName,
            const FormatsByIdentifier& formats,
            const XmlAttributes&       attributes,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        bool isProcessed = ChartFormat::readChildFormat(reader, tagName, formats, attributes, programFile, xmlVersion);

        if (!isProcessed) {
            if (tagName.endsWith(QString("AxisTitleFont"))) {
                bool         ok;
                QString      locationString = tagName.left(tagName.indexOf(QString("AxisTitleFont")));
                AxisLocation axisLocation   = PlotElement::toAxisLocation(locationString, &ok);

                if (ok) {
                    FontFormat titleFont;
                    titleFont.readAttributes(reader, attributes, formats, programFile, xmlVersion);

                    currentAxisFormatsByLocation[axisLocation].setTitleFont(titleFont);
                    isProcessed = true;
                } else {
                    reader->raiseError(tr("Unknown tag \"%s\".").arg(tagName));
                }
            } else if (tagName.endsWith(QString("AxisNumberFont"))) {
                bool         ok;
                QString      locationString = tagName.left(tagName.indexOf(QString("AxisNumberFont")));
                AxisLocation axisLocation   = PlotElement::toAxisLocation(locationString, &ok);

                if (ok) {
                    FontFormat numberFont;
                    numberFont.readAttributes(reader, attributes, formats, programFile, xmlVersion);

                    currentAxisFormatsByLocation[axisLocation].setNumberFont(numberFont);
                    isProcessed = true;
                } else {
                    reader->raiseError(tr("Unknown tag \"%s\".").arg(tagName));
                }
            } else if (tagName.endsWith(QString("AxisMajorLineStyle"))) {
                bool         ok;
                QString      locationString = tagName.left(tagName.indexOf(QString("AxisMajorLineStyle")));
                AxisLocation axisLocation   = PlotElement::toAxisLocation(locationString, &ok);

                if (ok) {
                    ChartLineStyle lineStyle;
                    lineStyle.readAttributes(reader, attributes, formats, programFile, xmlVersion);

                    currentAxisFormatsByLocation[axisLocation].setMajorTickLineStyle(lineStyle);
                    isProcessed = true;
                } else {
                    reader->raiseError(tr("Unknown tag \"%s\".").arg(tagName));
                }
            } else if (tagName.endsWith(QString("AxisMinorLineStyle"))) {
                bool         ok;
                QString      locationString = tagName.left(tagName.indexOf(QString("AxisMinorLineStyle")));
                AxisLocation axisLocation   = PlotElement::toAxisLocation(locationString, &ok);

                if (ok) {
                    ChartLineStyle lineStyle;
                    lineStyle.readAttributes(reader, attributes, formats, programFile, xmlVersion);

                    currentAxisFormatsByLocation[axisLocation].setMinorTickLineStyle(lineStyle);
                    isProcessed = true;
                } else {
                    reader->raiseError(tr("Unknown tag \"%s\".").arg(tagName));
                }
            } else if (tagName.startsWith("PlotSeries")) {
                bool     ok;
                QString  seriesNumberString = tagName.mid(10);
                unsigned seriesNumber       = seriesNumberString.toUInt(&ok);

                if (ok && seriesNumber > 0) {
                    PlotSeries plotSeries;
                    plotSeries.readAttributes(reader, attributes, formats, programFile, xmlVersion);

                    setPlotSeriesHelper(seriesNumber - 1, plotSeries);
                    isProcessed = true;
                } else {
                    reader->raiseError(tr("Unknown tag \"%s\".").arg(tagName));
                }
            } else if (tagName.endsWith(QString("Axis"))) {
                bool         ok;
                QString      locationString = tagName.left(tagName.indexOf(QString("Axis")));
                AxisLocation axisLocation   = PlotElement::toAxisLocation(locationString, &ok);

                if (ok) {
                    FontFormat     titleFont              = ChartAxisFormat::defaultTitleFontFormat;
                    FontFormat     numberFont             = ChartAxisFormat::defaultNumberFontFormat;
                    ChartLineStyle majorAxisTickLineStyle = ChartAxisFormat::defaultMajorTickLineStyle;
                    ChartLineStyle minorAxisTickLineStyle = ChartAxisFormat::defaultMinorTickLineStyle;

                    if (currentAxisFormatsByLocation.contains(axisLocation)) {
                        const ChartAxisFormat& axisFormat = currentAxisFormatsByLocation[axisLocation];

                        titleFont              = axisFormat.titleFont();
                        numberFont             = axisFormat.numberFont();
                        majorAxisTickLineStyle = axisFormat.majorTickLineStyle();
                        minorAxisTickLineStyle = axisFormat.minorTickLineStyle();
                    }

                    ChartAxisFormat chartAxisFormat;
                    chartAxisFormat.readAttributes(reader, attributes, formats, programFile, xmlVersion);

                    chartAxisFormat.setTitleFont(titleFont);
                    chartAxisFormat.setNumberFont(numberFont);
                    chartAxisFormat.setMajorTickLineStyle(majorAxisTickLineStyle);
                    chartAxisFormat.setMinorTickLineStyle(minorAxisTickLineStyle);

                    currentAxisFormatsByLocation[axisLocation] = chartAxisFormat;
                    isProcessed = true;
                } else {
                    reader->raiseError(tr("Unknown tag \"%s\".").arg(tagName));
                }
            }
        }

        return isProcessed;
    }


    void PlotFormat::setPlotSeriesHelper(unsigned seriesIndex, const PlotSeries& plotSeries) {
        unsigned currentNumberPlotSeries = static_cast<unsigned>(currentPlotSeries.size());

        if (seriesIndex < currentNumberPlotSeries) {
            currentPlotSeries[seriesIndex] = plotSeries;
        } else {
            while (currentNumberPlotSeries < seriesIndex) {
                unsigned          defaultIndex      = seriesIndex % PlotSeries::numberDefaultPlotSeries;
                const PlotSeries& defaultPlotSeries = PlotSeries::defaultPlotSeries[defaultIndex];

                currentPlotSeries.append(defaultPlotSeries);
                ++currentNumberPlotSeries;
            }

            currentPlotSeries.append(plotSeries);
        }
    }
}
