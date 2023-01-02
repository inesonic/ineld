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
* This file implements a single function that can be called configure file load/save functions.
***********************************************************************************************************************/

#include <QCoreApplication>

#include "ld_configure_helpers.h"
#include "ld_data_type.h"
#include "ld_variable_name.h"
#include "ld_function_data.h"
#include "ld_function_database.h"
#include "ld_configure_file_load_save_functions.h"

#define TR(_x) QCoreApplication::translate("ConfigureFileLoadSaveFunctions", (_x))

namespace Ld {
    void configureFileLoadSaveFunctions() {
        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::fileOpenRead"),
                QString(),
                VariableName(TR("FileOpenRead")),
                false,
                TR("fileopenread"),
                TR("Opens a file for reading."),
                TR("Low Level File I/O"),
                TR("fileopenread"),
                false,
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::TUPLE, TR("filename")),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::BOOLEAN, TR("binary")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::fileOpenWriteTruncate"),
                QString(),
                VariableName(TR("FileOpenWriteTruncate")),
                false,
                TR("fileopenwritetruncate"),
                TR("Opens a file for reading and writing, destroying existing content."),
                TR("Low Level File I/O"),
                TR("fileopenwritetruncate"),
                false,
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::TUPLE, TR("filename")),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::BOOLEAN, TR("binary")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::fileOpenWriteAppend"),
                QString(),
                VariableName(TR("FileOpenWrite")),
                false,
                TR("fileopenwrite"),
                TR("Opens a file for reading and writing, content will be preserved."),
                TR("Low Level File I/O"),
                TR("fileopenwrite"),
                false,
                FunctionVariant(DataType::ValueType::INTEGER, DataType::ValueType::TUPLE, TR("filename")),
                FunctionVariant(
                    DataType::ValueType::INTEGER,
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::BOOLEAN, TR("binary")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::fileExists"),
                QString(),
                VariableName(TR("FileExists")),
                false,
                TR("fileexists"),
                TR("Determines if a file exists."),
                TR("Low Level File I/O"),
                TR("fileexists"),
                false,
                FunctionVariant(DataType::ValueType::BOOLEAN, DataType::ValueType::TUPLE, TR("filename"))
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::fileClose"),
                QString(),
                VariableName(TR("FileClose")),
                false,
                TR("fileclose"),
                TR("Closes an open file"),
                TR("Low Level File I/O"),
                TR("fileclose"),
                false,
                FunctionVariant(DataType::ValueType::BOOLEAN, DataType::ValueType::INTEGER, TR("file number")),
                FunctionVariant(DataType::ValueType::BOOLEAN, DataType::ValueType::REAL, TR("file number")),
                FunctionVariant(DataType::ValueType::BOOLEAN, DataType::ValueType::COMPLEX, TR("file number"))
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::fileDelete"),
                QString(),
                VariableName(TR("FileDelete")),
                false,
                TR("filedelete"),
                TR("Deletes a file"),
                TR("Low Level File I/O"),
                TR("filedelete"),
                false,
                FunctionVariant(DataType::ValueType::BOOLEAN, DataType::ValueType::TUPLE, TR("filename")),
                FunctionVariant(DataType::ValueType::BOOLEAN, DataType::ValueType::INTEGER, TR("file number")),
                FunctionVariant(DataType::ValueType::BOOLEAN, DataType::ValueType::REAL, TR("file number")),
                FunctionVariant(DataType::ValueType::BOOLEAN, DataType::ValueType::COMPLEX, TR("file number"))
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::fileSeek"),
                QString(),
                VariableName(TR("FileSeek")),
                false,
                TR("fileseek"),
                TR("Seeks to a specific location in a file"),
                TR("Low Level File I/O"),
                TR("fileseek"),
                false,
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::INTEGER, TR("offset")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::INTEGER, TR("offset")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::INTEGER, TR("offset")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::REAL, TR("offset")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::REAL, TR("offset")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::REAL, TR("offset")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("offset")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("offset")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("offset")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::fileReadByte"),
                QString(),
                VariableName(TR("FileReadByte")),
                false,
                TR("filereadbyte"),
                TR("Reads a single byte from a file"),
                TR("Low Level File I/O"),
                TR("filereadbyte"),
                false,
                FunctionVariant(DataType::ValueType::TUPLE, DataType::ValueType::INTEGER, TR("file number")),
                FunctionVariant(DataType::ValueType::TUPLE, DataType::ValueType::REAL, TR("file number")),
                FunctionVariant(DataType::ValueType::TUPLE, DataType::ValueType::COMPLEX, TR("file number"))
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::fileReadString"),
                QString(),
                VariableName(TR("FileReadString")),
                false,
                TR("filereadstring"),
                TR("Reads a string of text from a file"),
                TR("Low Level File I/O"),
                TR("filereadstring"),
                false,
                FunctionVariant(DataType::ValueType::TUPLE, DataType::ValueType::INTEGER, TR("file number")),
                FunctionVariant(DataType::ValueType::TUPLE, DataType::ValueType::REAL, TR("file number")),
                FunctionVariant(DataType::ValueType::TUPLE, DataType::ValueType::COMPLEX, TR("file number")),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::INTEGER, TR("length")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::INTEGER, TR("length")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::INTEGER, TR("length")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::REAL, TR("length")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::REAL, TR("length")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::REAL, TR("length")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("length")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("length")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("length")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::INTEGER, TR("length"),
                    DataType::ValueType::BOOLEAN, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::INTEGER, TR("length"),
                    DataType::ValueType::BOOLEAN, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::INTEGER, TR("length"),
                    DataType::ValueType::BOOLEAN, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::REAL, TR("length"),
                    DataType::ValueType::BOOLEAN, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::REAL, TR("length"),
                    DataType::ValueType::BOOLEAN, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::REAL, TR("length"),
                    DataType::ValueType::BOOLEAN, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("length"),
                    DataType::ValueType::BOOLEAN, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("length"),
                    DataType::ValueType::BOOLEAN, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("length"),
                    DataType::ValueType::BOOLEAN, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::INTEGER, TR("length"),
                    DataType::ValueType::INTEGER, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::INTEGER, TR("length"),
                    DataType::ValueType::INTEGER, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::INTEGER, TR("length"),
                    DataType::ValueType::INTEGER, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::REAL, TR("length"),
                    DataType::ValueType::INTEGER, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::REAL, TR("length"),
                    DataType::ValueType::INTEGER, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::REAL, TR("length"),
                    DataType::ValueType::INTEGER, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("length"),
                    DataType::ValueType::INTEGER, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("length"),
                    DataType::ValueType::INTEGER, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("length"),
                    DataType::ValueType::INTEGER, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::INTEGER, TR("length"),
                    DataType::ValueType::REAL, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::INTEGER, TR("length"),
                    DataType::ValueType::REAL, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::INTEGER, TR("length"),
                    DataType::ValueType::REAL, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::REAL, TR("length"),
                    DataType::ValueType::REAL, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::REAL, TR("length"),
                    DataType::ValueType::REAL, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::REAL, TR("length"),
                    DataType::ValueType::REAL, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("length"),
                    DataType::ValueType::REAL, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("length"),
                    DataType::ValueType::REAL, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("length"),
                    DataType::ValueType::REAL, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::INTEGER, TR("length"),
                    DataType::ValueType::COMPLEX, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::INTEGER, TR("length"),
                    DataType::ValueType::COMPLEX, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::INTEGER, TR("length"),
                    DataType::ValueType::COMPLEX, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::REAL, TR("length"),
                    DataType::ValueType::COMPLEX, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::REAL, TR("length"),
                    DataType::ValueType::COMPLEX, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::REAL, TR("length"),
                    DataType::ValueType::COMPLEX, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("length"),
                    DataType::ValueType::COMPLEX, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("length"),
                    DataType::ValueType::COMPLEX, TR("utf-8")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("length"),
                    DataType::ValueType::COMPLEX, TR("utf-8")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::fileReadInteger"),
                QString(),
                VariableName(TR("FileReadInteger")),
                false,
                TR("filereadinteger"),
                TR("Reads an integer from a file"),
                TR("Low Level File I/O"),
                TR("filereadinteger"),
                false,
                FunctionVariant(DataType::ValueType::TUPLE, DataType::ValueType::INTEGER, TR("file number")),
                FunctionVariant(DataType::ValueType::TUPLE, DataType::ValueType::REAL, TR("file number")),
                FunctionVariant(DataType::ValueType::TUPLE, DataType::ValueType::COMPLEX, TR("file number")),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::INTEGER, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::INTEGER, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::INTEGER, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::REAL, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::REAL, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::REAL, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("size/endian")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::fileReadReal"),
                QString(),
                VariableName(TR("FileReadReal")),
                false,
                TR("filereadreal"),
                TR("Reads a real value from a file"),
                TR("Low Level File I/O"),
                TR("filereadreal"),
                false,
                FunctionVariant(DataType::ValueType::TUPLE, DataType::ValueType::INTEGER, TR("file number")),
                FunctionVariant(DataType::ValueType::TUPLE, DataType::ValueType::REAL, TR("file number")),
                FunctionVariant(DataType::ValueType::TUPLE, DataType::ValueType::COMPLEX, TR("file number")),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::INTEGER, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::INTEGER, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::INTEGER, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::REAL, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::REAL, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::REAL, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::TUPLE,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("size/endian")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::fileWriteByte"),
                QString(),
                VariableName(TR("FileWriteByte")),
                false,
                TR("filewritebyte"),
                TR("Writes a single byte to a file"),
                TR("Low Level File I/O"),
                TR("filewritebyte"),
                false,
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::INTEGER, TR("value value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::INTEGER, TR("byte value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::INTEGER, TR("byte value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::REAL, TR("byte value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::REAL, TR("byte value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::REAL, TR("byte value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("byte value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("byte value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("byte value")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::fileWriteString"),
                QString(),
                VariableName(TR("FileWriteString")),
                false,
                TR("filewritestring"),
                TR("Writes a string of text to a file"),
                TR("Low Level File I/O"),
                TR("filewritestring"),
                false,
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::TUPLE, TR("string")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::TUPLE, TR("string")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::TUPLE, TR("string")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::TUPLE, TR("string"),
                    DataType::ValueType::BOOLEAN, TR("include new-line")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::TUPLE, TR("string"),
                    DataType::ValueType::BOOLEAN, TR("include new-line")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::TUPLE, TR("string"),
                    DataType::ValueType::BOOLEAN, TR("include new-line")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::TUPLE, TR("string"),
                    DataType::ValueType::INTEGER, TR("include new-line")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::TUPLE, TR("string"),
                    DataType::ValueType::INTEGER, TR("include new-line")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::TUPLE, TR("string"),
                    DataType::ValueType::INTEGER, TR("include new-line")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::TUPLE, TR("string"),
                    DataType::ValueType::REAL, TR("include new-line")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::TUPLE, TR("string"),
                    DataType::ValueType::REAL, TR("include new-line")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::TUPLE, TR("string"),
                    DataType::ValueType::REAL, TR("include new-line")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::TUPLE, TR("string"),
                    DataType::ValueType::COMPLEX, TR("include new-line")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::TUPLE, TR("string"),
                    DataType::ValueType::COMPLEX, TR("include new-line")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::TUPLE, TR("string"),
                    DataType::ValueType::COMPLEX, TR("include new-line")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::fileWriteInteger"),
                QString(),
                VariableName(TR("FileWriteInteger")),
                false,
                TR("filewriteinteger"),
                TR("Writes an integer value to a file"),
                TR("Low Level File I/O"),
                TR("filewriteinteger"),
                false,
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::INTEGER, TR("integer value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::INTEGER, TR("integer value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::INTEGER, TR("integer value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::REAL, TR("integer value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::REAL, TR("integer value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::REAL, TR("integer value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("integer value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("integer value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("integer value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::INTEGER, TR("integer value"),
                    DataType::ValueType::INTEGER, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::INTEGER, TR("integer value"),
                    DataType::ValueType::INTEGER, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::INTEGER, TR("integer value"),
                    DataType::ValueType::INTEGER, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::REAL, TR("integer value"),
                    DataType::ValueType::INTEGER, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::REAL, TR("integer value"),
                    DataType::ValueType::INTEGER, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::REAL, TR("integer value"),
                    DataType::ValueType::INTEGER, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("integer value"),
                    DataType::ValueType::INTEGER, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("integer value"),
                    DataType::ValueType::INTEGER, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("integer value"),
                    DataType::ValueType::INTEGER, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::INTEGER, TR("integer value"),
                    DataType::ValueType::REAL, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::INTEGER, TR("integer value"),
                    DataType::ValueType::REAL, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::INTEGER, TR("integer value"),
                    DataType::ValueType::REAL, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::REAL, TR("integer value"),
                    DataType::ValueType::REAL, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::REAL, TR("integer value"),
                    DataType::ValueType::REAL, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::REAL, TR("integer value"),
                    DataType::ValueType::REAL, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("integer value"),
                    DataType::ValueType::REAL, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("integer value"),
                    DataType::ValueType::REAL, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("integer value"),
                    DataType::ValueType::REAL, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::INTEGER, TR("integer value"),
                    DataType::ValueType::COMPLEX, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::INTEGER, TR("integer value"),
                    DataType::ValueType::COMPLEX, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::INTEGER, TR("integer value"),
                    DataType::ValueType::COMPLEX, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::REAL, TR("integer value"),
                    DataType::ValueType::COMPLEX, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::REAL, TR("integer value"),
                    DataType::ValueType::COMPLEX, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::REAL, TR("integer value"),
                    DataType::ValueType::COMPLEX, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("integer value"),
                    DataType::ValueType::COMPLEX, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("integer value"),
                    DataType::ValueType::COMPLEX, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("integer value"),
                    DataType::ValueType::COMPLEX, TR("size/endian")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::fileWriteReal"),
                QString(),
                VariableName(TR("FileWriteReal")),
                false,
                TR("filewritereal"),
                TR("Writes a real value to a file"),
                TR("Low Level File I/O"),
                TR("filewritereal"),
                false,
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::INTEGER, TR("real value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::INTEGER, TR("real value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::INTEGER, TR("real value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::REAL, TR("real value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::REAL, TR("real value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::REAL, TR("real value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("real value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("real value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("real value")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::INTEGER, TR("real value"),
                    DataType::ValueType::INTEGER, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::INTEGER, TR("real value"),
                    DataType::ValueType::INTEGER, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::INTEGER, TR("real value"),
                    DataType::ValueType::INTEGER, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::REAL, TR("real value"),
                    DataType::ValueType::INTEGER, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::REAL, TR("real value"),
                    DataType::ValueType::INTEGER, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::REAL, TR("real value"),
                    DataType::ValueType::INTEGER, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("real value"),
                    DataType::ValueType::INTEGER, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("real value"),
                    DataType::ValueType::INTEGER, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("real value"),
                    DataType::ValueType::INTEGER, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::INTEGER, TR("real value"),
                    DataType::ValueType::REAL, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::INTEGER, TR("real value"),
                    DataType::ValueType::REAL, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::INTEGER, TR("real value"),
                    DataType::ValueType::REAL, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::REAL, TR("real value"),
                    DataType::ValueType::REAL, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::REAL, TR("real value"),
                    DataType::ValueType::REAL, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::REAL, TR("real value"),
                    DataType::ValueType::REAL, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("real value"),
                    DataType::ValueType::REAL, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("real value"),
                    DataType::ValueType::REAL, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("real value"),
                    DataType::ValueType::REAL, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::INTEGER, TR("real value"),
                    DataType::ValueType::COMPLEX, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::INTEGER, TR("real value"),
                    DataType::ValueType::COMPLEX, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::INTEGER, TR("real value"),
                    DataType::ValueType::COMPLEX, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::REAL, TR("real value"),
                    DataType::ValueType::COMPLEX, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::REAL, TR("real value"),
                    DataType::ValueType::COMPLEX, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::REAL, TR("real value"),
                    DataType::ValueType::COMPLEX, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::INTEGER, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("real value"),
                    DataType::ValueType::COMPLEX, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::REAL, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("real value"),
                    DataType::ValueType::COMPLEX, TR("size/endian")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::COMPLEX, TR("file number"),
                    DataType::ValueType::COMPLEX, TR("real value"),
                    DataType::ValueType::COMPLEX, TR("size/endian")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::loadBooleanMatrix"),
                QString(),
                VariableName(TR("LoadBooleanMatrix")),
                false,
                TR("loadbooleanmatrix"),
                TR("Loads a boolean matrix from a CSV or binary file"),
                TR("High Level File I/O"),
                TR("loadbooleanmatrix"),
                false,
                FunctionVariant(DataType::ValueType::MATRIX_BOOLEAN, DataType::ValueType::TUPLE, TR("filename"))
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::saveBooleanMatrix"),
                QString(),
                VariableName(TR("SaveBooleanMatrix")),
                false,
                TR("savebooleanmatrix"),
                TR("Saves a boolean matrix to a CSV or binary file"),
                TR("High Level File I/O"),
                TR("savebooleanmatrix"),
                false,
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::BOOLEAN, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::BOOLEAN, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::BOOLEAN, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::BOOLEAN, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::INTEGER, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::INTEGER, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::INTEGER, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::INTEGER, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::REAL, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::REAL, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::REAL, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::REAL, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::COMPLEX, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::COMPLEX, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::COMPLEX, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::COMPLEX, TR("binary")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::loadIntegerMatrix"),
                QString(),
                VariableName(TR("LoadIntegerMatrix")),
                false,
                TR("loadintegermatrix"),
                TR("Loads an integer matrix from a CSV or binary file"),
                TR("High Level File I/O"),
                TR("loadintegermatrix"),
                false,
                FunctionVariant(DataType::ValueType::MATRIX_INTEGER, DataType::ValueType::TUPLE, TR("filename"))
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::saveIntegerMatrix"),
                QString(),
                VariableName(TR("SaveIntegerMatrix")),
                false,
                TR("saveintegermatrix"),
                TR("Saves an integer matrix to a CSV or binary file"),
                TR("High Level File I/O"),
                TR("saveintegermatrix"),
                false,
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::BOOLEAN, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::BOOLEAN, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::INTEGER, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::INTEGER, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::REAL, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::REAL, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::COMPLEX, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::COMPLEX, TR("binary")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::loadRealMatrix"),
                QString(),
                VariableName(TR("LoadRealMatrix")),
                false,
                TR("loadrealmatrix"),
                TR("Loads a real matrix from a CSV or binary file"),
                TR("High Level File I/O"),
                TR("loadrealmatrix"),
                false,
                FunctionVariant(DataType::ValueType::MATRIX_REAL, DataType::ValueType::TUPLE, TR("filename"))
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::saveRealMatrix"),
                QString(),
                VariableName(TR("SaveRealMatrix")),
                false,
                TR("saverealmatrix"),
                TR("Saves a real matrix to a CSV or binary file"),
                TR("High Level File I/O"),
                TR("saverealmatrix"),
                false,
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::BOOLEAN, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::BOOLEAN, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::BOOLEAN, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::INTEGER, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::INTEGER, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::INTEGER, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::REAL, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::REAL, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::REAL, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::COMPLEX, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::COMPLEX, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::COMPLEX, TR("binary")
                )
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::loadComplexMatrix"),
                QString(),
                VariableName(TR("LoadComplexMatrix")),
                false,
                TR("loadcomplexmatrix"),
                TR("Loads a complex matrix from a CSV or binary file"),
                TR("High Level File I/O"),
                TR("loadcomplexmatrix"),
                false,
                FunctionVariant(DataType::ValueType::MATRIX_COMPLEX, DataType::ValueType::TUPLE, TR("filename"))
            )
        );

        FunctionDatabase::registerFunction(
            FunctionData(
                FunctionData::Type::BUILT_IN,
                QString("M::saveComplexMatrix"),
                QString(),
                VariableName(TR("SaveComplexMatrix")),
                false,
                TR("savecomplexmatrix"),
                TR("Saves a complex matrix to a CSV or binary file"),
                TR("High Level File I/O"),
                TR("savecomplexmatrix"),
                false,
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::BOOLEAN, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::BOOLEAN, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::BOOLEAN, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::BOOLEAN, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::INTEGER, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::INTEGER, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::INTEGER, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::INTEGER, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::REAL, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::REAL, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::REAL, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::REAL, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_BOOLEAN, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::COMPLEX, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_INTEGER, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::COMPLEX, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_REAL, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::COMPLEX, TR("binary")
                ),
                FunctionVariant(
                    DataType::ValueType::BOOLEAN,
                    DataType::ValueType::MATRIX_COMPLEX, TR("matrix"),
                    DataType::ValueType::TUPLE, TR("filename"),
                    DataType::ValueType::COMPLEX, TR("binary")
                )
            )
        );
    }
}
