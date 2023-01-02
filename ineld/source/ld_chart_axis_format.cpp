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
* This file implements the \ref Ld::ChartAxisFormat class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>
#include <QHash>
#include <QSet>

#include <limits>
#include <cassert>

#include <util_hash_functions.h>

#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_format.h"
#include "ld_chart_line_style.h"
#include "ld_chart_axis_format.h"

/***********************************************************************************************************************
 * Ld::ChartAxisFormat::Aggregation
 */

namespace Ld {
    ChartAxisFormat::Aggregation::Aggregation() {}


    ChartAxisFormat::Aggregation::~Aggregation() {}


    QString ChartAxisFormat::Aggregation::typeName() const {
        return ChartAxisFormat::formatName;
    }


    void ChartAxisFormat::Aggregation::clear() {
        Format::Aggregation::clear();

        currentAxisScales.clear();
        currentMinimumValues.clear();
        currentMaximumValues.clear();
        currentMajorStepSizes.clear();
        currentMinorStepSizes.clear();
        currentTitleFontFormats.clear();
        currentNumberFontFormats.clear();
        currentMajorTickStyles.clear();
        currentMinorTickStyles.clear();
        currentMajorTickLineStyles.clear();
        currentMinorTickLineStyles.clear();
    }


    bool ChartAxisFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<ChartAxisFormat> chartAxisFormat = formatInstance.dynamicCast<ChartAxisFormat>();
        if (!chartAxisFormat.isNull()) {
            Q_ASSERT(!chartAxisFormat.isNull());

            success = Format::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentAxisScales << chartAxisFormat->currentAxisScale;
                currentMinimumValues << chartAxisFormat->currentMinimumValue;
                currentMaximumValues << chartAxisFormat->currentMaximumValue;
                currentMajorStepSizes << chartAxisFormat->currentMajorStepSize;
                currentMinorStepSizes << chartAxisFormat->currentMinorStepSize;
                currentTitleFontFormats << chartAxisFormat->currentTitleFontFormat;
                currentNumberFontFormats << chartAxisFormat->currentNumberFontFormat;
                currentMajorTickStyles << chartAxisFormat->currentMajorTickStyle;
                currentMinorTickStyles << chartAxisFormat->currentMinorTickStyle;
                currentMajorTickLineStyles << chartAxisFormat->currentMajorTickLineStyle;
                currentMinorTickLineStyles << chartAxisFormat->currentMinorTickLineStyle;
            }
        }

        return success;
    }


    const QSet<ChartAxisFormat::AxisScale>& ChartAxisFormat::Aggregation::axisScales() const {
        return currentAxisScales;
    }


    const QSet<double>& ChartAxisFormat::Aggregation::minimumValues() const {
        return currentMinimumValues;
    }


    const QSet<double>& ChartAxisFormat::Aggregation::maximumValues() const {
        return currentMaximumValues;
    }


    const QSet<double>& ChartAxisFormat::Aggregation::majorStepSizes() const {
        return currentMajorStepSizes;
    }


    const QSet<double>& ChartAxisFormat::Aggregation::minorStepSizes() const {
        return currentMinorStepSizes;
    }


    const QSet<FontFormat>& ChartAxisFormat::Aggregation::titleFontFormats() const {
        return currentTitleFontFormats;
    }


    const QSet<FontFormat>& ChartAxisFormat::Aggregation::numberFontFormats() const {
        return currentNumberFontFormats;
    }


    const QSet<ChartAxisFormat::TickStyle>& ChartAxisFormat::Aggregation::majorTickStyles() const {
        return currentMajorTickStyles;
    }


    const QSet<ChartAxisFormat::TickStyle>& ChartAxisFormat::Aggregation::minorTickStyles() const {
        return currentMinorTickStyles;
    }


    const QSet<ChartLineStyle>& ChartAxisFormat::Aggregation::majorTickLineStyles() const {
        return currentMajorTickLineStyles;
    }


    const QSet<ChartLineStyle>& ChartAxisFormat::Aggregation::minorTickLineStyles() const {
        return currentMinorTickLineStyles;
    }
}

