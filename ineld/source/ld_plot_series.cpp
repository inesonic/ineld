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
* This file implements the \ref Ld::PlotSeries class.
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
#include "ld_plot_series.h"

/***********************************************************************************************************************
 * Ld::PlotSeries::Aggregation
 */

namespace Ld {
    PlotSeries::Aggregation::Aggregation() {}


    PlotSeries::Aggregation::~Aggregation() {}


    QString PlotSeries::Aggregation::typeName() const {
        return PlotSeries::formatName;
    }


    void PlotSeries::Aggregation::clear() {
        ChartLineStyle::Aggregation::clear();

        currentMarkerStyles.clear();
        currentSplineTypes.clear();
    }


    bool PlotSeries::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<PlotSeries> plotSeries = formatInstance.dynamicCast<PlotSeries>();
        if (!plotSeries.isNull()) {
            Q_ASSERT(!plotSeries.isNull());

            success = ChartLineStyle::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentMarkerStyles << plotSeries->currentMarkerStyle;
                currentSplineTypes << plotSeries->currentSplineType;
                currentGradientTypes << plotSeries->currentGradientType;
            }
        }

        return success;
    }


    const QSet<PlotSeries::MarkerStyle>& PlotSeries::Aggregation::markerStyles() const {
        return currentMarkerStyles;
    }


    const QSet<PlotSeries::SplineType>& PlotSeries::Aggregation::splineTypes() const {
        return currentSplineTypes;
    }


    const QSet<PlotSeries::GradientType>& PlotSeries::Aggregation::gradientTypes() const {
        return currentGradientTypes;
    }
}

/***********************************************************************************************************************
 * Ld::PlotSeries::Aggregation
 */

namespace Ld {
    const QString                  PlotSeries::formatName("PlotSeries");
    const PlotSeries::MarkerStyle  PlotSeries::defaultMarkerStyle = PlotSeries::MarkerStyle::NONE;
    const PlotSeries::SplineType   PlotSeries::defaultSplineType = PlotSeries::SplineType::LINE;
    const PlotSeries::GradientType PlotSeries::defaultGradientType = PlotSeries::GradientType::NONE;

    const PlotSeries PlotSeries::defaultPlotSeries[] = {
        PlotSeries(
            PlotSeries::LineStyle::SOLID_LINE,
            1,
            QColor(Qt::GlobalColor::blue),
            PlotSeries::MarkerStyle::DOT
        ),
        PlotSeries(
            PlotSeries::LineStyle::SOLID_LINE,
            1,
            QColor(Qt::GlobalColor::red),
            PlotSeries::MarkerStyle::SQUARE
        ),
        PlotSeries(
            PlotSeries::LineStyle::SOLID_LINE,
            1,
            QColor(Qt::GlobalColor::darkYellow),
            PlotSeries::MarkerStyle::CIRCLE
        ),
        PlotSeries(
            PlotSeries::LineStyle::SOLID_LINE,
            1,
            QColor(Qt::GlobalColor::black),
            PlotSeries::MarkerStyle::DIAMOND
        ),
        PlotSeries(
            PlotSeries::LineStyle::SOLID_LINE,
            1,
            QColor(Qt::GlobalColor::darkBlue),
            PlotSeries::MarkerStyle::DOT
        ),
        PlotSeries(
            PlotSeries::LineStyle::SOLID_LINE,
            1,
            QColor(Qt::GlobalColor::darkRed),
            PlotSeries::MarkerStyle::SQUARE
        ),
        PlotSeries(
            PlotSeries::LineStyle::SOLID_LINE,
            1,
            QColor(Qt::GlobalColor::darkGray),
            PlotSeries::MarkerStyle::DIAMOND
        ),
        PlotSeries(
            PlotSeries::LineStyle::SOLID_LINE,
            1,
            QColor(Qt::GlobalColor::cyan),
            PlotSeries::MarkerStyle::DOT
        ),
        PlotSeries(
            PlotSeries::LineStyle::SOLID_LINE,
            1,
            QColor(Qt::GlobalColor::magenta),
            PlotSeries::MarkerStyle::SQUARE
        )
    };

