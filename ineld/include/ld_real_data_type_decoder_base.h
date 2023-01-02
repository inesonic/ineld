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
* This header defines the \ref Ld::RealDataTypeDecoderBase class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_REAL_DATA_TYPE_DECODER_BASE_H
#define LD_REAL_DATA_TYPE_DECODER_BASE_H

#include <QCoreApplication>
#include <QString>

#include <model_intrinsic_types.h>

#include "ld_common.h"
#include "ld_format_structures.h"
#include "ld_data_type_decoder.h"

namespace Ld {
    class RealDataTypeFormat;

    /**
     * Base class providing several common private methods for the \ref Ld::RealDataTypeDecoder and
     * \ref Ld::ComplexDataTypeDecoder classes.
     */
    class LD_PUBLIC_API RealDataTypeDecoderBase:public virtual DataTypeDecoder {
        Q_DECLARE_TR_FUNCTIONS(Ld::RealDataTypeDecoderBase)

        public:
            RealDataTypeDecoderBase();

            ~RealDataTypeDecoderBase() override;

        protected:
            /**
             * Method you can overload to create a textual representation of a real value.
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
            static QString realToString(Model::Real value, QSharedPointer<Ld::RealDataTypeFormat> format);
    };
};

#endif
