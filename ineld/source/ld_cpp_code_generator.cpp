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
* This file implements the \ref Ld::CppCodeGenerator class.
***********************************************************************************************************************/

#include <QString>
#include <QByteArray>
#include <QSharedPointer>

#include <cstdint>

#include <cbe_cpp_compiler.h>
#include <cbe_linker.h>
#include <cbe_dynamic_library_linker.h>
#include <cbe_dynamic_library_loader.h>

#include "ld_code_generator_output_type_container.h"
#include "ld_cpp_code_generator_output_types.h"
#include "ld_cpp_declaration_payload.h"
#include "ld_diagnostic.h"
#include "ld_element.h"
#include "ld_root_element.h"
#include "ld_cpp_code_generator_diagnostic.h"
#include "ld_cpp_code_generation_engine.h"
#include "ld_cpp_context.h"
#include "ld_code_generator.h"
#include "ld_cpp_code_generator.h"

namespace Ld {
    const char CppCodeGenerator::codeGeneratorName[] = "CppCodeGenerator";

    CppCodeGenerator::CppCodeGenerator(
            CodeGeneratorVisual* visual
        ):CodeGenerator(
            visual
        ),currentCompiler(
            new Cbe::CppCompiler
        ),currentLinker(
            new Cbe::DynamicLibraryLinker
        ) {
        currentOutputTypes.clear();
        currentOutputTypes << CodeGeneratorOutputTypeContainer(new CppLoadableModuleOutputType());
    }


    CppCodeGenerator::~CppCodeGenerator() {}


    QString CppCodeGenerator::typeName() const {
        return QString(codeGeneratorName);
    }


    void CppCodeGenerator::setDebugOutputEnabled(bool enableDebugOutput) {
        currentCompiler->setDebugOutputEnabled(enableDebugOutput);
        currentLinker->setDebugOutputEnabled(enableDebugOutput);
    }


    void CppCodeGenerator::setDebugOutputDisabled(bool disableDebugOutput) {
        currentCompiler->setDebugOutputDisabled(disableDebugOutput);
        currentLinker->setDebugOutputDisabled(disableDebugOutput);
    }


    bool CppCodeGenerator::debugOutputEnabled() const {
        return currentCompiler->debugOutputEnabled();
    }


    bool CppCodeGenerator::debugOutputDisabled() const {
        return currentCompiler->debugOutputDisabled();
    }


    QList<CodeGeneratorOutputTypeContainer> CppCodeGenerator::supportedOutputTypes() const {
        return currentOutputTypes;
    }


    void CppCodeGenerator::setExecutableDirectory(const QString& newExecutableDirectory) {
        currentCompiler->setExecutableDirectory(newExecutableDirectory);
        currentLinker->setExecutableDirectory(newExecutableDirectory);
    }


    QString CppCodeGenerator::executableDirectory() const {
        QString result = currentCompiler->executableDirectory();

        if (result.isEmpty()) {
            result = currentLinker->executableDirectory();
        }

        return result;
    }


    void CppCodeGenerator::setLinkerExecutable(const QString& linkerExecutable) {
        currentLinker->setLinkerExecutable(linkerExecutable);
    }


    QString CppCodeGenerator::linkerExecutable() const {
        return currentLinker->linkerExecutable();
    }


    void CppCodeGenerator::setSystemLibraries(const QList<QString>& systemLibraries) {
        currentLinker->setSystemLibraries(systemLibraries);
    }


    QList<QString> CppCodeGenerator::systemLibraries() const {
        return currentLinker->systemLibraries();
    }


    void CppCodeGenerator::setSystemRoot(const QString& newSystemRoot) {
        currentCompiler->setSystemRoot(newSystemRoot);
        currentLinker->setSystemRoot(newSystemRoot);
    }


    QString CppCodeGenerator::systemRoot() const {
        QString result = currentCompiler->systemRoot();

        if (result.isEmpty()) {
            result = currentLinker->systemRoot();
        }

        return result;
    }


