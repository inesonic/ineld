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
* This file implements the \ref Ld::Location class.
***********************************************************************************************************************/

#include <QPointF>
#include <QHash>

#include <limits>
#include <cmath>
#include <cassert>

#include <util_hash_functions.h>

#include "ld_location.h"

namespace Ld {
    bool Location::currentLeftToRightOrder = Location::defaultLeftToRightOrder;

    Location::Location() {
        currentX = std::numeric_limits<double>::quiet_NaN();
        currentY = std::numeric_limits<double>::quiet_NaN();
    }


    Location::Location(const Location& other) {
        currentX = other.currentX;
        currentY = other.currentY;
    }


    Location::Location(double x, double y) {
        setPosition(x, y);
    }


    Location::Location(const QPointF& position) {
        setPosition(position.x(), position.y());
    }


    Location::~Location() {}


    void Location::setPosition(double newX, double newY) {
        currentX = newX;
        currentY = newY;
    }


    double Location::x() const {
        return currentX;
    }


    double Location::y() const {
        return currentY;
    }


    bool Location::isValid() const {
        return !std::isnan(currentX) && !std::isnan(currentY);
    }


    bool Location::isInvalid() const {
        return !isValid();
    }


    Location& Location::operator=(const Location& other) {
        currentX = other.currentX;
        currentY = other.currentY;

        return *this;
    }


    bool Location::operator==(const Location& other) const {
        return other.currentX == currentX && other.currentY == currentY;
    }


    bool Location::operator!=(const Location& other) const {
        return other.currentX != currentX || other.currentY != currentY;
    }


    bool Location::operator<(const Location& other) const {
        bool wePrecede = false;

        if (y() < other.y()) {
            wePrecede = true;
        } else if (y() == other.y()) {
            if (currentLeftToRightOrder && x() < other.x()) {
                wePrecede = true;
            } else if (!currentLeftToRightOrder && x() > other.x()) {
                wePrecede = true;
            }
        }

        return wePrecede;
    }


    bool Location::operator>(const Location& other) const {
        bool weFollow = false;

        if (y() > other.y()) {
            weFollow = true;
        } else if (y() == other.y()) {
            if (currentLeftToRightOrder && x() > other.x()) {
                weFollow = true;
            } else if (!currentLeftToRightOrder && x() < other.x()) {
                weFollow = true;
            }
        }

        return weFollow;
    }


    bool Location::operator<=(const Location& other) const {
        return operator<(other) || operator==(other);
    }


    bool Location::operator>=(const Location& other) const {
        return operator>(other) || operator==(other);
    }


    QPointF Location::toQPointF() const {
        assert(isValid());
        return QPointF(currentX, currentY);
    }


    Location::operator QPointF() const {
        return toQPointF();
    }


    bool Location::leftToRightOrder() {
        return currentLeftToRightOrder;
    }


    bool Location::rightToLeftOrder() {
        return !leftToRightOrder();
    }


    void Location::setLeftToRightOrder(bool nowLeftToRightOrder) {
        currentLeftToRightOrder = nowLeftToRightOrder;
    }


    void Location::setRightToLeftOrder(bool nowRightToLeftOrder) {
        setLeftToRightOrder(!nowRightToLeftOrder);
    }


    Util::HashResult qHash(const Location& entry, Util::HashSeed seed) {
        Util::HashResult result;

        if (entry.isValid()) {
            result = ::qHash(entry.x(), seed) ^ ::qHash(entry.y(), seed);
        } else {
            result = 0;
        }

        return result;
    }
}
