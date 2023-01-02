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
* This header defines the \ref Ld::HtmlParagraphElementTranslator class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_HTML_PARAGRAPH_ELEMENT_TRANSLATOR_H
#define LD_HTML_PARAGRAPH_ELEMENT_TRANSLATOR_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_html_translator.h"

namespace Ld {
    class HtmlCodeGenerationEngine;
    class XmlExportContext;

    /**
     * HTML translator class for the paragraph element.
     */
    class LD_PUBLIC_API HtmlParagraphElementTranslator:public HtmlTranslator {
        Q_DECLARE_TR_FUNCTIONS(Ld::HtmlParagraphElementTranslator)

        public:
            ~HtmlParagraphElementTranslator() override;

            /**
             * Method that returns the name of the element this translator operates on.
             *
             * \return Returns the name of the element this translator operates on.
             */
            QString elementName() const final;

            /**
             * Method that is called to insert body content.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            bool body(ElementPointer element, HtmlCodeGenerationEngine& generationEngine) final;

        private:
            /**
             * Method that inserts HTML for a traditional paragraph.
             *
             * \param[in] element The element we're processing.
             *
             * \param[in] format  The format tied to the element.
             *
             * \param[in] engine  The code generation engine we're using to generate HTML.
             *
             * \return Returns true on success, returns false on error.
             */
            bool bodyForNormalParagraph(
                ElementPointer            element,
                FormatPointer             format,
                HtmlCodeGenerationEngine& engine
            );

            /**
             * Method that inserts HTML for the start of an ordered list paragraph.
             *
             * \param[in] element The element we're processing.
             *
             * \param[in] format  The format tied to the element.
             *
             * \param[in] engine  The code generation engine we're using to generate HTML.
             *
             * \return Returns true on success, returns false on error.
             */
            bool bodyForTopOfOrderedList(
                ElementPointer            element,
                FormatPointer             format,
                HtmlCodeGenerationEngine& engine
            );

            /**
             * Method that inserts HTML for the start of an unordered list paragraph.
             *
             * \param[in] element The element we're processing.
             *
             * \param[in] format  The format tied to the element.
             *
             * \param[in] engine  The code generation engine we're using to generate HTML.
             *
             * \return Returns true on success, returns false on error.
             */
            bool bodyForTopOfUnorderedList(
                ElementPointer            element,
                FormatPointer             format,
                HtmlCodeGenerationEngine& engine
            );

            /**
             * Method that inserts HTML for a subsequent entry in a list.
             *
             * \param[in] element The element we're processing.
             *
             * \param[in] format  The format tied to the element.
             *
             * \param[in] engine  The code generation engine we're using to generate HTML.
             *
             * \return Returns true on success, returns false on error.
             */
            bool bodyForAdditionalListEntry(
                ElementPointer            element,
                FormatPointer             format,
                HtmlCodeGenerationEngine& engine
            );

            /**
             * Method that identifies the format to be used for this paragraph.
             *
             * \param[in] element  The element to determine the driving format for.
             *
             * \param[in] distance The distance, in counts, between this element and the driving element in visible
             *                     numbers or bullets.
             *
             * \return Returns a pointer to the format to be used for this paragraph.
             */
            static FormatPointer findDrivingFormat(ElementPointer element, unsigned& distance);

            /**
             * Method that determines if this paragraph is the last paragraph in this list.
             *
             * \param[in] element The paragraph element to be checked.
             *
             * \return Returns true if this is the last paragraph in this list.  Returns false if this is not the last
             *         paragraph in the list.
             */
            static bool isLastInList(ElementPointer element);

            /**
             * Method that adds HTML 4.01 paragraph tag and attributes.
             *
             * \param[in] context The context to receive the tags.
             *
             * \param[in] format  The paragraph format.
             */
            static void addParagraphTag(XmlExportContext& context, FormatPointer format);

            /**
             * Method that adds an HTML 4.01 list tag list and attributes.
             *
             * \param[in] context The context to receive the tags.
             *
             * \param[in] format  The driving list format.
             *
             * \param[in] offset  The number offset to assign to this list entry.  The offset is ignored for
             *                    unordered lists.
             */
            static void addListTag(XmlExportContext& context, FormatPointer format, unsigned long offset = 0);
    };
};

#endif
