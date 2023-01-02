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
* This file implements the \ref Ld::IdentifierContainer class.
***********************************************************************************************************************/

#include <QString>
#include <QList>
#include <QHash>

#include <cassert>

#include <util_hash_functions.h>

#include "ld_data_type.h"
#include "ld_variable_name.h"
#include "ld_identifier.h"
#include "ld_identifier_container.h"

namespace Ld {
    const QString      IdentifierContainer::emptyString;
    const VariableName IdentifierContainer::emptyVariableName;

    IdentifierContainer::IdentifierContainer() {}


    IdentifierContainer::IdentifierContainer(Identifier* identifier) {
        currentIdentifier.reset(identifier);
    }


    IdentifierContainer::IdentifierContainer(QSharedPointer<Identifier> identifier) {
        currentIdentifier = identifier;
    }


    IdentifierContainer::IdentifierContainer(const IdentifierContainer& other) {
        currentIdentifier = other.currentIdentifier;
    }


    IdentifierContainer::~IdentifierContainer() {}


    QSharedPointer<Identifier> IdentifierContainer::pointer() const {
        return currentIdentifier;
    }


    bool IdentifierContainer::isValid() const {
        return currentIdentifier.isNull() ? false : currentIdentifier->isValid();
    }


    bool IdentifierContainer::isInvalid() const {
        return currentIdentifier.isNull() ? true : currentIdentifier->isInvalid();
    }


    Identifier::Handle IdentifierContainer::handle() const {
        return currentIdentifier.isNull() ? Identifier::invalidHandle : currentIdentifier->handle();
    }


    QString IdentifierContainer::text1() const {
        return currentIdentifier.isNull() ? QString() : currentIdentifier->text1();
    }


    QString IdentifierContainer::text2() const {
        return currentIdentifier.isNull() ? QString() : currentIdentifier->text2();
    }


    const VariableName& IdentifierContainer::variableName() const {
        return currentIdentifier.isNull() ? emptyVariableName : currentIdentifier->variableName();
    }


    void IdentifierContainer::setParentScope(ElementPointer newParentScope) {
        if (!currentIdentifier.isNull()) {
            currentIdentifier->setParentScope(newParentScope);
        }
    }


    ElementPointer IdentifierContainer::parentScope() const {
        return currentIdentifier.isNull() ? ElementPointer() : currentIdentifier->parentScope();
    }


    void IdentifierContainer::setDefinedAs(Identifier::DefinedAs nowDefinedAs) {
        if (!currentIdentifier.isNull()) {
            currentIdentifier->setDefinedAs(nowDefinedAs);
        }
    }


    Identifier::DefinedAs IdentifierContainer::definedAs() const {
        return currentIdentifier.isNull() ? Identifier::DefinedAs::INVALID : currentIdentifier->definedAs();
    }


    void IdentifierContainer::setDataType(const DataType& newDataType) {
        if (!currentIdentifier.isNull()) {
            currentIdentifier->setDataType(newDataType);
        }
    }


    DataType IdentifierContainer::dataType() const {
        return currentIdentifier.isNull() ? DataType() : currentIdentifier->dataType();
    }


    const QString& IdentifierContainer::internalName() const {
        return currentIdentifier.isNull() ? emptyString : currentIdentifier->internalName();
    }


    bool IdentifierContainer::typeExplicitlySet() const {
        return currentIdentifier.isNull() ? false : currentIdentifier->typeExplicitlySet();
    }


    bool IdentifierContainer::typeImplicitlySet() const {
        return currentIdentifier.isNull() ? false : currentIdentifier->typeImplicitlySet();
    }


    void IdentifierContainer::setTypeExplicitlySet(bool nowTypeExplicitlySet) {
        if (!currentIdentifier.isNull()) {
            currentIdentifier->setTypeExplicitlySet(nowTypeExplicitlySet);
        }
    }


    void IdentifierContainer::setTypeimplicitlySet(bool nowTypeImplicitlySet) {
        if (!currentIdentifier.isNull()) {
            currentIdentifier->setTypeimplicitlySet(nowTypeImplicitlySet);
        }
    }


