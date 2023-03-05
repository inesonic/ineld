##-*-makefile-*-########################################################################################################
# Copyright 2016 - 2023 Inesonic, LLC
#
# This file is licensed under two licenses.
#
# Inesonic Commercial License, Version 1:
#   All rights reserved.  Inesonic, LLC retains all rights to this software, including the right to relicense the
#   software in source or binary formats under different terms.  Unauthorized use under the terms of this license is
#   strictly prohibited.
#
# GNU Public License, Version 2:
#   This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
#   License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later
#   version.
#
#   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
#   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
#   details.
#
#   You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
#   Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
########################################################################################################################

########################################################################################################################
# Basic build characteristics
#

TEMPLATE = app
QT += core testlib widgets
CONFIG += testcase c++14

HEADERS = test_license_data.h \
          test_plug_in_manager.h \
          test_handle.h \
          test_location.h \
          test_data_type.h \
          test_identifier.h \
          test_identifier_container.h \
          test_identifier_database.h \
          test_operation.h \
          test_operation_database.h \
          test_function_variant.h \
          test_function_data.h \
          test_function_database.h \
          test_format_container.h \
          test_format_organizer.h \
          test_aggregations_by_capability.h \
          test_character_format.h \
          test_page_format.h \
          test_block_format.h \
          test_justified_block_format.h \
          test_text_block_format.h \
          test_unordered_list_paragraph_format.h \
          test_ordered_list_paragraph_format.h \
          test_ordered_list_continuation_paragraph_format.h \
          test_root_import.h \
          test_program_file.h \
          test_xml_attributes.h \
          test_xml_memory_export_context.h \
          test_xml_file_export_context.h \
          test_xml_temporary_file_export_context.h \
          test_text_memory_export_context.h \
          test_text_file_export_context.h \
          test_element_with_no_children.h \
          test_element_with_fixed_children.h \
          test_element_with_positional_children.h \
          test_element_with_floating_children.h \
          test_element_with_grouped_children.h \
          test_element_with_grid_children.h \
          test_element_group.h \
          test_element_position.h \
          test_element_cursor.h \
          test_cursor_state_collection_entry.h \
          test_cursor_state_collection.h \
          test_root_element.h \
          test_text_element.h \
          test_payload_element.h \
          test_image_element.h \
          test_image_format.h \
          test_page_break_element.h \
          test_table_frame_element.h \
          test_table_frame_format.h \
          test_table_column_width.h \
          test_table_line_settings.h \
          test_literal_element.h \
          test_program_load_save.h \
          test_environment.h \
          test_html_code_generator_output_types.h \
          test_html_translation_phase.h \
          test_html_code_generator_diagnostic.h \
          test_html_code_generator.h \
          test_latex_code_generator_output_types.h \
          test_latex_translation_phase.h \
          test_latex_code_generator_diagnostic.h \
          test_latex_code_generator.h \
          test_cpp_code_generator_output_types.h \
          test_cpp_translation_phase.h \
          test_cpp_code_generator_diagnostic.h \
          test_cpp_code_generator.h \
          test_boolean_data_type_format.h \
          test_integer_data_type_format.h \
          test_complex_data_type_format.h \
          test_list_data_type_format.h \
          test_matrix_data_type_format.h \
          test_value_field_format.h \
          test_chart_line_style.h \
          test_chart_format.h \
          test_plot_format_base.h \
          test_chart_axis_format.h \

SOURCES = test_ineld.cpp \
          test_license_data.cpp \
          test_plug_in_manager.cpp \
          test_handle.cpp \
          test_location.cpp \
          test_data_type.cpp \
          test_identifier.cpp \
          test_identifier_container.cpp \
          test_identifier_database.cpp \
          test_operation.cpp \
          test_operation_database.cpp \
          test_function_variant.cpp \
          test_function_data.cpp \
          test_function_database.cpp \
          test_format_container.cpp \
          test_format_organizer.cpp \
          test_aggregations_by_capability.cpp \
          test_character_format.cpp \
          test_page_format.cpp \
          test_block_format.cpp \
          test_justified_block_format.cpp \
          test_text_block_format.cpp \
          test_unordered_list_paragraph_format.cpp \
          test_ordered_list_paragraph_format.cpp \
          test_ordered_list_continuation_paragraph_format.cpp \
          test_root_import.cpp \
          test_program_file.cpp \
          test_xml_attributes.cpp \
          test_xml_memory_export_context.cpp \
          test_xml_file_export_context.cpp \
          test_xml_temporary_file_export_context.cpp \
          test_text_memory_export_context.cpp \
          test_text_file_export_context.cpp \
          test_element_with_no_children.cpp \
          test_element_with_fixed_children.cpp \
          test_element_with_positional_children.cpp \
          test_element_with_floating_children.cpp \
          test_element_with_grouped_children.cpp \
          test_element_with_grid_children.cpp \
          test_element_group.cpp \
          test_element_position.cpp \
          test_element_cursor.cpp \
          test_cursor_state_collection_entry.cpp \
          test_cursor_state_collection.cpp \
          test_root_element.cpp \
          test_text_element.cpp \
          test_payload_element.cpp \
          test_image_element.cpp \
          test_image_format.cpp \
          test_page_break_element.cpp \
          test_table_frame_element.cpp \
          test_table_frame_format.cpp \
          test_table_column_width.cpp \
          test_table_line_settings.cpp \
          test_literal_element.cpp \
          test_program_load_save.cpp \
          test_environment.cpp \
          test_html_code_generator_output_types.cpp \
          test_html_translation_phase.cpp \
          test_html_code_generator_diagnostic.cpp \
          test_html_code_generator.cpp \
          test_latex_code_generator_output_types.cpp \
          test_latex_translation_phase.cpp \
          test_latex_code_generator_diagnostic.cpp \
          test_latex_code_generator.cpp \
          test_cpp_code_generator_output_types.cpp \
          test_cpp_translation_phase.cpp \
          test_cpp_code_generator_diagnostic.cpp \
          test_cpp_code_generator.cpp \
          test_boolean_data_type_format.cpp \
          test_integer_data_type_format.cpp \
          test_complex_data_type_format.cpp \
          test_list_data_type_format.cpp \
          test_matrix_data_type_format.cpp \
          test_value_field_format.cpp \
          test_chart_line_style.cpp \
          test_chart_format.cpp \
          test_plot_format_base.cpp \
          test_chart_axis_format.cpp \

