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
* This file implements tests for the \ref Ld::CppCodeGenerator class and several other associated classes such as
* the \ref Ld::CodeGenerator class, \ref Ld::CodeGenerationEngine class, \ref Ld::Translator,
* \ref Ld::CppTranslator, and \ref Ld::CppCodeGenerationEngine classes.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QCoreApplication>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QtTest/QtTest>

#include <iostream>

#include <mkl_api.h>

#include <model_api.h>
#include <model_status.h>
#include <model_rng.h>

#include <cbe_loader_notifier.h>
#include <cbe_dynamic_library_loader.h>

#include <ld_element_structures.h>
#include <ld_diagnostic_structures.h>
#include <ld_environment.h>
#include <ld_configure.h>
#include <ld_code_generator.h>
#include <ld_cpp_code_generator.h>
#include <ld_cpp_translator.h>
#include <ld_cpp_code_generation_engine.h>
#include <ld_cpp_context.h>
#include <ld_code_generator_visual.h>
#include <ld_code_generator_output_type_container.h>
#include <ld_cpp_translation_phase.h>
#include <ld_cpp_root_element_translator.h>
#include <ld_diagnostic.h>
#include <ld_cpp_code_generator_diagnostic.h>
#include <ld_assignment_operator_element.h>
#include <ld_addition_operator_element.h>
#include <ld_subtraction_operator_element.h>
#include <ld_multiplication_operator_element.h>
#include <ld_division_operator_element.h>
#include <ld_element_of_set_operator_element.h>
#include <ld_integer_type_element.h>
#include <ld_real_type_element.h>
#include <ld_complex_type_element.h>
#include <ld_variable_element.h>
#include <ld_literal_element.h>
#include <ld_division_operator_element.h>
#include <ld_character_format.h>
#include <ld_operator_format.h>
#include <ld_multiplication_operator_format.h>
#include <ld_division_operator_format.h>
#include <ld_capabilities.h>
#include <ld_element_with_fixed_children.h>
#include <ld_root_element.h>

#include "test_cpp_code_generator.h"

/***********************************************************************************************************************
 * CodeGeneratorVisual:
 */

class CodeGeneratorVisual:public Ld::CodeGeneratorVisual {
    public:
        CodeGeneratorVisual();

        ~CodeGeneratorVisual() override;

        void reset();

        bool unexpectedCall() const;

        bool rootElementChanged() const;

        bool outputTypeChanged() const;

        bool unexpectedTranslationPhase() const;

        bool translationStartedCalled() const;

        bool translationPhaseStartedCalled() const;

        bool translationCompletedCalled() const;

        bool translationAbortedCalled() const;

        bool successful() const;

        QSharedPointer<Ld::RootElement> reportedRootElement() const;

        Ld::CodeGeneratorOutputTypeContainer reportedOutputType() const;

        Ld::CppTranslationPhase reportedTranslationPhase() const;

        Ld::DiagnosticPointer reportedDiagnostic() const;

        void translationStarted(
            QSharedPointer<Ld::RootElement>             rootElement,
            const Ld::CodeGeneratorOutputTypeContainer& outputType,
            unsigned long                               numberPerElementTranslationSteps
        ) final;

        void translationPhaseStarted(
            QSharedPointer<Ld::RootElement>             rootElement,
            const Ld::CodeGeneratorOutputTypeContainer& outputType,
            const Ld::TranslationPhase&                 phase
        ) final;

        void translationStepCompleted(unsigned long numberCompletedSteps) final;

        void translationCompleted(
            bool                                              success,
            QSharedPointer<Ld::RootElement>             rootElement,
            const Ld::CodeGeneratorOutputTypeContainer& outputType
        ) final;

        void translationAborted(
            QSharedPointer<Ld::RootElement>             rootElement,
            const Ld::CodeGeneratorOutputTypeContainer& outputType
        ) final;

        void translationErrorDetected(Ld::DiagnosticPointer diagnostic) final;

