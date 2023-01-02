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
* This file implements the \ref Ld::LaTeXValueFieldElementTranslator class.
***********************************************************************************************************************/

#include <QString>
#include <QColor>

#include <model_variant.h>

#include "ld_element.h"
#include "ld_element_structures.h"
#include "ld_root_element.h"
#include "ld_value_field_element.h"
#include "ld_calculated_value.h"
#include "ld_format.h"
#include "ld_format_structures.h"
#include "ld_data_type.h"
#include "ld_data_type_translator.h"
#include "ld_latex_data_type_translator.h"
#include "ld_text_export_context.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_latex_code_generator_diagnostic.h"
#include "ld_latex_translator.h"
#include "ld_latex_value_field_element_translator.h"

namespace Ld {
    LaTeXValueFieldElementTranslator::~LaTeXValueFieldElementTranslator() {}


    QString LaTeXValueFieldElementTranslator::elementName() const {
        return ValueFieldElement::elementName;
    }


    bool LaTeXValueFieldElementTranslator::body(ElementPointer element, LaTeXCodeGenerationEngine& engine) {
        bool success;

        CalculatedValue    calculatedValue = engine.rootElement()->elementCalculatedValue(element, 0);
        const DataType&    dataType        = calculatedValue.dataType();
        TextExportContext& context         = engine.context();

        if (dataType.valueType() != DataType::ValueType::NONE) {
            const DataTypeTranslator* dataTypeTranslator = dataType.translator(LaTeXCodeGenerator::codeGeneratorName);
            if (dataTypeTranslator != nullptr) {
                const LaTeXDataTypeTranslator*
                    latexDataTypeTranslator = dynamic_cast<const LaTeXDataTypeTranslator*>(dataTypeTranslator);

                context << " $ ";
                success = latexDataTypeTranslator->toLaTeX(calculatedValue.variant(), element, engine);
                context << " $ ";
            } else {
                LaTeXCodeGeneratorDiagnostic* diagnostic = new LaTeXCodeGeneratorDiagnostic(
                    element,
                    LaTeXCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                    dynamic_cast<const LaTeXTranslationPhase&>(engine.translationPhase()),
                    LaTeXCodeGeneratorDiagnostic::Code::MISSING_TRANSLATOR,
                    engine.contextPointer()
                );

                engine.translationErrorDetected(diagnostic);
                success = false;
            }
        } else {
            success = LaTeXDataTypeTranslator::generateDefaultLaTeX(element, engine);
        }

        return success;
    }
}
