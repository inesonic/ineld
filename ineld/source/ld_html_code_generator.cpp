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
* This file implements the \ref Ld::HtmlCodeGenerator class.
***********************************************************************************************************************/

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
#include "ld_html_code_generator_output_types.h"
#include "ld_code_generation_engine.h"
#include "ld_html_code_generation_engine.h"
#include "ld_code_generator.h"
#include "ld_html_code_generator.h"

namespace Ld {
    const char                         HtmlCodeGenerator::codeGeneratorName[] = "HtmlCodeGenerator";
    const HtmlCodeGenerator::MathMode  HtmlCodeGenerator::defaultMathMode     = HtmlCodeGenerator::MathMode::MATHML;
    const HtmlCodeGenerator::HtmlStyle HtmlCodeGenerator::defaultHtmlStyle    =
        HtmlCodeGenerator::HtmlStyle::HTML5_WITH_CSS;

    const char HtmlCodeGenerator::mathJaxUri[] = "https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.5/latest.js";
    const char HtmlCodeGenerator::mathJaxConfigurationFile[] = "TeX-MML-AM_CHTML";



    HtmlCodeGenerator::HtmlCodeGenerator(CodeGeneratorVisual* visual):CodeGenerator(visual) {
        currentIgnoreMissingPerElementTranslators = false;
        currentMathMode                           = defaultMathMode;
        currentHtmlStyle                          = defaultHtmlStyle;
        currentImageHandlingMode                  = ImageHandlingMode::SEPARATE_PAYLOADS;
        currentProcessImports                     = false;
    }


    HtmlCodeGenerator::~HtmlCodeGenerator() {}


    QString HtmlCodeGenerator::typeName() const {
        return QString(codeGeneratorName);
    }


    bool HtmlCodeGenerator::supportsSelectionsAsInput() const {
        return true;
    }


    QList<CodeGeneratorOutputTypeContainer> HtmlCodeGenerator::supportedOutputTypes() const {
        QList<CodeGeneratorOutputTypeContainer> supportedOutputTypes;
        supportedOutputTypes << new HtmlOutputType;

        return supportedOutputTypes;
    }


    QSharedPointer<XmlExportContext> HtmlCodeGenerator::context() const {
        return dynamic_cast<const HtmlCodeGenerationEngine&>(codeGenerationEngine()).contextPointer();
    }


    bool HtmlCodeGenerator::ignoreMissingPerElementTranslators() const {
        return currentIgnoreMissingPerElementTranslators;
    }


    bool HtmlCodeGenerator::reportMissingPerElementTranslators() const {
        return !currentIgnoreMissingPerElementTranslators;
    }


    void HtmlCodeGenerator::setIgnoreMissingPerElementTranslators(bool nowIgnoreMissingPerElementTranslators) {
        currentIgnoreMissingPerElementTranslators = nowIgnoreMissingPerElementTranslators;
    }


    void HtmlCodeGenerator::setReportMissingPerElementTranslators(bool nowReportMissingPerElementTranslators) {
        currentIgnoreMissingPerElementTranslators = !nowReportMissingPerElementTranslators;
    }


    HtmlCodeGenerator::MathMode HtmlCodeGenerator::mathMode() const {
        return currentMathMode;
    }


    void HtmlCodeGenerator::setMathMode(MathMode newMathMode) {
        currentMathMode = newMathMode;
    }


    HtmlCodeGenerator::HtmlStyle HtmlCodeGenerator::htmlStyle() const {
        return currentHtmlStyle;
    }


    void HtmlCodeGenerator::setHtmlStyle(HtmlCodeGenerator::HtmlStyle newHtmlStyle) {
        currentHtmlStyle = newHtmlStyle;
    }


    HtmlCodeGenerator::ImageHandlingMode HtmlCodeGenerator::imageHandlingMode() const {
        return currentImageHandlingMode;
    }


    void HtmlCodeGenerator::setImageHandlingMode(ImageHandlingMode newImageHandlingMode) {
        currentImageHandlingMode = newImageHandlingMode;
    }


    bool HtmlCodeGenerator::processImports() const {
        return currentProcessImports;
    }


    bool HtmlCodeGenerator::processNoImports() const {
        return !currentProcessImports;
    }


    void HtmlCodeGenerator::setProcessImports(bool nowProcessImports) {
        currentProcessImports = nowProcessImports;
    }


    void HtmlCodeGenerator::setProcessNoImports(bool nowProcessNoImports) {
        currentProcessImports = !nowProcessNoImports;
    }


    QSharedPointer<CodeGenerationEngine> HtmlCodeGenerator::createEngine(
            QSharedPointer<RootElement>             rootElement,
            const QString&                          outputFile,
            const CodeGeneratorOutputTypeContainer& outputType,
            CodeGeneratorOutputType::ExportMode     exportMode,
            Ud::UsageData*                          usageData
        ) {
        HtmlCodeGenerationEngine* engine = new HtmlCodeGenerationEngine(
            this,
            rootElement,
            outputFile,
            outputType,
            exportMode,
            usageData,
            currentIgnoreMissingPerElementTranslators,
            currentMathMode,
            currentHtmlStyle,
            currentImageHandlingMode,
            currentProcessImports
        );

        return QSharedPointer<CodeGenerationEngine>(engine);
    }
}
