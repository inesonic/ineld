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
* This file implements the \ref Ld::MultiplicationOperatorElement class.
***********************************************************************************************************************/

#include <QCoreApplication>
#include <QString>
#include <QSharedPointer>

#include "ld_capabilities.h"
#include "ld_data_type.h"
#include "ld_multiplication_operator_visual.h"
#include "ld_binary_operator_element_base.h"
#include "ld_multiplication_operator_element.h"

namespace Ld {
    const QString             MultiplicationOperatorElement::elementName("MultiplicationOperator");
    const DataType::ValueType MultiplicationOperatorElement::allowedConversions
        [DataType::numberValueTypes]
        [DataType::numberValueTypes] = {
        /* DataType::ValueType::NONE */
        {
            DataType::ValueType::NONE,           /* DataType::ValueType::NONE */
            DataType::ValueType::NONE,           /* DataType::ValueType::VARIANT */
            DataType::ValueType::NONE,           /* DataType::ValueType::BOOLEAN */
            DataType::ValueType::NONE,           /* DataType::ValueType::INTEGER */
            DataType::ValueType::NONE,           /* DataType::ValueType::REAL */
            DataType::ValueType::NONE,           /* DataType::ValueType::COMPLEX */
            DataType::ValueType::NONE,           /* DataType::ValueType::SET */
            DataType::ValueType::NONE,           /* DataType::ValueType::TUPLE */
            DataType::ValueType::NONE,           /* DataType::ValueType::MATRIX_BOOLEAN */
            DataType::ValueType::NONE,           /* DataType::ValueType::MATRIX_INTEGER */
            DataType::ValueType::NONE,           /* DataType::ValueType::MATRIX_REAL */
            DataType::ValueType::NONE            /* DataType::ValueType::MATRIX_COMPLEX */
        },
        /* DataType::ValueType::VARIANT */
        {
            DataType::ValueType::NONE,           /* DataType::ValueType::NONE */
            DataType::ValueType::VARIANT,        /* DataType::ValueType::VARIANT */
            DataType::ValueType::VARIANT,        /* DataType::ValueType::BOOLEAN */
            DataType::ValueType::VARIANT,        /* DataType::ValueType::INTEGER */
            DataType::ValueType::VARIANT,        /* DataType::ValueType::REAL */
            DataType::ValueType::VARIANT,        /* DataType::ValueType::COMPLEX */
            DataType::ValueType::NONE,           /* DataType::ValueType::SET */
            DataType::ValueType::TUPLE,          /* DataType::ValueType::TUPLE */
            DataType::ValueType::VARIANT,        /* DataType::ValueType::MATRIX_BOOLEAN */
            DataType::ValueType::VARIANT,        /* DataType::ValueType::MATRIX_INTEGER */
            DataType::ValueType::VARIANT,        /* DataType::ValueType::MATRIX_REAL */
            DataType::ValueType::MATRIX_COMPLEX  /* DataType::ValueType::MATRIX_COMPLEX */
        },
        /* DataType::ValueType::BOOLEAN */
        {
            DataType::ValueType::NONE,           /* DataType::ValueType::NONE */
            DataType::ValueType::VARIANT,        /* DataType::ValueType::VARIANT */
            DataType::ValueType::BOOLEAN,        /* DataType::ValueType::BOOLEAN */
            DataType::ValueType::INTEGER,        /* DataType::ValueType::INTEGER */
            DataType::ValueType::REAL,           /* DataType::ValueType::REAL */
            DataType::ValueType::COMPLEX,        /* DataType::ValueType::COMPLEX */
            DataType::ValueType::NONE,           /* DataType::ValueType::SET */
            DataType::ValueType::NONE,           /* DataType::ValueType::TUPLE */
            DataType::ValueType::MATRIX_BOOLEAN, /* DataType::ValueType::MATRIX_BOOLEAN */
            DataType::ValueType::MATRIX_INTEGER, /* DataType::ValueType::MATRIX_INTEGER */
            DataType::ValueType::MATRIX_REAL,    /* DataType::ValueType::MATRIX_REAL */
            DataType::ValueType::MATRIX_COMPLEX  /* DataType::ValueType::MATRIX_COMPLEX */
        },
        /* DataType::ValueType::INTEGER */
        {
            DataType::ValueType::NONE,           /* DataType::ValueType::NONE */
            DataType::ValueType::VARIANT,        /* DataType::ValueType::VARIANT */
            DataType::ValueType::INTEGER,        /* DataType::ValueType::BOOLEAN */
            DataType::ValueType::INTEGER,        /* DataType::ValueType::INTEGER */
            DataType::ValueType::REAL,           /* DataType::ValueType::REAL */
            DataType::ValueType::COMPLEX,        /* DataType::ValueType::COMPLEX */
            DataType::ValueType::NONE,           /* DataType::ValueType::SET */
            DataType::ValueType::NONE,           /* DataType::ValueType::TUPLE */
            DataType::ValueType::MATRIX_INTEGER, /* DataType::ValueType::MATRIX_BOOLEAN */
            DataType::ValueType::MATRIX_INTEGER, /* DataType::ValueType::MATRIX_INTEGER */
            DataType::ValueType::MATRIX_REAL,    /* DataType::ValueType::MATRIX_REAL */
            DataType::ValueType::MATRIX_COMPLEX  /* DataType::ValueType::MATRIX_COMPLEX */
        },
        /* DataType::ValueType::REAL */
        {
            DataType::ValueType::NONE,           /* DataType::ValueType::NONE */
            DataType::ValueType::VARIANT,        /* DataType::ValueType::VARIANT */
            DataType::ValueType::REAL,           /* DataType::ValueType::BOOLEAN */
            DataType::ValueType::REAL,           /* DataType::ValueType::INTEGER */
            DataType::ValueType::REAL,           /* DataType::ValueType::REAL */
            DataType::ValueType::COMPLEX,        /* DataType::ValueType::COMPLEX */
            DataType::ValueType::NONE,           /* DataType::ValueType::SET */
            DataType::ValueType::NONE,           /* DataType::ValueType::TUPLE */
            DataType::ValueType::MATRIX_REAL,    /* DataType::ValueType::MATRIX_BOOLEAN */
            DataType::ValueType::MATRIX_REAL,    /* DataType::ValueType::MATRIX_INTEGER */
            DataType::ValueType::MATRIX_REAL,    /* DataType::ValueType::MATRIX_REAL */
            DataType::ValueType::MATRIX_COMPLEX  /* DataType::ValueType::MATRIX_COMPLEX */
        },
        /* DataType::ValueType::COMPLEX */
        {
            DataType::ValueType::NONE,           /* DataType::ValueType::NONE */
            DataType::ValueType::VARIANT,        /* DataType::ValueType::VARIANT */
            DataType::ValueType::COMPLEX,        /* DataType::ValueType::BOOLEAN */
            DataType::ValueType::COMPLEX,        /* DataType::ValueType::INTEGER */
            DataType::ValueType::COMPLEX,        /* DataType::ValueType::REAL */
            DataType::ValueType::COMPLEX,        /* DataType::ValueType::COMPLEX */
            DataType::ValueType::NONE,           /* DataType::ValueType::SET */
            DataType::ValueType::NONE,           /* DataType::ValueType::TUPLE */
            DataType::ValueType::MATRIX_COMPLEX, /* DataType::ValueType::MATRIX_BOOLEAN */
            DataType::ValueType::MATRIX_COMPLEX, /* DataType::ValueType::MATRIX_INTEGER */
            DataType::ValueType::MATRIX_COMPLEX, /* DataType::ValueType::MATRIX_REAL */
            DataType::ValueType::MATRIX_COMPLEX  /* DataType::ValueType::MATRIX_COMPLEX */
        },
        /* DataType::ValueType::SET */
        {
            DataType::ValueType::NONE,           /* DataType::ValueType::NONE */
            DataType::ValueType::NONE,           /* DataType::ValueType::VARIANT */
            DataType::ValueType::NONE,           /* DataType::ValueType::BOOLEAN */
            DataType::ValueType::NONE,           /* DataType::ValueType::INTEGER */
            DataType::ValueType::NONE,           /* DataType::ValueType::REAL */
            DataType::ValueType::NONE,           /* DataType::ValueType::COMPLEX */
            DataType::ValueType::NONE,           /* DataType::ValueType::SET */
            DataType::ValueType::NONE,           /* DataType::ValueType::TUPLE */
            DataType::ValueType::NONE,           /* DataType::ValueType::MATRIX_BOOLEAN */
            DataType::ValueType::NONE,           /* DataType::ValueType::MATRIX_INTEGER */
            DataType::ValueType::NONE,           /* DataType::ValueType::MATRIX_REAL */
            DataType::ValueType::NONE            /* DataType::ValueType::MATRIX_COMPLEX */
        },
        /* DataType::ValueType::TUPLE */
        {
            DataType::ValueType::NONE,           /* DataType::ValueType::NONE */
            DataType::ValueType::TUPLE,          /* DataType::ValueType::VARIANT */
            DataType::ValueType::NONE,           /* DataType::ValueType::BOOLEAN */
            DataType::ValueType::NONE,           /* DataType::ValueType::INTEGER */
            DataType::ValueType::NONE,           /* DataType::ValueType::REAL */
            DataType::ValueType::NONE,           /* DataType::ValueType::COMPLEX */
            DataType::ValueType::NONE,           /* DataType::ValueType::SET */
            DataType::ValueType::TUPLE,          /* DataType::ValueType::TUPLE */
            DataType::ValueType::NONE,           /* DataType::ValueType::MATRIX_BOOLEAN */
            DataType::ValueType::NONE,           /* DataType::ValueType::MATRIX_INTEGER */
            DataType::ValueType::NONE,           /* DataType::ValueType::MATRIX_REAL */
            DataType::ValueType::NONE            /* DataType::ValueType::MATRIX_COMPLEX */
        },
        /* DataType::ValueType::MATRIX_BOOLEAN */
        {
            DataType::ValueType::NONE,           /* DataType::ValueType::NONE */
            DataType::ValueType::VARIANT,        /* DataType::ValueType::VARIANT */
            DataType::ValueType::MATRIX_BOOLEAN, /* DataType::ValueType::BOOLEAN */
            DataType::ValueType::MATRIX_INTEGER, /* DataType::ValueType::INTEGER */
            DataType::ValueType::MATRIX_REAL,    /* DataType::ValueType::REAL */
            DataType::ValueType::MATRIX_COMPLEX, /* DataType::ValueType::COMPLEX */
            DataType::ValueType::NONE,           /* DataType::ValueType::SET */
            DataType::ValueType::NONE,           /* DataType::ValueType::TUPLE */
            DataType::ValueType::MATRIX_BOOLEAN, /* DataType::ValueType::MATRIX_BOOLEAN */
            DataType::ValueType::MATRIX_INTEGER, /* DataType::ValueType::MATRIX_INTEGER */
            DataType::ValueType::MATRIX_REAL,    /* DataType::ValueType::MATRIX_REAL */
            DataType::ValueType::MATRIX_COMPLEX  /* DataType::ValueType::MATRIX_COMPLEX */
        },
        /* DataType::ValueType::MATRIX_INTEGER */
        {
            DataType::ValueType::NONE,           /* DataType::ValueType::NONE */
            DataType::ValueType::VARIANT,        /* DataType::ValueType::VARIANT */
            DataType::ValueType::MATRIX_INTEGER, /* DataType::ValueType::BOOLEAN */
            DataType::ValueType::MATRIX_INTEGER, /* DataType::ValueType::INTEGER */
            DataType::ValueType::MATRIX_REAL,    /* DataType::ValueType::REAL */
            DataType::ValueType::MATRIX_COMPLEX, /* DataType::ValueType::COMPLEX */
            DataType::ValueType::NONE,           /* DataType::ValueType::SET */
            DataType::ValueType::NONE,           /* DataType::ValueType::TUPLE */
            DataType::ValueType::MATRIX_INTEGER, /* DataType::ValueType::MATRIX_BOOLEAN */
            DataType::ValueType::MATRIX_INTEGER, /* DataType::ValueType::MATRIX_INTEGER */
            DataType::ValueType::MATRIX_REAL,    /* DataType::ValueType::MATRIX_REAL */
            DataType::ValueType::MATRIX_COMPLEX  /* DataType::ValueType::MATRIX_COMPLEX */
        },
        /* DataType::ValueType::MATRIX_REAL */
        {
            DataType::ValueType::NONE,           /* DataType::ValueType::NONE */
            DataType::ValueType::VARIANT,        /* DataType::ValueType::VARIANT */
            DataType::ValueType::MATRIX_REAL,    /* DataType::ValueType::BOOLEAN */
            DataType::ValueType::MATRIX_REAL,    /* DataType::ValueType::INTEGER */
            DataType::ValueType::MATRIX_REAL,    /* DataType::ValueType::REAL */
            DataType::ValueType::MATRIX_COMPLEX, /* DataType::ValueType::COMPLEX */
            DataType::ValueType::NONE,           /* DataType::ValueType::SET */
            DataType::ValueType::NONE,           /* DataType::ValueType::TUPLE */
            DataType::ValueType::MATRIX_REAL,    /* DataType::ValueType::MATRIX_BOOLEAN */
            DataType::ValueType::MATRIX_REAL,    /* DataType::ValueType::MATRIX_INTEGER */
            DataType::ValueType::MATRIX_REAL,    /* DataType::ValueType::MATRIX_REAL */
            DataType::ValueType::MATRIX_COMPLEX  /* DataType::ValueType::MATRIX_COMPLEX */
        },
        /* DataType::ValueType::MATRIX_COMPLEX */
        {
            DataType::ValueType::NONE,           /* DataType::ValueType::NONE */
            DataType::ValueType::MATRIX_COMPLEX, /* DataType::ValueType::VARIANT */
            DataType::ValueType::MATRIX_COMPLEX, /* DataType::ValueType::BOOLEAN */
            DataType::ValueType::MATRIX_COMPLEX, /* DataType::ValueType::INTEGER */
            DataType::ValueType::MATRIX_COMPLEX, /* DataType::ValueType::REAL */
            DataType::ValueType::MATRIX_COMPLEX, /* DataType::ValueType::COMPLEX */
            DataType::ValueType::NONE,           /* DataType::ValueType::SET */
            DataType::ValueType::NONE,           /* DataType::ValueType::TUPLE */
            DataType::ValueType::MATRIX_COMPLEX, /* DataType::ValueType::MATRIX_BOOLEAN */
            DataType::ValueType::MATRIX_COMPLEX, /* DataType::ValueType::MATRIX_INTEGER */
            DataType::ValueType::MATRIX_COMPLEX, /* DataType::ValueType::MATRIX_REAL */
            DataType::ValueType::MATRIX_COMPLEX  /* DataType::ValueType::MATRIX_COMPLEX */
        }
    };


