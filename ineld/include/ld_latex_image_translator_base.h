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
* This header defines the \ref Ld::LaTeXImageTranslatorBase class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_LATEX_IMAGE_TRANSLATOR_BASE_H
#define LD_LATEX_IMAGE_TRANSLATOR_BASE_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QByteArray>

#include <utility>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_format.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_latex_translator.h"

namespace Ld {
    class LaTeXCodeGenerationEngine;

    /**
     * Base class for LaTeX translators that insert images into the LaTeX stream.
     */
    class LD_PUBLIC_API LaTeXImageTranslatorBase:public LaTeXTranslator {
        Q_DECLARE_TR_FUNCTIONS(Ld::LaTeXImageTranslatorBase)

        public:
            ~LaTeXImageTranslatorBase() override;

            /**
             * Method that is called during the \ref Ld::LaTeXTranslationPhase::Phase::IDENTIFY_DEPENDENCIES phase to
             * collect a list of packages needed by this element translator.  You can also use this phase to define
             * any special custom commands.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            bool identifyDependencies(ElementPointer element, LaTeXCodeGenerationEngine& generationEngine) override;

            /**
             * Method that is called to insert the LaTeX body content.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            bool body(ElementPointer element, LaTeXCodeGenerationEngine& generationEngine) override;

        protected:
            /**
             * The default dots per inch to use for generated images.
             */
            static constexpr float defaultDpi = 300.0F;

            /**
             * Method you should overload to provide the requested image.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns the image as a raw payload.  An empty payload is returned on error.
             */
            virtual QByteArray getRawImage(ElementPointer element, LaTeXCodeGenerationEngine& generationEngine);

            /**
             * Method you should overload to provide the required image rotation setting.
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
            virtual Format::Rotation getRotation(
                ElementPointer             element,
                FormatPointer              format,
                LaTeXCodeGenerationEngine& generationEngine
            );

            /**
             * Method you should overload to obtain the desired image size, in points.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] format           A pointer to the element format.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns a std::pair<float, float> holding the desired image size, in points.  A value of
             *         std::make_pair(0.0F, 0/0F) indicates that the image size should be determined from the provided
             *         image.  The default implementation returns std::make_pair(0.0F, 0.0F).
             */
            virtual std::pair<float, float> getSize(
                ElementPointer             element,
                FormatPointer              format,
                LaTeXCodeGenerationEngine& generationEngine
            );

            /**
             * Method you should overload to provide the required horizontal scaling setting.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] format           A pointer to the element format.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns the required horizontal scaling setting.  The default implementation returns
             *         \ref Ld::Format::ImageScalingMode::IGNORED.
             */
            virtual Format::ImageScalingMode getHorizontalScaling(
                ElementPointer             element,
                FormatPointer              format,
                LaTeXCodeGenerationEngine& generationEngine
            );

            /**
             * Method you should overload to provide the required vertical scaling setting.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] format           A pointer to the element format.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns the required horizontal scaling setting.  The default implementation returns
             *         \ref Ld::Format::ImageScalingMode::IGNORED.
             */
            virtual Format::ImageScalingMode getVerticalScaling(
                ElementPointer             element,
                FormatPointer              format,
                LaTeXCodeGenerationEngine& generationEngine
            );

        private:
            /**
             * The default dots per point to assume for scaling purposes.
             */
            static constexpr float defaultDotsPerPoint = 96.0F / 72.0F;

            /**
             * Methods used to convert images to different formats.
             *
             * \param[in]  currentPayload       The current image payload.
             *
             * \param[in]  currentPayloadFormat A string representing the current image format.
             *
             * \param[in]  imageMode            The image translation mode.
             *
             * \param[out] newPayload           A buffer to hold the image in the new format.
             *
             * \param[out] imageWidthPoints     The reported image width, in points.
             *
             * \param[out] imageHeightPoints    The reported image height, in points.
             *
             * \return Returns a string holding the new format name/extension.  An empty string is returned if an
             *         error occurs.
             */
            static QString translateImage(
                const QByteArray&                    currentPayload,
                const QString&                       currentPayloadFormat,
                LaTeXCodeGenerationEngine::ImageMode imageMode,
                QByteArray&                          newPayload,
                float&                               imageWidthPoints,
                float&                               imageHeightPoints
            );

            /**
             * Method that builds the LaTeX command used to write the image.
             *
             * \param[in]     payloadFilename   The absolute path to the payload file.
             *
             * \param[in,out] element           A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine  The generation engine driving the conversion.
             *
             * \param[in]     format            The image format used to set LaTeX command options.
             *
             * \param[in]     imageWidthPoints  The default image width, in points.
             *
             * \param[in]     imageHeightPoints The default image height, in points.
             *
             * \return Returns the LaTeX command used to present the image to the user.
             */
            QString latexCommand(
                const QString&             payloadFilename,
                ElementPointer             element,
                FormatPointer              format,
                LaTeXCodeGenerationEngine& generationEngine,
                float                      imageWidthPoints,
                float                      imageHeightPoints
            );
    };
};

#endif