########################################################################################################################
# ineld library:
#

LD_BASE = $${OUT_PWD}/../../ineld/
INCLUDEPATH = $${PWD}/../../ineld/include/ $${PWD}/../../ineld/customer_include/

unix {
    CONFIG(debug, debug|release) {
        LIBS += -L$${LD_BASE}/build/debug/ -lineld

        macx {
            PRE_TARGETDEPS += $${LD_BASE}/build/debug/libineld.dylib
        } else {
            PRE_TARGETDEPS += $${LD_BASE}/build/debug/libineld.so
        }
    } else {
        LIBS += -L$${LD_BASE}/build/release/ -lineld

        macx {
            PRE_TARGETDEPS += $${LD_BASE}/build/release/libineld.dylib
        } else {
            PRE_TARGETDEPS += $${LD_BASE}/build/release/libineld.so
        }
    }
}

win32 {
    CONFIG(debug, debug|release) {
        LIBS += $${LD_BASE}/build/Debug/ineld.lib
        PRE_TARGETDEPS += $${LD_BASE}/build/Debug/ineld.lib
    } else {
        LIBS += $${LD_BASE}/build/Release/ineld.lib
        PRE_TARGETDEPS += $${LD_BASE}/build/Release/ineld.lib
    }
}

########################################################################################################################
# Libraries
#

defined(SETTINGS_PRI, var) {
    include($${SETTINGS_PRI})
}

INCLUDEPATH += $${INECONTAINER_INCLUDE}
INCLUDEPATH += $${INEQCONTAINER_INCLUDE}
INCLUDEPATH += $${INECBE_INCLUDE}
INCLUDEPATH += $${INEM_INCLUDE}
INCLUDEPATH += $${INEMAT_INCLUDE}
INCLUDEPATH += $${INEUTIL_INCLUDE}
INCLUDEPATH += $${INEUD_INCLUDE}
INCLUDEPATH += $${INEWH_INCLUDE}
INCLUDEPATH += $${INECRYPTO_INCLUDE}
INCLUDEPATH += $${BOOST_INCLUDE}

defined(INEMAT_PRI, var) {
    include($${INEMAT_PRI})
}

LIBS += -L$${INECONTAINER_LIBDIR} -linecontainer
LIBS += -L$${INEQCONTAINER_LIBDIR} -lineqcontainer
LIBS += -L$${INECBE_LIBDIR} -linecbe
LIBS += -L$${INEM_LIBDIR} -linem
LIBS += -L$${INEUTIL_LIBDIR} -lineutil
LIBS += -L$${INEUD_LIBDIR} -lineud
LIBS += -L$${INEWH_LIBDIR} -linewh
LIBS += -L$${INECRYPTO_LIBDIR} -linecrypto

defined(LLVM_PRI, var) {
    include($${LLVM_PRI})
}

defined(INEMAT_PRI, var) {
    include($${INEMAT_PRI})
}

########################################################################################################################
# Locate build intermediate and output products
#

TARGET = test_ineld

CONFIG(debug, debug|release) {
    unix:DESTDIR = build/debug
    win32:DESTDIR = build/Debug
} else {
    unix:DESTDIR = build/release
    win32:DESTDIR = build/Release
}

win32 {
    # The use of template libraries causes multiple copies of the same symbols (instantiated by templates) in libraries
    # Unlike real, properly architected operating systems, Windows thinks this is an error.  These linker options tells
    # Windows to perform the link even if this is the case and to suppress the warning messages generated.
    #
    # Unfortunately, Microsoft still insists on issuing a warning indicating the user of the /FORCE option which the
    # linker will not allow you to override.

    QMAKE_LFLAGS += /FORCE:MULTIPLE /IGNORE:4006
}

OBJECTS_DIR = $${DESTDIR}/objects
MOC_DIR = $${DESTDIR}/moc
RCC_DIR = $${DESTDIR}/rcc
UI_DIR = $${DESTDIR}/ui
