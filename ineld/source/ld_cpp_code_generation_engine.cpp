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
* This file implements the \ref Ld::CppCodeGenerationEngine class.
***********************************************************************************************************************/

#include <QSharedPointer>
#include <QString>
#include <QByteArray>
#include <QSet>
#include <QTemporaryFile>
#include <QFile>
#include <QSemaphore>

#include <cassert>

#include <model_api.h>
#include <model_status.h>

#include <ud_usage_data.h>

#include <cbe_cpp_compiler.h>
#include <cbe_dynamic_library_linker.h>
#include <cbe_dynamic_library_loader.h>
#include <cbe_cpp_compiler_diagnostic.h>
#include <cbe_source_range.h>

#include "ld_environment.h"
#include "ld_element_structures.h"
#include "ld_element.h"
#include "ld_root_element.h"
#include "ld_code_generator_output_type.h"
#include "ld_code_generator_output_type_container.h"
#include "ld_translation_phase.h"
#include "ld_cpp_translation_phase.h"
#include "ld_cpp_translator.h"
#include "ld_code_generator.h"
#include "ld_cpp_code_generator.h"
#include "ld_cpp_context.h"
#include "ld_cpp_declaration_payload.h"
#include "ld_cpp_library_information.h"
#include "ld_cpp_library_dependencies.h"
#include "ld_cpp_code_generator_diagnostic.h"
#include "ld_diagnostic.h"
#include "ld_diagnostic_structures.h"
#include "ld_identifier.h"
#include "ld_identifier_container.h"
#include "ld_identifier_database.h"
#include "ld_operation.h"
#include "ld_operation_database.h"
#include "ld_data_type.h"
#include "ld_data_type_translator.h"
#include "ld_cpp_data_type_translator.h"
#include "ld_cpp_code_generator_diagnostic.h"
#include "ld_code_generation_engine.h"
#include "ld_cpp_code_generation_engine.h"

namespace Ld {
    const QString CppCodeGenerationEngine::modelAllocatorFunctionName(Model::allocatorFunctionName);
    const QString CppCodeGenerationEngine::modelDeallocatorFunctionName(Model::deallocatorFunctionName);

    CppCodeGenerationEngine::CppCodeGenerationEngine(
            CodeGenerator*                          codeGenerator,
            QSharedPointer<RootElement>             rootElement,
            const QString&                          outputFile,
            const CodeGeneratorOutputTypeContainer& outputType,
            CodeGeneratorOutputType::ExportMode     exportMode,
            Ud::UsageData*                          usageData
        ):CodeGenerationEngine(
            codeGenerator,
            rootElement,
            outputFile,
            outputType,
            exportMode,
            usageData
        ),currentIdentifierDatabase(
            rootElement->identifierDatabase()
        ) {
        if (outputType.applicationLoadable()) {

            #if (defined(Q_OS_WIN))

                objectFile = temporaryFilename(".obj");

            #elif (defined(Q_OS_LINUX) || defined(Q_OS_DARWIN))

                objectFile = temporaryFilename(".o");

            #else

                #error Unknown platform.

            #endif

        } else {
            objectFile = outputFile;
        }

        currentContext = QSharedPointer<CppContext>(new CppContext(objectFile, outputFile, this));

        currentScopeElement = rootElement;
        currentScopeForced  = true;

        parentScopeStack.append(rootElement);
        parentScopeForcedStack.append(true);

        currentAsLValue = false;
    }


    CppCodeGenerationEngine::~CppCodeGenerationEngine() {
        if (outputType().isDefined() && outputType().applicationLoadable()) {
            deleteFileIfExists(objectFile);
        }

        if (exportMode() == CodeGeneratorOutputType::ExportMode::NO_EXPORT) {
            deleteFileIfExists(outputFile());
        }
    }


    void CppCodeGenerationEngine::dependsOnHeader(const QString& headerName) {
        requiredHeaders.insert(headerName);
    }


    void CppCodeGenerationEngine::dependsOnLibrary(const QString& libraryName) {
        if (!requiredLibraries.contains(libraryName)) {
            CppDeclarationPayload declarationPayload = CppDeclarationPayload::payload(libraryName);
            if (declarationPayload.isValid()) {
                QByteArray declaration = declarationPayload.payload();
                *currentContext << QString::fromUtf8(declaration);
            }

            requiredLibraries.insert(libraryName);
        }
    }


