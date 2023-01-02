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
* This file implements the \ref Ld::Identifier class.
***********************************************************************************************************************/

#include <QString>
#include <QList>
#include <QSet>
#include <QHash>

#include <cassert>

#include <util_hash_functions.h>

#include "ld_data_type.h"
#include "ld_element_value_data.h"
#include "ld_variable_name.h"
#include "ld_identifier_container.h"
#include "ld_identifier.h"

namespace Ld {
    const Identifier::Handle Identifier::invalidHandle       = static_cast<Identifier::Handle>(-1);
    Identifier::Handle       Identifier::currentGlobalHandle = Identifier::invalidHandle;

    void Identifier::reset() {
        currentGlobalHandle = invalidHandle;
    }


    Identifier::Identifier() {
        currentDefinedAs         = DefinedAs::INVALID;
        currentHandle            = assignHandle();
        currentTypeExplicitlySet = false;
    }


    Identifier::Identifier(const VariableName& variableName, const DataType& dataType) {
        currentVariableName      = variableName;
        currentDefinedAs         = DefinedAs::GLOBAL_SCOPE_VARIABLE;
        currentDataType          = dataType;
        currentHandle            = assignHandle();
        currentTypeExplicitlySet = false;
    }


    Identifier::Identifier(
            const VariableName& variableName,
            ElementPointer      parentScope,
            const DataType&     dataType
        ) {
        currentVariableName      = variableName;
        currentParentScope       = parentScope.toWeakRef();
        currentDefinedAs         = DefinedAs::GLOBAL_SCOPE_VARIABLE;
        currentDataType          = dataType;
        currentHandle            = assignHandle();
        currentTypeExplicitlySet = false;
    }


    Identifier::Identifier(
            const VariableName&   variableName,
            Identifier::DefinedAs definedAs,
            ElementPointer        parentScope,
            const DataType&       dataType
        ) {
        currentVariableName      = variableName;
        currentParentScope       = parentScope.toWeakRef();
        currentDefinedAs         = definedAs;
        currentDataType          = dataType;
        currentHandle            = assignHandle();
        currentTypeExplicitlySet = false;
    }


    Identifier::Identifier(const QString& text1, const DataType& dataType) {
        currentVariableName.setText1(text1);

        currentDefinedAs         = DefinedAs::GLOBAL_SCOPE_VARIABLE;
        currentDataType          = dataType;
        currentHandle            = assignHandle();
        currentTypeExplicitlySet = false;
    }


    Identifier::Identifier(const QString& text1, const QString& text2, const DataType& dataType) {
        currentVariableName.setText1(text1);
        currentVariableName.setText2(text2);

        currentDefinedAs         = DefinedAs::GLOBAL_SCOPE_VARIABLE;
        currentDataType          = dataType;
        currentHandle            = assignHandle();
        currentTypeExplicitlySet = false;
    }


    Identifier::Identifier(
            const QString&  text1,
            const QString&  text2,
            ElementPointer  parentScope,
            const DataType& dataType
        ) {
        currentVariableName.setText1(text1);
        currentVariableName.setText2(text2);

        currentParentScope       = parentScope.toWeakRef();
        currentDefinedAs         = DefinedAs::GLOBAL_SCOPE_VARIABLE;
        currentDataType          = dataType;
        currentHandle            = assignHandle();
        currentTypeExplicitlySet = false;
    }


    Identifier::Identifier(
            const QString&        text1,
            const QString&        text2,
            Identifier::DefinedAs definedAs,
            ElementPointer        parentScope,
            const DataType&       dataType
        ) {
        currentVariableName.setText1(text1);
        currentVariableName.setText2(text2);

        currentParentScope       = parentScope.toWeakRef();
        currentDefinedAs         = definedAs;
        currentDataType          = dataType;
        currentHandle            = assignHandle();
        currentTypeExplicitlySet = false;
    }


    Identifier::~Identifier() {}