    private:
        bool                                 unexpectedCallOccurred;
        bool                                 rootElementChangedUnexpectedly;
        bool                                 outputTypeChangedUnexpectedly;
        bool                                 unexpectedTranslationPhaseDetected;
        bool                                 wasSuccessful;
        bool                                 translationStartedWasCalled;
        bool                                 translationPhaseStartedWasCalled;
        bool                                 translationCompletedWasCalled;
        bool                                 translationAbortedWasCalled;
        QWeakPointer<Ld::RootElement>        lastReportedRootElement;
        Ld::CodeGeneratorOutputTypeContainer lastReportedOutputType;
        Ld::CppTranslationPhase              lastReportedTranslationPhase;
        Ld::CppTranslationPhase              nextExpectedTranslationPhase;
        Ld::DiagnosticPointer                lastReportedDiagnostic;
};


CodeGeneratorVisual::CodeGeneratorVisual() {
    reset();
}


CodeGeneratorVisual::~CodeGeneratorVisual() {}


void CodeGeneratorVisual::reset() {
    unexpectedCallOccurred             = false;
    rootElementChangedUnexpectedly     = false;
    outputTypeChangedUnexpectedly      = false;
    unexpectedTranslationPhaseDetected = false;
    wasSuccessful                      = false;
    translationStartedWasCalled        = false;
    translationPhaseStartedWasCalled   = false;
    translationCompletedWasCalled      = false;
    translationAbortedWasCalled        = false;
    lastReportedRootElement            = nullptr;
    lastReportedOutputType             = Ld::CodeGeneratorOutputTypeContainer();
    lastReportedTranslationPhase       = Ld::CppTranslationPhase();
    nextExpectedTranslationPhase       = Ld::CppTranslationPhase();

    lastReportedDiagnostic.reset();
}


bool CodeGeneratorVisual::unexpectedCall() const {
    return unexpectedCallOccurred;
}


bool CodeGeneratorVisual::rootElementChanged() const {
    return rootElementChangedUnexpectedly;
}


bool CodeGeneratorVisual::outputTypeChanged() const {
    return outputTypeChangedUnexpectedly;
}


bool CodeGeneratorVisual::unexpectedTranslationPhase() const {
    return unexpectedTranslationPhaseDetected;
}


bool CodeGeneratorVisual::successful() const {
    return wasSuccessful;
}


bool CodeGeneratorVisual::translationStartedCalled() const {
    return translationStartedWasCalled;
}


bool CodeGeneratorVisual::translationPhaseStartedCalled() const {
    return translationPhaseStartedWasCalled;
}


bool CodeGeneratorVisual::translationCompletedCalled() const {
    return translationCompletedWasCalled;
}


bool CodeGeneratorVisual::translationAbortedCalled() const {
    return translationAbortedWasCalled;
}


QSharedPointer<Ld::RootElement> CodeGeneratorVisual::reportedRootElement() const {
    return lastReportedRootElement.toStrongRef();
}


Ld::CodeGeneratorOutputTypeContainer CodeGeneratorVisual::reportedOutputType() const {
    return lastReportedOutputType;
}


Ld::CppTranslationPhase CodeGeneratorVisual::reportedTranslationPhase() const {
    return lastReportedTranslationPhase;
}


Ld::DiagnosticPointer CodeGeneratorVisual::reportedDiagnostic() const {
    return lastReportedDiagnostic;
}


void CodeGeneratorVisual::translationStarted(
        QSharedPointer<Ld::RootElement>             rootElement,
        const Ld::CodeGeneratorOutputTypeContainer& outputType,
        unsigned long                               /* numberPerElementTranslationSteps */
        ) {
    if (translationStartedWasCalled      ||
        translationPhaseStartedWasCalled ||
        translationCompletedWasCalled    ||
        translationAbortedWasCalled         ) {
        unexpectedCallOccurred = true;
    }

    if (!lastReportedRootElement) {
        lastReportedRootElement = rootElement;
        lastReportedOutputType  = Ld::CodeGeneratorOutputTypeContainer(outputType);
    }

    translationStartedWasCalled = true;
}


