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
* This file implements the \ref Ld::HtmlCodeGenerationEngine class.
***********************************************************************************************************************/

#include <QCoreApplication> // For QCoreApplication::applicationName and Q_DECLARE_TR_FUNCTIONS
#include <QString>
#include <QByteArray>
#include <QBuffer>
#include <QUrl>

#include <cassert>

#include "ld_element_structures.h"
#include "ld_root_element.h"
#include "ld_paragraph_element.h"
#include "ld_text_element.h"
#include "ld_format_container.h"
#include "ld_format_organizer.h"
#include "ld_code_generator_output_type_container.h"
#include "ld_translation_phase.h"
#include "ld_translator.h"
#include "ld_code_generator.h"
#include "ld_xml_file_export_context.h"
#include "ld_xml_memory_export_context.h"
#include "ld_xml_temporary_file_export_context.h"
#include "ld_html_code_generator_output_types.h"
#include "ld_html_code_generator_diagnostic.h"
#include "ld_html_translation_phase.h"
#include "ld_code_generation_engine.h"
#include "ld_html_code_generation_engine.h"

namespace Ld {
    static const char mathJaxHtml5Configuration[] = \
            "MathJax.Hub.Config({\n"
            "    tex2jax: {\n"
            "        inlineMath:  [ [ \"\\\\(\", \"\\\\)\" ] ],\n"
            "        displayMath: [ [ \"\\\\[\", \"\\\\]\" ] ]\n"
            "    }\n"
            "});\n";

    static const char mathJaxHtml4Configuration[] = \
        "MathJax.Hub.Config({\n"
        "    tex2jax: {\n"
        "        inlineMath:  [ [ \"\\\\(\", \"\\\\)\" ] ],\n"
        "        displayMath: [ [ \"\\\\[\", \"\\\\]\" ] ],\n"
        "        skipTags:    [ \"script\", \"noscript\", \"style\", \"textarea\", \"pre\", \"code\", \"span\" ]\n"
        "    }\n"
        "});\n";

    HtmlCodeGenerationEngine::HtmlCodeGenerationEngine(
            CodeGenerator*                              codeGenerator,
            QSharedPointer<RootElement>                 rootElement,
            const QString&                              outputFile,
            const CodeGeneratorOutputTypeContainer&     outputType,
            CodeGeneratorOutputType::ExportMode         exportMode,
            Ud::UsageData*                              usageData,
            bool                                        ignoreMissingTranslators,
            HtmlCodeGenerationEngine::MathMode          mathMode,
            HtmlCodeGenerationEngine::HtmlStyle         htmlStyle,
            HtmlCodeGenerationEngine::ImageHandlingMode imageHandlingMode,
            bool                                        includeImports
        ):CodeGenerationEngine(
            codeGenerator,
            rootElement,
            outputFile,
            outputType,
            exportMode,
            usageData
        ) {
        createExportContext(outputFile, exportMode);

        currentIgnoreMissingPerElementTranslators = ignoreMissingTranslators;
        currentMathMode                           = mathMode;
        currentHtmlStyle                          = htmlStyle;
        currentImageHandingMode                   = imageHandlingMode;
        currentIncludeImports                     = includeImports;
        currentMathModeNesting                    = 0;
        currentInlineMode                         = false;
    }


    HtmlCodeGenerationEngine::~HtmlCodeGenerationEngine() {}


    HtmlCodeGenerationEngine::MathMode HtmlCodeGenerationEngine::mathMode() const {
        return currentMathMode;
    }


    HtmlCodeGenerationEngine::HtmlStyle HtmlCodeGenerationEngine::htmlStyle() const {
        return currentHtmlStyle;
    }


    HtmlCodeGenerationEngine::ImageHandlingMode HtmlCodeGenerationEngine::imageHandlingMode() const {
        return currentImageHandingMode;
    }


    XmlExportContext& HtmlCodeGenerationEngine::context() {
        return *currentContext;
    }


    const XmlExportContext& HtmlCodeGenerationEngine::context() const {
        return *currentContext;
    }


