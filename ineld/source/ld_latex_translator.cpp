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
* This file implements the \ref Ld::LaTeXTranslator class.
***********************************************************************************************************************/

#include <QString>
#include <QChar>
#include <QSharedPointer>
#include <QUrl>
#include <QMap>

#include "ld_element.h"
#include "ld_element_structures.h"
#include "ld_special_characters.h"
#include "ld_format.h"
#include "ld_format_structures.h"
#include "ld_text_export_context.h"
#include "ld_code_generation_engine.h"
#include "ld_latex_translation_phase.h"
#include "ld_latex_code_generator_diagnostic.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_latex_translator.h"

namespace Ld {
    QMap<SpecialCharacters::UnicodeValue, QString> LaTeXTranslator::commandsByUnicodeValue;

    LaTeXTranslator::~LaTeXTranslator() {}

    bool LaTeXTranslator::translate(ElementPointer element, CodeGenerationEngine& codeGenerationEngine) {
        bool  success;

        LaTeXCodeGenerationEngine& engine = dynamic_cast<LaTeXCodeGenerationEngine&>(codeGenerationEngine);

        const LaTeXTranslationPhase& laTeXTranslationPhase =
            dynamic_cast<const LaTeXTranslationPhase&>(codeGenerationEngine.translationPhase());

        switch (laTeXTranslationPhase.phase()) {
            case LaTeXTranslationPhase::Phase::IDENTIFY_DEPENDENCIES: {
                success = identifyDependencies(element, engine);
                break;
            }

            case LaTeXTranslationPhase::Phase::BODY: {
                success = body(element, engine);
                break;
            }

            default: {
                LaTeXCodeGeneratorDiagnostic* diagnostic = new LaTeXCodeGeneratorDiagnostic(
                    element,
                    LaTeXCodeGeneratorDiagnostic::Type::INTERNAL_ERROR,
                    laTeXTranslationPhase,
                    LaTeXCodeGeneratorDiagnostic::Code::UNEXPECTED_TRANSLATION_PHASE,
                    engine.contextPointer()
                );

                engine.translationErrorDetected(diagnostic);

                success = false;
                break;
            }
        }

        return success;
    }


    bool LaTeXTranslator::identifyDependencies(ElementPointer, LaTeXCodeGenerationEngine&) {
        return true;
    }


    bool LaTeXTranslator::body(ElementPointer, LaTeXCodeGenerationEngine&) {
        return true;
    }


    QString LaTeXTranslator::escapeText(
            const QString&                         rawText,
            LaTeXCodeGenerationEngine::UnicodeMode unicodeTranslationMode,
            bool                                   textMode
        ) {
        QString  result;
        unsigned length = static_cast<unsigned>(rawText.length());

        for (unsigned index=0 ; index<length ; ++index) {
            QChar c = rawText.at(index);
            if (c == QChar('$')) {
                result += "\\$";
            } else if (c == QChar('%')) {
                result += "\\%";
            } else if (c == QChar('&')) {
                result += "\\&";
            } else if (c == QChar('#')) {
                result += "\\#";
            } else if (c == QChar('_')) {
                result += "\\_";
            } else if (c == QChar('{')) {
                result += "\\{";
            } else if (c == QChar('}')) {
                result += "\\}";
            } else if (c == QChar('~')) {
                if (textMode) {
                    result += "\\textasciitilde";
                } else {
                    result += "\\sim";
                }
            } else if (c == QChar('^')) {
                if (textMode) {
                    result += "\\textasciicircum";
                } else {
                    result += "\\^{}";
                }
            } else if (c == QChar('\\')) {
                if (textMode) {
                    result += "\\textbackslash";
                } else {
                    result += "\\backslash";
                }
            } else {
                SpecialCharacters::UnicodeValue unicodeValue = c.unicode();
                if (unicodeValue > 0x7F) {
                    switch (unicodeTranslationMode) {
                        case LaTeXCodeGenerationEngine::UnicodeMode::INSERT_UNICODE: {
                            result += c;
                            break;
                        }

                        case LaTeXCodeGenerationEngine::UnicodeMode::CONVERT_TO_HAT_NOTATION: {
                            QString  unicodeString       = QString::number(unicodeValue, 16);
                            unsigned unicodeStringLength = static_cast<unsigned>(unicodeString.length());

                            result += QString("^").repeated(unicodeStringLength) + unicodeString;
                            break;
                        }

                        case LaTeXCodeGenerationEngine::UnicodeMode::CONVERT_TO_LATEX_COMMAND: {
                            if (commandsByUnicodeValue.isEmpty()) {
                                commandsByUnicodeValue = SpecialCharacters::characterCommandsByUnicodeValue(
                                    true, // Greek
                                    true, // Diacriticals
                                    true, // other special characters
                                    true  // other special symbols
                                );
                            }

                            if (commandsByUnicodeValue.contains(unicodeValue)) {
                                QString command = commandsByUnicodeValue.value(unicodeValue);
                                if (textMode) {
                                    result += QString("$\\%1$").arg(command);
                                } else {
                                    result += QString("\\%1").arg(command);
                                }
                            } else {
                                result += QString("\textbf{!!Unknown Character U%1!!}")
                                          .arg(unicodeValue, 4, 16, QChar('0'));
                            }
                        }
                    }
                } else {
                    result += c;
                }
            }
        }

        return result;
    }
}
