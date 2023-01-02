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
* This file implements the \ref Ld::PlotFormatBase class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>
#include <QHash>
#include <QSet>

#include <cassert>

#include <util_hash_functions.h>

#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_program_file.h"
#include "ld_chart_line_style.h"
#include "ld_format.h"
#include "ld_chart_format.h"
#include "ld_plot_format_base.h"

/***********************************************************************************************************************
 * Ld::PlotFormatBase::Aggregation
 */

namespace Ld {
    PlotFormatBase::Aggregation::Aggregation() {}


    PlotFormatBase::Aggregation::~Aggregation() {}


    Format::Aggregation* PlotFormatBase::Aggregation::creator(const QString&) {
        return new PlotFormatBase::Aggregation;
    }


    QString PlotFormatBase::Aggregation::typeName() const {
        return PlotFormatBase::formatName;
    }


    void PlotFormatBase::Aggregation::clear() {
        ChartFormat::Aggregation::clear();

        currentPlotAreaLeftMargins.clear();
        currentPlotAreaRightMargins.clear();
        currentPlotAreaTopMargins.clear();
        currentPlotAreaBottomMargins.clear();
        currentPlotBackgroundAreaColors.clear();
        currentPlotDrawingAreaBackgroundColors.clear();
        currentPlotTitleFontFormats.clear();
        currentLegendLocations.clear();
        currentLegendFontFormats.clear();
        currentLegendBorderlineStyles.clear();
        currentLegendBackgroundColors.clear();
        currentPlotDrawingAreaOutlineStyles.clear();
    }


    bool PlotFormatBase::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<PlotFormatBase> plotFormatBase = formatInstance.dynamicCast<PlotFormatBase>();
        if (!plotFormatBase.isNull()) {
            Q_ASSERT(!plotFormatBase.isNull());

            success = ChartFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentPlotAreaLeftMargins << plotFormatBase->currentPlotAreaLeftMargin;
                currentPlotAreaRightMargins << plotFormatBase->currentPlotAreaRightMargin;
                currentPlotAreaTopMargins << plotFormatBase->currentPlotAreaTopMargin;
                currentPlotAreaBottomMargins << plotFormatBase->currentPlotAreaBottomMargin;
                currentPlotBackgroundAreaColors << plotFormatBase->currentPlotBackgroundAreaColor;
                currentPlotDrawingAreaBackgroundColors << plotFormatBase->currentPlotDrawingAreaBackgroundColor;
                currentPlotTitleFontFormats << plotFormatBase->currentPlotTitleFontFormat;
                currentLegendLocations << plotFormatBase->currentLegendLocation;
                currentLegendFontFormats << plotFormatBase->currentLegendFontFormat;
                currentLegendBorderlineStyles << plotFormatBase->currentLegendBorderlineStyle;
                currentLegendBackgroundColors << plotFormatBase->currentLegendBackgroundColor;
                currentPlotDrawingAreaOutlineStyles << plotFormatBase->currentPlotDrawingAreaOutlineStyle;
            }
        }

        return success;
    }

    const QSet<float>& PlotFormatBase::Aggregation::plotAreaLeftMargins() const {
        return currentPlotAreaLeftMargins;
    }


    const QSet<float>& PlotFormatBase::Aggregation::plotAreaRightMargins() const {
        return currentPlotAreaRightMargins;
    }


    const QSet<float>& PlotFormatBase::Aggregation::plotAreaTopMargins() const {
        return currentPlotAreaTopMargins;
    }


    const QSet<float>& PlotFormatBase::Aggregation::plotAreaBottomMargins() const {
        return currentPlotAreaBottomMargins;
    }


    const QSet<QColor>& PlotFormatBase::Aggregation::plotBackgroundAreaColors() const {
        return currentPlotBackgroundAreaColors;
    }


    const QSet<QColor>& PlotFormatBase::Aggregation::plotDrawingAreaBackgroundColors() const {
        return currentPlotDrawingAreaBackgroundColors;
    }


    const QSet<FontFormat>& PlotFormatBase::Aggregation::titleFontFormats() const {
        return currentPlotTitleFontFormats;
    }


    const QSet<PlotFormatBase::LegendLocation>& PlotFormatBase::Aggregation::legendLocations() const {
        return currentLegendLocations;
    }


    const QSet<FontFormat>& PlotFormatBase::Aggregation::legendFontFormats() const {
        return currentLegendFontFormats;
    }


    const QSet<ChartLineStyle>& PlotFormatBase::Aggregation::legendBorderlineStyles() const {
        return currentLegendBorderlineStyles;
    }


    const QSet<QColor>& PlotFormatBase::Aggregation::legendBackgroundColors() const {
        return currentLegendBackgroundColors;
    }


    const QSet<ChartLineStyle>& PlotFormatBase::Aggregation::plotDrawingAreaOutlineStyles() const {
        return currentPlotDrawingAreaOutlineStyles;
    }
}

