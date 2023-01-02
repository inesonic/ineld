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
* This header defines the \ref Ld::LaTeXSetDataTypeTranslator class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_LATEX_SET_DATA_TYPE_TRANSLATOR_H
#define LD_LATEX_SET_DATA_TYPE_TRANSLATOR_H

#include <QString>

#include <model_variant.h>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_identifier_container.h"
#include "ld_data_type.h"
#include "ld_latex_data_type_translator.h"

namespace Ld {

    /**
     * Class that translates set values to LaTeX.
     */
    class LD_PUBLIC_API LaTeXSetDataTypeTranslator:public LaTeXDataTypeTranslator {
        public:
            /**
             * Static instance.
             */
            static const LaTeXSetDataTypeTranslator instance;

            LaTeXSetDataTypeTranslator();

            ~LaTeXSetDataTypeTranslator() override;

            /**
             * Method that returns the name of the element this translator operates on.
             *
             * \return Returns the name of the element this translator operates on.
             */
            QString elementName() const override;

            /**
             * Overloaded method that creates a LaTeX representation of the value.  Method assumes LaTeX is operating
             * in math mode.
             *
             * \param[in]     variant          The variant to be translated.
             *
             * \param[in]     element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.
             */
            bool toLaTeX(
                const Model::Variant&      variant,
                ElementPointer             element,
                LaTeXCodeGenerationEngine& generationEngine
            ) const override;

        private:
            /**
             * Method that inserts a specified number of entries at a provided set iterator position using LaTeX.
             *
             * \param[in,out] isFirst          A boolean value used to track whether we've inserted an entry.  The flag
             *                                 will be cleared if we insert one or more entries.
             *
             * \param[in,out] iterator         The iterator to be used to locate entries.  The iterator will be
             *                                 advanced by this operation.
             *
             * \param[in]     count            The number of entries we want to insert.
             *
             * \param[in]     element          The element driving this translation.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             */
            static void insertLaTeXFromSet(
                bool&                      isFirst,
                Model::Set::ConstIterator& iterator,
                unsigned long              count,
                ElementPointer             element,
                LaTeXCodeGenerationEngine& generationEngine
            );
    };
};

#endif
