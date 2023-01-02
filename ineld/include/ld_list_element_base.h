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
* This header defines the \ref Ld::ListElementBase class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_LIST_ELEMENT_BASE_H
#define LD_LIST_ELEMENT_BASE_H

#include <QString>
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_element_cursor.h"
#include "ld_element_with_positional_children.h"

namespace Ld {
    class FrameVisual;

    /**
     * Pure virtual base class you can use for sets, arrays, vectors, and similar mathematics constructs that manage
     * lists of member elements.
     */
    class LD_PUBLIC_API ListElementBase:public ElementWithPositionalChildren {
        public:
            ListElementBase();

            ~ListElementBase() override;

            /**
             * Method you can call to determine if this element represents a list of elements, typically separated by
             * commas.
             *
             * \return Returns true if this element represents an open-ended list of elements.  Returns false if this
             *         element does not represent an open-ended list of elements.  This version returns true.
             */
            bool isListElement() const override;

            /**
             * Method you can call to detemrine if this element provides a logical stop for function parenthesis.  If
             * true, functions will act as if they're top level.  If false, functions will act as if they're within a
             * larger equation and will always place parenthesis around parameters.
             *
             * This version will return true if the provided function child index is the last non list or function
             * placeholder child.  The function will return false in all other cases.
             *
             * \param[in] functionChildIndex The index of the function element being queried.
             *
             * \return Returns true if the function can assume it's at a logical stop in the equation such as the last
             *         position in an equation or at a position, such as directly under a matrix, that can be treated
             *         as the last position.  Returns false in all other cases.
             */
            bool isFunctionLogicalStop(unsigned long functionChildIndex) const override;

        protected:
            /**
             * Method that is called when this element is grafted to a new tree to perform any additional
             * initialization that may be needed.  This method is called by \ref Ld::Element::graftedToTree just before
             * triggering \ref Ld::Visual::graftedToTree.
             *
             * This version will setup the text element's format if the format is not defined and the root is
             * of type \ref Ld::RootElement.
             */
            void updateAfterGraft() override;
    };
};

#endif
