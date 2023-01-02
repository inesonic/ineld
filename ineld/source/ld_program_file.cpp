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
* This file implements the \ref Ld::ProgramFile class.
***********************************************************************************************************************/

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro and QCoreApplication::applicationName().
#include <QDateTime>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QImage>
#include <QImageReader>
#include <QPointer>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QFile>
#include <QTemporaryFile>
#include <QDir>
#include <QFileInfo>
#include <QByteArray>
#include <QBuffer>
#include <QByteArray>

#if (defined(Q_OS_WIN))

    #include <Windows.h>

#endif

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cerrno>
#include <algorithm>

#include <util_system.h>

#include <qfile_container.h>
#include <qvirtual_file.h>

#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_payload_data.h"
#include "ld_handle.h"
#include "ld_program_file.h"

namespace Ld {
    const QString ProgramFile::fileIdentifier = QString("Inesonic, LLC.\nProgram Container");

    ProgramFile::ProgramFile():container(new QFileContainer(ProgramFile::fileIdentifier)) {}


    ProgramFile::~ProgramFile() {
        movePayloadsToLocalStorageAndDisconnect();
    }


    bool ProgramFile::openExisting(const QString& filename, bool readOnly) {
        bool success;

        if (container->openMode() != QFileContainer::OpenMode::CLOSED) {
            success = false;
            lastError = tr("Container currently open.");
        } else {
            success = true;
        }

        QString workingFilename;

        if (success) {
            QFileInfo fileInformation(filename);

            if (readOnly) {
                workingFilename = fileInformation.absoluteFilePath();
            } else {
                QString templateFilename = QString("%1/.~%2.XXXXXX.tmp")
                                           .arg(QDir::tempPath(), fileInformation.fileName());

                QTemporaryFile workingFile(templateFilename);
                workingFile.setAutoRemove(false);

                success = workingFile.open();
                if (success) {
                    workingFilename = workingFile.fileName();

                    QFile originalFile(filename);
                    success = originalFile.open(QFile::OpenModeFlag::ReadOnly);
                    if (success) {
                        long long fileSizeInBytes = originalFile.size();
                        long long remainingBytes  = fileSizeInBytes;
                        while (success && remainingBytes != 0) {
                            long long bytesToRead = remainingBytes;
                            if (bytesToRead > 16384) {
                                bytesToRead = 16384;
                            }

                            QByteArray readData = originalFile.read(bytesToRead);
                            if (readData.size() == bytesToRead) {
                                long long bytesWritten = workingFile.write(readData);
                                success = (bytesToRead == bytesWritten);

                                if (success) {
                                    remainingBytes -= bytesToRead;
                                } else {
                                    lastError = tr("Could not create working file for %1: %2")
                                                .arg(fileInformation.absoluteFilePath(), workingFile.errorString());
                                }
                            } else {
                                lastError = tr("Could not create working file for %1: %2")
                                            .arg(fileInformation.absoluteFilePath(), originalFile.errorString());
                                success = false;
                            }
                        }

                        originalFile.close();
                    }

                    workingFile.close();
                } else {
                    lastError = tr("Could not create working file for %1: %2")
                                .arg(fileInformation.absoluteFilePath(), workingFile.errorString());
                }
            }
        }

        if (success) {
            success = container->open(
                workingFilename,
                readOnly ? QFileContainer::OpenMode::READ_ONLY : QFileContainer::OpenMode::READ_WRITE
            );

            if (!success) {
                lastError = container->errorString();
            } else {
                containerFileInformation.setFile(filename);
                resetState();
                isNewContainer = false;
            }
        }

        return success;
    }


    bool ProgramFile::openNew() {
        bool success;

        if (container->openMode() != QFileContainer::OpenMode::CLOSED) {
            success = false;
            lastError = tr("Container currently open.");
        } else {
            QString userName          = Util::username();
            QString temporaryFilename = QString("%1/%2-%3-%4.tmp")
                                        .arg(QDir::tempPath(), QCoreApplication::applicationName(), userName)
                                        .arg(QDateTime::currentMSecsSinceEpoch());

            success = container->open(temporaryFilename, ProgramFile::OpenMode::OVERWRITE);

            if (!success) {
                lastError = tr("Could not create temporary file %1").arg(temporaryFilename);
            }
        }

        if (success) {
            containerFileInformation = QFileInfo();

            resetState();
            isNewContainer = true;
        }

        return success;
    }


