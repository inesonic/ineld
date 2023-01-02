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
* This header defines the \ref Ld::SpecialCharacters class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_SPECIAL_CHARACTERS_H
#define LD_SPECIAL_CHARACTERS_H

#include <QCoreApplication>
#include <QMap>
#include <QHash>

#include <cstdint>

#include "ld_common.h"
#include "ld_capabilities.h"

namespace Ld {
    /**
     * Class that provides support capabilities for special characters.  Commands provided for characters are the
     * same commands used by LaTeX.  You can use the commands to map special unicode values to commands.
     */
    class LD_PUBLIC_API SpecialCharacters {
        Q_DECLARE_TR_FUNCTIONS(Ld::SpecialCharacters)

        public:
            /**
             * Type used to represent a unicode value.
             */
            typedef std::uint16_t UnicodeValue;

            /**
             * Value indicating an invalid unicode value.
             */
            static constexpr UnicodeValue invalidUnicodeValue = 0;

            /**
             * String holding the expected prefix to be placed in front of each command.
             */
            static const QString defaultPrefix;

            /**
             * Static method you can use to obtain a map of strings containing descriptions referenced by commands.
             *
             * \param[in] includeGreekSymbols       If true, the map will include Greek symbols.
             *
             * \param[in] includeDiacriticalSymbols If true, the map will include diacritical characters and symbols.
             *
             * \param[in] includeSpecialCharacters  If true, the map will include special character symbols such as
             *                                      Plank's constant, Furthark eth's, etc.
             *
             * \param[in] includeSpecialSymbols     If true, the map will include special non-character symbols such as
             *                                      Planck's constant, infinity, etc.
             *
             * \return Returns a map of descriptions indexed by command.
             */
            static QMap<QString, QString> characterDescriptionsByCommand(
                bool includeGreekSymbols,
                bool includeDiacriticalSymbols,
                bool includeSpecialCharacters,
                bool includeSpecialSymbols
            );

            /**
             * Static method you can use to obtain a map of descriptions of special characters, by unicode value.
             *
             * \param[in] includeGreekSymbols       If true, the map will include Greek symbols.
             *
             * \param[in] includeDiacriticalSymbols If true, the map will include diacritical characters and symbols.
             *
             * \param[in] includeSpecialCharacters  If true, the map will include special character symbols such as
             *                                      Plank's constant, Furthark eth's, etc.
             *
             * \param[in] includeSpecialSymbols     If true, the map will include special non-character symbols such as
             *                                      Planck's constant, infinity, etc.
             *
             * \return Returns a map of descriptions indexed by unicode value.
             */
            static QMap<UnicodeValue, QString> characterDescriptionsByUnicodeValue(
                bool includeGreekSymbols,
                bool includeDiacriticalSymbols,
                bool includeSpecialCharacters,
                bool includeSpecialSymbols
            );

            /**
             * Static method you can use to obtain a map of commands for special characters, by unicode value.
             *
             * \param[in] includeGreekSymbols       If true, the map will include Greek symbols.
             *
             * \param[in] includeDiacriticalSymbols If true, the map will include diacritical characters and symbols.
             *
             * \param[in] includeSpecialCharacters  If true, the map will include special character symbols such as
             *                                      Plank's constant, Furthark eth's, etc.
             *
             * \param[in] includeSpecialSymbols     If true, the map will include special non-character symbols such as
             *                                      Planck's constant, infinity, etc.
             *
             * \return Returns a map of descriptions indexed by unicode value.
             */
            static QMap<UnicodeValue, QString> characterCommandsByUnicodeValue(
                bool includeGreekSymbols,
                bool includeDiacriticalSymbols,
                bool includeSpecialCharacters,
                bool includeSpecialSymbols
            );

            /**
             * Static method you can use to obtain the unicode value associated with a special symbol, by command.
             *
             * \param[in] commandString The command to obtain a unicode value for.
             *
             * \return Returns the unicode value.  A value of 0 is returned if the command string is unknown.
             */
            static UnicodeValue unicodeForCommand(const QString& commandString);

            /**
             * Static method you can use to obtain a recommended ordering for characters.
             *
             * \param[in] includeGreekSymbols       If true, the list will include Greek symbols.
             *
             * \param[in] includeDiacriticalSymbols If true, the list will include diacritical characters and symbols.
             *
             * \param[in] includeSpecialCharacters  If true, the map will include special character symbols such as
             *                                      Plank's constant, Furthark eth's, etc.
             *
             * \param[in] includeSpecialSymbols     If true, the list will include special non-character symbols such
             *                                      as Planck's constant, infinity, etc.
             *
             * \return Returns an ordered list of unicode values.
             */
            static QList<UnicodeValue> recommendedCharacterOrder(
                bool includeGreekSymbols,
                bool includeDiacriticalSymbols,
                bool includeSpecialCharacters,
                bool includeSpecialSymbols
            );

