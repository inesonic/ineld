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
* This file implements the \ref Ld::UnifiedDataTypeFormat class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>
#include <QSet>
#include <QStringList>

#include <cassert>

#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_program_file.h"
#include "ld_boolean_data_type_format.h"
#include "ld_integer_data_type_format.h"
#include "ld_real_data_type_format.h"
#include "ld_complex_data_type_format.h"
#include "ld_list_data_type_format.h"
#include "ld_set_data_type_format.h"
#include "ld_tuple_data_type_format.h"
#include "ld_matrix_data_type_format.h"
#include "ld_matrix_boolean_data_type_format.h"
#include "ld_matrix_integer_data_type_format.h"
#include "ld_matrix_real_data_type_format.h"
#include "ld_matrix_complex_data_type_format.h"
#include "ld_unified_data_type_format.h"

/***********************************************************************************************************************
 * Ld::UnifiedDataTypeFormat::Aggregation
 */

namespace Ld {
    UnifiedDataTypeFormat::Aggregation::Aggregation() {}


    UnifiedDataTypeFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* UnifiedDataTypeFormat::Aggregation::creator(const QString&) {
        return new UnifiedDataTypeFormat::Aggregation;
    }


    QString UnifiedDataTypeFormat::Aggregation::typeName() const {
        return UnifiedDataTypeFormat::formatName;
    }


    void UnifiedDataTypeFormat::Aggregation::clear() {
        BooleanDataTypeFormat::Aggregation::clear(); // Includes MatrixBoolean
        IntegerDataTypeFormat::Aggregation::clear(); // Includes MatrixInteger
        ComplexDataTypeFormat::Aggregation::clear(); // Complex includes real and matrix types
        ListDataTypeFormat::Aggregation::clear(); // Includes SetDataTypeFormat
        TupleDataTypeFormat::Aggregation::clear();
        MatrixDataTypeFormat::Aggregation::clear(); // Includes all matrix types

        currentTypeNames.clear();
    }


    bool UnifiedDataTypeFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<UnifiedDataTypeFormat>
            unifiedDataTypeFormat = formatInstance.dynamicCast<UnifiedDataTypeFormat>();
        if (!unifiedDataTypeFormat.isNull()) {
            success = BooleanDataTypeFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                success = IntegerDataTypeFormat::Aggregation::addFormat(formatInstance, true);
            }

            if (success) {
                success = ComplexDataTypeFormat::Aggregation::addFormat(formatInstance, true); // Complex includes real
            }

            if (success) {
                // ListDataTypeFormat addresses SetDataTypeFormat
                success = ListDataTypeFormat::Aggregation::addFormat(formatInstance, true);
            }

            if (success) {
                success = TupleDataTypeFormat::Aggregation::addFormat(formatInstance, true);
            }

            if (success) {
                success = MatrixDataTypeFormat::Aggregation::addFormat(formatInstance, true);
            }

            if (success) {
                currentTypeNames << unifiedDataTypeFormat->currentTypeName;
            }
        }

        return success;
    }


    QSet<QString> UnifiedDataTypeFormat::Aggregation::typeNames() const {
        return currentTypeNames;
    }
}

/***********************************************************************************************************************
 * Ld::UnifiedDataTypeFormat
 */

namespace Ld {
    const QString UnifiedDataTypeFormat::formatName("UnifiedDataTypeFormat");

    const UnifiedDataTypeFormat::ImaginaryUnit
        UnifiedDataTypeFormat::defaultImaginaryUnit = UnifiedDataTypeFormat::ImaginaryUnit::I;

    UnifiedDataTypeFormat::UnifiedDataTypeFormat() {
        currentTypeName = formatName;
    }


    UnifiedDataTypeFormat::UnifiedDataTypeFormat(
            const UnifiedDataTypeFormat& other
        ):Format(
            other
        ),FontFormat(
            other
        ),DataTypeFormat(
            other
        ),BooleanDataTypeFormat(
            other
        ),NumericDataTypeFormat(
            other
        ),IntegerDataTypeFormat(
            other
        ),RealDataTypeFormat(
            other
        ),ComplexDataTypeFormat(
            other
        ),ListDataTypeFormat(
            other
        ),SetDataTypeFormat(
            other
        ),TupleDataTypeFormat(
            other
        ),MatrixDataTypeFormat(
            other
        ),MatrixBooleanDataTypeFormat(
            other
        ),MatrixIntegerDataTypeFormat(
            other
        ),MatrixRealDataTypeFormat(
            other
        ),MatrixComplexDataTypeFormat(
            other
        ) {
        currentTypeName = other.currentTypeName;
    }


    UnifiedDataTypeFormat::~UnifiedDataTypeFormat() {}


    FormatPointer UnifiedDataTypeFormat::clone() const {
        return FormatPointer(new UnifiedDataTypeFormat(*this));
    }


