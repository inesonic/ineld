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
* This file implements the \ref Ld::LaTeXTextElementTranslator class.
***********************************************************************************************************************/

#include <QString>
#include <QColor>

#include "ld_element.h"
#include "ld_element_structures.h"
#include "ld_text_element.h"
#include "ld_format.h"
#include "ld_format_structures.h"
#include "ld_font_format.h"
#include "ld_character_format.h"
#include "ld_text_export_context.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_latex_translator.h"
#include "ld_latex_text_element_translator.h"

namespace Ld {
    LaTeXTextElementTranslator::~LaTeXTextElementTranslator() {}


    QString LaTeXTextElementTranslator::elementName() const {
        return TextElement::elementName;
    }


    bool LaTeXTextElementTranslator::identifyDependencies(ElementPointer element, LaTeXCodeGenerationEngine& engine) {
        FormatPointer rawFormat = element->format();
        if (!rawFormat.isNull() && rawFormat->capabilities().contains(FontFormat::formatName)) {
            QSharedPointer<FontFormat> format = rawFormat.dynamicCast<FontFormat>();
            if (format->strikeout()) {
                engine.addPackage("cancel");
            }

            if (format->overline()) {
                engine.addPreamble(
                    "\\newcommand{\\inetextol}[1]{$\\overline{\\hbox{#1}}\\m@th$}",
                    LaTeXCodeGenerationEngine::nominalNewCommandRelativeOrdering
                );
            }

            if (format->fontColor().isValid()) {
                engine.addPackage("xcolor");
            }

            if (format->fontBackgroundColor().isValid()) {
                engine.addPackage("soul");
                engine.addPreamble(
                    "\\newcommand{\\inetexthlc}[2][yellow]{{\\sethlcolor{$1}\\hl{#2}}}",
                    LaTeXCodeGenerationEngine::nominalNewCommandRelativeOrdering
                );
            }
        }

        return true;
    }


    bool LaTeXTextElementTranslator::body(ElementPointer element, LaTeXCodeGenerationEngine& engine) {
        TextExportContext&   context = engine.context();
        QString              text    = element.dynamicCast<TextElement>()->text();

        if (!text.isEmpty()) {
            QString escaped = escapeText(text, engine.unicodeTranslationMode(), true);

            FormatPointer rawFormat = element->format();
            if (!rawFormat.isNull() && rawFormat->capabilities().contains(FontFormat::formatName)) {
                QSharedPointer<FontFormat> format = rawFormat.dynamicCast<FontFormat>();
                if (static_cast<int>(format->fontWeight()) > static_cast<int>(FontFormat::Weight::NORMAL)) {
                    escaped = QString("\\textbf{%1}").arg(escaped);
                }

                if (format->italics()) {
                    escaped = QString("\\textit{%1}").arg(escaped);
                }

                if (format->underline()) {
                    escaped = QString("\\underline{%1}").arg(escaped);
                }

                if (format->overline()) {
                    escaped = QString("\\inetextol{%1}").arg(escaped);
                }

                if (format->strikeout()) {
                    escaped = QString("\\cancel{%1}").arg(escaped);
                }

                QColor fontColor = format->fontColor();
                if (fontColor.isValid()) {
                    escaped = QString("\\textcolor[]{%1,%2,%3,%4}{%5}")
                              .arg(fontColor.cyanF())
                              .arg(fontColor.magentaF())
                              .arg(fontColor.yellowF())
                              .arg(fontColor.blackF())
                              .arg(escaped);
                }

                QColor fontBackgroundColor = format->fontBackgroundColor();
                if (fontBackgroundColor.isValid()) {
                    escaped = QString("\\inetexthlc[%1,%2,%3,%4]{%5}")
                              .arg(fontBackgroundColor.cyanF())
                              .arg(fontBackgroundColor.magentaF())
                              .arg(fontBackgroundColor.yellowF())
                              .arg(fontBackgroundColor.blackF())
                              .arg(escaped);
                }
            }

            escaped += ' ';

            while (!escaped.isEmpty()) {
                unsigned long currentWidth = context.columnNumber();
                if (currentWidth >= maximumTextWidth) {
                    context << "\n";
                } else {
                    int nextWhitespace = escaped.indexOf(' ');
                    if (nextWhitespace < 0) {
                        if (currentWidth == 0) {
                            context << escaped;
                            escaped.clear();
                        } else {
                            context << '\n';
                        }
                    } else {
                        long remainingThisLine = maximumTextWidth - currentWidth;
                        if (nextWhitespace > remainingThisLine) {
                            if (currentWidth == 0) {
                                context << escaped.left(nextWhitespace);
                                escaped = escaped.left(nextWhitespace + 1);
                            }

                            context << '\n';
                        } else {
                            int bestWhitespace = -1;
                            while (nextWhitespace >= 0 && nextWhitespace <= remainingThisLine) {
                                bestWhitespace = nextWhitespace;
                                nextWhitespace = escaped.indexOf(' ',nextWhitespace + 1);
                            }

                            assert(bestWhitespace >= 0);

                            context << escaped.left(bestWhitespace);
                            escaped = escaped.mid(bestWhitespace + 1);
                        }
                    }
                }
            }

            if (text.at(text.length() - 1).isSpace() && context.columnNumber() != 0) {
                context << ' ';
            }
        }

        return true;
    }
}
