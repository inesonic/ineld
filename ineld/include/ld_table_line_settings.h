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
* This header defines the \ref Ld::TableLineSettings class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_TABLE_LINE_SETTINGS_H
#define LD_TABLE_LINE_SETTINGS_H

#include <QString>

#include <cstdint>

#include <util_hash_functions.h>

#include "ld_common.h"

namespace Ld {
    /**
     * Trivial class that holds information describing the width of a table column.
     */
    class LD_PUBLIC_API TableLineSettings {
        public:
            /**
             * Enumeration of supported line styles.
             */
            enum class Style:std::uint8_t {
                /**
                 * The line setting is invalid.
                 */
                INVALID = 0,

                /**
                 * Do not use a line.
                 */
                NO_LINE = 1,

                /**
                 * Use a single solid line.
                 */
                SINGLE = 2,

                /**
                 * Use double lines.
                 */
                DOUBLE = 3
            };

            /**
             * Enumeration of supported line positions.
             */
            enum class Position:std::uint8_t {
                /**
                 * All borders.
                 */
                ALL_POSITIONS = 0,

                /**
                 * Left border.
                 */
                LEFT_POSITION = 1,

                /**
                 * Right border.
                 */
                RIGHT_POSITION = 2,

                /**
                 * Top border.
                 */
                TOP_POSITION = 3,

                /**
                 * Bottom border.
                 */
                BOTTOM_POSITION = 4
            };

            /**
             * Epsilon used to compare line width values.
             */
            static const float lineWidthEpsilon;

            /**
             * Constructor
             *
             * \param[in] newLineStyle The line style to apply.
             *
             * \param[in] newWidth     The line width, in points.  This value represents the width of each line drawn
             *                         not the total width.
             */
            TableLineSettings(Style newLineStyle, float newWidth);

            TableLineSettings();

            /**
             * Copy constructor.
             *
             * \param[in] other The instance to be copied.
             */
            TableLineSettings(const TableLineSettings& other);

            ~TableLineSettings();

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
             * Method you can use to set the line style.
             *
             * \param[in] newLineStyle The new line style.
             */
            void setLineStyle(Style newLineStyle);

            /**
             * Method you can use to determine the desired line style.
             *
             * \return Returns the width type value.
             */
            Style lineStyle() const;

            /**
             * Method you can use to set the line width.
             *
             * \param[in] newWidth The new width value, in points.
             */
            void setWidth(float newWidth);

            /**
             * Method you can use to determine the width value for the line.
             *
             * \return Returns the width value, in points.
             */
            float width() const;

            /**
             * Method you can use to set the entire line style at once.
             *
             * \param[in] newLineStyle The new line style to apply.
             *
             * \param[in] newWidth     The new line width, in points.
             *
             * \return Returns true if the provided values are valid.  Returns false if the values are invalid.  The
             *         line settings will not be updated if invalid values are provided.
             */
            bool setLine(TableLineSettings::Style newLineStyle, float newWidth);

            /**
             * Method that returns CSS content that can be used to emulate this format.  The returned string contains
             * no newline and does not include leading or trailing braces.
             *
             * \param[in] position The border position.
             *
             * \return Returns CSS content comparable to this format.
             */
            QString toCss(Position position = Position::ALL_POSITIONS) const;

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to be copied.
             */
            TableLineSettings& operator=(const TableLineSettings& other);

            /**
             * Comparison operator.  Note that the comparison operator will compare values within a fixed espilon
             * value.
             *
             * \param[in] other The instance to be compared.
             *
             * \return Returns true if the instances are equal.  Returns false if the instances are different.
             */
            bool operator==(const TableLineSettings& other) const;

            /**
             * Comparison operator.  Note that the comparison operator will compare values within a fixed espilon
             * value.
             *
             * \param[in] other The instance to be compared.
             *
             * \return Returns true if the instances are not equal.  Returns false if the instances are identical.
             */
            bool operator!=(const TableLineSettings& other) const;

        private:
            /**
             * The current line style.
             */
            Style currentLineStyle;

            /**
             * The current width value.
             */
            float currentWidth;
    };

    /**
     * Function that calculates a hash for the \ref Ld::TableLineSettings class.
     *
     * \param[in] key  The key to calculate a hash for.
     *
     * \param[in] seed An optional seed used to add entropy to the hash.
     */
    LD_PUBLIC_API Util::HashResult qHash(const TableLineSettings& key, Util::HashSeed seed = 0);

    /**
     * Function that calculates a hash for the \ref Ld::TableLineSettings::Style enumerated value.
     *
     * \param[in] key  The key to calculate a hash for.
     *
     * \param[in] seed An optional seed used to add entropy to the hash.
     */
    LD_PUBLIC_API Util::HashResult qHash(TableLineSettings::Style key, Util::HashSeed seed = 0);
}

#endif
