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
* This header defines the \ref Ld::CodeGenerator class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CODE_GENERATOR_H
#define LD_CODE_GENERATOR_H

#include <QString>
#include <QSharedPointer>
#include <QByteArray>
#include <QList>
#include <QHash>
#include <QThread>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_diagnostic_structures.h"
#include "ld_diagnostic.h"
#include "ld_code_generator_output_type.h"
#include "ld_code_generator_output_type_container.h"
#include "ld_translation_phase.h"

namespace Model {
    class Status;
};

namespace Ud {
    class UsageData;
};

namespace Ld {
    class RootElement;
    class CodeGeneratorVisual;
    class CodeGenerationEngine;
    class Translator;

    /**
     * Pure virtual base class for code generators that convert the syntax tree from the \ref Ld::RootElement downward
     * into an intermediate representation such as an AST or LaTeX source file.  The class can also convert the
     * intermediate representation into a final representation such as DLL, dynamic library, shared object, PDF, or DVI
     * file.
     *
     * This class provides mechanisms to define the available translation phases, to invoke each phase, and to obtain
     * the output from each phase of the translation.
     *
     * The class also provides a collection of method you can use to register different types of code generators
     * allowing plug-ins to be created that provide application specific code generators.
     */
    class LD_PUBLIC_API CodeGenerator {
        friend class CodeGeneratorVisual;

        public:
            /**
             * Constructor
             *
             * \param[in] visual The visual that can be used to obtain feedback from the code generator.
             */
            CodeGenerator(CodeGeneratorVisual* visual = nullptr);

            virtual ~CodeGenerator();

            /**
             * Method you can call to register a code generator with the system.
             *
             * \param[in] newCodeGenerator A shared pointer to the code generator.
             *
             * \return Returns true on success, returns false if the code generator is already registered.
             */
            static bool registerCodeGenerator(QSharedPointer<CodeGenerator> newCodeGenerator);

            /**
             * Method you can call to register a code generator with the system.
             *
             * \param[in] newCodeGenerator A pointer to the new code generator.  Note that this method will take
             *                             ownership of the generator.
             *
             * \return Returns true on success, returns false on error.  You must delete the pointer to the generator if
             *         an error is detected.
             */
            static bool registerCodeGenerator(CodeGenerator* newCodeGenerator);

            /**
             * Method you can call to release storage used by all the code generators.
             */
            static void releaseCodeGenerators();

            /**
             * Method you can use to obtain a list of all the code generators.
             *
             * \return Returns a list of all the shared pointers to all the supported code generators.
             */
            static QList<QSharedPointer<CodeGenerator>> codeGenerators();

            /**
             * Method you can use to obtain a list of all the code generators, by name.
             *
             * \return Returns a list of names for all the code generators.
             */
            static QList<QString> codeGeneratorNames();

            /**
             * Method you can use to obtain a code generator, by name.
             *
             * \param[in] generatorName The name of the desired code generator.
             *
             * \return Returns a shared pointer to the requested code generator.  A null pointer is returned if the
             *         requested code generator does not exist.
             */
            static QSharedPointer<CodeGenerator> codeGenerator(const QString& generatorName);

            /**
             * Method you can use to obtain a list of all the supported output types across all registered generators.
             *
             * \return Returns a list of all the supported output types.
             */
            static QList<CodeGeneratorOutputTypeContainer> allSupportedOutputTypes();

            /**
             * Method you can use to obtain a code generator, by output type.
             *
             * \param[in] outputType The desired output type.
             *
             * \return Returns a shared pointer to the requested code generator.  A null pointer is returned if the
             *         requested code generator does not exist.
             */
            static QSharedPointer<CodeGenerator> codeGenerator(const CodeGeneratorOutputTypeContainer& outputType);

            /**
             * Method you should overload to specify the name of the code generator.
             *
             * \return Returns the name of the code generator.
             */
            virtual QString typeName() const = 0;

            /**
             * Method you should overload to indicate if this code generator supports selections as an input type.
             *
             * \return Returns true if the generator supports selections as an input.  Returns false if the code
             *         generator does not support selections as an input.  This version always returns true.
             */
            virtual bool supportsSelectionsAsInput() const;

