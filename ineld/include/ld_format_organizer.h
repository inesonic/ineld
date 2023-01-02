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
* This header defines the \ref Ld::FormatOrganizer class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_FORMAT_ORGANIZER_H
#define LD_FORMAT_ORGANIZER_H

#include <QSharedPointer>
#include <QMap>
#include <QList>

#include "ld_common.h"
#include "ld_format_structures.h"
#include "ld_format.h"
#include "ld_format_container.h"

namespace Ld {
    class Format;

    /**
     * Class you can use to organize and track formats.  You can add formats to the class and requests lists of formats
     * based on specific criteria.
     *
     * Note that this class defines the concept of a format identifier which is a unique number for each unique format.
     * The identifiers are only unique so-long as new formats are not added to the list.
     */
    class LD_PUBLIC_API FormatOrganizer {
        public:
            FormatOrganizer();

            ~FormatOrganizer();

            /**
             * Method you can use to add a new format to the list of formats.
             *
             * \param[in] newFormatContainer The new format container to be added.
             */
            void addFormat(FormatContainer& newFormatContainer);

            /**
             * Method you can use to add a new format to the list of formats.
             *
             * \param[in] newFormat The new format to be added.
             */
            void addFormat(FormatPointer newFormat);

            /**
             * Method you can use to add a new format to the list of formats.
             *
             * \param[in] newFormat The new format to be added.
             */
            void addFormat(const Format* newFormat);

            /**
             * Method you can use to add a new format to the list of formats.
             *
             * \param[in] newFormat The new format to be added.
             */
            void addFormat(const Format& newFormat);

            /**
             * Method you can use to obtain an identifier for a specified format.
             *
             * \param[in] format A pointer to the format you wish an identifier for.
             *
             * \return Returns the identifier tied to this format.  A value of FormatOrganizer::invalidIdentifier is
             *         returned if the format is not known.
             */
            FormatIdentifier identifier(FormatPointer format);

            /**
             * Method you can use to obtain an identifier for a specified format.
             *
             * \param[in] format A pointer to the format you wish an identifier for.
             *
             * \return Returns the identifier tied to this format.  A value of FormatOrganizer::invalidIdentifier is
             *         returned if the format is not known.
             */
            FormatIdentifier identifier(const Format* format);

            /**
             * Method you can use to obtain a copy of a format from an identifier.n identifier for a specified format.
             *
             * \param[in] identifier A pointer to the format identifier you want the format for.
             *
             * \return Returns a shared pointer containing the desired format instance.  Do not modify this format
             *         instance.
             */
            FormatPointer createFormat(FormatIdentifier identifier);

            /**
             * Method you can use to obtain a mapping of format identifiers by format.
             *
             * \return Returns a mapping of format identifiers, by format.  The method will maintain a locally cached
             *         list and will only re-generate the list when needed.
             */
            const FormatIdentifiersByFormat& identifiersByFormat();

            /**
             * Method you can use to obtain a list of all unique formats by identifier number.  The list will be ordered
             * from most commonly referenced format to the least commonly referenced format.
             *
             * \return Returns a paredo of unique formats.  The index to the list can be used as a format identifier.
             */
            const FormatsByIdentifier& formatsByIdentifier();

            /**
             * Method you can use to obtain a mapping of paredos for each format by format identifier.
             *
             * \return Returns a mapping of paredos of unique formats for each type of format.
             */
            const FormatParedos& formatParedos();

        private:
            /**
             * Method that is called to update the map of identifiers by format.
             */
            void updateDataStructures();

            /**
             * Flag that indicates that the internal data structures need to be updated.
             */
            bool updateNeeded;

            /**
             * A map of all known, unique, formats.  Map is used rather than set/hash to avoid the need for a hash
             * function.
             */
            QMap<FormatContainer, unsigned long> uniqueFormats;

            /**
             * The current map of identifiers by format.
             */
            FormatIdentifiersByFormat currentIdentifiersByFormat;

            /**
             * The current map of formats by unique identifier.
             */
            FormatsByIdentifier currentFormatsByIdentifier;

            /**
             * The current format paredos.
             */
            FormatParedos currentFormatParedos;
    };
};

#endif
