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
* \file Ld::CppCompoundStatementOperatorTranslator class.
***********************************************************************************************************************/

#include <QString>

#include "ld_compound_statement_operator_element.h"
#include "ld_root_element.h"
#include "ld_set_element.h"
#include "ld_tuple_element.h"
#include "ld_range_2_element.h"
#include "ld_range_3_element.h"
#include "ld_list_placeholder_element.h"
#include "ld_data_type.h"
#include "ld_identifier_database.h"
#include "ld_variable_element.h"
#include "ld_data_type_translator.h"
#include "ld_cpp_code_generator_diagnostic.h"
#include "ld_cpp_code_generator.h"
#include "ld_cpp_code_generation_engine.h"
#include "ld_cpp_data_type_translator.h"
#include "ld_cpp_context.h"
#include "ld_cpp_translator.h"
#include "ld_cpp_compound_statement_operator_translator.h"

namespace Ld {
    CppCompoundStatementOperatorTranslator::~CppCompoundStatementOperatorTranslator() {}


    QString CppCompoundStatementOperatorTranslator::elementName() const {
        return CompoundStatementOperatorElement::elementName;
    }


    bool CppCompoundStatementOperatorTranslator::identifyDependenciesAndExplicitTypes(
            ElementPointer           element,
            CppCodeGenerationEngine& engine
        ) {
        engine.pushNewScope(element);
        bool result = CppTranslator::identifyDependenciesAndExplicitTypes(element, engine);
        engine.popCurrentScope();

        return result;
    }


    bool CppCompoundStatementOperatorTranslator::identifyInferredTypes(
            ElementPointer           element,
            CppCodeGenerationEngine& engine
        ) {
        engine.pushNewScope(element);
        bool result = CppTranslator::identifyInferredTypes(element, engine);
        engine.popCurrentScope();

        return result;
    }


    bool CppCompoundStatementOperatorTranslator::threadImplementation(
            ElementPointer           element,
            CppCodeGenerationEngine& engine
        ) {
        ElementPointer parent = element->parent();
        assert(!parent.isNull());

        engine.pushNewScope(element);

        CppContext&   context        = engine.context();
        unsigned long numberChildren = element->numberChildren();

        context(element) << "[&](){\n";
        context.startedNewStatement();

        bool                                       success               = true;
        QList<IdentifierContainer>                 identifiers           = engine.identifiersInScope(element);
        QList<IdentifierContainer>::const_iterator identifierIterator    = identifiers.constBegin();
        QList<IdentifierContainer>::const_iterator identifierEndIterator = identifiers.constEnd();
        while (success && identifierIterator != identifierEndIterator) {
            const IdentifierContainer identifier = *identifierIterator;
            Identifier::DefinedAs     definedAs  = identifier.definedAs();

            if (definedAs == Identifier::DefinedAs::LOCAL_SCOPE_VARIABLE) {
                DataType dataType = identifier.dataType();
                if (dataType.isInvalid()) {
                    dataType = DataType::defaultDataType();
                }

                const CppDataTypeTranslator* translator = dynamic_cast<const CppDataTypeTranslator*>(
                    dataType.translator(CppCodeGenerator::codeGeneratorName)
                );

                if (translator != nullptr) {
                    QString dataTypeName = translator->dataTypeName();

                    context.startNewStatement();
                    context(element) << QString("%1 %2").arg(dataTypeName, identifier.internalName());
                } else {
                    ElementPointer primaryElement = identifier->primaryElement();
                    if (primaryElement.isNull()) {
                        for (  Identifier::ElementIterator elementIterator    = identifier->begin(),
                                                           elementEndIterator = identifier->end()
                             ; elementIterator != elementEndIterator
                             ; ++elementIterator
                            ) {
                            ElementPointer element = elementIterator->element();
                            if (!element.isNull()) {
                                CppCodeGeneratorDiagnostic* diagnostic = new CppCodeGeneratorDiagnostic(
                                    element,
                                    CppCodeGeneratorDiagnostic::Type::INTERNAL_ERROR,
                                    dynamic_cast<const CppTranslationPhase&>(engine.translationPhase()),
                                    CppCodeGeneratorDiagnostic::Code::MISSING_DATA_TYPE_TRANSLATOR
                                );
                                engine.translationErrorDetected(diagnostic);
                            }
                        }

                        success = false;
                    } else {
                        CppCodeGeneratorDiagnostic* diagnostic = new CppCodeGeneratorDiagnostic(
                            primaryElement,
                            CppCodeGeneratorDiagnostic::Type::INTERNAL_ERROR,
                            dynamic_cast<const CppTranslationPhase&>(engine.translationPhase()),
                            CppCodeGeneratorDiagnostic::Code::MISSING_DATA_TYPE_TRANSLATOR
                        );
                        engine.translationErrorDetected(diagnostic);
                    }
                }
            }

            ++identifierIterator;
        }

        unsigned long childIndex = 0;
        while (childIndex < numberChildren) {
            context.startNewStatement();
            ElementPointer child = element->child(childIndex);
            success = engine.translateChild(child) && success;

            ++childIndex;
        }

        context.startNewStatement();

        context(element) << "}()";

        engine.popCurrentScope();

        return success;
    }


    bool CppCompoundStatementOperatorTranslator::methodDefinitions(
            ElementPointer           element,
            CppCodeGenerationEngine& engine
        ) {
        return threadImplementation(element, engine);
    }
}
