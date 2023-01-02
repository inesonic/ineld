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
#include <QMap>
#include <QSet>

#include <cassert>
#include <limits>

#include <model_api_types.h>

#include "ld_variable_name.h"
#include "ld_data_type.h"
#include "ld_capabilities.h"
#include "ld_element_structures.h"
#include "ld_function_variant.h"
#include "ld_function_translation_engine.h"
#include "ld_function_data.h"
#include "ld_function_data_private.h"

namespace Ld {
    QHash<QString, const FunctionTranslationEngine*> FunctionData::Private::currentDefaultTranslators;

    FunctionData::Private::Private(
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
        ):currentFunctionType(
            functionType
        ),currentInternalName(
            internalName
        ),currentPrimaryLibraryName(
            libraryName
        ),currentUserVisibleName(
            userVisibleName
        ),currentIncludeSubscriptParameter(
            includeSubscriptParameter
        ),currentFunctionCommand(
            functionCommand
        ),currentDescription(
            description
        ),currentCategory(
            category
        ),currentHelpData(
            helpData
        ),currentRequiresPerThreadInstance(
            requiresPerThreadInstance
        ) {
        currentMinimumNumberParameters = std::numeric_limits<unsigned>::max();
        currentMaximumNumberParameters = 0;
    }


    FunctionData::Private::~Private() {}


    bool FunctionData::Private::registerDefaultTranslator(
            const QString&                   generatorName,
            const FunctionTranslationEngine* defaultTranslator
        ) {
        bool result;

        if (!currentDefaultTranslators.contains(generatorName)) {
            currentDefaultTranslators.insert(generatorName, defaultTranslator);
            result = true;
        } else {
            result = false;
        }

        return result;
    }


    const FunctionTranslationEngine* FunctionData::Private::defaultTranslator(const QString& generatorName) {
        return currentDefaultTranslators.value(generatorName, Q_NULLPTR);
    }


    bool FunctionData::Private::registerTranslator(
            const QString&                   generatorName,
            const FunctionTranslationEngine* translator
        ) {
        bool result;

        if (!currentTranslators.contains(generatorName)) {
            currentTranslators.insert(generatorName, translator);
            result = true;
        } else {
            result = false;
        }

        return result;
    }


    const FunctionTranslationEngine* FunctionData::Private::translator(const QString& generatorName) const {
        const FunctionTranslationEngine* result = currentTranslators.value(generatorName, Q_NULLPTR);

        if (result == Q_NULLPTR) {
            result = currentDefaultTranslators.value(generatorName);
        }

        return result;
    }


    QString FunctionData::Private::primaryLibraryName() const {
        return currentPrimaryLibraryName;
    }


    QList<QString> FunctionData::Private::allLibraries() const {
        QSet<QString> libraries;

        for (  QMap<FunctionVariant, Capabilities>::const_iterator it  = currentVariants.constBegin(),
                                                                   end = currentVariants.constEnd()
             ; it != end
             ; ++it
            ) {
            QString libraryName = it.key().libraryName();
            if (!libraryName.isEmpty()) {
                libraries << libraryName;
            }
        }

        QList<QString> result;
        if (!currentPrimaryLibraryName.isEmpty()) {
            libraries.remove(currentPrimaryLibraryName);
            result = libraries.values();
            result.prepend(currentPrimaryLibraryName);
        } else {
            result = libraries.values();
        }

        return result;
    }


    bool FunctionData::Private::addVariant(const FunctionVariant& variant) {
        bool result;

        if (!currentVariants.contains(variant)) {
            currentVariants.insert(variant, variant.variantCapabilities());

            unsigned numberParameters = variant.numberParameters();

            if (numberParameters < currentMinimumNumberParameters) {
                currentMinimumNumberParameters = numberParameters;
            }

            if (numberParameters > currentMaximumNumberParameters) {
                currentMaximumNumberParameters = numberParameters;
            }

            allSupportedCapabilities |= variant.variantCapabilities();

            result = true;
        } else {
            result = false;
        }

        return result;
    }


    QList<FunctionVariant> FunctionData::Private::variants() const {
        return currentVariants.keys();
    }