    QSharedPointer<XmlExportContext> HtmlCodeGenerationEngine::contextPointer() const {
        return currentContext;
    }


    void HtmlCodeGenerationEngine::registerFormat(FormatPointer format) {
        formatOrganizer.addFormat(format);
    }


    FormatIdentifier HtmlCodeGenerationEngine::identiferForFormat(FormatPointer format) {
        return formatOrganizer.identifier(format);
    }


    QString HtmlCodeGenerationEngine::cssClassForFormat(FormatPointer format) {
        QString result;

        FormatIdentifier identifier = formatOrganizer.identifier(format);
        if (identifier != invalidFormatIdentifier) {
            result = QString("c%1").arg(identifier);
        }

        return result;
    }


    QString HtmlCodeGenerationEngine::toUri(const QString& filename) {
        QString result;
        switch (exportMode()) {
            case CodeGeneratorOutputType::ExportMode::EXPORT_IN_MEMORY: {
                result = filename;
                break;
            }

            case CodeGeneratorOutputType::ExportMode::EXPORT_AS_DIRECTORY: {
                QFileInfo fileInformation(filename);
                result = fileInformation.fileName();
                break;
            }

            case CodeGeneratorOutputType::ExportMode::EXPORT_AS_MIXED_TEMPORARY_OBJECT: {
                QUrl uri = QUrl::fromLocalFile(filename);
                result = uri.toString();
                break;
            }

            default: { // All other options are not supported by the HTML code generator.
                assert(false);
                break;
            }
        }

        return result;
    }


    void HtmlCodeGenerationEngine::enterMathMode(bool inlineMode) {
        if (currentMathModeNesting == 0) {
            if (currentMathMode == MathMode::MATHML || currentMathMode == MathMode::MATHJAX_MATHML) {
                currentContext->writeStartElement("math");
                currentContext->writeAttribute("xmlns", "http://www.w3.org/1998/Math/MathML");
                currentContext->writeAttribute("display", inlineMode ? "inline" : "block");

                currentContext->writeStartElement("semantics");
                currentContext->writeStartElement("mrow");
            } else if (currentMathMode == MathMode::MATHJAX_LATEX) {
                currentContext->writeCharacters(inlineMode ? "\\(" : "\\[");
            }

            currentInlineMode = inlineMode;
        }

        ++currentMathModeNesting;
    }


    void HtmlCodeGenerationEngine::exitMathMode() {
        --currentMathModeNesting;
        if (currentMathModeNesting == 0) {
            if (currentMathMode == MathMode::MATHML || currentMathMode == MathMode::MATHJAX_MATHML) {
                currentContext->writeEndElement(); // End mrow
                currentContext->writeEndElement(); // End semantics
                currentContext->writeEndElement(); // End math
            } else {
                currentContext->writeCharacters(currentInlineMode ? "\\)" : "\\]");
            }
        }
    }


    bool HtmlCodeGenerationEngine::useInlineMathMode(ElementPointer element) {
        bool isInlineElement = true;

        ElementPointer parent = element->parent();
        if (!parent.isNull() && parent->typeName() == ParagraphElement::elementName) {
            unsigned long numberChildren = parent->numberChildren();
            if (numberChildren == 1) {
                isInlineElement = false;
            } else if (numberChildren == 3) {
                bool containsNormalText = parent.dynamicCast<ParagraphElement>()->containsNormalText();
                if (!containsNormalText) {
                    isInlineElement = (
                           parent->child(0)->typeName() != TextElement::elementName
                        || parent->child(1) != element
                        || parent->child(2)->typeName() != TextElement::elementName
                    );
                }
            }
        }

        return isInlineElement;
    }


    TranslationPhase* HtmlCodeGenerationEngine::createTranslationPhase() const {
        return new HtmlTranslationPhase;
    }


    bool HtmlCodeGenerationEngine::includeRootImports() const {
        return currentIncludeImports;
    }


