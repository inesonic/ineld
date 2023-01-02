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
* This file implements the \ref Ld::HtmlValueFieldElementTranslator class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>
#include <QColor>

#include <algorithm>

#include <model_variant.h>

#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_value_field_element.h"
#include "ld_root_element.h"
#include "ld_data_type.h"
#include "ld_calculated_value.h"
#include "ld_data_type_translator.h"
#include "ld_html_data_type_translator.h"
#include "ld_font_format.h"
#include "ld_xml_export_context.h"
#include "ld_html_code_generator.h"
#include "ld_code_generation_engine.h"
#include "ld_html_code_generator_diagnostic.h"
#include "ld_html_code_generation_engine.h"
#include "ld_html_translator.h"
#include "ld_html_value_field_element_translator.h"

namespace Ld {
    HtmlValueFieldElementTranslator::~HtmlValueFieldElementTranslator() {}


    QString HtmlValueFieldElementTranslator::elementName() const {
        return Ld::ValueFieldElement::elementName;
    }


    bool HtmlValueFieldElementTranslator::body(ElementPointer element, HtmlCodeGenerationEngine& engine) {
        bool            success         = false;
        CalculatedValue calculatedValue = engine.rootElement()->elementCalculatedValue(element, 0);
        const DataType& dataType        = calculatedValue.dataType();

        if (dataType.valueType() != DataType::ValueType::NONE) {
            const DataTypeTranslator* dataTypeTranslator = dataType.translator(HtmlCodeGenerator::codeGeneratorName);
            if (dataTypeTranslator != nullptr) {
                HtmlCodeGenerationEngine::HtmlStyle htmlStyle     = engine.htmlStyle();
                XmlExportContext&                   context       = engine.context();
                unsigned                            numberEndTags = 0;

                const HtmlDataTypeTranslator*
                    htmlDataTypeTranslator = dynamic_cast<const HtmlDataTypeTranslator*>(dataTypeTranslator);

                HtmlCodeGenerationEngine::MathMode mathMode = engine.mathMode();

                if (mathMode == HtmlCodeGenerationEngine::MathMode::NO_MATH       ||
                    mathMode == HtmlCodeGenerationEngine::MathMode::MATHJAX_LATEX    ) {
                    if (htmlStyle == HtmlCodeGenerationEngine::HtmlStyle::HTML5_WITH_CSS) {
                        QString className = engine.cssClassForFormat(element->format());
                        if (!className.isEmpty()) {
                            context.writeStartElement("span");
                            context.writeAttribute("class", className);
                            numberEndTags = 1;
                        }
                    } else if (htmlStyle == HtmlCodeGenerationEngine::HtmlStyle::HTML4_WITHOUT_CSS) {
                        QSharedPointer<FontFormat> fontFormat = element->format().dynamicCast<FontFormat>();

                        if (!fontFormat.isNull()) {
                            context.writeStartElement("font");
                            context.writeAttribute("face", fontFormat->family());
                            numberEndTags = 1;

                            int size = std::max(
                                1,
                                std::min(7, 3 + (static_cast<int>(fontFormat->fontSize()) - 12) / 2)
                            );
                            if (size != 3) {
                                context.writeAttribute("size", QString::number(size));
                            }

                            QColor fontColor = fontFormat->fontColor();
                            if (fontColor.isValid()) {
                                context.writeAttribute("color", fontColor.name());
                            }

                            if (fontFormat->italics()) {
                                context.writeStartElement("i");
                                ++numberEndTags;
                            }

                            unsigned fontWeightValue   = static_cast<unsigned>(fontFormat->fontWeight());
                            unsigned normalWeightValue = static_cast<unsigned>(FontFormat::Weight::NORMAL);
                            if (fontWeightValue > normalWeightValue) {
                                context.writeStartElement("b");
                                ++numberEndTags;
                            }

                            if (fontFormat->underline()) {
                                context.writeStartElement("u");
                                ++numberEndTags;
                            }

                            if (fontFormat->strikeout()) {
                                context.writeStartElement("s");
                                ++numberEndTags;
                            }
                        }
                    }
                }

                if (mathMode == HtmlCodeGenerationEngine::MathMode::NO_MATH) {
                    success = htmlDataTypeTranslator->toHtml(calculatedValue.variant(), element, engine);
                } else {
                    if (mathMode == HtmlCodeGenerationEngine::MathMode::MATHML         ||
                        mathMode == HtmlCodeGenerationEngine::MathMode::MATHJAX_MATHML    ) {
                        context.writeStartElement("math");
                        context.writeAttribute("xmlns", "http://www.w3.org/1998/Math/MathML");
                        context.writeAttribute("display", "inline");

                        if (htmlStyle == HtmlCodeGenerationEngine::HtmlStyle::HTML5_WITH_CSS) {
                            QString className = engine.cssClassForFormat(element->format());
                            if (!className.isEmpty()) {
                                context.writeAttribute("class", className);
                            }
                        }

                        context.writeStartElement("semantics");
                        numberEndTags += 2;

                        success = htmlDataTypeTranslator->toMathML(calculatedValue.variant(), element, engine);
                    } else if (mathMode == HtmlCodeGenerationEngine::MathMode::MATHJAX_LATEX) {
                        context.writeCharacters("\\(");
                        success = htmlDataTypeTranslator->toLaTeX(calculatedValue.variant(), element, engine);
                        context.writeCharacters("\\)");
                    } else if (mathMode == HtmlCodeGenerationEngine::MathMode::IMAGES) {
                        success = writeAsImage(element, engine);
                    } else {
                        Q_ASSERT(false);
                    }
                }

                while (numberEndTags > 0) {
                    --numberEndTags;
                    context.writeEndElement();
                }
            } else {
                HtmlCodeGeneratorDiagnostic* diagnostic = new HtmlCodeGeneratorDiagnostic(
                    element,
                    HtmlCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                    dynamic_cast<const HtmlTranslationPhase&>(engine.translationPhase()),
                    HtmlCodeGeneratorDiagnostic::Code::MISSING_TRANSLATOR,
                    engine.contextPointer()
                );

                engine.translationErrorDetected(diagnostic);
                success = false;
            }
        } else {
            success = HtmlDataTypeTranslator::generateDefaultHtml(element, engine);
        }

        return success;
    }
}
