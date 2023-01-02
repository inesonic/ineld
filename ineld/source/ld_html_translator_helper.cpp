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
* This file implements the \ref Ld::HtmlTranslatorHelper class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>
#include <QUrl>
#include <QColor>
#include <QBuffer>
#include <QByteArray>
#include <QImageReader>
#include <QTransform>
#include <QImage>

#include "ld_element.h"
#include "ld_element_structures.h"
#include "ld_format.h"
#include "ld_format_structures.h"
#include "ld_xml_export_context.h"
#include "ld_code_generation_engine.h"
#include "ld_root_element.h"
#include "ld_html_translation_phase.h"
#include "ld_html_code_generator_diagnostic.h"
#include "ld_html_code_generation_engine.h"
#include "ld_html_translator_helper.h"

namespace Ld {
    HtmlTranslatorHelper::~HtmlTranslatorHelper() {}



    bool HtmlTranslatorHelper::writeEscapedText(const QString& text, HtmlCodeGenerationEngine& engine, bool escapeMathJax) {
        bool success = true;

        XmlExportContext& context = engine.context();

        unsigned long textLength    = static_cast<unsigned long>(text.length());
        unsigned long startingIndex = 0;
        unsigned long endingIndex   = 0;

        while (success && endingIndex < textLength) {
            QChar c = text.at(endingIndex);
            if (c.unicode() > 0x7F) {
                if (endingIndex > startingIndex) {
                    context.writeCharacters(text.mid(startingIndex, endingIndex - startingIndex));
                }

                context.writeEntityReference(QString("#x%1").arg(c.unicode(), 4, 16, QChar('0')));

                ++endingIndex;
                startingIndex = endingIndex;
            } else if ((c == QChar('$') || c == QChar('\\')) && escapeMathJax) {
                if (endingIndex > startingIndex) {
                    context.writeCharacters(text.mid(startingIndex, endingIndex - startingIndex));
                }

                if (engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML5_WITH_CSS) {
                    context.writeStartElement("span");
                    context.writeAttribute("class", "tex2jax_ignore");
                    context.writeCharacters(QString(c));
                    context.writeEndElement();
                } else if (engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML4_WITHOUT_CSS) {
                    context.writeStartElement("span");
                    context.writeCharacters(QString(c));
                    context.writeEndElement();
                } else {
                    HtmlCodeGeneratorDiagnostic* diagnostic = new HtmlCodeGeneratorDiagnostic(
                        nullptr,
                        HtmlCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                        dynamic_cast<const HtmlTranslationPhase&>(engine.translationPhase()),
                        HtmlCodeGeneratorDiagnostic::Code::HTML_STYLE_NOT_SUPPORTED,
                        engine.contextPointer()
                    );

                    engine.translationErrorDetected(diagnostic);
                    success = false;
                }

                ++endingIndex;
                startingIndex = endingIndex;
            } else {
                ++endingIndex;
            }
        }

        if (startingIndex < textLength) {
            context.writeCharacters(text.mid(startingIndex));
        }

        return success;
    }


    bool HtmlTranslatorHelper::writeLaTeX(const QString& latexEquation, HtmlCodeGenerationEngine& engine) {
        XmlExportContext& context = engine.context();
        context.writeCharacters(QString("\\(%1\\)").arg(latexEquation));

        return true;
    }


    bool HtmlTranslatorHelper::writeAsImage(ElementPointer element, HtmlCodeGenerationEngine& engine) const {
        bool success;

        if (engine.imageHandlingMode() != HtmlCodeGenerationEngine::ImageHandlingMode::EXCLUDE) {
            QByteArray payloadData = getRawImage(element, engine);

            if (!payloadData.isEmpty()) {
                if (engine.imageHandlingMode() == HtmlCodeGenerationEngine::ImageHandlingMode::SEPARATE_PAYLOADS) {
                    success = exportPayload(payloadData, element, engine);
                } else {
                    Q_ASSERT(engine.imageHandlingMode() == HtmlCodeGenerationEngine::ImageHandlingMode::EMBEDDED);
                    success = embedPayload(payloadData, element, engine);
                }
            } else {
                HtmlCodeGeneratorDiagnostic* diagnostic = new HtmlCodeGeneratorDiagnostic(
                    element,
                    HtmlCodeGeneratorDiagnostic::Type::INTERNAL_ERROR,
                    dynamic_cast<const HtmlTranslationPhase&>(engine.translationPhase()),
                    HtmlCodeGeneratorDiagnostic::Code::BAD_IMAGE_PAYLOAD,
                    engine.contextPointer()
                );

                engine.translationErrorDetected(diagnostic);
                success = false;
            }
        } else {
            success = true;
        }

        return success;
    }


