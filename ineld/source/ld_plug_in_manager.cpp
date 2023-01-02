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
* This file implements the \ref Ld::PlugInManager class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QString>
#include <QCoreApplication>
#include <QDir>
#include <QStringList>
#include <QFileInfo>
#include <QStringList>
#include <QFileInfoList>
#include <QLibrary>
#include <QFunctionPointer>
#include <QMap>

#include <iostream> // For console error messages.

#include "user_function_definition.h"
#include "run_time_library_definition.h"
#include "plug_in_data.h"
#include "ld_cpp_declaration_payload.h"
#include "ld_function_database.h"
#include "ld_function_data.h"
#include "ld_function_variant.h"
#include "ld_plug_in_registrar.h"
#include "ld_plug_in_information.h"
#include "ld_plug_in_manager.h"

#define PLUG_IN_STRUCTURE_NAME ("__plugInData")

namespace Ld {
    PlugInManager::PlugInManager() {}


    PlugInManager::~PlugInManager() {
        unloadAll();
    }


    bool PlugInManager::loadPlugIns(const QList<QString>& plugInFiles, PlugInRegistrar* registrar) {
        bool success = true;

        QList<QString>::const_iterator plugInIterator    = plugInFiles.constBegin();
        QList<QString>::const_iterator plugInEndIterator = plugInFiles.constEnd();

        while (plugInIterator != plugInEndIterator) {
            QString       filename           = *plugInIterator;
            ::PlugInData* plugInData         = nullptr;
            bool          isThirdPartyPlugIn = true;

            aboutToLoad(filename);

            QLibrary* library            = new QLibrary(filename);
            bool      successThisLibrary = library->load();

            if (successThisLibrary) {
                plugInData = reinterpret_cast<::PlugInData*>(library->resolve(PLUG_IN_STRUCTURE_NAME));
                if (plugInData == nullptr) {
                    successThisLibrary = false;
                } else {
                    if (loadedPlugInsByName.contains(QString(plugInData->name))) {
                        successThisLibrary = false;
                    }
                }
            }

            if (successThisLibrary) {
                successThisLibrary = registerFunctions(
                    plugInData->numberFunctionDefinitions,
                    plugInData->userFunctionDefinitions
                );
            }

            if (successThisLibrary) {
                PlugInRegistrar::InitializationFunction
                    initializationFunction = reinterpret_cast<PlugInRegistrar::InitializationFunction>(
                        library->resolve(PLUG_IN_INITIALZATION_FUNCTION_NAME)
                    );

                if (initializationFunction != nullptr) {
                    isThirdPartyPlugIn = false;
                    (*initializationFunction)(registrar);

                    successThisLibrary = registrar->successful();
                }
            }

            if (successThisLibrary) {
                successThisLibrary = buildFunctionDeclarations(plugInData);
            }

            if (successThisLibrary) {
                PlugInInformation plugInInformation(plugInData, isThirdPartyPlugIn);
                if (plugInInformation.isValid()) {
                    loadedPlugInsByName.insert(plugInInformation.name(), plugInInformation);
                    plugInLoaded(filename, plugInInformation);
                } else {
                    successThisLibrary = false;
                }
            }

            if (!successThisLibrary) {
                if (library->isLoaded()) {
                    library->unload();
                }

                plugInFailedToLoad(filename);
                success = false;
            }

            ++plugInIterator;
        }

        return success;
    }


    const PlugInsByName& PlugInManager::plugInsByName() const {
        return loadedPlugInsByName;
    }


    void PlugInManager::aboutToLoad(const QString& /* loadPath */) {}


    void PlugInManager::plugInLoaded(const QString& /* loadPath */, PlugInInformation /* plugInInformation */) {}


    void PlugInManager::plugInFailedToLoad(const QString& /* loadPath */) {}


    void PlugInManager::unloadAll() {
        QList<QLibrary*>::iterator it  = loadedLibraryInstances.begin();
        QList<QLibrary*>::iterator end = loadedLibraryInstances.end();

        while (it != end) {
            QLibrary* library = *it;

            PlugInRegistrar::DestructionFunction
                destructionFunction = library->resolve(PLUG_IN_DESTRUCTOR_FUNCTION_NAME);

            if (destructionFunction != nullptr) {
                (*destructionFunction)();
            }

            library->unload();

            ++it;
        }
    }


