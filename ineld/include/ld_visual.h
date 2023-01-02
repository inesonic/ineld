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
* This header defines the \ref Ld::Visual class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_VISUAL_H
#define LD_VISUAL_H

#include <QSharedPointer>
#include <QWeakPointer>
#include <QByteArray>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_diagnostic_structures.h"
#include "ld_element.h"

namespace M {
    class Variant;
}

namespace Ld {
    class CodeGeneratorDiagnostic;
    class Element;
    class Format;
    class CalculatedValue;

    /**
     * Pure virtual base class for visuals that are used to view and manipulate a language \ref Element.
     */
    class LD_PUBLIC_API Visual {
        friend class Element;
        friend class Format;

        public:
            /**
             * Type used to define function pointers to visual creation functions.
             *
             * \param[in] typeName The name used to reference this creator function.  A single creator function can be
             *                     used to create multiple visual types using this value to identify what type of
             *                     visual to be created.
             */
            typedef Visual* (*CreatorFunction)(const QString& typeName);

            Visual();

            virtual ~Visual();

            /**
             * Method that can be called to determine how children are managed and tracked by this visual.
             *
             * \return Returns a value indicating how children are tracked by this element.
             */
            virtual Element::ChildPlacement childPlacement() const = 0;

            /**
             * Method that returns the type name associated with this visual.
             *
             * \return Returns the typename associated with this visual.  The value must match the type used to invoke
             *         the static \ref Visual::create and \ref Visual::registerCreator methods.
             */
            virtual QString typeName() const = 0;

            /**
             * Method that returns the name of the plug-in that provides this visual.
             *
             * \return Returns the name of the plug-in that provides this visual. The name must match the name provided
             *         in the plug-in structures.  Visuals defined as part of the ineld library should return an empty
             *         string.
             */
            virtual QString plugInName() const = 0;

            /**
             * Static method you can use to create an visual given a name.
             *
             * \param[in] typeName The name used to reference this visual type.
             *
             * \return Returns a pointer to a class derived from \ref Visual.
             */
            static Visual* create(const QString& typeName);

            /**
             * Static convenience method you can use to create an visual given a name.
             *
             * \param[in] typeName The name used to reference this visual type.
             *
             * \return Returns a pointer to a class derived from \ref Visual.
             */
            static Visual* create(const char* typeName);

            /**
             * Method you can use to obtain the \ref Element associated with this visual.
             *
             * \return Returns a pointer to the associated \ref Element instance.
             */
            ElementPointer element() const;

            /**
             * Static method you can use to register methods used to create visuals by type name.
             *
             * \param[in] typeName          The name used to reference this visual type.
             *
             * \param[in] creatorFunction   Pointer to the function that should be called to create the visual.
             *
             * \param[in] overwriteExisting If true, any existing creatir method will be overwritten with a new entry.
             *                              If false, this method will fail (return false) and the existing entry will
             *                              not be overwritten.
             *
             * \return Returns true if this entry is unique.  Returns false if an entry for the element already exists.
             *         Note that false will be returned even if an existing entry was overwritten.
             */
            static bool registerCreator(
                const QString&          typeName,
                Visual::CreatorFunction creatorFunction,
                bool                    overwriteExisting = false
            );

            /**
             * Static convenience method you can use to register methods used to create visuals by type name.
             *
             * \param[in] typeName          The name used to reference this visual type.
             *
             * \param[in] creatorFunction   Pointer to the function that should be called to create the visual.
             *
             * \param[in] overwriteExisting If true, any existing creator method will be overwritten with a new entry.
             *                              If false, this method will fail (return false) and the existing entry will
             *                              not be overwritten.
             *
             * \return Returns true if this entry is unique.  Returns false if an entry for the element already exists.
             *         Note that false will be returned even if an existing entry was overwritten.
             */
            static bool registerCreator(
                const char*             typeName,
                Visual::CreatorFunction creatorFunction,
                bool                    overwriteExisting = false
            );

        protected:
            /**
             * Method that is called when this visual is tied to an element.
             *
             * \param[in] element The element that this visual is now tied to.
             */
            virtual void coupledToElement(ElementPointer element);

