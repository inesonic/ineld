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
* This file implements the \ref Ld::FrameElement class.
***********************************************************************************************************************/

#include <QString>

#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_data_type.h"
#include "ld_element_with_positional_children.h"
#include "ld_element_cursor.h"
#include "ld_format.h"
#include "ld_frame_element.h"

namespace Ld {
    FrameElement::FrameElement() {}


    FrameElement::~FrameElement() {}


    DataType::ValueType FrameElement::valueType() const {
        return DataType::ValueType::NONE;
    }


//    ElementPointer FrameElement::split(const ElementCursor& cursor, CursorStateCollection* cursorStateCollection) {
//        ElementPointer rightElement;

//        if (splitAllowed(cursor, true)) {
//            rightElement = Element::create(typeName());
//            FormatPointer leftFormat = format();
//            if (leftFormat) {
//                FormatPointer rightFormat = leftFormat->formatForSplit(weakThis().toStrongRef());
//                if (!rightFormat.isNull()) {
//                    rightElement->setFormat(rightFormat);
//                } else {
//                    rightElement->setFormat(leftFormat);
//                }
//            }

//            QSharedPointer<ElementWithPositionalChildren> positionalRightElement =
//                rightElement.dynamicCast<ElementWithPositionalChildren>();

//            unsigned long      childIndex    = cursor.childIndex();
//            ElementPointerList children      = FrameElement::children();
//            unsigned long      countChildren = static_cast<unsigned long>(children.size());

//            if (!cursor.isWholeElement()) {
//                ElementPointer newElement = children.at(childIndex)->split(cursor, cursorStateCollection);
//                Q_ASSERT(!newElement.isNull());

//                if (childIndex + 1 < countChildren) {
//                    ElementPointer rightChild = children.at(childIndex + 1);
//                    if (newElement->mergeAllowed(rightChild, true, true)  &&
//                        rightChild->mergeAllowed(newElement, false, true)    ) {
//                        newElement->merge(rightChild, cursorStateCollection);
//                        children.removeAt(childIndex + 1);
//                        --countChildren;
//                    }
//                }

//                ++childIndex;
//                ++countChildren;

//                children.insert(childIndex, newElement);
//            }

//            while (childIndex < countChildren) {
//                ElementPointer child = children.at(childIndex);
//                positionalRightElement->append(child, cursorStateCollection);
//                ++childIndex;
//            }
//        }

//        return rightElement;
//    }
}
