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
* This file implements the \ref Ld::Visual class.
***********************************************************************************************************************/

#include <QSharedPointer>
#include <QByteArray>

#include <model_variant.h>

#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_element.h"
#include "ld_format.h"
#include "ld_diagnostic.h"
#include "ld_diagnostic_structures.h"
#include "ld_data_type.h"
#include "ld_visual.h"

namespace Ld {
    QMap<QString, Visual::CreatorFunction> Visual::creators;

    Visual::Visual() {
        currentElement = nullptr;
    }


    Visual::~Visual() {
        ElementPointer element = currentElement.toStrongRef();
        if (element) {
            element->currentVisual = nullptr;
        }
    }


    Visual* Visual::create(const QString& typeName) {
        Visual*                 newVisual      = nullptr;
        Visual::CreatorFunction creatorFunction = creators.value(typeName);

        if (creatorFunction != nullptr) {
            newVisual = (*creatorFunction)(typeName);
        }

        return newVisual;
    }


    Visual* Visual::create(const char* typeName) {
        return create(QString(typeName));
    }


    ElementPointer Visual::element() const {
        return currentElement.toStrongRef();
    }


    bool Visual::registerCreator(
            const QString&          typeName,
            Visual::CreatorFunction creatorFunction,
            bool                    overwriteExisting
        ) {
        bool uniqueEntry = !creators.contains(typeName);

        if (uniqueEntry || overwriteExisting) {
            creators.insert(typeName, creatorFunction);
        }

        return uniqueEntry;
    }


    bool Visual::registerCreator(
            const char*             typeName,
            Visual::CreatorFunction creatorFunction,
            bool                    overwriteExisting
        ) {
        return registerCreator(QString(typeName), creatorFunction, overwriteExisting);
    }


    void Visual::coupledToElement(ElementPointer) {}


    void Visual::decoupledFromElement() {}


    void Visual::formatChanged(FormatPointer, FormatPointer) {}


    void Visual::descendantFormatChanged(ElementPointer, FormatPointer) {}


    void Visual::descendantFormatUpdated(ElementPointer, FormatPointer) {}


    void Visual::elementDataChanged() {}


    void Visual::diagnosticReported(DiagnosticPointer) {}


    void Visual::diagnosticCleared() {}


    void Visual::graftedToTree() {}


    void Visual::aboutToUngraftFromTree() {}


    void Visual::instructionBreakpointUpdated(bool) {}


    void Visual::calculatedValueUpdated(unsigned, const CalculatedValue&) {}


    void Visual::calculatedValueCleared() {}


    QByteArray Visual::generateImage(float /* dpi */) const {
        return QByteArray();
    }
}
