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
* This header defines the \ref Ld::HtmlTranslator class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_HTML_TRANSLATOR_H
#define LD_HTML_TRANSLATOR_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QByteArray>
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_format.h"
#include "ld_translator.h"
#include "ld_html_translator_helper.h"

namespace Ld {
    class HtmlCodeGenerationEngine;
    class XmlExportContext;

    /**
     * Virtual base class for per-element translators that convert the element in the syntax tree to an HTML
     * representation.
     */
    class LD_PUBLIC_API HtmlTranslator:public Translator, public HtmlTranslatorHelper {
        Q_DECLARE_TR_FUNCTIONS(Ld::HtmlTranslator)

        public:
            ~HtmlTranslator() override;

            /**
             * Method that calls the correct translation method for the required translation.
             *
             * \param[in]     element              The element to be translated.
             *
             * \param[in,out] codeGenerationEngine The code generation engine to be invoked for this translation.
             *
             * \return Returns true on success, returns false on error.
             */
            bool translate(ElementPointer element, CodeGenerationEngine& codeGenerationEngine) final;

            /**
             * Method that is called to register formats with the code generation engine.  The default implementation
             * will register any format tied to the supplied element with the generation engine.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            virtual bool registerFormats(ElementPointer element, HtmlCodeGenerationEngine& generationEngine);

            /**
             * Method that is called to insert header content.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            virtual bool header(ElementPointer element, HtmlCodeGenerationEngine& generationEngine);

            /**
             * Method that is called to insert body content.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            virtual bool body(ElementPointer element, HtmlCodeGenerationEngine& generationEngine);
    };
};

#endif
