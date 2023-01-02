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
* This file implements the \ref Ld::EnvironmentPrivate class.
***********************************************************************************************************************/

#include <QFileInfo>
#include <QList>
#include <QString>
#include <QDir>

#include "ld_environment.h"
#include "ld_environment_private.h"

namespace Ld {
    EnvironmentPrivate::EnvironmentPrivate(const QString& executableFilePath, bool useHeaderFiles) {
        QFileInfo executableInfo(executableFilePath);

        currentExecutableDirectory = executableInfo.absolutePath();
        currentUseHeaderFiles      = useHeaderFiles;
    }


    EnvironmentPrivate::~EnvironmentPrivate() {}


    bool EnvironmentPrivate::useHeaderFiles() const {
        return currentUseHeaderFiles;
    }


    QString EnvironmentPrivate::executableDirectory() const {
        return currentExecutableDirectory;
    }


    QString EnvironmentPrivate::userPlugInRootDirectory() const {
        QFileInfo userPlugInRoot(QDir::homePath() + "/Aion/");
        return userPlugInRoot.isDir() ? userPlugInRoot.canonicalFilePath() : QString();
    }


    QList<QString> EnvironmentPrivate::userPlugInDirectories() const {
        QList<QString> result;

        QString plugInRoot = Environment::userPlugInRootDirectory();
        if (!plugInRoot.isEmpty()) {
            QFileInfoList fileInfoList = QDir(plugInRoot).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

            for (  QFileInfoList::const_iterator it = fileInfoList.constBegin(), end = fileInfoList.constEnd()
                 ; it!=end
                 ; ++it
                ) {
                QString canonicalPath = it->canonicalFilePath();

                #if (defined(Q_OS_WINDOWS) || defined(Q_OS_LINUX))

                    if (QFileInfo(canonicalPath).isDir()) {
                        result.append(canonicalPath);
                    }

                #elif (defined(Q_OS_DARWIN))

                    if (QFileInfo(canonicalPath + "/Contents/").isDir()) {
                        result.append(canonicalPath);
                    }

                #else

                    #error Unknown platform

                #endif
            }
        }

        return result;
    }


    QString EnvironmentPrivate::gccPrefix() const {
        return QString();
    }


    QString EnvironmentPrivate::targetTriple() {
        return QString();
    }


    bool EnvironmentPrivate::addPathIfExists(
            QList<QString>& stringList,
            const QString&  path,
            const QString&  filename
        ) {
        bool      success;
        QFileInfo fileInformation(path + "/" + filename);

        if (fileInformation.exists() && fileInformation.isFile()) {
            stringList << path;
            success = true;
        } else {
            success = false;
        }

        return success;
    }
}
