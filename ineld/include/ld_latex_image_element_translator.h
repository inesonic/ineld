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
* This header defines the \ref Ld::LaTeXImageElementTranslator class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_LATEX_IMAGE_ELEMENT_TRANSLATOR_H
#define LD_LATEX_IMAGE_ELEMENT_TRANSLATOR_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QByteArray>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_latex_image_translator_base.h"

namespace Ld {
    class LaTeXCodeGenerationEngine;

    /**
     * LaTeX translator for the image element.
     */
    class LD_PUBLIC_API LaTeXImageElementTranslator:public LaTeXImageTranslatorBase {
        Q_DECLARE_TR_FUNCTIONS(Ld::LaTeXImageElementTranslator)

        public:
            ~LaTeXImageElementTranslator() override;

            /**
             * Method that returns the name of the element this translator operates on.
             *
             * \return Returns the name of the element this translator operates on.
             */
            QString elementName() const override;

        protected:
            /**
             * Method you should overload to provide the required image rotation setting.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] format           A pointer to the element format.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns the required rotation setting.
             */
            Format::Rotation getRotation(
                ElementPointer             element,
                FormatPointer              format,
                LaTeXCodeGenerationEngine& generationEngine
            ) override;

            /**
             * Method you should overload to obtain the desired image size, in points.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] format           A pointer to the element format.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns a std::pair<float, float> holding the desired image size, in points.
             */
            std::pair<float, float> getSize(
                ElementPointer             element,
                FormatPointer              format,
                LaTeXCodeGenerationEngine& generationEngine
            ) override;

            /**
             * Method you should overload to provide the required horizontal scaling setting.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] format           A pointer to the element format.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns the required horizontal scaling setting.
             */
            Format::ImageScalingMode getHorizontalScaling(
                ElementPointer             element,
                FormatPointer              format,
                LaTeXCodeGenerationEngine& generationEngine
            ) override;

            /**
             * Method you should overload to provide the required vertical scaling setting.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] format           A pointer to the element format.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns the required horizontal scaling setting.
             */
            Format::ImageScalingMode getVerticalScaling(
                ElementPointer             element,
                FormatPointer              format,
                LaTeXCodeGenerationEngine& generationEngine
            ) override;
    };
};

#endif
