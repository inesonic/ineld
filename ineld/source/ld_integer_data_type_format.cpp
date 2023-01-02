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
* This file implements the \ref Ld::IntegerDataTypeFormat class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>
#include <QSet>

#include <cassert>

#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_program_file.h"
#include "ld_numeric_data_type_format.h"
#include "ld_integer_data_type_format.h"

/***********************************************************************************************************************
 * Ld::IntegerDataTypeFormat::Aggregation
 */

namespace Ld {
    IntegerDataTypeFormat::Aggregation::Aggregation() {}


    IntegerDataTypeFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* IntegerDataTypeFormat::Aggregation::creator(const QString&) {
        return new IntegerDataTypeFormat::Aggregation;
    }


    QString IntegerDataTypeFormat::Aggregation::typeName() const {
        return IntegerDataTypeFormat::formatName;
    }


    void IntegerDataTypeFormat::Aggregation::clear() {
        NumericDataTypeFormat::Aggregation::clear();

        currentWidths.clear();
        currentBases.clear();
        currentIntegerNumberStyles.clear();
    }


    bool IntegerDataTypeFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<IntegerDataTypeFormat>
            integerDataTypeFormat = formatInstance.dynamicCast<IntegerDataTypeFormat>();
        if (!integerDataTypeFormat.isNull()) {
            success = NumericDataTypeFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentWidths              << integerDataTypeFormat->width();
                currentBases               << integerDataTypeFormat->base();
                currentIntegerNumberStyles << integerDataTypeFormat->integerNumberStyle();
            }
        }

        return success;
    }


    QSet<unsigned> IntegerDataTypeFormat::Aggregation::widths() const {
        return currentWidths;
    }


    QSet<unsigned> IntegerDataTypeFormat::Aggregation::bases() const {
        return currentBases;
    }


    QSet<IntegerDataTypeFormat::IntegerNumberStyle> IntegerDataTypeFormat::Aggregation::integerNumberStyles() const {
        return currentIntegerNumberStyles;
    }
}

/***********************************************************************************************************************
 * Ld::IntegerDataTypeFormat
 */

namespace Ld {
    const QString  IntegerDataTypeFormat::formatName("IntegerDataTypeFormat");
    const unsigned IntegerDataTypeFormat::defaultWidth =  0;
    const unsigned IntegerDataTypeFormat::ignoreWidth  =  0;
    const unsigned IntegerDataTypeFormat::defaultBase  = 10;

    const IntegerDataTypeFormat::IntegerNumberStyle
        IntegerDataTypeFormat::defaultIntegerNumberStyle = IntegerDataTypeFormat::IntegerNumberStyle::NONE;

    IntegerDataTypeFormat::IntegerDataTypeFormat() {
        currentWidth              = defaultWidth;
        currentBase               = defaultBase;
        currentIntegerNumberStyle = defaultIntegerNumberStyle;
    }


    IntegerDataTypeFormat::IntegerDataTypeFormat(
            const IntegerDataTypeFormat& other
        ):Format(
            other
        ),FontFormat(
            other
        ),DataTypeFormat(
            other
        ),NumericDataTypeFormat(
            other
        ) {
        currentWidth              = other.currentWidth;
        currentBase               = other.currentBase;
        currentIntegerNumberStyle = other.currentIntegerNumberStyle;
    }


    IntegerDataTypeFormat::~IntegerDataTypeFormat() {}


    FormatPointer IntegerDataTypeFormat::clone() const {
        return FormatPointer(new IntegerDataTypeFormat(*this));
    }


    Format* IntegerDataTypeFormat::creator(const QString&) {
        return new IntegerDataTypeFormat();
    }


    QString IntegerDataTypeFormat::typeName() const {
        return formatName;
    }


    bool IntegerDataTypeFormat::isValid() const {
        return (
               NumericDataTypeFormat::isValid()
            && currentBase > 1
            && currentIntegerNumberStyle != IntegerNumberStyle::INVALID
        );
    }


    Format::Capabilities IntegerDataTypeFormat::capabilities() const {
        return NumericDataTypeFormat::capabilities() << formatName;
    }


    void IntegerDataTypeFormat::setWidth(unsigned newFieldWidth) {
        currentWidth = newFieldWidth;
        reportFormatUpdated();
    }


    unsigned IntegerDataTypeFormat::width() const {
        return currentWidth;
    }


    void IntegerDataTypeFormat::setBase(unsigned newBase) {
        currentBase = newBase;
        reportFormatUpdated();
    }


    unsigned IntegerDataTypeFormat::base() const {
        return currentBase;
    }


    void IntegerDataTypeFormat::setIntegerNumberStyle(IntegerDataTypeFormat::IntegerNumberStyle newPrefixStyle) {
        currentIntegerNumberStyle = newPrefixStyle;
        reportFormatUpdated();
    }


