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
* This file implements the \ref Ld::IdentifierDatabase class.
***********************************************************************************************************************/

#include <QString>
#include <QList>
#include <QSet>
#include <QHash>

#include <cassert>

#include "ld_data_type.h"
#include "ld_variable_name.h"
#include "ld_identifier.h"
#include "ld_identifier_container.h"
#include "ld_identifier_database.h"

namespace Ld {
    IdentifierDatabase::IdentifierDatabase() {}


    IdentifierDatabase::IdentifierDatabase(const IdentifierDatabase& other) {
        identifiersByHandle            = other.identifiersByHandle;
        identifiersByNameByParentScope = other.identifiersByNameByParentScope;
        identifiersByName              = other.identifiersByName;
    }


    IdentifierDatabase::~IdentifierDatabase() {}


    bool IdentifierDatabase::isEmpty() const {
        return identifiersByHandle.isEmpty();
    }


    bool IdentifierDatabase::isNotEmpty() const {
        return !identifiersByHandle.isEmpty();
    }


    unsigned IdentifierDatabase::size() const {
        return static_cast<unsigned>(identifiersByHandle.size());
    }


    void IdentifierDatabase::clear() {
        identifiersByHandle.clear();
        identifiersByNameByParentScope.clear();
        identifiersByName.clear();
    }


    bool IdentifierDatabase::containsEntryByName(const VariableName& variableName) const {
        return identifiersByName.contains(variableName);
    }


    bool IdentifierDatabase::containsEntryByName(const VariableName& variableName, ElementPointer parentScope) const {
        const QHash<VariableName, IdentifierContainer>&
            identifiersByName = identifiersByNameByParentScope.value(parentScope);

        return identifiersByName.contains(variableName);
    }


    bool IdentifierDatabase::containsEntryByName(const QString& text1, const QString& text2) const {
        return containsEntryByName(VariableName(text1, text2));
    }


    bool IdentifierDatabase::containsEntryByName(
            const QString& text1,
            const QString& text2,
            ElementPointer parentScope
        ) const {
        return containsEntryByName(VariableName(text1, text2), parentScope);
    }


    bool IdentifierDatabase::containsEntryByHandle(Identifier::Handle handle) const {
        return identifiersByHandle.contains(handle);
    }


    void IdentifierDatabase::insert(Identifier* identifier) {
        insert(IdentifierContainer(identifier));
    }

    void IdentifierDatabase::insert(QSharedPointer<Identifier> identifier) {
        insert(IdentifierContainer(identifier));
    }


    void IdentifierDatabase::insert(const IdentifierContainer& container) {
        Identifier::Handle identifierHandle = container.handle();

        ElementPointer parentScope = container.parentScope();
        identifiersByHandle.insert(identifierHandle, container);
        identifiersByNameByParentScope[parentScope].insert(container.variableName(), container);
        identifiersByName[container.variableName()].append(container);
    }


    QList<IdentifierContainer> IdentifierDatabase::entriesByName(const VariableName& variableName) const {
        return identifiersByName.value(variableName);
    }


    IdentifierContainer IdentifierDatabase::entryByName(
            const VariableName& variableName,
            ElementPointer      parentScope
        ) const {
        const QHash<VariableName, IdentifierContainer>&
            identifiersByName = identifiersByNameByParentScope.value(parentScope);

        return identifiersByName.value(variableName);
    }


    QList<IdentifierContainer> IdentifierDatabase::entriesByName(const QString& text1, const QString& text2) const {
        return entriesByName(VariableName(text1, text2));
    }


    IdentifierContainer IdentifierDatabase::entryByName(
            const QString& text1,
            const QString& text2,
            ElementPointer parentScope
        ) const{
        return entryByName(VariableName(text1, text2), parentScope);
    }


    IdentifierContainer IdentifierDatabase::entryByHandle(Identifier::Handle handle) const {
        return identifiersByHandle.value(handle);
    }


    QList<IdentifierContainer> IdentifierDatabase::identifiers() const {
        return identifiersByHandle.values();
    }


    QList<IdentifierContainer> IdentifierDatabase::identifiersInScope(ElementPointer parentScope) const {
        const QHash<VariableName, IdentifierContainer>&
            identifiersByName = identifiersByNameByParentScope.value(parentScope);

        return identifiersByName.values();
    }
}
