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
* This header defines the \ref Ld::ElementWithGroupedChildren class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_ELEMENT_WITH_GROUPED_CHILDREN_H
#define LD_ELEMENT_WITH_GROUPED_CHILDREN_H

#include <QList>
#include <QSharedPointer>
#include <QList>
#include <QMap>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_format_organizer.h"
#include "ld_xml_writer.h"
#include "ld_xml_attributes.h"
#include "ld_element_with_positional_children.h"

namespace Ld {
    class VisualWithGroupedChildren;

    /**
     * Base class for elements that support a variable number of children that are at well defined relative positions.
     * The class will have a single group after instantiation.
     */
    class LD_PUBLIC_API ElementWithGroupedChildren:public ElementWithPositionalChildren {
        public:
            /**
             * Value you can use to indicate an invalid child group.
             */
            static const unsigned invalidGroup;

            ElementWithGroupedChildren();

            ~ElementWithGroupedChildren() override;

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
             *     * Create empty groups, matching the number of groups in this element.
             *
             *     * If enabled, the method then recursively clones each child element, placing the clone into the
             *       correct group.
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
             *     * Create empty groups, matching the number of groups in this element.
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
             *         \ref Ld::Element::ChildPlacement::GROUPED.
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
            void setVisual(VisualWithGroupedChildren* newVisual);

            /**
             * Method you can use to obtain the visual representation of this element.
             *
             * \return Returns a pointer to the visual representation of this element.  A null pointer is returned if
             *         no visual representation has been defined.
             */
            VisualWithGroupedChildren* visual() const;

            /**
             * Method that is called to remove a child from this class by index.  Calling this method will reduce the
             * number of children tracked by this class by 1.
             *
             * \param[in]     childIndex            The zero based index to remove.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             *
             * \return Returns true on success.  Returns false if index is invalid.
             */
            bool removeChild(unsigned long childIndex, CursorStateCollection* cursorStateCollection) override;

            /**
             * Method that can be called to remove every child element.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             */
            void removeChildren(CursorStateCollection* cursorStateCollection) override;

            /**
             * Method that can be called to insert a new child before the specified index.
             *
             * \param[in]     childIndex            The index of the child to follow the newly inserted child.  After
             *                                      calling this method, the newly inserted child will exist at this
             *                                      position and all children that were at this position will be moved
             *                                      one position down.  An invalid index will cause the child to be
             *                                      inserted at the end.
             *
             * \param[in]     childElement          The child to be inserted.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             */
            void insertBefore(
                unsigned long          childIndex,
                ElementPointer         childElement,
                CursorStateCollection* cursorStateCollection
            ) override;

            /**
             * Method that can be called to insert a new child after the specified index.
             *
             * \param[in]     childIndex            The index of the child to follow the newly inserted child.  After
             *                                      calling this method, the newly inserted child will exist after the
             *                                      specified location and all other children after the specified index
             *                                      will be moved down by one position.  An invalid index will cause
             *                                      the child to be inserted at the end.
             *
             * \param[in]     childElement          The child to be inserted.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             */
            void insertAfter(
                unsigned long          childIndex,
                ElementPointer         childElement,
                CursorStateCollection* cursorStateCollection
            ) override;

            /**
             * Method you can use to determine the current number of child groups.
             *
             * \return Returns the current number of child groups.
             */
            unsigned numberGroups() const;

            /**
             * Method you can use to determien the number of children in any given group.
             *
             * \param[in] groupIndex The zero based group index of the group to query.
             *
             * \return Returns the number of children in the group. A value of invalidChildIndex will be returned if
             *         the group index is invalid.
             */
            unsigned long numberChildrenInGroup(unsigned groupIndex) const;

            /**
             * Method you can use to obtain a child within a group.
             *
             * \param[in] groupIndex        The zero based group index of the group to query.
             *
             * \param[in] childIndexInGroup The index of the child relative to the start of the group.
             *
             * \return Returns a shared pointer to the requested element.  A null pointer is returned if the group
             *         index or child index in the group is invalid.
             */
            ElementPointer childInGroup(unsigned groupIndex, unsigned long childIndexInGroup) const;

