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
* This file implements the \ref Ld::ChartFormat class.
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
#include "ld_chart_line_style.h"
#include "ld_format.h"
#include "ld_chart_format.h"

/***********************************************************************************************************************
 * Ld::ChartFormat::Aggregation
 */

namespace Ld {
    ChartFormat::Aggregation::Aggregation() {}


    ChartFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* ChartFormat::Aggregation::creator(const QString&) {
        return new ChartFormat::Aggregation;
    }


    QString ChartFormat::Aggregation::typeName() const {
        return ChartFormat::formatName;
    }


    void ChartFormat::Aggregation::clear() {
        Format::Aggregation::clear();

        currentChartTypes.clear();
        currentChartWidths.clear();
        currentChartHeights.clear();
        currentBorderLineStyles.clear();
    }


    bool ChartFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<ChartFormat> chartFormat = formatInstance.dynamicCast<ChartFormat>();
        if (!chartFormat.isNull()) {
            Q_ASSERT(!chartFormat.isNull());

            success = Format::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentChartTypes << chartFormat->chartType();
                currentChartWidths << chartFormat->chartWidth();
                currentChartHeights << chartFormat->chartHeight();
                currentBorderLineStyles << chartFormat->borderLineStyle();
            }
        }

        return success;
    }


    const QSet<QString>& ChartFormat::Aggregation::chartTypes() const {
        return currentChartTypes;
    }


    const QSet<float>& ChartFormat::Aggregation::chartWidths() const {
        return currentChartWidths;
    }


    const QSet<float>& ChartFormat::Aggregation::chartHeights() const {
        return currentChartHeights;
    }


    const QSet<ChartLineStyle>& ChartFormat::Aggregation::borderLineStyles() const {
        return currentBorderLineStyles;
    }
}

/***********************************************************************************************************************
 * Ld::ChartFormat
 */

namespace Ld {
    const QString ChartFormat::formatName("ChartFormat");
    const float   ChartFormat::defaultChartWidth  = 5.0F * 72.0F;
    const float   ChartFormat::defaultChartHeight = 4.0F * 72.0F;

    ChartFormat::ChartFormat() {
        currentChartWidth  = defaultChartWidth;
        currentChartHeight = defaultChartHeight;
    }


    ChartFormat::ChartFormat(
            const ChartFormat& other
        ):Format(
            other
        ),currentChartType(
            other.currentChartType
        ),currentChartWidth(
            other.currentChartWidth
        ),currentChartHeight(
            other.currentChartHeight
        ),currentBorderLineStyle(
            other.currentBorderLineStyle
        ) {}


    ChartFormat::~ChartFormat() {}


    FormatPointer ChartFormat::clone() const {
        return FormatPointer(new ChartFormat(*this));
    }


    Format* ChartFormat::creator(const QString&) {
        return new ChartFormat();
    }


    QString ChartFormat::typeName() const {
        return formatName;
    }


    bool ChartFormat::isValid() const {
        return (
               !currentChartType.isEmpty()
            && currentChartWidth > 0
            && currentChartHeight > 0
            && currentBorderLineStyle.isValid()
        );
    }


    Format::Capabilities ChartFormat::capabilities() const {
        return Format::capabilities() << formatName;
    }


    void ChartFormat::setChartType(const QString& newChartType) {
        currentChartType = newChartType;
        reportFormatUpdated();
    }


    const QString& ChartFormat::chartType() const {
        return currentChartType;
    }


    void ChartFormat::setChartWidth(float newWidth) {
        currentChartWidth = newWidth;
        reportFormatUpdated();
    }


    float ChartFormat::chartWidth() const {
        return currentChartWidth;
    }


    void ChartFormat::setChartHeight(float newHeight) {
        currentChartHeight = newHeight;
        reportFormatUpdated();
    }


    float ChartFormat::chartHeight() const {
        return currentChartHeight;
    }


    void ChartFormat::setBorderLineStyle(const ChartLineStyle& newBorderLineStyle) {
        currentBorderLineStyle = newBorderLineStyle;
        reportFormatUpdated();
    }


    const ChartLineStyle& ChartFormat::borderLineStyle() const {
        return currentBorderLineStyle;
    }


