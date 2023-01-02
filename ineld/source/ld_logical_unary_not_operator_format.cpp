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
* This file implements the \ref Ld::LogicalUnaryNotOperatorFormat class.
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
#include "ld_logical_unary_not_operator_format.h"

/***********************************************************************************************************************
 * Ld::LogicalUnaryNotOperatorFormat::Aggregation:
 */

namespace Ld {
    LogicalUnaryNotOperatorFormat::Aggregation::Aggregation() {
        clear();
    }


    LogicalUnaryNotOperatorFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* LogicalUnaryNotOperatorFormat::Aggregation::creator(const QString&) {
        return new LogicalUnaryNotOperatorFormat::Aggregation;
    }


    QString LogicalUnaryNotOperatorFormat::Aggregation::typeName() const {
        return LogicalUnaryNotOperatorFormat::formatName;
    }


    void LogicalUnaryNotOperatorFormat::Aggregation::clear() {
        OperatorFormat::Aggregation::clear();
        currentLogicalNotSymbols.clear();
    }


    bool LogicalUnaryNotOperatorFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<LogicalUnaryNotOperatorFormat> logicalUnaryNotOperatorFormat =
            formatInstance.dynamicCast<LogicalUnaryNotOperatorFormat>();

        if (!logicalUnaryNotOperatorFormat.isNull()) {
            success = OperatorFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentLogicalNotSymbols << logicalUnaryNotOperatorFormat->logicalNotSymbol();
            }
        }

        return success;
    }


    QSet<LogicalUnaryNotOperatorFormat::LogicalNotSymbol>
        LogicalUnaryNotOperatorFormat::Aggregation::logicalNotSymbols() const {
        return currentLogicalNotSymbols;
    }
}

/***********************************************************************************************************************
 * Ld::LogicalUnaryNotOperatorFormat
 */

namespace Ld {
    const QString LogicalUnaryNotOperatorFormat::formatName("LogicalUnaryNotOperatorFormat");
    const LogicalUnaryNotOperatorFormat::LogicalNotSymbol LogicalUnaryNotOperatorFormat::defaultLogicalNotSymbol =
        LogicalUnaryNotOperatorFormat::LogicalNotSymbol::DEFAULT;

    LogicalUnaryNotOperatorFormat::LogicalUnaryNotOperatorFormat() {
        currentLogicalNotSymbol = defaultLogicalNotSymbol;
    }


    LogicalUnaryNotOperatorFormat::LogicalUnaryNotOperatorFormat(
            const LogicalUnaryNotOperatorFormat& other
        ):Format(
            other
        ),FontFormat(
            other
        ),ParenthesisFormat(
            other
        ),OperatorFormat(
            other
        ) {
        currentLogicalNotSymbol = other.currentLogicalNotSymbol;
    }


    LogicalUnaryNotOperatorFormat::~LogicalUnaryNotOperatorFormat() {}


    FormatPointer LogicalUnaryNotOperatorFormat::clone() const {
        return FormatPointer(new LogicalUnaryNotOperatorFormat(*this));
    }


    Format* LogicalUnaryNotOperatorFormat::creator(const QString&) {
        return new LogicalUnaryNotOperatorFormat();
    }


    QString LogicalUnaryNotOperatorFormat::typeName() const {
        return formatName;
    }


    bool LogicalUnaryNotOperatorFormat::isValid() const {
        return true;
    }


    Format::Capabilities LogicalUnaryNotOperatorFormat::capabilities() const {
        return OperatorFormat::capabilities() << formatName;
    }


    void LogicalUnaryNotOperatorFormat::setLogicalNotSymbol(
            LogicalUnaryNotOperatorFormat::LogicalNotSymbol newLogicalNotSymbol
        ) {
        currentLogicalNotSymbol = newLogicalNotSymbol;
        reportFormatUpdated();
    }


    LogicalUnaryNotOperatorFormat::LogicalNotSymbol LogicalUnaryNotOperatorFormat::logicalNotSymbol() const {
        return currentLogicalNotSymbol;
    }


    QString LogicalUnaryNotOperatorFormat::toString() const {
        QString result = QString("%1,%2").arg(OperatorFormat::toString()).arg(toString(currentLogicalNotSymbol));
        return result;
    }


    QString LogicalUnaryNotOperatorFormat::toCss() const {
        QString result = OperatorFormat::toCss();
        return result;
    }


    void LogicalUnaryNotOperatorFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        attributes.append("not_symbol", toString(currentLogicalNotSymbol));
    }


    void LogicalUnaryNotOperatorFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        if (!reader->hasError()) {
            LogicalNotSymbol logicalNotSymbol = defaultLogicalNotSymbol;

            if (attributes.hasAttribute("multiplication_symbol")) {
                QString          symbolString     = attributes.value<QString>("multiplication_symbol").toLower();
                LogicalNotSymbol logicalNotSymbol = toLogicalNotSymbol(symbolString);

                if (logicalNotSymbol == LogicalNotSymbol::INVALID) {
                    QString tag = reader->qualifiedName().toString();
                    reader->raiseError(
                        tr("Tag \"%1\" has invalid logical not symbol \"%2\"").arg(tag).arg(symbolString)
                    );
                }
            }

            if (!reader->hasError()) {
                setLogicalNotSymbol(logicalNotSymbol);
            }
        }
    }


    void LogicalUnaryNotOperatorFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        FontFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        ParenthesisFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        OperatorFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        LogicalUnaryNotOperatorFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void LogicalUnaryNotOperatorFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        FontFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        ParenthesisFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        OperatorFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        LogicalUnaryNotOperatorFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }


    QString LogicalUnaryNotOperatorFormat::toString(LogicalUnaryNotOperatorFormat::LogicalNotSymbol value) {
        QString result;

        switch (value) {
            case LogicalNotSymbol::INVALID:  { result = QString("INVALID");   break; }
            case LogicalNotSymbol::DEFAULT:  { result = QString("DEFAULT");   break; }
            case LogicalNotSymbol::NEGATION: { result = QString("NEGATION");  break; }
            case LogicalNotSymbol::OVERBAR:  { result = QString("OVERBAR");   break; }
            case LogicalNotSymbol::TILDE:    { result = QString("TILDE");     break; }

            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    LogicalUnaryNotOperatorFormat::LogicalNotSymbol LogicalUnaryNotOperatorFormat::toLogicalNotSymbol(
            const QString& text
        ) {
        LogicalNotSymbol result;
        QString          value = text.toLower();

        if        (value == QString("INVALID")) {
            result = LogicalNotSymbol::INVALID;
        } else if (value == QString("DEFAULT")) {
            result = LogicalNotSymbol::DEFAULT;
        } else if (value == QString("NEGATION")) {
            result = LogicalNotSymbol::NEGATION;
        } else if (value == QString("OVERBAR")) {
            result = LogicalNotSymbol::OVERBAR;
        } else if (value == QString("TILDE")) {
            result = LogicalNotSymbol::TILDE;
        } else {
            result = LogicalNotSymbol::INVALID;
        }

        return result;
    }


    Util::HashResult qHash(LogicalUnaryNotOperatorFormat::LogicalNotSymbol symbol, Util::HashSeed seed) {
        return ::qHash(static_cast<unsigned>(symbol), seed);
    }
}
