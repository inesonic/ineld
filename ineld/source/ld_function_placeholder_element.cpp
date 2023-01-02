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
* This file implements the \ref Ld::FunctionPlaceholderElement class.
***********************************************************************************************************************/

#include <QCoreApplication>
#include <QString>
#include <QSharedPointer>

#include "ld_data_type.h"
#include "ld_capabilities.h"
#include "ld_function_placeholder_visual.h"
#include "ld_special_symbol_element_base.h"
#include "ld_function_placeholder_element.h"

namespace Ld {
    const QString      FunctionPlaceholderElement::elementName("FunctionPlaceholder");
    const Capabilities FunctionPlaceholderElement::childProvides = Capabilities();

    FunctionPlaceholderElement::FunctionPlaceholderElement() {}


    FunctionPlaceholderElement::~FunctionPlaceholderElement() {}


    Element* FunctionPlaceholderElement::creator(const QString&) {
        return new FunctionPlaceholderElement();
    }


    QString FunctionPlaceholderElement::typeName() const {
        return elementName;
    }


    QString FunctionPlaceholderElement::plugInName() const {
        return QString();
    }


    QString FunctionPlaceholderElement::description() const {
        return tr("placeholder");
    }


    DataType::ValueType FunctionPlaceholderElement::valueType() const {
        return DataType::ValueType::NONE;
    }


    bool FunctionPlaceholderElement::isPlaceholder() const {
        return true;
    }


    Capabilities FunctionPlaceholderElement::childProvidesCapabilities() const {
        return childProvides;
    }


    void FunctionPlaceholderElement::setVisual(FunctionPlaceholderVisual *newVisual) {
        SpecialSymbolElementBase::setVisual(newVisual);
    }


    FunctionPlaceholderVisual* FunctionPlaceholderElement::visual() const {
        return dynamic_cast<FunctionPlaceholderVisual*>(SpecialSymbolElementBase::visual());
    }


    void FunctionPlaceholderElement::setFieldString(const QString& newFieldString) {
        currentFieldString = newFieldString;
        elementDataChanged();
    }


    QString FunctionPlaceholderElement::fieldString() const {
        return currentFieldString;
    }
}
