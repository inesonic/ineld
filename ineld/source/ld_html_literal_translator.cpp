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
* This file implements the \ref Ld::HtmlLiteralTranslator class.
***********************************************************************************************************************/

#include <QString>

#include "ld_element_structures.h"
#include "ld_literal_element.h"
#include "ld_operator_format.h"
#include "ld_xml_export_context.h"
#include "ld_code_generation_engine.h"
#include "ld_html_code_generator_diagnostic.h"
#include "ld_html_code_generation_engine.h"
#include "ld_html_translator.h"
#include "ld_html_literal_translator.h"

namespace Ld {
    HtmlLiteralTranslator::~HtmlLiteralTranslator() {}


    QString HtmlLiteralTranslator::elementName() const {
        return Ld::LiteralElement::elementName;
    }


    bool HtmlLiteralTranslator::body(ElementPointer element, HtmlCodeGenerationEngine& engine) {
        bool result;

        XmlExportContext&                  context        = engine.context();
        HtmlCodeGenerationEngine::MathMode mathMode       = engine.mathMode();
        QSharedPointer<LiteralElement>     literalElement = element.dynamicCast<Ld::LiteralElement>();
        LiteralElement::SectionList        sections       = literalElement->section();

        Element::ParenthesisStyle parenthesisStyle = element->parenthesisStyle();

        switch (mathMode) {
            case HtmlCodeGenerationEngine::MathMode::NO_MATH: {
                result = true;
                break;
            }

            case HtmlCodeGenerationEngine::MathMode::MATHML:
            case HtmlCodeGenerationEngine::MathMode::MATHJAX_MATHML: {
                if (parenthesisStyle != Element::ParenthesisStyle::INVALID_OR_NONE) {
                    context.writeStartElement("mrow");
                    context.writeStartElement("mo");
                    context.writeAttribute("stretchy", "true");

                    switch (parenthesisStyle) {
                        case Element::ParenthesisStyle::INVALID_OR_NONE: {
                            break;
                        }

                        case Element::ParenthesisStyle::PARENTHESIS: {
                            context.writeCharacters(tr("("));
                            break;
                        }

                        case Element::ParenthesisStyle::BRACKETS: {
                            context.writeCharacters(tr("["));
                            break;
                        }

                        case Element::ParenthesisStyle::BRACES: {
                            context.writeCharacters(tr("{"));
                            break;
                        }

                        default: {
                            assert(false);
                            break;
                        }
                    }

                    context.writeEndElement(); // mo
                }

                context.writeStartElement("mrow");

                bool inExponent = false;
                for (  LiteralElement::SectionList::const_iterator sectionIterator    = sections.constBegin(),
                                                                   sectionEndIterator = sections.constEnd()
                     ; sectionIterator != sectionEndIterator
                     ; ++sectionIterator
                    ) {
                    const LiteralElement::Section& section = *sectionIterator;
                    switch (section.sectionType()) {
                        case LiteralElement::SectionType::INVALID: {
                            assert(false);
                            break;
                        }

                        case LiteralElement::SectionType::STRING_CONTENT: {
                            if (!section.isEmpty()) {
                                QString leaderCharacter = section.at(0);

                                context.writeStartElement("mtext");

                                if (leaderCharacter == tr("\"")) {
                                    context.writeCharacters(tr("%1").arg(QChar(0x201C)));
                                    context.writeCharacters(section.mid(1));
                                    context.writeCharacters(tr("%1").arg(QChar(0x201D)));
                                } else if (leaderCharacter == tr("'")) {
                                    context.writeCharacters(tr("%1").arg(QChar(0x2018)));
                                    context.writeCharacters(section.mid(1));
                                    context.writeCharacters(tr("%1").arg(QChar(0x2019)));
                                } else if (leaderCharacter == tr("<")                     ||
                                           leaderCharacter == tr("%1").arg(QChar(0x27E8))    ) {
                                    context.writeCharacters(tr("%1").arg(QChar(0x27E8)));
                                    context.writeCharacters(section.mid(1));
                                    context.writeCharacters(tr("%1").arg(QChar(0x27E9)));
                                } else {
                                    context.writeCharacters(section);
                                }

                                context.writeEndElement();
                            }

                            break;
                        }

                        case LiteralElement::SectionType::MANTISSA_SIGN: {
                            if (inExponent) {
                                context.writeEndElement(); // mrow
                                context.writeEndElement(); // msup
                                inExponent = false;
                            }

                            context.writeStartElement("mo");
                            context.writeAttribute("stretchy", "false");
                            context.writeCharacters(section);
                            context.writeEndElement(); // mo

                            break;

                        }

                        case LiteralElement::SectionType::MANTISSA_VALUE: {
                            if (inExponent) {
                                context.writeEndElement(); // mrow
                                context.writeEndElement(); // msup
                                inExponent = false;
                            }

                            context.writeStartElement("mn");
                            context.writeCharacters(section);
                            context.writeEndElement(); // mn

                            break;
                        }

                        case LiteralElement::SectionType::EXPONENT_SYMBOL: {
                            context.writeStartElement("mo");
                            context.writeAttribute("stretchy", "false");
                            context.writeCharacters(QChar(0x00D7));
                            context.writeEndElement(); // mo

                            context.writeStartElement("msup");
                            context.writeStartElement("mn");
                            context.writeCharacters(tr("10"));
                            context.writeEndElement(); // mn
                            context.writeStartElement("mrow");

                            inExponent = true;
                            break;
                        }

                        case LiteralElement::SectionType::EXPONENT_SIGN: {
                            context.writeStartElement("mo");
                            context.writeAttribute("stretchy", "false");
                            context.writeCharacters(section);
                            context.writeEndElement(); // mo

                            break;
                        }

                        case LiteralElement::SectionType::EXPONENT_VALUE: {
                            context.writeStartElement("mn");
                            context.writeCharacters(section);
                            context.writeEndElement(); // mn

                            break;
                        }

                        case LiteralElement::SectionType::IMAGINARY_SYMBOL: {
                            if (inExponent) {
                                context.writeEndElement(); // mrow
                                context.writeEndElement(); // msup
                                inExponent = false;
                            }

                            context.writeStartElement("mi");
                            context.writeAttribute("stretchy", "false");
                            context.writeCharacters(section);
                            context.writeEndElement(); // mi

                            break;
                        }

                        default: {
                            assert(false);
                            break;
                        }
                    }
                }

                if (inExponent) {
                    context.writeEndElement(); // mrow
                    context.writeEndElement(); // msup
                }

                context.writeEndElement(); // mrow

                if (parenthesisStyle != Element::ParenthesisStyle::INVALID_OR_NONE) {
                    context.writeStartElement("mo");
                    context.writeAttribute("stretchy", "true");

                    switch (parenthesisStyle) {
                        case Element::ParenthesisStyle::INVALID_OR_NONE: {
                            break;
                        }

                        case Element::ParenthesisStyle::PARENTHESIS: {
                            context.writeCharacters(tr(")"));
                            break;
                        }

                        case Element::ParenthesisStyle::BRACKETS: {
                            context.writeCharacters(tr(")"));
                            break;
                        }

                        case Element::ParenthesisStyle::BRACES: {
                            context.writeCharacters(tr("}"));
                            break;
                        }

                        default: {
                            assert(false);
                            break;
                        }
                    }

                    context.writeEndElement(); // mo
                    context.writeEndElement(); // mrow
                }

                result = true;
                break;
            }

            case HtmlCodeGenerationEngine::MathMode::MATHJAX_LATEX: {
                switch (parenthesisStyle) {
                    case Element::ParenthesisStyle::INVALID_OR_NONE: {
                        break;
                    }

                    case Element::ParenthesisStyle::PARENTHESIS: {
                        context.writeCharacters(tr("\\left ("));
                        break;
                    }

                    case Element::ParenthesisStyle::BRACKETS: {
                        context.writeCharacters(tr("\\left ["));
                        break;
                    }

                    case Element::ParenthesisStyle::BRACES: {
                        context.writeCharacters(tr("\\left \\lbrace"));
                        break;
                    }

                    default: {
                        assert(false);
                        break;
                    }
                }

                bool inExponent = false;
                for (  LiteralElement::SectionList::const_iterator sectionIterator    = sections.constBegin(),
                                                                   sectionEndIterator = sections.constEnd()
                     ; sectionIterator != sectionEndIterator
                     ; ++sectionIterator
                    ) {
                    const LiteralElement::Section& section = *sectionIterator;
                    switch (section.sectionType()) {
                        case LiteralElement::SectionType::INVALID: {
                            assert(false);
                            break;
                        }

                        case LiteralElement::SectionType::STRING_CONTENT: {
                            if (!section.isEmpty()) {
                                QString leaderCharacter = section.at(0);

                                if (leaderCharacter == tr("\"")) {
                                    context.writeCharacters(tr("%1").arg(QChar(0x201C)));
                                    writeEscapedText(section.mid(1), engine, true);
                                    context.writeCharacters(tr("%1").arg(QChar(0x201D)));
                                } else if (leaderCharacter == tr("'")) {
                                    context.writeCharacters(tr("\\text{%1}").arg(QChar(0x2018)));
                                    writeEscapedText(section.mid(1), engine, true);
                                    context.writeCharacters(tr("\\text{%1}").arg(QChar(0x2019)));
                                } else if (leaderCharacter == tr("<")                     ||
                                           leaderCharacter == tr("%1").arg(QChar(0x27E8))    ) {
                                    context.writeCharacters(tr("\\text{%1}").arg(QChar(0x27E8)));
                                    writeEscapedText(section.mid(1), engine, true);
                                    context.writeCharacters(tr("\\text{%1}").arg(QChar(0x27E9)));
                                } else {
                                    context.writeCharacters(section);
                                }
                            }

                            break;
                        }

                        case LiteralElement::SectionType::MANTISSA_SIGN:
                        case LiteralElement::SectionType::MANTISSA_VALUE: {
                            if (inExponent) {
                                context.writeCharacters(tr(" } "));
                                inExponent = false;
                            }

                            context.writeCharacters(section);
                            break;
                        }

                        case LiteralElement::SectionType::EXPONENT_SYMBOL: {
                            context.writeCharacters(tr("\\times 10 ^{ "));
                            inExponent = true;
                            break;
                        }

                        case LiteralElement::SectionType::EXPONENT_SIGN:
                        case LiteralElement::SectionType::EXPONENT_VALUE: {
                            assert(inExponent);
                            context.writeCharacters(section);
                            break;
                        }

                        case LiteralElement::SectionType::IMAGINARY_SYMBOL: {
                            if (inExponent) {
                                context.writeCharacters(tr(" } "));
                                inExponent = false;
                            }

                            context.writeCharacters(tr("\\text{ %1 }").arg(section));
                            break;
                        }

                        default: {
                            assert(false);
                            break;
                        }
                    }
                }

                if (inExponent) {
                    context.writeCharacters(tr(" } "));
                }

                switch (parenthesisStyle) {
                    case Element::ParenthesisStyle::INVALID_OR_NONE: {
                        break;
                    }

                    case Element::ParenthesisStyle::PARENTHESIS: {
                        context.writeCharacters(tr("\\right )"));
                        break;
                    }

                    case Element::ParenthesisStyle::BRACKETS: {
                        context.writeCharacters(tr("\\right ]"));
                        break;
                    }

                    case Element::ParenthesisStyle::BRACES: {
                        context.writeCharacters(tr("\\right \\rbrace"));
                        break;
                    }

                    default: {
                        assert(false);
                        break;
                    }
                }

                result = true;
                break;
            }

            case HtmlCodeGenerationEngine::MathMode::IMAGES: {
                result = writeAsImage(element, engine);
                break;
            }

            default: {
                result = false;

                assert(false);
                break;
            }
        }

        return result;
    }
}
