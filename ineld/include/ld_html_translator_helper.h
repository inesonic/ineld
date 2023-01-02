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
* This header defines the \ref Ld::HtmlTranslatorHelper class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_HTML_TRANSLATOR_HELPER_H
#define LD_HTML_TRANSLATOR_HELPER_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QByteArray>
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_format.h"
#include "ld_translator.h"

namespace Ld {
    class HtmlCodeGenerationEngine;
    class XmlExportContext;

    /**
     * Virtual base class for per-element translators that convert the element in the syntax tree to an HTML
     * representation.
     */
    class LD_PUBLIC_API HtmlTranslatorHelper {
        Q_DECLARE_TR_FUNCTIONS(Ld::HtmlTranslatorHelper)

        public:
            virtual ~HtmlTranslatorHelper();

        protected:
            /**
             * The default image DPI setting.
             */
            static constexpr float defaultGeneratedImageDpi = 300;

            /**
             * The default HTML image DPI setting.
             */
            static constexpr float defaultHtmlImageDpi = 96;

            /**
             * Method you can use to write escaped text.  You can use this in place of the
             * Ld::XmlExportContext::writeCharacters method to escape characters that are not ASCII.  The method can
             * also escape dollar signs and backslash characters in order to avoid issues with MathJax.
             *
             * \param[in] text             The text to be written.
             *
             * \param[in] generationEngine The engine driving the conversion.
             *
             * \param[in] escapeMathJax    If true, characters used to start/end MathJax statements will be escaped.
             *                             If false, special MathJax characters will not be escaped.
             *
             * \return Returns true on success, returns false on error.
             */
            static bool writeEscapedText(
                const QString&            text,
                HtmlCodeGenerationEngine& generationEngine,
                bool                      escapeMathJax
            );

            /**
             * Method you can use this method to insert properly escaped MathJax LaTeX equations.
             *
             * \param[in] latexEquation    The equation in LaTeX format to be presented to MathJax.
             *
             * \param[in] generationEngine The engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            static bool writeLaTeX(const QString& latexEquation, HtmlCodeGenerationEngine& generationEngine);

            /**
             * Method you can use to export the current element as an image.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            bool writeAsImage(ElementPointer element, HtmlCodeGenerationEngine& generationEngine) const;

            /**
             * Method you should overload to provide the requested image.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns the image as a raw payload.  An empty payload is returned on error.
             */
            virtual QByteArray getRawImage(ElementPointer element, HtmlCodeGenerationEngine& generationEngine) const;

            /**
             * Method you should overload to specify the desired image DPI used when generating the image.
             *
             * \return Returns the desired image DPI.
             */
            virtual float generatedImageDpi() const;

            /**
             * Method you should overload to specify the desired image DPI used to calculate image width and height.
             *
             * \return Returns the desired image DPI.
             */
            virtual float htmlImageDpi() const;

            /**
             * Method you should overload to provide the required image rotation setting.  This method is only used if
             * the HTML is generated without CSS.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] format           A pointer to the element format.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns the required rotation setting.  The default implementation returns
             *         \ref Ld::Format::Rotation::NO_ROTATION.
             */
            virtual Format::Rotation getImageRotation(
                ElementPointer            element,
                FormatPointer             format,
                HtmlCodeGenerationEngine& generationEngine
            ) const;

            /**
             * Method that exports an image as a distinct payload.
             *
             * \param[in]     imageData The buffer containing the image to be exported.
             *
             * \param[in,out] element   A pointer to the element to be translated.
             *
             * \param[in,out] engine    The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            bool exportPayload(QByteArray& imageData, ElementPointer element, HtmlCodeGenerationEngine& engine) const;

            /**
             * Method that embeds an image into the HTML stream.
             *
             * \param[in]     imageData The buffer containing the image to be exported.
             *
             * \param[in,out] element   A pointer to the element to be translated.
             *
             * \param[in,out] engine    The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            bool embedPayload(QByteArray& imageData, ElementPointer element, HtmlCodeGenerationEngine& engine) const;

            /**
             * Method that embeds an image into the HTML stream.
             *
             * \param[in]     imageData The buffer containing the image to be exported.
             *
             * \param[in]     format    The format for the image. This will be one of "png", "jpeg", or "jpg".
             *
             * \param[in]     htmlSize  The HTML size to specify.
             *
             * \param[in,out] element   A pointer to the element to be translated.
             *
             * \param[in,out] engine    The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            bool embedPreparedPayload(
                const QByteArray&         imageData,
                const QString&            format,
                const QSize&              htmlSize,
                ElementPointer            element,
                HtmlCodeGenerationEngine& engine
            ) const;
    };
};

#endif