/***********************************************************************************************************************
 * Ld::PlotFormatBase
 */

namespace Ld {
    const QString                        PlotFormatBase::formatName("PlotFormatBase");
    const float                          PlotFormatBase::defaultChartTopMargin = 24.0F;
    const float                          PlotFormatBase::defaultChartLeftMargin = 24.0F;
    const float                          PlotFormatBase::defaultChartRightMargin = 24.0F;
    const float                          PlotFormatBase::defaultChartBottomMargin = 24.0F;
    const QColor                         PlotFormatBase::defaultPlotBackgroundAreaColor(Qt::GlobalColor::white);
    const QColor                         PlotFormatBase::defaultPlotDrawingAreaBackgroundColor(Qt::GlobalColor::white);
    const FontFormat                     PlotFormatBase::defaultPlotTitleFontFormat;
    const PlotFormatBase::LegendLocation PlotFormatBase::defaultLegendLocation =
                                             PlotFormatBase::LegendLocation::NO_LEGEND;
    const FontFormat                     PlotFormatBase::defaultLegendFontFormat;
    const ChartLineStyle                 PlotFormatBase::defaultLegendBorderlineStyle;
    const QColor                         PlotFormatBase::defaultLegendBackgroundColor(Qt::GlobalColor::white);
    const ChartLineStyle                 PlotFormatBase::defaultPlotDrawingAreaOutputStyle;

    PlotFormatBase::PlotFormatBase() {
        currentPlotAreaLeftMargin             = defaultChartLeftMargin;
        currentPlotAreaRightMargin            = defaultChartRightMargin;
        currentPlotAreaTopMargin              = defaultChartTopMargin;
        currentPlotAreaBottomMargin           = defaultChartBottomMargin;
        currentPlotBackgroundAreaColor        = defaultPlotBackgroundAreaColor;
        currentPlotDrawingAreaBackgroundColor = defaultPlotDrawingAreaBackgroundColor;
        currentPlotTitleFontFormat            = defaultPlotTitleFontFormat;
        currentLegendLocation                 = defaultLegendLocation;
        currentLegendFontFormat               = defaultLegendFontFormat;
        currentLegendBorderlineStyle          = defaultLegendBorderlineStyle;
        currentLegendBackgroundColor          = defaultLegendBackgroundColor;
        currentPlotDrawingAreaOutlineStyle    = defaultPlotDrawingAreaOutputStyle;
    }


    PlotFormatBase::PlotFormatBase(
            const PlotFormatBase& other
        ):Format(
            other
        ),ChartFormat(
            other
        ),currentPlotAreaLeftMargin(
            other.currentPlotAreaLeftMargin
        ),currentPlotAreaRightMargin(
            other.currentPlotAreaRightMargin
        ),currentPlotAreaTopMargin(
            other.currentPlotAreaTopMargin
        ),currentPlotAreaBottomMargin(
            other.currentPlotAreaBottomMargin
        ),currentPlotBackgroundAreaColor(
            other.currentPlotBackgroundAreaColor
        ),currentPlotDrawingAreaBackgroundColor(
            other.currentPlotDrawingAreaBackgroundColor
        ),currentPlotTitleFontFormat(
            other.currentPlotTitleFontFormat
        ),currentLegendLocation(
            other.currentLegendLocation
        ),currentLegendFontFormat(
            other.currentLegendFontFormat
        ),currentLegendBorderlineStyle(
            other.currentLegendBorderlineStyle
        ),currentLegendBackgroundColor(
            other.currentLegendBackgroundColor
        ),currentPlotDrawingAreaOutlineStyle(
            other.currentPlotDrawingAreaOutlineStyle
        ) {}