    bool CppCodeGenerationEngine::insertOperationCheckpoint(ElementPointer element) {
        bool                        result = false;
        QSharedPointer<RootElement> root   = element->root().dynamicCast<RootElement>();

        if (!root.isNull()) {
            Operation operation = root->operationDatabase().createOperation(element);
            if (operation.isValid()) {
                currentContext->startNewStatement();
                (*currentContext) << QString("{ c(pt, %1); } ").arg(operation.handle());
                currentContext->startedNewStatement();
                result = true;
            }
        }

        return result;
    }


    bool CppCodeGenerationEngine::insertOperationCheckpoint(
            ElementPointer     element,
            Identifier::Handle identifierHandle
        ) {
        bool                        result = false;
        QSharedPointer<RootElement> root   = element->root().dynamicCast<RootElement>();

        if (!root.isNull()) {
            Operation operation = root->operationDatabase().createOperation(element);
            if (operation.isValid()) {
                currentContext->startNewStatement();
                (*currentContext) << QString("{ c(pt, %1, %2); } ")
                                     .arg(operation.handle())
                                     .arg(identifierHandle);
                currentContext->startedNewStatement();

                result = true;
            }
        }

        return result;
    }


    CppContext& CppCodeGenerationEngine::context() {
        return *currentContext;
    }


    const CppContext& CppCodeGenerationEngine::context() const {
        return *currentContext;
    }


    QSharedPointer<CppContext> CppCodeGenerationEngine::contextPointer() const {
        return currentContext;
    }



    void CppCodeGenerationEngine::addIdentifier(Identifier* newIdentifier) {
        return addIdentifier(IdentifierContainer(newIdentifier));
    }


    void CppCodeGenerationEngine::addIdentifier(QSharedPointer<Identifier> newIdentifier) {
        return addIdentifier(IdentifierContainer(newIdentifier));
    }


    void CppCodeGenerationEngine::addIdentifier(const IdentifierContainer& newIdentifier) {
        currentIdentifierDatabase.insert(newIdentifier);
    }


    void CppCodeGenerationEngine::addIdentifier(
        const QString&        text1,
        const QString&        text2,
        Identifier::DefinedAs definedAs,
        const DataType&       dataType
    ) {
        IdentifierContainer identifier(
            new Identifier(
                text1,
                text2,
                definedAs,
                currentScopeElement,
                dataType
            )
        );

        addIdentifier(identifier);
    }


    bool CppCodeGenerationEngine::tieElementToIdentifier(
            const QString& text1,
            const QString& text2,
            ElementPointer element
        ) {
        bool result;

        IdentifierContainer identifier = currentIdentifierDatabase.entryByName(text1, text2, currentScopeElement);
        if (identifier.isValid()) {
            identifier->insertElementData(ElementValueData(element));
            result = true;
        } else {
            result = false;
        }

        return result;
    }


    IdentifierContainer CppCodeGenerationEngine::identifier(const QString& text1, const QString& text2) {
        IdentifierContainer result;

        VariableName variableName(text1, text2);

        if (currentScopeForced) {
            result = currentIdentifierDatabase.entryByName(variableName, currentScopeElement);
        } else {
            unsigned     currentScopeDepth = static_cast<unsigned>(parentScopeStack.size());
            if (currentScopeDepth >  0) {
                unsigned depth = currentScopeDepth;
                do {
                    --depth;
                    ElementPointer parentScope = parentScopeStack.at(depth);
                    result = currentIdentifierDatabase.entryByName(variableName, parentScope);
                } while (result.isInvalid() && depth > 0);
            }
        }

        return result;
    }


    IdentifierContainer CppCodeGenerationEngine::identifier(
            const QString& text1,
            const QString& text2,
            ElementPointer identifier
        ) {
        return currentIdentifierDatabase.entryByName(text1, text2, identifier);
    }


    void CppCodeGenerationEngine::pushNewScope(ElementPointer newParentScope, bool forceParentScope) {
        parentScopeStack.append(newParentScope);
        parentScopeForcedStack.append(forceParentScope);

        currentScopeElement = newParentScope;
        currentScopeForced  = forceParentScope;
    }


    void CppCodeGenerationEngine::popCurrentScope() {
        assert(!parentScopeStack.isEmpty());

        parentScopeStack.removeLast();
        parentScopeForcedStack.removeLast();

        assert(!parentScopeStack.isEmpty());

        currentScopeElement = parentScopeStack.last();
        currentScopeForced  = parentScopeForcedStack.last();

        currentAsLValue = false;
    }


