/*-*-c++-*-************************f*************************************************************************************
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
* This header defines the \ref Ld::Capabilities bit set.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CAPABILITIES_H
#define LD_CAPABILITIES_H

#include <util_bit_set.h>

#include "ld_common.h"

namespace Ld {
    /**
     * Specialization of the Util::BitSet class that is used to track parent requirements or child capabilities
     * for elements throughout the application.
     *
     * This class also defines several specific and common parent requirements.
     */
    class LD_PUBLIC_API Capabilities:public Util::BitSet {
        private:
            static Util::BitSet::BitNameHash bitNameHash;

        public:
            /**
             * Pre-defined instance of \ref Capabilities, indicates selection are OK.
             */
            static const Capabilities selection;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts a frame.
             */
            static const Capabilities frame;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts text annotations.
             */
            static const Capabilities textAnnotations;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts non-text anntotations such
             * as images and fields.
             */
            static const Capabilities nonTextAnnotations;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts element of set operators
             * used as a type declaration.
             */
            static const Capabilities typeDeclaration;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts an assignment operation.
             */
            static const Capabilities assignment;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts an iteration/looping
             * operator.
             */
            static const Capabilities iterationOperator;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts an top level conditional
             * operator.  Note that this excludes brace conditionals.
             */
            static const Capabilities conditionalOperator;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts a compound statement
             * operator.
             */
            static const Capabilities compoundStatementOperator;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts a therefore or Halmos
             * operator.
             */
            static const Capabilities compoundExitOperator;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts data-types.
             */
            static const Capabilities dataTypes;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts variables.
             */
            static const Capabilities variables;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts subscripted values.
             */
            static const Capabilities subscripts;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts special integer values.
             */
            static const Capabilities specialIntegerValues;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts special real values.
             */
            static const Capabilities specialRealValues;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts special complex values.
             */
            static const Capabilities specialComplexValues;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts special boolean values.
             */
            static const Capabilities specialBooleanValues;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts special set values.
             */
            static const Capabilities specialSetValues;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts function declaration
             * (L-value).
             */
            static const Capabilities functionDeclarations;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts boolean function references
             * (R-value).
             */
            static const Capabilities booleanFunctions;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts integer function references
             * (R-value).
             */
            static const Capabilities integerFunctions;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts real function references
             * (R-value).
             */
            static const Capabilities realFunctions;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts complex function references
             * (R-value).
             */
            static const Capabilities complexFunctions;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts set function references
             * (R-value).
             */
            static const Capabilities setFunctions;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts tuple function references
             * (R-value).
             */
            static const Capabilities tupleFunctions;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts boolean matrix function
             * references (R-value).
             */
            static const Capabilities matrixBooleanFunctions;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts integer matrix function
             * references (R-value).
             */
            static const Capabilities matrixIntegerFunctions;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts real matrix function
             * references (R-value).
             */
            static const Capabilities matrixRealFunctions;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts complex matrix function
             * references (R-value).
             */
            static const Capabilities matrixComplexFunctions;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts a numeric literal value.
             */
            static const Capabilities numericLiterals;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts a set literal value.
             */
            static const Capabilities setLiterals;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts a tuple literal value.
             */
            static const Capabilities tupleLiterals;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts a matrix literal value.
             */
            static const Capabilities matrixLiterals;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts a range.
             */
            static const Capabilities ranges;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts boolean operators.
             */
            static const Capabilities booleanOperators;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts integer operators.
             */
            static const Capabilities integerOperators;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts real operators.
             */
            static const Capabilities realOperators;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts complex operators.
             */
            static const Capabilities complexOperators;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts set operators.
             */
            static const Capabilities setOperators;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts tuple operators.
             */
            static const Capabilities tupleOperators;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts boolean matrix operators.
             */
            static const Capabilities matrixBooleanOperators;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts integer matrix operators.
             */
            static const Capabilities matrixIntegerOperators;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts real matrix operators.
             */
            static const Capabilities matrixRealOperators;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts complex matrix operators.
             */
            static const Capabilities matrixComplexOperators;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts ASCII alphabetic symbols "A"
             * through "Z" and "a" through "z".
             */
            static const Capabilities alphabeticSymbols;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts a string identifier symbol,
             * which is one of: <, ', or ".
             */
            static const Capabilities stringIdentifierSymbols;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts ASCII numeric symbols "0"
             * through "9".
             */
            static const Capabilities numericDecimalSymbols;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts ASCII numeric symbols "0"
             * and "1".
             */
            static const Capabilities numericBinarySymbols;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts ASCII numeric symbols "0"
             * through "9" and letters "a" through "f".
             */
            static const Capabilities numericHexidecimalSymbols;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts "x" prefixes to indicate
             * hexidecimal values.
             */
            static const Capabilities numericHexidecimalBaseIdentifierSymbols;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts "b" prefixes to indicate
             * binary values.
             */
            static const Capabilities numericBinaryBaseIdentifierSymbols;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts "i" and "j" suffixes to
             * represent complex values.
             */
            static const Capabilities numericImaginaryUnitSymbols;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accept unary plus/minus signs.
             */
            static const Capabilities numericSignSymbols;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accept numeric decimal points.
             */
            static const Capabilities numericDecimalPointSymbols;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accept numeric decimal points.
             */
            static const Capabilities numericExponentialSymbols;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts non-ASCII alphabetic symbols
             * such as the Greek, Hebrew, and Futhark letters.
             */
            static const Capabilities nonAsciiAlphabeticSymbols;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts special symbols such as the
             * Yen symbol, dollar sign, etc.
             */
            static const Capabilities specialSymbols;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts new-line/return characters.
             */
            static const Capabilities newLine;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts language expressions.
             * This value represents the union of:
             *
             *     * \ref Capabilities::assignment
             *     * \ref Capabilities::typeDeclaration
             *     * \ref Ld::Capabiltiies::iterationOperator
             *     * \ref Capabilities::conditionalOperator
             */
            static const Capabilities expressions;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts any iterable literal value.
             * This value represents the union of:
             *
             *     * \ref Capabilities::setLiterals
             *     * \ref Capabilities::tupleLiterals
             *     * \ref Capabilities::matrixLiterals
             */
            static const Capabilities iterableLiterals;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts any literal value.  This
             * value represents the union of:
             *
             *     * \ref Capabilities::numericLiterals
             *     * \ref Capabilities::setLiterals
             *     * \ref Capabilities::tupleLiterals
             *     * \ref Capabilities::matrixLiterals
             */
            static const Capabilities allLiterals;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts any operator that provides
             * a numeric value.  This value represents the union of:
             *
             *     * \ref Capabilities::integerOperators
             *     * \ref Capabilities::realOperators
             *     * \ref Capabilities::complexOperators
             */
            static const Capabilities numericOperators;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts any operator that provides
             * a scalar value.  This value represents the union of:
             *
             *     * \ref Capabilities::booleanOperators
             *     * \ref Capabilities::integerOperators
             *     * \ref Capabilities::realOperators
             *     * \ref Capabilities::complexOperators
             */
            static const Capabilities scalarOperators;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts any operator that provides
             * a matrix value.  This value represents the union of:
             *
             *     * \ref Capabilities::matrixBooleanOperators
             *     * \ref Capabilities::matrixIntegerOperators
             *     * \ref Capabilities::matrixRealOperators
             *     * \ref Capabilities::matrixComplexOperators
             */
            static const Capabilities matrixOperators;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts any operator that provides
             * an iterable value.
             *
             *     * \ref Capabilities::setOperators
             *     * \ref Capabilities::tupleOperators
             *     * \ref Capabilities::matrixBooleanOperators
             *     * \ref Capabilities::matrixIntegerOperators
             *     * \ref Capabilities::matrixRealOperators
             *     * \ref Capabilities::matrixComplexOperators
             */
            static const Capabilities iterableOperators;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts numeric and matrix
             * operators.
             *
             *     * \ref Capabilities::numericOperators
             *     * \ref Capabilities::matrixOperators
             */
            static const Capabilities numericAndMatrixOperators;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts any operator.
             *
             *     * \ref Capabilities::booleanOperators
             *     * \ref Capabilities::integerOperators
             *     * \ref Capabilities::realOperators
             *     * \ref Capabilities::complexOperators
             *     * \ref Capabilities::setOperators
             *     * \ref Capabilities::tupleOperators
             *     * \ref Capabilities::matrixBooleanOperators
             *     * \ref Capabilities::matrixIntegerOperators
             *     * \ref Capabilities::matrixRealOperators
             *     * \ref Capabilities::matrixComplexOperators
             */
            static const Capabilities allOperators;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts any function.  This value
             * represents the union of:
             *
             *     *\ref Capabilities::booleanFunctions
             *     *\ref Capabilities::integerFunctions
             *     *\ref Capabilities::realFunctions
             *     *\ref Capabilities::complexFunctions
             *     *\ref Capabilities::setFunctions
             *     *\ref Capabilities::tupleFunctions
             *     *\ref Capabilities::matrixBooleanFunctions
             *     *\ref Capabilities::matrixIntegerFunctions
             *     *\ref Capabilities::matrixRealFunctions
             *     *\ref Capabilities::matrixComplexFunctions
             */
            static const Capabilities allFunctions;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts any function returning a
             * scalar value.  This value represents the union of:
             *
             *     *\ref Capabilities::booleanFunctions
             *     *\ref Capabilities::integerFunctions
             *     *\ref Capabilities::realFunctions
             *     *\ref Capabilities::complexFunctions
             */
            static const Capabilities allScalarFunctions;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts any function returning a
             * numeric value.  This value represents the union of:
             *
             *     *\ref Capabilities::integerFunctions
             *     *\ref Capabilities::realFunctions
             *     *\ref Capabilities::complexFunctions
             */
            static const Capabilities allNumericFunctions;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts any function returning an
             * iterable value.  This value represents the union of:
             *
             *     *\ref Capabilities::setFunctions
             *     *\ref Capabilities::tupleFunctions
             *     *\ref Capabilities::matrixBooleanFunctions
             *     *\ref Capabilities::matrixIntegerFunctions
             *     *\ref Capabilities::matrixRealFunctions
             *     *\ref Capabilities::matrixComplexFunctions
             */
            static const Capabilities allIterableFunctions;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts any function returning a
             * matrix value.  This value represents the union of:
             *
             *     *\ref Capabilities::matrixBooleanFunctions
             *     *\ref Capabilities::matrixIntegerFunctions
             *     *\ref Capabilities::matrixRealFunctions
             *     *\ref Capabilities::matrixComplexFunctions
             */
            static const Capabilities allMatrixFunctions;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts an L-value.  This value
             * represents the union of:
             *
             *     * \ref Capabilities::variables
             *     * \ref Capabilities::subscripts
             */
            static const Capabilities allLValues;

            /**
             * pre-defined instance of \ref Capabilities, indicates the parent accepts a boolean R-value.  This
             * value represents the union of:
             *
             *     * \ref Capabilities::allLValues;
             *     * \ref Capabilities::booleanOperators
             *     * \ref Capabilities::specialBooleanValues
             *     * \ref Capabilities::numericLiterals
             *     * \ref Capabilities::booleanFunctions
             */
            static const Capabilities allBooleanRValues;

            /**
             * pre-defined instance of \ref Capabilities, indicates the parent accepts an integer R-value.  This
             * value represents the union of:
             *
             *     * \ref Capabilities::allLValues;
             *     * \ref Capabilities::specialBooleanValues
             *     * \ref Capabilities::specialIntegerValues
             *     * \ref Capabilities::booleanOperators
             *     * \ref Capabilities::integerOperators
             *     * \ref Capabilities::numericLiterals
             *     * \ref Capabilities::integerFunctions
             */
            static const Capabilities allIntegerRValues;

            /**
             * pre-defined instance of \ref Capabilities, indicates the parent accepts a real R-value.  This value
             * represents the union of:
             *
             *     * \ref Capabilities::allLValues;
             *     * \ref Capabilities::specialBooleanValues
             *     * \ref Capabilities::specialIntegerValues
             *     * \ref Capabilities::specialRealValues
             *     * \ref Capabilities::booleanOperators
             *     * \ref Capabilities::integerOperators
             *     * \ref Capabilities::realOperators
             *     * \ref Capabilities::numericLiterals
             *     * \ref Capabilities::realFunctions
             */
            static const Capabilities allRealRValues;

            /**
             * pre-defined instance of \ref Capabilities, indicates the parent accepts a complex R-value.  This
             * value represents the union of:
             *
             *     * \ref Capabilities::allLValues;
             *     * \ref Capabilities::specialBooleanValues
             *     * \ref Capabilities::specialIntegerValues
             *     * \ref Capabilities::specialRealValues
             *     * \ref Capabilities::specialComplexValues
             *     * \ref Capabilities::booleanOperators
             *     * \ref Capabilities::integerOperators
             *     * \ref Capabilities::realOperators
             *     * \ref Capabilities::complexOperators
             *     * \ref Ld::Capabilifties::numericLiterals
             *     * \ref Capabilities::complexFunctions
             */
            static const Capabilities allComplexRValues;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts all numeric R-values.  This
             * value represents the union of:
             *
             *     * \ref Capabilities::allLValues;
             *     * \ref Capabilities::specialIntegerValues
             *     * \ref Capabilities::specialRealValues
             *     * \ref Capabilities::specialComplexValues
             *     * \ref Capabilities::integerOperators
             *     * \ref Capabilities::realOperators
             *     * \ref Capabilities::complexOperators
             *     * \ref Capabilities::numericLiterals
             *     * \ref Capabilities::allNumericFunctions
             *
             * This value differs from \ref Capabilities::allNumericRValues in that is excludes booleans.
             */
            static const Capabilities allNumericRValues;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts a scalar R-value.  This
             * value is a synonym for \ref Capabilities::allComplexRValues.
             */
            static const Capabilities allScalarRValues;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts a scalar or matrix R-value.
             * This value represents the union of:
             *
             *     * \ref Capabilities::allScalarRValues
             *     * \ref Capabilities::matrixLiterals
             *     * \ref Capabilities::matrixOperators
             */
            static const Capabilities allScalarAndMatrixRValues;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts a scalar or matrix R-value.
             * This value represents the union of:
             *
             *     * \ref Capabilities::allNumericRValues
             *     * \ref Capabilities::matrixLiterals
             *     * \ref Capabilities::matrixOperators
             */
            static const Capabilities allNumericAndMatrixRValues;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts any set value with a finite
             * number of elements.  This value represents the union of:
             *
             *     * \ref Capabilities::allLValues;
             *     * \ref Capabilities::specialSetValues
             *     * \ref Capabilities::setLiterals
             *     * \ref Capabilities::setOperators
             *     * \ref Capabilities::setFunctions
             */
            static const Capabilities allFiniteSetRValues;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts any set value.  This value
             * represents the union of:
             *
             *     * \ref Capabilities::allLValues;
             *     * \ref Capabilities::allFiniteSetRValues
             *     * \ref Capabilities::dataTypes
             *     * \ref Capabilities::setFunctions
             */
            static const Capabilities allSetRValues;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts any tuple value.  This value
             * represents the union of:
             *
             *     * \ref Capabilities::allLValues;
             *     * \ref Capabilities::tupleLiterals
             *     * \ref Capabilities::tupleOperators
             *     * \ref Capabilities::tupleFunctions
             */
            static const Capabilities allTupleRValues;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts any boolean matrix value.
             * THis value represents the union of:
             *
             *     * \ref Capabilities::allLValues;
             *     * \ref Capabilities::matrixBooleanOperators
             *     * \ref Capabilities::matrixLiterals
             *     * \ref Capabilities::matrixBooleanFunctions
             */
            static const Capabilities allMatrixBooleanRValues;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts any integer matrix value.
             * THis value represents the union of:
             *
             *     * \ref Capabilities::allLValues;
             *     * \ref Capabilities::matrixIntegerOperators
             *     * \ref Capabilities::matrixLiterals
             *     * \ref Capabilities::matrixIntegerFunctions
             */
            static const Capabilities allMatrixIntegerRValues;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts any real matrix value.  This
             * value represents the union of:
             *
             *     * \ref Capabilities::allLValues;
             *     * \ref Capabilities::matrixRealOperators
             *     * \ref Capabilities::matrixLiterals
             *     * \ref Capabilities::matrixRealFunctions
             */
            static const Capabilities allMatrixRealRValues;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts any complex matrix value.
             * THis value represents the union of:
             *
             *     * \ref Capabilities::allLValues;
             *     * \ref Capabilities::matrixComplexOperators
             *     * \ref Capabilities::matrixLiterals
             *     * \ref Capabilities::matrixComplexFunctions
             */
            static const Capabilities allMatrixComplexRValues;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts any matrix value.  This
             * value represents the union of:
             *
             *     * \ref Capabilities::allLValues;
             *     * \ref Capabilities::matrixOperators
             *     * \ref Capabilities::matrixLiterals
             *     * \ref Capabilities::allFunctions
             */
            static const Capabilities allMatrixRValues;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts any finite set or tuple
             * value.  This value represents the union of:
             *
             *     * \ref Capabilities::allFiniteSetRValues
             *     * \ref Capabilities::allTupleRValues
             */
            static const Capabilities allFiniteSetAndTupleRValues;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts a scalar, finite set, or
             * matrix R-value.  This value represents the union of:
             *
             *     * \ref Capabilities::allLValues;
             *     * \ref Capabilities::allScalarAndMatrixRValues
             *     * \ref Capabilities::allFiniteSetRValues
             */
            static const Capabilities allScalarFiniteSetAndMatrixRValues;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts an iterable R-value.  The
             * value represents the union of:
             *     * \ref Capabilities::allLValues
             *     * \ref Capabilities::setLiterals
             *     * \ref Capabilities::tupleLiterals
             *     * \ref Capabilities::matrixLiterals
             *     * \ref Capabilities::setOperators
             *     * \ref Capabilities::tupleOperators
             *     * \ref Capabilities::matrixOperators
             *     * \ref Capabilities::allIterableFunctions
             */
            static const Capabilities allIterableRValues;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts an iterable R-value,
             * including ranges.  The value represents the union of:
             *     * \ref Capabilities::allIterableRValues
             *     * \ref Capabilities::ranges
             */
            static const Capabilities allIterableAndRangeRValues;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts any R-value.  This value
             * represents the union of:
             *
             *     * \ref Capabilities::allLValues;
             *     * \ref Capabilities::specialNumericValues
             *     * \ref Capabilities::specialBooleanValues
             *     * \ref Capabilities::specialSetValues
             *     * \ref Capabilities::allLiterals
             *     * \ref Capabilities::allOperators
             */
            static const Capabilities allRValues;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts any R-value as well as
             * ranges.  This value represents the union of:
             *
             *     * \ref Capabilities::allLValues
             *     * \ref Capabilities::allScalarAndMatrixRValues
             *     * \ref Capabilities::allSetRValues
             *     * \ref Capabilities::allTupleRValues
             */
            static const Capabilities allRValuesAndRanges;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts function parameters.  This
             * value represents the union of:
             *
             *     * \ref Capabilities::variables
             *     * \ref Capabilities::typeDeclaration
             */
            static const Capabilities functionDeclarationParameters;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts any assignable type that.
             * accepts a value.  This value is intended to be used only for explicit variable type assignments.
             *
             *     * \ref Capabilities::variables
             *     * \ref Capabilities::subscripts
             */
            static const Capabilities allValueAssignables;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts any assignable type.  This
             * value is intended to be used only for the left side of an assignment and represents the union of:
             *
             *     * \ref Capabilities::variables
             *     * \ref Capabilities::subscripts
             *     * \ref Capabilities::typeDeclarations
             *     * \ref Capabilities::functionDeclarations
             */
            static const Capabilities allAssignables;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts all annotations.  This value
             * represents the union of:
             *
             *     * \ref Capabilities::textAnnotations
             *     * \ref Capabilities::nonTextAnnotations.
             */
            static const Capabilities allAnnotations;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts all ASCII alphabetic and
             * numeric symbols.  This value represents the union of:
             *
             *     * \ref Capabilities::alphabeticSymbols
             *     * \ref Capabilities::numericSymbols
             */
            static const Capabilities asciiAlphanumericSymbols;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts all ASCII and non-ASCII
             * alphabetic symbols.  THis value represents the union of:
             *
             *     * \ref Capabilities::alphabeticSymbols
             *     * \ref Capabilities::nonAsciiAlphabeticSymbols
             */
            static const Capabilities allAlphabeticSymbols;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts all ASCII and non-ASCII
             * alphabetic and numeric symbols.  This value represents the union of:
             *
             *     * \ref Capabilities::alphabeticSymbols
             *     * \ref Capabilities::numericSymbols
             *     * \ref Ld::ParentRequiremetns::nonAsciiAlphabeticSymbols
             */
            static const Capabilities allAlphanumericSymbols;

            /**
             * Pre-defined instance of \ref Capabilities, indicates the parent accepts all symbols.  This value
             * represents the union of:
             *
             *     * \ref Capabilities::alphabeticSymbols
             *     * \ref Capabilities::numericSymbols
             *     * \ref Capabilities::nonAsciiAlphabeticSymbols
             *     * \ref Capabilities::specialSymbols
             */
            static const Capabilities allSymbols;

            Capabilities();

            ~Capabilities();

            /**
             * Copy constructor.
             *
             * \param[in] other The instance to be copied.
             */
            Capabilities(const Util::BitSet& other);

            /**
             * Copy constructor.
             *
             * \param[in] other The instance to be copied.
             */
            Capabilities(const Capabilities& other);

            /**
             * Constructor
             *
             * \param[in] bitName A literal string values or QString indicating the name of a bit to be set on
             *                    instance creation. The bit will be created if it does not already exist.
             */
            template<typename T> LD_PUBLIC_TEMPLATE_METHOD Capabilities(
                    const T& bitName
                ):Util::BitSet(
                    &bitNameHash
                ) {
                assignAndSetBits(bitName);
            }

            /**
             * Constructor
             *
             * \param[in] firstBitName       A literal string values or QString indicating the name of a bit to be set
             *                               on instance creation. The bit will be created if it does not already
             *                               exist.
             *
             * \param[in] additionalBitNames Variadic function pack that contains one or more literal string values or
             *                               QString instances.  Bits will be set based on the provided names.  The
             *                               bits will be created if they does not already exist.
             */
            template<typename T1, typename... T2> LD_PUBLIC_TEMPLATE_METHOD Capabilities(
                    const T1&    firstBitName,
                    const T2&... additionalBitNames
                ):Util::BitSet(
                    &bitNameHash
                ) {
                assignAndSetBits(firstBitName);
                assignAndSetBits(additionalBitNames...);
            }

            /**
             * Method you can use to add a bit to this bit set.
             *
             * \param[in] bitName The name of the bit.
             *
             * \return Returns true on success, returns false if the bit was already assigned.
             */
            static bool assignBit(const QString& bitName);

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            inline Capabilities operator=(const Capabilities& other) {
                Util::BitSet::operator=(other);
                return *this;
            }

        private:
            /**
             * Method that can be used to set a bit.  If the bit is not already assigned, a new assignment will be
             * created for it.
             *
             * \param[in] bitName The name of the bit.
             */
            template<typename T> LD_PUBLIC_TEMPLATE_METHOD void assignAndSetBits(const T& bitName) {
                assignBit(bitName);
                setBit(bitName, true);
            }

            /**
             * Method that can be used to set a bit.  If the bit is not already assigned, a new assignment will be
             * created for it.
             *
             * \param[in] firstBitName       A literal string values or QString indicating the name of a bit to be set
             *                               on instance creation.
             *
             * \param[in] additionalBitNames Variadic function pack that contains one or more literal string values or
             *                               QString instances.  Bits will be set based on the provided names.
             */
            template<typename T1, typename... T2> LD_PUBLIC_TEMPLATE_METHOD void assignAndSetBits(
                    const T1&    firstBitName,
                    const T2&... additionalBitNames
                ) {
                assignAndSetBits(firstBitName);
                assignAndSetBits(additionalBitNames...);
            }
    };
};

#endif
