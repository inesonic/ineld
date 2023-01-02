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
* This header defines the \ref Ld::CppCodeGenerator class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CPP_CODE_GENERATOR_H
#define LD_CPP_CODE_GENERATOR_H

#include <QString>
#include <QSharedPointer>
#include <QScopedPointer>
#include <QByteArray>
#include <QMap>
#include <QList>

#include "ld_common.h"
#include "ld_code_generator_output_type_container.h"
#include "ld_code_generator.h"

namespace Cbe {
    class CppCompiler;
    class DynamicLibraryLinker;
    class DynamicLibraryLoader;
}

namespace Ld {
    class RootElement;
    class Element;
    class CodeGeneratorVisual;
    class CppCodeGenerationEngine;

    /**
     * Class that specializes the \ref Ld::CodeGenerator class to translate the user program in C++ code and then to a
     * final binary representation.
     */
    class LD_PUBLIC_API CppCodeGenerator:public CodeGenerator {
        friend class CodeGeneratorVisual;
        friend class CppCodeGenerationEngine;

        public:
            /**
             * String holding this code generator's name.
             */
            static const char codeGeneratorName[];

            /**
             * Constructor
             *
             * \param[in] visual The visual that can be used to obtain feedback from the code generator.
             */
            CppCodeGenerator(CodeGeneratorVisual* visual = nullptr);

            ~CppCodeGenerator() override;

            /**
             * Method you should overload to specify the name of the code generator.
             *
             * \return Returns the name of the code generator.
             */
            QString typeName() const final;

            /**
             * Method that returns a map listing the supported output types for this class.
             *
             * \return Returns a map of the supported output types.
             */
            QList<CodeGeneratorOutputTypeContainer> supportedOutputTypes() const final;

            /**
             * Method you can use to enable or disable debug output.
             *
             * \param[in] enableDebugOutput If true, debug output will be enabled.  If false, debug output will be
             *                              disabled.
             */
            void setDebugOutputEnabled(bool enableDebugOutput = true);

            /**
             * Method you can use to disable or enable debug output.
             *
             * \param[in] disableDebugOutput If true, debug output will be disbled.  If false, debug output will be
             *                               enabled.
             */
            void setDebugOutputDisabled(bool disableDebugOutput = true);

            /**
             * Method you can use to determine if debug output is enabled.
             *
             * \return Returns true if debug output is enabled.  Returns false if debug output is disabled.
             */
            bool debugOutputEnabled() const;

            /**
             * Method you can use to determine if debug output is disabled.
             *
             * \return Returns true if debug output is disabled.  Returns false if debug output is enabled.
             */
            bool debugOutputDisabled() const;

            /**
             * Method you can use to set the executable directory.  Call to this function will be ignored if the linker
             * is an internal function.  If the linker is external, you must call this function prior to invoking the
             * linker.
             *
             * \param[in] newExecutableDirectory The directory where the linker executable is expected to be found.
             */
            void setExecutableDirectory(const QString& newExecutableDirectory);

            /**
             * Method you can use to determine the current executable directory.
             *
             * \return Returns the path to the executable directory.  An empty string will be returned if the linker is
             *         an internal function.
             */
            QString executableDirectory() const;

            /**
             * Method you can use to set the executable name.  Calls to this function will be ignored if the linker is
             * an internal function.  Note that a default external linker will be used if this method is not called.
             *
             * \param[in] linkerExecutable The name of the linker executable to be used.
             */
            void setLinkerExecutable(const QString& linkerExecutable);

            /**
             * Method you can use to determine the name of the linker executable.  An empty string will be returned if
             * an internal linker is being used.
             *
             * \return Returns the name of the external linker executable being used.
             */
            QString linkerExecutable() const;

            /**
             * Method you can use to specify the location and name of the system library.  The value is passed to the
             * linker.  You can use this method if you need to override the default settings.
             *
             * \param[in] systemLibraries List of non-default system libraries to be used.
             */
            void setSystemLibraries(const QList<QString>& systemLibraries);

