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
* This file implements the \ref Ld::CodeGenerator class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>
#include <QByteArray>
#include <QList>
#include <QMap>

#include <cassert>

#include <model_status.h>

#include <ud_usage_data.h>

#include "ld_element_structures.h"
#include "ld_element.h"
#include "ld_root_element.h"
#include "ld_code_generator_visual.h"
#include "ld_diagnostic.h"
#include "ld_diagnostic_structures.h"
#include "ld_code_generator_output_type.h"
#include "ld_code_generator_output_type_container.h"
#include "ld_code_generation_engine.h"
#include "ld_translation_phase.h"
#include "ld_translator.h"
#include "ld_code_generator.h"

namespace Ld {
    QHash<QString, QSharedPointer<CodeGenerator>>                          CodeGenerator::currentGenerators;
    QHash<CodeGeneratorOutputTypeContainer, QSharedPointer<CodeGenerator>> CodeGenerator::currentGeneratorsByOutput;

    const QByteArray            CodeGenerator::emptyIntermediateRepresentation;
    const DiagnosticPointerList CodeGenerator::emptyDiagnosticsList;

    CodeGenerator::CodeGenerator(CodeGeneratorVisual* visual) {
        currentVisual = nullptr;
        setVisual(visual);

        currentUsageData = nullptr;

        currentEnabledDiagnosticTypes << Diagnostic::Type::FATAL_ERROR
                                      << Diagnostic::Type::INTERNAL_ERROR
                                      << Diagnostic::Type::RUNTIME_ERROR;
    }


    CodeGenerator::~CodeGenerator() {}


    bool CodeGenerator::registerCodeGenerator(QSharedPointer<CodeGenerator> newCodeGenerator) {
        bool    success;
        QString generatorName = newCodeGenerator->typeName();

        if (currentGenerators.contains(generatorName)) {
            success = false;
        } else {
            currentGenerators.insert(generatorName, newCodeGenerator);

            QList<CodeGeneratorOutputTypeContainer> outputTypes = newCodeGenerator->supportedOutputTypes();

            QList<CodeGeneratorOutputTypeContainer>::iterator it  = outputTypes.begin();
            QList<CodeGeneratorOutputTypeContainer>::iterator end = outputTypes.end();
            while (it != end) {
                currentGeneratorsByOutput.insert(*it, newCodeGenerator);
                ++it;
            }

            success = true;
        }

        return success;
    }


    bool CodeGenerator::registerCodeGenerator(CodeGenerator *newCodeGenerator) {
        bool    success;
        QString generatorName = newCodeGenerator->typeName();

        if (currentGenerators.contains(generatorName)) {
            success = false;
        } else {
            success = registerCodeGenerator(QSharedPointer<CodeGenerator>(newCodeGenerator));
            assert(success);
        }

        return success;
    }


    void CodeGenerator::releaseCodeGenerators() {
        currentGenerators.clear();
        currentGeneratorsByOutput.clear();
    }


    QList<QSharedPointer<CodeGenerator>> CodeGenerator::codeGenerators() {
        return currentGenerators.values();
    }


    QList<QString> CodeGenerator::codeGeneratorNames() {
        return currentGenerators.keys();
    }


    QSharedPointer<CodeGenerator> CodeGenerator::codeGenerator(const QString& generatorName) {
        return currentGenerators.value(generatorName);
    }


    QList<CodeGeneratorOutputTypeContainer> CodeGenerator::allSupportedOutputTypes() {
        return currentGeneratorsByOutput.keys();
    }


    QSharedPointer<CodeGenerator> CodeGenerator::codeGenerator(const CodeGeneratorOutputTypeContainer& outputType) {
        return currentGeneratorsByOutput.value(outputType);
    }


    bool CodeGenerator::supportsSelectionsAsInput() const {
        return false;
    }


    void CodeGenerator::setVisual(CodeGeneratorVisual* newVisual) {
        if (currentVisual != nullptr) {
            currentVisual->currentCodeGenerator = nullptr;
        }

        if (newVisual != nullptr) {
            if (newVisual->currentCodeGenerator != nullptr) {
                newVisual->currentCodeGenerator->currentVisual = nullptr;
            }

            newVisual->currentCodeGenerator = this;
        }

        currentVisual = newVisual;
    }


    CodeGeneratorVisual* CodeGenerator::visual() const {
        return currentVisual;
    }


    void CodeGenerator::setUsageData(Ud::UsageData* usageData) {
        currentUsageData = usageData;
    }


    Ud::UsageData* CodeGenerator::usageData() const {
        return currentUsageData;
    }


    bool CodeGenerator::registerTranslator(QSharedPointer<Translator> translator) {
        bool    success;
        QString elementName = translator->elementName();
        if (currentTranslators.contains(elementName)) {
            success = false;
        } else {
            currentTranslators.insert(elementName, translator);
            success = true;
        }

        return success;
    }


    bool CodeGenerator::registerTranslator(Translator* translator) {
        bool    success;
        QString elementName = translator->elementName();
        if (currentTranslators.contains(elementName)) {
            success = false;
        } else {
            currentTranslators.insert(elementName, QSharedPointer<Translator>(translator));
            success = true;
        }

        return success;
    }


    bool CodeGenerator::registerTranslator(const QString& codeGenerator, QSharedPointer<Translator> translator) {
        bool success;
        QSharedPointer<CodeGenerator> generator = currentGenerators.value(codeGenerator);

        if (generator.isNull()) {
            success = false;
        } else {
            success = generator->registerTranslator(translator);
        }

        return success;
    }


