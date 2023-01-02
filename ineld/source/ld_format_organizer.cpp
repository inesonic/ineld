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
* This file implements the \ref Ld::FormatOrganizer class.
***********************************************************************************************************************/

#include <cassert>

#include "ld_format_structures.h"
#include "ld_format.h"
#include "ld_format_container.h"
#include "ld_format_organizer.h"

namespace Ld {
    FormatOrganizer::FormatOrganizer() {
        updateNeeded = false;
    }


    FormatOrganizer::~FormatOrganizer() {}


    void FormatOrganizer::addFormat(FormatContainer& newFormatContainer) {
        if (!uniqueFormats.contains(newFormatContainer)) {
            uniqueFormats.insert(newFormatContainer, 1);
        } else {
            uniqueFormats[newFormatContainer]++;
        }

        updateNeeded = true;
    }


    void FormatOrganizer::addFormat(FormatPointer newFormat) {
        FormatContainer container(newFormat);
        addFormat(container);
    }


    void FormatOrganizer::addFormat(const Format* newFormat) {
        FormatContainer container(newFormat);
        addFormat(container);
    }


    void FormatOrganizer::addFormat(const Format& newFormat) {
        FormatContainer container(&newFormat);
        addFormat(container);
    }


    FormatIdentifier FormatOrganizer::identifier(FormatPointer format) {
        return identifier(format.data());
    }


    FormatIdentifier FormatOrganizer::identifier(const Format* format) {
        FormatIdentifier identifier;

        if (updateNeeded) {
            updateNeeded = false;
            updateDataStructures();
        }

        const FormatContainer container(format);

        if (currentIdentifiersByFormat.contains(container)) {
            identifier = currentIdentifiersByFormat.value(container);
        } else {
            identifier = invalidFormatIdentifier;
        }

        return identifier;
    }


    FormatPointer FormatOrganizer::createFormat(FormatIdentifier identifier) {
        if (updateNeeded) {
            updateNeeded = false;
            updateDataStructures();
        }

        FormatContainer container;
        if (currentFormatsByIdentifier.contains(identifier)) {
            container = currentFormatsByIdentifier.value(identifier);
        }

        return container.format();
    }


    const FormatIdentifiersByFormat& FormatOrganizer::identifiersByFormat() {
        if (updateNeeded) {
            updateNeeded = false;
            updateDataStructures();
        }

        return currentIdentifiersByFormat;
    }


    const FormatsByIdentifier& FormatOrganizer::formatsByIdentifier() {
        if (updateNeeded) {
            updateNeeded = false;
            updateDataStructures();
        }

        return currentFormatsByIdentifier;
    }


    const FormatParedos& FormatOrganizer::formatParedos() {
        if (updateNeeded) {
            updateNeeded = false;
            updateDataStructures();
        }

        return currentFormatParedos;
    }


    void FormatOrganizer::updateDataStructures() {
        currentFormatsByIdentifier.clear();
        currentIdentifiersByFormat.clear();
        currentFormatParedos.clear();

        QMultiMap<long long, FormatContainer> reverseParedo;

        FormatIdentifiersByFormat::const_iterator formatsIterator    = uniqueFormats.begin();
        FormatIdentifiersByFormat::const_iterator formatsEndIterator = uniqueFormats.end();

        while (formatsIterator != formatsEndIterator) {
            FormatContainer container      = formatsIterator.key();
            unsigned long   referenceCount = formatsIterator.value();
            reverseParedo.insert(-static_cast<long long>(referenceCount), container);

            ++formatsIterator;
        }

        QMultiMap<long long, FormatContainer>::const_iterator paredoIterator    = reverseParedo.begin();
        QMultiMap<long long, FormatContainer>::const_iterator paredoEndIterator = reverseParedo.end();

        while (paredoIterator != paredoEndIterator) {
            FormatContainer  container  = paredoIterator.value();
            FormatIdentifier identifier = currentFormatsByIdentifier.size();
            QString          formatType = (*container).typeName();

            currentFormatsByIdentifier.insert(identifier, container);
            currentIdentifiersByFormat.insert(container, identifier);

            currentFormatParedos[formatType].insert(identifier, container);

            ++paredoIterator;
        }
    }
}
