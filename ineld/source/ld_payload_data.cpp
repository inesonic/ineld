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
* This file implements the \ref Ld::PayloadData class.
***********************************************************************************************************************/

#include <QString>
#include <QPointer>
#include <QByteArray>

#include <qfile_container.h>
#include <qvirtual_file.h>

#include <cstdint>

#include "ld_program_file.h"
#include "ld_payload_data_private.h"
#include "ld_payload_data.h"

template<> Ld::PayloadData::Private* QSharedDataPointer<Ld::PayloadData::Private>::clone() {
    return d->clone();
}


namespace Ld {
    const PayloadData::PayloadId PayloadData::invalidPayloadId = static_cast<PayloadData::PayloadId>(-1);

    PayloadData::PayloadData() {
        impl = new PayloadData::PrivateInvalid;
    }


    PayloadData::PayloadData(const PayloadData& other):impl(other.impl) {
        connectToProgramFile();
    }


    PayloadData::~PayloadData() {
        disconnectFromProgramFile();
    }


    bool PayloadData::isValid() const {
        return impl->isValid();
    }


    bool PayloadData::isInvalid() const {
        return !impl->isValid();
    }


    PayloadData::StorageMode PayloadData::storageMode() const {
        return impl->storageMode();
    }


    PayloadData::PayloadId PayloadData::payloadId() const {
        return impl->payloadId();
    }


    unsigned PayloadData::numberReferences() const {
        return impl->numberReferences();
    }


    bool PayloadData::writeData(const QByteArray& newData) {
        if (impl->storageMode() == StorageMode::NOT_STORED) {
            impl = new PrivateLocal;
        }

        return impl->writeData(newData);
    }


    bool PayloadData::readData(QByteArray& newData) {
        return impl->readData(newData);
    }


    PayloadData& PayloadData::operator=(const PayloadData& other) {
        disconnectFromProgramFile();
        impl = other.impl;
        connectToProgramFile();

        return *this;
    }


    PayloadData::PayloadData(ProgramFile* programFile, PayloadId payloadId) {
        impl = new PrivateFile(programFile, payloadId);
        connectToProgramFile();
    }


    void PayloadData::makeInvalid() {
        impl = new PrivateInvalid();
    }


    void PayloadData::connectToProgramFile() {
        PayloadId payloadId = impl->payloadId();
        if (payloadId != invalidPayloadId) {
            PrivateFile* privateFile = dynamic_cast<PrivateFile*>(impl.data());
            assert(privateFile != nullptr);

            ProgramFile* programFile = privateFile->programFile();
            assert(programFile != nullptr);

            programFile->payloadDataInstances.insert(this);
            programFile->increaseReferenceCount(payloadId);
        }
    }


    void PayloadData::disconnectFromProgramFile() {
        PayloadId payloadId = impl->payloadId();
        if (payloadId != invalidPayloadId) {
            PrivateFile* privateFile = dynamic_cast<PrivateFile*>(impl.data());
            assert(privateFile != nullptr);

            ProgramFile* programFile = privateFile->programFile();
            assert(programFile != nullptr);

            programFile->payloadDataInstances.remove(this);
            programFile->decreaseReferenceCount(payloadId);
        }
    }


    bool PayloadData::moveToLocalStorageAndDisconnect() {
        bool      success;
        PayloadId payloadId = impl->payloadId();

        if (payloadId != invalidPayloadId) {
            PrivateFile* privateFile = dynamic_cast<PrivateFile*>(impl.data());
            assert(privateFile != nullptr);

            ProgramFile* programFile = privateFile->programFile();
            assert(programFile != nullptr);

            QByteArray data;
            success = privateFile->readData(data);

            if (success) {
                PrivateLocal* localVersion = new PrivateLocal;
                success = localVersion->writeData(data);

                if (success) {
                    impl = localVersion;
                } else {
                    delete localVersion;
                    impl = new PrivateInvalid;
                }
            } else {
                impl = new PrivateInvalid;
            }

            programFile->payloadDataInstances.remove(this);
            programFile->decreaseReferenceCount(payloadId);
        } else {
            success = true;
        }

        return success;
    }
}
