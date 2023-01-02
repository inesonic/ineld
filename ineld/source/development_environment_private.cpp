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
* This file implements the \ref DevelopmentEnvironmentPrivate class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QFileInfo>
#include <QDir>
#include <QFileInfoList>
#include <QString>

#include <QDebug> // Debug

#include "ld_environment.h"
#include "ld_environment_private.h"
#include "development_environment_private.h"

DevelopmentEnvironmentPrivate::DevelopmentEnvironmentPrivate(
        const QString& executableFilePath,
        bool           useHeaderFiles
    ):Ld::EnvironmentPrivate(
        executableFilePath,
        useHeaderFiles
    ) {}


DevelopmentEnvironmentPrivate::~DevelopmentEnvironmentPrivate() {}


QString DevelopmentEnvironmentPrivate::supportExecutableDirectory() const {
    #if (defined(Q_OS_WIN))

        QString result = QString(LLVM_DEVELOPMENT_BINARY_PATH).replace(QChar('/'), QChar('\\'));

    #elif (defined(Q_OS_LINUX) || defined(Q_OS_DARWIN))

        QString result = LLVM_DEVELOPMENT_BINARY_PATH;

    #else

        #error Unknown platform.

    #endif

    return result;
}


QString DevelopmentEnvironmentPrivate::compilerRoot() const {
    QString result;

    if (!useHeaderFiles()) {
        // FIXME;
    }

    return result;
}


QString DevelopmentEnvironmentPrivate::compilerResourceDirectory() const {
    QString result;

    #if (defined(Q_OS_WIN))

        if (QFile("C:/opt/llvm-5.0.1/release").exists()) {
            result = "C:\\opt\\llvm-5.0.1\\release\\lib\\clang\\5.0.1\\";
        } else {
            result = "C:\\opt\\llvm-5.0.1\\debug\\lib\\clang\\5.0.01\\";
        }

    #elif (defined(Q_OS_LINUX) || defined(Q_OS_DARWIN))

        if (QFile("/opt/llvm-5.0.1/debug").exists()) {
            result = "/opt/llvm-5.0.1/debug/lib/clang/5.0.1/";
        } else {
            result = "/opt/llvm-5.0.1/release/lib/clang/5.0.1/";
        }

    #else

        #error Unknown platform.

    #endif

    return result;
}


QList<QString> DevelopmentEnvironmentPrivate::headerSearchPaths() const {
    QString sourceTopLevelDirectory = sourceRoot();

    QList<QString> result;
    result << QFileInfo(sourceTopLevelDirectory + "/libraries/inem/customer_include/").canonicalFilePath()
           << QFileInfo(sourceTopLevelDirectory + "/libraries/inem/include/").canonicalFilePath();

    return result;
}


QList<QString> DevelopmentEnvironmentPrivate::pchSearchPaths() const {
    QList<QString> result;
    result << QFileInfo(applicationRoot() + "/libraries/inem/pch/").canonicalFilePath();

    return result;
}


QList<QString> DevelopmentEnvironmentPrivate::librarySearchPaths(const QString& overrideRuntime) const {
    #if (defined(Q_OS_WIN))

        QList<QString> result = QString(LLVM_DEVELOPMENT_LIBRARY_PATHS).replace(QChar('/'), QChar('\\')).split(";");

        QString root = Ld::Environment::applicationRoot();

        if (!addPathIfExists(result, root + "/libraries/inem/build/Debug/", "inem.lib")) {
            addPathIfExists(result, root + "/libraries/inem/build/Release/", "inem.lib");
        }

        if (!addPathIfExists(result, root + "/libraries/inert/build/Debug/", "inert.lib")) {
            addPathIfExists(result, root + "/libraries/inert/build/Release/", "inert.lib");
        }

        if (!overrideRuntime.isEmpty()) {
            QFileInfo overrideRuntimeFileInformation(overrideRuntime);
            result << overrideRuntimeFileInformation.canonicalPath();
        }

        result << QFileInfo(MKL_ROOT).canonicalFilePath();

    #elif (defined(Q_OS_LINUX) || defined(Q_OS_DARWIN))

        (void) overrideRuntime;

        QList<QString> result = QString(LLVM_DEVELOPMENT_LIBRARY_PATHS).split(";");

        QString root = Ld::Environment::applicationRoot();
        if (QFile(root + "/libraries/inem/build/debug").exists()) {
            result << QFileInfo(root + "/libraries/inem/build/debug/").canonicalFilePath();
        } else {
            result << QFileInfo(root + "/libraries/inem/build/release/").canonicalFilePath();
        }

    #else

        #error Unknown platform.

    #endif

    return result;
}


