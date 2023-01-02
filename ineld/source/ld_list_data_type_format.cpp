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
* This file implements the \ref Ld::ListDataTypeFormat class.
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
#include "ld_list_data_type_format.h"

/***********************************************************************************************************************
 * Ld::ListDataTypeFormat::Aggregation
 */

namespace Ld {
    ListDataTypeFormat::Aggregation::Aggregation() {}


    ListDataTypeFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* ListDataTypeFormat::Aggregation::creator(const QString&) {
        return new ListDataTypeFormat::Aggregation;
    }


    QString ListDataTypeFormat::Aggregation::typeName() const {
        return ListDataTypeFormat::formatName;
    }


    void ListDataTypeFormat::Aggregation::clear() {
        DataTypeFormat::Aggregation::clear();

        currentLeadingMemberCounts.clear();
        currentTrailingMemberCounts.clear();
        currentShowHiddenMemberCounts.clear();
    }


    bool ListDataTypeFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<ListDataTypeFormat> listDataTypeFormat = formatInstance.dynamicCast<ListDataTypeFormat>();

        if (!listDataTypeFormat.isNull()) {
            success = DataTypeFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentLeadingMemberCounts    << listDataTypeFormat->leadingMemberCount();
                currentTrailingMemberCounts   << listDataTypeFormat->trailingMemberCount();
                currentShowHiddenMemberCounts << listDataTypeFormat->displayHiddenMemberCount();
            }
        }

        return success;
    }


    const QSet<unsigned long>& ListDataTypeFormat::Aggregation::leadingMemberCounts() const {
        return currentLeadingMemberCounts;
    }


    const QSet<unsigned long>& ListDataTypeFormat::Aggregation::trailingMemberCounts() const {
        return currentTrailingMemberCounts;
    }


    const Format::BooleanAggregation& ListDataTypeFormat::Aggregation::showHiddenMemberCounts() const {
        return currentShowHiddenMemberCounts;
    }
}

/***********************************************************************************************************************
 * Ld::ListDataTypeFormat
 */

namespace Ld {
    const QString ListDataTypeFormat::formatName("ListDataTypeFormat");

    const unsigned long ListDataTypeFormat::defaultLeadingMemberCount    = ListDataTypeFormat::showAllMembers;
    const unsigned long ListDataTypeFormat::defaultTrailingMemberCount   = ListDataTypeFormat::showAllMembers;
    const bool          ListDataTypeFormat::defaultShowHiddenMemberCount = true;
    const unsigned long ListDataTypeFormat::showAllMembers               = static_cast<unsigned long>(-1);

    ListDataTypeFormat::ListDataTypeFormat() {
        currentLeadingMemberCount    = defaultLeadingMemberCount;
        currentTrailingMemberCount   = defaultTrailingMemberCount;
        currentShowHiddenMemberCount = defaultShowHiddenMemberCount;
    }


    ListDataTypeFormat::ListDataTypeFormat(
            const ListDataTypeFormat& other
        ):Format(
            other
        ),FontFormat(
            other
        ),DataTypeFormat(
            other
        ) {
        currentLeadingMemberCount    = other.currentLeadingMemberCount;
        currentTrailingMemberCount   = other.currentTrailingMemberCount;
        currentShowHiddenMemberCount = other.currentShowHiddenMemberCount;
    }


    ListDataTypeFormat::~ListDataTypeFormat() {}


    FormatPointer ListDataTypeFormat::clone() const {
        return FormatPointer(new ListDataTypeFormat(*this));
    }


    Format* ListDataTypeFormat::creator(const QString&) {
        return new ListDataTypeFormat();
    }


    QString ListDataTypeFormat::typeName() const {
        return formatName;
    }


    Format::Capabilities ListDataTypeFormat::capabilities() const {
        return DataTypeFormat::capabilities() << formatName;
    }


    void ListDataTypeFormat::setLeadingMemberCount(unsigned long newLeadingMemberCount) {
        currentLeadingMemberCount = newLeadingMemberCount;
        reportFormatUpdated();
    }


    unsigned long ListDataTypeFormat::leadingMemberCount() const {
        return currentLeadingMemberCount;
    }


    void ListDataTypeFormat::setTrailingMemberCount(unsigned long newTrailingMemberCount) {
        currentTrailingMemberCount = newTrailingMemberCount;
        reportFormatUpdated();
    }


    unsigned long ListDataTypeFormat::trailingMemberCount() const {
        return currentTrailingMemberCount;
    }


