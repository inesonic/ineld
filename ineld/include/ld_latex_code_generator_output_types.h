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
* This header defines the \ref Ld::LaTeXOutputType class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_LATEX_CODE_GENERATOR_OUTPUT_TYPES_H
#define LD_LATEX_CODE_GENERATOR_OUTPUT_TYPES_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_code_generator_output_type_base.h"

namespace Ld {
    class CodeGeneratorOutputTypeContainer;

    /**
     * Class that indicates LaTeX output from a code generator.
     */
    class LD_PUBLIC_API LaTeXOutputType:public CodeGeneratorOutputTypeBase {
        Q_DECLARE_TR_FUNCTIONS(Ld::LaTeXOutputType)

        public:
            /**
             * Method that returns the class name for this type of code generation output.
             *
             * \return Returns the class name for this code generator output type.
             */
            QString name() const final;

            /**
             * Method that returns a short description for this type of code generation output.
             *
             * \return Returns a short description for this code generator output type.
             */
            QString shortDescription() const final;

            /**
             * Method that returns a description for this type of code generation output.
             *
             * \return Returns a detailed description for this code generation output.
             */
            QString description() const final;

            /**
             * Method that indicates if this is the default output type.  Note that there can only be one default output
             * type per code generator.
             *
             * \return Returns true if this is the default output type.  Returns false if this is not the default
             *         output type.
             */
            bool defaultOutputType() const final;

            /**
             * Method that determines if this code generator output will be user readable.
             *
             * \return Returns true if this generator output is user-readable.
             */
            bool userReadable() const final;

            /**
             * Method that determines if this code generator output can be dynamically loadable by the
             * application.
             *
             * \return Returns true if this generator output is application loadable.
             */
            bool applicationLoadable() const final;

            /**
             * Method that determines if this code generator output is exportable in source or binary format.
             *
             * \return Returns true if this generator output is exportable in a source or binary format.
             */
            ExportModeSet exportable() const final;

            /**
             * Pure virtual method that indicates the default export mode.
             *
             * \return Returns the default export mode.  This method return
             *         \ref Ld::CodeGeneratorOutputType::ExportMode::EXPORT_AS_DIRECTORY
             */
            ExportMode defaultExportMode() const final;

            /**
             * Method that determines if this code generator output requires an intermediate representation.
             *
             * \return Returns true if this generator output requires an intermediate representation.
             */
            bool requiresIntermediateRepresentation() const final;
    };
};

#endif
