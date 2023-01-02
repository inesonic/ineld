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
* This header defines the Ld::PayloadData::Private class and several derived classes. class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_PAYLOAD_DATA_PRIVATE_H
#define LD_PAYLOAD_DATA_PRIVATE_H

#include <QByteArray>
#include <QSharedData>

#include <cstdint>
#include <string>

#include "ld_common.h"
#include "ld_payload_data.h"

namespace Ld {
    class ProgramFile;

    /**
     * Class used to indicate that a \ref Ld::PayloadData class contains no valid data.
     */
    class LD_PUBLIC_API PayloadData::PrivateInvalid:public PayloadData::Private {
        public:
            /**
             * Constructor.
             */
            PrivateInvalid();

            ~PrivateInvalid() override;

            /**
             * Clone method used to create deep copies.
             *
             * \return Returns a pointer to a deep copy of this class.
             */
            virtual Private* clone() const final;

            /**
             * Method you can use to determine if this payload data instance is valid.
             *
             * \return Returns true if the payload data instance is valid.  Returns false if the payload data instance
             *         is invalid.
             */
            bool isValid() const final;

            /**
             * Method you can use to determine the storage mode for this payload.
             *
             * \return Returns the current payload storage mechanism.
             */
            PayloadData::StorageMode storageMode() const final;

            /**
             * Method you can use to determine the ID assigned to this payload.
             *
             * \return Returns the payload ID of this payload.
             */
            PayloadData::PayloadId payloadId() const final;

            /**
             * Method you can use to determine the number of references to this payload.
             *
             * \return Returns a count of the number of references to this payload data.
             */
            unsigned numberReferences() const final;

            /**
             * Method you can use to update the payload.  Note that data is not locally cached so calls to this method
             * trigger file I/O.
             *
             * \param[in] newData The new data to be written to the payload.
             *
             * \return Returns true on success, returns false on error.
             */
            bool writeData(const QByteArray& newData) final;

            /**
             * Method you can use to obtain the payload data.  Note that data is not locally cached so calls to this
             * method trigger file I/O.
             *
             * \param[out] newData A reference to a QByteArray instance to hold the newly read data.
             *
             * \return Returns true on success, returns false on error.
             */
            bool readData(QByteArray& newData) final;
    };

    /**
     * Class used to track track and manage payload information for the \ref Ld::PayloadData class stored in a
     * \ref Ld::ProgramFile file.
     */
    class LD_PUBLIC_API PayloadData::PrivateFile:public PayloadData::Private {
        public:
            /**
             * Constructor.
             */
            PrivateFile();

            /**
             * Private constructor used to create payload data instances tied to a \ref ProgramFile.
             *
             * \param[in] programFile A pointer to the program file to tie to this payload data instance.
             *
             * \param[in] payloadId   The zero based ID of the payload.
             */
            PrivateFile(ProgramFile* programFile, PayloadData::PayloadId payloadId);

            ~PrivateFile() override;

            /**
             * Clone method used to create deep copies.
             *
             * \return Returns a pointer to a deep copy of this class.
             */
            virtual Private* clone() const final;

            /**
             * Method you can use to determine if this payload data instance is valid.
             *
             * \return Returns true if the payload data instance is valid.  Returns false if the payload data instance
             *         is invalid.
             */
            bool isValid() const final;

            /**
             * Method you can use to determine the storage mode for this payload.
             *
             * \return Returns the current payload storage mechanism.
             */
            PayloadData::StorageMode storageMode() const final;

            /**
             * Method you can use to determine the ID assigned to this payload.
             *
             * \return Returns the payload ID of this payload.
             */
            PayloadData::PayloadId payloadId() const final;

            /**
             * Method you can use to determine the number of references to this payload.
             *
             * \return Returns a count of the number of references to this payload data.
             */
            unsigned numberReferences() const final;

            /**
             * Method you can use to update the payload.  Note that data is not locally cached so calls to this method
             * trigger file I/O.
             *
             * \param[in] newData The new data to be written to the payload.
             *
             * \return Returns true on success, returns false on error.
             */
            bool writeData(const QByteArray& newData) final;

            /**
             * Method you can use to obtain the payload data.  Note that data is not locally cached so calls to this
             * method trigger file I/O.
             *
             * \param[out] newData A reference to a QByteArray instance to hold the newly read data.
             *
             * \return Returns true on success, returns false on error.
             */
            bool readData(QByteArray& newData) final;

            /**
             * Method you can use to obtain a pointer to the current program file.
             *
             * \return Returns a pointer to the associated program file.
             */
            ProgramFile* programFile() const;

        private:
            /**
             * The program data instance tied to this class.
             */
            ProgramFile* currentProgramFile;

            /**
             * The payload ID tied to this class.
             */
            PayloadData::PayloadId currentPayloadId;
    };

    /**
     * Class used to track track and manage payload information for the \ref Ld::PayloadData class stored in local
     * memory.
     */
    class LD_PUBLIC_API PayloadData::PrivateLocal:public PayloadData::Private {
        public:
            /**
             * Constructor.
             */
            PrivateLocal();

            ~PrivateLocal() override;

            /**
             * Clone method used to create deep copies.
             *
             * \return Returns a pointer to a deep copy of this class.
             */
            virtual Private* clone() const final;

            /**
             * Method you can use to determine if this payload data instance is valid.
             *
             * \return Returns true if the payload data instance is valid.  Returns false if the payload data instance
             *         is invalid.
             */
            bool isValid() const final;

            /**
             * Method you can use to determine the storage mode for this payload.
             *
             * \return Returns the current payload storage mechanism.
             */
            PayloadData::StorageMode storageMode() const final;

            /**
             * Method you can use to determine the ID assigned to this payload.
             *
             * \return Returns the payload ID of this payload.
             */
            PayloadData::PayloadId payloadId() const final;

            /**
             * Method you can use to determine the number of references to this payload.
             *
             * \return Returns a count of the number of references to this payload data.
             */
            unsigned numberReferences() const final;

            /**
             * Method you can use to update the payload.  Note that data is not locally cached so calls to this method
             * trigger file I/O.
             *
             * \param[in] newData The new data to be written to the payload.
             *
             * \return Returns true on success, returns false on error.
             */
            bool writeData(const QByteArray& newData) final;

            /**
             * Method you can use to obtain the payload data.  Note that data is not locally cached so calls to this
             * method trigger file I/O.
             *
             * \param[out] newData A reference to a QByteArray instance to hold the newly read data.
             *
             * \return Returns true on success, returns false on error.
             */
            bool readData(QByteArray& newData) final;

        private:
            /**
             * The local data storage.
             */
            QByteArray currentData;
    };
};

#endif
