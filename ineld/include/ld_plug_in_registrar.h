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
* This header defines the \ref Ld::PlugInRegistrar class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_PLUG_IN_REGISTRAR_H
#define LD_PLUG_IN_REGISTRAR_H

#include <QString>
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_format.h"
#include "ld_document_setting.h"
#include "ld_element.h"
#include "ld_visual.h"
#include "ld_data_type.h"
#include "ld_cpp_library_information.h"

/**
 * Name of the initialization function called by the plug-in.
 */
#define PLUG_IN_INITIALZATION_FUNCTION_NAME ("__initialize")

/**
 * Name of the destruction function called by the plug-in.
 */
#define PLUG_IN_DESTRUCTOR_FUNCTION_NAME ("__destroy")


namespace Ld {
    class DataTypeTranslator;
    class DataTypeDecoder;
    class CodeGenerator;
    class FunctionTranslationEngine;
    class Fixer;
    class FunctionData;
    class FunctionDatabase;
    class VariableName;

    /**
     * Pure virtual plug-in registrar class
     */
    class LD_PUBLIC_API PlugInRegistrar {
        public:
            /**
             * Typedef defining the initialization function calling sequence.
             *
             * \param[in] registrar A pointer to an instance of a class derived from \ref Ld::PlugInRegistrar.
             */
            typedef void (*InitializationFunction)(PlugInRegistrar* registrar);

            /**
             * Typedef defining the unload function calling sequence.  This function will not be called if the plug
             * in was not loaded successfully.
             */
            typedef void (*DestructionFunction)();

            virtual ~PlugInRegistrar() = default;

            /**
             * Method you can call to indicate that we're about to configure a new plug-in.
             *
             * \param[in] plugInName The name of the plug-in we're about to load.
             */
            virtual void startConfiguration(const QString& plugInName) = 0;

            /**
             * Method you can call to indicate that we've finished configuring a plug-in.
             *
             * \param[in] plugInName The name of the plug-in we've finished configuring.
             */
            virtual void endConfiguration(const QString& plugInName) = 0;

            /**
             * Method you can call to determine if the configuration was successful.
             *
             * \return Returns true if the configuration was successful.  Returns false if the configuration failed.
             */
            virtual bool successful() const = 0;

            /**
             * Method you can call to register a new format instance.
             *
             * \param[in] formatName      The name of the format class.
             *
             * \param[in] creatorFunction The format creator function.
             */
            virtual void registerFormat(const QString& formatName, Format::CreatorFunction creatorFunction) = 0;

            /**
             * Method you can call to register a new document setting type.
             *
             * \param[in] documentSettingName The name used to reference this document setting.
             *
             * \param[in] creatorFunction     Pointer to the function that should be called to create a default
             *                                instance of the correct object type.
             */
            virtual void registerDocumentSetting(
                const QString&                   documentSettingName,
                DocumentSetting::CreatorFunction creatorFunction
            ) = 0;

            /**
             * Method you can call to register a new element type.
             *
             * \param[in] elementName     The name used to reference this element type.
             *
             * \param[in] creatorFunction Pointer to the function that should be called to create a default instance of
             *                            the correct object type.
             */
            virtual void registerElement(const QString& elementName, Element::CreatorFunction creatorFunction) = 0;

            /**
             * Method you can use to register a new visual.
             *
             * \param[in] elementName       The name used to reference this visual type.
             *
             * \param[in] creatorFunction   Pointer to the function that should be called to create the visual.
             *
             * \param[in] overwriteExisting If true, any existing creatir method will be overwritten with a new entry.
             *                              If false, this method will fail (return false) and the existing entry will
             *                              not be overwritten.
             */
            virtual void registerVisual(
                const QString&          elementName,
                Visual::CreatorFunction creatorFunction,
                bool                    overwriteExisting = false
            ) = 0;

            /**
             * Method you can call to register a new data type.
             *
             * \param[in] valueType             The \ref Ld::DataType::ValueType this data type represents.
             *
             * \param[in] description           A description of this data type.
             *
             * \param[in] typeSymbolElementName The name of the symbol used to define this data-type.  An empty string
             *                                  can be provided if there is no element representing this data type.
             *
             * \param[in] properties            Properties to apply to the data type.
             */
            virtual void registerDataType(
                DataType::ValueType valueType,
                const QString&      description,
                const QString&      typeSymbolElementName = QString(),
                DataType::Property  properties = DataType::defaultProperties
            ) = 0;

