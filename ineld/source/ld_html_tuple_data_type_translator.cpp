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
* This file implements the \ref Ld::HtmlTupleDataTypeTranslator class.
***********************************************************************************************************************/

#include <QString>

#include <model_intrinsic_types.h>
#include <model_tuple.h>
#include <model_variant.h>

#include <util_string.h>

#include "ld_element.h"
#include "ld_tuple_element.h"
#include "ld_tuple_data_type_format.h"
#include "ld_value_field_format.h"
#include "ld_element_structures.h"
#include "ld_html_code_generator.h"
#include "ld_html_code_generation_engine.h"
#include "ld_html_code_generator_diagnostic.h"
#include "ld_data_type_translator.h"
#include "ld_html_data_type_translator.h"
#include "ld_xml_export_context.h"
#include "ld_identifier_container.h"
#include "ld_html_data_type_translator.h"
#include "ld_html_tuple_data_type_translator.h"

// TODO: Support multiple display modes rather than just the horizontal/wrapped mode.

namespace Ld {
    const HtmlTupleDataTypeTranslator HtmlTupleDataTypeTranslator::instance;

    HtmlTupleDataTypeTranslator::HtmlTupleDataTypeTranslator() {}


    HtmlTupleDataTypeTranslator::~HtmlTupleDataTypeTranslator() {}


    QString HtmlTupleDataTypeTranslator::elementName() const {
        return TupleElement::elementName;
    }


