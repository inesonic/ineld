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
* This header defines the \ref Ld::FunctionDatabase class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_FUNCTION_DATABASE_H
#define LD_FUNCTION_DATABASE_H

#include <QString>
#include <QList>
#include <QMap>
#include <QHash>
#include <QSet>
#include <QRegularExpression>

#include <util_fuzzy_search.h>

#include "ld_common.h"
#include "ld_variable_name.h"
#include "ld_function_data.h"
#include "ld_function_variant.h"

namespace Ld {
    class FunctionData;

    /**
     * Class used to store information about supported functions.  The class can be used to pre-defined functions
     * provided by the application core or provided by plug-ins.
     */
    class LD_PUBLIC_API FunctionDatabase {
        public:
            FunctionDatabase();

            ~FunctionDatabase();

            /**
             * Method you can call to clear all persistent data.
             */
            static void reset();

            /**
             * Method you can use to pre-define function categories.
             *
             * \param[in] categories The list of pre-defined categories.
             */
            static void addCategories(const QList<QString>& categories);

            /**
             * Method you can call to register a new persistent function.
             *
             * \param[in] functionData The new function data instance.
             *
             * \return Returns true on success, returns false if the function is already defined.
             */
            static bool registerFunction(const FunctionData& functionData);

            /**
             * Method you can call to obtain a persistent function by internal name.  You can use this method to add
             * new variants to a persistent function.
             *
             * \param[in] internalName The internal name for the function.
             *
             * \return Returns the persistent function data instance associated with the given internal name.
             */
            static FunctionData& function(const QString& internalName);

            /**
             * Method you can call to obtain a persistent function by internal name.  You can use this method to add
             * new variants to a persistent function.
             *
             * \param[in] variableName The variable name for the function.
             *
             * \return Returns the persistent function data instance associated with the given internal name.
             */
            static FunctionData& function(const VariableName& variableName);

            /**
             * Method you can call to obtain an alphabetized list of persistent functions.  Functions are ordered by
             * user-readable name.
             *
             * \return Returns a list of every persistent function.
             */
            static QList<FunctionData> functions();

            /**
             * Method you can call to build the search engine database.  Call this after all persistent functions have
             * been registered.
             */
            static void buildSearchDatabase();

            /**
             * Method you can use to obtain a list of all persistent function categories.
             *
             * \return Returns a sorted list of function categories.
             */
            static QList<QString> categories();

            /**
             * Method you can use to search for a function by keyword and optional category.
             *
             * \param[in] capabilities The capabilities required by the function.
             *
             * \param[in] keyword      String used to perform the search.  An empty string indicates all functions in
             *                         all categories.
             *
             * \param[in] categories   An optional list of categories.  An empty list indicates all categories are
             *                         of interest.
             */
            static QList<FunctionData> search(
                const Capabilities&   capabilities,
                const QString&        keyword,
                const QList<QString>& categories = QList<QString>()
            );

            /**
             * Method you can use to obtain a list of every function in one or more categories.
             *
             * \param[in] capabilities The capabilities required by the function.
             *
             * \param[in] categories   The categories to obtain the functions for.
             *
             * \return Returns a sorted list of functions in this category.
             */
            static QList<FunctionData> search(const Capabilities& capabilities, const QList<QString>& categories);

            /**
             * Method you can use to search for a function by keyword and optional category.
             *
             * \param[in] keyword    String used to perform the search.  An empty string indicates all functions in
             *                       all categories.
             *
             * \param[in] categories An optional list of categories.  An empty list indicates all categories are
             *                       of interest.
             */
            static QList<FunctionData> search(
                const QString&        keyword,
                const QList<QString>& categories = QList<QString>()
            );

            /**
             * Method you can use to obtain a list of every function in one or more categories.
             *
             * \param[in] categories The categories to obtain the functions for.
             *
             * \return Returns a sorted list of functions in this category.
             */
            static QList<FunctionData> search(const QList<QString>& categories);

            /**
             * Method that dumps the function database to a file.
             *
             * \param[in] filename The name of the file to contain the database.
             *
             * \return Returns true on success, return false on error.
             */
            static bool dumpFunctionList(const QString& filename);

        private:
            /**
             * Method that builds a list of search group IDs from a list of user categories.
             *
             * \param[in] categoryList The list of categories to be included.
             *
             * \return Returns a list of group IDs.
             */
            static QList<Util::FuzzySearchEngine::GroupId> groupIdsFromCategories(const QList<QString>& categoryList);

            /**
             * Method that converts a type to a type string.
             *
             * \param[in] valueType The value type to be translated.
             *
             * \return Returns the translated value.
             */
            static QString valueToText(DataType::ValueType valueType);

            /**
             * Flag indicating if the search engine database has been built.
             */
            static bool searchEngineConfigured;

            /**
             * A list of pre-defined categories.
             */
            static QList<QString> preDefinedCategories;

            /**
             * A hash of persistent functions by internal name.
             */
            static QHash<QString, FunctionData> functionsByInternalName;

            /**
             * A sorted list of all known persistent functions by user readable name.
             */
            static QMap<VariableName, FunctionData> functionsByUserReadableName;

            /**
             * List of functions by index.
             */
            static QList<FunctionData> functionsByIndex;

            /**
             * List of categories by group ID.
             */
            static QList<QString> categoriesByGroupId;

            /**
             * Hash of group IDs by category.
             */
            static QHash<QString, Util::FuzzySearchEngine::GroupId> groupIdsByCategory;

            /**
             * Fuzzy search engine used to locate functions.
             */
            static Util::FuzzySearchEngine functionSearchEngine;
    };
};

#endif
