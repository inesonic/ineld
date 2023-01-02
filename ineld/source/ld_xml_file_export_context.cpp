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
* This file implements the \ref Ld::XmlFileExportContext class.
***********************************************************************************************************************/

#include <QString>
#include <QMap>
#include <QSharedPointer>
#include <QByteArray>
#include <QFileInfo>
#include <QFile>
#include <QDir>

#include "ld_xml_export_context.h"
#include "ld_xml_file_export_context.h"

namespace Ld {
    XmlFileExportContext::XmlFileExportContext(const QString& outputFile, bool allowOverwrite) {
        currentOutputFile.setFile(outputFile);
        currentAllowOverwrite = allowOverwrite;

        QString outputDirectoryName = currentOutputFile.absolutePath();
        QFileInfo outputDirectory(outputDirectoryName);
        if (!outputDirectory.exists()) {
            QFileInfo parentDirectoryInformation(outputDirectory.absolutePath());
            if (parentDirectoryInformation.exists()) {
                if (parentDirectoryInformation.isDir()) {
                    QDir    parentDirectory(parentDirectoryInformation.canonicalFilePath());
                    QString containerDirectoryName = outputDirectory.fileName();

                    if (!parentDirectory.mkdir(containerDirectoryName)) {
                        lastError = tr("Could not create directory \"%1/%2\"")
                                    .arg(parentDirectory.canonicalPath())
                                    .arg(containerDirectoryName);
                    } else {
                        currentOutputDirectory.setPath(parentDirectory.canonicalPath() + "/" + containerDirectoryName);
                    }
                } else {
                    lastError = tr("\"%1\" is not a directory").arg(parentDirectoryInformation.canonicalFilePath());
                }
            }
        } else if (outputDirectory.isDir()) {
            currentOutputDirectory.setPath(outputDirectoryName);
        } else {
            lastError = tr("Invalid output directory \"%1\"").arg(outputDirectory.canonicalFilePath());
        }

        if (lastError.isEmpty()) {
            if (!currentOutputFile.exists() || allowOverwrite) {
                QFile* outputFileDevice = new QFile(currentOutputFile.absoluteFilePath());
                if (outputFileDevice->error() == QFile::FileError::NoError) {
                    if (outputFileDevice->open(QFile::WriteOnly | QFile::Truncate)) {
                        setDevice(outputFileDevice);
                        payloadNames.insert(currentOutputFile.fileName());
                    } else {
                        lastError = outputFileDevice->errorString();
                        delete outputFileDevice;
                    }
                } else {
                    lastError = outputFileDevice->errorString();
                    delete outputFileDevice;
                }
            } else {
                lastError = tr("File \"%1\" already exists.").arg(outputFile);
            }
        }
    }


    XmlFileExportContext::~XmlFileExportContext() {
        device()->close();
        delete device();
    }


    QString XmlFileExportContext::addPayload(const QString& payloadName, const QByteArray& payload) {
        QString result;

        QString payloadFilename = currentOutputDirectory.canonicalPath() + "/" + payloadName;
        QFileInfo payloadFileInformation(payloadFilename);

        if (!payloadFileInformation.exists() || currentAllowOverwrite) {
            QFile payloadFile(payloadFilename);
            if (payloadFile.error() == QFile::FileError::NoError) {
                if (payloadFile.open(QFile::WriteOnly)) {
                    long long bytesWritten = payloadFile.write(payload);
                    if (payloadFile.error() == QFile::FileError::NoError && bytesWritten == payload.size()) {
                        payloadNames.insert(payloadName);

                        payloadFile.close();
                        result = payloadFileInformation.canonicalFilePath();
                    } else {
                        payloadFile.close();
                        lastError = payloadFile.errorString();
                    }
                } else {
                    lastError = payloadFile.errorString();
                }
            } else {
                lastError = payloadFile.errorString();
            }
        } else {
            lastError = tr("Payload \"%1\" already exists").arg(payloadFilename);
        }

        return result;
    }


    QByteArray XmlFileExportContext::payload(const QString& payloadName) {
        QByteArray result;

        if (payloadNames.contains(payloadName)) {
            QString   payloadFilename = currentOutputDirectory.canonicalPath() + "/" + payloadName;
            QFileInfo payloadFileInformation(payloadFilename);

            if (payloadFileInformation.exists()) {
                if (payloadFileInformation == currentOutputFile) {
                    QFile* outputFileDevice = dynamic_cast<QFile*>(device());
                    outputFileDevice->flush();
                }

                QFile payloadFile(payloadFilename);
                if (payloadFile.error() == QFile::FileError::NoError) {
                    if (payloadFile.open(QFile::ReadOnly)) {
                        long long bytesToRead = payloadFile.size();
                        result = payloadFile.read(bytesToRead);

                        if (result.size() != bytesToRead || payloadFile.error() != QFile::FileError::NoError) {
                            lastError = payloadFile.errorString();
                            result.clear();
                        }
                    } else {
                        lastError = payloadFile.errorString();
                        result.clear();
                    }
                } else {
                    lastError = payloadFile.errorString();
                    result.clear();
                }
            } else {
                lastError = tr("Payload \"%1\" file does not exist").arg(payloadFilename);
                result.clear();
            }
        }

        return result;
    }


    QList<QString> XmlFileExportContext::payloads() const {
        return payloadNames.values();
    }


    bool XmlFileExportContext::hasError() const {
        return XmlExportContext::hasError() || !lastError.isEmpty();
    }


    QString XmlFileExportContext::errorString() const {
        QString result;

        if (!lastError.isEmpty()) {
            result = lastError;
        } else {
            result = XmlExportContext::errorString();
        }

        return result;
    }


    bool XmlFileExportContext::close() {
        device()->close();
        return true;
    }
}
