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
* This file implements the \ref Ld::TupleDataTypeFormat class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>
#include <QSet>
#include <QHash>

#include <cassert>

#include <util_hash_functions.h>

#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_program_file.h"
#include "ld_list_data_type_format.h"
#include "ld_tuple_data_type_format.h"

/***********************************************************************************************************************
 * Ld::TupleDataTypeFormat::Aggregation
 */

namespace Ld {
    TupleDataTypeFormat::Aggregation::Aggregation() {}


    TupleDataTypeFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* TupleDataTypeFormat::Aggregation::creator(const QString&) {
        return new TupleDataTypeFormat::Aggregation;
    }


    QString TupleDataTypeFormat::Aggregation::typeName() const {
        return TupleDataTypeFormat::formatName;
    }


    void TupleDataTypeFormat::Aggregation::clear() {
        ListDataTypeFormat::Aggregation::clear();
        currentDisplayModes.clear();
    }


    bool TupleDataTypeFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<TupleDataTypeFormat> tupleDataTypeFormat = formatInstance.dynamicCast<TupleDataTypeFormat>();

        if (!tupleDataTypeFormat.isNull()) {
            success = ListDataTypeFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentDisplayModes << tupleDataTypeFormat->displayMode();
            }
        }

        return success;

    }


    const QSet<TupleDataTypeFormat::DisplayMode>& TupleDataTypeFormat::Aggregation::displayModes() const {
        return currentDisplayModes;
    }
}

/***********************************************************************************************************************
 * Ld::TupleDataTypeFormat
 */

namespace Ld {
    const QString TupleDataTypeFormat::formatName("TupleDataTypeFormat");
    const TupleDataTypeFormat::DisplayMode
                  TupleDataTypeFormat::defaultDisplayMode = TupleDataTypeFormat::DisplayMode::NORMAL;

    TupleDataTypeFormat::TupleDataTypeFormat() {
        currentDisplayMode = defaultDisplayMode;
    }


    TupleDataTypeFormat::TupleDataTypeFormat(
            const TupleDataTypeFormat& other
        ):Format(
            other
        ),FontFormat(
            other
        ),DataTypeFormat(
            other
        ),ListDataTypeFormat(
            other
        ) {
        currentDisplayMode = other.currentDisplayMode;
    }


    TupleDataTypeFormat::~TupleDataTypeFormat() {}


    FormatPointer TupleDataTypeFormat::clone() const {
        return FormatPointer(new TupleDataTypeFormat(*this));
    }


    Format* TupleDataTypeFormat::creator(const QString&) {
        return new TupleDataTypeFormat();
    }


    QString TupleDataTypeFormat::typeName() const {
        return formatName;
    }


    bool TupleDataTypeFormat::isValid() const {
        return ListDataTypeFormat::isValid() && currentDisplayMode != DisplayMode::INVALID;
    }


    Format::Capabilities TupleDataTypeFormat::capabilities() const {
        return ListDataTypeFormat::capabilities() << formatName;
    }


    void TupleDataTypeFormat::setDisplayMode(TupleDataTypeFormat::DisplayMode newDisplayMode) {
        currentDisplayMode = newDisplayMode;
        reportFormatUpdated();
    }


    TupleDataTypeFormat::DisplayMode TupleDataTypeFormat::displayMode() const {
        return currentDisplayMode;
    }


    QString TupleDataTypeFormat::toString() const {
        return QString("%1,%2").arg(ListDataTypeFormat::toString()).arg(toString(currentDisplayMode));
    }


    QString TupleDataTypeFormat::toCss() const {
        QString result = ListDataTypeFormat::toCss();
        return result;
    }


    void TupleDataTypeFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (currentDisplayMode != defaultDisplayMode) {
            attributes.append("display_mode", toString(currentDisplayMode));
        }
    }


    void TupleDataTypeFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        if (!reader->hasError()) {
            if (attributes.hasAttribute("display_mode")) {
                bool        ok;
                QString     displayModeString = attributes.value<QString>("display_mode");
                DisplayMode newDisplayMode    = toDisplayMode(displayModeString, &ok);

                if (!ok) {
                    QString tag = reader->qualifiedName().toString();
                    reader->raiseError(
                        tr("Tag \"%1\", invalid \"display_mode\" attribute \"%2\"").arg(tag).arg(displayModeString)
                    );
                } else {
                    currentDisplayMode = newDisplayMode;
                }
            }
        }
    }


    void TupleDataTypeFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        FontFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        DataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        ListDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        TupleDataTypeFormat::writeAddImmediateAttributes(attributes,formats, programFile);
    }


    void TupleDataTypeFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        FontFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        DataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        ListDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        TupleDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }


    QString TupleDataTypeFormat::toString(TupleDataTypeFormat::DisplayMode displayMode) {
        QString result;
        switch (displayMode) {
            case DisplayMode::INVALID:               { result = QString("INVALID");                 break; }
            case DisplayMode::NORMAL:                { result = QString("NORMAL");                  break; }
            case DisplayMode::STRING_BARE:           { result = QString("STRING_BARE");             break; }
            case DisplayMode::STRING_ANGLE_BRACKETS: { result = QString("STRING_ANGLE_BRACKETS");   break; }
            case DisplayMode::STRING_SINGLE_QUOTES:  { result = QString("STRING_SINGLE_QUOTES");    break; }
            case DisplayMode::STRING_DOUBLE_QUOTES:  { result = QString("STRING_DOUBLE_QUOTES");    break; }
            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    TupleDataTypeFormat::DisplayMode TupleDataTypeFormat::toDisplayMode(const QString& str, bool* ok) {
        DisplayMode result;
        bool        isOk = true;
        QString     s    = str.trimmed().toLower();

        if (s == QString("invalid")) {
            result = DisplayMode::INVALID;
        } else if (s == QString("normal")) {
            result = DisplayMode::NORMAL;
        } else if (s == QString("string_bare")) {
            result = DisplayMode::STRING_BARE;
        } else if (s == QString("string_angle_brackets")) {
            result = DisplayMode::STRING_ANGLE_BRACKETS;
        } else if (s == QString("string_single_quotes")) {
            result = DisplayMode::STRING_SINGLE_QUOTES;
        } else if (s == QString("string_double_quotes")) {
            result = DisplayMode::STRING_DOUBLE_QUOTES;
        } else {
            result = DisplayMode::INVALID;
            isOk   = false;
        }

        if (ok != nullptr) {
            *ok = isOk;
        }

        return result;
    }


    Util::HashResult qHash(TupleDataTypeFormat::DisplayMode displayMode, Util::HashSeed seed) {
        return ::qHash(static_cast<unsigned>(displayMode), seed);
    }
}
