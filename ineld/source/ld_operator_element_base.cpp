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
* This file implements the \ref Ld::OperatorElementBase class.
***********************************************************************************************************************/

#include <QSharedPointer>

#include "ld_capabilities.h"
#include "ld_element_structures.h"
#include "ld_character_format.h"
#include "ld_operator_format.h"
#include "ld_root_element.h"
#include "ld_element_with_fixed_children.h"
#include "ld_operator_element_base.h"

namespace Ld {
    OperatorElementBase::OperatorElementBase() {}


    OperatorElementBase::~OperatorElementBase() {}


    DataType::ValueType OperatorElementBase::valueType() const {
        unsigned long  numberChildren = ElementWithFixedChildren::numberChildren();
        ElementPointer child0         = child(0);

        DataType::ValueType bestUpcast = child0.isNull() ? DataType::ValueType::NONE : child0->valueType();

        for (unsigned long childIndex=1 ; childIndex<numberChildren ; ++childIndex) {
            ElementPointer      child     = ElementWithFixedChildren::child(childIndex);
            DataType::ValueType valueType = child.isNull() ? DataType::ValueType::NONE : child->valueType();

            bestUpcast = DataType::bestUpcast(bestUpcast, valueType);
        }

        return bestUpcast;
    }


    Element::Precedence OperatorElementBase::parentPrecedence(unsigned long /* childIndex */) const {
        return intrinsicPrecedence();
    }


    Element::Precedence OperatorElementBase::childPrecedence() const {
        Precedence result;

        QSharedPointer<OperatorFormat> format = OperatorElementBase::format()
                                                .dynamicCast<OperatorFormat>();

        if (format.isNull()) {
            result = intrinsicPrecedence();
        } else {
            if (format->capabilities().contains(Ld::OperatorFormat::formatName)) {
                bool forcedParenthesis = format->forcedParenthesis();
                if (forcedParenthesis) {
                    result = groupingOperatorPrecedence;
                } else {
                    result = intrinsicPrecedence();
                }
            } else {
                result = intrinsicPrecedence();
            }
        }

        return result;
    }


    Element::Precedence OperatorElementBase::intrinsicPrecedence() const {
        return defaultPrecedence;
    }


    void OperatorElementBase::updateAfterGraft() {
        if (format().isNull()) {
            ElementPointer rootElement = root();
            assert(!rootElement.isNull());

            if (rootElement->typeName() == Ld::RootElement::elementName) {
                QSharedPointer<Ld::RootElement> root = rootElement.dynamicCast<Ld::RootElement>();
                assert(!root.isNull());

                setFormat(root->defaultMathTextFormat());
            }
        }
    }
}