void CodeGeneratorVisual::translationPhaseStarted(
        QSharedPointer<Ld::RootElement>             rootElement,
        const Ld::CodeGeneratorOutputTypeContainer& outputType,
        const Ld::TranslationPhase&                 phase
    ) {
    if (!translationStartedWasCalled || translationCompletedWasCalled || translationAbortedWasCalled) {
        unexpectedCallOccurred = true;
    }

    if (lastReportedRootElement != rootElement) {
        rootElementChangedUnexpectedly = true;
    }

    if (lastReportedOutputType != outputType) {
        outputTypeChangedUnexpectedly = true;
    }

    lastReportedTranslationPhase = dynamic_cast<const Ld::CppTranslationPhase&>(phase);

    if (!translationPhaseStartedWasCalled) {
        nextExpectedTranslationPhase = lastReportedTranslationPhase;
        if (   nextExpectedTranslationPhase.phase()
            != Ld::CppTranslationPhase::Phase::IDENTIFY_DEPENDENCIES_AND_EXPLICIT_TYPES) {
            unexpectedTranslationPhaseDetected = true;
        }

        translationPhaseStartedWasCalled = true;
    } else {
        nextExpectedTranslationPhase.nextPhase();
        if (nextExpectedTranslationPhase != lastReportedTranslationPhase) {
            unexpectedTranslationPhaseDetected = true;
        }
    }
}


void CodeGeneratorVisual::translationStepCompleted(unsigned long /* numberCompletedSteps */) {}


void CodeGeneratorVisual::translationCompleted(
        bool                                              success,
        QSharedPointer<Ld::RootElement>             rootElement,
        const Ld::CodeGeneratorOutputTypeContainer& outputType
    ) {
    wasSuccessful = success;

    if (!translationStartedWasCalled      ||
        !translationPhaseStartedWasCalled ||
        translationCompletedWasCalled     ||
        translationAbortedWasCalled          ) {
        unexpectedCallOccurred = true;
    }

    if (lastReportedRootElement != rootElement) {
        rootElementChangedUnexpectedly = true;
    }

    if (lastReportedOutputType != outputType) {
        outputTypeChangedUnexpectedly = true;
    }

    translationCompletedWasCalled = true;
}


void CodeGeneratorVisual::translationAborted(
        QSharedPointer<Ld::RootElement>             rootElement,
        const Ld::CodeGeneratorOutputTypeContainer& outputType
    ) {
    if (!translationStartedWasCalled      ||
        !translationPhaseStartedWasCalled ||
        translationCompletedWasCalled     ||
        translationAbortedWasCalled          ) {
        unexpectedCallOccurred = true;
    }

    if (lastReportedRootElement != rootElement) {
        rootElementChangedUnexpectedly = true;
    }

    if (lastReportedOutputType != outputType) {
        outputTypeChangedUnexpectedly = true;
    }

    translationAbortedWasCalled = true;
}


void CodeGeneratorVisual::translationErrorDetected(Ld::DiagnosticPointer diagnostic) {
    QSharedPointer<Ld::CppCodeGeneratorDiagnostic>
        cppDiagnostic = diagnostic.dynamicCast<Ld::CppCodeGeneratorDiagnostic>();

    std::cout << "Error:" << std::endl
              << "    Message: " << cppDiagnostic->diagnosticMessage().toStdString() << std::endl
              << "    Phase:   " << cppDiagnostic->translationPhase().currentPhaseName().toStdString() << std::endl
              << "    Line:    " << cppDiagnostic->lineNumber()
              << "    Column:  " << cppDiagnostic->columnNumber() << std::endl
              << "    Backend: " << cppDiagnostic->backendMessage().toStdString() << std::endl;

    lastReportedDiagnostic = diagnostic;
}

