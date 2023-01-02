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
* This header defines the \ref Ld::CodeGeneratorOutputTypeContainer class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CODE_GENERATOR_OUTPUT_TYPE_CONTAINER_H
#define LD_CODE_GENERATOR_OUTPUT_TYPE_CONTAINER_H

#include <QString>
#include <QSharedPointer>
#include <QList>

#include "ld_common.h"
#include "ld_code_generator_output_type.h"

namespace Ld {
    /**
     * Class that can be used to efficiently manage and share instances of \ref CodeGeneratorOutputType.  In most cases
     * you will want to use this clas over using \ref CodeGeneratorOutputType directly.
     */
    class LD_PUBLIC_API CodeGeneratorOutputTypeContainer:public CodeGeneratorOutputType {
        public:
            CodeGeneratorOutputTypeContainer();

            /**
             * Constructor
             *
             * \param[in] outputType Pointer to an underlying \ref CodeGeneratorOutputType to hold.
             */
            CodeGeneratorOutputTypeContainer(CodeGeneratorOutputType* outputType);

            /**
             * Constructor
             *
             * \param[in] outputType Shared pointer to the underlying \ref CodeGeneratorOutputType to hold.
             */
            CodeGeneratorOutputTypeContainer(QSharedPointer<CodeGeneratorOutputType> outputType);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            CodeGeneratorOutputTypeContainer(const CodeGeneratorOutputTypeContainer& other);

            ~CodeGeneratorOutputTypeContainer() override;

            /**
             * Method you can use to determine if this container holds a valid output type.
             *
             * \return Returns true if this container holds a valid output type.  Returns false if this container does
             *         not hold a valid output type.
             */
            bool isDefined() const;

            /**
             * Method you can use to determine if this container does not hold a valid output type.
             *
             * \return Returns true if this container does not hold a valid output type.  Returns false if this
             *         container does hold a valid output type.
             */
            bool isUndefined() const;

            /**
             * Method that returns the class name for this type of code generation output.
             *
             * \return Returns the class name for this code generator output type.
             */
            QString name() const final;

            /**
             * Method that returns an identifier for this output type.
             *
             * \return Returns a unique identifier for this output type.
             */
            unsigned identifier() const final;

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
             * Method that determines if this code generator output can be dynamically loadable by the application.
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
             * Method that indicates the default export mode.
             *
             * \return Returns the default export mode.  The value
             *         \ref Ld::CodeGeneratorOutputType::ExportMode::DEFAULT is returned if no export mode is
             *         supported.
             */
            ExportMode defaultExportMode() const final;

            /**
             * Method that determines if this code generator output requires an intermediate
             * representation.
             *
             * \return Returns true if this generator output requires an intermediate representation.
             */
            bool requiresIntermediateRepresentation() const final;

            /**
             * Method that returns a shared pointer to an output type that must be generated in tandem with this output
             * type.  An example sibling type might be the Python export module source that binds a model to the Python
             * interpreter.
             *
             * \return Returns a shared pointer to another code generator that creates export module source or header
             *         to be used with this output type.  The default implementation returns a null pointer.
             */
            CodeGeneratorOutputTypeContainer& siblingOutputType() const final;

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this object.
             */
            CodeGeneratorOutputTypeContainer& operator=(const CodeGeneratorOutputTypeContainer& other);

        private:
            /**
             * Shared pointer to the underlying object instance.
             */
            QSharedPointer<CodeGeneratorOutputType> instance;
    };
};

#endif