/***********************************************************************************************************************
 * Ld::ChartAxisFormat::Aggregation
 */

namespace Ld {
    const QString                    ChartAxisFormat::formatName("ChartAxisFormat");
    const double                     ChartAxisFormat::automaticScaling = std::numeric_limits<double>::infinity();
    const ChartAxisFormat::AxisScale ChartAxisFormat::defaultAxisScale = ChartAxisFormat::AxisScale::LINEAR;
    const double                     ChartAxisFormat::defaultMinimumValue = ChartAxisFormat::automaticScaling;
    const double                     ChartAxisFormat::defaultMaximumValue = ChartAxisFormat::automaticScaling;
    const double                     ChartAxisFormat::defaultMajorStepSize = ChartAxisFormat::automaticScaling;
    const double                     ChartAxisFormat::defaultMinorStepSize = ChartAxisFormat::automaticScaling;
    const FontFormat                 ChartAxisFormat::defaultTitleFontFormat;
    const FontFormat                 ChartAxisFormat::defaultNumberFontFormat;
    const ChartAxisFormat::TickStyle ChartAxisFormat::defaultMajorTickStyle = ChartAxisFormat::TickStyle::LINE;
    const ChartAxisFormat::TickStyle ChartAxisFormat::defaultMinorTickStyle = ChartAxisFormat::TickStyle::NONE;
    const ChartLineStyle             ChartAxisFormat::defaultMajorTickLineStyle(
        ChartLineStyle::LineStyle::DOTTED_LINE,
        0,
        QColor(Qt::GlobalColor::gray)
    );
    const ChartLineStyle ChartAxisFormat::defaultMinorTickLineStyle(
        ChartLineStyle::LineStyle::NO_LINE,
        0,
        QColor(Qt::GlobalColor::gray)
    );

    ChartAxisFormat::ChartAxisFormat(
            AxisScale             axisScale,
            double                minimumValue,
            double                maximumValue,
            double                majorStepSize,
            double                minorStepSize,
            TickStyle             majorTickStyle,
            TickStyle             minorTickStyle,
            const ChartLineStyle& majorTickLineStyle,
            const ChartLineStyle& minorTickLineStyle,
            const FontFormat& titleFont,
            const FontFormat& numberFont
        ):Format(
        ),currentAxisScale(
            axisScale
        ),currentMinimumValue(
            minimumValue
        ),currentMaximumValue(
            maximumValue
        ),currentMajorStepSize(
            majorStepSize
        ),currentMinorStepSize(
            minorStepSize
        ),currentTitleFontFormat(
            titleFont
        ),currentNumberFontFormat(
            numberFont
        ),currentMajorTickStyle(
            majorTickStyle
        ),currentMinorTickStyle(
            minorTickStyle
        ),currentMajorTickLineStyle(
            majorTickLineStyle
        ),currentMinorTickLineStyle(
            minorTickLineStyle
        ) {}


    ChartAxisFormat::ChartAxisFormat(
            const ChartAxisFormat& other
        ):Format(
            other
        ),currentAxisScale(
            other.currentAxisScale
        ),currentMinimumValue(
            other.currentMinimumValue
        ),currentMaximumValue(
            other.currentMaximumValue
        ),currentMajorStepSize(
            other.currentMajorStepSize
        ),currentMinorStepSize(
            other.currentMinorStepSize
        ),currentTitleFontFormat(
            other.currentTitleFontFormat
        ),currentNumberFontFormat(
            other.currentNumberFontFormat
        ),currentMajorTickStyle(
            other.currentMajorTickStyle
        ),currentMinorTickStyle(
            other.currentMinorTickStyle
        ),currentMajorTickLineStyle(
            other.currentMajorTickLineStyle
        ),currentMinorTickLineStyle(
            other.currentMinorTickLineStyle
        ) {}


