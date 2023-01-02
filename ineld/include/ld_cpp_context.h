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
* This header defines the \ref Ld::CppContext class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CPP_CONTEXT_H
#define LD_CPP_CONTEXT_H

#include <QString>
#include <QMap>
#include <QSharedPointer>
#include <QWeakPointer>

#include <cbe_cpp_compiler_context.h>
#include <cbe_linker_context.h>

#include "ld_common.h"
#include "ld_element_structures.h"

namespace Ld {
    class CppCodeGenerationEngine;

    /**
     * Class you can use as a context for the backend compiler and linker.  The class combines both the compiler and
     * linker context into a single class that shares both sets of methods.  The class also manages processing of
     * the backend compiler and linker status information.
     */
    class LD_PUBLIC_API CppContext:public Cbe::CppCompilerContext, public Cbe::LinkerContext {
        public:
            /**
             * Constructor.
             *
             * \param[in] objectFile The object file to be compiled and linked.
             *
             * \param[in] outputFile The name of the generated output file.
             *
             * \param[in] engine     The code generation engine to receive callbacks from this context.
             */
            CppContext(const QString& objectFile, const QString& outputFile, CppCodeGenerationEngine* engine);

            ~CppContext() override;

            /**
             * Method that can be called to indicate that a new statement has been started.  Calling this method will
             * supress insertion of a semicolon + newline if \ref Ld::CppContext::startNewStatement is called without
             * additional content inserted first.
             */
            void startedNewStatement();

            /**
             * Method that can be called to start a new C++ statement within the translation buffer.  The method will
             * conditionally add a semicolon and newline to the buffer.
             */
            void startNewStatement();

            /**
             * Method that can be used to flag the start of buffer content tied to an element.  Call this method each
             * time you are about to add content to this context.
             *
             * \param[in] element The element to tie to the current buffer position.  If multiple elements are added to
             *                    the same buffer position, the newest element will be used.  Note that element pointers
             *                    are stored as weak references.
             */
            void startElement(ElementPointer element);

            /**
             * Method you can use to determine the element that resides at a given location.
             *
             * \param[in] byteOffset The byte offset into the buffer we want information on.
             *
             * \return Returns the element at the specified location.  A null pointer is returned if the byte offset can
             *         not be tied to a location or if the underlying element has been destroyed.
             */
            ElementPointer elementAt(unsigned long byteOffset) const;

            /**
             * Overloaded function operator.  You can use this to combine the use of the
             * \ref Ld::CppContext::startElement with the stream operators in a compact fashion.  See the example below.
             *
             * \code
               context(myElement) << myElement->variableName(); \endcode
             *
             * The above code would be functionally identical to:
             *
             * \code
               context.startElement(myElement);
               context.append(myElement->variableName()); \endcode
             *
             * \param[in] element Pointer to the element we're about to add content for.
             *
             * \return Returns a reference to this class instance.
             */
            inline CppContext& operator()(ElementPointer element) {
                startElement(element);
                return *this;
            }

        private:
            /**
             * Virtual method you can overload to receive notification of diagnostic messages from the compiler.  Note
             * that the method may be called from a different thread than the one used to invoke the compiler.
             *
             * The default implementation simply returns without performing any additional function.
             *
             * \param[in] diagnostic The diagnostic information generated by the compiler.
             */
            void handleCompilerDiagnostic(const Cbe::CppCompilerDiagnostic& diagnostic) final;

            /**
             * Virtual method you can overload to receive notification when the compiler has finished.  Note that the
             * method may be called from a different thread than the one used to invoke the compiler.
             *
             * The default implementation simply returns.
             *
             * \param[in] success Holds true if the compiler completed successfully, returns false if an error is
             *                    reported.
             */
            void compilerFinished(bool success) final;

            /**
             * Virtual method you can overload to receive notification of diagnostic messages from the linker.  Note
             * that the method may be called from a different thread than the one used to invoke the linker.
             *
             * The default implementation simply returns without performing any additional function.
             *
             * \param[in] diagnosticMessage The diagnostic message generated by the linker.
             */
            void handleLinkerDiagnostic(const QString& diagnosticMessage) final;

            /**
             * Virtual method you can overload to receive notification when the linker has finished.  Note that the
             * method may be called from a different thread than the one used to invoke the linker.
             *
             * The default implementation simply returns.
             *
             * \param[in] success Holds true if the linker completed successfully, returns false if an error is
             *                    reported.
             */
            void linkerFinished(bool success) final;

            /**
             * Pointer to the engine to receive notification from the compilation and link operation.
             */
            CppCodeGenerationEngine* currentEngine;

            /**
             * The length of the underlying buffer the last time we started a new statement.  A negative value indicates
             * that we've never started a statement.
             */
            long lastByteOffset;

            /**
             * Map of elements by buffer location.  Note that the same element can appear in the map multiple times.
             */
            QMap<unsigned long, ElementWeakPointer> elementStartLocations;
    };
};

#endif