            /**
             * Static method you can use to report the capabilities required by a string.
             *
             * \param[in] text The text to determine the requirements for.
             *
             * \return Returns the capabilities needed to support the string.
             */
            static Capabilities requiredCapabilities(const QString& text);

            /**
             * Static method you can use to determine if a string meets a set of requirements.
             *
             * \param[in] text         The text to determine the requirements for.
             *
             * \param[in] requirements The requirements that the test must meet.
             *
             * \return Returns true if the text meets the requirements.  Returns false if the text does not meet the
             *         requirements.
             */
            static bool textMeetsRequirements(const QString& text, const Capabilities& requirements);

        private:
            /**
             * Method that builds the internal character database, if needed.
             */
            static void prepareDatabase();

            /**
             * Method that prepares text capabilities by character for the latin 1 character set.
             */
            static void prepareCapabilitiesByCharacter();

            /**
             * Method that merges the content of one map into another map.
             *
             * \param[in,out] toBeUpdated The map to be updated.
             *
             * \param[in]     source      The map containing the data to be merged.
             *
             * \return returns a referenced to the resulting map.
             */
            static QMap<QString, QString>& mergeMap(
                QMap<QString, QString>&       toBeUpdated,
                const QMap<QString, QString>& source
            );

            /**
             * Method that merges the content of one map into another map.
             *
             * \param[in,out] toBeUpdated The map to be updated.
             *
             * \param[in]     source      The map containing the data to be merged.
             *
             * \return returns a referenced to the resulting map.
             */
            static QMap<UnicodeValue, QString>& mergeMap(
                QMap<UnicodeValue, QString>&       toBeUpdated,
                const QMap<UnicodeValue, QString>& source
            );

            /**
             * Map containing character descriptions by command for Greek symbols.
             */
            static QMap<QString, QString> currentGreekDescriptionsByCommand;

            /**
             * Map containing character descriptions by command for diacritical characters and symbols.
             */
            static QMap<QString, QString> currentDiacriticalDescriptionsByCommand;

            /**
             * Map containing character descriptions by command for special characters.
             */
            static QMap<QString, QString> currentSpecialCharactersDescriptionsByCommand;

            /**
             * Map containing character descriptions by command for special symbols.
             */
            static QMap<QString, QString> currentSpecialSymbolsDescriptionsByCommand;

            /**
             * Map containing character descriptions by unicode value for Greek symbols.
             */
            static QMap<UnicodeValue, QString> currentGreekDescriptionsByUnicodeValue;

            /**
             * Map containing character descriptions by unicode value for diacritical characters and symbols.
             */
            static QMap<UnicodeValue, QString> currentDiacriticalDescriptionsByUnicodeValue;

            /**
             * Map containing character descriptions by unicode value for special characters.
             */
            static QMap<UnicodeValue, QString> currentSpecialCharactersDescriptionsByUnicodeValue;

            /**
             * Map containing character descriptions by unicode value for special symbols.
             */
            static QMap<UnicodeValue, QString> currentSpecialSymbolsDescriptionsByUnicodeValue;

            /**
             * Map containing character commands by unicode value for Greek symbols.
             */
            static QMap<UnicodeValue, QString> currentGreekCommandsByUnicodeValue;

            /**
             * Map containing character commands by unicode value for diacritical characters and symbols.
             */
            static QMap<UnicodeValue, QString> currentDiacriticalCommandsByUnicodeValue;

            /**
             * Map containing character commands by unicode value for special characters.
             */
            static QMap<UnicodeValue, QString> currentSpecialCharactersCommandsByUnicodeValue;

            /**
             * Map containing character commands by unicode value for special symbols.
             */
            static QMap<UnicodeValue, QString> currentSpecialSymbolsCommandsByUnicodeValue;

            /**
             * Hash used to identify the unicode value for a command.
             */
            static QHash<QString, UnicodeValue> currentUnicodeValuesByCommand;

            /**
             * List containing text requirements indexed by character.
             */
            static QList<Capabilities> currentLatin1TextCapabilitiesByUnicodeValue;
    };
}

#endif