    bool HtmlTupleDataTypeTranslator::toMathML(
            const Model::Variant&     variant,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
        ) const {
        bool         success;
        Model::Tuple tupleValue = variant.toTuple(&success);

        if (success) {
            Ld::TupleDataTypeFormat::DisplayMode displayMode;
            unsigned long                      leadingMemberCount;
            unsigned long                      trailingMemberCount;
            unsigned long                      hiddenMemberCount;
            bool                               showHiddenMemberCount;
            FormatPointer                      format;

            if (!element.isNull()) {
                format = element->format();
                if (!format.isNull()                                                         &&
                    (format->capabilities().contains(Ld::TupleDataTypeFormat::formatName) ||
                     format->capabilities().contains(Ld::ValueFieldFormat::formatName)     )    ) {
                    QSharedPointer<TupleDataTypeFormat> tupleFormat = format.dynamicCast<TupleDataTypeFormat>();

                    displayMode           = tupleFormat->displayMode();
                    leadingMemberCount    = tupleFormat->leadingMemberCount();
                    trailingMemberCount   = tupleFormat->trailingMemberCount();
                    showHiddenMemberCount = tupleFormat->displayHiddenMemberCount();
                } else {
                    displayMode           = TupleDataTypeFormat::defaultDisplayMode;
                    leadingMemberCount    = TupleDataTypeFormat::defaultLeadingMemberCount;
                    trailingMemberCount   = TupleDataTypeFormat::defaultTrailingMemberCount;
                    showHiddenMemberCount = TupleDataTypeFormat::defaultShowHiddenMemberCount;
                }
            } else {
                displayMode           = TupleDataTypeFormat::defaultDisplayMode;
                leadingMemberCount    = TupleDataTypeFormat::defaultLeadingMemberCount;
                trailingMemberCount   = TupleDataTypeFormat::defaultTrailingMemberCount;
                showHiddenMemberCount = TupleDataTypeFormat::defaultShowHiddenMemberCount;
            }

            XmlExportContext& context = engine.context();

            if (displayMode == TupleDataTypeFormat::DisplayMode::NORMAL  ||
                displayMode == TupleDataTypeFormat::DisplayMode::INVALID    ) {
                unsigned long numberTupleMembers = tupleValue.size();
                if (leadingMemberCount == TupleDataTypeFormat::showAllMembers      ||
                    trailingMemberCount == TupleDataTypeFormat::showAllMembers     ||
                    leadingMemberCount + trailingMemberCount >= numberTupleMembers    ) {
                    leadingMemberCount  = numberTupleMembers;
                    hiddenMemberCount   = 0;
                    trailingMemberCount = 0;
                } else {
                    hiddenMemberCount = numberTupleMembers - leadingMemberCount - trailingMemberCount;
                }

                context.writeStartElement("mrow");

                context.writeStartElement("mo");
                context.writeAttribute("fence", "true");
                context.writeAttribute("stretchy", "true");
                context.writeCharacters(tr("("));
                context.writeEndElement(); // mo

                Model::Tuple::ConstIterator it      = tupleValue.constBegin();
                bool                        isFirst = true;

                insertMathMlFromTuple(isFirst, it, leadingMemberCount, element, engine);

                if (hiddenMemberCount > 0) {
                    if (!isFirst) {
                        context.writeStartElement("mo");
                        context.writeCharacters(tr(", "));
                        context.writeEndElement();
                    } else {
                        isFirst = false;
                    }

                    if (leadingMemberCount > 0) {
                        context.writeStartElement("mo");
                        context.writeCharacters(tr("%1").arg(QChar(0x2026)));
                        context.writeEndElement();
                    }

                    if (showHiddenMemberCount) {
                        context.writeStartElement("mtext");
                        context.writeCharacters(tr("%1 entries").arg(hiddenMemberCount));
                        context.writeEndElement();

                        if (trailingMemberCount > 0) {
                            context.writeStartElement("mo");
                            context.writeCharacters(tr("%1").arg(QChar(0x2026)));
                            context.writeEndElement();
                        }
                    }

                    it.advance(hiddenMemberCount);
                }

                insertMathMlFromTuple(isFirst, it, trailingMemberCount, element, engine);

                context.writeStartElement("mo");
                context.writeAttribute("fence", "true");
                context.writeAttribute("stretchy", "true");
                context.writeCharacters(tr(")"));
                context.writeEndElement(); // mo

                success = true;
            } else {
                QString leadingQuote;
                QString trailingQuote;

                switch (displayMode) {
                    case TupleDataTypeFormat::DisplayMode::INVALID:
                    case TupleDataTypeFormat::DisplayMode::NORMAL: {
                        assert(false);
                        break;
                    }

                    case TupleDataTypeFormat::DisplayMode::STRING_BARE: {
                        break;
                    }

                    case TupleDataTypeFormat::DisplayMode::STRING_ANGLE_BRACKETS: {
                        leadingQuote  = tr("%1").arg(QChar(0x27E8));
                        trailingQuote = tr("%1").arg(QChar(0x27E9));
                        break;
                    }

                    case TupleDataTypeFormat::DisplayMode::STRING_SINGLE_QUOTES: {
                        leadingQuote = tr("\"");
                        trailingQuote = tr("\"");
                        break;
                    }

                    case TupleDataTypeFormat::DisplayMode::STRING_DOUBLE_QUOTES: {
                        leadingQuote  = tr("'");
                        trailingQuote = tr("'");
                        break;
                    }

                    default: {
                        assert(false);
                        break;
                    }
                }

                if (!leadingQuote.isEmpty()) {
                    context.writeStartElement("mrow");
                    context.writeStartElement("mo");
                    context.writeAttribute("fence", "true");
                    context.writeCharacters(leadingQuote);
                    context.writeEndElement(); // mo
                }

                context.writeStartElement("mrow");
                context.writeStartElement("mtext");

                char* s = tupleValue.toString();
                if (s == nullptr) {
                    context.writeCharacters(tr("*** INVALID TEXT CONTENT ***"));
                } else {
                    context.writeCharacters(QString::fromUtf8(s));
                    delete[] s;
                }

                context.writeEndElement(); // mtext
                context.writeEndElement(); // mrow

                if (!trailingQuote.isEmpty()) {
                    context.writeStartElement("mo");
                    context.writeAttribute("fence", "true");
                    context.writeCharacters(trailingQuote);
                    context.writeEndElement(); // mo
                    context.writeEndElement(); // mrow
                }

                success = true;
            }
        }

        return success;
    }