    bool PlugInManager::registerFunctions(
            unsigned                        numberFunctionDefinitions,
            const ::UserFunctionDefinition* userFunctionDefinitions
        ) {
        bool     success       = true;
        unsigned functionIndex = 0;

        while (success && functionIndex < numberFunctionDefinitions) {
            const UserFunctionDefinition& userFunctionDefinition = userFunctionDefinitions[functionIndex];

            success = (
                   userFunctionDefinition.internalName != nullptr
                && userFunctionDefinition.runtimeLibraryName != nullptr
                && userFunctionDefinition.visibleName != nullptr
            );

            if (success) {
                QString internalName(userFunctionDefinition.internalName);
                QString runtimeLibraryName(userFunctionDefinition.runtimeLibraryName);
                QString visibleName(userFunctionDefinition.visibleName);

                if (!internalName.isEmpty() && !runtimeLibraryName.isEmpty() && !visibleName.isEmpty()) {
                    unsigned numberVariants = userFunctionDefinition.numberVariants;
                    if (numberVariants > 0) {
                        QString visibleNameSubscript = QString::fromUtf8(
                              userFunctionDefinition.visibleNameSubscript == nullptr
                            ? ""
                            : userFunctionDefinition.visibleNameSubscript
                        );

                        QString command = QString::fromUtf8(
                              userFunctionDefinition.command == nullptr
                            ? ""
                            : userFunctionDefinition.command
                        );

                        QString description(
                              userFunctionDefinition.description == nullptr
                            ? tr("User defined")
                            : QString::fromUtf8(userFunctionDefinition.description)
                        );

                        QString category = QString::fromUtf8(
                              userFunctionDefinition.category == nullptr
                            ? ""
                            : userFunctionDefinition.category
                        );

                        if (category.isEmpty()) {
                            category = tr("User Defined");
                        }

                        bool    requiresPerThreadInstance = userFunctionDefinition.requiresRNG;
                        bool    subscriptedFirstParameter = userFunctionDefinition.subscriptedFirstParameter;

                        QList<FunctionVariant> functionVariants;
                        for (unsigned variantIndex=0 ; variantIndex<numberVariants ; ++variantIndex) {
                            const UserFunctionVariant&
                                userFunctionVariant = userFunctionDefinition.variants[variantIndex];

                            QList<DataType::ValueType> parameterTypes;
                            QList<QString>             parameterDescriptions;

                            unsigned numberParameters = userFunctionVariant.numberParameters;
                            for (unsigned parameterIndex=0 ; parameterIndex<numberParameters ; ++parameterIndex) {
                                const UserFunctionParameter&
                                    parameter = userFunctionVariant.parameters[parameterIndex];

                                parameterTypes.append(parameter.valueType);
                                parameterDescriptions.append(QString(parameter.description));
                            }

                            functionVariants.append(
                                FunctionVariant(
                                    userFunctionVariant.returnValueType,
                                    parameterTypes,
                                    parameterDescriptions
                                )
                            );
                        }

                        FunctionData functionData(
                            FunctionData::Type::USER_DEFINED,
                            internalName,
                            runtimeLibraryName,
                            VariableName(visibleName, visibleNameSubscript),
                            subscriptedFirstParameter,
                            command,
                            description,
                            category,
                            QString(),
                            requiresPerThreadInstance,
                            functionVariants
                        );

                        success = FunctionDatabase::registerFunction(functionData);
                    } else {
                        success = false;
                    }
                } else {
                    success = false;
                }
            }

            ++functionIndex;
        }

        return success;
    }


