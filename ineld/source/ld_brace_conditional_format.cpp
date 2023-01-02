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
* This file implements the \ref Ld::BraceConditionalFormat class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QString>
#include <QSharedPointer>
#include <QColor>
#include <QFont>
#include <QSet>
#include <QRegularExpression>

#include <cassert>

#include <util_hash_functions.h>

#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_program_file.h"
#include "ld_font_format.h"
#include "ld_brace_conditional_format.h"

/***********************************************************************************************************************
 * Ld::BraceConditionalFormat::Aggregation:
 */

namespace Ld {
    BraceConditionalFormat::Aggregation::Aggregation() {
        clear();
    }


    BraceConditionalFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* BraceConditionalFormat::Aggregation::creator(const QString&) {
        return new BraceConditionalFormat::Aggregation;
    }


    QString BraceConditionalFormat::Aggregation::typeName() const {
        return BraceConditionalFormat::formatName;
    }


    void BraceConditionalFormat::Aggregation::clear() {
        FontFormat::Aggregation::clear();

        currentConditionPreambleStrings.clear();
        currentElseConditionStrings.clear();
    }


    bool BraceConditionalFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<BraceConditionalFormat>
            braceConditionalFormat = formatInstance.dynamicCast<BraceConditionalFormat>();

        if (!braceConditionalFormat.isNull()) {
            success = FontFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentConditionPreambleStrings << braceConditionalFormat->currentConditionPreambleString;
                currentElseConditionStrings     << braceConditionalFormat->currentElseConditionString;
            }
        }

        return success;
    }


    const QSet<QString>& BraceConditionalFormat::Aggregation::conditionPreambleStrings() const {
        return currentConditionPreambleStrings;
    }


    const QSet<QString>& BraceConditionalFormat::Aggregation::elseConditionStrings() const {
        return currentElseConditionStrings;
    }
}

/***********************************************************************************************************************
 * Ld::BraceConditionalFormat
 */

namespace Ld {
    const QString                          BraceConditionalFormat::formatName("BraceConditionalFormat");
    const QString                          BraceConditionalFormat::defaultConditionPreambleString("if ");
    const QString                          BraceConditionalFormat::defaultElseConditionString("otherwise");
    QSharedPointer<BraceConditionalFormat> BraceConditionalFormat::currentApplicationGlobalBraceConditionFormat;
    bool                                   BraceConditionalFormat::currentApplicationGlobalShowElseClause = true;

    QRegularExpression BraceConditionalFormat::textSplitRegularExpression("[^\\],");

    void BraceConditionalFormat::setApplicationDefaultBraceConditionalFormat(
            QSharedPointer<BraceConditionalFormat> newGlobalFormat
        ) {
        currentApplicationGlobalBraceConditionFormat = newGlobalFormat;
    }


    QSharedPointer<BraceConditionalFormat> BraceConditionalFormat::applicationDefaultBraceConditionalFormat() {
        if (currentApplicationGlobalBraceConditionFormat.isNull()) {
            currentApplicationGlobalBraceConditionFormat.reset(new BraceConditionalFormat);
        }

        return currentApplicationGlobalBraceConditionFormat;
    }


    void BraceConditionalFormat::setApplicationDefaultShowElseClause(bool nowShowElseClause) {
        currentApplicationGlobalShowElseClause = nowShowElseClause;
    }


    bool BraceConditionalFormat::applicationDefaultShowElseClause() {
        return currentApplicationGlobalShowElseClause;
    }


    BraceConditionalFormat::BraceConditionalFormat() {
        currentConditionPreambleString = tr("if ");
        currentElseConditionString     = tr("otherwise");
    }


    BraceConditionalFormat::BraceConditionalFormat(
            const BraceConditionalFormat& other
        ):Format(
            other
        ),FontFormat(
            other
        ) {
        currentConditionPreambleString = other.currentConditionPreambleString;
        currentElseConditionString     = other.currentElseConditionString;
    }


    BraceConditionalFormat::~BraceConditionalFormat() {}


    FormatPointer BraceConditionalFormat::clone() const {
        return FormatPointer(new BraceConditionalFormat(*this));
    }


