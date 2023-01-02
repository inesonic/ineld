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
* This header defines the \ref Ld::FunctionVariant class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_FUNCTION_VARIANT_H
#define LD_FUNCTION_VARIANT_H

#include <QString>
#include <QList>
#include <QHash>

#include "ld_common.h"
#include "ld_data_type.h"
#include "ld_capabilities.h"
#include "ld_element_structures.h"

namespace Ld {
    class FunctionTranslator;
    class FunctionVariant;
    class DataTypeTranslator;
    class DataTypeDecoder;

    /**
     * Class used to store the parameter list and return value data for a given function.  This function uses a PIMPL
     * implementation to support fast copy operations.  Note that the data is internally pooled to facilitate reduced
     * memory utilization and fast ordering during run-time (at the expense of slightly increased start-up time).
     */
    class LD_PUBLIC_API FunctionVariant {
        public:
            /**
             * Function that clears the underlying database.
             */
            static void clearDatabase();

            FunctionVariant();

            /**
             * Constructor
             *
             * \param[in] returnValueType       The type of the return value for this variant of this function.
             *
             * \param[in] parameterTypes        A list of parameter types.
             *
             * \param[in] parameterDescriptions A list of parameter descriptions.
             */
            FunctionVariant(
                DataType::ValueType        returnValueType,
                QList<DataType::ValueType> parameterTypes,
                QList<QString>             parameterDescriptions
            );

            /**
             * Constructor
             *
             * \param[in] libraryName           The name of the library defining this variant.
             *
             * \param[in] returnValueType       The type of the return value for this variant of this function.
             *
             * \param[in] parameterTypes        A list of parameter types.
             *
             * \param[in] parameterDescriptions A list of parameter descriptions.
             */
            FunctionVariant(
                const QString&             libraryName,
                DataType::ValueType        returnValueType,
                QList<DataType::ValueType> parameterTypes,
                QList<QString>             parameterDescriptions
            );

            /**
             * Templatized constructor
             *
             * \param[in] returnValueType The type of the return value for this variant of this function.
             *
             * \param[in] parameters      A list of parameters.
             */
            template<typename... Parameters> LD_PUBLIC_TEMPLATE_METHOD FunctionVariant(
                    DataType::ValueType returnValueType,
                    Parameters...       parameters
                ) {
                QList<DataType::ValueType> parameterTypes;
                QList<QString>             parameterDescriptions;
                buildParameterData(parameterTypes, parameterDescriptions, parameters...);

                configure(
                    QString(),
                    QList<DataType::ValueType>(),
                    returnValueType,
                    parameterTypes,
                    parameterDescriptions
                );
            }

            /**
             * Templatized constructor
             *
             * \param[in] libraryName     The name of the library defining this variant.
             *
             * \param[in] returnValueType The type of the return value for this variant of this function.
             *
             * \param[in] parameters      A list of parameters.
             */
            template<typename... Parameters> LD_PUBLIC_TEMPLATE_METHOD FunctionVariant(
                    const QString&      libraryName,
                    DataType::ValueType returnValueType,
                    Parameters...       parameters
                ) {
                QList<DataType::ValueType> parameterTypes;
                QList<QString>             parameterDescriptions;
                buildParameterData(parameterTypes, parameterDescriptions, parameters...);

                configure(
                    libraryName,
                    QList<DataType::ValueType>(),
                    returnValueType,
                    parameterTypes,
                    parameterDescriptions
                );
            }

            /**
             * Constructor
             *
             * \param[in] supportedVariadicTypes A list of supported variadic types.
             *
             * \param[in] returnValueType        The type of the return value for this variant of this function.
             *
             * \param[in] parameterTypes         A list of parameter types.
             *
             * \param[in] parameterDescriptions  A list of parameter descriptions.
             */
            FunctionVariant(
                const QList<DataType::ValueType>& supportedVariadicTypes,
                DataType::ValueType               returnValueType,
                QList<DataType::ValueType>        parameterTypes,
                QList<QString>                    parameterDescriptions
            );

            /**
             * Constructor
             *
             * \param[in] supportedVariadicTypes A list of supported variadic types.
             *
             * \param[in] libraryName            The name of the library defining this variant.
             *
             * \param[in] returnValueType        The type of the return value for this variant of this function.
             *
             * \param[in] parameterTypes         A list of parameter types.
             *
             * \param[in] parameterDescriptions  A list of parameter descriptions.
             */
            FunctionVariant(
                const QString&                    libraryName,
                const QList<DataType::ValueType>& supportedVariadicTypes,
                DataType::ValueType               returnValueType,
                QList<DataType::ValueType>        parameterTypes,
                QList<QString>                    parameterDescriptions
            );

            /**
             * Constructor
             *
             * \param[in] supportedVariadicTypes A list of supported variadic types.
             *
             * \param[in] returnValueType        The type of the return value for this variant of this function.
             */
            FunctionVariant(
                const QList<DataType::ValueType>& supportedVariadicTypes,
                DataType::ValueType               returnValueType
            );

            /**
             * Constructor
             *
             * \param[in] supportedVariadicTypes A list of supported variadic types.
             *
             * \param[in] libraryName            The name of the library defining this variant.
             *
             * \param[in] returnValueType        The type of the return value for this variant of this function.
             */
            FunctionVariant(
                const QString&                    libraryName,
                const QList<DataType::ValueType>& supportedVariadicTypes,
                DataType::ValueType               returnValueType
            );

