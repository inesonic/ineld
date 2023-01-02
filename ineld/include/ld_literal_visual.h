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
* This header defines the \ref Ld::LiteralVisual class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_LITERAL_VISUAL_H
#define LD_LITERAL_VISUAL_H

#include <QSharedPointer>

#include "ld_common.h"
#include "ld_variable_element.h"
#include "ld_visual_with_no_children.h"

namespace Ld {
    class LiteralElement;

    /**
     * Visual used to interact with a \ref Ld::LiteralElement instance.
     */
    class LD_PUBLIC_API LiteralVisual:public virtual VisualWithNoChildren {
        friend class LiteralElement;

        public:
            LiteralVisual();

            ~LiteralVisual() override;

            /**
             * Method you can use to obtain the \ref ElementWithNoChildren class associated with this visual.
             *
             * \return Returns a pointer to the associated \ref ElementWithNoChildren instance.
             */
            QSharedPointer<LiteralElement> element() const;

            /**
             * Method that returns the type name associated with this visual.
             *
             * \return Returns the typename associated with this visual.  The value must match the type used to invoke
             *         the static \ref Visual::create and \ref Visual::registerCreator methods.
             */
            QString typeName() const override;

            /**
             * Method that returns the name of the plug-in that provides this visual.
             *
             * \return Returns the name of the plug-in that provides this visual. The name must match the name provided
             *         in the plug-in structures.  Visuals defined as part of the ineld library should return an empty
             *         string.
             */
            QString plugInName() const override;

        protected:
            /**
             * Method that is called when the text managed by the element is changed.  You can overload this method to
             * receive notification that text has changed.  Also note the \ref Ld::Visual::elementDataChanged method.
             *
             * \param[in] newText     The new text.
             *
             * \param[in] regionIndex The zero based region index.
             */
            virtual void textChanged(const QString& newText, unsigned regionIndex);
    };
};

#endif