    ChartAxisFormat::~ChartAxisFormat() {}


    FormatPointer ChartAxisFormat::clone() const {
        return FormatPointer(new ChartAxisFormat(*this));
    }


    Format* ChartAxisFormat::creator(const QString& /* formatType */) {
        return new ChartAxisFormat;
    }


    QString ChartAxisFormat::typeName() const {
        return formatName;
    }


    bool ChartAxisFormat::isValid() const {
        return (
               currentMajorStepSize > 0
            && currentMinorStepSize > 0
            && currentTitleFontFormat.isValid()
            && currentNumberFontFormat.isValid()
            && currentMajorTickLineStyle.isValid()
            && currentMinorTickLineStyle.isValid()
        );
    }


    bool ChartAxisFormat::isDefault() const {
        return (
               currentAxisScale == defaultAxisScale
            && currentMinimumValue == defaultMinimumValue
            && currentMaximumValue == defaultMaximumValue
            && currentMajorStepSize == defaultMajorStepSize
            && currentMinorStepSize == defaultMinorStepSize
            && currentTitleFontFormat == defaultTitleFontFormat
            && currentNumberFontFormat == defaultNumberFontFormat
            && currentMajorTickStyle == defaultMajorTickStyle
            && currentMinorTickStyle == defaultMinorTickStyle
            && currentMajorTickLineStyle == defaultMajorTickLineStyle
            && currentMinorTickLineStyle == defaultMinorTickLineStyle
        );
    }


    bool ChartAxisFormat::isNotDefault() const {
        return !isDefault();
    }


    void ChartAxisFormat::setAxisScale(ChartAxisFormat::AxisScale newScaling) {
        currentAxisScale = newScaling;
        reportFormatUpdated();
    }


    ChartAxisFormat::AxisScale ChartAxisFormat::axisScale() const {
        return currentAxisScale;
    }


    void ChartAxisFormat::setMinimumValue(double newMinimumValue) {
        currentMinimumValue = newMinimumValue;
        reportFormatUpdated();
    }


    double ChartAxisFormat::minimumValue() const {
        return currentMinimumValue;
    }


    void ChartAxisFormat::setMaximumValue(double newMaximumValue) {
        currentMaximumValue = newMaximumValue;
        reportFormatUpdated();
    }


    double ChartAxisFormat::maximumValue() const {
        return currentMaximumValue;
    }


    void ChartAxisFormat::setMajorStepSize(double newMajorStepSize) {
        currentMajorStepSize = newMajorStepSize;
        reportFormatUpdated();
    }


    double ChartAxisFormat::majorStepSize() const {
        return currentMajorStepSize;
    }


    void ChartAxisFormat::setMinorStepSize(double newMinorStepSize) {
        currentMinorStepSize = newMinorStepSize;
        reportFormatUpdated();
    }


    double ChartAxisFormat::minorStepSize() const {
        return currentMinorStepSize;
    }


    void ChartAxisFormat::setMajorTickStyle(ChartAxisFormat::TickStyle newTickStyle) {
        currentMajorTickStyle = newTickStyle;
        reportFormatUpdated();
    }


    ChartAxisFormat::TickStyle ChartAxisFormat::majorTickStyle() const {
        return currentMajorTickStyle;
    }


    void ChartAxisFormat::setMinorTickStyle(ChartAxisFormat::TickStyle newTickStyle) {
        currentMinorTickStyle = newTickStyle;
        reportFormatUpdated();
    }


    ChartAxisFormat::TickStyle ChartAxisFormat::minorTickStyle() const {
        return currentMinorTickStyle;
    }


    void ChartAxisFormat::setMajorTickLineStyle(const ChartLineStyle& newTickLineStyle) {
        currentMajorTickLineStyle = newTickLineStyle;
        reportFormatUpdated();
    }


