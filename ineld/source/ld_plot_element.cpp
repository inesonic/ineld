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
* This file implements the \ref Ld::PlotElement class.
***********************************************************************************************************************/

#include <QCoreApplication>
#include <QString>
#include <QSharedPointer>

#include <limits>

#include <util_hash_functions.h>

#include "ld_data_type.h"
#include "ld_capabilities.h"
#include "ld_calculated_value.h"
#include "ld_plot_visual.h"
#include "ld_plot_element.h"

/***********************************************************************************************************************
 * Ld::PlotElement::SourceData
 */

namespace Ld {
    PlotElement::SourceData::SourceData() {
        currentAxisLocation = AxisLocation::LEFT_Y_R_RC;
    }


    PlotElement::SourceData::SourceData(const VariableName& variableName, PlotElement::AxisLocation axisLocation) {
        currentVariableName = variableName;
        currentAxisLocation = axisLocation;
    }


    PlotElement::SourceData::SourceData(
            const PlotElement::SourceData& other
        ):currentVariableName(
            other.currentVariableName
        ),currentAxisLocation(
            other.currentAxisLocation
        ) {}


    PlotElement::SourceData::~SourceData() {}


    void PlotElement::SourceData::setVariableName(const VariableName& newVariableName) {
        currentVariableName = newVariableName;
    }


    void PlotElement::SourceData::setText1(const QString& newText1) {
        currentVariableName.setText1(newText1);
    }


    void PlotElement::SourceData::setText2(const QString& newText2) {
        currentVariableName.setText2(newText2);
    }


    const VariableName& PlotElement::SourceData::variableName() const {
        return currentVariableName;
    }


    QString PlotElement::SourceData::text1() const {
        return currentVariableName.text1();
    }


    QString PlotElement::SourceData::text2() const {
        return currentVariableName.text2();
    }


    void PlotElement::SourceData::setAxisLocation(PlotElement::AxisLocation newAxisLocaiton) {
        currentAxisLocation = newAxisLocaiton;
    }


    PlotElement::AxisLocation PlotElement::SourceData::axisLocation() const {
        return currentAxisLocation;
    }


    PlotElement::SourceData& PlotElement::SourceData::operator=(const PlotElement::SourceData& other) {
        currentVariableName = other.currentVariableName;
        currentAxisLocation = other.currentAxisLocation;

        return *this;
    }
}

/***********************************************************************************************************************
 * Ld::PlotElement
 */

namespace Ld {
    const QString      PlotElement::elementName("Plot");
    const Capabilities PlotElement::childProvides = Capabilities::nonTextAnnotations;

    PlotElement::PlotElement() {
        setNumberTextRegions(1);
    }


    PlotElement::~PlotElement() {}


    Element* PlotElement::creator(const QString&) {
        return new PlotElement();
    }


    QString PlotElement::typeName() const {
        return elementName;
    }


    QString PlotElement::plugInName() const {
        return QString();
    }


    QString PlotElement::description() const {
        return tr("Plot");
    }


    DataType::ValueType PlotElement::valueType() const {
        return DataType::ValueType::NONE;
    }


    Capabilities PlotElement::childProvidesCapabilities() const {
        return childProvides;
    }


    void PlotElement::setVisual(PlotVisual *newVisual) {
        ElementWithNoChildren::setVisual(newVisual);
    }


    PlotVisual* PlotElement::visual() const {
        return dynamic_cast<PlotVisual*>(ElementWithNoChildren::visual());
    }


    PlotElement::ExportImageCapability PlotElement::exportImageCapability() const {
        return ExportImageCapability::THROUGH_VISUAL_EPHEMERAL;
    }


    void PlotElement::setPlotTitle(const QString& newPlotTitle) {
        currentTitle = newPlotTitle;
        elementDataChanged();
    }


    QString PlotElement::plotTitle() const {
        return currentTitle;
    }


    void PlotElement::setPlotSubTitle(const QString& newPlotSubTitle) {
        currentSubTitle = newPlotSubTitle;
        elementDataChanged();
    }


    QString PlotElement::plotSubTitle() const {
        return currentSubTitle;
    }