    bool ProgramFile::save() {
        bool success;

        if (container->openMode() == QFileContainer::OpenMode::CLOSED) {
            success = false;
            lastError = tr("Container closed.");
        } else if (container->openMode() == QFileContainer::OpenMode::READ_ONLY){
            success = false;
            lastError = tr("Container opened read-only.  Can not save.");
        } else if (isNewContainer) {
            success = false;
            lastError = tr("New container must be saved \"as\".");
        } else {
            QString containerFilename = container->filename();
            success = container->close();

            if (!success) {
                lastError = container->errorString();
            } else {
                success = renameContainer(
                    containerFilename.toLocal8Bit().constData(),
                    containerFileInformation.absoluteFilePath().toLocal8Bit().constData()
                );
            }

            if (success) {
                success = openExisting(containerFileInformation.absoluteFilePath(), false);
            }
        }

        return success;
    }


    bool ProgramFile::saveAs(const QString& newFilename) {
        bool success;

        if (container->openMode() == QFileContainer::OpenMode::CLOSED) {
            success = false;
            lastError = tr("Container closed.");
        } else {
            QFileInfo newFileInformation(newFilename);
            QString saveFilePath = newFileInformation.absoluteFilePath();

            QString containerFilename = container->filename();
            success = container->close();

            if (!success) {
                lastError = container->errorString();
            } else {
                success = renameContainer(
                    containerFilename.toLocal8Bit().constData(),
                    saveFilePath.toLocal8Bit().constData()
                );
            }

            if (success) {
                success = openExisting(saveFilePath, false);
            }
        }

        return success;
    }


    bool ProgramFile::close() {
        bool success;

        if (container->openMode() == QFileContainer::OpenMode::CLOSED) {
            success = false;
            lastError = tr("Container already closed.");
        } else {
            QString                  containerFilename = container->filename();
            QFileContainer::OpenMode openMode          = container->openMode();

            success = movePayloadsToLocalStorageAndDisconnect();

            if (success) {
                success = container->close();
            }

            if (!success) {
                lastError = container->errorString();
            } else {
                if (openMode != QFileContainer::OpenMode::READ_ONLY) {
                    success = QFile(containerFilename).remove();

                    if (!success) {
                        lastError = tr("Could not delete working file.");
                    }
                }

                if (success) {
                    resetState();
                }
            }
        }

        return success;
    }


    QString ProgramFile::filename() const {
        return containerFileInformation.absoluteFilePath();
    }


    ProgramFile::OpenMode ProgramFile::openMode() const {
        return container->openMode();
    }


    PayloadData ProgramFile::payload(PayloadData::PayloadId payloadId) {
        PayloadData result;

        if (payloadReferenceCounts.contains(payloadId)) {
            QSet<PayloadData*>::const_iterator it  = payloadDataInstances.constBegin();
            QSet<PayloadData*>::const_iterator end = payloadDataInstances.constEnd();

            while (it != end && (*it)->payloadId() != payloadId) {
                ++it;
            }

            if (it != end) {
                result = **it;
            }
        } else {
            if (container->directory().contains(payloadName(payloadId))) {
                result = PayloadData(this, payloadId);
            }
        }

        return result;
    }


    PayloadData ProgramFile::newPayload() {
        PayloadData::PayloadId newPayloadId;

        if (container->openMode() != QFileContainer::OpenMode::CLOSED    &&
            container->openMode() != QFileContainer::OpenMode::READ_ONLY    ) {
            bool newPayloadIdFound = false;
            newPayloadId = 0;

            do {
                while (payloadReferenceCounts.contains(newPayloadId)) {
                    ++newPayloadId;
                }

                if (container->directory().contains(payloadName(newPayloadId))) {
                    ++newPayloadId;
                } else {
                    newPayloadIdFound = true;
                }
            } while (!newPayloadIdFound);
        } else {
            newPayloadId = PayloadData::invalidPayloadId;
        }

        PayloadData result(this, newPayloadId);
        return result;
    }