    const ChartLineStyle& ChartAxisFormat::majorTickLineStyle() const {
        return currentMajorTickLineStyle;
    }


    void ChartAxisFormat::setMinorTickLineStyle(const ChartLineStyle& newTickLineStyle) {
        currentMinorTickLineStyle = newTickLineStyle;
        reportFormatUpdated();
    }


    const ChartLineStyle& ChartAxisFormat::minorTickLineStyle() const {
        return currentMinorTickLineStyle;
    }


    void ChartAxisFormat::setTitleFont(const FontFormat& newTitleFont) {
        currentTitleFontFormat = newTitleFont;
        reportFormatUpdated();
    }


    const FontFormat& ChartAxisFormat::titleFont() const {
        return currentTitleFontFormat;
    }


    void ChartAxisFormat::setNumberFont(const FontFormat& newNumberFont) {
        currentNumberFontFormat = newNumberFont;
        reportFormatUpdated();
    }


    const FontFormat& ChartAxisFormat::numberFont() const {
        return currentNumberFontFormat;
    }


    QString ChartAxisFormat::toString() const {
        QString titleFontString = currentTitleFontFormat.toString();
        titleFontString = titleFontString.mid(titleFontString.indexOf(QChar(',')) + 1);

        QString numberFontString = currentNumberFontFormat.toString();
        numberFontString = numberFontString.mid(numberFontString.indexOf(QChar(',')) + 1);

        QString result = typeName();

        result += QString(",%1,%2,%3,%4,%5,%6,%7,%8,%9")
                  .arg(toString(currentAxisScale))
                  .arg(currentMinimumValue)
                  .arg(currentMaximumValue)
                  .arg(currentMajorStepSize)
                  .arg(currentMinorStepSize)
                  .arg(titleFontString)
                  .arg(numberFontString)
                  .arg(toString(currentMajorTickStyle))
                  .arg(toString(currentMinorTickStyle));

        result += QString(",%1,%2")
                  .arg(currentMajorTickLineStyle.toString())
                  .arg(currentMinorTickLineStyle.toString());

        return result;
    }


    QString ChartAxisFormat::toCss() const {
        return QString();
    }


    ChartAxisFormat& ChartAxisFormat::operator=(const ChartAxisFormat& other) {
        Format::operator=(other);

        currentAxisScale = other.currentAxisScale;
        currentMinimumValue = other.currentMinimumValue;
        currentMaximumValue = other.currentMaximumValue;
        currentMajorStepSize = other.currentMajorStepSize;
        currentMinorStepSize = other.currentMinorStepSize;
        currentTitleFontFormat = other.currentTitleFontFormat;
        currentNumberFontFormat = other.currentNumberFontFormat;
        currentMajorTickStyle = other.currentMajorTickStyle;
        currentMinorTickStyle = other.currentMinorTickStyle;
        currentMajorTickLineStyle = other.currentMajorTickLineStyle;
        currentMinorTickLineStyle = other.currentMinorTickLineStyle;

        return *this;
    }


    bool ChartAxisFormat::operator==(const ChartAxisFormat& other) const {
        return (
               currentAxisScale == other.currentAxisScale
            && currentMinimumValue == other.currentMinimumValue
            && currentMaximumValue == other.currentMaximumValue
            && currentMajorStepSize == other.currentMajorStepSize
            && currentMinorStepSize == other.currentMinorStepSize
            && currentTitleFontFormat == other.currentTitleFontFormat
            && currentNumberFontFormat == other.currentNumberFontFormat
            && currentMajorTickStyle == other.currentMajorTickStyle
            && currentMinorTickStyle == other.currentMinorTickStyle
            && currentMajorTickLineStyle == other.currentMajorTickLineStyle
            && currentMinorTickLineStyle == other.currentMinorTickLineStyle
        );
    }


