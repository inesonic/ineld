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
* This header defines the \ref Ld::CppDataTypeTranslator class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CPP_DATA_TYPE_TRANSLATOR_H
#define LD_CPP_DATA_TYPE_TRANSLATOR_H

#include <QString>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_cpp_code_generation_engine.h"
#include "ld_identifier_container.h"
#include "ld_data_type_translator.h"
#include "ld_cpp_translator.h"

namespace Ld {

    /**
     * Base class for C++ data type translators.  You can use this class both as a data-type translator an an element
     * translator.
     */
    class LD_PUBLIC_API CppDataTypeTranslator:public DataTypeTranslator, public CppTranslator {
        public:
            CppDataTypeTranslator();

            ~CppDataTypeTranslator() override;

            /**
             * Method that returns the internal value type for this data type.  Values should represent the names of
             * elements from the M::ValueType enumeration.  For example, the value returned for boolean types should be
             * "M::ValueType::BOOLEAN".
             *
             * \return Returns the text name tied to this data type.
             */
            virtual QString valueTypeName() const = 0;

            /**
             * Method that returns the internal data type name.
             *
             * \return Returns the internal data type name.
             */
            virtual QString dataTypeName() const = 0;

            /**
             * Method that indicates if this type is best passed by reference.
             *
             * \return Returns true if this type is best passed by reference.  Returns false if this type is best
             *         passed by value.
             */
            virtual bool preferPassByReference() const = 0;

            /**
             * Method that is called prior to actually performing any translation to identify any and all required
             * headers and required static or dynamic libraries.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.  The default implementation always returns true.
             */
            bool identifyDependenciesAndExplicitTypes(
                ElementPointer           element,
                CppCodeGenerationEngine& generationEngine
            ) override;

            /**
             * Method that is called during translation to insert the actual implementation.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.  The default implementation always returns
             *         true.
             */
            bool threadImplementation(ElementPointer element, CppCodeGenerationEngine& generationEngine) override;

            /**
             * Method that is called during translation to insert any required method definitions.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.  The default implementation always returns
             *         true.
             */
            bool methodDefinitions(ElementPointer element, CppCodeGenerationEngine& generationEngine) override;
    };
};

#endif