            /**
             * Method you can use to obtain all the children in a group.
             *
             * \param[in] groupIndex The zero based group index of the group to query.
             *
             * \return Returns a list of shared pointers to all the children in the group.  An empty list is returned if
             *         the group index is invalid.
             */
            ElementPointerList childrenInGroup(unsigned groupIndex) const;

            /**
             * Method you can use to obtain the child index of a given child based on group index and child index in
             * the group.
             *
             * \param[in] groupIndex        The zero based group index of the group to query.
             *
             * \param[in] childIndexInGroup The index of the child relative to the start of the group.
             *
             * \return Returns the child index of the indicated child.  An invalid child index is returned if the
             *         requested child does not exist.
             */
            unsigned long childIndexFromGroup(unsigned groupIndex, unsigned long childIndexInGroup) const;

            /**
             * Method you can use to obtain the group index and, optionally, the child index in the group based on a
             * child index.
             *
             * \param[in]     childIndex        The child index to obtain group information for.
             *
             * \param[in,out] childIndexInGroup A pointer to an unsigned long value that can optionally be populated
             *                                  with the child index in the group.  The parameter is ignored if a null
             *                                  pointer is provided.  An invalid child index is returned if the
             *                                  provided child index is invalid.
             *
             * \return Returns the zero based group index of the group containing the child.
             */
            unsigned groupContainingChild(unsigned long childIndex, unsigned long* childIndexInGroup = nullptr) const;

            /**
             * Method you can use to remove a child from a group.
             *
             * \param[in] groupIndex                The zero based group index of the group to query.
             *
             * \param[in] childIndexInGroup         The index of the child relative to the start of the group.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             *
             * \return Returns true on success, returns false if either index is invalid.
             */
            bool removeChildFromGroup(
                unsigned               groupIndex,
                unsigned long          childIndexInGroup,
                CursorStateCollection* cursorStateCollection
            );

            /**
             * Method you can use to remove all the children from a group.
             *
             * \param[in]     groupIndex            The zero based group index of the group to query.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             *
             * \return Returns true if the group index is valid.  Returns false if the group index is invalid.
             */
            bool removeChildrenFromGroup(unsigned groupIndex, CursorStateCollection* cursorStateCollection);

            /**
             * Method that can be called to insert a new child before the specified index within a group.
             *
             * \param[in]     groupIndex            The zero based index of the group where the child should be added.
             *                                      An invalid group will cause the child to be added to the last
             *                                      group.
             *
             * \param[in]     childIndexInGroup     The index of the child in the group to follow the newly inserted
             *                                      child.  After calling this method, the newly inserted child will
             *                                      exist at this position and all children that were at this position
             *                                      will be moved one position down.  An invalid index will cause the
             *                                      child to be at the end of the group.
             *
             * \param[in]     childElement          The child to be inserted.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             */
            virtual void insertIntoGroupBefore(
                unsigned               groupIndex,
                unsigned long          childIndexInGroup,
                ElementPointer         childElement,
                CursorStateCollection* cursorStateCollection
            );

            /**
             * Method that can be called to insert a new child after the specified index within a group.
             *
             * \param[in]     groupIndex            The zero based index of the group where the child should be added.
             *                                      An invalid group will cause the child to be added to the last
             *                                      group.
             *
             * \param[in]     childIndexInGroup     The index of the child in the group to precede the newly inserted
             *                                      child.  After calling this method, the newly inserted child will
             *                                      exist just after the specified location and all other children
             *                                      after the specified index will be moved down by one position.  An
             *                                      invalid index will cause the child to be inserted at the end of
             *                                      the group.
             *
             * \param[in]     childElement          The child to be inserted.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             */
            virtual void insertIntoGroupAfter(
                unsigned               groupIndex,
                unsigned long          childIndexInGroup,
                ElementPointer         childElement,
                CursorStateCollection* cursorStateCollection
            );

            /**
             * Convenience method that can be called to prepend a child.
             *
             * \param[in]     groupIndex            The zero based index of the group where the child should be added.
             *                                      An invalid group will cause the child to be added to the last
             *                                      group.
             *
             * \param[in]     childElement          The child to be inserted.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             */
            void prependToGroup(
                unsigned               groupIndex,
                ElementPointer         childElement,
                CursorStateCollection* cursorStateCollection
            );

