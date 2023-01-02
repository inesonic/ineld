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
* This file implements various classes derived from \ref Container::Status.  Classes are used to report status
* conditions associated with program load/save operations.
***********************************************************************************************************************/

#include <string>
#include <cstdint>
#include <sstream>
#include <memory>
#include <cerrno>
#include <cstring>

#include <container_status.h>
#include "ld_status.h"

/***********************************************************************************************************************
 * Container::LoadSaveError::PimplBase
 */

namespace Ld {
    LoadSaveError::PimplBase::PimplBase() {}


    LoadSaveError::PimplBase::~PimplBase() {}


    Container::Status::Class LoadSaveError::PimplBase::statusClass() const {
        return Container::Status::Class::APPLICATION_ERROR;
    }
}

/***********************************************************************************************************************
 * Ld::LoadSaveError
 */

namespace Ld {
    LoadSaveError::~LoadSaveError() {}


    LoadSaveError::LoadSaveError(Container::Status::PimplBase* pimpl):Status(pimpl) {}


    LoadSaveError::LoadSaveError(const Status& other):Status(other) {}
}

/***********************************************************************************************************************
 * Ld::XmlError::PimplBase
 */

namespace Ld {
    XmlError::PimplBase::PimplBase(
            unsigned long long characterOffset,
            unsigned long long lineNumber,
            unsigned long long columnNumber
        ) {
        currentCharacterOffset = characterOffset;
        currentLineNumber      = lineNumber;
        currentColumnNumber    = columnNumber;
    }


    XmlError::PimplBase::~PimplBase() {}


    unsigned long long XmlError::PimplBase::characterOffset() const {
        return currentCharacterOffset;
    }


    unsigned long long XmlError::PimplBase::lineNumber() const {
        return currentLineNumber;
    }


    unsigned long long XmlError::PimplBase::columnNumber() const {
        return currentColumnNumber;
    }
}

/***********************************************************************************************************************
 * Ld::XmlError
 */

namespace Ld {
    XmlError::~XmlError() {}

    unsigned long long XmlError::characterOffset() const {
        return std::dynamic_pointer_cast<XmlError::PimplBase>(pimpl())->characterOffset();
    }


    unsigned long long XmlError::lineNumber() const {
        return std::dynamic_pointer_cast<XmlError::PimplBase>(pimpl())->lineNumber();
    }


    unsigned long long XmlError::columnNumber() const {
        return std::dynamic_pointer_cast<XmlError::PimplBase>(pimpl())->columnNumber();
    }


    XmlError::XmlError(XmlError::PimplBase* pimpl):LoadSaveError(pimpl) {}


    XmlError::XmlError(const Status& other):LoadSaveError(other) {}
}

/***********************************************************************************************************************
 * Ld::InvalidDocTypeDeclError::Pimpl
 */

namespace Ld {
    class InvalidDocTypeDeclError::Pimpl:public XmlError::PimplBase {
        public:
            Pimpl(
                unsigned long long characterOffset,
                unsigned long long lineNumber,
                unsigned long long columnNumber
            );

            ~Pimpl() override;

            int errorCode() const final;

            std::string description() const final;
    };


    InvalidDocTypeDeclError::Pimpl::Pimpl(
            unsigned long long characterOffset,
            unsigned long long lineNumber,
            unsigned long long columnNumber
        ):XmlError::PimplBase(
            characterOffset,
            lineNumber,
            columnNumber
        ) {}


    InvalidDocTypeDeclError::Pimpl::~Pimpl() {}


    int InvalidDocTypeDeclError::Pimpl::errorCode() const {
        return InvalidDocTypeDeclError::reportedErrorCode;
    }


    std::string InvalidDocTypeDeclError::Pimpl::description() const {
        std::stringstream stream;

        stream << "Invalid doctypedecl (" << characterOffset() << ", " << lineNumber() << ", " << columnNumber() << ")";

        return stream.str();
    }
}

/***********************************************************************************************************************
 * Ld::InvalidDocTypeDeclError
 */

namespace Ld {
    InvalidDocTypeDeclError::InvalidDocTypeDeclError(
            unsigned long long characterOffset,
            unsigned long long lineNumber,
            unsigned long long columnNumber
        ):XmlError(
            new InvalidDocTypeDeclError::Pimpl(
                characterOffset,
                lineNumber,
                columnNumber
            )
        ) {}


    InvalidDocTypeDeclError::InvalidDocTypeDeclError(
            const Status &other
        ):XmlError(
            other
        ) {}


    InvalidDocTypeDeclError::~InvalidDocTypeDeclError() {}
}
