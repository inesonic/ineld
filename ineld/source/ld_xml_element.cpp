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
* This file implements the \ref Ld::XmlElement class.
***********************************************************************************************************************/

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QSharedPointer>
#include <QMap>

#include "ld_handle.h"
#include "ld_program_file.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_format_container.h"
#include "ld_xml_element.h"

namespace Ld {
    XmlElement::XmlElement() {};


    XmlElement::~XmlElement() {};


    void XmlElement::writeXml(
            QSharedPointer<XmlWriter>       writer,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile,
            const XmlAttributes&            inheritedAttributes
        ) const {
        XmlAttributes attributes = inheritedAttributes;
        writeAddAttributes(attributes, formats, programFile);

        writer->writeStartElement(typeName());
        writer->writeAttributes(attributes);

        QString elementData = writeData(formats, programFile);
        if (!elementData.isEmpty()) {
            writer->writeCharacters(elementData);
        }

        XmlAttributes childAttributes;
        writeChildren(writer, formats, programFile, childAttributes);

        writer->writeEndElement();
    }


    void XmlElement::readXml(
            QSharedPointer<XmlReader>  reader,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            const XmlAttributes&       attributes,
            unsigned                   xmlVersion
        ) {
        if (!attributes.isEmpty()) {
            readAttributes(reader, attributes, formats, programFile, xmlVersion);
        }

        if (!reader->atEnd() && !reader->hasError()) {
            XmlReader::TokenType tokenType;

            do {
                tokenType = reader->readNext();

                if (tokenType == XmlReader::Characters) {
                    QString pcData = reader->text().toString();
                    readData(reader, pcData, formats, programFile, xmlVersion);
                } else if (tokenType == XmlReader::StartElement) {
                    QString       childToken      = reader->qualifiedName().toString();
                    XmlAttributes childAttributes = reader->attributes();
                    readChild(reader, childToken, formats, programFile, childAttributes, xmlVersion);
                } else if (tokenType != XmlReader::EndElement) {
                    reader->raiseError(tr("Invalid primitive"));
                }
            } while (!reader->hasError() && tokenType != XmlReader::EndElement);
        }
    }


    QString XmlElement::writeData(QSharedPointer<const FormatOrganizer>, ProgramFile&) const {
        return QString();
    }


    void XmlElement::writeAddAttributes(XmlAttributes&, QSharedPointer<FormatOrganizer>, ProgramFile&) const {}


    void XmlElement::readAttributes(
            QSharedPointer<XmlReader> reader,
            const XmlAttributes&,
            const FormatsByIdentifier&,
            ProgramFile&,
            unsigned
        ) {
        reader->raiseError(tr("Tag \"%1\" does not allow attributes").arg(reader->qualifiedName().toString()));
    }


    void XmlElement::readData(
            QSharedPointer<XmlReader>  reader,
            const QString&             text,
            const FormatsByIdentifier&,
            ProgramFile&,
            unsigned
        ) {
        if (!text.trimmed().isEmpty()) {
            reader->raiseError(tr("Tag \"%1\" does not allow text").arg(reader->qualifiedName().toString()));
        }
    }


    void XmlElement::readChild(
            QSharedPointer<XmlReader> reader,
            const QString&,
            const FormatsByIdentifier&,
            ProgramFile&,
            const XmlAttributes&,
            unsigned
        ) {
        reader->raiseError(tr("Tag \"%1\" does not child tags").arg(reader->qualifiedName().toString()));
    }
}