QList<QString> DevelopmentEnvironmentPrivate::standardHeaderFiles() const {
    QList<QString> result;
    result << "model_api.h"
           << "model_interface.h"
           << "m_model_base.h";

    return result;
}


QList<QString> DevelopmentEnvironmentPrivate::standardPchFiles() const {
    QList<QString> result;
    result << "inem.pch";

    return result;
}


QList<QString> DevelopmentEnvironmentPrivate::standardStaticLibraries(const QString& overrideRuntime) const {
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


QList<QString> DevelopmentEnvironmentPrivate::standardDynamicLibraries() const {
    QList<QString> result;

    #if (defined(Q_OS_LINUX))

        result << "inem";
        result << "c++";

    #elif (defined(Q_OS_DARWIN))

        result << "inem";
        result << "c++";

    #elif (defined(Q_OS_WIN))

        // On Windows, the inem import library is included as part of the static runtime.  We don't need to include
        // it here.
        // result << "inem";

    #else

        #error Unknown platform.

    #endif

    return result;
}


QList<QString> DevelopmentEnvironmentPrivate::runTimeLibrarySearchPaths() const {
    QList<QString> result;

    #if (defined(Q_OS_LINUX))

        QString root = Ld::Environment::applicationRoot();

        if (QFile(root + "/libraries/inem/build/debug").exists()) {
            result << QFileInfo(root + "/libraries/inem/build/debug/").canonicalFilePath();
        } else {
            result << QFileInfo(root + "/libraries/inem/build/release/").canonicalFilePath();
        }

        if (QFile("/opt/llvm-10.0.0/debug/lib/").exists()) {
            result << "/opt/llvm-10.0.0/debug/lib/";
        } else {
            result << "/opt/llvm-10.0.0/release/lib/";
        }

    #elif (defined(Q_OS_DARWIN))

        QString root = Ld::Environment::applicationRoot();

        if (QFile(root + "/libraries/inem/build/debug").exists()) {
            result << QFileInfo(root + "/libraries/inem/build/debug/").canonicalFilePath();
        } else {
            result << QFileInfo(root + "/libraries/inem/build/release/").canonicalFilePath();
        }

        if (QFile("/opt/llvm-10.0.0/debug/lib/").exists()) {
            result << "/opt/llvm-10.0.0/debug/lib/";
        } else {
            result << "/opt/llvm-10.0.0/release/lib/";
        }

    #elif (!defined(Q_OS_WIN))

        #error Unknown platform.

    #endif

    return result;
}


QList<QString> DevelopmentEnvironmentPrivate::fontSearchPaths() const {
    QList<QString> result;

    QDir fontsDirectory("/opt/fonts");
    if (fontsDirectory.exists()) {
        result << fontsDirectory.path();
    }

    return result;
}


QString DevelopmentEnvironmentPrivate::examplesDirectory() const {
    return QFileInfo(sourceRoot() + "/../examples/").canonicalFilePath();
}


QString DevelopmentEnvironmentPrivate::plugInRootDirectory() const {
    return QFileInfo(Ld::Environment::applicationRoot() + "/plugins/").canonicalFilePath();
}


QList<QString> DevelopmentEnvironmentPrivate::plugInDirectories() const {
    QList<QString> result;
    QString        plugInRoot   = Ld::Environment::plugInRootDirectory();
    QFileInfoList  fileInfoList = QDir(plugInRoot).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (QFileInfoList::const_iterator it=fileInfoList.constBegin(),end=fileInfoList.constEnd() ; it!=end ; ++it) {
        result.append(it->canonicalFilePath());
    }

    return result;
}


QList<QString> DevelopmentEnvironmentPrivate::plugInFiles() const {
    QList<QString> result;
    QList<QString> plugInDirectories = Ld::Environment::plugInDirectories();

    for (  QList<QString>::const_iterator plugInIterator    = plugInDirectories.constBegin(),
                                          plugInEndIterator = plugInDirectories.constEnd()
         ; plugInIterator != plugInEndIterator
         ; ++plugInIterator
        ) {
        QString plugInRoot = *plugInIterator;
        QString plugInName = QFileInfo(plugInRoot).fileName();

        QStringList   filters;
        QFileInfoList plugInFiles;

        #if (defined(Q_OS_LINUX) || defined(Q_OS_DARWIN))

            #if (defined(Q_OS_LINUX))

                filters << "*.so";

            #elif (defined(Q_OS_DARWIN))

                filters << "*.dylib" << "*.bundle";

            #endif

            QFileInfo debugPlugInFileDirectory(plugInRoot + "/" + plugInName + "/build/debug/");
            if (debugPlugInFileDirectory.exists() && debugPlugInFileDirectory.isDir()) {
                plugInFiles += QDir(debugPlugInFileDirectory.canonicalFilePath()).entryInfoList(filters, QDir::Files);
            }

            QFileInfo releasePlugInFileDirectory(plugInRoot + "/" + plugInName + "/build/release/");
            if (releasePlugInFileDirectory.exists() && releasePlugInFileDirectory.isDir()) {
                plugInFiles += QDir(releasePlugInFileDirectory.canonicalFilePath()).entryInfoList(
                    filters,
                    QDir::Files
                );
            }

        #elif (defined(Q_OS_WIN))

            filters << "*.dll";

            QFileInfo debugPlugInFileDirectory(plugInRoot + "/" + plugInName + "/build/Debug/");
            if (debugPlugInFileDirectory.exists() && debugPlugInFileDirectory.isDir()) {
                plugInFiles += QDir(debugPlugInFileDirectory.canonicalFilePath()).entryInfoList(filters, QDir::Files);
            }

            QFileInfo releasePlugInFileDirectory(plugInRoot + "/" + plugInName + "/build/Release/");
            if (releasePlugInFileDirectory.exists() && releasePlugInFileDirectory.isDir()) {
                plugInFiles += QDir(releasePlugInFileDirectory.canonicalFilePath()).entryInfoList(
                    filters,
                    QDir::Files
                );
            }

        #else

            #error Unknown platform

        #endif

        for (  QFileInfoList::const_iterator plugInIterator    = plugInFiles.constBegin(),
                                             plugInEndIterator = plugInFiles.constEnd()
             ; plugInIterator != plugInEndIterator
             ; ++plugInIterator
            ) {
            result << plugInIterator->canonicalFilePath();
        }
    }

    plugInDirectories = Ld::Environment::userPlugInDirectories();
    for (  QList<QString>::const_iterator plugInIterator    = plugInDirectories.constBegin(),
                                          plugInEndIterator = plugInDirectories.constEnd()
         ; plugInIterator != plugInEndIterator
         ; ++plugInIterator
        ) {
        QString plugInRoot = *plugInIterator;
        QString plugInName = QFileInfo(plugInRoot).fileName();

        QStringList   filters;
        QFileInfoList plugInFiles;

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


QList<QString> DevelopmentEnvironmentPrivate::plugInHeaderSearchPaths() const {
    QList<QString> result;
    QString        sourcePlugInRoot        = QFileInfo(sourceRoot() + "/plugins/").canonicalFilePath();
    QFileInfoList  plugInSourceDirectories = QDir(sourcePlugInRoot).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (  QFileInfoList::const_iterator plugInDirectoryIterator    = plugInSourceDirectories.constBegin(),
                                         plugInDirectoryEndIterator = plugInSourceDirectories.constEnd()
         ; plugInDirectoryIterator != plugInDirectoryEndIterator
         ; ++plugInDirectoryIterator
        ) {
        QString plugInRoot = plugInDirectoryIterator->canonicalFilePath();
        QFileInfo plugInLibraryRoot(plugInRoot + "/libraries/");

        if (plugInLibraryRoot.exists() && plugInLibraryRoot.isDir()) {
            QDir plugInLibraryDirectory(plugInLibraryRoot.canonicalFilePath());
            QFileInfoList plugInLibraries = plugInLibraryDirectory.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
            for (  QFileInfoList::const_iterator plugInLibraryIterator    = plugInLibraries.constBegin(),
                                                 plugInLibraryEndIterator = plugInLibraries.constEnd()
                 ; plugInLibraryIterator != plugInLibraryEndIterator
                 ; ++plugInLibraryIterator
                ) {
                QString libraryRoot = plugInLibraryIterator->canonicalFilePath();

                QFileInfo plugInIncludePathInfo(libraryRoot + "/include/");
                if (plugInIncludePathInfo.exists() && plugInIncludePathInfo.isDir()) {
                    result.append(plugInIncludePathInfo.canonicalFilePath());
                }

                QFileInfo plugInCustomerIncludePath(libraryRoot + "/customer_include/");
                if (plugInCustomerIncludePath.exists() && plugInCustomerIncludePath.isDir()) {
                    result.append(plugInCustomerIncludePath.canonicalFilePath());
                }
            }
        }
    }

    return result;
}


QList<QString> DevelopmentEnvironmentPrivate::plugInPchSearchPaths() const {
    QList<QString> result;

    QList<QString> plugInDirectories = Ld::Environment::plugInDirectories() + Ld::Environment::userPlugInDirectories();
    for (  QList<QString>::const_iterator plugInDirectoryIterator    = plugInDirectories.constBegin(),
                                          plugInDirectoryEndIterator = plugInDirectories.constEnd()
         ; plugInDirectoryIterator != plugInDirectoryEndIterator
         ; ++plugInDirectoryIterator
        ) {
        QString plugInDirectory = *plugInDirectoryIterator;
        QFileInfo plugInLibraryRoot(plugInDirectory + "/libraries/");

        if (plugInLibraryRoot.exists() && plugInLibraryRoot.isDir()) {
            QDir plugInLibraryDirectory(plugInLibraryRoot.canonicalFilePath());
            QFileInfoList plugInLibraries = plugInLibraryDirectory.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
            for (  QFileInfoList::const_iterator plugInLibraryIterator    = plugInLibraries.constBegin(),
                                                 plugInLibraryEndIterator = plugInLibraries.constEnd()
                 ; plugInLibraryIterator != plugInLibraryEndIterator
                 ; ++plugInLibraryIterator
                ) {
                QString libraryRoot = plugInLibraryIterator->canonicalFilePath();

                QFileInfo plugInPchPathInfo(libraryRoot + "/pch/");
                if (plugInPchPathInfo.exists() && plugInPchPathInfo.isDir()) {
                    result.append(plugInPchPathInfo.canonicalFilePath());
                }
            }
        }
    }

    return result;
}


QList<QString> DevelopmentEnvironmentPrivate::plugInLibrarySearchPaths() const {
    QList<QString> result;

    QList<QString> plugInDirectories = Ld::Environment::plugInDirectories();
    for (  QList<QString>::const_iterator plugInDirectoryIterator    = plugInDirectories.constBegin(),
                                          plugInDirectoryEndIterator = plugInDirectories.constEnd()
         ; plugInDirectoryIterator != plugInDirectoryEndIterator
         ; ++plugInDirectoryIterator
        ) {
        QString plugInDirectory = *plugInDirectoryIterator;
        QFileInfo plugInLibraryRoot(plugInDirectory + "/libraries/");

        if (plugInLibraryRoot.exists() && plugInLibraryRoot.isDir()) {
            QDir plugInLibraryDirectory(plugInLibraryRoot.canonicalFilePath());
            QFileInfoList plugInLibraries = plugInLibraryDirectory.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
            for (  QFileInfoList::const_iterator plugInLibraryIterator    = plugInLibraries.constBegin(),
                                                 plugInLibraryEndIterator = plugInLibraries.constEnd()
                 ; plugInLibraryIterator != plugInLibraryEndIterator
                 ; ++plugInLibraryIterator
                ) {
                QString libraryRoot = plugInLibraryIterator->canonicalFilePath();

                QFileInfo debugPath(libraryRoot + "/build/debug/");
                if (debugPath.exists()) {
                    result.append(debugPath.canonicalFilePath());
                }

                QFileInfo releasePath(libraryRoot + "/build/release/");
                if (releasePath.exists()) {
                    result.append(releasePath.canonicalFilePath());
                }
            }
        }
    }

    plugInDirectories = Ld::Environment::userPlugInDirectories();
    for (  QList<QString>::const_iterator plugInDirectoryIterator    = plugInDirectories.constBegin(),
                                          plugInDirectoryEndIterator = plugInDirectories.constEnd()
         ; plugInDirectoryIterator != plugInDirectoryEndIterator
         ; ++plugInDirectoryIterator
        ) {
        QFileInfo libraryDirectory;
        QString plugInDirectory = *plugInDirectoryIterator;

        #if (defined(Q_OS_WIN))

            libraryDirectory = QFileInfo(QFileInfo(plugInDirectory + "/lib/").absoluteFilePath());

        #elif (defined(Q_OS_LINUX))

            libraryDirectory = QFileInfo(QFileInfo(plugInDirectory + "/lib/").absoluteFilePath());

        #elif (defined(Q_OS_DARWIN))

            libraryDirectory = QFileInfo(QFileInfo(plugInDirectory + "/Contents/Resources/lib/").absoluteFilePath());

        #else

            #error Unknown platform.

        #endif

        if (libraryDirectory.exists() && libraryDirectory.isDir()) {
            result << libraryDirectory.canonicalFilePath();
        }
    }

    return result;
}


QList<QString> DevelopmentEnvironmentPrivate::plugInRunTimeLibrarySearchPaths() const {
    QList<QString> result;

    #if (defined(Q_OS_WIN))

        QFileInfo runtimePath("C:/Program Files (x86)/Windows Kits/10/Lib/10.0.17763.0/ucrt/x64/");
        if (runtimePath.exists() && runtimePath.isDir()) {
            result << runtimePath.canonicalFilePath();
        } else {
            runtimePath = QFileInfo("C:/Program Files (x86)/Windows Kits/10/Lib/10.0.19041.0/ucrt/x64/");
            if (runtimePath.exists() && runtimePath.isDir()) {
                result << runtimePath.canonicalFilePath();
            }
        }

        result += DevelopmentEnvironmentPrivate::plugInLibrarySearchPaths();

    #elif (defined(Q_OS_DARWIN) || defined(Q_OS_LINUX))

        result = DevelopmentEnvironmentPrivate::plugInLibrarySearchPaths();

    #else

        #error Unknown platform

    #endif

    return result;
}


QList<QString> DevelopmentEnvironmentPrivate::plugInFontSearchPaths() const {
    QList<QString> result;

    QList<QString> plugInDirectories = Ld::Environment::plugInDirectories();
    for (  QList<QString>::const_iterator plugInDirectoryIterator    = plugInDirectories.constBegin(),
                                          plugInDirectoryEndIterator = plugInDirectories.constEnd()
         ; plugInDirectoryIterator != plugInDirectoryEndIterator
         ; ++plugInDirectoryIterator
        ) {
        QString plugInDirectory = *plugInDirectoryIterator;
        QFileInfo plugInLibraryRoot(plugInDirectory + "/libraries/");

        if (plugInLibraryRoot.exists() && plugInLibraryRoot.isDir()) {
            QDir plugInLibraryDirectory(plugInLibraryRoot.canonicalFilePath());
            QFileInfoList plugInLibraries = plugInLibraryDirectory.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
            for (  QFileInfoList::const_iterator plugInLibraryIterator    = plugInLibraries.constBegin(),
                                                 plugInLibraryEndIterator = plugInLibraries.constEnd()
                 ; plugInLibraryIterator != plugInLibraryEndIterator
                 ; ++plugInLibraryIterator
                ) {
                QString libraryRoot = plugInLibraryIterator->canonicalFilePath();

                QFileInfo debugPath(libraryRoot + "/build/debug/");
                if (debugPath.exists()) {
                    result.append(debugPath.canonicalFilePath());
                }

                QFileInfo releasePath(libraryRoot + "/build/release/");
                if (releasePath.exists()) {
                    result.append(releasePath.canonicalFilePath());
                }
            }
        }
    }

    plugInDirectories = Ld::Environment::userPlugInDirectories();
    for (  QList<QString>::const_iterator plugInDirectoryIterator    = plugInDirectories.constBegin(),
                                          plugInDirectoryEndIterator = plugInDirectories.constEnd()
         ; plugInDirectoryIterator != plugInDirectoryEndIterator
         ; ++plugInDirectoryIterator
        ) {
        QFileInfo libraryDirectory;
        QString plugInDirectory = *plugInDirectoryIterator;

        #if (defined(Q_OS_WIN))

            libraryDirectory = QFileInfo(QFileInfo(plugInDirectory + "/fonts/").absoluteFilePath());

        #elif (defined(Q_OS_LINUX))

            libraryDirectory = QFileInfo(QFileInfo(plugInDirectory + "/fonts/").absoluteFilePath());

        #elif (defined(Q_OS_DARWIN))

            libraryDirectory = QFileInfo(
                QFileInfo(plugInDirectory + "/Contents/Resources/fonts/").absoluteFilePath()
            );

        #else

            #error Unknown platform.

        #endif

        if (libraryDirectory.exists() && libraryDirectory.isDir()) {
            result << libraryDirectory.canonicalFilePath();
        }
    }

    return result;
}


QString DevelopmentEnvironmentPrivate::sourceRoot() const {
    QString sourceRoot = QFileInfo(QFileInfo(__FILE__).canonicalPath() + "/../../").canonicalPath();
    return sourceRoot;
}
