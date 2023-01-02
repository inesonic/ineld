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
* This file implements the \ref Ld::ListAdditionalParagraphFormat class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>
#include <QSet>

#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_program_file.h"
#include "ld_paragraph_format.h"
#include "ld_format.h"
#include "ld_list_additional_paragraph_format.h"

/***********************************************************************************************************************
 * Ld::ListAdditionalParagraphFormat::Aggregation
 */

namespace Ld {
    ListAdditionalParagraphFormat::Aggregation::Aggregation() {}


    ListAdditionalParagraphFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* ListAdditionalParagraphFormat::Aggregation::creator(const QString&) {
        return new ListAdditionalParagraphFormat::Aggregation;
    }


    QString ListAdditionalParagraphFormat::Aggregation::typeName() const {
        return ListAdditionalParagraphFormat::formatName;
    }


    void ListAdditionalParagraphFormat::Aggregation::clear() {
        Format::Aggregation::clear();
        currentBulletNumberStates.clear();
    }


    bool ListAdditionalParagraphFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<Ld::ListAdditionalParagraphFormat> format =
            formatInstance.dynamicCast<ListAdditionalParagraphFormat>();

        if (!format.isNull()) {
            success = Format::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentBulletNumberStates << format->bulletNumberDisplayEnabled();
            }
        }

        return success;
    }


    Ld::Format::BooleanAggregation ListAdditionalParagraphFormat::Aggregation::listBulletNumberStates() const {
        return currentBulletNumberStates;
    }
}

/***********************************************************************************************************************
 * Ld::ListAdditionalParagraphFormat
 */

namespace Ld {
    const QString ListAdditionalParagraphFormat::formatName("ListAdditionalParagraphFormat");
    const bool    ListAdditionalParagraphFormat::defaultBulletNumberShown = true;

    ListAdditionalParagraphFormat::ListAdditionalParagraphFormat() {
        currentBulletNumberDisplayEnabled = defaultBulletNumberShown;
    }


    ListAdditionalParagraphFormat::ListAdditionalParagraphFormat(
            const ListAdditionalParagraphFormat& other
        ):Format(
            other
        ) {
        currentBulletNumberDisplayEnabled = other.currentBulletNumberDisplayEnabled;
    }


    ListAdditionalParagraphFormat::~ListAdditionalParagraphFormat() {}


    FormatPointer ListAdditionalParagraphFormat::clone() const {
        return FormatPointer(new ListAdditionalParagraphFormat(*this));
    }


    Format* ListAdditionalParagraphFormat::creator(const QString&) {
        return new ListAdditionalParagraphFormat;
    }


    QString ListAdditionalParagraphFormat::typeName() const {
        return formatName;
    }


    bool ListAdditionalParagraphFormat::isValid() const {
        return true;
    }


    Format::Capabilities ListAdditionalParagraphFormat::capabilities() const {
        return Format::capabilities() << formatName;
    }


    void ListAdditionalParagraphFormat::setBulletNumberDisplayEnabled(bool nowDisplayed) {
        currentBulletNumberDisplayEnabled = nowDisplayed;
        reportFormatUpdated();
    }


    void ListAdditionalParagraphFormat::setBulletNumberDisplayDisabled(bool nowHidden) {
        setBulletNumberDisplayEnabled(!nowHidden);
    }


    bool ListAdditionalParagraphFormat::bulletNumberDisplayEnabled() const {
        return currentBulletNumberDisplayEnabled;
    }


    bool ListAdditionalParagraphFormat::bulletNumberDisplayDisabled() const {
        return !bulletNumberDisplayEnabled();
    }


    QString ListAdditionalParagraphFormat::toString() const {
        return QString("%1,%2").arg(formatName).arg(currentBulletNumberDisplayEnabled ? "true" : "false");
    }


    QString ListAdditionalParagraphFormat::toCss() const {
        // We never use the format data from this class directly so we don't generate CSS with it.
        return QString();
    }


    void ListAdditionalParagraphFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (currentBulletNumberDisplayEnabled != defaultBulletNumberShown) {
            attributes.append("show_list", true);
        }
    }


    void ListAdditionalParagraphFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        if (!reader->hasError()) {
            if (attributes.hasAttribute("show_list")) {
                bool ok;
                bool nowEnabled = attributes.value<bool>("show_list", &ok);
                if (!ok) {
                    QString tag   = reader->qualifiedName().toString();
                    QString value = attributes.value<QString>("show_list");
                    reader->raiseError(
                        tr("Tag \"%1\", invalid \"show_list\" attribute \"%2\"").arg(tag).arg(value)
                    );
                } else {
                    currentBulletNumberDisplayEnabled = nowEnabled;
                }
            } else {
                currentBulletNumberDisplayEnabled = defaultBulletNumberShown;
            }
        }
    }


    void ListAdditionalParagraphFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        ListAdditionalParagraphFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void ListAdditionalParagraphFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        ListAdditionalParagraphFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }
}
