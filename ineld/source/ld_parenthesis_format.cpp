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
* This file implements the \ref Ld::ParenthesisFormat class.
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
#include "ld_font_format.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_program_file.h"
#include "ld_format.h"
#include "ld_parenthesis_format.h"

/***********************************************************************************************************************
 * Ld::ParenthesisFormat::Aggregation:
 */

namespace Ld {
    ParenthesisFormat::Aggregation::Aggregation() {
        clear();
    }


    ParenthesisFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* ParenthesisFormat::Aggregation::creator(const QString&) {
        return new ParenthesisFormat::Aggregation;
    }


    QString ParenthesisFormat::Aggregation::typeName() const {
        return ParenthesisFormat::formatName;
    }


    void ParenthesisFormat::Aggregation::clear() {
        Format::Aggregation::clear();
        currentParenthesisStyles.clear();
    }


    bool ParenthesisFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<ParenthesisFormat> parenthesisFormat = formatInstance.dynamicCast<ParenthesisFormat>();
        if (!parenthesisFormat.isNull()) {
            success = FontFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentDefaultParenthesisStyleAllowed << parenthesisFormat->supportsDefaultParenthesisStyle();
                currentParenthesisStyles << parenthesisFormat->parenthesisStyle();
            }
        }

        return success;
    }


    const Ld::Format::BooleanAggregation& ParenthesisFormat::Aggregation::defaultParenthesisStyleAllowed() const {
        return currentDefaultParenthesisStyleAllowed;
    }


    const QSet<ParenthesisFormat::ParenthesisStyle>& ParenthesisFormat::Aggregation::parenthesisStyles() const {
        return currentParenthesisStyles;
    }
}

/***********************************************************************************************************************
 * Ld::ParenthesisFormat
 */

namespace Ld {
    const QString                          ParenthesisFormat::formatName("ParenthesisFormat");
    const ParenthesisFormat::ParenthesisStyle ParenthesisFormat::defaultParenthesisStyle =
        ParenthesisFormat::ParenthesisStyle::DEFAULT;

    ParenthesisFormat::ParenthesisFormat() {
        currentParenthesisStyle = defaultParenthesisStyle;
    }


    ParenthesisFormat::ParenthesisFormat(
            const ParenthesisFormat& other
        ):Format(
            other
        ),FontFormat(
            other
        ) {
        currentParenthesisStyle = other.currentParenthesisStyle;
    }


    ParenthesisFormat::~ParenthesisFormat() {}


    FormatPointer ParenthesisFormat::clone() const {
        return FormatPointer(new ParenthesisFormat(*this));
    }


    Format* ParenthesisFormat::creator(const QString&) {
        return new ParenthesisFormat();
    }


    QString ParenthesisFormat::typeName() const {
        return formatName;
    }


    bool ParenthesisFormat::isValid() const {
        return true;
    }


    Format::Capabilities ParenthesisFormat::capabilities() const {
        return FontFormat::capabilities() << formatName;
    }


    bool ParenthesisFormat::supportsDefaultParenthesisStyle() const {
        return true;
    }


    void ParenthesisFormat::setParenthesisStyle(ParenthesisStyle newParenthesisStyle) {
        if (!supportsDefaultParenthesisStyle() && newParenthesisStyle == ParenthesisStyle::DEFAULT) {
            currentParenthesisStyle = ParenthesisStyle::PARENTHESIS;
        } else {
            currentParenthesisStyle = newParenthesisStyle;
        }

        reportFormatUpdated();
    }


    ParenthesisFormat::ParenthesisStyle ParenthesisFormat::parenthesisStyle() const {
        return currentParenthesisStyle;
    }


    QString ParenthesisFormat::toString() const {
        QString style  = toString(currentParenthesisStyle);
        QString result = QString("%1,%2").arg(FontFormat::toString()).arg(style);
        return result;
    }


    QString ParenthesisFormat::toCss() const {
        return FontFormat::toCss();
    }


    QString ParenthesisFormat::toString(ParenthesisFormat::ParenthesisStyle parenthesisStyle) {
        QString style;

        switch (parenthesisStyle) {
            case ParenthesisStyle::DEFAULT:     { style = QString("DEFAULT");       break; }
            case ParenthesisStyle::PARENTHESIS: { style = QString("PARENTHESIS");   break; }
            case ParenthesisStyle::BRACKETS:    { style = QString("BRACKETS");     break; }
            case ParenthesisStyle::BRACES:      { style = QString("BRACES");        break; }
            default:                            { style = QString("PARENTHESIS");   break; }
        }

        return style;
    }


    ParenthesisFormat::ParenthesisStyle ParenthesisFormat::toParenthesisStyle(const QString& str, bool* ok) {
        ParenthesisStyle result;
        bool             isOk      = true;
        QString          upperCase = str.toUpper();

        if (upperCase == QString("DEFAULT")) {
            result = ParenthesisStyle::DEFAULT;
        } else if (upperCase == QString("PARENTHESIS")) {
            result = ParenthesisStyle::PARENTHESIS;
        } else if (upperCase == QString("BRACKETS")) {
            result = ParenthesisStyle::BRACKETS;
        } else if (upperCase == QString("BRACES")) {
            result = ParenthesisStyle::BRACES;
        } else {
            isOk   = false;
            result = ParenthesisStyle::INVALID_OR_NONE;
        }

        if (ok != nullptr) {
            *ok = isOk;
        }

        return result;
    }


    void ParenthesisFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (currentParenthesisStyle != defaultParenthesisStyle) {
            QString style = toString(currentParenthesisStyle).toLower();
            attributes.append("parenthesis_style", style);
        }
    }


    void ParenthesisFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        ParenthesisStyle parenthesisStyle  = defaultParenthesisStyle;

        if (!reader->hasError() && attributes.hasAttribute("parenthesis_style")) {
            QString parenthesisStyleText = attributes.value<QString>("parenthesis_style");
            bool    ok;

            parenthesisStyle = toParenthesisStyle(parenthesisStyleText, &ok);
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(
                    tr("Tag \"%1\" has invalid parenthesis style \"%2\"").arg(tag).arg(parenthesisStyleText)
                );
            }
        }

        if (!reader->hasError()) {
            setParenthesisStyle(parenthesisStyle);
        }
    }


    void ParenthesisFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        FontFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        ParenthesisFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void ParenthesisFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        FontFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        ParenthesisFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }


    Util::HashResult qHash(ParenthesisFormat::ParenthesisStyle parenthesisStyle, Util::HashSeed seed) {
        return ::qHash(static_cast<unsigned>(parenthesisStyle), seed);
    }
}
