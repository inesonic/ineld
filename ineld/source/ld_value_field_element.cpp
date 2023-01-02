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
* This file implements the \ref Ld::ValueFieldElement class.
***********************************************************************************************************************/

#include <QCoreApplication>
#include <QString>
#include <QSharedPointer>

#include <model_variant.h>

#include "ld_capabilities.h"
#include "ld_data_type.h"
#include "ld_calculated_value.h"
#include "ld_value_field_visual.h"
#include "ld_element_with_no_children.h"
#include "ld_value_field_element.h"

namespace Ld {
    const QString      ValueFieldElement::elementName("ValueField");
    const Capabilities ValueFieldElement::childProvides = Capabilities::nonTextAnnotations;

    ValueFieldElement::ValueFieldElement() {}


    ValueFieldElement::~ValueFieldElement() {}


    Element* ValueFieldElement::creator(const QString&) {
        return new ValueFieldElement();
    }


    QString ValueFieldElement::typeName() const {
        return elementName;
    }


    QString ValueFieldElement::plugInName() const {
        return QString();
    }


    QString ValueFieldElement::description() const {
        return tr("Value Field");
    }


    DataType::ValueType ValueFieldElement::valueType() const {
        return DataType::ValueType::NONE;
    }


    Capabilities ValueFieldElement::childProvidesCapabilities() const {
        return childProvides;
    }


    void ValueFieldElement::setVisual(ValueFieldVisual *newVisual) {
        ElementWithNoChildren::setVisual(newVisual);
    }


    ValueFieldVisual* ValueFieldElement::visual() const {
        return dynamic_cast<ValueFieldVisual*>(ElementWithNoChildren::visual());
    }


    ValueFieldElement::ExportImageCapability ValueFieldElement::exportImageCapability() const {
        return ExportImageCapability::THROUGH_VISUAL_EPHEMERAL;
    }


    bool ValueFieldElement::requiresCalculatedDataToRender() const {
        return true;
    }


    unsigned ValueFieldElement::numberCalculatedValues() const {
        return 1;
    }


    void ValueFieldElement::setCalculatedValue(unsigned valueIndex, const CalculatedValue& calculatedValue) {
        if (valueIndex == 0) {
            currentCalculatedValue = calculatedValue;
        }

        ElementWithNoChildren::setCalculatedValue(valueIndex, calculatedValue);
    }


    void ValueFieldElement::clearCalculatedValue() {
        currentCalculatedValue = CalculatedValue();
        ElementWithNoChildren::clearCalculatedValue();
    }


    CalculatedValue ValueFieldElement::calculatedValue(unsigned valueIndex) const {
        return valueIndex == 0 ? currentCalculatedValue : CalculatedValue();
    }
}