    bool Identifier::isValid() const {
        return currentVariableName.isValid();
    }


    bool Identifier::isInvalid() const {
        return currentVariableName.isInvalid();
    }


    Identifier::Handle Identifier::handle() const {
        return currentHandle;
    }


    const VariableName& Identifier::variableName() const {
        return currentVariableName;
    }


    QString Identifier::text1() const {
        return currentVariableName.text1();
    }


    QString Identifier::text2() const {
        return currentVariableName.text2();
    }


    void Identifier::setParentScope(ElementPointer newParentScope) {
        currentParentScope = newParentScope.toWeakRef();
    }


    ElementPointer Identifier::parentScope() const {
        return currentParentScope.toStrongRef();
    }


    void Identifier::setDefinedAs(Identifier::DefinedAs nowDefinedAs) {
        currentDefinedAs = nowDefinedAs;
    }


    Identifier::DefinedAs Identifier::definedAs() const {
        return currentDefinedAs;
    }


    void Identifier::setDataType(const DataType& newDataType) {
        currentDataType = newDataType;
    }


    DataType Identifier::dataType() const {
        return currentDataType;
    }


    const QString& Identifier::internalName() const {
        if (currentInternalName.isEmpty()) {
            currentInternalName = QString("_");

            Handle remainder = currentHandle;
            do {
                unsigned unicode = (remainder % 26) + 'a';
                remainder /= 26;

                currentInternalName.append(QChar(unicode));
            } while (remainder != 0);
        }

        return currentInternalName;
    }


    bool Identifier::typeExplicitlySet() const {
        return currentTypeExplicitlySet;
    }


    bool Identifier::typeImplicitlySet() const {
        return !currentTypeExplicitlySet;
    }


    void Identifier::setTypeExplicitlySet(bool nowExplicitlySet) {
        currentTypeExplicitlySet = nowExplicitlySet;
    }


    void Identifier::setTypeimplicitlySet(bool nowTypeImplicitlySet) {
        currentTypeExplicitlySet = !nowTypeImplicitlySet;
    }


    void Identifier::clear() {
        currentElementValues.clear();
    }


    bool Identifier::insertElementData(const ElementValueData& elementData) {
        bool result;

        if (!currentElementValues.contains(elementData)) {
            currentElementValues.insert(elementData);
            result = true;
        } else {
            result = false;
        }

        return result;
    }


    bool Identifier::removeElementData(const ElementValueData& elementData) {
        bool result;

        if (currentElementValues.contains(elementData)) {
            currentElementValues.remove(elementData);
            result = true;
        } else {
            result = false;
        }

        return result;
    }


    bool Identifier::inContainer(const ElementValueData& elementData) const {
        return currentElementValues.contains(elementData);
    }


    bool Identifier::notInContainer(const ElementValueData& elementData) const {
        return !currentElementValues.contains(elementData);
    }


    QList<ElementValueData> Identifier::elementValueList() const {
        return currentElementValues.values();
    }


    Identifier::ElementIterator Identifier::begin() const {
        return currentElementValues.begin();
    }


    Identifier::ElementIterator Identifier::end() const {
        return currentElementValues.end();
    }


    void Identifier::setPrimaryElement(ElementPointer element) {
        currentPrimaryElement = element.toWeakRef();
    }


    ElementPointer Identifier::primaryElement() const {
        return currentPrimaryElement.toStrongRef();
    }


    bool Identifier::operator==(const Identifier& other) const {
        return other.currentHandle == currentHandle;
    }


    bool Identifier::operator!=(const Identifier& other) const {
        return other.currentHandle != currentHandle;
    }


    Identifier::Handle Identifier::assignHandle() {
        do {
            ++currentGlobalHandle;
        } while (currentGlobalHandle == invalidHandle);

        return currentGlobalHandle;
    }


    Util::HashResult qHash(const Identifier& entry, Util::HashSeed seed ) {
        return ::qHash(entry.handle(), seed);
    }
}
