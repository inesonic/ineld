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
* This file implements the \ref Ld::configure function.
***********************************************************************************************************************/

#include <QDebug> // Debug

#include <cstdint>
#include <string>

#if (defined(Q_OS_WIN))

    #include <Windows.h>

#endif

#include <ud_usage_data.h>

#include <model_api_types.h>

#include "ld_common.h"
#include "ld_handle.h"
#include "ld_plug_in_manager.h"
#include "ld_plug_in_registrar.h"
#include "ld_variable_name.h"
#include "ld_function_variant.h"
#include "ld_function_data.h"
#include "ld_function_database.h"

#include "ld_format.h"
#include "ld_font_format.h"
#include "ld_character_format.h"
#include "ld_block_format.h"
#include "ld_justified_block_format.h"
#include "ld_text_block_format.h"
#include "ld_paragraph_format.h"
#include "ld_unordered_list_paragraph_format.h"
#include "ld_ordered_list_paragraph_format.h"
#include "ld_list_additional_paragraph_format.h"
#include "ld_page_format.h"
#include "ld_image_format.h"
#include "ld_table_frame_format.h"
#include "ld_value_field_format.h"
#include "ld_operator_format.h"
#include "ld_parenthesis_format.h"
#include "ld_multiplication_operator_format.h"
#include "ld_division_operator_format.h"
#include "ld_logical_unary_not_operator_format.h"
#include "ld_brace_conditional_format.h"
#include "ld_function_format.h"
#include "ld_plot_format.h"

#include "ld_document_setting.h"
#include "ld_default_format_setting.h"

#include "ld_environment.h"
#include "ld_element.h"
#include "ld_root_element.h"
#include "ld_text_element.h"
#include "ld_paragraph_element.h"
#include "ld_image_element.h"
#include "ld_page_break_element.h"
#include "ld_table_frame_element.h"
#include "ld_value_field_element.h"
#include "ld_placeholder_element.h"
#include "ld_list_placeholder_element.h"
#include "ld_function_placeholder_element.h"
#include "ld_assignment_operator_element.h"
#include "ld_variable_element.h"
#include "ld_literal_element.h"
#include "ld_set_element.h"
#include "ld_tuple_element.h"
#include "ld_range_2_element.h"
#include "ld_range_3_element.h"
#include "ld_pi_special_value_element.h"
#include "ld_eulers_number_special_value_element.h"
#include "ld_epsilon_special_value_element.h"
#include "ld_infinity_special_value_element.h"
#include "ld_null_set_special_value_element.h"
#include "ld_element_of_set_operator_element.h"
#include "ld_boolean_type_element.h"
#include "ld_integer_type_element.h"
#include "ld_real_type_element.h"
#include "ld_complex_type_element.h"
#include "ld_addition_operator_element.h"
#include "ld_subtraction_operator_element.h"
#include "ld_multiplication_operator_element.h"
#include "ld_division_operator_element.h"
#include "ld_fraction_operator_element.h"
#include "ld_unary_plus_operator_element.h"
#include "ld_unary_minus_operator_element.h"
#include "ld_grouping_parenthesis_operator_element.h"
#include "ld_grouping_brackets_operator_element.h"
#include "ld_grouping_braces_operator_element.h"
#include "ld_power_operator_element.h"
#include "ld_square_root_operator_element.h"
#include "ld_root_operator_element.h"
#include "ld_complex_conjugate_operator_element.h"
#include "ld_factorial_operator_element.h"
#include "ld_absolute_value_operator_element.h"
#include "ld_floor_operator_element.h"
#include "ld_ceiling_operator_element.h"
#include "ld_nearest_integer_operator_element.h"
#include "ld_not_equal_to_operator_element.h"
#include "ld_less_than_operator_element.h"
#include "ld_greater_than_operator_element.h"
#include "ld_less_than_or_equal_to_operator_element.h"
#include "ld_greater_than_or_equal_to_operator_element.h"
#include "ld_union_operator_element.h"
#include "ld_intersection_operator_element.h"
#include "ld_disjoint_union_operator_element.h"
#include "ld_cartesian_product_operator_element.h"
#include "ld_set_difference_operator_element.h"
#include "ld_symmetric_difference_operator_element.h"
#include "ld_nary_union_operator_element.h"
#include "ld_nary_disjoint_union_operator_element.h"
#include "ld_not_element_of_set_operator_element.h"
#include "ld_set_contains_as_member_operator_element.h"
#include "ld_not_set_contains_as_member_operator_element.h"
#include "ld_subset_operator_element.h"
#include "ld_proper_subset_operator_element.h"
#include "ld_superset_operator_element.h"
#include "ld_proper_superset_operator_element.h"
#include "ld_not_subset_operator_element.h"
#include "ld_not_proper_subset_operator_element.h"
#include "ld_not_superset_operator_element.h"
#include "ld_not_proper_superset_operator_element.h"
#include "ld_logical_unary_not_operator_element.h"
#include "ld_logical_overbar_not_operator_element.h"
#include "ld_logical_and_operator_element.h"
#include "ld_logical_or_operator_element.h"
#include "ld_logical_exclusive_or_operator_element.h"
#include "ld_logical_conditional_operator_element.h"
#include "ld_logical_biconditional_operator_element.h"
#include "ld_logical_true_special_value_element.h"
#include "ld_logical_false_special_value_element.h"
#include "ld_subscript_index_operator_element.h"
#include "ld_subscript_row_column_operator_element.h"
#include "ld_matrix_operator_element.h"
#include "ld_matrix_combine_left_to_right_operator_element.h"
#include "ld_matrix_combine_top_to_bottom_operator_element.h"
#include "ld_hadamard_product_operator_element.h"
#include "ld_kronecker_product_operator_element.h"
#include "ld_matrix_transpose_operator_element.h"
#include "ld_matrix_conjugate_transpose_operator_element.h"
#include "ld_for_all_in_operator_element.h"
#include "ld_while_operator_element.h"
#include "ld_compound_statement_operator_element.h"
#include "ld_if_operator_element.h"
#include "ld_else_if_operator_element.h"
#include "ld_else_operator_element.h"
#include "ld_brace_conditional_operator_element.h"
#include "ld_therefore_operator_element.h"
#include "ld_function_element.h"
#include "ld_plot_element.h"

#include "ld_code_generator.h"
#include "ld_translator.h"
#include "ld_data_type.h"

