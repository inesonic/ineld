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
* This file implements the \ref Ld::DataType class.
***********************************************************************************************************************/

#include <QString>
#include <QList>
#include <QMap>
#include <QHash>
#include <QFile>
#include <QRegularExpression>

#include <cassert>

#include <util_string.h>

#include "ld_variable_name.h"
#include "ld_function_data.h"
#include "ld_function_variant.h"
#include "ld_function_database.h"

namespace Ld {
    bool                                             FunctionDatabase::searchEngineConfigured = false;
    QList<QString>                                   FunctionDatabase::preDefinedCategories;
    QHash<QString, FunctionData>                     FunctionDatabase::functionsByInternalName;
    QMap<VariableName, FunctionData>                 FunctionDatabase::functionsByUserReadableName;
    QList<FunctionData>                              FunctionDatabase::functionsByIndex;
    QList<QString>                                   FunctionDatabase::categoriesByGroupId;
    QHash<QString, Util::FuzzySearchEngine::GroupId> FunctionDatabase::groupIdsByCategory;
    Util::FuzzySearchEngine                          FunctionDatabase::functionSearchEngine;

    FunctionDatabase::FunctionDatabase() {}


    FunctionDatabase::~FunctionDatabase() {}


    void FunctionDatabase::reset() {
        searchEngineConfigured = false;
        preDefinedCategories.clear();
        functionsByInternalName.clear();
        functionsByUserReadableName.clear();
        functionsByIndex.clear();
        categoriesByGroupId.clear();
        groupIdsByCategory.clear();
        functionSearchEngine.clear();
    }


    void FunctionDatabase::addCategories(const QList<QString>& categories) {
        preDefinedCategories   = categories;
        searchEngineConfigured = false;
    }


    bool FunctionDatabase::registerFunction(const FunctionData& functionData) {
        bool result;

        QString      internalName     = functionData.internalName();
        VariableName userReadableName = functionData.userVisibleName();

        if (!functionsByInternalName.contains(internalName)         &&
            !functionsByUserReadableName.contains(userReadableName)    ) {
            functionsByInternalName.insert(internalName, functionData);
            functionsByUserReadableName.insert(userReadableName, functionData);

            searchEngineConfigured = false;
            result                 = true;
        } else {
            result = false;
        }

        return result;
    }


    FunctionData& FunctionDatabase::function(const QString& internalName) {
        static FunctionData defaultReturnValue;

        return   functionsByInternalName.contains(internalName)
               ? functionsByInternalName[internalName]
               : defaultReturnValue;
    }


    FunctionData& FunctionDatabase::function(const VariableName& variableName) {
        static FunctionData defaultReturnValue;

        return   functionsByUserReadableName.contains(variableName)
               ? functionsByUserReadableName[variableName]
               : defaultReturnValue;
    }


    QList<FunctionData> FunctionDatabase::functions() {
        return functionsByUserReadableName.values();
    }


    void FunctionDatabase::buildSearchDatabase() {
        if (!searchEngineConfigured) {
            searchEngineConfigured = true;

            functionSearchEngine.clear();
            functionsByIndex.clear();

            categoriesByGroupId = preDefinedCategories;
            unsigned numberCategories = static_cast<unsigned>(categoriesByGroupId.size());

            groupIdsByCategory.clear();
            for (Util::FuzzySearchEngine::GroupId groupId=0 ; groupId<numberCategories ; ++groupId) {
                groupIdsByCategory.insert(categoriesByGroupId.at(groupId), groupId);
            }

            QList<FunctionData> functionList = functionsByUserReadableName.values();
            for (  QMap<VariableName, FunctionData>::const_iterator
                       functionIterator    = functionsByUserReadableName.constBegin(),
                       functionEndIterator = functionsByUserReadableName.constEnd()
                 ; functionIterator != functionEndIterator
                 ; ++functionIterator
                ) {
                    const VariableName& functionName = functionIterator.key();
                    const FunctionData& function     = functionIterator.value();
                    const QString&      category     = function.category();
                    const QString&      description  = function.description();

                    QString patternString;
                    if (functionName.text2().isEmpty()) {
                        patternString = QString("%1 %2 %3")
                                        .arg(functionName.text1())
                                        .arg(category)
                                        .arg(description);
                    } else {
                        patternString = QString("%1 %2 %1_%2 %3 %4")
                                        .arg(functionName.text1())
                                        .arg(functionName.text2())
                                        .arg(category)
                                        .arg(description);
                    }

                    Util::FuzzySearchEngine::GroupId groupId = groupIdsByCategory.value(
                        category,
                        Util::FuzzySearchEngine::invalidGroupId
                    );

                    if (groupId == Util::FuzzySearchEngine::invalidGroupId) {
                        groupId = numberCategories;
                        ++numberCategories;

                        categoriesByGroupId.append(category);
                        groupIdsByCategory.insert(category, groupId);
                    }

                    Util::FuzzySearchEngine::PatternId
                        patternId = static_cast<Util::FuzzySearchEngine::PatternId>(functionsByIndex.size());

                    functionSearchEngine.registerPattern(patternString, groupId, patternId);
                    functionsByIndex.append(function);
            }
        }
    }


