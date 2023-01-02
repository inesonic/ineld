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
* This file implements the \ref Ld::LaTeXImageTranslatorBase class.
***********************************************************************************************************************/

#include <QString>
#include <QBuffer>
#include <QImageReader>
#include <QImage>
#include <QFileInfo>

#include <utility>

#include <util_units.h>

#include "ld_element.h"
#include "ld_element_structures.h"
#include "ld_root_element.h"
#include "ld_format.h"
#include "ld_format_structures.h"
#include "ld_text_export_context.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_latex_translation_phase.h"
#include "ld_latex_code_generator_diagnostic.h"
#include "ld_latex_translator.h"
#include "ld_latex_image_translator_base.h"

namespace Ld {
    LaTeXImageTranslatorBase::~LaTeXImageTranslatorBase() {}


    bool LaTeXImageTranslatorBase::identifyDependencies(ElementPointer, LaTeXCodeGenerationEngine& engine) {
        LaTeXCodeGenerationEngine::ImageMode imageMode = engine.imageMode();
        if (imageMode != LaTeXCodeGenerationEngine::ImageMode::NO_IMAGES) {
            engine.addPackage("graphicx");
        }

        return true;
    }


    bool LaTeXImageTranslatorBase::body(ElementPointer element, LaTeXCodeGenerationEngine& engine) {
        bool               success;
        TextExportContext& context = engine.context();

        QByteArray rawPayload = getRawImage(element, engine);

        if (!rawPayload.isEmpty()) {
            QBuffer      buffer(&rawPayload);
            QImageReader reader(&buffer);

            QString currentImageFormat = QString(reader.format()).toLower();
            Handle  elementHandle      = element->handle();
            QString command;

            LaTeXCodeGenerationEngine::ImageMode imageMode = engine.imageMode();
            if (imageMode == LaTeXCodeGenerationEngine::ImageMode::NO_IMAGES) {
                QString imageFilename = elementHandle.toCaseInsensitiveQString() + "." + currentImageFormat;
                command = tr("% Insert image %1 here.\n").arg(imageFilename);
                success = true;
            } else {
                QByteArray translatedPayload;

                float imageWidthPoints  = 0;
                float imageHeightPoints = 0;

                QString newFormat = translateImage(
                    rawPayload,
                    currentImageFormat,
                    imageMode,
                    translatedPayload,
                    imageWidthPoints,
                    imageHeightPoints
                );

                if (!newFormat.isEmpty()) {
                    QString imageFilename   = elementHandle.toCaseInsensitiveQString() + "." + newFormat;
                    QString payloadLocation = context.addPayload(imageFilename, translatedPayload);

                    if (!payloadLocation.isEmpty()) {
                        command = latexCommand(
                            payloadLocation,
                            element,
                            element->format(),
                            engine,
                            imageWidthPoints,
                            imageHeightPoints
                        );

                        success = true;
                    } else {
                        LaTeXCodeGeneratorDiagnostic* diagnostic = new LaTeXCodeGeneratorDiagnostic(
                            element,
                            LaTeXCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                            dynamic_cast<const LaTeXTranslationPhase&>(engine.translationPhase()),
                            LaTeXCodeGeneratorDiagnostic::Code::CONTEXT_ERROR,
                            engine.contextPointer()
                        );

                        engine.translationErrorDetected(diagnostic);
                        success = false;
                    }
                } else {
                    success = false;

                    LaTeXCodeGeneratorDiagnostic* diagnostic = new LaTeXCodeGeneratorDiagnostic(
                        element,
                        LaTeXCodeGeneratorDiagnostic::Type::INTERNAL_ERROR,
                        dynamic_cast<const LaTeXTranslationPhase&>(engine.translationPhase()),
                        LaTeXCodeGeneratorDiagnostic::Code::COULD_NOT_TRANSLATE_IMAGE,
                        engine.contextPointer()
                    );

                    engine.translationErrorDetected(diagnostic);
                }
            }

            if (success) {
                context << command << '\n';
            }
        } else {
            LaTeXCodeGeneratorDiagnostic* diagnostic = new LaTeXCodeGeneratorDiagnostic(
                element,
                LaTeXCodeGeneratorDiagnostic::Type::INTERNAL_ERROR,
                dynamic_cast<const LaTeXTranslationPhase&>(engine.translationPhase()),
                LaTeXCodeGeneratorDiagnostic::Code::BAD_IMAGE_PAYLOAD,
                engine.contextPointer()
            );

            engine.translationErrorDetected(diagnostic);
            success = false;
        }

        return success;
    }


