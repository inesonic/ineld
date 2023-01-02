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
* This file implements the \ref Ld::Handle class.
***********************************************************************************************************************/

#include <cstdint>
#include <string>
#include <ctime>
#include <cassert>
#include <cstring>

#include <util_hash_functions.h>

#include "ld_handle.h"

namespace Ld {
    std::uint32_t Handle::nextLower = 0;
    std::uint64_t Handle::nextUpper = 0;

    Handle::Handle() {
        lower = 0;
        upper = 0;
    }


    Handle::Handle(const Handle& other){
        lower = other.lower;
        upper = other.upper;
    }

    Handle::~Handle() {}

    bool Handle::isValid() const {
        return upper != 0;
    }


    bool Handle::isInvalid() const {
        return !isValid();
    }


    void Handle::initialize(std::uint64_t uniqueSystemId) {
        struct std::tm timeReference;

        std::memset(&timeReference, 0, sizeof(timeReference));
        timeReference.tm_sec  =  0;
        timeReference.tm_min  =  0;
        timeReference.tm_hour =  8;
        timeReference.tm_mday =  7;
        timeReference.tm_mon  = 12;
        timeReference.tm_year = 2015 - 1900;

        std::time_t timeBase   = std::mktime(&timeReference);
        std::time_t systemTime = std::time(nullptr);

        std::uint64_t timeOffset     = static_cast<std::uint64_t>(0.5 + std::difftime(systemTime, timeBase) / 20.0);
        std::uint64_t mungedSystemId = (uniqueSystemId >> 39) ^ uniqueSystemId;

        nextUpper = timeOffset ^ (mungedSystemId << 25);
        nextLower = 0;
    }


    Handle Handle::create() {
        assert(nextUpper != 0);

        Handle handle;

        handle.lower = nextLower;
        handle.upper = nextUpper;

        ++nextLower;
        if (nextLower == 0) {
            ++nextUpper;
        }

        return handle;
    }


    std::string Handle::toString() const {
        std::uint8_t packed[12];

        packed[ 0] = static_cast<std::uint8_t>(lower      );
        packed[ 1] = static_cast<std::uint8_t>(lower >>  8);
        packed[ 2] = static_cast<std::uint8_t>(lower >> 16);
        packed[ 3] = static_cast<std::uint8_t>(lower >> 24);
        packed[ 4] = static_cast<std::uint8_t>(upper      );
        packed[ 5] = static_cast<std::uint8_t>(upper >>  8);
        packed[ 6] = static_cast<std::uint8_t>(upper >> 16);
        packed[ 7] = static_cast<std::uint8_t>(upper >> 24);
        packed[ 8] = static_cast<std::uint8_t>(upper >> 32);
        packed[ 9] = static_cast<std::uint8_t>(upper >> 40);
        packed[10] = static_cast<std::uint8_t>(upper >> 48);
        packed[11] = static_cast<std::uint8_t>(upper >> 56);

        // 12 bytes * 8 bits/byte = 96 bits
        // 96 bits / (6 bits/encode) = 16 characters

        char  encoded[17];

        for (unsigned encodedIndex=0 ; encodedIndex<16 ; ++encodedIndex) {
            unsigned packedBitOffset = encodedIndex * 6;
            unsigned byteLocation    = packedBitOffset / 8;
            unsigned startBitInByte  = packedBitOffset % 8;

            unsigned char unpacked;
            if (startBitInByte <= 2) {
                unpacked = (packed[byteLocation] >> startBitInByte) & 0x3F;
            } else {
                std::uint16_t packedBits = (
                      packed[byteLocation]
                    | (static_cast<std::uint16_t>(packed[byteLocation+1]) << 8)
                );
                unpacked = static_cast<char>((packedBits >> startBitInByte) & 0x3F);
            }

            // Encode as per RFC 4648 (or MIME, same in this case) -- Rely on fixed length to simplify things somewhat.

            if (unpacked < 26) {
                encoded[encodedIndex] = unpacked -  0 + 'A';
            } else if (unpacked < 52) {
                encoded[encodedIndex] = unpacked - 26 + 'a';
            } else if (unpacked < 62) {
                encoded[encodedIndex] = unpacked - 52 + '0';
            } else if (unpacked == 62) {
                encoded[encodedIndex] = '+';
            } else {
                encoded[encodedIndex] = '/';
            }
        }

        encoded[16] = '\0';
        return std::string(encoded);
    }


