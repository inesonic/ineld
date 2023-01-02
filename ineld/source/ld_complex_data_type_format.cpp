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
* This file implements the \ref Ld::ComplexDataTypeFormat class.
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
#include "ld_program_file.h"
#include "ld_real_data_type_format.h"
#include "ld_complex_data_type_format.h"

/***********************************************************************************************************************
 * Ld::ComplexDataTypeFormat::Aggregation
 */

namespace Ld {
    ComplexDataTypeFormat::Aggregation::Aggregation() {}


    ComplexDataTypeFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* ComplexDataTypeFormat::Aggregation::creator(const QString&) {
        return new ComplexDataTypeFormat::Aggregation;
    }


    QString ComplexDataTypeFormat::Aggregation::typeName() const {
        return ComplexDataTypeFormat::formatName;
    }


    void ComplexDataTypeFormat::Aggregation::clear() {
        RealDataTypeFormat::Aggregation::clear();
        currentImaginaryUnits.clear();
    }


    bool ComplexDataTypeFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<ComplexDataTypeFormat>
            complexDataTypeFormat = formatInstance.dynamicCast<ComplexDataTypeFormat>();
        if (!complexDataTypeFormat.isNull()) {
            success = RealDataTypeFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentImaginaryUnits << complexDataTypeFormat->imaginaryUnit();
            }
        }

        return success;
    }


    QSet<ComplexDataTypeFormat::ImaginaryUnit> ComplexDataTypeFormat::Aggregation::imaginaryUnits() const {
        return currentImaginaryUnits;
    }
}

/***********************************************************************************************************************
 * Ld::ComplexDataTypeFormat
 */

namespace Ld {
    const QString ComplexDataTypeFormat::formatName("ComplexDataTypeFormat");

    const ComplexDataTypeFormat::ImaginaryUnit
        ComplexDataTypeFormat::defaultImaginaryUnit = ComplexDataTypeFormat::ImaginaryUnit::I;

    ComplexDataTypeFormat::ComplexDataTypeFormat() {
        currentImaginaryUnit = defaultImaginaryUnit;
    }


    ComplexDataTypeFormat::ComplexDataTypeFormat(
            const ComplexDataTypeFormat& other
        ):Format(
            other
        ),FontFormat(
            other
        ),DataTypeFormat(
            other
        ),NumericDataTypeFormat(
            other
        ),RealDataTypeFormat(
            other
        ) {
        currentImaginaryUnit = other.currentImaginaryUnit;
    }


    ComplexDataTypeFormat::~ComplexDataTypeFormat() {}


    FormatPointer ComplexDataTypeFormat::clone() const {
        return FormatPointer(new ComplexDataTypeFormat(*this));
    }


    Format* ComplexDataTypeFormat::creator(const QString&) {
        return new ComplexDataTypeFormat();
    }


    QString ComplexDataTypeFormat::typeName() const {
        return formatName;
    }


    bool ComplexDataTypeFormat::isValid() const {
        return RealDataTypeFormat::isValid() && currentImaginaryUnit != ImaginaryUnit::INVALID;
    }


    Format::Capabilities ComplexDataTypeFormat::capabilities() const {
        return RealDataTypeFormat::capabilities() << formatName;
    }


    void ComplexDataTypeFormat::setImaginaryUnit(ImaginaryUnit newImaginaryUnit) {
        currentImaginaryUnit = newImaginaryUnit;
        reportFormatUpdated();
    }


    ComplexDataTypeFormat::ImaginaryUnit ComplexDataTypeFormat::imaginaryUnit() const {
        return currentImaginaryUnit;
    }


    QString ComplexDataTypeFormat::toString() const {
        return QString("%1,%2").arg(RealDataTypeFormat::toString()).arg(toString(currentImaginaryUnit));
    }


    QString ComplexDataTypeFormat::toCss() const {
        QString result = NumericDataTypeFormat::toCss();
        return result;
    }


    void ComplexDataTypeFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (currentImaginaryUnit != defaultImaginaryUnit) {
            attributes.append("imaginary_unit", toString(currentImaginaryUnit));
        }
    }


    void ComplexDataTypeFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        if (!reader->hasError()) {
            if (attributes.hasAttribute("imaginary_unit")) {
                QString unitString = attributes.value<QString>("imaginary_unit");

                bool          ok;
                ImaginaryUnit newImaginaryUnit = toImaginaryUnit(unitString, &ok);
                if (!ok) {
                    QString tag = reader->qualifiedName().toString();
                    reader->raiseError(tr("Tag \"%1\", invalid \"imaginary_unit\" attribute").arg(tag));
                } else {
                    currentImaginaryUnit = newImaginaryUnit;
                }
            } else {
                currentImaginaryUnit = defaultImaginaryUnit;
            }
        }
    }


    void ComplexDataTypeFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        FontFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        DataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        NumericDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        RealDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        ComplexDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void ComplexDataTypeFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        FontFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        DataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        NumericDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        RealDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        ComplexDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }


    QString ComplexDataTypeFormat::toString(ComplexDataTypeFormat::ImaginaryUnit imaginaryUnit) {
        QString result;
        switch (imaginaryUnit) {
            case ImaginaryUnit::INVALID: { result = QString("INVALID");  break; }
            case ImaginaryUnit::I:       { result = QString("I");        break; }
            case ImaginaryUnit::J:       { result = QString("J");        break; }

            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    ComplexDataTypeFormat::ImaginaryUnit ComplexDataTypeFormat::toImaginaryUnit(const QString& str, bool* ok) {
        ImaginaryUnit result;
        bool          success;

        QString lc = str.toLower();
        if        (lc == QString("invalid")) {
            result  = ImaginaryUnit::INVALID;
            success = true;
        } else if (lc == QString("i")) {
            result  = ImaginaryUnit::I;
            success = true;
        } else if (lc == QString("j")) {
            result  = ImaginaryUnit::J;
            success = true;
        } else {
            result  = ImaginaryUnit::INVALID;
            success = false;
        }

        if (ok != nullptr) {
            *ok = success;
        }

        return result;
    }


    Util::HashResult qHash(ComplexDataTypeFormat::ImaginaryUnit imaginaryUnit, Util::HashSeed seed) {
        return ::qHash(static_cast<unsigned>(imaginaryUnit), seed);
    }
}
