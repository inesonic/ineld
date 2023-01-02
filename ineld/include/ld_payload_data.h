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
* This header defines the \ref Ld::PayloadData class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_PAYLOAD_DATA_H
#define LD_PAYLOAD_DATA_H

#include <QByteArray>
#include <QSharedDataPointer>
#include <QSharedData>

#include <cstdint>
#include <string>

#include "ld_common.h"

namespace Ld {
    class ProgramFile;

    /**
     * Class that tracks and manages a payload of information where a payload can represent any quantum of binary data
     * such as images, text, or other arbitrary bits that are are expected to be stored as binary data in a
     * \ref Ld::ProgramFile.
     *
     * The class can track data stored either directly in the \ref Ld::ProgramFile or, when the payload data is
     * disconnected from a \ref Ld::ProgramFile either because the \ref Ld::ProgramFile goes out of scope or the
     * \ref Ld::ProgramFile is clased, in local memory.
     *
     * When possible, data sharing and copy on write is used to reduce the memory footprint required for program data.
     * The class also maintains reference counts for \ref Ld::PayloadData instances.
     *
     * \todo Currently the \ref Ld::PayloadData class does not link in-memory copies so, when an in-memory copy is
     *       cloned in a file, the existing in-memory copies remain in memory.  This can become an issue when large
     *       amounts of information are copied between program files and then the source file is closed.
     */
    class LD_PUBLIC_API PayloadData {
        friend class ProgramFile;

        public:
            /**
             * Type used to reference a payload, by ID.
             */
            typedef std::uint32_t PayloadId;

            /**
             * Value used to indicate an invalid payload ID.
             */
            static const PayloadId invalidPayloadId;

            /**
             * Enumeration of storage mechanisms.
             */
            enum class StorageMode {
                /**
                 * Indicates the payload is invalid.
                 */
                NOT_STORED,

                /**
                 * Indicates the payload is currently stored in a file.
                 */
                STORED_IN_FILE,

                /**
                 * Indicates the payload is stored in memory.
                 */
                STORED_IN_MEMORY
            };

            PayloadData();

            /**
             * Copy constructor.
             *
             * \param[in] other The instance to be copied.
             */
            PayloadData(const PayloadData& other);

            ~PayloadData();

            /**
             * Method you can use to determine if this payload data instance is valid.
             *
             * \return Returns true if the payload data instance is valid.  Returns false if the payload data instance
             *         is invalid.
             */
            bool isValid() const;

            /**
             * Method you can use to determine if this payload data instance is invalid.
             *
             * \return Returns true if the payload data instance is invalid.  Returns false if the payload data
             *         instance is valid.
             */
            bool isInvalid() const;

            /**
             * Method you can use to determine the storage mode for this payload.
             *
             * \return Returns the current payload storage mechanism.
             */
            StorageMode storageMode() const;

            /**
             * Method you can use to determine the ID assigned to this payload.
             *
             * \return Returns the payload ID of this payload.
             */
            PayloadId payloadId() const;

            /**
             * Method you can use to determine the number of references to this payload.
             *
             * \return Returns a count of the number of references to this payload data.
             */
            unsigned numberReferences() const;

            /**
             * Method you can use to update the payload.  Note that data is not locally cached so calls to this method
             * trigger file I/O.
             *
             * \param[in] newData The new data to be written to the payload.
             *
             * \return Returns true on success, returns false on error.
             */
            bool writeData(const QByteArray& newData);

            /**
             * Method you can use to obtain the payload data.  Note that data is not locally cached so calls to this
             * method trigger file I/O.
             *
             * \param[out] newData A reference to a QByteArray instance to hold the newly read data.
             *
             * \return Returns true on success, returns false on error.
             */
            bool readData(QByteArray& newData);

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            PayloadData& operator=(const PayloadData& other);

        private:
            /**
             * Pure virtual private base class used to track and manage payload information for the
             * \ref Ld::PayloadData class.
             */
            class LD_PUBLIC_API Private:public QSharedData {
                public:
                    Private();

                    /**
                     * Copy constructor.
                     *
                     * \param[in] other The instance to be copied.
                     */
                    Private(const Private& other);

                    virtual ~Private();

                    /**
                     * Clone method used to create deep copies.
                     *
                     * \return Returns a pointer to a deep copy of this class.
                     */
                    virtual Private* clone() const = 0;

                    /**
                     * Method you can use to determine if this payload data instance is valid.
                     *
                     * \return Returns true if the payload data instance is valid.  Returns false if the payload data instance
                     *         is invalid.
                     */
                    virtual bool isValid() const = 0;

                    /**
                     * Method you can use to determine the storage mode for this payload.
                     *
                     * \return Returns the current payload storage mechanism.
                     */
                    virtual PayloadData::StorageMode storageMode() const = 0;

                    /**
                     * Method you can use to determine the ID assigned to this payload.
                     *
                     * \return Returns the payload ID of this payload.
                     */
                    virtual PayloadData::PayloadId payloadId() const = 0;

                    /**
                     * Method you can use to determine the number of references to this payload.
                     *
                     * \return Returns a count of the number of references to this payload data.
                     */
                    virtual unsigned numberReferences() const = 0;

                    /**
                     * Method you can use to update the payload.  Note that data is not locally cached so calls to this method
                     * trigger file I/O.
                     *
                     * \param[in] newData The new data to be written to the payload.
                     *
                     * \return Returns true on success, returns false on error.
                     */
                    virtual bool writeData(const QByteArray& newData) = 0;

                    /**
                     * Method you can use to obtain the payload data.  Note that data is not locally cached so calls to this
                     * method trigger file I/O.
                     *
                     * \param[out] newData A reference to a QByteArray instance to hold the newly read data.
                     *
                     * \return Returns true on success, returns false on error.
                     */
                    virtual bool readData(QByteArray& newData) = 0;
            };

            /**
             * Private invalid data class.
             */
            class PrivateInvalid;

            /**
             * Private file data class.
             */
            class PrivateFile;

            /**
             * Private local data class.
             */
            class PrivateLocal;

            /**
             * Private constructor used to create payload data instances tied to a \ref ProgramFile.
             *
             * \param[in] programFile      A pointer to the program file to tie to this payload data instance.
             *
             * \param[in] payloadId        The zero based ID of the payload.
             */
            PayloadData(ProgramFile* programFile, PayloadId payloadId);

            /**
             * Method called by the \ref ProgramFile class to invalidate this payload data instance.
             */
            void makeInvalid();

            /**
             * Method called to connect this class instance to a program file.
             */
            void connectToProgramFile();

            /**
             * Method called to disconnect this class instance from a program file.
             */
            void disconnectFromProgramFile();

            /**
             * Method called to move data to local storage and disconnect from a \ref Ld::ProgramFile.
             *
             * \return Returns true on success, returns false on error.
             */
            bool moveToLocalStorageAndDisconnect();

            /**
             * Class internal data.
             */
            QSharedDataPointer<Private> impl;

            friend class ::QSharedDataPointer<Private>;
    };
};

#endif