    QByteArray LaTeXImageTranslatorBase::getRawImage(ElementPointer element, LaTeXCodeGenerationEngine& /* engine */) {
        QSharedPointer<RootElement> root = element->root().dynamicCast<RootElement>();
        return root->exportElementImage(element, defaultDpi);
    }


    Format::Rotation LaTeXImageTranslatorBase::getRotation(
            ElementPointer             /* element */,
            FormatPointer              /* format */,
            LaTeXCodeGenerationEngine& /* generationEngine */
        ) {
        return Format::Rotation::NO_ROTATION;
    }


    std::pair<float, float> LaTeXImageTranslatorBase::getSize(
            ElementPointer             /* element */,
            FormatPointer              /* forma */,
            LaTeXCodeGenerationEngine& /* generationEngine */
        ) {
        return std::make_pair(0.0F, 0.0F);
    }


    Format::ImageScalingMode LaTeXImageTranslatorBase::getHorizontalScaling(
            ElementPointer             /* element */,
            FormatPointer              /* format */,
            LaTeXCodeGenerationEngine& /* generationEngine */
        ) {
        return Format::ImageScalingMode::IGNORED;
    }


    Format::ImageScalingMode LaTeXImageTranslatorBase::getVerticalScaling(
            ElementPointer             /* element */,
            FormatPointer              /* format */,
            LaTeXCodeGenerationEngine& /* generationEngine */
        ) {
        return Format::ImageScalingMode::IGNORED;
    }


