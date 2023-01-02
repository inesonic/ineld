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
* This file implements the \ref Ld::CodeGenerationEngine class.
***********************************************************************************************************************/

#include <QString>
#include <QList>
#include <QSet>

#include <cassert>

#include <ud_usage_data.h>

#include <model_status.h>

#include "ld_element_structures.h"
#include "ld_diagnostic.h"
#include "ld_diagnostic_structures.h"
#include "ld_root_element.h"
#include "ld_root_import.h"
#include "ld_code_generator_output_type.h"
#include "ld_code_generator_output_type_container.h"
#include "ld_translation_phase.h"
#include "ld_translator.h"
#include "ld_code_generator.h"
#include "ld_code_generation_engine.h"

namespace Ld {
    CodeGenerationEngine::CodeGenerationEngine(
            CodeGenerator*                          codeGenerator,
            QSharedPointer<RootElement>             rootElement,
            const QString&                          outputFile,
            const CodeGeneratorOutputTypeContainer& outputType,
            CodeGeneratorOutputType::ExportMode     exportMode,
            Ud::UsageData*                          usageData
        ) {
        abortRequested              = false;
        currentOutputFile           = outputFile;
        currentGenerator            = codeGenerator;
        currentRootElement          = rootElement;
        currentTranslationPhase     = nullptr;
        currentOutputType           = outputType;
        currentUsageData            = usageData;
        currentTranslationAvailable = false;

        if (exportMode == CodeGeneratorOutputType::ExportMode::DEFAULT) {
            currentExportMode = outputType.defaultExportMode();
        } else {
            currentExportMode = exportMode;
        }
    }


    CodeGenerationEngine::~CodeGenerationEngine() {
        if (currentTranslationPhase != nullptr) {
            delete currentTranslationPhase;
        }
    }


    CodeGenerator& CodeGenerationEngine::codeGenerator() {
        return *currentGenerator;
    }


    const CodeGenerator& CodeGenerationEngine::codeGenerator() const {
        return *currentGenerator;
    }


    QString CodeGenerationEngine::outputFile() const {
        return currentOutputFile;
    }


    QSharedPointer<RootElement> CodeGenerationEngine::rootElement() const {
        return currentRootElement;
    }


    Ud::UsageData* CodeGenerationEngine::usageData() const {
        return currentUsageData;
    }


    const TranslationPhase& CodeGenerationEngine::translationPhase() const {
        return *currentTranslationPhase;
    }


    const CodeGeneratorOutputTypeContainer& CodeGenerationEngine::outputType() const {
        return currentOutputType;
    }


    CodeGeneratorOutputType::ExportMode CodeGenerationEngine::exportMode() const {
        return currentExportMode;
    }


    bool CodeGenerationEngine::translate() {
        bool success;

        if (isRunning()) {
            success = false;
        } else {
            start();
            success = true;
        }

        return success;
    }


    void CodeGenerationEngine::abort() {
        abortRequested = true;
        wait();
    }


    const DiagnosticPointerList& CodeGenerationEngine::reportedDiagnostics() const {
        return currentReportedDiagnostics;
    }


    bool CodeGenerationEngine::translationAvailable() const {
        return currentTranslationAvailable;
    }


    QSharedPointer<Translator> CodeGenerationEngine::translator(const QString& elementName) const {
        return currentGenerator->translator(elementName);
    }


    bool CodeGenerationEngine::translateChild(ElementPointer element) {
        return translateElement(element);
    }


    void CodeGenerationEngine::translationErrorDetected(DiagnosticPointer diagnostic) {
        currentReportedDiagnostics.append(diagnostic);
        currentGenerator->translationErrorDetected(diagnostic);
    }


    void CodeGenerationEngine::translationErrorDetected(Diagnostic* diagnostic) {
        translationErrorDetected(DiagnosticPointer(diagnostic));
    }


    void CodeGenerationEngine::translationStepCompleted() {
        ++numberTranslationStepsCompleted;
        translationStepCompleted(numberTranslationStepsCompleted);
    }


    void CodeGenerationEngine::run() {
        bool success = true;

        assert(currentTranslationPhase == nullptr);
        currentTranslationPhase = createTranslationPhase();

        RootElement::RootElementList           rootElements            = rootElementsToProcess();
        RootElement::RootElementList::iterator rootElementsIterator    = rootElements.begin();
        RootElement::RootElementList::iterator rootElementsEndIterator = rootElements.end();

        unsigned long numberElements = 0;
        for (  RootElement::RootElementList::iterator it  = rootElementsIterator, end = rootElementsEndIterator
             ; it != end
             ; ++it
            ) {
            QSharedPointer<RootElement> root = *it;
            numberElements += static_cast<unsigned long>(root->descendants().size()) + 1;
        }

        unsigned long numberTranslationSteps = numberPerElementTranslationStepsToPerform(numberElements);

        translationStarted(numberTranslationSteps);
        numberTranslationStepsCompleted = 0;

        while (rootElementsIterator != rootElementsEndIterator) {
            clearErrorsUnderRootElement(*rootElementsIterator);
            ++rootElementsIterator;
        }

        clearErrorsUnderRootElement(currentRootElement);

        bool done = false;
        do {
            translationPhaseStarted();

            success = preTranslate();

            rootElementsIterator    = rootElements.begin();
            rootElementsEndIterator = rootElements.end();

            while (!abortRequested && rootElementsIterator != rootElementsEndIterator) {
                success = translateRootElement(*rootElementsIterator) && success;
                ++rootElementsIterator;
            }

            if (success && !abortRequested) {
                success = postTranslate();
            }

            if (success) {
                translationStepCompleted();
            }

            done = translationPhase().lastPhase();
            advanceToNextPhase();
        } while (success && !abortRequested && !done);

        if (success) {
            currentTranslationAvailable = true;
        }

        if (abortRequested) {
            translationAborted();
        } else {
            translationCompleted(success);
        }
    }