    bool HtmlCodeGenerationEngine::preTranslate() {
        bool success;
        const HtmlTranslationPhase&
            htmlTranslationPhase = dynamic_cast<const HtmlTranslationPhase&>(translationPhase());

        switch (htmlTranslationPhase.phase()) {
            case HtmlTranslationPhase::Phase::DTD: {
                success = preDtd();
                break;
            }

            case HtmlTranslationPhase::Phase::REGISTER_FORMATS: {
                success = preRegisterFormats();
                break;
            }

            case HtmlTranslationPhase::Phase::HEADER: {
                success = preHeader();
                break;
            }

            case HtmlTranslationPhase::Phase::BODY: {
                success = preBody();
                break;
            }

            default: {
                HtmlCodeGeneratorDiagnostic* diagnostic = new HtmlCodeGeneratorDiagnostic(
                    nullptr,
                    HtmlCodeGeneratorDiagnostic::Type::INTERNAL_ERROR,
                    htmlTranslationPhase,
                    HtmlCodeGeneratorDiagnostic::Code::UNEXPECTED_TRANSLATION_PHASE,
                    currentContext
                );

                translationErrorDetected(diagnostic);
                success = false;

                break;
            }
        }

        return success;
    }


    bool HtmlCodeGenerationEngine::postTranslate() {
        bool success;
        const HtmlTranslationPhase&
            htmlTranslationPhase = dynamic_cast<const HtmlTranslationPhase&>(translationPhase());

        switch (htmlTranslationPhase.phase()) {
            case HtmlTranslationPhase::Phase::DTD: {
                success = postDtd();
                break;
            }

            case HtmlTranslationPhase::Phase::REGISTER_FORMATS: {
                success = postRegisterFormats();
                break;
            }

            case HtmlTranslationPhase::Phase::HEADER: {
                success = postHeader();
                break;
            }

            case HtmlTranslationPhase::Phase::BODY: {
                success = postBody();
                break;
            }

            default: {
                HtmlCodeGeneratorDiagnostic* diagnostic = new HtmlCodeGeneratorDiagnostic(
                    nullptr,
                    HtmlCodeGeneratorDiagnostic::Type::INTERNAL_ERROR,
                    htmlTranslationPhase,
                    HtmlCodeGeneratorDiagnostic::Code::UNEXPECTED_TRANSLATION_PHASE,
                    currentContext
                );

                translationErrorDetected(diagnostic);
                success = false;

                break;
            }
        }

        return success;
    }


    bool HtmlCodeGenerationEngine::missingTranslator(ElementPointer element) {
        bool result;

        if (currentIgnoreMissingPerElementTranslators) {
            result = false;
        } else {
            HtmlCodeGeneratorDiagnostic* diagnostic = new HtmlCodeGeneratorDiagnostic(
                element,
                HtmlCodeGeneratorDiagnostic::Type::INTERNAL_ERROR,
                dynamic_cast<const HtmlTranslationPhase&>(translationPhase()),
                HtmlCodeGeneratorDiagnostic::Code::MISSING_TRANSLATOR,
                currentContext
            );

            translationErrorDetected(diagnostic);
            result = true;
        }

        return result;
    }


    bool HtmlCodeGenerationEngine::preDtd() {
        bool success;

        if (currentHtmlStyle == HtmlStyle::HTML5_WITH_CSS) {
            // Optional for HTML, this injects an XML description indicating the character set.
            currentContext->writeStartDocument();
            currentContext->writeDTD("<!DOCTYPE html>");

            success = true;
        } else if (currentHtmlStyle == HtmlStyle::HTML4_WITHOUT_CSS) {
            currentContext->writeDTD(
                "<!DOCTYPE HTML PUBLIC "
                "\"-//W3C//DTD HTML 4.01 Transitional//EN\" "
                "\"http://www.w3.org/TR/html4/loose.dtd\">"
            );

            success = true;
        } else {
            HtmlCodeGeneratorDiagnostic* diagnostic = new HtmlCodeGeneratorDiagnostic(
                nullptr,
                HtmlCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                dynamic_cast<const HtmlTranslationPhase&>(translationPhase()),
                HtmlCodeGeneratorDiagnostic::Code::HTML_STYLE_NOT_SUPPORTED,
                currentContext
            );

            translationErrorDetected(diagnostic);
            success = false;
        }

        return success;
    }


