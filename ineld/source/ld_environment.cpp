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
* This file implements the \ref Ld::Environment class.
***********************************************************************************************************************/

#include <QString>
#include <QList>
#include <QScopedPointer>

#include <cassert>

#include "ld_environment_private.h"
#include "development_main_application_environment_private.h"
#include "deployment_main_application_environment_private.h"
#include "development_test_environment_private.h"
#include "ld_environment.h"

namespace Ld {
    const char Environment::environmentVariableUseHeaderFiles[] = "INESONIC_USE_HEADER_FILES";
    const char Environment::environmentVariableApplicationRoot[] = "INESONIC_APPLICATION_ROOT";
    const char Environment::environmentVariableSupportExecutableDirectory[] = "INESONIC_SUPPORT_EXECUTABLE_DIRECTORY";
    const char Environment::environmentVariableCompilerRoot[] = "INESONIC_COMPILER_ROOT";
    const char Environment::environmentVariableGccPrefix[] = "INESONIC_GCC_PREFIX";
    const char Environment::environmentVariableCompilerResourceDirectory[] = "INESONIC_COMPILER_RESOURCE_DIRECTORY";
    const char Environment::environmentVariableHeaderSearchPaths[] = "INESONIC_HEADER_SEARCH_PATHS";
    const char Environment::environmentVariablePchSearchPaths[] = "INESONIC_PCH_SEARCH_PATHS";
    const char Environment::environmentVariableLibrarySearchPaths[] = "INESONIC_LIBRARY_SEARCH_PATHS";
    const char Environment::environmentVariableStandardHeaderFiles[] = "INESONIC_STANDARD_HEADER_FILES";
    const char Environment::environmentVariableStandardPchFiles[] = "INESONIC_STANDARD_PCH_FILES";
    const char Environment::environmentVariableStandardStaticLibraries[] = "INESONIC_STANDARD_STATIC_LIBRARIES";
    const char Environment::environmentVariableStandardDynamicLibraries[] = "INESONIC_STANDARD_DYNAMIC_LIBRARIES";
    const char Environment::environmentVariableRunTimeLibrarySearchPaths[] = "INESONIC_RUNTIME_LIBRARY_SEARCH_PATHS";
    const char Environment::environmentVariableAdditionalLibrarySearchPaths[] =
        "INESONIC_ADDITIONAL_LIBRARY_SEARCH_PATHS";
    const char Environment::environmentVariableFontSearchPaths[] = "INESONIC_FONT_SEARCH_PATHS";
    const char Environment::environmentVariableExamplesDirectory[] = "INESONIC_EXAMPLES_DIRECTORY";
    const char Environment::environmentVariablePlugInRootDirectory[] = "INESONIC_PLUG_IN_ROOT_DIRECTORY";
    const char Environment::environmentVariableUserPlugInRootDirectory[] = "INESONIC_USER_PLUG_IN_ROOT_DIRECTORY";
    const char Environment::environmentVariablePlugInDirectories[] = "INESONIC_PLUG_IN_DIRECTORIES";
    const char Environment::environmentVariableUserPlugInDirectories[] = "INESONIC_USER_PLUG_IN_DIRECTORIES";
    const char Environment::environmentVariablePlugInFiles[] = "INESONIC_PLUG_IN_FILES";
    const char Environment::environmentVariablePlugInHeaderSearchPaths[] = "INESONIC_PLUG_IN_HEADER_SEARCH_PATHS";
    const char Environment::environmentVariablePlugInPchSearchPaths[] = "INESONIC_PLUG_IN_PCH_SEARCH_PATHS";
    const char Environment::environmentVariablePlugInLibrarySearchPaths[] = "INESONIC_PLUG_IN_LIBRARY_SEARCH_PATHS";
    const char Environment::environmentVariablePlugInRunTimeLibrarySearchPaths[] =
        "INESONIC_PLUG_IN_RUN_TIME_LIBRARY_SEARCH_PATHS";
    const char Environment::environmentVariablePlugInFontSearchPaths[] = "INESONIC_PLUG_IN_FONT_SEARCH_PATHS";