    QByteArray HtmlTranslatorHelper::getRawImage(
            ElementPointer            element,
            HtmlCodeGenerationEngine& /* engine */
        ) const {
        QSharedPointer<RootElement> root = element->root().dynamicCast<RootElement>();
        return root->exportElementImage(element, generatedImageDpi());
    }


    float HtmlTranslatorHelper::generatedImageDpi() const {
        return defaultGeneratedImageDpi;
    }


    float HtmlTranslatorHelper::htmlImageDpi() const {
        return defaultHtmlImageDpi;
    }


    Format::Rotation HtmlTranslatorHelper::getImageRotation(
            ElementPointer            /* element */,
            FormatPointer             /* format */,
            HtmlCodeGenerationEngine& /* generationEngine */
        ) const {
        return Format::Rotation::NO_ROTATION;
    }


    bool HtmlTranslatorHelper::exportPayload(
            QByteArray&               imageData,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
        ) const {
        bool success = true;

        QBuffer      buffer(&imageData);
        QImageReader reader(&buffer);
        QByteArray   imageFormat = reader.format();

        Handle  elementHandle = element->handle();
        QString imageFilename = elementHandle.toCaseInsensitiveQString() + "." + QString(imageFormat);
        QSize   imageSize     = reader.size();
        float   scaleFactor   = htmlImageDpi() / generatedImageDpi();
        QSize   htmlSize(int(0.5 + imageSize.width() * scaleFactor), int(0.5 + imageSize.height() * scaleFactor));

        if (engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML4_WITHOUT_CSS) {
            FormatPointer format   = element->format();

            if (!format.isNull()) {
                Format::Rotation rotation = getImageRotation(element, format, engine);
                if (rotation != Format::Rotation::NO_ROTATION) {
                    QImage image;
                    success = image.loadFromData(imageData);
                    if (success) {
                        QTransform transformationMatrix;
                        transformationMatrix.rotate(-90.0 * static_cast<int>(rotation));
                        image = image.transformed(transformationMatrix, Qt::TransformationMode::SmoothTransformation);

                        QByteArray newImageData;
                        QBuffer saveBuffer(&newImageData);
                        success = image.save(&saveBuffer, imageFormat.data());
                        if (success) {
                            imageData = newImageData;
                        }
                    } else {
                        HtmlCodeGeneratorDiagnostic* diagnostic = new HtmlCodeGeneratorDiagnostic(
                            element,
                            HtmlCodeGeneratorDiagnostic::Type::INTERNAL_ERROR,
                            dynamic_cast<const HtmlTranslationPhase&>(engine.translationPhase()),
                            HtmlCodeGeneratorDiagnostic::Code::BAD_IMAGE_PAYLOAD,
                            engine.contextPointer()
                        );

                        engine.translationErrorDetected(diagnostic);
                    }
                }
            }
        }

        if (success) {
            XmlExportContext& context         = engine.context();
            QString           payloadLocation = context.addPayload(imageFilename, imageData);
            if (payloadLocation.isEmpty()) {
                HtmlCodeGeneratorDiagnostic* diagnostic = new HtmlCodeGeneratorDiagnostic(
                    element,
                    HtmlCodeGeneratorDiagnostic::Type::INTERNAL_ERROR,
                    dynamic_cast<const HtmlTranslationPhase&>(engine.translationPhase()),
                    HtmlCodeGeneratorDiagnostic::Code::CONTEXT_ERROR,
                    engine.contextPointer()
                );

                engine.translationErrorDetected(diagnostic);
            } else {
                context.writeEmptyElement("img");

                context.writeAttribute("src", engine.toUri(payloadLocation));
                context.writeAttribute("alt", elementHandle.toQString());
                context.writeAttribute("width", QString::number(htmlSize.width()));
                context.writeAttribute("height", QString::number(htmlSize.height()));
                FormatPointer imageFormat = element->format();
                if (!imageFormat.isNull()                                                     &&
                    engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML5_WITH_CSS    ) {
                    QString className = engine.cssClassForFormat(imageFormat);
                    if (!className.isEmpty()) {
                        context.writeAttribute("class", className);
                    }
                }
            }
        }

        return success;
    }