    const unsigned PlotSeries::numberDefaultPlotSeries = sizeof(PlotSeries::defaultPlotSeries) / sizeof(Ld::PlotSeries);

    PlotSeries::PlotSeries(
            PlotSeries::LineStyle    lineStyle,
            float                    lineWidth,
            const QColor&            lineColor,
            PlotSeries::MarkerStyle  markerStyle,
            PlotSeries::SplineType   splineType,
            PlotSeries::GradientType gradientType
        ):ChartLineStyle(
            lineStyle,
            lineWidth,
            lineColor
        ),currentMarkerStyle(
            markerStyle
        ),currentSplineType(
            splineType
        ),currentGradientType(
            gradientType
        ) {}


    PlotSeries::PlotSeries(
            const PlotSeries& other
        ):Format(
            other
        ),ChartLineStyle(
            other
        ),currentMarkerStyle(
            other.currentMarkerStyle
        ),currentSplineType(
            other.currentSplineType
        ),currentGradientType(
            other.currentGradientType
        ) {}


    PlotSeries::~PlotSeries() {}


    FormatPointer PlotSeries::clone() const {
        return FormatPointer(new PlotSeries(*this));
    }


    Format* PlotSeries::creator(const QString& /* formatType */) {
        return new PlotSeries;
    }


    QString PlotSeries::typeName() const {
        return formatName;
    }


    bool PlotSeries::isValid() const {
        return true;
    }


    bool PlotSeries::isDefault() const {
        return (
               ChartLineStyle::isDefault()
            && currentMarkerStyle == defaultMarkerStyle
            && currentSplineType == defaultSplineType
            && currentGradientType == defaultGradientType
        );
    }


    bool PlotSeries::isNotDefault() const {
        return !isDefault();
    }


    void PlotSeries::setMarkerStyle(PlotSeries::MarkerStyle newMarkerStyle) {
        currentMarkerStyle = newMarkerStyle;
        reportFormatUpdated();
    }


    PlotSeries::MarkerStyle PlotSeries::markerStyle() const {
        return currentMarkerStyle;
    }


    void PlotSeries::setSplineType(PlotSeries::SplineType newSplineType) {
        currentSplineType = newSplineType;
        reportFormatUpdated();
    }


    PlotSeries::SplineType PlotSeries::splineType() const {
        return currentSplineType;
    }


    void PlotSeries::setGradientType(GradientType newGradientType) {
        currentGradientType = newGradientType;
        reportFormatUpdated();
    }


    PlotSeries::GradientType PlotSeries::gradientType() const {
        return currentGradientType;
    }


    QString PlotSeries::toString() const {
        return QString("%1,%2,%3,%4")
               .arg(ChartLineStyle::toString())
               .arg(toString(currentMarkerStyle))
               .arg(toString(currentSplineType))
               .arg(toString(currentGradientType));
    }


    QString PlotSeries::toCss() const {
        return QString();
    }


    PlotSeries& PlotSeries::operator=(const PlotSeries& other) {
        ChartLineStyle::operator=(other);

        currentMarkerStyle  = other.currentMarkerStyle;
        currentSplineType   = other.currentSplineType;
        currentGradientType = other.currentGradientType;

        return *this;
    }


    bool PlotSeries::operator==(const PlotSeries& other) const {
        return (
               ChartLineStyle::operator==(other)
            && currentMarkerStyle == other.currentMarkerStyle
            && currentSplineType == other.currentSplineType
            && currentGradientType == other.currentGradientType
        );
    }


    bool PlotSeries::operator!=(const PlotSeries& other) const {
        return (
               ChartLineStyle::operator!=(other)
            || currentMarkerStyle != other.currentMarkerStyle
            || currentSplineType != other.currentSplineType
            || currentGradientType != other.currentGradientType
        );
    }


