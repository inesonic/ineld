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
* This file implements the \ref Ld::Capabilities class.
***********************************************************************************************************************/

#include <QString>

#include <util_bit_set.h>

#include "ld_common.h"
#include "ld_capabilities.h"

/***********************************************************************************************************************
 * Capabilities
 */

namespace Ld {
    Util::BitSet::BitNameHash Capabilities::bitNameHash;

    const Capabilities Capabilities::selection("SELECTION");
    const Capabilities Capabilities::frame("FRAME");
    const Capabilities Capabilities::textAnnotations("TEXT_ANNOTATIONS");
    const Capabilities Capabilities::nonTextAnnotations("NON_TEXT_ANNOTATIONS");
    const Capabilities Capabilities::typeDeclaration("TYPE_DECLARATIONS");
    const Capabilities Capabilities::assignment("ASSIGNMENT");
    const Capabilities Capabilities::iterationOperator("ITERATION_OPERATOR");
    const Capabilities Capabilities::conditionalOperator("CONDITIONAL_OPERATOR");
    const Capabilities Capabilities::compoundStatementOperator("COMPOUND_STATEMENT_OPERATOR");
    const Capabilities Capabilities::compoundExitOperator("COMPOUND_EXIT_OPERATOR");
    const Capabilities Capabilities::dataTypes("DATA_TYPES");
    const Capabilities Capabilities::variables("VARIABLES");
    const Capabilities Capabilities::subscripts("SUBSCRIPTS");
    const Capabilities Capabilities::specialBooleanValues("SPECIAL_BOOLEAN_VALUES");
    const Capabilities Capabilities::specialIntegerValues("SPECIAL_INTEGER_VALUES");
    const Capabilities Capabilities::specialRealValues("SPECIAL_REAL_VALUES");
    const Capabilities Capabilities::specialComplexValues("SPECIAL_COMPLEX_VALUES");
    const Capabilities Capabilities::specialSetValues("SPECIAL_SET_VALUES");
    const Capabilities Capabilities::functionDeclarations("FUNCTION_DECLARATIONS");
    const Capabilities Capabilities::booleanFunctions("BOOLEAN_FUNCTIONS");
    const Capabilities Capabilities::integerFunctions("INTEGER_FUNCTIONS");
    const Capabilities Capabilities::realFunctions("REAL_FUNCTIONS");
    const Capabilities Capabilities::complexFunctions("COMPLEX_FUNCTIONS");
    const Capabilities Capabilities::setFunctions("SET_FUNCTIONS");
    const Capabilities Capabilities::tupleFunctions("TUPLE_FUNCTIONS");
    const Capabilities Capabilities::matrixBooleanFunctions("MATRIX_BOOLEAN_FUNCTIONS");
    const Capabilities Capabilities::matrixIntegerFunctions("MATRIX_INTEGER_FUNCTIONS");
    const Capabilities Capabilities::matrixRealFunctions("MATRIX_REAL_FUNCTIONS");
    const Capabilities Capabilities::matrixComplexFunctions("MATRIX_COMPLEX_FUNCTIONS");
    const Capabilities Capabilities::numericLiterals("NUMERIC_LITERALS");
    const Capabilities Capabilities::setLiterals("SET_LITERALS");
    const Capabilities Capabilities::tupleLiterals("TUPLE_LITERALS");
    const Capabilities Capabilities::matrixLiterals("MATRIX_LITERALS");
    const Capabilities Capabilities::ranges("RANGES");
    const Capabilities Capabilities::booleanOperators("BOOLEAN_OPERATORS");
    const Capabilities Capabilities::integerOperators("INTEGER_OPERATORS");
    const Capabilities Capabilities::realOperators("REAL_OPERATORS");
    const Capabilities Capabilities::complexOperators("COMPLEX_OPERATORS");
    const Capabilities Capabilities::setOperators("SET_OPERATORS");
    const Capabilities Capabilities::tupleOperators("TUPLE_OPERATORS");
    const Capabilities Capabilities::matrixBooleanOperators("MATRIX_BOOLEAN_OPERATORS");
    const Capabilities Capabilities::matrixIntegerOperators("MATRIX_INTEGER_OPERATORS");
    const Capabilities Capabilities::matrixRealOperators("MATRIX_REAL_OPERATORS");
    const Capabilities Capabilities::matrixComplexOperators("MATRIX_COMPLEX_OPERATORS");
    const Capabilities Capabilities::alphabeticSymbols("ALPHABETIC_SYMBOLS");
    const Capabilities Capabilities::stringIdentifierSymbols("STRING_IDENTIFIER_SYMBOLS");
    const Capabilities Capabilities::numericBinarySymbols("NUMERIC_BINARY_SYMBOLS");
    const Capabilities Capabilities::numericDecimalSymbols("NUMERIC_DECIMAL_SYMBOLS");
    const Capabilities Capabilities::numericHexidecimalSymbols("NUMERIC_HEXIDECIMAL_SYMBOLS");
    const Capabilities Capabilities::numericHexidecimalBaseIdentifierSymbols(
        "NUMERIC_HEXIDECIMAL_BASE_IDENTIFIER_SYMBOLS"
    );
    const Capabilities Capabilities::numericBinaryBaseIdentifierSymbols("NUMERIC_BINARY_BASE_IDENTIFIER_SYMBOLS");
    const Capabilities Capabilities::numericImaginaryUnitSymbols("NUMERIC_IMAGINARY_UNIT_SYMBOLS");
    const Capabilities Capabilities::numericSignSymbols("NUMERIC_SIGN_SYMBOLS");
    const Capabilities Capabilities::numericDecimalPointSymbols("NUMERIC_DECIMAL_POINT_SYMBOLS");
    const Capabilities Capabilities::numericExponentialSymbols("NUMERIC_EXPONENTIAL_SYMBOLS");
    const Capabilities Capabilities::nonAsciiAlphabeticSymbols("NON_ASCII_ALPHABETIC_SYMBOLS");
    const Capabilities Capabilities::specialSymbols("SPECIAL_SYMBOLS");
    const Capabilities Capabilities::newLine("NEW_LINE");

