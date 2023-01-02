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
* This file implements the \ref Ld::LaTeXCodeGenerationEngine class.
***********************************************************************************************************************/

#include <QCoreApplication> // For QCoreApplication::applicationName and Q_DECLARE_TR_FUNCTIONS
#include <QString>
#include <QByteArray>
#include <QBuffer>
#include <QSharedPointer>
#include <QDate>

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
#include "ld_text_file_export_context.h"
#include "ld_text_memory_export_context.h"
#include "ld_latex_code_generator_output_types.h"
#include "ld_latex_code_generator_diagnostic.h"
#include "ld_latex_translation_phase.h"
#include "ld_latex_code_generator.h"
#include "ld_code_generation_engine.h"
#include "ld_latex_code_generation_engine.h"

namespace Ld {
    LaTeXCodeGenerationEngine::LaTeXCodeGenerationEngine(
            CodeGenerator*                          codeGenerator,
            QSharedPointer<RootElement>             rootElement,
            const QString&                          outputFile,
            const CodeGeneratorOutputTypeContainer& outputType,
            CodeGeneratorOutputType::ExportMode     exportMode,
            Ud::UsageData*                          usageData,
            bool                                    singleFile,
            bool                                    includeCopyright,
            LaTeXCodeGenerationEngine::UnicodeMode  unicodeTranslationMode,
            bool                                    ignoreMissingTranslators,
            LaTeXCodeGenerationEngine::ImageMode    imageMode,
            bool                                    includeImports
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
        currentImageMode                          = imageMode;
        currentSingleFile                         = singleFile;
        currentIncludeCopyright                   = includeCopyright;
        currentUnicodeTranslationMode             = unicodeTranslationMode;
        currentIncludeImports                     = includeImports;
        currentMathModeNesting                    = 0;
        currentInlineMode                         = false;
    }


    LaTeXCodeGenerationEngine::~LaTeXCodeGenerationEngine() {}


    LaTeXCodeGenerationEngine::ImageMode LaTeXCodeGenerationEngine::imageMode() const {
        return currentImageMode;
    }


    bool LaTeXCodeGenerationEngine::singleFile() const {
        return currentSingleFile;
    }


    bool LaTeXCodeGenerationEngine::includeCopyright() const {
        return currentIncludeCopyright;
    }


    LaTeXCodeGenerationEngine::UnicodeMode LaTeXCodeGenerationEngine::unicodeTranslationMode() const {
        return currentUnicodeTranslationMode;
    }


    TextExportContext& LaTeXCodeGenerationEngine::context() {
        return *currentContext;
    }


    const TextExportContext& LaTeXCodeGenerationEngine::context() const {
        return *currentContext;
    }


    QSharedPointer<TextExportContext> LaTeXCodeGenerationEngine::contextPointer() const {
        return currentContext;
    }


    bool LaTeXCodeGenerationEngine::addPreamble(const QString& commandSequence, int relativeOrder) {
        bool success;

        if (!currentPreambleData.contains(relativeOrder)) {
            QSet<QString> commandSequences;
            commandSequences << commandSequence;

            currentPreambleData.insert(relativeOrder, commandSequences);
            success = true;
        } else {
            QSet<QString>& commandSequences = currentPreambleData[relativeOrder];
            if (!commandSequences.contains(commandSequence)) {
                commandSequences.insert(commandSequence);
                success = true;
            } else {
                success = false;
            }
        }

        return success;
    }


    bool LaTeXCodeGenerationEngine::addPackage(
            const QString&        packageName,
            const QList<QString>& options,
            int                   relativeOrdering
        ) {
        QString commandSequence("\\usepackage");

        for (  QList<QString>::const_iterator optionIterator    = options.constBegin(),
                                              optionEndIterator = options.constEnd()
             ; optionIterator != optionEndIterator
             ; ++optionIterator
            ) {
            commandSequence += QString("[%1]").arg(*optionIterator);
        }

        commandSequence += QString("{%1}").arg(packageName);

        return addPreamble(commandSequence, relativeOrdering);
    }


    bool LaTeXCodeGenerationEngine::addPackage(const QString& packageName, int relativeOrdering) {
        return addPackage(packageName, QList<QString>(), relativeOrdering);
    }


    bool LaTeXCodeGenerationEngine::addPackage(
            const QString& packageName,
            const QString& option,
            int            relativeOrdering
        ) {
        QList<QString> options;
        options << option;

        return addPackage(packageName, options, relativeOrdering);
    }


    bool LaTeXCodeGenerationEngine::addPackage(
            const QString& packageName,
            const QString& option1,
            const QString& option2,
            int            relativeOrdering
        ) {
        QList<QString> options;
        options << option1 << option2;

        return addPackage(packageName, options, relativeOrdering);
    }