            /**
             * Templatized constructor
             *
             * \param[in] supportedVariadicTypes A list of supported variadic types.
             *
             * \param[in] returnValueType        The type of the return value for this variant of this function.
             *
             * \param[in] parameters             A list of parameters.
             */
            template<typename... Parameters> LD_PUBLIC_TEMPLATE_METHOD FunctionVariant(
                    const QList<DataType::ValueType>& supportedVariadicTypes,
                    DataType::ValueType               returnValueType,
                    Parameters...                     parameters
                ) {
                QList<DataType::ValueType> parameterTypes;
                QList<QString>             parameterDescriptions;
                buildParameterData(parameterTypes, parameterDescriptions, parameters...);

                configure(QString(), supportedVariadicTypes, returnValueType, parameterTypes, parameterDescriptions);
            }

            /**
             * Templatized constructor
             *
             * \param[in] libraryName            The name of the library defining this variant.
             *
             * \param[in] supportedVariadicTypes A list of supported variadic types.
             *
             * \param[in] returnValueType        The type of the return value for this variant of this function.
             *
             * \param[in] parameters             A list of parameters.
             */
            template<typename... Parameters> LD_PUBLIC_TEMPLATE_METHOD FunctionVariant(
                    const QString&                    libraryName,
                    const QList<DataType::ValueType>& supportedVariadicTypes,
                    DataType::ValueType               returnValueType,
                    Parameters...                     parameters
                ) {
                QList<DataType::ValueType> parameterTypes;
                QList<QString>             parameterDescriptions;
                buildParameterData(parameterTypes, parameterDescriptions, parameters...);

                configure(libraryName, supportedVariadicTypes, returnValueType, parameterTypes, parameterDescriptions);
            }

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            FunctionVariant(const FunctionVariant& other);

            ~FunctionVariant();

            /**
             * Method you can use to determine if this function variant instance is valid.
             *
             * \return Returns true if this instance is valid.  Returns false if this instance is invalid.
             */
            bool isValid() const;

            /**
             * Method you can use to determine if this function variant instance is invalid.
             *
             * \return Returns true if this instance is invalid.  Returns false if this instance is valid.
             */
            bool isInvalid() const;

            /**
             * Method you can use to determine if this function variant instance allows variadic parameters.
             *
             * \return Returns true if this instance allows variadic parameters.  Returns false if this instance
             *         does not allow variadic parameters.
             */
            bool allowsVariadicParameters() const;

            /**
             * Method you can use to determine if this function variant instance does not allow variadic parameters.
             *
             * \return Returns true if this instance does not allow variadic parameters.  Returns false if this
             *         instance allows variadic parameters.
             */
            bool doesNotAllowVariadicParameters() const;

            /**
             * Method you can use to determine the supported variadic parameter types.
             *
             * \return Returns a list of supported variadic parameter types.
             */
            const QList<DataType::ValueType>& supportedVariadicTypes() const;

            /**
             * Method you can use to obtain the name of the library containing this variant.
             *
             * \return Returns the name of the library implementing this variant.
             */
            QString libraryName() const;

            /**
             * Method you can use to obtain the return value tied to this function variant.
             *
             * \return Returns the return value type for this function variant.
             */
            DataType::ValueType returnValueType() const;

            /**
             * Method you can use to obtain the number of parameters tied to this function variant.
             *
             * \return Returns the number of parameters.
             */
            unsigned numberParameters() const;

            /**
             * Method you can use to obtain a list of all the parameter types.
             *
             * \return Returns a list of parameter types.
             */
            const QList<DataType::ValueType>& parameterTypes() const;

            /**
             * Method you can use to obtain a list of all the parameter descriptions.
             *
             * \return Returns a list of parameter descriptions.
             */
            const QList<QString> parameterDescriptions() const;

            /**
             * Method you can use to obtain a list of recommended parameter capability values. This list is generated
             * from the provided parameter types.
             *
             * \return Returns a list of the recommended capabilities supported by this function variant.
             */
            QList<Capabilities> parameterCapabilities() const;

            /**
             * Method you can use to obtain the capabilities provided by this function variant.
             *
             * \return Returns the variant's capabilities.
             */
            Capabilities variantCapabilities() const;

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to assign to this instance.
             */
            FunctionVariant& operator=(const FunctionVariant& other);

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instances are equal.  Returns false if the instances are not equal.
             */
            bool operator==(const FunctionVariant& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instances are not equal.  Returns false if the instances are equal.
             */
            bool operator!=(const FunctionVariant& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if this instance should precede the other instance.  Returns false if this instance
             *         is equal to or follows the other instance.
             */
            bool operator<(const FunctionVariant& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if this instance should follow the other instance.  Returns false if this instance
             *         is equal to or precedes the other instance.
             */
            bool operator>(const FunctionVariant& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if this instance is equal to or should precede the other instance.  Returns false
             *         if this instance follows the other instance.
             */
            bool operator<=(const FunctionVariant& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if this instance is equal to or should follow the other instance.  Returns false if
             *         this instance precedes the other instance.
             */
            bool operator>=(const FunctionVariant& other) const;

        private:
            /**
             * Method that configures this widget.
             *
             * \param[in] libraryName            The name of the library defining this variant.
             *
             * \param[in] supportedVariadicTypes A list of supported variadic types.
             *
             * \param[in] returnValueType        The type of the return value for this variant of this function.
             *
             * \param[in] parameterTypes         A list of parameter types.
             *
             * \param[in] parameterDescriptions  A list of parameter descriptions.
             */
            void configure(
               const QString&                    libraryName,
               const QList<DataType::ValueType>& supportedVariadicTypes,
               DataType::ValueType               returnValueType,
               QList<DataType::ValueType>        parameterTypes,
               QList<QString>                    parameterDescriptions
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
