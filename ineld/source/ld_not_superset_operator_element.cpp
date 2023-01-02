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
* This file implements the \ref Ld::NotSupersetOperatorElement class.
***********************************************************************************************************************/

#include <QCoreApplication>
#include <QString>
#include <QSharedPointer>

#include "ld_capabilities.h"
#include "ld_data_type.h"
#include "ld_not_superset_operator_visual.h"
#include "ld_subset_operator_element_base.h"
#include "ld_not_superset_operator_element.h"

namespace Ld {
    const QString NotSupersetOperatorElement::elementName("NotSupersetOperator");

    NotSupersetOperatorElement::NotSupersetOperatorElement() {}


    NotSupersetOperatorElement::~NotSupersetOperatorElement() {}


    Element* NotSupersetOperatorElement::creator(const QString&) {
        return new NotSupersetOperatorElement();
    }


    QString NotSupersetOperatorElement::typeName() const {
        return elementName;
    }


    QString NotSupersetOperatorElement::plugInName() const {
        return QString();
    }


    QString NotSupersetOperatorElement::description() const {
        return tr("not superset operator");
    }


    void NotSupersetOperatorElement::setVisual(NotSupersetOperatorVisual *newVisual) {
        SubsetOperatorElementBase::setVisual(newVisual);
    }


    NotSupersetOperatorVisual* NotSupersetOperatorElement::visual() const {
        return dynamic_cast<NotSupersetOperatorVisual*>(SubsetOperatorElementBase::visual());
    }
}
