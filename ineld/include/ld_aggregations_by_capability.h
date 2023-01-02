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
* This header defines the \ref Ld::AggregationsByCapability class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_AGGREGATIONS_BY_CAPABILITY_H
#define LD_AGGREGATIONS_BY_CAPABILITY_H

#include <QString>
#include <QList>
#include <QHash>
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_format.h"

namespace Ld {
    /**
     * Class that manages a collection of \ref Ld::Format::Aggregation instances.
     */
    class LD_PUBLIC_API AggregationsByCapability {
        public:
            AggregationsByCapability();

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            AggregationsByCapability(const AggregationsByCapability& other);

            ~AggregationsByCapability();

            /**
             * Method that determines if this instance contains any aggregations.
             *
             * \return Returns true if this instance contains one or more aggregations.  Returns false if this instance
             *         does not contain aggregations.
             */
            bool isEmpty() const;

            /**
             * Method that determines if this instance contains one or more aggregations.
             *
             * \return Returns true if this instance does not contain any aggregations.  Returns false if this instance
             *         contains one or more aggregations.
             */
            bool isNotEmpty() const;

            /**
             * Method that clears this aggregation.
             */
            void clear();

            /**
             * Method that returns a list of all the capabilities tracked by this class.
             *
             * \return Returns a list of capabilities.
             */
            QList<QString> capabilities() const;

            /**
             * Method that determines if this class contains an aggregation for a capability.
             *
             * \param[in] capabilityName The capability to check for.
             *
             * \return Returns true if the capability is included in this class.  Returns false if the capability is
             *         not included in this class.
             */
            bool contains(const QString& capabilityName) const;

            /**
             * Method that returns a list of all the elements contained in the aggregations.
             *
             * \return Returns a set of all unique elements contains across all aggregations.
             */
            ElementPointerSet elements() const;

            /**
             * Method that returns a list of all the elements contained in an aggregation for the specified capability.
             *
             * \param[in] capabilityName The capability we want the aggregations for.
             */
            ElementPointerSet elements(const QString& capabilityName) const;

            /**
             * Method that returns a list of all the elements contained in one or more aggregations.
             *
             * \param[in] capabilityNames A list of capability names.
             */
            ElementPointerSet elements(const QList<QString>& capabilityNames) const;

            /**
             * Template method that returns an aggregation properly cast to the correct type.
             *
             * \param[in] capabilityName The capability name.
             *
             * \return Returns the aggregation, properly cast.
             */
            template<typename T> LD_PUBLIC_TEMPLATE_METHOD const T& aggregation(const QString& capabilityName) const {
                return dynamic_cast<const T&>(getAggregation(capabilityName));
            }

            /**
             * Template method that returns an aggregation properly cast to the correct type.
             *
             * \param[in] T              The aggregation type.  Type must be const reference and derived from
             *
             *
             * \param[in] capabilityName The capability name.
             *
             * \return Returns the aggregation, properly cast.
             */
            template<typename T> LD_PUBLIC_TEMPLATE_METHOD const typename T::Aggregation& aggregationForFormat() const {
                return dynamic_cast<const typename T::Aggregation&>(getAggregation(T::formatName));
            }

            /**
             * Method that adds a format to the aggregations by capability.
             *
             * \param[in] format A shared pointer to the format to be added.
             */
            void addFormat(Ld::FormatPointer format);

            /**
             * Method that adds a format to the aggregations by capability.
             *
             * \param[in] element A shared pointer to the element containing the format of interest.
             */
            void addFormat(Ld::ElementPointer element);

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            AggregationsByCapability& operator=(const AggregationsByCapability& other);

        private:
            /**
             * Type used to represent the internal data hash.
             */
            typedef QHash<QString, QSharedPointer<Ld::Format::Aggregation>> HashType;

            /**
             * Method that returns an aggregation for a specific capability.
             *
             * \param[in] capabilityName The aggregation we want based on capability or class name.
             *
             * \return Returns a constant reference to the requested capability.
             */
            const Ld::Format::Aggregation& getAggregation(const QString& capabilityName) const;

            /**
             * Method that adds a format and optional element to the aggregation.
             *
             * \param[in] format  A shared pointer to the format to be added.
             *
             * \param[in] element A shared pointer to the element containing the format of interest.
             */
            void addFormat(FormatPointer format, ElementPointer element);

            /**
             * Method that creates a copy of an aggregation.
             *
             * \param[in] oldAggregation A shared pointer to the old aggregation.
             *
             * \return    Returns a shared pointer to the newly created aggregation.
             */
            QSharedPointer<Format::Aggregation> copyAggregation(QSharedPointer<Format::Aggregation> oldAggregation);

            /**
             * Flag that indicates that a copy on write is currently needed.
             */
            mutable bool copyOnWriteNeeded;

            /**
             * Hash containing the aggregations we're tracking.
             */
            HashType currentAggregationsByCapability;
    };
}

#endif
