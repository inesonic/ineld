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
* This file implements the \ref Ld::BlockFormat class.
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
#include "ld_format.h"
#include "ld_block_format.h"

/***********************************************************************************************************************
 * Ld::BlockFormat::Aggregation
 */

namespace Ld {
    BlockFormat::Aggregation::Aggregation() {}


    BlockFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* BlockFormat::Aggregation::creator(const QString&) {
        return new BlockFormat::Aggregation;
    }


    QString BlockFormat::Aggregation::typeName() const {
        return BlockFormat::formatName;
    }


    void BlockFormat::Aggregation::clear() {
        Format::Aggregation::clear();

        currentTopSpacings.clear();
        currentBottomSpacings.clear();
        currentLeftIndentations.clear();
        currentRightIndentations.clear();
    }


    bool BlockFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<BlockFormat> blockFormat = formatInstance.dynamicCast<BlockFormat>();
        if (!blockFormat.isNull()) {
            success = Format::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentTopSpacings       << blockFormat->topSpacing();
                currentBottomSpacings    << blockFormat->bottomSpacing();
                currentLeftIndentations  << blockFormat->leftIndentation();
                currentRightIndentations << blockFormat->rightIndentation();
            }
        }

        return success;
    }


    QSet<float> BlockFormat::Aggregation::topSpacings() const {
        return currentTopSpacings;
    }


    QSet<float> BlockFormat::Aggregation::bottomSpacings() const {
        return currentBottomSpacings;
    }


    QSet<float> BlockFormat::Aggregation::leftIndentations() const {
        return currentLeftIndentations;
    }


    QSet<float> BlockFormat::Aggregation::rightIndentations() const {
        return currentRightIndentations;
    }
}

/***********************************************************************************************************************
 * Ld::BlockFormat
 */

namespace Ld {
    const QString BlockFormat::formatName("BlockFormat");
    const float   BlockFormat::defaultLeftIndentation = 0;
    const float   BlockFormat::defaultRightIndentation = 0;
    const float   BlockFormat::defaultTopSpacing = 6.0;
    const float   BlockFormat::defaultBottomSpacing = 6.0;

    BlockFormat::BlockFormat() {
        currentLeftIndentation  = defaultLeftIndentation;
        currentRightIndentation = defaultRightIndentation;
        currentTopSpacing       = defaultTopSpacing;
        currentBottomSpacing    = defaultBottomSpacing;
    }


    BlockFormat::BlockFormat(const BlockFormat& other):Format(other) {
        currentLeftIndentation  = other.currentLeftIndentation;
        currentRightIndentation = other.currentRightIndentation;
        currentTopSpacing       = other.currentTopSpacing;
        currentBottomSpacing    = other.currentBottomSpacing;
    }


    BlockFormat::~BlockFormat() {}


    FormatPointer BlockFormat::clone() const {
        return FormatPointer(new BlockFormat(*this));
    }


    Format* BlockFormat::creator(const QString&) {
        return new BlockFormat();
    }


    QString BlockFormat::typeName() const {
        return formatName;
    }


    bool BlockFormat::isValid() const {
        return (
               currentLeftIndentation >= 0
            && currentRightIndentation >= 0
            && currentTopSpacing >= 0
            && currentBottomSpacing >= 0
        );
    }


    Format::Capabilities BlockFormat::capabilities() const {
        return Format::capabilities() << formatName;
    }


    void BlockFormat::setLeftIndentation(float newLeftIndentationPoints) {
        currentLeftIndentation = newLeftIndentationPoints;
        reportFormatUpdated();
    }


    float BlockFormat::leftIndentation() const {
        return currentLeftIndentation;
    }


    void BlockFormat::setRightIndentation(float newRightIndentationPoints) {
        currentRightIndentation = newRightIndentationPoints;
        reportFormatUpdated();
    }


    float BlockFormat::rightIndentation() const {
        return currentRightIndentation;
    }


    void BlockFormat::setTopSpacing(float newTopSpacingPoints) {
        currentTopSpacing = newTopSpacingPoints;
        reportFormatUpdated();
    }


    float BlockFormat::topSpacing() const {
        return currentTopSpacing;
    }


    void BlockFormat::setBottomSpacing(float newBottomSpacingPoints) {
        currentBottomSpacing = newBottomSpacingPoints;
        reportFormatUpdated();
    }


    float BlockFormat::bottomSpacing() const {
        return currentBottomSpacing;
    }


    QString BlockFormat::toString() const {
        return QString("%1,%2,%3,%4,%5").arg(typeName())
                                         .arg(currentLeftIndentation)
                                         .arg(currentRightIndentation)
                                         .arg(currentTopSpacing)
                                         .arg(currentBottomSpacing);
    }


    QString BlockFormat::toCss() const {
        QString result;

        if (currentLeftIndentation != 0.0F) {
            result = QString("margin-left: %1pt; ").arg(currentLeftIndentation);
        }

        if (currentRightIndentation != 0.0F) {
            result += QString("margin-right: %1pt; ").arg(currentRightIndentation);
        }

        if (currentTopSpacing != 0.0F) {
            result += QString("margin-top: %1pt; ").arg(currentTopSpacing);
        }

        if (currentBottomSpacing != 0.0F) {
            result += QString("margin-bottom: %1pt; ").arg(currentBottomSpacing);
        }

        return result;
    }


    void BlockFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (leftIndentation() != defaultLeftIndentation) {
            attributes.append("left", leftIndentation());
        }

        if (rightIndentation() != defaultRightIndentation) {
            attributes.append("right", rightIndentation());

        }

        if (topSpacing() != defaultTopSpacing) {
            attributes.append("top", topSpacing());
        }

        if (bottomSpacing() != defaultBottomSpacing) {
            attributes.append("bottom", bottomSpacing());
        }
    }


    void BlockFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        bool ok;
        setLeftIndentation(attributes.value<float>("left", defaultLeftIndentation, &ok));
        if (!ok) {
            QString tag = reader->qualifiedName().toString();
            reader->raiseError(tr("Tag \"%1\", invalid/missing \"left\" attribute").arg(tag));
        }

        if (!reader->hasError()) {
            setRightIndentation(attributes.value<float>("right", defaultRightIndentation, &ok));
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid/missing \"right\" attribute").arg(tag));
            }
        }

        if (!reader->hasError()) {
            setTopSpacing(attributes.value<float>("top", defaultTopSpacing, &ok));
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid/missing \"top\" attribute").arg(tag));
            }
        }

        if (!reader->hasError()) {
            setBottomSpacing(attributes.value<float>("bottom", defaultBottomSpacing, &ok));
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid/missing \"bottom\" attribute").arg(tag));
            }
        }
    }


    void BlockFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        BlockFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void BlockFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        BlockFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }
}
