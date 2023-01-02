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
* This file implements the \ref Ld::HtmlMatrixDataTypeTranslator class.
***********************************************************************************************************************/

#include <QString>

#include <model_matrix.h>

#include "ld_html_code_generation_engine.h"
#include "ld_xml_export_context.h"
#include "ld_matrix_data_type_format.h"
#include "ld_html_data_type_translator.h"
#include "ld_html_matrix_data_type_translator.h"

/***********************************************************************************************************************
 * HtmlMatrixDataTypeTranslator::Context:
 */

namespace Ld {
    HtmlMatrixDataTypeTranslator::Context::Context(
            const Model::Matrix& /* matrix */,
            FormatPointer        /* format */
        ) {}


    HtmlMatrixDataTypeTranslator::Context::~Context() {}


    void HtmlMatrixDataTypeTranslator::Context::insertText(
            const QString&            str,
            FormatPointer             format,
            HtmlCodeGenerationEngine& engine
        ) {
        XmlExportContext& context       = engine.context();
        unsigned          numberEndTags = 0;

        if (engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML5_WITH_CSS) {
            QString className = engine.cssClassForFormat(format);
            if (!className.isEmpty()) {
                context.writeStartElement("span");
                context.writeAttribute("class", className);
                numberEndTags = 1;
            }
        } else if (engine.htmlStyle() == HtmlCodeGenerationEngine::HtmlStyle::HTML4_WITHOUT_CSS) {
            QSharedPointer<FontFormat> fontFormat = format.dynamicCast<FontFormat>();
            if (!fontFormat.isNull()) {
                context.writeStartElement("font");
                context.writeAttribute("face", fontFormat->family());

                int size = std::max(1, std::min(7, 3 + (static_cast<int>(fontFormat->fontSize()) - 12) / 2));
                if (size != 3) {
                    context.writeAttribute("size", QString::number(size));
                }

                QColor fontColor = fontFormat->fontColor();
                if (fontColor.isValid()) {
                    context.writeAttribute("color", fontColor.name());
                }

                numberEndTags = 1;

                if (fontFormat->italics()) {
                    context.writeStartElement("i");
                    ++numberEndTags;
                }

                unsigned fontWeightValue   = static_cast<unsigned>(fontFormat->fontWeight());
                unsigned normalWeightValue = static_cast<unsigned>(FontFormat::Weight::NORMAL);
                if (fontWeightValue > normalWeightValue) {
                    context.writeStartElement("b");
                    ++numberEndTags;
                }

                if (fontFormat->underline()) {
                    context.writeStartElement("u");
                    ++numberEndTags;
                }

                if (fontFormat->strikeout()) {
                    context.writeStartElement("s");
                    ++numberEndTags;
                }
            }
        }

        context.writeCharacters(str);

        while (numberEndTags > 0) {
            context.writeEndElement();
            --numberEndTags;
        }
    }
}

/***********************************************************************************************************************
 * HtmlMatrixDataTypeTranslator:
 */

namespace Ld {
    HtmlMatrixDataTypeTranslator::HtmlMatrixDataTypeTranslator() {}


    HtmlMatrixDataTypeTranslator::~HtmlMatrixDataTypeTranslator() {}


    QString HtmlMatrixDataTypeTranslator::elementName() const {
        return QString();
    }


