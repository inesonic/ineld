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
* This header defines the \ref Ld::ParagraphElement class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_PARAGRAPH_ELEMENT_H
#define LD_PARAGRAPH_ELEMENT_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_capabilities.h"
#include "ld_frame_element.h"

namespace Ld {
    class ParagraphFormat;
    class ListAdditionalParagraphFormat;
    class OrderedListParagraphFormat;
    class UnorderedListParagraphFormat;

    /**
     * Class that manages a paragraph containing \ref Ld::TextElement and other element types.  THis class represents
     * the frame containing most top level element types and is generally has the \ref Ld::RootElement as its parent.
     */
    class LD_PUBLIC_API ParagraphElement:public FrameElement {
        public:
            /**
             * The typename for the paragraph element.
             */
            static const QString elementName;

            /**
             * Static value indicating the capabilities provides by this element.
             */
            static const Capabilities childProvides;

            ParagraphElement();

            ~ParagraphElement() override;

            /**
             * Factory method that can be used to create a default instance of a paragraph element.
             *
             * \param[in] typeName The name used to reference the creator function.
             *
             * \return Returns a newly created, default, paragraph element instance.
             */
            static Element* creator(const QString& typeName);

            /**
             * Method that returns the type name associated with this object.
             *
             * \return Returns the typename associated with this object.  The value must match the type used to invoke
             *         the static \ref Element::create and \ref Element::registerCreator methods.
             */
            QString typeName() const final;

            /**
             * Method that returns the name of the plug-in that provides this element.
             *
             * \return Returns the name of the plug-in that provides this element. The name must match the name provided
             *         in the plug-in structures.  Elements defined as part of the ineld library
             *         should return an empty string.
             */
            QString plugInName() const final;

            /**
             * Method that returns a description of this element.
             *
             * \return Returns a description of this element suitable for use in a GUI.
             */
            QString description() const final;

            /**
             * Method you can call to determine the requirements imposed by this element on a specific child element.
             * Note that no testing is performed by elements to confirm that requirements are met.  The information is
             * expected to be used by the user interface.
             *
             * \param[in] childIndex The zero based child index.
             *
             * \return Returns the requirements imposed by this parent on the child element.
             */
            Capabilities parentRequires(unsigned long childIndex) const final;

            /**
             * Method you can call to determine what capabilities this element will provide.  Note that this object
             * also provides a static constant value named "childProvides" returning the same information.
             *
             * \return Returns the requirements met by this element.
             */
            Capabilities childProvidesCapabilities() const final;

            /**
             * Method you can use to tie a format to this element used when rendering the element in the visual.
             *
             * \param[in] newFormat A shared pointer to the format to tie to this element.
             */
            void setFormat(QSharedPointer<ParagraphFormat> newFormat);

            /**
             * Method you can use to tie a format to this element used when rendering the element in the visual.
             *
             * \param[in] newFormat A shared pointer to the format to tie to this element.
             */
            void setFormat(QSharedPointer<ListAdditionalParagraphFormat> newFormat);

            /**
             * Method you can use to tie a format to this element used when rendering the element in the visual.
             *
             * \param[in] newFormat A shared pointer to the format to tie to this element.
             */
            void setFormat(QSharedPointer<OrderedListParagraphFormat> newFormat);

            /**
             * Method you can use to tie a format to this element used when rendering the element in the visual.
             *
             * \param[in] newFormat A shared pointer to the format to tie to this element.
             */
            void setFormat(QSharedPointer<UnorderedListParagraphFormat> newFormat);

            /**
             * Convenience method you can use to tie a format to this element used when rendering the element in the
             * visual.
             *
             * \param[in] newFormat The format to tie to this element.  Note that the element will assign the format to
             *                      a shared pointer, thus taking shared ownership of it.
             */
            void setFormat(ParagraphFormat* newFormat);

            /**
             * Method you can use to obtain the current format tied to this element.
             *
             * \return Returns a shared pointer to the desired format.
             */
            QSharedPointer<ParagraphFormat> format() const;

            /**
             * Method that indicates if the \ref Ld::Cursor can place the cursor on this element.
             *
             * \return Returns the allowed cursor park mode.
             */
            CursorParkMode cursorParkMode() const override;

            /**
             * Method you can use to determine if this element supports image export capability.
             *
             * \return Returns the export image capability for this element.  This implementatin returns
             *         \ref Ld::Element::ExportImageCapability::NONE.
             */
            ExportImageCapability exportImageCapability() const override;

            /**
             * Method you can call to detemrine if this element provides a logical stop for function parenthesis.  If
             * true, functions will act as if they're top level.  If false, functions will act as if they're within a
             * larger equation and will always place parenthesis around parameters.
             *
             * \param[in] functionChildIndex The index of the function element being queried.
             *
             * \return Returns true if the function can assume it's at a logical stop in the equation such as the last
             *         position in an equation or at a position, such as directly under a matrix, that can be treated
             *         as the last position.  Returns false in all other cases.  This version always returns true.
             */
            bool isFunctionLogicalStop(unsigned long functionChildIndex) const override;

            /**
             * Method that determines if this paragraph contains any "normal" text.  Normal text is defined as text
             * within a \ref Ld::TextElement instance.
             *
             * \return Returns true if the paragraph contains normal text.  Returns false if the paragraph does not
             *         contain any normal text.
             */
            bool containsNormalText() const;
    };
};

#endif
