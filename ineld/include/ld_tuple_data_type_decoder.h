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
* This header defines the \ref Ld::TupleDataTypeDecoder class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_TUPLE_DATA_TYPE_DECODER_H
#define LD_TUPLE_DATA_TYPE_DECODER_H

#include <QCoreApplication>
#include <QString>

#include <model_tuple.h>
#include <model_variant.h>

#include "ld_common.h"
#include "ld_format_structures.h"
#include "ld_data_type_decoder.h"

namespace Ld {
    class DataTypeFormat;

    /**
     * Class you can use to decode tuple values of the type Model::Tuple.  Note that the Model::Tuple type acts as a
     * container so this decoder will trigger other decoders to render the contents of the tuple
     */
    class LD_PUBLIC_API TupleDataTypeDecoder:public virtual DataTypeDecoder {
        Q_DECLARE_TR_FUNCTIONS(Ld::TupleDataTypeDecoder)

        public:
            TupleDataTypeDecoder();

            ~TupleDataTypeDecoder() override;

            /**
             * Method you can overload to create a textual representation of a value of this data-type.
             *
             * \param[in] value   A variant type containing the value to be translated.
             *
             * \param[in] format  Format information used to control how the data-type is translated.  Note that the
             *                    translation engine must be able to perform a reasonable translation even if a format
             *                    has not been supplied.
             *
             * \return Returns the value translated to a string.  An empty string is returned if the translation could
             *         not be performed.
             */
            QString toString(const Model::Variant& value, Ld::FormatPointer format) const override;

            /**
             * Method you can overload to create a brief textual description of a value of this data type.
             *
             * \param[in] value A variant type containing the value to generate the description for.
             *
             * \return Returns a brief (one line) description.  The description will exclude the actual value.
             */
            QString toDescription(const Model::Variant& value) const override;

        private:
            /**
             * Method that inserts a specified number of entries at a provided tuple iterator position.
             *
             * \param[in,out] str      The string to receive the newly added entries.
             *
             * \param[in,out] isFirst  A boolean value used to track whether we've inserted an entry.  The flag will
             *                         be cleared if we insert one or more entries.
             *
             * \param[in,out] iterator The iterator to be used to locate entries.  The iterator will be advanced by
             *                         this operation.
             *
             * \param[in]     count    The number of entries we want to insert.
             *
             * \param[in]     format   The format to apply to child values.
             */
            static void insertFromTuple(
                QString&                     str,
                bool&                        isFirst,
                Model::Tuple::ConstIterator& iterator,
                unsigned long                count,
                FormatPointer                format
            );
    };
};

#endif
