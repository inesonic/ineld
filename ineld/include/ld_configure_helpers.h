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
* This header provides helper functions used to configure the language definition.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CONFIGURE_HELPERS_H
#define LD_CONFIGURE_HELPERS_H

#include <QCoreApplication>
#include <QList>
#include <QString>

#include <cstdint>

#include "ld_common.h"
#include "ld_data_type.h"
#include "ld_function_variant.h"

namespace Ud {
    class UsageData;
};

namespace Ld {
    class PlugInManager;
    class PlugInRegistrar;

    /**
     * List of all supported scalar types.
     */
    extern const DataType::ValueType scalarTypes[];

    /**
     * Method that generates a list of function variants for a distribution function.
     *
     * \param[in] returnType         The function return type.  A value of Ld::DataType::ValueType::NONE will prevent
     *                               scalar forms of the function from being included.
     *
     * \param[in] matrixReturnType   The function return type for the matrix form.  A value of
     *                               Ld::DataType::ValueType::NONE will prevent matrix forms of the function from being
     *                               included.
     *
     * \return Returns a list of function variants.
     */
    QList<FunctionVariant> generateVariants(
        DataType::ValueType returnType,
        DataType::ValueType matrixReturnType
    );

    /**
     * Method that generates a list of function variants for a distribution function.
     *
     * \param[in] parameterName1     The name of the first distribution parameter.
     *
     * \param[in] parameter1Optional If true, the first parameter will be treated as optional.
     *
     * \param[in] returnType         The function return type.  A value of Ld::DataType::ValueType::NONE will
     *                               prevent scalar forms of the function from being included.
     *
     * \param[in] matrixReturnType   The function return type for the matrix form.  A value of
     *                               Ld::DataType::ValueType::NONE will prevent matrix forms of the function from being
     *                               included.
     *
     * \return Returns a list of function variants.
     */
    QList<FunctionVariant> generateVariants(
        const QString&      parameterName1,
        bool                parameter1Optional,
        DataType::ValueType returnType,
        DataType::ValueType matrixReturnType
    );

    /**
     * Method that generates a list of function variants for a distribution function.
     *
     * \param[in] parameterName1     The name of the first distribution parameter.
     *
     * \param[in] parameter1Optional If true, the first parameter will be treated as optional.
     *
     * \param[in] parameterName2     The name of the second distribution parameter.
     *
     * \param[in] parameter2Optional If true, the second parameter will be treated as optional.
     *
     * \param[in] returnType         The function return type.  A value of Ld::DataType::ValueType::NONE will
     *                               prevent scalar forms of the function from being included.
     *
     * \param[in] matrixReturnType   The function return type for the matrix form.  A value of
     *                               Ld::DataType::ValueType::NONE will prevent matrix forms of the function from being
     *                               included.
     *
     * \return Returns a list of function variants.
     */
    QList<FunctionVariant> generateVariants(
        const QString&      parameterName1,
        bool                parameter1Optional,
        const QString&      parameterName2,
        bool                parameter2Optional,
        DataType::ValueType returnType,
        DataType::ValueType matrixReturnType
    );

    /**
     * Method that generates a list of function variants for a distribution function.
     *
     * \param[in] parameterName1     The name of the first distribution parameter.
     *
     * \param[in] parameter1Optional If true, the first parameter will be treated as optional.
     *
     * \param[in] parameterName2     The name of the second distribution parameter.
     *
     * \param[in] parameter2Optional If true, the second parameter will be treated as optional.
     *
     * \param[in] parameterName3     The name of the third distribution parameter.
     *
     * \param[in] parameter3Optional If true, the third parameter will be treated as optional.
     *
     * \param[in] returnType         The function return type.  A value of Ld::DataType::ValueType::NONE will prevent
     *                               scalar forms of the function from being included.
     *
     * \param[in] matrixReturnType   The function return type for the matrix form.  A value of
     *                               Ld::DataType::ValueType::NONE will prevent matrix forms of the function from being
     *                               included.
     *
     * \return Returns a list of function variants.
     */
    QList<FunctionVariant> generateVariants(
        const QString&      parameterName1,
        bool                parameter1Optional,
        const QString&      parameterName2,
        bool                parameter2Optional,
        const QString&      parameterName3,
        bool                parameter3Optional,
        DataType::ValueType returnType,
        DataType::ValueType matrixReturnType
    );