    bool ChartAxisFormat::operator!=(const ChartAxisFormat& other) const {
        return (
               currentAxisScale != other.currentAxisScale
            || currentMinimumValue != other.currentMinimumValue
            || currentMaximumValue != other.currentMaximumValue
            || currentMajorStepSize != other.currentMajorStepSize
            || currentMinorStepSize != other.currentMinorStepSize
            || currentTitleFontFormat != other.currentTitleFontFormat
            || currentNumberFontFormat != other.currentNumberFontFormat
            || currentMajorTickStyle != other.currentMajorTickStyle
            || currentMinorTickStyle != other.currentMinorTickStyle
            || currentMajorTickLineStyle != other.currentMajorTickLineStyle
            || currentMinorTickLineStyle != other.currentMinorTickLineStyle
        );
    }


    void ChartAxisFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void ChartAxisFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        readImmediateAttributes(reader, attributes, formats,programFile, xmlVersion);
    }


    void ChartAxisFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (currentAxisScale != defaultAxisScale) {
            attributes.append("axis_scale", toString(currentAxisScale));
        }

        if (currentMinimumValue != defaultMinimumValue) {
            attributes.append("minimum", currentMinimumValue);
        }

        if (currentMaximumValue != defaultMaximumValue) {
            attributes.append("maximum", currentMaximumValue);
        }

        if (currentMajorStepSize != defaultMajorStepSize) {
            attributes.append("major_step_size", currentMajorStepSize);
        }

        if (currentMinorStepSize != defaultMinorStepSize) {
            attributes.append("minor_step_size", currentMinorStepSize);
        }

        if (currentMajorTickStyle != defaultMajorTickStyle) {
            attributes.append("major_tick_style", toString(currentMajorTickStyle));
        }

        if (currentMinorTickStyle != defaultMinorTickStyle) {
            attributes.append("minor_tick_style", toString(currentMinorTickStyle));
        }
    }


    void ChartAxisFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        bool ok;

        if (attributes.hasAttribute("axis_scale")) {
            QString axisScaleString = attributes.value<QString>("axis_scale");

            currentAxisScale = toAxisScale(axisScaleString, &ok);
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid \"axis_scale\" attribute").arg(tag));
            }
        } else {
            currentAxisScale = defaultAxisScale;
        }

        if (!reader->hasError()) {
            currentMinimumValue = attributes.value<double>("minimum", defaultMinimumValue, &ok);
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid \"minimum\" attribute").arg(tag));
            }
        }

        if (!reader->hasError()) {
            currentMaximumValue = attributes.value<double>("maximum", defaultMaximumValue, &ok);
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid \"maximum\" attribute").arg(tag));
            }
        }

        if (!reader->hasError()) {
            currentMajorStepSize = attributes.value<double>("major_step_size", defaultMajorStepSize, &ok);
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid \"major_step_size\" attribute").arg(tag));
            }
        }

        if (!reader->hasError()) {
            currentMinorStepSize = attributes.value<double>("minor_step_size", defaultMinorStepSize, &ok);
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid \"minor_step_size\" attribute").arg(tag));
            }
        }

        if (!reader->hasError() && attributes.hasAttribute("major_tick_style")) {
            QString majorTickStyleString = attributes.value<QString>("major_tick_style");

            currentMajorTickStyle = toTickStyle(majorTickStyleString, &ok);
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid \"major_tick_style\" attribute").arg(tag));
            }
        } else {
            currentMajorTickStyle = defaultMajorTickStyle;
        }

        if (!reader->hasError() && attributes.hasAttribute("minor_tick_style")) {
            QString minorTickStyleString = attributes.value<QString>("minor_tick_style");

            currentMinorTickStyle = toTickStyle(minorTickStyleString, &ok);
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid \"minor_tick_style\" attribute").arg(tag));
            }
        } else {
            currentMinorTickStyle = defaultMinorTickStyle;
        }
    }


    QString ChartAxisFormat::toString(ChartAxisFormat::AxisScale mode) {
        QString result;

        switch (mode) {
            case AxisScale::LINEAR:    { result = QString("LINEAR");     break; }
            case AxisScale::LOG:       { result = QString("LOG");        break; }
            case AxisScale::DB_10_LOG: { result = QString("DB_10_LOG");  break; }
            case AxisScale::DB_20_LOG: { result = QString("DB_20_LOG");  break; }
            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    ChartAxisFormat::AxisScale ChartAxisFormat::toAxisScale(const QString& str, bool*ok) {
        bool      isOk;
        AxisScale result;

        QString lc = str.trimmed().toLower();

        if (lc == QString("linear")) {
            result = AxisScale::LINEAR;
            isOk   = true;
        } else if (lc == QString("log")) {
            result = AxisScale::LOG;
            isOk   = true;
        } else if (lc == QString("db_10_log")) {
            result = AxisScale::DB_10_LOG;
            isOk   = true;
        } else if (lc == QString("db_20_log")) {
            result = AxisScale::DB_20_LOG;
            isOk   = true;
        } else {
            result = AxisScale::LINEAR;
            isOk   = false;
        }

        if (ok != nullptr) {
            *ok = isOk;
        }

        return result;
    }


    QString ChartAxisFormat::toString(ChartAxisFormat::TickStyle mode) {
        QString result;

        switch (mode) {
            case TickStyle::NONE:          { result = QString("NONE");           break; }
            case TickStyle::OUTER:         { result = QString("OUTER");          break; }
            case TickStyle::INNER:         { result = QString("INNER");          break; }
            case TickStyle::SPANNING:      { result = QString("SPANNING");       break; }
            case TickStyle::LINE:          { result = QString("LINE");           break; }
            case TickStyle::LINE_AND_TICK: { result = QString("LINE_AND_TICK");  break; }
            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    ChartAxisFormat::TickStyle ChartAxisFormat::toTickStyle(const QString& str, bool*ok) {
        bool      isOk;
        TickStyle result;

        QString lc = str.trimmed().toLower();

        if (lc == QString("none")) {
            result = TickStyle::NONE;
            isOk   = true;
        } else if (lc == QString("outer")) {
            result = TickStyle::OUTER;
            isOk   = true;
        } else if (lc == QString("inner")) {
            result = TickStyle::INNER;
            isOk   = true;
        } else if (lc == QString("spanning")) {
            result = TickStyle::SPANNING;
            isOk   = true;
        } else if (lc == QString("line")) {
            result = TickStyle::LINE;
            isOk   = true;
        } else if (lc == QString("line_and_tick")) {
            result = TickStyle::LINE_AND_TICK;
            isOk   = true;
        } else {
            result = TickStyle::NONE;
            isOk   = false;
        }

        if (ok != nullptr) {
            *ok = isOk;
        }

        return result;
    }


    Util::HashResult qHash(ChartAxisFormat::AxisScale value, Util::HashSeed seed) {
        return ::qHash(static_cast<int>(value), seed);
    }


    Util::HashResult qHash(ChartAxisFormat::TickStyle value, Util::HashSeed seed) {
        return ::qHash(static_cast<int>(value), seed);
    }


    Util::HashResult qHash(const ChartAxisFormat& value, Util::HashSeed seed) {
        return (
              qHash(value.currentAxisScale, seed)
            ^ ::qHash(value.currentMinimumValue, seed)
            ^ ::qHash(value.currentMaximumValue, seed)
            ^ ::qHash(value.currentMajorStepSize, seed)
            ^ ::qHash(value.currentMinorStepSize, seed)
            ^ qHash(value.currentTitleFontFormat, seed)
            ^ qHash(value.currentNumberFontFormat, seed)
            ^ qHash(value.currentMajorTickStyle, seed)
            ^ qHash(value.currentMinorTickStyle, seed)
            ^ qHash(value.currentMajorTickLineStyle, seed)
            ^ qHash(value.currentMinorTickLineStyle, seed)
        );
    }
}
