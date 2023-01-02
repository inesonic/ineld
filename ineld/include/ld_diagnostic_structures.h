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
* This header defines various data structures using the \ref Ld::Diagnostic class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_DIAGNOSTIC_STRUCTURES_H
#define LD_DIAGNOSTIC_STRUCTURES_H

#include <QSharedPointer>
#include <QWeakPointer>
#include <QMap>
#include <QList>
#include <QSet>

#include <util_hash_functions.h>

#include "ld_common.h"
#include "ld_diagnostic.h"

namespace Ld {
    class Diagnostic;

    /**
     * Type used to represent a pointer to an Diagnostic instance.
     */
    typedef QSharedPointer<Diagnostic> DiagnosticPointer;

    /**
     * Type used to represent a weak pointer to a Diagnostic instance.
     */
    typedef QWeakPointer<Diagnostic> DiagnosticWeakPointer;

    /**
     * Type used to represent a list of diagnostic pointers.
     */
    typedef QList<DiagnosticPointer> DiagnosticPointerList;

    /**
     * Type used to represent a list of diagnostic weak pointers.
     */
    typedef QList<DiagnosticWeakPointer> DiagnosticWeakPointerList;

    /**
     * Type used to represent a set of diagnostic pointers.
     */
    typedef QSet<DiagnosticPointer> DiagnosticPointerSet;

    /**
     * Type used to represent a set of diagnostic weak pointer.
     */
    typedef QSet<DiagnosticWeakPointer> DiagnosticWeakPointerSet;

    /**
     * Function that calculates a hash for a pointer to a diagnostic.
     *
     * \param[in] key  The key to calculate a hash for.
     *
     * \param[in] seed An optional seed used to add entropy to the hash.
     */
    LD_PUBLIC_API Util::HashResult qHash(const DiagnosticPointer& key, Util::HashSeed seed = 0);

    /**
     * Function that calculates a hash for a weak pointer to a diagnostic.
     *
     * \param[in] key  The key to calculate a hash for.
     *
     * \param[in] seed An optional seed used to add entropy to the hash.
     */
    LD_PUBLIC_API Util::HashResult qHash(const DiagnosticWeakPointer& key, Util::HashSeed seed = 0);
};

#endif