            /**
             * Method you can use to determine the system libraries being passed to to the linker.
             *
             * \return Returns the current list of system libraries.
             */
            QList<QString> systemLibraries() const;

            /**
             * Method you can use to set the system root directory used to locate header and PCH files.
             *
             * This method will block until all pending contexts have been processed by the compiler.
             *
             * \param[in] newSystemRoot The new system root directory.
             */
            void setSystemRoot(const QString& newSystemRoot);

            /**
             * Method you can use to obitan the current system root directory used to locate header and PCH files.
             *
             * \return Returns the current system root directory.
             */
            QString systemRoot() const;

            /**
             * Method you can use to change the current list of header search paths.
             *
             * This method will block until all pending contexts have been processed by the compiler.
             *
             * \param[in] newHeaderSearchPaths the new list of header search paths.
             */
            void setHeaderSearchPaths(const QList<QString>& newHeaderSearchPaths);

            /**
             * Method you can use to obtain the current list of header search paths.
             *
             * \return Returns a list of the header search paths to be used by the compiler.
             */
            QList<QString> headerSearchPaths() const;

            /**
             * Method you can use to change the standard list of headers to be included in any build.
             *
             * This method will block until all pending contexts have been processed by the compiler.
             *
             * \param[in] newHeaders A list of headers to prepended to the C++ build.
             */
            void setStandardHeaders(const QList<QString>& newHeaders);

            /**
             * Method you can use to obtain a list of the standard headers included with each build.
             *
             * \return Returns a list of all the headers included into the build.
             */
            QList<QString> standardHeaders() const;

            /**
             * Method you can use to change the standard list of PCH files to be included in any build.
             *
             * This method will block until all pending contexts have been processed by the compiler.
             *
             * \param[in] newPchFiles A list of PCH files to prepended to the C++ build.
             */
            void setStandardPchFiles(const QList<QString>& newPchFiles);

            /**
             * Method you can use to obtain a list of the PCH files to be included with each build.
             *
             * \return Returns a list of all the PCH files included into the build.
             */
            QList<QString> standardPchFiles() const;

            /**
             * Method you can use to change the current resource directory.
             *
             * This method will block until all pending contexts have been processed by the compiler.
             *
             * \param[in] newResourceDirectory The new resource directory to be used.
             */
            void setResourceDirectory(const QString& newResourceDirectory);

            /**
             * Method you can use to obtain the current resource directory.
             *
             * \return Returns the current resource directory.
             */
            QString resourceDirectory() const;

            /**
             * Method you can use to change the GCC toolchain prefix directory.  Setting this parameter to an empty
             * string will cause the LLVM/CLANG backends to use CLANG's libraries rather than GCC.
             *
             * This method will block until all pending contexts have been processed by the compiler.
             *
             * \param[in] newGccToolchainPrefix The new directory for the GCC toolchain.
             */
            void setGccToolchain(const QString& newGccToolchainPrefix);

            /**
             * Method you can use to determine the GCC standard library prefix.  Specifying that option will cause
             * the compiler to use GCC's c++ includes, libraries, etc. rather than CLANGs.
             *
             * \return Returns the current resource directory.
             */
            QString gccToolchain() const;

            /**
             * Method you can use to change the target triple.
             *
             * \param[in] newTargetTriple The new target triple to be used by the compiler.  An empty string will cause
             *                            the compiler to guess the correct target triple for the system.
             */
            void setTargetTriple(const QString& newTargetTriple);

            /**
             * Method you can use to obtain the current target triple override.
             *
             * \return Returns the current target triple.  An empty string indicates the compiler will guess a correct
             *         target triple for the platform.
             */
            QString targetTriple() const;

            /**
             * Method you can use to change the current list of application specific library search paths.
             *
             * \param[in] newLibrarySearchPaths the new list of library search paths.
             */
            void setLibrarySearchPaths(const QList<QString>& newLibrarySearchPaths);