    QScopedPointer<EnvironmentPrivate> Environment::impl;

    void Environment::configure(
            const QString&    executableFilePath,
            Environment::Type executableType,
            bool              useHeaderFiles
        ) {
        EnvironmentPrivate* env = nullptr;
        switch (executableType) {
            case Environment::Type::MAIN_APPLICATION_DEVELOPMENT: {
                env = new DevelopmentMainApplicationEnvironmentPrivate(executableFilePath, useHeaderFiles);
                break;
            }

            case Environment::Type::MAIN_APPLICATION_DEPLOYMENT: {
                env = new DeploymentMainApplicationEnvironmentPrivate(executableFilePath, useHeaderFiles);
                break;
            }

            case Environment::Type::TEST_DEVELOPMENT: {
                env = new DevelopmentTestEnvironmentPrivate(executableFilePath, useHeaderFiles);
                break;
            }

            default: {
                assert(false);
                env = nullptr;
            }
        }

        impl.reset(env);
    }


    bool Environment::useHeaderFiles() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        bool result;

        if (qEnvironmentVariableIsSet(environmentVariableUseHeaderFiles)) {
            QString forcedOption = qEnvironmentVariable(Environment::environmentVariableUseHeaderFiles).toLower();

            if (forcedOption == "true" && forcedOption == "1" && forcedOption == "yes") {
                result = false;
            } else if (forcedOption == "false" && forcedOption == "0" && forcedOption == "no") {
                result = true;
            } else {
                result = impl->useHeaderFiles();
            }
        } else {
            result = impl->useHeaderFiles();
        }