            /**
             * Method that is called when this visual is disconnected from an element.
             */
            virtual void decoupledFromElement();

            /**
             * Method that is called when the format tied to the associated \ref Ld::Element changes or is updated.  The
             * method can be triggered by either the \ref Ld::Element::setFormat method or by the
             * \ref Ld::Format::reportFormatUpdated method.  The default implementation simply returns.
             *
             * \param[in] oldFormat A pointer to the old \ref Ld::Format instance.  This value may be a null pointer if
             *                      the \ref Ld::Element did not previously have an associated \ref Ld::Format instance.
             *
             * \param[in] newFormat A pointer to the new \ref Ld::Format instance.  This pointer will be the same as the
             *                      oldFormat pointer if the \ref Ld::Format instance was not changed.
             */
            virtual void formatChanged(FormatPointer oldFormat, FormatPointer newFormat);

            /**
             * Method that is triggered whenever a descendant's format has been changed.
             *
             * \param[in] changedElement The element who's format has been changed.
             *
             * \param[in] newFormat      The new format associated with this element.
             */
            virtual void descendantFormatChanged(ElementPointer changedElement, FormatPointer newFormat);

            /**
             * Method that is triggered whenever a descendant's format has been updated.
             *
             * \param[in] changedElement The element who's format has been changed.
             *
             * \param[in] newFormat      The updated format associated with this element.
             */
            virtual void descendantFormatUpdated(ElementPointer changedElement, FormatPointer newFormat);

            /**
             * Method that is called when an aspect of the associated \ref Ld::Element has been changed.  This method
             * exists to cover such things as text updates that are not covered by the addition or removal of children,
             * nor format updates.  The default implementation simply returns.
             */
            virtual void elementDataChanged();

            /**
             * Method that is called to indicate that a diagnostic was just reported for the associated element.  Note
             * that this method may be called from multiple threads.
             *
             * \param[in] diagnostic A code indicating the type of error.
             */
            virtual void diagnosticReported(DiagnosticPointer diagnostic);

            /**
             * Method that is called to indicate that any pending diagnostic condition has been cleared.  Note that this
             * method may be called even if there is no reported diagnostic and may be called from multiple threads.
             */
            virtual void diagnosticCleared();

            /**
             * Method that is called to indicate that the element has just ben grafted to a new element tree.
             *
             * The default implementation simply returns.
             */
            virtual void graftedToTree();

            /**
             * Method that is called to indicate that the element is about to be ungrafted from the element tree.  Note
             * that the element being ungrafted from the tree may be different than the element being deleted.
             *
             * The default implementation simply returns.
             */
            virtual void aboutToUngraftFromTree();

            /**
             * Method that is called to indicate that an instruction breakpoint has been set or cleared on this
             * element.
             *
             * \param[in] breakpointNowSet If true, a breakpoint on this element has been set.  If false, a breakpoint
             *                             on this element has been cleared.
             */
            virtual void instructionBreakpointUpdated(bool breakpointNowSet);

            /**
             * Method that is called to indicate that a calculated value associated with the element has been
             * updated.  The default implementation does nothing.
             *
             * \param[in] valueIndex      The zero based value index that was just updated.
             *
             * \param[in] calculatedValue The calculated value.
             */
            virtual void calculatedValueUpdated(unsigned valueIndex, const CalculatedValue& calculatedValue);

            /**
             * Method you can overload to receive notification when the calculated value should be cleared.  The
             * default method simply reports the clearing event to the visual.
             */
            virtual void calculatedValueCleared();

            /**
             * Method you can overload to handle a request for an image of the associated element.
             *
             * \param[in] dpi The image resolution in DPI.
             *
             * \return Returns a byte array containing a binary representation of the requested image.  An empty byte
             *         array is returned if the requested image could not be generated.  The default implementation
             *         returns an empty byte array.
             */
            virtual QByteArray generateImage(float dpi) const;

        private:
            /**
             * Dictionary used to identify visual creators based on an visual type name.
             */
            static QMap<QString, Visual::CreatorFunction> creators;

            /**
             * Pointer to the \ref Element tied to this visual.
             */
            ElementWeakPointer currentElement;
    };
};

#endif
