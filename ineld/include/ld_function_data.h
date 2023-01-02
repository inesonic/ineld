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
* This header defines the \ref Ld::FunctionData class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_FUNCTION_DATA_H
#define LD_FUNCTION_DATA_H

#include <QString>
#include <QList>
#include <QHash>

#include "ld_common.h"
#include "ld_variable_name.h"
#include "ld_data_type.h"
#include "ld_capabilities.h"
#include "ld_function_variant.h"
#include "ld_element_structures.h"

namespace Ld {
    class FunctionTranslationEngine;
    class FunctionVariant;

    /**
     * Class used to store information about a function.
     */
    class LD_PUBLIC_API FunctionData {
        public:
            /**
             * Enumeration of supported function types
             */
            enum Type : std::uint8_t {
                /**
                 * Indicates an Inesonic built-in function.
                 */
                BUILT_IN = 0,

                /**
                 * Indicates an Inesonic plug-in function.
                 */
                PLUG_IN = 1,

                /**
                 * Indicates a user-defined function that is instantiated in an external library.
                 */
                USER_DEFINED = 2
            };

            FunctionData();

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
             * \param[in] functionCommand           An optional command used to enter the function.  This field may
             *                                      also be used to define GUI actions and other GUI related
             *                                      identifiers.
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
             *
             * \param[in] variant                   The first variant instance.
             */
            FunctionData(
                Type                   functionType,
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
            );

            /**
             * Constructor
             *
             * \param[in] functionType              The type of function being defined.
             *
             * \param[in] internalName              The internal name for the function.  This name is typically used by
             *                                      the code translation engines.
             *
             * \param[in] userVisibleName           The function name as entered/seen by the user.
             *
             * \param[in] includeSubscriptParameter If true, the function should include a subscript for the first
             *                                      parameter.
             *
             * \param[in] functionCommand           An optional command used to enter the function.  This field may
             *                                      also be used to define GUI actions and other GUI related
             *                                      identifiers.
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
             *
             * \param[in] variant                   The first variant instance.
             */
            FunctionData(
                Type                   functionType,
                const QString&         internalName,
                const VariableName&    userVisibleName,
                bool                   includeSubscriptParameter,
                const QString&         functionCommand,
                const QString&         description,
                const QString&         category,
                const QString&         helpData,
                bool                   requiresPerThreadInstance,
                const FunctionVariant& variant
            );

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
             * \param[in] functionCommand           An optional command used to enter the function.  This field may
             *                                      also be used to define GUI actions and other GUI related
             *                                      identifiers.
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
             *
             * \param[in] returnValueType           The type of the return value for the first variant of this
             *                                      function.
             *
             * \param[in] parameterTypes            A list of parameter types.
             *
             * \param[in] parameterDescriptions     A list of parameter descriptions.
             */
            FunctionData(
                Type                       functionType,
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
            );

            /**
             * Constructor
             *
             * \param[in] functionType              The type of function being defined.
             *
             * \param[in] internalName              The internal name for the function.  This name is typically used by
             *                                      the code translation engines.
             *
             * \param[in] userVisibleName           The function name as entered/seen by the user.
             *
             * \param[in] includeSubscriptParameter If true, the function should include a subscript for the first
             *                                      parameter.
             *
             * \param[in] functionCommand           An optional command used to enter the function.  This field may
             *                                      also be used to define GUI actions and other GUI related
             *                                      identifiers.
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
             *
             * \param[in] returnValueType           The type of the return value for the first variant of this
             *                                      function.
             *
             * \param[in] parameterTypes            A list of parameter types.
             *
             * \param[in] parameterDescriptions     A list of parameter descriptions.
             */
            FunctionData(
                Type                       functionType,
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
            );