    PlotFormatBase::~PlotFormatBase() {}


    FormatPointer PlotFormatBase::clone() const {
        return FormatPointer(new PlotFormatBase(*this));
    }


    Format* PlotFormatBase::creator(const QString&) {
        return new PlotFormatBase();
    }


    QString PlotFormatBase::typeName() const {
        return formatName;
    }


    bool PlotFormatBase::isValid() const {
        return (
               ChartFormat::isValid()
            && currentPlotAreaLeftMargin >= 0
            && currentPlotAreaRightMargin >= 0
            && currentPlotAreaTopMargin >= 0
            && currentPlotAreaBottomMargin >= 0
            && currentPlotDrawingAreaOutlineStyle.isValid()
        );
    }


    Format::Capabilities PlotFormatBase::capabilities() const {
        return ChartFormat::capabilities() << formatName;
    }


    void PlotFormatBase::setPlotAreaLeftMargin(float newLeftMargin) {
        currentPlotAreaLeftMargin = newLeftMargin;
        reportFormatUpdated();
    }


    float PlotFormatBase::plotAreaLeftMargin() const {
        return currentPlotAreaLeftMargin;
    }


    void PlotFormatBase::setPlotAreaRightMargin(float newRightMargin) {
        currentPlotAreaRightMargin = newRightMargin;
        reportFormatUpdated();
    }


    float PlotFormatBase::plotAreaRightMargin() const {
        return currentPlotAreaRightMargin;
    }


    void PlotFormatBase::setPlotAreaTopMargin(float newTopMargin) {
        currentPlotAreaTopMargin = newTopMargin;
        reportFormatUpdated();
    }


    float PlotFormatBase::plotAreaTopMargin() const {
        return currentPlotAreaTopMargin;
    }


    void PlotFormatBase::setPlotAreaBottomMargin(float newBottomMargin) {
        currentPlotAreaBottomMargin = newBottomMargin;
        reportFormatUpdated();
    }


    float PlotFormatBase::plotAreaBottomMargin() const {
        return currentPlotAreaBottomMargin;
    }


    void PlotFormatBase::setPlotAreaBackgroundColor(const QColor& newBackgroundColor) {
        currentPlotBackgroundAreaColor = newBackgroundColor;
        reportFormatUpdated();
    }


    const QColor& PlotFormatBase::plotAreaBackgroundColor() const {
        return currentPlotBackgroundAreaColor;
    }


    void PlotFormatBase::setPlotDrawingAreaBackgroundColor(const QColor& newBackgroundColor) {
        currentPlotDrawingAreaBackgroundColor = newBackgroundColor;
        reportFormatUpdated();
    }


    const QColor& PlotFormatBase::plotDrawingAreaBackgroundColor() const {
        return currentPlotDrawingAreaBackgroundColor;
    }


    void PlotFormatBase::setTitleFontFormat(const FontFormat& newTitleFont) {
        currentPlotTitleFontFormat = newTitleFont;
        reportFormatUpdated();
    }


    const FontFormat& PlotFormatBase::titleFontFormat() const {
        return currentPlotTitleFontFormat;
    }


    void PlotFormatBase::setLegendLocation(PlotFormatBase::LegendLocation newLegendLocation) {
        currentLegendLocation = newLegendLocation;
        reportFormatUpdated();
    }


    PlotFormatBase::LegendLocation PlotFormatBase::legendLocation() const {
        return currentLegendLocation;
    }


    void PlotFormatBase::setLegendFontFormat(const FontFormat& newFontFormat) {
        currentLegendFontFormat = newFontFormat;
        reportFormatUpdated();
    }


    const FontFormat& PlotFormatBase::legendFontFormat() const {
        return currentLegendFontFormat;
    }


    void PlotFormatBase::setLegendBorderLineStyle(const ChartLineStyle& newLineStyle) {
        currentLegendBorderlineStyle = newLineStyle;
        reportFormatUpdated();
    }


    const ChartLineStyle& PlotFormatBase::legendBorderlineStyle() const {
        return currentLegendBorderlineStyle;
    }