            /**
             * Method you can use to set the visual used by the code generator to report status.  Note that using this
             * method will cause the visual to also point back to this code generator, creating a 1-to-1 relationship
             * between the two objects.
             *
             * If the newly added \ref CodeGeneratorVisual already points to a different code generator then those
             * connections will be broken in order to maintain the 1-to-1 relationship between the two objects.
             *
             * If the code generator currently has an associated visual, then that visual will be disconnected from this
             * code generator.  The disconnected visual will *not* be deleted.
             *
             * \param[in] newVisual The new visual.
             */
            void setVisual(CodeGeneratorVisual* newVisual);

            /**
             * Method you can use to obtain a pointer to the underlying visual.
             *
             * \return Returns a pointer to the visual.
             */
            CodeGeneratorVisual* visual() const;

            /**
             * Method that returns a map listing the supported output types for this class.
             *
             * \return Returns a map of the supported output types.
             */
            virtual QList<CodeGeneratorOutputTypeContainer> supportedOutputTypes() const = 0;

            /**
             * Method you can use to set a usage data instance that will be used by this code generator.
             *
             * \param[in] usageData The usage data instance.
             */
            void setUsageData(Ud::UsageData* usageData);

            /**
             * Method you can use to get the usage data instance used by this code generator.
             *
             * \return Returns a pointer to the usage data instance used by this code generator.  A null pointer is
             *         returned if no usage data instance has been assigned.
             */
            Ud::UsageData* usageData() const;

            /**
             * Method you can call to register a translator with this code generator.
             *
             * \param[in] translator A shared pointer to the translator instance.
             *
             * \return Returns true on success, returns false on error.
             */
            bool registerTranslator(QSharedPointer<Translator> translator);

            /**
             * Method you can call to register a translator with this code generator.
             *
             * \param[in] translator A pointer to the translator instance.  On success, this class will take ownership
             *                       of the object.  You are required to destroy this object on failure.
             *
             * \return Returns true on success, returns false on error.
             */
            bool registerTranslator(Translator* translator);

            /**
             * Method you can call to register a translator with a code generator.
             *
             * \param[in] codeGenerator The name of the code generator.
             *
             * \param[in] translator    A shared pointer to the translator instance.
             *
             * \return Returns true on success, returns false on error.
             */
            static bool registerTranslator(const QString& codeGenerator, QSharedPointer<Translator> translator);

            /**
             * Method you can call to register a translator with this code generator.
             *
             * \param[in] codeGenerator The name of the code generator.
             *
             * \param[in] translator    A pointer to the translator instance.  On success, this class will take
             *                          ownership of the object.  You are required to destroy this object on failure.
             *
             * \return Returns true on success, returns false on error.
             */
            static bool registerTranslator(const QString& codeGenerator, Translator* translator);

            /**
             * Method you can use to obtain a translator, by name.
             *
             * \param[in] elementName The name of the element we want a translator for.
             *
             * \return Returns a shared pointer to the desired translator.  A null pointer is returned if no translator
             *         is available.
             */
            QSharedPointer<Translator> translator(const QString& elementName) const;

            /**
             * Method you can use to start translation.  Note that the translation process will run in a separate
             * thread.  You can overload the following methods to obtain results:
             *
             *     * \ref Ld::CodeGenerator::translationStarted
             *     * \ref Ld::CodeGenerator::translationCompleted
             *     * \ref Ld::CodeGenerator::translationErrorDetected
             *
             * \param[in] rootElement The root element you should use to start translation.
             *
             * \param[in] outputFile  The name of the translated output file.  If the output type is not exportable,
             *                        then the translation will be deleted by this class either when a new translation
             *                        is requested or when the class reaches end of scope.
             *
             * \param[in] outputType  A value used to indicate the type of output to be generated.  The meaning of
             *                        this value is dependent on the type of generator.
             *
             * \param[in] exportMode  A value indicating the export mode to be used.
             *
             * \return Returns true if the translation can be started.  Returns false if either a translation is in
             *         progress or a translation is being executed.
             */
            bool translate(
                QSharedPointer<RootElement>             rootElement,
                const QString&                          outputFile,
                const CodeGeneratorOutputTypeContainer& outputType,
                CodeGeneratorOutputType::ExportMode     exportMode = CodeGeneratorOutputType::ExportMode::DEFAULT
            );

