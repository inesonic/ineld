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
* This file implements the \ref Ld::DocumentSetting class.
***********************************************************************************************************************/

#include <QString>
#include <QMap>
#include <QList>
#include <QSet>
#include <QHash>
#include <QSharedPointer>
#include <QWeakPointer>

#include <util_hash_functions.h>

#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_xml_element.h"
#include "ld_document_setting.h"

namespace Ld {
    QMap<QString, DocumentSetting::CreatorFunction> DocumentSetting::creators;

    QSharedPointer<DocumentSetting> DocumentSetting::create(const QString& typeName) {
        DocumentSetting*         newDocumentSetting       = nullptr;
        CreatorFunction creatorFunction = creators.value(typeName);

        if (creatorFunction != nullptr) {
            newDocumentSetting = (*creatorFunction)(typeName);
        }

        return QSharedPointer<DocumentSetting>(newDocumentSetting);
    }


    bool DocumentSetting::registerCreator(const QString& typeName, DocumentSetting::CreatorFunction creatorFunction) {
        bool success;

        if (creatorFunction != nullptr && !creators.contains(typeName)) {
            creators.insert(typeName, creatorFunction);
            success = true;
        } else {
            success = false;
        }

        return success;
    }


    DocumentSetting::DocumentSetting() {}


    DocumentSetting::~DocumentSetting() {}


    bool DocumentSetting::isInvalid() const {
        return !isValid();
    }


    void DocumentSetting::writeXml(QSharedPointer<XmlWriter> writer) {
        XmlAttributes attributes;
        writeAddAttributes(attributes);

        writer->writeStartElement(typeName());
        writer->writeAttributes(attributes);

        QString elementData = writeData();
        if (!elementData.isEmpty()) {
            writer->writeCharacters(elementData);
        }

        writer->writeEndElement();
    }


    void DocumentSetting::readXml(
            QSharedPointer<XmlReader> reader,
            const XmlAttributes&      attributes,
            unsigned                  xmlVersion
        ) {
        if (!attributes.isEmpty()) {
            readAttributes(reader, attributes, xmlVersion);
        }

        if (!reader->atEnd() && !reader->hasError()) {
            XmlReader::TokenType tokenType;

            do {
                tokenType = reader->readNext();

                if (tokenType == XmlReader::Characters) {
                    QString pcData = reader->text().toString();
                    readData(reader, pcData, xmlVersion);
                } else if (tokenType == XmlReader::StartElement) {
                    QString childToken = reader->qualifiedName().toString();
                    reader->raiseError(tr("Unexpected token \"%1\"").arg(childToken));
                } else if (tokenType != XmlReader::EndElement) {
                    reader->raiseError(tr("Invalid primitive"));
                }
            } while (!reader->hasError() && tokenType != XmlReader::EndElement);
        }
    }


    QString DocumentSetting::writeData() {
        return QString();
    }


    void DocumentSetting::writeAddAttributes(XmlAttributes&) {}


    void DocumentSetting::readAttributes(QSharedPointer<XmlReader> reader, const XmlAttributes&, unsigned) {
        reader->raiseError(tr("Tag \"%1\" does not allow attributes").arg(reader->qualifiedName().toString()));
    }


    void DocumentSetting::readData(QSharedPointer<XmlReader> reader, const QString& text, unsigned) {
        if (!text.trimmed().isEmpty()) {
            reader->raiseError(tr("Tag \"%1\" does not allow text").arg(reader->qualifiedName().toString()));
        }
    }
}


Util::HashResult qHash(QSharedPointer<Ld::DocumentSetting> documentSetting, Util::HashSeed seed) {
    return qHash(documentSetting.data(), seed);
}


Util::HashResult qHash(QWeakPointer<Ld::DocumentSetting> documentSetting, Util::HashSeed seed) {
    return qHash(documentSetting.toStrongRef().data(), seed);
}
