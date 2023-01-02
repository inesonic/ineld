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
* This file implements the \ref Ld::HtmlNearestIntegerOperatorTranslator class.
***********************************************************************************************************************/

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS
#include <QString>

#include "ld_format_structures.h"
#include "ld_nearest_integer_operator_element.h"
#include "ld_html_grouping_operator_translator_base.h"
#include "ld_html_nearest_integer_operator_translator.h"

namespace Ld {
    HtmlNearestIntegerOperatorTranslator::~HtmlNearestIntegerOperatorTranslator() {}


    QString HtmlNearestIntegerOperatorTranslator::elementName() const {
        return NearestIntegerOperatorElement::elementName;
    }


    QString HtmlNearestIntegerOperatorTranslator::mathMlLeftParenthesis(ElementPointer /* element */) const {
        return tr("%1").arg(QChar(0x23A3));
    }


    QString HtmlNearestIntegerOperatorTranslator::mathMlRightParenthesis(ElementPointer /* element */) const {
        return tr("%1").arg(QChar(0x23A4));
    }


    QString HtmlNearestIntegerOperatorTranslator::latexLeftParenthesis(ElementPointer /* element */) const {
        return QString("\\lfloor");
    }


    QString HtmlNearestIntegerOperatorTranslator::latexRightParenthesis(ElementPointer /* element */) const {
        return QString("\\rceil");
    }
}
