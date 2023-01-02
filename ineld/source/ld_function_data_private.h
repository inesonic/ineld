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
* This header defines the \ref Ld::FunctionData::Private class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_FUNCTION_DATA_PRIVATE_H
#define LD_FUNCTION_DATA_PRIVATE_H

#include <QString>
#include <QList>
#include <QHash>
#include <QMap>

#include "ld_common.h"
#include "ld_variable_name.h"
#include "ld_data_type.h"
#include "ld_capabilities.h"
#include "ld_element_structures.h"
#include "ld_function_data.h"

namespace Ld {
    class FunctionTranslationEngine;
    class FunctionVariant;
    class DataTypeTranslator;
    class DataTypeDecoder;

    /**
     * Private implemetnation of the \ref Ld::FunctionData class.
     */
    class LD_PUBLIC_API FunctionData::Private {
        public:
            /**
             * Constructor
             *
             * \param[in] functionType              The type of function being defined.
             *
             * \param[in] internalName              The internal name for the function.  This name is typically used by
             *                                      the code translation engines.
             *
             * \param[in] primaryLibraryName        The name of the library containing the function implementation.  An
             *                                      empty string indicates that this function is internally defined.
             *
             * \param[in] userVisibleName           The function name as entered/seen by the user.
             *
             * \param[in] includeSubscriptParameter If true, the function should include a subscript for the first
             *                                      parameter.
             *
             * \param[in] functionCommand           An optional command used to enter the function.
             *
             * \param[in] description               A description for the function.
             *
             * \param[in] category                  An open-ended category name for the function.
             *
             * \param[in] helpData                  A string that specifies where to look for information on this
             *                                      function.  The meaning of this string is open ended.
             *
             * \param[in] requiresPerThreadInstance If true, this function requires an instance of M::PerThread as the
             *                                      first parameter.
             */
            Private(
                Type                functionType,
                const QString&      internalName,
                const QString&      primaryLibraryName,
                const VariableName& userVisibleName,
                bool                includeSubscriptParameter,
                const QString&      functionCommand,
                const QString&      description,
                const QString&      category,
                const QString&      helpData,
                bool                requiresPerThreadInstance
            );

            ~Private();

            /**
             * Method you can use to register a default function translator for a translation engine.
             *
             * \param[in] generatorName     The name of the generator.
             *
             * \param[in] defaultTranslator A pointer to the default translator.  This translator is expected to be
             *                              statically defined and will not be deleted.
             *
             * \return Returns true on success, returns false if a translator has already been registered for the
             *         indicated generator.
             */
            static bool registerDefaultTranslator(
                const QString&                   generatorName,
                const FunctionTranslationEngine* defaultTranslator
            );

            /**
             * Method you can use to obtain the default function translator for a translation engine.
             *
             * \param[in] generatorName The name of the desired generator.
             *
             * \return Returns a pointer to the default translator.  A null pointer is returned if no default generator
             *         has been defined for the function.
             */
            static const FunctionTranslationEngine* defaultTranslator(const QString& generatorName);

            /**
             * Method you can use to register a function specific translator for a translation engine.  The specified
             * translator will be used in leu of the default translator for all variants of this function.
             *
             * \param[in] generatorName The name of the generator.
             *
             * \param[in] translator    A pointer to the default translator.  This translator is expected to be
             *                          statically defined and will not be deleted.
             *
             * \return Returns true on success, returns false if a translator has already been registered for the
             *         indicated generator.
             */
            bool registerTranslator(const QString& generatorName, const FunctionTranslationEngine* translator);

            /**
             * Method you can use to obtain the translator to be used for this function.
             *
             * \param[in] generatorName The name of the generator.
             *
             * \return Returns a pointer to the translator to be used for this function.
             */
            const FunctionTranslationEngine* translator(const QString& generatorName) const;

            /**
             * Method you can use to obtain the parimary library tied to this function.
             *
             * \return Returns the name of the primary library tied to this function.
             */
            QString primaryLibraryName() const;

            /**
             * Method you can use to obtain all the libraries tied to this function.
             *
             * \return Returns a list of all the libraries tied to this function.
             */
            QList<QString> allLibraries() const;

            /**
             * Method you can call to define a new variant for this function.
             *
             * \param[in] variant The new variant to be registered with this function.
             *
             * \return Returns true on success, returns false if the variant is already registered with the function.
             */
            bool addVariant(const FunctionVariant& variant);

            /**
             * Method you can call to obtain a list of all variants for this function.
             *
             * \return Returns a list of all the variants for this function.
             */
            QList<FunctionVariant> variants() const;

            /**
             * Method you can use to determine the function type.
             *
             * \return Returns the function type.
             */
            Type functionType() const;

            /**
             * Method you can use to determine the internal name of the function.
             *
             * \return Returns the internal name of the function.
             */
            const QString& internalName() const;

            /**
             * Method you can use to determine the library name of the library implementating this function.
             *
             * \return Returns the library that implements this function.
             */
            const QString& libraryName() const;

            /**
             * Method you can use to determine the user visible name of the funciton.
             *
             * \return Returns the user visible name of the function.
             */
            const VariableName& userVisibleName() const;

            /**
             * Method you can use to determine if this function type include an initial subscript parameter.
             *
             * \return Returns true if this function should include an initial subscript parameter.
             */
            bool includeSubscriptParameter() const;

            /**
             * Method you can use to determine the command used to rapidly insert this function.
             *
             * \return Returns the command the user can use to insert this function.
             */
            const QString& functionCommand() const;

            /**
             * Method you can use to determine the description for the function.
             *
             * \return Returns a description of the function.
             */
            const QString& description() const;

