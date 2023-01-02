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
* This file implements the \ref Ld::LaTeXSetDataTypeTranslator class.
***********************************************************************************************************************/

#include <QString>

#include <model_set.h>
#include <model_variant.h>

#include <util_string.h>

#include "ld_element.h"
#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_font_format.h"
#include "ld_set_data_type_format.h"
#include "ld_value_field_format.h"
#include "ld_set_element.h"
#include "ld_latex_code_generator.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_latex_code_generator_diagnostic.h"
#include "ld_text_export_context.h"
#include "ld_latex_data_type_translator.h"
#include "ld_latex_set_data_type_translator.h"

namespace Ld {
    const LaTeXSetDataTypeTranslator LaTeXSetDataTypeTranslator::instance;

    LaTeXSetDataTypeTranslator::LaTeXSetDataTypeTranslator() {}


    LaTeXSetDataTypeTranslator::~LaTeXSetDataTypeTranslator() {}


    QString LaTeXSetDataTypeTranslator::elementName() const {
        return SetElement::elementName;
    }


    bool LaTeXSetDataTypeTranslator::toLaTeX(
            const Model::Variant&      variant,
            ElementPointer             element,
            LaTeXCodeGenerationEngine& engine
        ) const {
        bool       success;
        Model::Set setValue = variant.toSet(&success);

        if (success) {
            unsigned long leadingMemberCount;
            unsigned long trailingMemberCount;
            unsigned long hiddenMemberCount;
            bool          showHiddenMemberCount;
            FormatPointer format;

            if (!element.isNull()) {
                format = element->format();
                if (!format.isNull()                                                         &&
                    (format->capabilities().contains(Ld::SetDataTypeFormat::formatName) ||
                     format->capabilities().contains(Ld::ValueFieldFormat::formatName)     )    ) {
                    QSharedPointer<SetDataTypeFormat> setFormat = format.dynamicCast<SetDataTypeFormat>();

                    leadingMemberCount    = setFormat->leadingMemberCount();
                    trailingMemberCount   = setFormat->trailingMemberCount();
                    showHiddenMemberCount = setFormat->displayHiddenMemberCount();
                } else {
                    leadingMemberCount    = SetDataTypeFormat::defaultLeadingMemberCount;
                    trailingMemberCount   = SetDataTypeFormat::defaultTrailingMemberCount;
                    showHiddenMemberCount = SetDataTypeFormat::defaultShowHiddenMemberCount;
                }
            } else {
                leadingMemberCount    = SetDataTypeFormat::defaultLeadingMemberCount;
                trailingMemberCount   = SetDataTypeFormat::defaultTrailingMemberCount;
                showHiddenMemberCount = SetDataTypeFormat::defaultShowHiddenMemberCount;
            }

            unsigned long numberSetMembers = setValue.size();
            if (leadingMemberCount == SetDataTypeFormat::showAllMembers      ||
                trailingMemberCount == SetDataTypeFormat::showAllMembers     ||
                leadingMemberCount + trailingMemberCount >= numberSetMembers    ) {
                leadingMemberCount  = numberSetMembers;
                hiddenMemberCount   = 0;
                trailingMemberCount = 0;
            } else {
                hiddenMemberCount = numberSetMembers - leadingMemberCount - trailingMemberCount;
            }

            TextExportContext&        context = engine.context();
            Model::Set::ConstIterator it      = setValue.constBegin();
            bool                      isFirst = true;

            context << "\\left \\lbrace ";
            insertLaTeXFromSet(isFirst, it, leadingMemberCount, element, engine);

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

            insertLaTeXFromSet(isFirst, it, trailingMemberCount, element, engine);
            context << "\\right \\rbrace";

            success = true;
        }

        return success;
    }


    void LaTeXSetDataTypeTranslator::insertLaTeXFromSet(
            bool&                      isFirst,
            Model::Set::ConstIterator& iterator,
            unsigned long              count,
            ElementPointer             element,
            LaTeXCodeGenerationEngine& engine
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
