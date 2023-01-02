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
* This header defines the \ref Ld::FrameElement class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_FRAME_ELEMENT_H
#define LD_FRAME_ELEMENT_H

#include <QString>
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_data_type.h"
#include "ld_element_cursor.h"
#include "ld_element_with_positional_children.h"

namespace Ld {
    class FrameVisual;

    /**
     * Pure virtual base class you can use the frame class as a container for other elements, primarily to format those
     * elements across the rendered document view of the model.
     *
     * The class is currently just a thin wrapper on the \ref Ld::ElementWithPositionalChildren class and exists in case
     * frames are later found to need additional methods.
     */
    class LD_PUBLIC_API FrameElement:public ElementWithPositionalChildren {
        public:
            FrameElement();

            ~FrameElement() override;

            /**
             * Method you can use to determine the value type this element represents.  Elements typically either
             * either report a fixed value or calculate the value based on their children.
             *
             * \return Returns the value type this element would represent.  A value of ValueType::NONE indicates
             *         either that the element does not return a value or the value type could not be determined at
             *         this time.
             */
            DataType::ValueType valueType() const override;
    };
};

#endif
