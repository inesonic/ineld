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
* This file implements the \ref Ld::ListParagraphFormatBase class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>
#include <QSet>

#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_program_file.h"
#include "ld_format.h"
#include "ld_block_format.h"
#include "ld_justified_block_format.h"
#include "ld_text_block_format.h"
#include "ld_paragraph_format.h"
#include "ld_font_format.h"
#include "ld_list_additional_paragraph_format.h"
#include "ld_list_paragraph_format_base.h"

/***********************************************************************************************************************
 * Ld::ListParagraphFormatBase::Aggregation
 */

namespace Ld {
    ListParagraphFormatBase::Aggregation::Aggregation() {}


    ListParagraphFormatBase::Aggregation::~Aggregation() {}


    Format::Aggregation* ListParagraphFormatBase::Aggregation::creator(const QString&) {
        return new ListParagraphFormatBase::Aggregation;
    }


    QString ListParagraphFormatBase::Aggregation::typeName() const {
        return ListParagraphFormatBase::formatName;
    }


    void ListParagraphFormatBase::Aggregation::clear() {
        FontFormat::Aggregation::clear();
        ParagraphFormat::Aggregation::clear();
    }


    bool ListParagraphFormatBase::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<Ld::ListParagraphFormatBase> format = formatInstance.dynamicCast<ListParagraphFormatBase>();
        if (!format.isNull()) {
            success = FontFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                success = ParagraphFormat::Aggregation::addFormat(formatInstance, true);
            }

            if (success) {
                currentListIndentations << format->listIndentation();
            }
        }

        return success;
    }


    QSet<float> ListParagraphFormatBase::Aggregation::listIndentations() const {
        return currentListIndentations;
    }
}

/***********************************************************************************************************************
 * Ld::ListParagraphFormatBase
 */

namespace Ld {
    const QString ListParagraphFormatBase::formatName("ListParagraphFormatBase");
    const float   ListParagraphFormatBase::defaultListIndentation = 18.0;

    ListParagraphFormatBase::ListParagraphFormatBase() {
        currentListIndentation = defaultListIndentation;
    }


    ListParagraphFormatBase::ListParagraphFormatBase(
            const ListParagraphFormatBase& other
        ):Format(
            other
        ),BlockFormat(
            other
        ),JustifiedBlockFormat(
            other
        ),TextBlockFormat(
            other
        ),ParagraphFormat(
            other
        ),FontFormat(
            other
        ) {
        currentListIndentation = other.currentListIndentation;
    }


    ListParagraphFormatBase::~ListParagraphFormatBase() {}


    FormatPointer ListParagraphFormatBase::clone() const {
        return FormatPointer(new ListParagraphFormatBase(*this));
    }


    FormatPointer ListParagraphFormatBase::formatForSplit(ElementPointer) {
        QSharedPointer<ListAdditionalParagraphFormat> listFormat(new ListAdditionalParagraphFormat);
        listFormat->bulletNumberDisplayEnabled();

        return listFormat;
    }


    QString ListParagraphFormatBase::typeName() const {
        return formatName;
    }


    bool ListParagraphFormatBase::isValid() const {
        return ParagraphFormat::isValid() || FontFormat::isValid();
    }


    Format::Capabilities ListParagraphFormatBase::capabilities() const {
        return ParagraphFormat::capabilities() << formatName; // Note: FontFormat is intentionally excluded
    }


    void ListParagraphFormatBase::setListIndentation(float newListIndentation) {
        currentListIndentation = newListIndentation;
        reportFormatUpdated();
    }


    float ListParagraphFormatBase::listIndentation() const {
        return currentListIndentation;
    }


    QString ListParagraphFormatBase::toString() const {
        QString paragraphFormatString = ParagraphFormat::toString();

        QString fontFormatString = FontFormat::toString();
        int     firstComma       = fontFormatString.indexOf(",");

        QString result = QString("%1%2,%3").arg(paragraphFormatString)
                                           .arg(fontFormatString.mid(firstComma))
                                           .arg(currentListIndentation);

        return result;
    }


    QString ListParagraphFormatBase::toCss() const {
        QString result = ParagraphFormat::toCss();
        result += "padding-left: 0pt; "; // Required for cross-browser compatibility

        return result;
    }


    void ListParagraphFormatBase::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (currentListIndentation != defaultListIndentation) {
            attributes.append("list_indentation", currentListIndentation);
        }
    }


    void ListParagraphFormatBase::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        if (!reader->hasError()) {
            if (attributes.hasAttribute("list_indentation")) {
                bool ok;
                float newIndentation = attributes.value<float>("list_indentation", &ok);
                if (!ok) {
                    QString tag   = reader->qualifiedName().toString();
                    QString value = attributes.value<QString>("list_indentation");
                    reader->raiseError(
                        tr("Tag \"%1\", invalid \"list_indentation\" attribute \"%2\"").arg(tag).arg(value)
                    );
                } else {
                    currentListIndentation = newIndentation;
                }
            } else {
                currentListIndentation = defaultListIndentation;
            }
        }
    }


    void ListParagraphFormatBase::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        BlockFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        JustifiedBlockFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        TextBlockFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        ParagraphFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        FontFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        ListParagraphFormatBase::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void ListParagraphFormatBase::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        BlockFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        JustifiedBlockFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        TextBlockFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        ParagraphFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        FontFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        ListParagraphFormatBase::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }
}
