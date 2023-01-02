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
* This file implements the \ref Ld::FunctionData class.
***********************************************************************************************************************/

#include <QString>
#include <QList>
#include <QHash>

#include <cassert>

#include <model_api_types.h>

#include "ld_variable_name.h"
#include "ld_data_type.h"
#include "ld_capabilities.h"
#include "ld_element_structures.h"
#include "ld_function_variant.h"
#include "ld_function_data.h"
#include "ld_function_data_private.h"

namespace Ld {
    FunctionData::FunctionData() {}


    FunctionData::FunctionData(
            FunctionData::Type     functionType,
            const QString&         internalName,
            const QString&         primaryLibraryName,
            const VariableName&    userVisibleName,
            bool                   includeSubscriptParameter,
            const QString&         functionCommand,
            const QString&         description,
            const QString&         category,
            const QString&         helpData,
            bool                   requiresPerThreadInstance,
            const FunctionVariant& variant
        ) {
        configure(
            functionType,
            internalName,
            primaryLibraryName,
            userVisibleName,
            includeSubscriptParameter,
            functionCommand,
            description,
            category,
            helpData,
            requiresPerThreadInstance
        );

        impl->addVariant(variant);
    }


    FunctionData::FunctionData(
            FunctionData::Type     functionType,
            const QString&         internalName,
            const VariableName&    userVisibleName,
            bool                   includeSubscriptParameter,
            const QString&         functionCommand,
            const QString&         description,
            const QString&         category,
            const QString&         helpData,
            bool                   requiresPerThreadInstance,
            const FunctionVariant& variant
        ) {
        configure(
            functionType,
            internalName,
            QString(),
            userVisibleName,
            includeSubscriptParameter,
            functionCommand,
            description,
            category,
            helpData,
            requiresPerThreadInstance
        );

        impl->addVariant(variant);
    }


    FunctionData::FunctionData(
            FunctionData::Type         functionType,
            const QString&             internalName,
            const QString&             primaryLibraryName,
            const VariableName&        userVisibleName,
            bool                       includeSubscriptParameter,
            const QString&             functionCommand,
            const QString&             description,
            const QString&             category,
            const QString&             helpData,
            bool                       requiresPerThreadInstance,
            DataType::ValueType        returnValueType,
            QList<DataType::ValueType> parameterTypes,
            QList<QString>             parameterDescriptions
        ) {
        configure(
            functionType,
            internalName,
            primaryLibraryName,
            userVisibleName,
            includeSubscriptParameter,
            functionCommand,
            description,
            category,
            helpData,
            requiresPerThreadInstance
        );

        impl->addVariant(FunctionVariant(returnValueType, parameterTypes, parameterDescriptions));
    }


    FunctionData::FunctionData(
            FunctionData::Type         functionType,
            const QString&             internalName,
            const VariableName&        userVisibleName,
            bool                       includeSubscriptParameter,
            const QString&             functionCommand,
            const QString&             description,
            const QString&             category,
            const QString&             helpData,
            bool                       requiresPerThreadInstance,
            DataType::ValueType        returnValueType,
            QList<DataType::ValueType> parameterTypes,
            QList<QString>             parameterDescriptions
        ) {
        configure(
            functionType,
            internalName,
            QString(),
            userVisibleName,
            includeSubscriptParameter,
            functionCommand,
            description,
            category,
            helpData,
            requiresPerThreadInstance
        );

        impl->addVariant(FunctionVariant(returnValueType, parameterTypes, parameterDescriptions));
    }


    FunctionData::FunctionData(
            FunctionData::Type            functionType,
            const QString&                internalName,
            const VariableName&           userVisibleName,
            bool                          includeSubscriptParameter,
            const QString&                functionCommand,
            const QString&                description,
            const QString&                category,
            const QString&                helpData,
            bool                          requiresPerThreadInstance,
            const QList<FunctionVariant>& variants
        ) {
        configure(
            functionType,
            internalName,
            QString(),
            userVisibleName,
            includeSubscriptParameter,
            functionCommand,
            description,
            category,
            helpData,
            requiresPerThreadInstance
        );

        for (  QList<FunctionVariant>::const_iterator it = variants.constBegin(),end = variants.constEnd()
             ; it != end
             ; ++it
            ) {
            impl->addVariant(*it);
        }
    }


    FunctionData::FunctionData(
            FunctionData::Type            functionType,
            const QString&                internalName,
            const QString&                primaryLibraryName,
            const VariableName&           userVisibleName,
            bool                          includeSubscriptParameter,
            const QString&                functionCommand,
            const QString&                description,
            const QString&                category,
            const QString&                helpData,
            bool                          requiresPerThreadInstance,
            const QList<FunctionVariant>& variants
        ) {
        configure(
            functionType,
            internalName,
            primaryLibraryName,
            userVisibleName,
            includeSubscriptParameter,
            functionCommand,
            description,
            category,
            helpData,
            requiresPerThreadInstance
        );

        for (  QList<FunctionVariant>::const_iterator it = variants.constBegin(),end = variants.constEnd()
             ; it != end
             ; ++it
            ) {
            impl->addVariant(*it);
        }
    }


    FunctionData::FunctionData(const FunctionData& other):impl(other.impl) {}


