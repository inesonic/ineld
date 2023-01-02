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
* This file implements the \ref Ld::HtmlRange3Translator class.
***********************************************************************************************************************/

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QChar>

#include "ld_format_structures.h"
#include "ld_range_3_element.h"
#include "ld_xml_export_context.h"
#include "ld_html_code_generation_engine.h"
#include "ld_html_range_3_translator.h"

namespace Ld {
    HtmlRange3Translator::~HtmlRange3Translator() {}


    QString HtmlRange3Translator::elementName() const {
        return Ld::Range3Element::elementName;
    }


    bool HtmlRange3Translator::buildMathMlOperator(ElementPointer element, HtmlCodeGenerationEngine& engine) {
        XmlExportContext& context = engine.context();

        Ld::ElementPointer child0 = element->child(0);
        Ld::ElementPointer child1 = element->child(1);
        Ld::ElementPointer child2 = element->child(2);

        engine.translateChild(child0);

        context.writeStartElement("mi");
        context.writeCharacters(",");
        context.writeEndElement(); // mi

        engine.translateChild(child1);

        context.writeStartElement("mi");
        context.writeCharacters(tr(",%1,").arg(QChar(0x2026)));
        context.writeEndElement();

        engine.translateChild(child2);

        return true;
    }


    bool HtmlRange3Translator::buildLaTeXOperator(ElementPointer element, HtmlCodeGenerationEngine& engine) {
        XmlExportContext& context = engine.context();

        Ld::ElementPointer child0 = element->child(0);
        Ld::ElementPointer child1 = element->child(1);
        Ld::ElementPointer child2 = element->child(2);

        engine.translateChild(child0);
        context.writeCharacters(tr(", "));
        engine.translateChild(child1);
        context.writeCharacters(tr(",\\ldots, "));
        engine.translateChild(child2);

        return true;
    }
}