    IntegerDataTypeFormat::IntegerNumberStyle IntegerDataTypeFormat::integerNumberStyle() const {
        return currentIntegerNumberStyle;
    }


    QString IntegerDataTypeFormat::toString() const {
        return QString("%1,%2,%3,%4")
               .arg(NumericDataTypeFormat::toString())
               .arg(currentWidth)
               .arg(toString(currentIntegerNumberStyle))
               .arg(currentBase);
    }


    QString IntegerDataTypeFormat::toCss() const {
        QString result = NumericDataTypeFormat::toCss();
        return result;
    }


    void IntegerDataTypeFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (currentWidth != defaultWidth) {
            attributes.append("width", currentWidth);
        }

        if (currentBase != defaultBase) {
            attributes.append("base", currentBase);
        }

        if (currentIntegerNumberStyle != defaultIntegerNumberStyle) {
            attributes.append("integer_style", toString(currentIntegerNumberStyle));
        }
    }


    void IntegerDataTypeFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        if (!reader->hasError()) {
            if (attributes.hasAttribute("width")) {
                bool ok;
                unsigned newWidth = attributes.value<unsigned>("width", &ok);
                if (!ok) {
                    QString tag = reader->qualifiedName().toString();
                    reader->raiseError(tr("Tag \"%1\", invalid \"width\" attribute").arg(tag));
                } else {
                    currentWidth = newWidth;
                }
            } else {
                currentWidth = defaultWidth;
            }
        }

        if (!reader->hasError()) {
            if (attributes.hasAttribute("base")) {
                bool     ok;
                unsigned newBase = attributes.value<unsigned>("base", &ok);
                if (!ok || newBase < 2) {
                    QString tag = reader->qualifiedName().toString();
                    reader->raiseError(tr("Tag \"%1\", invalid \"base\" attribute").arg(tag));
                } else {
                    currentBase = newBase;
                }
            } else {
                currentBase = defaultBase;
            }
        }

        if (!reader->hasError()) {
            if (attributes.hasAttribute("integer_style")) {
                QString prefixString = attributes.value<QString>("integer_style");

                bool               ok;
                IntegerNumberStyle newIntegerNumberStyle = toIntegerNumberStyle(prefixString, &ok);

                if (!ok || newIntegerNumberStyle == IntegerNumberStyle::INVALID) {
                    QString tag = reader->qualifiedName().toString();
                    reader->raiseError(tr("Tag \"%1\", invalid \"integer_number_style\" attribute").arg(tag));
                } else {
                    currentIntegerNumberStyle = newIntegerNumberStyle;
                }
            } else {
                currentIntegerNumberStyle = defaultIntegerNumberStyle;
            }
        }
    }


    void IntegerDataTypeFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        FontFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        DataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        NumericDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        IntegerDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void IntegerDataTypeFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        FontFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        DataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        NumericDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        IntegerDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }


    QString IntegerDataTypeFormat::toString(IntegerDataTypeFormat::IntegerNumberStyle integerNumberStyle) {
        QString result;
        switch (integerNumberStyle) {
            case IntegerNumberStyle::INVALID:         { result = QString("INVALID");          break; }
            case IntegerNumberStyle::NONE:            { result = QString("NONE");             break; }
            case IntegerNumberStyle::C_STYLE:         { result = QString("C_STYLE");          break; }
            case IntegerNumberStyle::VERILOG_STYLE:   { result = QString("VERILOG_STYLE");    break; }
            case IntegerNumberStyle::SUBSCRIPT_STYLE: { result = QString("SUBSCRIPT_style");  break; }

            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    IntegerDataTypeFormat::IntegerNumberStyle IntegerDataTypeFormat::toIntegerNumberStyle(
            const QString& str,
            bool*          ok
        ) {
        IntegerNumberStyle result;
        bool               success;

        QString lc = str.toLower();
        if        (lc == QString("invalid")) {
            result  = IntegerNumberStyle::INVALID;
            success = true;
        } else if (lc == QString("none")) {
            result  = IntegerNumberStyle::NONE;
            success = true;
        } else if (lc == QString("c_style")) {
            result  = IntegerNumberStyle::C_STYLE;
            success = true;
        } else if (lc == QString("verilog_style")) {
            result  = IntegerNumberStyle::VERILOG_STYLE;
            success = true;
        } else if (lc == QString("subscript_style")) {
            result  = IntegerNumberStyle::SUBSCRIPT_STYLE;
            success = true;
        } else {
            result  = IntegerNumberStyle::INVALID;
            success = false;
        }

        if (ok != nullptr) {
            *ok = success;
        }

        return result;
    }
}
