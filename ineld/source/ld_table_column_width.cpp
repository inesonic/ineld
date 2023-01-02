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
* This file implements the \ref Ld::TableColumnWidth class.
***********************************************************************************************************************/

#include <QHash>
#include <QList>
#include <QString>

#include <cmath>

#include <util_units.h>
#include <util_hash_functions.h>

#include "ld_table_column_width.h"

namespace Ld {
    const float TableColumnWidth::fixedColumnWidthEpsilon        = static_cast<float>(Util::Units::pointEpsilon);
    const float TableColumnWidth::proportionalColumnWidthEpsilon = 0.00001F;

    TableColumnWidth::TableColumnWidth(WidthType newWidthType, float newWidth) {
        currentWidthType = newWidthType;
        currentWidth     = newWidth;
    }


    TableColumnWidth::TableColumnWidth() {
        currentWidthType = WidthType::INVALID;
    }


    TableColumnWidth::TableColumnWidth(const TableColumnWidth& other) {
        currentWidthType = other.currentWidthType;
        currentWidth     = other.currentWidth;
    }


    TableColumnWidth::~TableColumnWidth() {}


    bool TableColumnWidth::isValid() const {
        return currentWidthType != WidthType::INVALID && currentWidth > 0.0;
    }


    bool TableColumnWidth::isInvalid() const {
        return !isValid();
    }


    void TableColumnWidth::setWidthType(TableColumnWidth::WidthType newWidthType) {
        currentWidthType = newWidthType;
    }


    TableColumnWidth::WidthType TableColumnWidth::widthType() const {
        return currentWidthType;
    }


    void TableColumnWidth::setWidth(float newWidth) {
        currentWidth = newWidth;
    }


    float TableColumnWidth::width() const {
        return currentWidth;
    }


    bool TableColumnWidth::setWidth(WidthType newWidthType, float newWidth) {
        bool success;

        if ((newWidthType == WidthType::FIXED        && newWidth > 0)                    ||
            (newWidthType == WidthType::PROPORTIONAL && newWidth > 0 && newWidth <= 1.0)    ) {
            success = true;

            currentWidthType = newWidthType;
            currentWidth     = newWidth;
        } else {
            success = false;
        }

        return success;
    }


    QString TableColumnWidth::toCss() const {
        QString result;
        switch (currentWidthType) {
            case TableColumnWidth::WidthType::INVALID: {
                result = "width: auto; ";
                break;
            }

            case TableColumnWidth::WidthType::FIXED: {
                result = QString("min-width: %1pt; max-width: %1pt; width: %1pt; ").arg(currentWidth);
                break;
            }

            case TableColumnWidth::WidthType::PROPORTIONAL: {
                result = QString("min-width: %1%; max-width: %1%; width: %1%; ")
                      .arg(static_cast<unsigned>(100.0 * currentWidth + 0.5));
                break;
            }

            default: {
                result = "width: auto; ";
                break;
            }
        }

        result += "border-style: 0px none; min-height: 1px; max-height: 1px; height: 1px; ";
        return result;
    }


    TableColumnWidth& TableColumnWidth::operator=(const TableColumnWidth& other) {
        currentWidthType = other.currentWidthType;
        currentWidth     = other.currentWidth;

        return *this;
    }


    bool TableColumnWidth::operator==(const TableColumnWidth& other) const {
        bool result;

        if (other.currentWidthType == currentWidthType) {
            if (currentWidthType == WidthType::PROPORTIONAL) {
                result = (fabs(currentWidth - other.currentWidth) <= proportionalColumnWidthEpsilon);
            } else if (currentWidthType == WidthType::FIXED) {
                result = (fabs(currentWidth - other.currentWidth) <= fixedColumnWidthEpsilon);
            } else {
                assert(currentWidthType == WidthType::INVALID);
                result = true;
            }
        } else {
            result = false;
        }

        return result;
    }


    bool TableColumnWidth::operator!=(const TableColumnWidth& other) const {
        return !operator==(other);
    }


    Util::HashResult qHash(const Ld::TableColumnWidth& key, Util::HashSeed seed) {
        double hashValue = (key.width() * 16) + static_cast<int>(key.widthType());
        return ::qHash(hashValue, seed);
    }
}


Util::HashResult qHash(const QList<Ld::TableColumnWidth>& key, Util::HashSeed seed) {
    Util::HashResult hash = 0;

    for (QList<Ld::TableColumnWidth>::const_iterator it=key.constBegin(),end=key.constEnd() ; it!=end ; ++it) {
        hash ^= qHash(*it, seed);
    }

    return hash;
}
