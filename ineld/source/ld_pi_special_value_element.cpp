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
* This file implements the \ref Ld::PiSpecialValueElement class.
***********************************************************************************************************************/

#include <QCoreApplication>
#include <QString>
#include <QSharedPointer>

#include "ld_data_type.h"
#include "ld_capabilities.h"
#include "ld_pi_special_value_visual.h"
#include "ld_special_symbol_element_base.h"
#include "ld_pi_special_value_element.h"

namespace Ld {
    const QString PiSpecialValueElement::elementName("PiSpecialValue");
    const Capabilities PiSpecialValueElement::childProvides = Capabilities::specialRealValues;

    PiSpecialValueElement::PiSpecialValueElement() {}


    PiSpecialValueElement::~PiSpecialValueElement() {}


    Element* PiSpecialValueElement::creator(const QString&) {
        return new PiSpecialValueElement();
    }


    QString PiSpecialValueElement::typeName() const {
        return elementName;
    }


    QString PiSpecialValueElement::plugInName() const {
        return QString();
    }


    QString PiSpecialValueElement::description() const {
        return tr("special value %1").arg(QChar(0x3C0));
    }


    DataType::ValueType PiSpecialValueElement::valueType() const {
        return DataType::ValueType::REAL;
    }


    Capabilities PiSpecialValueElement::childProvidesCapabilities() const {
        return childProvides;
    }


    void PiSpecialValueElement::setVisual(PiSpecialValueVisual *newVisual) {
        SpecialSymbolElementBase::setVisual(newVisual);
    }


    PiSpecialValueVisual* PiSpecialValueElement::visual() const {
        return dynamic_cast<PiSpecialValueVisual*>(SpecialSymbolElementBase::visual());
    }
}
