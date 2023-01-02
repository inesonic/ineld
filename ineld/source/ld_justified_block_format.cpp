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
* This file implements the \ref Ld::JustifiedBlockFormat class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>
#include <QSet>

#include <cassert>

#include <util_hash_functions.h>

#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_format.h"
#include "ld_block_format.h"
#include "ld_justified_block_format.h"

/***********************************************************************************************************************
 * Ld::JustifiedBlockFormat::Aggregation
 */

namespace Ld {
    JustifiedBlockFormat::Aggregation::Aggregation() {
        clear();
    }


    JustifiedBlockFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* JustifiedBlockFormat::Aggregation::creator(const QString&) {
        return new JustifiedBlockFormat::Aggregation;
    }


    QString JustifiedBlockFormat::Aggregation::typeName() const {
        return JustifiedBlockFormat::formatName;
    }


    void JustifiedBlockFormat::Aggregation::clear() {
        BlockFormat::Aggregation::clear();

        currentJustifications.clear();
        currentSupportedJustifications.clear();

        currentFullySupportedJustifications.clear();
        currentFullySupportedJustifications.insert(Justification::CENTER);
        currentFullySupportedJustifications.insert(Justification::LEFT);
        currentFullySupportedJustifications.insert(Justification::RIGHT);
        currentFullySupportedJustifications.insert(Justification::JUSTIFY);
    }


    bool JustifiedBlockFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<JustifiedBlockFormat> blockFormat = formatInstance.dynamicCast<JustifiedBlockFormat>();
        if (!blockFormat.isNull()) {
            success = BlockFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentJustifications << blockFormat->justificationMode();

                if (blockFormat->supportsJustificationMode(Justification::CENTER)) {
                    currentSupportedJustifications.insert(Justification::CENTER);
                } else {
                    currentFullySupportedJustifications.remove(Justification::CENTER);
                }

                if (blockFormat->supportsJustificationMode(Justification::LEFT)) {
                    currentSupportedJustifications.insert(Justification::LEFT);
                } else {
                    currentFullySupportedJustifications.remove(Justification::LEFT);
                }

                if (blockFormat->supportsJustificationMode(Justification::RIGHT)) {
                    currentSupportedJustifications.insert(Justification::RIGHT);
                } else {
                    currentFullySupportedJustifications.remove(Justification::RIGHT);
                }

                if (blockFormat->supportsJustificationMode(Justification::JUSTIFY)) {
                    currentSupportedJustifications.insert(Justification::JUSTIFY);
                } else {
                    currentFullySupportedJustifications.remove(Justification::JUSTIFY);
                }
            }
        }

        return success;
    }


    QSet<JustifiedBlockFormat::Justification> JustifiedBlockFormat::Aggregation::justifications() const {
        return currentJustifications;
    }


    QSet<JustifiedBlockFormat::Justification> JustifiedBlockFormat::Aggregation::supportedJustifications() const {
        return currentSupportedJustifications;
    }


    QSet<JustifiedBlockFormat::Justification> JustifiedBlockFormat::Aggregation::fullySupportedJustifications() const {
        QSet<JustifiedBlockFormat::Justification> justifications;

        if (!currentSupportedJustifications.isEmpty()) {
            justifications = currentFullySupportedJustifications;
        }

        return justifications;
    }
}

/***********************************************************************************************************************
 * Ld::JustifiedBlockFormat
 */

namespace Ld {
    const QString JustifiedBlockFormat::formatName("JustifiedBlockFormat");

    const JustifiedBlockFormat::Justification JustifiedBlockFormat::defaultJustification =
                                              JustifiedBlockFormat::Justification::LEFT;

    JustifiedBlockFormat::JustifiedBlockFormat() {
        currentJustification = defaultJustification;

        setSupportedJustificationModes(
            Justification::LEFT,
            Justification::RIGHT,
            Justification::CENTER
        );
    }


    JustifiedBlockFormat::JustifiedBlockFormat(
            const JustifiedBlockFormat& other
        ):Format(
            other
        ),BlockFormat(
            other
        ) {
        currentJustification               = other.currentJustification;
        currentSupportedJustificationModes = other.currentSupportedJustificationModes;
    }


    JustifiedBlockFormat::~JustifiedBlockFormat() {}


    FormatPointer JustifiedBlockFormat::clone() const {
        return FormatPointer(new JustifiedBlockFormat(*this));
    }


    Format* JustifiedBlockFormat::creator(const QString&) {
        return new JustifiedBlockFormat();
    }


    QString JustifiedBlockFormat::typeName() const {
        return formatName;
    }


    bool JustifiedBlockFormat::isValid() const {
        return BlockFormat::isValid() && currentJustification != Justification::NONE;
    }


    Format::Capabilities JustifiedBlockFormat::capabilities() const {
        return BlockFormat::capabilities() << formatName;
    }


