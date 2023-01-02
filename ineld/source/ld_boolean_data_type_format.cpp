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
* This file implements the \ref Ld::BooleanDataTypeFormat class.
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
#include "ld_data_type_format.h"
#include "ld_boolean_data_type_format.h"

/***********************************************************************************************************************
 * Ld::BooleanDataTypeFormat::Aggregation
 */

namespace Ld {
    BooleanDataTypeFormat::Aggregation::Aggregation() {}


    BooleanDataTypeFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* BooleanDataTypeFormat::Aggregation::creator(const QString&) {
        return new BooleanDataTypeFormat::Aggregation;
    }


    QString BooleanDataTypeFormat::Aggregation::typeName() const {
        return BooleanDataTypeFormat::formatName;
    }


    void BooleanDataTypeFormat::Aggregation::clear() {
        DataTypeFormat::Aggregation::clear();
        currentStyles.clear();
    }


    bool BooleanDataTypeFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<BooleanDataTypeFormat>
            booleanDataTypeFormat = formatInstance.dynamicCast<BooleanDataTypeFormat>();
        if (!booleanDataTypeFormat.isNull()) {
            success = DataTypeFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentStyles << booleanDataTypeFormat->booleanStyle();
            }
        }

        return success;
    }


    QSet<BooleanDataTypeFormat::BooleanStyle> BooleanDataTypeFormat::Aggregation::booleanStyles() const {
        return currentStyles;
    }
}

/***********************************************************************************************************************
 * Ld::BooleanDataTypeFormat
 */

namespace Ld {
    const QString  BooleanDataTypeFormat::formatName("BooleanDataTypeFormat");
    const BooleanDataTypeFormat::BooleanStyle
        BooleanDataTypeFormat::defaultBooleanStyle = BooleanDataTypeFormat::BooleanStyle::LOWER_CASE_TRUE_FALSE;

    BooleanDataTypeFormat::BooleanDataTypeFormat() {
        currentStyle = defaultBooleanStyle;
    }


    BooleanDataTypeFormat::BooleanDataTypeFormat(
            const BooleanDataTypeFormat& other
        ):Format(
            other
        ),FontFormat(
            other
        ),DataTypeFormat(
            other
        ) {
        currentStyle = other.currentStyle;
    }


    BooleanDataTypeFormat::~BooleanDataTypeFormat() {}


    FormatPointer BooleanDataTypeFormat::clone() const {
        return FormatPointer(new BooleanDataTypeFormat(*this));
    }


    Format* BooleanDataTypeFormat::creator(const QString&) {
        return new BooleanDataTypeFormat();
    }


    QString BooleanDataTypeFormat::typeName() const {
        return formatName;
    }


    bool BooleanDataTypeFormat::isValid() const {
        return DataTypeFormat::isValid() && currentStyle != BooleanStyle::INVALID;
    }


    Format::Capabilities BooleanDataTypeFormat::capabilities() const {
        return DataTypeFormat::capabilities() << formatName;
    }


    void BooleanDataTypeFormat::setBooleanStyle(BooleanStyle newBooleanStyle) {
        currentStyle = newBooleanStyle;
        reportFormatUpdated();
    }


    BooleanDataTypeFormat::BooleanStyle BooleanDataTypeFormat::booleanStyle() const {
        return currentStyle;
    }


    QString BooleanDataTypeFormat::toString() const {
        return QString("%1,%2").arg(DataTypeFormat::toString()).arg(toString(currentStyle));
    }


    QString BooleanDataTypeFormat::toCss() const {
        QString result = DataTypeFormat::toCss();
        return result;
    }


    void BooleanDataTypeFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (currentStyle != defaultBooleanStyle) {
            attributes.append("boolean_style", toString(currentStyle));
        }
    }


    void BooleanDataTypeFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        if (!reader->hasError()) {
            if (attributes.hasAttribute("boolean_style")) {
                QString booleanStyleString = attributes.value<QString>("boolean_style");

                bool         ok;
                BooleanStyle newBooleanStyle = toBooleanStyle(booleanStyleString, &ok);
                if (!ok) {
                    QString tag = reader->qualifiedName().toString();
                    reader->raiseError(
                        tr("Tag \"%1\", invalid \"boolean_style\" attribute, \"%2\"").arg(tag).arg(booleanStyleString)
                    );
                } else {
                    currentStyle = newBooleanStyle;
                }
            } else {
                currentStyle = defaultBooleanStyle;
            }
        }
    }


    void BooleanDataTypeFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        FontFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        DataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        BooleanDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void BooleanDataTypeFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        FontFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        DataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        BooleanDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }


    QString BooleanDataTypeFormat::toString(BooleanDataTypeFormat::BooleanStyle booleanStyle) {
        QString result;
        switch (booleanStyle) {
            case BooleanStyle::INVALID:               { result = QString("INVALID");                break; }
            case BooleanStyle::LOWER_CASE_TRUE_FALSE: { result = QString("LOWER_CASE_TRUE_FALSE");  break; }
            case BooleanStyle::TITLE_CASE_TRUE_FALSE: { result = QString("TITLE_CASE_TRUE_FALSE");  break; }
            case BooleanStyle::UPPER_CASE_TRUE_FALSE: { result = QString("UPPER_CASE_TRUE_FALSE");  break; }
            case BooleanStyle::LOWER_CASE_YES_NO:     { result = QString("LOWER_CASE_YES_NO");      break; }
            case BooleanStyle::TITLE_CASE_YES_NO:     { result = QString("TITLE_CASE_YES_NO");      break; }
            case BooleanStyle::UPPER_CASE_YES_NO:     { result = QString("UPPER_CASE_YES_NO");      break; }
            case BooleanStyle::NUMERIC_1_0:           { result = QString("NUMERIC_1_0");            break; }

            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    BooleanDataTypeFormat::BooleanStyle BooleanDataTypeFormat::toBooleanStyle(const QString& str, bool* ok) {
        BooleanStyle result;
        bool         success;

        QString lc = str.toLower();
        if        (lc == QString("invalid")) {
            result  = BooleanStyle::INVALID;
            success = true;
        } else if (lc == QString("lower_case_true_false")) {
            result  = BooleanStyle::LOWER_CASE_TRUE_FALSE;
            success = true;
        } else if (lc == QString("title_case_true_false")) {
            result  = BooleanStyle::TITLE_CASE_TRUE_FALSE;
            success = true;
        } else if (lc == QString("upper_case_true_false")) {
            result  = BooleanStyle::UPPER_CASE_TRUE_FALSE;
            success = true;
        } else if (lc == QString("lower_case_yes_no")) {
            result  = BooleanStyle::LOWER_CASE_YES_NO;
            success = true;
        } else if (lc == QString("title_case_yes_no")) {
            result  = BooleanStyle::TITLE_CASE_YES_NO;
            success = true;
        } else if (lc == QString("upper_case_yes_no")) {
            result  = BooleanStyle::UPPER_CASE_YES_NO;
            success = true;
        } else if (lc == QString("numeric_1_0")) {
            result  = BooleanStyle::NUMERIC_1_0;
            success = true;
        } else {
            result  = BooleanStyle::INVALID;
            success = false;
        }

        if (ok != nullptr) {
            *ok = success;
        }

        return result;
    }
}
