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
* This header defines various classes derived from \ref Container::Status.  Classes are used to report status
* conditions associated with program load/save operations.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_STATUS_H
#define LD_STATUS_H

#include <string>
#include <cstdint>

#include <container_status.h>

namespace Ld {
    /**
     * Base class for language load/save errors.
     */
    class LoadSaveError:public Container::Status {
        public:
            ~LoadSaveError();

        protected:
            /**
             * Private implementation.
             */
            class PimplBase:public Container::Status::ErrorPimplBase {
                public:
                    PimplBase();

                    ~PimplBase() override;

                    /**
                     * Method that returns the status code for this type of error.
                     *
                     * \return Returns the status class.
                     */
                    Class statusClass() const final;
            };

            /**
             * Constructor used to create derived error instances.
             *
             * \param[in] pimpl The pimpl class used to create functional error instances.
             */
            explicit LoadSaveError(Container::Status::PimplBase* pimpl);

            /**
             * Copy constructor.
             *
             * \param[in] other The instance to be copied.
             */
            LoadSaveError(const Status& other);
    };

    /**
     * Base class for errors reported during the processing of an XML description.
     */
    class XmlError:public LoadSaveError {
        public:
            ~XmlError();

            /**
             * Method you can use to obtain the reported character offset.
             *
             * \return Returns the zero based character offset where the error was reported.
             */
            unsigned long long characterOffset() const;

            /**
             * Method you can use to obtain the line number where the error was reported.
             *
             * \return Returns the one based line number where the error was reported.
             */
            unsigned long long lineNumber() const;

            /**
             * Method you can use to obtain the character offset in the line where the error was reported.
             *
             * \return Returns the zero based column number in the line where the error was reported.
             */
            unsigned long long columnNumber() const;

        protected:
            /**
             * Private implementation.
             */
            class PimplBase:public Ld::LoadSaveError::PimplBase {
                public:
                    /**
                     * Constructor
                     *
                     * \param[in] characterOffset The zero based character offset where the error was reported.
                     *
                     * \param[in] lineNumber      The one based line number where the error was reported.
                     *
                     * \param[in] columnNumber    The zero based column number where the error was reporeted.
                     */
                    PimplBase(
                        unsigned long long characterOffset,
                        unsigned long long lineNumber,
                        unsigned long long columnNumber
                    );

                    ~PimplBase() override;

                    /**
                     * Method you can use to obtain the reported character offset.
                     *
                     * \return Returns the zero based character offset where the error was reported.
                     */
                    unsigned long long characterOffset() const;

                    /**
                     * Method you can use to obtain the line number where the error was reported.
                     *
                     * \return Returns the one based line number where the error was reported.
                     */
                    unsigned long long lineNumber() const;

                    /**
                     * Method you can use to obtain the character offset in the line where the error was reported.
                     *
                     * \return Returns the zero based column number in the line where the error was reported.
                     */
                    unsigned long long columnNumber() const;

                private:
                    unsigned long long currentCharacterOffset;
                    unsigned long long currentLineNumber;
                    unsigned long long currentColumnNumber;
            };

            /**
             * Constructor used to create derived error instances.
             *
             * \param[in] pimpl The pimpl class used to create functional error instances.
             */
            explicit XmlError(PimplBase* pimpl);

            /**
             * Copy constructor.
             *
             * \param[in] other The instance to be copied.
             */
            XmlError(const Status& other);
    };

    /**
     * Class that reports an invalid XML doctypedecl
     */
    class InvalidDocTypeDeclError:public XmlError {
        public:
            /**
             * The error code used to report filename mismatch errors.
             */
            static constexpr int reportedErrorCode = 100;

            /**
             * Constructor
             *
             * \param[in] characterOffset The zero based character offset where the error was reported.
             *
             * \param[in] lineNumber      The one based line number where the error was reported.
             *
             * \param[in] columnNumber    The zero based column number where the error was reporeted.
             */
            InvalidDocTypeDeclError(
                unsigned long long characterOffset,
                unsigned long long lineNumber,
                unsigned long long columnNumber
            );

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            InvalidDocTypeDeclError(const Status& other);

            ~InvalidDocTypeDeclError();

        private:
            class Pimpl;
    };


}

#endif
