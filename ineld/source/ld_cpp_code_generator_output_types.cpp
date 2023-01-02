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
* This file implements the \ref Ld::CppCodeGeneratorOutputType class and other closely related derived classes.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>

#include "ld_code_generator_output_type.h"
#include "ld_cpp_code_generator_output_types.h"

/***********************************************************************************************************************
 * Ld::CppLoadableModuleOutputType
 */

namespace Ld {
    QString CppLoadableModuleOutputType::name() const {
        return QString("CppLoadableModuleOutputType");
    }


    QString CppLoadableModuleOutputType::shortDescription() const {
        return tr("Application loadable module.");
    }


    QString CppLoadableModuleOutputType::description() const {
        return tr("A dynamic library that can be loaded and executed by the application.");
    }


    bool CppLoadableModuleOutputType::defaultOutputType() const {
        return true;
    }


    bool CppLoadableModuleOutputType::userReadable() const {
        return false;
    }


    bool CppLoadableModuleOutputType::applicationLoadable() const {
        return true;
    }


    CppLoadableModuleOutputType::ExportModeSet CppLoadableModuleOutputType::exportable() const {
        return ExportModeSet() << ExportMode::EXPORT_AS_FILE << ExportMode::NO_EXPORT;
    }


    CppLoadableModuleOutputType::ExportMode CppLoadableModuleOutputType::defaultExportMode() const {
        return ExportMode::NO_EXPORT;
    }


    bool CppLoadableModuleOutputType::requiresIntermediateRepresentation() const {
        return true;
    }
}

/***********************************************************************************************************************
 * Ld::CppObjectFileOutputType
 */

namespace Ld {
    QString CppObjectFileOutputType::name() const {
        return QString("CppObjectFileOutputType");
    }


    QString CppObjectFileOutputType::shortDescription() const {
        return tr("Object file.");
    }


    QString CppObjectFileOutputType::description() const {
        return tr("A binary object file that can be linked against other software.");
    }


    bool CppObjectFileOutputType::defaultOutputType() const {
        return false;
    }


    bool CppObjectFileOutputType::userReadable() const {
        return false;
    }


    bool CppObjectFileOutputType::applicationLoadable() const {
        return false;
    }


    CppObjectFileOutputType::ExportModeSet CppObjectFileOutputType::exportable() const {
        return ExportModeSet() << ExportMode::EXPORT_AS_FILE;
    }


    CppObjectFileOutputType::ExportMode CppObjectFileOutputType::defaultExportMode() const {
        return ExportMode::EXPORT_AS_FILE;
    }


    bool CppObjectFileOutputType::requiresIntermediateRepresentation() const {
        return true;
    }
}