    const Capabilities Capabilities::expressions = (
          Capabilities::assignment
        | Capabilities::typeDeclaration
        | Capabilities::iterationOperator
        | Capabilities::conditionalOperator
    );

    const Capabilities Capabilities::iterableLiterals = (
          Capabilities::setLiterals
        | Capabilities::tupleLiterals
        | Capabilities::matrixLiterals
    );

    const Capabilities Capabilities::allLiterals = (
          Capabilities::numericLiterals
        | Capabilities::setLiterals
        | Capabilities::tupleLiterals
        | Capabilities::matrixLiterals
    );

    const Capabilities Capabilities::numericOperators = (
          Capabilities::integerOperators
        | Capabilities::realOperators
        | Capabilities::complexOperators
    );

    const Capabilities Capabilities::scalarOperators = (
          Capabilities::booleanOperators
        | Capabilities::integerOperators
        | Capabilities::realOperators
        | Capabilities::complexOperators
    );

    const Capabilities Capabilities::matrixOperators = (
          Capabilities::matrixBooleanOperators
        | Capabilities::matrixIntegerOperators
        | Capabilities::matrixRealOperators
        | Capabilities::matrixComplexOperators
    );

    const Capabilities Capabilities::iterableOperators = (
          Capabilities::setOperators
        | Capabilities::tupleOperators
        | Capabilities::matrixBooleanOperators
        | Capabilities::matrixIntegerOperators
        | Capabilities::matrixRealOperators
        | Capabilities::matrixComplexOperators
    );

    const Capabilities Capabilities::numericAndMatrixOperators = (
          Capabilities::numericOperators
        | Capabilities::matrixOperators
    );

    const Capabilities Capabilities::allOperators = (
          Capabilities::booleanOperators
        | Capabilities::integerOperators
        | Capabilities::realOperators
        | Capabilities::complexOperators
        | Capabilities::setOperators
        | Capabilities::tupleOperators
        | Capabilities::matrixBooleanOperators
        | Capabilities::matrixIntegerOperators
        | Capabilities::matrixRealOperators
        | Capabilities::matrixComplexOperators
    );

    const Capabilities Capabilities::allFunctions = (
          Capabilities::booleanFunctions
        | Capabilities::integerFunctions
        | Capabilities::realFunctions
        | Capabilities::complexFunctions
        | Capabilities::setFunctions
        | Capabilities::tupleFunctions
        | Capabilities::matrixBooleanFunctions
        | Capabilities::matrixIntegerFunctions
        | Capabilities::matrixRealFunctions
        | Capabilities::matrixComplexFunctions
    );

