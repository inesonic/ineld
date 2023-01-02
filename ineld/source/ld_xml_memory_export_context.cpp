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
* This file implements the \ref Ld::XmlMemoryExportContext class.
***********************************************************************************************************************/

#include <QString>
#include <QMap>
#include <QSharedPointer>
#include <QByteArray>
#include <QBuffer>

#include "ld_xml_export_context.h"
#include "ld_xml_memory_export_context.h"

namespace Ld {
    XmlMemoryExportContext::XmlMemoryExportContext(const QString& outputFile) {
        QSharedPointer<QByteArray> byteArray(new QByteArray);

        QBuffer* newBuffer = new QBuffer(byteArray.data());
        newBuffer->open(QBuffer::ReadWrite);

        setDevice(newBuffer);

        payloadsByName.insert(outputFile, byteArray);
    }


    XmlMemoryExportContext::~XmlMemoryExportContext() {
        device()->close();
        delete device();
    }


    QString XmlMemoryExportContext::addPayload(const QString& payloadName, const QByteArray& payload) {
        bool result;

        if (!payloadsByName.contains(payloadName)) {
            payloadsByName.insert(payloadName, QSharedPointer<QByteArray>(new QByteArray(payload)));
            result = true;
        } else {
            result = false;
            lastPayloadError = tr("Payload \"%1\" already exists").arg(payloadName);
        }

        return result ? payloadName : QString();
    }


    QByteArray XmlMemoryExportContext::payload(const QString& payloadName) {
        QByteArray result;

        if (payloadsByName.contains(payloadName)) {
            result = *payloadsByName.value(payloadName);
        }

        return result;
    }


    QList<QString> XmlMemoryExportContext::payloads() const {
        return payloadsByName.keys();
    }


    bool XmlMemoryExportContext::hasError() const {
        return XmlExportContext::hasError() || !lastPayloadError.isEmpty();
    }


    QString XmlMemoryExportContext::errorString() const {
        QString result;

        if (!lastPayloadError.isEmpty()) {
            result = lastPayloadError;
        } else {
            result = XmlExportContext::errorString();
        }

        return result;
    }
}