            /**
             * Function data builder method.
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
             * \param[in] functionCommand           An optional command used to enter the function.  This field may
             *                                      also be used to define GUI actions and other GUI related
             *                                      identifiers.
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
             *
             * \param[in] variant                   The first variant instance.
             *
             * \param[in] additional                The additional variant values to include.
             */
            template<typename... Additional> LD_PUBLIC_TEMPLATE_METHOD FunctionData(
                    Type                   functionType,
                    const QString&         internalName,
                    const QString&         primaryLibraryName,
                    const VariableName&    userVisibleName,
                    bool                   includeSubscriptParameter,
                    const QString&         functionCommand,
                    const QString&         description,
                    const QString&         category,
                    const QString&         helpData,
                    bool                   requiresPerThreadInstance,
                    const FunctionVariant& variant,
                    Additional...          additional
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

                addVariant(variant);
                addVariants(additional...);
            }

            /**
             * Function data builder method.
             *
             * \param[in] functionType              The type of function being defined.
             *
             * \param[in] internalName              The internal name for the function.  This name is typically used by
             *                                      the code translation engines.
             *
             * \param[in] userVisibleName           The function name as entered/seen by the user.
             *
             * \param[in] includeSubscriptParameter If true, the function should include a subscript for the first
             *                                      parameter.
             *
             * \param[in] functionCommand           An optional command used to enter the function.  This field may
             *                                      also be used to define GUI actions and other GUI related
             *                                      identifiers.
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
             *
             * \param[in] variant                   The first variant instance.
             *
             * \param[in] additional                The additional variant values to include.
             */
            template<typename... Additional> LD_PUBLIC_TEMPLATE_METHOD FunctionData(
                    Type                   functionType,
                    const QString&         internalName,
                    const VariableName&    userVisibleName,
                    bool                   includeSubscriptParameter,
                    const QString&         functionCommand,
                    const QString&         description,
                    const QString&         category,
                    const QString&         helpData,
                    bool                   requiresPerThreadInstance,
                    const FunctionVariant& variant,
                    Additional...          additional
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

                addVariant(variant);
                addVariants(additional...);
            }

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
             * \param[in] functionCommand           An optional command used to enter the function.  This field may
             *                                      also be used to define GUI actions and other GUI related
             *                                      identifiers.
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
             *
             * \param[in] returnValueType           The type of the return value for the first variant of this
             *                                      function.
             *
             * \param[in] additional                The additional parameters.
             */
            template<typename... Additional> LD_PUBLIC_TEMPLATE_METHOD FunctionData(
                    Type                functionType,
                    const QString&      internalName,
                    const QString&      primaryLibraryName,
                    const VariableName& userVisibleName,
                    bool                includeSubscriptParameter,
                    const QString&      functionCommand,
                    const QString&      description,
                    const QString&      category,
                    const QString&      helpData,
                    bool                requiresPerThreadInstance,
                    DataType::ValueType returnValueType,
                    Additional...       additional
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

                QList<DataType::ValueType> parameterTypesList;
                QList<QString>             parameterDescriptionList;
                buildParameterData(parameterTypesList, parameterDescriptionList, additional...);

                addVariant(FunctionVariant(returnValueType, parameterTypesList, parameterDescriptionList));
            }

            /**
             * Constructor
             *
             * \param[in] functionType              The type of function being defined.
             *
             * \param[in] internalName              The internal name for the function.  This name is typically used by
             *                                      the code translation engines.
             *
             * \param[in] userVisibleName           The function name as entered/seen by the user.
             *
             * \param[in] includeSubscriptParameter If true, the function should include a subscript for the first
             *                                      parameter.
             *
             * \param[in] functionCommand           An optional command used to enter the function.  This field may
             *                                      also be used to define GUI actions and other GUI related
             *                                      identifiers.
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
             *
             * \param[in] returnValueType           The type of the return value for the first variant of this
             *                                      function.
             *
             * \param[in] additional                The additional parameters.
             */
            template<typename... Additional> LD_PUBLIC_TEMPLATE_METHOD FunctionData(
                    Type                functionType,
                    const QString&      internalName,
                    const VariableName& userVisibleName,
                    bool                includeSubscriptParameter,
                    const QString&      functionCommand,
                    const QString&      description,
                    const QString&      category,
                    const QString&      helpData,
                    bool                requiresPerThreadInstance,
                    DataType::ValueType returnValueType,
                    Additional...       additional
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

                QList<DataType::ValueType> parameterTypesList;
                QList<QString>             parameterDescriptionList;
                buildParameterData(parameterTypesList, parameterDescriptionList, additional...);

                addVariant(FunctionVariant(returnValueType, parameterTypesList, parameterDescriptionList));
            }