    void PlotSeries::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        ChartLineStyle::writeAddImmediateAttributes(attributes, formats, programFile);
        writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void PlotSeries::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        ChartLineStyle::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }


    void PlotSeries::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (currentMarkerStyle != defaultMarkerStyle) {
            attributes.append("marker_style", toString(currentMarkerStyle));
        }

        if (currentSplineType != defaultSplineType) {
            attributes.append("spline_type", toString(currentSplineType));
        }

        if (currentGradientType != defaultGradientType) {
            attributes.append("gradient_type", toString(currentGradientType));
        }
    }


    void PlotSeries::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        bool ok;

        if (attributes.hasAttribute("marker_style")) {
            QString markerStyleString = attributes.value<QString>("marker_style");

            currentMarkerStyle = toMarkerStyle(markerStyleString, &ok);
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid \"marker_style\" attribute").arg(tag));
            }
        } else {
            currentMarkerStyle = defaultMarkerStyle;
        }

        if (!reader->hasError() && attributes.hasAttribute("spline_type")) {
            QString splineTypeString = attributes.value<QString>("spline_type");

            currentSplineType = toSplineType(splineTypeString, &ok);
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid \"spline_type\" attribute").arg(tag));
            }
        } else {
            currentSplineType = defaultSplineType;
        }

        if (!reader->hasError() && attributes.hasAttribute("gradient_type")) {
            QString gradientTypeString = attributes.value<QString>("gradient_type");

            currentGradientType = toGradientType(gradientTypeString, &ok);
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid \"gradient_type\" attribute").arg(tag));
            }
        } else {
            currentGradientType = defaultGradientType;
        }
    }


    QString PlotSeries::toString(PlotSeries::MarkerStyle mode) {
        QString result;

        switch (mode) {
            case MarkerStyle::NONE:    { result = QString("NONE");     break; }
            case MarkerStyle::DOT:     { result = QString("DOT");      break; }
            case MarkerStyle::CIRCLE:  { result = QString("CIRCLE");   break; }
            case MarkerStyle::SQUARE:  { result = QString("SQUARE");   break; }
            case MarkerStyle::DIAMOND: { result = QString("DIAMOND");  break; }
            case MarkerStyle::STAR:    { result = QString("STAR");     break; }
            case MarkerStyle::X:       { result = QString("X");        break; }
            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    PlotSeries::MarkerStyle PlotSeries::toMarkerStyle(const QString& str, bool*ok) {
        bool        isOk;
        MarkerStyle result;

        QString lc = str.trimmed().toLower();

        if (lc == QString("none")) {
            result = MarkerStyle::NONE;
            isOk   = true;
        } else if (lc == QString("dot")) {
            result = MarkerStyle::DOT;
            isOk   = true;
        } else if (lc == QString("circle")) {
            result = MarkerStyle::CIRCLE;
            isOk   = true;
        } else if (lc == QString("square")) {
            result = MarkerStyle::SQUARE;
            isOk   = true;
        } else if (lc == QString("diamond")) {
            result = MarkerStyle::DIAMOND;
            isOk   = true;
        } else if (lc == QString("star")) {
            result = MarkerStyle::STAR;
            isOk   = true;
        } else if (lc == QString("x")) {
            result = MarkerStyle::X;
            isOk   = true;
        } else {
            result = MarkerStyle::NONE;
            isOk   = false;
        }

        if (ok != nullptr) {
            *ok = isOk;
        }

        return result;
    }


    QString PlotSeries::toString(PlotSeries::SplineType mode) {
        QString result;

        switch (mode) {
            case SplineType::NONE:              { result = QString("NONE");                break; }
            case SplineType::LINE:              { result = QString("LINE");                break; }
            case SplineType::SPLINE:            { result = QString("SPLINE");              break; }
            case SplineType::LINEAR_REGRESSION: { result = QString("LINEAR_REGRESSION");   break; }
            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    PlotSeries::SplineType PlotSeries::toSplineType(const QString& str, bool*ok) {
        bool        isOk;
        SplineType result;

        QString lc = str.trimmed().toLower();

        if (lc == QString("none")) {
            result = SplineType::NONE;
            isOk   = true;
        } else if (lc == QString("line")) {
            result = SplineType::LINE;
            isOk   = true;
        } else if (lc == QString("spline")) {
            result = SplineType::SPLINE;
            isOk   = true;
        } else if (lc == QString("linear_regression")) {
            result = SplineType::LINEAR_REGRESSION;
            isOk   = true;
        } else {
            result = SplineType::NONE;
            isOk   = false;
        }

        if (ok != nullptr) {
            *ok = isOk;
        }

        return result;
    }


    QString PlotSeries::toString(PlotSeries::GradientType mode) {
        QString result;

        switch (mode) {
            case GradientType::NONE:             { result = QString("NONE");                           break; }
            case GradientType::INTENSITY:        { result = QString("INTENSITY");                      break; }
            case GradientType::HUE_POSITIVE_60:  { result = QString("HUE_POSITIVE_60");                break; }
            case GradientType::HUE_POSITIVE_120: { result = QString("HUE_POSITIVE_120");               break; }
            case GradientType::HUE_POSITIVE_180: { result = QString("HUE_POSITIVE_180");               break; }
            case GradientType::HUE_POSITIVE_240: { result = QString("HUE_POSITIVE_240");               break; }
            case GradientType::HUE_POSITIVE_300: { result = QString("HUE_POSITIVE_300");               break; }
            case GradientType::HUE_NEGATIVE_60:  { result = QString("HUE_NEGATIVE_60");                break; }
            case GradientType::HUE_NEGATIVE_120: { result = QString("HUE_NEGATIVE_120");               break; }
            case GradientType::HUE_NEGATIVE_180: { result = QString("HUE_NEGATIVE_180");               break; }
            case GradientType::HUE_NEGATIVE_240: { result = QString("HUE_NEGATIVE_240");               break; }
            case GradientType::HUE_NEGATIVE_300: { result = QString("HUE_NEGATIVE_300");               break; }
            default:                             { result = QString::number(static_cast<int>(mode));   break; }
        }

        return result;
    }


    PlotSeries::GradientType PlotSeries::toGradientType(const QString& str, bool* ok) {
        GradientType result;
        bool         isOk;

        QString      s = str.toLower();
        if (s == QString("none")) {
            result = GradientType::NONE;
            isOk   = true;
        } else if (s == QString("intensity")) {
            result = GradientType::INTENSITY;
            isOk   = true;
        } else if (s == QString("hue_positive_60")) {
            result = GradientType::HUE_POSITIVE_60;
            isOk   = true;
        } else if (s == QString("hue_positive_120")) {
            result = GradientType::HUE_POSITIVE_120;
            isOk   = true;
        } else if (s == QString("hue_positive_180")) {
            result = GradientType::HUE_POSITIVE_180;
            isOk   = true;
        } else if (s == QString("hue_positive_240")) {
            result = GradientType::HUE_POSITIVE_240;
            isOk   = true;
        } else if (s == QString("hue_positive_300")) {
            result = GradientType::HUE_POSITIVE_300;
            isOk   = true;
        } else if (s == QString("hue_negative_60")) {
            result = GradientType::HUE_NEGATIVE_60;
            isOk   = true;
        } else if (s == QString("hue_negative_120")) {
            result = GradientType::HUE_NEGATIVE_120;
            isOk   = true;
        } else if (s == QString("hue_negative_180")) {
            result = GradientType::HUE_NEGATIVE_180;
            isOk   = true;
        } else if (s == QString("hue_negative_240")) {
            result = GradientType::HUE_NEGATIVE_240;
            isOk   = true;
        } else if (s == QString("hue_negative_300")) {
            result = GradientType::HUE_NEGATIVE_300;
            isOk   = true;
        } else {
            result = static_cast<GradientType>(str.toInt(&isOk));
        }

        if (ok != nullptr) {
            *ok = isOk;
        }

        return result;
    }


    Util::HashResult qHash(PlotSeries::MarkerStyle value, Util::HashSeed seed) {
        return ::qHash(static_cast<int>(value), seed);
    }


    Util::HashResult qHash(PlotSeries::SplineType value, Util::HashSeed seed) {
        return ::qHash(static_cast<int>(value), seed);
    }


    Util::HashResult qHash(PlotSeries::GradientType value, Util::HashSeed seed) {
        return ::qHash(static_cast<int>(value), seed);
    }


    Util::HashResult qHash(const PlotSeries& value, Util::HashSeed seed) {
        return (
              qHash(static_cast<const ChartLineStyle&>(value), seed)
            ^ qHash(value.currentMarkerStyle, seed)
            ^ qHash(value.currentSplineType, seed)
            ^ qHash(value.currentGradientType, seed)
        );
    }
}