            /**
             * Method you can use to determine the category for the function.
             *
             * \return Returns the category for the function.
             */
            const QString& category() const;

            /**
             * Method you can use to obtain the help data for the function.
             *
             * \return Returns the help data for the function.
             */
            const QString& helpData() const;

            /**
             * Method you can use to determine if this function requires a per-thread instance.
             *
             * \return Returns true if the code generation engines should supply a per-thread instance for random
             *         number generation and other thread-aware tasks.
             */
            bool requiresPerThreadInstance() const;

            /**
             * Method you can use to determine the minimum number of function parameters.
             *
             * \return Returns the minimum number of function parameters.  This value will exclude the per-thread
             *         instance that can optionally be supplied as the first parameter.
             */
            unsigned minimumNumberParameters() const;

            /**
             * Method you can use to determine the maximum number of function parameters.
             *
             * \return Returns the maximum number of function parameters.
             */
            unsigned maximumNumberParameters() const;

            /**
             * Method you can use to obtain the best fit (simplest) function variant meeting a given set of constraints.
             *
             * \param[in] requiredCapabiltiies The required capabilities for the function.
             *
             * \param[in] knownParameterList   A list holding the known parameter values.
             *
             * \return Returns the best fit function variant.
             */
            FunctionVariant bestFitVariant(
                const Capabilities&               requiredCapabiltiies,
                const QList<DataType::ValueType>& knownParameterList
            ) const;

            /**
             * Method you can use to obtain a sorted list of all variants that meet a parameter list.
             *
             * \param[in] requiredCapabiltiies The required capabilities for the function.
             *
             * \param[in] knownParameterList   A list holding the known parameter values.  Entries with the value
             *                                 Ld::DataType::ValueType::NONE are ignored.  Entries not defined by this
             *                                 list are also ignored.
             *
             * \return Returns a sorted list holding all variants that are compatible with the provided list.
             */
            QList<FunctionVariant> allCompatibleVariants(
                const Capabilities&               requiredCapabiltiies,
                const QList<DataType::ValueType>& knownParameterList
            ) const;

            /**
             * Method you can call to obtain the reported capabilities of this function based on the return values of
             * the variants.
             *
             * \return Returns the capabilities of this function.
             */
            Capabilities childCapabilities() const;

            /**
             * Method you can call to obtain the required capabilities for a specific parameter of this function.
             *
             * \param[in] parameterIndex       The zero based parameter index.
             *
             * \param[in] requiredCapabilities The required capabilities for the function.
             *
             * \param[in] knownParameterList   A list of the known values types for the currently populated parameters.
             *
             * \return Returns the capabilities for the requested parameter.
             */
            Capabilities parentRequires(
                unsigned                          parameterIndex,
                const Capabilities&               requiredCapabilities,
                const QList<DataType::ValueType>& knownParameterList
            ) const;

            /**
             * Method you can call to obtain the required capabilities for all parameters.
             *
             * \param[in] requiredCapabilities The required capabilities for the function.
             *
             * \param[in] knownParameterList   A list of the known values types for the currently populated parameters.
             *
             * \return Returns a list of required capabilities for all parameters.
             */
            QList<Capabilities> parentRequires(
                const Capabilities&               requiredCapabilities,
                const QList<DataType::ValueType>& knownParameterList
            ) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instances are equal.  Returns false if the instances are not equal.
             */
            bool operator==(const Private& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instances are not equal.  Returns false if the instances are equal.
             */
            bool operator!=(const Private& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if this instance should precede the other instance.  Returns false if this instance
             *         is equal to or follows the other instance.
             */
            bool operator<(const Private& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if this instance should follow the other instance.  Returns false if this instance
             *         is equal to or precedes the other instance.
             */
            bool operator>(const Private& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if this instance is equal to or should precede the other instance.  Returns false
             *         if this instance follows the other instance.
             */
            bool operator<=(const Private& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if this instance is equal to or should follow the other instance.  Returns false if
             *         this instance precedes the other instance.
             */
            bool operator>=(const Private& other) const;

        private:
            /**
             * The default translators by generator name.
             */
            static QHash<QString, const FunctionTranslationEngine*> currentDefaultTranslators;

            /**
             * Overridden map of translators to be used to translate this function.  If no entry is present for a given
             * generator, the default translator should be used.
             */
            QHash<QString, const FunctionTranslationEngine*> currentTranslators;

            /**
             * Map of supported function variants for this function.
             */
            QMap<FunctionVariant, Capabilities> currentVariants;

            /**
             * The function type.
             */
            Type currentFunctionType;

            /**
             * The function internal name.
             */
            QString currentInternalName;

            /**
             * The function library name.
             */
            QString currentPrimaryLibraryName;

            /**
             * The function's user visible name.  This is the name as entered by the user.
             */
            VariableName currentUserVisibleName;

            /**
             * Flag used to indicate if the first parameter should be a subscript.
             */
            bool currentIncludeSubscriptParameter;

            /**
             * Command string the user can use to quickly instantiate the function.
             */
            QString currentFunctionCommand;

            /**
             * A user readable description of the funciton.
             */
            QString currentDescription;

            /**
             * A category to insert the function under.
             */
            QString currentCategory;

            /**
             * The help data tied to the function.
             */
            QString currentHelpData;

            /**
             * Flag indicating if the function requires a per-thread instance as the first function parameter.
             */
            bool currentRequiresPerThreadInstance;

            /**
             * The minimum number of function parameters.
             */
            unsigned currentMinimumNumberParameters;

            /**
             * The maximum number of function parameters.
             */
            unsigned currentMaximumNumberParameters;

            /**
             * The current capabilities that can be supplied by any variant of this function.
             */
            Capabilities allSupportedCapabilities;
    };
};

#endif
