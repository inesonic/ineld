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
* This file implements the \ref DeploymentEnvironmentPrivate class.
***********************************************************************************************************************/

#include <QDebug>
#include <QtGlobal>
#include <QFileInfo>
#include <QString>
#include <QDir>

#include "ld_environment.h"
#include "ld_environment_private.h"
#include "deployment_environment_private.h"

DeploymentEnvironmentPrivate::DeploymentEnvironmentPrivate(
        const QString& executableFilePath,
        bool           useHeaderFiles
    ):Ld::EnvironmentPrivate(
        executableFilePath,
        useHeaderFiles
    ) {}


DeploymentEnvironmentPrivate::~DeploymentEnvironmentPrivate() {}


QString DeploymentEnvironmentPrivate::supportExecutableDirectory() const {
    QString result;
    QString root = Ld::Environment::applicationRoot();

    #if (defined(Q_OS_WIN))

        result = QFileInfo(root + "/bin/").absoluteFilePath();

    #elif (defined(Q_OS_LINUX))

        result = QFileInfo(root + "/bin/").absoluteFilePath();

    #elif (defined(Q_OS_DARWIN))

        result = QFileInfo(root + "/Contents/Resources/bin/").absoluteFilePath();

    #else

        #error Unknown platform.

    #endif

    return result;
}


QString DeploymentEnvironmentPrivate::compilerRoot() const {
    QString result;
    QString root = Ld::Environment::applicationRoot();

    #if (defined(Q_OS_WIN))

        result = root;

    #elif (defined(Q_OS_LINUX))

        result = root;

    #elif (defined(Q_OS_DARWIN))

        result = QFileInfo(root + "/Contents/Resources/").absoluteFilePath();

    #else

        #error Unknown platform.

    #endif

    return result;
}


QString DeploymentEnvironmentPrivate::compilerResourceDirectory() const {
    QString result;

    #if (false)

        // No longer needed

        QString root = Ld::Environment::applicationRoot();

        #if (defined(Q_OS_WIN) || defined(Q_OS_LINUX))

            result = QFileInfo(root + "/lib/clang/5.0.1/").absoluteFilePath();

        #elif (defined(Q_OS_DARWIN))

            result = QFileInfo(root + "/Contents/Resources/lib/clang/5.0.1/").absoluteFilePath();

        #else

            #error Unknown platform.

        #endif

    #endif

    return result;
}


QList<QString> DeploymentEnvironmentPrivate::headerSearchPaths() const {
    QList<QString> result;
    QString root = Ld::Environment::applicationRoot();

    #if (defined(Q_OS_WIN))

        result << QFileInfo(root + "/include/").absoluteFilePath();

    #elif (defined(Q_OS_LINUX))

        result << QFileInfo(root + "/include/").absoluteFilePath();

    #elif (defined(Q_OS_DARWIN))

        result << QFileInfo(root + "/Contents/Resources/include/").absoluteFilePath();

    #else

        #error Unknown platform.

    #endif

    return result;
}


QList<QString> DeploymentEnvironmentPrivate::pchSearchPaths() const {
    QList<QString> result;
    QString root = Ld::Environment::applicationRoot();

    #if (defined(Q_OS_WIN))

        result << QFileInfo(root + "/pch/").absoluteFilePath();

    #elif (defined(Q_OS_LINUX))

        result << QFileInfo(root + "/pch/").absoluteFilePath();

    #elif (defined(Q_OS_DARWIN))

        result << QFileInfo(root + "/Contents/Resources/pch/").absoluteFilePath();

    #else

        #error Unknown platform.

    #endif

    return result;
}