    void CodeGenerationEngine::setTranslationAvailable(bool nowTranslationAvailable) {
        currentTranslationAvailable = nowTranslationAvailable;
    }


    void CodeGenerationEngine::advanceToNextPhase() {
        assert(currentTranslationPhase != nullptr);
        currentTranslationPhase->nextPhase();
    }


    bool CodeGenerationEngine::translateElement(ElementPointer element) {
        bool success;

        if (!abortRequested && !element.isNull()) {
            QString                    elementName       = element->typeName();
            QSharedPointer<Translator> elementTranslator = translator(elementName);

            if (elementTranslator.isNull()) {
                if (missingTranslator(element)) {
                    success = false;
                } else {
                    success = true;
                }
            } else {
                success = elementTranslator->translate(element, *this);
            }

            if (success) {
                translationStepCompleted();
            }
        } else {
            success = true;
        }

        return success;
    }


    void CodeGenerationEngine::translationStarted(unsigned long numberPerElementTranslationSteps) {
        currentGenerator->translationStarted(currentRootElement, currentOutputType, numberPerElementTranslationSteps);
    }


    void CodeGenerationEngine::translationPhaseStarted() {
        currentGenerator->translationPhaseStarted(currentRootElement, currentOutputType, *currentTranslationPhase);
    }


    void CodeGenerationEngine::translationStepCompleted(unsigned long numberCompletedSteps) {
        currentGenerator->translationStepCompleted(numberCompletedSteps);
    }


    void CodeGenerationEngine::translationCompleted(bool success) {
        currentGenerator->translationCompleted(success, currentRootElement, currentOutputType);
    }


    void CodeGenerationEngine::translationAborted() {
        currentGenerator->translationAborted(currentRootElement, currentOutputType);
    }


    void CodeGenerationEngine::clearErrorsUnderRootElement(QSharedPointer<RootElement> rootElement) {
        rootElement->clearDiagnostic();

        RootElement::ElementIterator elementIterator    = rootElement->elementBegin();
        RootElement::ElementIterator elementEndIterator = rootElement->elementEnd();

        while (elementIterator != elementEndIterator) {
            ElementPointer element = *elementIterator;
            if (!element.isNull()) {
                element->clearDiagnostic();
            }

            ++elementIterator;
        }
    }


    bool CodeGenerationEngine::translateRootElement(QSharedPointer<RootElement> rootElement) {
        bool success;

        TranslationPhase::TranslationMode translationMode = translationPhase().currentTranslationMode();

        if (translationMode == TranslationPhase::TranslationMode::HONOR_HEIRARCHY) {
            success = translateChild(rootElement);
        } else if (translationMode == TranslationPhase::TranslationMode::IGNORE_HEIRARCHY) {
            success = true;

            RootElement::ElementIterator elementIterator    = rootElement->elementBegin();
            RootElement::ElementIterator elementEndIterator = rootElement->elementEnd();

            while (!abortRequested && elementIterator != elementEndIterator) {
                success = translateElement(*elementIterator) && success;
                ++elementIterator;
            }

            if (success) {
                translationStepCompleted();
            }
        } else {
            assert(translationMode == TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION);
            success = true;
        }

        return success;
    }


    RootElement::RootElementList CodeGenerationEngine::rootElementsToProcess() {
        RootElement::RootElementList buildList =   includeRootImports()
                                                 ? currentRootElement->allDependencies()
                                                 : RootElement::RootElementList();
        buildList << currentRootElement;

        return buildList;
    }


    unsigned long CodeGenerationEngine::numberPerElementTranslationStepsToPerform(unsigned long numberElements) const {
        TranslationPhase* translationPhase = createTranslationPhase();

        unsigned          numberIgnoreHeirarchyPhases         = 0;
        unsigned          numberHonorHeirarchyPhases          = 0;
        unsigned          numberNoPerElementTranslationPhases = 0;
        bool              done;

        do {
            TranslationPhase::TranslationMode mode = translationPhase->currentTranslationMode();
            switch (mode) {
                case TranslationPhase::TranslationMode::HONOR_HEIRARCHY: {
                    ++numberHonorHeirarchyPhases;
                    break;
                }

                case TranslationPhase::TranslationMode::IGNORE_HEIRARCHY: {
                    ++numberIgnoreHeirarchyPhases;
                    break;
                }

                case TranslationPhase::TranslationMode::NO_PER_ELEMENT_TRANSLATION: {
                    ++numberNoPerElementTranslationPhases;
                    break;
                }

                default: {
                    assert(false);
                    break;
                }
            }

            done = translationPhase->lastPhase();
            translationPhase->nextPhase();
        } while (!done);

        delete translationPhase;

        unsigned long result = (numberIgnoreHeirarchyPhases + numberHonorHeirarchyPhases) * numberElements;
        result += numberIgnoreHeirarchyPhases + numberHonorHeirarchyPhases + numberNoPerElementTranslationPhases;

        return result;
    }
}