        return result;
    }


    bool Environment::usePchFiles() {
        return !useHeaderFiles();
    }


    Environment::Type Environment::executableType() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.
        return impl->executableType();
    }


    QString Environment::applicationRoot() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        QString result;

        if (qEnvironmentVariableIsSet(environmentVariableApplicationRoot)) {
            result = qEnvironmentVariable(environmentVariableApplicationRoot);
        } else {
            result = impl->applicationRoot();
        }

        return result;
    }


    QString Environment::executableDirectory() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.
        return impl->executableDirectory();
    }


    QString Environment::supportExecutableDirectory() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        QString result;

        if (qEnvironmentVariableIsSet(environmentVariableSupportExecutableDirectory)) {
            result = qEnvironmentVariable(environmentVariableSupportExecutableDirectory);
        } else {
            result = impl->supportExecutableDirectory();
        }

        return result;
    }


    QString Environment::compilerRoot() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        QString result;

        if (qEnvironmentVariableIsSet(environmentVariableCompilerRoot)) {
            result = qEnvironmentVariable(environmentVariableCompilerRoot);
        } else {
            if (useHeaderFiles()) {
                result = QString();
            } else {
                result = impl->compilerRoot();
            }
        }

        return result;
    }


    QString Environment::gccPrefix() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        QString result;

        if (qEnvironmentVariableIsSet(environmentVariableGccPrefix)) {
            result = qEnvironmentVariable(environmentVariableGccPrefix);
        } else {
            if (useHeaderFiles()) {
                result = QString();
            } else {
                result = impl->gccPrefix();
            }
        }

        return result;
    }


    QString Environment::compilerResourceDirectory() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        QString result;

        if (qEnvironmentVariableIsSet(environmentVariableCompilerResourceDirectory)) {
            result = qEnvironmentVariable(environmentVariableCompilerResourceDirectory);
        } else {
            if (useHeaderFiles()) {
                result = QString();
            } else {
                result = impl->compilerResourceDirectory();
            }
        }

        return result;
    }


    QList<QString> Environment::headerSearchPaths() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        QList<QString> result;

        if (qEnvironmentVariableIsSet(environmentVariableHeaderSearchPaths)) {
            result = splitEnvironmentVariable(environmentVariableHeaderSearchPaths);
        } else {
            if (useHeaderFiles()) {
                result = impl->headerSearchPaths();
            } else {
                result = QList<QString>();
            }
        }

        return result;
    }


    QList<QString> Environment::pchSearchPaths() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        QList<QString> result;

        if (qEnvironmentVariableIsSet(environmentVariablePchSearchPaths)) {
            result = splitEnvironmentVariable(environmentVariablePchSearchPaths);
        } else {
            if (useHeaderFiles()) {
                result = QList<QString>();
            } else {
                result = impl->pchSearchPaths();
            }
        }

        return result;
    }


    QList<QString> Environment::librarySearchPaths(const QString& overrideRuntime) {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        QList<QString> result;

        if (qEnvironmentVariableIsSet(environmentVariableLibrarySearchPaths)) {
            result = splitEnvironmentVariable(environmentVariableLibrarySearchPaths);
        } else {
            result = impl->librarySearchPaths(overrideRuntime);
        }

        if (qEnvironmentVariableIsSet(environmentVariableAdditionalLibrarySearchPaths)) {
            result += splitEnvironmentVariable(environmentVariableAdditionalLibrarySearchPaths);
        }

        return result;
    }


    QList<QString> Environment::standardHeaderFiles() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        QList<QString> result;

        if (qEnvironmentVariableIsSet(environmentVariableStandardHeaderFiles)) {
            result = splitEnvironmentVariable(environmentVariableStandardHeaderFiles);
        } else {
            if (useHeaderFiles()) {
                result = impl->standardHeaderFiles();
            } else {
                result = QList<QString>();
            }
        }

        return result;
    }


    QList<QString> Environment::standardPchFiles() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        QList<QString> result;

        if (qEnvironmentVariableIsSet(environmentVariableStandardPchFiles)) {
            result = splitEnvironmentVariable(environmentVariableStandardPchFiles);
        } else {
            if (useHeaderFiles()) {
                result = QList<QString>();
            } else {
                result = impl->standardPchFiles();
            }
        }

        return result;
    }


    QList<QString> Environment::standardStaticLibraries(const QString& overrideRuntime) {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        QList<QString> result;

        if (qEnvironmentVariableIsSet(environmentVariableStandardStaticLibraries)) {
            result = splitEnvironmentVariable(environmentVariableStandardStaticLibraries);
        } else {
            result = impl->standardStaticLibraries(overrideRuntime);
        }

        return result;
    }


    QList<QString> Environment::standardDynamicLibraries() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        QList<QString> result;

        if (qEnvironmentVariableIsSet(environmentVariableStandardDynamicLibraries)) {
            result = splitEnvironmentVariable(environmentVariableStandardDynamicLibraries);
        } else {
            result = impl->standardDynamicLibraries();
        }

        return result;
    }


    QList<QString> Environment::runTimeLibrarySearchPaths() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        QList<QString> result;

        if (qEnvironmentVariableIsSet(environmentVariableRunTimeLibrarySearchPaths)) {
            result = splitEnvironmentVariable(environmentVariableRunTimeLibrarySearchPaths);
        } else {
            result = impl->runTimeLibrarySearchPaths();
        }

        return result;
    }


    QList<QString> Environment::fontSearchPaths() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        QList<QString> result;

        if (qEnvironmentVariableIsSet(environmentVariableFontSearchPaths)) {
            result = splitEnvironmentVariable(environmentVariableFontSearchPaths);
        } else {
            result = impl->fontSearchPaths();
        }

        return result;
    }


    QString Environment::examplesDirectory() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        QString result;

        if (qEnvironmentVariableIsSet(environmentVariableExamplesDirectory)) {
            result = qEnvironmentVariable(environmentVariableExamplesDirectory);
        } else {
            result = impl->examplesDirectory();
        }

        return result;
    }


    QString Environment::plugInRootDirectory() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        QString result;

        if (qEnvironmentVariableIsSet(environmentVariablePlugInRootDirectory)) {
            result = qEnvironmentVariable(environmentVariablePlugInRootDirectory);
        } else {
            result = impl->plugInRootDirectory();
        }

        return result;
    }


    QString Environment::userPlugInRootDirectory() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        QString result;

        if (qEnvironmentVariableIsSet(environmentVariableUserPlugInRootDirectory)) {
            result = qEnvironmentVariable(environmentVariableUserPlugInRootDirectory);
        } else {
            result = impl->userPlugInRootDirectory();
        }

        return result;
    }


    QList<QString> Environment::plugInDirectories() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        QList<QString> result;

        if (qEnvironmentVariableIsSet(environmentVariablePlugInDirectories)) {
            result = splitEnvironmentVariable(environmentVariablePlugInDirectories);
        } else {
            result = impl->plugInDirectories();
        }

        return result;
    }


    QList<QString> Environment::userPlugInDirectories() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        QList<QString> result;

        if (qEnvironmentVariableIsSet(environmentVariableUserPlugInDirectories)) {
            result = splitEnvironmentVariable(environmentVariableUserPlugInDirectories);
        } else {
            result = impl->userPlugInDirectories();
        }

        return result;
    }


    QList<QString> Environment::plugInFiles() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        QList<QString> result;

        if (qEnvironmentVariableIsSet(environmentVariablePlugInFiles)) {
            result = splitEnvironmentVariable(environmentVariablePlugInFiles);
        } else {
            result = impl->plugInFiles();
        }

        return result;
    }


    QList<QString> Environment::plugInHeaderSearchPaths() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        QList<QString> result;

        if (qEnvironmentVariableIsSet(environmentVariablePlugInHeaderSearchPaths)) {
            result = splitEnvironmentVariable(environmentVariablePlugInHeaderSearchPaths);
        } else {
            result = impl->plugInHeaderSearchPaths();
        }

        return result;
    }


    QList<QString> Environment::plugInPchSearchPaths() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        QList<QString> result;

        if (qEnvironmentVariableIsSet(environmentVariablePlugInPchSearchPaths)) {
            result = splitEnvironmentVariable(environmentVariablePlugInPchSearchPaths);
        } else {
            result = impl->plugInPchSearchPaths();
        }

        return result;
    }


    QList<QString> Environment::plugInLibrarySearchPaths() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        QList<QString> result;

        if (qEnvironmentVariableIsSet(environmentVariablePlugInLibrarySearchPaths)) {
            result = splitEnvironmentVariable(environmentVariablePlugInLibrarySearchPaths);
        } else {
            result = impl->plugInLibrarySearchPaths();
        }

        return result;
    }


    QList<QString> Environment::plugInRunTimeLibrarySearchPaths() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        QList<QString> result;

        if (qEnvironmentVariableIsSet(environmentVariablePlugInRunTimeLibrarySearchPaths)) {
            result = splitEnvironmentVariable(environmentVariablePlugInRunTimeLibrarySearchPaths);
        } else {
            result = impl->plugInRunTimeLibrarySearchPaths();
        }

        return result;
    }


    QList<QString> Environment::plugInFontSearchPaths() {
        assert(!impl.isNull()); // If this assert fires, it's because the Environment::configure method was not called.

        QList<QString> result;

        if (qEnvironmentVariableIsSet(environmentVariablePlugInFontSearchPaths)) {
            result = splitEnvironmentVariable(environmentVariablePlugInFontSearchPaths);
        } else {
            result = impl->plugInFontSearchPaths();
        }

        return result;
    }


    QString Environment::targetTriple() {
        return impl->targetTriple();
    }


    QList<QString> Environment::splitEnvironmentVariable(const char* environmentVariable) {
        QString rawValue = qEnvironmentVariable(environmentVariable);
        QList<QString> result;

        #if (defined(Q_OS_WIN))

            result = rawValue.split(QChar(';'), Qt::SplitBehaviorFlags::SkipEmptyParts);

        #elif (defined(Q_OS_LINUX) || defined(Q_OS_DARWIN))

            result = rawValue.split(":", Qt::SplitBehaviorFlags::SkipEmptyParts);

        #else

            #error Unknown platform.

        #endif

        return result;
    }
}