            /**
             * Method you can use to obtain the current list of application specific library search paths.
             *
             * \return Returns a list of the library search paths to be used by the linker.
             */
            QList<QString> librarySearchPaths() const;

            /**
             * Method you can use to change the list of explicitly included application specific static libraries.
             *
             * This method will block until all pending contexts have been processed by the compiler.
             *
             * \param[in] newStaticLibraries the new list of static libraries.
             */
            void setStaticLibraries(const QList<QString>& newStaticLibraries);

            /**
             * Method you can use to obtain the current list of explicitly included application specific static
             * libraries.
             *
             * \return Returns a list of the static libraries that should be explicitly included into the build.
             */
            QList<QString> staticLibraries() const;

            /**
             * Method you can use to change the list of explicitly included application specific dynamic libraries.
             *
             * This method will block until all pending contexts have been processed by the compiler.
             *
             * \param[in] newDynamicLibraries the new list of dynamic libraries.
             */
            void setDynamicLibraries(const QList<QString>& newDynamicLibraries);

            /**
             * Method you can use to obtain the current list of explicitly included application specific dynamic
             * libraries.
             *
             * \return Returns a list of the dynamic libraries that should be explicitly included into the build.
             */
            QList<QString> dynamicLibraries() const;

            /**
             * Method you can use to change the list of run-time search paths to be used by the linked object.  This
             * method provides capabilities similar to the --rpath switch on Linux and -rpath switch on MacOS.  The
             * call is ignored on Windows.
             *
             * \param[in] newRunTimeSearchPaths The new list of run-time search paths.
             */
            void setRunTimeSearchPaths(const QList<QString>& newRunTimeSearchPaths);

            /**
             * Method you can use to obtain the current list of run-time search paths to be used by the linked object.
             *
             * \return Returns a list of the run-time paths to be used by the linked object.  Note that this method will
             *         always return an empty list on Windows.
             */
            QList<QString> runTimeSearchPaths() const;

            /**
             * Method you can use to obtain the last generated intermediate representation.
             *
             * \return Returns the translated intermediate representation.   The default implementation returns an empty
             *         byte array.
             */
            const QByteArray& intermediateRepresentation() const final;

            /**
             * Method you can use to register a C++ declaration payload.
             *
             * \param[in] library    The library tied to this payload.
             *
             * \param[in] payload    The payload.  The payload is expected to remain resident during the entire
             *                       lifetime of the application.
             *
             * \param[in] length     The length of the payload, in bytes.
             *
             * \param[in] compressed If true, the payload is compressed.  If false, the payload is not compressed.
             *
             * \return Returns true on success, returns false on error.
             */
            bool registerCppDeclarationPayload(
                const QString&       library,
                const unsigned char* payload,
                unsigned long        length,
                bool                 compressed = false
            );

        protected:
            /**
             * Method that should be overloaded in derived classes to create the correct code generation engine.
             *
             * \param[in] rootElement The root element to translate.
             *
             * \param[in] outputFile  The name of the output file to be created.
             *
             * \param[in] outputType  The output type to generate code for.
             *
             * \param[in] exportMode  A value indicating the export mode to be used.
             *
             * \param[in] usageData   The usage data instance.
             *
             * \return Returns a shared pointer to the code generation engine to be used.
             */
            virtual QSharedPointer<CodeGenerationEngine> createEngine(
                QSharedPointer<RootElement>             rootElement,
                const QString&                          outputFile,
                const CodeGeneratorOutputTypeContainer& outputType,
                CodeGeneratorOutputType::ExportMode     exportMode,
                Ud::UsageData*                          usageData
            ) final;

        private:
            /**
             * List of supported output types.
             */
            QList<CodeGeneratorOutputTypeContainer> currentOutputTypes;

            /**
             * The C++ compiler to use for compilation jobs.
             */
            QScopedPointer<Cbe::CppCompiler> currentCompiler;

            /**
             * The linker used to link shared objects.
             */
            QScopedPointer<Cbe::DynamicLibraryLinker> currentLinker;
    };
};

#endif
