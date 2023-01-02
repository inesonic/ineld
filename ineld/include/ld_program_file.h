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
* This header defines the \ref Ld::ProgramFile class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_PROGRAM_FILE_H
#define LD_PROGRAM_FILE_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QImage>
#include <QSharedPointer>
#include <QFileInfo>
#include <QSet>
#include <QHash>
#include <QBitArray>

#include <util_bit_array.h>

#include <qfile_container.h>

#include "ld_common.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_payload_data.h"
#include "ld_handle.h"

namespace Ld {
    class PayloadData;

    /**
     * Class that provides in interface to read and write program files.  The class provides methods to load and save
     * the program as well as methods to manage image files.
     *
     * The class also maintains a single static instance that is intended to be used as a temporary parking spot for
     * program files.
     */
    class LD_PUBLIC_API ProgramFile {
        friend class PayloadData;

        Q_DECLARE_TR_FUNCTIONS(Ld::ProgramFile)

        public:
            /**
             * String used as a container file identifier.
             */
            static const QString fileIdentifier;

            /**
             * Type used to represent the file open mode.
             */
            typedef QFileContainer::OpenMode OpenMode;

            ProgramFile();

            ~ProgramFile();

            /**
             * Method you can call to open a program file for reading and writing.
             *
             * When opening in read-only mode, the container file is opened directly but is not modified.  When opened
             * in read/write mode or overwrite mode, the container file is copied and the copy is modified.  The
             * original container file is left unchanged.
             *
             * \param[in] filename The name of the file to be read.
             *
             * \param[in] readOnly If true, the file will be opened read-only.  If false, the file will be opened in
             *                     read/write mode.
             *
             * \return Returns true on success, returns false on error.
             */
            bool openExisting(const QString& filename, bool readOnly);

            /**
             * Method you can call to create a new program file for reading and writing.  The new file will be opened
             * in read/write mode.  The method will fail if the requested file already exists.
             *
             * \return Returns true on success, returns false on error.
             */
            bool openNew();

            /**
             * Method you can use to close the working container and overwrite the customer's version.    This method
             * will fail if the container was opened read-only.
             *
             * \return Returns true on success, returns false on error.
             */
            bool save();

            /**
             * Method you can use to close the working container, and rename as a new file.  The working copy is also
             * renamed.  If the container was opened read-only then the program will simply copy the container under a
             * new name and then open that container in read/write mode.
             *
             * \param[in] newFilename The new name to associate with this program.
             *
             * \return Returns the name of the program file.
             */
            bool saveAs(const QString& newFilename);

            /**
             * Method you can use to close the working container and remove it without modifying the customer's copy.
             *
             * \return Returns true on success, returns false on error.
             */
            bool close();

            /**
             * Method you can call to get the name of the currently open file.
             *
             * \return Returns the name of the currently open container.
             */
            QString filename() const;

            /**
             * Method you can use to determine how the container was opened.
             *
             * \return Returns the container open mode.
             */
            OpenMode openMode() const;

            /**
             * Method you can use to obtain a payload instance by payload ID.
             *
             * \param[in] payloadId The ID of the desired payload.
             *
             * \return Returns the requested payload data.  An invalid payload will be returned on error.
             */
            PayloadData payload(PayloadData::PayloadId payloadId);

            /**
             * Method you can use to obtain a new-pristine payload instance.
             *
             * \return Returns a new and pristine payload instance.
             */
            PayloadData newPayload();

            /**
             * Method you can use to clone a payload into this program file.  When this method is called, a copy of the
             * payload indicated by a provided \ref Ld::PayloadData instance will be created and stored in this
             * container.
             *
             * \param[in] payloadData The payload data to be cloned.
             *
             * \return Returns a new \ref Ld::PayloadData instance containing the new clone stored in this program
             *         file.
             */
            PayloadData clonePayload(PayloadData& payloadData);

            /**
             * Method you can use to purge all payloads with a reference count of 0.
             *
             * \return Returns true on success, returns false on error.
             */
            bool purgeUnreferencedPayloads();