    bool HtmlCodeGenerationEngine::postDtd() {
        currentContext->writeStartElement("html");
        return true;
    }


    bool HtmlCodeGenerationEngine::preRegisterFormats() {
        return true;
    }


    bool HtmlCodeGenerationEngine::postRegisterFormats() {
        return true;
    }


    bool HtmlCodeGenerationEngine::preHeader() {
        bool success;

        if (currentMathMode == MathMode::MATHJAX_MATHML || currentMathMode == MathMode::MATHJAX_LATEX) {
            currentContext->writeEmptyElement("meta");
            currentContext->writeAttribute("http-equiv", "X-UA-Compatible");
            currentContext->writeAttribute("content", "IE=edge");
        }

        currentContext->writeStartElement("head");
        currentContext->writeStartElement("title");
        currentContext->writeCharacters(tr("Generated By %1").arg(QCoreApplication::applicationName()));
        currentContext->writeEndElement(); // end title

        currentContext->writeEmptyElement("meta");
        currentContext->writeAttribute("name", "generator");
        currentContext->writeAttribute("content", QCoreApplication::applicationName());

        currentContext->writeEmptyElement("meta");
        currentContext->writeAttribute("name", "viewport");
        currentContext->writeAttribute("content", "width=device-width, initial-scale=1.0");

        if (currentHtmlStyle == HtmlStyle::HTML5_WITH_CSS) {
            QString css;
            FormatsByIdentifier formatsByIdentifier = formatOrganizer.formatsByIdentifier();
            for (  FormatsByIdentifier::const_iterator it  = formatsByIdentifier.constBegin(),
                                                       end = formatsByIdentifier.constEnd()
                 ; it != end
                 ; ++it
                ) {
                FormatIdentifier       identifier = it.key();
                const FormatContainer& container  = it.value();
                QString                className  = QString("c%1").arg(identifier);
                FormatPointer          format     = container.format();

                css += format->toCssEntry(className);
            }

            QByteArray payload = css.toLatin1();
            QString styleSheetLocation = currentContext->addPayload("styles.css", payload);

            currentContext->writeEmptyElement("link");
            currentContext->writeAttribute("rel", "stylesheet");
            currentContext->writeAttribute("type", "text/css");

            // TODO: Absolute filepath.
            currentContext->writeAttribute("href", toUri(styleSheetLocation));

            success = true;
        } else if (currentHtmlStyle == HtmlStyle::HTML4_WITHOUT_CSS) {
            currentContext->writeEmptyElement("meta");
            currentContext->writeAttribute("http-equiv", "content-type");
            currentContext->writeAttribute("content", "text/html; charset=UTF-8");

            success = true;
        } else {
            HtmlCodeGeneratorDiagnostic* diagnostic = new HtmlCodeGeneratorDiagnostic(
                nullptr,
                HtmlCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                dynamic_cast<const HtmlTranslationPhase&>(translationPhase()),
                HtmlCodeGeneratorDiagnostic::Code::HTML_STYLE_NOT_SUPPORTED,
                currentContext
            );

            translationErrorDetected(diagnostic);
            success = false;
        }

        return success;
    }


