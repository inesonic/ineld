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
* This file implements the \ref Ld::LaTeXLiteralTranslator class.
***********************************************************************************************************************/

#include <QString>

#include <cassert>

#include "ld_element_structures.h"
#include "ld_literal_element.h"
#include "ld_operator_format.h"
#include "ld_text_export_context.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_latex_translator.h"
#include "ld_latex_literal_translator.h"

namespace Ld {
    LaTeXLiteralTranslator::~LaTeXLiteralTranslator() {}


    QString LaTeXLiteralTranslator::elementName() const {
        return LiteralElement::elementName;
    }


    bool LaTeXLiteralTranslator::body(ElementPointer element, LaTeXCodeGenerationEngine& engine) {
        TextExportContext&             context          = engine.context();
        QSharedPointer<LiteralElement> literalElement   = element.dynamicCast<LiteralElement>();
        Element::ParenthesisStyle      parenthesisStyle = element->parenthesisStyle();

        switch (parenthesisStyle) {
            case Element::ParenthesisStyle::INVALID_OR_NONE: {
                break;
            }

            case Element::ParenthesisStyle::PARENTHESIS: {
                context << tr("\\left (");
                break;
            }

            case Element::ParenthesisStyle::BRACKETS: {
                context << tr("\\left [");
                break;
            }

            case Element::ParenthesisStyle::BRACES: {
                context << tr("\\left \\lbrace");
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        LiteralElement::SectionList sections = literalElement->section();

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
                            context << tr(
                                "\\text{``%1''}"
                            ).arg(escapeText(section.mid(1), engine.unicodeTranslationMode(), true));
                        } else if (leaderCharacter == tr("'")) {
                            context << tr(
                                "\\text{\\lq%1\\rq}"
                            ).arg(escapeText(section.mid(1), engine.unicodeTranslationMode(), true));
                        } else if (leaderCharacter == tr("<")                     ||
                                   leaderCharacter == tr("%1").arg(QChar(0x27E8))    ) {
                            context << tr(
                                "\\left \\langle \\text{%1} \\right \\rangle"
                            ).arg(escapeText(section.mid(1), engine.unicodeTranslationMode(), true));
                        } else {
                            context << tr(
                                "\\text{%1}"
                            ).arg(escapeText(section.mid(1), engine.unicodeTranslationMode(), true));
                        }
                    }

                    break;
                }

                case LiteralElement::SectionType::MANTISSA_SIGN:
                case LiteralElement::SectionType::MANTISSA_VALUE: {
                    if (inExponent) {
                        context << tr(" } ");
                        inExponent = false;
                    }

                    context << section;
                    break;
                }

                case LiteralElement::SectionType::EXPONENT_SYMBOL: {
                    context << tr("\\times 10 ^{ ");
                    inExponent = true;
                    break;
                }

                case LiteralElement::SectionType::EXPONENT_SIGN:
                case LiteralElement::SectionType::EXPONENT_VALUE: {
                    assert(inExponent);
                    context << section;
                    break;
                }

                case LiteralElement::SectionType::IMAGINARY_SYMBOL: {
                    if (inExponent) {
                        context << tr(" } ");
                        inExponent = false;
                    }

                    context << tr("\\text{ %1 }").arg(section);
                    break;
                }

                default: {
                    assert(false);
                    break;
                }
            }
        }

        if (inExponent) {
            context << tr(" } ");
        }

        switch (parenthesisStyle) {
            case Element::ParenthesisStyle::INVALID_OR_NONE: {
                break;
            }

            case Element::ParenthesisStyle::PARENTHESIS: {
                context << tr("\\right )");
                break;
            }

            case Element::ParenthesisStyle::BRACKETS: {
                context << tr("\\right ]");
                break;
            }

            case Element::ParenthesisStyle::BRACES: {
                context << tr("\\right \\rbrace");
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        return true;
    }
}
