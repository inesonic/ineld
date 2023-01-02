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
* This file implements the \ref Ld::MultiplicationOperatorFormat class.
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
#include "ld_operator_format.h"
#include "ld_multiplication_operator_format.h"

/***********************************************************************************************************************
 * Ld::MultiplicationOperatorFormat::Aggregation:
 */

namespace Ld {
    MultiplicationOperatorFormat::Aggregation::Aggregation() {
        clear();
    }


    MultiplicationOperatorFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* MultiplicationOperatorFormat::Aggregation::creator(const QString&) {
        return new MultiplicationOperatorFormat::Aggregation;
    }


    QString MultiplicationOperatorFormat::Aggregation::typeName() const {
        return MultiplicationOperatorFormat::formatName;
    }


    void MultiplicationOperatorFormat::Aggregation::clear() {
        OperatorFormat::Aggregation::clear();
        currentMultiplicationSymbols.clear();
    }


    bool MultiplicationOperatorFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<MultiplicationOperatorFormat> multiplicationOperatorFormat =
            formatInstance.dynamicCast<MultiplicationOperatorFormat>();

        if (!multiplicationOperatorFormat.isNull()) {
            success = OperatorFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentMultiplicationSymbols << multiplicationOperatorFormat->multiplicationSymbol();
            }
        }

        return success;
    }


    QSet<MultiplicationOperatorFormat::MultiplicationSymbol>
        MultiplicationOperatorFormat::Aggregation::multiplicationSymbols() const {
        return currentMultiplicationSymbols;
    }
}

/***********************************************************************************************************************
 * Ld::MultiplicationOperatorFormat
 */

namespace Ld {
    const QString MultiplicationOperatorFormat::formatName("MultiplicationOperatorFormat");
    const MultiplicationOperatorFormat::MultiplicationSymbol MultiplicationOperatorFormat::defaultMultiplicationSymbol =
        MultiplicationOperatorFormat::MultiplicationSymbol::DEFAULT;

    MultiplicationOperatorFormat::MultiplicationOperatorFormat() {
        currentMultiplicationSymbol = defaultMultiplicationSymbol;
    }


    MultiplicationOperatorFormat::MultiplicationOperatorFormat(
            const MultiplicationOperatorFormat& other
        ):Format(
            other
        ),FontFormat(
            other
        ),ParenthesisFormat(
            other
        ),OperatorFormat(
            other
        ) {
        currentMultiplicationSymbol = other.currentMultiplicationSymbol;
    }


    MultiplicationOperatorFormat::~MultiplicationOperatorFormat() {}


    FormatPointer MultiplicationOperatorFormat::clone() const {
        return FormatPointer(new MultiplicationOperatorFormat(*this));
    }


    Format* MultiplicationOperatorFormat::creator(const QString&) {
        return new MultiplicationOperatorFormat();
    }


    QString MultiplicationOperatorFormat::typeName() const {
        return formatName;
    }


    bool MultiplicationOperatorFormat::isValid() const {
        return true;
    }


    Format::Capabilities MultiplicationOperatorFormat::capabilities() const {
        return OperatorFormat::capabilities() << formatName;
    }


    void MultiplicationOperatorFormat::setMultiplicationSymbol(
            MultiplicationOperatorFormat::MultiplicationSymbol newMultiplicationSymbol
        ) {
        currentMultiplicationSymbol = newMultiplicationSymbol;
        reportFormatUpdated();
    }


    MultiplicationOperatorFormat::MultiplicationSymbol MultiplicationOperatorFormat::multiplicationSymbol() const {
        return currentMultiplicationSymbol;
    }


    QString MultiplicationOperatorFormat::toString() const {
        QString symbol;

        switch (currentMultiplicationSymbol) {
            case MultiplicationSymbol::DEFAULT: { symbol = QString("DEFAULT");  break; }
            case MultiplicationSymbol::SPACE:   { symbol = QString("SPACE");    break; }
            case MultiplicationSymbol::DOT:     { symbol = QString("DOT");      break; }
            case MultiplicationSymbol::CROSS:   { symbol = QString("CROSS");    break; }
            case MultiplicationSymbol::CIRCLE:  { symbol = QString("CIRCLE");   break; }
            default:                            { symbol = QString("DEFAULT");  break; }
        }

        QString result = QString("%1,%2").arg(OperatorFormat::toString()).arg(symbol);
        return result;
    }


    QString MultiplicationOperatorFormat::toCss() const {
        QString result = OperatorFormat::toCss();
        return result;
    }


    void MultiplicationOperatorFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        QString symbol;

        switch (currentMultiplicationSymbol) {
            case MultiplicationSymbol::DEFAULT: { symbol = QString("DEFAULT");  break; }
            case MultiplicationSymbol::SPACE:   { symbol = QString("SPACE");    break; }
            case MultiplicationSymbol::DOT:     { symbol = QString("DOT");      break; }
            case MultiplicationSymbol::CROSS:   { symbol = QString("CROSS");    break; }
            case MultiplicationSymbol::CIRCLE:  { symbol = QString("CIRCLE");   break; }

            default: {
                assert(false);
                break;
            }
        }

        attributes.append("multiplication_symbol", symbol);
    }


    void MultiplicationOperatorFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        if (!reader->hasError()) {
            MultiplicationSymbol multiplicationSymbol = defaultMultiplicationSymbol;

            if (attributes.hasAttribute("multiplication_symbol")) {
                QString symbolString = attributes.value<QString>("multiplication_symbol").toLower();

                if (symbolString == QString("default")) {
                    multiplicationSymbol = MultiplicationSymbol::DEFAULT;
                } else if (symbolString == QString("space")) {
                    multiplicationSymbol = MultiplicationSymbol::SPACE;
                } else if (symbolString == QString("dot")) {
                    multiplicationSymbol = MultiplicationSymbol::DOT;
                } else if (symbolString == QString("cross")) {
                    multiplicationSymbol = MultiplicationSymbol::CROSS;
                } else if (symbolString == QString("circle")) {
                    multiplicationSymbol = MultiplicationSymbol::CIRCLE;
                } else {
                    QString tag = reader->qualifiedName().toString();
                    reader->raiseError(
                        tr("Tag \"%1\" has invalid multiplication symbol \"%2\"").arg(tag).arg(symbolString)
                    );
                }
            }

            if (!reader->hasError()) {
                setMultiplicationSymbol(multiplicationSymbol);
            }
        }
    }


    void MultiplicationOperatorFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        FontFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        ParenthesisFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        OperatorFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        MultiplicationOperatorFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void MultiplicationOperatorFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        FontFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        ParenthesisFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        OperatorFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        MultiplicationOperatorFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }


    Util::HashResult qHash(MultiplicationOperatorFormat::MultiplicationSymbol symbol, Util::HashSeed seed) {
        return ::qHash(static_cast<unsigned>(symbol), seed);
    }
}
