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

#include <cstring>

#include "ld_text_export_context.h"

namespace Ld {
    TextExportContext::TextExportContext() {
        currentDevice           = nullptr;
        currentSuppressNewLines = false;
        currentLineNumber       = 0;
        currentColumnNumber     = 0;
    }


    TextExportContext::TextExportContext(QIODevice* device) {
        currentDevice           = device;
        currentSuppressNewLines = false;
        currentLineNumber       = 0;
        currentColumnNumber     = 0;
    }


    TextExportContext::~TextExportContext() {
        if (currentDevice != nullptr) {
            if (currentDevice->isOpen()) {
                currentDevice->close();
            }

            delete currentDevice;
        }
    }


    unsigned long TextExportContext::lineNumber() const {
        return currentLineNumber;
    }


    unsigned long TextExportContext::columnNumber() const {
        return currentColumnNumber;
    }


    bool TextExportContext::newlinesAreSuppressed() const {
        return currentSuppressNewLines;
    }


    bool TextExportContext::newlinesAreAllowed() const {
        return !currentSuppressNewLines;
    }


    void TextExportContext::setNewlinesSuppressed(bool nowSuppress) {
        currentSuppressNewLines = nowSuppress;
    }


    void TextExportContext::setNewlinesAllowed(bool nowAllowed ) {
        currentSuppressNewLines = !nowAllowed;
    }


    bool TextExportContext::write(char character) {
        bool result;

        if (!currentSuppressNewLines || character != '\n' || currentColumnNumber > 0) {
            if (currentDevice != nullptr) {
                long long count = currentDevice->write(&character, 1);
                result = (count == 1);
            } else {
                currentLastError = tr("No device");
                result = false;
            }

            if (result) {
                if (character == '\n') {
                    ++currentLineNumber;
                    currentColumnNumber = 0;
                } else {
                    ++currentColumnNumber;
                }
            }
        } else {
            result = true;
        }

        return result;
    }


    bool TextExportContext::write(const char* characters, unsigned long length) {
        return write(QString::fromLatin1(characters, length));
    }


    bool TextExportContext::write(const char* characters) {
        return write(QString::fromLatin1(characters));
    }


    bool TextExportContext::write(const QString& text) {
        bool result;

        QString adjustedText = text;
        if (currentSuppressNewLines && !adjustedText.isEmpty()) {
            if (currentColumnNumber == 0 && adjustedText.left(1) == "\n") {
                adjustedText = adjustedText.mid(1);
            }

            adjustedText.replace(QString("\n\n"), QString("\n"));
        }

        if (currentDevice != nullptr) {
            QByteArray toSend = adjustedText.toLocal8Bit();
            long long  length = toSend.size();
            long long  count  = currentDevice->write(toSend);
            result = (count == length);
        } else {
            currentLastError = tr("No device");
            result = false;
        }

        if (result) {
            currentLineNumber += adjustedText.count('\n');

            int lastNewline = adjustedText.lastIndexOf('\n');
            if (lastNewline < 0) {
                currentColumnNumber += adjustedText.length();
            } else {
                currentColumnNumber = adjustedText.length() - lastNewline - 1;
            }
        }

        return result;
    }


    bool TextExportContext::hasError() const {
        bool error;

        if (currentDevice != nullptr && !currentDevice->errorString().isEmpty()) {
            error = true;
        } else if (!currentLastError.isEmpty()) {
            error = true;
        } else {
            error = false;
        }

        return error;
    }


    QString TextExportContext::errorString() const {
        QString result;

        if (!currentLastError.isEmpty()) {
            result = currentLastError;
        } else if (currentDevice != nullptr) {
            result = currentDevice->errorString();
        }

        return result;
    }


    bool TextExportContext::close() {
        bool success;

        if (currentDevice != nullptr) {
            if (currentDevice->isOpen()) {
                currentDevice->close();
                success = true;
            } else {
                currentLastError = tr("File not open");
                success          = false;
            }
        } else {
            currentLastError = tr("No device");
            success          = false;
        }

        return success;
    }


    QIODevice* TextExportContext::device() const {
        return currentDevice;
    }


    void TextExportContext::setDevice(QIODevice* device) {
        if (currentDevice != nullptr) {
            if (currentDevice->isOpen()) {
                currentDevice->close();
            }

            delete currentDevice;
        }

        currentDevice       = device;
        currentLineNumber   = 0;
        currentColumnNumber = 0;
    }


    void TextExportContext::setError(const QString& errorMessage) {
        currentLastError = errorMessage;
    }
}
