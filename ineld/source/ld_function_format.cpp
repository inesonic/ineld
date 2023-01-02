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
* This file implements the \ref Ld::FunctionFormat class.
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
#include "ld_function_format.h"

/***********************************************************************************************************************
 * Ld::FunctionFormat::Aggregation:
 */

namespace Ld {
    FunctionFormat::Aggregation::Aggregation() {
        clear();
    }


    FunctionFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* FunctionFormat::Aggregation::creator(const QString&) {
        return new FunctionFormat::Aggregation;
    }


    QString FunctionFormat::Aggregation::typeName() const {
        return FunctionFormat::formatName;
    }


    void FunctionFormat::Aggregation::clear() {
        FontFormat::Aggregation::clear();
        ParenthesisFormat::Aggregation::clear();
        OperatorFormat::Aggregation::clear();

        currentSubscriptedParameters.clear();
        currentParameterParenthesisStyles.clear();
        currentForceParameterParenthesisValues.clear();
    }


    bool FunctionFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<FunctionFormat> functionFormat = formatInstance.dynamicCast<FunctionFormat>();
        if (!functionFormat.isNull()) {
            success = OperatorFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentSubscriptedParameters << functionFormat->subscriptedParameter();
                currentParameterParenthesisStyles << functionFormat->parameterParenthesisStyle();
                currentForceParameterParenthesisValues << functionFormat->forceParameterParenthesis();
            }
        }

        return success;
    }


    const FunctionFormat::BooleanAggregation& FunctionFormat::Aggregation::subscriptedParameters() const {
        return currentSubscriptedParameters;
    }


    const QSet<ParenthesisFormat::ParenthesisStyle>& FunctionFormat::Aggregation::parameterParenthesisStyles() const {
        return currentParameterParenthesisStyles;
    }


    const FunctionFormat::BooleanAggregation& FunctionFormat::Aggregation::forceParameterParenthesis() const {
        return currentForceParameterParenthesisValues;
    }
}

/***********************************************************************************************************************
 * Ld::FunctionFormat
 */

namespace Ld {
    const QString FunctionFormat::formatName("FunctionFormat");

    FunctionFormat::FunctionFormat() {
        currentFirstParameterSubscripted = false;
        currentParameterParenthesisStyle = ParenthesisStyle::PARENTHESIS;
        currentForceParameterParenthesis = false;
    }


    FunctionFormat::FunctionFormat(
            const FunctionFormat& other
        ):Format(
            other
        ),FontFormat(
            other
        ),ParenthesisFormat(
            other
        ),OperatorFormat(
            other
        ) {
        currentFirstParameterSubscripted = other.currentFirstParameterSubscripted;
        currentParameterParenthesisStyle = other.currentParameterParenthesisStyle;
        currentForceParameterParenthesis = other.currentForceParameterParenthesis;
    }


    FunctionFormat::~FunctionFormat() {}


    FormatPointer FunctionFormat::clone() const {
        return FormatPointer(new FunctionFormat(*this));
    }


    Format* FunctionFormat::creator(const QString&) {
        return new FunctionFormat();
    }


    QString FunctionFormat::typeName() const {
        return formatName;
    }


    bool FunctionFormat::isValid() const {
        return true;
    }


    Format::Capabilities FunctionFormat::capabilities() const {
        return OperatorFormat::capabilities() << formatName;
    }


    void FunctionFormat::setSubscriptedParameter(bool nowSubscripted) {
        currentFirstParameterSubscripted = nowSubscripted;
        reportFormatUpdated();
    };


    void FunctionFormat::setNoSubscriptedParameter(bool nowNotSubscripted) {
        return setSubscriptedParameter(!nowNotSubscripted);
    }


    bool FunctionFormat::subscriptedParameter() const {
        return currentFirstParameterSubscripted;
    }


    bool FunctionFormat::noSubscriptedParameter() const {
        return !currentFirstParameterSubscripted;
    }


