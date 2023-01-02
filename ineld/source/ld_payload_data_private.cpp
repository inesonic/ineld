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
* This file implements the \ref Ld::PayloadData::Private class and derived classes.
***********************************************************************************************************************/

#include <QString>
#include <QPointer>
#include <QByteArray>

#include <qfile_container.h>
#include <qvirtual_file.h>

#include <cstdint>

#include "ld_program_file.h"
#include "ld_payload_data.h"
#include "ld_payload_data_private.h"

/***********************************************************************************************************************
 * Ld::PayloadData::Private
 */

namespace Ld {
    PayloadData::Private::Private() {}


    PayloadData::Private::Private(const Private& other):QSharedData(other) {}


    PayloadData::Private::~Private() {}
}

/***********************************************************************************************************************
 * Ld::PayloadData::PrivateInvalid
 */

namespace Ld {
    PayloadData::PrivateInvalid::PrivateInvalid() {}


    PayloadData::PrivateInvalid::~PrivateInvalid() {}


    PayloadData::Private* PayloadData::PrivateInvalid::clone() const {
        return new PrivateInvalid;
    }


    bool PayloadData::PrivateInvalid::isValid() const {
        return false;
    }


    PayloadData::StorageMode PayloadData::PrivateInvalid::storageMode() const {
        return PayloadData::StorageMode::NOT_STORED;
    }


    PayloadData::PayloadId PayloadData::PrivateInvalid::payloadId() const {
        return PayloadData::invalidPayloadId;
    }


    unsigned PayloadData::PrivateInvalid::numberReferences() const {
        return 0;
    }


    bool PayloadData::PrivateInvalid::writeData(const QByteArray&) {
        return false;
    }

    bool PayloadData::PrivateInvalid::readData(QByteArray&) {
        return false;
    }
}

/***********************************************************************************************************************
 * Ld::PayloadData::PrivateFile
 */

namespace Ld {
    PayloadData::PrivateFile::PrivateFile() {}


    PayloadData::PrivateFile::PrivateFile(ProgramFile* programFile, PayloadData::PayloadId payloadId) {
        currentProgramFile = programFile;
        currentPayloadId   = payloadId;
    }


    PayloadData::PrivateFile::~PrivateFile() {}


    PayloadData::Private* PayloadData::PrivateFile::clone() const {
        return new PrivateFile(currentProgramFile, currentPayloadId);
    }


    bool PayloadData::PrivateFile::isValid() const {
        return true;
    }


    PayloadData::StorageMode PayloadData::PrivateFile::storageMode() const {
        return PayloadData::StorageMode::STORED_IN_FILE;
    }


    PayloadData::PayloadId PayloadData::PrivateFile::payloadId() const {
        return currentPayloadId;
    }


    unsigned PayloadData::PrivateFile::numberReferences() const {
        return currentProgramFile->payloadReferenceCounts.value(currentPayloadId);
    }


    bool PayloadData::PrivateFile::writeData(const QByteArray& newData) {
        bool success;

        QString payloadName = ProgramFile::payloadName(currentPayloadId);
        QPointer<QVirtualFile> virtualFile;

        if (currentProgramFile->container->directory().contains(payloadName)) {
            virtualFile = currentProgramFile->container->directory().value(payloadName);
            success     = true;
        } else {
            virtualFile = currentProgramFile->container->newVirtualFile(payloadName);
            success     = !virtualFile.isNull();
        }

        if (success) {
            success = virtualFile->open(QVirtualFile::WriteOnly);
        }

        if (success) {
            unsigned long long writeCount = virtualFile->write(newData);
            success = (writeCount == static_cast<unsigned long long>(newData.size()));
        }

        if (!virtualFile.isNull()) {
            virtualFile->close();
        }

        return success;
    }


    bool PayloadData::PrivateFile::readData(QByteArray& newData) {
        bool success;

        QString payloadName = ProgramFile::payloadName(currentPayloadId);

        if (currentProgramFile->container->directory().contains(payloadName)) {
            QPointer<QVirtualFile> virtualFile = currentProgramFile->container->directory().value(payloadName);

            success = !virtualFile.isNull();

            if (success) {
                success = virtualFile->open(QVirtualFile::ReadOnly);
            }

            if (success) {
                newData = virtualFile->readAll();

                long long fileSize = virtualFile->size();
                success = (fileSize == static_cast<long long>(newData.size()));
            }

            if (!virtualFile.isNull()) {
                virtualFile->close();
            }
        } else {
            success = false;
        }

        return success;
    }


    ProgramFile* PayloadData::PrivateFile::programFile() const {
        return currentProgramFile;
    }
}

/***********************************************************************************************************************
 * Ld::PayloadData::PrivateLocal
 */

namespace Ld {
    PayloadData::PrivateLocal::PrivateLocal() {}


    PayloadData::PrivateLocal::~PrivateLocal() {}


    PayloadData::Private* PayloadData::PrivateLocal::clone() const {
        PrivateLocal* cloned = new PrivateLocal;
        cloned->currentData = currentData;

        return cloned;
    }


    bool PayloadData::PrivateLocal::isValid() const {
        return true;
    }


    PayloadData::StorageMode PayloadData::PrivateLocal::storageMode() const {
        return PayloadData::StorageMode::STORED_IN_MEMORY;
    }


    PayloadData::PayloadId PayloadData::PrivateLocal::payloadId() const {
        return PayloadData::invalidPayloadId;
    }


    unsigned PayloadData::PrivateLocal::numberReferences() const {
        return 0;
    }


    bool PayloadData::PrivateLocal::writeData(const QByteArray& newData) {
        currentData = newData;
        return true;
    }


    bool PayloadData::PrivateLocal::readData(QByteArray& newData) {
        newData = currentData;
        return true;
    }
}