            /**
             * Method you can call to obtain a description of the last reported error.
             *
             * \return Returns a textual description of the last reported error.
             */
            QString errorString() const;

            /**
             * Method you can call to initialize the XML stream reader.
             *
             * \param[in] streamName The name of the XML stream.  The method will automatically append the correct
             *                       extension to the stream name (currently ".xml")
             *
             * \return Returns true on success, returns false on error.
             */
            bool initializeXmlReader(const QString& streamName = QString());

            /**
             * Method you can use to access the current XML reader.
             *
             * \return Returns a shared pointer to the XML reader.
             */
            QSharedPointer<XmlReader> reader();

            /**
             * Method you can call to clean-up the XML stream reader and close the virtual file.
             *
             * \return Returns true on success, returns false on error.
             */
            bool releaseXmlReader();

            /**
             * Method you can call to initialize the XML stream writer.
             *
             * \param[in] streamName The name of the XML stream.  The method will automatically append the correct
             *                       extension to the stream name (currently ".xml")
             *
             * \return Returns true on success, returns false on error.
             */
            bool initializeXmlWriter(const QString& streamName = QString());

            /**
             * Method you can use to access the current XML writer.
             *
             * \return Returns a shared pointer to the XML writer.
             */
            QSharedPointer<XmlWriter> writer();

            /**
             * Method you can call to clean-up the XML stream writer and close the virtual file.
             *
             * \return Returns true on success, returns false on error.
             */
            bool releaseXmlWriter();

        private:
            /**
             * Method used internally to reset the program file state.
             */
            void resetState();

            /**
             * Method that renames the container file.
             *
             * \param[in] oldName The old container name
             *
             * \param[in] newName The new container name
             *
             * \return Returns true on success, returns false on error.
             */
            bool renameContainer(const QString& oldName, const QString& newName);

            /**
             * Method that tells any payload data instances to either relocate their content to local memory.
             *
             * \return Returns true on success, returns false on error.
             */
            bool movePayloadsToLocalStorageAndDisconnect();

            /**
             * Method called by the \ref PayloadData instance to increase a reference count.
             *
             * \param[in] payloadId The payload ID of the payload to adjust reference counts for.
             */
            void increaseReferenceCount(PayloadData::PayloadId payloadId);

            /**
             * Method called by the \ref PayloadData instance to decrease a reference count.
             *
             * \param[in] payloadId The payload ID of the payload to adjust reference counts for.
             *
             * \return Returns the final reference count for the payload.
             */
            unsigned long decreaseReferenceCount(PayloadData::PayloadId payloadId);

            /**
             * Method that returns the name of a payload virtual file given a payload ID.
             *
             * \param[in] payloadId The payload ID we want the associated filename for.
             *
             * \return Returns the filename to use for the given payload ID.
             */
            static QString payloadName(PayloadData::PayloadId payloadId);

            /**
             * The file container being used to manage the current program.
             */
            QScopedPointer<QFileContainer> container;

            /**
             * File information structure for file provided by the user and used to open the container.  Note that this
             * for overwrite and read/write open modes, the container file is different from file provided to the
             * \ref ProgramFile::openExisting method.  For this reason, this value does *not* represent the file
             * information for the ProgramFile::container instance.
             */
            QFileInfo containerFileInformation;

            /**
             * Flag that indicates if this container was opened as new or was copied from an existing container.
             */
            bool isNewContainer;

            /**
             * String holding the last reported error during file I/O operations.
             */
            QString lastError;

            /**
             * The current virtual file being used by the XML reader or XML writer.
             */
            QPointer<QVirtualFile> currentDevice;

            /**
             * A shared pointer to the currently open/active XML reader.
             */
            QSharedPointer<XmlReader> currentReader;

            /**
             * A shared pointer to the currently open/active XML writer.
             */
            QSharedPointer<XmlWriter> currentWriter;

            /**
             * A set of known \ref Ld::PayloadData instances tied to this program file.
             */
            QSet<PayloadData*> payloadDataInstances;

            /**
             * Map used to track payload data instances.
             */
            QHash<PayloadData::PayloadId, unsigned long> payloadReferenceCounts;
    };
};

#endif