    QString ChartFormat::toString() const {
        return QString("%1,%2,%3,%4,%5")
               .arg(typeName())
               .arg(currentChartType)
               .arg(currentChartWidth)
               .arg(currentChartHeight)
               .arg(currentBorderLineStyle.toString());
    }


    QString ChartFormat::toCss() const {
        return QString("width: %1pt; height: %2pt;").arg(currentChartWidth).arg(currentChartHeight);
    }


    void ChartFormat::readXml(
            QSharedPointer<XmlReader>  reader,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            const XmlAttributes&       attributes,
            unsigned                   xmlVersion
        ) {
        aboutToReadXml();
        Format::readXml(reader, formats, programFile, attributes, xmlVersion);
    }


    void ChartFormat::aboutToReadXml() {
        currentBorderLineStyle = ChartLineStyle();
    }


    void ChartFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        attributes.append("chart_type", currentChartType);

        if (currentChartWidth != defaultChartWidth) {
            attributes.append("chart_width", currentChartWidth);
        }

        if (currentChartHeight != defaultChartHeight) {
            attributes.append("chart_height", currentChartHeight);
        }
    }


    void ChartFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier&,
            ProgramFile&,
            unsigned
        ) {
        if (attributes.hasAttribute("chart_type")) {
            currentChartType = attributes.value<QString>("chart_type");
            // TODO: Validate the chart type.
        } else {
            QString tag = reader->qualifiedName().toString();
            reader->raiseError(tr("Tag \"%1\", missing \"chart_type\" attribute").arg(tag));
        }

        if (attributes.hasAttribute("chart_width")) {
            bool ok;
            currentChartWidth = attributes.value<float>("chart_width", &ok);
            if (!ok || currentChartWidth <= 0) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid \"chart_width\" attribute").arg(tag));
            }
        } else {
            currentChartWidth = defaultChartWidth;
        }

        if (attributes.hasAttribute("chart_height")) {
            bool ok;
            currentChartHeight = attributes.value<float>("chart_height", &ok);
            if (!ok || currentChartHeight <= 0) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid \"chart_height\" attribute").arg(tag));
            }
        } else {
            currentChartHeight = defaultChartHeight;
        }
    }


    void ChartFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        ChartFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void ChartFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        ChartFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }


    void ChartFormat::writeChildFormats(
            QSharedPointer<XmlWriter>       writer,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        if (currentBorderLineStyle.isNotDefault()) {
            XmlAttributes attributes;
            currentBorderLineStyle.writeAddImmediateAttributes(attributes, formats, programFile);

            writer->writeStartElement("BorderLineStyle");
            writer->writeAttributes(attributes);
            writer->writeEndElement();
        }
    }


    bool ChartFormat::readChildFormat(
            QSharedPointer<XmlReader>  reader,
            const QString&             tagName,
            const FormatsByIdentifier& formats,
            const XmlAttributes&       attributes,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        bool result;

        if (tagName == QString("BorderLineStyle")) {
            currentBorderLineStyle.readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
            result = true;
        } else {
            result = false;
        }

        return result;
    }


    void ChartFormat::writeChildren(
            QSharedPointer<XmlWriter>       writer,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile,
            const XmlAttributes&            /* inheritedAttributes */
        ) const {
        writeChildFormats(writer, formats, programFile);
    }


    void ChartFormat::readChild(
            QSharedPointer<XmlReader>  reader,
            const QString&             tagName,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            const XmlAttributes&       attributes,
            unsigned                   xmlVersion
        ) {
        readChildFormat(reader, tagName, formats, attributes, programFile, xmlVersion);

        if (!reader->atEnd() && !reader->hasError()) {
            XmlReader::TokenType tokenType;

            do {
                tokenType = reader->readNext();

                if (tokenType == XmlReader::Characters && !reader->isWhitespace()) {
                    reader->raiseError(tr("Unexpected content"));
                } else if (tokenType == XmlReader::StartElement) {
                    reader->raiseError(
                        tr("Unexpected child tag \"%s\" under \"%s\".")
                        .arg(reader->qualifiedName().toString())
                        .arg(tagName)
                    );
                } else if (tokenType != XmlReader::EndElement) {
                    reader->raiseError(tr("Invalid primitive"));
                }
            } while (!reader->hasError() && tokenType != XmlReader::EndElement);
        }
    }
}