    QString LaTeXImageTranslatorBase::translateImage(
            const QByteArray&                    currentPayload,
            const QString&                       currentPayloadFormat,
            LaTeXCodeGenerationEngine::ImageMode imageMode,
            QByteArray&                          newPayload,
            float&                               imageWidthPoints,
            float&                               imageHeightPoints
        ) {
        bool    convertImage = false;
        QString newPayloadFormat;

        imageWidthPoints  = 0;
        imageHeightPoints = 0;

        switch (imageMode) {
            case LaTeXCodeGenerationEngine::ImageMode::ORIGINAL: {
                newPayloadFormat = currentPayloadFormat;
                break;
            }

            case LaTeXCodeGenerationEngine::ImageMode::FORCE_PNG: {
                convertImage     = (currentPayloadFormat != "png");
                newPayloadFormat = "png";

                break;
            }

            case LaTeXCodeGenerationEngine::ImageMode::FORCE_JPEG: {
                convertImage     = (currentPayloadFormat != "jpg" && currentPayloadFormat != "jpeg");
                newPayloadFormat = "jpg";

                break;
            }

            case LaTeXCodeGenerationEngine::ImageMode::PREFER_PNG: {
                convertImage = (
                       currentPayloadFormat != "jpg"
                    && currentPayloadFormat != "jpeg"
                    && currentPayloadFormat != "png"
                );

                if (convertImage) {
                    newPayloadFormat = "png";
                } else {
                    newPayloadFormat = currentPayloadFormat;
                }

                break;
            }

            case LaTeXCodeGenerationEngine::ImageMode::PREFER_JPEG: {
                convertImage   = (
                       currentPayloadFormat != "jpg"
                    && currentPayloadFormat != "jpeg"
                    && currentPayloadFormat != "png"
                );

                if (convertImage) {
                    newPayloadFormat = "jpeg";
                } else {
                    newPayloadFormat = currentPayloadFormat;
                }

                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        QImage  image;
        bool success = image.loadFromData(currentPayload);

        if (success) {
            imageWidthPoints  = Util::Units::pointsPerMm * 1000.0 * image.width() / image.dotsPerMeterX();
            imageHeightPoints = Util::Units::pointsPerMm * 1000.0 * image.height() / image.dotsPerMeterY();

            if (convertImage) {
                QBuffer imageBuffer(&newPayload);
                success = image.save(&imageBuffer, newPayloadFormat.toLocal8Bit());
            } else {
                newPayload = currentPayload;
            }
        }

        if (!success) {
            newPayloadFormat.clear();
        }

        return newPayloadFormat;
    }


    QString LaTeXImageTranslatorBase::latexCommand(
            const QString&             filename,
            ElementPointer             element,
            FormatPointer              format,
            LaTeXCodeGenerationEngine& engine,
            float                      imageWidthPoints,
            float                      imageHeightPoints
        ) {
        QFileInfo fileInformation(filename);
        QString   graphicsFilename = fileInformation.fileName();
        QString   options;

        float rotatedWidthPoints;
        float rotatedHeightPoints;

        Format::Rotation rotation = getRotation(element, format, engine);
        switch (rotation) {
            case Format::Rotation::NO_ROTATION: {
                options = "angle=0";
                rotatedWidthPoints  = imageWidthPoints;
                rotatedHeightPoints = imageHeightPoints;
                break;
            }

            case Format::Rotation::ROTATE_CCW_90: {
                options = "angle=90";

                rotatedWidthPoints  = imageHeightPoints;
                rotatedHeightPoints = imageWidthPoints;

                break;
            }

            case Format::Rotation::FLIP: {
                options = "angle=180";

                rotatedWidthPoints  = imageWidthPoints;
                rotatedHeightPoints = imageHeightPoints;

                break;
            }

            case Format::Rotation::ROTATE_CW_90: {
                options = "angle=270";

                rotatedWidthPoints  = imageHeightPoints;
                rotatedHeightPoints = imageWidthPoints;

                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        Format::ImageScalingMode horizontalScalingMode = getHorizontalScaling(element, format, engine);
        Format::ImageScalingMode verticalScalingMode   = getVerticalScaling(element, format, engine);

        std::pair<float, float> formatSize = getSize(element, format, engine);

        switch (horizontalScalingMode) {
            case Format::ImageScalingMode::INVALID:
            case Format::ImageScalingMode::IGNORED: {
                options = QString("%1,width=%2pt").arg(options).arg(rotatedWidthPoints);
                break;
            }

            case Format::ImageScalingMode::FIXED_POINTS: {
                options = QString("%1,width=%2pt").arg(options).arg(formatSize.first);
                break;
            }

            case Format::ImageScalingMode::FRACTIONAL: {
                options = QString("%1,width=%2pt").arg(options).arg(rotatedWidthPoints * formatSize.first);
                break;
            }

            case Format::ImageScalingMode::ASPECT_RATIO: {
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        switch (verticalScalingMode) {
            case Format::ImageScalingMode::INVALID:
            case Format::ImageScalingMode::IGNORED: {
                if (rotation == Format::Rotation::NO_ROTATION) {
                    options = QString("%1,height=%2pt").arg(options).arg(rotatedHeightPoints);
                } else {
                    options = QString("%1,totalheight=%2pt").arg(options).arg(rotatedHeightPoints);
                }

                break;
            }

            case Format::ImageScalingMode::FIXED_POINTS: {
                if (rotation == Format::Rotation::NO_ROTATION) {
                    options = QString("%1,height=%2pt").arg(options).arg(formatSize.second);
                } else {
                    options = QString("%1,totalheight=%2pt").arg(options).arg(formatSize.second);
                }

                break;
            }

            case Format::ImageScalingMode::FRACTIONAL: {
                if (rotation == Format::Rotation::NO_ROTATION) {
                    options = QString("%1,height=%2pt")
                              .arg(options)
                              .arg(rotatedHeightPoints * formatSize.second);
                } else {
                    options = QString("%1,totalheight=%2pt")
                              .arg(options)
                              .arg(rotatedHeightPoints * formatSize.second);
                }

                break;
            }

            case Format::ImageScalingMode::ASPECT_RATIO: {
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        QString result;
        if (!options.isEmpty()) {
            result = QString("\\includegraphics[%1]{%2}").arg(options).arg(graphicsFilename);
        } else {
            result = QString("\\includegraphics{%2}").arg(options).arg(graphicsFilename);
        }

        return result;
    }
}
