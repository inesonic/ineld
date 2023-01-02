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
* This file implements the \ref HtmlParagraphElementTranslator class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>

#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_paragraph_element.h"
#include "ld_block_format.h"
#include "ld_justified_block_format.h"
#include "ld_text_block_format.h"
#include "ld_paragraph_format.h"
#include "ld_unordered_list_paragraph_format.h"
#include "ld_ordered_list_paragraph_format.h"
#include "ld_list_additional_paragraph_format.h"
#include "ld_xml_export_context.h"
#include "ld_html_code_generator_diagnostic.h"
#include "ld_code_generation_engine.h"
#include "ld_html_code_generation_engine.h"
#include "ld_html_translator.h"
#include "ld_html_paragraph_element_translator.h"

namespace Ld {
    HtmlParagraphElementTranslator::~HtmlParagraphElementTranslator() {}


    QString HtmlParagraphElementTranslator::elementName() const {
        return ParagraphElement::elementName;
    }


    bool HtmlParagraphElementTranslator::body(ElementPointer element, HtmlCodeGenerationEngine& engine) {
        bool success;

        FormatPointer format = element->format();
        if (!format.isNull()) {
            QString formatTypeName = format->typeName();
            if (formatTypeName == ListAdditionalParagraphFormat::formatName) {
                success = bodyForAdditionalListEntry(element, format, engine);
            } else if (formatTypeName == UnorderedListParagraphFormat::formatName) {
                success = bodyForTopOfUnorderedList(element, format, engine);
            } else if (formatTypeName == OrderedListParagraphFormat::formatName) {
                success = bodyForTopOfOrderedList(element, format, engine);
            } else {
                success = bodyForNormalParagraph(element, format, engine);
            }
        } else {
            success = bodyForNormalParagraph(element, format, engine);
        }

        return success;
    }


    bool HtmlParagraphElementTranslator::bodyForNormalParagraph(
            ElementPointer            element,
            FormatPointer             format,
            HtmlCodeGenerationEngine& engine
        ) {
        bool              success;
        XmlExportContext& context = engine.context();

        if (engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML5_WITH_CSS) {
            context.writeStartElement("p");

            QString className;
            if (!format.isNull()) {
                className = engine.cssClassForFormat(format);
            }

            if (!className.isEmpty()) {
                context.writeAttribute("class", className);
            }

            success = translateAllChildren(element, engine);
            context.writeEndElement(); // p tag
        } else if (engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML4_WITHOUT_CSS) {
            addParagraphTag(context, format);

            success = translateAllChildren(element, engine);
            context.writeEndElement(); // p tag
        } else {
            HtmlCodeGeneratorDiagnostic* diagnostic = new HtmlCodeGeneratorDiagnostic(
                nullptr,
                HtmlCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                dynamic_cast<const HtmlTranslationPhase&>(engine.translationPhase()),
                HtmlCodeGeneratorDiagnostic::Code::HTML_STYLE_NOT_SUPPORTED,
                engine.contextPointer()
            );

            engine.translationErrorDetected(diagnostic);
            success = false;
        }

        return success;
    }


    bool HtmlParagraphElementTranslator::bodyForTopOfOrderedList(
            ElementPointer            element,
            FormatPointer             format,
            HtmlCodeGenerationEngine& engine
        ) {
        bool              success;
        XmlExportContext& context = engine.context();

        QSharedPointer<OrderedListParagraphFormat> orderedFormat = format.dynamicCast<OrderedListParagraphFormat>();

        if (engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML5_WITH_CSS) {
            context.writeStartElement("ol");
            context.writeStartElement("li");

            QString className = engine.cssClassForFormat(orderedFormat);
            if (!className.isEmpty()) {
                context.writeAttribute("class", className);
            }

            unsigned long startingNumber = orderedFormat->startingNumber();
            if (startingNumber != 1) {
                context.writeAttribute("style", "value", QString::number(startingNumber));
            }

            success = translateAllChildren(element, engine);
            context.writeEndElement(); // li tag

            if (isLastInList(element)) {
                context.writeEndElement(); // ol
            }
        } else if (engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML4_WITHOUT_CSS) {
            context.writeStartElement("ol");
            addListTag(context, format); // li tag
            addParagraphTag(context, format); // p tag

            success = translateAllChildren(element, engine);
            context.writeEndElement(); // p tag

            if (isLastInList(element)) {
                context.writeEndElement(); // li tag
                context.writeEndElement(); // ol tag
            }
        } else {
            HtmlCodeGeneratorDiagnostic* diagnostic = new HtmlCodeGeneratorDiagnostic(
                nullptr,
                HtmlCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                dynamic_cast<const HtmlTranslationPhase&>(engine.translationPhase()),
                HtmlCodeGeneratorDiagnostic::Code::HTML_STYLE_NOT_SUPPORTED,
                engine.contextPointer()
            );

            engine.translationErrorDetected(diagnostic);
            success = false;
        }

        return success;
    }