    QString Handle::toQString() const {
        return QString::fromStdString(toString());
    }


    std::string Handle::toCaseInsensitiveString() const {
        std::uint8_t characterValues[20];

        characterValues[19] = static_cast<std::uint8_t>(lower      ) & 0x1F;
        characterValues[18] = static_cast<std::uint8_t>(lower >>  5) & 0x1F;
        characterValues[17] = static_cast<std::uint8_t>(lower >> 10) & 0x1F;
        characterValues[16] = static_cast<std::uint8_t>(lower >> 15) & 0x1F;
        characterValues[15] = static_cast<std::uint8_t>(lower >> 20) & 0x1F;
        characterValues[14] = static_cast<std::uint8_t>(lower >> 25) & 0x1F;
        characterValues[13] = (
              (static_cast<std::uint8_t>(lower >> 30) & 0x03)
            | (static_cast<std::uint8_t>(upper <<  2) & 0x1C)
        );
        characterValues[12] = static_cast<std::uint8_t>(upper >>  3) & 0x1F;
        characterValues[11] = static_cast<std::uint8_t>(upper >>  8) & 0x1F;
        characterValues[10] = static_cast<std::uint8_t>(upper >> 13) & 0x1F;
        characterValues[ 9] = static_cast<std::uint8_t>(upper >> 18) & 0x1F;
        characterValues[ 8] = static_cast<std::uint8_t>(upper >> 23) & 0x1F;
        characterValues[ 7] = static_cast<std::uint8_t>(upper >> 28) & 0x1F;
        characterValues[ 6] = static_cast<std::uint8_t>(upper >> 33) & 0x1F;
        characterValues[ 5] = static_cast<std::uint8_t>(upper >> 38) & 0x1F;
        characterValues[ 4] = static_cast<std::uint8_t>(upper >> 43) & 0x1F;
        characterValues[ 3] = static_cast<std::uint8_t>(upper >> 48) & 0x1F;
        characterValues[ 2] = static_cast<std::uint8_t>(upper >> 53) & 0x1F;
        characterValues[ 1] = static_cast<std::uint8_t>(upper >> 58) & 0x1F;
        characterValues[ 0] = static_cast<std::uint8_t>(upper >> 63) & 0x01;

        char encodedString[21];

        for (unsigned index=0 ; index<20 ; ++index) {
            std::uint8_t value = characterValues[index];
            char         encoded;
            if (value < 26) {
                encoded = 'a' + value;
            } else {
                encoded = '0' + value - 26;
            }

            encodedString[index] = encoded;
        }

        encodedString[20] = '\0';
        return std::string(encodedString);
    }


    QString Handle::toCaseInsensitiveQString() const {
        return QString::fromStdString(toCaseInsensitiveString());
    }


