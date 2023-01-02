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
* This header provides a single function that can be called to perform start-up configuration of the Ld library.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CONFIGURE_H
#define LD_CONFIGURE_H

#include <QCoreApplication>
#include <QList>
#include <QString>

#include <cstdint>

#include "ld_common.h"
#include "ld_data_type.h"
#include "ld_function_variant.h"

namespace Ud {
    class UsageData;
};

namespace Ld {
    class PlugInManager;
    class PlugInRegistrar;

    /**
     * Class that can be used to configure the ineld library.  This class contains a single static public method you
     * can call initialize the ineld library.
     *
     * Be sure to call this function prior to performing other operations on classes in the Ld namespace.
     *
     * This function performs the following tasks:
     *
     *     * Initializes the seed used by the \ref Ld::Handle class.
     *     * Registers all \ref Ld::Element::CreatorFunction instances used by the element factory to create elements.
     *     * Creates a default instance of each code generator supported by the core language.
     *     * Registers all \ref Ld::Element::Translator instances needed to support all elements supported by the core
     *       language.
     *
     * Note that we define the configuration mechanism as a class so that we can more easily handle localization.
     */
    class LD_PUBLIC_API Configure {
        Q_DECLARE_TR_FUNCTIONS(Ld::Configure)

        public:
            /**
             * Static configuration method.
             *
             * \param[in] uniqueSystemId A value that must be unique for this system.
             *
             * \param[in] usageData      The usage data instance.
             */
            static void configure(std::uint64_t uniqueSystemId, Ud::UsageData* usageData);

            /**
             * Method you can use to enable or disable debug output.
             *
             * \param[in] enableDebugOutput If true, debug output will be enabled.  If false, debug output will be
             *                              disabled.
             */
            static void setDebugOutputEnabled(bool enableDebugOutput = true);

            /**
             * Method you can use to disable or enable debug output.
             *
             * \param[in] disableDebugOutput If true, debug output will be disbled.  If false, debug output will be
             *                               enabled.
             */
            static void setDebugOutputDisabled(bool disableDebugOutput = true);

            /**
             * Method you can use to determine if debug output is enabled.
             *
             * \return Returns true if debug output is enabled.  Returns false if debug output is disabled.
             */
            static bool debugOutputEnabled();

            /**
             * Method you can use to determine if debug output is disabled.
             *
             * \return Returns true if debug output is disabled.  Returns false if debug output is enabled.
             */
            static bool debugOutputDisabled();

            /**
             * Static configuration method that can be used to update linker settings.
             *
             * \param[in] linkerPath       The path to the linker executable.
             *
             * \param[in] linkerExecutable The name of the linker executable to be used.
             */
            static void configureLinker(const QString& linkerPath, const QString& linkerExecutable);

            /**
             * Static configuration finalation method.  This method builds any internal databases using the supplied
             * configuration data.
             */
            static void configureFinal();

        private:
            /**
             * Method that configures formats and format aggregations.
             */
            static void configureFormats();

            /**
             * Method that configures document settings.
             */
            static void configureDocumentSettingTypes();

            /**
             * Method that defines the element creators.
             */
            static void configureElementCreators();

            /**
             * Method that sets up support for different data types.
             */
            static void configureDataTypes();

            /**
             * Method that configures the C++ code generator.
             *
             * \param[in] usageData The usage data instance.
             */
            static void configureCppCodeGenerator(Ud::UsageData* usageData);

            /**
             * Method that configures the HTML code generator.
             *
             * \param[in] usageData The usage data instance.
             */
            static void configureHtmlCodeGenerator(Ud::UsageData* usageData);

            /**
             * Method that configures the LaTeX code generator.
             *
             * \param[in] usageData The usage data instance.
             */
            static void configureLaTeXCodeGenerator(Ud::UsageData* usageData);

            /**
             * Method that configures functions.
             */
            static void configureFunctions();

            /**
             * Method that generates a list of PCH files given a list of search paths and base filenames.
             *
             * \param[in] pchSearchPaths A list of PCH file search paths.
             *
             * \param[in] pchFiles       A list of PCH base filenames to include.
             *
             * \return Returns a list of full paths to PCH files.
             */
            static QList<QString> generatePchFileList(
                const QList<QString>& pchSearchPaths,
                const QList<QString>& pchFiles
            );
    };
};

#endif
