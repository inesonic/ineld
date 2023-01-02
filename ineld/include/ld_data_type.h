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
* This header defines the \ref Ld::DataType class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_DATA_TYPE_H
#define LD_DATA_TYPE_H

#include <QString>
#include <QList>
#include <QHash>

#include <util_hash_functions.h>

#include <model_api_types.h>
#include <model_variant.h>

#include "ld_common.h"
#include "ld_element_structures.h"

namespace Ld {
    class DataTypeTranslator;
    class DataTypeDecoder;

    /**
     * Class used to store information about supported data types.  The class uses a PIMPL implementation to provide
     * for fast assignment and comparison operations.
     */
    class LD_PUBLIC_API DataType {
        friend LD_PUBLIC_API Util::HashResult qHash(const DataType&, Util::HashSeed);

        public:
            /**
             * Enumeration of dynamic type conversion cases.
             */
            enum class DynamicConversion {
                /**
                 * Value indicating that dynamic conversion is never possible.
                 */
                NEVER,

                /**
                 * Value indicating that dynamic conversion is possible in some scenarios.
                 */
                SOMETIMES,

                /**
                 * Value indicating that dynamic conversion is always possible.
                 */
                ALWAYS
            };

            /**
             * Type representing all known, supported value types.
             */
            typedef Model::ValueType ValueType;

            /**
             * Type used to represent data-type properties.
             */
            typedef unsigned Property;

            /**
             * Value indicating the number of value types.
             */
            static constexpr unsigned numberValueTypes = static_cast<unsigned>(Model::ValueType::NUMBER_TYPES);

            /**
             * Empty property value.
             */
            static constexpr Property nullProperty = 0;

            /**
             * Property value indicating that the data-type is assignable.
             */
            static constexpr Property assignable = 1;

            /**
             * Property value indicating that the data-type is a matrix.
             */
            static constexpr Property matrix = 2;

            /**
             * Property value indicating that the data-type is a container.
             */
            static constexpr Property container = 4;

            /**
             * Property value indicating that the data-type is variant.
             */
            static constexpr Property variant = 8;

            /**
             * The default properties.
             */
            static const Property defaultProperties;

            /**
             * Method you can call to register a new data type with the program.
             *
             * \param[in] valueType             The \ref Ld::DataType::ValueType this data type represents.
             *
             * \param[in] description           A description of this data type.
             *
             * \param[in] typeSymbolElementName The name of the symbol used to define this data-type.  An empty string
             *                                  can be provided if there is no element representing this data type.
             *
             * \param[in] properties            Properties to apply to the data type.
             *
             * \return Returns true if this data-type was registered.  Returns false if a data-type with the same
             *         type-name already exists.
             */
            static bool registerType(
                ValueType      valueType,
                const QString& description,
                const QString& typeSymbolElementName = QString(),
                Property       properties = defaultProperties
            );

            /**
             * Method you can call to obtain a list of every supported data-type.
             *
             * \return Returns a list of every supported data type.
             */
            static const QList<DataType>& allTypes();

            /**
             * Method you can call to obtain a data-type by value type.
             *
             * \param[in] valueType The value type we want the entry for.
             *
             * \return Returns the associated data-type.  An invalid data-type will be returned if the value type is
             *         not recognized.
             */
            static DataType fromValueType(ValueType valueType);

            /**
             * Method you can call to obtain a data-type by symbol element name.
             *
             * \param[in] typeName   The type name of the symbol used to identify the element.
             *
             * \param[in] matrixType Optional parameter you can use to specify that the type should be a matrix of the
             *                       specified type.
             *
             * \return Returns the associated data-type.  An invalid data-type will be returned if the symbol name is
             *         not recognized.
             */
            static DataType fromSymbol(const QString& typeName, bool matrixType = false);

            /**
             * Method you can call to obtain a data-type by a symbol element.
             *
             * \param[in] element    The element we want to associated type for.
             *
             * \param[in] matrixType Optional parameter you can use to specify that the type should be a matrix of the
             *                       specified type.
             *
             * \return Returns the assocaited data-type.  An invalid data-type will be returned if the symbol name is
             *         not recognized.
             */
            static DataType fromSymbol(ElementPointer element, bool matrixType = false);

            /**
             * Method you can use to set the default data-type.
             *
             * \param[in] defaultType The default data-type.
             */
            static void setDefaultDataType(const DataType& defaultType);

