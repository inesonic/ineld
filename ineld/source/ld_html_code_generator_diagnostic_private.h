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
* This header defines the \ref Ld::HtmlCodeGeneratorDiagnostic::Private class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_HTML_CODE_GENERATOR_DIAGNOSTIC_PRIVATE_H
#define LD_HTML_CODE_GENERATOR_DIAGNOSTIC_PRIVATE_H

#include <QString>
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_html_translation_phase.h"
#include "ld_html_code_generator_diagnostic.h"

namespace Ld {
    class XmlExportContext;

    /**
     * Private implementation of the \ref Ld::HtmlCodeGeneratorDiagnostic class.
     */
    class LD_PUBLIC_API HtmlCodeGeneratorDiagnostic::Private {
        public:
            /**
             * Constructor
             *
             * \param[in] translationPhase The translation phase that generated the diagnostic.
             *
             * \param[in] diagnosticCode   The diagnostic code.
             *
             * \param[in] context          The context containing the intermediate representation plus the compiler
             *                             and linker settings.
             */
            Private(
                const HtmlTranslationPhase&       translationPhase,
                HtmlCodeGeneratorDiagnostic::Code diagnosticCode,
                QSharedPointer<XmlExportContext>  context
            );

            ~Private();

            /**
             * Method you can use to obtain the translation phase that generated the diagnostic.
             *
             * \return The translation phase where the error was detected.
             */
            const HtmlTranslationPhase& translationPhase() const;

            /**
             * Method you can use to obtain the diagnostic code.
             *
             * \return Returns the diagnostic code.
             */
            HtmlCodeGeneratorDiagnostic::Code diagnosticCode() const;

            /**
             * Method you can use to obtain the compiler context holding the intermediate representation.
             *
             * \return Returns a shared pointer to the underlying compiler context.
             */
            QSharedPointer<XmlExportContext> context() const;

        private:
            /**
             * The translation phase where the diagnostic was generated.
             */
            HtmlTranslationPhase currentTranslationPhase;

            /**
             * The diagnostic code for this diagnostic.
             */
            HtmlCodeGeneratorDiagnostic::Code currentDiagnosticCode;

            /**
             * Shared pointer to the C++ compiler context.
             */
            QSharedPointer<XmlExportContext> currentContext;
    };
};

#endif