    void CppCodeGenerator::setHeaderSearchPaths(const QList<QString>& newHeaderSearchPaths) {
        currentCompiler->setHeaderSearchPaths(newHeaderSearchPaths);
    }


    QList<QString> CppCodeGenerator::headerSearchPaths() const {
        return currentCompiler->headerSearchPaths();
    }


    void CppCodeGenerator::setStandardHeaders(const QList<QString>& newHeaders) {
        currentCompiler->setHeaders(newHeaders);
    }


    QList<QString> CppCodeGenerator::standardHeaders() const {
        return currentCompiler->headers();
    }


    void CppCodeGenerator::setStandardPchFiles(const QList<QString>& newPchFiles) {
        currentCompiler->setPrecompiledHeaders(newPchFiles);
    }


    QList<QString> CppCodeGenerator::standardPchFiles() const {
        return currentCompiler->precompiledHeaders();
    }


    void CppCodeGenerator::setResourceDirectory(const QString& newResourceDirectory) {
        currentCompiler->setResourceDirectory(newResourceDirectory);
    }


    QString CppCodeGenerator::resourceDirectory() const {
        return currentCompiler->resourceDirectory();
    }


    void CppCodeGenerator::setGccToolchain(const QString& newGccToolchainPrefix) {
        currentCompiler->setGccToolchain(newGccToolchainPrefix);
    }


    QString CppCodeGenerator::gccToolchain() const {
        return currentCompiler->gccToolchain();
    }


    void CppCodeGenerator::setTargetTriple(const QString& newTargetTriple) {
        currentCompiler->setTargetTriple(newTargetTriple);
    }


    QString CppCodeGenerator::targetTriple() const {
        return currentCompiler->targetTriple();
    }


    void CppCodeGenerator::setLibrarySearchPaths(const QList<QString>& newLibrarySearchPaths) {
        currentLinker->setLibrarySearchPaths(newLibrarySearchPaths);
    }


    QList<QString> CppCodeGenerator::librarySearchPaths() const {
        return currentLinker->librarySearchPaths();
    }


    void CppCodeGenerator::setStaticLibraries(const QList<QString>& newStaticLibraries) {
        currentLinker->setStaticLibraries(newStaticLibraries);
    }


    QList<QString> CppCodeGenerator::staticLibraries() const {
        return currentLinker->staticLibraries();
    }


    void CppCodeGenerator::setDynamicLibraries(const QList<QString>& newDynamicLibraries) {
        currentLinker->setDynamicLibraries(newDynamicLibraries);
    }


    QList<QString> CppCodeGenerator::dynamicLibraries() const {
        return currentLinker->dynamicLibraries();
    }


    void CppCodeGenerator::setRunTimeSearchPaths(const QList<QString>& newRunTimeSearchPaths) {
        currentLinker->setRunTimeSearchPaths(newRunTimeSearchPaths);
    }


    QList<QString> CppCodeGenerator::runTimeSearchPaths() const {
        return currentLinker->runTimeSearchPaths();
    }


    const QByteArray& CppCodeGenerator::intermediateRepresentation() const {
        return dynamic_cast<const CppCodeGenerationEngine&>(codeGenerationEngine()).context().sourceData();
    }


    bool CppCodeGenerator::registerCppDeclarationPayload(
            const QString&       library,
            const unsigned char* payload,
            unsigned long        length,
            bool                 compressed
        ) {
        return CppDeclarationPayload::registerPayload(CppDeclarationPayload(library, payload, length, compressed));
    }


    QSharedPointer<CodeGenerationEngine> CppCodeGenerator::createEngine(
            QSharedPointer<RootElement>             rootElement,
            const QString&                          outputFile,
            const CodeGeneratorOutputTypeContainer& outputType,
            CodeGeneratorOutputType::ExportMode     exportMode,
            Ud::UsageData*                          usageData
        ) {
        CodeGenerationEngine* engine = new CppCodeGenerationEngine(
            this,
            rootElement,
            outputFile,
            outputType,
            exportMode,
            usageData
        );

        return QSharedPointer<CodeGenerationEngine>(engine);
    }
}
