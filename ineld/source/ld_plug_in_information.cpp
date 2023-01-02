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
* This file implements the \ref Ld::PlugInInformation class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>

#include "plug_in_data.h"
#include "ld_plug_in_information.h"

/***********************************************************************************************************************
 * Ld::PlugInInformation::Private
 */

namespace Ld {
    class PlugInInformation::Private {
        public:
            Private(const ::PlugInData* plugInData, bool isThirdParty);

            ~Private();

            bool isValid() const;

            bool isThirdPartyPlugIn() const;

            QString name() const;

            QString author() const;

            bool useStandardAuthor() const;

            QString company() const;

            bool useStandardCompany() const;

            QString license() const;

            bool useStandardLicense() const;

            QString briefDescription() const;

            QString detailedDescription() const;

            QString version() const;

        private:
            bool    currentIsThirdParty;
            QString currentName;
            QString currentAuthor;
            QString currentCompany;
            QString currentLicense;
            QString currentBriefDescription;
            QString currentDetailedDescription;
            QString currentVersion;

            bool dataValid;
            bool usingStandardAuthor;
            bool usingStandardCompany;
            bool usingStandardLicense;
    };


    PlugInInformation::Private::Private(const ::PlugInData* plugInData, bool isThirdParty) {
        currentIsThirdParty = isThirdParty;

        if (plugInData->name != nullptr) {
            currentName = QString::fromLocal8Bit(plugInData->name);
            dataValid   = !currentName.isEmpty();
        } else {
            dataValid = false;
            currentName = QString();
        }

        if (plugInData->author != nullptr) {
            currentAuthor       = QString::fromLocal8Bit(plugInData->author);
            usingStandardAuthor = false;

            if (currentAuthor.isEmpty()) {
                dataValid = false;
            } else {
                if (plugInData->company != nullptr) {
                    currentCompany = QString::fromLocal8Bit(plugInData->company);
                    if (currentCompany.isEmpty()) {
                        dataValid = false;
                    }
                } else {
                    dataValid = false;
                }
            }
        } else {
            currentAuthor        = QString();
            currentCompany       = QString();
            usingStandardAuthor  = true;
            usingStandardCompany = true;
        }

        if (plugInData->license != nullptr) {
            currentLicense = QString::fromLocal8Bit(plugInData->license);
            usingStandardLicense = false;

            if (currentLicense.isEmpty()) {
                dataValid = false;
            }
        } else {
            currentLicense = QString();
            usingStandardLicense = true;
        }

        if (plugInData->briefDescription != nullptr) {
            currentBriefDescription = QString::fromLocal8Bit(plugInData->briefDescription);

            if (currentBriefDescription.isEmpty()) {
                dataValid = false;
            }
        } else {
            currentBriefDescription = QString();
        }

        if (plugInData->detailedDescription != nullptr) {
            currentDetailedDescription = QString::fromLocal8Bit(plugInData->detailedDescription);
        } else {
            currentDetailedDescription = QString();
        }

        if (plugInData->version != nullptr) {
            currentVersion = QString::fromLocal8Bit(plugInData->version);

            if (currentVersion.isEmpty()) {
                dataValid = false;
            }
        } else {
            currentVersion = QString();
            dataValid      = false;
        }
    }


    PlugInInformation::Private::~Private() {}


    bool PlugInInformation::Private::isValid() const {
        return dataValid;
    }


    bool PlugInInformation::Private::isThirdPartyPlugIn() const {
        return currentIsThirdParty;
    }


    QString PlugInInformation::Private::name() const {
        return currentName;
    }


    QString PlugInInformation::Private::author() const {
        return currentAuthor;
    }


    bool PlugInInformation::Private::useStandardAuthor() const {
        return usingStandardAuthor;
    }


    QString PlugInInformation::Private::company() const {
        return currentCompany;
    }


    bool PlugInInformation::Private::useStandardCompany() const {
        return usingStandardCompany;
    }


    QString PlugInInformation::Private::license() const {
        return currentLicense;
    }


    bool PlugInInformation::Private::useStandardLicense() const {
        return usingStandardLicense;
    }


    QString PlugInInformation::Private::briefDescription() const {
        return currentBriefDescription;
    }


    QString PlugInInformation::Private::detailedDescription() const {
        return currentDetailedDescription;
    }


    QString PlugInInformation::Private::version() const {
        return currentVersion;
    }
}

/***********************************************************************************************************************
 * Ld::PlugInInformation
 */

namespace Ld {
    PlugInInformation::PlugInInformation(
            const PlugInData* plugInData,
            bool              isThirdParty
        ):impl(
            new PlugInInformation::Private(plugInData, isThirdParty)
        ) {}


    PlugInInformation::PlugInInformation() {}


    PlugInInformation::PlugInInformation(const PlugInInformation& other) {
        impl = other.impl;
    }


    PlugInInformation::~PlugInInformation() {}


    bool PlugInInformation::isValid() const {
        return impl->isValid();
    }


    bool PlugInInformation::isInvalid() const {
        return !isValid();
    }


    bool PlugInInformation::isInesonicPlugIn() const {
        return !isThirdPartyPlugIn();
    }


    bool PlugInInformation::isThirdPartyPlugIn() const {
        return impl->isThirdPartyPlugIn();
    }


    QString PlugInInformation::name() const {
        return impl->name();
    }


    QString PlugInInformation::author() const {
        return impl->author();
    }


    bool PlugInInformation::useStandardAuthor() const {
        return impl->useStandardAuthor();
    }


    QString PlugInInformation::company() const {
        return impl->company();
    }


    bool PlugInInformation::useStandardCompany() const {
        return impl->useStandardCompany();
    }


    QString PlugInInformation::license() const {
        return impl->license();
    }


    bool PlugInInformation::useStandardLicense() const {
        return impl->useStandardLicense();
    }


    QString PlugInInformation::briefDescription() const {
        return impl->briefDescription();
    }


    QString PlugInInformation::detailedDescription() const {
        return impl->detailedDescription();
    }


    QString PlugInInformation::version() const {
        return impl->version();
    }


    PlugInInformation& PlugInInformation::operator=(const PlugInInformation& other) {
        impl = other.impl;
        return *this;
    }
}
