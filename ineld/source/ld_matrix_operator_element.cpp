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
* This file implements the \ref Ld::MatrixOperatorElement class.
***********************************************************************************************************************/

#include <QCoreApplication>
#include <QString>
#include <QSharedPointer>

#include "ld_capabilities.h"
#include "ld_data_type.h"
#include "ld_matrix_operator_visual.h"
#include "ld_element_with_grid_children.h"
#include "ld_matrix_combine_left_to_right_operator_element.h"
#include "ld_matrix_combine_top_to_bottom_operator_element.h"
#include "ld_parenthesis_format.h"
#include "ld_parenthesis_format.h"
#include "ld_operator_format.h"
#include "ld_matrix_operator_element.h"

namespace Ld {
    const QString      MatrixOperatorElement::elementName("MatrixOperator");
    const Capabilities MatrixOperatorElement::childProvides = Capabilities::matrixLiterals;

    MatrixOperatorElement::MatrixOperatorElement() {}


    MatrixOperatorElement::~MatrixOperatorElement() {}


    Element* MatrixOperatorElement::creator(const QString&) {
        return new MatrixOperatorElement();
    }


    QString MatrixOperatorElement::typeName() const {
        return elementName;
    }


    QString MatrixOperatorElement::plugInName() const {
        return QString();
    }


    QString MatrixOperatorElement::description() const {
        return tr("MatrixOperator");
    }


    Element::Precedence MatrixOperatorElement::parentPrecedence(unsigned long /* childIndex */) const {
        return matrixOperatorPrecedence;
    }


    Element::Precedence MatrixOperatorElement::childPrecedence() const {
        return matrixOperatorPrecedence;
    }


    Capabilities MatrixOperatorElement::parentRequires(unsigned long /* index */) const {
        return Capabilities::allScalarRValues;
    }


    Capabilities MatrixOperatorElement::childProvidesCapabilities() const {
        return childProvides;
    }


    Element::ParenthesisStyle MatrixOperatorElement::parenthesisStyle() const {
        ParenthesisStyle result = ParenthesisStyle::INVALID_OR_NONE;

        FormatPointer format = MatrixOperatorElement::format();
        if (format->capabilities().contains(OperatorFormat::formatName)) {
            QSharedPointer<OperatorFormat> operatorFormat = format.dynamicCast<OperatorFormat>();
            assert(!operatorFormat.isNull());

            if (operatorFormat->forcedParenthesis() || parenthesisRequired()) {
                result = operatorFormat->parenthesisStyle();
            }
        } else {
            if (parenthesisRequired()) {
                if (format->capabilities().contains(ParenthesisFormat::formatName)) {
                    QSharedPointer<ParenthesisFormat> parenthesisFormat = format.dynamicCast<ParenthesisFormat>();
                    result = parenthesisFormat->parenthesisStyle();
                }
            }
        }

        if (result == ParenthesisStyle::DEFAULT) {
            result = ParenthesisStyle::BRACKETS;
        }

        return result;
    }


    DataType::ValueType MatrixOperatorElement::valueType() const {
        DataType::ValueType result = DataType::ValueType::NONE;

        unsigned long currentNumberChildren = numberChildren();
        if (currentNumberChildren > 0) {
            result = child(0)->valueType();

            for (unsigned long i=1 ; i<currentNumberChildren ; ++i) {
                ElementPointer c = child(i);
                DataType::ValueType childType = c->valueType();
                result = DataType::bestUpcast(result, childType);
            }
        }

        return   result == DataType::ValueType::VARIANT
               ? DataType::ValueType::MATRIX_COMPLEX
               : DataType::matrixTypeFromBaseType(result);
    }


    bool MatrixOperatorElement::isMatrix() const {
        return true;
    }


    bool MatrixOperatorElement::isFunctionLogicalStop(unsigned long /* functionChildIndex */) const {
        return true;
    }


    void MatrixOperatorElement::setVisual(MatrixOperatorVisual *newVisual) {
        ElementWithGridChildren::setVisual(newVisual);
    }


    MatrixOperatorVisual* MatrixOperatorElement::visual() const {
        return dynamic_cast<MatrixOperatorVisual*>(ElementWithGridChildren::visual());
    }


    void MatrixOperatorElement::updateAfterGraft() {
        ElementWithGridChildren::updateAfterGraft();
    }


    bool MatrixOperatorElement::parenthesisRequired() const {
        bool           result = true;
        ElementPointer parent = MatrixOperatorElement::parent();
        if (!parent.isNull()) {
            QString parentTypeName = parent->typeName();
            result = (
                   parentTypeName != MatrixCombineLeftToRightOperatorElement::elementName
                && parentTypeName != MatrixCombineTopToBottomOperatorElement::elementName
            );
        }

        return result;
    }
}
