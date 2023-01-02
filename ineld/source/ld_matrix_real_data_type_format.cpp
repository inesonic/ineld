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
* This file implements the \ref Ld::MatrixRealDataTypeFormat class.
***********************************************************************************************************************/

#include <QString>

#include <cassert>

#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_matrix_data_type_format.h"
#include "ld_real_data_type_format.h"
#include "ld_matrix_real_data_type_format.h"

/***********************************************************************************************************************
 * Ld::MatrixRealDataTypeFormat::Aggregation
 */

namespace Ld {
    MatrixRealDataTypeFormat::Aggregation::Aggregation() {}


    MatrixRealDataTypeFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* MatrixRealDataTypeFormat::Aggregation::creator(const QString&) {
        return new MatrixRealDataTypeFormat::Aggregation;
    }


    QString MatrixRealDataTypeFormat::Aggregation::typeName() const {
        return MatrixRealDataTypeFormat::formatName;
    }


    void MatrixRealDataTypeFormat::Aggregation::clear() {
        RealDataTypeFormat::Aggregation::clear();
        MatrixDataTypeFormat::Aggregation::clear();
    }


    bool MatrixRealDataTypeFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<MatrixRealDataTypeFormat>
            matrixRealDataTypeFormat = formatInstance.dynamicCast<MatrixRealDataTypeFormat>();
        if (!matrixRealDataTypeFormat.isNull()) {
            success = RealDataTypeFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                success = MatrixDataTypeFormat::Aggregation::addFormat(formatInstance, true);
            }
        }

        return success;
    }
}

/***********************************************************************************************************************
 * Ld::MatrixRealDataTypeFormat
 */

namespace Ld {
    const QString MatrixRealDataTypeFormat::formatName("MatrixRealDataTypeFormat");

    MatrixRealDataTypeFormat::MatrixRealDataTypeFormat() {}


    MatrixRealDataTypeFormat::MatrixRealDataTypeFormat(
            const MatrixRealDataTypeFormat& other
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
        ) {}


    MatrixRealDataTypeFormat::~MatrixRealDataTypeFormat() {}


    FormatPointer MatrixRealDataTypeFormat::clone() const {
        return FormatPointer(new MatrixRealDataTypeFormat(*this));
    }


    Format* MatrixRealDataTypeFormat::creator(const QString&) {
        return new MatrixRealDataTypeFormat();
    }


    QString MatrixRealDataTypeFormat::typeName() const {
        return formatName;
    }


    bool MatrixRealDataTypeFormat::isValid() const {
        return MatrixDataTypeFormat::isValid() && RealDataTypeFormat::isValid();
    }


    Format::Capabilities MatrixRealDataTypeFormat::capabilities() const {
        Capabilities result;

        result += MatrixDataTypeFormat::capabilities();
        result += RealDataTypeFormat::capabilities();
        result << formatName;

        return result;
    }


    QString MatrixRealDataTypeFormat::toString() const {
        return tr("%1,%2,%3,4,%5").arg(RealDataTypeFormat::toString())
                                  .arg(leadingRows())
                                  .arg(trailingRows())
                                  .arg(leadingColumns())
                                  .arg(trailingColumns());
    }


    QString MatrixRealDataTypeFormat::toCss() const {
        return tr("%1 ; %2").arg(RealDataTypeFormat::toCss()).arg(MatrixDataTypeFormat::toCss());
    }


    void MatrixRealDataTypeFormat::writeAddImmediateAttributes(
            XmlAttributes&                  /* attributes */,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
    }

    void MatrixRealDataTypeFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  /* reader */,
            const XmlAttributes&       /* attributes */,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
    }


    void MatrixRealDataTypeFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        FontFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        DataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        NumericDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        RealDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        MatrixDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void MatrixRealDataTypeFormat::readAttributes(
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
        MatrixDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }
}