    ElementPointer CppCodeGenerationEngine::currentScope() const {
        return currentScopeElement;
    }


    bool CppCodeGenerationEngine::currentScopeIsForced() const {
        return currentScopeForced;
    }


    void CppCodeGenerationEngine::clearForcedScope(bool nowNotForced) {
        currentScopeForced = !nowNotForced;
        parentScopeForcedStack.last() = currentScopeForced;
    }



    QList<IdentifierContainer> CppCodeGenerationEngine::identifiersInScope(ElementPointer scope) const {
        return currentIdentifierDatabase.identifiersInScope(scope);
    }


    bool CppCodeGenerationEngine::atGlobalScope() const {
        return (
               parentScopeStack.isEmpty()
            || (parentScopeStack.size() == 1 && parentScopeStack.first() == rootElement())
        );
    }


    void CppCodeGenerationEngine::setAsLValue(bool nowAsLValue) {
        currentAsLValue = nowAsLValue;
    }


    void CppCodeGenerationEngine::setAsRValue(bool nowAsRValue) {
        setAsLValue(!nowAsRValue);
    }


    bool CppCodeGenerationEngine::asLValue() const {
        return currentAsLValue;
    }


    bool CppCodeGenerationEngine::asRValue() const {
        return !asLValue();
    }


    TranslationPhase* CppCodeGenerationEngine::createTranslationPhase() const {
        bool generateDynamicLibrary = outputType().applicationLoadable();
        return new CppTranslationPhase(generateDynamicLibrary);
    }


    bool CppCodeGenerationEngine::includeRootImports() const {
        return true;
    }


    bool CppCodeGenerationEngine::preTranslate() {
        bool                       success;
        const CppTranslationPhase& cppTranslationPhase = dynamic_cast<const CppTranslationPhase&>(translationPhase());
        switch(cppTranslationPhase.phase()) {
            case CppTranslationPhase::Phase::IDENTIFY_DEPENDENCIES_AND_EXPLICIT_TYPES: {
                success = preIdentifyDependenciesAndExplicitTypes();
                break;
            }

            case CppTranslationPhase::Phase::IDENTIFY_INFERRED_TYPES: {
                success = preIdentifyInferredTypes();
                break;
            }

            case CppTranslationPhase::Phase::DECLARATIONS: {
                success = preDeclarations();
                break;
            }

            case CppTranslationPhase::Phase::MODEL_CLASS_DECLARATION: {
                success = preModelClassDeclaration();
                break;
            }

            case CppTranslationPhase::Phase::METHOD_DECLARATIONS: {
                success = preMethodDeclarations();
                break;
            }

            case CppTranslationPhase::Phase::CLASS_SCOPE_VARIABLES: {
                success = preClassScopeVariables();
                break;
            }

            case CppTranslationPhase::Phase::END_MODEL_CLASS_DECLARATION: {
                success = preEndModelClassDeclaration();
                break;
            }

            case CppTranslationPhase::Phase::THREAD_DEFINITION: {
                success = preThreadDefinition();
                break;
            }

            case CppTranslationPhase::Phase::THREAD_LOCALS: {
                success = preThreadLocals();
                break;
            }

            case CppTranslationPhase::Phase::THREAD_IMPLEMENTATION: {
                success = preThreadImplementation();
                break;
            }

            case CppTranslationPhase::Phase::THREAD_POST_PROCESSING: {
                success = preThreadPostProcessing();
                break;
            }

            case CppTranslationPhase::Phase::THREAD_CLEANUP: {
                success = preThreadCleanup();
                break;
            }

            case CppTranslationPhase::Phase::THREAD_END: {
                success = preThreadEnd();
                break;
            }

            case CppTranslationPhase::Phase::METHOD_DEFINITIONS: {
                success = preMethodDefinitions();
                break;
            }

            case CppTranslationPhase::Phase::BOOKKEEPING_DEFINITION: {
                success = preBookkeepingDefinition();
                break;
            }

            case CppTranslationPhase::Phase::MODEL_ALLOCATOR: {
                success = preModelAllocator();
                break;
            }

            case CppTranslationPhase::Phase::CONVERT_IR_TO_OBJECT: {
                success = preConvertIrToObject();
                break;
            }

            case CppTranslationPhase::Phase::LINK: {
                success = preLink();
                break;
            }

            default: {
                CppCodeGeneratorDiagnostic* diagnostic = new CppCodeGeneratorDiagnostic(
                    nullptr,
                    CppCodeGeneratorDiagnostic::Type::INTERNAL_ERROR,
                    cppTranslationPhase,
                    CppCodeGeneratorDiagnostic::Code::UNEXPECTED_TRANSLATION_PHASE
                );

                translationErrorDetected(diagnostic);
                success = false;

                break;
            }
        }

        return success;
    }


