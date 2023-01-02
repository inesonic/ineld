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
* This header defines the \ref Ld::PayloadElement class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_PAYLOAD_ELEMENT_H
#define LD_PAYLOAD_ELEMENT_H

#include <QSharedPointer>
#include <QByteArray>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_payload_data.h"
#include "ld_xml_writer.h"
#include "ld_xml_attributes.h"
#include "ld_element_with_no_children.h"

namespace Ld {
    class PayloadVisual;

    /**
     * Pure virtual base class for child-less elements that track external payloads.
     *
     * Note that this class defines the XML attribute "payload_id" which is used to tie this class instance to a
     * specific payload.
     */
    class LD_PUBLIC_API PayloadElement:public ElementWithNoChildren {
        public:
            PayloadElement();

            ~PayloadElement() override;

            /**
             * Type used to report the storage mode for this payload elmeent.
             */
            typedef PayloadData::StorageMode StorageMode;

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
             *     * Using the tracked payload to clone the payload.
             *
             * \param[in] includeChildren If true, clones of every child will be included in generated clone.  If
             *                            false, children will be excluded from the clone.  This parameter is included
             *                            for compatibility and is always ignored.
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
             *     * Using the tracked payload to clone the payload.
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
                unsigned long        startingTextIndex,
                unsigned             startingRegionIndex,
                unsigned long        endingTextIndex,
                unsigned             endingRegionIndex,
                bool                 includeChildren
            ) const override;

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
            void setVisual(PayloadVisual* newVisual);

            /**
             * Method you can use to obtain the visual representation of this element.
             *
             * \return Returns a pointer to the visual representation of this element.  A null pointer is returned if
             *         no visual representation has been defined.
             */
            PayloadVisual* visual() const;

            /**
             * Method you can use to determine the payload storage mode currently being used by this element.
             *
             * \return Returns the current payload storage mode.
             */
            StorageMode storageMode() const;

            /**
             * Method you can use to determine if this element supports payloads.
             *
             * \return Returns true if this element supports payloads.  Returns false if this element does not support
             *         payloads.  The default implementation always returns false.
             */
            bool supportsPayloads() const override;

            /**
             * Method you can use to update the payload data tracked by this element.  Note that this method triggers
             * file I/O.
             *
             * \param[in] newData The new data to be tracked for this element.
             *
             * \return Returns true on success, returns false on error.
             */
            bool updatePayload(const QByteArray& newData);

            /**
             * Method you can use to obtain the payload data tracked by this element.  Note that this method triggers
             * file I/O.
             *
             * \param[out] newData A QByteArray that will be populated with the payload data.
             *
             * \return Returns true on success, returns false on error.
             */
            bool getPayload(QByteArray& newData) override;

        protected:
            /**
             * Method that is triggered when this element or a descendant has been inserted into a tree of elements.
             * You can overload this method if you want notification that this element is not under a different tree.
             * Please be certain to call the base class implementation should you overload this method as the base class
             * implementation manages event propagation to child elements as well as payload tracking.
             */
            void graftedToTree() override;

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

        private:
            /**
             * Method that reports to the visual whenever the payload changes.
             */
            void payloadChanged();

            /**
             * Method that reports to the visual whenver a payload could not be loaded.
             */
            void payloadCouldNotBeLoaded();

            /**
             * The tracked payload.
             */
            PayloadData currentPayload;
    };
};

#endif