            /**
             * Method you can use to obtain the default data-type.
             *
             * \return Returns the default data-type.
             */
            static DataType defaultDataType();

            /**
             * Method you can use to register a new data type translator to this data type.
             *
             * \param[in] generatorName The name of the generator using this translator.
             *
             * \param[in] valueType     The value type this translator works with.
             *
             * \param[in] newTranslator The new data-type translator.  The translator is expected to be defined as a
             *                          static instance variable and will *not* be deleted.
             *
             * \return Returns true on success, returns false if the translator was already registered.
             */
            static bool registerTranslator(
                const QString&            generatorName,
                ValueType                 valueType,
                const DataTypeTranslator* newTranslator
            );

            /**
             * Method you can use to register a data type decoder to this data type.  Note that there can be only one
             * decoder tied to each data-type.
             *
             * \param[in] valueType  The value type this translator works with.
             *
             * \param[in] newDecoder The new data-type translator.  Data type decoders should be defined consistently
             *                       as static instance variables or on the heap.  If defined on the heap, be sure to
             *                       call the \ref Ld::DataType::destroyDecoders method when the program terminates to
             *                       destroy the decoders.
             *
             * \return Returns true on success, returns false if a visual for the data type has already been
             *         registered.the translator was already registered.
             */
            static bool registerDecoder(ValueType valueType, const DataTypeDecoder* newDecoder);

            /**
             * Method you can use to destroy all decoders.
             */
            static void destroyDecoders();

            /**
             * Method you can use to determine the best upcase across a range of value types.
             *
             * \param[in] values The values to calculate the best upcast for.
             *
             * \return Returns the recommended value type to use for comparisons and math operations between all the
             *         supplied values types.
             */
            template<typename... Types> static inline ValueType bestUpcast(Types... values) {
                return Model::Variant::bestUpcast(values...);
            }

            /**
             * Method you can use to determine if we can dynamically convert between two types.
             *
             * \param[in] fromType The type we must convert from.
             *
             * \param[in] toType   The type we are converting to.
             *
             * \return Returns when dynamic conversion is possible.
             */
            static DynamicConversion canDynamicallyConvert(ValueType fromType, ValueType toType);

            DataType();

            /**
             * Copy constructor.
             *
             * \param[in] other The other instance to be copied.
             */
            DataType(const DataType& other);

            ~DataType();

            /**
             * Method you can use to determine if this data type is valid.
             *
             * \return Returns true if the data type is valid.  Returns false if the data type is invalid.
             */
            bool isValid() const;

            /**
             * Method you can use to detemrine if this data type is invalid.
             *
             * \return Returns true if the data type is invalid.  Returns false if the data type is valid.
             */
            bool isInvalid() const;

            /**
             * Method you can use to obtain the value type for this data type.
             *
             * \return Returns the value type for this data type.
             */
            ValueType valueType() const;

            /**
             * Method you can use to obtain a description for this type.
             *
             * \return Returns a description of this type.
             */
            const QString& description() const;

            /**
             * Method you can use to obtain the symbol element name.
             *
             * \return Returns the symbol element name.
             */
            const QString& typeSymbolElementName() const;

            /**
             * Method you can use to obtain a new symbol element instance.
             *
             * \return Returns a new symbol element.
             */
            ElementPointer typeSymbolElement() const;

            /**
             * Method you can use to obtain the data type properties.
             *
             * \return Returns the properties for this data type.
             */
            Property properties() const;

            /**
             * Method you can use to register a new data type translator to this data type.
             *
             * \param[in] generatorName The name of the generator using this translator.
             *
             * \param[in] newTranslator The new data-type translator.  The translator is expected to be defined as a
             *                          static instance variable and will *not* be deleted.
             *
             * \return Returns true on success, returns false if the translator was already registered.
             */
            bool registerTranslator(const QString& generatorName, const DataTypeTranslator* newTranslator);

            /**
             * Method you can use to obtain a data-type translator for a specific code generator.
             *
             * \param[in] generatorName The name of the code generator to obtain a translator for.
             *
             * \return Returns a pointer to the requested translator.  A null pointer is returned if a translator has
             *         not been registered for this data-type.
             */
            const DataTypeTranslator* translator(const QString& generatorName) const;

            /**
             * Method you can use to obtain a data-type translator for a specific code generator and data-type.
             *
             * \param[in] generatorName The name of the code generator to obtain a translator for.
             *
             * \param[in] valueType     The value type this translator works with.
             *
             * \return Returns a pointer to the requested translator.  A null pointer is returned if a translator has
             *         not been registered for this data-type.
             */
            static const DataTypeTranslator* translator(const QString& generatorName, ValueType valueType);