            /**
             * Method you can use to register a new data type translator to this data type.
             *
             * \param[in] generatorName The name of the generator using this translator.
             *
             * \param[in] valueType     The value type this translator works with.
             *
             * \param[in] newTranslator The new data-type translator.  The translator is expected to be defined as a
             *                          static instance variable and will *not* be deleted.
             */
            virtual void registerDataTypeTranslator(
                const QString&            generatorName,
                DataType::ValueType       valueType,
                const DataTypeTranslator* newTranslator
            ) = 0;

            /**
             * Method you can use to register a data type decoder to a data type.  Note that there can be only one
             * decoder tied to each data-type.
             *
             * \param[in] valueType  The value type this translator works with.
             *
             * \param[in] newDecoder The new data-type translator.  Data type decoders should be defined consistently
             *                       as static instance variables or on the heap.  If defined on the heap, be sure to
             *                       call the \ref Ld::DataType::destroyDecoders method when the program terminates to
             *                       destroy the decoders.
             */
            virtual void registerDataTypeDecoder(DataType::ValueType valueType, const DataTypeDecoder* newDecoder) = 0;

            /**
             * Method you can use to register a new code generator.
             *
             * \param[in] newCodeGenerator The code generator to be added to the system.
             */
            virtual void registerCodeGenerator(QSharedPointer<CodeGenerator> newCodeGenerator) = 0;

            /**
             * Convenience method you can use to register a new code generator.
             *
             * \param[in] newCodeGenerator The code generator to be added to the system.  The system will take
             *                             ownership of the code generator.
             */
            virtual void registerCodeGenerator(CodeGenerator* newCodeGenerator) = 0;

            /**
             * Method you can use to obtain an existing code generator, by name.
             *
             * \param[in] codeGeneratorName The name of the desired code generator.
             *
             * \return Returns a shared pointer to the desired code generator.  A null pointer is returned if the
             *         requested code generator does not already exist.
             */
            virtual QSharedPointer<CodeGenerator> codeGenerator(const QString& codeGeneratorName) = 0;

            /**
             * Method you can use to register a default function translator for a translation engine.
             *
             * \param[in] generatorName     The name of the generator.
             *
             * \param[in] defaultTranslator A pointer to the default translator.  This translator is expected to be
             *                              statically defined and will not be deleted.
             */
            virtual void registerDefaultTranslator(
                const QString&                   generatorName,
                const FunctionTranslationEngine* defaultTranslator
            ) = 0;

            /**
             * Method you can use to register a new C++ declaration payload with the system.
             *
             * \param[in] library    The library tied to this payload.
             *
             * \param[in] payload    The payload.  The payload is expected to remain resident during the entire
             *                       lifetime of the application.
             *
             * \param[in] length     The length of the payload, in bytes.
             *
             * \param[in] compressed If true, the payload is zlib compressed.  if false, the payload is not compressed.
             */
            virtual void registerCppDeclarationPayload(
                const QString&       library,
                const unsigned char* payload,
                unsigned long        length,
                bool                 compressed = false
            ) = 0;

            /**
             * Method you can use to register a dependency between libraries.
             *
             * \param[in] libraryName  The name of the library we have dependencies to.  The name should exclude the
             *                         path, any prepended "lib" and the extension.
             *
             * \param[in] dependencies A list of dependent libraries.
             */
            virtual void registerCppLibraryDependencies(
                const QString&                      libraryName,
                const QList<CppLibraryInformation>& dependencies
            ) = 0;

            /**
             * Method you can call to register a new persistent function.
             *
             * \param[in] functionData The new function data instance.
             */
            virtual void registerFunction(const FunctionData& functionData) = 0;

            /**
             * Method you can call to obtain a persistent function by internal name.  You can use this method to add
             * new variants to a persistent function.
             *
             * \param[in] internalName The internal name for the function.
             *
             * \return Returns the persistent function data instance associated with the given internal name.
             */
            virtual FunctionData& function(const QString& internalName) = 0;

            /**
             * Method you can call to obtain a persistent function by internal name.  You can use this method to add
             * new variants to a persistent function.
             *
             * \param[in] variableName The variable name for the function.
             *
             * \return Returns the persistent function data instance associated with the given internal name.
             */
            virtual FunctionData& function(const VariableName& variableName) = 0;

    };
};

#endif