    QList<QString> FunctionDatabase::categories() {
        return categoriesByGroupId;
    }


    QList<FunctionData> FunctionDatabase::search(
            const Capabilities&   capabilities,
            const QString&        keyword,
            const QList<QString>& categories
        ) {
        QMap<QString, FunctionData> result;

        Util::TokenizedString                     tokenizedKeyword(keyword);
        QList<Util::FuzzySearchEngine::GroupId>   groupIds = groupIdsFromCategories(categories);
        QList<Util::FuzzySearchEngine::PatternId> patterns = functionSearchEngine.search(tokenizedKeyword, groupIds);

        unsigned numberFunctions = static_cast<unsigned>(functionsByIndex.size());
        for (  QList<Util::FuzzySearchEngine::PatternId>::const_iterator resultIterator    = patterns.constBegin(),
                                                                         resultEndIterator = patterns.constEnd()
             ; resultIterator != resultEndIterator
             ; ++resultIterator
            ) {
            Util::FuzzySearchEngine::PatternId patternId = *resultIterator;
            if (patternId < numberFunctions) {
                const FunctionData& functionData = functionsByIndex.at(patternId);
                if (functionData.childCapabilities().intersects(capabilities)) {
                    const FunctionData& functionData    = functionsByIndex.at(patternId);
                    const VariableName& userVisibleName = functionData.userVisibleName();
                    QString sortString = QString("%1~%2").arg(userVisibleName.text1()).arg(userVisibleName.text2());
                    result.insert(sortString, functionData);
                }
            }
        }

        return result.values();
    }


    QList<FunctionData> FunctionDatabase::search(const Capabilities& capabilities, const QList<QString>& categories) {
        return search(capabilities, QString(), categories);
    }


    QList<FunctionData> FunctionDatabase::search(const QString& keyword, const QList<QString>& categories) {
        Util::TokenizedString                     tokenizedKeyword(keyword);
        QList<Util::FuzzySearchEngine::GroupId>   groupIds = groupIdsFromCategories(categories);
        QList<Util::FuzzySearchEngine::PatternId> patterns = functionSearchEngine.search(tokenizedKeyword, groupIds);

        unsigned                    numberFunctions = static_cast<unsigned>(functionsByIndex.size());
        QMap<QString, FunctionData> result;
        for (  QList<Util::FuzzySearchEngine::PatternId>::const_iterator resultIterator    = patterns.constBegin(),
                                                                         resultEndIterator = patterns.constEnd()
             ; resultIterator != resultEndIterator
             ; ++resultIterator
            ) {
            Util::FuzzySearchEngine::PatternId patternId = *resultIterator;
            if (patternId < numberFunctions) {
                const FunctionData& functionData    = functionsByIndex.at(patternId);
                const VariableName& userVisibleName = functionData.userVisibleName();
                QString sortString = QString("%1~%2").arg(userVisibleName.text1()).arg(userVisibleName.text2());
                result.insert(sortString, functionData);
            }
        }

        return result.values();
    }


    QList<FunctionData> FunctionDatabase::search(const QList<QString>& categories) {
        return search(QString(), categories);
    }


