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
* This file implements tests of the Ld::Environment class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QCoreApplication>
#include <QDebug>
#include <QObject>
#include <QFileInfo>

#include <ld_environment.h>

#include "test_environment.h"

TestEnvironment::TestEnvironment() {}

TestEnvironment::~TestEnvironment() {}

void TestEnvironment::testDevelopmentTestEnvironment() {
    QString executableFilePath = QCoreApplication::applicationFilePath();
    QString sourceRoot         = QFileInfo(QFileInfo(__FILE__).absolutePath() + "/../../").canonicalFilePath();

    for (unsigned useHeaderFile=0 ; useHeaderFile<=1 ; ++useHeaderFile) {
        Ld::Environment::configure(
            executableFilePath,
            Ld::Environment::Type::TEST_DEVELOPMENT,
            bool(useHeaderFile)
        );

        QVERIFY(Ld::Environment::useHeaderFiles() == bool(useHeaderFile));
        QVERIFY(Ld::Environment::usePchFiles() != bool(useHeaderFile));
        QVERIFY(Ld::Environment::executableType() == Ld::Environment::Type::TEST_DEVELOPMENT);

        // TODO: Add support for Ld::Environment::fontSearchPaths and plugInFontSearchPaths

        #if (defined(Q_OS_WIN))

            QVERIFY(Ld::Environment::executableDirectory() == QFileInfo(executableFilePath).absolutePath());

            QString supportExecutableDirectory = Ld::Environment::supportExecutableDirectory();
            QVERIFY(   QFileInfo(supportExecutableDirectory) == QFileInfo("C:\\opt\\llvm-5.0.1\\debug\\bin\\")
                    || QFileInfo(supportExecutableDirectory) == QFileInfo("C:\\opt\\llvm-5.0.1\\release\\bin\\")
                   );

            if (Ld::Environment::useHeaderFiles()) {
                QVERIFY(Ld::Environment::compilerRoot().isEmpty());

                QList<QString> headerSearchPaths = Ld::Environment::headerSearchPaths();
                QVERIFY(headerSearchPaths.size() == 2);
                QString inemInclude = QFileInfo(sourceRoot + "/libraries/inem/").canonicalFilePath();
                QVERIFY(headerSearchPaths.contains(inemInclude + "/include"));
                QVERIFY(headerSearchPaths.contains(inemInclude + "/customer_include"));

                QList<QString> pchSearchPaths = Ld::Environment::pchSearchPaths();
                QVERIFY(pchSearchPaths.isEmpty());

                QList<QString> headers = Ld::Environment::standardHeaderFiles();
                QVERIFY(headers.contains("model_api.h"));
                QVERIFY(headers.contains("m_model_base.h"));
                QVERIFY(headers.contains("model_interface.h"));

                QVERIFY(Ld::Environment::standardPchFiles().isEmpty());
            } else {
                // This section needs to be fixed.
            }

            QList<QString> librarySearchPaths = Ld::Environment::librarySearchPaths();
            QVERIFY(librarySearchPaths.size() == 2);

            QString buildRoot = QFileInfo(
                QFileInfo(executableFilePath).absolutePath() + "/../../../../"
            ).canonicalFilePath();

            QString libraryDirectory = QFileInfo(buildRoot + "/libraries/inem/build/").canonicalFilePath();
            QVERIFY(   librarySearchPaths.contains(libraryDirectory + "/Debug")
                    || librarySearchPaths.contains(libraryDirectory + "/Release")
                   );
            QVERIFY(   librarySearchPaths.contains("C:/opt/llvm-5.0.1/Debug/lib/")
                    || librarySearchPaths.contains("C:/opt/llvm-5.0.1/Release/lib/")
                   );

            QList<QString> standardStaticLibraries = Ld::Environment::standardStaticLibraries();
            QVERIFY(standardStaticLibraries.size() == 2);
            QVERIFY(   standardStaticLibraries.contains(libraryDirectory + "/Debug/inem")
                    || standardStaticLibraries.contains(libraryDirectory + "/Release/inem")
                   );
            QVERIFY(standardStaticLibraries.contains(
                "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/lib/libcpmt"
            ));

            QList<QString> standardDynamicLibraries = Ld::Environment::standardDynamicLibraries();
            QVERIFY(standardDynamicLibraries.isEmpty());

            QList<QString> runTimeLibrarySearchPaths = Ld::Environment::runTimeLibrarySearchPaths();
            QVERIFY(runTimeLibrarySearchPaths.isEmpty());

            QVERIFY(false); // Add test for plugInRootDirectory() root directory
            QVERIFY(false); // Add test for plugInDirectories()
            QVERIFY(false); // Add test for plugInHeaderSearchPaths()
            QVERIFY(false); // Add test for plugInPchSearchPaths()
            QVERIFY(false); // Add test for plugInLibrarySearchPaths()
            QVERIFY(false); // Add test for plugInFontSearchPaths()

            QString plugInRootDirectory = Ld::Environment::plugInRootDirectory();
            QVERIFY(QFileInfo(plugInRootDirectory) == QFileInfo(buildRoot + "/plugins/"));

            QList<QString> plugInDirectories = Ld::Environment::plugInDirectories();
            QVERIFY(plugInDirectories.size() >= 1);
            QVERIFY(plugInDirectories.contains(QFileInfo(plugInRootDirectory + "/audiofile/").canonicalFilePath()));

            QList<QString> plugInFiles = Ld::Environment::plugInFiles();

            QString expectedDebugPlugIn   = (
                  buildRoot
                + "/plugins/audiofile/audiofile/build/Debug/libaudiofile.1.0.0.dylib"
            );

            QString expectedReleasePlugIn = (
                  buildRoot
                + "/plugins/audiofile/audiofile/build/Release/libaudiofile.1.0.0.dylib"
            );

            QVERIFY(    plugInFiles.contains(QFileInfo(expectedDebugPlugIn).canonicalFilePath())
                    || plugInFiles.contains(QFileInfo(expectedReleasePlugIn).canonicalFilePath())
                   );

            QList<QString> plugInIncludeDirectories = Ld::Environment::plugInHeaderSearchPaths();
            QFileInfoList  headerFiles;

            for (  QList<QString>::const_iterator
                       plugInIncludeDirectoryIterator    = plugInIncludeDirectories.constBegin(),
                       plugInIncludeDirectoryEndIterator = plugInIncludeDirectories.constEnd()
                 ; plugInIncludeDirectoryIterator != plugInIncludeDirectoryEndIterator
                 ; ++plugInIncludeDirectoryIterator
                ) {
                QString includeDirectory = *plugInIncludeDirectoryIterator;
                headerFiles += QDir(includeDirectory).entryInfoList(QDir::Files);
            }

            QSet<QString> allHeaders;
            for (  QFileInfoList::const_iterator headerIterator    = headerFiles.constBegin(),
                                                 headerEndIterator = headerFiles.constEnd()
                 ; headerIterator != headerEndIterator
                 ; ++headerIterator
                ) {
                allHeaders << headerIterator->fileName();
            }

            QVERIFY(allHeaders.contains("audiofile_common.h"));

            QList<QString> plugInPchDirectories = Ld::Environment::plugInPchSearchPaths();
            QFileInfoList  pchFiles;
            for (  QList<QString>::const_iterator
                       plugInPchDirectoryIterator    = plugInPchDirectories.constBegin(),
                       plugInPchDirectoryEndIterator = plugInPchDirectories.constEnd()
                 ; plugInPchDirectoryIterator != plugInPchDirectoryEndIterator
                 ; ++plugInPchDirectoryIterator
                ) {
                QString pchDirectory = *plugInPchDirectoryIterator;
                pchFiles += QDir(pchDirectory).entryInfoList(QDir::Files);
            }

            QSet<QString> allPchFiles;
            for (  QFileInfoList::const_iterator pchIterator    = pchFiles.constBegin(),
                                                 pchEndIterator = pchFiles.constEnd()
                 ; pchIterator != pchEndIterator
                 ; ++pchIterator
                ) {
                allPchFiles << pchIterator->fileName();
            }

            QVERIFY(allPchFiles.contains("audiofile.pch"));

            QList<QString> plugInLibrarySearchPath = Ld::Environment::plugInLibrarySearchPaths();

            expectedDebugPlugIn   = buildRoot + "/plugins/audiofile/libraries/ineaudiofile/build/Debug";
            expectedReleasePlugIn = buildRoot + "/plugins/audiofile/libraries/ineaudiofile/build/Release";

            QVERIFY(   plugInLibrarySearchPath.contains(QFileInfo(expectedDebugPlugIn).canonicalFilePath())
                    || plugInLibrarySearchPath.contains(QFileInfo(expectedReleasePlugIn).canonicalFilePath())
            );

//            QVERIFY(false); // Add test for plugInFontSearchPaths()

        #elif (defined(Q_OS_LINUX))

            QVERIFY(Ld::Environment::executableDirectory() == QFileInfo(executableFilePath).absolutePath());

            QString supportExecutableDirectory = Ld::Environment::supportExecutableDirectory();
            QVERIFY(   QFileInfo(supportExecutableDirectory) == QFileInfo("/opt/llvm-8.0.0/debug/bin/")
                    || QFileInfo(supportExecutableDirectory) == QFileInfo("/opt/llvm-8.0.0/release/bin/")
                   );

            if (Ld::Environment::useHeaderFiles()) {
                QVERIFY(Ld::Environment::compilerRoot().isEmpty());

                QList<QString> headerSearchPaths = Ld::Environment::headerSearchPaths();
                QVERIFY(headerSearchPaths.size() == 2);
                QString inemInclude = QFileInfo(sourceRoot + "/libraries/inem/").canonicalFilePath();
                QVERIFY(headerSearchPaths.contains(inemInclude + "/include"));
                QVERIFY(headerSearchPaths.contains(inemInclude + "/customer_include"));

                QList<QString> pchSearchPaths = Ld::Environment::pchSearchPaths();
                QVERIFY(pchSearchPaths.isEmpty());

                QList<QString> headers = Ld::Environment::standardHeaderFiles();
                QVERIFY(headers.contains("model_api.h"));
                QVERIFY(headers.contains("m_model_base.h"));
                QVERIFY(headers.contains("model_interface.h"));

                QVERIFY(Ld::Environment::standardPchFiles().isEmpty());
            } else {
                // This section needs to be fixed.
            }

            QList<QString> librarySearchPaths = Ld::Environment::librarySearchPaths();
            QVERIFY(librarySearchPaths.size() == 2);

            QString buildRoot = QFileInfo(
                QFileInfo(executableFilePath).absolutePath() + "/../../../../"
            ).canonicalFilePath();

            QString libraryDirectory = QFileInfo(buildRoot + "/libraries/inem/build/").canonicalFilePath();
            QVERIFY(   librarySearchPaths.contains(libraryDirectory + "/debug")
                    || librarySearchPaths.contains(libraryDirectory + "/release")
                   );

            QVERIFY(   librarySearchPaths.contains("/opt/llvm-5.0.1/release/lib/")
                    || librarySearchPaths.contains("opt/llvm-5.0.1/debug/lib/")
                   );

            QList<QString> standardStaticLibraries = Ld::Environment::standardStaticLibraries();
            QVERIFY(standardStaticLibraries.isEmpty());

            QList<QString> standardDynamicLibraries = Ld::Environment::standardDynamicLibraries();
            QVERIFY(standardDynamicLibraries.size() == 2);
            QVERIFY(standardDynamicLibraries.contains("inem"));
            QVERIFY(standardDynamicLibraries.contains("c++"));

            QList<QString> runTimeLibrarySearchPaths = Ld::Environment::runTimeLibrarySearchPaths();
            QVERIFY(runTimeLibrarySearchPaths.size() == 2);

            QVERIFY(   runTimeLibrarySearchPaths.contains(libraryDirectory + "/debug")
                    || runTimeLibrarySearchPaths.contains(libraryDirectory + "/release")
                   );

            QVERIFY(   runTimeLibrarySearchPaths.contains("/opt/llvm-5.0.1/release/lib/")
                    || runTimeLibrarySearchPaths.contains("opt/llvm-5.0.1/debug/lib/")
                   );

            QVERIFY(false); // Add test for plugInRootDirectory() root directory
            QVERIFY(false); // Add test for plugInDirectories()
            QVERIFY(false); // Add test for plugInHeaderSearchPaths()
            QVERIFY(false); // Add test for plugInPchSearchPaths()
            QVERIFY(false); // Add test for plugInLibrarySearchPaths()
            QVERIFY(false); // Add test for plugInFontSearchPaths()

            QString plugInRootDirectory = Ld::Environment::plugInRootDirectory();
            QVERIFY(QFileInfo(plugInRootDirectory) == QFileInfo(buildRoot + "/plugins/"));

            QList<QString> plugInDirectories = Ld::Environment::plugInDirectories();
            QVERIFY(plugInDirectories.size() >= 1);
            QVERIFY(plugInDirectories.contains(QFileInfo(plugInRootDirectory + "/audiofile/").canonicalFilePath()));

            QList<QString> plugInFiles = Ld::Environment::plugInFiles();

            QString expectedDebugPlugIn   = (
                  buildRoot
                + "/plugins/audiofile/audiofile/build/debug/libaudiofile.1.0.0.dylib"
            );
            QString expectedReleasePlugIn = (
                  buildRoot
                + "/plugins/audiofile/audiofile/build/release/libaudiofile.1.0.0.dylib"
            );

            QVERIFY(    plugInFiles.contains(QFileInfo(expectedDebugPlugIn).canonicalFilePath())
                    || plugInFiles.contains(QFileInfo(expectedReleasePlugIn).canonicalFilePath())
                   );

            QList<QString> plugInIncludeDirectories = Ld::Environment::plugInHeaderSearchPaths();
            QFileInfoList  headerFiles;

            for (  QList<QString>::const_iterator
                       plugInIncludeDirectoryIterator    = plugInIncludeDirectories.constBegin(),
                       plugInIncludeDirectoryEndIterator = plugInIncludeDirectories.constEnd()
                 ; plugInIncludeDirectoryIterator != plugInIncludeDirectoryEndIterator
                 ; ++plugInIncludeDirectoryIterator
                ) {
                QString includeDirectory = *plugInIncludeDirectoryIterator;
                headerFiles += QDir(includeDirectory).entryInfoList(QDir::Files);
            }

            QSet<QString> allHeaders;
            for (  QFileInfoList::const_iterator headerIterator    = headerFiles.constBegin(),
                                                 headerEndIterator = headerFiles.constEnd()
                 ; headerIterator != headerEndIterator
                 ; ++headerIterator
                ) {
                allHeaders << headerIterator->fileName();
            }

            QVERIFY(allHeaders.contains("audiofile_common.h"));

            QList<QString> plugInPchDirectories = Ld::Environment::plugInPchSearchPaths();
            QFileInfoList  pchFiles;
            for (  QList<QString>::const_iterator
                       plugInPchDirectoryIterator    = plugInPchDirectories.constBegin(),
                       plugInPchDirectoryEndIterator = plugInPchDirectories.constEnd()
                 ; plugInPchDirectoryIterator != plugInPchDirectoryEndIterator
                 ; ++plugInPchDirectoryIterator
                ) {
                QString pchDirectory = *plugInPchDirectoryIterator;
                pchFiles += QDir(pchDirectory).entryInfoList(QDir::Files);
            }

            QSet<QString> allPchFiles;
            for (  QFileInfoList::const_iterator pchIterator    = pchFiles.constBegin(),
                                                 pchEndIterator = pchFiles.constEnd()
                 ; pchIterator != pchEndIterator
                 ; ++pchIterator
                ) {
                allPchFiles << pchIterator->fileName();
            }

            QVERIFY(allPchFiles.contains("audiofile.pch"));

            QList<QString> plugInLibrarySearchPath = Ld::Environment::plugInLibrarySearchPaths();

            expectedDebugPlugIn   = buildRoot + "/plugins/audiofile/libraries/ineaudiofile/build/debug";
            expectedReleasePlugIn = buildRoot + "/plugins/audiofile/libraries/ineaudiofile/build/release";

            QVERIFY(   plugInLibrarySearchPath.contains(QFileInfo(expectedDebugPlugIn).canonicalFilePath())
                    || plugInLibrarySearchPath.contains(QFileInfo(expectedReleasePlugIn).canonicalFilePath())
            );

//            QVERIFY(false); // Add test for plugInFontSearchPaths()

        #elif (defined(Q_OS_DARWIN))

            QVERIFY(Ld::Environment::executableDirectory() == QFileInfo(executableFilePath).absolutePath());

            QString supportExecutableDirectory = Ld::Environment::supportExecutableDirectory();
            QVERIFY(   QFileInfo(supportExecutableDirectory) == QFileInfo("/opt/llvm-10.0.0/debug/bin/")
                    || QFileInfo(supportExecutableDirectory) == QFileInfo("/opt/llvm-10.0.0/release/bin/")
                   );

            if (Ld::Environment::useHeaderFiles()) {
                QVERIFY(Ld::Environment::compilerRoot().isEmpty());

                QList<QString> headerSearchPaths = Ld::Environment::headerSearchPaths();
                QVERIFY(headerSearchPaths.size() == 2);
                QString inemInclude = QFileInfo(sourceRoot + "/libraries/inem/").canonicalFilePath();
                QVERIFY(headerSearchPaths.contains(inemInclude + "/include"));
                QVERIFY(headerSearchPaths.contains(inemInclude + "/customer_include"));

                QList<QString> pchSearchPaths = Ld::Environment::pchSearchPaths();
                QVERIFY(pchSearchPaths.isEmpty());

                QList<QString> headers = Ld::Environment::standardHeaderFiles();
                QVERIFY(headers.contains("model_api.h"));
                QVERIFY(headers.contains("m_model_base.h"));
                QVERIFY(headers.contains("model_interface.h"));

                QVERIFY(Ld::Environment::standardPchFiles().isEmpty());
            } else {
                // FIXME
                // QVERIFY(Ld::Environment::standardPchFiles().isEmpty()); // This section needs to be fixed.
            }

            QList<QString> librarySearchPaths = Ld::Environment::librarySearchPaths();
            QVERIFY(librarySearchPaths.size() == 2);

            QString buildRoot = QFileInfo(
                QFileInfo(executableFilePath).absolutePath() + "/../../../../../../../"
            ).canonicalFilePath();

            QString libraryDirectory = QFileInfo(buildRoot + "/libraries/inem/build/").canonicalFilePath();
            QVERIFY(   librarySearchPaths.contains(libraryDirectory + "/debug")
                    || librarySearchPaths.contains(libraryDirectory + "/release")
                   );
            QList<QString> standardStaticLibraries = Ld::Environment::standardStaticLibraries();
            QVERIFY(standardStaticLibraries.isEmpty());

            QList<QString> standardDynamicLibraries = Ld::Environment::standardDynamicLibraries();
            QVERIFY(standardDynamicLibraries.size() == 2);
            QVERIFY(standardDynamicLibraries.contains("inem"));
            QVERIFY(standardDynamicLibraries.contains("c++"));

            QList<QString> runTimeLibrarySearchPaths = Ld::Environment::runTimeLibrarySearchPaths();
            QVERIFY(runTimeLibrarySearchPaths.size() == 2);

            QVERIFY(   runTimeLibrarySearchPaths.contains(libraryDirectory + "/debug")
                    || runTimeLibrarySearchPaths.contains(libraryDirectory + "/release")
                   );

            QVERIFY(   runTimeLibrarySearchPaths.contains("/opt/llvm-5.0.1/release/lib/")
                    || runTimeLibrarySearchPaths.contains("/opt/llvm-5.0.1/debug/lib/")
                   );

            QString plugInRootDirectory = Ld::Environment::plugInRootDirectory();
            QVERIFY(QFileInfo(plugInRootDirectory) == QFileInfo(buildRoot + "/plugins/"));

            QList<QString> plugInDirectories = Ld::Environment::plugInDirectories();
            QVERIFY(plugInDirectories.size() >= 1);
            QVERIFY(plugInDirectories.contains(QFileInfo(plugInRootDirectory + "/audiofile/").canonicalFilePath()));

            QList<QString> plugInFiles = Ld::Environment::plugInFiles();

            QString expectedDebugPlugIn   = (
                  buildRoot
                + "/plugins/audiofile/audiofile/build/debug/libaudiofile.1.0.0.dylib"
            );
            QString expectedReleasePlugIn = (
                  buildRoot
                + "/plugins/audiofile/audiofile/build/release/libaudiofile.1.0.0.dylib"
            );

            QVERIFY(    plugInFiles.contains(QFileInfo(expectedDebugPlugIn).canonicalFilePath())
                    || plugInFiles.contains(QFileInfo(expectedReleasePlugIn).canonicalFilePath())
                   );

            QList<QString> plugInIncludeDirectories = Ld::Environment::plugInHeaderSearchPaths();
            QFileInfoList  headerFiles;

            for (  QList<QString>::const_iterator
                       plugInIncludeDirectoryIterator    = plugInIncludeDirectories.constBegin(),
                       plugInIncludeDirectoryEndIterator = plugInIncludeDirectories.constEnd()
                 ; plugInIncludeDirectoryIterator != plugInIncludeDirectoryEndIterator
                 ; ++plugInIncludeDirectoryIterator
                ) {
                QString includeDirectory = *plugInIncludeDirectoryIterator;
                headerFiles += QDir(includeDirectory).entryInfoList(QDir::Files);
            }

            QSet<QString> allHeaders;
            for (  QFileInfoList::const_iterator headerIterator    = headerFiles.constBegin(),
                                                 headerEndIterator = headerFiles.constEnd()
                 ; headerIterator != headerEndIterator
                 ; ++headerIterator
                ) {
                allHeaders << headerIterator->fileName();
            }

            QVERIFY(allHeaders.contains("audiofile_common.h"));

            QList<QString> plugInPchDirectories = Ld::Environment::plugInPchSearchPaths();
            QFileInfoList  pchFiles;
            for (  QList<QString>::const_iterator
                       plugInPchDirectoryIterator    = plugInPchDirectories.constBegin(),
                       plugInPchDirectoryEndIterator = plugInPchDirectories.constEnd()
                 ; plugInPchDirectoryIterator != plugInPchDirectoryEndIterator
                 ; ++plugInPchDirectoryIterator
                ) {
                QString pchDirectory = *plugInPchDirectoryIterator;
                pchFiles += QDir(pchDirectory).entryInfoList(QDir::Files);
            }

            QSet<QString> allPchFiles;
            for (  QFileInfoList::const_iterator pchIterator    = pchFiles.constBegin(),
                                                 pchEndIterator = pchFiles.constEnd()
                 ; pchIterator != pchEndIterator
                 ; ++pchIterator
                ) {
                allPchFiles << pchIterator->fileName();
            }

            QVERIFY(allPchFiles.contains("audiofile.pch"));

            QList<QString> plugInLibrarySearchPath = Ld::Environment::plugInLibrarySearchPaths();

            expectedDebugPlugIn   = buildRoot + "/plugins/audiofile/libraries/ineaudiofile/build/debug";
            expectedReleasePlugIn = buildRoot + "/plugins/audiofile/libraries/ineaudiofile/build/release";

            QVERIFY(   plugInLibrarySearchPath.contains(QFileInfo(expectedDebugPlugIn).canonicalFilePath())
                    || plugInLibrarySearchPath.contains(QFileInfo(expectedReleasePlugIn).canonicalFilePath())
            );

//            QVERIFY(false); // Add test for plugInFontSearchPaths()

        #else

            #error Unknown platform.

        #endif
    }
}