    FunctionData::Type FunctionData::Private::functionType() const {
        return currentFunctionType;
    }


    const QString& FunctionData::Private::internalName() const {
        return currentInternalName;
    }


    const VariableName& FunctionData::Private::userVisibleName() const {
        return currentUserVisibleName;
    }


    bool FunctionData::Private::includeSubscriptParameter() const {
        return currentIncludeSubscriptParameter;
    }


    const QString& FunctionData::Private::functionCommand() const {
        return currentFunctionCommand;
    }


    const QString& FunctionData::Private::description() const {
        return currentDescription;
    }


    const QString& FunctionData::Private::category() const {
        return currentCategory;
    }


    const QString& FunctionData::Private::helpData() const {
        return currentHelpData;
    }


    bool FunctionData::Private::requiresPerThreadInstance() const {
        return currentRequiresPerThreadInstance;
    }


    unsigned FunctionData::Private::minimumNumberParameters() const {
        return currentMinimumNumberParameters;
    }


    unsigned FunctionData::Private::maximumNumberParameters() const {
        return currentMaximumNumberParameters;
    }


    FunctionVariant FunctionData::Private::bestFitVariant(
            const Capabilities&               requiredCapabilities,
            const QList<DataType::ValueType>& knownParameterList
        ) const {
        FunctionVariant result;

        unsigned knownParameterListSize = static_cast<unsigned>(knownParameterList.size());
        unsigned bestNumberMismatches   = static_cast<unsigned>(-1);

        QMap<FunctionVariant, Capabilities>::const_iterator it  = currentVariants.constBegin();
        QMap<FunctionVariant, Capabilities>::const_iterator end = currentVariants.constEnd();
        while (it != end && bestNumberMismatches != 0) {
            const FunctionVariant& variant = it.key();
            if (variant.variantCapabilities().intersects(requiredCapabilities)) {
                unsigned variantSize       = variant.numberParameters();
                unsigned parametersToCheck = std::min(variantSize, knownParameterListSize);
                unsigned numberMismatches  = 2 * (std::max(variantSize, knownParameterListSize) - parametersToCheck);

                const QList<DataType::ValueType>& parameterList = variant.parameterTypes();
                for (unsigned parameterIndex=0 ; parameterIndex<parametersToCheck ; ++parameterIndex) {
                    DataType::ValueType functionValueType = parameterList.at(parameterIndex);
                    DataType::ValueType queryValueType    = knownParameterList.at(parameterIndex);

                    if (functionValueType != queryValueType) {
                        ++numberMismatches;
                    }
                }

                if (numberMismatches < bestNumberMismatches) {
                    result               = variant;
                    bestNumberMismatches = numberMismatches;
                }
            }

            ++it;
        }

        return result;
    }


    QList<FunctionVariant> FunctionData::Private::allCompatibleVariants(
            const Capabilities&               requiredCapabilities,
            const QList<DataType::ValueType>& knownParameterList
        ) const {
        QList<FunctionVariant> result;

        QMap<FunctionVariant, Capabilities>::const_iterator it  = currentVariants.constBegin();
        QMap<FunctionVariant, Capabilities>::const_iterator end = currentVariants.constEnd();

        unsigned knownParameterListSize = static_cast<unsigned>(knownParameterList.size());

        while (it != end) {
            const FunctionVariant& variant = it.key();
            if (variant.variantCapabilities().intersects(requiredCapabilities)) {
                unsigned variantSize       = variant.numberParameters();
                unsigned parametersToCheck = std::min(variantSize, knownParameterListSize);
                unsigned parameterIndex    = 0;

                const QList<DataType::ValueType>& parameterList = variant.parameterTypes();
                while (parameterIndex < parametersToCheck                                              &&
                       (knownParameterList.at(parameterIndex) == DataType::ValueType::NONE        ||
                        parameterList.at(parameterIndex) == knownParameterList.at(parameterIndex)    )    ) {
                    ++parameterIndex;
                }

                if (parameterIndex >= parametersToCheck) {
                    result.append(variant);
                }
            }

            ++it;
        }

        return result;
    }


    Capabilities FunctionData::Private::childCapabilities() const {
        return allSupportedCapabilities;
    }


