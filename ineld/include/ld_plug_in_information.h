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
* This header defines the \ref Ld::PlugInInformation class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_PLUG_IN_INFORMATION_H
#define LD_PLUG_IN_INFORMATION_H

#include <QString>
#include <QSharedPointer>
#include <QMap>
#include <QSet>

#include "ld_common.h"

struct PlugInData;

namespace Ld {
    class PlugInManager;

    /**
     * Trivial class that manages information associated with a plug-in.  The class uses a pimpl design pattern so it
     * can be copied and passed directly into and out of functions with minimal overhead.
     */
    class LD_PUBLIC_API PlugInInformation {
        friend class PlugInManager;

        private:
            /**
             * Private constructor.
             *
             * \param[in] plugInData   The plug-in data instance used to initialize this plug-in information.
             *
             * \param[in] isThirdParty If true, the plug-in is a third party plug-in.  If false, the plug-in is an
             *                         Inesonic plug-in.
             */
            PlugInInformation(const ::PlugInData* plugInData, bool isThirdParty);

        public:
            PlugInInformation();

            /**
             * Copy constructor.
             *
             * \param[in] other The instance to be copied.
             */
            PlugInInformation(const PlugInInformation& other);

            ~PlugInInformation();

            /**
             * Method you can use to determine if the supplied plug-in data was valid.
             *
             * \return Returns true if the plug-in data appeared to be valid.  Returns false if the plug-in data
             *         appeared to be invalid.
             */
            bool isValid() const;

            /**
             * Method you can use to determine if the supplied plug-in data was invalid.
             *
             * \return Returns true if the plug-in data appeared to be invalid.  Returns false if the plug-in data
             *         appeared to be valid.
             */
            bool isInvalid() const;

            /**
             * Method you can use to determine if this plug-in is an Inesonic plug-in.
             *
             * \return Returns true if the plug-in is an Inesonic plug-in.  Returns false if the plug-in is a third
             *         party plug-in.
             */
            bool isInesonicPlugIn() const;

            /**
             * Method you can use to determine if this plug-in is a third party plug-in.
             *
             * \return Returns true if the plug-in is a third party plug-in.  Returns false if the plug-in is an
             *         Inesonic plug-in.
             */
            bool isThirdPartyPlugIn() const;

            /**
             * Method you can use to obtain the name of this plug-in.
             *
             * \return Returns the name of the plug-in.
             */
            QString name() const;

            /**
             * Method you can use to obtain the author associated with this plug-in.
             *
             * \return Returns the author tied to the plug-in.
             */
            QString author() const;

            /**
             * Method you can use to determine if the standard author name should be used.
             *
             * \return Returns true if the standard author should be used for this plug-in.
             */
            bool useStandardAuthor() const;

            /**
             * Method you can use to obtain the company associated with this plug-in.
             *
             * \return Returns the name of the company tied to the plug-in.
             */
            QString company() const;

            /**
             * Method you can use to determine if the standard company name should be used.
             *
             * \return Returns true if the standard company name should be used for this plug-in.
             */
            bool useStandardCompany() const;

            /**
             * Method you can use to obtain the plug-in license text.
             *
             * \ref PlugInInformation::useStandardLicense()
             *
             * \return Returns the plug-in license text.  An empty string indicates no license or that the standard
             *         Inesonic, LLC. license should be applied.
             */
            QString license() const;

            /**
             * Method you can use to determine if the standard license should be applied to this plug-in.
             *
             * \return Returns true if the standard license should be applied to this license.  Returns false if a
             *         distinct license should be applied.
             */
            bool useStandardLicense() const;

            /**
             * Method you can use to obtain a brief description for this plug-in.
             *
             * \return Returns a brief description of the plug-in.
             */
            QString briefDescription() const;

            /**
             * Method you can use to obtain a detailed description for this plug-in.
             *
             * \return Returns a detailed description of the plug in.  An empty string will be returned if there is no
             *         detailed description available.
             */
            QString detailedDescription() const;

            /**
             * Method you can use to obtain the plug-in version information.
             *
             * \return Returns the plug-in version data.
             */
            QString version() const;

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            PlugInInformation& operator=(const PlugInInformation& other);

        private:
            /**
             * Class pimpl.
             */
            class Private;

            /**
             * Class pimpl.
             */
            QSharedPointer<Private> impl;
    };

    /**
     * Typedef used to maintain a map of plug-in data, by plug-in.
     */
    typedef QMap<QString, PlugInInformation> PlugInsByName;
};

#endif