    bool HtmlTranslatorHelper::embedPayload(
            QByteArray&               imageData,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
        ) const {
        bool success = true;

        QBuffer      buffer(&imageData);
        QImageReader reader(&buffer);
        QString      imageFormat    = QString(reader.format()).toLower();
        bool         rebuildPayload = false;
        QImage       image;

        success = image.loadFromData(imageData);
        if (success) {
            QSize imageSize     = image.size();
            float scaleFactor   = htmlImageDpi() / generatedImageDpi();
            QSize htmlSize(int(0.5 + imageSize.width() * scaleFactor), int(0.5 + imageSize.height() * scaleFactor));

            if (engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML4_WITHOUT_CSS) {
                FormatPointer    format   = element->format();
                Format::Rotation rotation =   !format.isNull()
                                            ? getImageRotation(element, format, engine)
                                            : Format::Rotation::NO_ROTATION;

                if (rotation != Format::Rotation::NO_ROTATION) {
                    QTransform transformationMatrix;
                    transformationMatrix.rotate(-90.0 * static_cast<int>(rotation));
                    image = image.transformed(transformationMatrix, Qt::TransformationMode::SmoothTransformation);
                    rebuildPayload = true;
                }
            }

            if (imageFormat != "png" && imageFormat != "jpg" && imageFormat != "jpeg") {
                imageFormat    = "png";
                rebuildPayload = true;
            }

            if (rebuildPayload) {
                QByteArray newImageData;
                QBuffer saveBuffer(&newImageData);
                success = image.save(&saveBuffer, imageFormat.toLocal8Bit().data());
                if (success) {
                    success = embedPreparedPayload(newImageData, imageFormat, htmlSize, element, engine);
                } else {
                    HtmlCodeGeneratorDiagnostic* diagnostic = new HtmlCodeGeneratorDiagnostic(
                        element,
                        HtmlCodeGeneratorDiagnostic::Type::INTERNAL_ERROR,
                        dynamic_cast<const HtmlTranslationPhase&>(engine.translationPhase()),
                        HtmlCodeGeneratorDiagnostic::Code::IMAGE_SAVE_ERROR,
                        engine.contextPointer()
                    );

                    engine.translationErrorDetected(diagnostic);
                }
            } else {
                success = embedPreparedPayload(imageData, imageFormat, htmlSize, element, engine);
            }
        } else {
            HtmlCodeGeneratorDiagnostic* diagnostic = new HtmlCodeGeneratorDiagnostic(
                element,
                HtmlCodeGeneratorDiagnostic::Type::INTERNAL_ERROR,
                dynamic_cast<const HtmlTranslationPhase&>(engine.translationPhase()),
                HtmlCodeGeneratorDiagnostic::Code::BAD_IMAGE_PAYLOAD,
                engine.contextPointer()
            );

            engine.translationErrorDetected(diagnostic);
        }

        return success;
    }


    bool HtmlTranslatorHelper::embedPreparedPayload(
            const QByteArray&         imageData,
            const QString&            format,
            const QSize&              htmlSize,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
        ) const {
        XmlExportContext& context = engine.context();
        Handle  elementHandle     = element->handle();
        QString mimeType          = QString("image/%1").arg(format == "jpg" ? QString("jpeg") : format);
        QString sourceAttribute   = QString("data:%1;base64,%2")
                                    .arg(mimeType, imageData.toBase64().data());

        context.writeEmptyElement("img");
        context.writeAttribute("src", sourceAttribute);
        context.writeAttribute("alt", elementHandle.toQString());
        context.writeAttribute("width", QString::number(htmlSize.width()));
        context.writeAttribute("height", QString::number(htmlSize.height()));

        FormatPointer imageFormat = element->format();
        if (!imageFormat.isNull()                                                     &&
            engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML5_WITH_CSS    ) {
            QString className = engine.cssClassForFormat(imageFormat);
            if (!className.isEmpty()) {
                context.writeAttribute("class", className);
            }
        }

        return true;
    }
}
