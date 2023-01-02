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
* This header defines the \ref Ld::IntegerDataTypeDecoder class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_INTEGER_DATA_TYPE_DECODER_H
#define LD_INTEGER_DATA_TYPE_DECODER_H

#include <QCoreApplication>
#include <QString>

#include <model_variant.h>

#include "ld_common.h"
#include "ld_format_structures.h"
#include "ld_data_type_decoder.h"

namespace Ld {
    class DataTypeFormat;

    /**
     * Class you can use to decode integer values of the type Model::Integer
     */
    class LD_PUBLIC_API IntegerDataTypeDecoder:public virtual DataTypeDecoder {
        Q_DECLARE_TR_FUNCTIONS(Ld::IntegerDataTypeDecoder)

        public:
            IntegerDataTypeDecoder();

            ~IntegerDataTypeDecoder() override;

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
    };
};

#endif