/***********************************************************************************************************************
 * Loader Notifier:
 */

class LoaderNotifier:public Cbe::LoaderNotifier {
    public:
        LoaderNotifier();

        ~LoaderNotifier() final;

        void libraryLoaded(const QString& libraryFilename) final;

        void libraryLoadFailed(const QString& libraryFilename, const QString& errorReason) final;

        void aboutToUnload(const QString& libraryFilename) final;

        void libraryUnloaded(const QString& libraryFilename) final;

        void libraryUnloadFailed(const QString& libraryFilename) final;
};

LoaderNotifier::LoaderNotifier() {}


LoaderNotifier::~LoaderNotifier() {}


void LoaderNotifier::libraryLoaded(const QString& libraryFilename) {
    (void) libraryFilename;
}


void LoaderNotifier::libraryLoadFailed(const QString& libraryFilename, const QString& errorReason) {
    (void) libraryFilename;
    (void) errorReason;
}


void LoaderNotifier::aboutToUnload(const QString& libraryFilename) {
    (void) libraryFilename;
}


void LoaderNotifier::libraryUnloaded(const QString& libraryFilename) {
    (void) libraryFilename;
}


void LoaderNotifier::libraryUnloadFailed(const QString& libraryFilename) {
    (void) libraryFilename;
}

/***********************************************************************************************************************
 * ModelStatus:
 */

class TestModelStatus:public Model::Status {
    public:
        TestModelStatus();

        ~TestModelStatus() override;

        void started(Model::Api* modelApi) override;

        void finished(Model::Api* modelApi) override;

        void aborted(
            Model::Api*            modelApi,
            Model::AbortReason     abortReason,
            Model::OperationHandle operationHandle
        ) override;

        void pausedOnUserRequest(Model::Api* modelApi, Model::OperationHandle operationHandle) final;

        void pausedAtOperation(Model::Api* modelApi, Model::OperationHandle operationHandle) final;

        void pausedOnVariableUpdate(
            Model::Api*             modelApi,
            Model::OperationHandle  operationHandle,
            Model::IdentifierHandle identifierHandle
        ) final;

        void resumed(Model::Api* modelApi) override;

        void threadStarted(Model::Api* modelApi, unsigned threadId) override;

        void threadFinished(Model::Api* modelApi, unsigned threadId) override;

        void threadAborted(Model::Api* modelApi, unsigned threadId) override;
};


TestModelStatus::TestModelStatus() {}


TestModelStatus::~TestModelStatus() {}


void TestModelStatus::started(Model::Api*) {}


void TestModelStatus::finished(Model::Api*) {}


void TestModelStatus::aborted(Model::Api*, Model::AbortReason, Model::OperationHandle) {}


void TestModelStatus::pausedOnUserRequest(Model::Api*, Model::OperationHandle) {}


void TestModelStatus::pausedAtOperation(Model::Api*, Model::OperationHandle) {}


void TestModelStatus::pausedOnVariableUpdate(Model::Api*, Model::OperationHandle, Model::IdentifierHandle) {}


void TestModelStatus::resumed(Model::Api*) {}


void TestModelStatus::threadStarted(Model::Api*, unsigned) {}


void TestModelStatus::threadFinished(Model::Api*, unsigned) {}


void TestModelStatus::threadAborted(Model::Api*, unsigned) {}

/***********************************************************************************************************************
 * TestCppCodeGenerator:
 */

TestCppCodeGenerator::TestCppCodeGenerator() {}

TestCppCodeGenerator::~TestCppCodeGenerator() {}


