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
* This file implements the \ref Ld::MatrixBooleanDataTypeFormat class.
***********************************************************************************************************************/

#include <QString>

#include <cassert>

#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_matrix_data_type_format.h"
#include "ld_boolean_data_type_format.h"
#include "ld_matrix_boolean_data_type_format.h"

/***********************************************************************************************************************
 * Ld::MatrixBooleanDataTypeFormat::Aggregation
 */

namespace Ld {
    MatrixBooleanDataTypeFormat::Aggregation::Aggregation() {}


    MatrixBooleanDataTypeFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* MatrixBooleanDataTypeFormat::Aggregation::creator(const QString&) {
        return new MatrixBooleanDataTypeFormat::Aggregation;
    }


    QString MatrixBooleanDataTypeFormat::Aggregation::typeName() const {
        return MatrixBooleanDataTypeFormat::formatName;
    }


    void MatrixBooleanDataTypeFormat::Aggregation::clear() {
        BooleanDataTypeFormat::Aggregation::clear();
        MatrixDataTypeFormat::Aggregation::clear();
    }


    bool MatrixBooleanDataTypeFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<MatrixBooleanDataTypeFormat>
            matrixBooleanDataTypeFormat = formatInstance.dynamicCast<MatrixBooleanDataTypeFormat>();
        if (!matrixBooleanDataTypeFormat.isNull()) {
            success = BooleanDataTypeFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                success = MatrixDataTypeFormat::Aggregation::addFormat(formatInstance, true);
            }
        }

        return success;
    }
}

/***********************************************************************************************************************
 * Ld::MatrixBooleanDataTypeFormat
 */

namespace Ld {
    const QString MatrixBooleanDataTypeFormat::formatName("MatrixBooleanDataTypeFormat");

    MatrixBooleanDataTypeFormat::MatrixBooleanDataTypeFormat() {}


    MatrixBooleanDataTypeFormat::MatrixBooleanDataTypeFormat(
            const MatrixBooleanDataTypeFormat& other
        ):Format(
            other
        ),FontFormat(
            other
        ),DataTypeFormat(
            other
        ),MatrixDataTypeFormat(
            other
        ),BooleanDataTypeFormat(
            other
        ) {}


    MatrixBooleanDataTypeFormat::~MatrixBooleanDataTypeFormat() {}


    FormatPointer MatrixBooleanDataTypeFormat::clone() const {
        return FormatPointer(new MatrixBooleanDataTypeFormat(*this));
    }


    Format* MatrixBooleanDataTypeFormat::creator(const QString&) {
        return new MatrixBooleanDataTypeFormat();
    }


    QString MatrixBooleanDataTypeFormat::typeName() const {
        return formatName;
    }


    bool MatrixBooleanDataTypeFormat::isValid() const {
        return MatrixDataTypeFormat::isValid() && BooleanDataTypeFormat::isValid();
    }


    Format::Capabilities MatrixBooleanDataTypeFormat::capabilities() const {
        Capabilities result;

        result += MatrixDataTypeFormat::capabilities();
        result += BooleanDataTypeFormat::capabilities();
        result << formatName;

        return result;
    }


    QString MatrixBooleanDataTypeFormat::toString() const {
        return tr("%1,%2,%3,4,%5").arg(BooleanDataTypeFormat::toString())
                                  .arg(leadingRows())
                                  .arg(trailingRows())
                                  .arg(leadingColumns())
                                  .arg(trailingColumns());
    }


    QString MatrixBooleanDataTypeFormat::toCss() const {
        return tr("%1 ; %2").arg(BooleanDataTypeFormat::toCss()).arg(MatrixDataTypeFormat::toCss());
    }


    void MatrixBooleanDataTypeFormat::writeAddImmediateAttributes(
            XmlAttributes&                  /* attributes */,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
    }

    void MatrixBooleanDataTypeFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  /* reader */,
            const XmlAttributes&       /* attributes */,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
    }


    void MatrixBooleanDataTypeFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        FontFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        DataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        BooleanDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        MatrixDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void MatrixBooleanDataTypeFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        FontFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        DataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        BooleanDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        MatrixDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }
}
