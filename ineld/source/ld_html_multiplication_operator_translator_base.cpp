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
* This file implements the \ref Ld::HtmlMultiplicationOperatorTranslatorBase class.
***********************************************************************************************************************/

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QChar>

#include <cassert>

#include "ld_format_structures.h"
#include "ld_multiplication_operator_format.h"
#include "ld_multiplication_operator_element.h"
#include "ld_html_multiplication_operator_translator_base.h"

namespace Ld {
    HtmlMultiplicationOperatorTranslatorBase::~HtmlMultiplicationOperatorTranslatorBase() {}


    QString HtmlMultiplicationOperatorTranslatorBase::mathMlOperator(FormatPointer operatorFormat) const {
        QString result;

        MultiplicationOperatorFormat::MultiplicationSymbol symbol = multiplicationSymbol(operatorFormat);
        switch (symbol) {
            case MultiplicationOperatorFormat::MultiplicationSymbol::INVALID: {
                assert(false);
                break;
            }

            case MultiplicationOperatorFormat::MultiplicationSymbol::DEFAULT:
            case MultiplicationOperatorFormat::MultiplicationSymbol::SPACE: {
                result = QString();
                break;
            }


            case MultiplicationOperatorFormat::MultiplicationSymbol::DOT: {
                result = QString(QChar(0x2219));
                break;
            }

            case MultiplicationOperatorFormat::MultiplicationSymbol::CROSS: {
                result = QString(QChar(0x00D7));
                break;
            }

            case MultiplicationOperatorFormat::MultiplicationSymbol::CIRCLE: {
                result = QString(QChar(0x2218));
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    QString HtmlMultiplicationOperatorTranslatorBase::latexOperator(FormatPointer operatorFormat) const {
        QString result;

        MultiplicationOperatorFormat::MultiplicationSymbol symbol = multiplicationSymbol(operatorFormat);
        switch (symbol) {
            case MultiplicationOperatorFormat::MultiplicationSymbol::INVALID: {
                assert(false);
                break;
            }

            case MultiplicationOperatorFormat::MultiplicationSymbol::DEFAULT:
            case MultiplicationOperatorFormat::MultiplicationSymbol::SPACE: {
                result = tr(" ");
                break;
            }

            case MultiplicationOperatorFormat::MultiplicationSymbol::DOT: {
                result = tr("\\cdot");
                break;
            }

            case MultiplicationOperatorFormat::MultiplicationSymbol::CROSS: {
                result = tr("\\times");
                break;
            }

            case MultiplicationOperatorFormat::MultiplicationSymbol::CIRCLE: {
                result = tr("\\circ");
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    MultiplicationOperatorFormat::MultiplicationSymbol HtmlMultiplicationOperatorTranslatorBase::multiplicationSymbol(
            FormatPointer format
        ) const {
        MultiplicationOperatorFormat::MultiplicationSymbol result;

        if (!format.isNull() && format->capabilities().contains(MultiplicationOperatorFormat::formatName)) {
            QSharedPointer<MultiplicationOperatorFormat> multiplicationFormat =
                format.dynamicCast<MultiplicationOperatorFormat>();

            result = multiplicationFormat->multiplicationSymbol();

            if (result == MultiplicationOperatorFormat::MultiplicationSymbol::DEFAULT) {
                result = defaultSymbol();
            }
        } else {
            result = defaultSymbol();
        }

        return result;
    }
}
