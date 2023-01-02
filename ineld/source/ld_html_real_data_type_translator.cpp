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
* This file implements the \ref Ld::HtmlRealDataTypeTranslator class.
***********************************************************************************************************************/

#include <QString>

#include <model_intrinsic_types.h>
#include <model_variant.h>

#include <util_string.h>

#include "ld_element.h"
#include "ld_real_type_element.h"
#include "ld_real_data_type_format.h"
#include "ld_value_field_format.h"
#include "ld_element_structures.h"
#include "ld_html_code_generator.h"
#include "ld_html_code_generation_engine.h"
#include "ld_html_code_generator_diagnostic.h"
#include "ld_xml_export_context.h"
#include "ld_identifier_container.h"
#include "ld_html_data_type_translator.h"
#include "ld_html_real_data_type_translator.h"

namespace Ld {
    const HtmlRealDataTypeTranslator HtmlRealDataTypeTranslator::instance;

    HtmlRealDataTypeTranslator::HtmlRealDataTypeTranslator() {}


    HtmlRealDataTypeTranslator::~HtmlRealDataTypeTranslator() {}


    QString HtmlRealDataTypeTranslator::elementName() const {
        return RealTypeElement::elementName;
    }


    bool HtmlRealDataTypeTranslator::toMathML(
            const Model::Variant&     variant,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
        ) const {
        bool        success;
        Model::Real value = variant.toReal(&success);

        if (success) {
            bool                                upperCase;
            unsigned                            precision;
            double                              multiplier;
            RealDataTypeFormat::RealNumberStyle realNumberStyle;

            FormatPointer format = element->format();
            if (!format.isNull()                                                         &&
                (format->capabilities().contains(RealDataTypeFormat::formatName) ||
                 format->capabilities().contains(ValueFieldFormat::formatName)         )    ) {
                QSharedPointer<Ld::RealDataTypeFormat> realFormat = format.dynamicCast<RealDataTypeFormat>();

                upperCase       = realFormat->upperCase();
                precision       = realFormat->precision();
                multiplier      = realFormat->multiplier();
                realNumberStyle = realFormat->realNumberStyle();
            } else {
                upperCase       = RealDataTypeFormat::defaultUpperCase;
                precision       = RealDataTypeFormat::defaultPrecision;
                multiplier      = RealDataTypeFormat::defaultMultiplier;
                realNumberStyle = RealDataTypeFormat::defaultRealNumberStyle;
            }

            QString mantissaString;
            QString exponentString;

            Util::longDoubleToMantissaAndExponentStrings(
                value * multiplier,
                realNumberStyle,
                precision,
                mantissaString,
                exponentString
            );

            XmlExportContext& context = engine.context();

            if (exponentString.isEmpty()) {
                context.writeStartElement("mn");
                context.writeCharacters(mantissaString);
                context.writeEndElement(); // mn
            } else {
                if (realNumberStyle == RealDataTypeFormat::RealNumberStyle::COMPUTER_SCIENTIFIC ||
                    realNumberStyle == RealDataTypeFormat::RealNumberStyle::COMPUTER_CONCISE       ) {
                    QString text =   upperCase
                                   ? tr("%1E%2").arg(mantissaString).arg(exponentString)
                                   : tr("%1e%2").arg(mantissaString).arg(exponentString);

                    context.writeStartElement("mn");
                    context.writeCharacters(text);
                    context.writeEndElement(); // mn
                } else {
                    context.writeStartElement("mrow");
                    context.writeStartElement("mn");
                    context.writeCharacters(mantissaString);
                    context.writeEndElement(); // mn
                    context.writeStartElement("mo");
                    context.writeAttribute("stretchy", "false");
                    context.writeCharacters(QString(QChar(0x00D7)));
                    context.writeEndElement(); // mo
                    context.writeStartElement("msup");
                    context.writeStartElement("mn");
                    context.writeCharacters(tr("10"));
                    context.writeEndElement(); // mn
                    context.writeStartElement("mn");
                    context.writeCharacters(exponentString);
                    context.writeEndElement(); // mn
                    context.writeEndElement(); // msup
                }
            }
        }

        return success;
    }