    Capabilities FunctionData::Private::parentRequires(
            unsigned                          parameterIndex,
            const Capabilities&               requiredCapabilities,
            const QList<DataType::ValueType>& knownParameterList
        ) const {
        Capabilities result;

        QMap<FunctionVariant, Capabilities>::const_iterator it  = currentVariants.constBegin();
        QMap<FunctionVariant, Capabilities>::const_iterator end = currentVariants.constEnd();

        unsigned knownParameterListSize = static_cast<unsigned>(knownParameterList.size());

        while (it != end) {
            const FunctionVariant& variant = it.key();
            if (variant.variantCapabilities().intersects(requiredCapabilities)) {
                unsigned variantSize       = variant.numberParameters();
                unsigned parametersToCheck = std::min(variantSize, knownParameterListSize);
                unsigned index    = 0;

                const QList<DataType::ValueType>& parameterList = variant.parameterTypes();
                while (index < parametersToCheck                                       &&
                       (knownParameterList.at(index) == DataType::ValueType::NONE ||
                        parameterList.at(index) == knownParameterList.at(index)      )    ) {
                    ++index;
                }

                if (index >= parametersToCheck) {
                    result |= variant.parameterCapabilities().at(parameterIndex);
                }
            }

            ++it;
        }

        return result;
    }


    QList<Capabilities> FunctionData::Private::parentRequires(
            const Capabilities&               requiredCapabilities,
            const QList<DataType::ValueType>& knownParameterList
        ) const {
        QList<Capabilities> result;

        QMap<FunctionVariant, Capabilities>::const_iterator it  = currentVariants.constBegin();
        QMap<FunctionVariant, Capabilities>::const_iterator end = currentVariants.constEnd();

        unsigned knownParameterListSize = static_cast<unsigned>(knownParameterList.size());

        while (it != end) {
            const FunctionVariant& variant = it.key();
            if (variant.variantCapabilities().intersects(requiredCapabilities)) {
                unsigned variantSize       = variant.numberParameters();
                unsigned parametersToCheck = std::min(variantSize, knownParameterListSize);
                unsigned parameterIndex    = 0;

                const QList<DataType::ValueType>& parameterList = variant.parameterTypes();
                while (parameterIndex < parametersToCheck                                              &&
                       (knownParameterList.at(parameterIndex) == DataType::ValueType::NONE        ||
                        parameterList.at(parameterIndex) == knownParameterList.at(parameterIndex)    )    ) {
                    ++parameterIndex;
                }

                if (parameterIndex >= parametersToCheck) {
                    unsigned numberExistingValues = static_cast<unsigned>(result.size());
                    unsigned numberToUpdate       = std::min(numberExistingValues, variantSize);

                    const QList<Capabilities> variantCapabilities = variant.parameterCapabilities();
                    for (unsigned index=0 ; index<numberToUpdate ; ++index) {
                        result[index] |= variantCapabilities.at(index);
                    }

                    if (variantSize > numberExistingValues) {
                        for (unsigned index=numberExistingValues ; index<variantSize ; ++index) {
                            result.append(variantCapabilities.at(index));
                        }
                    }
                }
            }

            ++it;
        }

        return result;
    }


    bool FunctionData::Private::operator==(const FunctionData::Private& other) const {
        return currentUserVisibleName == other.currentUserVisibleName;
    }


    bool FunctionData::Private::operator!=(const FunctionData::Private& other) const {
        return currentUserVisibleName != other.currentUserVisibleName;
    }


    bool FunctionData::Private::operator<(const FunctionData::Private& other) const {
        return currentUserVisibleName < other.currentUserVisibleName;
    }


    bool FunctionData::Private::operator>(const FunctionData::Private& other) const {
        return currentUserVisibleName > other.currentUserVisibleName;
    }


    bool FunctionData::Private::operator<=(const FunctionData::Private& other) const {
        return currentUserVisibleName <= other.currentUserVisibleName;
    }


    bool FunctionData::Private::operator>=(const FunctionData::Private& other) const {
        return currentUserVisibleName >= other.currentUserVisibleName;
    }
}
