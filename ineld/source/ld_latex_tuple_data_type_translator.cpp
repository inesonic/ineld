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
* This file implements the \ref Ld::LaTeXTupleDataTypeTranslator class.
***********************************************************************************************************************/

#include <QString>

#include <model_tuple.h>
#include <model_variant.h>

#include <util_string.h>

#include "ld_element.h"
#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_font_format.h"
#include "ld_tuple_data_type_format.h"
#include "ld_value_field_format.h"
#include "ld_tuple_element.h"
#include "ld_latex_code_generator.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_latex_code_generator_diagnostic.h"
#include "ld_text_export_context.h"
#include "ld_latex_data_type_translator.h"
#include "ld_latex_tuple_data_type_translator.h"

namespace Ld {
    const LaTeXTupleDataTypeTranslator LaTeXTupleDataTypeTranslator::instance;

    LaTeXTupleDataTypeTranslator::LaTeXTupleDataTypeTranslator() {}


    LaTeXTupleDataTypeTranslator::~LaTeXTupleDataTypeTranslator() {}


    QString LaTeXTupleDataTypeTranslator::elementName() const {
        return TupleElement::elementName;
    }


    bool LaTeXTupleDataTypeTranslator::toLaTeX(
            const Model::Variant&      variant,
            ElementPointer             element,
            LaTeXCodeGenerationEngine& engine
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

            TextExportContext& context = engine.context();

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
                bool                      isFirst = true;

                context << "\\left ( ";
                insertLaTeXFromTuple(isFirst, it, leadingMemberCount, element, engine);

                if (hiddenMemberCount > 0) {
                    if (!isFirst) {
                        context << ", ";
                    } else {
                        isFirst = false;
                    }

                    if (leadingMemberCount > 0) {
                        context << tr("\\ldots ");
                    }

                    if (showHiddenMemberCount) {
                        context << tr("\\text{%1 entries}").arg(hiddenMemberCount);
                        if (trailingMemberCount > 0) {
                            context << tr("\\ldots ");
                        }
                    }

                    it.advance(hiddenMemberCount);
                }

                insertLaTeXFromTuple(isFirst, it, trailingMemberCount, element, engine);
                context << "\\right ) ";

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
                        context << "\\text{"
                                << escapeText(str, engine.unicodeTranslationMode(), true)
                                << "}";
                        break;
                    }

                    case TupleDataTypeFormat::DisplayMode::STRING_ANGLE_BRACKETS: {
                        context << "\\left \\langle \\text{"
                                << escapeText(str, engine.unicodeTranslationMode(), true)
                                << "} \\right \\rangle ";
                        break;
                    }

                    case TupleDataTypeFormat::DisplayMode::STRING_SINGLE_QUOTES: {
                        context << "\\text{'"
                                << escapeText(str, engine.unicodeTranslationMode(), true)
                                << "'}";
                        break;
                    }

                    case TupleDataTypeFormat::DisplayMode::STRING_DOUBLE_QUOTES: {
                        context << "\\text{\""
                                << escapeText(str, engine.unicodeTranslationMode(), true)
                                << "\"}";
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


    void LaTeXTupleDataTypeTranslator::insertLaTeXFromTuple(
            bool&                        isFirst,
            Model::Tuple::ConstIterator& iterator,
            unsigned long                count,
            ElementPointer               element,
            LaTeXCodeGenerationEngine&   engine
        ) {
        TextExportContext& context = engine.context();

        for (unsigned long i=0 ; i<count ; ++i) {
            const Model::Variant& contents          = iterator.constReference();
            Model::ValueType      contentsValueType = contents.valueType();
            DataType              contentsDataType  = DataType::fromValueType(contentsValueType);

            if (!isFirst) {
                context << ", ";
            } else {
                isFirst = false;
            }

            if (contentsDataType.isValid()) {
                const DataTypeDecoder* contentsDecoder = contentsDataType.decoder();
                if (contentsDecoder == nullptr) {
                    context << tr("\\text{*** MISSING DECODER %1 ***}").arg(static_cast<int>(contentsValueType));
                } else {
                    const LaTeXDataTypeTranslator* contentsTranslator = dynamic_cast<const LaTeXDataTypeTranslator*>(
                        contentsDataType.translator(LaTeXCodeGenerator::codeGeneratorName)
                    );

                    if (contentsTranslator != nullptr) {
                        contentsTranslator->toLaTeX(contents, element, engine);
                    } else {
                        context << tr("\\text{*** MISSING TRANSLATOR %1 ***}")
                                   .arg(static_cast<int>(contentsValueType));
                    }
                }
            } else {
                context << tr("\\text{*** UNKNOWN TYPE %1 ***}").arg(static_cast<int>(contentsValueType));
            }

            iterator.advance();
        }
    }
}