    bool HtmlTupleDataTypeTranslator::toLaTeX(
            const Model::Variant&     variant,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
        ) const {
        bool         success;
        Model::Tuple tupleValue = variant.toTuple(&success);

        if (success) {
            Ld::TupleDataTypeFormat::DisplayMode displayMode;
            unsigned long                        leadingMemberCount;
            unsigned long                        trailingMemberCount;
            unsigned long                        hiddenMemberCount;
            bool                                 showHiddenMemberCount;
            FormatPointer                        format;

            if (!element.isNull()) {
                format = element->format();
                if (!format.isNull()                                                         &&
                    (format->capabilities().contains(Ld::TupleDataTypeFormat::formatName) ||
                     format->capabilities().contains(Ld::ValueFieldFormat::formatName)     )    ) {
                    QSharedPointer<TupleDataTypeFormat> tupleFormat = format.dynamicCast<TupleDataTypeFormat>();

                    displayMode           = tupleFormat->displayMode();
                    leadingMemberCount    = tupleFormat->leadingMemberCount();
                    trailingMemberCount   = tupleFormat->trailingMemberCount();
                    showHiddenMemberCount = tupleFormat->displayHiddenMemberCount();
                } else {
                    displayMode           = TupleDataTypeFormat::defaultDisplayMode;
                    leadingMemberCount    = TupleDataTypeFormat::defaultLeadingMemberCount;
                    trailingMemberCount   = TupleDataTypeFormat::defaultTrailingMemberCount;
                    showHiddenMemberCount = TupleDataTypeFormat::defaultShowHiddenMemberCount;
                }
            } else {
                displayMode           = TupleDataTypeFormat::defaultDisplayMode;
                leadingMemberCount    = TupleDataTypeFormat::defaultLeadingMemberCount;
                trailingMemberCount   = TupleDataTypeFormat::defaultTrailingMemberCount;
                showHiddenMemberCount = TupleDataTypeFormat::defaultShowHiddenMemberCount;
            }

            XmlExportContext& context = engine.context();

            if (displayMode == TupleDataTypeFormat::DisplayMode::NORMAL  ||
                displayMode == TupleDataTypeFormat::DisplayMode::INVALID    ) {
                unsigned long numberTupleMembers = tupleValue.size();
                if (leadingMemberCount == TupleDataTypeFormat::showAllMembers      ||
                    trailingMemberCount == TupleDataTypeFormat::showAllMembers     ||
                    leadingMemberCount + trailingMemberCount >= numberTupleMembers    ) {
                    leadingMemberCount  = numberTupleMembers;
                    hiddenMemberCount   = 0;
                    trailingMemberCount = 0;
                } else {
                    hiddenMemberCount = numberTupleMembers - leadingMemberCount - trailingMemberCount;
                }

                Model::Tuple::ConstIterator it      = tupleValue.constBegin();
                bool                        isFirst = true;

                context.writeCharacters("\\left ( ");
                insertLaTeXFromTuple(isFirst, it, leadingMemberCount, element, engine);

                if (hiddenMemberCount > 0) {
                    if (!isFirst) {
                        context.writeCharacters(", ");
                    } else {
                        isFirst = false;
                    }

                    if (leadingMemberCount > 0) {
                        context.writeCharacters("\\ldots ");
                    }

                    if (showHiddenMemberCount) {
                        context.writeCharacters(tr("\\text{%1 entries}").arg(hiddenMemberCount));
                        if (trailingMemberCount > 0) {
                            context.writeCharacters("\\ldots ");
                        }
                    }

                    it.advance(hiddenMemberCount);
                }

                insertLaTeXFromTuple(isFirst, it, trailingMemberCount, element, engine);
                context.writeCharacters("\\right ) ");

                success = true;
            } else {
                QString str;
                char*   s = tupleValue.toString();
                if (s == nullptr) {
                    str = tr("*** INVALID TEXT CONTENT ***");
                } else {
                    str = QString::fromUtf8(s);
                    delete[] s;
                }

                switch (displayMode) {
                    case TupleDataTypeFormat::DisplayMode::INVALID:
                    case TupleDataTypeFormat::DisplayMode::NORMAL: {
                        assert(false);
                        break;
                    }

                    case TupleDataTypeFormat::DisplayMode::STRING_BARE: {
                        context.writeCharacters("\\text{");
                        writeEscapedText(str, engine, true);
                        context.writeCharacters("}");

                        break;
                    }

                    case TupleDataTypeFormat::DisplayMode::STRING_ANGLE_BRACKETS: {
                        context.writeCharacters("{\\left\\langle\\text{");
                        writeEscapedText(str, engine, true);
                        context.writeCharacters("}\\right\\rangle}");

                        break;
                    }

                    case TupleDataTypeFormat::DisplayMode::STRING_SINGLE_QUOTES: {
                        context.writeCharacters("\\text{'");
                        writeEscapedText(str, engine, true);
                        context.writeCharacters("'}");

                        break;
                    }

                    case TupleDataTypeFormat::DisplayMode::STRING_DOUBLE_QUOTES: {
                        context.writeCharacters("\\text{\"");
                        writeEscapedText(str, engine, true);
                        context.writeCharacters("\"}");

                        break;
                    }

                    default: {
                        assert(false);
                        break;
                    }
                }

                success = true;
            }

        }

        return success;
    }