    Format* UnifiedDataTypeFormat::creator(const QString&) {
        return new UnifiedDataTypeFormat();
    }


    QString UnifiedDataTypeFormat::typeName() const {
        return currentTypeName;
    }


    void UnifiedDataTypeFormat::setTypeName(const QString& newTypeName) {
        currentTypeName = newTypeName;
    }


    void UnifiedDataTypeFormat::setType(UnifiedDataTypeFormat::Type newType) {
        switch (newType) {
            case Type::NONE: {
                currentTypeName = formatName;
                break;
            }

            case Type::BOOLEAN: {
                currentTypeName = BooleanDataTypeFormat::formatName;
                break;
            }

            case Type::INTEGER: {
                currentTypeName = IntegerDataTypeFormat::formatName;
                break;
            }

            case Type::REAL: {
                currentTypeName = RealDataTypeFormat::formatName;
                break;
            }

            case Type::COMPLEX: {
                currentTypeName = ComplexDataTypeFormat::formatName;
                break;
            }

            case Type::SET: {
                currentTypeName = SetDataTypeFormat::formatName;
                break;
            }

            case Type::TUPLE: {
                currentTypeName = TupleDataTypeFormat::formatName;
                break;
            }

            case Type::MATRIX_BOOLEAN: {
                currentTypeName = MatrixBooleanDataTypeFormat::formatName;
                break;
            }

            case Type::MATRIX_INTEGER: {
                currentTypeName = MatrixIntegerDataTypeFormat::formatName;
                break;
            }

            case Type::MATRIX_REAL: {
                currentTypeName = MatrixRealDataTypeFormat::formatName;
                break;
            }

            case Type::MATRIX_COMPLEX: {
                currentTypeName = MatrixComplexDataTypeFormat::formatName;
                break;
            }

            default: {
                Q_ASSERT(false);
                break;
            }
        }
    }


    bool UnifiedDataTypeFormat::isValid() const {
        bool result;

        if (currentTypeName == BooleanDataTypeFormat::formatName) {
            result = BooleanDataTypeFormat::isValid();
        } else  if (currentTypeName == IntegerDataTypeFormat::formatName) {
            result = IntegerDataTypeFormat::isValid();
        } else if (currentTypeName == RealDataTypeFormat::formatName) {
            result = RealDataTypeFormat::isValid();
        } else if (currentTypeName == ComplexDataTypeFormat::formatName) {
            result = ComplexDataTypeFormat::isValid();
        } else if (currentTypeName == ListDataTypeFormat::formatName) {
            result = ListDataTypeFormat::isValid();
        } else if (currentTypeName == SetDataTypeFormat::formatName) {
            result = SetDataTypeFormat::isValid();
        } else if (currentTypeName == TupleDataTypeFormat::formatName) {
            result = TupleDataTypeFormat::isValid();
        } else if (currentTypeName == MatrixBooleanDataTypeFormat::formatName) {
            result = MatrixBooleanDataTypeFormat::isValid();
        } else if (currentTypeName == MatrixIntegerDataTypeFormat::formatName) {
            result = MatrixIntegerDataTypeFormat::isValid();
        } else if (currentTypeName == MatrixRealDataTypeFormat::formatName) {
            result = MatrixRealDataTypeFormat::isValid();
        } else if (currentTypeName == MatrixComplexDataTypeFormat::formatName) {
            result = MatrixComplexDataTypeFormat::isValid();
        } else {
            result = false;
        }

        return result;
    }


    Format::Capabilities UnifiedDataTypeFormat::capabilities() const {
        Capabilities result;

        if (currentTypeName == BooleanDataTypeFormat::formatName) {
            result = BooleanDataTypeFormat::capabilities();
        } else if (currentTypeName == IntegerDataTypeFormat::formatName) {
                result = IntegerDataTypeFormat::capabilities();
        } else if (currentTypeName == RealDataTypeFormat::formatName) {
            result = RealDataTypeFormat::capabilities();
        } else if (currentTypeName == ComplexDataTypeFormat::formatName) {
            result = ComplexDataTypeFormat::capabilities();
        } else if (currentTypeName == ListDataTypeFormat::formatName) {
            result = ListDataTypeFormat::capabilities();
        } else if (currentTypeName == SetDataTypeFormat::formatName) {
            result = SetDataTypeFormat::capabilities();
        } else if (currentTypeName == TupleDataTypeFormat::formatName) {
            result = TupleDataTypeFormat::capabilities();
        } else if (currentTypeName == MatrixBooleanDataTypeFormat::formatName) {
            result = MatrixBooleanDataTypeFormat::capabilities();
        } else if (currentTypeName == MatrixIntegerDataTypeFormat::formatName) {
            result = MatrixIntegerDataTypeFormat::capabilities();
        } else if (currentTypeName == MatrixRealDataTypeFormat::formatName) {
            result = MatrixRealDataTypeFormat::capabilities();
        } else if (currentTypeName == MatrixComplexDataTypeFormat::formatName) {
            result = MatrixComplexDataTypeFormat::capabilities();
        } else {
            result = DataTypeFormat::capabilities();
        }

        result << formatName;

        return result;
    }