    const Capabilities Capabilities::allScalarFunctions = (
          Capabilities::booleanFunctions
        | Capabilities::integerFunctions
        | Capabilities::realFunctions
        | Capabilities::complexFunctions
    );

    const Capabilities Capabilities::allNumericFunctions = (
          Capabilities::booleanFunctions
        | Capabilities::integerFunctions
        | Capabilities::realFunctions
        | Capabilities::complexFunctions
    );

    const Capabilities Capabilities::allIterableFunctions = (
          Capabilities::setFunctions
        | Capabilities::tupleFunctions
        | Capabilities::matrixBooleanFunctions
        | Capabilities::matrixIntegerFunctions
        | Capabilities::matrixRealFunctions
        | Capabilities::matrixComplexFunctions
    );

    const Capabilities Capabilities::allMatrixFunctions = (
          Capabilities::matrixBooleanFunctions
        | Capabilities::matrixIntegerFunctions
        | Capabilities::matrixRealFunctions
        | Capabilities::matrixComplexFunctions
    );

    const Capabilities Capabilities::allLValues = (
          Capabilities::variables
        | Capabilities::subscripts
    );

    const Capabilities Capabilities::allBooleanRValues = (
          Capabilities::allLValues
        | Capabilities::booleanOperators
        | Capabilities::specialBooleanValues
        | Capabilities::numericLiterals
        | Capabilities::booleanFunctions
    );

    const Capabilities Capabilities::allIntegerRValues = (
          Capabilities::allLValues
        | Capabilities::specialBooleanValues
        | Capabilities::specialIntegerValues
        | Capabilities::booleanOperators
        | Capabilities::integerOperators
        | Capabilities::numericLiterals
        | Capabilities::integerFunctions
    );

    const Capabilities Capabilities::allRealRValues = (
          Capabilities::allLValues
        | Capabilities::specialBooleanValues
        | Capabilities::specialIntegerValues
        | Capabilities::specialRealValues
        | Capabilities::booleanOperators
        | Capabilities::integerOperators
        | Capabilities::realOperators
        | Capabilities::numericLiterals
        | Capabilities::realFunctions
    );

    const Capabilities Capabilities::allComplexRValues = (
          Capabilities::allLValues
        | Capabilities::specialBooleanValues
        | Capabilities::specialIntegerValues
        | Capabilities::specialRealValues
        | Capabilities::specialComplexValues
        | Capabilities::booleanOperators
        | Capabilities::integerOperators
        | Capabilities::realOperators
        | Capabilities::complexOperators
        | Capabilities::numericLiterals
        | Capabilities::complexFunctions
    );

    const Capabilities Capabilities::allNumericRValues = (
          Capabilities::allLValues
        | Capabilities::specialIntegerValues
        | Capabilities::specialRealValues
        | Capabilities::specialComplexValues
        | Capabilities::integerOperators
        | Capabilities::realOperators
        | Capabilities::complexOperators
        | Capabilities::numericLiterals
        | Capabilities::allNumericFunctions
    );

    const Capabilities Capabilities::allScalarRValues = Capabilities::allComplexRValues;

    const Capabilities Capabilities::allScalarAndMatrixRValues = (
          Capabilities::allScalarRValues
        | Capabilities::matrixLiterals
        | Capabilities::matrixOperators
    );

    const Capabilities Capabilities::allNumericAndMatrixRValues = (
          Capabilities::allNumericRValues
        | Capabilities::matrixLiterals
        | Capabilities::matrixOperators
    );

    const Capabilities Capabilities::allFiniteSetRValues = (
          Capabilities::allLValues
        | Capabilities::specialSetValues
        | Capabilities::setLiterals
        | Capabilities::setOperators
        | Capabilities::setFunctions
    );

    const Capabilities Capabilities::allSetRValues = (
          Capabilities::allLValues
        | Capabilities::allFiniteSetRValues
        | Capabilities::dataTypes
        | Capabilities::setFunctions
    );

    const Capabilities Capabilities::allTupleRValues = (
          Capabilities::tupleLiterals
        | Capabilities::tupleOperators
        | Capabilities::tupleFunctions
    );

    const Capabilities Capabilities::allMatrixBooleanRValues = (
          Capabilities::allLValues
        | Capabilities::matrixBooleanOperators
        | Capabilities::matrixLiterals
        | Capabilities::matrixBooleanFunctions
    );