    bool LaTeXCodeGenerationEngine::addPackage(
            const QString& packageName,
            const QString& option1,
            const QString& option2,
            const QString& option3,
            int            relativeOrdering
        ) {
        QList<QString> options;
        options << option1 << option2 << option3;

        return addPackage(packageName, options, relativeOrdering);
    }


    bool LaTeXCodeGenerationEngine::addPackage(
            const QString& packageName,
            const QString& option1,
            const QString& option2,
            const QString& option3,
            const QString& option4,
            int            relativeOrdering
        ) {
        QList<QString> options;
        options << option1 << option2 << option3 << option4;

        return addPackage(packageName, options, relativeOrdering);
    }


    void LaTeXCodeGenerationEngine::enterMathMode(bool inlineMode) {
        if (currentMathModeNesting == 0) {
            if (inlineMode) {
                if (currentContext->columnNumber() > 0) {
                    (*currentContext) << " $ ";
                } else {
                    (*currentContext) << "$ ";
                }
            } else {
                if (currentContext->columnNumber() > 0) {
                    (*currentContext) << "\n\n";
                }

                (*currentContext) << "\\begin{equation*}\n";
            }

            currentInlineMode = inlineMode;
        }

        ++currentMathModeNesting;
    }


    void LaTeXCodeGenerationEngine::exitMathMode() {
        --currentMathModeNesting;
        if (currentMathModeNesting == 0) {
            if (currentInlineMode) {
                (*currentContext) << " $";
            } else {
                if (currentContext->columnNumber() > 0) {
                    (*currentContext) << "\n";
                }

                (*currentContext) << "\\end{equation*}\n\n";
            }
        }
    }


