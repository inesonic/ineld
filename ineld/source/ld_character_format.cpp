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
* This file implements the \ref Ld::CharacterFormat class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QString>
#include <QSharedPointer>
#include <QColor>
#include <QFont>
#include <QSet>

#include <cassert>

#include <util_hash_functions.h>

#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_program_file.h"
#include "ld_character_format.h"

/***********************************************************************************************************************
 * Ld::CharacterFormat::Aggregation:
 */

namespace Ld {
    CharacterFormat::Aggregation::Aggregation() {
        clear();
    }


    CharacterFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* CharacterFormat::Aggregation::creator(const QString&) {
        return new CharacterFormat::Aggregation;
    }


    QString CharacterFormat::Aggregation::typeName() const {
        return CharacterFormat::formatName;
    }


    void CharacterFormat::Aggregation::clear() {
        FontFormat::Aggregation::clear();
        currentPositions.clear();
    }


    bool CharacterFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<CharacterFormat> characterFormat = formatInstance.dynamicCast<CharacterFormat>();
        if (!characterFormat.isNull()) {
            success = FontFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentPositions << characterFormat->position();
            }
        }

        return success;
    }


    QSet<CharacterFormat::Position> CharacterFormat::Aggregation::positions() const {
        return currentPositions;
    }
}

/***********************************************************************************************************************
 * Ld::CharacterFormat
 */

namespace Ld {
    const QString                   CharacterFormat::formatName("CharacterFormat");
    const CharacterFormat::Position CharacterFormat::defaultFontPosition = CharacterFormat::Position::NORMAL;
    QSharedPointer<CharacterFormat> CharacterFormat::currentApplicationGlobalTextFont;
    QSharedPointer<CharacterFormat> CharacterFormat::currentApplicationGlobalMathFont;
    QSharedPointer<CharacterFormat> CharacterFormat::currentApplicationGlobalMathIdentifierFont;
    QSharedPointer<CharacterFormat> CharacterFormat::currentApplicationGlobalMathFunctionFont;

    void CharacterFormat::setApplicationDefaultTextFont(QSharedPointer<CharacterFormat> newGlobalFont) {
        currentApplicationGlobalTextFont = newGlobalFont;
    }


    QSharedPointer<CharacterFormat> CharacterFormat::applicationDefaultTextFont() {
        if (currentApplicationGlobalTextFont.isNull()) {
            currentApplicationGlobalTextFont.reset(new CharacterFormat);
        }

        return currentApplicationGlobalTextFont;
    }


    void CharacterFormat::setApplicationDefaultMathFont(QSharedPointer<CharacterFormat> newGlobalFont) {
        currentApplicationGlobalMathFont = newGlobalFont;
    }


    QSharedPointer<CharacterFormat> CharacterFormat::applicationDefaultMathFont() {
        if (currentApplicationGlobalMathFont.isNull()) {
            currentApplicationGlobalMathFont.reset(new CharacterFormat);
        }

        return currentApplicationGlobalMathFont;
    }


    void CharacterFormat::setApplicationDefaultMathIdentifierFont(QSharedPointer<CharacterFormat> newGlobalFont) {
        currentApplicationGlobalMathIdentifierFont = newGlobalFont;
    }


    QSharedPointer<CharacterFormat> CharacterFormat::applicationDefaultMathIdentifierFont() {
        if (currentApplicationGlobalMathIdentifierFont.isNull()) {
            currentApplicationGlobalMathIdentifierFont.reset(new CharacterFormat);
        }

        return currentApplicationGlobalMathIdentifierFont;
    }


    void CharacterFormat::setApplicationDefaultMathFunctionFont(QSharedPointer<CharacterFormat> newGlobalFont) {
        currentApplicationGlobalMathFunctionFont = newGlobalFont;
    }


    QSharedPointer<CharacterFormat> CharacterFormat::applicationDefaultMathFunctionFont() {
        if (currentApplicationGlobalMathFunctionFont.isNull()) {
            currentApplicationGlobalMathFunctionFont.reset(new CharacterFormat);
        }

        return currentApplicationGlobalMathFunctionFont;
    }


    CharacterFormat::CharacterFormat() {
        configure();
    }


    CharacterFormat::CharacterFormat(
            const QString&     family,
            unsigned           pointSize,
            FontFormat::Weight weight,
            bool               italic
        ):FontFormat(
            family,
            pointSize,
            weight,
            italic
        ) {
        configure();
    }


    CharacterFormat::CharacterFormat(
            const char*        family,
            unsigned           pointSize,
            FontFormat::Weight weight,
            bool               italic
        ):FontFormat(
            family,
            pointSize,
            weight,
            italic
        ) {
        configure();
    }


    CharacterFormat::CharacterFormat(const CharacterFormat& other):Format(other),FontFormat(other) {
        currentPosition = other.currentPosition;
    }


    CharacterFormat::CharacterFormat(const FontFormat& other):Format(other),FontFormat(other) {
        currentPosition = defaultFontPosition;
    }


    CharacterFormat::~CharacterFormat() {}


    FormatPointer CharacterFormat::clone() const {
        return FormatPointer(new CharacterFormat(*this));
    }


    Format* CharacterFormat::creator(const QString&) {
        return new CharacterFormat();
    }


    QString CharacterFormat::typeName() const {
        return formatName;
    }


    bool CharacterFormat::isValid() const {
        return true;
    }


    Format::Capabilities CharacterFormat::capabilities() const {
        return FontFormat::capabilities() << formatName;
    }


    void CharacterFormat::setPosition(CharacterFormat::Position newPosition) {
        currentPosition = newPosition;
        reportFormatUpdated();
    }


    CharacterFormat::Position CharacterFormat::position() const {
        return currentPosition;
    }


