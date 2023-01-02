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
* This header defines the \ref Ld::CppDeclarationPayload class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CPP_DECLARATION_PAYLOAD_H
#define LD_CPP_DECLARATION_PAYLOAD_H

#include <QSharedPointer>
#include <QString>
#include <QByteArray>

#include <cstdint>

#include "ld_common.h"

namespace Ld {
    /**
     * Class that holds an optionally compressed payload with the declarations for a C++ external library.
     */
    class LD_PUBLIC_API CppDeclarationPayload {
        public:
            /**
             * Method you can use to register a new payload with the system.
             *
             * \param[in] newPayload The new payload to be registered.
             *
             * \return Returns true on success, returns false if a payload with the requested name already exists.
             */
            static bool registerPayload(const CppDeclarationPayload& newPayload);

            /**
             * Method you can use to obtain a payload by library name
             *
             * \param[in] libraryName The library we desire the payload for.
             *
             * \return Returns the requested payload.
             */
            static CppDeclarationPayload payload(const QString& libraryName);

            CppDeclarationPayload();

            /**
             * Constructor.
             *
             * \param[in] library    The library tied to this payload.
             *
             * \param[in] payload    The payload.  The payload is expected to remain resident during the entire
             *                       lifetime of the application.
             *
             * \param[in] length     The length of the payload, in bytes.
             *
             * \param[in] compressed If true, the payload is compressed.  If false, the payload is not compressed.
             */
            CppDeclarationPayload(
                const QString&       library,
                const unsigned char* payload,
                unsigned long        length,
                bool                 compressed = false
            );

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            CppDeclarationPayload(const CppDeclarationPayload& other);

            /**
             * Method you can use to determine if this payload is valid.
             *
             * \return Returns true if this payload is valid.  Returns false if this payload is not valid.
             */
            bool isValid() const;

            /**
             * Method you can use to determine if this payload is invalid.
             *
             * \return Returns true if this payload is invalid.  Returns false if this payload is valid.
             */
            bool isInvalid() const;

            /**
             * Method you can use to obtain the library name of the library tied to this payload.
             *
             * \return Returns the library name
             */
            const QString& library() const;

            /**
             * Method you can use to obtain the raw payload data.
             *
             * \return Returns a pointer to the raw payload data.
             */
            const unsigned char* rawPayload() const;

            /**
             * Method you can use to obtain the length of the raw payload.
             *
             * \return Returns the length of the raw payload.
             */
            unsigned long length() const;

            /**
             * Method you can use to determine if the raw payload is compressed.
             *
             * \return Returns true if the payload is compressed.  Returns false if the payload is not compressed.
             */
            bool isCompressed() const;

            /**
             * Method you can use to obtain the uncompressed payload.
             *
             * \return Returns a byte array holding the uncompressed payload.
             */
            QByteArray payload() const;

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to be copied to this instance.
             */
            CppDeclarationPayload& operator=(const CppDeclarationPayload& other);

        private:
            /**
             * Private implementation
             */
            class Private;

            /**
             * Private implementation
             */
            QSharedPointer<Private> impl;
    };
}

#endif
