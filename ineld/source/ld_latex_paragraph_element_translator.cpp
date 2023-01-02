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
* This file implements the \ref Ld::LaTeXParagraphElementTranslator class.
***********************************************************************************************************************/

#include <QString>
#include <QRegularExpression>

#include "ld_element.h"
#include "ld_element_structures.h"
#include "ld_format.h"
#include "ld_format_structures.h"
#include "ld_justified_block_format.h"
#include "ld_paragraph_format.h"
#include "ld_list_additional_paragraph_format.h"
#include "ld_ordered_list_paragraph_format.h"
#include "ld_unordered_list_paragraph_format.h"
#include "ld_paragraph_element.h"
#include "ld_text_export_context.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_latex_translator.h"
#include "ld_latex_paragraph_element_translator.h"

namespace Ld {
    const QRegularExpression LaTeXParagraphElementTranslator::listSpecialCharacters("([1aAiI])");

    LaTeXParagraphElementTranslator::~LaTeXParagraphElementTranslator() {}


    QString LaTeXParagraphElementTranslator::elementName() const {
        return ParagraphElement::elementName;
    }


    bool LaTeXParagraphElementTranslator::identifyDependencies(
            ElementPointer             element,
            LaTeXCodeGenerationEngine& engine
        ) {
        FormatPointer format = element->format();
        if (!format.isNull()) {
            if (format->capabilities().contains(OrderedListParagraphFormat::formatName)   ||
                format->capabilities().contains(UnorderedListParagraphFormat::formatName)    ) {
                engine.addPackage("enumitem");
            }

            engine.addPackage("ragged2e");
        }

        return true;
    }


    bool LaTeXParagraphElementTranslator::body(ElementPointer element, LaTeXCodeGenerationEngine& engine) {
        bool                 success;
        TextExportContext&   context      = engine.context();
        FormatPointer        rawFormat    = element->format();
        Format::Capabilities capabilities = rawFormat.isNull() ? Format::Capabilities() : rawFormat->capabilities();

        if (capabilities.contains(OrderedListParagraphFormat::formatName)) {
            QSharedPointer<OrderedListParagraphFormat> format = rawFormat.dynamicCast<OrderedListParagraphFormat>();

            if (context.columnNumber() > 0) {
                context << "\n\n";
            }

            addRagged2eEnvironment(format, engine);

            QString prefix = escapedTextForOrderedList(format->prefix(), engine.unicodeTranslationMode());
            QString suffix = escapedTextForOrderedList(format->suffix(), engine.unicodeTranslationMode());

            QString options;
            switch (format->style()) {
                case OrderedListParagraphFormat::Style::INVALID:
                case OrderedListParagraphFormat::Style::NUMBER: {
                    if (!prefix.isEmpty() || !suffix.isEmpty()) {
                        options = QString("label={%1\\arabic*%2}").arg(prefix).arg(suffix);
                    }

                    break;
                }

                case OrderedListParagraphFormat::Style::LOWER_CASE: {
                    options = QString("label={%1\\alph*%2}").arg(prefix).arg(suffix);
                    break;
                }

                case OrderedListParagraphFormat::Style::UPPER_CASE: {
                    options = QString("label={%1\\Alph*%2}").arg(prefix).arg(suffix);
                    break;
                }

                case OrderedListParagraphFormat::Style::ROMAN_LOWER_CASE: {
                    options = QString("label={%1\\roman*%2}").arg(prefix).arg(suffix);
                    break;
                }

                case OrderedListParagraphFormat::Style::ROMAN_UPPER_CASE: {
                    options = QString("label={%1\\Roman*%2}").arg(prefix).arg(suffix);
                    break;
                }

                default: {
                    assert(false);
                    break;
                }
            }

            float leftIndentation = format->leftIndentation();
            if (leftIndentation != 0.0F) {
                if (!options.isEmpty()) {
                    options += ", ";
                }

                options += QString("leftmargin=%1pt").arg(leftIndentation);
            }

            if (!options.isEmpty()) {
                context << QString("\\begin{enumerate}[%1]\n").arg(options);
            } else {
                context << QString("\\begin{enumerate}\n");
            }

            if (format->startingNumber() != 1) {
                context << QString("\\setcounter{enumi}{%1}\n").arg(static_cast<int>(format->startingNumber()) - 1);
            }

            context << "\\item ";
            success = translateAllChildren(element, engine);

            if (success) {
                context << "\n\n";
                success = endListIfNeeded(element, engine);
            }
        } else if (capabilities.contains(UnorderedListParagraphFormat::formatName)) {
            QSharedPointer<UnorderedListParagraphFormat>
                format = rawFormat.dynamicCast<UnorderedListParagraphFormat>();

            if (context.columnNumber() > 0) {
                context << "\n\n";
            }

            addRagged2eEnvironment(format, engine);

            QString bullet  = escapeText(format->bulletStyle(), engine.unicodeTranslationMode(), true);
            QString options = QString("label={%1}").arg(bullet);

            float leftIndentation = format->leftIndentation();
            if (leftIndentation != 0) {
                if (!options.isEmpty()) {
                    options += ", ";
                }

                options += QString("leftmargin=%1pt").arg(leftIndentation);
            }

            if (!options.isEmpty()) {
                context << QString("\\begin{itemize}[%1]\n").arg(options);
            } else {
                context << QString("\\begin{itemize}\n");
            }

            context << "\\item ";
            success = translateAllChildren(element, engine);

            if (success) {
                context << "\n\n";
                success = endListIfNeeded(element, engine);
            }
        } else if (capabilities.contains(ListAdditionalParagraphFormat::formatName)) {
            QSharedPointer<ListAdditionalParagraphFormat> format =
                rawFormat.dynamicCast<ListAdditionalParagraphFormat>();

            if (context.columnNumber() > 0) {
                context << "\n\n";
            }

            if (format->bulletNumberDisplayEnabled()) {
                context << "\\item ";
            }

            success = translateAllChildren(element, engine);

            if (success) {
                context << "\n\n";
                success = endListIfNeeded(element, engine);
            }
        } else {
            // We assume we have a regular paragraph.
            if (context.columnNumber() > 0) {
                context << "\n\n";
            }

            addRagged2eEnvironment(rawFormat, engine);

            success = translateAllChildren(element, engine);

            if (success) {
                context << "\n\n";
            }
        }

        return success;
    }