    void IdentifierContainer::clear() {
        if (!currentIdentifier.isNull()) {
            currentIdentifier->clear();
        }
    }


    bool IdentifierContainer::insertElementData(const ElementValueData& elementData) {
        bool result;

        if (!currentIdentifier.isNull()) {
            result = currentIdentifier->insertElementData(elementData);
        } else {
            result = false;
        }

        return result;
    }


    bool IdentifierContainer::removeElementData(const ElementValueData& elementData) {
        bool result;

        if (!currentIdentifier.isNull()) {
            result = currentIdentifier->removeElementData(elementData);
        } else {
            result = false;
        }

        return result;
    }


    bool IdentifierContainer::inContainer(const ElementValueData& elementData) const {
        bool result;

        if (!currentIdentifier.isNull()) {
            result = currentIdentifier->inContainer(elementData);
        } else {
            result = false;
        }

        return result;
    }


    bool IdentifierContainer::notInContainer(const ElementValueData& elementData) const {
        bool result;

        if (!currentIdentifier.isNull()) {
            result = currentIdentifier->notInContainer(elementData);
        } else {
            result = false;
        }

        return result;
    }


    QList<ElementValueData> IdentifierContainer::elementValueList() const {
        return !currentIdentifier.isNull() ? currentIdentifier->elementValueList() : QList<ElementValueData>();
    }


    Identifier::ElementIterator IdentifierContainer::begin() const {
        return !currentIdentifier.isNull() ? currentIdentifier->begin() : Identifier::ElementIterator();
    }


    Identifier::ElementIterator IdentifierContainer::end() const {
        return !currentIdentifier.isNull() ? currentIdentifier->end() : Identifier::ElementIterator();
    }


    bool IdentifierContainer::setPrimaryElement(ElementPointer element) {
        bool result;

        if (!currentIdentifier.isNull()) {
            currentIdentifier->setPrimaryElement(element);
            result = true;
        } else {
            result = false;
        }

        return result;
    }


    ElementPointer IdentifierContainer::primaryElement() const {
        return currentIdentifier.isNull() ? ElementPointer() : currentIdentifier->primaryElement();
    }


    Identifier& IdentifierContainer::operator*() {
        return currentIdentifier.operator*();
    }


    const Identifier& IdentifierContainer::operator*() const {
        return currentIdentifier.operator*();
    }


    Identifier* IdentifierContainer::operator->() {
        return currentIdentifier.data();
    }


    const Identifier* IdentifierContainer::operator->() const {
        return currentIdentifier.data();
    }


    IdentifierContainer& IdentifierContainer::operator=(const IdentifierContainer& other) {
        currentIdentifier = other.currentIdentifier;
        return *this;
    }


    IdentifierContainer& IdentifierContainer::operator=(QSharedPointer<Identifier> other) {
        currentIdentifier = other;
        return *this;
    }


    bool IdentifierContainer::operator==(const IdentifierContainer& other) const {
        bool result;

        if (currentIdentifier.isNull() || other.currentIdentifier.isNull()) {
            result = false;
        } else {
            result = (*currentIdentifier == *other.currentIdentifier);
        }

        return result;
    }


    bool IdentifierContainer::operator!=(const IdentifierContainer& other) const {
        bool result;

        if (currentIdentifier.isNull() || other.currentIdentifier.isNull()) {
            result = true;
        } else {
            result = (*currentIdentifier != *other.currentIdentifier);
        }

        return result;
    }


    bool IdentifierContainer::operator==(const Identifier& other) const {
        bool result;

        if (currentIdentifier.isNull()) {
            result = false;
        } else {
            result = (*currentIdentifier == other);
        }

        return result;
    }


    bool IdentifierContainer::operator!=(const Identifier& other) const {
        bool result;

        if (currentIdentifier.isNull()) {
            result = true;
        } else {
            result = (*currentIdentifier != other);
        }

        return result;
    }


    Util::HashResult qHash(const IdentifierContainer& entry, Util::HashSeed seed) {
        return qHash(*entry, seed);
    }
}