    bool HtmlRealDataTypeTranslator::toLaTeX(
            const Model::Variant&     variant,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
        ) const {
        bool        success;
        Model::Real value = variant.toReal(&success);

        if (success) {
            bool                                upperCase;
            unsigned                            precision;
            double                              multiplier;
            RealDataTypeFormat::RealNumberStyle realNumberStyle;

            FormatPointer format = element->format();
            if (!format.isNull()                                                         &&
                (format->capabilities().contains(RealDataTypeFormat::formatName) ||
                 format->capabilities().contains(ValueFieldFormat::formatName)         )    ) {
                QSharedPointer<Ld::RealDataTypeFormat> realFormat = format.dynamicCast<RealDataTypeFormat>();

                upperCase       = realFormat->upperCase();
                precision       = realFormat->precision();
                multiplier      = realFormat->multiplier();
                realNumberStyle = realFormat->realNumberStyle();
            } else {
                upperCase       = RealDataTypeFormat::defaultUpperCase;
                precision       = RealDataTypeFormat::defaultPrecision;
                multiplier      = RealDataTypeFormat::defaultMultiplier;
                realNumberStyle = RealDataTypeFormat::defaultRealNumberStyle;
            }

            QString mantissaString;
            QString exponentString;

            Util::longDoubleToMantissaAndExponentStrings(
                value * multiplier,
                realNumberStyle,
                precision,
                mantissaString,
                exponentString
            );

            XmlExportContext& context = engine.context();

            if (exponentString.isEmpty()) {
                context.writeCharacters(tr("{%1}").arg(mantissaString));
            } else {
                if (realNumberStyle == RealDataTypeFormat::RealNumberStyle::COMPUTER_SCIENTIFIC ||
                    realNumberStyle == RealDataTypeFormat::RealNumberStyle::COMPUTER_CONCISE       ) {
                    QString text =   upperCase
                                   ? tr("%1E%2").arg(mantissaString).arg(exponentString)
                                   : tr("%1e%2").arg(mantissaString).arg(exponentString);

                    context.writeCharacters(tr("\\text{%1}").arg(text));
                } else {
                    context.writeCharacters(
                        tr("\\text{%1}\\times\\text{10}^\\text{%2}").arg(mantissaString).arg(exponentString)
                    );
                }
            }
        }

        return success;
    }


    bool HtmlRealDataTypeTranslator::toHtml(
            const Model::Variant&     variant,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
        ) const {
        bool        success;
        Model::Real value = variant.toReal(&success);

        if (success) {
            bool                                upperCase;
            unsigned                            precision;
            double                              multiplier;
            RealDataTypeFormat::RealNumberStyle realNumberStyle;

            FormatPointer format = element->format();
            if (!format.isNull()                                                         &&
                (format->capabilities().contains(RealDataTypeFormat::formatName) ||
                 format->capabilities().contains(ValueFieldFormat::formatName)         )    ) {
                QSharedPointer<Ld::RealDataTypeFormat> realFormat = format.dynamicCast<RealDataTypeFormat>();

                upperCase       = realFormat->upperCase();
                precision       = realFormat->precision();
                multiplier      = realFormat->multiplier();
                realNumberStyle = realFormat->realNumberStyle();
            } else {
                upperCase       = RealDataTypeFormat::defaultUpperCase;
                precision       = RealDataTypeFormat::defaultPrecision;
                multiplier      = RealDataTypeFormat::defaultMultiplier;
                realNumberStyle = RealDataTypeFormat::defaultRealNumberStyle;
            }

            QString mantissaString;
            QString exponentString;

            Util::longDoubleToMantissaAndExponentStrings(
                value * multiplier,
                realNumberStyle,
                precision,
                mantissaString,
                exponentString
            );

            XmlExportContext& context       = engine.context();
            unsigned          numberEndTags = 0;

            if (engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML5_WITH_CSS) {
                QString className = engine.cssClassForFormat(format);
                if (!className.isEmpty()) {
                    context.writeStartElement("span");
                    context.writeAttribute("class", className);
                    numberEndTags = 1;
                }
            } else if (engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML4_WITHOUT_CSS) {
                QSharedPointer<FontFormat> fontFormat = format.dynamicCast<FontFormat>();
                if (!fontFormat.isNull()) {
                    context.writeStartElement("font");
                    context.writeAttribute("face", fontFormat->family());

                    int size = std::max(1, std::min(7, 3 + (static_cast<int>(fontFormat->fontSize()) - 12) / 2));
                    if (size != 3) {
                        context.writeAttribute("size", QString::number(size));
                    }

                    QColor fontColor = fontFormat->fontColor();
                    if (fontColor.isValid()) {
                        context.writeAttribute("color", fontColor.name());
                    }

                    numberEndTags = 1;

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
            } else {
                HtmlCodeGeneratorDiagnostic* diagnostic = new HtmlCodeGeneratorDiagnostic(
                    element,
                    HtmlCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                    dynamic_cast<const HtmlTranslationPhase&>(engine.translationPhase()),
                    HtmlCodeGeneratorDiagnostic::Code::HTML_STYLE_NOT_SUPPORTED,
                    engine.contextPointer()
                );

                engine.translationErrorDetected(diagnostic);
                success = false;
            }

            if (success) {
                if (exponentString.isEmpty()) {
                    context.writeCharacters(mantissaString);
                } else {
                    if (realNumberStyle == RealDataTypeFormat::RealNumberStyle::COMPUTER_SCIENTIFIC ||
                        realNumberStyle == RealDataTypeFormat::RealNumberStyle::COMPUTER_CONCISE       ) {
                        QString text =   upperCase
                                       ? tr("%1E%2").arg(mantissaString).arg(exponentString)
                                       : tr("%1e%2").arg(mantissaString).arg(exponentString);

                        context.writeCharacters(text);
                    } else {
                        context.writeCharacters(mantissaString + QChar(0x00D7) + tr("10"));
                        context.writeStartElement("sup");
                        context.writeCharacters(exponentString);
                        ++numberEndTags;
                    }
                }
            }

            while (numberEndTags > 0) {
                context.writeEndElement();
                --numberEndTags;
            }
        }

        return success;
    }
}