    void PlotElement::setPlotAxisTitle(PlotElement::AxisLocation axisLocation, const QString& axisTitle) {
        if (currentAxisTitlesByLocation.contains(axisLocation)) {
            QString currentTitle = currentAxisTitlesByLocation.value(axisLocation);
            if (currentTitle != axisTitle) {
                currentAxisTitlesByLocation.insert(axisLocation, axisTitle);
                elementDataChanged();
            }
        } else {
            currentAxisTitlesByLocation.insert(axisLocation, axisTitle);
            elementDataChanged();
        }
    }


    QString PlotElement::axisTitle(PlotElement::AxisLocation axisLocation) const {
        return currentAxisTitlesByLocation.value(axisLocation);
    }


    QList<PlotElement::AxisLocation> PlotElement::definedAxisTitles() const {
        return currentAxisTitlesByLocation.keys();
    }


    void PlotElement::setLegendTitle(unsigned seriesIndex, const QString& legendTitle) {
        if (setLegendTitleHelper(seriesIndex, legendTitle)) {
            elementDataChanged();
        }
    }


    QString PlotElement::legendTitle(unsigned seriesIndex) const {
        return currentLegendTitlesBySeriesIndex.at(seriesIndex);
    }


    const QList<QString>& PlotElement::legendTitles() const {
        return currentLegendTitlesBySeriesIndex;
    }


    void PlotElement::setDataSource(
            unsigned                  seriesIndex,
            unsigned                  sourceIndex,
            PlotElement::AxisLocation axisLocation,
            const VariableName&       variableName
        ) {
        if (setDataSourceHelper(seriesIndex, sourceIndex, axisLocation, variableName)) {
            elementDataChanged();
        }
    }


    VariableName PlotElement::dataSource(unsigned seriesIndex, unsigned sourceIndex) const {
        VariableName variableName;

        if (seriesIndex < static_cast<unsigned>(currentSourceDataBySourceBySeries.size())) {
            const QList<SourceData>& sourceList = currentSourceDataBySourceBySeries.at(seriesIndex);
            if (sourceIndex < static_cast<unsigned>(sourceList.size())) {
                variableName = sourceList.at(sourceIndex).variableName();
            }
        }

        return variableName;
    }


    unsigned PlotElement::numberDataSeries() const {
        return static_cast<unsigned>(currentSourceDataBySourceBySeries.size());
    }


    unsigned PlotElement::numberDataSources(unsigned seriesIndex) const {
        return   seriesIndex < static_cast<unsigned>(currentSourceDataBySourceBySeries.size())
               ? static_cast<unsigned>(currentSourceDataBySourceBySeries.at(seriesIndex).size())
               : 0;
    }


    PlotElement::AxisLocation PlotElement::axisLocationForSource(unsigned seriesIndex, unsigned sourceIndex) const {
        AxisLocation result = AxisLocation::LEFT_Y_R_RC;

        if (seriesIndex < static_cast<unsigned>(currentSourceDataBySourceBySeries.size())) {
            const QList<SourceData>& sourceList = currentSourceDataBySourceBySeries.at(seriesIndex);
            if (sourceIndex < static_cast<unsigned>(sourceList.size())) {
                result = sourceList.at(sourceIndex).axisLocation();
            }
        }

        return result;
    }


    void PlotElement::clearAxisTitles() {
        currentAxisTitlesByLocation.clear();
        elementDataChanged();
    }


    void PlotElement::clearLegendTitles() {
        currentLegendTitlesBySeriesIndex.clear();
    }


    void PlotElement::clearSeriesData() {
        currentLegendTitlesBySeriesIndex.clear();
        currentSourceDataBySourceBySeries.clear();

        elementDataChanged();
    }


    void PlotElement::clearDataSources() {
        currentSourceDataBySourceBySeries.clear();
    }


    void PlotElement::clearDataSources(unsigned seriesIndex) {
        if (seriesIndex < static_cast<unsigned>(currentSourceDataBySourceBySeries.size())) {
            currentSourceDataBySourceBySeries[seriesIndex].clear();
        }
    }


    bool PlotElement::requiresCalculatedDataToRender() const {
        return true;
    }


    unsigned PlotElement::numberCalculatedValues() const {
        return static_cast<unsigned>(currentCalculatedValues.size());
    }


