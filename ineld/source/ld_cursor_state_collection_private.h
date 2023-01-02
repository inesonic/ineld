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
* This header defines the Ld::CursorStateData::Private class and other associated classes.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CURSOR_STATE_COLLECTION_PRIVATE_H
#define LD_CURSOR_STATE_COLLECTION_PRIVATE_H

#include <QSharedPointer>

#include "ld_common.h"
#include "ld_cursor.h"
#include "ld_element_cursor.h"
#include "ld_cursor_state_collection.h"

namespace Ld {
    /**
     * Types of cursor tracking mechanisms.
     */
    enum class CursorTrackingMechanism {
        /**
         * The cursor location is invalid.
         */
        INVALID,

        /**
         * Tracking a cursor location using the entire element.
         */
        WHOLE_ELEMENT,

        /**
         * Tracking a cursor location using a parent and index.
         */
        PARENT_AND_CHILD_INDEX,

        /**
         * Tracking a cursor using an element, text index, and region index.
         */
        TEXT_REGION_INDEX
    };

    /**
     * Pure virtual private implementation base class for the \ref Ld::CursorStateData class.
     */
    class LD_PUBLIC_API CursorStateData::Private {
        public:
            /**
             * Constructor.
             */
            Private();

            ~Private() override;

            /**
             * Method you can call to create a copy of this instance.
             *
             * \return Returns a newly allocated copy of this class instance.
             */
            virtual Private* clone() const = 0;

            /**
             * Method that indicates the tracking mechanism used.
             *
             * \return Returns the tracking mechanism we're using to track the cursor location.
             */
            virtual CursorTrackingMechanism trackingMechanism() const = 0;

            /**
             * Method you can call to obtain a properly adjusted element cursor instance.
             *
             * \return Returns an adjusted element cursor instanace.
             */
            virtual ElementCursor toElementCursor() const = 0;

            /**
             * Method you can call to obtain a properly adjusted element cursor instance.  This version will adjust
             * the element cursor based on newly inserted or removed text.
             *
             * The default implementation simply returns.
             *
             * \param[in] element         The element where the insertion occurred.
             *
             * \param[in] textIndex       The text index prior to the insertion (or removal) operation.
             *
             * \param[in] regionIndex     The region index of the text region where the operation was performed.
             *
             * \param[in] insertionLength The number of newly inserted characters.  A negative value can be used to
             *                            indicate that text was removed.
             */
            virtual void updateAfterTextInsertion(
                QSharedPointer<Element> element,
                unsigned long           textIndex,
                unsigned                regionIndex,
                long                    insertionLength
            );

            /**
             * Method you can call to obtain a properly adjusted element cursor instance.  This version will adjust
             * the element cursor based on a text field split across frames.
             *
             * The default implementation simply returns.
             *
             * \param[in] oldElement  The element where the split was performed.
             *
             * \param[in] newElement  The new element containing text after the split.
             */
            virtual void updateAfterSplit(QSharedPointer<Element> oldElement, QSharedPointer<Element> newElement);

            /**
             * Method you can call to obtain a properly adjusted element cursor instance.  This version will adjust
             * the element cursor based on two merged text fields.
             *
             * The default implementation simply returns.
             *
             * \param[in] leftElement  The element on the left side of the merge that we expect to be preserved.
             *
             * \param[in] rightElement The element on the right side of the merge that is being merged into the left
             *                         element.  We expect this element to be destroyed.
             */
            virtual void updateBeforeMerge(
                QSharedPointer<Element> leftElement,
                QSharedPointer<Element> rightElement
            );

            /**
             * Method you can call to obtain a properly adjusted element cursor instance.  This version will adjust
             * the element cursor based on the assumption that an element is about to be deleted.
             *
             * The default implementation simply returns.
             *
             * \param[in] element               The element that is about to be deleted.
             *
             * \param[in] preferEarlierPosition If true, cursors should prefer an earlier position.  If false, cursors
             *                                  should prefer a later position.
             */
            virtual void updateBeforeDelete(QSharedPointer<Element> element, bool preferEarlierPosition);
    };

    /**
     * Specialization of CursorStateData::Private that tracks a whole element directly.
     */
    class LD_PUBLIC_API CursorStateData::Invalid:public CursorStateData::Private {
        public:
            /**
             * Constructor
             *
             * \param[in] element The element to be tracked.
             */
            Invalid();

            ~Invalid() override;

            /**
             * Method you can call to create a copy of this instance.
             *
             * \return Returns a newly allocated copy of this class instance.
             */
            Private* clone() const override;

            /**
             * Method that indicates the tracking mechanism used.
             *
             * \return Returns the tracking mechanism we're using to track the cursor location.
             */
            CursorTrackingMechanism trackingMechanism() const override;