    bool FunctionDatabase::dumpFunctionList(const QString& filename) {
        QFile file(filename);
        bool success = file.open(QFile::OpenModeFlag::WriteOnly);

        QList<FunctionData>::const_iterator functionIterator    = functionsByIndex.constBegin();
        QList<FunctionData>::const_iterator functionEndIterator = functionsByIndex.constEnd();
        while (success && functionIterator != functionEndIterator) {
            const FunctionData& fd = *functionIterator;
            QString text1  = fd.userVisibleName().text1();
            QString text2  = fd.userVisibleName().text2();

            QString rstFunctionName;
            if (text2.isEmpty()) {
                rstFunctionName = QString("\\text{%1}").arg(text1);
            } else {
                rstFunctionName = QString("\\text{%1}_\\text{%2}").arg(text1).arg(text2);
            }

            QString output = QString("%1\t%2\t%3\t:math:`%4`\t%5\t%6\t%7\t%8\t%9")
                             .arg(fd.functionCommand())
                             .arg(text1)
                             .arg(text2)
                             .arg(rstFunctionName)
                             .arg(fd.category())
                             .arg(fd.includeSubscriptParameter() ? "true" : "false")
                             .arg(fd.minimumNumberParameters())
                             .arg(fd.maximumNumberParameters())
                             .arg(fd.description());

            QList<FunctionVariant> variants = fd.variants();
            for (  QList<FunctionVariant>::const_iterator variantIterator    = variants.constBegin(),
                                                          variantEndIterator = variants.constEnd()
                 ; variantIterator != variantEndIterator
                 ; ++variantIterator
                ) {
                const FunctionVariant& v = *variantIterator;
                QString variantString = QString(":math:`\\text{%1 } %2 \\left ( ")
                                        .arg(valueToText(v.returnValueType()))
                                        .arg(rstFunctionName);

                unsigned numberParameters = v.numberParameters();
                for (unsigned parameterIndex=0 ; parameterIndex<numberParameters ; ++parameterIndex) {
                    QString             description = v.parameterDescriptions().at(parameterIndex);
                    DataType::ValueType valueType   = v.parameterTypes().at(parameterIndex);

                    if (parameterIndex != 0) {
                        variantString += " , ";
                    }
                    variantString += QString("\\underbrace{\\text{<%1>}}_\\text{%2}")
                                     .arg(description)
                                     .arg(valueToText(valueType));
                }

                if (v.allowsVariadicParameters()) {
                    if (numberParameters != 0) {
                        variantString += ", ";
                    }

                    variantString += QString("\\ldots");
                }
                variantString += QString(" \\right )`");
                output += QString("\t") + variantString;
            }

            output += "\n";
            QByteArray outputData = output.toUtf8();
            long long bytesWritten = file.write(outputData);
            success = (bytesWritten == outputData.size());

            ++functionIterator;
        }

        file.close();
        return success;
    }


    QString FunctionDatabase::valueToText(DataType::ValueType valueType) {
        QString result;

        switch (valueType) {
            case DataType::ValueType::NONE:           { result = QString("none");           break; }
            case DataType::ValueType::VARIANT:        { result = QString("variant");        break; }
            case DataType::ValueType::BOOLEAN:        { result = QString("boolean");        break; }
            case DataType::ValueType::INTEGER:        { result = QString("integer");        break; }
            case DataType::ValueType::REAL:           { result = QString("real");           break; }
            case DataType::ValueType::COMPLEX:        { result = QString("complex");        break; }
            case DataType::ValueType::SET:            { result = QString("set");            break; }
            case DataType::ValueType::TUPLE:          { result = QString("tuple");          break; }
            case DataType::ValueType::MATRIX_BOOLEAN: { result = QString("boolean matrix"); break; }
            case DataType::ValueType::MATRIX_INTEGER: { result = QString("integer matrix"); break; }
            case DataType::ValueType::MATRIX_REAL:    { result = QString("real matrix");    break; }
            case DataType::ValueType::MATRIX_COMPLEX: { result = QString("complex matrix"); break; }
            default: {
                Q_ASSERT(false);
                break;
            }
        }

        return result;
    }


    QList<Util::FuzzySearchEngine::GroupId> FunctionDatabase::groupIdsFromCategories(
            const QList<QString>& categoryList
        ) {
        QList<Util::FuzzySearchEngine::GroupId> groupIds;
        for (  QList<QString>::const_iterator groupIterator    = categoryList.constBegin(),
                                              groupEndIterator = categoryList.constEnd()
             ; groupIterator != groupEndIterator
             ; ++groupIterator
            ) {
            Util::FuzzySearchEngine::GroupId groupId = groupIdsByCategory.value(
                *groupIterator,
                Util::FuzzySearchEngine::invalidGroupId
            );
            groupIds << groupId;
        }

        return groupIds;
    }
}
