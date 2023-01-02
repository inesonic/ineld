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
* This file implements tests of the Ld::PlugInManager and Ld::PlugInInformation classes.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QtTest/QtTest>

#include <ld_handle.h>
#include <ld_plug_in_information.h>
#include <ld_plug_in_manager.h>
#include <plug_in_data.h>

#include "test_plug_in_manager.h"

struct PlugInData;
class PlugInRegistrar;

/***********************************************************************************************************************
 * Ld::Registrar
 */

namespace Ld {
    class Registrar {
        public:
            Registrar();

            ~Registrar();

            void recordInitializeMethodCall(const char* plugInName);
    };
}
/***********************************************************************************************************************
 * PlugInManagerWrapper
 */

class PlugInManagerWrapper:public Ld::PlugInManager {
    public:
        PlugInManagerWrapper();

        ~PlugInManagerWrapper() override;

        void clearStatus();

        const QList<QString>& reportedAttemptedPlugIns() const;

        const QList<Ld::PlugInInformation>& reportedLoadedPlugIns() const;

        const QList<QString>& reportedFailedPlugIns() const;

    protected:
        void aboutToLoad(const QString& loadPath) final;

        void plugInLoaded(const QString& loadPath, Ld::PlugInInformation plugInInformation) final;

        void plugInFailedToLoad(const QString& loadPath) final;

    private:
        QList<QString>               plugInsAttempted;
        QList<Ld::PlugInInformation> plugInsReportedLoaded;
        QList<QString>               plugInsThatWouldNotLoad;
};


PlugInManagerWrapper::PlugInManagerWrapper() {}


PlugInManagerWrapper::~PlugInManagerWrapper() {}


void PlugInManagerWrapper::clearStatus() {
    plugInsAttempted.clear();
    plugInsReportedLoaded.clear();
    plugInsThatWouldNotLoad.clear();
}


const QList<QString>& PlugInManagerWrapper::reportedAttemptedPlugIns() const {
    return plugInsAttempted;
}


const QList<Ld::PlugInInformation>& PlugInManagerWrapper::reportedLoadedPlugIns() const {
    return plugInsReportedLoaded;
}


const QList<QString>& PlugInManagerWrapper::reportedFailedPlugIns() const {
    return plugInsThatWouldNotLoad;
}


void PlugInManagerWrapper::aboutToLoad(const QString& loadPath) {
    plugInsAttempted.append(loadPath);
}


void PlugInManagerWrapper::plugInLoaded(const QString&, Ld::PlugInInformation plugInInformation) {
    plugInsReportedLoaded.append(plugInInformation);
}


void PlugInManagerWrapper::plugInFailedToLoad(const QString& loadPath) {
    plugInsThatWouldNotLoad.append(loadPath);
}

/***********************************************************************************************************************
 * TestPlugInManager
 */

TestPlugInManager::TestPlugInManager() {}


TestPlugInManager::~TestPlugInManager() {}


void TestPlugInManager::initTestCase() {
    Ld::Handle::initialize(0x123456789ABCDEF0ULL);
}

void TestPlugInManager::testPlugInLoaderGoodCase() {
}


void TestPlugInManager::testPlugInLoaderBadCase() {
}