    bool HtmlMatrixDataTypeTranslator::translateToMathML(
            Context&                  context,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
        ) const {
        bool success = true;

        unsigned long numberRows            = context.numberRows();
        unsigned long numberColumns         = context.numberColumns();
        unsigned long numberLeadingRows     = context.numberLeadingRows();
        unsigned long numberTrailingRows    = context.numberTrailingRows();
        unsigned long firstTrailingRow      = context.firstTrailingRow();
        unsigned long numberLeadingColumns  = context.numberLeadingColumns();
        unsigned long numberTrailingColumns = context.numberTrailingColumns();
        unsigned long firstTrailingColumn   = context.firstTrailingColumn();

        XmlExportContext& xmlContext = engine.context();

        xmlContext.writeStartElement("math");
        xmlContext.writeAttribute("xmlns", "http://www.w3.org/1998/Math/MathML");
        xmlContext.writeAttribute("display", "inline");

        xmlContext.writeStartElement("semantics");
        xmlContext.writeStartElement("mrow");

        xmlContext.writeStartElement("mo");
        xmlContext.writeAttribute("fence", "true");
        xmlContext.writeAttribute("stretchy", "true");
        xmlContext.writeCharacters(tr("["));
        xmlContext.writeEndElement(); // mo

        xmlContext.writeStartElement("mrow");
        xmlContext.writeStartElement("mtable");

        unsigned long totalNumberColumns = numberLeadingColumns + numberTrailingColumns;
        if (numberTrailingRows > 0 || numberLeadingColumns < numberColumns) {
            ++totalNumberColumns;
        }

        for (unsigned long row=1 ; row<=numberLeadingRows ; ++row) {
            xmlContext.writeStartElement("mtr");

            for (unsigned long column=1 ; column<=numberLeadingColumns ; ++column) {
                xmlContext.writeStartElement("mtd");
                success = success && context.toMathML(row, column, element, engine);
                xmlContext.writeEndElement(); // mtd
            }

            if (numberTrailingColumns > 0 || numberLeadingColumns < numberColumns) {
                xmlContext.writeStartElement("mtd");
                xmlContext.writeStartElement("mn");
                xmlContext.writeCharacters(tr("%1").arg(QChar(0x2026)));
                xmlContext.writeEndElement(); // mn
                xmlContext.writeEndElement(); // mtd

                for (unsigned long column=firstTrailingColumn ; column<=numberColumns ; ++column) {
                    xmlContext.writeStartElement("mtd");
                    success = success && context.toMathML(row, column, element, engine);
                    xmlContext.writeEndElement(); // mtd
                }
            }

            xmlContext.writeEndElement(); // mtr
        }

        if (numberTrailingRows > 0 || numberLeadingRows < numberRows) {
            xmlContext.writeStartElement("mtr");

            for (unsigned long column=0 ; column<totalNumberColumns ; ++column) {
                xmlContext.writeStartElement("mtd");
                xmlContext.writeStartElement("mn");
                xmlContext.writeCharacters(tr("%1").arg(QChar(0x22EE)));
                xmlContext.writeEndElement(); // mn
                xmlContext.writeEndElement(); // mtd
            }

            xmlContext.writeEndElement(); // mtr

            for (unsigned long row=firstTrailingRow ; row<=numberRows ; ++row) {
                xmlContext.writeStartElement("mtr");

                for (unsigned long column=1 ; column<=numberLeadingColumns ; ++column) {
                    xmlContext.writeStartElement("mtd");
                    success = success && context.toMathML(row, column, element, engine);
                    xmlContext.writeEndElement(); // mtd
                }

                if (numberTrailingColumns > 0 || numberLeadingColumns < numberColumns) {
                    xmlContext.writeStartElement("mtd");
                    xmlContext.writeStartElement("mn");
                    xmlContext.writeCharacters(tr("%1").arg(QChar(0x2026)));
                    xmlContext.writeEndElement(); // mn
                    xmlContext.writeEndElement(); // mtd

                    for (unsigned long column=firstTrailingColumn ; column<=numberColumns ; ++column) {
                        xmlContext.writeStartElement("mtd");
                        success = success && context.toMathML(row, column, element, engine);
                        xmlContext.writeEndElement(); // mtd
                    }
                }

                xmlContext.writeEndElement(); // mtr
            }
        }

        xmlContext.writeEndElement(); // mtable
        xmlContext.writeEndElement(); // mrow

        xmlContext.writeStartElement("mo");
        xmlContext.writeAttribute("fence", "true");
        xmlContext.writeAttribute("stretchy", "true");
        xmlContext.writeCharacters(tr("]"));
        xmlContext.writeEndElement(); // mo

        xmlContext.writeEndElement(); // mrow
        xmlContext.writeEndElement(); // semantics
        xmlContext.writeEndElement(); // math

        return success;
    }


    bool HtmlMatrixDataTypeTranslator::translateToLaTeX(
            Context&                  context,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
        ) const {
        bool success = true;

        unsigned long numberRows            = context.numberRows();
        unsigned long numberColumns         = context.numberColumns();
        unsigned long numberLeadingRows     = context.numberLeadingRows();
        unsigned long numberTrailingRows    = context.numberTrailingRows();
        unsigned long firstTrailingRow      = context.firstTrailingRow();
        unsigned long numberLeadingColumns  = context.numberLeadingColumns();
        unsigned long numberTrailingColumns = context.numberTrailingColumns();
        unsigned long firstTrailingColumn   = context.firstTrailingColumn();

        unsigned long totalNumberColumns = numberLeadingColumns + numberTrailingColumns;
        if (numberTrailingRows > 0 || numberLeadingColumns < numberColumns) {
            ++totalNumberColumns;
        }

        XmlExportContext& xmlContext = engine.context();

        xmlContext.writeCharacters(QString("\\left [ \\begin{array}"));
        xmlContext.writeCharacters(QString("{%1}").arg(QString("c").repeated(totalNumberColumns)));

        bool firstRow = true;
        for (unsigned long row=1 ; row<=numberLeadingRows ; ++row) {
            if (!firstRow) {
                xmlContext.writeCharacters("\\\\");
            } else {
                firstRow = false;
            }

            bool firstInRow = true;

            for (unsigned long column=1 ; column<=numberLeadingColumns ; ++column) {
                if (!firstInRow) {
                    xmlContext.writeCharacters(" & ");
                } else {
                    firstInRow = false;
                }

                success = success && context.toLaTeX(row, column, element, engine);
            }

            if (numberTrailingColumns > 0 || numberLeadingColumns < numberColumns) {
                if (!firstInRow) {
                    xmlContext.writeCharacters(" & ");
                } else {
                    firstInRow = false;
                }

                xmlContext.writeCharacters("\\cdots");

                for (unsigned long column=firstTrailingColumn ; column<=numberColumns ; ++column) {
                    if (!firstInRow) {
                        xmlContext.writeCharacters(" & ");
                    } else {
                        firstInRow = false;
                    }

                    success = success && context.toLaTeX(row, column, element, engine);
                }
            }
        }

        if (numberTrailingRows > 0 || numberLeadingRows < numberRows) {
            if (!firstRow) {
                xmlContext.writeCharacters("\\\\");
            } else {
                firstRow = false;
            }

            for (unsigned long column=0 ; column<totalNumberColumns ; ++column) {
                if (column > 0) {
                    xmlContext.writeCharacters(" & ");
                }

                xmlContext.writeCharacters("\\vdots");
            }

            for (unsigned long row=firstTrailingRow ; row<=numberRows ; ++row) {
                xmlContext.writeCharacters("\\\\");

                bool firstInRow = true;

                for (unsigned long column=1 ; column<=numberLeadingColumns ; ++column) {
                    if (!firstInRow) {
                        xmlContext.writeCharacters(" & ");
                    } else {
                        firstInRow = false;
                    }

                    success = success && context.toLaTeX(row, column, element, engine);
                }

                if (numberTrailingColumns > 0 || numberLeadingColumns < numberColumns) {
                    if (!firstInRow) {
                        xmlContext.writeCharacters(" & ");
                    } else {
                        firstInRow = false;
                    }

                    xmlContext.writeCharacters("\\cdots");

                    for (unsigned long column=firstTrailingColumn ; column<=numberColumns ; ++column) {
                        if (!firstInRow) {
                            xmlContext.writeCharacters(" & ");
                        } else {
                            firstInRow = false;
                        }

                        success = success && context.toLaTeX(row, column, element, engine);
                    }
                }
            }
        }

        xmlContext.writeCharacters("\\end{array} \\right ]");

        return success;
    }