    bool CppCodeGenerationEngine::postTranslate() {
        return true;
    }


    bool CppCodeGenerationEngine::preIdentifyDependenciesAndExplicitTypes() {
        requiredHeaders.clear();
        requiredLibraries.clear();

        rootElement()->identifierDatabase().clear();

        return true;
    }


    bool CppCodeGenerationEngine::preIdentifyInferredTypes() {
        #if (defined(Q_OS_WIN))

            // Block below is required on Windows using the Visual Studion 2017 libcpmt library to get around a new
            // Microsoft dependency between the library and the linker.  Apparently the newer Microsoft Linker inserts
            // these symbols as required by the new libcpmt library.  The code below should be removed when we migrate
            // to a different run-time library.
            //
            // The __guard_eh_cont_table and __guard_eh_cont_count variables were added to support the
            // VC-LTL runtime library.  That runtime expects code to be built with the /guard:ehcont switch on MSVC
            // and the MSVC linker.  Since we're based on CLANG/LLVM, and don't throw exceptions within the code that
            // LLVM/CLANG creates, we simply avoid this feature.

            *currentContext << "extern \"C\" {\n"
                            << "void* __enclave_config;\n"
                            << "void* __enclave_configerror;\n"
                            << "void* __guard_eh_cont_table[1];\n"
                            << "unsigned long __guard_eh_cont_count = 0;\n"
                            << "}\n";

        #endif

        currentContext->startedNewStatement();

        return true;
    }


    bool CppCodeGenerationEngine::preDeclarations() {
        return true;
    }


    bool CppCodeGenerationEngine::preModelClassDeclaration() {
        currentContext->startNewStatement();

        *currentContext << "class ModelImpl:public M::ModelBase {\n"
                           "public:\n"
                           "ModelImpl() = default;\n"
                           "virtual ~ModelImpl() = default;\n"
                           "unsigned numberThreads() const final;\n"
                           "M::OperationHandle numberOperationHandles() const final;\n"
                           "M::IdentifierDatabase identifierDatabase() final;\n"
                           "protected:\n";
        currentContext->startedNewStatement();

        const CppTranslationPhase& cppTranslationPhase = dynamic_cast<const CppTranslationPhase&>(translationPhase());
        unsigned                   numberThreads       = cppTranslationPhase.numberThreads();
        for (unsigned threadId=0 ; threadId<numberThreads ; ++threadId) {
            currentContext->startNewStatement();
            *currentContext << QString("void t%1(M::PerThread& pt) final").arg(threadId + 1);
        }

        return true;
    }


    bool CppCodeGenerationEngine::preClassScopeVariables() {
        bool                              success     = true;
        const IdentifierDatabase&         database    = rootElement()->identifierDatabase();
        const QList<IdentifierContainer>& identifiers = database.identifiersInScope(rootElement());

        currentContext->startNewStatement();
        *currentContext << "private:\n";
        currentContext->startedNewStatement();

        QList<IdentifierContainer>::const_iterator identifierIterator    = identifiers.constBegin();
        QList<IdentifierContainer>::const_iterator identifierEndIterator = identifiers.constEnd();
        while (success && identifierIterator != identifierEndIterator) {
            const IdentifierContainer identifier = *identifierIterator;
            Identifier::DefinedAs     definedAs  = identifier.definedAs();

            if (definedAs == Identifier::DefinedAs::GLOBAL_SCOPE_VARIABLE) {
                DataType dataType = identifier.dataType();
                if (dataType.isInvalid()) {
                    dataType = DataType::defaultDataType();
                }

                const CppDataTypeTranslator* translator = dynamic_cast<const CppDataTypeTranslator*>(
                    dataType.translator(CppCodeGenerator::codeGeneratorName)
                );

                if (translator != nullptr) {
                    QString dataTypeName = translator->dataTypeName();

                    currentContext->startNewStatement();
                    (*currentContext) << QString("%1 %2").arg(dataTypeName).arg(identifier.internalName());
                } else {
                    ElementPointer primaryElement = identifier->primaryElement();
                    if (primaryElement.isNull()) {
                        for (  Identifier::ElementIterator elementIterator    = identifier->begin(),
                                                           elementEndIterator = identifier->end()
                             ; elementIterator != elementEndIterator
                             ; ++elementIterator
                            ) {
                            ElementPointer element = elementIterator->element();
                            if (!element.isNull()) {
                                CppCodeGeneratorDiagnostic* diagnostic = new CppCodeGeneratorDiagnostic(
                                    element,
                                    CppCodeGeneratorDiagnostic::Type::INTERNAL_ERROR,
                                    dynamic_cast<const CppTranslationPhase&>(translationPhase()),
                                    CppCodeGeneratorDiagnostic::Code::MISSING_DATA_TYPE_TRANSLATOR
                                );
                                translationErrorDetected(diagnostic);
                            }
                        }

                        success = false;
                    }
                }
            }

            ++identifierIterator;
        }

        return success;
    }


