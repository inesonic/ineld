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
* This file implements the \ref Ld::DefaultFormatSetting class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>

#include <limits>

#include "ld_handle.h"
#include "ld_element_cursor.h"
#include "ld_element.h"
#include "ld_root_element.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_xml_element.h"
#include "ld_document_setting.h"
#include "ld_default_format_setting.h"

namespace Ld {
    const char DefaultFormatSetting::defaultTextFormatSettingName[]           = "DefaultTextFormatSetting";
    const char DefaultFormatSetting::defaultMathFormatSettingName[]           = "DefaultMathFormatSetting";
    const char DefaultFormatSetting::defaultMathIdentifierFormatSettingName[] = "DefaultMathIdentifierFormatSetting";
    const char DefaultFormatSetting::defaultMathFunctionFormatSettingName[]   = "DefaultMathFunctionFormatSetting";
    const char
        DefaultFormatSetting::defaultBraceConditionalFormatSettingName[] = "DefaultBraceConditionalFormatSetting";

    DefaultFormatSetting::DefaultFormatSetting(const QString& settingName) {
        currentSettingName      = settingName;
        currentFormatIdentifier = invalidFormatIdentifier;
    }


    DefaultFormatSetting::~DefaultFormatSetting() {}


    Ld::DocumentSetting* DefaultFormatSetting::creator(const QString& typeName) {
        return new DefaultFormatSetting(typeName);
    }


    QString DefaultFormatSetting::typeName() const {
        return QString(currentSettingName);
    }


    bool DefaultFormatSetting::isValid() const {
        return currentFormatIdentifier != invalidFormatIdentifier;
    }


    void DefaultFormatSetting::setFormatIdentifier(FormatIdentifier newFormatIdentifier) {
        currentFormatIdentifier = newFormatIdentifier;
    }


    FormatIdentifier DefaultFormatSetting::formatIdentifier() const {
        return currentFormatIdentifier;
    }


    void DefaultFormatSetting::writeAddAttributes(Ld::XmlAttributes& attributes) {
        if (currentFormatIdentifier != invalidFormatIdentifier) {
            attributes.append("value", currentFormatIdentifier);
        }
    }


    void DefaultFormatSetting::readAttributes(
            QSharedPointer<Ld::XmlReader> reader,
            const Ld::XmlAttributes&      attributes,
            unsigned
        ) {
        if (attributes.hasAttribute("value")) {
            bool ok;
            currentFormatIdentifier = attributes.value<FormatIdentifier>("value", &ok);

            if (!ok) {
                currentFormatIdentifier = invalidFormatIdentifier;

                QString formatIdentifierString = attributes.value<QString>("value");
                reader->raiseError(tr("Invalid format identifier \"%1\"").arg(formatIdentifierString));
            }
        } else {
            currentFormatIdentifier = invalidFormatIdentifier;
        }
    }
}
