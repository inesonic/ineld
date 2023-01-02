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
* This header defines the \ref Ld::PayloadVisual class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_PAYLOAD_VISUAL_H
#define LD_PAYLOAD_VISUAL_H

#include "ld_common.h"
#include "ld_element.h"
#include "ld_visual_with_no_children.h"

namespace Ld {
    class PayloadElement;

    /**
     * Pure virtual base class for visuals for \ref Ld::PayloadElement instances.
     */
    class LD_PUBLIC_API PayloadVisual:public virtual VisualWithNoChildren {
        friend class PayloadElement;

        public:
            PayloadVisual();

            ~PayloadVisual() override;

            /**
             * Method you can use to obtain the \ref PayloadElement class associated with this visual.
             *
             * \return Returns a pointer to the associated \ref PayloadElement instance.
             */
            QSharedPointer<PayloadElement> element() const;

        protected:
            /**
             * Method that is called when the payload tied to this visual changes.  The default implementation simply
             * returns.
             */
            virtual void payloadChanged();

            /**
             * Method that is called when a load failure occurs with a payload.  The default implementation simply
             * returns.
             */
            virtual void payloadCouldNotBeLoaded();
    };
};

#endif