    bool CppCodeGenerationEngine::preMethodDeclarations() {
        return true;
    }


    bool CppCodeGenerationEngine::preEndModelClassDeclaration() {
        currentContext->startNewStatement();
        *currentContext << "}";

        return true;
    }


    bool CppCodeGenerationEngine::preThreadDefinition() {
        const CppTranslationPhase& cppTranslationPhase = dynamic_cast<const CppTranslationPhase&>(translationPhase());
        unsigned                   threadId            = cppTranslationPhase.threadId();

        currentContext->startNewStatement();
        *currentContext << QString(
            "void ModelImpl::t%1(M::PerThread& pt) {\n"
            "pt.threadLocalSetup();\n"
            "M::Console::report("
                "%1, "
                "M::Console::MessageType::INFORMATION, "
                "\"Entered thread %1 .\""
            ");\n"
        ).arg(threadId + 1);
        currentContext->startedNewStatement();

        return true;
    }


    bool CppCodeGenerationEngine::preThreadLocals() {
        return true;
    }


    bool CppCodeGenerationEngine::preThreadImplementation() {
        rootElement()->operationDatabase().clear();
        return true;
    }


    bool CppCodeGenerationEngine::preThreadPostProcessing() {
        return true;
    }


    bool CppCodeGenerationEngine::preThreadCleanup() {
        currentContext->startNewStatement();
        return true;
    }


    bool CppCodeGenerationEngine::preThreadEnd() {
        const CppTranslationPhase& cppTranslationPhase = dynamic_cast<const CppTranslationPhase&>(translationPhase());
        unsigned                   threadId            = cppTranslationPhase.threadId();

        currentContext->startNewStatement();
        *currentContext << QString(
            "M::Console::report("
                "%1, "
                "M::Console::MessageType::INFORMATION, "
                "\"Exiting thread %1.\""
            ");\n"
            "return;\n"
            "}\n"
        ).arg(threadId + 1);
        currentContext->startedNewStatement();

        return true;
    }


    bool CppCodeGenerationEngine::preMethodDefinitions() {
        return true;
    }


