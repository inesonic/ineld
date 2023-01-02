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
* This file implements the \ref Ld::SetDataTypeDecoder class.
***********************************************************************************************************************/

#include <QString>

#include <util_string.h>

#include <model_intrinsic_types.h>
#include <model_set.h>
#include <model_variant.h>

#include "ld_format_structures.h"
#include "ld_set_data_type_format.h"
#include "ld_value_field_format.h"
#include "ld_data_type_decoder.h"
#include "ld_set_data_type_decoder.h"

namespace Ld {
    SetDataTypeDecoder::SetDataTypeDecoder() {}


    SetDataTypeDecoder::~SetDataTypeDecoder() {}


    QString SetDataTypeDecoder::toString(const Model::Variant& value, Ld::FormatPointer format) const {
        QString result;

        bool       ok;
        Model::Set setValue = value.toSet(&ok);

        if (ok) {
            unsigned long                      leadingMemberCount;
            unsigned long                      trailingMemberCount;
            unsigned long                      hiddenMemberCount;
            bool                               showHiddenMemberCount;

            if (!format.isNull()                                                         &&
                (format->capabilities().contains(Ld::SetDataTypeFormat::formatName) ||
                 format->capabilities().contains(Ld::ValueFieldFormat::formatName)     )    ) {
                QSharedPointer<SetDataTypeFormat> setFormat = format.dynamicCast<SetDataTypeFormat>();

                leadingMemberCount    = setFormat->leadingMemberCount();
                trailingMemberCount   = setFormat->trailingMemberCount();
                showHiddenMemberCount = setFormat->displayHiddenMemberCount();
            } else {
                leadingMemberCount    = SetDataTypeFormat::defaultLeadingMemberCount;
                trailingMemberCount   = SetDataTypeFormat::defaultTrailingMemberCount;
                showHiddenMemberCount = SetDataTypeFormat::defaultShowHiddenMemberCount;
            }

            unsigned long numberSetMembers = setValue.size();
            if (leadingMemberCount == SetDataTypeFormat::showAllMembers      ||
                trailingMemberCount == SetDataTypeFormat::showAllMembers     ||
                leadingMemberCount + trailingMemberCount >= numberSetMembers    ) {
                leadingMemberCount  = numberSetMembers;
                hiddenMemberCount   = 0;
                trailingMemberCount = 0;
            } else {
                hiddenMemberCount = numberSetMembers - leadingMemberCount - trailingMemberCount;
            }

            result = tr("{ ");

            Model::Set::ConstIterator it      = setValue.constBegin();
            bool                      isFirst = true;

            insertFromSet(result, isFirst, it, leadingMemberCount, format);

            if (hiddenMemberCount > 0) {
                if (!isFirst) {
                    result += tr(", ");
                } else {
                    isFirst = false;
                }

                if (showHiddenMemberCount) {
                    result += tr("... %1 entries ...").arg(hiddenMemberCount);
                } else {
                    result = tr("...");
                }

                it += hiddenMemberCount;
            }

            insertFromSet(result, isFirst, it, trailingMemberCount, format);

            result += tr(" }");
        } else {
            result = tr("???");
        }

        return result;
    }


    QString SetDataTypeDecoder::toDescription(const Model::Variant& value) const {
        QString    result;

        bool       ok;
        Model::Set s = value.toSet(&ok);

        if (ok) {
            result = tr("Set (%1 members)").arg(s.size());
        } else {
            result = tr("Set (*** could not decode ***)");
        }

        return result;
    }


    void SetDataTypeDecoder::insertFromSet(
            QString&                   str,
            bool&                      isFirst,
            Model::Set::ConstIterator& iterator,
            unsigned long              count,
            FormatPointer              format
        ) {
        for (unsigned long i=0 ; i<count ; ++i) {
            const Model::Variant& contents          = iterator.constReference();
            Model::ValueType      contentsValueType = contents.valueType();
            DataType              contentsDataType  = DataType::fromValueType(contentsValueType);

            if (!isFirst) {
                str += tr(", ");
            } else {
                isFirst = false;
            }

            if (contentsDataType.isValid()) {
                const DataTypeDecoder* contentsDecoder = contentsDataType.decoder();
                if (contentsDecoder == nullptr) {
                    str += tr("*** MISSING DECODER %1 ***").arg(static_cast<unsigned>(contentsValueType));
                } else {
                    str += contentsDecoder->toString(contents, format);
                }
            } else {
                str += tr("*** UNKNOWN TYPE %1 ***").arg(static_cast<unsigned>(contentsValueType));
            }

            iterator.advance();
        }
    }
}
