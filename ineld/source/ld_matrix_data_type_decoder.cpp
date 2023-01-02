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
* \filef
*
* This file implements the \ref Ld::MatrixDataTypeDecoder class.
***********************************************************************************************************************/

#include <algorithm>

#include <model_matrix.h>

#include "ld_matrix_data_type_format.h"
#include "ld_value_field_format.h"
#include "ld_data_type_decoder.h"
#include "ld_matrix_data_type_decoder.h"

/***********************************************************************************************************************
 * Ld::MatrixDataTypeDecoder::Context:
 */

namespace Ld {
    MatrixDataTypeDecoder::Context::Context(FormatPointer format):currentFormat(format) {}


    MatrixDataTypeDecoder::Context::~Context() {}


    FormatPointer MatrixDataTypeDecoder::Context::format() const {
        return currentFormat;
    }


    unsigned long MatrixDataTypeDecoder::Context::numberLeadingRows() const {
        return currentNumberLeadingRows;
    }


    unsigned long MatrixDataTypeDecoder::Context::numberTrailingRows() const {
        return currentNumberTrailingRows;
    }


    unsigned long MatrixDataTypeDecoder::Context::firstTrailingRow() const {
        return currentFirstTrailingRow;
    }


    unsigned long MatrixDataTypeDecoder::Context::numberLeadingColumns() const {
        return currentNumberLeadingColumns;
    }


    unsigned long MatrixDataTypeDecoder::Context::numberTrailingColumns() const {
        return currentNumberTrailingColumns;
    }


    unsigned long MatrixDataTypeDecoder::Context::firstTrailingColumn() const {
        return currentFirstTrailingColumn;
    }


    void MatrixDataTypeDecoder::Context::configure(unsigned long numberRows, unsigned long numberColumns) {
        if (!format().isNull()                                                            &&
            (format()->capabilities().contains(Ld::MatrixDataTypeFormat::formatName) ||
             format()->capabilities().contains(Ld::ValueFieldFormat::formatName)        )    ) {
            QSharedPointer<MatrixDataTypeFormat> matrixFormat = format().dynamicCast<MatrixDataTypeFormat>();

            currentNumberLeadingRows     = matrixFormat->leadingRows();
            currentNumberTrailingRows    = matrixFormat->trailingRows();
            currentNumberLeadingColumns  = matrixFormat->leadingColumns();
            currentNumberTrailingColumns = matrixFormat->trailingColumns();
        } else {
            currentNumberLeadingRows     = MatrixDataTypeFormat::defaultLeadingMemberCount;
            currentNumberTrailingRows    = MatrixDataTypeFormat::defaultTrailingMemberCount;

            currentNumberLeadingColumns  = MatrixDataTypeFormat::defaultLeadingMemberCount;
            currentNumberTrailingColumns = MatrixDataTypeFormat::defaultTrailingMemberCount;
        }

        if (currentNumberLeadingRows == MatrixDataTypeFormat::showAllMembers   ||
            currentNumberTrailingRows == MatrixDataTypeFormat::showAllMembers  ||
            currentNumberLeadingRows + currentNumberTrailingRows >= numberRows    ) {
            currentNumberLeadingRows  = numberRows;
            currentNumberTrailingRows = 0;
            currentFirstTrailingRow   = numberRows + 1;
        } else {
            currentFirstTrailingRow = numberRows - currentNumberTrailingRows + 1;
        }

        if (currentNumberLeadingColumns == MatrixDataTypeFormat::showAllMembers         ||
            currentNumberTrailingColumns == MatrixDataTypeFormat::showAllMembers        ||
            currentNumberLeadingColumns + currentNumberTrailingColumns >= numberColumns    ) {
            currentNumberLeadingColumns  = numberColumns;
            currentNumberTrailingColumns = 0;
            currentFirstTrailingColumn   = numberColumns + 1;
        } else {
            currentFirstTrailingColumn = numberColumns - currentNumberTrailingColumns + 1;
        }
    }
}

/***********************************************************************************************************************
 * Ld::MatrixDataTypeDecoder:
 */

namespace Ld {
    MatrixDataTypeDecoder::MatrixDataTypeDecoder() {}


    MatrixDataTypeDecoder::~MatrixDataTypeDecoder() {}