            /**
             * Method you can call to obtain a properly adjusted element cursor instance.
             *
             * \return Returns an adjusted element cursor instanace.
             */
            ElementCursor toElementCursor() const override;
    };


    /**
     * Specialization of CursorStateData::Private that tracks a whole element directly.
     */
    class LD_PUBLIC_API CursorStateData::WholeElement:public CursorStateData::Private {
        public:
            /**
             * Constructor
             *
             * \param[in] element The element to be tracked.
             */
            WholeElement(QSharedPointer<Element> element);

            ~WholeElement() override;

            /**
             * Method you can call to create a copy of this instance.
             *
             * \return Returns a newly allocated copy of this class instance.
             */
            Private* clone() const override;

            /**
             * Method that indicates the tracking mechanism used.
             *
             * \return Returns the tracking mechanism we're using to track the cursor location.
             */
            CursorTrackingMechanism trackingMechanism() const override;

            /**
             * Method you can use to change the current element.
             *
             * \param[in] newElement A shared pointer to the new element.
             */
            void setElement(QSharedPointer<Element> newElement);

            /**
             * Method you can use to obtain the tracked element.
             *
             * \return Returns a shared pointer to the tracked element.
             */
            QSharedPointer<Element> element() const;

            /**
             * Method you can call to obtain a properly adjusted element cursor instance.
             *
             * \return Returns an adjusted element cursor instanace.
             */
            ElementCursor toElementCursor() const override;

            /**
             * Method you can call to obtain a properly adjusted element cursor instance.  This version will adjust
             * the element cursor based on two merged text fields.
             *
             * \param[in] leftElement  The element on the left side of the merge that we expect to be preserved.
             *
             * \param[in] rightElement The element on the right side of the merge that is being merged into the left
             *                         element.  We expect this element to be destroyed.
             */
            void updateBeforeMerge(QSharedPointer<Element> leftElement, QSharedPointer<Element> rightElement) override;

            /**
             * Method you can call to obtain a properly adjusted element cursor instance.  This version will adjust
             * the element cursor based on the assumption that an element is about to be deleted.
             *
             * \param[in] element               The element that is about to be deleted.
             *
             * \param[in] preferEarlierPosition If true, cursors should prefer an earlier position.  If false, cursors
             *                                  should prefer a later position.
             */
            void updateBeforeDelete(QSharedPointer<Element> element, bool preferEarlierPosition) override;

        private:
            /**
             * The element we want to track.
             */
            QSharedPointer<Element> currentElement;
    };

    /**
     * Specialization of CursorStateData::Private that tracks a whole element by parent and index.
     */
    class LD_PUBLIC_API CursorStateData::ParentIndex:public CursorStateData::Private {
        public:
            /**
             * Constructor
             *
             * \param[in] parent      A shared pointer to the parent we are using to track the location.
             *
             * \param[in] childIndex The child index value relative to the parent.
             */
            ParentIndex(QSharedPointer<Element> parent, unsigned long childIndex);

            ~ParentIndex() override;

            /**
             * Method you can call to create a copy of this instance.
             *
             * \return Returns a newly allocated copy of this class instance.
             */
            Private* clone() const override;

            /**
             * Method that indicates the tracking mechanism used.
             *
             * \return Returns the tracking mechanism we're using to track the cursor location.
             */
            CursorTrackingMechanism trackingMechanism() const override;

            /**
             * Method you can use to change the parent element.
             *
             * \param[in] newParent A shared pointer to the new parent element.
             */
            void setParent(QSharedPointer<Element> newParent);

            /**
             * Method you can use to obtain the parent element.
             *
             * \return Returns a shared pointer to the tracked parent.
             */
            QSharedPointer<Element> parent() const;

            /**
             * Method you can use to set the child index.
             *
             * \param[in] newChildIndex The new child index to be set.
             */
            void setChildIndex(unsigned long newChildIndex);

            /**
             * Method you can use to obtain the child index.
             *
             * \return Returns the child index relative to this parent.
             */
            unsigned long childIndex() const;

            /**
             * Method you can call to obtain a properly adjusted element cursor instance.
             *
             * \return Returns an adjusted element cursor instanace.
             */
            ElementCursor toElementCursor() const override;

            /**
             * Method you can call to obtain a properly adjusted element cursor instance.  This version will adjust
             * the element cursor based on a text field split across frames.
             *
             * \param[in] oldElement  The element where the split was performed.
             *
             * \param[in] newElement  The new element containing text after the split.
             */
            void updateAfterSplit(QSharedPointer<Element> oldElement, QSharedPointer<Element> newElement) override;

