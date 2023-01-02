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
* This file implements the \ref Ld::RootVisual class.
***********************************************************************************************************************/

#include <QSharedPointer>

#include "ld_element_structures.h"
#include "ld_element.h"
#include "ld_root_element.h"
#include "ld_visual_with_positional_children.h"
#include "ld_root_visual.h"

namespace Ld {
    RootVisual::RootVisual() {}


    RootVisual::~RootVisual() {}


    QSharedPointer<RootElement> RootVisual::element() const {
        return Visual::element().dynamicCast<RootElement>();
    }


    QString RootVisual::typeName() const {
        return RootElement::elementName;
    }


    QString RootVisual::plugInName() const {
        return QString();
    }


    bool RootVisual::canDeleteRootElement() {
        return true;
    }


    void RootVisual::elementChanged(ElementPointer) {}


    void RootVisual::elementAdded(ElementPointer) {}


    void RootVisual::elementRemoved(ElementPointer) {}


    void RootVisual::pageFormatChanged(unsigned long, QSharedPointer<PageFormat>, QSharedPointer<PageFormat>) {}


    void RootVisual::nowPristine() {}


    void RootVisual::nowModified() {}


    void RootVisual::nowChanged() {}


    void RootVisual::referencesChanged() {}


    void RootVisual::programSaved(const QString&) {}


    void RootVisual::programSavedAs(const QString&) {}


    void RootVisual::programSaveFailed(const QString&) {}


    void RootVisual::programLoaded(const QString&, unsigned, unsigned) {}


    void RootVisual::programLoadFailed(const QString&) {}


    void RootVisual::newProgramCreated() {}


    void RootVisual::aboutToClose() {}
}
