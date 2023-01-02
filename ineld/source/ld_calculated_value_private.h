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
* This header defines the \ref Ld::CalculatedValue::Private private class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CALCULATED_VALUE_PRIVATE_H
#define LD_CALCULATED_VALUE_PRIVATE_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS
#include <QString>

#include <model_variant.h>

#include "ld_common.h"
#include "ld_data_type.h"
#include "ld_variable_name.h"
#include "ld_calculated_value.h"

namespace Ld {
    /**
     * Private implementation of the \ref CalculatedValue class.
     */
    class LD_PUBLIC_API CalculatedValue::Private:public Model::Variant {
        Q_DECLARE_TR_FUNCTIONS(CalculatedValue::Private)

        public:
            /**
             * Constructor
             *
             * \param[in] name    The name of the calculated value.
             *
             * \param[in] variant The variant we should use to initialize this calculated value instance.
             */
            Private(const Ld::VariableName& name, const Model::Variant& variant);

            /**
             * Method you can use to determine if this calculated value is valid.
             *
             * \return Returns true if the calculated value is valid.  Returns false if the calculated value is not
             *         valid.
             */
            bool isValid() const;

            /**
             * Method you can use to obtain the name of the calculated value.
             *
             * \return Returns the name of the calculated value.
             */
            const Ld::VariableName& name() const;

            /**
             * Method you can use to obtain the data type tied to this calculated value.
             *
             * \return Returns the data type tied to this calculated value.
             */
            const Ld::DataType& dataType() const;

            /**
             * Method you can use to obtain a description of the calculated value as a string.
             *
             * \return Returns a brief description of the calculated value as a string.
             */
            const QString& description() const;

            /**
             * Method you can use to obtain a debug representation of the calculated value as a string.
             *
             * \return Returns a debug version of the calculated value as a string.  This is a multi-line string that
             *         contains the description plus a type dependent representation of the current value.
             */
            const QString& debugString() const;

            /**
             * Method you can use to obtain a more detailed single line representation of the calculated value.
             *
             * \return Returns a single line detailed description of the calculated value. This string is similar to
             *          the value returned by \ref Ld::CalculatedValue::debugString but occupies only a single line.
             */
            const QString& detailedDescription() const;

        private:
            /**
             * The variable name.
             */
            Ld::VariableName currentVariableName;

            /**
             * The underlying datatype.
             */
            mutable DataType currentDataType;

            /**
             * The description string.
             */
            mutable QString currentDescription;

            /**
             * The debug string.
             */
            mutable QString currentDebugString;

            /**
             * The detailed description.
             */
            mutable QString currentDetailedDescription;
    };
}

#endif
