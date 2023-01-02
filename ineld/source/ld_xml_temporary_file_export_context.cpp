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
* This file implements the \ref Ld::XmlTemporaryFileExportContext class.
***********************************************************************************************************************/

#include <QString>
#include <QMap>
#include <QSharedPointer>
#include <QByteArray>
#include <QFileInfo>
#include <QBuffer>
#include <QDir>
#include <QTemporaryFile>

#include "ld_xml_export_context.h"
#include "ld_xml_temporary_file_export_context.h"

namespace Ld {
    XmlTemporaryFileExportContext::XmlTemporaryFileExportContext(const QString& outputFile) {
        QBuffer* newBuffer = new QBuffer(&currentXmlData);
        newBuffer->open(QBuffer::ReadWrite);

        setDevice(newBuffer);
        currentXmlPayloadName = outputFile;
    }


    XmlTemporaryFileExportContext::~XmlTemporaryFileExportContext() {
        device()->close();
        delete device();
    }


    QString XmlTemporaryFileExportContext::addPayload(const QString& payloadName, const QByteArray& payload) {
        QString result;

        if (!payloadsByName.contains(payloadName) && payloadName != currentXmlPayloadName) {
            QString templateFilename = QDir::tempPath() + "/" + QString("XXXXXX%1").arg(payloadName);
            QSharedPointer<QTemporaryFile> temporaryFile(new QTemporaryFile(templateFilename));
            if (temporaryFile->open()) {
                long long bytesWritten = temporaryFile->write(payload);
                if (temporaryFile->error() == QFile::FileError::NoError && bytesWritten == payload.size()) {
                    temporaryFile->close();
                    result = temporaryFile->fileName();

                    payloadsByName.insert(payloadName, temporaryFile);
                } else {
                    temporaryFile->close();
                    lastPayloadError = temporaryFile->errorString();
                }
            } else {
                lastPayloadError = temporaryFile->errorString();
            }
        } else {
            lastPayloadError = tr("Payload \"%1\" already exists").arg(payloadName);
        }

        return result;
    }


    QByteArray XmlTemporaryFileExportContext::payload(const QString& payloadName) {
        QByteArray result;

        if (payloadName == currentXmlPayloadName) {
            result = currentXmlData;
        } else if (payloadsByName.contains(payloadName)) {
            QSharedPointer<QTemporaryFile> temporaryFile = payloadsByName.value(payloadName);
            if (temporaryFile->open()) {
                long long bytesToRead = temporaryFile->size();
                result = temporaryFile->read(bytesToRead);

                if (result.size() != bytesToRead || temporaryFile->error() != QFile::FileError::NoError) {
                    lastPayloadError = temporaryFile->errorString();
                    result.clear();
                }

                temporaryFile->close();
            } else {
                lastPayloadError = temporaryFile->errorString();
            }
        } else {
            lastPayloadError = tr("Payload \"%1\" file does not exist").arg(payloadName);
            result.clear();
        }

        return result;
    }


    QList<QString> XmlTemporaryFileExportContext::payloads() const {
        QList<QString> payloadNames = payloadsByName.keys();
        payloadNames.prepend(currentXmlPayloadName);

        return payloadNames;
    }


    bool XmlTemporaryFileExportContext::hasError() const {
        return XmlExportContext::hasError() || !lastPayloadError.isEmpty();
    }


    QString XmlTemporaryFileExportContext::errorString() const {
        QString result;

        if (!lastPayloadError.isEmpty()) {
            result = lastPayloadError;
        } else {
            result = XmlExportContext::errorString();
        }

        return result;
    }


    bool XmlTemporaryFileExportContext::close() {
        device()->close();
        return true;
    }
}