    bool HtmlCodeGenerationEngine::postHeader() {
        bool success;

        if (currentMathMode == MathMode::MATHJAX_MATHML || currentMathMode == MathMode::MATHJAX_LATEX) {
            QString mathJaxSource = QString("%1?config=%2")
                                    .arg(HtmlCodeGenerator::mathJaxUri)
                                    .arg(HtmlCodeGenerator::mathJaxConfigurationFile);

            if (currentHtmlStyle == HtmlStyle::HTML5_WITH_CSS) {
                QString configuration(mathJaxHtml5Configuration);
                if (!configuration.isEmpty()) {
                    currentContext->writeStartElement("script");
                    currentContext->writeAttribute("type", "text/x-mathjax-config");
                    currentContext->writeCharacters("\n"); // Allows direct I/O below.
                    currentContext->device()->write(configuration.toUtf8());
                    currentContext->writeEndElement();
                }

                currentContext->writeStartElement("script");
                currentContext->writeAttribute("type", "text/javascript");
                currentContext->writeAttribute("src", mathJaxSource);
                currentContext->writeAttribute("async", "async");
                currentContext->writeCharacters(""); // This forces a distinct end tag.
                currentContext->writeEndElement();

                success = true;
            } else if (currentHtmlStyle == HtmlStyle::HTML4_WITHOUT_CSS) {
                QString configuration(mathJaxHtml4Configuration);
                if (!configuration.isEmpty()) {
                    currentContext->writeStartElement("script");
                    currentContext->writeAttribute("type", "text/x-mathjax-config");
                    currentContext->writeCharacters("\n"); // Allows direct I/O below.
                    currentContext->device()->write(configuration.toUtf8());
                    currentContext->writeEndElement();
                }

                currentContext->writeStartElement("script");
                currentContext->writeAttribute("type", "text/javascript");
                currentContext->writeAttribute("src", mathJaxSource);
                currentContext->writeCharacters(""); // This forces a distinct end tag.
                currentContext->writeEndElement();

                success = true;
            } else {
                HtmlCodeGeneratorDiagnostic* diagnostic = new HtmlCodeGeneratorDiagnostic(
                    nullptr,
                    HtmlCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                    dynamic_cast<const HtmlTranslationPhase&>(translationPhase()),
                    HtmlCodeGeneratorDiagnostic::Code::HTML_STYLE_NOT_SUPPORTED,
                    currentContext
                );

                translationErrorDetected(diagnostic);
                success = false;
            }
        } else {
            success = true;
        }

        if (success) {
            currentContext->writeEndElement(); // head tag
        }

        return success;
    }


    bool HtmlCodeGenerationEngine::preBody() {
        bool success;

        if (currentHtmlStyle == HtmlStyle::HTML5_WITH_CSS) {
            currentContext->writeStartElement("body");
            currentContext->writeAttribute("style", "background-color: white;");

            success = true;
        } else if (currentHtmlStyle == HtmlStyle::HTML4_WITHOUT_CSS) {
            currentContext->writeStartElement("body");
            currentContext->writeAttribute("bgcolor", "white");

            success = true;
        } else {
            HtmlCodeGeneratorDiagnostic* diagnostic = new HtmlCodeGeneratorDiagnostic(
                nullptr,
                HtmlCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                dynamic_cast<const HtmlTranslationPhase&>(translationPhase()),
                HtmlCodeGeneratorDiagnostic::Code::HTML_STYLE_NOT_SUPPORTED,
                currentContext
            );

            translationErrorDetected(diagnostic);
            success = false;
        }

        return success;
    }


    bool HtmlCodeGenerationEngine::postBody() {
        currentContext->writeEndElement(); // body tag
        currentContext->writeEndElement(); // html tag

        currentContext->close();

        return true;
    }


    void HtmlCodeGenerationEngine::createExportContext(
            const QString&                      outputFile,
            CodeGeneratorOutputType::ExportMode exportMode
        ) {
        if (exportMode == CodeGeneratorOutputType::ExportMode::EXPORT_IN_MEMORY) {
            currentContext.reset(new XmlMemoryExportContext(outputFile));
        } else if (exportMode == CodeGeneratorOutputType::ExportMode::EXPORT_AS_DIRECTORY) {
            currentContext.reset(new XmlFileExportContext(outputFile, true));
        } else if (exportMode == CodeGeneratorOutputType::ExportMode::EXPORT_AS_MIXED_TEMPORARY_OBJECT) {
            currentContext.reset(new XmlTemporaryFileExportContext(outputFile));
        } else {
            assert(false);
        }

        if (currentContext->hasError()) {
            HtmlCodeGeneratorDiagnostic* diagnostic = new HtmlCodeGeneratorDiagnostic(
                nullptr,
                HtmlCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                HtmlTranslationPhase(),
                HtmlCodeGeneratorDiagnostic::Code::CONTEXT_ERROR,
                currentContext
            );

            translationErrorDetected(diagnostic);
        }
    }
}
