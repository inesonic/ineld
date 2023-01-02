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
* This file implements the \ref Ld::OperatorFormat class.
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
#include "ld_font_format.h"
#include "ld_operator_format.h"

/***********************************************************************************************************************
 * Ld::OperatorFormat::Aggregation:
 */

namespace Ld {
    OperatorFormat::Aggregation::Aggregation() {
        clear();
    }


    OperatorFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* OperatorFormat::Aggregation::creator(const QString&) {
        return new OperatorFormat::Aggregation;
    }


    QString OperatorFormat::Aggregation::typeName() const {
        return OperatorFormat::formatName;
    }


    void OperatorFormat::Aggregation::clear() {
        FontFormat::Aggregation::clear();
        ParenthesisFormat::Aggregation::clear();

        currentForceParenthesisValues.clear();
    }


    bool OperatorFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<OperatorFormat> operatorFormat = formatInstance.dynamicCast<OperatorFormat>();
        if (!operatorFormat.isNull()) {
            success = ParenthesisFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentForceParenthesisValues << operatorFormat->forcedParenthesis();
            }
        }

        return success;
    }


    const OperatorFormat::BooleanAggregation& OperatorFormat::Aggregation::forceParenthesis() const {
        return currentForceParenthesisValues;
    }
}

/***********************************************************************************************************************
 * Ld::OperatorFormat
 */

namespace Ld {
    const QString OperatorFormat::formatName("OperatorFormat");
    const ParenthesisFormat::ParenthesisStyle OperatorFormat::defaultParenthesisStyle =
        ParenthesisFormat::ParenthesisStyle::PARENTHESIS;

    OperatorFormat::OperatorFormat() {
        setParenthesisStyle(defaultParenthesisStyle);
        currentForceParenthesis = false;
    }


    OperatorFormat::OperatorFormat(
            const OperatorFormat& other
        ):Format(
            other
        ),FontFormat(
            other
        ),ParenthesisFormat(
            other
        ) {
        currentForceParenthesis = other.currentForceParenthesis;
    }


    OperatorFormat::~OperatorFormat() {}


    FormatPointer OperatorFormat::clone() const {
        return FormatPointer(new OperatorFormat(*this));
    }


    Format* OperatorFormat::creator(const QString&) {
        return new OperatorFormat();
    }


    QString OperatorFormat::typeName() const {
        return formatName;
    }


    bool OperatorFormat::isValid() const {
        return true;
    }


    Format::Capabilities OperatorFormat::capabilities() const {
        return ParenthesisFormat::capabilities() << formatName;
    }


    bool OperatorFormat::supportsDefaultParenthesisStyle() const {
        return false;
    }


    void OperatorFormat::setForcedParenthesis(bool nowForced) {
        currentForceParenthesis = nowForced;
        reportFormatUpdated();
    }


    bool OperatorFormat::forcedParenthesis() const {
        return currentForceParenthesis;
    }


    QString OperatorFormat::toString() const {
        QString forcedState = currentForceParenthesis ? "TRUE" : "FALSE";
        QString result = QString("%1,%2,%3").arg(ParenthesisFormat::toString()).arg(forcedState);
        return result;
    }


    QString OperatorFormat::toCss() const {
        QString result = FontFormat::toCss();
        return result;
    }


    void OperatorFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (currentForceParenthesis != false) {
            attributes.append("forced_parenthesis", currentForceParenthesis);
        }
    }


    void OperatorFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        bool forcedParenthesis = false;

        if (!reader->hasError() && attributes.hasAttribute("forced_parenthesis")) {
            bool ok;
            forcedParenthesis = attributes.value<bool>("forced_parenthesis", &ok);

            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                QString value = attributes.value<QString>("forced_parenthesis");
                reader->raiseError(
                    tr("Tag \"%1\" has invalid forced parenthesis value \"%2\"").arg(tag).arg(value)
                );
            }
        }

        if (!reader->hasError()) {
            setForcedParenthesis(forcedParenthesis);
        }
    }


    void OperatorFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        FontFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        ParenthesisFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        OperatorFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void OperatorFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        FontFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        ParenthesisFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        OperatorFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }
}