#include "ld_cpp_code_generator.h"
#include "ld_cpp_variant_data_type_translator.h"
#include "ld_cpp_boolean_data_type_translator.h"
#include "ld_cpp_integer_data_type_translator.h"
#include "ld_cpp_real_data_type_translator.h"
#include "ld_cpp_complex_data_type_translator.h"
#include "ld_cpp_set_data_type_translator.h"
#include "ld_cpp_tuple_data_type_translator.h"
#include "ld_cpp_matrix_boolean_data_type_translator.h"
#include "ld_cpp_matrix_integer_data_type_translator.h"
#include "ld_cpp_matrix_real_data_type_translator.h"
#include "ld_cpp_matrix_complex_data_type_translator.h"
#include "ld_cpp_function_translation_engine.h"
#include "ld_cpp_ln_function_translation_engine.h"
#include "ld_cpp_log_function_translation_engine.h"
#include "ld_cpp_root_element_translator.h"
#include "ld_cpp_paragraph_element_translator.h"
#include "ld_cpp_table_frame_element_translator.h"
#include "ld_cpp_value_field_element_translator.h"
#include "ld_cpp_text_element_translator.h"
#include "ld_cpp_page_break_element_translator.h"
#include "ld_cpp_image_element_translator.h"
#include "ld_cpp_placeholder_translator.h"
#include "ld_cpp_assignment_operator_translator.h"
#include "ld_cpp_variable_translator.h"
#include "ld_cpp_literal_translator.h"
#include "ld_cpp_element_of_set_operator_translator.h"
#include "ld_cpp_set_translator.h"
#include "ld_cpp_tuple_translator.h"
#include "ld_cpp_list_placeholder_translator.h"
#include "ld_cpp_function_placeholder_translator.h"
#include "ld_cpp_range_2_translator.h"
#include "ld_cpp_range_3_translator.h"
#include "ld_cpp_pi_special_value_translator.h"
#include "ld_cpp_eulers_number_special_value_translator.h"
#include "ld_cpp_epsilon_special_value_translator.h"
#include "ld_cpp_infinity_special_value_translator.h"
#include "ld_cpp_null_set_special_value_translator.h"
#include "ld_cpp_addition_operator_translator.h"
#include "ld_cpp_subtraction_operator_translator.h"
#include "ld_cpp_multiplication_operator_translator.h"
#include "ld_cpp_division_operator_translator.h"
#include "ld_cpp_fraction_operator_translator.h"
#include "ld_cpp_unary_plus_operator_translator.h"
#include "ld_cpp_unary_minus_operator_translator.h"
#include "ld_cpp_grouping_parenthesis_operator_translator.h"
#include "ld_cpp_grouping_brackets_operator_translator.h"
#include "ld_cpp_grouping_braces_operator_translator.h"
#include "ld_cpp_power_operator_translator.h"
#include "ld_cpp_square_root_operator_translator.h"
#include "ld_cpp_root_operator_translator.h"
#include "ld_cpp_complex_conjugate_operator_translator.h"
#include "ld_cpp_factorial_operator_translator.h"
#include "ld_cpp_absolute_value_operator_translator.h"
#include "ld_cpp_floor_operator_translator.h"
#include "ld_cpp_ceiling_operator_translator.h"
#include "ld_cpp_nearest_integer_operator_translator.h"
#include "ld_cpp_not_equal_to_operator_translator.h"
#include "ld_cpp_less_than_operator_translator.h"
#include "ld_cpp_greater_than_operator_translator.h"
#include "ld_cpp_less_than_or_equal_to_operator_translator.h"
#include "ld_cpp_greater_than_or_equal_to_operator_translator.h"
#include "ld_cpp_union_operator_translator.h"
#include "ld_cpp_intersection_operator_translator.h"
#include "ld_cpp_disjoint_union_operator_translator.h"
#include "ld_cpp_cartesian_product_operator_translator.h"
#include "ld_cpp_set_difference_operator_translator.h"
#include "ld_cpp_symmetric_difference_operator_translator.h"
#include "ld_cpp_nary_union_operator_translator.h"
#include "ld_cpp_nary_disjoint_union_operator_translator.h"
#include "ld_cpp_not_element_of_set_operator_translator.h"
#include "ld_cpp_set_contains_as_member_operator_translator.h"
#include "ld_cpp_not_set_contains_as_member_operator_translator.h"
#include "ld_cpp_subset_operator_translator.h"
#include "ld_cpp_proper_subset_operator_translator.h"
#include "ld_cpp_superset_operator_translator.h"
#include "ld_cpp_proper_superset_operator_translator.h"
#include "ld_cpp_not_subset_operator_translator.h"
#include "ld_cpp_not_proper_subset_operator_translator.h"
#include "ld_cpp_not_superset_operator_translator.h"
#include "ld_cpp_not_proper_superset_operator_translator.h"
#include "ld_cpp_logical_unary_not_operator_translator.h"
#include "ld_cpp_logical_overbar_not_operator_translator.h"
#include "ld_cpp_logical_and_operator_translator.h"
#include "ld_cpp_logical_or_operator_translator.h"
#include "ld_cpp_logical_exclusive_or_operator_translator.h"
#include "ld_cpp_logical_conditional_operator_translator.h"
#include "ld_cpp_logical_biconditional_operator_translator.h"
#include "ld_cpp_logical_true_special_value_translator.h"
#include "ld_cpp_logical_false_special_value_translator.h"
#include "ld_cpp_subscript_index_operator_translator.h"
#include "ld_cpp_subscript_row_column_operator_translator.h"
#include "ld_cpp_matrix_operator_translator.h"
#include "ld_cpp_matrix_combine_left_to_right_operator_translator.h"
#include "ld_cpp_matrix_combine_top_to_bottom_operator_translator.h"
#include "ld_cpp_hadamard_product_operator_translator.h"
#include "ld_cpp_kronecker_product_operator_translator.h"
#include "ld_cpp_matrix_transpose_operator_translator.h"
#include "ld_cpp_matrix_conjugate_transpose_operator_translator.h"
#include "ld_cpp_for_all_in_operator_translator.h"
#include "ld_cpp_while_operator_translator.h"
#include "ld_cpp_compound_statement_operator_translator.h"
#include "ld_cpp_if_operator_translator.h"
#include "ld_cpp_else_if_operator_translator.h"
#include "ld_cpp_else_operator_translator.h"
#include "ld_cpp_brace_conditional_operator_translator.h"
#include "ld_cpp_therefore_operator_translator.h"
#include "ld_cpp_function_translator.h"
#include "ld_cpp_plot_translator.h"

#include "ld_html_code_generator.h"
#include "ld_html_boolean_data_type_translator.h"
#include "ld_html_integer_data_type_translator.h"
#include "ld_html_real_data_type_translator.h"
#include "ld_html_complex_data_type_translator.h"
#include "ld_html_set_data_type_translator.h"
#include "ld_html_tuple_data_type_translator.h"
#include "ld_html_matrix_boolean_data_type_translator.h"
#include "ld_html_matrix_integer_data_type_translator.h"
#include "ld_html_matrix_real_data_type_translator.h"
#include "ld_html_matrix_complex_data_type_translator.h"
#include "ld_html_function_translation_engine.h"
#include "ld_html_root_element_translator.h"
#include "ld_html_paragraph_element_translator.h"
#include "ld_html_text_element_translator.h"
#include "ld_html_image_element_translator.h"
#include "ld_html_page_break_element_translator.h"
#include "ld_html_table_frame_element_translator.h"
#include "ld_html_value_field_element_translator.h"
#include "ld_html_placeholder_element_translator.h"
#include "ld_html_assignment_operator_translator.h"
#include "ld_html_variable_translator.h"
#include "ld_html_literal_translator.h"
#include "ld_html_element_of_set_operator_translator.h"
#include "ld_html_boolean_type_translator.h"
#include "ld_html_integer_type_translator.h"
#include "ld_html_real_type_translator.h"
#include "ld_html_complex_type_translator.h"
#include "ld_html_set_translator.h"
#include "ld_html_tuple_translator.h"
#include "ld_html_list_placeholder_translator.h"
#include "ld_html_function_placeholder_element_translator.h"
#include "ld_html_range_2_translator.h"
#include "ld_html_range_3_translator.h"
#include "ld_html_pi_special_value_translator.h"
#include "ld_html_eulers_number_special_value_translator.h"
#include "ld_html_epsilon_special_value_translator.h"
#include "ld_html_infinity_special_value_translator.h"
#include "ld_html_null_set_special_value_translator.h"
#include "ld_html_addition_operator_translator.h"
#include "ld_html_subtraction_operator_translator.h"
#include "ld_html_multiplication_operator_translator.h"
#include "ld_html_division_operator_translator.h"
#include "ld_html_fraction_operator_translator.h"
#include "ld_html_unary_plus_operator_translator.h"
#include "ld_html_unary_minus_operator_translator.h"
#include "ld_html_grouping_parenthesis_operator_translator.h"
#include "ld_html_grouping_brackets_operator_translator.h"
#include "ld_html_grouping_braces_operator_translator.h"
#include "ld_html_power_operator_translator.h"
#include "ld_html_square_root_operator_translator.h"
#include "ld_html_root_operator_translator.h"
#include "ld_html_complex_conjugate_operator_translator.h"
#include "ld_html_factorial_operator_translator.h"
#include "ld_html_absolute_value_operator_translator.h"
#include "ld_html_floor_operator_translator.h"
#include "ld_html_ceiling_operator_translator.h"
#include "ld_html_nearest_integer_operator_translator.h"
#include "ld_html_not_equal_to_operator_translator.h"
#include "ld_html_less_than_operator_translator.h"
#include "ld_html_greater_than_operator_translator.h"
#include "ld_html_less_than_or_equal_to_operator_translator.h"
#include "ld_html_greater_than_or_equal_to_operator_translator.h"
#include "ld_html_union_operator_translator.h"
#include "ld_html_intersection_operator_translator.h"
#include "ld_html_disjoint_union_operator_translator.h"
#include "ld_html_cartesian_product_operator_translator.h"
#include "ld_html_set_difference_operator_translator.h"
#include "ld_html_symmetric_difference_operator_translator.h"
#include "ld_html_nary_union_operator_translator.h"
#include "ld_html_nary_disjoint_union_operator_translator.h"
#include "ld_html_not_element_of_set_operator_translator.h"
#include "ld_html_set_contains_as_member_operator_translator.h"
#include "ld_html_not_set_contains_as_member_operator_translator.h"
#include "ld_html_subset_operator_translator.h"
#include "ld_html_proper_subset_operator_translator.h"
#include "ld_html_superset_operator_translator.h"
#include "ld_html_proper_superset_operator_translator.h"
#include "ld_html_not_subset_operator_translator.h"
#include "ld_html_not_proper_subset_operator_translator.h"
#include "ld_html_not_superset_operator_translator.h"
#include "ld_html_not_proper_superset_operator_translator.h"
#include "ld_html_logical_unary_not_operator_translator.h"
#include "ld_html_logical_overbar_not_operator_translator.h"
#include "ld_html_logical_and_operator_translator.h"
#include "ld_html_logical_or_operator_translator.h"
#include "ld_html_logical_exclusive_or_operator_translator.h"
#include "ld_html_logical_conditional_operator_translator.h"
#include "ld_html_logical_biconditional_operator_translator.h"
#include "ld_html_logical_true_special_value_translator.h"
#include "ld_html_logical_false_special_value_translator.h"
#include "ld_html_subscript_index_operator_translator.h"
#include "ld_html_subscript_row_column_operator_translator.h"
#include "ld_html_matrix_operator_translator.h"
#include "ld_html_matrix_combine_left_to_right_operator_translator.h"
#include "ld_html_matrix_combine_top_to_bottom_operator_translator.h"
#include "ld_html_hadamard_product_operator_translator.h"
#include "ld_html_kronecker_product_operator_translator.h"
#include "ld_html_matrix_transpose_operator_translator.h"
#include "ld_html_matrix_conjugate_transpose_operator_translator.h"
#include "ld_html_for_all_in_operator_translator.h"
#include "ld_html_while_operator_translator.h"
#include "ld_html_compound_statement_operator_translator.h"
#include "ld_html_if_operator_translator.h"
#include "ld_html_else_if_operator_translator.h"
#include "ld_html_else_operator_translator.h"
#include "ld_html_brace_conditional_operator_translator.h"
#include "ld_html_therefore_operator_translator.h"
#include "ld_html_function_translator.h"
#include "ld_html_plot_translator.h"