            /**
             * Convenience method that can be called to append a child.
             *
             * \param[in]     groupIndex            The zero based index of the group where the child should be added.
             *                                      An invalid group will cause the child to be added to the last
             *                                      group.
             *
             * \param[in]     childElement          The child to be inserted.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             */
            void appendToGroup(
                unsigned               groupIndex,
                ElementPointer         childElement,
                CursorStateCollection* cursorStateCollection
            );

        protected:
            /**
             * Method you can use to remove a group.  Children in the group will also be removed.
             *
             * \param[in]     startingGroupIndex    The zero based index of the group to be removed.
             *
             * \param[in]     numberGroups          The number of groups to remove.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             */
            void removeGroups(
                unsigned               startingGroupIndex,
                unsigned               numberGroups,
                CursorStateCollection* cursorStateCollection
            );

            /**
             * Method you can use to insert a new group.
             *
             * \param[in] groupIndex   The zero based index of the group to follow this group.  The newly inserted
             *                         groups will take on this group index.  An invalid group index will cause the new
             *                         groups to be appended to the end of the groups.
             *
             * \param[in] numberGroups The number of groups to be added.
             */
            void insertGroupsBefore(unsigned groupIndex, unsigned numberGroups);

            /**
             * Method you can use to insert a new group.
             *
             * \param[in] groupIndex   The zero based index of the group to precede this group.  The newly inserted
             *                         groups will appear immediately after this group.  An invalid group index will
             *                         cause the new groups to be appended to the end of the groups.
             *
             * \param[in] numberGroups The number of groups to be added.
             */
            void insertGroupsAfter(unsigned groupIndex, unsigned numberGroups);

            /**
             * Method you can optionally overload to add additional attributes to the XML description of this element.
             *
             * \param[in]     attributes  A reference to the list of attributes to be modified.
             *
             * \param[in]     formats     A \ref FormatOrganizer instance used to track all known formats in the
             *                            program.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             */
            void writeAddAttributes(
                XmlAttributes&                  attributes,
                QSharedPointer<FormatOrganizer> formats,
                ProgramFile&                    programFile
            ) const override;

            /**
             * Method you can overload to parse incoming XML attributes.  This method will only be called if there are
             * provided attributes.
             *
             * Errors are reported directly to the reader using the raiseError method.
             *
             * \param[in]     reader      The reader used to parse the incoming XML stream.
             *
             * \param[in]     attributes  The XML attributes to be parsed.
             *
             * \param[in]     formats     A map of format instances by format identifier.  You can use this parameter
             *                            to either tie a format to an object or add a new format to the list of known
             *                            formats.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             *
             * \param[in]     xmlVersion  The XML major version code.  The value can be used to modify how the incoming
             *                            XML stream is parsed.
             */
            void readAttributes(
                QSharedPointer<XmlReader>  reader,
                const XmlAttributes&       attributes,
                const FormatsByIdentifier& formats,
                ProgramFile&               programFile,
                unsigned                   xmlVersion
            ) override;

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
            ) const override;

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
            ) override;

            /**
             * Method used by the clone methods to clone the child elements.
             *
             * \param[in] element Shared pointer to the element to receive the clones.
             *
             * \return Returns true on success, returns false on failure.
             */
            bool cloneChildren(QSharedPointer<ElementWithPositionalChildren> element) const override;

        private:
            /**
             * Type used to track child indexes by group.
             */
            typedef QList<unsigned long> FirstChildList;

            /**
             * Method that obtains the base child index for a given group.  An invalid group number will return the
             * total number of children.
             *
             * \param[in] groupIndex The zero based group index to query.
             *
             * \return Returns the base child index for this group.
             */
            unsigned long baseChildIndex(unsigned groupIndex) const;

            /**
             * Method that is called to adjust the first child in each group after a child has been inserted.
             *
             * \param[in] groupIndex  The group index of the group to receive the child.
             *
             * \param[in] numberAdded The number of children added to the group.
             */
            void childrenAdded(unsigned groupIndex, unsigned long numberAdded = 1);

            /**
             * List tracking the first child index in each group.  An empty group will have the same number of
             * children as the group that follows it.  The last group will have the same number of children as the
             * total number of children managed by this instance.
             */
            FirstChildList firstChildIndexInGroup;
    };
};

#endif
