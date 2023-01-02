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
* This header defines the \ref Ld::DataType::Private class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_DATA_TYPE_PRIVATE_H
#define LD_DATA_TYPE_PRIVATE_H

#include <QString>
#include <QList>
#include <QMap>

#include <model_api_types.h>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_data_type.h"

namespace Ld {
    class DataTypeTranslator;
    class DataTypeDecoder;

    /**
     * Private implementation of the \ref Ld::DataType class.
     */
    class LD_PUBLIC_API DataType::Private {
        public:
            /**
             * Constructor
             *
             *
             * \param[in] valueType             The value type tied to this data type.
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
            Private(
                DataType::ValueType valueType,
                const QString&      description,
                const QString&      typeSymbolElementName,
                DataType::Property  properties
            );

            ~Private();

            /**
             * Method you can use to obtain the value type tied to this data type.
             *
             * \return Returns the model implementation value type for this data type.
             */
            DataType::ValueType valueType() const;

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
            DataType::Property properties() const;

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
             * Method you can use to register a decoder with this data-type.
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
             * Method that is called to destroy the underlying decoder.
             */
            void destroyDecoder();

        private:
            /**
             * The data model value type.
             */
            DataType::ValueType currentValueType;

            /**
             * The description for the data type.
             */
            QString currentDescription;

            /**
             * The element name of the symbol used to represent this type.
             */
            QString currentTypeSymbolElementName;

            /**
             * The data type properties.
             */
            DataType::Property currentProperties;

            /**
             * Map of translators by generator name.
             */
            QMap<QString, const DataTypeTranslator*> currentTranslators;

            /**
             * The decoder tied to this data-type.
             */
            const DataTypeDecoder* currentDecoder;
    };
};

#endif
