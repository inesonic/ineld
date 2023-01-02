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
* This header defines the \ref Ld::PlugInManager class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_PLUG_IN_MANAGER_H
#define LD_PLUG_IN_MANAGER_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QMap>
#include <QList>

#include <model_api_types.h>

#include "plug_in_data.h"
#include "user_function_definition.h"

#include "ld_common.h"
#include "ld_plug_in_information.h"

class QLibrary;

struct PlugInData;

namespace Ld {
    class PlugInRegistrar;

    /**
     * Class that loads and manages language plug-in.
     */
    class LD_PUBLIC_API PlugInManager {
        Q_DECLARE_TR_FUNCTIONS(Ld::PlugInManager)

        friend class Element;
        friend class Format;

        public:
            /**
             * Constructor
             */
            PlugInManager();

            virtual ~PlugInManager();

            /**
             * Method you can call to locate and load plug-ins.
             *
             * \param[in] plugInFiles A list of plug-in files to be dynamically loaded.
             *
             * \param[in] registrar   Pointer to the class to be used by Inesonic proprietary plug-ins to register
             *                        themselves with the system.  The class is expected to exist during the entire
             *                        period that plug-ins are being loaded.
             *
             * \return Returns true on success, returns false on error.
             */
            bool loadPlugIns(const QList<QString>& plugInFiles, PlugInRegistrar* registrar);

            /**
             * Method you can use to obtain information on every loaded plug-in, by plug-in name.
             */
            const PlugInsByName& plugInsByName() const;

        protected:
            /**
             * Method you can overload to receiving notification that a plug-in is about to be loaded.  The default
             * implementation simply returns.
             *
             * \param[in] loadPath The path used to load the library.
             */
            virtual void aboutToLoad(const QString& loadPath);

            /**
             * Method you can overload to receiving notification that a plug-in was successfully loaded.  The default
             * implementation simply returns.
             *
             * \param[in] loadPath          The path used to load the library.
             *
             * \param[in] plugInInformation The information associated with the plug-in.
             */
            virtual void plugInLoaded(const QString& loadPath, PlugInInformation plugInInformation);

            /**
             * Method you can overload to receive notification that a plug-in was not successfully loaded.  The default
             * implementation dumps an error message to stderr.
             *
             * \param[in] loadPath The path that was used to load the library.
             */
            virtual void plugInFailedToLoad(const QString& loadPath);

        private:
            /**
             * Method that can be called to attempt to unload all the loaded libraries.
             */
            void unloadAll();

            /**
             * Function that registers all functions defined using the \ref PlugInData structure.
             *
             * \param[in] numberFunctionDefinitions The number of supplied function definitions.
             *
             * \param[in] userFunctionDefinitions   Pointer to a list of user function definitions.
             *
             * \return Returns true on success, returns false on error.
             */
            bool registerFunctions(
                unsigned                        numberFunctionDefinitions,
                const ::UserFunctionDefinition* userFunctionDefinitions
            );

            /**
             * Method that builds function declarations for a given library.
             *
             * \param[in] plugInData The plug-in data to be processed.
             */
            bool buildFunctionDeclarations(const ::PlugInData* plugInData);

            /**
             * Method that builds a user visible inem type from a value type.
             *
             * \param[in] valueType The value type to be built.
             *
             * \param[in] asParameter If true, then some value types will be defined as const ...& rather than as the
             *                        basic type.
             *
             * \return Returns the type as a string.
             */
            static QString userTypeFromValue(Model::ValueType valueType, bool asParameter = true);

            /**
             * Map holding information about the loaded plug-ins.
             */
            PlugInsByName loadedPlugInsByName;

            /**
             * List used to track all the loaded libraries.
             */
            QList<QLibrary*> loadedLibraryInstances;

            /**
             * List of customer library declaration payloads.
             */
            QList<QByteArray> customerDeclarationPayloads;
    };
};

#endif
