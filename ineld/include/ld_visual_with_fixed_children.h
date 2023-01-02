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
* This header defines the \ref Ld::VisualWithFixedChildren class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_VISUAL_WITH_FIXED_CHILDREN_H
#define LD_VISUAL_WITH_FIXED_CHILDREN_H

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_element.h"
#include "ld_visual.h"

namespace Ld {
    class ElementWithFixedChildren;

    /**
     * Pure virtual base class for visuals with a fixed number of children.  This class is intended to be used in tandem
     * with the \ref ElementWithFixedChildren class as a base class for visuals.
     */
    class LD_PUBLIC_API VisualWithFixedChildren:public virtual Visual {
        friend class ElementWithFixedChildren;

        public:
            VisualWithFixedChildren();

            ~VisualWithFixedChildren() override;

            /**
             * Method that can be called to determine how children are managed and tracked by this visual.
             *
             * \return Returns a value indicating how children are tracked by this element.  This version returns
             *         \ref Ld::Element::ChildPlacement::FIXED.
             */
            Element::ChildPlacement childPlacement() const override;

            /**
             * Method you can use to obtain the \ref ElementWithFixedChildren class associated with this visual.
             *
             * \return Returns a pointer to the associated \ref ElementWithFixedChildren instance.
             */
            QSharedPointer<ElementWithFixedChildren> element() const;

        protected:
            /**
             * Pure virtual method that is called just before a child is removed by the associated
             * \ref ElementWithFixedChildren class.  This method is triggered by both the
             * ElementWithFixedChildren::removeChild(ElementPointer) method and the
             * ElementWithFixedChildren::removeChild(unsigned long) method.
             *
             * \param[in] childIndex   The zero based index of the removed child.
             *
             * \param[in] childElement The child element being removed.
             */
            virtual void aboutToRemoveChild(unsigned long childIndex, ElementPointer childElement) = 0;

            /**
             * Pure virtual method that is called when a child is changed by the associated
             * \ref ElementWithFixedChildren class.  This method is triggered by the
             * \ref ElementWithFixedChildren::setChild method.
             *
             * \param[in] childIndex   The zero based index where the desired child should be placed.
             *
             * \param[in] childElement The new child element being placed.
             */
            virtual void childSet(unsigned long childIndex, ElementPointer childElement) = 0;
    };
};

#endif