    bool HtmlTupleDataTypeTranslator::toHtml(
            const Model::Variant&     variant,
            ElementPointer            element,
            HtmlCodeGenerationEngine& engine
        ) const {
        bool         success;
        Model::Tuple tupleValue = variant.toTuple(&success);

        if (success) {
            Ld::TupleDataTypeFormat::DisplayMode displayMode;
            unsigned long                      leadingMemberCount;
            unsigned long                      trailingMemberCount;
            unsigned long                      hiddenMemberCount;
            bool                               showHiddenMemberCount;
            FormatPointer                      format;

            if (!element.isNull()) {
                format = element->format();
                if (!format.isNull()                                                         &&
                    (format->capabilities().contains(Ld::TupleDataTypeFormat::formatName) ||
                     format->capabilities().contains(Ld::ValueFieldFormat::formatName)     )    ) {
                    QSharedPointer<TupleDataTypeFormat> tupleFormat = format.dynamicCast<TupleDataTypeFormat>();

                    displayMode           = tupleFormat->displayMode();
                    leadingMemberCount    = tupleFormat->leadingMemberCount();
                    trailingMemberCount   = tupleFormat->trailingMemberCount();
                    showHiddenMemberCount = tupleFormat->displayHiddenMemberCount();
                } else {
                    displayMode           = TupleDataTypeFormat::defaultDisplayMode;
                    leadingMemberCount    = TupleDataTypeFormat::defaultLeadingMemberCount;
                    trailingMemberCount   = TupleDataTypeFormat::defaultTrailingMemberCount;
                    showHiddenMemberCount = TupleDataTypeFormat::defaultShowHiddenMemberCount;
                }
            } else {
                displayMode           = TupleDataTypeFormat::defaultDisplayMode;
                leadingMemberCount    = TupleDataTypeFormat::defaultLeadingMemberCount;
                trailingMemberCount   = TupleDataTypeFormat::defaultTrailingMemberCount;
                showHiddenMemberCount = TupleDataTypeFormat::defaultShowHiddenMemberCount;
            }

            XmlExportContext& context = engine.context();

            if (displayMode == TupleDataTypeFormat::DisplayMode::NORMAL  ||
                displayMode == TupleDataTypeFormat::DisplayMode::INVALID    ) {
                unsigned long numberTupleMembers = tupleValue.size();
                if (leadingMemberCount == TupleDataTypeFormat::showAllMembers      ||
                    trailingMemberCount == TupleDataTypeFormat::showAllMembers     ||
                    leadingMemberCount + trailingMemberCount >= numberTupleMembers    ) {
                    leadingMemberCount  = numberTupleMembers;
                    hiddenMemberCount   = 0;
                    trailingMemberCount = 0;
                } else {
                    hiddenMemberCount = numberTupleMembers - leadingMemberCount - trailingMemberCount;
                }

                Model::Tuple::ConstIterator it      = tupleValue.constBegin();
                bool                        isFirst = true;

                context.writeCharacters("( ");
                insertHtmlFromTuple(isFirst, it, leadingMemberCount, element, engine);

                if (hiddenMemberCount > 0) {
                    if (!isFirst) {
                        context.writeCharacters(", ");
                    } else {
                        isFirst = false;
                    }

                    if (leadingMemberCount > 0) {
                        context.writeCharacters(tr("%1").arg(QChar(0x2026)));
                    }

                    if (showHiddenMemberCount) {
                        context.writeCharacters(tr("%1 entries").arg(hiddenMemberCount));
                        if (trailingMemberCount > 0) {
                            context.writeCharacters(tr("%1").arg(QChar(0x2026)));
                        }
                    }

                    it.advance(hiddenMemberCount);
                }

                insertHtmlFromTuple(isFirst, it, trailingMemberCount, element, engine);
                context.writeCharacters(" )");

                success = true;
            } else {
                QString str;
                char*   s = tupleValue.toString();
                if (s == nullptr) {
                    str = tr("*** INVALID TEXT CONTENT ***");
                } else {
                    str = QString::fromUtf8(s);
                    delete[] s;
                }

                switch (displayMode) {
                    case TupleDataTypeFormat::DisplayMode::INVALID:
                    case TupleDataTypeFormat::DisplayMode::NORMAL: {
                        assert(false);
                        break;
                    }

                    case TupleDataTypeFormat::DisplayMode::STRING_BARE: {
                        break;
                    }

                    case TupleDataTypeFormat::DisplayMode::STRING_ANGLE_BRACKETS: {
                        str = tr("%1%2%3").arg(QChar(0x27E8)).arg(str).arg(QChar(0x27E8));
                        break;
                    }

                    case TupleDataTypeFormat::DisplayMode::STRING_SINGLE_QUOTES: {
                        str = tr("'%1'").arg(str);
                        break;
                    }

                    case TupleDataTypeFormat::DisplayMode::STRING_DOUBLE_QUOTES: {
                        str = tr("\"%1\"").arg(str);
                        break;
                    }

                    default: {
                        assert(false);
                        break;
                    }
                }

                writeEscapedText(str, engine, false);
                success = true;
            }

        }

        return success;
    }


