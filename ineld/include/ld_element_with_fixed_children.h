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
* This header defines the \ref Ld::ElementWithFixedChildren class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_ELEMENT_WITH_FIXED_CHILDREN_H
#define LD_ELEMENT_WITH_FIXED_CHILDREN_H

#include <QList>
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_xml_writer.h"
#include "ld_xml_attributes.h"
#include "ld_element.h"

namespace Ld {
    class VisualWithFixedChildren;

    /**
     * Pure virtual base class for elements that support a fixed and pre-defined number of children.
     */
    class LD_PUBLIC_API ElementWithFixedChildren:public Element {
        public:
            ElementWithFixedChildren();

            ~ElementWithFixedChildren() override;

            /**
             * Method you can use to create a clone of this element.  This version will create a clone by:
             *
             *     * Using the typeName of the element to create a version using using the element.
             *
             *     * Use the \ref Ld::Format::clone method to create a clone of the format.
             *
             *     * Using the \ref Ld::Element::setNumberTextRegions and \ref Ld::Element::setText to populate any
             *       text regions for the element.
             *
             *     * If enabled, the method then recursively clones each child element.
             *
             * \param[in] includeChildren If true, clones of every child will be included in generated clone.  If
             *                            false, children will be excluded from the clone.
             *
             * \return Returns a new instance of this element with clones of each child included.  Note that a null
             *         pointer may be returned in the unlikely event of an error.
             */
            ElementPointer clone(bool includeChildren) const override;

            /**
             * Method you can use to create a clone of this element based on a starting and ending cursor position.
             * This version is intended to be used to clone elements containing text in one or more regions and will
             * create a clone by:
             *
             *     * Using the typeName of the element to create a version using using the element.
             *
             *     * Use the \ref Ld::Format::clone method to create a clone of the format.
             *
             *     * Using the \ref Ld::Element::setNumberTextRegions and \ref Ld::Element::setText to populate any
             *       text regions for the element.  Text regions and contents will be limited by the provided index
             *       values.  All other text will either be excluded or set to an empty value.
             *
             *     * The method then recursively clones each child element.
             *
             * \param[in] startingTextIndex   The starting text index of the first specified region to include in the
             *                                clone.
             *
             * \param[in] startingRegionIndex The startng text region to include in the clone.
             *
             * \param[in] endingTextIndex     The ending text index of the last specified region to include in the
             *                                clone.  A value of \ref Ld::Element::invalidTextIndex indicates all the
             *                                text in the ending region should be included.
             *
             * \param[in] endingRegionIndex   The ending text region to include in the clone.  A value of
             *                                \ref Ld::Element::invalidRegionIndex indicates the close should include
             *                                the last region.
             *
             * \param[in] includeChildren     If true, clones of every child will be included in generated clone.  If
             *                                false, children will be excluded from the clone.
             *
             * \return Returns a new instance of the element.  The clone may optionally include children if this
             *         element supports children.  A null pointer will be returned if the specified ranges are invalid.
             */
            ElementPointer clone(
                unsigned long startingTextIndex,
                unsigned      startingRegionIndex,
                unsigned long endingTextIndex,
                unsigned      endingRegionIndex,
                bool          includeChildren
            ) const override;

            /**
             * Method that can be called to determine how children are managed and tracked by this element.
             *
             * \return Returns a value indicating how children are tracked by this element.  This version always returns
             *         \ref Ld::Element::ChildPlacement::FIXED.
             */
            Element::ChildPlacement childPlacement() const override;

            /**
             * Method you can use to change the object used to manage visual representation of this element.  Note that
             * using this method will also cause the \ref Visual::element method to point back to this \ref Element
             * instance.
             *
             * If the newly added \ref Visual already points to a different element, then those connections will be
             * broken in order to maintain a 1:1 relationship between any \ref Visual instance and \ref Element
             * instance.
             *
             * If this element currently has an associated visual, then that visual will disconnected from this
             * element and will be deleted.
             *
             * \param[in] newVisual A pointer to the new visual object.
             */
            void setVisual(VisualWithFixedChildren* newVisual);

            /**
             * Method you can use to obtain the visual representation of this element.
             *
             * \return Returns a pointer to the visual representation of this element.  A null pointer is returned if
             *         no visual representation has been defined.
             */
            VisualWithFixedChildren* visual() const;

            /**
             * Method that can be used to determine the current number of children.
             *
             * \return Returns a count of the number of children.
             */
            unsigned long numberChildren() const override;

            /**
             * Method that is called to remove a child from this class by index.
             *
             * \param[in]     childIndex            The zero based index to remove.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             *
             * \return Returns true on success.  Returns false if index is invalid or no child exists at the specified
             *         index.
             */
            bool removeChild(unsigned long childIndex, CursorStateCollection* cursorStateCollection);

            /**
             * Method that is called to remove a child from this class.  This version will maintain the positionl of all
             * other children, setting the child at the specified position to a null pointer.
             *
             * \param[in]     childToRemove         The child to be removed.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             *
             * \return Returns true on success.  Returns false if the child does not belong to this element.
             */
            bool removeChild(ElementPointer childToRemove, CursorStateCollection* cursorStateCollection) override;

