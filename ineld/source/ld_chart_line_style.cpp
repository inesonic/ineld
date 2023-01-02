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
* This file implements the \ref Ld::ChartLineStyle class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>
#include <QHash>
#include <QSet>

#include <cassert>

#include <util_hash_functions.h>

#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_format.h"
#include "ld_chart_line_style.h"

/***********************************************************************************************************************
 * Ld::ChartLineStyle::Aggregation
 */

namespace Ld {
    ChartLineStyle::Aggregation::Aggregation() {}


    ChartLineStyle::Aggregation::~Aggregation() {}


    QString ChartLineStyle::Aggregation::typeName() const {
        return ChartLineStyle::formatName;
    }


    void ChartLineStyle::Aggregation::clear() {
        Format::Aggregation::clear();

        currentLineStyles.clear();
        currentLineWidths.clear();
        currentLineColors.clear();
    }


    bool ChartLineStyle::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<ChartLineStyle> chartLineStyle = formatInstance.dynamicCast<ChartLineStyle>();
        if (!chartLineStyle.isNull()) {
            Q_ASSERT(!chartLineStyle.isNull());

            success = Format::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentLineStyles << chartLineStyle->lineStyle();
                currentLineWidths << chartLineStyle->lineWidth();
                currentLineColors << chartLineStyle->lineColor();
            }
        }

        return success;
    }


    const QSet<ChartLineStyle::LineStyle>& ChartLineStyle::Aggregation::lineStyles() const {
        return currentLineStyles;
    }


    const QSet<float>& ChartLineStyle::Aggregation::lineWidths() const {
        return currentLineWidths;
    }


    const QSet<QColor>& ChartLineStyle::Aggregation::lineColors() const {
        return currentLineColors;
    }
}

/***********************************************************************************************************************
 * Ld::ChartLineStyle::Aggregation
 */

namespace Ld {
    const QString                   ChartLineStyle::formatName("ChartLineStyle");
    const ChartLineStyle::LineStyle ChartLineStyle::defaultLineStyle = ChartLineStyle::LineStyle::SOLID_LINE;
    const float                     ChartLineStyle::defaultLineWidth = 1;
    const QColor                    ChartLineStyle::defaultLineColor = QColor(Qt::black);

    ChartLineStyle::ChartLineStyle(
            ChartLineStyle::LineStyle lineStyle,
            float                     lineWidth,
            const QColor&             lineColor
        ):currentLineStyle(
            lineStyle
        ),currentLineWidth(
            lineWidth
        ),currentLineColor(
            lineColor
        ) {}


    ChartLineStyle::ChartLineStyle(
            const ChartLineStyle& other
        ):Format(
            other
        ),currentLineStyle(
            other.currentLineStyle
        ),currentLineWidth(
            other.currentLineWidth
        ),currentLineColor(
            other.currentLineColor
        ) {}


    ChartLineStyle::~ChartLineStyle() {}


    FormatPointer ChartLineStyle::clone() const {
        return FormatPointer(new ChartLineStyle(*this));
    }


    Format* ChartLineStyle::creator(const QString& /* formatType */) {
        return new ChartLineStyle;
    }


    QString ChartLineStyle::typeName() const {
        return formatName;
    }


    bool ChartLineStyle::isValid() const {
        return true;
    }


    bool ChartLineStyle::isDefault() const {
        return (
               currentLineStyle == defaultLineStyle
            && currentLineWidth == defaultLineWidth
            && currentLineColor == defaultLineColor
        );
    }


    bool ChartLineStyle::isNotDefault() const {
        return !isDefault();
    }


    void ChartLineStyle::setLineStyle(ChartLineStyle::LineStyle newLineStyle) {
        currentLineStyle = newLineStyle;
        reportFormatUpdated();
    }


    ChartLineStyle::LineStyle ChartLineStyle::lineStyle() const {
        return currentLineStyle;
    }


    void ChartLineStyle::setLineWidth(float newLineWidth) {
        currentLineWidth = newLineWidth;
        reportFormatUpdated();
    }


    float ChartLineStyle::lineWidth() const {
        return currentLineWidth;
    }


    void ChartLineStyle::setLineColor(const QColor& newLineColor) {
        currentLineColor = newLineColor;
        reportFormatUpdated();
    }


    const QColor& ChartLineStyle::lineColor() const {
        return currentLineColor;
    }


    QString ChartLineStyle::toString() const {
        return QString("%1,%2,%3")
               .arg(toString(currentLineStyle))
               .arg(currentLineWidth)
               .arg(currentLineColor.name(QColor::NameFormat::HexArgb));
    }