    Handle Handle::fromString(const std::string& string) {
        Handle handle;

        if (string.length() == 16) {
            bool success = true;

            // Decode as per RFC 4648.

            std::uint8_t packed[12];
            std::memset(packed, 0, 12);

            unsigned     encodedIndex = 0;
            do {
                std::uint16_t unpacked;
                char          encoded = string.at(encodedIndex);

                if (encoded >= 'A' && encoded <= 'Z') {
                    unpacked = encoded - 'A';
                } else if (encoded >= 'a' && encoded <= 'z') {
                    unpacked = encoded - 'a' + 26;
                } else if (encoded >= '0' && encoded <= '9') {
                    unpacked = encoded - '0' + 52;
                } else if (encoded == '+') {
                    unpacked = 62;
                } else if (encoded == '/') {
                    unpacked = 63;
                } else {
                    success = false;
                }

                if (success) {
                    unsigned packedBitOffset = encodedIndex * 6;
                    unsigned packedByteOffset = packedBitOffset / 8;
                    unsigned packedBitInByte  = packedBitOffset % 8;

                    if (packedBitInByte <= 2) {
                        // Fits into one byte.
                        packed[packedByteOffset] |= unpacked << packedBitInByte;
                    } else {
                        // Spans across bytes.
                        std::uint16_t unpackedBits = unpacked << packedBitInByte;
                        packed[packedByteOffset + 0] |= static_cast<uint8_t>(unpackedBits);
                        packed[packedByteOffset + 1] |= static_cast<uint8_t>(unpackedBits >> 8);
                    }

                    ++encodedIndex;
                }
            } while (encodedIndex < 16 && success);

            if (success) {
                handle.lower = (
                      (static_cast<std::uint32_t>(packed[0]) & 0xFF)
                    | (static_cast<std::uint32_t>(packed[1]) & 0xFF) <<  8
                    | (static_cast<std::uint32_t>(packed[2]) & 0xFF) << 16
                    | (static_cast<std::uint32_t>(packed[3]) & 0xFF) << 24
                );

                handle.upper = (
                      (static_cast<std::uint64_t>(packed[ 4]) & 0xFF)
                    | (static_cast<std::uint64_t>(packed[ 5]) & 0xFF) <<  8
                    | (static_cast<std::uint64_t>(packed[ 6]) & 0xFF) << 16
                    | (static_cast<std::uint64_t>(packed[ 7]) & 0xFF) << 24
                    | (static_cast<std::uint64_t>(packed[ 8]) & 0xFF) << 32
                    | (static_cast<std::uint64_t>(packed[ 9]) & 0xFF) << 40
                    | (static_cast<std::uint64_t>(packed[10]) & 0xFF) << 48
                    | (static_cast<std::uint64_t>(packed[11]) & 0xFF) << 56
                );
            }
        }

        return handle;
    }


    Handle Handle::fromString(const QString& string) {
        return Handle::fromString(string.toStdString());
    }


    Handle& Handle::operator=(const Handle& other) {
        lower = other.lower;
        upper = other.upper;

        return *this;
    }


    bool Handle::operator==(const Handle& other) const {
        return other.lower == lower && other.upper == upper;
    }


    bool Handle::operator!=(const Handle& other) const {
        return !operator==(other);
    }


    bool Handle::operator<(const Handle& other) const {
        return upper < other.upper || (upper == other.upper && lower < other.lower);
    }


    bool Handle::operator>(const Handle& other) const {
        return upper > other.upper || (upper == other.upper && lower > other.lower);
    }


    bool Handle::operator<=(const Handle& other) const {
        return operator<(other) || operator==(other);
    }


    bool Handle::operator>=(const Handle& other) const {
        return operator>(other) || operator==(other);
    }


    Util::HashResult Handle::hash(Util::HashSeed seed) const {
        std::uint32_t currentHash = murmurIteration(lower, static_cast<std::uint32_t>(seed));

        currentHash = murmurIteration(static_cast<std::uint32_t>(upper), currentHash);
        currentHash = murmurIteration(static_cast<std::uint32_t>(upper >> 32), currentHash);

        currentHash ^= 12; // 12 is the length in bytes.
        currentHash ^= currentHash >> 16;
        currentHash *= 0x85ebca6b;
        currentHash ^= currentHash >> 13;
        currentHash *= 0xc2b2ae35;
        currentHash ^= currentHash >> 16;

        return currentHash;
    }


    std::uint32_t Handle::murmurIteration(std::uint32_t keyValue, std::uint32_t currentHash) {
        // Note: magic values are defined by the Murmur 3 algorithm and have no particular meaning or reason for
        //       adjustment.  For clarity, I've left them in-place as magic numbers as they align exactly with the
        //       published implementation.
        //
        // Note: The implementation also assumes a little-endian machine although the only difference would be in how
        //       the "keyValue" or inputs to the hash are represented and thus shouldn't matter for our application.

        keyValue *= 0xcc9e2d51;
        keyValue = (keyValue << 15) | (keyValue >> 17);
        keyValue *= 0x1b873593;
        currentHash ^= keyValue;
        currentHash = (currentHash << 13) | (currentHash >> 19);
        currentHash = (currentHash * 5) + 0xe6546b64;

        return currentHash;
    }

    Util::HashResult qHash(const Handle& handle, Util::HashSeed seed) {
        return handle.hash(seed);
    }
}
