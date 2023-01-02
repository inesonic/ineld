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
* This file is the main entry point for the ineld unit tests.
***********************************************************************************************************************/

#include <QApplication>
#include <QtTest/QtTest>

#include "test_plug_in_manager.h"
#include "test_license_data.h"
#include "test_handle.h"
#include "test_location.h"
#include "test_data_type.h"
#include "test_identifier.h"
#include "test_identifier_container.h"
#include "test_identifier_database.h"
#include "test_operation.h"
#include "test_operation_database.h"
#include "test_function_variant.h"
#include "test_function_data.h"
#include "test_function_database.h"
#include "test_program_file.h"
#include "test_xml_attributes.h"
#include "test_xml_memory_export_context.h"
#include "test_xml_file_export_context.h"
#include "test_xml_temporary_file_export_context.h"
#include "test_text_memory_export_context.h"
#include "test_text_file_export_context.h"
#include "test_format_container.h"
#include "test_format_organizer.h"
#include "test_aggregations_by_capability.h"
#include "test_character_format.h"
#include "test_page_format.h"
#include "test_block_format.h"
#include "test_justified_block_format.h"
#include "test_text_block_format.h"
#include "test_unordered_list_paragraph_format.h"
#include "test_ordered_list_paragraph_format.h"
#include "test_ordered_list_continuation_paragraph_format.h"
#include "test_root_import.h"
#include "test_element_with_no_children.h"
#include "test_element_with_fixed_children.h"
#include "test_element_with_positional_children.h"
#include "test_element_with_floating_children.h"
#include "test_element_with_grouped_children.h"
#include "test_element_with_grid_children.h"
#include "test_element_group.h"
#include "test_element_position.h"
#include "test_element_cursor.h"
#include "test_cursor_state_collection_entry.h"
#include "test_cursor_state_collection.h"
#include "test_root_element.h"
#include "test_text_element.h"
#include "test_payload_element.h"
#include "test_image_element.h"
#include "test_image_format.h"
#include "test_page_break_element.h"
#include "test_table_frame_element.h"
#include "test_table_frame_format.h"
#include "test_table_column_width.h"
#include "test_table_line_settings.h"
#include "test_literal_element.h"
#include "test_program_load_save.h"
#include "test_environment.h"
#include "test_cpp_code_generator_output_types.h"
#include "test_cpp_translation_phase.h"
#include "test_cpp_code_generator_diagnostic.h"
#include "test_cpp_code_generator.h"
#include "test_html_code_generator_output_types.h"
#include "test_html_translation_phase.h"
#include "test_html_code_generator_diagnostic.h"
#include "test_html_code_generator.h"
#include "test_latex_code_generator_output_types.h"
#include "test_latex_translation_phase.h"
#include "test_latex_code_generator_diagnostic.h"
#include "test_latex_code_generator.h"
#include "test_boolean_data_type_format.h"
#include "test_integer_data_type_format.h"
#include "test_complex_data_type_format.h"
#include "test_list_data_type_format.h"
#include "test_matrix_data_type_format.h"
#include "test_value_field_format.h"
#include "test_chart_line_style.h"
#include "test_chart_format.h"
#include "test_plot_format_base.h"
#include "test_chart_axis_format.h"

#define TEST(_X) {                                                  \
    _X _x;                                                          \
    testStatus |= QTest::qExec(&_x, argumentCount, argumentValues); \
}

int main(int argumentCount, char** argumentValues) {
    QApplication applicationInstance(argumentCount, argumentValues); // This is needed for QTemporaryFile to work.

    int testStatus = 0;

    TEST(TestEnvironment)
    TEST(TestPlugInManager)
    TEST(TestLicenseData)
    TEST(TestPlotFormatBase)
    TEST(TestChartAxisFormat)
    TEST(TestHandle)
    TEST(TestLocation)
    TEST(TestDataType)
    TEST(TestIdentifier)
    TEST(TestIdentifierContainer)
    TEST(TestIdentifierDatabase)
    TEST(TestOperation)
    TEST(TestOperationDatabase)
    TEST(TestFunctionVariant)
    TEST(TestFunctionData)
    TEST(TestFunctionDatabase)
    TEST(TestProgramFile)
    TEST(TestXmlAttributes)
    TEST(TestXmlMemoryExportContext)
    TEST(TestXmlFileExportContext)
    TEST(TestXmlTemporaryFileExportContext)
    TEST(TestTextMemoryExportContext)
    TEST(TestTextFileExportContext)
    TEST(TestFormatContainer)
    TEST(TestFormatOrganizer)
    TEST(TestAggregationsByCapability)
    TEST(TestCharacterFormat)
    TEST(TestPageFormat)
    TEST(TestBlockFormat)
    TEST(TestJustifiedBlockFormat)
    TEST(TestTextBlockFormat)
    TEST(TestUnorderedListParagraphFormat)
    TEST(TestOrderedListParagraphFormat)
    TEST(TestOrderedListContinuationParagraphFormat)
    TEST(TestRootImport)
    TEST(TestElementWithNoChildren)
    TEST(TestElementWithFixedChildren)
    TEST(TestElementWithPositionalChildren)
    TEST(TestElementWithFloatingChildren)
    TEST(TestElementWithGroupedChildren)
    TEST(TestElementWithGridChildren)
    TEST(TestElementGroup)
    TEST(TestElementPosition)
    TEST(TestElementCursor)
    TEST(TestCursorStateCollectionEntry)
    TEST(TestCursorStateCollection)
    TEST(TestRootElement)
    TEST(TestTextElement)
    TEST(TestPayloadElement)
    TEST(TestImageElement)
    TEST(TestImageFormat)
    TEST(TestPageBreakElement)
    TEST(TestTableFrameElement)
    TEST(TestTableFrameFormat)
    TEST(TestTableColumnWidth)
    TEST(TestTableLineSettings)
    TEST(TestLiteralElement)
    TEST(TestProgramLoadSave)
    TEST(TestCppTranslationPhase)
    TEST(TestCppCodeGeneratorDiagnostic)
    TEST(TestCppCodeGenerator)
    TEST(TestHtmlTranslationPhase)
    TEST(TestHtmlCodeGeneratorDiagnostic)
    TEST(TestHtmlCodeGenerator)
    TEST(TestLaTeXTranslationPhase)
    TEST(TestLaTeXCodeGeneratorDiagnostic)
    TEST(TestLaTeXCodeGenerator)
    TEST(TestBooleanDataTypeFormat)
    TEST(TestIntegerDataTypeFormat)
    TEST(TestComplexDataTypeFormat)
    TEST(TestListDataTypeFormat)
    TEST(TestMatrixDataTypeFormat)
    TEST(TestValueFieldFormat)
    TEST(TestChartLineStyle)
    TEST(TestChartFormat)

    // The tests below must be run in the order provided.
    TEST(TestCppCodeGeneratorOutputTypes)
    TEST(TestHtmlCodeGeneratorOutputTypes)
    TEST(TestLaTeXCodeGeneratorOutputTypes)

    return testStatus;
}
