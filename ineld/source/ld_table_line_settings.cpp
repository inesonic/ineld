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
* This file implements the \ref Ld::TableLineSettings class.
***********************************************************************************************************************/

#include <QString>
#include <QHash>

#include <cmath>
#include <cassert>

#include <util_hash_functions.h>

#include "ld_table_line_settings.h"

namespace Ld {
    const float TableLineSettings::lineWidthEpsilon = 0.1F;

    TableLineSettings::TableLineSettings(Style newLineStyle, float newWidth) {
        currentLineStyle = newLineStyle;
        currentWidth     = newWidth;
    }


    TableLineSettings::TableLineSettings() {
        currentLineStyle = Style::INVALID;
        currentWidth     = 0;
    }


    TableLineSettings::TableLineSettings(const TableLineSettings& other) {
        currentLineStyle = other.currentLineStyle;
        currentWidth     = other.currentWidth;
    }


    TableLineSettings::~TableLineSettings() {}


    bool TableLineSettings::isValid() const {
        return (currentLineStyle != Style::INVALID && currentWidth > 0);
    }


    bool TableLineSettings::isInvalid() const {
        return !isValid();
    }


    void TableLineSettings::setLineStyle(TableLineSettings::Style newLineStyle) {
        currentLineStyle = newLineStyle;
    }


    TableLineSettings::Style TableLineSettings::lineStyle() const {
        return currentLineStyle;
    }


    void TableLineSettings::setWidth(float newWidth) {
        currentWidth = newWidth;
    }


    float TableLineSettings::width() const {
        return currentWidth;
    }


    bool TableLineSettings::setLine(TableLineSettings::Style newLineStyle, float newWidth) {
        bool success;

        if (newLineStyle != Style::INVALID && newWidth > 0) {
            currentLineStyle = newLineStyle;
            currentWidth     = newWidth;

            success = true;
        } else {
            success = false;
        }

        return success;
    }


    QString TableLineSettings::toCss(TableLineSettings::Position position) const {
        QString propertyName;
        float  width = currentWidth;
        switch (position) {
            case Position::ALL_POSITIONS:   { propertyName = "border";        break; }
            case Position::LEFT_POSITION:   { propertyName = "border-left";   break; }
            case Position::RIGHT_POSITION:  { propertyName = "border-right";  break; }
            case Position::TOP_POSITION:    { propertyName = "border-top";    break; }
            case Position::BOTTOM_POSITION: { propertyName = "border-bottom"; break; }
            default: {
                assert(false);
                break;
            }
        }

        QString styleName;
        switch (currentLineStyle) {
            case Style::INVALID:
            case Style::NO_LINE: {
                styleName = "none";
                break;
            }

            case Style::SINGLE: {
                styleName = "solid";
                break;
            }

            case Style::DOUBLE: {
                styleName = "double";
                width *= 3;
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        return QString("%1: %2pt %3 black; ").arg(propertyName).arg(width).arg(styleName);
    }


    TableLineSettings& TableLineSettings::operator=(const TableLineSettings& other) {
        currentLineStyle = other.currentLineStyle;
        currentWidth     = other.currentWidth;

        return *this;
    }


    bool TableLineSettings::operator==(const TableLineSettings& other) const {
        bool result;

        if (other.currentLineStyle == currentLineStyle) {
            result = (fabs(other.currentWidth - currentWidth) <= lineWidthEpsilon);
        } else {
            result = false;
        }

        return result;
    }


    bool TableLineSettings::operator!=(const TableLineSettings& other) const {
        return !operator==(other);
    }


    Util::HashResult qHash(const TableLineSettings& key, Util::HashSeed seed) {
        double hashValue = (key.width() * 16) + static_cast<unsigned>(key.lineStyle());
        return ::qHash(hashValue, seed);
    }


    Util::HashResult qHash(TableLineSettings::Style key, Util::HashSeed seed) {
        return ::qHash(static_cast<unsigned>(key), seed);
    }
}
