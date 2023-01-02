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
* This file implements the \ref Ld::ParagraphFormat class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>

#include "ld_format_structures.h"
#include "ld_format.h"
#include "ld_block_format.h"
#include "ld_justified_block_format.h"
#include "ld_text_block_format.h"
#include "ld_paragraph_format.h"

/***********************************************************************************************************************
 * Ld::ParagraphFormat::Aggregation
 */

namespace Ld {
    ParagraphFormat::Aggregation::Aggregation() {}


    ParagraphFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* ParagraphFormat::Aggregation::creator(const QString&) {
        return new ParagraphFormat::Aggregation;
    }


    QString ParagraphFormat::Aggregation::typeName() const {
        return ParagraphFormat::formatName;
    }
}

/***********************************************************************************************************************
 * Ld::ParagraphFormat
 */

namespace Ld {
    const QString                        ParagraphFormat::formatName("ParagraphFormat");
    const float                          ParagraphFormat::defaultLeftIndentation = 0;
    const float                          ParagraphFormat::defaultRightIndentation = 0;
    const float                          ParagraphFormat::defaultTopSpacing = 6;
    const float                          ParagraphFormat::defaultBottomSpacing = 6;
    const ParagraphFormat::Justification ParagraphFormat::defaultJustification = ParagraphFormat::Justification::LEFT;

    ParagraphFormat::ParagraphFormat() {
        setSupportedJustificationModes(
            Justification::LEFT,
            Justification::RIGHT,
            Justification::CENTER,
            Justification::JUSTIFY
        );

        setJustification(defaultJustification);
        setLeftIndentation(defaultLeftIndentation);
        setRightIndentation(defaultRightIndentation);
        setTopSpacing(defaultTopSpacing);
        setBottomSpacing(defaultBottomSpacing);
    }


    ParagraphFormat::ParagraphFormat(
            const ParagraphFormat& other
        ):Format(
            other
        ),BlockFormat(
            other
        ),JustifiedBlockFormat(
            other
        ),TextBlockFormat(
            other
        ) {}


    ParagraphFormat::~ParagraphFormat() {}


    FormatPointer ParagraphFormat::clone() const {
        return FormatPointer(new ParagraphFormat(*this));
    }


    Format* ParagraphFormat::creator(const QString&) {
        return new ParagraphFormat();
    }


    QString ParagraphFormat::typeName() const {
        return formatName;
    }


    Format::Capabilities ParagraphFormat::capabilities() const {
        return TextBlockFormat::capabilities() << formatName;
    }


    void ParagraphFormat::writeAddImmediateAttributes(
            XmlAttributes&                  /* attributes */,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {}


    void ParagraphFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  /* reader */,
            const XmlAttributes&       /* attributes */,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {}


    void ParagraphFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        BlockFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        JustifiedBlockFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        TextBlockFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        ParagraphFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void ParagraphFormat::readAttributes(
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
    }
}
