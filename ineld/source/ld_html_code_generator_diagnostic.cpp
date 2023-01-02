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
* This file implements the \ref Ld::HtmlCodeGeneratorDiagnostic class.
***********************************************************************************************************************/

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QSharedPointer>

#include <cbe_cpp_compiler_error_codes.h>

#include "ld_element_structures.h"
#include "ld_element.h"
#include "ld_translation_phase.h"
#include "ld_html_translation_phase.h"
#include "ld_xml_export_context.h"
#include "ld_code_generator.h"
#include "ld_html_code_generator_diagnostic_private.h"
#include "ld_diagnostic.h"
#include "ld_html_code_generator_diagnostic.h"

namespace Ld {
    static HtmlTranslationPhase defaultTranslationPhase;

    HtmlCodeGeneratorDiagnostic::HtmlCodeGeneratorDiagnostic() {}


    HtmlCodeGeneratorDiagnostic::HtmlCodeGeneratorDiagnostic(
            ElementPointer                           element,
            Type                                     diagnosticType,
            const HtmlTranslationPhase&              translationPhase,
            HtmlCodeGeneratorDiagnostic::Code        diagnosticCode,
            QSharedPointer<XmlExportContext>              context
        ):Diagnostic(
            element,
            QString("HtmlCodeGenerator"),
            diagnosticType
        ),impl(
            new Private(
                translationPhase,
                diagnosticCode,
                context
            )
        ) {}


    HtmlCodeGeneratorDiagnostic::HtmlCodeGeneratorDiagnostic(
            const HtmlCodeGeneratorDiagnostic& other
        ):Diagnostic(
            other
        ) {
        impl = other.impl;
    }


    HtmlCodeGeneratorDiagnostic::~HtmlCodeGeneratorDiagnostic() {}


    QString HtmlCodeGeneratorDiagnostic::diagnosticMessage() const {
        QString message;

        if (impl.isNull()) {
            message = tr("No diagnostic.");
        } else {
            QString description;

            switch (diagnosticCode()) {
                case Code::NO_DIAGNOSTIC: {
                    description = tr("No diagnostic message.");
                    break;
                }

                case Code::UNEXPECTED_TRANSLATION_PHASE: {
                    description = tr("Unexpected translation phase.  Please report this error to Inesonic, LLC.");
                    break;
                }

                case Code::MISSING_TRANSLATOR: {
                    description = tr("Missing translator for \"%1\".  Please report this error to Inesonic, LLC.")
                                  .arg(element()->typeName());
                    break;
                }

                case Code::CONTEXT_ERROR: {
                    description = impl->context()->errorString();
                    break;
                }

                case Code::BAD_PAYLOAD: {
                    description = tr("Bad payload");
                    break;
                }

                case Code::BAD_IMAGE_PAYLOAD: {
                    description = tr("Bad image payload");
                    break;
                }

                case Code::IMAGE_SAVE_ERROR: {
                    description = tr("Could not save image");
                    break;
                }

                case Code::HTML_STYLE_NOT_SUPPORTED: {
                    description = tr("HTML style not supported.  Please report this error to Inesonic, LLC.");
                    break;
                }

                case Code::FORMAT_ERROR: {
                    description = tr("Formatting error.  Please report this error to Inesonic, LLC.");
                    break;
                }

                default: {
                    description = tr("Undefined error %1.").arg(static_cast<unsigned>(diagnosticCode()));
                    break;
                }
            }

            message = tr("%1: %2").arg(diagnosticTypeMessage()).arg(description);
        }

        return message;
    }


    const HtmlTranslationPhase& HtmlCodeGeneratorDiagnostic::translationPhase() const {
        return impl.isNull() ? defaultTranslationPhase : impl->translationPhase();
    }


    HtmlCodeGeneratorDiagnostic::Code HtmlCodeGeneratorDiagnostic::diagnosticCode() const {
        return impl.isNull() ? Code::NO_DIAGNOSTIC : impl->diagnosticCode();
    }


    QSharedPointer<XmlExportContext> HtmlCodeGeneratorDiagnostic::context() const {
        return impl.isNull() ? QSharedPointer<XmlExportContext>() : impl->context();
    }


    HtmlCodeGeneratorDiagnostic& HtmlCodeGeneratorDiagnostic::operator=(const HtmlCodeGeneratorDiagnostic& other) {
        Diagnostic::operator=(other);
        impl = other.impl;

        return *this;
    }
}