    void ListDataTypeFormat::setHiddenMemberCountVisible(bool nowDisplayed) {
        currentShowHiddenMemberCount = nowDisplayed;
        reportFormatUpdated();
    }


    void ListDataTypeFormat::setHiddenMemberCountHidden(bool nowHidden) {
        setHiddenMemberCountVisible(!nowHidden);
    }


    bool ListDataTypeFormat::displayHiddenMemberCount() const {
        return currentShowHiddenMemberCount;
    }


    bool ListDataTypeFormat::hideHiddenMemberCount() const {
        return !displayHiddenMemberCount();
    }


    QString ListDataTypeFormat::toString() const {
        return QString("%1,%2,%3,%4")
               .arg(DataTypeFormat::toString())
               .arg(currentLeadingMemberCount)
               .arg(currentTrailingMemberCount)
               .arg(currentShowHiddenMemberCount ? "SHOW" : "HIDE");
    }


    QString ListDataTypeFormat::toCss() const {
        QString result = DataTypeFormat::toCss();
        return result;
    }


    void ListDataTypeFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if ((currentLeadingMemberCount == showAllMembers && defaultLeadingMemberCount != showAllMembers)   &&
            (currentTrailingMemberCount == showAllMembers && defaultTrailingMemberCount != showAllMembers)    ) {
            attributes.append("show_all_members", true);
        } else {
            if (currentLeadingMemberCount != defaultLeadingMemberCount) {
                attributes.append("leading_member_count", currentLeadingMemberCount);
            }

            if (currentTrailingMemberCount != defaultTrailingMemberCount) {
                attributes.append("trailing_member_count", currentTrailingMemberCount);
            }
        }

        if (currentShowHiddenMemberCount != defaultShowHiddenMemberCount) {
            attributes.append("show_hidden_member_count", currentShowHiddenMemberCount);
        }
    }


    void ListDataTypeFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        bool enableShowAllMembers = (
               defaultLeadingMemberCount == showAllMembers
            || defaultTrailingMemberCount == showAllMembers
        );

        if (!reader->hasError()) {
            if (attributes.hasAttribute("show_all_members")) {
                bool    ok;
                enableShowAllMembers = attributes.value<bool>("show_all_members", &ok);
                if (!ok) {
                    QString tag   = reader->qualifiedName().toString();
                    QString value = attributes.value<QString>("show_all_members");
                    reader->raiseError(
                        tr("Tag \"%1\", invalid \"show_all_members\" attribute \"%2\"").arg(tag).arg(value)
                    );
                }
            }
        }

        if (!reader->hasError()) {
            if (enableShowAllMembers) {
                currentLeadingMemberCount  = showAllMembers;
                currentTrailingMemberCount = showAllMembers;
            } else {
                bool ok;

                if (attributes.hasAttribute("leading_member_count")) {
                    currentLeadingMemberCount = attributes.value<unsigned long>("leading_member_count", &ok);
                    if (!ok) {
                        QString tag   = reader->qualifiedName().toString();
                        QString value = attributes.value<QString>("leading_member_count");
                        reader->raiseError(
                            tr("Tag \"%1\", invalid \"leading_member_count\" attribute \"%2\"").arg(tag).arg(value)
                        );
                    }
                } else {
                    currentLeadingMemberCount = defaultLeadingMemberCount;
                }

                if (!reader->hasError()) {
                    if (attributes.hasAttribute("trailing_member_count")) {
                        currentTrailingMemberCount = attributes.value<unsigned long>("trailing_member_count", &ok);
                        if (!ok) {
                            QString tag   = reader->qualifiedName().toString();
                            QString value = attributes.value<QString>("trailing_member_count");
                            reader->raiseError(
                                tr("Tag \"%1\", invalid \"trailing_member_count\" attribute \"%2\"").arg(tag).arg(value)
                            );
                        }
                    } else {
                        currentTrailingMemberCount = defaultTrailingMemberCount;
                    }
                }
            }
        }

        if (!reader->hasError()) {
            if (attributes.hasAttribute("show_hidden_member_count")) {
                bool    ok;
                currentShowHiddenMemberCount = attributes.value<bool>("show_hidden_member_count", &ok);
                if (!ok) {
                    QString tag   = reader->qualifiedName().toString();
                    QString value = attributes.value<QString>("show_hidden_member_count");
                    reader->raiseError(
                        tr("Tag \"%1\", invalid \"show_hidden_member_count\" attribute \"%2\"").arg(tag).arg(value)
                    );
                }
            }
        }
    }


    void ListDataTypeFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        FontFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        DataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        ListDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void ListDataTypeFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        FontFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        DataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        ListDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }
}