#include "ld_latex_code_generator.h"
#include "ld_latex_boolean_data_type_translator.h"
#include "ld_latex_integer_data_type_translator.h"
#include "ld_latex_real_data_type_translator.h"
#include "ld_latex_complex_data_type_translator.h"
#include "ld_latex_set_data_type_translator.h"
#include "ld_latex_tuple_data_type_translator.h"
#include "ld_latex_matrix_boolean_data_type_translator.h"
#include "ld_latex_matrix_integer_data_type_translator.h"
#include "ld_latex_matrix_real_data_type_translator.h"
#include "ld_latex_matrix_complex_data_type_translator.h"
#include "ld_latex_function_translation_engine.h"
#include "ld_latex_root_element_translator.h"
#include "ld_latex_paragraph_element_translator.h"
#include "ld_latex_text_element_translator.h"
#include "ld_latex_image_element_translator.h"
#include "ld_latex_page_break_element_translator.h"
#include "ld_latex_table_frame_element_translator.h"
#include "ld_latex_value_field_element_translator.h"
#include "ld_latex_placeholder_element_translator.h"
#include "ld_latex_assignment_operator_translator.h"
#include "ld_latex_variable_translator.h"
#include "ld_latex_literal_translator.h"
#include "ld_latex_element_of_set_operator_translator.h"
#include "ld_latex_boolean_type_translator.h"
#include "ld_latex_integer_type_translator.h"
#include "ld_latex_real_type_translator.h"
#include "ld_latex_complex_type_translator.h"
#include "ld_latex_set_translator.h"
#include "ld_latex_tuple_translator.h"
#include "ld_latex_list_placeholder_translator.h"
#include "ld_latex_function_placeholder_element_translator.h"
#include "ld_latex_range_2_translator.h"
#include "ld_latex_range_3_translator.h"
#include "ld_latex_pi_special_value_translator.h"
#include "ld_latex_eulers_number_special_value_translator.h"
#include "ld_latex_epsilon_special_value_translator.h"
#include "ld_latex_null_set_special_value_translator.h"
#include "ld_latex_infinity_special_value_translator.h"
#include "ld_latex_addition_operator_translator.h"
#include "ld_latex_subtraction_operator_translator.h"
#include "ld_latex_multiplication_operator_translator.h"
#include "ld_latex_division_operator_translator.h"
#include "ld_latex_fraction_operator_translator.h"
#include "ld_latex_unary_plus_operator_translator.h"
#include "ld_latex_unary_minus_operator_translator.h"
#include "ld_latex_grouping_parenthesis_operator_translator.h"
#include "ld_latex_grouping_brackets_operator_translator.h"
#include "ld_latex_grouping_braces_operator_translator.h"
#include "ld_latex_power_operator_translator.h"
#include "ld_latex_square_root_operator_translator.h"
#include "ld_latex_root_operator_translator.h"
#include "ld_latex_complex_conjugate_operator_translator.h"
#include "ld_latex_factorial_operator_translator.h"
#include "ld_latex_absolute_value_operator_translator.h"
#include "ld_latex_floor_operator_translator.h"
#include "ld_latex_ceiling_operator_translator.h"
#include "ld_latex_nearest_integer_operator_translator.h"
#include "ld_latex_not_equal_to_operator_translator.h"
#include "ld_latex_less_than_operator_translator.h"
#include "ld_latex_greater_than_operator_translator.h"
#include "ld_latex_less_than_or_equal_to_operator_translator.h"
#include "ld_latex_greater_than_or_equal_to_operator_translator.h"
#include "ld_latex_union_operator_translator.h"
#include "ld_latex_intersection_operator_translator.h"
#include "ld_latex_disjoint_union_operator_translator.h"
#include "ld_latex_cartesian_product_operator_translator.h"
#include "ld_latex_set_difference_operator_translator.h"
#include "ld_latex_symmetric_difference_operator_translator.h"
#include "ld_latex_nary_union_operator_translator.h"
#include "ld_latex_nary_disjoint_union_operator_translator.h"
#include "ld_latex_not_element_of_set_operator_translator.h"
#include "ld_latex_set_contains_as_member_operator_translator.h"
#include "ld_latex_not_set_contains_as_member_operator_translator.h"
#include "ld_latex_subset_operator_translator.h"
#include "ld_latex_proper_subset_operator_translator.h"
#include "ld_latex_superset_operator_translator.h"
#include "ld_latex_proper_superset_operator_translator.h"
#include "ld_latex_not_subset_operator_translator.h"
#include "ld_latex_not_proper_subset_operator_translator.h"
#include "ld_latex_not_superset_operator_translator.h"
#include "ld_latex_not_proper_superset_operator_translator.h"
#include "ld_latex_logical_unary_not_operator_translator.h"
#include "ld_latex_logical_overbar_not_operator_translator.h"
#include "ld_latex_logical_and_operator_translator.h"
#include "ld_latex_logical_or_operator_translator.h"
#include "ld_latex_logical_exclusive_or_operator_translator.h"
#include "ld_latex_logical_conditional_operator_translator.h"
#include "ld_latex_logical_biconditional_operator_translator.h"
#include "ld_latex_logical_true_special_value_translator.h"
#include "ld_latex_logical_false_special_value_translator.h"
#include "ld_latex_subscript_index_operator_translator.h"
#include "ld_latex_subscript_row_column_operator_translator.h"
#include "ld_latex_matrix_operator_translator.h"
#include "ld_latex_matrix_combine_left_to_right_operator_translator.h"
#include "ld_latex_matrix_combine_top_to_bottom_operator_translator.h"
#include "ld_latex_hadamard_product_operator_translator.h"
#include "ld_latex_kronecker_product_operator_translator.h"
#include "ld_latex_matrix_transpose_operator_translator.h"
#include "ld_latex_matrix_conjugate_transpose_operator_translator.h"
#include "ld_latex_for_all_in_operator_translator.h"
#include "ld_latex_while_operator_translator.h"
#include "ld_latex_compound_statement_operator_translator.h"
#include "ld_latex_if_operator_translator.h"
#include "ld_latex_else_if_operator_translator.h"
#include "ld_latex_else_operator_translator.h"
#include "ld_latex_brace_conditional_operator_translator.h"
#include "ld_latex_therefore_operator_translator.h"
#include "ld_latex_function_translator.h"
#include "ld_latex_plot_translator.h"

