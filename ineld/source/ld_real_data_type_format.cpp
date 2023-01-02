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
* This file implements the \ref Ld::RealDataTypeFormat class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>
#include <QSet>

#include <cassert>

#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_program_file.h"
#include "ld_numeric_data_type_format.h"
#include "ld_real_data_type_format.h"

/***********************************************************************************************************************
 * Ld::RealDataTypeFormat::Aggregation
 */

namespace Ld {
    RealDataTypeFormat::Aggregation::Aggregation() {}


    RealDataTypeFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* RealDataTypeFormat::Aggregation::creator(const QString&) {
        return new RealDataTypeFormat::Aggregation;
    }


    QString RealDataTypeFormat::Aggregation::typeName() const {
        return RealDataTypeFormat::formatName;
    }


    void RealDataTypeFormat::Aggregation::clear() {
        NumericDataTypeFormat::Aggregation::clear();
        currentPrecisions.clear();
        currentRealNumberStyles.clear();
        currentMultipliers.clear();
    }


    bool RealDataTypeFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<RealDataTypeFormat> realDataTypeFormat = formatInstance.dynamicCast<RealDataTypeFormat>();
        if (!realDataTypeFormat.isNull()) {
            success = NumericDataTypeFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentPrecisions       << realDataTypeFormat->precision();
                currentRealNumberStyles << realDataTypeFormat->realNumberStyle();
                currentMultipliers      << realDataTypeFormat->multiplier();
            }
        }

        return success;
    }


    QSet<unsigned> RealDataTypeFormat::Aggregation::precisions() const {
        return currentPrecisions;
    }


    QSet<RealDataTypeFormat::RealNumberStyle> RealDataTypeFormat::Aggregation::realNumberStyles() const {
        return currentRealNumberStyles;
    }


    QSet<double> RealDataTypeFormat::Aggregation::multipliers() const {
        return currentMultipliers;
    }
}

/***********************************************************************************************************************
 * Ld::RealDataTypeFormat
 */

namespace Ld {
    const QString                             RealDataTypeFormat::formatName("RealDataTypeFormat");
    const unsigned                            RealDataTypeFormat::inferPrecision    = static_cast<unsigned>(-1);
    const unsigned                            RealDataTypeFormat::defaultPrecision  = static_cast<unsigned>(-1);
    const double                              RealDataTypeFormat::defaultMultiplier = double(1);
    const RealDataTypeFormat::RealNumberStyle
        RealDataTypeFormat::defaultRealNumberStyle = RealDataTypeFormat::RealNumberStyle::CONCISE;

    RealDataTypeFormat::RealDataTypeFormat() {
        currentPrecision       = defaultPrecision;
        currentRealNumberStyle = defaultRealNumberStyle;
        currentMultiplier      = defaultMultiplier;
    }


    RealDataTypeFormat::RealDataTypeFormat(
            const RealDataTypeFormat& other
        ):Format(
            other
        ),FontFormat(
            other
        ),DataTypeFormat(
            other
        ),NumericDataTypeFormat(
            other
        ) {
        currentPrecision       = other.currentPrecision;
        currentRealNumberStyle = other.currentRealNumberStyle;
        currentMultiplier      = other.currentMultiplier;
    }


    RealDataTypeFormat::~RealDataTypeFormat() {}


    FormatPointer RealDataTypeFormat::clone() const {
        return FormatPointer(new RealDataTypeFormat(*this));
    }


    Format* RealDataTypeFormat::creator(const QString&) {
        return new RealDataTypeFormat();
    }


    QString RealDataTypeFormat::typeName() const {
        return formatName;
    }


    bool RealDataTypeFormat::isValid() const {
        return (
               NumericDataTypeFormat::isValid()
            && currentRealNumberStyle != RealNumberStyle::INVALID
            && currentMultiplier != 0.0F
        );
    }


    Format::Capabilities RealDataTypeFormat::capabilities() const {
        return NumericDataTypeFormat::capabilities() << formatName;
    }


    void RealDataTypeFormat::setPrecision(unsigned newPrecision) {
        currentPrecision = newPrecision;
        reportFormatUpdated();
    }


    unsigned RealDataTypeFormat::precision() const {
        return currentPrecision;
    }


    void RealDataTypeFormat::setRealNumberStyle(RealDataTypeFormat::RealNumberStyle newRealNumberStyle) {
        currentRealNumberStyle = newRealNumberStyle;
        reportFormatUpdated();
    }


    RealDataTypeFormat::RealNumberStyle RealDataTypeFormat::realNumberStyle() const {
        return currentRealNumberStyle;
    }


    void RealDataTypeFormat::setMultiplier(double newMultiplier) {
        currentMultiplier = newMultiplier;
        reportFormatUpdated();
    }


    double RealDataTypeFormat::multiplier() const {
        return currentMultiplier;
    }