QList<QString> DeploymentEnvironmentPrivate::librarySearchPaths(const QString& overrideRuntime) const {
    QList<QString> result;
    QString root = Ld::Environment::applicationRoot();

    #if (defined(Q_OS_WIN))

        if (!overrideRuntime.isEmpty()) {
            QFileInfo overrideRuntimeFileInformation(overrideRuntime);
            result << overrideRuntimeFileInformation.canonicalPath();
        }

        result << QFileInfo(root + "/lib/").absoluteFilePath();

    #elif (defined(Q_OS_LINUX))

        (void) overrideRuntime;
        result << QFileInfo(root + "/lib/").absoluteFilePath();

    #elif (defined(Q_OS_DARWIN))

        (void) overrideRuntime;
        result << QFileInfo(root + "/Contents/Resources/lib/").absoluteFilePath();

    #else

        #error Unknown platform.

    #endif

    return result;
}


QList<QString> DeploymentEnvironmentPrivate::standardHeaderFiles() const {
    QList<QString> result;
    result << "model_api.h"
           << "model_interface.h"
           << "m_model_base.h";

    return result;
}


QList<QString> DeploymentEnvironmentPrivate::standardPchFiles() const {
    QList<QString> result;
    result << "inem.pch";

    return result;
}


QList<QString> DeploymentEnvironmentPrivate::standardStaticLibraries(const QString& overrideRuntime) const {
    QList<QString> result;

    #if (defined(Q_OS_WIN))

        if (overrideRuntime.isEmpty()) {
            // On Windows, we use a different runtime that combines inem with a few hooks that avoid the need for
            // and of Microsoft's non-distributable libraries.

            result << "inert";
        } else {
            QFileInfo overrideRuntimeFileInformation(overrideRuntime);
            result << overrideRuntimeFileInformation.baseName();
        }

    #elif (defined(Q_OS_LINUX) || defined(Q_OS_DARWIN))

        (void) overrideRuntime;

    #else

        #error Unknown platform.

    #endif

    return result;
}


QList<QString> DeploymentEnvironmentPrivate::standardDynamicLibraries() const {
    QList<QString> result;

    #if (defined(Q_OS_LINUX))

        result << "inem";
        result << "c++";

    #elif (defined(Q_OS_DARWIN))

        result << "inem";
        result << "c++";

    #elif (!defined(Q_OS_WIN))

        #error Unknown platform.

    #endif

    return result;
}


QList<QString> DeploymentEnvironmentPrivate::runTimeLibrarySearchPaths() const {
    QList<QString> result;
    QString root = Ld::Environment::applicationRoot();

    #if (defined(Q_OS_LINUX) || defined(Q_OS_WIN))

        result << QFileInfo(root + "/lib/").absoluteFilePath();

    #elif (defined(Q_OS_DARWIN))

        result << QFileInfo(root + "/Contents/Resources/lib/").absoluteFilePath();

    #elif (!defined(Q_OS_WIN))

        #error Unknown platform.

    #endif

    return result;
}


QList<QString> DeploymentEnvironmentPrivate::fontSearchPaths() const {
    QList<QString> result;

    QString root = Ld::Environment::applicationRoot();

    #if (defined(Q_OS_LINUX) || defined(Q_OS_WIN))

        QString fontDirectoryPath = root + "/fonts/";

    #elif (defined(Q_OS_DARWIN))

        QString fontDirectoryPath = root + "/Contents/Resources/fonts/";

    #else

        #error Unknown platform

    #endif

    QDir fontsDirectory(fontDirectoryPath);
    if (fontsDirectory.exists()) {
        result << fontDirectoryPath;
    }

    return result;
}


QString DeploymentEnvironmentPrivate::examplesDirectory() const {
    QString result;

    QString root = Ld::Environment::applicationRoot();

    #if (defined(Q_OS_LINUX) || defined(Q_OS_WIN))

        result = root + "/examples/";

    #elif (defined(Q_OS_DARWIN))

        result = root + "/Contents/Resources/examples/";

    #else

        #error Unknown platform

    #endif

    return result;
}


