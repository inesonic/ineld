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
* This header defines the \ref Ld::TableColumnWidth class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_TABLE_COLUMN_WIDTH_H
#define LD_TABLE_COLUMN_WIDTH_H

#include <QList>
#include <QString>

#include <cstdint>

#include <util_units.h>
#include <util_hash_functions.h>

#include "ld_common.h"

namespace Ld {
    /**
     * Trivial class that holds information describing the width of a table column.
     */
    class LD_PUBLIC_API TableColumnWidth {
        public:
            /**
             * Enumeration of supported column width adjustments.
             */
            enum class WidthType:std::uint8_t {
                /**
                 * The column width is invalid.
                 */
                INVALID = 0,

                /**
                 * The column should be fixed width.
                 */
                FIXED = 1,

                /**
                 * The column's width should be a proportion of the total remaining space.  The actual column width is
                 * calculated by first assigning widths to the fixed width columns and then assigning the specified
                 * proportion of the remaining space to this column.
                 */
                PROPORTIONAL = 2
            };

            /**
             * Epsilon used to compare fixed column width values.
             */
            static const float fixedColumnWidthEpsilon;

            /**
             * Epsilon used to compare proportional column width values.
             */
            static const float proportionalColumnWidthEpsilon;

            /**
             * Constructor
             *
             * \param[in] newWidthType Indicates the method used to determine the width of the column.
             *
             * \param[in] newWidth     The width value to apply.  Note that the interpretation of the value depends on
             *                         the widthType parameter.  If the width type is \ref WidthType::FIXED, then the
             *                         value represents the column width, in points.  If the width type is
             *                         \ref WidthType::PROPORTIONAL, then the value represents the fraction of the
             *                         remaining space to allocate to this column.
             */
            TableColumnWidth(WidthType newWidthType, float newWidth = 0);

            TableColumnWidth();

            /**
             * Copy constructor.
             *
             * \param[in] other The instance to be copied.
             */
            TableColumnWidth(const TableColumnWidth& other);

            ~TableColumnWidth();

            /**
             * Method you can call to determine if this column width is valid or invalid.
             *
             * \return Returns true if the column width is valid.  Returns false if the column width is invalid.
             */
            bool isValid() const;

            /**
             * Method you can call to determine if this column width is invalid or valid.
             *
             * \return Returns true if the column width is invalid.  Returns false if the column width is valid.
             */
            bool isInvalid() const;

            /**
             * Method you can use to set the width type.
             *
             * \param[in] newWidthType The new width type.
             */
            void setWidthType(WidthType newWidthType);

            /**
             * Method you can use to determine the width type parameter.  This value determines how the column width
             * should be calculated.
             *
             * \return Returns the width type value.
             */
            WidthType widthType() const;

            /**
             * Method you can use to set the width value for the column.
             *
             * \param[in] newWidth The new width value.
             */
            void setWidth(float newWidth);

            /**
             * Method you can use to determine the width value for the column.
             *
             * \return Returns the width value.
             */
            float width() const;

            /**
             * Method you can use to set the table column width.
             *
             * \param[in] newWidthType The new width type value.
             *
             * \param[in] newWidth     The new width value.
             *
             * \return Returns true if the values are valid.  Returns false if the values are invalid.
             */
            bool setWidth(WidthType newWidthType, float newWidth);

            /**
             * Method that returns CSS content that can be used to emulate this format.  The returned string contains
             * no newline and does not include leading or trailing braces.
             *
             * \return Returns CSS content comparable to this format.
             */
            QString toCss() const;

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to be copied.
             */
            TableColumnWidth& operator=(const TableColumnWidth& other);

            /**
             * Comparison operator.  Note that the comparison operator will compare values within a fixed espilon
             * value based on the column width type.
             *
             * \param[in] other The instance to be compared.
             *
             * \return Returns true if the instances are equal.  Returns false if the instances are different.
             */
            bool operator==(const TableColumnWidth& other) const;

            /**
             * Comparison operator.  Note that the comparison operator will compare values within a fixed espilon
             * value based on the column width type.
             *
             * \param[in] other The instance to be compared.
             *
             * \return Returns true if the instances are not equal.  Returns false if the instances are identical.
             */
            bool operator!=(const TableColumnWidth& other) const;

        private:
            /**
             * The current width type.
             */
            WidthType currentWidthType;

            /**
             * The current width value.
             */
            float currentWidth;
    };

    /**
     * Function that calculates a hash for the \ref Ld::TableColumnWidth class.
     *
     * \param[in] key  The key to calculate a hash for.
     *
     * \param[in] seed An optional seed used to add entropy to the hash.
     */
    LD_PUBLIC_API Util::HashResult qHash(const Ld::TableColumnWidth& key, Util::HashSeed seed = 0);
}

/**
 * Function that calculates a hash for a list of \ref Ld::TableColumnWidth class instances.
 *
 * \param[in] key  The key to calculate a hash for.
 *
 * \param[in] seed An optional seed used to add entropy to the hash.
 */
LD_PUBLIC_API Util::HashResult qHash(const QList<Ld::TableColumnWidth>& key, Util::HashSeed seed = 0);

#endif