    void PlotFormatBase::setLegendBackgroundColor(const QColor& newBackgroundColor) {
        currentLegendBackgroundColor = newBackgroundColor;
        reportFormatUpdated();
    }


    const QColor& PlotFormatBase::legendBackgroundColor() const {
        return currentLegendBackgroundColor;
    }


    void PlotFormatBase::setDrawingAreaOutlineStyle(const ChartLineStyle& newLineStyle) {
        currentPlotDrawingAreaOutlineStyle = newLineStyle;
        reportFormatUpdated();
    }


    const ChartLineStyle& PlotFormatBase::drawingAreaOutlineStyle() const {
        return currentPlotDrawingAreaOutlineStyle;
    }


    QString PlotFormatBase::toString() const {
        QString fontFormatString = currentPlotTitleFontFormat.toString();
        fontFormatString = fontFormatString.mid(fontFormatString.indexOf(QChar(',')) + 1);

        QString result = ChartFormat::toString();

        result += QString(",%1,%2,%3,%4,%5,%6,%7,%8,%9")
                  .arg(currentPlotAreaLeftMargin)
                  .arg(currentPlotAreaRightMargin)
                  .arg(currentPlotAreaTopMargin)
                  .arg(currentPlotAreaBottomMargin)
                  .arg(currentPlotBackgroundAreaColor.name(QColor::NameFormat::HexArgb))
                  .arg(currentPlotDrawingAreaBackgroundColor.name(QColor::NameFormat::HexArgb))
                  .arg(fontFormatString)
                  .arg(toString(currentLegendLocation))
                  .arg(currentLegendFontFormat.toString());

        result += QString(",%1,%2,%3")
                  .arg(currentLegendBorderlineStyle.toString())
                  .arg(currentLegendBackgroundColor.name(QColor::NameFormat::HexArgb))
                  .arg(currentPlotDrawingAreaOutlineStyle.toString());

        return result;
    }


    QString PlotFormatBase::toCss() const {
        return QString();
    }