    FunctionData::~FunctionData() {}


    bool FunctionData::isValid() const {
        return !impl.isNull();
    }


    bool FunctionData::isInvalid() const {
        return impl.isNull();
    }


    QString FunctionData::primaryLibraryName() const {
        return impl->primaryLibraryName();
    }


    QList<QString> FunctionData::allLibraries() const {
        return impl->allLibraries();
    }


    bool FunctionData::registerDefaultTranslator(
            const QString&                   generatorName,
            const FunctionTranslationEngine* defaultTranslator
        ) {
        return Private::registerDefaultTranslator(generatorName, defaultTranslator);
    }


    const FunctionTranslationEngine* FunctionData::defaultTranslator(const QString& generatorName) {
        return Private::defaultTranslator(generatorName);
    }


    bool FunctionData::registerTranslator(const QString& generatorName, const FunctionTranslationEngine* translator) {
        return impl->registerTranslator(generatorName, translator);
    }


    const FunctionTranslationEngine* FunctionData::translator(const QString& generatorName) const {
        return impl->translator(generatorName);
    }


    bool FunctionData::addVariant(const FunctionVariant& variant) {
        return impl->addVariant(variant);
    }


    QList<FunctionVariant> FunctionData::variants() const {
        return impl->variants();
    }


    FunctionData::Type FunctionData::functionType() const {
        return impl->functionType();
    }


    const QString& FunctionData::internalName() const {
        return impl->internalName();
    }


    const VariableName& FunctionData::userVisibleName() const {
        return impl->userVisibleName();
    }


    bool FunctionData::includeSubscriptParameter() const {
        return impl->includeSubscriptParameter();
    }


    const QString& FunctionData::functionCommand() const {
        return impl->functionCommand();
    }


    const QString& FunctionData::description() const {
        return impl->description();
    }


    const QString& FunctionData::category() const {
        return impl->category();
    }


    const QString& FunctionData::helpData() const {
        return impl->helpData();
    }


    bool FunctionData::requiresPerThreadInstance() const {
        return impl->requiresPerThreadInstance();
    }


    unsigned FunctionData::minimumNumberParameters() const {
        return impl->minimumNumberParameters();
    }


    unsigned FunctionData::maximumNumberParameters() const {
        return impl->maximumNumberParameters();
    }


    FunctionVariant FunctionData::bestFitVariant(
            const Capabilities&               requiredCapabilities,
            const QList<DataType::ValueType>& knownParameterList
        ) const {
        return impl->bestFitVariant(requiredCapabilities, knownParameterList);
    }


    QList<FunctionVariant> FunctionData::allCompatibleVariants(
            const Capabilities&               requiredCapabiltiies,
            const QList<DataType::ValueType>& knownParameterList
        ) const {
        return impl->allCompatibleVariants(requiredCapabiltiies, knownParameterList);
    }


    Capabilities FunctionData::childCapabilities() const {
        return impl->childCapabilities();
    }


    Capabilities FunctionData::parentRequires(
            unsigned                          parameterIndex,
            const Capabilities&               requiredCapabilities,
            const QList<DataType::ValueType>& knownParameterList
        ) const {
        return impl->parentRequires(parameterIndex, requiredCapabilities, knownParameterList);
    }


    QList<Capabilities> FunctionData::parentRequires(
            const Capabilities&               requiredCapabilities,
            const QList<DataType::ValueType>& knownParameterList
        ) const {
        return impl->parentRequires(requiredCapabilities, knownParameterList);
    }


    FunctionData& FunctionData::operator=(const FunctionData& other) {
        impl = other.impl;
        return *this;
    }


    bool FunctionData::operator==(const FunctionData& other) const {
        return impl->operator==(*other.impl);
    }


    bool FunctionData::operator!=(const FunctionData& other) const {
        return impl->operator!=(*other.impl);
    }


    bool FunctionData::operator<(const FunctionData& other) const {
        return impl->operator<(*other.impl);
    }


    bool FunctionData::operator>(const FunctionData& other) const {
        return impl->operator>(*other.impl);
    }


    bool FunctionData::operator<=(const FunctionData& other) const {
        return impl->operator<=(*other.impl);
    }


    bool FunctionData::operator>=(const FunctionData& other) const {
        return impl->operator>=(*other.impl);
    }


    void FunctionData::configure(
            FunctionData::Type  functionType,
            const QString&      internalName,
            const QString&      libraryName,
            const VariableName& userVisibleName,
            bool                includeSubscriptParameter,
            const QString&      functionCommand,
            const QString&      description,
            const QString&      category,
            const QString&      helpData,
            bool                requiresPerThreadInstance
        ) {
        impl.reset(new Private(
            functionType,
            internalName,
            libraryName,
            userVisibleName,
            includeSubscriptParameter,
            functionCommand,
            description,
            category,
            helpData,
            requiresPerThreadInstance
        ));
    }


    void FunctionData::buildParameterData(
            QList<DataType::ValueType>& parameterTypesList,
            QList<QString>&             parameterDescriptionList,
            DataType::ValueType         parameterType,
            const QString&              parameterDescription
        ) {
        parameterTypesList.append(parameterType);
        parameterDescriptionList.append(parameterDescription);
    }
}