void TestEnvironment::testDevelopmentMainApplicationEnvironment() {
    // The test code below could be better but there's no good way to test this without making significant changes to
    // the overall project structure.  For now we test enough to know things will work.

    QString executableFilePath = QCoreApplication::applicationFilePath();

    Ld::Environment::configure(
        executableFilePath,
        Ld::Environment::Type::MAIN_APPLICATION_DEVELOPMENT,
        false
    );

    #if (defined(Q_OS_WIN))

        QString root = QFileInfo(QFileInfo(executableFilePath).absolutePath() + "/../../../").canonicalPath();

    #elif (defined(Q_OS_LINUX))

        QString root = QFileInfo(QFileInfo(executableFilePath).absolutePath() + "/../../../").canonicalPath();

    #elif (defined(Q_OS_DARWIN))

        QString root = QFileInfo(QFileInfo(executableFilePath).absolutePath() + "/../../../../../../").canonicalPath();

    #else

        #error Unexpected platform.

    #endif

    QVERIFY(Ld::Environment::executableType() == Ld::Environment::Type::MAIN_APPLICATION_DEVELOPMENT);
    QVERIFY(QFileInfo(root) == QFileInfo(Ld::Environment::applicationRoot()));
}


void TestEnvironment::testDeploymentMainApplicationEnvironment() {
    // Note that the deployed paths do not exist in the development environment so we need to avoid using the
    // QFileInfo::canonicalFilePath and QFileInfo::canonicalPath methods in this test.

    QString executableFilePath = QCoreApplication::applicationFilePath();

    // TODO: Add support for Ld::Environment::fontSearchPaths and plugInFontSearchPaths

    for (unsigned useHeaderFile=0 ; useHeaderFile<=1 ; ++useHeaderFile) {
        Ld::Environment::configure(
            executableFilePath,
            Ld::Environment::Type::MAIN_APPLICATION_DEPLOYMENT,
            bool(useHeaderFile)
        );

        QString executableDirectory = QFileInfo(executableFilePath).canonicalPath();

        #if (defined(Q_OS_WIN) || defined(Q_OS_LINUX))

            QString applicationRoot            = QFileInfo(executableDirectory + "/../").absoluteFilePath();
            QString supportExecutableDirectory = QFileInfo(applicationRoot + "/bin/").absoluteFilePath();
            QString compilerRoot               = applicationRoot;
            QString headerSearchPath           = QFileInfo(applicationRoot + "/include/").absoluteFilePath();
            QString pchSearchPath              = QFileInfo(applicationRoot + "/pch/").absoluteFilePath();
            QString librarySearchPath          = QFileInfo(applicationRoot + "/lib/").absoluteFilePath();
            QString plugInRootDirectory        = QFileInfo(applicationRoot + "/plugins/").absoluteFilePath();

        #elif (defined(Q_OS_DARWIN))

            QString applicationRoot            = QFileInfo(executableDirectory + "/../../").absoluteFilePath();
            QString resourcesDirectory         = QFileInfo(applicationRoot + "/Contents/Resources/").absoluteFilePath();
            QString supportExecutableDirectory = QFileInfo(resourcesDirectory + "/bin/").absoluteFilePath();
            QString compilerRoot               = resourcesDirectory;
            QString headerSearchPath           = QFileInfo(resourcesDirectory + "/include/").absoluteFilePath();
            QString pchSearchPath              = QFileInfo(resourcesDirectory + "/pch/").absoluteFilePath();
            QString librarySearchPath          = QFileInfo(resourcesDirectory + "/lib/").absoluteFilePath();
            QString plugInRootDirectory        = QFileInfo(applicationRoot + "/Contents/PlugIns/").absoluteFilePath();

        #else

            #error Unknown platform.

        #endif

        QVERIFY(Ld::Environment::useHeaderFiles() == bool(useHeaderFile));
        QVERIFY(Ld::Environment::usePchFiles() != bool(useHeaderFile));
        QVERIFY(Ld::Environment::executableType() == Ld::Environment::Type::MAIN_APPLICATION_DEPLOYMENT);

        QVERIFY(QFileInfo(Ld::Environment::applicationRoot()) == QFileInfo(applicationRoot));
        QVERIFY(QFileInfo(Ld::Environment::executableDirectory()) == QFileInfo(executableDirectory));
        QVERIFY(   QFileInfo(Ld::Environment::supportExecutableDirectory())
                == QFileInfo(supportExecutableDirectory)
               );

        if (Ld::Environment::useHeaderFiles()) {
            QVERIFY(Ld::Environment::compilerRoot().isEmpty());

            QVERIFY(Ld::Environment::headerSearchPaths().size() == 1);
            QVERIFY(Ld::Environment::headerSearchPaths().contains(headerSearchPath));

            QVERIFY(Ld::Environment::pchSearchPaths().isEmpty());

            QVERIFY(Ld::Environment::standardHeaderFiles().size() == 3);
            QVERIFY(Ld::Environment::standardHeaderFiles().contains("model_api.h"));
            QVERIFY(Ld::Environment::standardHeaderFiles().contains("m_model_base.h"));
            QVERIFY(Ld::Environment::standardHeaderFiles().contains("model_interface.h"));

            QVERIFY(Ld::Environment::standardPchFiles().isEmpty());
        } else {
            QVERIFY(QFileInfo(Ld::Environment::compilerRoot()) == QFileInfo(compilerRoot));

            QVERIFY(Ld::Environment::headerSearchPaths().isEmpty());

            QVERIFY(Ld::Environment::pchSearchPaths().size() == 1);
            QVERIFY(Ld::Environment::pchSearchPaths().contains(pchSearchPath));

            QVERIFY(Ld::Environment::standardHeaderFiles().isEmpty());

            QVERIFY(Ld::Environment::standardPchFiles().size() == 3);
            QVERIFY(Ld::Environment::standardPchFiles().contains("m_model_api.pch"));
            QVERIFY(Ld::Environment::standardPchFiles().contains("m_model_base.pch"));
            QVERIFY(Ld::Environment::standardPchFiles().contains("m_model_interface.pch"));
        }

        QVERIFY(Ld::Environment::librarySearchPaths().size() == 1);
        QVERIFY(Ld::Environment::librarySearchPaths().contains(librarySearchPath));

        #if (defined(Q_OS_WIN))

            QVERIFY(Ld::Environment::standardStaticLibraries().size() == 1);
            QString standardLibrary = QFileInfo(librarySearchPath + "/inem").absoluteFilePath();
            QVERIFY(Ld::Environment::standardStaticLibraries().contains(standardLibrary));

            QVERIFY(Ld::Environment::standardDynamicLibraries().isEmpty());

            QVERIFY(Ld::Environment::runTimeLibrarySearchPaths().isEmpty());

        #elif (defined(Q_OS_LINUX) || defined(Q_OS_DARWIN))

            QVERIFY(Ld::Environment::standardStaticLibraries().isEmpty());

            QVERIFY(Ld::Environment::standardDynamicLibraries().size() == 2);
            QVERIFY(Ld::Environment::standardDynamicLibraries().contains("inem"));
            QVERIFY(Ld::Environment::standardDynamicLibraries().contains("c++"));

            QVERIFY(Ld::Environment::runTimeLibrarySearchPaths().size() == 1);
            QVERIFY(Ld::Environment::runTimeLibrarySearchPaths().contains(librarySearchPath));

        #else

            #error Unknown platform.

        #endif

        QVERIFY(QFileInfo(Ld::Environment::plugInRootDirectory()) == QFileInfo(plugInRootDirectory));
    }
}