    /**
     * Method that generates a list of function variants for a scalar function with scalar parameters.
     *
     * \param[in] returnType         The function return type.  A value of Ld::DataType::ValueType::NONE will prevent
     *                               scalar forms of the function from being included.
     *
     * \param[in] parameterName1     The name of the first distribution parameter.
     *
     * \param[in] parameter1Optional If true, the first parameter will be treated as optional.
     *
     * \return Returns a list of function variants.
     */
    QList<FunctionVariant> generateVariants(
        DataType::ValueType returnType,
        const QString&      parameterName1,
        bool                parameter1Optional
    );

    /**
     * Method that generates a list of function variants for a scalar function with scalar parameters.
     *
     * \param[in] returnType         The function return type.  A value of Ld::DataType::ValueType::NONE will prevent
     *                               scalar forms of the function from being included.
     *
     * \param[in] parameterName1     The name of the first distribution parameter.
     *
     * \param[in] parameter1Optional If true, the first parameter will be treated as optional.
     *
     * \param[in] parameterName2     The name of the second distribution parameter.
     *
     * \param[in] parameter2Optional If true, the second parameter will be treated as optional.
     *
     * \return Returns a list of function variants.
     */
    QList<FunctionVariant> generateVariants(
        DataType::ValueType returnType,
        const QString&      parameterName1,
        bool                parameter1Optional,
        const QString&      parameterName2,
        bool                parameter2Optional
    );

    /**
     * Method that generates a list of function variants for a scalar function with scalar parameters.
     *
     * \param[in] returnType         The function return type.  A value of Ld::DataType::ValueType::NONE will prevent
     *                               scalar forms of the function from being included.
     *
     * \param[in] parameterName1     The name of the first distribution parameter.
     *
     * \param[in] parameter1Optional If true, the first parameter will be treated as optional.
     *
     * \param[in] parameterName2     The name of the second distribution parameter.
     *
     * \param[in] parameter2Optional If true, the second parameter will be treated as optional.
     *
     * \param[in] parameterName3     The name of the third distribution parameter.
     *
     * \param[in] parameter3Optional If true, the third parameter will be treated as optional.
     *
     * \return Returns a list of function variants.
     */
    QList<FunctionVariant> generateVariants(
        DataType::ValueType returnType,
        const QString&      parameterName1,
        bool                parameter1Optional,
        const QString&      parameterName2,
        bool                parameter2Optional,
        const QString&      parameterName3,
        bool                parameter3Optional
    );

    /**
     * Method that generates a list of function variants for a scalar function with scalar parameters.
     *
     * \param[in] returnType         The function return type.  A value of Ld::DataType::ValueType::NONE will prevent
     *                               scalar forms of the function from being included.
     *
     * \param[in] parameterName1     The name of the first distribution parameter.
     *
     * \param[in] parameter1Optional If true, the first parameter will be treated as optional.
     *
     * \param[in] parameterName2     The name of the second distribution parameter.
     *
     * \param[in] parameter2Optional If true, the second parameter will be treated as optional.
     *
     * \param[in] parameterName3     The name of the third distribution parameter.
     *
     * \param[in] parameter3Optional If true, the third parameter will be treated as optional.
     *
     * \param[in] parameterName4     The name of the fourth distribution parameter.
     *
     * \param[in] parameter4Optional If true, the fourth parameter will be treated as optional.
     *
     * \return Returns a list of function variants.
     */
    QList<FunctionVariant> generateVariants(
        DataType::ValueType returnType,
        const QString&      parameterName1,
        bool                parameter1Optional,
        const QString&      parameterName2,
        bool                parameter2Optional,
        const QString&      parameterName3,
        bool                parameter3Optional,
        const QString&      parameterName4,
        bool                parameter4Optional
    );
};

#endif
