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
* This file implements the \ref Ld::VariableName class.
***********************************************************************************************************************/

#include <QString>
#include <QHash>

#include <util_hash_functions.h>

#include "ld_variable_name.h"

namespace Ld {
    VariableName::VariableName() {}


    VariableName::VariableName(const QString& text1, const QString& text2) {
        currentText1 = text1;
        currentText2 = text2;
    }


    VariableName::VariableName(const VariableName& other) {
        currentText1 = other.currentText1;
        currentText2 = other.currentText2;
    }


    VariableName::~VariableName() {}


    bool VariableName::isValid() const {
        return !currentText1.isEmpty();
    }


    bool VariableName::isInvalid() const {
        return currentText1.isEmpty();
    }


    void VariableName::setText1(const QString& newText1) {
        currentText1 = newText1;
    }


    QString VariableName::text1() const {
        return currentText1;
    }


    void VariableName::setText2(const QString& newText2) {
        currentText2 = newText2;
    }


    QString VariableName::text2() const {
        return currentText2;
    }


    VariableName& VariableName::operator=(const VariableName& other) {
        currentText1 = other.currentText1;
        currentText2 = other.currentText2;

        return *this;
    }


    bool VariableName::operator==(const VariableName& other) const {
        return other.currentText1 == currentText1 && other.currentText2 == currentText2;
    }


    bool VariableName::operator!=(const VariableName& other) const {
        return other.currentText1 != currentText1 || other.currentText2 != currentText2;
    }


    bool VariableName::operator<(const VariableName& other) const {
        return (
               currentText1 < other.currentText1
            || (currentText1 == other.currentText1 && currentText2 < other.currentText2)
        );
    }


    bool VariableName::operator>(const VariableName& other) const {
        return (
               currentText1 > other.currentText1
            || (currentText1 == other.currentText1 && currentText2 > other.currentText2)
        );
    }


    bool VariableName::operator<=(const VariableName& other) const {
        return !operator>(other);
    }


    bool VariableName::operator>=(const VariableName& other) const {
        return !operator<(other);
    }


    Util::HashResult qHash(const VariableName& entry, Util::HashSeed seed) {
        return ::qHash(entry.text1(), seed) ^ ::qHash(entry.text2(), seed);
    }
}