    void FunctionFormat::setParameterParenthesisStyle(ParenthesisFormat::ParenthesisStyle newParenthesisStyle) {
        currentParameterParenthesisStyle = newParenthesisStyle;
        reportFormatUpdated();
    }


    ParenthesisFormat::ParenthesisStyle FunctionFormat::parameterParenthesisStyle() const {
        return currentParameterParenthesisStyle;
    }


    void FunctionFormat::setForceParameterParenthesis(bool nowForceParenthesis) {
        currentForceParameterParenthesis = nowForceParenthesis;
        reportFormatUpdated();
    }


    bool FunctionFormat::forceParameterParenthesis() const {
        return currentForceParameterParenthesis;
    }


    QString FunctionFormat::toString() const {
        QString result = QString("%1,%2,%3,%4")
                         .arg(OperatorFormat::toString())
                         .arg(currentFirstParameterSubscripted ? "TRUE" : "FALSE")
                         .arg(ParenthesisFormat::toString(currentParameterParenthesisStyle))
                         .arg(currentForceParameterParenthesis ? "TRUE" : "FALSE");
        return result;
    }


    QString FunctionFormat::toCss() const {
        QString result = FontFormat::toCss();
        return result;
    }


    void FunctionFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (currentFirstParameterSubscripted != false) {
            attributes.append("first_parameter_subscripted", currentFirstParameterSubscripted);
        }

        if (currentParameterParenthesisStyle != defaultParenthesisStyle) {
            attributes.append(
                "parameter_parenthesis_style",
                ParenthesisFormat::toString(currentParameterParenthesisStyle)
            );
        }

        if (currentForceParameterParenthesis != false) {
            attributes.append("forced_parameter_parenthesis", currentForceParameterParenthesis);
        }
    }


    void FunctionFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        bool             firstParameterSubscripted  = false;
        ParenthesisStyle parameterParenthesisStyle  = defaultParenthesisStyle;
        bool             forcedParameterParenthesis = false;

        if (!reader->hasError() && attributes.hasAttribute("first_parameter_subscripted")) {
            bool ok;
            firstParameterSubscripted = attributes.value<bool>("first_parameter_subscripted", &ok);

            if (!ok) {
                QString tag   = reader->qualifiedName().toString();
                QString value = attributes.value<QString>("first_parameter_subscripted");
                reader->raiseError(
                    tr("Tag \"%1\" has invalid first parameter subscripted value \"%2\"").arg(tag).arg(value)
                );
            }
        }

        if (!reader->hasError() && attributes.hasAttribute("parameter_parenthesis_style")) {
            bool ok;
            QString parameterParenthesisStyleText = attributes.value<QString>("parameter_parenthesis_style");
            parameterParenthesisStyle = toParenthesisStyle(parameterParenthesisStyleText, &ok);

            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(
                    tr("Tag \"%1\" has invalid parameter parenthesis style value \"%2\"")
                        .arg(tag)
                        .arg(parameterParenthesisStyleText)
                );
            }
        }

        if (!reader->hasError() && attributes.hasAttribute("forced_parameter_parenthesis")) {
            bool ok;
            forcedParameterParenthesis = attributes.value<bool>("forced_parameter_parenthesis", &ok);

            if (!ok) {
                QString tag   = reader->qualifiedName().toString();
                QString value = attributes.value<QString>("first_parameter_subscripted");
                reader->raiseError(
                    tr("Tag \"%1\" has invalid forced parameter parenthesis value \"%2\"").arg(tag).arg(value)
                );
            }
        }

        if (!reader->hasError()) {
            setSubscriptedParameter(firstParameterSubscripted);
            setParameterParenthesisStyle(parameterParenthesisStyle);
            setForceParameterParenthesis(forcedParameterParenthesis);
        }
    }


    void FunctionFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        FontFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        ParenthesisFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        OperatorFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        FunctionFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void FunctionFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        FontFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        ParenthesisFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        OperatorFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        FunctionFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }
}