            /**
             * Method you can call to obtain a properly adjusted element cursor instance.  This version will adjust
             * the element cursor based on two merged text fields.
             *
             * \param[in] leftElement  The element on the left side of the merge that we expect to be preserved.
             *
             * \param[in] rightElement The element on the right side of the merge that is being merged into the left
             *                         element.  We expect this element to be destroyed.
             */
            void updateBeforeMerge(QSharedPointer<Element> leftElement, QSharedPointer<Element> rightElement) override;

            /**
             * Method you can call to obtain a properly adjusted element cursor instance.  This version will adjust
             * the element cursor based on the assumption that an element is about to be deleted.
             *
             * \param[in] element               The element that is about to be deleted.
             *
             * \param[in] preferEarlierPosition If true, cursors should prefer an earlier position.  If false, cursors
             *                                  should prefer a later position.
             */
            void updateBeforeDelete(QSharedPointer<Element> element, bool preferEarlierPosition) override;

        private:
            /**
             * The parent element.
             */
            QSharedPointer<Element> currentParent;

            /**
             * The child index.
             */
            unsigned long currentChildIndex;
    };

    /**
     * Specialization of CursorStateData::Private that tracks a text position in an element directly.
     */
    class LD_PUBLIC_API CursorStateData::TextRegionIndex:public CursorStateData::WholeElement {
        public:
            /**
             * Constructor
             *
             * \param[in] element     The element to be tracked.
             *
             * \param[in] textIndex   The zero based text index.
             *
             * \param[in] regionIndex The zero based region index.
             */
            TextRegionIndex(QSharedPointer<Element> element, unsigned long textIndex, unsigned regionIndex);

            ~TextRegionIndex() override;

            /**
             * Method you can call to create a copy of this instance.
             *
             * \return Returns a newly allocated copy of this class instance.
             */
            Private* clone() const override;

            /**
             * Method that indicates the tracking mechanism used.
             *
             * \return Returns the tracking mechanism we're using to track the cursor location.
             */
            CursorTrackingMechanism trackingMechanism() const override;

            /**
             * Method you can use to obtain the text index for this position.
             *
             * \return Returns the text index when the cursor state was saved.
             */
            unsigned long textIndex() const;

            /**
             * Method you can use to obtain the region index for this position.
             *
             * \return Returns the region index when the cursor state was saved.
             */
            unsigned regionIndex() const;

            /**
             * Method you can call to obtain a properly adjusted element cursor instance.
             *
             * \return Returns an adjusted element cursor instanace.
             */
            ElementCursor toElementCursor() const override;

            /**
             * Method you can call to obtain a properly adjusted element cursor instance.  This version will adjust
             * the element cursor based on newly inserted or removed text.
             *
             * \param[in] element         The element where the insertion occurred.
             *
             * \param[in] textIndex       The text index prior to the insertion (or removal) operation.
             *
             * \param[in] regionIndex     The region index of the text region where the operation was performed.
             *
             * \param[in] insertionLength The number of newly inserted characters.  A negative value can be used to
             *                            indicate that text was removed.
             */
            void updateAfterTextInsertion(
                QSharedPointer<Element> element,
                unsigned long           textIndex,
                unsigned                regionIndex,
                long                    insertionLength
            ) override;

            /**
             * Method you can call to obtain a properly adjusted element cursor instance.  This version will adjust
             * the element cursor based on a text field split across frames.
             *
             * \param[in] oldElement  The element where the split was performed.
             *
             * \param[in] newElement  The new element containing text after the split.
             */
            void updateAfterSplit(QSharedPointer<Element> oldElement, QSharedPointer<Element> newElement) override;

            /**
             * Method you can call to obtain a properly adjusted element cursor instance.  This version will adjust
             * the element cursor based on two merged text fields.
             *
             * \param[in] leftElement  The element on the left side of the merge that we expect to be preserved.
             *
             * \param[in] rightElement The element on the right side of the merge that is being merged into the left
             *                         element.  We expect this element to be destroyed.
             */
            void updateBeforeMerge(QSharedPointer<Element> leftElement, QSharedPointer<Element> rightElement) override;

            /**
             * Method you can call to obtain a properly adjusted element cursor instance.  This version will adjust
             * the element cursor based on the assumption that an element is about to be deleted.
             *
             * \param[in] element               The element that is about to be deleted.
             *
             * \param[in] preferEarlierPosition If true, cursors should prefer an earlier position.  If false, cursors
             *                                  should prefer a later position.
             */
            void updateBeforeDelete(QSharedPointer<Element> element, bool preferEarlierPosition) override;

        private:
            /**
             * The element we want to track.
             */
            QSharedPointer<Element> currentElement;

            /**
             * The text index into the current element.
             */
            unsigned long currentTextIndex;

            /**
             * The region index into the current element.
             */
            unsigned long currentRegionIndex;
    };
}

#endif