    void HtmlTupleDataTypeTranslator::insertMathMlFromTuple(
            bool&                        isFirst,
            Model::Tuple::ConstIterator& iterator,
            unsigned long                count,
            ElementPointer               element,
            HtmlCodeGenerationEngine&    engine
        ) {
        XmlExportContext& context = engine.context();

        for (unsigned long i=0 ; i<count ; ++i) {
            const Model::Variant& contents          = iterator.constReference();
            Model::ValueType      contentsValueType = contents.valueType();
            DataType              contentsDataType  = DataType::fromValueType(contentsValueType);

            if (!isFirst) {
                context.writeStartElement("mo");
                context.writeCharacters(tr(", "));
                context.writeEndElement();
            } else {
                isFirst = false;
            }

            if (contentsDataType.isValid()) {
                const DataTypeDecoder* contentsDecoder = contentsDataType.decoder();
                if (contentsDecoder == nullptr) {
                    context.writeStartElement("mtext");
                    context.writeCharacters(tr("*** MISSING DECODER %1 ***").arg(static_cast<int>(contentsValueType)));
                    context.writeEndElement();
                } else {
                    const HtmlDataTypeTranslator* contentsTranslator = dynamic_cast<const HtmlDataTypeTranslator*>(
                        contentsDataType.translator(HtmlCodeGenerator::codeGeneratorName)
                    );

                    if (contentsTranslator != nullptr) {
                        contentsTranslator->toMathML(contents, element, engine);
                    } else {
                        context.writeStartElement("mtext");
                        context.writeCharacters(
                            tr("*** MISSING TRANSLATOR %1 ***").arg(static_cast<int>(contentsValueType))
                        );
                        context.writeEndElement();
                    }
                }
            } else {
                context.writeStartElement("mtext");
                context.writeCharacters(tr("*** UNKNOWN TYPE %1 ***").arg(static_cast<int>(contentsValueType)));
                context.writeEndElement();
            }

            iterator.advance();
        }
    }


