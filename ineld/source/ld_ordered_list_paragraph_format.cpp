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
* This file implements the \ref Ld::OrderedListParagraphFormat class.
***********************************************************************************************************************/

#include <QChar>
#include <QString>
#include <QSharedPointer>
#include <QSet>

#include <util_hash_functions.h>

#include "ld_format_structures.h"
#include "ld_format.h"
#include "ld_block_format.h"
#include "ld_justified_block_format.h"
#include "ld_text_block_format.h"
#include "ld_paragraph_format.h"
#include "ld_font_format.h"
#include "ld_list_paragraph_format_base.h"
#include "ld_ordered_list_paragraph_format.h"

/***********************************************************************************************************************
 * Ld::OrderedListParagraphFormat::Aggregation
 */

namespace Ld {
    OrderedListParagraphFormat::Aggregation::Aggregation() {}


    OrderedListParagraphFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* OrderedListParagraphFormat::Aggregation::creator(const QString&) {
        return new OrderedListParagraphFormat::Aggregation;
    }


    QString OrderedListParagraphFormat::Aggregation::typeName() const {
        return OrderedListParagraphFormat::formatName;
    }


    void OrderedListParagraphFormat::Aggregation::clear() {
        ListParagraphFormatBase::Aggregation::clear();
        currentStyles.clear();
        currentStartingNumbers.clear();
    }


    bool OrderedListParagraphFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<OrderedListParagraphFormat> format =
            formatInstance.dynamicCast<OrderedListParagraphFormat>();

        if (!format.isNull()) {
            success = ListParagraphFormatBase::Aggregation::addFormat(format, includeExisting);

            if (success) {
                currentStyles          << format->style();
                currentStartingNumbers << format->startingNumber();
                currentPrefixes        << format->prefix();
                currentSuffixes        << format->suffix();
            }
        }

        return success;
    }


    QSet<OrderedListParagraphFormat::Style> OrderedListParagraphFormat::Aggregation::styles() const {
        return currentStyles;
    }


    QSet<unsigned long> OrderedListParagraphFormat::Aggregation::startingNumbers() const {
        return currentStartingNumbers;
    }


    QSet<QString> OrderedListParagraphFormat::Aggregation::prefixes() const {
        return currentPrefixes;
    }


    QSet<QString> OrderedListParagraphFormat::Aggregation::suffixes() const {
        return currentSuffixes;
    }
}

/***********************************************************************************************************************
 * Ld::OrderedListParagraphFormat
 */

namespace Ld {
    const QString                            OrderedListParagraphFormat::formatName("OrderedListParagraphFormat");
    const OrderedListParagraphFormat::Style  OrderedListParagraphFormat::defaultNumberStyle =
                                                                             OrderedListParagraphFormat::Style::NUMBER;
    const QString                            OrderedListParagraphFormat::defaultPrefix = tr("");
    const QString                            OrderedListParagraphFormat::defaultSuffix = tr(".");

    OrderedListParagraphFormat::OrderedListParagraphFormat() {
        currentStyle          = defaultNumberStyle;
        currentStartingNumber = 1;
        currentPrefix         = defaultPrefix;
        currentSuffix         = defaultSuffix;
    }


    OrderedListParagraphFormat::OrderedListParagraphFormat(
            const OrderedListParagraphFormat& other
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
        ),ListParagraphFormatBase(
            other
        ) {
        currentStyle          = other.currentStyle;
        currentStartingNumber = other.currentStartingNumber;
        currentPrefix         = other.currentPrefix;
        currentSuffix         = other.currentSuffix;
    }


    OrderedListParagraphFormat::~OrderedListParagraphFormat() {}


    FormatPointer OrderedListParagraphFormat::clone() const {
        return FormatPointer(new OrderedListParagraphFormat(*this));
    }


    Format* OrderedListParagraphFormat::creator(const QString&) {
        return new OrderedListParagraphFormat();
    }


    QString OrderedListParagraphFormat::typeName() const {
        return formatName;
    }


    Format::Capabilities OrderedListParagraphFormat::capabilities() const {
        return ListParagraphFormatBase::capabilities() << formatName;
    }


    void OrderedListParagraphFormat::setStyle(OrderedListParagraphFormat::Style newStyle) {
        currentStyle = newStyle;
        reportFormatUpdated();
    }


    OrderedListParagraphFormat::Style OrderedListParagraphFormat::style() const {
        return currentStyle;
    }


    void OrderedListParagraphFormat::setStartingNumber(unsigned long newStartingNumber) {
        currentStartingNumber = newStartingNumber;
        reportFormatUpdated();
    }


    unsigned long OrderedListParagraphFormat::startingNumber() const {
        return currentStartingNumber;
    }

    void OrderedListParagraphFormat::setPrefix(const QString& newPrefix) {
        currentPrefix = newPrefix;
    }


    QString OrderedListParagraphFormat::prefix() const {
        return currentPrefix;
    }


    void OrderedListParagraphFormat::setSuffix(const QString& newSuffix) {
        currentSuffix = newSuffix;
    }


