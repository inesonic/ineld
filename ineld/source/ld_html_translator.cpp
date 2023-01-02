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
* This file implements the \ref Ld::HtmlTranslator class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>
#include <QUrl>
#include <QColor>
#include <QBuffer>
#include <QByteArray>
#include <QImageReader>
#include <QTransform>
#include <QImage>

#include "ld_element.h"
#include "ld_element_structures.h"
#include "ld_format.h"
#include "ld_format_structures.h"
#include "ld_xml_export_context.h"
#include "ld_code_generation_engine.h"
#include "ld_root_element.h"
#include "ld_html_translation_phase.h"
#include "ld_html_code_generator_diagnostic.h"
#include "ld_html_code_generation_engine.h"
#include "ld_html_translator.h"

namespace Ld {
    HtmlTranslator::~HtmlTranslator() {}

    bool HtmlTranslator::translate(ElementPointer element, CodeGenerationEngine& codeGenerationEngine) {
        bool  success;
        const TranslationPhase&     translationPhase     = codeGenerationEngine.translationPhase();
        const HtmlTranslationPhase& htmlTranslationPhase = dynamic_cast<const HtmlTranslationPhase&>(translationPhase);

        HtmlCodeGenerationEngine& engine = dynamic_cast<HtmlCodeGenerationEngine&>(codeGenerationEngine);

        switch (htmlTranslationPhase.phase()) {
            case HtmlTranslationPhase::Phase::REGISTER_FORMATS: {
                success = registerFormats(element, engine);
                break;
            }

            case HtmlTranslationPhase::Phase::HEADER: {
                success = header(element, engine);
                break;
            }

            case HtmlTranslationPhase::Phase::BODY: {
                success = body(element, engine);
                break;
            }

            default: {
                HtmlCodeGeneratorDiagnostic* diagnostic = new HtmlCodeGeneratorDiagnostic(
                    element,
                    HtmlCodeGeneratorDiagnostic::Type::INTERNAL_ERROR,
                    htmlTranslationPhase,
                    HtmlCodeGeneratorDiagnostic::Code::UNEXPECTED_TRANSLATION_PHASE,
                    engine.contextPointer()
                );

                engine.translationErrorDetected(diagnostic);

                success = false;
                break;
            }
        }

        return success;
    }


    bool HtmlTranslator::registerFormats(ElementPointer element, HtmlCodeGenerationEngine& engine) {
        FormatPointer format = element->format();
        if (!format.isNull()) {
            engine.registerFormat(format);
        }

        return true;
    }


    bool HtmlTranslator::header(ElementPointer, HtmlCodeGenerationEngine&) {
        return true;
    }


    bool HtmlTranslator::body(ElementPointer, HtmlCodeGenerationEngine&) {
        return true;
    }
}