    QString CharacterFormat::toString() const {
        QString position;

        switch (currentPosition) {
            case Position::NORMAL: {
                position = QString("NORMAL");
                break;
            }

            case Position::SUBSCRIPT: {
                position = QString("SUBSCRIPT");
                break;
            }

            case Position::SUPERSCRIPT: {
                position = QString("SUPERSCRIPT");
                break;
            }

            default: {
                position = QString::number(static_cast<quint8>(currentPosition));
                break;
            }
        }

        QString result = QString("%1,%2").arg(FontFormat::toString()).arg(position);
        return result;
    }


    QString CharacterFormat::toCss() const {
        QString result = FontFormat::toCss();

        switch (currentPosition) {
            case Position::NORMAL:      {                                      break; }
            case Position::SUBSCRIPT:   { result += "vertical-align: sub; ";   break; }
            case Position::SUPERSCRIPT: { result += "vertical-align: super; "; break; }
            default:                    {                                      break; }
        }

        return result;
    }


    CharacterFormat CharacterFormat::fromString(const QString& identifier, bool* ok) {
        CharacterFormat result;

        QList<QString> sections = identifier.split(QChar(','));
        bool           isOk     = sections.size() == 9;

        if (isOk) {
            QString typeNameString        = sections.at(0);
            QString familyString          = sections.at(1);
            QString fontSizeString        = sections.at(2);
            QString fontWeightString      = sections.at(3);
            QString modifiersString       = sections.at(4);
            QString spacingString         = sections.at(5);
            QString fontColorString       = sections.at(6);
            QString backgroundColorString = sections.at(7);
            QString positionString        = sections.at(8);

            unsigned fontSize      = defaultFontSizePoints;
            Weight   fontWeight    = Weight::NORMAL;
            bool     italics       = false;
            bool     underline     = false;
            bool     overline      = false;
            bool     strikeout     = false;
            float    letterSpacing = 0.0F;
            Position position      = Position::NORMAL;
            QColor   fontColor;
            QColor   backgroundColor;

            isOk = typeNameString == formatName && !familyString.isEmpty();
            if (isOk) {
                fontSize = fontSizeString.toUInt(&isOk);
                isOk     = isOk && fontSize >= minimumFontSize;
            }

            if (isOk) {
                int fontWeightValue = static_cast<int>(fontWeightString.toFloat(&isOk) * 100.0 + 0.5);
                isOk = (
                       isOk
                    && fontWeightValue >= static_cast<int>(Weight::THIN)
                    && fontWeightValue <= static_cast<int>(Weight::BLACK)
                );

                if (isOk) {
                    fontWeight = static_cast<Weight>(fontWeightValue);
                }
            }

            if (isOk) {
                italics   = modifiersString.contains(QChar('I'));
                underline = modifiersString.contains(QChar('U'));
                overline  = modifiersString.contains(QChar('O'));
                strikeout = modifiersString.contains(QChar('S'));

                letterSpacing = spacingString.toFloat(&isOk);
                isOk = isOk && letterSpacing >= minimumLetterSpacing && letterSpacing <= maximumLetterSpacing;
            }

            if (isOk) {
                fontColor       = QColor(fontColorString);
                backgroundColor = QColor(backgroundColorString);

                if (positionString == "NORMAL") {
                    position = Position::NORMAL;
                } else if (positionString == "SUBSCRIPT") {
                    position = Position::SUBSCRIPT;
                } else if (positionString == "SUPERSCRIPT") {
                    position = Position::SUPERSCRIPT;
                } else {
                    isOk = false;
                }
            }

            if (isOk) {
                result.setFamily(familyString);
                result.setFontSize(fontSize);
                result.setFontWeight(fontWeight);
                result.setItalics(italics);
                result.setUnderline(underline);
                result.setOverline(overline);
                result.setStrikeout(strikeout);
                result.setLetterSpacing(letterSpacing);
                result.setFontColor(fontColor);
                result.setFontBackgroundColor(backgroundColor);
                result.setPosition(position);
            }
        }

        if (ok != nullptr) {
            *ok = isOk;
        }

        return result;
    }


    void CharacterFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (position() != defaultFontPosition) {
            QString positionText;

            switch (position()) {
                case Position::NORMAL:      { positionText = QString("normal");      break; }
                case Position::SUBSCRIPT:   { positionText = QString("subscript");   break; }
                case Position::SUPERSCRIPT: { positionText = QString("superscript"); break; }
                default: {
                    positionText = QString("invalid");
                    assert(false);
                    break;
                }
            }

            attributes.append("position", positionText);
        }
    }


    void CharacterFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        if (!reader->hasError()) {
            Position fontPosition = defaultFontPosition;
            if (attributes.hasAttribute("position")) {
                QString positionText = attributes.value<QString>("position");

                if (positionText == "normal") {
                    fontPosition = Position::NORMAL;
                } else if (positionText == "subscript") {
                    fontPosition = Position::SUBSCRIPT;
                } else if (positionText == "superscript") {
                    fontPosition = Position::SUPERSCRIPT;
                } else {
                    QString tag = reader->qualifiedName().toString();
                    reader->raiseError(tr("Tag \"%1\" has invalid position \"%2\"").arg(tag).arg(positionText));
                }
            }

            if (!reader->hasError()) {
                setPosition(fontPosition);
            }
        }
    }


    void CharacterFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        FontFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        CharacterFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void CharacterFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        FontFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        CharacterFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }


    void CharacterFormat::configure() {
        currentPosition = defaultFontPosition;
    }


    Util::HashResult qHash(CharacterFormat::Position position, Util::HashSeed seed) {
        return ::qHash(static_cast<unsigned>(position), seed);
    }
}
