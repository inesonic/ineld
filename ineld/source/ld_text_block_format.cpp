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
* This file implements the \ref Ld::TextBlockFormat class.
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
#include "ld_format.h"
#include "ld_block_format.h"
#include "ld_justified_block_format.h"
#include "ld_text_block_format.h"

/***********************************************************************************************************************
 * Ld::TextBlockFormat::Aggregation
 */

namespace Ld {
    TextBlockFormat::Aggregation::Aggregation() {}


    TextBlockFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* TextBlockFormat::Aggregation::creator(const QString&) {
        return new TextBlockFormat::Aggregation;
    }


    QString TextBlockFormat::Aggregation::typeName() const {
        return TextBlockFormat::formatName;
    }


    void TextBlockFormat::Aggregation::clear() {
        JustifiedBlockFormat::Aggregation::clear();

        currentFirstLineLeftIndentations.clear();
        currentLineSpacings.clear();
    }


    bool TextBlockFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<TextBlockFormat> blockFormat = formatInstance.dynamicCast<TextBlockFormat>();
        if (!blockFormat.isNull()) {
            success = BlockFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentFirstLineLeftIndentations << blockFormat->firstLineLeftIndentation();
                currentLineSpacings << blockFormat->lineSpacing();
            }
        }

        return success;
    }


    QSet<float> TextBlockFormat::Aggregation::firstLineLeftIndentations() const {
        return currentFirstLineLeftIndentations;
    }


    QSet<float> TextBlockFormat::Aggregation::lineSpacings() const {
        return currentLineSpacings;
    }
}

/***********************************************************************************************************************
 * Ld::TextBlockFormat
 */

namespace Ld {
    const QString TextBlockFormat::formatName("TextBlockFormat");
    const float   TextBlockFormat::defaultLineSpacing = 1.0;

    TextBlockFormat::TextBlockFormat() {
        currentFirstLineIndentation    = -1;
        currentUseFirstLineIndentation = false;
        currentLineSpacing             = defaultLineSpacing;
    }


    TextBlockFormat::TextBlockFormat(
            const TextBlockFormat& other
        ):Format(
            other
        ),BlockFormat(
            other
        ),JustifiedBlockFormat(
            other
        ) {
        currentFirstLineIndentation    = other.currentFirstLineIndentation;
        currentUseFirstLineIndentation = other.currentUseFirstLineIndentation;
        currentLineSpacing             = other.currentLineSpacing;
    }


    TextBlockFormat::~TextBlockFormat() {}


    FormatPointer TextBlockFormat::clone() const {
        return FormatPointer(new TextBlockFormat(*this));
    }


    Format* TextBlockFormat::creator(const QString&) {
        return new TextBlockFormat();
    }


    QString TextBlockFormat::typeName() const {
        return formatName;
    }


    // This method is not really needed as it just calls the base-class version; however, providing it here makes for a
    // clearer API: specialization and clear API's is more important than minimizeding code.
    bool TextBlockFormat::isValid() const {
        return JustifiedBlockFormat::isValid();
    }


    Format::Capabilities TextBlockFormat::capabilities() const {
        return JustifiedBlockFormat::capabilities() << formatName;
    }


    void TextBlockFormat::setFirstLineLeftIndentation(float newFirstLineIndentation) {
        currentFirstLineIndentation    = newFirstLineIndentation;
        currentUseFirstLineIndentation = true;
        reportFormatUpdated();
    }


    float TextBlockFormat::firstLineLeftIndentation() const {
        return currentUseFirstLineIndentation ? currentFirstLineIndentation : leftIndentation();
    }


    void TextBlockFormat::clearFirstLineLeftIndentation() {
        currentUseFirstLineIndentation = false;
        reportFormatUpdated();
    }


    void TextBlockFormat::setLineSpacing(float newLineSpacing) {
        currentLineSpacing = newLineSpacing;
        reportFormatUpdated();
    }


    float TextBlockFormat::lineSpacing() const {
        return currentLineSpacing;
    }


    QString TextBlockFormat::toCss() const {
        QString result = JustifiedBlockFormat::toCss();

        if (currentFirstLineIndentation != 0) {
            result += QString("text-indent: %1pt; ").arg(currentFirstLineIndentation);
        }

        if (currentLineSpacing != 1.0F) {
            result += QString("line-height: %1; ").arg(currentLineSpacing);
        }

        return result;
    }


    QString TextBlockFormat::toString() const {
        return QString("%1,%2,%3").arg(JustifiedBlockFormat::toString()).arg(firstLineLeftIndentation())
                                                                        .arg(currentLineSpacing);
    }


    void TextBlockFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (firstLineLeftIndentation() != leftIndentation()) {
            attributes.append("hanging", firstLineLeftIndentation());
        }

        if (lineSpacing() != defaultLineSpacing) {
            attributes.append("spacing", lineSpacing());
        }
    }


    void TextBlockFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        if (!reader->hasError()) {
            if (attributes.hasAttribute("hanging")) {
                bool ok;
                setFirstLineLeftIndentation(attributes.value<float>("hanging", &ok));
                if (!ok) {
                    QString tag = reader->qualifiedName().toString();
                    reader->raiseError(tr("Tag \"%1\", invalid \"hanging\" attribute").arg(tag));
                }
            } else {
                clearFirstLineLeftIndentation();
            }
        }

        if (!reader->hasError()) {
            bool ok;
            setLineSpacing(attributes.value<float>("spacing", defaultLineSpacing, &ok));
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid \"spacing\" attribute").arg(tag));
            }
        }
    }


    void TextBlockFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        BlockFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        JustifiedBlockFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        TextBlockFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void TextBlockFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        BlockFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        JustifiedBlockFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        TextBlockFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }
}