    bool CppCodeGenerationEngine::preBookkeepingDefinition() {
        const CppTranslationPhase& cppTranslationPhase = dynamic_cast<const CppTranslationPhase&>(translationPhase());
        unsigned                   numberThreads       = cppTranslationPhase.numberThreads();

        currentContext->startNewStatement();
        *currentContext << QString("unsigned ModelImpl::numberThreads() const { return %1; }\n").arg(numberThreads);
        currentContext->startedNewStatement();

        OperationDatabase&  operationDatabase       = rootElement()->operationDatabase();
        Operation::Handle   numberOperationHandles  = operationDatabase.size();

        currentContext->startNewStatement();
        *currentContext << QString(
            "M::OperationHandle ModelImpl::numberOperationHandles() const { return %1; }\n"
        ).arg(numberOperationHandles);
        currentContext->startedNewStatement();

        *currentContext << "M::IdentifierDatabase ModelImpl::identifierDatabase() {\n"
                        << "M::IdentifierDatabase r;\n";

        QList<IdentifierContainer> identifiers = currentIdentifierDatabase.identifiersInScope(rootElement());
        for (  QList<IdentifierContainer>::const_iterator identifierIterator    = identifiers.constBegin(),
                                                          identifierEndIterator = identifiers.constEnd()
             ; identifierIterator != identifierEndIterator
             ; ++identifierIterator
            ) {
            const IdentifierContainer& identifier = *identifierIterator;
            Identifier::DefinedAs      definedAs  = identifier.definedAs();
            if (definedAs == Identifier::DefinedAs::GLOBAL_SCOPE_VARIABLE ||
                definedAs == Identifier::DefinedAs::GLOBAL_SCOPE_FUNCTION    ) {

                DataType dataType = identifier.dataType();
                if (dataType.isInvalid()) {
                    dataType = DataType::defaultDataType();
                }

                const CppDataTypeTranslator* dataTypeTranslator = dynamic_cast<const CppDataTypeTranslator*>(
                    dataType.translator(CppCodeGenerator::codeGeneratorName)
                );

                if (definedAs == Identifier::DefinedAs::GLOBAL_SCOPE_VARIABLE) {
                    *currentContext << QString("r.insert(%1, \"%2\", \"%3\", false, %4, &%5);\n")
                                       .arg(identifier.handle())
                                       .arg(identifier.text1())
                                       .arg(identifier.text2())
                                       .arg(dataTypeTranslator->valueTypeName())
                                       .arg(identifier.internalName());
                } else {
                    *currentContext << QString(
                                           "r.insert(%1, \"%2\", \"%3\", true, %4, reinterpret_cast<void*>(&W%5));\n"
                                       ).arg(identifier.handle())
                                        .arg(identifier.text1())
                                        .arg(identifier.text2())
                                        .arg(dataTypeTranslator->valueTypeName())
                                        .arg(identifier.internalName());
                }
            }
        }

        *currentContext << "return r;\n}\n";
        currentContext->startedNewStatement();

        return true;
    }


    bool CppCodeGenerationEngine::preModelAllocator() {
        #if (defined(Q_OS_WIN))

            QString declspec = QString("__declspec(dllexport)");

        #elif (defined(Q_OS_LINUX) || defined(Q_OS_DARWIN))

            QString declspec = QString();

        #else

            #error Unknown platform.

        #endif

        currentContext->startNewStatement();
        *currentContext << QString(
                               "extern \"C\" %1 void* %2(const MatApi* matrixApi) { "
                                   "M::Api::setMatrixApi(matrixApi);"
                                   "ModelImpl* model = new ModelImpl;"
                                   "M::Api* api = new M::Api(model);"
                                   "model->setApi(api);"
                                   "return api;"
                               "}\n"
                           ).arg(declspec).arg(modelAllocatorFunctionName);

        currentContext->startedNewStatement();

        currentContext->startNewStatement();
        *currentContext << QString(
            "extern \"C\" %1 void %2(void* model) { delete reinterpret_cast<M::Api*>(model); }\n"
        ).arg(declspec).arg(modelDeallocatorFunctionName);

        currentContext->startedNewStatement();

        return true;
    }


    bool CppCodeGenerationEngine::preConvertIrToObject() {
        assert(backendSemaphore.available() == 0);

        dynamic_cast<CppCodeGenerator&>(codeGenerator()).currentCompiler->compile(currentContext);
        backendSemaphore.acquire(1);

        return backendWasSuccessful;
    }


    bool CppCodeGenerationEngine::preLink() {
        assert(backendSemaphore.available() == 0);

        QSet<QString> staticLibraries = requiredLibraries;
        QSet<QString> dynamicLibraries;

        for (  QSet<QString>::const_iterator libraryIterator = requiredLibraries.constBegin(),
                                             libraryEndITerator = requiredLibraries.constEnd()
             ; libraryIterator != libraryEndITerator
             ; ++libraryIterator
            ) {
            QList<CppLibraryInformation> dependencies = CppLibraryDependencies::dependencies(*libraryIterator);
            for (  QList<CppLibraryInformation>::const_iterator dependenciesIterator    = dependencies.constBegin(),
                                                                dependenciesEndIterator = dependencies.constEnd()
                 ; dependenciesIterator != dependenciesEndIterator
                 ; ++dependenciesIterator
                ) {
                if (dependenciesIterator->libraryType() == CppLibraryInformation::LibraryType::STATIC) {
                    staticLibraries.insert(dependenciesIterator->libraryName());
                } else {
                    assert(dependenciesIterator->libraryType() == CppLibraryInformation::LibraryType::DYNAMIC);
                    dynamicLibraries.insert(dependenciesIterator->libraryName());
                }
            }
        }

        currentContext->setStaticLibraries(staticLibraries.values());
        currentContext->setDynamicLibraries(dynamicLibraries.values());

        dynamic_cast<CppCodeGenerator&>(codeGenerator()).currentLinker->link(currentContext);
        backendSemaphore.acquire(1);

        return backendWasSuccessful;
    }


