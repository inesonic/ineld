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
* This header defines the \ref Ld::ElementWithFloatingChildren class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_ELEMENT_WITH_FLOATING_CHILDREN_H
#define LD_ELEMENT_WITH_FLOATING_CHILDREN_H

#include <QList>
#include <QSharedPointer>
#include <QMap>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_xml_writer.h"
#include "ld_xml_attributes.h"
#include "ld_location.h"
#include "ld_element.h"

namespace Ld {
    class VisualWithFloatingChildren;

    /**
     * Base class for elements that support a variable number of children that can "float" in an area relative to each
     * other.  Elements are logically ordered in the program based on their relative positions in reading order.
     */
    class LD_PUBLIC_API ElementWithFloatingChildren:public Element {
        public:
            ElementWithFloatingChildren();

            ~ElementWithFloatingChildren() override;

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
             *         \ref Ld::Element::ChildPlacement::FLOATING.
             */
            Element::ChildPlacement childPlacement() const final;

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
            void setVisual(VisualWithFloatingChildren* newVisual);

            /**
             * Method you can use to obtain the visual representation of this element.
             *
             * \return Returns a pointer to the visual representation of this element.  A null pointer is returned if
             *         no visual representation has been defined.
             */
            VisualWithFloatingChildren* visual() const;

            /**
             * Method that can be used to determine the current number of children.
             *
             * \return Returns a count of the number of children.
             */
            unsigned long numberChildren() const final;

            /**
             * Method that is called to remove a child from this class.  This version of this method will remove the
             * child and re-order the other children accordingly.
             *
             * \param[in]     childToRemove         The child to be removed.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             *
             * \return Returns true on success.  Returns false if the child does not belong to this element.
             */
            bool removeChild(ElementPointer childToRemove, CursorStateCollection* cursorStateCollection) final;

            /**
             * Method that can be called to remove every child element.  The children are destroyed by this method.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             */
            void removeChildren(CursorStateCollection* cursorStateCollection) final;

            /**
             * Method that can be called to insert a child at a given location.
             *
             * \param[in]     childElement          The child to be inserted.
             *
             * \param[in]     location              The location where the child element is being placed.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             */
            void insertChild(
                ElementPointer         childElement,
                const Location&        location,
                CursorStateCollection* cursorStateCollection
            );

            /**
             * Convenience method that can be called to insert a child at a given location.
             *
             * \param[in]     childElement          The child to be inserted.
             *
             * \param[in]     x                     The child X offset.
             *
             * \param[in]     y                     The child Y offset.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             */
            void insertChild(
                ElementPointer         childElement,
                double                 x,
                double                 y,
                CursorStateCollection* cursorStateCollection
            );

            /**
             * Method that can be called to indicate that a child's location is changed.
             *
             * \param[in] childElement              The child to be relocated.
             *
             * \param[in] location                  The new location for the child.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             *
             * \return Returns true on success.  Returns false if the child does not belong to this element.
             */
            bool relocateChild(
                ElementPointer         childElement,
                const Location&        location,
                CursorStateCollection* cursorStateCollection
            );

            /**
             * Convenience method that can be called to indicate that a child's location is changed.
             *
             * \param[in]     childElement          The child to be relocated.
             *
             * \param[in]     x                     The child X offset.
             *
             * \param[in]     y                     The child Y offset.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             *
             * \return Returns true on success.  Returns false if the child does not belong to this element.
             */
            bool relocateChild(
                ElementPointer         childElement,
                double                 x,
                double                 y,
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
            ElementPointer child(unsigned long childIndex) const final;

            /**
             * Method that can be used to obtain the location of a child.
             *
             * \param[in] childElement Pointer to the child to obtain the location of.
             *
             * \return Returns the location of the child. A null location is returned if the child does not exist under
             *         this parent.
             */
            Location childLocation(ElementPointer childElement) const;

            /**
             * Convenience method that can be used to obtain the location of a child.
             *
             * \param[in] childIndex The zero based index of the child to obtain the location of.
             *
             * \return Returns the location of the child. A null location is returned if the index is invalid.
             */
            Location childLocation(unsigned long childIndex) const;

            /**
             * Method that can be used to obtain a list of child elements.
             *
             * \return Returns a list of child elements.  The list may contain null pointers.
             */
            ElementPointerList children() const final;

            /**
             * Method that can be used to obtain a complete mapping of child elements by location.  This version is
             * slower as it creates a new map; however, the returned map can be freely modified and destroyed.
             *
             * \return Returns a map of \ref Ld::Element pointers indexed by \ref Ld::Location.
             */
            QMap<Location, ElementPointer> childrenByLocation() const;

            /**
             * Method you can call to determine the index to use to reference a child.  The value will match the value
             * to be used with the child(unsigned long) method to access the child.
             *
             * \param[in] childElement A shared pointer to the child element that we want to locate.
             *
             * \return Returns the zero based index of the child.  The value \ref Ld::Element::invalidChildIndex is
             *         returned if the provided element is not a child of this element.
             */
            unsigned long indexOfChild(ElementPointer childElement) const final;

        protected:
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

        private:
            /**
             * Method used by the clone methods to clone the child elements.
             *
             * \param[in] element Shared pointer to the element to receive the clones.
             *
             * \return Returns true on success, returns false on failure.
             */
            bool cloneChildren(QSharedPointer<ElementWithFloatingChildren> element) const;

            /**
             * Mapping of child elements by location.
             */
            QMap<Location, ElementPointer> currentChildrenByLocation;

            /**
             * Mapping of locations for each child.
             */
            QMap<ElementPointer, Location> currentLocationsByChild;
    };
};

#endif
