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
* This file implements the \ref Ld::CompoundStatementOperatorElement class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>

#include "ld_data_type.h"
#include "ld_therefore_operator_element.h"
#include "ld_element.h"
#include "ld_list_element_base.h"
#include "ld_compound_statement_operator_visual.h"
#include "ld_compound_statement_operator_element.h"

namespace Ld {
    const QString      CompoundStatementOperatorElement::elementName("CompoundStatementOperator");
    const Capabilities CompoundStatementOperatorElement::childProvides = Capabilities::compoundStatementOperator;

    CompoundStatementOperatorElement::CompoundStatementOperatorElement() {}


    CompoundStatementOperatorElement::~CompoundStatementOperatorElement() {}


    Element* CompoundStatementOperatorElement::creator(const QString&) {
        return new CompoundStatementOperatorElement();
    }


    QString CompoundStatementOperatorElement::typeName() const {
        return elementName;
    }


    QString CompoundStatementOperatorElement::plugInName() const {
        return QString();
    }


    QString CompoundStatementOperatorElement::description() const {
        return tr("compound statement");
    }


    Element::Precedence CompoundStatementOperatorElement::parentPrecedence(unsigned long /* childIndex */) const {
        return defaultPrecedence;
    }


    Element::Precedence CompoundStatementOperatorElement::childPrecedence() const {
        return defaultPrecedence;
    }


    DataType::ValueType CompoundStatementOperatorElement::valueType() const {
        DataType::ValueType result = DataType::ValueType::NONE;

        unsigned long numberChildren = CompoundStatementOperatorElement::numberChildren();
        for (unsigned long i=0 ; i<numberChildren ; ++i) {
            ElementPointer child = CompoundStatementOperatorElement::child(i);
            if (!child.isNull() && child->typeName() == ThereforeOperatorElement::elementName) {
                DataType::ValueType childValueType = child->valueType();
                if (childValueType != DataType::ValueType::NONE) {
                    if (result == DataType::ValueType::NONE) {
                        result = childValueType;
                    } else {
                        result = DataType::bestUpcast(result, childValueType);
                    }
                }
            }
        }

        return result;
    }


    Capabilities CompoundStatementOperatorElement::parentRequires(unsigned long) const {
        return Capabilities::expressions | Capabilities::compoundExitOperator;
    }


    Capabilities CompoundStatementOperatorElement::childProvidesCapabilities() const {
        return childProvides;
    }


    void CompoundStatementOperatorElement::setVisual(CompoundStatementOperatorVisual* newVisual) {
        ListElementBase::setVisual(newVisual);
    }


    CompoundStatementOperatorVisual* CompoundStatementOperatorElement::visual() const {
        return dynamic_cast<CompoundStatementOperatorVisual*>(ListElementBase::visual());
    }


    bool CompoundStatementOperatorElement::isFunctionLogicalStop(unsigned long /* functionChildIndex */) const {
        return true;
    }
}