    Format* BraceConditionalFormat::creator(const QString&) {
        return new BraceConditionalFormat();
    }


    QString BraceConditionalFormat::typeName() const {
        return formatName;
    }


    bool BraceConditionalFormat::isValid() const {
        return true;
    }


    Format::Capabilities BraceConditionalFormat::capabilities() const {
        return FontFormat::capabilities() << formatName;
    }


    void BraceConditionalFormat::setConditionPreambleString(const QString& newConditionPreambleString) {
        currentConditionPreambleString = newConditionPreambleString;
        reportFormatUpdated();
    }


    QString BraceConditionalFormat::conditionPreambleString() const {
        return currentConditionPreambleString;
    }


    void BraceConditionalFormat::setElseConditionString(const QString& newElseConditionString) {
        currentElseConditionString = newElseConditionString;
        reportFormatUpdated();
    }


    QString BraceConditionalFormat::elseConditionString() const {
        return currentElseConditionString;
    }


    QString BraceConditionalFormat::toString() const {
        QString formatString                   = FontFormat::toString();
        QString escapedConditionPreambleString = currentConditionPreambleString;
        QString escapedElseConditionString     = currentElseConditionString;

        escapedConditionPreambleString.replace(QChar('\\'), QString("\\\\"));
        escapedConditionPreambleString.replace(QChar(','), QString("\\,"));

        escapedElseConditionString.replace(QChar('\\'), QString("\\\\"));
        escapedElseConditionString.replace(QChar(','), QString("\\,"));

        return QString("%1,%2,%3")
               .arg(formatString)
               .arg(escapedConditionPreambleString)
               .arg(escapedElseConditionString);
    }


    QString BraceConditionalFormat::toCss() const {
        return FontFormat::toCss();
    }


    BraceConditionalFormat BraceConditionalFormat::fromString(const QString& identifier, bool* ok) {
        BraceConditionalFormat result;

        QList<QString> sections;
        unsigned long  stringLength  = identifier.length();
        bool           lastWasEscape = false;
        QString        currentField;
        for (unsigned long i=0 ; i<stringLength ; ++i) {
            QChar c = identifier.at(i);
            if (c == QChar('\\')) {
                lastWasEscape = true;
            } else {
                if (lastWasEscape) {
                    currentField += c;
                    lastWasEscape = false;
                } else {
                    if (c == QChar(',')) {
                        sections.append(currentField);
                        currentField.clear();
                    } else {
                        currentField += c;
                    }
                }
            }
        }

        sections.append(currentField);
        bool isOk = sections.size() == 10;

        if (isOk) {
            QString typeNameString        = sections.at(0);
            QString familyString          = sections.at(1);
            QString fontSizeString        = sections.at(2);
            QString fontWeightString      = sections.at(3);
            QString modifiersString       = sections.at(4);
            QString spacingString         = sections.at(5);
            QString fontColorString       = sections.at(6);
            QString backgroundColorString = sections.at(7);
            QString preambleString        = sections.at(8);
            QString elseString            = sections.at(9);

            unsigned fontSize      = defaultFontSizePoints;
            Weight   fontWeight    = Weight::NORMAL;
            bool     italics       = false;
            bool     underline     = false;
            bool     overline      = false;
            bool     strikeout     = false;
            float    letterSpacing = 0.0F;
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
                result.setConditionPreambleString(preambleString);
                result.setElseConditionString(elseString);
            }
        }

        if (ok != nullptr) {
            *ok = isOk;
        }

        return result;
    }


    void BraceConditionalFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (currentConditionPreambleString != defaultConditionPreambleString) {
            attributes.append("condition_preamble", currentConditionPreambleString);
        }

        if (currentElseConditionString != defaultElseConditionString) {
            attributes.append("else_condition", currentElseConditionString);
        }
    }


    void BraceConditionalFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        if (!reader->hasError()) {
            currentConditionPreambleString = attributes.value<QString>(
                "condition_preamble",
                defaultConditionPreambleString
            );
            currentElseConditionString = attributes.value<QString>("else_condition", defaultElseConditionString);
        }
    }


    void BraceConditionalFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        FontFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        BraceConditionalFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void BraceConditionalFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        FontFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        BraceConditionalFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }
}
