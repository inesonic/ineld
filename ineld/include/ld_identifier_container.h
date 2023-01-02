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
* This header defines the \ref Ld::IdentifierContainer class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_IDENTIFIER_CONTAINER_H
#define LD_IDENTIFIER_CONTAINER_H

#include <QSharedPointer>
#include <QString>
#include <QList>

#include <util_hash_functions.h>

#include "ld_common.h"
#include "ld_data_type.h"
#include "ld_element_structures.h"
#include "ld_variable_name.h"
#include "ld_identifier.h"

namespace Ld {
    /**
     * Container class for the \ref Ld::Identifier class.  You can use this class to simplify managing lists of
     * identifiers.
     */
    class LD_PUBLIC_API IdentifierContainer {
        public:
            IdentifierContainer();

            /**
             * Constructor
             *
             * \param[in] identifier The identifier to tie to this instance.
             */
            IdentifierContainer(Identifier* identifier);

            /**
             * Constructor
             *
             * \param[in] identifier The identifier to tie to this instance.
             */
            IdentifierContainer(QSharedPointer<Identifier> identifier);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            IdentifierContainer(const IdentifierContainer& other);

            ~IdentifierContainer();

            /**
             * Method you can use to obtain a shared pointer to the underlying \ref Ld::Identifier instance.
             *
             * \return Returns a shared pointer to the underlying identifier instance.
             */
            QSharedPointer<Identifier> pointer() const;

            /**
             * Method that indicates if the identifier is valid.
             *
             * \return Returns true if the entry is valid.  Returns false if the entry is invalid.
             */
            bool isValid() const;

            /**
             * Method that indicates if the identifier is invalid.
             *
             * \return Returns true if the entry is invalid.  Returns false if the entry is valid.
             */
            bool isInvalid() const;

            /**
             * Method you can use to obtain the handle associated with this identifier.
             *
             * \return Returns the handle tied to this identifier.
             */
            Identifier::Handle handle() const;

            /**
             * Method you can use to obtain the current variable name.
             *
             * \return Returns the current variable name.
             */
            const VariableName& variableName() const;

            /**
             * Method you can use to determine the first text value.
             *
             * \return Returns the first text value.
             */
            QString text1() const;

            /**
             * Method you can use to determine the second text value.
             *
             * \return Returns the second text value.
             */
            QString text2() const;

            /**
             * Method you can use to set the parent identifier that defines the scope of this variable.
             *
             * \param[in] newParentScope The new parent identifier that defines the scope of this identifier.  This is
             *                           a shared pointer to the element that defines the scope.
             */
            void setParentScope(ElementPointer newParentScope);

            /**
             * Method you can use to determine the scoping prefix for this variable.
             *
             * \return Returns the parent identifier that defines the scope of this prefix.  This is a shared pointer
             *         to the element that defines the scope.
             */
            ElementPointer parentScope() const;

            /**
             * Method you can use to specify where this identifier was defined.
             *
             * \param[in] nowDefinedAs The location the identifier was defined.  By default, identifiers are defined
             *                         at global scope.
             */
            void setDefinedAs(Identifier::DefinedAs nowDefinedAs);

            /**
             * Method you can use to determine where and how this identifier was defined.
             *
             * \return Returns the location where this identifier was defined.
             */
            Identifier::DefinedAs definedAs() const;

            /**
             * Method you can use to set the data type.
             *
             * \param[in] newDataType The new data type to assign to this identifier.
             */
            void setDataType(const DataType& newDataType);

            /**
             * Method you can use to determine the current data type.
             *
             * \return Returns the data type assigned to this identifier.
             */
            DataType dataType() const;

            /**
             * Method you can use to obtain the internal name of the identifier.
             *
             * \return Returns the internal name for the identifier.  The internal name is derived from the text
             *         entries.
             */
            const QString& internalName() const;

            /**
             * Method you can use to determine if the type of this identifier was explicitly set or implicitly set.
             *
             * \return Returns true if the identifier type was explicitly set.  Returns false if the identifier type
             *         was implicitly set.
             */
            bool typeExplicitlySet() const;

            /**
             * Method you can use to determine if the type of this identifier was implicitly or explicitly set.
             *
             * \return Returns true if the identifier type was implicitly set.  Returns false if the identifier type
             *         was explicitly set.
             */
            bool typeImplicitlySet() const;

            /**
             * Method you can use indicate that the identifier type was explicitly or implicitly set.
             *
             * \param[in] nowTypeExplicitlySet If true, the type will be flagged as explicitly set.  If false, the
             *                                 type will be flagged as implicitly set.
             */
            void setTypeExplicitlySet(bool nowTypeExplicitlySet = true);

            /**
             * Method you can use indicate that the identifier type was implicitly or explicitly set.
             *
             * \param[in] nowTypeImplicitlySet If true, the type will be flagged as implicitly set.  If false, the
             *                                 type will be flagged as explicitly set.
             */
            void setTypeimplicitlySet(bool nowTypeImplicitlySet = true);

