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
* This file implements a single function that can be called configure statistical functions.
***********************************************************************************************************************/

#include <QCoreApplication>

#include "ld_configure_helpers.h"
#include "ld_data_type.h"
#include "ld_variable_name.h"
#include "ld_function_data.h"
#include "ld_function_database.h"
#include "ld_configure_statistical_functions.h"

#define TR(_x) QCoreApplication::translate("ConfigureSpecialFunctions", (_x))

namespace Ld {
    void configureStatisticalFunctions() {
        QList<DataType::ValueType> allDataTypes;
        allDataTypes << DataType::ValueType::BOOLEAN
                     << DataType::ValueType::INTEGER
                     << DataType::ValueType::REAL
                     << DataType::ValueType::COMPLEX
                     << DataType::ValueType::SET
                     << DataType::ValueType::TUPLE
                     << DataType::ValueType::MATRIX_BOOLEAN
                     << DataType::ValueType::MATRIX_INTEGER
                     << DataType::ValueType::MATRIX_REAL
                     << DataType::ValueType::MATRIX_COMPLEX;

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::randomInteger64"),
                QString(),
                VariableName(TR("RInt64")),
                false,
                TR("rint64"),
                TR("Returns a 64-bit signed random integer"),
                TR("Statistics"),
                TR("rint64"),
                true,
                generateVariants(DataType::ValueType::INTEGER, DataType::ValueType::MATRIX_INTEGER)
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::randomInteger32"),
                QString(),
                VariableName(TR("RInt32")),
                false,
                TR("rint32"),
                TR("Returns a 32-bit unsigned random integer"),
                TR("Statistics"),
                TR("rint32"),
                true,
                generateVariants(DataType::ValueType::INTEGER, DataType::ValueType::MATRIX_INTEGER)
            )
        );


        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::trng32"),
                QString(),
                VariableName(TR("TRNG32")),
                false,
                TR("trng32"),
                TR("Returns a 32-bit unsigned true random integer"),
                TR("Statistics"),
                TR("trng32"),
                true,
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::BOOLEAN, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::INTEGER, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::REAL, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::COMPLEX, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::SET, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::TUPLE, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::MATRIX_BOOLEAN, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::MATRIX_INTEGER, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::MATRIX_REAL, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::MATRIX_COMPLEX, TR("any value"))
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::trng64"),
                QString(),
                VariableName(TR("TRNG64")),
                false,
                TR("trng64"),
                TR("Returns a 64-bit signed true random integer"),
                TR("Statistics"),
                TR("trng64"),
                true,
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::BOOLEAN, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::INTEGER, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::REAL, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::COMPLEX, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::SET, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::TUPLE, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::MATRIX_BOOLEAN, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::MATRIX_INTEGER, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::MATRIX_REAL, TR("any value")),
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::MATRIX_COMPLEX, TR("any value"))
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::trngU"),
                QString(),
                VariableName(TR("TRNGU")),
                false,
                TR("trngu"),
                TR("Returns a true random uniform value over the range [0, 1]"),
                TR("Statistics"),
                TR("trngu"),
                true,
                FunctionVariant(DataType::ValueType::REAL, QList<DataType::ValueType>(), QList<QString>())
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::uniformDeviateInclusive"),
                QString(),
                VariableName(TR("UniformInclusive")),
                false,
                TR("uniforminc"),
                TR("Returns one or more uniform random numbers over the range [0, 1]"),
                TR("Statistics"),
                TR("uniforminc"),
                true,
                generateVariants(DataType::ValueType::REAL, DataType::ValueType::MATRIX_REAL)
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::uniformDeviateExclusive"),
                QString(),
                VariableName(TR("UniformExclusive")),
                false,
                TR("uniformex"),
                TR("Returns one or more uniform random numbers over the range (0, 1)"),
                TR("Statistics"),
                TR("uniformex"),
                true,
                generateVariants(DataType::ValueType::REAL, DataType::ValueType::MATRIX_REAL)
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::normalDeviate"),
                QString(),
                VariableName(TR("NormalVariate")),
                false,
                TR("normalv"),
                TR("Returns one or more random numbers in a normal distribution"),
                TR("Statistics"),
                TR("normalv"),
                true,
                generateVariants(
                    TR("%1").arg(QChar(0x03BC)),
                    false,
                    TR("%1").arg(QChar(0x03C3)),
                    true,
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_REAL
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::normalPdf"),
                QString(),
                VariableName(TR("NormalPDF")),
                false,
                TR("normalp"),
                TR("Calculates the normal probability density function"),
                TR("Statistics"),
                TR("normalp"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("x"),
                    false,
                    TR("%1").arg(QChar(0x03BC)),
                    true,
                    TR("%1").arg(QChar(0x03C3)),
                    true
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::normalCdf"),
                QString(),
                VariableName(TR("NormalCDF")),
                false,
                TR("normalc"),
                TR("Calculates the normal cumulative distribution function"),
                TR("Statistics"),
                TR("normalc"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("x"),
                    false,
                    TR("%1").arg(QChar(0x03BC)),
                    true,
                    TR("%1").arg(QChar(0x03C3)),
                    true
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::normalQuantile"),
                QString(),
                VariableName(TR("NormalQuantile")),
                false,
                TR("normalq"),
                TR("Calculates the normal quantile function"),
                TR("Statistics"),
                TR("normalq"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("p"),
                    false,
                    TR("%1").arg(QChar(0x03BC)),
                    true,
                    TR("%1").arg(QChar(0x03C3)),
                    true
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::gammaDeviate"),
                QString(),
                VariableName(TR("GammaVariate")),
                false,
                TR("gammav"),
                TR("Returns one or more random numbers in a gamma distribution"),
                TR("Statistics"),
                TR("gammav"),
                true,
                generateVariants(
                    TR("k"),
                    false,
                    TR("%1").arg(QChar(0x03B8)),
                    true,
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_REAL
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::gammaPdf"),
                QString(),
                VariableName(TR("GammaPDF")),
                false,
                TR("gammap"),
                TR("Calculates the gamma probability density function"),
                TR("Statistics"),
                TR("gammap"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("x"),
                    false,
                    TR("k"),
                    false,
                    TR("%1").arg(QChar(0x03B8)),
                    true
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::gammaCdf"),
                QString(),
                VariableName(TR("GammaCDF")),
                false,
                TR("gammac"),
                TR("Calculates the gamma cumulative distribution function"),
                TR("Statistics"),
                TR("gammac"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("x"),
                    false,
                    TR("k"),
                    false,
                    TR("%1").arg(QChar(0x03B8)),
                    true
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::gammaQuantile"),
                QString(),
                VariableName(TR("GammaQuantile")),
                false,
                TR("gammaq"),
                TR("Calculates the gamma quantile function"),
                TR("Statistics"),
                TR("gammaq"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("p"),
                    false,
                    TR("k"),
                    true,
                    TR("%1").arg(QChar(0x03B8)),
                    true
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::weibullDeviate"),
                QString(),
                VariableName(TR("WeibullVariate")),
                false,
                TR("weibullv"),
                TR("Returns one or more random numbers in a Weibull distribution"),
                TR("Statistics"),
                TR("weibullv"),
                true,
                generateVariants(
                    TR("k"),
                    false,
                    TR("%1").arg(QChar(0x03BB)),
                    false,
                    TR("delay"),
                    true,
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_REAL
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::weibullPdf"),
                QString(),
                VariableName(TR("WeibullPDF")),
                false,
                TR("weibullp"),
                TR("Calculates the Weibull probability density function"),
                TR("Statistics"),
                TR("weibullp"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("x"),
                    false,
                    TR("k"),
                    false,
                    TR("%1").arg(QChar(0x03BB)),
                    false,
                    TR("delay"),
                    true
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::weibullCdf"),
                QString(),
                VariableName(TR("WeibullCDF")),
                false,
                TR("weibullc"),
                TR("Calculates the Weibull cumulative distribution function"),
                TR("Statistics"),
                TR("weibullc"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("x"),
                    false,
                    TR("k"),
                    false,
                    TR("%1").arg(QChar(0x03BB)),
                    false,
                    TR("delay"),
                    true
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::weibullQuantile"),
                QString(),
                VariableName(TR("WeibullQuantile")),
                false,
                TR("weibullq"),
                TR("Calculates the Weibull quantile function"),
                TR("Statistics"),
                TR("weibullq"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("p"),
                    false,
                    TR("k"),
                    false,
                    TR("%1").arg(QChar(0x03BB)),
                    false,
                    TR("delay"),
                    true
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::exponentialDeviate"),
                QString(),
                VariableName(TR("ExponentialVariate")),
                false,
                TR("exponentialv"),
                TR("Returns one or more random numbers in an exponential distribution"),
                TR("Statistics"),
                TR("exponentialv"),
                true,
                generateVariants(
                    TR("%1").arg(QChar(0x03BB)),
                    false,
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_REAL
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::exponentialPdf"),
                QString(),
                VariableName(TR("ExponentialPDF")),
                false,
                TR("exponentialp"),
                TR("Calculates the exponential probability density function"),
                TR("Statistics"),
                TR("exponentialp"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("x"),
                    false,
                    TR("%1").arg(QChar(0x03BB)),
                    false
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::exponentialCdf"),
                QString(),
                VariableName(TR("ExponentialCDF")),
                false,
                TR("exponentialc"),
                TR("Calculates the exponential cumulative distribution function"),
                TR("Statistics"),
                TR("exponentialc"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("x"),
                    false,
                    TR("%1").arg(QChar(0x03BB)),
                    false
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::exponentialQuantile"),
                QString(),
                VariableName(TR("ExponentialQuantile")),
                false,
                TR("exponentialq"),
                TR("Calculates the exponential quantile function"),
                TR("Statistics"),
                TR("exponentialq"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("p"),
                    false,
                    TR("%1").arg(QChar(0x03BB)),
                    false
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::rayleighDeviate"),
                QString(),
                VariableName(TR("RayleighVariate")),
                false,
                TR("rayleighv"),
                TR("Returns one or more random numbers in a Rayleigh distribution"),
                TR("Statistics"),
                TR("rayleighv"),
                true,
                generateVariants(
                    TR("%1").arg(QChar(0x03C3)),
                    false,
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_REAL
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::rayleighPdf"),
                QString(),
                VariableName(TR("RayleighPDF")),
                false,
                TR("rayleighp"),
                TR("Calculates the Rayleigh distribution probability density function"),
                TR("Statistics"),
                TR("rayleighp"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("x"),
                    false,
                    TR("%1").arg(QChar(0x03C3)),
                    false
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::rayleighCdf"),
                QString(),
                VariableName(TR("RayleighCDF")),
                false,
                TR("rayleighc"),
                TR("Calculates the Rayleigh cumulative distribution function"),
                TR("Statistics"),
                TR("rayleighc"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("x"),
                    false,
                    TR("%1").arg(QChar(0x03C3)),
                    false
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::rayleighQuantile"),
                QString(),
                VariableName(TR("RayleighQuantile")),
                false,
                TR("rayleighq"),
                TR("Calculates the Rayleigh distribution quantile function"),
                TR("Statistics"),
                TR("rayleighq"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("p"),
                    false,
                    TR("%1").arg(QChar(0x03C3)),
                    false
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::chiSquaredDeviate"),
                QString(),
                VariableName(TR("ChiSquaredVariate")),
                false,
                TR("chisquaredv"),
                TR("Returns one or more random numbers in a chi-squared distribution"),
                TR("Statistics"),
                TR("chisquaredv"),
                true,
                generateVariants(
                    TR("k"),
                    false,
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_REAL
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::chiSquaredPdf"),
                QString(),
                VariableName(TR("ChiSquaredPDF")),
                false,
                TR("chisquaredp"),
                TR("Calculates the chi-squared distribution probability density function"),
                TR("Statistics"),
                TR("chisquaredp"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("x"),
                    false,
                    TR("k"),
                    false
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::chiSquaredCdf"),
                QString(),
                VariableName(TR("ChiSquaredCDF")),
                false,
                TR("chisquaredc"),
                TR("Calculates the chi-squared cumulative distribution function"),
                TR("Statistics"),
                TR("chisquaredc"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("x"),
                    false,
                    TR("k"),
                    false
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::chiSquaredQuantile"),
                QString(),
                VariableName(TR("ChiSquaredQuantile")),
                false,
                TR("chisquaredq"),
                TR("Calculates the chi-squared distribution quantile function"),
                TR("Statistics"),
                TR("chisquaredq"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("p"),
                    false,
                    TR("k"),
                    false
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::poissonDeviate"),
                QString(),
                VariableName(TR("PoissonVariate")),
                false,
                TR("poissonv"),
                TR("Returns one or more random numbers in an Poisson distribution"),
                TR("Statistics"),
                TR("poissonv"),
                true,
                generateVariants(
                    TR("%1").arg(QChar(0x03BB)),
                    false,
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::MATRIX_INTEGER
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::poissonPmf"),
                QString(),
                VariableName(TR("PoissonPMF")),
                false,
                TR("poissonp"),
                TR("Calculates the Poisson probability mass function"),
                TR("Statistics"),
                TR("poissonp"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("k"),
                    false,
                    TR("%1").arg(QChar(0x03BB)),
                    false
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::poissonCdf"),
                QString(),
                VariableName(TR("PoissonCDF")),
                false,
                TR("poissonc"),
                TR("Calculates the Poisson cumulative distribution function"),
                TR("Statistics"),
                TR("poissonc"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("k"),
                    false,
                    TR("%1").arg(QChar(0x03BB)),
                    false
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::binomialDeviate"),
                QString(),
                VariableName(TR("BinomialVariate")),
                false,
                TR("binomialv"),
                TR("Returns one or more random numbers in an binomial distribution"),
                TR("Statistics"),
                TR("binomialv"),
                true,
                generateVariants(
                    TR("n"),
                    false,
                    TR("p"),
                    false,
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::MATRIX_INTEGER
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::binomialPmf"),
                QString(),
                VariableName(TR("BinomialPMF")),
                false,
                TR("binomialp"),
                TR("Calculates the binomial probability mass function"),
                TR("Statistics"),
                TR("binomialp"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("k"),
                    false,
                    TR("n"),
                    false,
                    TR("p"),
                    false
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::binomialCdf"),
                QString(),
                VariableName(TR("BinomialCDF")),
                false,
                TR("binomialc"),
                TR("Calculates the binomial cumulative distribution function"),
                TR("Statistics"),
                TR("binomialc"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("k"),
                    false,
                    TR("n"),
                    false,
                    TR("p"),
                    false
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::logNormalDeviate"),
                QString(),
                VariableName(TR("LogNormalVariate")),
                false,
                TR("lognormalv"),
                TR("Returns one or more random numbers in a log-normal distribution"),
                TR("Statistics"),
                TR("lognormalv"),
                true,
                generateVariants(
                    TR("%1").arg(QChar(0x03BC)),
                    false,
                    TR("%1").arg(QChar(0x03C3)),
                    true,
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_REAL
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::logNormalPdf"),
                QString(),
                VariableName(TR("LogNormalPDF")),
                false,
                TR("lognormalp"),
                TR("Calculates the log-normal probability density function"),
                TR("Statistics"),
                TR("lognormalp"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("x"),
                    false,
                    TR("%1").arg(QChar(0x03BC)),
                    true,
                    TR("%1").arg(QChar(0x03C3)),
                    true
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::logNormalCdf"),
                QString(),
                VariableName(TR("LogNormalCDF")),
                false,
                TR("lognormalc"),
                TR("Calculates the log-normal cumulative distribution function"),
                TR("Statistics"),
                TR("lognormalc"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("x"),
                    false,
                    TR("%1").arg(QChar(0x03BC)),
                    true,
                    TR("%1").arg(QChar(0x03C3)),
                    true
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::logNormalQuantile"),
                QString(),
                VariableName(TR("LogNormalQuantile")),
                false,
                TR("lognormalq"),
                TR("Calculates the log-normal quantile function"),
                TR("Statistics"),
                TR("lognormalq"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("p"),
                    false,
                    TR("%1").arg(QChar(0x03BC)),
                    true,
                    TR("%1").arg(QChar(0x03C3)),
                    true
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::geometricDeviate"),
                QString(),
                VariableName(TR("GeometricVariate")),
                false,
                TR("geometricv"),
                TR("Returns one or more random numbers in an geometric distribution"),
                TR("Statistics"),
                TR("geometricv"),
                true,
                generateVariants(
                    TR("p"),
                    false,
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::MATRIX_INTEGER
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::geometricPmf"),
                QString(),
                VariableName(TR("GeometricPMF")),
                false,
                TR("geometricp"),
                TR("Calculates the geometric probability mass function"),
                TR("Statistics"),
                TR("geometricp"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("k"),
                    false,
                    TR("p"),
                    false
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::geometricCdf"),
                QString(),
                VariableName(TR("GeometricCDF")),
                false,
                TR("geometricc"),
                TR("Calculates the geometric cumulative distribution function"),
                TR("Statistics"),
                TR("geometricc"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("k"),
                    false,
                    TR("p"),
                    false
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::cauchyLorentzDeviate"),
                QString(),
                VariableName(TR("CauchyVariate")),
                false,
                TR("cauchyv"),
                TR("Returns one or more random numbers in a Cauchy distribution"),
                TR("Statistics"),
                TR("cauchyv"),
                true,
                generateVariants(
                    TR("location"),
                    false,
                    TR("%1").arg(QChar(0x03B3)),
                    false,
                    DataType::ValueType::REAL,
                    DataType::ValueType::MATRIX_REAL
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::cauchyLorentzPdf"),
                QString(),
                VariableName(TR("CauchyPDF")),
                false,
                TR("cauchyp"),
                TR("Calculates the Cauchy probability density function"),
                TR("Statistics"),
                TR("cauchyp"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("x"),
                    false,
                    TR("location"),
                    false,
                    TR("%1").arg(QChar(0x03B3)),
                    false
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::cauchyLorentzCdf"),
                QString(),
                VariableName(TR("CauchyCDF")),
                false,
                TR("cauchyc"),
                TR("Calculates the Cauchy cumulative distribution function"),
                TR("Statistics"),
                TR("cauchyc"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("x"),
                    false,
                    TR("location"),
                    false,
                    TR("%1").arg(QChar(0x03B3)),
                    false
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::cauchyLorentzQuantile"),
                QString(),
                VariableName(TR("CauchyQuantile")),
                false,
                TR("cauchyq"),
                TR("Calculates the Cauchy quantile function"),
                TR("Statistics"),
                TR("cauchyq"),
                false,
                generateVariants(
                    DataType::ValueType::REAL,
                    TR("p"),
                    false,
                    TR("location"),
                    false,
                    TR("%1").arg(QChar(0x03B3)),
                    false
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::count"),
                QString(),
                VariableName(TR("Count")),
                false,
                TR("count"),
                TR("Counts boolean, integer, real, and complex values in the supplied list."),
                TR("Statistics"),
                TR("count"),
                false,
                FunctionVariant(
                    allDataTypes,
                    DataType::ValueType::INTEGER
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::sum"),
                QString(),
                VariableName(TR("Sum")),
                false,
                TR("sum"),
                TR("Calculates the sum of scalar values in the supplied list."),
                TR("Statistics"),
                TR("sum"),
                false,
                FunctionVariant(
                    allDataTypes,
                    DataType::ValueType::COMPLEX
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::avg"),
                QString(),
                VariableName(TR("Average")),
                false,
                TR("average"),
                TR("Calculates the average of scalar values in the supplied list."),
                TR("Statistics"),
                TR("average"),
                false,
                FunctionVariant(
                    allDataTypes,
                    DataType::ValueType::COMPLEX
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::min"),
                QString(),
                VariableName(TR("Minimum")),
                false,
                TR("min"),
                TR("Calculates the minimum of scalar values in the supplied list."),
                TR("Statistics"),
                TR("min"),
                false,
                FunctionVariant(
                    allDataTypes,
                    DataType::ValueType::REAL
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::max"),
                QString(),
                VariableName(TR("Maximum")),
                false,
                TR("max"),
                TR("Calculates the maximum of scalar values in the supplied list."),
                TR("Statistics"),
                TR("max"),
                false,
                FunctionVariant(
                    allDataTypes,
                    DataType::ValueType::REAL
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::variance"),
                QString(),
                VariableName(TR("Variance")),
                false,
                TR("variance"),
                TR("Calculates the variance of scalar values in the supplied list."),
                TR("Statistics"),
                TR("variance"),
                false,
                FunctionVariant(
                    allDataTypes,
                    DataType::ValueType::REAL
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::stdDev"),
                QString(),
                VariableName(TR("StdDev")),
                false,
                TR("stddev"),
                TR("Calculates the standard deviation of scalar values in the supplied list."),
                TR("Statistics"),
                TR("stddev"),
                false,
                FunctionVariant(
                    allDataTypes,
                    DataType::ValueType::REAL
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::sampleStdDev"),
                QString(),
                VariableName(TR("SampleStdDev")),
                false,
                TR("sstddev"),
                TR("Calculates the sample standard deviation of scalar values in the supplied list."),
                TR("Statistics"),
                TR("sstddev"),
                false,
                FunctionVariant(
                    allDataTypes,
                    DataType::ValueType::REAL
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::median"),
                QString(),
                VariableName(TR("Median")),
                false,
                TR("median"),
                TR("Calculates the median of scalar values in the supplied list."),
                TR("Statistics"),
                TR("median"),
                false,
                FunctionVariant(
                    allDataTypes,
                    DataType::ValueType::REAL
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::mode"),
                QString(),
                VariableName(TR("Mode")),
                false,
                TR("mode"),
                TR("Calculates the mode of scalar values in the supplied list."),
                TR("Statistics"),
                TR("mode"),
                false,
                FunctionVariant(
                    allDataTypes,
                    DataType::ValueType::COMPLEX
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::sampleSkew"),
                QString(),
                VariableName(TR("SampleSkew")),
                false,
                TR("sskew"),
                TR("Calculates the sample skew of scalar values in the supplied list."),
                TR("Statistics"),
                TR("sskew"),
                false,
                FunctionVariant(
                    allDataTypes,
                    DataType::ValueType::REAL
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::excessKurtosis"),
                QString(),
                VariableName(TR("Kurtosis")),
                false,
                TR("kurtosis"),
                TR("Calculates the kurtosis of scalar values in the supplied list."),
                TR("Statistics"),
                TR("kurtosis"),
                false,
                FunctionVariant(
                    allDataTypes,
                    DataType::ValueType::REAL
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::histogram"),
                QString(),
                VariableName(TR("Histogram")),
                false,
                TR("histogram"),
                TR("Calculates the histogram of scalar values in the supplied list."),
                TR("Statistics"),
                TR("histogram"),
                false,
                FunctionVariant(
                    allDataTypes,
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::REAL, TR("lower bound"),
                    DataType::ValueType::REAL, TR("upper bound"),
                    DataType::ValueType::INTEGER, TR("number buckets")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::sort"),
                QString(),
                VariableName(TR("Sort")),
                false,
                TR("sort"),
                TR("Sorts any iterable type."),
                TR("Set/Tuple"),
                TR("sort"),
                false,
                FunctionVariant(DataType::ValueType::TUPLE, DataType::ValueType::SET, TR("set")),
                FunctionVariant(DataType::ValueType::TUPLE, DataType::ValueType::TUPLE, TR("tuple")),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                )
            )
        );


        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::sortDescending"),
                QString(),
                VariableName(TR("SortReverse")),
                false,
                TR("sortr"),
                TR("Sorts any iterable type in reverse order."),
                TR("Set/Tuple"),
                TR("sortr"),
                false,
                FunctionVariant(DataType::ValueType::TUPLE, DataType::ValueType::SET, TR("set")),
                FunctionVariant(DataType::ValueType::TUPLE, DataType::ValueType::TUPLE, TR("tuple")),
                FunctionVariant(
                    DataType::ValueType::MATRIX_BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_INTEGER,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_REAL,
                    DataType::ValueType::MATRIX_REAL, TR("matrix")
                ),
                FunctionVariant(
                    DataType::ValueType::MATRIX_COMPLEX,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix")
                )
            )
        );
    }
}