    bool LaTeXCodeGenerationEngine::useInlineMathMode(ElementPointer element) {
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


    TranslationPhase* LaTeXCodeGenerationEngine::createTranslationPhase() const {
        return new LaTeXTranslationPhase;
    }


    bool LaTeXCodeGenerationEngine::includeRootImports() const {
        return currentIncludeImports;
    }


    bool LaTeXCodeGenerationEngine::preTranslate() {
        bool success;
        const LaTeXTranslationPhase&
            latexTranslationPhase = dynamic_cast<const LaTeXTranslationPhase&>(translationPhase());

        switch (latexTranslationPhase.phase()) {
            case LaTeXTranslationPhase::Phase::IDENTIFY_DEPENDENCIES: {
                success = preIdentifyDependencies();
                break;
            }

            case LaTeXTranslationPhase::Phase::PREAMBLE: {
                success = prePreamble();
                break;
            }

            case LaTeXTranslationPhase::Phase::BODY: {
                success = preBody();
                break;
            }

            default: {
                LaTeXCodeGeneratorDiagnostic* diagnostic = new LaTeXCodeGeneratorDiagnostic(
                    nullptr,
                    LaTeXCodeGeneratorDiagnostic::Type::INTERNAL_ERROR,
                    latexTranslationPhase,
                    LaTeXCodeGeneratorDiagnostic::Code::UNEXPECTED_TRANSLATION_PHASE,
                    currentContext
                );

                translationErrorDetected(diagnostic);
                success = false;

                break;
            }
        }

        return success;
    }


    bool LaTeXCodeGenerationEngine::postTranslate() {
        bool success;
        const LaTeXTranslationPhase&
            latexTranslationPhase = dynamic_cast<const LaTeXTranslationPhase&>(translationPhase());

        switch (latexTranslationPhase.phase()) {
            case LaTeXTranslationPhase::Phase::IDENTIFY_DEPENDENCIES: {
                success = postIdentifyDependencies();
                break;
            }

            case LaTeXTranslationPhase::Phase::PREAMBLE: {
                success = postPreamble();
                break;
            }

            case LaTeXTranslationPhase::Phase::BODY: {
                success = postBody();
                break;
            }

            default: {
                LaTeXCodeGeneratorDiagnostic* diagnostic = new LaTeXCodeGeneratorDiagnostic(
                    nullptr,
                    LaTeXCodeGeneratorDiagnostic::Type::INTERNAL_ERROR,
                    latexTranslationPhase,
                    LaTeXCodeGeneratorDiagnostic::Code::UNEXPECTED_TRANSLATION_PHASE,
                    currentContext
                );

                translationErrorDetected(diagnostic);
                success = false;

                break;
            }
        }

        return success;
    }


    bool LaTeXCodeGenerationEngine::missingTranslator(ElementPointer element) {
        bool result;

        if (currentIgnoreMissingPerElementTranslators) {
            result = false;
        } else {
            LaTeXCodeGeneratorDiagnostic* diagnostic = new LaTeXCodeGeneratorDiagnostic(
                element,
                LaTeXCodeGeneratorDiagnostic::Type::INTERNAL_ERROR,
                dynamic_cast<const LaTeXTranslationPhase&>(translationPhase()),
                LaTeXCodeGeneratorDiagnostic::Code::MISSING_TRANSLATOR,
                currentContext
            );

            translationErrorDetected(diagnostic);
            result = true;
        }

        return result;
    }


    bool LaTeXCodeGenerationEngine::preIdentifyDependencies() {
        currentPreambleData.clear();

        addPreamble("\\documentclass[letterpaper, 10pt]{article}", std::numeric_limits<int>::min());

        addPreamble(
            "\\usepackage{ifxetex}\n"
            "\\usepackage{ifluatex}\n"
            "\n"
            "\\ifxetex\n"
            "  \\usepackage{fontspec}\n"
            "\\else \\ifluatex\n"
            "  \\usepackage{fontspec}\n"
            "\\else\n"
            "  \\usepackage[T1]{fontenc}\n"
            "\\fi \\fi",
            minimumPackageRelativeOrdering
        );

        addPackage("amsmath", minimumPackageRelativeOrdering + 1);
        addPackage("amssymb", minimumPackageRelativeOrdering + 1);

        return true;
    }


    bool LaTeXCodeGenerationEngine::postIdentifyDependencies() {
        return true;
    }


    bool LaTeXCodeGenerationEngine::prePreamble() {
        bool success;

        QString preamble = generateCopyright() + generatePreamble();

        if (currentSingleFile) {
            *currentContext << preamble;
            success = true;
        } else {
            QString filename = currentContext->addPayload(
                LaTeXCodeGenerator::latexPreambleFilename,
                preamble.toLatin1()
            );

            success = (!filename.isEmpty());
        }

        return success;
    }


    bool LaTeXCodeGenerationEngine::postPreamble() {
        return true;
    }


    bool LaTeXCodeGenerationEngine::preBody() {
        if (currentSingleFile) {
            *currentContext << QString("\\begin{document}\n");
        } else {
            *currentContext << generateCopyright();
        }

        return true;
    }


    bool LaTeXCodeGenerationEngine::postBody() {
        bool success;

        if (currentSingleFile) {
            *currentContext << QString("\\end{document}\n");
            success = true;
        } else {
            QString topData = generateCopyright();

            topData += QString("\\input{%1}\n").arg(LaTeXCodeGenerator::latexPreambleFilename);
            topData += QString("\\begin{document}\n");
            topData += QString("  \\input{%1}\n").arg(latexBodyFilename);
            topData += QString("\\end{document}\n");

            QString filename = currentContext->addPayload(LaTeXCodeGenerator::latexTopFilename, topData.toLatin1());
            success = (!filename.isEmpty());
        }

        currentContext->close();

        return success;
    }


    void LaTeXCodeGenerationEngine::createExportContext(
            const QString&                      outputFile,
            CodeGeneratorOutputType::ExportMode exportMode
        ) {
        QFileInfo latexBodyFileInformation(outputFile);
        latexBodyFilename = latexBodyFileInformation.fileName();

        switch (exportMode) {
            case CodeGeneratorOutputType::ExportMode::EXPORT_IN_MEMORY: {
                currentContext.reset(new TextMemoryExportContext(outputFile));
                break;
            }

            case CodeGeneratorOutputType::ExportMode::EXPORT_AS_DIRECTORY: {
                currentContext.reset(new TextFileExportContext(outputFile, true));
                break;
            }

            default: {
                assert(false);
                break;
            }
        }
    }


    QString LaTeXCodeGenerationEngine::generateCopyright() const {
        QString  result;

        if (currentIncludeCopyright) {
            QDate    currentDate = QDate::currentDate();
            unsigned currentYear = static_cast<unsigned>(currentDate.year());
            result = tr(
                "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
                "% Copyright %1 Inesonic, LLC.\n"
                "% Use under the terms of the %2 Software EULA.\n"
                "%\n"
                "\n"
            ).arg(currentYear).arg(QCoreApplication::applicationName());
        }

        return result;
    }


    QString LaTeXCodeGenerationEngine::generatePreamble() const {
        QString result;

        for (  QMap<int, QSet<QString>>::const_iterator preambleOrderIterator    = currentPreambleData.constBegin(),
                                                        preambleOrderEndIterator = currentPreambleData.constEnd()
             ; preambleOrderIterator != preambleOrderEndIterator
             ; ++preambleOrderIterator
            ) {
            const QSet<QString>& preambles = preambleOrderIterator.value();
            for (  QSet<QString>::const_iterator preambleIterator    = preambles.constBegin(),
                                                 preambleEndIterator = preambles.constEnd()
                 ; preambleIterator != preambleEndIterator
                 ; ++preambleIterator
                ) {
                const QString& preamble = *preambleIterator;
                result += preamble + "\n";
            }

            result += "\n";
        }

        return result;
    }
}
