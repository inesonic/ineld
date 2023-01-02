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
* This header defines the \ref Ld::VisualWithGroupedChildren class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_VISUAL_WITH_GROUPED_CHILDREN_H
#define LD_VISUAL_WITH_GROUPED_CHILDREN_H

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_element.h"
#include "ld_visual_with_positional_children.h"

namespace Ld {
    class ElementWithGroupedChildren;

    /**
     * Pure virtual base class for visuals with a variable number of elements that occupy fixed relative positions in
     * one or more group.  THis class is intended to be used in tandem with the \ref ElementWithGroupedChildren class
     * as a base class for visuals.
     */
    class LD_PUBLIC_API VisualWithGroupedChildren:public virtual VisualWithPositionalChildren {
        friend class ElementWithGroupedChildren;

        public:
            VisualWithGroupedChildren();

            ~VisualWithGroupedChildren() override;

            /**
             * Method that can be called to determine how children are managed and tracked by this visual.
             *
             * \return Returns a value indicating how children are tracked by this element.  This version returns
             *         \ref Ld::Element::ChildPlacement::GROUPED.
             */
            Element::ChildPlacement childPlacement() const override;

            /**
             * Method you can use to obtain the \ref ElementWithPositionalChildren class associated with this visual.
             *
             * \return Returns a pointer to the associated \ref ElementWithPositionalChildren instance.
             */
            QSharedPointer<ElementWithGroupedChildren> element() const;

        protected:
            /**
             * Pure virtual method that is called just before a child is removed by the associated
             * \ref ElementWithGroupedChildren class.  This method is triggered by the methods:
             *
             *     * \ref ElementWithPositionalChildren::removeChild(ElementPointer, CursorStateCollection*)
             *     * \ref ElementWithPositionalChildren::removeChild(unsigned long, CursorStateCollection*)
             *     * \ref ElementWithGroupedChildren:removeChildFromGroup
             *
             * \param[in] childIndex        The zero based index of the removed child.
             *
             * \param[in] groupIndex        The zero based group index of the group containing the child.
             *
             * \param[in] childIndexInGroup The index of the child relative to the start of the group.
             *
             * \param[in] childElement The child element being removed.
             */
            virtual void aboutToRemoveChildFromGroup(
                unsigned long  childIndex,
                unsigned       groupIndex,
                unsigned long  childIndexInGroup,
                ElementPointer childElement
            ) = 0;

            /**
             * Pure virtual method that is called just before all the children are removed from a group by the
             * associated \ref ElementWithGroupedChildren class.  This method is triggered by the methods:
             *
             *     * \ref ElementWithPositionalChildren::removeChild(ElementPointer, CursorStateCollection*)
             *     * \ref ElementWithPositionalChildren::removeChild(unsigned long, CursorStateCollection*)
             *     * \ref ElementWithGroupedChildren:removeChildFromGroup
             *
             * \param[in] startingChildIndex The zero based index of the first child to remove.
             *
             * \param[in] numberChildren     The number of children in the group.
             *
             * \param[in] groupIndex         The zero based group index of the group containing the child.
             */
            virtual void aboutToRemoveChildrenFromGroup(
                unsigned long  startingChildIndex,
                unsigned long  numberChildren,
                unsigned       groupIndex
            ) = 0;

            /**
             * Pure virtual method that is called when a child is inserted before a specified location by the
             * \ref ElementWithPositionalChildren class.  This method is triggered by the methods:
             *
             *     * \ref ElementWithPositionalChildren::insertBefore
             *     * \ref ElementWithGroupedChildren::insertIntoGroupBefore
             *
             * \param[in] childIndex        The zero based index where the desired child should be placed.
             *
             * \param[in] groupIndex        The zero based group index of the group containing the child.
             *
             * \param[in] childIndexInGroup The index of the child relative to the start of the group.
             *
             * \param[in] childElement      The new child element being placed.
             */
            virtual void childInsertedIntoGroupBefore(
                unsigned long  childIndex,
                unsigned       groupIndex,
                unsigned long  childIndexInGroup,
                ElementPointer childElement
            ) = 0;

            /**
             * Pure virtual method that is called when a child is inserted after a specified location by the
             * \ref ElementWithPositionalChildren class.  This method is triggered by the methods:
             *
             *     * \ref ElementWithPositionalChildren::insertAfter
             *     * \ref ElementWithGroupedChildren::insertIntoGroupAfter
             *
             * \param[in] childIndex        The zero based index where the desired child should be placed.  An invalid
             *                              index indicates that the child was inserted as the first element.
             *
             * \param[in] groupIndex        The zero based group index of the group containing the child.
             *
             * \param[in] childIndexInGroup The index of the child relative to the start of the group.  An invalid
             *                              index indicates that the child was inserted into an empty group.
             *
             * \param[in] childElement      The new child element being placed.
             */
            virtual void childInsertedIntoGroupAfter(
                unsigned long  childIndex,
                unsigned       groupIndex,
                unsigned long  childIndexInGroup,
                ElementPointer childElement
            ) = 0;

            /**
             * Pure virtual method that is called when one or more groups are removed.  This method is triggered by
             * the \ref Ld::ElementWithGroupedChildren::removeGroups.
             *
             * \param[in] startingGroupIndex The starting group index of the range being removed.
             *
             * \param[in] numberGroups       The number of groups being removed.
             */
            virtual void groupsRemoved(unsigned startingGroupIndex, unsigned numberGroups) = 0;

            /**
             * Pure virtual method that is called when a new group is inserted.  This method is triggered by the
             * methods:
             *
             *     * \ref Ld::ElementWithGroupedChildren::insertGroupBefore
             *     * \ref Ld::ElementWithGroupedChildren::insertGroupAfter
             *
             * \param[in] groupIndex   The group index of the first newly inserted group.
             *
             * \param[in] numberGroups The number of newly added groups.
             */
            virtual void groupsInserted(unsigned groupIndex, unsigned numberGroups) = 0;
    };
};

#endif