            /**
             * Method you can use to start translation of the default output type.
             *
             * \param[in] rootElement The root element you should use to start translation.
             *
             * \param[in] outputFile  The name of the translated output file.
             *
             * \param[in] exportMode  A value indicating the export mode to be used.
             *
             * \return Returns true on success.  Returns false on failure.
             */
            bool translate(
                QSharedPointer<RootElement>         rootElement,
                const QString&                      outputFile,
                CodeGeneratorOutputType::ExportMode exportMode = CodeGeneratorOutputType::ExportMode::DEFAULT
            );

            /**
             * Method that can be called to stall this thread until the translation process completes.
             */
            void waitComplete();

            /**
             * Method that can be called to stall this thread until the translation process completes.
             *
             * \param[in] maximumTimeInMilliseconds The maximum time to wait, in milliseconds.
             *
             * \return Returns true on success, returns false if the translation timed out.
             */
            bool waitComplete(unsigned long maximumTimeInMilliseconds);

            /**
             * Method you can call to determine if the code generator is running in the background.
             *
             * \return Returns true if the code generator is active.  Returns false if the code generator is not active.
             */
            bool active() const;

            /**
             * Method you can call to abort the code generator.  The method will block until the code generator has
             * aborted.  This will also abort an executing translation.
             */
            void abort();

            /**
             * Method you can use to set the enabled diagnostic types.  By default, only errors and fatal errors are
             * enabled.
             *
             * \param[in] diagnosticTypes The set of enabled diagnostic types.
             */
            void setEnabledDiagnosticTypes(const QSet<Diagnostic::Type>& diagnosticTypes);

            /**
             * Method you can use to obtain the currently enabled diagnostic types.
             *
             * \return Returns the set of enabled diagnostic types.
             */
            const QSet<Diagnostic::Type>& enabledDiagnosticTypes() const;

            /**
             * Method you can use to obtain a list of reported diagnostics.
             *
             * \return Returns a list of reported diagnostics from the last translation.  The returned reference will
             *         remain valid until the next translation operation.
             */
            const DiagnosticPointerList& reportedDiagnostics() const;

            /**
             * Method you can use to determine the output type for the last translation that was performed.
             *
             * \return Returns the output type of the last translation performed.
             */
            CodeGeneratorOutputTypeContainer translationOutputType() const;

            /**
             * Method you can use to determine if a translation is ready for export.
             *
             * \param[in] outputType A value used to indicate the type of output to be generated.  The meaning of this
             *                       value is dependent on the type of generator.
             *
             * \return Returns true if an exectable image is available for execution.
             */
            bool translationAvailable() const;

            /**
             * Pure virtual method you can use to obtain the last generated intermediate representation.
             *
             * \return Returns the translated intermediate representation.   The default implementation returns an empty
             *         byte array.
             */
            virtual const QByteArray& intermediateRepresentation() const;

            /**
             * Method that is called when the code generator starts a translation.  Reports the start of translation to
             * the visual tied to the code generator.
             *
             * \param[in] rootElement                      The root element that is used as the basis for the
             *                                             translation.  A null pointer indicates we're processing a
             *                                             selection.
             *
             * \param[in] outputType                       A value used to indicate the type of output being generated.
             *                                             The meaning of this value is dependent on the type of
             *                                             generator.
             *
             * \param[in] numberPerElementTranslationSteps The total number of per-element translation steps that the
             *                                             code generator will need to perform.
             */
            virtual void translationStarted(
                QSharedPointer<RootElement>             rootElement,
                const CodeGeneratorOutputTypeContainer& outputType,
                unsigned long                           numberPerElementTranslationSteps
            );

            /**
             * Method that is called when the code generator begins a new translation phase.  Reports the phase change
             * to the visual tied to the code generator.
             *
             * \param[in] rootElement The root element you should use to start translation.  A null pointer indicates
             *                        we're processing a selection.
             *
             * \param[in] outputType  A value used to indicate the type of output being generated.  The meaning of this
             *                        value is dependent on the type of generator.
             *
             * \param[in] phase       The translation phase being started.
             */
            virtual void translationPhaseStarted(
                QSharedPointer<RootElement>             rootElement,
                const CodeGeneratorOutputTypeContainer& outputType,
                const TranslationPhase&                 phase
            );