            /**
             * Method that can be called to remove every child element.  The children are destroyed by this method.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             */
            void removeChildren(CursorStateCollection* cursorStateCollection) override;

            /**
             * Method that can be used to change the child at a given position.  If a child is already defined for that
             * position, the child will be destroyed.
             *
             * \param[in]     childIndex            The zero based index where the desired child should be placed.
             *
             * \param[in]     childElement          The child element to be placed.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             *
             * \return Returns true on success, returns false if the index is invalid.
             */
            bool setChild(
                unsigned long          childIndex,
                ElementPointer         childElement,
                CursorStateCollection* cursorStateCollection
            );

            /**
             * Method that can be used to relocate a child within this element.  If needed, the number of children will
             * be resized to fit.
             *
             * \param[in]     oldChildIndex         The old child index.
             *
             * \param[in]     newChildIndex         The new child index.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             *
             * \return Returns a shared pointer to the displaced child.
             */
            ElementPointer moveChild(
                unsigned               oldChildIndex,
                unsigned               newChildIndex,
                CursorStateCollection* cursorStateCollection
            );

            /**
             * Method that can be used to obtain a child using its index.  Note that the index for each child may change
             * dynamically as children are added and removed.
             *
             * \param[in] childIndex The zero based index of the desired child.
             *
             * \return Returns a pointer to the desired child.
             */
            ElementPointer child(unsigned long childIndex) const override;

            /**
             * Method that can be used to obtain a list of child elements.
             *
             * \return Returns a list of child elements.  The list may contain null pointers.
             */
            ElementPointerList children() const override;

            /**
             * Method you can call to determine the index to use to reference a child.  The value will match the value
             * to be used with the Element::child(unsigned long) method to access the child.
             *
             * \param[in] childElement A shared pointer to the child element that we want to locate.
             *
             * \return Returns the zero based index of the child.  The value \ref Ld::Element::invalidChildIndex is
             *         returned if the provided element is not a child of this element.
             */
            unsigned long indexOfChild(ElementPointer childElement) const final;

        protected:
            /**
             * Method you can call from derived classes to set the total number of children.
             *
             * \param[in]     newNumberChildren     A count of the total expected number of children.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             */
            void setNumberChildren(unsigned newNumberChildren, CursorStateCollection* cursorStateCollection);

            /**
             * Method that is triggered when this element or a descendant has been inserted into a tree of elements.
             * You can overload this method if you want notification that this element is not under a different tree.
             * Please be certain to call the base class implementation should you overload this method as the base class
             * implementation manages event propagation to child elements.
             */
            void graftedToTree() override;

            /**
             * Method that is triggered when this element or an ancestor is about to be removed from a tree of elements.
             * You can overload this method if you want notification that the element has been disconnected from the
             * previous tree structure.  If you do overload this method, be sure to call the base class implementation
             * as it manages event propagation to child elements.
             *
             * Note that this virtual method will *not* be triggered when this element or an ascestor is being
             * destroyed.
             */
            void aboutToUngraftFromTree() override;

            /**
             * Method that writes child elements to the program file.
             *
             * \param[in]     writer              The writer used to write the XML description of the program.
             *
             * \param[in]     formats             A \ref FormatOrganizer instance used to track all known formats in
             *                                    the program.
             *
             * \param[in,out] programFile         The program file instance that can be used to store (or retrieve)
             *                                    additional side-band information.
             *
             * \param[in]     inheritedAttributes Attributes provided by the calling class to this class.  Provided
             *                                    attributes should be provided with child's XML element.  This
             *                                    parameter exists to allow the parent to include positional
             *                                    information as attributes of each child.
             */
            void writeChildren(
                QSharedPointer<XmlWriter>       writer,
                QSharedPointer<FormatOrganizer> formats,
                ProgramFile&                    programFile,
                const XmlAttributes&            inheritedAttributes
            ) const final;

            /**
             * Method that reads and processes the XML description of a child, inserting the child into the element
             * tree.
             *
             * Errors are reported directly to the reader using the raiseError method.
             *
             * \param[in]     reader      The reader used to parse the incoming XML stream.
             *
             * \param[in]     tagName     The name of the spotted XML tag.
             *
             * \param[in]     formats     A map of format instances by format identifier.  You can use this parameter
             *                            to either tie a format to an object or add a new format to the list of known
             *                            formats.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             *
             * \param[in]     attributes  Attributes tied to the XML start tag.
             *
             * \param[in]     xmlVersion  The XML major version code.  The value can be used to modify how the incoming
             *                            XML stream is parsed.
             */
            void readChild(
                QSharedPointer<XmlReader>  reader,
                const QString&             tagName,
                const FormatsByIdentifier& formats,
                ProgramFile&               programFile,
                const XmlAttributes&       attributes,
                unsigned                   xmlVersion
            ) final;

            /**
             * Method used by the clone methods to clone the child elements.
             *
             * \param[in] element Shared pointer to the element to receive the clones.
             *
             * \return Returns true on success, returns false on failure.
             */
            virtual bool cloneChildren(QSharedPointer<ElementWithFixedChildren> element) const;

        private:
            /**
             * List of child elements.
             */
            ElementPointerList currentChildren;
    };
};

#endif