    QString RealDataTypeFormat::toString() const {
        return QString("%1,%2,%3,%4")
               .arg(NumericDataTypeFormat::toString())
               .arg(toString(currentRealNumberStyle))
               .arg(currentPrecision)
               .arg(static_cast<double>(currentMultiplier));
    }


    QString RealDataTypeFormat::toCss() const {
        QString result = NumericDataTypeFormat::toCss();
        return result;
    }


    void RealDataTypeFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (currentPrecision != defaultPrecision) {
            attributes.append("precision", currentPrecision);
        }

        if (currentRealNumberStyle != defaultRealNumberStyle) {
            attributes.append("real_style", toString(currentRealNumberStyle));
        }

        if (currentMultiplier != defaultMultiplier) {
            attributes.append("multiplier", currentMultiplier);
        }
    }


    void RealDataTypeFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        if (!reader->hasError()) {
            if (attributes.hasAttribute("precision")) {
                bool     ok;
                unsigned newPrecision = attributes.value<unsigned>("precision", &ok);
                if (!ok) {
                    QString tag = reader->qualifiedName().toString();
                    reader->raiseError(tr("Tag \"%1\", invalid \"precision\" attribute").arg(tag));
                } else {
                    currentPrecision = newPrecision;
                }
            } else {
                currentPrecision = defaultPrecision;
            }
        }

        if (!reader->hasError()) {
            if (attributes.hasAttribute("real_style")) {
                QString notationString = attributes.value<QString>("real_style");

                bool            ok;
                RealNumberStyle newRealNumberStyle = toRealNumberStyle(notationString, &ok);

                if (!ok || newRealNumberStyle == RealNumberStyle::INVALID) {
                    QString tag = reader->qualifiedName().toString();
                    reader->raiseError(tr("Tag \"%1\", invalid \"real_style\" attribute").arg(tag));
                } else {
                    currentRealNumberStyle = newRealNumberStyle;
                }
            } else {
                currentRealNumberStyle = defaultRealNumberStyle;
            }
        }

        if (!reader->hasError()) {
            if (attributes.hasAttribute("multiplier")) {
                bool   ok;
                double newMultiplier = attributes.value<double>("multiplier", &ok);

                if (!ok || newMultiplier == 0) {
                    QString tag = reader->qualifiedName().toString();
                    reader->raiseError(tr("Tag \"%1\", invalid \"multiplier\" attribute").arg(tag));
                } else {
                    currentMultiplier = newMultiplier;
                }
            } else {
                currentMultiplier = defaultMultiplier;
            }
        }
    }


    void RealDataTypeFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        FontFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        DataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        NumericDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        RealDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void RealDataTypeFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        FontFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        DataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        NumericDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        RealDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }


    QString RealDataTypeFormat::toString(RealDataTypeFormat::RealNumberStyle realNumberStyle) {
        QString result;
        switch (realNumberStyle) {
            case RealNumberStyle::INVALID:             { result = QString("INVALID");              break; }
            case RealNumberStyle::FLOATING_POINT:      { result = QString("FLOATING_POINT");       break; }
            case RealNumberStyle::SCIENTIFIC:          { result = QString("SCIENTIFIC");           break; }
            case RealNumberStyle::ENGINEERING:         { result = QString("ENGINEERING");          break; }
            case RealNumberStyle::COMPUTER_SCIENTIFIC: { result = QString("COMPUTER_SCIENTIFIC");  break; }
            case RealNumberStyle::CONCISE:             { result = QString("CONCISE");              break; }
            case RealNumberStyle::COMPUTER_CONCISE:    { result = QString("COMPUTER_CONCISE");     break; }

            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    RealDataTypeFormat::RealNumberStyle RealDataTypeFormat::toRealNumberStyle(const QString& str, bool* ok) {
        RealNumberStyle result;
        bool            success;

        QString lc = str.toLower();
        if        (lc == QString("invalid")) {
            result  = RealNumberStyle::INVALID;
            success = true;
        } else if (lc == QString("floating_point")) {
            result  = RealNumberStyle::FLOATING_POINT;
            success = true;
        } else if (lc == QString("scientific")) {
            result  = RealNumberStyle::SCIENTIFIC;
            success = true;
        } else if (lc == QString("engineering")) {
            result  = RealNumberStyle::ENGINEERING;
            success = true;
        } else if (lc == QString("computer_scientific")) {
            result  = RealNumberStyle::COMPUTER_SCIENTIFIC;
            success = true;
        } else if (lc == QString("concise")) {
            result  = RealNumberStyle::CONCISE;
            success = true;
        } else if (lc == QString("computer_concise")) {
            result  = RealNumberStyle::COMPUTER_CONCISE;
            success = true;
        } else {
            result  = RealNumberStyle::INVALID;
            success = false;
        }

        if (ok != nullptr) {
            *ok = success;
        }

        return result;
    }
}