    QString PlotFormatBase::toString(PlotFormatBase::LegendLocation legendLocation) {
        QString result;

        switch (legendLocation) {
            case LegendLocation::NO_LEGEND: { result = QString("NO_LEGEND");  break;}
            case LegendLocation::TOP:       { result = QString("TOP");        break; }
            case LegendLocation::LEFT:      { result = QString("LEFT");       break; }
            case LegendLocation::RIGHT:     { result = QString("RIGHT");      break; }
            case LegendLocation::BOTTOM:    { result = QString("BOTTOM");     break; }

            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    PlotFormatBase::LegendLocation PlotFormatBase::toLegendLocation(const QString& str, bool* ok) {
        bool           isOk;
        LegendLocation result;

        QString s = str.trimmed().toLower();
        if (s == QString("no_legend")) {
            result = LegendLocation::NO_LEGEND;
            isOk   = true;
        } else if (s == QString("top")) {
            result = LegendLocation::TOP;
            isOk   = true;
        } else if (s == QString("left")) {
            result = LegendLocation::LEFT;
            isOk   = true;
        } else if (s == QString("right")) {
            result = LegendLocation::RIGHT;
            isOk   = true;
        } else if (s == QString("bottom")) {
            result = LegendLocation::BOTTOM;
            isOk   = true;
        } else {
            result = LegendLocation::NO_LEGEND;
            isOk   = false;
        }

        if (ok != nullptr) {
            *ok = isOk;
        }

        return result;
    }


    void PlotFormatBase::aboutToReadXml() {
        ChartFormat::aboutToReadXml();

        currentPlotTitleFontFormat         = defaultPlotTitleFontFormat;
        currentPlotDrawingAreaOutlineStyle = defaultPlotDrawingAreaOutputStyle;
        currentLegendFontFormat            = defaultLegendFontFormat;
        currentLegendBorderlineStyle       = defaultLegendBorderlineStyle;
    }


    void PlotFormatBase::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (currentPlotAreaLeftMargin != defaultChartLeftMargin) {
            attributes.append("chart_left_margin", currentPlotAreaLeftMargin);
        }

        if (currentPlotAreaRightMargin != defaultChartRightMargin) {
            attributes.append("chart_right_margin", currentPlotAreaRightMargin);
        }

        if (currentPlotAreaTopMargin != defaultChartTopMargin) {
            attributes.append("chart_top_margin", currentPlotAreaTopMargin);
        }

        if (currentPlotAreaBottomMargin != defaultChartBottomMargin) {
            attributes.append("chart_bottom_margin", currentPlotAreaBottomMargin);
        }

        if (currentPlotBackgroundAreaColor != defaultPlotBackgroundAreaColor) {
            attributes.append("chart_background_color", currentPlotBackgroundAreaColor);
        }

        if (currentPlotDrawingAreaBackgroundColor != defaultPlotDrawingAreaBackgroundColor) {
            attributes.append("chart_drawing_area_background_color", currentPlotDrawingAreaBackgroundColor);
        }

        if (currentLegendLocation != defaultLegendLocation) {
            attributes.append("chart_legend_location", toString(currentLegendLocation));
        }

        if (currentLegendBackgroundColor != defaultLegendBackgroundColor) {
            attributes.append("chart_legend_background_color", currentLegendBackgroundColor);
        }
    }


    void PlotFormatBase::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier&,
            ProgramFile&,
            unsigned
        ) {
        bool ok;

        currentPlotAreaLeftMargin = attributes.value<float>("chart_left_margin", defaultChartLeftMargin, &ok);
        if (!ok || currentPlotAreaLeftMargin < 0) {
            QString value = attributes.value<QString>("chart_left_margin");
            QString tag   = reader->qualifiedName().toString();
            reader->raiseError(tr("Tag \"%1\", invalid chart left margin \"%2\".").arg(tag).arg(value));
        }

        if (!reader->hasError()) {
            currentPlotAreaRightMargin = attributes.value<float>("chart_right_margin", defaultChartRightMargin, &ok);
            if (!ok || currentPlotAreaRightMargin < 0) {
                QString value = attributes.value<QString>("chart_right_margin");
                QString tag   = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid chart right margin \"%2\".").arg(tag).arg(value));
            }
        }

        if (!reader->hasError()) {
            currentPlotAreaTopMargin = attributes.value<float>("chart_top_margin", defaultChartTopMargin, &ok);
            if (!ok || currentPlotAreaTopMargin < 0) {
                QString value = attributes.value<QString>("chart_top_margin");
                QString tag   = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid chart top margin \"%2\".").arg(tag).arg(value));
            }
        }

        if (!reader->hasError()) {
            currentPlotAreaBottomMargin = attributes.value<float>(
                "chart_bottom_margin",
                defaultChartBottomMargin,
                &ok
            );

            if (!ok || currentPlotAreaBottomMargin < 0) {
                QString value = attributes.value<QString>("chart_bottom_margin");
                QString tag   = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid chart bottom margin \"%2\".").arg(tag).arg(value));
            }
        }

        if (!reader->hasError() && attributes.hasAttribute("chart_background_color")) {
            currentPlotBackgroundAreaColor= attributes.colorValue("chart_background_color", &ok);
            if (!ok) {
                QString tag         = reader->qualifiedName().toString();
                QString valueString = attributes.value<QString>("chart_background_color");
                reader->raiseError(tr("Tag \"%1\", invalid chart background color \"%2\".").arg(tag).arg(valueString));
            }
        } else {
            currentPlotBackgroundAreaColor = defaultPlotBackgroundAreaColor;
        }

        if (!reader->hasError() && attributes.hasAttribute("chart_drawing_area_background_color")) {
            currentPlotDrawingAreaBackgroundColor= attributes.colorValue("chart_drawing_area_background_color", &ok);

            if (!ok) {
                QString tag         = reader->qualifiedName().toString();
                QString valueString = attributes.value<QString>("chart_drawing_area_background_color");
                reader->raiseError(
                    tr("Tag \"%1\", invalid chart drawing area background color \"%2\".").arg(tag).arg(valueString)
                );
            }
        } else {
            currentPlotDrawingAreaBackgroundColor = defaultPlotDrawingAreaBackgroundColor;
        }

        if (!reader->hasError() && attributes.hasAttribute("chart_legend_location")) {
            QString legendLocationString = attributes.value<QString>("chart_legend_location");

            bool ok;
            currentLegendLocation = toLegendLocation(legendLocationString, &ok);

            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(
                    tr("Tag \"%1\", invalid chart legend location \"%2\".").arg(tag).arg(legendLocationString)
                );
            }
        } else {
            currentLegendLocation = defaultLegendLocation;
        }

        if (!reader->hasError() && attributes.hasAttribute("chart_legend_background_color")) {
            currentLegendBackgroundColor= attributes.colorValue("chart_legend_background_color", &ok);

            if (!ok) {
                QString tag         = reader->qualifiedName().toString();
                QString valueString = attributes.value<QString>("chart_legend_background_color");
                reader->raiseError(
                    tr("Tag \"%1\", invalid legend background color \"%2\".").arg(tag).arg(valueString)
                );
            }
        } else {
            currentLegendBackgroundColor = defaultLegendBackgroundColor;
        }
    }


    void PlotFormatBase::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        ChartFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        PlotFormatBase::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void PlotFormatBase::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        ChartFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        PlotFormatBase::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }


    void PlotFormatBase::writeChildFormats(
            QSharedPointer<XmlWriter>       writer,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        ChartFormat::writeChildFormats(writer, formats, programFile);

        if (currentPlotTitleFontFormat != defaultPlotTitleFontFormat) {
            XmlAttributes attributes;
            currentPlotTitleFontFormat.writeAddAttributes(attributes, formats, programFile);

            writer->writeStartElement("TitleFontFormat");
            writer->writeAttributes(attributes);
            writer->writeEndElement();
        }

        if (currentPlotDrawingAreaOutlineStyle != currentPlotDrawingAreaOutlineStyle) {
            XmlAttributes attributes;
            currentPlotDrawingAreaOutlineStyle.writeAddImmediateAttributes(attributes, formats, programFile);

            writer->writeStartElement("DrawingAreaBorderLineStyle");
            writer->writeAttributes(attributes);
            writer->writeEndElement();
        }

        if (currentLegendFontFormat != defaultLegendFontFormat) {
            XmlAttributes attributes;
            currentLegendFontFormat.writeAddAttributes(attributes, formats, programFile);

            writer->writeStartElement("LegendFontFormat");
            writer->writeAttributes(attributes);
            writer->writeEndElement();
        }

        if (currentLegendBorderlineStyle != defaultLegendBorderlineStyle) {
            XmlAttributes attributes;
            currentLegendBorderlineStyle.writeAddAttributes(attributes, formats, programFile);

            writer->writeStartElement("LegendBorderLineStyle");
            writer->writeAttributes(attributes);
            writer->writeEndElement();
        }
    }


    bool PlotFormatBase::readChildFormat(
            QSharedPointer<XmlReader>  reader,
            const QString&             tagName,
            const FormatsByIdentifier& formats,
            const XmlAttributes&       attributes,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        bool isProcessed = ChartFormat::readChildFormat(reader, tagName, formats, attributes, programFile, xmlVersion);

        if (!isProcessed) {
            if (tagName == QString("TitleFontFormat")) {
                currentPlotTitleFontFormat.readAttributes(reader, attributes, formats, programFile, xmlVersion);
                isProcessed = true;
            } else if (tagName == QString("DrawingAreaBorderLineStyle")) {
                currentPlotDrawingAreaOutlineStyle.readImmediateAttributes(
                    reader,
                    attributes,
                    formats,
                    programFile,
                    xmlVersion
                );
                isProcessed = true;
            } else if (tagName == QString("LegendFontFormat")) {
                currentLegendFontFormat.readImmediateAttributes(
                    reader,
                    attributes,
                    formats,
                    programFile,
                    xmlVersion
                );
                isProcessed = true;
            } else if (tagName == QString("LegendBorderLineStyle")) {
                currentLegendBorderlineStyle.readImmediateAttributes(
                    reader,
                    attributes,
                    formats,
                    programFile,
                    xmlVersion
                );
                isProcessed = true;
            }
        }

        return isProcessed;
    }


    Util::HashResult qHash(PlotFormatBase::LegendLocation value, Util::HashSeed seed) {
        return ::qHash(static_cast<int>(value), seed);
    }
}