            /**
             * Method that is called each time the code generator completes a translation step.  This method reports
             * the step's completion to the visual tied to the code generator.
             *
             * \param[in] numberCompletedSteps The current number of completed steps.
             */
            virtual void translationStepCompleted(unsigned long numberCompletedSteps);

            /**
             * Method that is called when the code generator completes a translation process.  Reports that the
             * translation has completed to the associated visual.
             *
             * \param[in] success     Holds true if the translation operation completed without error.
             *
             * \param[in] rootElement The root element that is used as the basis for the translation.  A null pointer
             *                        indicates we're processing a selection.
             *
             * \param[in] outputType  A value used to indicate the type of output being generated.  The meaning of this
             *                        value is dependent on the type of generator.
             */
            virtual void translationCompleted(
                bool                                    success,
                QSharedPointer<RootElement>             rootElement,
                const CodeGeneratorOutputTypeContainer& outputType
            );

            /**
             * Method that is called when the code generator is aborted during a translation.  Reports that the
             * translation has been aborted to the associated visual.
             *
             * \param[in] rootElement The root element that is used as the basis for the translation.  A null pointer
             *                        indicates we're processing a selection.
             *
             * \param[in] outputType  A value used to indicate the type of output being generated.  The meaning of this
             *                        value is dependent on the type of generator.
             */
            virtual void translationAborted(
                QSharedPointer<RootElement>             rootElement,
                const CodeGeneratorOutputTypeContainer& outputType
            );

            /**
             * Method that is called each time the code generator detects an error condition.  This method may be
             * triggered multiple times during a translation and may be triggered from a different thread from that used
             * to invoke the code generator.  Reports the error to the visual.
             *
             * \param[in] diagnostic The diagnostic tied to the error, warning, etc.
             */
            virtual void translationErrorDetected(DiagnosticPointer diagnostic);

        protected:
            /**
             * Method that should be overloaded in derived classes to create the correct code generation engine.
             *
             * \param[in] rootElement The root element to translate.
             *
             * \param[in] outputFile  The name of the output file to be created.
             *
             * \param[in] outputType  The output type to generate code for.
             *
             * \param[in] exportMode  A value indicating the export mode to be used.
             *
             * \param[in] usageData   The usage data instance.
             *
             * \return Returns a shared pointer to the code generation engine to be used.
             */
            virtual QSharedPointer<CodeGenerationEngine> createEngine(
                QSharedPointer<RootElement>             rootElement,
                const QString&                          outputFile,
                const CodeGeneratorOutputTypeContainer& outputType,
                CodeGeneratorOutputType::ExportMode     exportMode,
                Ud::UsageData*                          usageData
            ) = 0;

            /**
             * Method you can use to obtain access to the current code generation engine.
             *
             * \return Returns a reference to the underlying code generation engine.
             */
            CodeGenerationEngine& codeGenerationEngine();

            /**
             * Method you can use to obtain access to the current code generation engine.
             *
             * \return Returns a reference to the underlying code generation engine.
             */
            const CodeGenerationEngine& codeGenerationEngine() const;

        private:
            /**
             * The visual used to report code generation status.
             */
            CodeGeneratorVisual* currentVisual;

            /**
             * The usage data instance assigned to this generator.
             */
            Ud::UsageData* currentUsageData;

            /**
             * The current code generation engine.
             */
            QSharedPointer<CodeGenerationEngine> currentEngine;

            /**
             * A hash of translators to be used with this code generator.
             */
            QHash<QString, QSharedPointer<Translator>> currentTranslators;

            /**
             * A hash of all the code generators known in the system.
             */
            static QHash<QString, QSharedPointer<CodeGenerator>> currentGenerators;

            /**
             * A mapping of code generators based on their supported output types.
             */
            static QHash<CodeGeneratorOutputTypeContainer, QSharedPointer<CodeGenerator>> currentGeneratorsByOutput;

            /**
             * An buffer used to hold an empty intermediate representation.
             */
            static const QByteArray emptyIntermediateRepresentation;

            /**
             * The list of supported diagnostic types.
             */
            QSet<Diagnostic::Type> currentEnabledDiagnosticTypes;

            /**
             * An empty/default list of returned diagnostics.
             */
            static const DiagnosticPointerList emptyDiagnosticsList;
    };
};

#endif
