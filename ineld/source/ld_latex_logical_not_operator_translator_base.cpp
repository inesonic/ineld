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
* This file implements the \ref Ld::LaTeXLogicalNotOperatorTranslatorBase class.
***********************************************************************************************************************/

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>

#include "ld_format_structures.h"
#include "ld_logical_unary_not_operator_format.h"
#include "ld_text_export_context.h"
#include "ld_code_generation_engine.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_latex_operator_translator_base.h"
#include "ld_latex_logical_not_operator_translator_base.h"

namespace Ld {
    LaTeXLogicalNotOperatorTranslatorBase::~LaTeXLogicalNotOperatorTranslatorBase() {}


    bool LaTeXLogicalNotOperatorTranslatorBase::body(ElementPointer element, LaTeXCodeGenerationEngine& engine) {
        LogicalUnaryNotOperatorFormat::LogicalNotSymbol symbol   = logicalNotStyle(element->format());
        TextExportContext&                              context  = engine.context();
        ElementPointer                                  child0   = element->child(0);

        QString leftParenthesis;
        QString rightParenthesis;
        bool    requiresParenthesis = includeParenthesis(element, leftParenthesis, rightParenthesis);

        QString operatorString;
        if (symbol == LogicalUnaryNotOperatorFormat::LogicalNotSymbol::OVERBAR) {
            context << tr("\\overline{ ");
        } else {
            if (symbol == LogicalUnaryNotOperatorFormat::LogicalNotSymbol::TILDE) {
                operatorString = tr("{\\raise.18ex\\hbox{$\\scriptstyle\\sim$}}");
            } else {
                operatorString = tr("{\\neg}");
            }
        }

        if (requiresParenthesis) {
            context << leftParenthesis;
        }

        if (!operatorString.isEmpty()) {
            context << operatorString;
        }

        engine.translateChild(child0);

        if (requiresParenthesis) {
            context << rightParenthesis;
        }

        if (symbol == LogicalUnaryNotOperatorFormat::LogicalNotSymbol::OVERBAR) {
            context << tr(" }");
        }

        return true;
    }


    LogicalUnaryNotOperatorFormat::LogicalNotSymbol LaTeXLogicalNotOperatorTranslatorBase::logicalNotStyle(
            Ld::FormatPointer format
        ) const {
        Ld::LogicalUnaryNotOperatorFormat::LogicalNotSymbol result;

        if (format->capabilities().contains(Ld::LogicalUnaryNotOperatorFormat::formatName)) {
            QSharedPointer<Ld::LogicalUnaryNotOperatorFormat>
                logicalNotFormat = format.dynamicCast<Ld::LogicalUnaryNotOperatorFormat>();

            result = logicalNotFormat->logicalNotSymbol();

            if (result == Ld::LogicalUnaryNotOperatorFormat::LogicalNotSymbol::DEFAULT) {
                result = defaultLogicalNotSymbol();
            }
        } else {
            result = defaultLogicalNotSymbol();
        }

        return result;
    }
}