    bool PlugInManager::buildFunctionDeclarations(const ::PlugInData* plugInData) {
        bool success = true;

        if (plugInData->runTimeLibraryDefinition != nullptr) {
            const RunTimeLibraryDefinition* libraryDefinition = plugInData->runTimeLibraryDefinition;
            while (success && libraryDefinition != nullptr && libraryDefinition->libraryName != nullptr) {
                LibraryType libraryType = libraryDefinition->libraryType;
                if (libraryType == LibraryType::CUSTOMER_DYNAMIC_LIBRARY ||
                    libraryType == LibraryType::CUSTOMER_STATIC_LIBRARY     ) {
                    const char*                   cLibraryName    = libraryDefinition->libraryName;
                    QString                       libraryName     = QString::fromLocal8Bit(cLibraryName);
                    unsigned                      numberFunctions = plugInData->numberFunctionDefinitions;
                    const UserFunctionDefinition* functionData    = plugInData->userFunctionDefinitions;

                    QString libraryDeclarations;

                    for (unsigned functionIndex=0 ; functionIndex<numberFunctions ; ++functionIndex) {
                        if (QString::fromLocal8Bit(functionData->runtimeLibraryName) == libraryName) {
                            const char*                cInternalName   = functionData->internalName;
                            QString                    internalName    = QString::fromLocal8Bit(cInternalName);
                            unsigned                   numberVariants = functionData->numberVariants;
                            const UserFunctionVariant* variant        = functionData->variants;

                            for (unsigned variantIndex=0 ; variantIndex<numberVariants ; ++variantIndex) {
                                Model::ValueType             returnType       = variant->returnValueType;
                                unsigned                     numberParameters = variant->numberParameters;
                                const UserFunctionParameter* parameter        = variant->parameters;

                                QString declaration;
                                #if (defined(Q_OS_WIN))

                                    if (libraryType == LibraryType::CUSTOMER_DYNAMIC_LIBRARY) {
                                        declaration = QString("__declspec(dllimport) ");
                                    }

                                #elif (!defined(Q_OS_LINUX) && !defined(Q_OS_DARWIN))

                                    #error Unknown platform

                                #endif

                                declaration += userTypeFromValue(returnType, false) + " " + internalName + "(";

                                if (functionData->requiresRNG) {
                                    declaration += "Model::Rng&";
                                }

                                for (unsigned parameterIndex=0 ; parameterIndex<numberParameters ; ++parameterIndex) {
                                    if (parameterIndex != 0 || functionData->requiresRNG) {
                                        declaration += ",";
                                    }

                                    declaration += userTypeFromValue(parameter->valueType);
                                    ++parameter;
                                }

                                declaration += ");\n";
                                libraryDeclarations += declaration;

                                ++variant;
                            }
                        }

                        ++functionData;
                    }

                    customerDeclarationPayloads.append(libraryDeclarations.toLocal8Bit());
                    const QByteArray& customerDeclarationPayload = customerDeclarationPayloads.last();
                    const char*       rawPayload = customerDeclarationPayload.data();
                    success = CppDeclarationPayload::registerPayload(
                        CppDeclarationPayload(
                            libraryName,
                            reinterpret_cast<const std::uint8_t*>(rawPayload),
                            static_cast<unsigned long>(strlen(rawPayload))
                        )
                    );
                }

                ++libraryDefinition;
            }
        }

        return success;
    }


    QString PlugInManager::userTypeFromValue(Model::ValueType valueType, bool asParameter) {
        QString result;

        switch (valueType) {
            case Model::ValueType::NONE: {
                break;
            }

            case Model::ValueType::VARIANT: {
                result = asParameter ? QString("const Model::Variant&") : QString("Model::Variant");
                break;
            }

            case Model::ValueType::BOOLEAN: {
                result = QString("Model::Boolean");
                break;
            }

            case Model::ValueType::INTEGER: {
                result = QString("Model::Integer");
                break;
            }

            case Model::ValueType::REAL: {
                result = QString("Model::Real");
                break;
            }

            case Model::ValueType::COMPLEX: {
                result = asParameter ? QString("const Model::Complex&") : QString("Model::Complex");
                break;
            }

            case Model::ValueType::SET: {
                result = asParameter ? QString("const Model::Set&") : QString("Model::Set");
                break;
            }

            case Model::ValueType::TUPLE: {
                result = asParameter ? QString("const Model::Tuple&") : QString("Model::Tuple");
                break;
            }

            case Model::ValueType::MATRIX_BOOLEAN: {
                result = asParameter ? QString("const Model::MatrixBoolean&") : QString("Model::MatrixBoolean");
                break;
            }

            case Model::ValueType::MATRIX_INTEGER: {
                result = asParameter ? QString("const Model::MatrixInteger&") : QString("Model::MatrixInteger");
                break;
            }

            case Model::ValueType::MATRIX_REAL: {
                result = asParameter ? QString("const Model::MatrixReal&") : QString("Model::MatrixReal");
                break;
            }

            case Model::ValueType::MATRIX_COMPLEX: {
                result = asParameter ? QString("const Model::MatrixComplex&") : QString("Model::MatrixComplex");
                break;
            }

            default: {
                Q_ASSERT(false);
                break;
            }
        }

        return result;
    }
}