#include "ld_configure_helpers.h"
#include "ld_configure_basic_functions.h"
#include "ld_configure_set_functions.h"
#include "ld_configure_special_functions.h"
#include "ld_configure_matrix_functions.h"
#include "ld_configure_statistical_functions.h"
#include "ld_configure_file_load_save_functions.h"
#include "ld_configure.h"

namespace Ld {
    #if (defined(Q_OS_WIN))

        static QList<DLL_DIRECTORY_COOKIE> currentlyLoadedDllSearchPaths;

    #endif

    void Configure::configure(std::uint64_t uniqueSystemId, Ud::UsageData* usageData) {
        Handle::initialize(uniqueSystemId);

        configureFormats();
        configureDocumentSettingTypes();
        configureElementCreators();
        configureDataTypes();
        configureCppCodeGenerator(usageData);
        configureHtmlCodeGenerator(usageData);
        configureLaTeXCodeGenerator(usageData);
        configureFunctions();
    }


    void Configure::setDebugOutputEnabled(bool enableDebugOutput) {
        QSharedPointer<CppCodeGenerator>
            cppCodeGenerator = Ld::CodeGenerator::codeGenerator(CppCodeGenerator::codeGeneratorName)
                               .dynamicCast<CppCodeGenerator>();

        cppCodeGenerator->setDebugOutputEnabled(enableDebugOutput);
    }


    void Configure::setDebugOutputDisabled(bool disableDebugOutput) {
        setDebugOutputEnabled(!disableDebugOutput);
    }


    bool Configure::debugOutputEnabled() {
        QSharedPointer<CppCodeGenerator>
            cppCodeGenerator = Ld::CodeGenerator::codeGenerator(CppCodeGenerator::codeGeneratorName)
                               .dynamicCast<CppCodeGenerator>();

        return cppCodeGenerator->debugOutputEnabled();
    }


    bool Configure::debugOutputDisabled() {
        return !debugOutputEnabled();
    }


    void Configure::configureLinker(const QString& linkerPath, const QString& linkerExecutable) {
        QSharedPointer<CppCodeGenerator>
            cppCodeGenerator = Ld::CodeGenerator::codeGenerator(CppCodeGenerator::codeGeneratorName)
                               .dynamicCast<CppCodeGenerator>();

        cppCodeGenerator->setExecutableDirectory(linkerPath);
        cppCodeGenerator->setLinkerExecutable(linkerExecutable);
    }


    void Configure::configureFinal() {
        FunctionDatabase::buildSearchDatabase();

        QList<QString> standardPchFiles = Ld::Environment::standardPchFiles();
        QList<QString> pchSearchPaths   = Ld::Environment::pchSearchPaths();
        QList<QString> pchFiles         = generatePchFileList(pchSearchPaths, standardPchFiles);

        QList<QString> runTimeLibrarySearchPaths = Ld::Environment::runTimeLibrarySearchPaths();
        runTimeLibrarySearchPaths += Ld::Environment::plugInRunTimeLibrarySearchPaths();

        QList<QString> librarySearchPaths = Ld::Environment::librarySearchPaths();
        librarySearchPaths += Ld::Environment::plugInLibrarySearchPaths();

        QSharedPointer<CppCodeGenerator>
            cppCodeGenerator = Ld::CodeGenerator::codeGenerator(CppCodeGenerator::codeGeneratorName)
                               .dynamicCast<CppCodeGenerator>();

        cppCodeGenerator->setStandardPchFiles(pchFiles);

        cppCodeGenerator->setSystemRoot(Ld::Environment::compilerRoot());
        cppCodeGenerator->setResourceDirectory(Ld::Environment::compilerResourceDirectory());
        cppCodeGenerator->setTargetTriple(Ld::Environment::targetTriple());
        cppCodeGenerator->setStaticLibraries(Ld::Environment::standardStaticLibraries());
        cppCodeGenerator->setDynamicLibraries(Ld::Environment::standardDynamicLibraries());
        cppCodeGenerator->setGccToolchain(Ld::Environment::gccPrefix());
        cppCodeGenerator->setRunTimeSearchPaths(runTimeLibrarySearchPaths);
        cppCodeGenerator->setLibrarySearchPaths(librarySearchPaths);

        #if (defined(Q_OS_WIN))

            SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);

            for (  QList<DLL_DIRECTORY_COOKIE>::const_iterator
                       runTimeLibraryCookieIterator = currentlyLoadedDllSearchPaths.constBegin(),
                       runTimeLibraryCookieEndIterator = currentlyLoadedDllSearchPaths.constEnd()
                 ; runTimeLibraryCookieIterator != runTimeLibraryCookieEndIterator
                 ; ++runTimeLibraryCookieIterator
                ) {
                bool success = RemoveDllDirectory(*runTimeLibraryCookieIterator);
                (void) success;
                assert(success);
            }

            for (  QList<QString>::const_iterator
                       runTimeLibrarySearchPathIterator    = runTimeLibrarySearchPaths.constBegin(),
                       runTimeLibrarySearchPathEndIterator = runTimeLibrarySearchPaths.constEnd()
                 ; runTimeLibrarySearchPathIterator != runTimeLibrarySearchPathEndIterator
                 ; ++runTimeLibrarySearchPathIterator
                ) {
                const QString& runTimeLibrarySearchPath = *runTimeLibrarySearchPathIterator;
                std::wstring searchPath = runTimeLibrarySearchPath.toStdWString();

                DLL_DIRECTORY_COOKIE directoryCookie = AddDllDirectory(searchPath.data());
                assert(directoryCookie != nullptr);

                currentlyLoadedDllSearchPaths.push_back(directoryCookie);
            }

        #endif