            /**
             * Constructor
             *
             * \param[in] functionType              The type of function being defined.
             *
             * \param[in] internalName              The internal name for the function.  This name is typically used by
             *                                      the code translation engines.
             *
             * \param[in] userVisibleName           The function name as entered/seen by the user.
             *
             * \param[in] includeSubscriptParameter If true, the function should include a subscript for the first
             *                                      parameter.
             *
             * \param[in] functionCommand           An optional command used to enter the function.  This field may
             *                                      also be used to define GUI actions and other GUI related
             *                                      identifiers.
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
             *
             * \param[in] variants                  A list of variants for the function.  The list must not be empty.
             */
            FunctionData(
                Type                          functionType,
                const QString&                internalName,
                const VariableName&           userVisibleName,
                bool                          includeSubscriptParameter,
                const QString&                functionCommand,
                const QString&                description,
                const QString&                category,
                const QString&                helpData,
                bool                          requiresPerThreadInstance,
                const QList<FunctionVariant>& variants
            );

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
             * \param[in] functionCommand           An optional command used to enter the function.  This field may
             *                                      also be used to define GUI actions and other GUI related
             *                                      identifiers.
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
             *
             * \param[in] variants                  A list of variants for the function.  The list must not be empty.
             */
            FunctionData(
                Type                          functionType,
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
            );

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            FunctionData(const FunctionData& other);

            ~FunctionData();

            /**
             * Method you can use to determine if this function data instance is valid.
             *
             * \return Returns true if this instance is valid.  Returns false if this instance is invalid.
             */
            bool isValid() const;

            /**
             * Method you can use to determine if this function data instance is invalid.
             *
             * \return Returns true if this instance is invalid.  Returns false if this instance is valid.
             */
            bool isInvalid() const;

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
             * \param[in] generatorName     The name of the generator.
             *
             * \param[in] defaultTranslator A pointer to the default translator.  This translator is expected to be
             *                              statically defined and will not be deleted.
             *
             * \return Returns true on success, returns false if a translator has already been registered for the
             *         indicated generator.
             */
            bool registerTranslator(
                const QString&                   generatorName,
                const FunctionTranslationEngine* defaultTranslator
            );

            /**
             * Method you can use to obtain the translator to be used for this function.
             *
             * \param[in] generatorName The name of the generator.
             *
             * \return Returns a pointer to the translator to be used for this function.
             */
            const FunctionTranslationEngine* translator(const QString& generatorName) const;

            /**
             * Method you can call to define a new variant for this function.
             *
             * \param[in] variant The new variant to be registered with this function.
             *
             * \return Returns true on success, returns false if the variant is already registered with the function.
             */
            bool addVariant(const FunctionVariant& variant);

            /**
             * Method you can call to add one mor more variants.
             *
             * \param[in] variant The variant to registered with this function.
             */
            inline bool addVariants(const FunctionVariant& variant) {
                return addVariant(variant);
            }