    bool JustifiedBlockFormat::supportsJustificationMode(JustifiedBlockFormat::Justification mode) const {
        return (static_cast<std::uint8_t>(mode) & currentSupportedJustificationModes) != 0;
    }


    bool JustifiedBlockFormat::setJustification(JustifiedBlockFormat::Justification newJustificationMode) {
        bool success;

        if ((static_cast<std::uint8_t>(newJustificationMode) & currentSupportedJustificationModes) != 0) {
            currentJustification = newJustificationMode;
            success = true;

            reportFormatUpdated();
        } else {
            success = false;
        }

        return success;
    }


    JustifiedBlockFormat::Justification JustifiedBlockFormat::justificationMode() const {
        return currentJustification;
    }


    QString JustifiedBlockFormat::toString() const {
        QString mode;

        switch (currentJustification) {
            case Justification::NONE:    { mode = QString("NONE");    break; }
            case Justification::LEFT:    { mode = QString("LEFT");    break; }
            case Justification::RIGHT:   { mode = QString("RIGHT");   break; }
            case Justification::CENTER:  { mode = QString("CENTER");  break; }
            case Justification::JUSTIFY: { mode = QString("JUSTIFY"); break; }
            default:                     { mode = QString("UNKNOWN"); break; }
        }

        return QString("%1,%2").arg(BlockFormat::toString()).arg(mode);
    }


    QString JustifiedBlockFormat::toCss() const {
        QString result = BlockFormat::toCss();

        switch (currentJustification) {
            case Justification::NONE:
            case Justification::LEFT:    {                                    break; }
            case Justification::RIGHT:   { result += "text-align: right; ";   break; }
            case Justification::CENTER:  { result += "text-align: center; ";  break; }
            case Justification::JUSTIFY: { result += "text-align: justify; "; break; }
            default:                     {                                    break; }
        }

        return result;
    }


    void JustifiedBlockFormat::setSupportedJustificationModes(Justification justification) {
        currentSupportedJustificationModes = static_cast<std::uint8_t>(justification);
    }


    void JustifiedBlockFormat::setSupportedJustificationModes(
            Justification justification1,
            Justification justification2
        ) {
        currentSupportedJustificationModes = (
              static_cast<std::uint8_t>(justification1)
            | static_cast<std::uint8_t>(justification2)
        );
    }


    void JustifiedBlockFormat::setSupportedJustificationModes(
            Justification justification1,
            Justification justification2,
            Justification justification3
        ) {
        currentSupportedJustificationModes = (
              static_cast<std::uint8_t>(justification1)
            | static_cast<std::uint8_t>(justification2)
            | static_cast<std::uint8_t>(justification3)
        );
    }


    void JustifiedBlockFormat::setSupportedJustificationModes(
            Justification justification1,
            Justification justification2,
            Justification justification3,
            Justification justification4
        ) {
        currentSupportedJustificationModes = (
              static_cast<std::uint8_t>(justification1)
            | static_cast<std::uint8_t>(justification2)
            | static_cast<std::uint8_t>(justification3)
            | static_cast<std::uint8_t>(justification4)
        );
    }


    void JustifiedBlockFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (justificationMode() != defaultJustification) {
            QString justificationText;

            switch(justificationMode()) {
                case Justification::NONE:    { justificationText = QString("none");     break; }
                case Justification::LEFT:    { justificationText = QString("left");     break; }
                case Justification::RIGHT:   { justificationText = QString("right");    break; }
                case Justification::CENTER:  { justificationText = QString("centered"); break; }
                case Justification::JUSTIFY: { justificationText = QString("filled");   break; }
                default: {
                    justificationText = QString("invalid");
                    assert(false);
                    break;
                }
            }

            attributes.append("justification", justificationText);
        }
    }


    void JustifiedBlockFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        if (!reader->hasError()) {
            Justification justification;
            if (attributes.hasAttribute("justification")) {
                QString justificationText = attributes.value<QString>("justification");

                if (justificationText == "none") {
                    justification = Justification::NONE;
                } else if (justificationText == "left") {
                    justification = Justification::LEFT;
                } else if (justificationText == "right") {
                    justification = Justification::RIGHT;
                } else if (justificationText == "centered") {
                    justification = Justification::CENTER;
                } else if (justificationText == "filled") {
                    justification = Justification::JUSTIFY;
                } else {
                    justification = Justification::NONE;
                    QString tag = reader->qualifiedName().toString();
                    reader->raiseError(tr("Tag \"%1\" invalid justification \"%2\"").arg(tag).arg(justificationText));
                }
            } else {
                justification = defaultJustification;
            }

            if (!reader->hasError()) {
                setJustification(justification);
            }
        }
    }


    void JustifiedBlockFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        BlockFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        JustifiedBlockFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void JustifiedBlockFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        BlockFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        JustifiedBlockFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }


    Util::HashResult qHash(JustifiedBlockFormat::Justification value, Util::HashSeed seed) {
        return ::qHash(static_cast<std::uint8_t>(value), seed);
    }
}
