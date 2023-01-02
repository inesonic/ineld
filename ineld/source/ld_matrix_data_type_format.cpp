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
* This file implements the \ref Ld::MatrixDataTypeFormat class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>
#include <QSet>

#include <cassert>

#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_program_file.h"
#include "ld_data_type_format.h"
#include "ld_matrix_data_type_format.h"

/***********************************************************************************************************************
 * Ld::MatrixDataTypeFormat::Aggregation
 */

namespace Ld {
    MatrixDataTypeFormat::Aggregation::Aggregation() {}


    MatrixDataTypeFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* MatrixDataTypeFormat::Aggregation::creator(const QString&) {
        return new MatrixDataTypeFormat::Aggregation;
    }


    QString MatrixDataTypeFormat::Aggregation::typeName() const {
        return MatrixDataTypeFormat::formatName;
    }


    void MatrixDataTypeFormat::Aggregation::clear() {
        DataTypeFormat::Aggregation::clear();

        currentLeadingRows.clear();
        currentTrailingRows.clear();
        currentLeadingColumns.clear();
        currentTrailingColumns.clear();
    }


    bool MatrixDataTypeFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<MatrixDataTypeFormat> matrixDataTypeFormat = formatInstance.dynamicCast<MatrixDataTypeFormat>();
        if (!matrixDataTypeFormat.isNull()) {
            success = DataTypeFormat::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentLeadingRows     << matrixDataTypeFormat->currentLeadingRows;
                currentTrailingRows    << matrixDataTypeFormat->currentTrailingRows;
                currentLeadingColumns  << matrixDataTypeFormat->currentLeadingColumns;
                currentTrailingColumns << matrixDataTypeFormat->currentTrailingColumns;
            }
        }

        return success;
    }


    const QSet<unsigned long>& MatrixDataTypeFormat::Aggregation::leadingRowsCounts() const {
        return currentLeadingRows;
    }


    const QSet<unsigned long>& MatrixDataTypeFormat::Aggregation::trailingRowsCounts() const {
        return currentTrailingRows;
    }


    const QSet<unsigned long>& MatrixDataTypeFormat::Aggregation::leadingColumnsCounts() const {
        return currentLeadingColumns;
    }


    const QSet<unsigned long>& MatrixDataTypeFormat::Aggregation::trailingColumnsCounts() const {
        return currentTrailingColumns;
    }
}

/***********************************************************************************************************************
 * Ld::MatrixDataTypeFormat
 */

namespace Ld {
    const QString MatrixDataTypeFormat::formatName("MatrixDataTypeFormat");

    const unsigned long MatrixDataTypeFormat::defaultLeadingMemberCount  = 9;
    const unsigned long MatrixDataTypeFormat::defaultTrailingMemberCount = 1;
    const unsigned long MatrixDataTypeFormat::showAllMembers             = static_cast<unsigned long>(-1);

    MatrixDataTypeFormat::MatrixDataTypeFormat() {
        currentLeadingRows     = defaultLeadingMemberCount;
        currentTrailingRows    = defaultTrailingMemberCount;
        currentLeadingColumns  = defaultLeadingMemberCount;
        currentTrailingColumns = defaultTrailingMemberCount;
    }


    MatrixDataTypeFormat::MatrixDataTypeFormat(
            const MatrixDataTypeFormat& other
        ):Format(
            other
        ),FontFormat(
            other
        ),DataTypeFormat(
            other
        ),currentLeadingRows(
            other.currentLeadingRows
        ),currentTrailingRows(
            other.currentTrailingRows
        ),currentLeadingColumns(
            other.currentLeadingColumns
        ),currentTrailingColumns(
            other.currentTrailingColumns
        ) {}


    MatrixDataTypeFormat::~MatrixDataTypeFormat() {}


    FormatPointer MatrixDataTypeFormat::clone() const {
        return FormatPointer(new MatrixDataTypeFormat(*this));
    }


    Format* MatrixDataTypeFormat::creator(const QString&) {
        return new MatrixDataTypeFormat();
    }


    QString MatrixDataTypeFormat::typeName() const {
        return formatName;
    }


    bool MatrixDataTypeFormat::isValid() const {
        return DataTypeFormat::isValid();
    }


    Format::Capabilities MatrixDataTypeFormat::capabilities() const {
        return DataTypeFormat::capabilities() << formatName;
    }