            /**
             * Method you can call to add one mor more variants.
             *
             * \param[in] variant    The variant to registered with this function.
             *
             * \param[in] additional Additional variants to be added.
             */
            template<typename... Additional> inline bool addVariants(
                    const FunctionVariant& variant,
                    Additional...          additional
                ) {
                return addVariants(variant) && addVariants(additional...);
            }

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
             * \return Returns the command the user can use to insert this function.  This field may also be used to
             *         define GUI actions and other GUI related identifiers.
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
             * \param[in] requiredCapabilities The required capabilities for the function.
             *
             * \param[in] knownParameterList   A list holding the known parameter value types.
             *
             * \return Returns the best fit function variant.
             */
            FunctionVariant bestFitVariant(
                const Capabilities&               requiredCapabilities,
                const QList<DataType::ValueType>& knownParameterList
            ) const;

            /**
             * Method you can use to obtain a sorted list of all variants that meet a parameter list.
             *
             * \param[in] requiredCapabilities The required capabilities for the function.
             *
             * \param[in] knownParameterList   A list holding the known parameter values.  Entries with the value
             *                                 Ld::DataType::ValueType::NONE are ignored.  Entries not defined by this
             *                                 list are also ignored.
             *
             * \return Returns a sorted list holding all variants that are compatible with the provided list.
             */
            QList<FunctionVariant> allCompatibleVariants(
                const Capabilities&               requiredCapabilities,
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
             * Assignment operator.
             *
             * \param[in] other The instance to assign to this instance.
             */
            FunctionData& operator=(const FunctionData& other);

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instances are equal.  Returns false if the instances are not equal.
             */
            bool operator==(const FunctionData& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instances are not equal.  Returns false if the instances are equal.
             */
            bool operator!=(const FunctionData& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if this instance should precede the other instance.  Returns false if this instance
             *         is equal to or follows the other instance.
             */
            bool operator<(const FunctionData& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if this instance should follow the other instance.  Returns false if this instance
             *         is equal to or precedes the other instance.
             */
            bool operator>(const FunctionData& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if this instance is equal to or should precede the other instance.  Returns false
             *         if this instance follows the other instance.
             */
            bool operator<=(const FunctionData& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if this instance is equal to or should follow the other instance.  Returns false if
             *         this instance precedes the other instance.
             */
            bool operator>=(const FunctionData& other) const;

        private:
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
            void configure(
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

            /**
             * Method that builds up a pair of lists from a provided list of parameter values.
             *
             * \param[in,out] parameterTypesList       The list of parameter types.
             *
             * \param[in,out] parameterDescriptionList The list of parameter descriptions.
             *
             * \param[in]     parameterType            The parameter type to be appended to the list.
             *
             * \param[in]     parameterDescription     The parameter description to be appended to the list.
             */
            static void buildParameterData(
                QList<DataType::ValueType>& parameterTypesList,
                QList<QString>&             parameterDescriptionList,
                DataType::ValueType         parameterType,
                const QString&              parameterDescription
            );

            /**
             * Method that builds up a pair of lists from a provided list of parameter values.
             *
             * \param[in,out] parameterTypesList       The list of parameter types.
             *
             * \param[in,out] parameterDescriptionList The list of parameter descriptions.
             *
             * \param[in]     parameterType            The parameter type to be appended to the list.
             *
             * \param[in]     parameterDescription     The parameter description to be appended to the list.
             *
             * \param[in]     additional               The additional parameters to be
             */
            template<typename... Parameters> static LD_PUBLIC_TEMPLATE_METHOD void buildParameterData(
                    QList<DataType::ValueType>& parameterTypesList,
                    QList<QString>&             parameterDescriptionList,
                    DataType::ValueType         parameterType,
                    const QString&              parameterDescription,
                    Parameters...               additional

                ) {
                parameterTypesList.append(parameterType);
                parameterDescriptionList.append(parameterDescription);
                buildParameterData(parameterTypesList, parameterDescriptionList, additional...);
            }

            /**
             * The private implementation type.
             */
            class Private;

            /**
             * Pointer to the underlying private implementation.
             */
            QSharedPointer<Private> impl;
    };
};

#endif