        // Uncomment the line below to extract a list of functions to a file.
        // FunctionDatabase::dumpFunctionList("/Users/psmith/functions.csv");
    }


    void Configure::configureFormats() {
        Format::registerCreator(FontFormat::formatName, FontFormat::creator);
        Format::registerCreator(CharacterFormat::formatName, CharacterFormat::creator);
        Format::registerCreator(BlockFormat::formatName, BlockFormat::creator);
        Format::registerCreator(JustifiedBlockFormat::formatName, JustifiedBlockFormat::creator);
        Format::registerCreator(TextBlockFormat::formatName, TextBlockFormat::creator);
        Format::registerCreator(ParagraphFormat::formatName, ParagraphFormat::creator);
        Format::registerCreator(UnorderedListParagraphFormat::formatName, UnorderedListParagraphFormat::creator);
        Format::registerCreator(OrderedListParagraphFormat::formatName, OrderedListParagraphFormat::creator);
        Format::registerCreator(ListAdditionalParagraphFormat::formatName, ListAdditionalParagraphFormat::creator);
        Format::registerCreator(PageFormat::formatName, PageFormat::creator);
        Format::registerCreator(ImageFormat::formatName, ImageFormat::creator);
        Format::registerCreator(TableFrameFormat::formatName, TableFrameFormat::creator);
        Format::registerCreator(ValueFieldFormat::formatName, ValueFieldFormat::creator);
        Format::registerCreator(ParenthesisFormat::formatName, ParenthesisFormat::creator);
        Format::registerCreator(OperatorFormat::formatName, OperatorFormat::creator);
        Format::registerCreator(MultiplicationOperatorFormat::formatName, MultiplicationOperatorFormat::creator);
        Format::registerCreator(DivisionOperatorFormat::formatName, DivisionOperatorFormat::creator);
        Format::registerCreator(LogicalUnaryNotOperatorFormat::formatName, LogicalUnaryNotOperatorFormat::creator);
        Format::registerCreator(BraceConditionalFormat::formatName, BraceConditionalFormat::creator);
        Format::registerCreator(FunctionFormat::formatName, FunctionFormat::creator);
        Format::registerCreator(PlotFormat::formatName, PlotFormat::creator);

        /* Format aggregators */

        Format::Aggregation::registerCreator(FontFormat::formatName, FontFormat::Aggregation::creator);
        Format::Aggregation::registerCreator(CharacterFormat::formatName, CharacterFormat::Aggregation::creator);
        Format::Aggregation::registerCreator(BlockFormat::formatName, BlockFormat::Aggregation::creator);
        Format::Aggregation::registerCreator(
            JustifiedBlockFormat::formatName,
            JustifiedBlockFormat::Aggregation::creator
        );
        Format::Aggregation::registerCreator(TextBlockFormat::formatName, TextBlockFormat::Aggregation::creator);
        Format::Aggregation::registerCreator(ParagraphFormat::formatName, ParagraphFormat::Aggregation::creator);
        Format::Aggregation::registerCreator(
            ListParagraphFormatBase::formatName,
            ListParagraphFormatBase::Aggregation::creator
        );
        Format::Aggregation::registerCreator(
            ListAdditionalParagraphFormat::formatName,
            ListAdditionalParagraphFormat::Aggregation::creator
        );
        Format::Aggregation::registerCreator(
            OrderedListParagraphFormat::formatName,
            OrderedListParagraphFormat::Aggregation::creator
        );
        Format::Aggregation::registerCreator(
            UnorderedListParagraphFormat::formatName,
            UnorderedListParagraphFormat::Aggregation::creator
        );
        Format::Aggregation::registerCreator(PageFormat::formatName, PageFormat::Aggregation::creator);
        Format::Aggregation::registerCreator(ImageFormat::formatName, ImageFormat::Aggregation::creator);
        Format::Aggregation::registerCreator(TableFrameFormat::formatName, TableFrameFormat::Aggregation::creator);
        Format::Aggregation::registerCreator(ValueFieldFormat::formatName, ValueFieldFormat::Aggregation::creator);
        Format::Aggregation::registerCreator(ParenthesisFormat::formatName, ParenthesisFormat::Aggregation::creator);
        Format::Aggregation::registerCreator(OperatorFormat::formatName, OperatorFormat::Aggregation::creator);
        Format::Aggregation::registerCreator(
            MultiplicationOperatorFormat::formatName,
            MultiplicationOperatorFormat::Aggregation::creator
        );
        Format::Aggregation::registerCreator(
            DivisionOperatorFormat::formatName,
            DivisionOperatorFormat::Aggregation::creator
        );
        Format::Aggregation::registerCreator(
            LogicalUnaryNotOperatorFormat::formatName,
            LogicalUnaryNotOperatorFormat::Aggregation::creator
        );
        Format::Aggregation::registerCreator(
            BraceConditionalFormat::formatName,
            BraceConditionalFormat::Aggregation::creator
        );
        Format::Aggregation::registerCreator(FunctionFormat::formatName, FunctionFormat::Aggregation::creator);
        Format::Aggregation::registerCreator(PlotFormat::formatName, PlotFormat::Aggregation::creator);
    }


    void Configure::configureDocumentSettingTypes() {
        DocumentSetting::registerCreator(
            DefaultFormatSetting::defaultTextFormatSettingName,
            DefaultFormatSetting::creator
        );

        DocumentSetting::registerCreator(
            DefaultFormatSetting::defaultMathFormatSettingName,
            DefaultFormatSetting::creator
        );

        DocumentSetting::registerCreator(
            DefaultFormatSetting::defaultMathIdentifierFormatSettingName,
            DefaultFormatSetting::creator
        );

        DocumentSetting::registerCreator(
            DefaultFormatSetting::defaultMathFunctionFormatSettingName,
            DefaultFormatSetting::creator
        );

        DocumentSetting::registerCreator(
            DefaultFormatSetting::defaultBraceConditionalFormatSettingName,
            DefaultFormatSetting::creator
        );
    }


    void Configure::configureElementCreators() {
        Element::registerCreator(RootElement::elementName, RootElement::creator);
//        Element::registerCreator(SelectionData::elementName, SelectionData::creator);
        Element::registerCreator(TextElement::elementName, TextElement::creator);
        Element::registerCreator(ParagraphElement::elementName, ParagraphElement::creator);
        Element::registerCreator(ImageElement::elementName, ImageElement::creator);
        Element::registerCreator(PageBreakElement::elementName, PageBreakElement::creator);
        Element::registerCreator(TableFrameElement::elementName, TableFrameElement::creator);
        Element::registerCreator(ValueFieldElement::elementName, ValueFieldElement::creator);
        Element::registerCreator(PlaceholderElement::elementName, PlaceholderElement::creator);
        Element::registerCreator(ListPlaceholderElement::elementName, ListPlaceholderElement::creator);
        Element::registerCreator(FunctionPlaceholderElement::elementName, FunctionPlaceholderElement::creator);
        Element::registerCreator(AssignmentOperatorElement::elementName, AssignmentOperatorElement::creator);
        Element::registerCreator(VariableElement::elementName, VariableElement::creator);
        Element::registerCreator(LiteralElement::elementName, LiteralElement::creator);
        Element::registerCreator(SetElement::elementName, SetElement::creator);
        Element::registerCreator(TupleElement::elementName, TupleElement::creator);
        Element::registerCreator(Range2Element::elementName, Range2Element::creator);
        Element::registerCreator(Range3Element::elementName, Range3Element::creator);
        Element::registerCreator(PiSpecialValueElement::elementName, PiSpecialValueElement::creator);
        Element::registerCreator(
            EulersNumberSpecialValueElement::elementName,
            EulersNumberSpecialValueElement::creator
        );
        Element::registerCreator(
            EpsilonSpecialValueElement::elementName,
            EpsilonSpecialValueElement::creator
        );
        Element::registerCreator(InfinitySpecialValueElement::elementName, InfinitySpecialValueElement::creator);
        Element::registerCreator(NullSetSpecialValueElement::elementName, NullSetSpecialValueElement::creator);
        Element::registerCreator(ElementOfSetOperatorElement::elementName, ElementOfSetOperatorElement::creator);
        Element::registerCreator(BooleanTypeElement::elementName, BooleanTypeElement::creator);
        Element::registerCreator(IntegerTypeElement::elementName, IntegerTypeElement::creator);
        Element::registerCreator(RealTypeElement::elementName, RealTypeElement::creator);
        Element::registerCreator(ComplexTypeElement::elementName, ComplexTypeElement::creator);
        Element::registerCreator(AdditionOperatorElement::elementName, AdditionOperatorElement::creator);
        Element::registerCreator(SubtractionOperatorElement::elementName, SubtractionOperatorElement::creator);
        Element::registerCreator(MultiplicationOperatorElement::elementName, MultiplicationOperatorElement::creator);
        Element::registerCreator(DivisionOperatorElement::elementName, DivisionOperatorElement::creator);
        Element::registerCreator(FractionOperatorElement::elementName, FractionOperatorElement::creator);
        Element::registerCreator(UnaryPlusOperatorElement::elementName, UnaryPlusOperatorElement::creator);
        Element::registerCreator(UnaryMinusOperatorElement::elementName, UnaryMinusOperatorElement::creator);
        Element::registerCreator(
            GroupingParenthesisOperatorElement::elementName,
            GroupingParenthesisOperatorElement::creator
        );
        Element::registerCreator(
            GroupingBracketsOperatorElement::elementName,
            GroupingBracketsOperatorElement::creator
        );
        Element::registerCreator(PowerOperatorElement::elementName, PowerOperatorElement::creator);
        Element::registerCreator(SquareRootOperatorElement::elementName, SquareRootOperatorElement::creator);
        Element::registerCreator(RootOperatorElement::elementName, RootOperatorElement::creator);
        Element::registerCreator(
            ComplexConjugateOperatorElement::elementName,
            ComplexConjugateOperatorElement::creator
        );
        Element::registerCreator(FactorialOperatorElement::elementName, FactorialOperatorElement::creator);
        Element::registerCreator(AbsoluteValueOperatorElement::elementName, AbsoluteValueOperatorElement::creator);
        Element::registerCreator(FloorOperatorElement::elementName, FloorOperatorElement::creator);
        Element::registerCreator(CeilingOperatorElement::elementName, CeilingOperatorElement::creator);
        Element::registerCreator(NearestIntegerOperatorElement::elementName, NearestIntegerOperatorElement::creator);
        Element::registerCreator(GroupingBracesOperatorElement::elementName, GroupingBracesOperatorElement::creator);
        Element::registerCreator(NotEqualToOperatorElement::elementName, NotEqualToOperatorElement::creator);
        Element::registerCreator(LessThanOperatorElement::elementName, LessThanOperatorElement::creator);
        Element::registerCreator(GreaterThanOperatorElement::elementName, GreaterThanOperatorElement::creator);
        Element::registerCreator(
            LessThanOrEqualToOperatorElement::elementName,
            LessThanOrEqualToOperatorElement::creator
        );
        Element::registerCreator(
            GreaterThanOrEqualToOperatorElement::elementName,
            GreaterThanOrEqualToOperatorElement::creator
        );
        Element::registerCreator(UnionOperatorElement::elementName, UnionOperatorElement::creator);
        Element::registerCreator(IntersectionOperatorElement::elementName, IntersectionOperatorElement::creator);
        Element::registerCreator(
            DisjointUnionOperatorElement::elementName,
            DisjointUnionOperatorElement::creator
        );
        Element::registerCreator(
            CartesianProductOperatorElement::elementName,
            CartesianProductOperatorElement::creator
        );
        Element::registerCreator(SetDifferenceOperatorElement::elementName, SetDifferenceOperatorElement::creator);
        Element::registerCreator(
            SymmetricDifferenceOperatorElement::elementName,
            SymmetricDifferenceOperatorElement::creator
        );
        Element::registerCreator(NaryUnionOperatorElement::elementName, NaryUnionOperatorElement::creator);
        Element::registerCreator(
            NaryDisjointUnionOperatorElement::elementName,
            NaryDisjointUnionOperatorElement::creator
        );
        Element::registerCreator(NotElementOfSetOperatorElement::elementName, NotElementOfSetOperatorElement::creator);
        Element::registerCreator(
            SetContainsAsMemberOperatorElement::elementName,
            SetContainsAsMemberOperatorElement::creator
        );
        Element::registerCreator(
            NotSetContainsAsMemberOperatorElement::elementName,
            NotSetContainsAsMemberOperatorElement::creator
        );
        Element::registerCreator(SubsetOperatorElement::elementName, SubsetOperatorElement::creator);
        Element::registerCreator(ProperSubsetOperatorElement::elementName, ProperSubsetOperatorElement::creator);
        Element::registerCreator(SupersetOperatorElement::elementName, SupersetOperatorElement::creator);
        Element::registerCreator(ProperSupersetOperatorElement::elementName, ProperSupersetOperatorElement::creator);
        Element::registerCreator(NotSubsetOperatorElement::elementName, NotSubsetOperatorElement::creator);
        Element::registerCreator(NotProperSubsetOperatorElement::elementName, NotProperSubsetOperatorElement::creator);
        Element::registerCreator(NotSupersetOperatorElement::elementName, NotSupersetOperatorElement::creator);
        Element::registerCreator(
            NotProperSupersetOperatorElement::elementName,
            NotProperSupersetOperatorElement::creator
        );
        Element::registerCreator(LogicalUnaryNotOperatorElement::elementName, LogicalUnaryNotOperatorElement::creator);
        Element::registerCreator(
            LogicalOverbarNotOperatorElement::elementName,
            LogicalOverbarNotOperatorElement::creator
        );
        Element::registerCreator(LogicalAndOperatorElement::elementName, LogicalAndOperatorElement::creator);
        Element::registerCreator(LogicalOrOperatorElement::elementName, LogicalOrOperatorElement::creator);
        Element::registerCreator(
            LogicalExclusiveOrOperatorElement::elementName,
            LogicalExclusiveOrOperatorElement::creator
        );
        Element::registerCreator(
            LogicalConditionalOperatorElement::elementName,
            LogicalConditionalOperatorElement::creator
        );
        Element::registerCreator(
            LogicalBiconditionalOperatorElement::elementName,
            LogicalBiconditionalOperatorElement::creator
        );
        Element::registerCreator(
            LogicalTrueSpecialValueElement::elementName,
            LogicalTrueSpecialValueElement::creator
        );
        Element::registerCreator(
            LogicalFalseSpecialValueElement::elementName,
            LogicalFalseSpecialValueElement::creator
        );
        Element::registerCreator(SubscriptIndexOperatorElement::elementName, SubscriptIndexOperatorElement::creator);
        Element::registerCreator(
            SubscriptRowColumnOperatorElement::elementName,
            SubscriptRowColumnOperatorElement::creator
        );
        Element::registerCreator(MatrixOperatorElement::elementName, MatrixOperatorElement::creator);
        Element::registerCreator(
            MatrixCombineLeftToRightOperatorElement::elementName,
            MatrixCombineLeftToRightOperatorElement::creator
        );
        Element::registerCreator(
            MatrixCombineTopToBottomOperatorElement::elementName,
            MatrixCombineTopToBottomOperatorElement::creator
        );
        Element::registerCreator(
            HadamardProductOperatorElement::elementName,
            HadamardProductOperatorElement::creator
        );
        Element::registerCreator(
            KroneckerProductOperatorElement::elementName,
            KroneckerProductOperatorElement::creator
        );
        Element::registerCreator(MatrixTransposeOperatorElement::elementName, MatrixTransposeOperatorElement::creator);
        Element::registerCreator(
            MatrixConjugateTransposeOperatorElement::elementName,
            MatrixConjugateTransposeOperatorElement::creator
        );
        Element::registerCreator(ForAllInOperatorElement::elementName, ForAllInOperatorElement::creator);
        Element::registerCreator(WhileOperatorElement::elementName, WhileOperatorElement::creator);
        Element::registerCreator(
            CompoundStatementOperatorElement::elementName,
            CompoundStatementOperatorElement::creator
        );
        Element::registerCreator(IfOperatorElement::elementName, IfOperatorElement::creator);
        Element::registerCreator(ElseIfOperatorElement::elementName, ElseIfOperatorElement::creator);
        Element::registerCreator(ElseOperatorElement::elementName, ElseOperatorElement::creator);
        Element::registerCreator(
            BraceConditionalOperatorElement::elementName,
            BraceConditionalOperatorElement::creator
        );
        Element::registerCreator(ThereforeOperatorElement::elementName, ThereforeOperatorElement::creator);
        Element::registerCreator(FunctionElement::elementName, FunctionElement::creator);
        Element::registerCreator(PlotElement::elementName, PlotElement::creator);
    }


    void Configure::configureDataTypes() {
        DataType::registerType(DataType::ValueType::VARIANT, "variant",              QString(), DataType::variant);
        DataType::registerType(DataType::ValueType::BOOLEAN, "boolean type",         BooleanTypeElement::elementName);
        DataType::registerType(DataType::ValueType::INTEGER, "64-bit integer type",  IntegerTypeElement::elementName);
        DataType::registerType(DataType::ValueType::REAL,    "64-bit real type",     RealTypeElement::elementName);
        DataType::registerType(DataType::ValueType::COMPLEX, "128-bit complex type", ComplexTypeElement::elementName);
        DataType::registerType(DataType::ValueType::SET,     "ordered set type",     QString(), DataType::container);
        DataType::registerType(DataType::ValueType::TUPLE,   "tuple type",           QString(), DataType::container);
        DataType::registerType(
            DataType::ValueType::MATRIX_BOOLEAN,
            "boolean matrix",
            BooleanTypeElement::elementName,
            DataType::matrix
        );
        DataType::registerType(
            DataType::ValueType::MATRIX_INTEGER,
            "integer matrix",
            IntegerTypeElement::elementName,
            DataType::matrix
        );
        DataType::registerType(
            DataType::ValueType::MATRIX_REAL,
            "real matrix",
            RealTypeElement::elementName,
            DataType::matrix
        );
        DataType::registerType(
            DataType::ValueType::MATRIX_COMPLEX,
            "complex matrix",
            ComplexTypeElement::elementName,
            DataType::matrix
        );

        DataType::setDefaultDataType(DataType::fromValueType(DataType::ValueType::VARIANT));
    }


    void Configure::configureCppCodeGenerator(Ud::UsageData* usageData) {
        DataType::registerTranslator(
            CppCodeGenerator::codeGeneratorName,
            DataType::ValueType::VARIANT,
            &CppVariantDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            CppCodeGenerator::codeGeneratorName,
            DataType::ValueType::BOOLEAN,
            &CppBooleanDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            CppCodeGenerator::codeGeneratorName,
            DataType::ValueType::INTEGER,
            &CppIntegerDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            CppCodeGenerator::codeGeneratorName,
            DataType::ValueType::REAL,
            &CppRealDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            CppCodeGenerator::codeGeneratorName,
            DataType::ValueType::COMPLEX,
            &CppComplexDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            CppCodeGenerator::codeGeneratorName,
            DataType::ValueType::SET,
            &CppSetDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            CppCodeGenerator::codeGeneratorName,
            DataType::ValueType::TUPLE,
            &CppTupleDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            CppCodeGenerator::codeGeneratorName,
            DataType::ValueType::MATRIX_BOOLEAN,
            &CppMatrixBooleanDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            CppCodeGenerator::codeGeneratorName,
            DataType::ValueType::MATRIX_INTEGER,
            &CppMatrixIntegerDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            CppCodeGenerator::codeGeneratorName,
            DataType::ValueType::MATRIX_REAL,
            &CppMatrixRealDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            CppCodeGenerator::codeGeneratorName,
            DataType::ValueType::MATRIX_COMPLEX,
            &CppMatrixComplexDataTypeTranslator::instance
        );

        CppCodeGenerator* cppCodeGenerator = new CppCodeGenerator;
        CodeGenerator::registerCodeGenerator(cppCodeGenerator);

        cppCodeGenerator->setUsageData(usageData);
        cppCodeGenerator->setExecutableDirectory(Ld::Environment::supportExecutableDirectory());

        cppCodeGenerator->setHeaderSearchPaths(Ld::Environment::headerSearchPaths());
        cppCodeGenerator->setStandardHeaders(Ld::Environment::standardHeaderFiles());

        cppCodeGenerator->registerTranslator(new CppRootElementTranslator);
        cppCodeGenerator->registerTranslator(new CppParagraphElementTranslator);
        cppCodeGenerator->registerTranslator(new CppTableFrameElementTranslator);
        cppCodeGenerator->registerTranslator(new CppValueFieldElementTranslator);
        cppCodeGenerator->registerTranslator(new CppTextElementTranslator);
        cppCodeGenerator->registerTranslator(new CppPageBreakElementTranslator);
        cppCodeGenerator->registerTranslator(new CppImageElementTranslator);
        cppCodeGenerator->registerTranslator(new CppPlaceholderTranslator);
        cppCodeGenerator->registerTranslator(new CppAssignmentOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppVariableTranslator);
        cppCodeGenerator->registerTranslator(new CppLiteralTranslator);
        cppCodeGenerator->registerTranslator(new CppElementOfSetOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppBooleanDataTypeTranslator);
        cppCodeGenerator->registerTranslator(new CppIntegerDataTypeTranslator);
        cppCodeGenerator->registerTranslator(new CppRealDataTypeTranslator);
        cppCodeGenerator->registerTranslator(new CppComplexDataTypeTranslator);
        cppCodeGenerator->registerTranslator(new CppSetTranslator);
        cppCodeGenerator->registerTranslator(new CppTupleTranslator);
        cppCodeGenerator->registerTranslator(new CppListPlaceholderTranslator);
        cppCodeGenerator->registerTranslator(new CppFunctionPlaceholderTranslator);
        cppCodeGenerator->registerTranslator(new CppRange2Translator);
        cppCodeGenerator->registerTranslator(new CppRange3Translator);
        cppCodeGenerator->registerTranslator(new CppPiSpecialValueTranslator);
        cppCodeGenerator->registerTranslator(new CppEulersNumberSpecialValueTranslator);
        cppCodeGenerator->registerTranslator(new CppEpsilonSpecialValueTranslator);
        cppCodeGenerator->registerTranslator(new CppInfinitySpecialValueTranslator);
        cppCodeGenerator->registerTranslator(new CppNullSetSpecialValueTranslator);
        cppCodeGenerator->registerTranslator(new CppAdditionOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppSubtractionOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppMultiplicationOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppDivisionOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppFractionOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppUnaryPlusOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppUnaryMinusOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppGroupingParenthesisOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppGroupingBracketsOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppGroupingBracesOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppPowerOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppSquareRootOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppRootOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppComplexConjugateOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppFactorialOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppAbsoluteValueOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppFloorOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppCeilingOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppNearestIntegerOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppNotEqualToOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppLessThanOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppGreaterThanOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppLessThanOrEqualToOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppGreaterThanOrEqualToOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppUnionOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppIntersectionOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppDisjointUnionOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppCartesianProductOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppSetDifferenceOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppSymmetricDifferenceOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppNaryUnionOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppNaryDisjointUnionOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppNotElementOfSetOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppSetContainsAsMemberOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppNotSetContainsAsMemberOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppSubsetOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppProperSubsetOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppSupersetOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppProperSupersetOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppNotSubsetOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppNotProperSubsetOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppNotSupersetOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppNotProperSupersetOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppLogicalUnaryNotOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppLogicalOverbarNotOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppLogicalAndOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppLogicalOrOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppLogicalExclusiveOrOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppLogicalConditionalOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppLogicalBiconditionalOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppLogicalTrueSpecialValueTranslator);
        cppCodeGenerator->registerTranslator(new CppLogicalFalseSpecialValueTranslator);
        cppCodeGenerator->registerTranslator(new CppSubscriptIndexOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppSubscriptRowColumnOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppMatrixOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppMatrixCombineLeftToRightOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppMatrixCombineTopToBottomOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppHadamardProductOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppKroneckerProductOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppMatrixTransposeOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppMatrixConjugateTransposeOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppForAllInOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppWhileOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppCompoundStatementOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppIfOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppElseIfOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppElseOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppBraceConditionalOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppThereforeOperatorTranslator);
        cppCodeGenerator->registerTranslator(new CppFunctionTranslator);
        cppCodeGenerator->registerTranslator(new CppPlotTranslator);
    }


    void Configure::configureHtmlCodeGenerator(Ud::UsageData* usageData) {
        DataType::registerTranslator(
            HtmlCodeGenerator::codeGeneratorName,
            DataType::ValueType::BOOLEAN,
            &HtmlBooleanDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            HtmlCodeGenerator::codeGeneratorName,
            DataType::ValueType::INTEGER,
            &HtmlIntegerDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            HtmlCodeGenerator::codeGeneratorName,
            DataType::ValueType::REAL,
            &HtmlRealDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            HtmlCodeGenerator::codeGeneratorName,
            DataType::ValueType::COMPLEX,
            &HtmlComplexDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            HtmlCodeGenerator::codeGeneratorName,
            DataType::ValueType::SET,
            &HtmlSetDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            HtmlCodeGenerator::codeGeneratorName,
            DataType::ValueType::TUPLE,
            &HtmlTupleDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            HtmlCodeGenerator::codeGeneratorName,
            DataType::ValueType::MATRIX_BOOLEAN,
            &HtmlMatrixBooleanDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            HtmlCodeGenerator::codeGeneratorName,
            DataType::ValueType::MATRIX_INTEGER,
            &HtmlMatrixIntegerDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            HtmlCodeGenerator::codeGeneratorName,
            DataType::ValueType::MATRIX_REAL,
            &HtmlMatrixRealDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            HtmlCodeGenerator::codeGeneratorName,
            DataType::ValueType::MATRIX_COMPLEX,
            &HtmlMatrixComplexDataTypeTranslator::instance
        );

        HtmlCodeGenerator* htmlCodeGenerator = new HtmlCodeGenerator;
        CodeGenerator::registerCodeGenerator(htmlCodeGenerator);

        htmlCodeGenerator->setUsageData(usageData);

        htmlCodeGenerator->registerTranslator(new HtmlRootElementTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlParagraphElementTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlTextElementTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlImageElementTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlPageBreakElementTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlTableFrameElementTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlValueFieldElementTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlPlaceholderElementTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlAssignmentOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlVariableTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlLiteralTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlSetTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlTupleTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlListPlaceholderTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlFunctionPlaceholderElementTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlRange2Translator);
        htmlCodeGenerator->registerTranslator(new HtmlRange3Translator);
        htmlCodeGenerator->registerTranslator(new HtmlPiSpecialValueTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlEulersNumberSpecialValueTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlEpsilonSpecialValueTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlInfinitySpecialValueTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlNullSetSpecialValueTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlElementOfSetOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlBooleanTypeTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlIntegerTypeTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlRealTypeTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlComplexTypeTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlAdditionOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlSubtractionOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlMultiplicationOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlDivisionOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlFractionOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlUnaryPlusOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlUnaryMinusOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlGroupingParenthesisOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlGroupingBracketsOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlGroupingBracesOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlPowerOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlSquareRootOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlRootOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlComplexConjugateOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlFactorialOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlAbsoluteValueOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlFloorOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlCeilingOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlNearestIntegerOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlNotEqualToOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlLessThanOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlGreaterThanOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlLessThanOrEqualToOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlGreaterThanOrEqualToOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlUnionOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlIntersectionOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlDisjointUnionOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlCartesianProductOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlSetDifferenceOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlSymmetricDifferenceOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlNaryUnionOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlNaryDisjointUnionOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlNotElementOfSetOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlSetContainsAsMemberOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlNotSetContainsAsMemberOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlSubsetOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlProperSubsetOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlSupersetOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlProperSupersetOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlNotSubsetOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlNotProperSubsetOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlNotSupersetOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlNotProperSupersetOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlLogicalUnaryNotOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlLogicalOverbarNotOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlLogicalAndOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlLogicalOrOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlLogicalExclusiveOrOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlLogicalConditionalOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlLogicalBiconditionalOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlLogicalTrueSpecialValueTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlLogicalFalseSpecialValueTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlSubscriptIndexOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlSubscriptRowColumnOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlMatrixOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlMatrixCombineLeftToRightOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlMatrixCombineTopToBottomOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlHadamardProductOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlKroneckerProductOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlMatrixTransposeOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlMatrixConjugateTransposeOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlForAllInOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlWhileOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlCompoundStatementOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlIfOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlElseIfOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlElseOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlBraceConditionalOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlThereforeOperatorTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlFunctionTranslator);
        htmlCodeGenerator->registerTranslator(new HtmlPlotTranslator);
    }


    void Configure::configureLaTeXCodeGenerator(Ud::UsageData* usageData) {
        DataType::registerTranslator(
            LaTeXCodeGenerator::codeGeneratorName,
            DataType::ValueType::BOOLEAN,
            &LaTeXBooleanDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            LaTeXCodeGenerator::codeGeneratorName,
            DataType::ValueType::INTEGER,
            &LaTeXIntegerDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            LaTeXCodeGenerator::codeGeneratorName,
            DataType::ValueType::REAL,
            &LaTeXRealDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            LaTeXCodeGenerator::codeGeneratorName,
            DataType::ValueType::COMPLEX,
            &LaTeXComplexDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            LaTeXCodeGenerator::codeGeneratorName,
            DataType::ValueType::SET,
            &LaTeXSetDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            LaTeXCodeGenerator::codeGeneratorName,
            DataType::ValueType::TUPLE,
            &LaTeXTupleDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            LaTeXCodeGenerator::codeGeneratorName,
            DataType::ValueType::MATRIX_BOOLEAN,
            &LaTeXMatrixBooleanDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            LaTeXCodeGenerator::codeGeneratorName,
            DataType::ValueType::MATRIX_INTEGER,
            &LaTeXMatrixIntegerDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            LaTeXCodeGenerator::codeGeneratorName,
            DataType::ValueType::MATRIX_REAL,
            &LaTeXMatrixRealDataTypeTranslator::instance
        );

        DataType::registerTranslator(
            LaTeXCodeGenerator::codeGeneratorName,
            DataType::ValueType::MATRIX_COMPLEX,
            &LaTeXMatrixComplexDataTypeTranslator::instance
        );

        LaTeXCodeGenerator* latexCodeGenerator = new LaTeXCodeGenerator;
        CodeGenerator::registerCodeGenerator(latexCodeGenerator);

        latexCodeGenerator->setUsageData(usageData);

        latexCodeGenerator->registerTranslator(new LaTeXRootElementTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXParagraphElementTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXTextElementTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXImageElementTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXPageBreakElementTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXTableFrameElementTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXValueFieldElementTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXPlaceholderElementTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXAssignmentOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXVariableTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXLiteralTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXSetTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXTupleTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXListPlaceholderTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXFunctionPlaceholderElementTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXRange2Translator);
        latexCodeGenerator->registerTranslator(new LaTeXRange3Translator);
        latexCodeGenerator->registerTranslator(new LaTeXPiSpecialValueTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXEulersNumberSpecialValueTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXEpsilonSpecialValueTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXInfinitySpecialValueTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXNullSetSpecialValueTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXElementOfSetOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXBooleanTypeTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXIntegerTypeTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXRealTypeTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXComplexTypeTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXAdditionOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXSubtractionOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXMultiplicationOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXDivisionOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXFractionOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXUnaryPlusOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXUnaryMinusOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXGroupingParenthesisOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXGroupingBracketsOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXGroupingBracesOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXPowerOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXSquareRootOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXRootOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXComplexConjugateOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXFactorialOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXAbsoluteValueOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXFloorOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXCeilingOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXNearestIntegerOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXNotEqualToOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXLessThanOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXGreaterThanOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXLessThanOrEqualToOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXGreaterThanOrEqualToOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXUnionOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXIntersectionOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXDisjointUnionOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXCartesianProductOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXSetDifferenceOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXSymmetricDifferenceOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXNaryUnionOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXNaryDisjointUnionOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXNotElementOfSetOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXSetContainsAsMemberOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXNotSetContainsAsMemberOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXSubsetOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXProperSubsetOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXSupersetOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXProperSupersetOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXNotSubsetOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXNotProperSubsetOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXNotSupersetOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXNotProperSupersetOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXLogicalUnaryNotOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXLogicalOverbarNotOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXLogicalAndOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXLogicalOrOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXLogicalExclusiveOrOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXLogicalConditionalOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXLogicalBiconditionalOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXLogicalTrueSpecialValueTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXLogicalFalseSpecialValueTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXSubscriptIndexOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXSubscriptRowColumnOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXMatrixOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXMatrixCombineLeftToRightOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXMatrixCombineTopToBottomOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXHadamardProductOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXKroneckerProductOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXMatrixTransposeOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXMatrixConjugateTransposeOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXForAllInOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXWhileOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXCompoundStatementOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXIfOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXElseIfOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXElseOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXBraceConditionalOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXThereforeOperatorTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXFunctionTranslator);
        latexCodeGenerator->registerTranslator(new LaTeXPlotTranslator);
    }


    void Configure::configureFunctions() {
        FunctionData::registerDefaultTranslator(
            CppCodeGenerator::codeGeneratorName,
            &CppFunctionTranslationEngine::instance
        );

        FunctionData::registerDefaultTranslator(
            HtmlCodeGenerator::codeGeneratorName,
            &HtmlFunctionTranslationEngine::instance
        );

        FunctionData::registerDefaultTranslator(
            LaTeXCodeGenerator::codeGeneratorName,
            &LaTeXFunctionTranslationEngine::instance
        );

        configureBasicFunctions();
        configureSetFunctions();
        configureSpecialFunctions();
        configureMatrixFunctions();
        configureStatisticalFunctions();
        configureFileLoadSaveFunctions();
    }


    QList<QString> Configure::generatePchFileList(
            const QList<QString>& pchSearchPaths,
            const QList<QString>& pchFiles
        ) {
        QMap<QString, QString> pathsByFile;

        for (  QList<QString>::const_iterator pathIterator    = pchSearchPaths.constBegin(),
                                              pathEndIterator = pchSearchPaths.constEnd()
             ; pathIterator != pathEndIterator
             ; ++pathIterator
            ) {
            const QString& path = *pathIterator;
            for (  QList<QString>::const_iterator pchIterator    = pchFiles.constBegin(),
                                                  pchEndIterator = pchFiles.constEnd()
                 ; pchIterator != pchEndIterator
                 ; ++pchIterator
                ) {
                const QString& pchFile = *pchIterator;
                if (QFileInfo(pchFile).isRelative()) {
                    QString fullPath = path + "/" + pchFile;
                    QFileInfo pchFileInfo(fullPath);

                    if (pchFileInfo.exists() && pchFileInfo.isFile()) {
                        pathsByFile.insert(pchFile, pchFileInfo.canonicalFilePath());
                    }
                }
            }
        }

        for (  QList<QString>::const_iterator pchIterator    = pchFiles.constBegin(),
                                              pchEndIterator = pchFiles.constEnd()
             ; pchIterator != pchEndIterator
             ; ++pchIterator
            ) {
            const QString& pchFile = *pchIterator;
            QFileInfo pchFileInfo(pchFile);
            if (pchFileInfo.isAbsolute() && pchFileInfo.exists() && pchFileInfo.isFile()) {
                pathsByFile.insert(pchFile, pchFileInfo.canonicalFilePath());
            }
        }

        return pathsByFile.values();
    }
}
