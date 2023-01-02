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
* This header defines the \ref Ld::CodeGeneratorOutputType class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CODE_GENERATOR_OUTPUT_TYPE_H
#define LD_CODE_GENERATOR_OUTPUT_TYPE_H

#include <QString>
#include <QMap>
#include <QSet>

#include <util_hash_functions.h>

#include "ld_common.h"

namespace Ld {
    class CodeGeneratorOutputTypeContainer;

    /**
     * Pure virtual base class used to describe output supported by a \ref Ld::CodeGenerator.  Code generators should
     * create an overloaded instance of this class for each type of output that can be created by a code generator.
     */
    class LD_PUBLIC_API CodeGeneratorOutputType {
        public:
            /**
             * Value used to represent an invalid output type identifier.
             */
            static constexpr unsigned invalidIdentifier = static_cast<unsigned>(-1);

            /**
             * Enumeration of supported export modes.
             */
            enum class ExportMode {
                /**
                 * Indicates the default export mode should be used.  This value is only used when invoking a
                 * translation.
                 */
                DEFAULT,

                /**
                 * Indicates this type is not exportable or supports a non-exportable mode.
                 */
                NO_EXPORT,

                /**
                 * Indicates this type is exportable as a file.
                 */
                EXPORT_AS_FILE,

                /**
                 * Indicates this type is exportable as a directory containing multiple files.
                 */
                EXPORT_AS_DIRECTORY,

                /**
                 * Indicates this type is exportable as a map of byte arrays in memory.
                 */
                EXPORT_IN_MEMORY,

                /**
                 * Indicates this type exportable as a temporary construct.  The main exported product is maintained
                 * in memory and any side products are maintained in temporary files.
                 */
                EXPORT_AS_MIXED_TEMPORARY_OBJECT
            };

            /**
             * Type used to represent a collection of export modes.
             */
            typedef QSet<ExportMode> ExportModeSet;

            virtual ~CodeGeneratorOutputType();

            /**
             * Pure virtual method that returns the class name for this type of code generation output.
             *
             * \return Returns the class name for this code generator output type.
             */
            virtual QString name() const = 0;

            /**
             * Pure virtual method that returns an identifier for this output type.
             *
             * \return Returns a unique identifier for this output type.
             */
            virtual unsigned identifier() const = 0;

            /**
             * Pure virtual method that returns a short description for this type of code generation output.
             *
             * \return Returns a short description for this code generator output type.
             */
            virtual QString shortDescription() const = 0;

            /**
             * Pure virtual method that returns a description for this type of code generation output.
             *
             * \return Returns a detailed description for this code generation output.
             */
            virtual QString description() const = 0;

            /**
             * Pure virtual method that indicates if this is the default output type.  Note that there can only be one
             * default output type per code generator.
             *
             * \return Returns true if this is the default output type.  Returns false if this is not the default
             *         output type.
             */
            virtual bool defaultOutputType() const = 0;

            /**
             * Pure virtual method that determines if this code generator output will be user readable.
             *
             * \return Returns true if this generator output is user-readable.
             */
            virtual bool userReadable() const = 0;

            /**
             * Pure virtual method that determines if this code generator output can be dynamically loadable by the
             * application.
             *
             * \return Returns true if this generator output is application loadable.
             */
            virtual bool applicationLoadable() const = 0;

            /**
             * Pure virtual method that determines if this code generator output is exportable in source or binary
             * format as a single file.
             *
             * \return Returns true if this generator output is exportable in a source or binary format as a single
             *         file.
             */
            virtual ExportModeSet exportable() const = 0;

            /**
             * Pure virtual method that indicates the default export mode.
             *
             * \return Returns the default export mode.
             */
            virtual ExportMode defaultExportMode() const = 0;

            /**
             * Pure virtual method that determines if this code generator output requires an intermediate
             * representation.
             *
             * \return Returns true if this generator output requires an intermediate representation.
             */
            virtual bool requiresIntermediateRepresentation() const = 0;

            /**
             * Pure virtual method that returns a shared pointer to an output type that must be generated in tandem with
             * this output type.  An example sibling type might be the Python export module source that binds a model to
             * the Python interpreter.
             *
             * \return Returns a shared pointer to another code generator that creates export module source or header
             *         to be used with this output type.
             */
            virtual CodeGeneratorOutputTypeContainer& siblingOutputType() const = 0;

            /**
             * Method that can be used to determine if two output types are identical.  Comparison is made based on the
             * output type identifier.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the output types are identical.  Returns false if the output types are different.
             */
            bool operator==(const CodeGeneratorOutputType& other) const;

            /**
             * Method that can be used to determine if two output types are different.  Comparison is made based on the
             * output type identifier.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the output types are different.  Returns false if the output types are identical.
             */
            bool operator!=(const CodeGeneratorOutputType& other) const;

            /**
             * Overloaded cast.
             *
             * \return Returns the identifier for this output type.
             */
            operator unsigned() const;
    };

    /**
     * Hash function for the \ref Ld::CodeGeneratorOutputType::ExportMode enumeration.
     *
     * \param[in] value The value to be hashed.
     *
     * \param[in] seed  An optional seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    LD_PUBLIC_API Util::HashResult qHash(CodeGeneratorOutputType::ExportMode value, Util::HashSeed seed = 0);
};

#endif
