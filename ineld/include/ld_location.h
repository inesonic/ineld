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
* This header defines the \ref Ld::Location class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_LOCATION_H
#define LD_LOCATION_H

#include <QPointF>

#include <util_hash_functions.h>

#include "ld_common.h"

namespace Ld {
    /**
     * Class used to track the location of floating elements.  The class also provides operators to provide a comparison
     * of the relative position assuming either a left-to-right or right-to-left reading order.
     *
     * A location can also take on an "invalid" value indicating that no location value has been set.
     *
     * The class has been largely written without Qt dependencies; however, the class does include several functions and
     * methods to make integration with Qt reasonably seamless.
     */
    class LD_PUBLIC_API Location {
        public:
            Location();

            /**
             * Copy constructor.
             *
             * \param[in] other The other instance to be copied.
             */
            Location(const Location& other);

            /**
             * Constructor.  Accepts an X,Y coordinate value.
             *
             * \param[in] x The X position.
             *
             * \param[in] y The Y position.
             */
            Location(double x, double y);

            /**
             * Constructor.  Accepts a  QPointF instance.
             *
             * \param[in] position The desired position.  Note that, given limitations in the Qt QPointF class, a null
             *                     position will be treated as location 0,0.
             */
            Location(const QPointF& position);

            ~Location();

            /**
             * Method you can use to set the position of this location.
             *
             * \param[in] newX The new X offset.
             *
             * \param[in] newY The new Y offset.
             */
            void setPosition(double newX, double newY);

            /**
             * Method you can use to obtain the X offset.
             *
             * \return Returns the current X offset.  An invalid location will return a value of NaN.
             */
            double x() const;

            /**
             * Method you can use to obtain the Y offset.
             *
             * \return Returns the current Y offset.  A null location will return a value of NaN.
             */
            double y() const;

            /**
             * Method you can use to determine if this location is valid or invalid.
             *
             * \return Returns true if the location is valid.  Returns false if the location is invalid.
             */
            bool isValid() const;

            /**
             * Method you can use to determine if this location is invalidor valid.
             *
             * \return Returns true if the location is invalid.  Returns false if the location is valid.
             */
            bool isInvalid() const;

            /**
             * Assignment operator.
             *
             * \param[in] other The instance containing values to assign to this.
             *
             * \return Returns a reference to this.
             */
            Location& operator=(const Location& other);

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the two values point to the same location.
             */
            bool operator==(const Location& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the two values point to different locations.
             */
            bool operator!=(const Location& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if this instance should precede the other instance in program order.  Returns false
             *         if this instance should follow the other instance in program order. Will return false if the two
             *         instances are at the same location.
             */
            bool operator<(const Location& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if this instance should follow the other instance in program order.  Returns false
             *         if this instance should precede the other instance in program order. Will return false if the two
             *         instances are at the same location.
             */
            bool operator>(const Location& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if this instance should precede the other instance in program order.  Returns false
             *         if this instance should follow the other instance in program order. Will return true if the two
             *         instances are at the same location.
             */
            bool operator<=(const Location& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if this instance should follow the other instance in program order.  Returns false
             *         if this instance should precede the other instance in program order. Will return true if the two
             *         instances are at the same location.
             */
            bool operator>=(const Location& other) const;

            /**
             * Method you can call to convert this location to a QPointF instance.  The method will assert if the
             * location is invalid.
             *
             * \return Returns a QPointF instance matching this location.
             */
            QPointF toQPointF() const;

            /**
             * Casting operator that can be used to seamlessly cast this instance to a QPointF.  The operator will
             * assert if the location is invalid.
             *
             * \return Returns a QPointF instance matching this location.
             */
            operator QPointF() const;

            /**
             * Method you can use to determine if the language parser is assuming left-to-right order or right-to-left
             * order.
             *
             * \return Returns true if the system is assuming a left-to-right program order.  Returns false if the
             *         system is assuming right-to-left program order.
             */
            static bool leftToRightOrder();

            /**
             * Method you can use to determine if the language parser is assuming right-to-left order or left-to-right
             * order.
             *
             * \return Returns true if the system is assuming a right-to-left program order.  Returns false if the
             *         system is assuming left-to-right program order.
             */
            static bool rightToLeftOrder();

            /**
             * Method you can use to specify a left-to-right or right-to-left program order.  Note that the system will
             * assume a left-to-right program order, by default.
             *
             * \param[in] nowLeftToRightOrder If true, the program will assume a left-to-right program order.  If
             *                                false, the program will assume a right-to-left program order.
             */
            static void setLeftToRightOrder(bool nowLeftToRightOrder = true);

            /**
             * Method you can use to specify a right-to-left or left-to-right program order.  Note that the system will
             * assume a left-to-right program order, by default.
             *
             * \param[in] nowRightToLeftOrder If true, the program will assume a right-to-left program order.  If
             *                                false, the program will assume a left-to-right program order.
             */
            static void setRightToLeftOrder(bool nowRightToLeftOrder = true);

        private:
            /**
             * The current X offset.
             */
            double currentX;

            /**
             * The current Y offset.
             */
            double currentY;

            /**
             * Constant used to specify the default program order.
             */
            static constexpr bool defaultLeftToRightOrder = true;

            /**
             * Static value used to indicate the expected program order.
             */
            static bool currentLeftToRightOrder;
    };

    /**
     * Hashing function for use with the \ref Ld::Location class.  This function exists to support the Qt QSet and
     * QHash classes.
     *
     * \param[in] entry The entry to calculate a hash for.
     *
     * \param[in] seed  An optional seed value.
     *
     * \return Returns a hash for the provided seed.
     */
    LD_PUBLIC_API Util::HashResult qHash(const Location& entry, Util::HashSeed seed = 1);
};

#endif