    bool HtmlMatrixDataTypeTranslator::translateToHtml(
            Context&                  context,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
        ) const {
        bool success = true;

        unsigned long numberRows            = context.numberRows();
        unsigned long numberColumns         = context.numberColumns();
        unsigned long numberLeadingRows     = context.numberLeadingRows();
        unsigned long numberTrailingRows    = context.numberTrailingRows();
        unsigned long firstTrailingRow      = context.firstTrailingRow();
        unsigned long numberLeadingColumns  = context.numberLeadingColumns();
        unsigned long numberTrailingColumns = context.numberTrailingColumns();
        unsigned long firstTrailingColumn   = context.firstTrailingColumn();

        XmlExportContext& xmlContext = engine.context();

        xmlContext.writeStartElement("table");

        unsigned long totalNumberColumns = numberLeadingColumns + numberTrailingColumns;
        if (numberTrailingRows > 0 || numberLeadingColumns < numberColumns) {
            ++totalNumberColumns;
        }

        for (unsigned long row=1 ; row<=numberLeadingRows ; ++row) {
            xmlContext.writeStartElement("tr");

            for (unsigned long column=1 ; column<=numberLeadingColumns ; ++column) {
                xmlContext.writeStartElement("td");
                success = success && context.toHtml(row, column, element, engine);
                xmlContext.writeEndElement(); // td
            }

            if (numberTrailingColumns > 0 || numberLeadingColumns < numberColumns) {
                xmlContext.writeStartElement("td");
                xmlContext.writeCharacters(tr("%1").arg(QChar(0x2026)));
                xmlContext.writeEndElement(); // td

                for (unsigned long column=firstTrailingColumn ; column<=numberColumns ; ++column) {
                    xmlContext.writeStartElement("td");
                    success = success && context.toHtml(row, column, element, engine);
                    xmlContext.writeEndElement(); // td
                }
            }

            xmlContext.writeEndElement(); // tr
        }

        if (numberTrailingRows > 0 || numberLeadingRows < numberRows) {
            xmlContext.writeStartElement("tr");

            for (unsigned long column=0 ; column<totalNumberColumns ; ++column) {
                xmlContext.writeStartElement("td");
                xmlContext.writeCharacters(tr("%1").arg(QChar(0x22EE)));
                xmlContext.writeEndElement(); // td
            }

            xmlContext.writeEndElement(); // mtr

            for (unsigned long row=firstTrailingRow ; row<=numberRows ; ++row) {
                xmlContext.writeStartElement("tr");

                for (unsigned long column=1 ; column<=numberLeadingColumns ; ++column) {
                    xmlContext.writeStartElement("td");
                    success = success && context.toHtml(row, column, element, engine);
                    xmlContext.writeEndElement(); // td
                }

                if (numberTrailingColumns > 0 || numberLeadingColumns < numberColumns) {
                    xmlContext.writeStartElement("td");
                    xmlContext.writeCharacters(tr("%1").arg(QChar(0x2026)));
                    xmlContext.writeEndElement(); // td

                    for (unsigned long column=firstTrailingColumn ; column<=numberColumns ; ++column) {
                        xmlContext.writeStartElement("td");
                        success = success && context.toHtml(row, column, element, engine);
                        xmlContext.writeEndElement(); // td
                    }
                }

                xmlContext.writeEndElement(); // tr
            }
        }

        xmlContext.writeEndElement(); // table

        return success;
    }
}