    void HtmlTupleDataTypeTranslator::insertLaTeXFromTuple(
            bool&                        isFirst,
            Model::Tuple::ConstIterator& iterator,
            unsigned long                count,
            ElementPointer               element,
            HtmlCodeGenerationEngine&    engine
        ) {
        XmlExportContext& context = engine.context();

        for (unsigned long i=0 ; i<count ; ++i) {
            const Model::Variant& contents          = iterator.constReference();
            Model::ValueType      contentsValueType = contents.valueType();
            DataType              contentsDataType  = DataType::fromValueType(contentsValueType);

            if (!isFirst) {
                context.writeCharacters(tr(", "));
            } else {
                isFirst = false;
            }

            if (contentsDataType.isValid()) {
                const DataTypeDecoder* contentsDecoder = contentsDataType.decoder();
                if (contentsDecoder == nullptr) {
                    context.writeCharacters(
                        tr("\\text{*** MISSING DECODER %1 ***}").arg(static_cast<int>(contentsValueType))
                    );
                } else {
                    const HtmlDataTypeTranslator* contentsTranslator = dynamic_cast<const HtmlDataTypeTranslator*>(
                        contentsDataType.translator(HtmlCodeGenerator::codeGeneratorName)
                    );

                    if (contentsTranslator != nullptr) {
                        contentsTranslator->toLaTeX(contents, element, engine);
                    } else {
                        context.writeCharacters(
                            tr("\\text{*** MISSING TRANSLATOR %1 ***}").arg(static_cast<int>(contentsValueType))
                        );
                    }
                }
            } else {
                context.writeCharacters(
                    tr("\\text{*** UNKNOWN TYPE %1 ***}").arg(static_cast<int>(contentsValueType))
                );
            }

            iterator.advance();
        }
    }


    void HtmlTupleDataTypeTranslator::insertHtmlFromTuple(
            bool&                        isFirst,
            Model::Tuple::ConstIterator& iterator,
            unsigned long                count,
            ElementPointer               element,
            HtmlCodeGenerationEngine&    engine
        ) {
        XmlExportContext& context = engine.context();

        for (unsigned long i=0 ; i<count ; ++i) {
            const Model::Variant& contents          = iterator.constReference();
            Model::ValueType      contentsValueType = contents.valueType();
            DataType              contentsDataType  = DataType::fromValueType(contentsValueType);

            if (!isFirst) {
                context.writeCharacters(tr(", "));
            } else {
                isFirst = false;
            }

            if (contentsDataType.isValid()) {
                const DataTypeDecoder* contentsDecoder = contentsDataType.decoder();
                if (contentsDecoder == nullptr) {
                    context.writeCharacters(tr("*** MISSING DECODER %1 ***").arg(static_cast<int>(contentsValueType)));
                } else {
                    const HtmlDataTypeTranslator* contentsTranslator = dynamic_cast<const HtmlDataTypeTranslator*>(
                        contentsDataType.translator(HtmlCodeGenerator::codeGeneratorName)
                    );

                    if (contentsTranslator != nullptr) {
                        contentsTranslator->toHtml(contents, element, engine);
                    } else {
                        context.writeCharacters(
                            tr("*** MISSING TRANSLATOR %1 ***").arg(static_cast<int>(contentsValueType))
                        );
                    }
                }
            } else {
                context.writeCharacters(tr("*** UNKNOWN TYPE %1 ***").arg(static_cast<int>(contentsValueType)));
            }

            iterator.advance();
        }
    }
}