    QString LaTeXParagraphElementTranslator::escapedTextForOrderedList(
            const QString&                         unescapedText,
            LaTeXCodeGenerationEngine::UnicodeMode unicodeTranslationMode
        ) {
        return escapeText(unescapedText, unicodeTranslationMode, true).replace(listSpecialCharacters, "{\1}");
    }


    void LaTeXParagraphElementTranslator::addRagged2eEnvironment(
            FormatPointer              format,
            LaTeXCodeGenerationEngine& engine
        ) {
        if (format->capabilities().contains(JustifiedBlockFormat::formatName)) {
            QSharedPointer<JustifiedBlockFormat> justifiedFormat = format.dynamicCast<JustifiedBlockFormat>();
            TextExportContext&                   context         = engine.context();

            switch (justifiedFormat->justificationMode()) {
                case JustifiedBlockFormat::Justification::LEFT: {
                    context << "\\RaggedRight\n";
                    break;
                }

                case JustifiedBlockFormat::Justification::CENTER: {
                    context << "\\Centering\n";
                    break;
                }

                case JustifiedBlockFormat::Justification::RIGHT: {
                    context << "\\RaggedLeft\n";
                    break;
                }

                case JustifiedBlockFormat::Justification::JUSTIFY: {
                    context << "\\justifying\n";
                    break;
                }

                default: {
                    context << "\\justifying\n";
                    break;
                }
            }
        }
    }


    bool LaTeXParagraphElementTranslator::endListIfNeeded(ElementPointer element, LaTeXCodeGenerationEngine& engine) {
        ElementPointer nextSibling = element->nextSibling();
        if (nextSibling.isNull()                                                                             ||
            (!nextSibling->format().isNull()                                                            &&
             !nextSibling->format()->capabilities().contains(ListAdditionalParagraphFormat::formatName)    )    ) {
            ElementPointer listTop = element;
            while (!listTop.isNull() &&
                   !listTop->format().isNull()                                                           &&
                   listTop->format()->capabilities().contains(ListAdditionalParagraphFormat::formatName)    ) {
                listTop = listTop->previousSibling();
            }

            if (!listTop.isNull() && !listTop->format().isNull()) {
                TextExportContext& context       = engine.context();
                FormatPointer      listTopFormat = listTop->format();
                if (listTopFormat->capabilities().contains(OrderedListParagraphFormat::formatName)) {
                    context << "\\end{enumerate}\n\n";
                } else {
                    context << "\\end{itemize}\n\n";
                }
            }
        }

        return true;
    }
}