    bool HtmlParagraphElementTranslator::bodyForTopOfUnorderedList(
            ElementPointer            element,
            FormatPointer             format,
            HtmlCodeGenerationEngine& engine
        ) {
        bool              success;
        XmlExportContext& context = engine.context();

        if (engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML5_WITH_CSS) {
            context.writeStartElement("p");

            QString className = engine.cssClassForFormat(format);
            if (!className.isEmpty()) {
                context.writeAttribute("class", className);
            }

            success = translateAllChildren(element, engine);
            context.writeEndElement(); // p tag
        } else if (engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML4_WITHOUT_CSS) {
            context.writeStartElement("ul");
            addListTag(context, format); // li tag
            addParagraphTag(context, format); // p tag

            success = translateAllChildren(element, engine);
            context.writeEndElement(); // p tag

            if (isLastInList(element)) {
                context.writeEndElement(); // li tag
                context.writeEndElement(); // ol tag
            }
        } else {
            HtmlCodeGeneratorDiagnostic* diagnostic = new HtmlCodeGeneratorDiagnostic(
                nullptr,
                HtmlCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                dynamic_cast<const HtmlTranslationPhase&>(engine.translationPhase()),
                HtmlCodeGeneratorDiagnostic::Code::HTML_STYLE_NOT_SUPPORTED,
                engine.contextPointer()
            );

            engine.translationErrorDetected(diagnostic);
            success = false;
        }

        return success;
    }


    bool HtmlParagraphElementTranslator::bodyForAdditionalListEntry(
            ElementPointer            element,
            FormatPointer             format,
            HtmlCodeGenerationEngine& engine
        ) {
        bool              success;
        XmlExportContext& context = engine.context();

        unsigned      distance;
        FormatPointer drivingFormat = findDrivingFormat(element, distance);

        bool orderedList    = (drivingFormat->typeName() == OrderedListParagraphFormat::formatName);
        bool bulletOrNumber = (format.dynamicCast<ListAdditionalParagraphFormat>()->bulletNumberDisplayEnabled());

        if (engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML5_WITH_CSS) {
            QString className;
            if (orderedList) {
                context.writeStartElement("li");
                className = engine.cssClassForFormat(drivingFormat);

                if (!className.isEmpty()) {
                    context.writeAttribute("class", className);
                }

                if (!bulletOrNumber) {
                    context.writeAttribute("style", "list-style-type: none;");
                } else {
                    unsigned long base = drivingFormat.dynamicCast<OrderedListParagraphFormat>()->startingNumber();
                    context.writeAttribute("value", QString::number(base + distance));
                }
            } else {
                context.writeStartElement("p");
                className = engine.cssClassForFormat(drivingFormat);
                if (!className.isEmpty()) {
                    className += bulletOrNumber ? "_bullet" : "_no_bullet";
                    context.writeAttribute("class", className);
                }
            }

            success = translateAllChildren(element, engine);
            context.writeEndElement(); // li or p tag

            if (orderedList && isLastInList(element)) {
                context.writeEndElement(); // ol tag (but not ul tag)
            }
        } else if (engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML4_WITHOUT_CSS) {
            if (!bulletOrNumber) {
                addParagraphTag(context, drivingFormat);
            } else {
                context.writeEndElement(); // li tag
                addListTag(context, drivingFormat, distance);
                addParagraphTag(context, drivingFormat);
            }

            success = translateAllChildren(element, engine);
            context.writeEndElement(); // p tag

            if (isLastInList(element)) {
                context.writeEndElement(); // li tag
                context.writeEndElement(); // ol tag
            }
        } else {
            HtmlCodeGeneratorDiagnostic* diagnostic = new HtmlCodeGeneratorDiagnostic(
                nullptr,
                HtmlCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                dynamic_cast<const HtmlTranslationPhase&>(engine.translationPhase()),
                HtmlCodeGeneratorDiagnostic::Code::HTML_STYLE_NOT_SUPPORTED,
                engine.contextPointer()
            );

            engine.translationErrorDetected(diagnostic);
            success = false;
        }

        return success;
    }


    FormatPointer HtmlParagraphElementTranslator::findDrivingFormat(ElementPointer element, unsigned& distance) {
        FormatPointer formatToUse;

        distance = 0;
        ElementPointer drivingElement = element;
        while (!drivingElement.isNull() && formatToUse.isNull()) {
            FormatPointer format = drivingElement->format();
            if (!format.isNull()) {
                Format::Capabilities capabilities = format->capabilities();
                if (capabilities.contains(ParagraphFormat::formatName)              ||
                    capabilities.contains(OrderedListParagraphFormat::formatName)   ||
                    capabilities.contains(UnorderedListParagraphFormat::formatName)    ) {
                    formatToUse = format;
                } else {
                    if (capabilities.contains(ListAdditionalParagraphFormat::formatName)) {
                        QSharedPointer<ListAdditionalParagraphFormat>
                            additionalParagraphFormat = format.dynamicCast<ListAdditionalParagraphFormat>();

                        if (additionalParagraphFormat->bulletNumberDisplayEnabled()) {
                            ++distance;
                        }
                    }

                    drivingElement = drivingElement->previousSibling();
                }
            } else {
                drivingElement = drivingElement->previousSibling();
            }
        }

        return formatToUse;
    }


