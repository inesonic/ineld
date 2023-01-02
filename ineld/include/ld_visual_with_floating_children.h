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
* This header defines the \ref Ld::VisualWithFloatingChildren class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_VISUAL_WITH_FLOATING_CHILDREN_H
#define LD_VISUAL_WITH_FLOATING_CHILDREN_H

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_element.h"
#include "ld_location.h"
#include "ld_visual.h"

namespace Ld {
    class ElementWithFloatingChildren;

    /**
     * Pure virtual base class for visuals with a variable number of elements that occupy fixed relative positions. This
     * This class is intended to be used in tandem with the \ref ElementWithFloatingChildren class as a base class for
     * visuals.
     */
    class LD_PUBLIC_API VisualWithFloatingChildren:public virtual Visual {
        friend class ElementWithFloatingChildren;

        public:
            VisualWithFloatingChildren();

            ~VisualWithFloatingChildren() override;

            /**
             * Method that can be called to determine how children are managed and tracked by this visual.
             *
             * \return Returns a value indicating how children are tracked by this element.  This version returns
             *         \ref Ld::Element::ChildPlacement::POSITIONAL.
             */
            Element::ChildPlacement childPlacement() const override;

            /**
             * Method you can use to obtain the \ref ElementWithFloatingChildren class associated with this visual.
             *
             * \return Returns a pointer to the associated \ref ElementWithFloatingChildren instance.
             */
            QSharedPointer<ElementWithFloatingChildren> element() const;

        protected:
            /**
             * Pure virtual method that is called just before a child is removed by the associated
             * \ref ElementWithFloatingChildren class.  This method is triggered by both the
             * ElementWithFloatingChildren::removeChild(ElementPointer) method.
             *
             * \param[in] location     The current location for the child that is being removed.
             *
             * \param[in] childElement The child element being removed.
             */
            virtual void aboutToRemoveChild(const Location& location, ElementPointer childElement) = 0;

            /**
             * Pure virtual method that is called when a child is inserted at a specified location by the
             * \ref ElementWithFloatingChildren class.  This method is triggered by the
             * \ref ElementWithFloatingChildren::insertChild method.
             *
             * \param[in] childElement The child that was just added.
             *
             * \param[in] location     The desired location for the child element.
             */
            virtual void childInserted(ElementPointer childElement, const Location& location) = 0;

            /**
             * Pure virtual method that is called when a child is moved by the \ref ElementWithFloatingChildren class.
             * This method is triggered by the \ref ElementWithFloatingChildren::relocateChild method.
             *
             * \param[in] childElement The child to be relocated.
             *
             * \param[in] oldLocation  The old location for the child.
             *
             * \param[in] newLocation  The new location for the child.
             */
            virtual void childRelocated(
                ElementPointer  childElement,
                const Location& oldLocation,
                const Location& newLocation
            ) = 0;
    };
};

#endif