QString DeploymentEnvironmentPrivate::plugInRootDirectory() const {
    QString result;
    QString root = Ld::Environment::applicationRoot();

    #if (defined(Q_OS_WIN))

        result = QFileInfo(root + "/plugins/").absoluteFilePath();

    #elif (defined(Q_OS_LINUX))

        result = QFileInfo(root + "/plugins/").absoluteFilePath();

    #elif (defined(Q_OS_DARWIN))

        result = QFileInfo(root + "/Contents/PlugIns/").absoluteFilePath();

    #else

        #error Unknown platform.

    #endif

    return result;
}


QList<QString> DeploymentEnvironmentPrivate::plugInDirectories() const {
    QList<QString> result;
    QString       plugInRoot   = Ld::Environment::plugInRootDirectory();
    QFileInfoList fileInfoList = QDir(plugInRoot).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (QFileInfoList::const_iterator it=fileInfoList.constBegin(),end=fileInfoList.constEnd() ; it!=end ; ++it) {
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

    return result;
}


QList<QString> DeploymentEnvironmentPrivate::plugInFiles() const {
    QList<QString> result;
    QList<QString> plugInDirectories = Ld::Environment::plugInDirectories() + Ld::Environment::userPlugInDirectories();

    for (  QList<QString>::const_iterator plugInIterator    = plugInDirectories.constBegin(),
                                          plugInEndIterator = plugInDirectories.constEnd()
         ; plugInIterator != plugInEndIterator
         ; ++plugInIterator
        ) {
        QString     plugInRoot = *plugInIterator;
        QStringList filters;

        #if (defined(Q_OS_LINUX))

            QFileInfo plugInFileDirectory(plugInRoot + "/bin/");
            filters << "*.so";

        #elif (defined(Q_OS_DARWIN))

            QFileInfo plugInFileDirectory(plugInRoot + "/Contents/MacOS/");
            filters << "*.dylib" << "*.bundle";

        #elif (defined(Q_OS_WIN))

            QFileInfo plugInFileDirectory(plugInRoot + "/bin/");
            filters << "*.dll";

        #else

            #error Unknown platform

        #endif

        if (plugInFileDirectory.exists() && plugInFileDirectory.isDir()) {
            QFileInfoList plugInFiles = QDir(plugInFileDirectory.canonicalFilePath()).entryInfoList(
                filters,
                QDir::Files | QDir::NoSymLinks
            );

            for (  QFileInfoList::const_iterator plugInIterator    = plugInFiles.constBegin(),
                                                 plugInEndIterator = plugInFiles.constEnd()
                 ; plugInIterator != plugInEndIterator
                 ; ++plugInIterator
                ) {
                result << plugInIterator->canonicalFilePath();
            }
        }
    }

    return result;
}


QList<QString> DeploymentEnvironmentPrivate::plugInHeaderSearchPaths() const {
    QList<QString> result;
    QList<QString> plugInRoots = Ld::Environment::plugInDirectories() + Ld::Environment::userPlugInDirectories();

    for (QList<QString>::const_iterator it=plugInRoots.begin(),end=plugInRoots.end() ; it!=end ; ++it) {
        QString headerDirectory;
        QString plugInRoot = *it;

        #if (defined(Q_OS_WIN))

            headerDirectory = QFileInfo(plugInRoot + "/include/").absoluteFilePath();

        #elif (defined(Q_OS_LINUX))

            headerDirectory = QFileInfo(plugInRoot + "/include/").absoluteFilePath();

        #elif (defined(Q_OS_DARWIN))

            headerDirectory = QFileInfo(plugInRoot + "/Contents/Resources/include/").absoluteFilePath();

        #else

            #error Unknown platform.

        #endif

        if (QFileInfo(headerDirectory).exists() && QFileInfo(headerDirectory).isDir()) {
            result << headerDirectory;
        }
    }

    return result;
}


QList<QString> DeploymentEnvironmentPrivate::plugInPchSearchPaths() const {
    QList<QString> result;
    QList<QString> plugInRoots = Ld::Environment::plugInDirectories() + Ld::Environment::userPlugInDirectories();

    for (QList<QString>::const_iterator it=plugInRoots.begin(),end=plugInRoots.end() ; it!=end ; ++it) {
        QString headerDirectory;
        QString plugInRoot = *it;

        #if (defined(Q_OS_WIN))

            headerDirectory = QFileInfo(plugInRoot + "/pch/").absoluteFilePath();

        #elif (defined(Q_OS_LINUX))

            headerDirectory = QFileInfo(plugInRoot + "/pch/").absoluteFilePath();

        #elif (defined(Q_OS_DARWIN))

            headerDirectory = QFileInfo(plugInRoot + "/Contents/Resources/pch/").absoluteFilePath();

        #else

            #error Unknown platform.

        #endif

        if (QFileInfo(headerDirectory).exists() && QFileInfo(headerDirectory).isDir()) {
            result << headerDirectory;
        }
    }

    return result;
}


QList<QString> DeploymentEnvironmentPrivate::plugInLibrarySearchPaths() const {
    QList<QString> result;
    QList<QString> plugInRoots = Ld::Environment::plugInDirectories() + Ld::Environment::userPlugInDirectories();

    for (QList<QString>::const_iterator it=plugInRoots.begin(),end=plugInRoots.end() ; it!=end ; ++it) {
        QString libraryDirectory;
        QString plugInRoot = *it;

        #if (defined(Q_OS_WIN))

            libraryDirectory = QFileInfo(plugInRoot + "/lib/").absoluteFilePath();

        #elif (defined(Q_OS_LINUX))

            libraryDirectory = QFileInfo(plugInRoot + "/lib/").absoluteFilePath();

        #elif (defined(Q_OS_DARWIN))

            libraryDirectory = QFileInfo(plugInRoot + "/Contents/Resources/lib/").absoluteFilePath();

        #else

            #error Unknown platform.

        #endif

        if (QFileInfo(libraryDirectory).exists() && QFileInfo(libraryDirectory).isDir()) {
            result << libraryDirectory;
        }
    }

    return result;
}


QList<QString> DeploymentEnvironmentPrivate::plugInRunTimeLibrarySearchPaths() const {
    QList<QString> result;
    QList<QString> plugInRoots = Ld::Environment::plugInDirectories() + Ld::Environment::userPlugInDirectories();

    for (QList<QString>::const_iterator it=plugInRoots.begin(),end=plugInRoots.end() ; it!=end ; ++it) {
        QString libraryDirectory;
        QString plugInRoot = *it;

        #if (defined(Q_OS_WIN))

            libraryDirectory = QFileInfo(plugInRoot + "/lib/").absoluteFilePath();

        #elif (defined(Q_OS_LINUX))

            libraryDirectory = QFileInfo(plugInRoot + "/lib/").absoluteFilePath();

        #elif (defined(Q_OS_DARWIN))

            libraryDirectory = QFileInfo(plugInRoot + "/Contents/Resources/lib/").absoluteFilePath();

        #else

            #error Unknown platform.

        #endif

        if (QFileInfo(libraryDirectory).exists() && QFileInfo(libraryDirectory).isDir()) {
            result << libraryDirectory;
        }
    }

    return result;
}


QList<QString> DeploymentEnvironmentPrivate::plugInFontSearchPaths() const {
    QList<QString> result;
    QList<QString> plugInRoots = Ld::Environment::plugInDirectories() + Ld::Environment::userPlugInDirectories();

    for (QList<QString>::const_iterator it=plugInRoots.begin(),end=plugInRoots.end() ; it!=end ; ++it) {
        QString plugInRoot = *it;

        #if (defined(Q_OS_LINUX) || defined(Q_OS_WIN))

            QString fontDirectoryPath = plugInRoot + "/fonts/";

        #elif (defined(Q_OS_DARWIN))

            QString fontDirectoryPath = plugInRoot + "/Contents/Resources/fonts/";

        #else

            #error Unknown platform

        #endif

        QDir fontsDirectory(fontDirectoryPath);
        if (fontsDirectory.exists()) {
            result << fontDirectoryPath;
        }
    }

    return result;
}