void TestCppCodeGenerator::initTestCase() {
    QString executableFilePath = QCoreApplication::applicationFilePath();
    Ld::Environment::configure(executableFilePath, Ld::Environment::Type::TEST_DEVELOPMENT, false);

    Ld::CodeGenerator::releaseCodeGenerators();
    Ld::Configure::configure(0x123456789ABCDEF0ULL, nullptr);

    QSharedPointer<Ld::CppCodeGenerator>
        generator = Ld::CodeGenerator::codeGenerator(Ld::CppCodeGenerator::codeGeneratorName)
                    .dynamicCast<Ld::CppCodeGenerator>();

    CodeGeneratorVisual* visual = new CodeGeneratorVisual;
    generator->setVisual(visual);

//    generator->setExecutableDirectory(Ld::Environment::supportExecutableDirectory());
//    generator->setHeaderSearchPaths(Ld::Environment::headerSearchPaths());

//    generator->setStandardHeaders(Ld::Environment::standardHeaderFiles());
//    generator->setStandardPchFiles(Ld::Environment::standardPchFiles());

//    generator->setSystemRoot(Ld::Environment::compilerRoot());
//    generator->setResourceDirectory(Ld::Environment::compilerResourceDirectory());
//    generator->setLibrarySearchPaths(Ld::Environment::librarySearchPaths());
//    generator->

//    generator->setStaticLibraries(Ld::Environment::standardStaticLibraries());
//    generator->setDynamicLibraries(Ld::Environment::standardDynamicLibraries());

//    generator->setRunTimeSearchPaths(Ld::Environment::runTimeLibrarySearchPaths());
}