    QString MatrixDataTypeDecoder::translateToString(MatrixDataTypeDecoder::Context& context) const {
        unsigned long numberRows            = context.numberRows();
        unsigned long numberColumns         = context.numberColumns();
        unsigned long numberLeadingRows     = context.numberLeadingRows();
        unsigned long numberTrailingRows    = context.numberTrailingRows();
        unsigned long firstTrailingRow      = context.firstTrailingRow();
        unsigned long numberLeadingColumns  = context.numberLeadingColumns();
        unsigned long numberTrailingColumns = context.numberTrailingColumns();
        unsigned long firstTrailingColumn   = context.firstTrailingColumn();

        QString horizontalElipses(QChar(0x2026));
        QString verticalElipses(QChar(0x22EE));

        unsigned long totalNumberColumns = numberLeadingColumns + numberTrailingColumns;
        if (numberTrailingColumns > 0 || numberLeadingColumns < numberColumns) {
            ++totalNumberColumns;
        }

        QList<unsigned> columnWidths;
        for (unsigned long i=0 ; i<totalNumberColumns ; ++i) {
            columnWidths.append(0);
        }

        for (unsigned long row=1 ; row<=numberLeadingRows ; ++row) {
            unsigned long columnIndex = 0;
            for (unsigned long column=1 ; column<=numberLeadingColumns ; ++column) {
                QString  s = context.toString(row, column);
                unsigned l = static_cast<unsigned>(s.length());

                if (columnWidths[columnIndex] < l) {
                    columnWidths[columnIndex] = l;
                }

                ++columnIndex;
            }

            if (numberTrailingColumns > 0 || numberLeadingColumns < numberColumns) {
                unsigned l = static_cast<unsigned>(horizontalElipses.length());
                if (columnWidths[columnIndex] < l) {
                    columnWidths[columnIndex] = l;
                }

                ++columnIndex;

                for (unsigned long column=firstTrailingColumn ; column<=numberColumns ; ++column) {
                    QString  s = context.toString(row, column);
                    unsigned l = static_cast<unsigned>(s.length());

                    if (columnWidths[columnIndex] < l) {
                        columnWidths[columnIndex] = l;
                    }

                    ++columnIndex;
                }
            }
        }

        if (numberTrailingRows > 0 || numberLeadingRows < numberRows) {
            for (unsigned long i=0 ; i<totalNumberColumns ; ++i) {
                unsigned l = static_cast<unsigned>(verticalElipses.length());
                if (columnWidths[i] < l) {
                    columnWidths[i] = l;
                }
            }

            for (unsigned long row=firstTrailingRow ; row<=numberRows ; ++row) {
                unsigned long columnIndex = 0;
                for (unsigned long column=1 ; column<=numberLeadingColumns ; ++column) {
                    QString  s = context.toString(row, column);
                    unsigned l = static_cast<unsigned>(s.length());

                    if (columnWidths[columnIndex] < l) {
                        columnWidths[columnIndex] = l;
                    }

                    ++columnIndex;
                }

                if (numberTrailingColumns > 0 || numberLeadingColumns < numberColumns) {
                    unsigned l = static_cast<unsigned>(horizontalElipses.length());
                    if (columnWidths[columnIndex] < l) {
                        columnWidths[columnIndex] = l;
                    }

                    ++columnIndex;

                    for (unsigned long column=firstTrailingColumn ; column<=numberColumns ; ++column) {
                        QString  s = context.toString(row, column);
                        unsigned l = static_cast<unsigned>(s.length());

                        if (columnWidths[columnIndex] < l) {
                            columnWidths[columnIndex] = l;
                        }

                        ++columnIndex;
                    }
                }
            }
        }

        QString result;

        for (unsigned long row=1 ; row<=numberLeadingRows ; ++row) {
            unsigned long columnIndex = 0;
            for (unsigned long column=1 ; column<=numberLeadingColumns ; ++column) {
                QString  s = context.toString(row, column);
                if (columnIndex == 0) {
                    result += tr("%1").arg(s, columnWidths[columnIndex]);
                } else {
                    result += tr(" %1").arg(s, columnWidths[columnIndex]);
                }

                ++columnIndex;
            }

            if (numberTrailingColumns > 0 || numberLeadingColumns < numberColumns) {
                if (columnIndex == 0) {
                    result += tr("%1").arg(horizontalElipses, columnWidths[columnIndex]);
                } else {
                    result += tr(" %1").arg(horizontalElipses, columnWidths[columnIndex]);
                }

                ++columnIndex;

                for (unsigned long column=firstTrailingColumn ; column<=numberColumns ; ++column) {
                    QString  s = context.toString(row, column);
                    if (columnIndex == 0) {
                        result += tr("%1").arg(s, columnWidths[columnIndex]);
                    } else {
                        result += tr(" %1").arg(s, columnWidths[columnIndex]);
                    }

                    ++columnIndex;
                }
            }

            result += tr("\n");
        }

        if (numberTrailingRows > 0 || numberLeadingRows < numberRows) {
            unsigned columnIndex = 0;
            for (unsigned long i=0 ; i<totalNumberColumns ; ++i) {
                if (i == 0) {
                    result += tr("%1").arg(verticalElipses, columnWidths[columnIndex]);
                } else {
                    result += tr(" %1").arg(verticalElipses, columnWidths[columnIndex]);
                }

                ++columnIndex;
            }

            result += tr("\n");

            for (unsigned long row=firstTrailingRow ; row<=numberRows ; ++row) {
                unsigned long columnIndex = 0;
                for (unsigned long column=1 ; column<=numberLeadingColumns ; ++column) {
                    QString  s = context.toString(row, column);
                    if (columnIndex == 0) {
                        result += tr("%1").arg(s, columnWidths[columnIndex]);
                    } else {
                        result += tr(" %1").arg(s, columnWidths[columnIndex]);
                    }

                    ++columnIndex;
                }

                if (numberTrailingColumns > 0 || numberLeadingColumns < numberColumns) {
                    if (columnIndex == 0) {
                        result += tr("%1").arg(horizontalElipses, columnWidths[columnIndex]);
                    } else {
                        result += tr(" %1").arg(horizontalElipses, columnWidths[columnIndex]);
                    }

                    ++columnIndex;

                    for (unsigned long column=firstTrailingColumn ; column<=numberColumns ; ++column) {
                        QString  s = context.toString(row, column);
                        if (columnIndex == 0) {
                            result += tr("%1").arg(s, columnWidths[columnIndex]);
                        } else {
                            result += tr(" %1").arg(s, columnWidths[columnIndex]);
                        }

                        ++columnIndex;
                    }
                }

                result += tr("\n");
            }
        }

        return result;
    }
}