    QString OrderedListParagraphFormat::suffix() const {
        return currentSuffix;
    }


    QString OrderedListParagraphFormat::toString() const {
        return QString("%1,%2,%3").arg(ListParagraphFormatBase::toString())
                                  .arg(toString(currentStyle))
                                  .arg(currentStartingNumber);
    }


    QString OrderedListParagraphFormat::toCss() const {
        QString result = ListParagraphFormatBase::toCss();

        switch (currentStyle) {
            case Style::INVALID:          { result += "list-style-type: none; ";        break; }
            case Style::NUMBER:           { result += "list-style-type: decimal; ";     break; }
            case Style::LOWER_CASE:       { result += "list-style-type: lower-alpha: "; break; }
            case Style::UPPER_CASE:       { result += "list-style-type: upper-alpha; "; break; }
            case Style::ROMAN_LOWER_CASE: { result += "list-style-type: lower-roman; "; break; }
            case Style::ROMAN_UPPER_CASE: { result += "list-style-type: upper-roman; "; break; }
            default:                      {                                             break; }
        }

        return result;
    }


    void OrderedListParagraphFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (currentStyle != defaultNumberStyle) {
            attributes.append("style", toString(currentStyle));
        }

        if (currentStartingNumber != 0) {
            attributes.append("starting_value", currentStartingNumber);
        }

        if (currentPrefix != defaultPrefix) {
            attributes.append("prefix", currentPrefix);
        }

        if (currentSuffix != defaultSuffix) {
            attributes.append("suffix", currentSuffix);
        }
    }


    void OrderedListParagraphFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        if (!reader->hasError()) {
            if (attributes.hasAttribute("style")) {
                QString styleText = attributes.value<QString>("style");
                Style newStyle = toStyle(styleText);
                if (newStyle != Style::INVALID) {
                    setStyle(newStyle);
                } else {
                    QString tag = reader->qualifiedName().toString();
                    reader->raiseError(tr("Tag \"%1\" invalid style \"%2\"").arg(tag).arg(styleText));
                }
            } else {
                setStyle(defaultNumberStyle);
            }
        }

        if (!reader->hasError()) {
            if (attributes.hasAttribute("starting_value")) {
                bool ok;
                unsigned long newStartingValue = attributes.value<unsigned long>("starting_value", &ok);
                if (ok && newStartingValue != 0) {
                    setStartingNumber(newStartingValue);
                } else {
                    QString tag = reader->qualifiedName().toString();
                    QString value = attributes.value<QString>("starting_number");
                    reader->raiseError(tr("Tag \"%1\" invalid starting number \"%2\"").arg(tag).arg(value));
                }
            } else {
                setStartingNumber(0);
            }
        }

        if (!reader->hasError()) {
            if (attributes.hasAttribute("prefix")) {
                currentPrefix = attributes.value<QString>("prefix");
            } else {
                currentPrefix = defaultPrefix;
            }

            if (attributes.hasAttribute("suffix")) {
                currentSuffix = attributes.value<QString>("suffix");
            } else {
                currentSuffix = defaultSuffix;
            }
        }
    }


    void OrderedListParagraphFormat::writeAddAttributes(
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
        OrderedListParagraphFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void OrderedListParagraphFormat::readAttributes(
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
        OrderedListParagraphFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }


    QString OrderedListParagraphFormat::toString(OrderedListParagraphFormat::Style style) {
        QString result;

        switch (style) {
            case Style::INVALID: {
                result = QString("INVALID");
                break;
            }

            case Style::NUMBER: {
                result = QString("NUMBER");
                break;
            }

            case Style::LOWER_CASE: {
                result = QString("LOWER_CASE");
                break;
            }

            case Style::UPPER_CASE: {
                result = QString("UPPER_CASE");
                break;
            }

            case Style::ROMAN_UPPER_CASE: {
                result = QString("ROMAN_UPPER_CASE");
                break;
            }

            case Style::ROMAN_LOWER_CASE: {
                result = QString("ROMAN_LOWER_CASE");
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    OrderedListParagraphFormat::Style OrderedListParagraphFormat::toStyle(const QString& textualRepresentation) {
        Style result;

        if (textualRepresentation == QString("NUMBER")) {
            result = Style::NUMBER;
        } else if (textualRepresentation == QString("LOWER_CASE")) {
            result = Style::LOWER_CASE;
        } else if (textualRepresentation == QString("UPPER_CASE")) {
            result = Style::UPPER_CASE;
        } else if (textualRepresentation == QString("ROMAN_UPPER_CASE")) {
            result = Style::ROMAN_UPPER_CASE;
        } else if (textualRepresentation == QString("ROMAN_LOWER_CASE")) {
            result = Style::ROMAN_LOWER_CASE;
        } else {
            result = Style::INVALID;
        }

        return result;
    }


    Util::HashResult qHash(OrderedListParagraphFormat::Style value, Util::HashSeed seed) {
        return ::qHash(static_cast<unsigned>(value), seed);
    }
}