    QString ChartLineStyle::toCss() const {
        return QString();
    }


    ChartLineStyle& ChartLineStyle::operator=(const ChartLineStyle& other) {
        currentLineStyle = other.currentLineStyle;
        currentLineWidth = other.currentLineWidth;
        currentLineColor = other.currentLineColor;

        return *this;
    }


    bool ChartLineStyle::operator==(const ChartLineStyle& other) const {
        return (
               currentLineStyle == other.currentLineStyle
            && currentLineWidth == other.currentLineWidth
            && currentLineColor == other.currentLineColor
        );
    }


    bool ChartLineStyle::operator!=(const ChartLineStyle& other) const {
        return (
               currentLineStyle != other.currentLineStyle
            || currentLineWidth != other.currentLineWidth
            || currentLineColor != other.currentLineColor
        );
    }


    void ChartLineStyle::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void ChartLineStyle::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }



    void ChartLineStyle::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (currentLineStyle != defaultLineStyle) {
            attributes.append("line_style", toString(currentLineStyle));
        }

        if (currentLineWidth != defaultLineWidth) {
            attributes.append("line_width", currentLineWidth);
        }

        if (currentLineColor != defaultLineColor) {
            attributes.append("line_color", currentLineColor);
        }
    }


    void ChartLineStyle::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        if (attributes.hasAttribute("line_style")) {
            QString lineStyleString = attributes.value<QString>("line_style");
            bool    ok;

            currentLineStyle = toLineStyle(lineStyleString, &ok);
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid \"line_style\" attribute").arg(tag));
            }
        } else {
            currentLineStyle = defaultLineStyle;
        }

        if (!reader->hasError() && attributes.hasAttribute("line_width")) {
            bool ok;
            currentLineWidth = attributes.value<float>("line_width", &ok);

            if (!ok || currentLineWidth < 0) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid \"line_width\" attribute").arg(tag));
            }
        } else {
            currentLineWidth = defaultLineWidth;
        }

        if (!reader->hasError() && attributes.hasAttribute("line_color")) {
            bool ok;
            currentLineColor = attributes.colorValue("line_color", &ok);
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid \"line_color\" attribute").arg(tag));
            }
        } else {
            currentLineColor = defaultLineColor;
        }
    }


    QString ChartLineStyle::toString(ChartLineStyle::LineStyle mode) {
        QString result;

        switch (mode) {
            case LineStyle::NO_LINE:                 { result = QString("NO_LINE");                    break; }
            case LineStyle::SOLID_LINE:              { result = QString("SOLID_LINE");                 break; }
            case LineStyle::DASHED_LINE:             { result = QString("DASHED_LINE");                break; }
            case LineStyle::DOTTED_LINE:             { result = QString("DOTTED_LINE");                break; }
            case LineStyle::DASH_DOTTED_LINE:        { result = QString("DASH_DOTTED_LINE");           break; }
            case LineStyle::DASH_DOTTED_DOTTED_LINE: { result = QString("DASH_DOTTED_DOTTED_LINE");    break; }
            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    ChartLineStyle::LineStyle ChartLineStyle::toLineStyle(const QString& str, bool*ok) {
        bool      isOk;
        LineStyle result;

        QString lc = str.trimmed().toLower();

        if (lc == QString("no_line")) {
            result = LineStyle::NO_LINE;
            isOk   = true;
        } else if (lc == QString("solid_line")) {
            result = LineStyle::SOLID_LINE;
            isOk   = true;
        } else if (lc == QString("dashed_line")) {
            result = LineStyle::DASHED_LINE;
            isOk   = true;
        } else if (lc == QString("dotted_line")) {
            result = LineStyle::DOTTED_LINE;
            isOk   = true;
        } else if (lc == QString("dash_dotted_line")) {
            result = LineStyle::DASH_DOTTED_LINE;
            isOk   = true;
        } else if (lc == QString("dash_dotted_dotted_line")) {
            result = LineStyle::DASH_DOTTED_DOTTED_LINE;
            isOk   = true;
        } else {
            result = LineStyle::NO_LINE;
            isOk   = false;
        }


        if (ok != nullptr) {
            *ok = isOk;
        }

        return result;
    }


    Util::HashResult qHash(ChartLineStyle::LineStyle value, Util::HashSeed seed) {
        return ::qHash(static_cast<int>(value), seed);
    }


    Util::HashResult qHash(const ChartLineStyle& value, Util::HashSeed seed) {
        return (
              qHash(value.lineStyle(), seed)
            ^ ::qHash(value.lineWidth(), seed)
            ^ ::qHash(value.lineColor(), seed)
        );
    }
}
