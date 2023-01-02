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
* This header defines the \ref Ld::VariableName class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_VARIABLE_NAME_H
#define LD_VARIABLE_NAME_H

#include <QString>

#include <util_hash_functions.h>

#include "ld_common.h"

namespace Ld {
    /**
     * Class used to pass a variable name as a singular value.
     */
    class LD_PUBLIC_API VariableName {
        public:
            VariableName();

            /**
             * Constructor
             *
             * \param[in] text1 The first text in the variable name.
             *
             * \param[in] text2 Optional subscript for the variable name.
             */
            explicit VariableName(const QString& text1, const QString& text2 = QString());

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            VariableName(const VariableName& other);

            ~VariableName();

            /**
             * Method you can use to determine if the variable name is valid.
             *
             * \return Returns true if the variable name is valid.  Returns false if the variable name is invalid.
             */
            bool isValid() const;

            /**
             * Method you can use to determine if the variable name is invalid.
             *
             * \return Returns true if the variable name is invalid.  Returns false if the variable name is valid.
             */
            bool isInvalid() const;

            /**
             * Method you can use to set the first text parameter.
             *
             * \param[in] newText1 The new text of the variable name.
             */
            void setText1(const QString& newText1);

            /**
             * Method you can use to obtain the first text parameter.
             *
             * \return Returns the first text parameter.
             */
            QString text1() const;

            /**
             * Method you can use to set the second text parameter.
             *
             * \param[in] newText2 The new text of the variable name.
             */
            void setText2(const QString& newText2);

            /**
             * Method you can use to obtain the second text parameter.
             *
             * \return Returns the second text parameter.
             */
            QString text2() const;

            /**
             * Assignment operator
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            VariableName& operator=(const VariableName& other);

            /**
             * Comparison operator
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instances are equal.  Returns false if the instances are not equal.
             */
            bool operator==(const VariableName& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instances are not equal.  Returns false if the instances are equal.
             */
            bool operator!=(const VariableName& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if this instance should precede the other instance.  Returns false if the instances
             *         are equal or this instance should follow the other instance.
             */
            bool operator<(const VariableName& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if this instance should follow the other instance.  Returns false if the instances
             *         are equal or this instance should precede the other instance.
             */
            bool operator>(const VariableName& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if this instance should precede the other instance or if the instances are equal.
             *         Returns false if this instance follows the other instance.
             */
            bool operator<=(const VariableName& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if this instance should precede the other instance or if the instances are equal.
             *         Returns false if this instance follows the other instance.
             */
            bool operator>=(const VariableName& other) const;

        private:
            /**
             * The first text field.
             */
            QString currentText1;

            /**
             * The second text field.
             */
            QString currentText2;
    };

    /**
     * Hashing function for use with the \ref Ld::VariableName class.  This function exists to support the Qt QSet and
     * QHash classes.
     *
     * \param[in] entry The entry to calculate a hash for.
     *
     * \param[in] seed  An optional seed value.
     *
     * \return Returns a hash for the provided seed.
     */
    LD_PUBLIC_API Util::HashResult qHash(const VariableName& entry, Util::HashSeed seed = 0);
};

#endif