    PayloadData ProgramFile::clonePayload(PayloadData& payloadData) {
        PayloadData newPayload;

        if (payloadData.isValid()) {
            QByteArray data;
            bool success = payloadData.readData(data);
            if (success) {
                newPayload = ProgramFile::newPayload();

                if (newPayload.isValid()) {
                    success = newPayload.writeData(data);
                    if (!success) {
                        newPayload = PayloadData();
                    }
                }
            }
        }

        return newPayload;
    }


    bool ProgramFile::purgeUnreferencedPayloads() {
        typedef QHash<PayloadData::PayloadId, unsigned long> ReferenceHash;
        typedef ReferenceHash::const_iterator                ReferenceHashIterator;
        typedef QList<PayloadData::PayloadId>                PayloadIdList;
        typedef PayloadIdList::const_iterator                PayloadIdListIterator;

        bool success = true;

        PayloadIdList unusedPayloads;

        for (  ReferenceHashIterator referenceCountIterator    = payloadReferenceCounts.constBegin(),
                                     referenceCountEndIterator = payloadReferenceCounts.constEnd()
             ; referenceCountIterator != referenceCountEndIterator
             ; ++referenceCountIterator
            ){
            PayloadData::PayloadId payloadId      = referenceCountIterator.key();
            unsigned long          referenceCount = referenceCountIterator.value();

            if (referenceCount == 0) {
                unusedPayloads.append(payloadId);
            }
        }

        for (  PayloadIdListIterator payloadIdIterator    = unusedPayloads.constBegin(),
                                     payloadIdEndIterator = unusedPayloads.constEnd()
             ; payloadIdIterator != payloadIdEndIterator
             ; ++payloadIdIterator
            ) {
            PayloadData::PayloadId payloadId = *payloadIdIterator;
            QString                name      = payloadName(payloadId);

            if (container->directory().contains(name)) {
                QPointer<QVirtualFile> virtualFile = container->directory().value(name);
                bool erased = virtualFile->erase();
                if (!erased) {
                    success = false;
                }
            }
        }

        return success;
    }


    QString ProgramFile::errorString() const {
        return lastError;
    }


    bool ProgramFile::initializeXmlReader(const QString& streamName) {
        bool success = true;

        if (container->openMode() == QFileContainer::OpenMode::CLOSED) {
            success = false;
            lastError = tr("Container not open.");
        } else {
            if (!currentDevice.isNull()) {
                success = false;
                lastError = tr("Stream device already open.");
            } else {
                QString                      internalFilename = QString("%1.xml").arg(streamName);
                QFileContainer::DirectoryMap directory        = container->directory();

                if (directory.contains(internalFilename)) {
                    currentDevice = directory.value(internalFilename);

                    success = currentDevice->open(QVirtualFile::ReadOnly);
                    if (!success) {
                        lastError = currentDevice->errorString();
                    } else {
                        currentReader = QSharedPointer<XmlReader>(new XmlReader(currentDevice));
                    }
                } else {
                    lastError = tr("Container does not hold stream %1.").arg(streamName);
                    success   = false;
                }
            }
        }

        return success;
    }


    QSharedPointer<XmlReader> ProgramFile::reader() {
        return currentReader;
    }


    bool ProgramFile::releaseXmlReader() {
        bool success = true;

        if (container->openMode() == QFileContainer::OpenMode::CLOSED) {
            success = false;
            lastError = tr("Container not open.");
        } else {
            if (currentDevice.isNull()) {
                success = false;
                lastError = tr("XML reader not active (1).");
            } else if (currentReader.isNull()) {
                success = false;
                lastError = tr("XML reader not active (2).");
            } else {
                currentReader.clear();

                currentDevice->close();
                currentDevice.clear();
            }
        }

        return success;
    }


    bool ProgramFile::initializeXmlWriter(const QString& streamName) {
        bool success = true;

        if (container->openMode() == QFileContainer::OpenMode::CLOSED) {
            success = false;
            lastError = tr("Container not open.");
        } else {
            if (!currentDevice.isNull()) {
                success = false;
                lastError = tr("Stream device already open.");
            } else {
                QString                      internalFilename = QString("%1.xml").arg(streamName);
                QFileContainer::DirectoryMap directory        = container->directory();

                if (directory.contains(internalFilename)) {
                    QPointer<QVirtualFile> vf = directory.value(internalFilename);
                    success = vf->erase();

                    if (!success) {
                        lastError = tr("Could not erase stream %1").arg(streamName);
                    }
                }

                if (success) {
                    currentDevice = container->newVirtualFile(internalFilename);
                    if (currentDevice.isNull()) {
                        success = false;
                        lastError = container->errorString();
                    }
                }

                if (success) {
                    success = currentDevice->open(QVirtualFile::WriteOnly);
                    if (!success) {
                        lastError = currentDevice->errorString();
                    }
                }

                if (success) {
                    currentWriter = QSharedPointer<XmlWriter>(new XmlWriter(currentDevice));
                } else {
                    lastError = tr("Can not create stream %1.").arg(streamName);
                    success   = false;
                }

            }
        }

        return success;
    }


