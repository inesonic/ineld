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
* This file implements the \ref Ld::CppLiteralTranslator class.
***********************************************************************************************************************/

#include <QString>

#include <sstream>
#include <ios>
#include <iomanip>
#include <limits>
#include <cstring>

#include <model_intrinsic_types.h>
#include <model_complex.h>
#include <model_tuple.h>
#include <model_variant.h>

#include "ld_literal_element.h"
#include "ld_element_structures.h"
#include "ld_identifier.h"
#include "ld_identifier_container.h"
#include "ld_identifier_database.h"
#include "ld_data_type.h"
#include "ld_data_type_translator.h"
#include "ld_cpp_data_type_translator.h"
#include "ld_root_element.h"
#include "ld_cpp_context.h"
#include "ld_cpp_code_generation_engine.h"
#include "ld_cpp_code_generator_diagnostic.h"
#include "ld_cpp_translator.h"
#include "ld_cpp_literal_translator.h"

namespace Ld {
    CppLiteralTranslator::~CppLiteralTranslator() {}


    QString CppLiteralTranslator::elementName() const {
        return Ld::LiteralElement::elementName;
    }


    bool CppLiteralTranslator::threadImplementation(ElementPointer element, CppCodeGenerationEngine& engine) {
        QSharedPointer<Ld::LiteralElement> literalElement = element.dynamicCast<Ld::LiteralElement>();
        assert(!literalElement.isNull());

        CppContext&    context = engine.context();
        Model::Variant value   = literalElement->convert();
        bool           success;


        unsigned realPrecision = std::numeric_limits<Model::Real>::digits10 + 1;
        switch (value.valueType()) {
            case DataType::ValueType::NONE: {
                success = false;
                break;
            }

            case DataType::ValueType::INTEGER: {
                context(element) << QString("M::Integer(%1)").arg(value.toInteger(&success));
                break;
            }

            case DataType::ValueType::REAL: {
                Model::Real realValue = value.toReal(&success);
                if (success) {
                    std::stringstream stream;
                    stream << "M::Real("
                           << std::fixed << std::setprecision(realPrecision) << realValue << "L"
                           << ")";

                    context(element) << QString::fromStdString(stream.str());
                }

                break;
            }

            case DataType::ValueType::COMPLEX: {
                Model::Complex complexValue = value.toComplex(&success);
                if (success) {
                    std::stringstream stream;
                    stream << "M::Complex("
                           << std::fixed << std::setprecision(realPrecision) << complexValue.real() << "L"
                           << ","
                           << std::fixed << std::setprecision(realPrecision) << complexValue.imag() << "L"
                           << ")";

                    context(element) << QString::fromStdString(stream.str());
                }

                break;
            }

            case DataType::ValueType::TUPLE: {
                Model::Tuple tupleValue = value.toTuple(&success);
                if (success) {
                    char* s = tupleValue.toString();
                    if (s != nullptr) {
                        QByteArray cstr;
                        unsigned long rawLength = static_cast<unsigned long>(std::strlen(s));
                        for (unsigned long i=0 ; i<rawLength ; ++i) {
                            unsigned char c = reinterpret_cast<unsigned char*>(s)[i];
                            if (c == '\\') {
                                cstr.append("\\\\");
                            } else if (c == '\"') {
                                cstr.append("\\\"");
                            } else if (c < 0x20 || c > 0x7F) {
                                cstr.append(
                                    QString("\\x%1")
                                    .arg(static_cast<unsigned>(c), 2, 16, QLatin1Char('0'))
                                    .toLocal8Bit()
                                );
                            } else {
                                cstr.append(c);
                            }
                        }

                        delete[] s;

                        context(element) << QString("M::Tuple(\"%1\")").arg(cstr.data());
                    }
                }

                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        if (!success) {
            engine.translationErrorDetected(
                new CppCodeGeneratorDiagnostic(
                    element,
                    CppCodeGeneratorDiagnostic::Type::FATAL_ERROR,
                    dynamic_cast<const CppTranslationPhase&>(engine.translationPhase()),
                    CppCodeGeneratorDiagnostic::Code::MALFORMED_LITERAL_VALUE,
                    QString(),
                    engine.contextPointer(),
                    CppCodeGeneratorDiagnostic::CompilerCode::NO_ERROR,
                    context.byteOffset(),
                    context.lineNumber(),
                    context.columnNumber()
                )
            );
        }

        return success;
    }


    bool CppLiteralTranslator::methodDefinitions(ElementPointer element, CppCodeGenerationEngine& engine) {
        return threadImplementation(element, engine);
    }
}
