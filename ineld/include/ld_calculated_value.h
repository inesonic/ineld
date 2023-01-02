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
* This header defines the \ref Ld::CalculatedValue class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CALCULATED_VALUE_H
#define LD_CALCULATED_VALUE_H

#include <QString>
#include <QSharedPointer>

#include <model_variant.h>

#include "ld_common.h"
#include "ld_data_type.h"
#include "ld_variable_name.h"

namespace Ld {
    /**
     * Class that holds a calculated value returned from a model.  The class provides the capabilities of the model's
     * variant type as well as a default calculated string representation of the data suitable for use in a user
     * interface.
     *
     * The class uses a PIMPl implementation to reduce memory usage and overhead during copying.
     */
    class LD_PUBLIC_API CalculatedValue {
        public:
            CalculatedValue();

            /**
             * Constructor
             *
             * \param[in] name    The name of the calculated value.
             *
             * \param[in] variant The variant we should use to initialize this calculated value instance.
             */
            CalculatedValue(const Ld::VariableName& name, const Model::Variant& variant);

            /**
             * Constructor
             *
             * \param[in] name    The name of the calculated value.
             *
             * \param[in] variant The variant we should use to initialize this calculated value instance.
             */
            CalculatedValue(const QString& name, const Model::Variant& variant);

            /**
             * Constructor
             *
             * \param[in] name1   The name of the calculated value.
             *
             * \param[in] name2   The name of the calculated value.
             *
             * \param[in] variant The variant we should use to initialize this calculated value instance.
             */
            CalculatedValue(const QString& name1, const QString& name2, const Model::Variant& variant);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            CalculatedValue(const CalculatedValue& other);

            /**
             * Method you can use to determine if this calculated value is valid.
             *
             * \return Returns true if the calculated value is valid.  Returns false if the calculated value is not
             *         valid.
             */
            bool isValid() const;

            /**
             * Method you can use to determine if this calculated value is invalid.
             *
             * \return Returns true if the calculated value is invalid.  Returns false if the calculated value is
             *         valid.
             */
            bool isInvalid() const;

            /**
             * Method you can use to obtain the name of the calculated value.
             *
             * \return Returns the name of the calculated value.
             */
            const Ld::VariableName& name() const;

            /**
             * Method you can use to obtain the name of the calculated value.
             *
             * \return Returns the name of the calculated value, string 1.
             */
            QString name1() const;

            /**
             * Method you can use to obtain the name of the calculated value.
             *
             * \return Returns the name of the calculated value, string 2.
             */
            QString name2() const;

            /**
             * Method you can use to obtain the type of the calculated value.
             *
             * \return Returns the type of the calculated value.
             */
            Ld::DataType::ValueType valueType() const;

            /**
             * Method you can use to obtain the variant associated with this calculated value.
             *
             * \return Returns a reference to the calculated value.
             */
            const Model::Variant& variant() const;

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

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            CalculatedValue& operator=(const CalculatedValue& other);

        private:
            /**
             * The internal storage type.
             */
            class Private;

            /**
             * The private instance.
             */
            QSharedPointer<Private> impl;
    };
}

#endif
