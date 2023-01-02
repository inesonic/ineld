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
* This file implements the \ref Ld::DivisionOperatorFormat class.
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
#include "ld_division_operator_format.h"

/***********************************************************************************************************************
 * Ld::DivisionOperatorFormat::Aggregation:
 */

namespace Ld {
    DivisionOperatorFormat::Aggregation::Aggregation() {
        clear();
    }


    DivisionOperatorFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* DivisionOperatorFormat::Aggregation::creator(const QString&) {
        return new DivisionOperatorFormat::Aggregation;
    }


    QString DivisionOperatorFormat::Aggregation::typeName() const {
        return DivisionOperatorFormat::formatName;
    }


    void DivisionOperatorFormat::Aggregation::clear() {
        OperatorFormat::Aggregation::clear();
        currentDivisionSymbols.clear();
    }


    bool DivisionOperatorFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<DivisionOperatorFormat> divisionOperatorFormat =
            formatInstance.dynamicCast<DivisionOperatorFormat>();

        if (!divisionOperatorFormat.isNull()) {
            success = OperatorFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentDivisionSymbols << divisionOperatorFormat->divisionSymbol();
            }
        }

        return success;
    }


    QSet<DivisionOperatorFormat::DivisionSymbol>
        DivisionOperatorFormat::Aggregation::divisionSymbols() const {
        return currentDivisionSymbols;
    }
}

/***********************************************************************************************************************
 * Ld::DivisionOperatorFormat
 */

namespace Ld {
    const QString DivisionOperatorFormat::formatName("DivisionOperatorFormat");
    const DivisionOperatorFormat::DivisionSymbol DivisionOperatorFormat::defaultDivisionSymbol =
        DivisionOperatorFormat::DivisionSymbol::DEFAULT;

    DivisionOperatorFormat::DivisionOperatorFormat() {
        currentDivisionSymbol = defaultDivisionSymbol;
    }


    DivisionOperatorFormat::DivisionOperatorFormat(
            const DivisionOperatorFormat& other
        ):Format(
            other
        ),FontFormat(
            other
        ),ParenthesisFormat(
            other
        ),OperatorFormat(
            other
        ),currentDivisionSymbol(
            other.currentDivisionSymbol
        ) {}


    DivisionOperatorFormat::~DivisionOperatorFormat() {}


    FormatPointer DivisionOperatorFormat::clone() const {
        return FormatPointer(new DivisionOperatorFormat(*this));
    }


    Format* DivisionOperatorFormat::creator(const QString&) {
        return new DivisionOperatorFormat();
    }


    QString DivisionOperatorFormat::typeName() const {
        return formatName;
    }


    bool DivisionOperatorFormat::isValid() const {
        return true;
    }


    Format::Capabilities DivisionOperatorFormat::capabilities() const {
        return OperatorFormat::capabilities() << formatName;
    }


    void DivisionOperatorFormat::setDivisionSymbol(
            DivisionOperatorFormat::DivisionSymbol newDivisionSymbol
        ) {
        currentDivisionSymbol = newDivisionSymbol;
        reportFormatUpdated();
    }


    DivisionOperatorFormat::DivisionSymbol DivisionOperatorFormat::divisionSymbol() const {
        return currentDivisionSymbol;
    }


    QString DivisionOperatorFormat::toString() const {
        QString symbol;

        switch (currentDivisionSymbol) {
            case DivisionSymbol::DEFAULT:  { symbol = QString("DEFAULT");  break; }
            case DivisionSymbol::OBELUS:   { symbol = QString("OBELUS");   break; }
            case DivisionSymbol::FRACTION: { symbol = QString("FRACTION"); break; }
            default:                       { symbol = QString("DEFAULT");  break; }
        }

        QString result = QString("%1,%2").arg(OperatorFormat::toString()).arg(symbol);
        return result;
    }


    QString DivisionOperatorFormat::toCss() const {
        QString result = OperatorFormat::toCss();
        return result;
    }


    void DivisionOperatorFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        QString symbol;

        switch (currentDivisionSymbol) {
            case DivisionSymbol::DEFAULT:  { symbol = QString("DEFAULT");  break; }
            case DivisionSymbol::OBELUS:   { symbol = QString("OBELUS");   break; }
            case DivisionSymbol::FRACTION: { symbol = QString("FRACTION"); break; }

            default: {
                assert(false);
                break;
            }
        }

        attributes.append("division_symbol", symbol);
    }


    void DivisionOperatorFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        if (!reader->hasError()) {
            DivisionSymbol divisionSymbol = defaultDivisionSymbol;

            if (attributes.hasAttribute("division_symbol")) {
                QString symbolString = attributes.value<QString>("division_symbol").toLower();

                if (symbolString == QString("default")) {
                    divisionSymbol = DivisionSymbol::DEFAULT;
                } else if (symbolString == QString("obelus")) {
                    divisionSymbol = DivisionSymbol::OBELUS;
                } else if (symbolString == QString("fraction")) {
                    divisionSymbol = DivisionSymbol::FRACTION;
                } else {
                    QString tag = reader->qualifiedName().toString();
                    reader->raiseError(
                        tr("Tag \"%1\" has invalid division symbol \"%2\"").arg(tag).arg(symbolString)
                    );
                }
            }

            if (!reader->hasError()) {
                setDivisionSymbol(divisionSymbol);
            }
        }
    }


    void DivisionOperatorFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        FontFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        ParenthesisFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        OperatorFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        DivisionOperatorFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void DivisionOperatorFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        FontFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        ParenthesisFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        OperatorFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        DivisionOperatorFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }


    Util::HashResult qHash(DivisionOperatorFormat::DivisionSymbol symbol, Util::HashSeed seed) {
        return ::qHash(static_cast<unsigned>(symbol), seed);
    }
}