    void PlotElement::setCalculatedValue(unsigned valueIndex, const CalculatedValue& calculatedValue) {
        unsigned currentNumberValues = static_cast<unsigned>(currentCalculatedValues.size());

        if (valueIndex < currentNumberValues) {
            currentCalculatedValues[valueIndex] = calculatedValue;
        } else {
            currentCalculatedValues.reserve(valueIndex + 1);
            while (currentNumberValues < valueIndex) {
                currentCalculatedValues.append(CalculatedValue());
                ++currentNumberValues;
            }

            currentCalculatedValues.append(calculatedValue);
        }

        ElementWithNoChildren::setCalculatedValue(valueIndex, calculatedValue);
    }


    void PlotElement::clearCalculatedValue() {
        currentCalculatedValues.clear();
    }


    CalculatedValue PlotElement::calculatedValue(unsigned valueIndex) const {
        return   valueIndex < static_cast<unsigned>(currentCalculatedValues.size())
               ? currentCalculatedValues.at(valueIndex)
               : CalculatedValue();
    }


    QString PlotElement::toString(PlotElement::AxisLocation legendLocation, bool pascalCase) {
        QString result;

        if (pascalCase) {
            switch (legendLocation) {
                case AxisLocation::LEFT_Y_R_RC:   { result = QString("LeftYRRC");    break; }
                case AxisLocation::BOTTOM_X_A_GM: { result = QString("BottomXAGM");  break; }
                case AxisLocation::RIGHT_Y_R_RY:  { result = QString("RightYRRY");   break; }
                case AxisLocation::TOP_X_A_GM:    { result = QString("TopXAGM");     break; }
                case AxisLocation::Z_B_BK:        { result = QString("ZBBK");        break; }

                default: {
                    assert(false);
                    break;
                }
            }
        } else {
            switch (legendLocation) {
                case AxisLocation::LEFT_Y_R_RC:   { result = QString("LEFT_Y_R_RC");    break; }
                case AxisLocation::BOTTOM_X_A_GM: { result = QString("BOTTOM_X_A_GM");  break; }
                case AxisLocation::RIGHT_Y_R_RY:  { result = QString("RIGHT_Y_R_RY");   break; }
                case AxisLocation::TOP_X_A_GM:    { result = QString("TOP_X_A_GM");     break; }
                case AxisLocation::Z_B_BK:        { result = QString("Z_B_BK");         break; }

                default: {
                    assert(false);
                    break;
                }
            }
        }

        return result;
    }


    PlotElement::AxisLocation PlotElement::toAxisLocation(const QString &str, bool *ok) {
        bool         isOk;
        AxisLocation result;

        QString s = str.trimmed().toLower();
        if (str == QString("LeftYRRC") || s == QString("left_y_r_rc")) {
            result = AxisLocation::LEFT_Y_R_RC;
            isOk   = true;
        } else if (str == QString("BottomXAGM") || s == QString("bottom_x_a_gm")) {
            result = AxisLocation::BOTTOM_X_A_GM;
            isOk   = true;
        } else if (str == QString("RightYRRY") || s == QString("right_y_r_ry")) {
            result = AxisLocation::RIGHT_Y_R_RY;
            isOk   = true;
        } else if (str == QString("TopXAGM") || s == QString("top_x_a_gm")) {
            result = AxisLocation::TOP_X_A_GM;
            isOk   = true;
        } else if (str == QString("ZBBK") || s == QString("z_b_bk")) {
            result = AxisLocation::Z_B_BK;
            isOk   = true;
        } else {
            result = AxisLocation::LEFT_Y_R_RC;
            isOk   = false;
        }

        if (ok != nullptr) {
            *ok = isOk;
        }

        return result;
    }


