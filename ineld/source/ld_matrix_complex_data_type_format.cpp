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
* This file implements the \ref Ld::MatrixComplexDataTypeFormat class.
***********************************************************************************************************************/

#include <QString>

#include <cassert>

#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_matrix_data_type_format.h"
#include "ld_complex_data_type_format.h"
#include "ld_matrix_complex_data_type_format.h"

/***********************************************************************************************************************
 * Ld::MatrixComplexDataTypeFormat::Aggregation
 */

namespace Ld {
    MatrixComplexDataTypeFormat::Aggregation::Aggregation() {}


    MatrixComplexDataTypeFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* MatrixComplexDataTypeFormat::Aggregation::creator(const QString&) {
        return new MatrixComplexDataTypeFormat::Aggregation;
    }


    QString MatrixComplexDataTypeFormat::Aggregation::typeName() const {
        return MatrixComplexDataTypeFormat::formatName;
    }


    void MatrixComplexDataTypeFormat::Aggregation::clear() {
        ComplexDataTypeFormat::Aggregation::clear();
        MatrixDataTypeFormat::Aggregation::clear();
    }


    bool MatrixComplexDataTypeFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<MatrixComplexDataTypeFormat>
            matrixComplexDataTypeFormat = formatInstance.dynamicCast<MatrixComplexDataTypeFormat>();
        if (!matrixComplexDataTypeFormat.isNull()) {
            success = ComplexDataTypeFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                success = MatrixDataTypeFormat::Aggregation::addFormat(formatInstance, true);
            }
        }

        return success;
    }
}

/***********************************************************************************************************************
 * Ld::MatrixComplexDataTypeFormat
 */

namespace Ld {
    const QString MatrixComplexDataTypeFormat::formatName("MatrixComplexDataTypeFormat");

    MatrixComplexDataTypeFormat::MatrixComplexDataTypeFormat() {}


    MatrixComplexDataTypeFormat::MatrixComplexDataTypeFormat(
            const MatrixComplexDataTypeFormat& other
        ):Format(
            other
        ),FontFormat(
            other
        ),DataTypeFormat(
            other
        ),MatrixDataTypeFormat(
            other
        ),NumericDataTypeFormat(
              other
        ),RealDataTypeFormat(
            other
        ),ComplexDataTypeFormat(
            other
        ) {}


    MatrixComplexDataTypeFormat::~MatrixComplexDataTypeFormat() {}


    FormatPointer MatrixComplexDataTypeFormat::clone() const {
        return FormatPointer(new MatrixComplexDataTypeFormat(*this));
    }


    Format* MatrixComplexDataTypeFormat::creator(const QString&) {
        return new MatrixComplexDataTypeFormat();
    }


    QString MatrixComplexDataTypeFormat::typeName() const {
        return formatName;
    }


    bool MatrixComplexDataTypeFormat::isValid() const {
        return MatrixDataTypeFormat::isValid() && ComplexDataTypeFormat::isValid();
    }


    Format::Capabilities MatrixComplexDataTypeFormat::capabilities() const {
        Capabilities result;

        result += MatrixDataTypeFormat::capabilities();
        result += ComplexDataTypeFormat::capabilities();
        result << formatName;

        return result;
    }


    QString MatrixComplexDataTypeFormat::toString() const {
        return tr("%1,%2,%3,4,%5").arg(ComplexDataTypeFormat::toString())
                                  .arg(leadingRows())
                                  .arg(trailingRows())
                                  .arg(leadingColumns())
                                  .arg(trailingColumns());
    }


    QString MatrixComplexDataTypeFormat::toCss() const {
        return tr("%1 ; %2").arg(ComplexDataTypeFormat::toCss()).arg(MatrixDataTypeFormat::toCss());
    }


    void MatrixComplexDataTypeFormat::writeAddImmediateAttributes(
            XmlAttributes&                  /* attributes */,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
    }

    void MatrixComplexDataTypeFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  /* reader */,
            const XmlAttributes&       /* attributes */,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
    }


    void MatrixComplexDataTypeFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        FontFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        DataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        NumericDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        RealDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        ComplexDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        MatrixDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void MatrixComplexDataTypeFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        FontFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        DataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        NumericDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        RealDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        ComplexDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        MatrixDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }
}
