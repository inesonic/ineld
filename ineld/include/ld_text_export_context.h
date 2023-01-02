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
* This header defines the \ref Ld::TextExportContext class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_TEXT_EXPORT_CONTEXT_H
#define LD_TEXT_EXPORT_CONTEXT_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QIODevice>
#include <QByteArray>

#include "ld_common.h"

namespace Ld {
    /**
     * Class you can use as a base class for contexts for code generators that export text or text with additional
     * payload data.  The class provides a streaming interface for the primary text data as a separate interface to
     * manage payloads.
     */
    class LD_PUBLIC_API TextExportContext {
        Q_DECLARE_TR_FUNCTIONS(Ld::TextExportContext)

        protected:
            TextExportContext();

            /**
             * Protected constructor you can use to write the stream to an I/O device.
             *
             * \param[in] device The I/O device to receive the XHTML stream.  This class will take ownership of the
             *                   device.
             */
            TextExportContext(QIODevice* device);

        public:
            virtual ~TextExportContext();

            /**
             * Method you can use to stream one character into this object.
             *
             * \param[in] character The character to be written.
             *
             * \return Returns true on success, returns false on error.
             */
            bool write(char character);

            /**
             * Method you can use to determine the current line number in the exported text.
             *
             * \return Returns the current zero based line number in the exported text.
             */
            unsigned long lineNumber() const;

            /**
             * Method you can use to determine the current column number in the exported text.
             *
             * \return REturns the current zero based column number in the exported text.
             */
            unsigned long columnNumber() const;

            /**
             * Method you can use to determine if new-line characters are being suppressed.
             *
             * \return Returns true if new-line characters are being suppressed from the output.  Returns false if
             *         new-line characters are being included in the output.
             */
            bool newlinesAreSuppressed() const;

            /**
             * Method you can use to determine if new-line characters are being allowed.
             *
             * \return Returns true if new-line characters are being allowed into the output.  Returns false if
             *         new-line characters are being suppressed from the output.
             */
            bool newlinesAreAllowed() const;

            /**
             * Method you can use to force new-line characters to be suppressed.
             *
             * \param[in] nowSuppress If true, new-line characters will be suppressed from the output.  If false,
             *                        new-line characters will be included in the output.
             */
            void setNewlinesSuppressed(bool nowSuppress = true);

            /**
             * Method you can use to allow new-line characters in the output.
             *
             * \param[in] nowAllowed If true, new-line characters will be included in the output.  If false, new-line
             *                       characters will be suppressed.
             */
            void setNewlinesAllowed(bool nowAllowed = true);

            /**
             * Method you can use to stream one or more characters into this object.
             *
             * \param[in] characters The characters to be written.
             *
             * \param[in] length     The number of characters to be written.
             *
             * \return Returns true on success, returns false on error.
             */
            bool write(const char* characters, unsigned long length);

            /**
             * Method you can use to stream one or more characters into this object.  The method will search until
             * encountering a NUL characters.
             *
             * \param[in] characters The characters to be written.
             *
             * \return Returns true on success, returns false on error.
             */
            bool write(const char* characters);

            /**
             * Method you can use to stream one or more characters into this object using the local 8-bit
             * representation.
             *
             * \param[in] text The text to be written.
             *
             * \return Returns true on success, returns false on error.
             */
            bool write(const QString& text);

            /**
             * Method you can call to add side-band payload to the XHTML description.
             *
             * \param[in] payloadName The name to assign to the payload.  This will generally be a filename of some
             *                        kind.
             *
             * \param[in] payload     The payload to be included.
             *
             * \return Returns the absolute filepath where the payload was written.  If the payload is stored to
             *         memory, then this method will return the payload name.  An empty string will be returned if an
             *         error occurred.
             */
            virtual QString addPayload(const QString& payloadName, const QByteArray& payload) = 0;

            /**
             * Method you can call to obtain a payload, by name.
             *
             * \param[in] payloadName The name of the desired payload.
             *
             * \return Returns the requested payload.
             */
            virtual QByteArray payload(const QString& payloadName) = 0;

            /**
             * Method you can call to obtain a list of all the known payloads, by name.
             *
             * \return Returns a list of payloads, by name.
             */
            virtual QList<QString> payloads() const = 0;

            /**
             * Method you can call to determine if an error has been reported.
             *
             * \return Returns true if an error has been reported.  Returns false if there is no reported error.
             */
            virtual bool hasError() const;

            /**
             * Method you can call to obtain a textual description of any pending error.
             *
             * \return Returns a string describing the last reported error.
             */
            virtual QString errorString() const;

            /**
             * Method you can call to close any currently open file handles.
             *
             * \return Returns true on success, returns false on error.
             */
            virtual bool close();

            /**
             * Stream operator
             *
             * \param[in] character A single character to be written.
             *
             * \return Returns a reference to this object.
             */
            inline TextExportContext& operator<<(char character) {
                write(character);
                return *this;
            }

            /**
             * Stream operator
             *
             * \param[in] characters A NUL terminated array of characters to be written.
             *
             * \return Returns a reference to this object.
             */
            inline TextExportContext& operator<<(const char* characters) {
                write(characters);
                return *this;
            }

            /**
             * Stream operator
             *
             * \param[in] text Text to be written to the stream.
             *
             * \return Returns a reference to this object.
             */
            inline TextExportContext& operator<<(const QString& text) {
                write(text);
                return *this;
            }

        protected:
            /**
             * Protected method you can use to access the underlying device.
             *
             * \return Returns a pointer to the underlying device.
             */
            QIODevice* device() const;

            /**
             * Protected method that marshalls the call to set the I/O device.
             *
             * \param[in] device The new I/O device to be used.  This class will take ownership of the device.
             */
            void setDevice(QIODevice* device);

            /**
             * Method you can use to set the last reported error.
             *
             * \param[in] errorMessage The new error message.
             */
            void setError(const QString& errorMessage);

        private:
            /**
             * Pointer to the device we're streaming to.
             */
            QIODevice* currentDevice;

            /**
             * The last reported error.
             */
            QString currentLastError;

            /**
             * Flag that indicates if newline characters should be suppressed.
             */
            bool currentSuppressNewLines;

            /**
             * The current line number.
             */
            unsigned long currentLineNumber;

            /**
             * The current column number.
             */
            unsigned long currentColumnNumber;
    };
};

#endif