    bool CodeGenerator::registerTranslator(const QString& codeGenerator, Translator* translator) {
        bool success;
        QSharedPointer<CodeGenerator> generator = currentGenerators.value(codeGenerator);

        if (generator.isNull()) {
            success = false;
        } else {
            success = generator->registerTranslator(translator);
        }

        return success;
    }


    QSharedPointer<Translator> CodeGenerator::translator(const QString& elementName) const {
        return currentTranslators.value(elementName);
    }


    bool CodeGenerator::translate(
            QSharedPointer<RootElement>             rootElement,
            const QString&                          outputFile,
            const CodeGeneratorOutputTypeContainer& outputType,
            CodeGeneratorOutputType::ExportMode     exportMode
        ) {
        currentEngine = createEngine(rootElement, outputFile, outputType, exportMode, currentUsageData);
        bool success = currentEngine->translate();

        return success;
    }


    bool CodeGenerator::translate(
            QSharedPointer<RootElement>         rootElement,
            const QString&                      outputFile,
            CodeGeneratorOutputType::ExportMode exportMode
        ) {
        bool success;

        QList<CodeGeneratorOutputTypeContainer> outputTypes = supportedOutputTypes();

        QList<CodeGeneratorOutputTypeContainer>::const_iterator it  = outputTypes.begin();
        QList<CodeGeneratorOutputTypeContainer>::const_iterator end = outputTypes.end();

        while (it != end && !it->defaultOutputType()) {
            ++it;
        }

        if (it != end) {
            success = translate(rootElement, outputFile, *it, exportMode);
        } else {
            success = false;
        }

        return success;
    }


    void CodeGenerator::waitComplete() {
        if (!currentEngine.isNull()) {
            currentEngine->wait();
        }
    }


    bool CodeGenerator::waitComplete(unsigned long maximumTimeInMilliseconds) {
        bool success;

        if (currentEngine.isNull()) {
            success = true;
        } else {
            success = currentEngine->wait(maximumTimeInMilliseconds);
        }

        return success;
    }


    bool CodeGenerator::active() const {
        bool isActive;

        if (currentEngine.isNull()) {
            isActive = false;
        } else {
            isActive = currentEngine->isRunning();
        }

        return isActive;
    }


    void CodeGenerator::abort() {
        if (!currentEngine.isNull()) {
            currentEngine->abort();
        }
    }


    void CodeGenerator::setEnabledDiagnosticTypes(const QSet<Diagnostic::Type>& diagnosticTypes) {
        currentEnabledDiagnosticTypes = diagnosticTypes;
    }


    const QSet<Diagnostic::Type>& CodeGenerator::enabledDiagnosticTypes() const {
        return currentEnabledDiagnosticTypes;
    }


    const DiagnosticPointerList& CodeGenerator::reportedDiagnostics() const {
        return currentEngine.isNull() ? emptyDiagnosticsList : currentEngine->reportedDiagnostics();
    }


    CodeGeneratorOutputTypeContainer CodeGenerator::translationOutputType() const {
        return currentEngine.isNull() ? CodeGeneratorOutputTypeContainer() : currentEngine->outputType();
    }


    bool CodeGenerator::translationAvailable() const {
        return currentEngine.isNull() ? false : currentEngine->translationAvailable();
    }


    const QByteArray& CodeGenerator::intermediateRepresentation() const {
        return emptyIntermediateRepresentation;
    }


    void CodeGenerator::translationStarted(
            QSharedPointer<RootElement>             rootElement,
            const CodeGeneratorOutputTypeContainer& outputType,
            unsigned long                           numberPerElementTranslationSteps
        ) {
        if (currentVisual != nullptr) {
            currentVisual->translationStarted(rootElement, outputType, numberPerElementTranslationSteps);
        }
    }


    void CodeGenerator::translationPhaseStarted(
            QSharedPointer<RootElement>             rootElement,
            const CodeGeneratorOutputTypeContainer& outputType,
            const TranslationPhase&                 phase
        ) {
        if (currentVisual != nullptr) {
            currentVisual->translationPhaseStarted(rootElement, outputType, phase);
        }
    }


    void CodeGenerator::translationStepCompleted(unsigned long numberCompletedSteps) {
        if (currentVisual != nullptr) {
            currentVisual->translationStepCompleted(numberCompletedSteps);
        }
    }


    void CodeGenerator::translationCompleted(
            bool                                    success,
            QSharedPointer<RootElement>             rootElement,
            const CodeGeneratorOutputTypeContainer& outputType
        ) {
        if (currentUsageData != nullptr) {
            if (success) {
                currentUsageData->adjustEvent(QString("%1_Completed").arg(typeName()));
            } else {
                currentUsageData->adjustEvent(QString("%1_Failed").arg(typeName()));
            }
        }

        if (currentVisual != nullptr) {
            currentVisual->translationCompleted(success, rootElement, outputType);
        }
    }


    void CodeGenerator::translationAborted(
            QSharedPointer<RootElement>             rootElement,
            const CodeGeneratorOutputTypeContainer& outputType
        ) {
        if (currentUsageData != nullptr) {
            currentUsageData->adjustEvent(QString("%1_Aborted").arg(typeName()));
        }

        if (currentVisual != nullptr) {
            currentVisual->translationAborted(rootElement, outputType);
        }
    }


    void CodeGenerator::translationErrorDetected(DiagnosticPointer diagnostic) {
        if (currentVisual != nullptr) {
            currentVisual->translationErrorDetected(diagnostic);
        }

        ElementPointer element = diagnostic->element();
        if (!element.isNull()) {
            element->flagDiagnostic(diagnostic);
        }
    }


    CodeGenerationEngine& CodeGenerator::codeGenerationEngine() {
        return *currentEngine;
    }


    const CodeGenerationEngine& CodeGenerator::codeGenerationEngine() const {
        return *currentEngine;
    }
}