    void CppCodeGenerationEngine::handleCompilerDiagnostic(const Cbe::CppCompilerDiagnostic& diagnostic) {
        unsigned long  byteOffset     = diagnostic.sourceRange().byteOffset();
        ElementPointer problemElement = currentContext->elementAt(byteOffset);

        CppCodeGeneratorDiagnostic::Type diagnosticType;

        switch (diagnostic.level()) {
            case Cbe::CppCompilerDiagnostic::Level::UNKNOWN: {
                diagnosticType = CppCodeGeneratorDiagnostic::Type::NONE;
                break;
            }

            case Cbe::CppCompilerDiagnostic::Level::NOTE:
            case Cbe::CppCompilerDiagnostic::Level::REMARK:
            case Cbe::CppCompilerDiagnostic::Level::WARNING: {
                diagnosticType = CppCodeGeneratorDiagnostic::Type::WARNING;
                break;
            }

            case Cbe::CppCompilerDiagnostic::Level::ERROR: {
                diagnosticType = CppCodeGeneratorDiagnostic::Type::FATAL_ERROR;
                break;
            }

            default: {
                diagnosticType = CppCodeGeneratorDiagnostic::Type::INTERNAL_ERROR;
                break;
            }
        }

        if (codeGenerator().enabledDiagnosticTypes().contains(diagnosticType)) {
            CppCodeGeneratorDiagnostic* diag = new CppCodeGeneratorDiagnostic(
                problemElement,
                diagnosticType,
                dynamic_cast<const CppTranslationPhase&>(translationPhase()),
                CppCodeGeneratorDiagnostic::Code::COMPILER_DIAGNOSTIC,
                diagnostic.message(),
                currentContext,
                static_cast<CppCodeGeneratorDiagnostic::CompilerCode>(diagnostic.code()),
                byteOffset,
                diagnostic.sourceRange().startLineNumber(),
                diagnostic.sourceRange().startColumnNumber()
            );

            DiagnosticPointer languageDiagnostic(diag);

            if (problemElement != nullptr) {
                problemElement->flagDiagnostic(languageDiagnostic);
            }

            translationErrorDetected(languageDiagnostic);
        }
    }


    void CppCodeGenerationEngine::compilerFinished(bool success) {
        backendWasSuccessful = success;
        backendSemaphore.release(1);
    }


    void CppCodeGenerationEngine::handleLinkerDiagnostic(const QString& diagnosticMessage) {
        CppCodeGeneratorDiagnostic* diagnostic = new CppCodeGeneratorDiagnostic(
            nullptr,
            CppCodeGeneratorDiagnostic::Type::FATAL_ERROR,
            dynamic_cast<const CppTranslationPhase&>(translationPhase()),
            CppCodeGeneratorDiagnostic::Code::LINKER_DIAGNOSTIC,
            diagnosticMessage,
            currentContext
        );

        translationErrorDetected(diagnostic);
    }


    void CppCodeGenerationEngine::linkerFinished(bool success) {
        backendWasSuccessful = success;
        backendSemaphore.release(1);
    }


    bool CppCodeGenerationEngine::missingTranslator(ElementPointer element) {
        const CppTranslationPhase& cppTranslationPhase = dynamic_cast<const CppTranslationPhase&>(translationPhase());

        CppCodeGeneratorDiagnostic* diagnostic = new CppCodeGeneratorDiagnostic(
            element,
            CppCodeGeneratorDiagnostic::Type::INTERNAL_ERROR,
            cppTranslationPhase,
            CppCodeGeneratorDiagnostic::Code::MISSING_TRANSLATOR
        );

        translationErrorDetected(diagnostic);
        return true;
    }


    QString CppCodeGenerationEngine::temporaryFilename(const QString& suffix) {
        if (!temporaryFile.isOpen()) {
            temporaryFile.open();
        }

        return temporaryFile.fileName() + suffix;
    }


    void CppCodeGenerationEngine::deleteFileIfExists(const QString& filename) {
        QFile file(filename);
        if (file.exists()) {
            file.remove();
        }
    }
}
