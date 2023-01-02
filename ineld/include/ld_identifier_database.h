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
* This header defines the \ref Ld::IdentifierDatabase class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_IDENTIFIER_DATABASE_H
#define LD_IDENTIFIER_DATABASE_H

#include <QString>
#include <QList>
#include <QSet>
#include <QHash>

#include "ld_common.h"
#include "ld_data_type.h"
#include "ld_variable_name.h"
#include "ld_identifier.h"
#include "ld_identifier_container.h"
#include "ld_element_structures.h"

namespace Ld {
    /**
     * Class that maintains a database of identifiers.  You can use this class to track identifiers and request
     * identifiers by text values, by internal name, or by handle.
     */
    class LD_PUBLIC_API IdentifierDatabase {
        public:
            IdentifierDatabase();

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            IdentifierDatabase(const IdentifierDatabase& other);

            ~IdentifierDatabase();

            /**
             * Method you can use to determine if the database is empty.
             *
             * \return Returns true if the database is empty.  Returns false if the database is not empty.
             */
            bool isEmpty() const;

            /**
             * Method you can use to determine if the database is empty.
             *
             * \return Returns true if the database is not empty.  Returns false if the database is empty.
             */
            bool isNotEmpty() const;

            /**
             * Method you can use to determine the size of the database.
             *
             * \return Returns the size of the database, in entries.
             */
            unsigned size() const;

            /**
             * Method you can use to clear the database contents.
             */
            void clear();

            /**
             * Method you can use to determine if we have an identifier based on a variable name.
             *
             * \param[in] variableName The variable name of the variable to identifier.
             *
             * \return Returns true if an identifier with the name exists.  Returns false if the identifier does not
             *         exist.
             */
            bool containsEntryByName(const VariableName& variableName) const;

            /**
             * Method you can use to determine if we have an identifier based on text values.
             *
             * \param[in] variableName The variable name of the variable to identifier.
             *
             * \param[in] parentScope  The parent scope for this identifier.  This is a shared pointer to the element
             *                         that defines the scope.
             *
             * \return Returns true if an entry exists.  Returns false if the entry does not exist.
             */
            bool containsEntryByName(const VariableName& variableName, ElementPointer parentScope) const;

            /**
             * Method you can use to determine if we have an identifier based on text values.
             *
             * \param[in] text1 The first text value.
             *
             * \param[in] text2 The second text value.
             *
             * \return Returns true if an identifier with the name exists.  Returns false if the identifier does not
             *         exist.
             */
            bool containsEntryByName(const QString& text1, const QString& text2) const;

            /**
             * Method you can use to determine if we have an identifier based on text values.
             *
             * \param[in] text1       The first text value.
             *
             * \param[in] text2       The second text value.
             *
             * \param[in] parentScope The parent scope for this identifier.  This is a shared pointer to the element
             *                        that defines the scope.
             *
             * \return Returns true if an entry exists.  Returns false if the entry does not exist.
             */
            bool containsEntryByName(const QString& text1, const QString& text2, ElementPointer parentScope) const;

            /**
             * Method you can use to determine if we have an identifier based on a handle.
             *
             * \param[in] handle The handle to query for.
             *
             * \return Returns true if an entry exists.  Returns false if the entry does not exist.
             */
            bool containsEntryByHandle(Identifier::Handle handle) const;

            /**
             * Method you can use to add an identifier.  Adding an identifier will assign a handle to it.
             *
             * \param[in] identifier The identifier to be added.  The class will take ownership of the identifier.
             */
            void insert(Identifier* identifier);

            /**
             * Method you can use to add an identifier.  Adding an identifier will assign a handle to it.
             *
             * \param[in] identifier The identifier to be added.
             */
            void insert(QSharedPointer<Identifier> identifier);

            /**
             * Method you can use to add an identifier.  Adding an identifier will assign a handle to it.
             *
             * \param[in] container The identifier to be added.
             */
            void insert(const IdentifierContainer& container);

            /**
             * Method you can use to obtain an identifier by name.
             *
             * \param[in] variableName The variable name of the variable to identifier.
             *
             * \return Returns a list of variables by name.
             */
            QList<IdentifierContainer> entriesByName(const VariableName& variableName) const;

            /**
             * Method you can use to obtain an identifer by name.
             *
             * \param[in] variableName The variable name of the variable to identifier.
             *
             * \param[in] parentScope  The parent scope for this identifier.  This is a shared pointer to the element
             *                         that defines the scope.
             *
             * \return Returns the requested entry of an invalid container if the entry does not exist in the specified
             *         scope.
             */
            IdentifierContainer entryByName(const VariableName& variableName, ElementPointer parentScope) const;

            /**
             * Method you can use to obtain an identifier by name.
             *
             * \param[in] text1 The first text value.
             *
             * \param[in] text2 The second text value.
             *
             * \return Returns a list of variables by name.
             */
            QList<IdentifierContainer> entriesByName(const QString& text1, const QString& text2) const;

            /**
             * Method you can use to obtain an identifier by name.
             *
             * \param[in] text1       The first text value.
             *
             * \param[in] text2       The second text value.
             *
             * \param[in] parentScope The parent scope for this identifier.  This is a shared pointer to the element
             *                        that defines the scope.
             *
             * \return Returns the requested entry of an invalid container if the entry does not exist in the specified
             *         scope.
             */
            IdentifierContainer entryByName(
                const QString& text1,
                const QString& text2,
                ElementPointer parentScope
            ) const;

            /**
             * Method you can use to determine if we have an identifier based on a handle.
             *
             * \param[in] handle The handle to query for.
             *
             * \return Returns true if an entry exists.  Returns false if the entry does not exist.
             */
            IdentifierContainer entryByHandle(Identifier::Handle handle) const;

            /**
             * Method you can use to obtain a list of identifiers.
             *
             * \return Returns a list of all identifiers in handle order.
             */
            QList<IdentifierContainer> identifiers() const;

            /**
             * Method you can use to obtain all the identifiers in a given scope.
             *
             * \param[in] parentScope The parent scope.  This is a shared pointer to the element that defines the
             *                        scope.
             */
            QList<IdentifierContainer> identifiersInScope(ElementPointer parentScope) const;

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            IdentifierDatabase& operator=(const IdentifierDatabase& other) {
                identifiersByHandle = other.identifiersByHandle;
                identifiersByNameByParentScope = other.identifiersByNameByParentScope;
                identifiersByName = other.identifiersByName;

                return *this;
            }

            /**
             * Assignment operator (move semantics).
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            IdentifierDatabase& operator=(IdentifierDatabase&& other) {
                identifiersByHandle = other.identifiersByHandle;
                identifiersByNameByParentScope = other.identifiersByNameByParentScope;
                identifiersByName = other.identifiersByName;

                return *this;
            }

        private:
            /**
             * A list of all identifiers in handle order.
             */
            QHash<Identifier::Handle, IdentifierContainer> identifiersByHandle;

            /**
             * A hash of identifiers by parent scope.
             */
            QHash<ElementPointer, QHash<VariableName, IdentifierContainer>> identifiersByNameByParentScope;

            /**
             * A hash of identifiers by variable name.
             */
            QHash<VariableName, QList<IdentifierContainer>> identifiersByName;
    };
};

#endif
