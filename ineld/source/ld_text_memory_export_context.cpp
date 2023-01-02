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
* This file implements the \ref Ld::TextExportContext class.
***********************************************************************************************************************/

#include <QString>
#include <QIODevice>
#include <QByteArray>
#include <QBuffer>

#include <cstring>

#include "ld_text_export_context.h"
#include "ld_text_memory_export_context.h"

namespace Ld {
    TextMemoryExportContext::TextMemoryExportContext(const QString& outputFile, bool savePayloads) {
        currentOutputFilename = outputFile;
        QBuffer* buffer = new QBuffer(&currentOutputFile);
        bool success = buffer->open(QBuffer::WriteOnly);

        if (success) {
            setDevice(buffer);
            currentSavePayloads = savePayloads;
        } else {
            delete buffer;
        }
    }


    TextMemoryExportContext::~TextMemoryExportContext() {}


    QString TextMemoryExportContext::addPayload(const QString& payloadName, const QByteArray& payload) {
        QString result;

        if (payloadName != currentOutputFile && !payloadsByName.contains(payloadName)) {
            payloadsByName.insert(payloadName, currentSavePayloads ? payload : QByteArray());
            result = payloadName;
        } else {
            setError(tr("Duplicate payload \"%1\"").arg(payloadName));
        }

        return result;
    }


    QByteArray TextMemoryExportContext::payload(const QString& payloadName) {
        QByteArray result;

        if (payloadName == currentOutputFilename) {
            result = currentOutputFile;
        } else if (payloadsByName.contains(payloadName)) {
            result = payloadsByName.value(payloadName);
        }

        return result;
    }


    QList<QString> TextMemoryExportContext::payloads() const {
        QList<QString> result = payloadsByName.keys();
        result.prepend(currentOutputFilename);

        return result;
    }
}
