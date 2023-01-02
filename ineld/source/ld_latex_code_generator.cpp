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
* This file implements the \ref Ld::LaTeXCodeGenerator class.
***********************************************************************************************************************/

#include <QCoreApplication> // For QT_TR_NOOP
#include <QString>
#include <QSharedPointer>
#include <QList>

#include <cassert>

#include "ld_element_structures.h"
#include "ld_element.h"
#include "ld_root_element.h"
#include "ld_code_generator_visual.h"
#include "ld_diagnostic.h"
#include "ld_code_generator_output_type.h"
#include "ld_code_generator_output_type_container.h"
#include "ld_latex_code_generator_output_types.h"
#include "ld_code_generation_engine.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_code_generator.h"
#include "ld_latex_code_generator.h"

namespace Ld {
    const char LaTeXCodeGenerator::codeGeneratorName[] = "LaTeXCodeGenerator";

    const LaTeXCodeGenerator::ImageMode LaTeXCodeGenerator::defaultImageMode = LaTeXCodeGenerator::ImageMode::FORCE_PNG;

    const QString LaTeXCodeGenerator::latexPreambleFilename = QT_TR_NOOP("preamble.tex");
    const QString LaTeXCodeGenerator::latexBodyFilename     = QT_TR_NOOP("content.tex");
    const QString LaTeXCodeGenerator::latexTopFilename      = QT_TR_NOOP("document.tex");

    LaTeXCodeGenerator::LaTeXCodeGenerator(CodeGeneratorVisual* visual):CodeGenerator(visual) {
        currentIgnoreMissingPerElementTranslators = false;
        currentImageMode                          = defaultImageMode;
        currentProcessImports                     = false;
        currentSingleFile                         = false;
        currentIncludeCopyright                   = true;
        currentUnicodeTranslationMode             = UnicodeMode::CONVERT_TO_LATEX_COMMAND;
    }


    LaTeXCodeGenerator::~LaTeXCodeGenerator() {}


    QString LaTeXCodeGenerator::typeName() const {
        return QString(codeGeneratorName);
    }


    bool LaTeXCodeGenerator::supportsSelectionsAsInput() const {
        return true;
    }


    QList<CodeGeneratorOutputTypeContainer> LaTeXCodeGenerator::supportedOutputTypes() const {
        QList<CodeGeneratorOutputTypeContainer> supportedOutputTypes;
        supportedOutputTypes << new LaTeXOutputType;

        return supportedOutputTypes;
    }


    QSharedPointer<TextExportContext> LaTeXCodeGenerator::context() const {
        return dynamic_cast<const LaTeXCodeGenerationEngine&>(codeGenerationEngine()).contextPointer();
    }


    bool LaTeXCodeGenerator::ignoreMissingPerElementTranslators() const {
        return currentIgnoreMissingPerElementTranslators;
    }


    bool LaTeXCodeGenerator::reportMissingPerElementTranslators() const {
        return !currentIgnoreMissingPerElementTranslators;
    }


    void LaTeXCodeGenerator::setIgnoreMissingPerElementTranslators(bool nowIgnoreMissingPerElementTranslators) {
        currentIgnoreMissingPerElementTranslators = nowIgnoreMissingPerElementTranslators;
    }


    void LaTeXCodeGenerator::setReportMissingPerElementTranslators(bool nowReportMissingPerElementTranslators) {
        currentIgnoreMissingPerElementTranslators = !nowReportMissingPerElementTranslators;
    }


    LaTeXCodeGenerator::ImageMode LaTeXCodeGenerator::imageMode() const {
        return currentImageMode;
    }


    void LaTeXCodeGenerator::setImageMode(ImageMode newImageMode) {
        currentImageMode = newImageMode;
    }


    bool LaTeXCodeGenerator::singleFile() const {
        return currentSingleFile;
    }


    bool LaTeXCodeGenerator::multipleFiles() const {
        return !currentSingleFile;
    }


    void LaTeXCodeGenerator::setSingleFile(bool nowSingleFile) {
        currentSingleFile = nowSingleFile;
    }


    void LaTeXCodeGenerator::setMultipleFiles(bool nowMultipleFiles) {
        currentSingleFile = !nowMultipleFiles;
    }


    bool LaTeXCodeGenerator::includeCopyright() const {
        return currentIncludeCopyright;
    }


    bool LaTeXCodeGenerator::excludeCopyright() const {
        return !currentIncludeCopyright;
    }


    void LaTeXCodeGenerator::setCopyrightIncluded(bool nowIncludeCopyright) {
        currentIncludeCopyright = nowIncludeCopyright;
    }


    void LaTeXCodeGenerator::setCopyrightExcluded(bool nowExcludeCopyright) {
        currentIncludeCopyright = !nowExcludeCopyright;
    }


    LaTeXCodeGenerator::UnicodeMode LaTeXCodeGenerator::unicodeTranslationMode() const {
        return currentUnicodeTranslationMode;
    }


    void LaTeXCodeGenerator::setUnicodeTranslationMode(LaTeXCodeGenerator::UnicodeMode newUnicodeTranslationMode) {
        currentUnicodeTranslationMode = newUnicodeTranslationMode;
    }


    bool LaTeXCodeGenerator::processImports() const {
        return currentProcessImports;
    }


    bool LaTeXCodeGenerator::processNoImports() const {
        return !currentProcessImports;
    }


    void LaTeXCodeGenerator::setProcessImports(bool nowProcessImports) {
        currentProcessImports = nowProcessImports;
    }


    void LaTeXCodeGenerator::setProcessNoImports(bool nowProcessNoImports) {
        currentProcessImports = !nowProcessNoImports;
    }


    QSharedPointer<CodeGenerationEngine> LaTeXCodeGenerator::createEngine(
            QSharedPointer<RootElement>             rootElement,
            const QString&                          outputFile,
            const CodeGeneratorOutputTypeContainer& outputType,
            CodeGeneratorOutputType::ExportMode     exportMode,
            Ud::UsageData*                          usageData
        ) {
        LaTeXCodeGenerationEngine* engine = new LaTeXCodeGenerationEngine(
            this,
            rootElement,
            outputFile,
            outputType,
            exportMode,
            usageData,
            currentSingleFile,
            currentIncludeCopyright,
            currentUnicodeTranslationMode,
            currentIgnoreMissingPerElementTranslators,
            currentImageMode,
            currentProcessImports
        );

        return QSharedPointer<CodeGenerationEngine>(engine);
    }
}