            /**
             * Method you can call to clear the list of elements.
             */
            void clear();

            /**
             * Method you can use to add a new element and value to this container.
             *
             * \param[in] elementData Pointer to the element to be added.
             *
             * \return Returns true on success, returns false if the element already exists in the container.
             */
            bool insertElementData(const ElementValueData& elementData);

            /**
             * Method you can use to remove an element from this container.
             *
             * \param[in] elementData Pointer to the element to be removed.
             *
             * \return Returns true on success, returns false if the element does not exist in the container.
             */
            bool removeElementData(const ElementValueData& elementData);

            /**
             * Method you can use to determine if an element exists in the container.
             *
             * \param[in] elementData The element value to check.
             *
             * \return Returns true, this element/value is stored in the container.  Returns false, the element/value
             *         does not exist in the container.
             */
            bool inContainer(const ElementValueData& elementData) const;

            /**
             * Method you can use to determine if an element does not exist in the container.
             *
             * \param[in] elementData Pointer to the element to be checked for.
             *
             * \return Returns true, this element is not stored in the container.  Returns false, the element is stored
             *         in the container.
             */
            bool notInContainer(const ElementValueData& elementData) const;

            /**
             * Method you can use to obtain a list of every element in the container.
             *
             * \return Returns an element pointer list.
             */
            QList<ElementValueData> elementValueList() const;

            /**
             * Method you can use to obtain an iterator to the element/value pairs tied to this identifier.
             *
             * \return Returns an iterator to the first element/value tied to this identifier.
             */
            Identifier::ElementIterator begin() const;

            /**
             * Method you can use to obtain an iterator just past the last element/value pair tied to this identifier.
             *
             * \return Returns an iterator past the last element/value pair tied to this identifier.
             */
            Identifier::ElementIterator end() const;

            /**
             * Method you can use to set the primary element tied to this identifier.  This will generally be the
             * element that defines the variable type.
             *
             * \param[in] element Shared pointer to the primary element.
             *
             * \return Returns true on success, returns false if the container is invalid.
             */
            bool setPrimaryElement(ElementPointer element);

            /**
             * Method you can use to obtain the primary element instance.
             *
             * \return Returns a shared pointer to the primary element. A null pointer will be returned if the primary
             *         element has not been defined or if the primary element was deleted.
             */
            ElementPointer primaryElement() const;

            /**
             * Pointer operator.  Allows access to the underlying \ref Ld::Identifier instance.
             *
             * \return Returns a reference to a \ref Ld::Identifier instance.
             */
            Identifier& operator*();

            /**
             * Pointer operator.  Allows access to the underlying \ref Ld::Identifier instance.
             *
             * \return Returns a pointer to a \ref Ld::Identifier instance.
             */
            const Identifier& operator*() const;

            /**
             * Indirection operator.  Allows access to the underlying \ref Ld::Identifier instance.
             *
             * \return Returns a reference to a \ref Ld::Identifier instance.
             */
            Identifier* operator->();

            /**
             * Indirection operator.  Allows access to the underlying \ref Ld::Identifier instance.
             *
             * \return Returns a pointer to a \ref Ld::Identifier instance.
             */
            const Identifier* operator->() const;

            /**
             * Assignment operator
             *
             * \param[in] other The instance to assign to this instance.
             */
            IdentifierContainer& operator=(const IdentifierContainer& other);

            /**
             * Assignment operator
             *
             * \param[in] other The instance to assign to this instance.
             */
            IdentifierContainer& operator=(QSharedPointer<Identifier> other);

            /**
             * Comparison operator
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the two instances reference the same identifier name.  Returns false if the
             *         instances reference different identifier names.
             */
            bool operator==(const IdentifierContainer& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the two instances reference different identifier names.  Returns false if the
             *         two instances reference the same identifier name.
             */
            bool operator!=(const IdentifierContainer& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the two instances reference the same identifier name.  Returns false if the
             *         instances reference different identifier names.
             */
            bool operator==(const Identifier& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the two instances reference different identifier names.  Returns false if the
             *         two instances reference the same identifier name.
             */
            bool operator!=(const Identifier& other) const;

        private:
            /**
             * An empty string used for return values.
             */
            static const QString emptyString;

            /**
             * A default variable name.
             */
            static const VariableName emptyVariableName;

            /**
             * The shared pointer to the underlying identifier instance.
             */
            QSharedPointer<Identifier> currentIdentifier;
    };

    /**
     * Hashing function for use with the \ref Ld::IdentifierContainer class.  This function exists to support the Qt
     * QSet and QHash classes.
     *
     * \param[in] entry The entry to calculate a hash for.
     *
     * \param[in] seed  An optional seed value.
     *
     * \return Returns a hash for the provided seed.
     */
    LD_PUBLIC_API Util::HashResult qHash(const IdentifierContainer& entry, Util::HashSeed seed = 0);
};

#endif
