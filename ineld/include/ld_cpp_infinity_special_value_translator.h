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
* This header defines the \ref Ld::CppInfinitySpecialValueTranslator class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CPP_INFINITY_SPECIAL_VALUE_TRANSLATOR_H
#define LD_CPP_INFINITY_SPECIAL_VALUE_TRANSLATOR_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QByteArray>
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_cpp_translator.h"

namespace Ld {
    class CppCodeGenerationEngine;

    /**
     * C++ translator for the \ref Ld::InfinitySpecialValueElement class.
     */
    class LD_PUBLIC_API CppInfinitySpecialValueTranslator:public CppTranslator {
        Q_DECLARE_TR_FUNCTIONS(Ld::CppInfinitySpecialValueTranslator)

        public:
            ~CppInfinitySpecialValueTranslator() override;

            /**
             * Method that returns the name of the element this translator operates on.
             *
             * \return Returns the name of the element this translator operates on.
             */
            QString elementName() const final;

            /**
             * Method that is called during translation to insert the actual implementation.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.  The default implementation always returns true.
             */
            bool threadImplementation(ElementPointer element, CppCodeGenerationEngine& generationEngine) override;

            /**
             * Method that is called during translation to insert any required method definitions.
             *
             * \param[in,out] element          A pointer to the element to be translated.
             *
             * \param[in,out] generationEngine The generation engine driving the conversion.
             *
             * \return Returns true on success, returns false on error.  The default implementation always returns true.
             */
            bool methodDefinitions(ElementPointer element, CppCodeGenerationEngine& generationEngine) override;
    };
};

#endif