    QString UnifiedDataTypeFormat::toString() const {
        QStringList booleanFields = BooleanDataTypeFormat::toString().split(",");
        QStringList integerFields = IntegerDataTypeFormat::toString().split(",");
        QStringList complexFields = ComplexDataTypeFormat::toString().split(",");
        QStringList listFields    = ListDataTypeFormat::toString().split(",");
        QStringList tupleFields   = TupleDataTypeFormat::toString().split(",");
        QStringList matrixFields  = MatrixDataTypeFormat::toString().split(",");

        booleanFields.removeFirst();
        integerFields.removeFirst();
        complexFields.removeFirst();
        listFields.removeFirst();
        tupleFields.removeFirst();

        unsigned numberBooleanFields = static_cast<unsigned>(booleanFields.size());
        unsigned booleanIndex        = 0;

        while (booleanIndex < numberBooleanFields                      &&
               !complexFields.isEmpty()                                &&
               booleanFields.at(booleanIndex) == integerFields.first() &&
               booleanFields.at(booleanIndex) == complexFields.first() &&
               booleanFields.at(booleanIndex) == listFields.first()    &&
               booleanFields.at(booleanIndex) == tupleFields.first()   &&
               booleanFields.at(booleanIndex) == matrixFields.first()     ) {
            integerFields.removeFirst();
            complexFields.removeFirst();
            listFields.removeFirst();
            tupleFields.removeFirst();
            matrixFields.removeFirst();

            ++booleanIndex;
        }

        return QString("%1,%2,%3,%4,%5,%6,%7,%8")
               .arg(formatName)
               .arg(currentTypeName)
               .arg(booleanFields.join(","))
               .arg(integerFields.join(","))
               .arg(complexFields.join(","))
               .arg(listFields.join(","))
               .arg(tupleFields.join(","))
               .arg(matrixFields.join(","));
    }


    QString UnifiedDataTypeFormat::toCss() const {
        QString result;

        if (currentTypeName == BooleanDataTypeFormat::formatName) {
            result = BooleanDataTypeFormat::toCss();
        } else if (currentTypeName == IntegerDataTypeFormat::formatName) {
                result = IntegerDataTypeFormat::toCss();
        } else if (currentTypeName == RealDataTypeFormat::formatName) {
            result = RealDataTypeFormat::toCss();
        } else if (currentTypeName == ComplexDataTypeFormat::formatName) {
            result = ComplexDataTypeFormat::toCss();
        } else if (currentTypeName == ListDataTypeFormat::formatName) {
            result = ListDataTypeFormat::toCss();
        } else if (currentTypeName == SetDataTypeFormat::formatName) {
            result = SetDataTypeFormat::toCss();
        } else if (currentTypeName == TupleDataTypeFormat::formatName) {
            result = TupleDataTypeFormat::toCss();
        } else if (currentTypeName == MatrixBooleanDataTypeFormat::formatName) {
            result = MatrixBooleanDataTypeFormat::toCss();
        } else if (currentTypeName == MatrixIntegerDataTypeFormat::formatName) {
            result = MatrixIntegerDataTypeFormat::toCss();
        } else if (currentTypeName == MatrixRealDataTypeFormat::formatName) {
            result = MatrixRealDataTypeFormat::toCss();
        } else if (currentTypeName == MatrixComplexDataTypeFormat::formatName) {
            result = MatrixComplexDataTypeFormat::toCss();
        } else {
            result = NumericDataTypeFormat::toCss();
        }

        return result;
    }


    void UnifiedDataTypeFormat::writeAddImmediateAttributes(
            XmlAttributes&                  /* attributes */,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
    }


    void UnifiedDataTypeFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  /* reader */,
            const XmlAttributes&       /* attributes */,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
    }


    void UnifiedDataTypeFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        FontFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        DataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        NumericDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        BooleanDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        IntegerDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        RealDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        ComplexDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        ListDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        TupleDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        MatrixDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        MatrixBooleanDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        MatrixIntegerDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        MatrixRealDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        MatrixComplexDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        UnifiedDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void UnifiedDataTypeFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        FontFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        DataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        NumericDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        BooleanDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        IntegerDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        RealDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        ComplexDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        ListDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        TupleDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        MatrixDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        MatrixBooleanDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        MatrixIntegerDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        MatrixRealDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        MatrixComplexDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        UnifiedDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }
}