    bool HtmlParagraphElementTranslator::isLastInList(ElementPointer element) {
        bool isLast;

        ElementPointer nextElement = element->nextSibling();
        if (!nextElement.isNull()) {
            FormatPointer        nextFormat   = nextElement->format();
            Format::Capabilities capabilities = nextFormat->capabilities();

            if (capabilities.contains(UnorderedListParagraphFormat::formatName) ||
                capabilities.contains(OrderedListParagraphFormat::formatName)   ||
                capabilities.contains(ParagraphFormat::formatName)                 ) {
                isLast = true;
            } else {
                isLast = false;
            }
        } else {
            isLast = true;
        }

        return isLast;
    }


    void HtmlParagraphElementTranslator::addParagraphTag(XmlExportContext &context, FormatPointer format) {
        context.writeStartElement("p");

        if (!format.isNull()) {
            QSharedPointer<ParagraphFormat> paragraphFormat = format.dynamicCast<ParagraphFormat>();
            switch (paragraphFormat->justificationMode()) {
                case ParagraphFormat::Justification::NONE:
                case ParagraphFormat::Justification::LEFT: {
                    break;
                }

                case ParagraphFormat::Justification::RIGHT: {
                    context.writeAttribute("align", "right");
                    break;
                }

                case ParagraphFormat::Justification::CENTER: {
                    context.writeAttribute("align", "center");
                    break;
                }

                case ParagraphFormat::Justification::JUSTIFY: {
                    context.writeAttribute("align", "justify");
                    break;
                }

                default: {
                    assert(false);
                    break;
                }
            }
        }
    }


    void HtmlParagraphElementTranslator::addListTag(
            XmlExportContext& context,
            FormatPointer     format,
            unsigned long     offset
        ) {
        if (format->typeName() == OrderedListParagraphFormat::formatName) {
            QSharedPointer<OrderedListParagraphFormat>
                orderedFormat = format.dynamicCast<OrderedListParagraphFormat>();

            context.writeStartElement("li");

            switch (orderedFormat->style()) {
                case OrderedListParagraphFormat::Style::INVALID:
                case OrderedListParagraphFormat::Style::NUMBER: {
                    context.writeAttribute("type", "1");
                    break;
                }

                case OrderedListParagraphFormat::Style::LOWER_CASE: {
                    context.writeAttribute("type", "a");
                    break;
                }

                case OrderedListParagraphFormat::Style::UPPER_CASE: {
                    context.writeAttribute("type", "A");
                    break;
                }

                case OrderedListParagraphFormat::Style::ROMAN_LOWER_CASE: {
                    context.writeAttribute("type", "i");
                    break;
                }

                case OrderedListParagraphFormat::Style::ROMAN_UPPER_CASE: {
                    context.writeAttribute("type", "I");
                    break;
                }
                default: {
                    assert(false);
                    break;
                }
            }

            unsigned long listItemNumber = orderedFormat->startingNumber() + offset;
            if (listItemNumber != 1) {
                context.writeAttribute("value", QString::number(listItemNumber));
            }
        } else if (format->typeName() == UnorderedListParagraphFormat::formatName) {
            QSharedPointer<UnorderedListParagraphFormat>
                unorderedFormat = format.dynamicCast<UnorderedListParagraphFormat>();

            context.writeStartElement("li");

            QString bulletStyle = unorderedFormat->bulletStyle();

            const QList<UnorderedListParagraphFormat::StandardBullet>&
                standardBullets = UnorderedListParagraphFormat::standardBullets();

            QList<UnorderedListParagraphFormat::StandardBullet>::const_iterator
                bulletIterator = standardBullets.constBegin();

            QList<UnorderedListParagraphFormat::StandardBullet>::const_iterator
                bulletEndIterator = standardBullets.constEnd();

            while (bulletIterator != bulletEndIterator && bulletIterator->bullet() != bulletStyle) {
                ++bulletIterator;
            }

            QString htmlBulletStyle;
            switch (bulletIterator->htmlCategory()) {
                case UnorderedListParagraphFormat::StandardBullet::HtmlCategory::INVALID:
                case UnorderedListParagraphFormat::StandardBullet::HtmlCategory::DISC: {
                    context.writeAttribute("type", "disc");
                    break;
                }

                case UnorderedListParagraphFormat::StandardBullet::HtmlCategory::CIRCLE: {
                    context.writeAttribute("type", "circle");
                    break;
                }

                case UnorderedListParagraphFormat::StandardBullet::HtmlCategory::SQUARE: {
                    context.writeAttribute("type", "square");
                    break;
                }

                default: {
                    assert(false);
                    break;
                }
            }
        }
    }
}
