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
* This file implements the \ref Ld::ValueFieldFormat class.
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
#include "ld_variable_name.h"
#include "ld_unified_data_type_format.h"
#include "ld_value_field_format.h"

/***********************************************************************************************************************
 * Ld::ValueFieldFormat::Aggregation
 */

namespace Ld {
    ValueFieldFormat::Aggregation::Aggregation() {}


    ValueFieldFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* ValueFieldFormat::Aggregation::creator(const QString&) {
        return new ValueFieldFormat::Aggregation;
    }


    QString ValueFieldFormat::Aggregation::typeName() const {
        return ValueFieldFormat::formatName;
    }


    void ValueFieldFormat::Aggregation::clear() {
        UnifiedDataTypeFormat::Aggregation::clear();

        currentVariableNames.clear();
        currentText1.clear();
        currentText2.clear();
    }


    bool ValueFieldFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<ValueFieldFormat> valueFieldFormat = formatInstance.dynamicCast<ValueFieldFormat>();
        if (!valueFieldFormat.isNull()) {
            success = UnifiedDataTypeFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentVariableNames << valueFieldFormat->variableName();
                currentText1 << valueFieldFormat->variableName().text1();
                currentText2 << valueFieldFormat->variableName().text2();
            }
        }

        return success;
    }


    const QSet<VariableName>& ValueFieldFormat::Aggregation::variableNames() const {
        return currentVariableNames;
    }


    const QSet<QString>& ValueFieldFormat::Aggregation::text1() const {
        return currentText1;
    }


    const QSet<QString>& ValueFieldFormat::Aggregation::text2() const {
        return currentText2;
    }
}

/***********************************************************************************************************************
 * Ld::ValueFieldFormat
 */

namespace Ld {
    const QString ValueFieldFormat::formatName("ValueFieldFormat");

    ValueFieldFormat::ValueFieldFormat() {}


    ValueFieldFormat::ValueFieldFormat(
            const ValueFieldFormat& other
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
        ),UnifiedDataTypeFormat(
            other
        ) {
        currentVariableName = other.currentVariableName;
    }


    ValueFieldFormat::~ValueFieldFormat() {}


    FormatPointer ValueFieldFormat::clone() const {
        return FormatPointer(new ValueFieldFormat(*this));
    }


    Format* ValueFieldFormat::creator(const QString&) {
        return new ValueFieldFormat();
    }


    QString ValueFieldFormat::typeName() const {
        return formatName;
    }


    bool ValueFieldFormat::isValid() const {
        return !currentVariableName.text1().isEmpty() && UnifiedDataTypeFormat::isValid();
    }


    Format::Capabilities ValueFieldFormat::capabilities() const {
        return UnifiedDataTypeFormat::capabilities() << formatName;
    }


    void ValueFieldFormat::setVariableName(const VariableName& variableName) {
        currentVariableName = variableName;
        reportFormatUpdated();
    }


    void ValueFieldFormat::setVariableName(const QString& text1, const QString& text2) {
        currentVariableName.setText1(text1);
        currentVariableName.setText2(text2);

        reportFormatUpdated();
    }


    const VariableName& ValueFieldFormat::variableName() const {
        return currentVariableName;
    }


    QString ValueFieldFormat::toString() const {
        return QString("%1,%2,%3")
               .arg(UnifiedDataTypeFormat::toString())
               .arg(currentVariableName.text1())
               .arg(currentVariableName.text2());
    }


    QString ValueFieldFormat::toCss() const {
        QString result = UnifiedDataTypeFormat::toCss();
        return result;
    }


    void ValueFieldFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        attributes.append("text1", currentVariableName.text1());
        attributes.append("text2", currentVariableName.text2());
    }


    void ValueFieldFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        if (!reader->hasError()) {
            currentVariableName.setText1(attributes.value<QString>("text1", QString()));
            currentVariableName.setText2(attributes.value<QString>("text2", QString()));
        }
    }


    void ValueFieldFormat::writeAddAttributes(
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
        UnifiedDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        ValueFieldFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void ValueFieldFormat::readAttributes(
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
        UnifiedDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        ValueFieldFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }
}
