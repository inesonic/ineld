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
* This header defines the \ref Ld::Diagnostic::Private class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CODE_GENERATOR_DIAGNOSTIC_PRIVATE_H
#define LD_CODE_GENERATOR_DIAGNOSTIC_PRIVATE_H

#include <QString>
#include <QSharedPointer>
#include <QWeakPointer>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_diagnostic.h"

namespace Ld {
    class Element;
    class CodeGenerator;

    /**
     * Private implementation of the \ref Ld::Diagnostic class.
     */
    class LD_PUBLIC_API Diagnostic::Private {
        public:
            /**
             * Constructor
             *
             * \param[in] element           The element that is associated with this diagnostic.
             *
             * \param[in] codeGeneratorName The name of the code generator that created this diagnostic.
             *
             * \param[in] diagnosticType    Indicates if this diagnostic is fatal, a warning, etc.
             */
            Private(ElementPointer element, const QString& codeGeneratorName, Diagnostic::Type diagnosticType);

            ~Private();

            /**
             * Method that returns the element tied to this diagnostic.
             *
             * \return Returns a pointer to the element tied to this diagnostic.
             */
            ElementPointer element() const;

            /**
             * Method that returns the name of the generator that created this diagnostic.
             *
             * \return Returns the code generator name that originated this diagnostic.
             */
            QString codeGeneratorName() const;

            /**
             * Method that indicates if this diagnostic is a warning, and error, or something else.
             *
             * \return Returns the diagnostic type.
             */
            Type diagnosticType() const;

        private:
            /**
             * Pointer to the element that is tied to the diagnostic.
             */
            ElementWeakPointer currentElement;

            /**
             * The name of the code generator that produced this diagnostic.
             */
            QString currentCodeGeneratorName;

            /**
             * The diagnostic type.
             */
            Diagnostic::Type currentDiagnosticType;
    };
};

#endif
