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
* This file implements the \ref Ld::RootImport class.
***********************************************************************************************************************/

#include <QSharedPointer>
#include <QString>
#include <QFileInfo>
#include <QDir>

#include "ld_root_element.h"
#include "ld_root_import.h"

namespace Ld {
    RootImport::RootImport() {
        currentRootElement.reset();
        currentInvalidFilename.clear();

        useRelativePath = true;

    }


    RootImport::RootImport(QSharedPointer<RootElement> rootElement, bool isRelativeImport) {
        currentRootElement = rootElement;
        currentInvalidFilename.clear();

        useRelativePath = isRelativeImport;
    }


    RootImport::RootImport(const QString& invalidFilename, bool isRelativeImport) {
        currentRootElement.reset();
        currentInvalidFilename = invalidFilename;

        useRelativePath = isRelativeImport;
    }


    RootImport::RootImport(const RootImport& other) {
        currentRootElement     = other.currentRootElement;
        currentInvalidFilename = other.currentInvalidFilename;
        useRelativePath        = other.useRelativePath;
    }


    RootImport::~RootImport() {}


    bool RootImport::isValid() const {
        return !currentRootElement.isNull();
    }


    bool RootImport::isInvalid() const {
        return !isValid();
    }


    void RootImport::setRootElement(QSharedPointer<RootElement> newRootElement) {
        currentRootElement = newRootElement;
        currentInvalidFilename.clear();
    }


    void RootImport::setInvalidFilename(const QString& newInvalidFilename) {
        currentRootElement.clear();
        currentInvalidFilename = newInvalidFilename;
    }


    QSharedPointer<RootElement> RootImport::rootElement() const {
        return currentRootElement;
    }


    bool RootImport::hasFilename() const {
        bool elementHasFilename;

        if (currentRootElement.isNull()) {
            elementHasFilename = !currentInvalidFilename.isEmpty();
        } else {
            elementHasFilename = currentRootElement->hasFilename();
        }

        return elementHasFilename;
    }


    void RootImport::setRelativeImport(bool nowRelative) {
        useRelativePath = nowRelative;
    }


    void RootImport::setAbsoluteImport(bool nowAbsolute) {
        setRelativeImport(!nowAbsolute);
    }


    bool RootImport::isRelativeImport() const {
        return useRelativePath;
    }


    bool RootImport::isAbsoluteImport() const {
        return !isRelativeImport();
    }


    QString RootImport::absolutePath() const {
        QString path;

        if (!currentRootElement.isNull()) {
            if (currentRootElement->hasFilename()) {
                QFileInfo fileInformation(currentRootElement->filename());

                path = fileInformation.canonicalFilePath();
                if (path.isEmpty()) {
                    path = fileInformation.absoluteFilePath();
                }
            } else {
                path = currentRootElement->shortformName();
            }
        } else {
            path = QFileInfo(currentInvalidFilename).absoluteFilePath();
        }

        return path;
    }


    QString RootImport::relativePath(const QString& importPath) const {
        QString path;

        if (!currentRootElement.isNull()) {
            if (currentRootElement->hasFilename()) {
                QFileInfo fileInformation(currentRootElement->filename());

                QString absolutePath = fileInformation.canonicalFilePath();
                if (absolutePath.isEmpty()) {
                    absolutePath = fileInformation.absoluteFilePath();
                }

                if (importPath.isEmpty()) {
                    path = absolutePath;
                } else {
                    QDir importDirectory = QFileInfo(importPath).absoluteDir();
                    path = importDirectory.relativeFilePath(absolutePath);
                }
            } else {
                path = currentRootElement->shortformName();
            }
        } else {
            QFileInfo fileInformation(currentInvalidFilename);

            QString absolutePath = fileInformation.absoluteFilePath();
            QDir importDirectory = QFileInfo(importPath).absoluteDir();
            path = importDirectory.relativeFilePath(absolutePath);
        }

        return path;
    }


    QString RootImport::path(const QString& importPath) const {
        QString reportedPath;

        if (useRelativePath) {
            reportedPath = relativePath(importPath);
        } else {
            reportedPath = absolutePath();
        }

        return reportedPath;
    }


    RootImport& RootImport::operator=(const RootImport& other) {
        currentRootElement     = other.currentRootElement;
        currentInvalidFilename = other.currentInvalidFilename;
        useRelativePath        = other.useRelativePath;

        return *this;
    }


    bool RootImport::operator==(const RootImport& other) const {
        bool isEqual;

        if (currentRootElement.isNull()) {
            isEqual = (
                   other.currentInvalidFilename == currentInvalidFilename
                && other.useRelativePath == useRelativePath
            );
        } else {
            isEqual = (
                   other.currentRootElement == currentRootElement
                && other.useRelativePath == useRelativePath
            );
        }

        return isEqual;
    }


    bool RootImport::operator!=(const RootImport& other) const {
        return !operator==(other);
    }
}
