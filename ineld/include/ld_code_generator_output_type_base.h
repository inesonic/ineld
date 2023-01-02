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
* This header defines the \ref Ld::CodeGeneratorOutputTypeBase class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CODE_GENERATOR_OUTPUT_TYPE_BASE_H
#define LD_CODE_GENERATOR_OUTPUT_TYPE_BASE_H

#include <QString>
#include <QMap>

#include "ld_common.h"
#include "ld_code_generator_output_type.h"

namespace Ld {
    class CodeGeneratorOutputTypeContainer;

    /**
     * Pure virtual base class used to describe output supported by a \ref Ld::CodeGenerator.  Code generators should
     * create an overloaded instance of this class for each type of output that can be created by a code generator.
     *
     * You should use this class to define different output types for code generators.
     */
    class LD_PUBLIC_API CodeGeneratorOutputTypeBase:public CodeGeneratorOutputType {
        public:
            CodeGeneratorOutputTypeBase();

            ~CodeGeneratorOutputTypeBase() override;

            /**
             * Method that returns an identifier for this output type.
             *
             * \return Returns a unique identifier for this output type.
             */
            unsigned identifier() const final;

            /**
             * Virtual method that returns a shared pointer to an output type that must be generated in tandem with
             * this output type.  An example sibling type might be the Python export module source that binds a model to
             * the Python interpreter.
             *
             * \return Returns a shared pointer to another code generator that creates export module source or header
             *         to be used with this output type.  The default implementation returns an undefined container.
             */
            virtual CodeGeneratorOutputTypeContainer& siblingOutputType() const override;

        private:
            /**
             * Output type identifier.
             */
            mutable unsigned currentIdentifier;

            /**
             * Map of identifiers for all the output types.
             */
            static QMap<QString, unsigned> identifiers;
    };
};

#endif
