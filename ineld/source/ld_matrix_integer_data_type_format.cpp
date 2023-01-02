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
* This file implements the \ref Ld::MatrixIntegerDataTypeFormat class.
***********************************************************************************************************************/

#include <QString>

#include <cassert>

#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_matrix_data_type_format.h"
#include "ld_integer_data_type_format.h"
#include "ld_matrix_integer_data_type_format.h"

/***********************************************************************************************************************
 * Ld::MatrixIntegerDataTypeFormat::Aggregation
 */

namespace Ld {
    MatrixIntegerDataTypeFormat::Aggregation::Aggregation() {}


    MatrixIntegerDataTypeFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* MatrixIntegerDataTypeFormat::Aggregation::creator(const QString&) {
        return new MatrixIntegerDataTypeFormat::Aggregation;
    }


    QString MatrixIntegerDataTypeFormat::Aggregation::typeName() const {
        return MatrixIntegerDataTypeFormat::formatName;
    }


    void MatrixIntegerDataTypeFormat::Aggregation::clear() {
        IntegerDataTypeFormat::Aggregation::clear();
        MatrixDataTypeFormat::Aggregation::clear();
    }


    bool MatrixIntegerDataTypeFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<MatrixIntegerDataTypeFormat>
            matrixIntegerDataTypeFormat = formatInstance.dynamicCast<MatrixIntegerDataTypeFormat>();
        if (!matrixIntegerDataTypeFormat.isNull()) {
            success = IntegerDataTypeFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                success = MatrixDataTypeFormat::Aggregation::addFormat(formatInstance, true);
            }
        }

        return success;
    }
}

/***********************************************************************************************************************
 * Ld::MatrixIntegerDataTypeFormat
 */

namespace Ld {
    const QString MatrixIntegerDataTypeFormat::formatName("MatrixIntegerDataTypeFormat");

    MatrixIntegerDataTypeFormat::MatrixIntegerDataTypeFormat() {}


    MatrixIntegerDataTypeFormat::MatrixIntegerDataTypeFormat(
            const MatrixIntegerDataTypeFormat& other
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
        ),IntegerDataTypeFormat(
            other
        ) {}


    MatrixIntegerDataTypeFormat::~MatrixIntegerDataTypeFormat() {}


    FormatPointer MatrixIntegerDataTypeFormat::clone() const {
        return FormatPointer(new MatrixIntegerDataTypeFormat(*this));
    }


    Format* MatrixIntegerDataTypeFormat::creator(const QString&) {
        return new MatrixIntegerDataTypeFormat();
    }


    QString MatrixIntegerDataTypeFormat::typeName() const {
        return formatName;
    }


    bool MatrixIntegerDataTypeFormat::isValid() const {
        return MatrixDataTypeFormat::isValid() && IntegerDataTypeFormat::isValid();
    }


    Format::Capabilities MatrixIntegerDataTypeFormat::capabilities() const {
        Capabilities result;

        result += MatrixDataTypeFormat::capabilities();
        result += IntegerDataTypeFormat::capabilities();
        result << formatName;

        return result;
    }


    QString MatrixIntegerDataTypeFormat::toString() const {
        return tr("%1,%2,%3,4,%5").arg(IntegerDataTypeFormat::toString())
                                  .arg(leadingRows())
                                  .arg(trailingRows())
                                  .arg(leadingColumns())
                                  .arg(trailingColumns());
    }


    QString MatrixIntegerDataTypeFormat::toCss() const {
        return tr("%1 ; %2").arg(IntegerDataTypeFormat::toCss()).arg(MatrixDataTypeFormat::toCss());
    }


    void MatrixIntegerDataTypeFormat::writeAddImmediateAttributes(
            XmlAttributes&                  /* attributes */,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
    }

    void MatrixIntegerDataTypeFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  /* reader */,
            const XmlAttributes&       /* attributes */,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
    }


    void MatrixIntegerDataTypeFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        FontFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        DataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        NumericDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        IntegerDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        MatrixDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void MatrixIntegerDataTypeFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        FontFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        DataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        NumericDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        IntegerDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        MatrixDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }
}