            /**
             * Method you can use to register a visual with this data-type.
             *
             * \param[in] newDecoder The new data-type translator.  Data type decoders should be defined consistently
             *                       as static instance variables or on the heap.  If defined on the heap, be sure to
             *                       call the \ref Ld::DataType::destroyDecoders method when the program terminates to
             *                       destroy the decoders.
             */
            bool registerDecoder(const DataTypeDecoder* newDecoder);

            /**
             * Method you can use to obtain a decoder for this data-type.
             *
             * \return Returns a pointer to the decoder tied to this data type.
             */
            const DataTypeDecoder* decoder() const;

            /**
             * Method you can call to translate a matrix type to the underlying base type.
             *
             * \param[in] matrixValueType The matrix type to determine the base type from.
             *
             * \return Returns the underlying value type for the matrix.
             */
            static ValueType baseTypeFromMatrixType(ValueType matrixValueType);

            /**
             * Method you can call to translate a base type to a matrix type.
             *
             * \param[in] baseType The base to to determine a matrix type for.
             *
             * \return Returns the matrix type.
             */
            static ValueType matrixTypeFromBaseType(ValueType baseType);

            /**
             * Method you can use to obtain a decoder for a specific data-type.
             *
             * \param[in] valueType  The value type this translator works with.
             *
             * \return Returns a pointer to the requested decoder.
             */
            static const DataTypeDecoder* decoder(ValueType valueType);

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            DataType& operator=(const DataType& other);

            /**
             * Comparison operator
             *
             * \param[in] other The instance to compare to this instance.
             *
             * \return Returns true if the instances represent the same type.  Returns false if the instances represent
             *         different types.
             */
            bool operator==(const DataType& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The instance to compare to this instance.
             *
             * \return Returns true if the instances represent different types.  Returns false if the instances
             *         represent the same type.
             */
            bool operator!=(const DataType& other) const;

        private:
            /**
             * Private constructor.
             *
             * \param[in] valueType             The value type this translator works with.
             *
             * \param[in] description           A description of this data type.
             *
             * \param[in] typeSymbolElementName The name of the symbol used to define this data-type.
             *
             * \param[in] properties            Properties to apply to the data type.
             *
             * \return Returns true if this data-type was registered.  Returns false if a data-type with the same
             *         type-name already exists.
             */
            DataType(
                ValueType      valueType,
                const QString& description,
                const QString& typeSymbolElementName,
                Property       properties = defaultProperties
            );

            /**
             * Private implementation class.
             */
            class Private;

            /**
             * Static value used to return invalid data types.
             */
            static const DataType invalidDataType;

            /**
             * Static value holding am empty string.
             */
            static const QString emptyString;

            /**
             * Static value holding an empty datatype list.
             */
            static const QList<DataType> emptyList;

            /**
             * Hash of data types by value type.
             */
            static QHash<ValueType, DataType> dataTypesByValueType;

            /**
             * Hash of data types by element name.
             */
            static QHash<QString, DataType> dataTypesByElement;

            /**
             * Hash of matrix data types by element name.
             */
            static QHash<QString, DataType> matrixDataTypesByElement;

            /**
             * List of all data types.
             */
            static QList<DataType> allDataTypes;

            /**
             * The default data-type.
             */
            static DataType currentDefaultDataType;

            /**
             * The shared private implementation.
             */
            QSharedPointer<Private> impl;
    };

    /**
     * Hashing function for use with the \ref Ld::DataType class.  This function exists to support the Qt QSet and
     * QHash classes.
     *
     *  param[in] entry The entry to calculate a hash for.
     *
     *  param[in] seed  An optional seed value.
     *
     * \return Returns a hash for the provided seed.
     */
    LD_PUBLIC_API Util::HashResult qHash(const DataType& entry, Util::HashSeed seed = 0);
};

namespace Model {
    /**
     * Hashing function for use with the \ref Ld::DataType::ValueType enumeration.  This function exists to support
     * the Qt QSet and QHash classes.
     *
     *  param[in] valueType The entry to calculate a hash for.
     *
     *  param[in] seed      An optional seed value.
     *
     * \return Returns a hash for the provided seed.
     */
    LD_PUBLIC_API Util::HashResult qHash(ValueType valueType, Util::HashSeed seed = 0);
}

#endif
