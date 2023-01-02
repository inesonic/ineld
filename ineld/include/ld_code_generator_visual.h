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
* This header defines the \ref Ld::CodeGeneratorVisual class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CODE_GENERATOR_VISUAL_H
#define LD_CODE_GENERATOR_VISUAL_H

#include <QSharedPointer>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_diagnostic_structures.h"

namespace Ld {
    class CodeGenerator;
    class CodeGenerationEngine;
    class CodeGeneratorOutputTypeContainer;
    class TranslationPhase;
    class RootElement;

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
     * allowing plug-ins to be created that provide application specific code generators.  Each code generator includes
     * methods that can be used to register translators for each contruct under the root node.
     */
    class LD_PUBLIC_API CodeGeneratorVisual {
        friend class CodeGenerator;

        public:
            CodeGeneratorVisual();

            virtual ~CodeGeneratorVisual();

            /**
             * Method you can use to obtain the \ref CodeGenerator associated with this visual.
             *
             * \return Returns a pointer to the associated \ref CodeGenerator instance.
             */
            CodeGenerator* codeGenerator() const;

        protected:
            /**
             * Method that is called when the code generator starts a translation.
             *
             * \param[in] rootElement                      The root element that is used as the basis for the
             *                                             translation.  A null pointer indicates we're processing a
             *                                             selection.
             *
             * \param[in] outputType                       A value used to indicate the type of output being generated.
             *                                             The meaning of this value is dependent on the type of generator.
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
             * Method that is called when the code generator begins a new translation phase.
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
             * Method that is called when the code generator completes a translation process.
             *
             * \param[in] success     Holds true if the translation was successful.  Holds false if the translation
             *                        process failed.
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
             * Method that is called when the code generator is aborted during a translation.
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
             * to invoke the code generator.
             *
             * \param[in] diagnostic The diagnostic tied to the error, warning, etc.
             */
            virtual void translationErrorDetected(DiagnosticPointer diagnostic);

        private:
            CodeGenerator* currentCodeGenerator;
    };
};

#endif
