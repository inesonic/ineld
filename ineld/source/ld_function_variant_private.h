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
* This header defines the private implemetnation of the \ref Ld::FunctionVariant::Private class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_FUNCTION_VARIANT_PRIVATE_H
#define LD_FUNCTION_VARIANT_PRIVATE_H

#include <QSharedPointer>
#include <QString>
#include <QList>
#include <QMap>

#include "ld_common.h"
#include "ld_data_type.h"
#include "ld_capabilities.h"
#include "ld_element_structures.h"
#include "ld_function_variant.h"

namespace Ld {
    class FunctionTranslator;
    class FunctionVariant;
    class DataTypeTranslator;
    class DataTypeDecoder;

    /**
     * Private implementation of the \ref Ld::FunctionVariant class.
     */
    class LD_PUBLIC_API FunctionVariant::Private {
        public:
            /**
             * Function that clears the underlying database.  This function exists for test purposes and will
             * disassociate any existing entries from the database.
             */
            static void clearDatabase();

            /**
             * Method you can call to register a new private instance with the internal database.
             *
             * \param[in] instance The instance to be registered.  This method will take ownership of this instance and
             *                     may silently delete it.
             *
             * \return Returns a shared pointer to the instance you should use.  This instance may be the same as or
             *         different from the instance you supply.
             */
            static QSharedPointer<Private> registerPrivateInstance(Private* instance);

            /**
             * Method you can call to register a new private instance with the internal database.
             *
             * \param[in] instance The instance to be registered.  This method will take ownership of this instance and
             *                     may silently delete it.
             *
             * \return Returns a shared pointer to the instance you should use.  This instance may be the same as or
             *         different from the instance you supply.
             */
            static QSharedPointer<Private> registerPrivateInstance(QSharedPointer<Private> instance);

            /**
             * Constructor
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
            Private(
                const QString&                    libraryName,
                const QList<DataType::ValueType>& supportedVariadicTypes,
                DataType::ValueType               returnValueType,
                QList<DataType::ValueType>        parameterTypes,
                QList<QString>                    parameterDescriptions
            );

            ~Private();

            /**
             * Method you can use to determine if this function variant instance allows variadic parameters.
             *
             * \return Returns true if this instance allows variadic parameters.  Returns false if this instance
             *         does not allow variadic parameters.
             */
            bool allowsVariadicParameters() const;

            /**
             * Method you can use to determine the supported variadic parameter types.
             *
             * \return Returns a list of supported variadic parameter types.
             */
            const QList<DataType::ValueType>& supportedVariadicTypes() const;

            /**
             * Method you can use to obtain the library name of the library implementing this function variant.
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
             * Function used internally to map value types to capabilities.
             *
             * \param[in] valueType The value type for the function.
             */
            static Capabilities capabilitiesFromValueType(DataType::ValueType valueType);

            /**
             * Method that updates the database of function variants.
             */
            static void updateDatabase();

            /**
             * Container for the private implementation of the \ref Ld::FunctionVariant class.
             */
            class LD_PUBLIC_API Container {
                public:
                    Container();

                    /**
                     * Constructor
                     *
                     * \param[in] privateImplementation The private implementation to be tracked in this container.
                     *                                  This method will take ownership of the implementation.
                     */
                    Container(FunctionVariant::Private* privateImplementation);

                    /**
                     * Constructor
                     *
                     * \param[in] privateImplementation The private implementation to be tracked in this container.
                     */
                    Container(QSharedPointer<FunctionVariant::Private> privateImplementation);

                    /**
                     * Copy constructor
                     *
                     * \param[in] other The instance to be copied.
                     */
                    Container(const Container& other);

                    ~Container();

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
                     * Method you can use to obtain a shared pointer from this container.
                     *
                     * \return Returns a shared pointer for the container.
                     */
                    QSharedPointer<Private> pointer() const;

                    /**
                     * Method you can use to determine if this function variant instance allows variadic parameters.
                     *
                     * \return Returns true if this instance allows variadic parameters.  Returns false if this instance
                     *         does not allow variadic parameters.
                     */
                    bool allowsVariadicParameters() const;

                    /**
                     * Method you can use to determine the supported variadic parameter types.
                     *
                     * \return Returns a list of supported variadic parameter types.
                     */
                    const QList<DataType::ValueType>& supportedVariadicTypes() const;

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
                    Container& operator=(const Container& other);

                    /**
                     * Comparison operator.
                     *
                     * \param[in] other The instance to compare against this instance.
                     *
                     * \return Returns true if the instances are equal.  Returns false if the instances are not equal.
                     */
                    bool operator==(const Container& other) const;

                    /**
                     * Comparison operator.
                     *
                     * \param[in] other The instance to compare against this instance.
                     *
                     * \return Returns true if the instances are not equal.  Returns false if the instances are equal.
                     */
                    bool operator!=(const Container& other) const;

                    /**
                     * Comparison operator.
                     *
                     * \param[in] other The instance to compare against this instance.
                     *
                     * \return Returns true if this instance should precede the other instance.  Returns false if this instance
                     *         is equal to or follows the other instance.
                     */
                    bool operator<(const Container& other) const;

                    /**
                     * Comparison operator.
                     *
                     * \param[in] other The instance to compare against this instance.
                     *
                     * \return Returns true if this instance should follow the other instance.  Returns false if this instance
                     *         is equal to or precedes the other instance.
                     */
                    bool operator>(const Container& other) const;

                    /**
                     * Comparison operator.
                     *
                     * \param[in] other The instance to compare against this instance.
                     *
                     * \return Returns true if this instance is equal to or should precede the other instance.  Returns false
                     *         if this instance follows the other instance.
                     */
                    bool operator<=(const Container& other) const;

                    /**
                     * Comparison operator.
                     *
                     * \param[in] other The instance to compare against this instance.
                     *
                     * \return Returns true if this instance is equal to or should follow the other instance.  Returns false if
                     *         this instance precedes the other instance.
                     */
                    bool operator>=(const Container& other) const;

                private:
                    /**
                     * Static method that compares two data type lists.
                     *
                     * \param[in] list1 The first list to compare.
                     *
                     * \param[in] list2 The second list to compare.
                     *
                     * \return returns a negative value if list 1 should precede list 2.  Returns 0 if the lists are
                     *         equal.  Returns a positive value if list 1 should follow list 2.
                     */
                    static int compare(
                        const QList<DataType::ValueType>& list1,
                        const QList<DataType::ValueType>& list2
                    );

                    QSharedPointer<Private> currentInstance;
            };

            /**
             * Static map of index values by function variants.
             */
            static QMap<FunctionVariant::Private::Container, unsigned> currentFunctionVariants;

            /**
             * Flag indicating if the function variant's index values need to be updated.
             */
            static bool currentStaleIndexes;

            /**
             * The library containing the implementation of this variant.
             */
            QString currentLibrary;

            /**
             * The zero based index of this function variant.
             */
            unsigned currentIndex;

            /**
             * The supported variadic data types.  An empty list indicates that no variadic types are supported.
             */
            QList<DataType::ValueType> currentSupportedVariadicTypes;

            /**
             * The variant's return value type.
             */
            DataType::ValueType currentReturnValueType;

            /**
             * The variant's parameter types.
             */
            QList<DataType::ValueType> currentParameterTypes;

            /**
             * The variant's descriptions.
             */
            QList<QString> currentParameterDescriptions;

            /**
             * The capabilities of this function variant.
             */
            Capabilities currentVariantCapabilities;

            /**
             * The variant's parameter capabilities.
             */
            QList<Capabilities> currentParameterCapabilities;
    };
};

#endif
