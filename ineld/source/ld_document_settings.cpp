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
* This file implements the \ref Ld::DocumentSettings class.
***********************************************************************************************************************/

#include <QString>
#include <QMap>
#include <QSharedPointer>

#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_xml_element.h"
#include "ld_document_setting.h"
#include "ld_document_settings.h"

namespace Ld {
    DocumentSettings::DocumentSettings() {}


    DocumentSettings::DocumentSettings(
            const DocumentSettings& other
        ):QMap<QString, QSharedPointer<DocumentSetting>>(
            other
        ) {}


    DocumentSettings::~DocumentSettings() {}


    void DocumentSettings::insert(QSharedPointer<DocumentSetting> newInstance) {
        QMap<QString, QSharedPointer<DocumentSetting>>::insert(newInstance->typeName(), newInstance);
    }


    void DocumentSettings::writeXml(QSharedPointer<XmlWriter> writer) {
        for (const_iterator it=constBegin(),end=constEnd() ; it!=end ; ++it) {
            QSharedPointer<DocumentSetting> setting = it.value();
            setting->writeXml(writer);
        }
    }


    void DocumentSettings::readXml(QSharedPointer<XmlReader> reader, unsigned xmlVersion) {
        clear();

        XmlReader::TokenType tokenType;
        do {
            tokenType = reader->readNext();

            if (tokenType == XmlReader::StartElement) {
                QString settingName = reader->qualifiedName().toString();
                if (contains(settingName)) {
                    reader->raiseError(tr("Duplicate tag \"%1\" encountered").arg(settingName));
                } else {
                    QSharedPointer<DocumentSetting> newSetting = DocumentSetting::create(settingName);
                    if (newSetting.isNull()) {
                        reader->raiseError(tr("Unknown document setting \"%1\"").arg(settingName));
                    } else {
                        XmlAttributes childAttributes = reader->attributes();
                        newSetting->readXml(reader, childAttributes, xmlVersion);

                        if (!reader->hasError()) {
                            insert(newSetting);
                        }
                    }
                }
            } else if (tokenType == XmlReader::Characters) {
                QString pcData = reader->text().toString();
                if (!pcData.trimmed().isEmpty()) {
                    reader->raiseError(tr("Unexpected PCDATA \"%1\"").arg(pcData));
                }
            } else if (tokenType != XmlReader::EndElement) {
                reader->raiseError(tr("Invalid primitive"));
            }
        } while (!reader->hasError() && tokenType != XmlReader::EndElement);
    }
}