    MultiplicationOperatorElement::MultiplicationOperatorElement() {}


    MultiplicationOperatorElement::~MultiplicationOperatorElement() {}


    Element* MultiplicationOperatorElement::creator(const QString&) {
        return new MultiplicationOperatorElement();
    }


    QString MultiplicationOperatorElement::typeName() const {
        return elementName;
    }


    QString MultiplicationOperatorElement::plugInName() const {
        return QString();
    }


    QString MultiplicationOperatorElement::description() const {
        return tr("multiplication operator");
    }


    DataType::ValueType MultiplicationOperatorElement::valueType() const {
        DataType::ValueType result;

        ElementPointer child0 = child(0);
        ElementPointer child1 = child(1);

        if (!child0.isNull() && !child1.isNull()) {
            result = allowedConversions[static_cast<unsigned char>(child0->valueType())]
                                       [static_cast<unsigned char>(child1->valueType())];
        } else {
            result = DataType::ValueType::NONE;
        }

        return result;
    }


    void MultiplicationOperatorElement::setVisual(MultiplicationOperatorVisual *newVisual) {
        BinaryOperatorElementBase::setVisual(newVisual);
    }


    MultiplicationOperatorVisual* MultiplicationOperatorElement::visual() const {
        return dynamic_cast<MultiplicationOperatorVisual*>(BinaryOperatorElementBase::visual());
    }


    Element::Precedence MultiplicationOperatorElement::intrinsicPrecedence() const {
        return multiplicationOperatorPrecedence;
    }
}