void TestCppCodeGenerator::testBasicFunctionality() {
    QSharedPointer<Ld::RootElement> rootElement(new Ld::RootElement);
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<Ld::CharacterFormat>
        characterFormat = Ld::Format::create(Ld::CharacterFormat::formatName).dynamicCast<Ld::CharacterFormat>();

    QSharedPointer<Ld::OperatorFormat>
        operatorFormat = Ld::Format::create(Ld::OperatorFormat::formatName).dynamicCast<Ld::OperatorFormat>();


    QSharedPointer<Ld::RealTypeElement> realElement = Ld::Element::create(Ld::RealTypeElement::elementName)
                                                      .dynamicCast<Ld::RealTypeElement>();
    realElement->setFormat(characterFormat);


    QSharedPointer<Ld::VariableElement> aVariable = Ld::Element::create(Ld::VariableElement::elementName)
                                                    .dynamicCast<Ld::VariableElement>();
    aVariable->setText(QString("a"));
    aVariable->setFormat(characterFormat);

    QSharedPointer<Ld::ElementOfSetOperatorElement>
        aTypeElementOfSet = Ld::Element::create(Ld::ElementOfSetOperatorElement::elementName)
                            .dynamicCast<Ld::ElementOfSetOperatorElement>();
    aTypeElementOfSet->setFormat(characterFormat);
    rootElement->append(aTypeElementOfSet, nullptr);

    aTypeElementOfSet->setChild(0, aVariable, nullptr);
    aTypeElementOfSet->setChild(1, realElement, nullptr);


    realElement = Ld::Element::create(Ld::RealTypeElement::elementName).dynamicCast<Ld::RealTypeElement>();
    realElement->setFormat(characterFormat);

    QSharedPointer<Ld::VariableElement> bVariable = Ld::Element::create(Ld::VariableElement::elementName)
                                                    .dynamicCast<Ld::VariableElement>();
    bVariable->setText(QString("b"));
    bVariable->setFormat(characterFormat);

    QSharedPointer<Ld::ElementOfSetOperatorElement>
        bTypeElementOfSet = Ld::Element::create(Ld::ElementOfSetOperatorElement::elementName)
                            .dynamicCast<Ld::ElementOfSetOperatorElement>();
    bTypeElementOfSet->setFormat(characterFormat);
    rootElement->append(bTypeElementOfSet, nullptr);

    bTypeElementOfSet->setChild(0, bVariable, nullptr);
    bTypeElementOfSet->setChild(1, realElement, nullptr);


    realElement = Ld::Element::create(Ld::RealTypeElement::elementName).dynamicCast<Ld::RealTypeElement>();
    realElement->setFormat(characterFormat);

    QSharedPointer<Ld::VariableElement> cVariable = Ld::Element::create(Ld::VariableElement::elementName)
                                                    .dynamicCast<Ld::VariableElement>();
    cVariable->setText(QString("c"));
    cVariable->setFormat(characterFormat);

    QSharedPointer<Ld::ElementOfSetOperatorElement>
        cTypeElementOfSet = Ld::Element::create(Ld::ElementOfSetOperatorElement::elementName)
                            .dynamicCast<Ld::ElementOfSetOperatorElement>();
    cTypeElementOfSet->setFormat(characterFormat);
    rootElement->append(cTypeElementOfSet, nullptr);

    cTypeElementOfSet->setChild(0, cVariable, nullptr);
    cTypeElementOfSet->setChild(1, realElement, nullptr);


    aVariable = Ld::Element::create(Ld::VariableElement::elementName).dynamicCast<Ld::VariableElement>();
    aVariable->setText(QString("a"));
    aVariable->setFormat(characterFormat);

    QSharedPointer<Ld::LiteralElement> aLiteral = Ld::Element::create(Ld::LiteralElement::elementName)
                                                  .dynamicCast<Ld::LiteralElement>();
    aLiteral->setFormat(operatorFormat);
    aLiteral->setText(QString("1.5"));

    QSharedPointer<Ld::AssignmentOperatorElement>
        aAssignment = Ld::Element::create(Ld::AssignmentOperatorElement::elementName)
                      .dynamicCast<Ld::AssignmentOperatorElement>();
    aAssignment->setFormat(characterFormat);

    aAssignment->setChild(0, aVariable, nullptr);
    aAssignment->setChild(1, aLiteral, nullptr);
    rootElement->append(aAssignment, nullptr);


    bVariable = Ld::Element::create(Ld::VariableElement::elementName).dynamicCast<Ld::VariableElement>();
    bVariable->setText(QString("b"));
    bVariable->setFormat(characterFormat);

    QSharedPointer<Ld::LiteralElement> bLiteral = Ld::Element::create(Ld::LiteralElement::elementName)
                                                  .dynamicCast<Ld::LiteralElement>();
    bLiteral->setFormat(operatorFormat);
    bLiteral->setText(QString("2.5"));

    QSharedPointer<Ld::AssignmentOperatorElement>
        bAssignment = Ld::Element::create(Ld::AssignmentOperatorElement::elementName)
                      .dynamicCast<Ld::AssignmentOperatorElement>();
    bAssignment->setFormat(characterFormat);

    bAssignment->setChild(0, bVariable, nullptr);
    bAssignment->setChild(1, bLiteral, nullptr);
    rootElement->append(bAssignment, nullptr);


    aVariable = Ld::Element::create(Ld::VariableElement::elementName).dynamicCast<Ld::VariableElement>();
    aVariable->setText(QString("a"));
    aVariable->setFormat(characterFormat);

    bVariable = Ld::Element::create(Ld::VariableElement::elementName).dynamicCast<Ld::VariableElement>();
    bVariable->setText(QString("b"));
    bVariable->setFormat(characterFormat);

    QSharedPointer<Ld::AdditionOperatorElement>
        additionOperator = Ld::Element::create(Ld::AdditionOperatorElement::elementName)
                           .dynamicCast<Ld::AdditionOperatorElement>();
    additionOperator->setFormat(operatorFormat);

    additionOperator->setChild(0, aVariable, nullptr);
    additionOperator->setChild(1, bVariable, nullptr);


    cVariable = Ld::Element::create(Ld::VariableElement::elementName).dynamicCast<Ld::VariableElement>();
    cVariable->setText(QString("c"));
    cVariable->setFormat(characterFormat);

    QSharedPointer<Ld::AssignmentOperatorElement>
        cAssignment = Ld::Element::create(Ld::AssignmentOperatorElement::elementName)
                      .dynamicCast<Ld::AssignmentOperatorElement>();
    cAssignment->setFormat(characterFormat);

    cAssignment->setChild(0, cVariable, nullptr);
    cAssignment->setChild(1, additionOperator, nullptr);
    rootElement->append(cAssignment, nullptr);


    QSharedPointer<Ld::CodeGenerator>
        generator = Ld::CodeGenerator::codeGenerator(Ld::CppCodeGenerator::codeGeneratorName);

    CodeGeneratorVisual* visual = dynamic_cast<CodeGeneratorVisual*>(generator->visual());
    QVERIFY(visual != nullptr);

    visual->reset();

    #if (defined(Q_OS_WIN))

        QString libraryFileBasename = "test.dll";

    #elif (defined(Q_OS_LINUX))

        QString libraryFileBasename = "test.so";

    #elif (defined(Q_OS_DARWIN))

        QString libraryFileBasename = "test.dylib";

    #else

        #error Unknown platform.

    #endif

    QFileInfo libraryFileInfo(libraryFileBasename);
    QString   libraryFile = libraryFileInfo.absoluteFilePath();

    generator->translate(rootElement, libraryFile);
    generator->waitComplete();

    QString ir = QString(generator->intermediateRepresentation());
    std::cout << ir.toStdString() << std::endl;
    std::cout.flush();

    QVERIFY(visual->translationCompletedCalled());
    QVERIFY(visual->unexpectedCall() == false);
    QVERIFY(visual->unexpectedTranslationPhase() == false);
    QVERIFY(visual->rootElementChanged() == false);
    QVERIFY(visual->outputTypeChanged() == false);
    QVERIFY(visual->successful() == true);
    QVERIFY(visual->reportedDiagnostic().isNull());

    TestModelStatus statusInstance;

    LoaderNotifier loaderNotifier;
    Cbe::DynamicLibraryLoader loader(&loaderNotifier);

    bool success = loader.load(libraryFile);
    QVERIFY(success);

    Model::AllocatorFunction allocatorFunction = reinterpret_cast<Model::AllocatorFunction>(
        loader.resolve(Model::allocatorFunctionName)
    );

    QVERIFY(allocatorFunction != Q_NULLPTR);

    Model::DeallocatorFunction deallocatorFunction = reinterpret_cast<Model::DeallocatorFunction>(
        loader.resolve(Model::deallocatorFunctionName)
    );

    QVERIFY(deallocatorFunction != Q_NULLPTR);

    Model::Api* model = (*allocatorFunction)(&matApi);
    QVERIFY(model != Q_NULLPTR);

    Model::Rng::RngType rngType = Model::Rng::RngType::MT19937;
    Model::Rng::RngSeed rngSeed;

    model->run(rngType, rngSeed, &statusInstance);

    unsigned long numberOperationHandles  = model->numberOperationHandles();

    QCOMPARE(numberOperationHandles, 0U); // FIXME

//  ********** FIX COMMENTED OUT CODE BELOW ***********

//    Model::IdentifierData cIdentifier;
//    QList<Model::IdentifierData> identifiers;
//    for (unsigned handle=0 ; handle<numberIdentifierHandles ; ++handle) {
//        Model::IdentifierData identifierData = model->identifierData(handle);
//        identifiers.append(identifierData);

//        if (QString(identifierData.text1()) == "c") {
//            cIdentifier = identifierData;
//        }
//    }

//    QVERIFY(cIdentifier.isValid());
//    Model::Real c = cIdentifier.value().toReal(&success);

//    QCOMPARE(success, true);
//    QCOMPARE(c, 4.0L);

    (*deallocatorFunction)(model);
}


void TestCppCodeGenerator::testDiagnosticHandling() {

}


void TestCppCodeGenerator::cleanupTestCase() {
    QSharedPointer<Ld::CodeGenerator> generator = Ld::CodeGenerator::codeGenerator("CppCodeGenerator");
    delete generator->visual();

    Ld::CodeGenerator::releaseCodeGenerators();
}
