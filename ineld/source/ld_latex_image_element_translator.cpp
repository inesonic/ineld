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
* This file implements the \ref Ld::LaTeXImageElementTranslator class.
***********************************************************************************************************************/

#include <QString>
#include <QBuffer>
#include <QImageReader>
#include <QImage>
#include <QFileInfo>

#include <util_units.h>

#include "ld_element.h"
#include "ld_element_structures.h"
#include "ld_image_element.h"
#include "ld_format.h"
#include "ld_format_structures.h"
#include "ld_image_format.h"
#include "ld_text_export_context.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_latex_translation_phase.h"
#include "ld_latex_code_generator_diagnostic.h"
#include "ld_latex_translator.h"
#include "ld_latex_image_element_translator.h"

namespace Ld {
    LaTeXImageElementTranslator::~LaTeXImageElementTranslator() {}


    QString LaTeXImageElementTranslator::elementName() const {
        return ImageElement::elementName;
    }


    Format::Rotation LaTeXImageElementTranslator::getRotation(
            ElementPointer             /* element */,
            FormatPointer              format,
            LaTeXCodeGenerationEngine& /* generationEngine */
        ) {
        Format::Rotation result;

        QSharedPointer<ImageFormat> imageFormat = format.dynamicCast<ImageFormat>();
        if (!imageFormat.isNull()) {
            result = imageFormat->rotation();
        } else {
            result = Format::Rotation::NO_ROTATION;
        }

        return result;
    }


    std::pair<float, float> LaTeXImageElementTranslator::getSize(
            ElementPointer             /* element */,
            FormatPointer              format,
            LaTeXCodeGenerationEngine& /* generationEngine */
        ) {
        float horizontalValue = 0;
        float verticalValue   = 0;

        QSharedPointer<ImageFormat> imageFormat = format.dynamicCast<ImageFormat>();
        if (!imageFormat.isNull()) {
            const ImageFormat::Axis& horizontalAxis = imageFormat->horizontalAxis();
            const ImageFormat::Axis& verticalAxis   = imageFormat->verticalAxis();

            horizontalValue = horizontalAxis.value();
            verticalValue   = verticalAxis.value();
        }

        return std::make_pair(horizontalValue, verticalValue);
    }


    Format::ImageScalingMode LaTeXImageElementTranslator::getHorizontalScaling(
            ElementPointer             /* element */,
            FormatPointer              format,
            LaTeXCodeGenerationEngine& /* generationEngine */
        ) {
        Format::ImageScalingMode result;

        QSharedPointer<ImageFormat> imageFormat = format.dynamicCast<ImageFormat>();
        if (!imageFormat.isNull()) {
            const ImageFormat::Axis& horizontalAxis = imageFormat->horizontalAxis();
            result = horizontalAxis.scalingMode();
        } else {
            result = Format::ImageScalingMode::IGNORED;
        }

        return result;
    }


    Format::ImageScalingMode LaTeXImageElementTranslator::getVerticalScaling(
            ElementPointer             /* element */,
            FormatPointer              format,
            LaTeXCodeGenerationEngine& /* generationEngine */
        ) {
        Format::ImageScalingMode result;

        QSharedPointer<ImageFormat> imageFormat = format.dynamicCast<ImageFormat>();
        if (!imageFormat.isNull()) {
            const ImageFormat::Axis& verticalAxis = imageFormat->verticalAxis();
            result = verticalAxis.scalingMode();
        } else {
            result = Format::ImageScalingMode::IGNORED;
        }

        return result;
    }
}