    QSharedPointer<XmlWriter> ProgramFile::writer() {
        return currentWriter;
    }


    bool ProgramFile::releaseXmlWriter() {
        bool success = true;

        if (container->openMode() == QFileContainer::OpenMode::CLOSED) {
            success = false;
            lastError = tr("Container not open.");
        } else {
            if (currentDevice.isNull()) {
                success = false;
                lastError = tr("XML writer not active (1).");
            } else if (currentWriter.isNull()) {
                success = false;
                lastError = tr("XML writer not active (2).");
            } else {
                currentWriter.clear();

                currentDevice->close();
                currentDevice.clear();
            }
        }

        return success;
    }


    void ProgramFile::resetState() {
        currentReader.clear();
        currentWriter.clear();
    }


//    bool ProgramFile::loadFile(const QString& filename, QByteArray& fileBuffer) {
//        QFile file(filename);
//        bool success = file.open(QFile::ReadOnly);

//        if (!success) {
//            lastError = file.errorString();
//        } else {
//            fileBuffer = file.readAll();
//            if (fileBuffer.isEmpty() && file.size() > 0) {
//                success = false;
//                lastError = file.errorString();
//            }
//        }

//        return success;
//    }


    bool ProgramFile::renameContainer(const QString& oldName, const QString& newName) {
        bool success = true;

        // ANSI-C rename is atomic and automatically handles overwrite scenarios cleanly across the entire system.
        // Qt's QFile::rename method is not atomic and doesn't handle overwrite situations as desired.

        int result = std::rename(oldName.toLocal8Bit().constData(), newName.toLocal8Bit().constData());

        if (result != 0) {
            // Fall-back to copy and delete if the rename above fails -- may happen across drives.

            QFile newFile(newName);
            if (newFile.exists()) {
                success = newFile.remove();
                if (!success) {
                    lastError = newFile.errorString();
                }
            }

            if (success) {
                success = QFile::copy(oldName, newName);
                if (!success) {
                    lastError = tr("Could not overwrite %1 with new content.").arg(newName);
                } else {
                    success = QFile(oldName).remove();
                    if (!success) {
                        lastError = tr("Could not remove temporary file %1").arg(oldName);
                    }
                }
            }
        }

        return success;
    }


    bool ProgramFile::movePayloadsToLocalStorageAndDisconnect() {
        bool success = true;

        QList<PayloadData*> payloads = payloadDataInstances.values();
        for (  QList<PayloadData*>::const_iterator payloadIterator    = payloads.constBegin(),
                                                   payloadEndIterator = payloads.constEnd()
             ; payloadIterator != payloadEndIterator
             ; ++payloadIterator
            ) {
            PayloadData* payloadData = *payloadIterator;

            bool successThisPayload = payloadData->moveToLocalStorageAndDisconnect();
            if (!successThisPayload) {
                success = false;
            }
        }

        return success;
    }


    void ProgramFile::increaseReferenceCount(PayloadData::PayloadId payloadId) {
        unsigned long count = payloadReferenceCounts.value(payloadId, 0U);
        payloadReferenceCounts.insert(payloadId, count + 1);
    }


    unsigned long ProgramFile::decreaseReferenceCount(PayloadData::PayloadId payloadId) {
        unsigned long count = payloadReferenceCounts.value(payloadId, 0U);

        if (count > 0) {
            payloadReferenceCounts.insert(payloadId, count - 1);
        }

        return count;
    }


    QString ProgramFile::payloadName(PayloadData::PayloadId payloadId) {
        QString name = QString::number(payloadId, 16);
        while (name.length() < 8) {
            name = QString("0") + name;
        }

        name = QString("P") + name;

        return name;
    }
}