    const Capabilities Capabilities::allMatrixIntegerRValues = (
          Capabilities::allLValues
        | Capabilities::matrixIntegerOperators
        | Capabilities::matrixLiterals
        | Capabilities::matrixIntegerFunctions
    );

    const Capabilities Capabilities::allMatrixRealRValues = (
          Capabilities::allLValues
        | Capabilities::matrixRealOperators
        | Capabilities::matrixLiterals
        | Capabilities::matrixRealFunctions
    );

    const Capabilities Capabilities::allMatrixComplexRValues = (
          Capabilities::allLValues
        | Capabilities::matrixComplexOperators
        | Capabilities::matrixLiterals
        | Capabilities::matrixComplexFunctions
    );

    const Capabilities Capabilities::allMatrixRValues = (
          Capabilities::allLValues
        | Capabilities::matrixOperators
        | Capabilities::matrixLiterals
        | Capabilities::allFunctions
    );

    const Capabilities Capabilities::allFiniteSetAndTupleRValues = (
          Capabilities::allFiniteSetRValues
        | Capabilities::allTupleRValues
    );

    const Capabilities Capabilities::allScalarFiniteSetAndMatrixRValues = (
          Capabilities::allScalarAndMatrixRValues
        | Capabilities::allFiniteSetRValues
    );

    const Capabilities Capabilities::allIterableRValues = (
          Capabilities::allLValues
        | Capabilities::setLiterals
        | Capabilities::tupleLiterals
        | Capabilities::matrixLiterals
        | Capabilities::setOperators
        | Capabilities::tupleOperators
        | Capabilities::matrixOperators
        | Capabilities::allIterableFunctions
    );

    const Capabilities Capabilities::allIterableAndRangeRValues = (
          Capabilities::allIterableRValues
        | Capabilities::ranges
    );

    const Capabilities Capabilities::allRValues = (
          Capabilities::allLValues
        | Capabilities::allScalarAndMatrixRValues
        | Capabilities::allSetRValues
        | Capabilities::allTupleRValues
    );

    const Capabilities Capabilities::allRValuesAndRanges = (
          Capabilities::allRValues
        | Capabilities::ranges
    );

    const Capabilities Capabilities::functionDeclarationParameters = (
          Capabilities::variables
        | Capabilities::typeDeclaration
    );

    const Capabilities Capabilities::allAssignables = (
          Capabilities::variables
        | Capabilities::subscripts
        | Capabilities::typeDeclaration
        | Capabilities::functionDeclarations
    );

    const Capabilities Capabilities::allValueAssignables = (
          Capabilities::variables
        | Capabilities::subscripts
    );

    const Capabilities Capabilities::allAnnotations = (
          Capabilities::textAnnotations
        | Capabilities::nonTextAnnotations
    );

    const Capabilities Capabilities::asciiAlphanumericSymbols = (
          Capabilities::alphabeticSymbols
        | Capabilities::numericDecimalPointSymbols
    );

    const Capabilities Capabilities::allAlphabeticSymbols(
          Capabilities::alphabeticSymbols
        | Capabilities::nonAsciiAlphabeticSymbols
    );

    const Capabilities Capabilities::allAlphanumericSymbols(
          Capabilities::alphabeticSymbols
        | Capabilities::numericDecimalSymbols
        | Capabilities::nonAsciiAlphabeticSymbols
    );

    const Capabilities Capabilities::allSymbols(
          Capabilities::alphabeticSymbols
        | Capabilities::stringIdentifierSymbols
        | Capabilities::numericDecimalSymbols
        | Capabilities::nonAsciiAlphabeticSymbols
        | Capabilities::specialSymbols
    );

    Capabilities::Capabilities():Util::BitSet(&bitNameHash) {}


    Capabilities::~Capabilities() {}


    Capabilities::Capabilities(const Util::BitSet& other):BitSet(other) {
        Q_ASSERT(tracksSameBitsAs(other));
    }


    Capabilities::Capabilities(const Capabilities& other):Util::BitSet(other) {}


    bool Capabilities::assignBit(const QString& bitName) {
        bool success;

        if (!bitNameHash.contains(bitName)) {
            unsigned bitIndex = static_cast<unsigned>(bitNameHash.size());
            bitNameHash.insert(bitName, bitIndex);

            success = true;
        } else {
            success = false;
        }

        return success;
    }
}