    unsigned long MatrixDataTypeFormat::leadingRows() const {
        return currentLeadingRows;
    }


    void MatrixDataTypeFormat::setLeadingRows(unsigned long newNumberLeadingRows) {
        currentLeadingRows = newNumberLeadingRows;
        reportFormatUpdated();
    }


    unsigned long MatrixDataTypeFormat::trailingRows() const {
        return currentTrailingRows;
    }


    void MatrixDataTypeFormat::setTrailingRows(unsigned long newNumberTrailingRows) {
        currentTrailingRows = newNumberTrailingRows;
        reportFormatUpdated();
    }


    unsigned long MatrixDataTypeFormat::leadingColumns() const {
        return currentLeadingColumns;
    }


    void MatrixDataTypeFormat::setLeadingColumns(unsigned long newNumberLeadingColumns) {
        currentLeadingColumns = newNumberLeadingColumns;
        reportFormatUpdated();
    }


    unsigned long MatrixDataTypeFormat::trailingColumns() const {
        return currentTrailingColumns;
    }


    void MatrixDataTypeFormat::setTrailingColumns(unsigned long newNumberTrailingColumns) {
        currentTrailingColumns = newNumberTrailingColumns;
        reportFormatUpdated();
    }


    QString MatrixDataTypeFormat::toString() const {
        return QString("%1,%2,%3,%4,%5").arg(DataTypeFormat::toString())
                                        .arg(currentLeadingRows)
                                        .arg(currentTrailingRows)
                                        .arg(currentLeadingColumns)
                                        .arg(currentTrailingColumns);
    }


    QString MatrixDataTypeFormat::toCss() const {
        QString result = DataTypeFormat::toCss();
        return result;
    }


    void MatrixDataTypeFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (currentLeadingRows != defaultLeadingMemberCount) {
            attributes.append("leading_rows", currentLeadingRows);
        }

        if (currentTrailingRows != defaultTrailingMemberCount) {
            attributes.append("trailing_rows", currentTrailingRows);
        }

        if (currentLeadingColumns != defaultLeadingMemberCount) {
            attributes.append("leading_columns", currentLeadingColumns);
        }

        if (currentTrailingColumns != defaultTrailingMemberCount) {
            attributes.append("trailing_columns", currentTrailingColumns);
        }
    }


    void MatrixDataTypeFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        if (!reader->hasError()) {
            bool ok;
            currentLeadingRows = attributes.value<unsigned long>("leading_rows", defaultLeadingMemberCount, &ok);
            if (!ok) {
                QString tag   = reader->qualifiedName().toString();
                QString value = attributes.value<QString>("leading_rows");
                reader->raiseError(tr("Tag \"%1\", invalid \"leading_rows\" attribute, \"%12\"").arg(tag).arg(value));
            }
        }

        if (!reader->hasError()) {
            bool ok;
            currentTrailingRows = attributes.value<unsigned long>("trailing_rows", defaultTrailingMemberCount, &ok);
            if (!ok) {
                QString tag   = reader->qualifiedName().toString();
                QString value = attributes.value<QString>("trailing_rows");
                reader->raiseError(tr("Tag \"%1\", invalid \"trailing_rows\" attribute, \"%12\"").arg(tag).arg(value));
            }
        }

        if (!reader->hasError()) {
            bool ok;
            currentLeadingColumns = attributes.value<unsigned long>("leading_columns", defaultLeadingMemberCount, &ok);
            if (!ok) {
                QString tag   = reader->qualifiedName().toString();
                QString value = attributes.value<QString>("leading_columns");
                reader->raiseError(
                    tr("Tag \"%1\", invalid \"leading_columns\" attribute, \"%12\"").arg(tag).arg(value)
                );
            }
        }

        if (!reader->hasError()) {
            bool ok;
            currentTrailingColumns = attributes.value<unsigned long>(
                "trailing_columns",
                defaultTrailingMemberCount,
                &ok
            );

            if (!ok) {
                QString tag   = reader->qualifiedName().toString();
                QString value = attributes.value<QString>("trailing_columns");
                reader->raiseError(
                    tr("Tag \"%1\", invalid \"trailing_columns\" attribute, \"%12\"").arg(tag).arg(value)
                );
            }
        }
    }


    void MatrixDataTypeFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        FontFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        DataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        MatrixDataTypeFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void MatrixDataTypeFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        FontFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        DataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        MatrixDataTypeFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }
}
