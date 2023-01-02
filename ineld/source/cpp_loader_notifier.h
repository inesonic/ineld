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
* This header defines the Ld::CppLoaderNotifier class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CPP_LOADER_NOTIFIER_H
#define LD_CPP_LOADER_NOTIFIER_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>

#include <cbe_loader_notifier.h>

#include <ld_common.h>

namespace Ld {
    class CppCodeGenerator;

    /**
     * Class used by the CppCodeGenerator to marshall loader status updates.
     */
    class LD_PUBLIC_API CppLoaderNotifier:public Cbe::LoaderNotifier {
        Q_DECLARE_TR_FUNCTIONS(CppLoaderNotifier)

        public:
            /**
             * Constructor
             *
             * \param[in] generator The code generator to receive loader error messages.
             */
            CppLoaderNotifier(Ld::CppCodeGenerator* generator);

            ~CppLoaderNotifier() override;

            /**
             * Method that is called whenever an attempt to load a library fails.  The default implementation simply
             * returns.
             *
             * \param[in] libraryFilename The name of the library that we failed to load.
             *
             * \param[in] errorReason     An error message indicating why the library could not be loaded.
             */
            void libraryLoadFailed(const QString& libraryFilename, const QString& errorReason) final;

            /**
             * Method that is called when a library could not be unloaded.  The default implementation simply returns.
             *
             * \param[in] libraryFilename The name of the library that we could not unload.
             */
            void libraryUnloadFailed(const QString& libraryFilename) final;

        private:
            /**
             * The code generator to receive loader error messages.
             */
            Ld::CppCodeGenerator* currentGenerator;
    };
};
#endif
