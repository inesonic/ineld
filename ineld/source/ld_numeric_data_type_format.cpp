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
* This file implements the \ref Ld::NumericDataTypeFormat class.
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
#include "ld_numeric_data_type_format.h"

/***********************************************************************************************************************
 * Ld::NumericDataTypeFormat::Aggregation
 */

namespace Ld {
    NumericDataTypeFormat::Aggregation::Aggregation() {}


    NumericDataTypeFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* NumericDataTypeFormat::Aggregation::creator(const QString&) {
        return new NumericDataTypeFormat::Aggregation;
    }


    QString NumericDataTypeFormat::Aggregation::typeName() const {
        return NumericDataTypeFormat::formatName;
    }


    void NumericDataTypeFormat::Aggregation::clear() {
        DataTypeFormat::Aggregation::clear();
        currentUpperCase.clear();
        currentLowerCase.clear();
    }


    bool NumericDataTypeFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<NumericDataTypeFormat>
            numericDataTypeFormat = formatInstance.dynamicCast<NumericDataTypeFormat>();
        if (!numericDataTypeFormat.isNull()) {
            success = DataTypeFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentUpperCase << numericDataTypeFormat->upperCase();
                currentLowerCase << numericDataTypeFormat->lowerCase();
            }
        }

        return success;
    }


    const Format::BooleanAggregation& NumericDataTypeFormat::Aggregation::upperCase() const {
        return currentUpperCase;
    }


    const Format::BooleanAggregation& NumericDataTypeFormat::Aggregation::lowerCase() const {
        return currentLowerCase;
    }
}

/***********************************************************************************************************************
 * Ld::NumericDataTypeFormat
 */

namespace Ld {
    const QString  NumericDataTypeFormat::formatName("NumericDataTypeFormat");
    const bool     NumericDataTypeFormat::defaultUpperCase = true;

    NumericDataTypeFormat::NumericDataTypeFormat() {
        currentUpperCase = defaultUpperCase;
    }


    NumericDataTypeFormat::NumericDataTypeFormat(
            const NumericDataTypeFormat& other
        ):Format(
            other
        ),FontFormat(
            other
        ),DataTypeFormat(
            other
        ) {
        currentUpperCase = other.currentUpperCase;
    }


    NumericDataTypeFormat::~NumericDataTypeFormat() {}


    FormatPointer NumericDataTypeFormat::clone() const {
        return FormatPointer(new NumericDataTypeFormat(*this));
    }


    Format* NumericDataTypeFormat::creator(const QString&) {
        return new NumericDataTypeFormat();
    }


    QString NumericDataTypeFormat::typeName() const {
        return formatName;
    }


    Format::Capabilities NumericDataTypeFormat::capabilities() const {
        return DataTypeFormat::capabilities() << formatName;
    }


    void NumericDataTypeFormat::setUpperCase(bool nowUpperCase) {
        currentUpperCase = nowUpperCase;
        reportFormatUpdated();
    }


    void NumericDataTypeFormat::setLowerCase(bool nowLowerCase) {
        setUpperCase(!nowLowerCase);
    }


    bool NumericDataTypeFormat::upperCase() const {
        return currentUpperCase;
    }


    bool NumericDataTypeFormat::lowerCase() const {
        return !currentUpperCase;
    }


    QString NumericDataTypeFormat::toString() const {
        return QString("%1,%2").arg(DataTypeFormat::toString()).arg(currentUpperCase ? "UPPER" : "LOWER");
    }


    QString NumericDataTypeFormat::toCss() const {
        QString result = DataTypeFormat::toCss();
        return result;
    }


    void NumericDataTypeFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (currentUpperCase != defaultUpperCase) {
            attributes.append("upper_case", currentUpperCase);
        }
    }


    void NumericDataTypeFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        if (!reader->hasError()) {
            if (attributes.hasAttribute("upper_case")) {
                bool ok;
                bool newUpperCase = attributes.value<bool>("upper_case", &ok);
                if (!ok) {
                    QString tag = reader->qualifiedName().toString();
                    reader->raiseError(tr("Tag \"%1\", invalid \"upper_case\" attribute").arg(tag));
                } else {
                    currentUpperCase = newUpperCase;
                }
            } else {
                currentUpperCase = defaultUpperCase;
            }
        }
    }


    void NumericDataTypeFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        FontFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        DataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        NumericDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void NumericDataTypeFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        FontFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        DataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        NumericDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }
}