    void PlotElement::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        ElementWithNoChildren::writeAddAttributes(attributes, formats, programFile);
    }


    void PlotElement::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        ElementWithNoChildren::readAttributes(reader, attributes, formats, programFile, xmlVersion);
    }



    void PlotElement::writeChildren(
            QSharedPointer<XmlWriter>       writer,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */,
            const XmlAttributes&            /* inheritedAttributes */
        ) const {
        if (!currentTitle.isEmpty()) {
            writer->writeStartElement("TitleText");
            writer->writeCharacters(currentTitle);
            writer->writeEndElement();
        }

        if (!currentSubTitle.isEmpty()) {
            writer->writeStartElement("SubTitleText");
            writer->writeCharacters(currentSubTitle);
            writer->writeEndElement();
        }

        for (  QMap<AxisLocation, QString>::const_iterator
                   axisTitleIterator    = currentAxisTitlesByLocation.constBegin(),
                   axisTitleEndIterator = currentAxisTitlesByLocation.constEnd()
             ; axisTitleIterator != axisTitleEndIterator
             ; ++axisTitleIterator
            ) {
            AxisLocation   axisLocation = axisTitleIterator.key();
            const QString& axisTitle    = axisTitleIterator.value();

            XmlAttributes attributes;
            attributes.append("axis_location", toString(axisLocation));

            writer->writeStartElement("AxisTitleText");
            writer->writeAttributes(attributes);
            writer->writeCharacters(axisTitle);
            writer->writeEndElement();
        }

        unsigned numberDataSeries = static_cast<unsigned>(currentLegendTitlesBySeriesIndex.size());
        for (unsigned seriesIndex=0 ; seriesIndex<numberDataSeries ; ++seriesIndex) {
            const QString& seriesName = currentLegendTitlesBySeriesIndex.at(seriesIndex);
            if (!seriesName.isEmpty()) {
                XmlAttributes attributes;
                attributes.append("series", seriesIndex + 1);

                writer->writeStartElement("SeriesLabelText");
                writer->writeAttributes(attributes);
                writer->writeCharacters(seriesName);
                writer->writeEndElement();
            }
        }

        numberDataSeries = static_cast<unsigned>(currentSourceDataBySourceBySeries.size());
        for (unsigned seriesIndex=0 ; seriesIndex<numberDataSeries ; ++seriesIndex) {
            const QList<SourceData>& sourceDataList    = currentSourceDataBySourceBySeries.at(seriesIndex);
            unsigned                 numberDataSources = static_cast<unsigned>(sourceDataList.size());

            for (unsigned sourceIndex=0 ; sourceIndex<numberDataSources ; ++sourceIndex) {
                const SourceData& sourceData = sourceDataList.at(sourceIndex);

                XmlAttributes attributes;
                attributes.append("series", seriesIndex + 1);
                attributes.append("source", sourceIndex + 1);
                attributes.append("axis_location", toString(sourceData.axisLocation()));
                attributes.append("t1", sourceData.text1());
                if (!sourceData.text2().isEmpty()) {
                    attributes.append("t2", sourceData.text2());
                }

                writer->writeStartElement("DataSource");
                writer->writeAttributes(attributes);
                writer->writeEndElement();
            }
        }
    }


    void PlotElement::readChild(
            QSharedPointer<XmlReader>  reader,
            const QString&             tagName,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            const XmlAttributes&       attributes,
            unsigned                   xmlVersion
        ) {
        if (tagName == QString("TitleText")) {
            readTitleTextTag(reader, tagName, formats, programFile, attributes, xmlVersion);
        } else if (tagName == QString("SubTitleText")) {
            readSubTitleTextTag(reader, tagName, formats, programFile, attributes, xmlVersion);
        } else if (tagName == QString("AxisTitleText")) {
            readAxisTitleTextTag(reader, tagName, formats, programFile, attributes, xmlVersion);
        } else if (tagName == QString("SeriesLabelText")) {
            readSeriesLabelTextTag(reader, tagName, formats, programFile, attributes, xmlVersion);
        } else if (tagName == QString("DataSource")) {
            readDataSourcetTag(reader, tagName, formats, programFile, attributes, xmlVersion);
        } else {
            reader->raiseError(tr("Unexpected tag \"%1\".").arg(tagName));
        }
    }


    void PlotElement::readTitleTextTag(
            QSharedPointer<XmlReader>  reader,
            const QString&             tagName,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            const XmlAttributes&       /* attributes */,
            unsigned                   /* xmlVersion */
        ) {
        QString titleText = readTextTag(reader, tagName);
        if (!reader->hasError()) {
            currentTitle = titleText;
        }
    }


    void PlotElement::readSubTitleTextTag(
            QSharedPointer<XmlReader>  reader,
            const QString&             tagName,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            const XmlAttributes&       /* attributes */,
            unsigned                   /* xmlVersion */
        ) {
        QString subTitleText = readTextTag(reader, tagName);
        if (!reader->hasError()) {
            currentSubTitle = subTitleText;
        }

    }


    void PlotElement::readAxisTitleTextTag(
            QSharedPointer<XmlReader>  reader,
            const QString&             tagName,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            const XmlAttributes&       attributes,
            unsigned                   /* xmlVersion */
        ) {
        if (attributes.hasAttribute("axis_location")) {
            bool         ok;
            QString      axisLocationString = attributes.value<QString>("axis_location");
            AxisLocation axisLocation       = toAxisLocation(axisLocationString, &ok);

            if (ok) {
                if (!currentAxisTitlesByLocation.contains(axisLocation)) {
                    QString axisTitleText = readTextTag(reader, tagName);
                    if (!reader->hasError()) {
                        currentAxisTitlesByLocation.insert(axisLocation, axisTitleText);
                    }
                } else {
                    reader->raiseError(
                        tr("Tag \"%1\", duplicate axis location \"%2\".").arg(tagName).arg(axisLocationString)
                    );
                }
            } else {
                reader->raiseError(
                    tr("Tag \"%1\", invalid axis location \"%2\".").arg(tagName).arg(axisLocationString)
                );
            }
        } else {
            reader->raiseError(tr("Tag \"%1\", missing attribute \"axis_location\"").arg(tagName));
        }
    }


    void PlotElement::readSeriesLabelTextTag(
            QSharedPointer<XmlReader>  reader,
            const QString&             tagName,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            const XmlAttributes&       attributes,
            unsigned                   /* xmlVersion */
        ) {
        if (attributes.hasAttribute("series")) {
            bool         ok;
            unsigned     seriesNumber = attributes.value<unsigned>("series", &ok);

            if (ok && seriesNumber > 0) {
                QString seriesLabel = readTextTag(reader, tagName);
                setLegendTitleHelper(seriesNumber - 1, seriesLabel);
            } else {
                QString seriesString = attributes.value<QString>("series");
                reader->raiseError(tr("Tag \"%1\", invalid series \"%2\".").arg(tagName).arg(seriesString));
            }
        } else {
            reader->raiseError(tr("Tag \"%1\", missing \"series\" attribute.").arg(tagName));
        }
    }


    void PlotElement::readDataSourcetTag(
            QSharedPointer<XmlReader>  reader,
            const QString&             tagName,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            const XmlAttributes&       attributes,
            unsigned                   /* xmlVersion */
        ) {
        if (attributes.hasAttribute("series")) {
            bool     ok;
            unsigned seriesNumber = attributes.value<unsigned>("series", &ok);
            if (ok && seriesNumber > 0) {
                if (attributes.hasAttribute("source")) {
                    unsigned sourceNumber = attributes.value<unsigned>("source", &ok);
                    if (ok && sourceNumber > 0) {
                        if (attributes.hasAttribute("axis_location")) {
                            QString      axisLocationString = attributes.value<QString>("axis_location");
                            AxisLocation axisLocation       = toAxisLocation(axisLocationString, &ok);

                            if (ok) {
                                if (attributes.hasAttribute("t1")) {
                                    QString t1 = attributes.value<QString>("t1");
                                    QString t2 = attributes.value<QString>("t2", QString());

                                    ok = setDataSourceHelper(
                                        seriesNumber - 1,
                                        sourceNumber - 1,
                                        axisLocation,
                                        VariableName(t1, t2)
                                    );

                                    if (ok) {
                                        // Below is a sleezy way to find and gobble up the tag end.
                                        (void) readTextTag(reader, tagName);
                                    } else {
                                        reader->raiseError(
                                            tr("Tag \"%1\", duplicate data source %2/%3").arg(tagName)
                                                                                         .arg(seriesNumber)
                                                                                         .arg(sourceNumber)
                                        );
                                    }
                                } else {
                                    reader->raiseError(tr("Tag \"%1\", missing \"t1\" attribute.").arg(tagName));
                                }
                            } else {
                                reader->raiseError(
                                    tr("Tag \"%1\", invalid axis location \"%2\".").arg(tagName)
                                                                                   .arg(axisLocationString)
                                );
                            }
                        } else {
                            reader->raiseError(tr("Tag \"%1\", missing \"axis_location\" attribute.").arg(tagName));
                        }
                    } else {
                        QString sourceNumberString = attributes.value<QString>("source");
                        reader->raiseError(
                            tr("Tag \"%1\", invalid source attribute value \"%2\".").arg(tagName)
                                                                                    .arg(sourceNumberString)
                        );
                    }
                } else {
                    reader->raiseError(tr("Tag \"%1\", missing \"source\" attribute.").arg(tagName));
                }
            } else {
                QString seriesNumberString = attributes.value<QString>("series");
                reader->raiseError(
                    tr("Tag \"%1\", invalid series attribute value \"%2\".").arg(tagName).arg(seriesNumberString)
                );
            }
        } else {
            reader->raiseError(tr("Tag \"%1\", missing \"series\" attribute.").arg(tagName));
        }
    }


    QString PlotElement::readTextTag(QSharedPointer<XmlReader> reader, const QString& tagName) {
        QString              result;
        XmlReader::TokenType tokenType;

        do {
            tokenType = reader->readNext();

            if (tokenType == XmlReader::Characters) {
                result += reader->text().toString();
            } else if (tokenType == XmlReader::StartElement) {
                reader->raiseError(
                    tr("Unexpected tag \"%1\" under \"%2\".").arg(reader->qualifiedName()).arg(tagName)
                );
            } else if (tokenType != XmlReader::EndElement) {
                reader->raiseError(tr("Invalid primitive"));
            }
        } while (!reader->hasError() && tokenType != XmlReader::EndElement);

        return result;
    }


    bool PlotElement::setLegendTitleHelper(unsigned seriesIndex, const QString& legendTitle) {
        bool result = false;

        unsigned currentNumberLegendTitles = static_cast<unsigned>(currentLegendTitlesBySeriesIndex.size());
        if (seriesIndex < currentNumberLegendTitles) {
            const QString& currentLegendTitle = currentLegendTitlesBySeriesIndex.at(seriesIndex);
            if (currentLegendTitle != legendTitle) {
                currentLegendTitlesBySeriesIndex[seriesIndex] = legendTitle;
                result = true;
            }
        } else {
            while (currentNumberLegendTitles < seriesIndex) {
                currentLegendTitlesBySeriesIndex.append(QString());
                currentSourceDataBySourceBySeries.append(QList<SourceData>());
                ++currentNumberLegendTitles;
            }

            currentLegendTitlesBySeriesIndex.append(legendTitle);
            currentSourceDataBySourceBySeries.append(QList<SourceData>());

            result = true;
        }

        return result;
    }


    bool PlotElement::setDataSourceHelper(
            unsigned                  seriesIndex,
            unsigned                  sourceIndex,
            PlotElement::AxisLocation axisLocation,
            const VariableName&       variableName
        ) {
        bool result;

        unsigned currentNumberSeries = static_cast<unsigned>(currentSourceDataBySourceBySeries.size());
        if (seriesIndex < currentNumberSeries) {
            result = addDataSourceHelper(
                currentSourceDataBySourceBySeries[seriesIndex],
                sourceIndex,
                variableName,
                axisLocation
            );
        } else {
            while (currentNumberSeries <= seriesIndex) {
                currentLegendTitlesBySeriesIndex.append(QString());
                currentSourceDataBySourceBySeries.append(QList<SourceData>());
                ++currentNumberSeries;
            }

            (void) addDataSourceHelper(
                currentSourceDataBySourceBySeries.last(),
                sourceIndex,
                variableName,
                axisLocation
            );

            result = true;
        }

        return result;
    }


    bool PlotElement::addDataSourceHelper(
            QList<PlotElement::SourceData>& sourceList,
            unsigned                        sourceIndex,
            const VariableName&             variableName,
            PlotElement::AxisLocation       axisLocation
        ) {
        bool     result         = false;
        unsigned sourceListSize = static_cast<unsigned>(sourceList.size());

        if (sourceIndex < sourceListSize) {
            SourceData& sourceData = sourceList[sourceIndex];
            if (sourceData.variableName() != variableName || sourceData.axisLocation() != axisLocation) {
                sourceList[sourceIndex] = SourceData(variableName, axisLocation);
                result = true;
            }
        } else {
            while (sourceListSize < sourceIndex) {
                sourceList.append(SourceData());
                ++sourceListSize;
            }

            sourceList.append(SourceData(variableName, axisLocation));
            result = true;
        }

        return result;
    }


    Util::HashResult qHash(PlotElement::AxisLocation value, Util::HashSeed seed) {
        return ::qHash(static_cast<int>(value), seed);
    }
}
