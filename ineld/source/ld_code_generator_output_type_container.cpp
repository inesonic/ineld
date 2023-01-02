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
* This file implements the \ref Ld::CodeGeneratorOutputTypeContainer class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>
#include <QMap>
#include <QHash>

#include "ld_code_generator_output_type.h"
#include "ld_code_generator_output_type_container.h"

/***********************************************************************************************************************
 * CodeGeneratorOutputTypeContainer
 */

namespace Ld {
    static CodeGeneratorOutputTypeContainer undefinedContainer;

    CodeGeneratorOutputTypeContainer::CodeGeneratorOutputTypeContainer() {}


    CodeGeneratorOutputTypeContainer::CodeGeneratorOutputTypeContainer(CodeGeneratorOutputType* outputType) {
        instance.reset(outputType);
    }


    CodeGeneratorOutputTypeContainer::CodeGeneratorOutputTypeContainer(
            QSharedPointer<CodeGeneratorOutputType> outputType
        ) {
        instance = outputType;
    }


    CodeGeneratorOutputTypeContainer::CodeGeneratorOutputTypeContainer(const CodeGeneratorOutputTypeContainer& other) {
        instance = other.instance;
    }


    CodeGeneratorOutputTypeContainer::~CodeGeneratorOutputTypeContainer() {}


    bool CodeGeneratorOutputTypeContainer::isDefined() const {
        return !instance.isNull();
    }


    bool CodeGeneratorOutputTypeContainer::isUndefined() const {
        return instance.isNull();
    }


    QString CodeGeneratorOutputTypeContainer::name() const {
        return instance.isNull() ? QString() : instance->name();
    }


    unsigned CodeGeneratorOutputTypeContainer::identifier() const {
        return instance.isNull() ? invalidIdentifier : instance->identifier();
    }


    QString CodeGeneratorOutputTypeContainer::shortDescription() const {
        return instance.isNull() ? QString() : instance->shortDescription();
    }


    QString CodeGeneratorOutputTypeContainer::description() const {
        return instance.isNull() ? QString() : instance->description();
    }


    bool CodeGeneratorOutputTypeContainer::defaultOutputType() const {
        return instance.isNull() ? false : instance->defaultOutputType();
    }


    bool CodeGeneratorOutputTypeContainer::userReadable() const {
        return instance.isNull() ? false : instance->userReadable();
    }


    bool CodeGeneratorOutputTypeContainer::applicationLoadable() const {
        return instance.isNull() ? false : instance->applicationLoadable();
    }


    CodeGeneratorOutputTypeContainer::ExportModeSet CodeGeneratorOutputTypeContainer::exportable() const {
        return instance.isNull() ? ExportModeSet() : instance->exportable();
    }


    CodeGeneratorOutputTypeContainer::ExportMode CodeGeneratorOutputTypeContainer::defaultExportMode() const {
        return instance.isNull() ? ExportMode::DEFAULT : instance->defaultExportMode();
    }


    bool CodeGeneratorOutputTypeContainer::requiresIntermediateRepresentation() const {
        return instance.isNull() ? false : instance->requiresIntermediateRepresentation();
    }


    CodeGeneratorOutputTypeContainer& CodeGeneratorOutputTypeContainer::siblingOutputType() const {
        return instance.isNull() ? undefinedContainer : instance->siblingOutputType();
    }


    CodeGeneratorOutputTypeContainer& CodeGeneratorOutputTypeContainer::operator=(
            const CodeGeneratorOutputTypeContainer& other
        ) {
        instance = other.instance;
        return *this;
    }
}
