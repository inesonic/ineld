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
* This file implements the \ref Ld::AggregationsByCapability class.
***********************************************************************************************************************/

#include <QString>
#include <QList>
#include <QHash>
#include <QSharedPointer>

#include "ld_element.h"
#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_format.h"
#include "ld_aggregations_by_capability.h"

namespace Ld {
    AggregationsByCapability::AggregationsByCapability() {
        copyOnWriteNeeded = false;
    }


    AggregationsByCapability::AggregationsByCapability(const AggregationsByCapability& other) {
        currentAggregationsByCapability = other.currentAggregationsByCapability;
        copyOnWriteNeeded               = true;
        other.copyOnWriteNeeded         = true;
    }


    AggregationsByCapability::~AggregationsByCapability() {}


    bool AggregationsByCapability::isEmpty() const {
        return currentAggregationsByCapability.isEmpty();
    }


    bool AggregationsByCapability::isNotEmpty() const {
        return !currentAggregationsByCapability.isEmpty();
    }


    void AggregationsByCapability::clear() {
        currentAggregationsByCapability.clear();
        copyOnWriteNeeded = false;
    }


    QList<QString> AggregationsByCapability::capabilities() const {
        return currentAggregationsByCapability.keys();
    }


    bool AggregationsByCapability::contains(const QString& capabilityName) const {
        return currentAggregationsByCapability.contains(capabilityName);
    }


    ElementPointerSet AggregationsByCapability::elements() const {
        ElementPointerSet result;

        for (  HashType::const_iterator aggregationIterator    = currentAggregationsByCapability.constBegin(),
                                        aggregationEndIterator = currentAggregationsByCapability.constEnd()
             ; aggregationIterator != aggregationEndIterator
             ; ++aggregationIterator
            ) {
            QSharedPointer<Format::Aggregation> aggregation = aggregationIterator.value();
            for (  Format::Aggregation::const_iterator elementIterator    = aggregation->constBegin(),
                                                       elementEndIterator = aggregation->constEnd()
                 ; elementIterator != elementEndIterator
                 ; ++elementIterator
                ) {
                ElementPointer element = elementIterator->toStrongRef();
                if (!element.isNull()) {
                    result << element;
                }
            }
        }

        return result;
    }


    ElementPointerSet AggregationsByCapability::elements(const QString& capabilityName) const {
        ElementPointerSet result;

        if (currentAggregationsByCapability.contains(capabilityName)) {
            QSharedPointer<Format::Aggregation> aggregation = currentAggregationsByCapability.value(capabilityName);
            for (  Format::Aggregation::const_iterator elementIterator    = aggregation->constBegin(),
                                                       elementEndIterator = aggregation->constEnd()
                 ; elementIterator != elementEndIterator
                 ; ++elementIterator
                ) {
                ElementPointer element = elementIterator->toStrongRef();
                if (!element.isNull()) {
                    result << element;
                }
            }
        }

        return result;
    }


    ElementPointerSet AggregationsByCapability::elements(const QList<QString>& capabilityNames) const {
        ElementPointerSet result;

        for (  QList<QString>::const_iterator capabilityIterator = capabilityNames.constBegin(),
                                              capabilityEndIterator = capabilityNames.constEnd()
             ; capabilityIterator != capabilityEndIterator
             ; ++capabilityIterator
            ) {
            result += elements(*capabilityIterator);
        }

        return result;
    }


    void AggregationsByCapability::addFormat(FormatPointer format) {
        addFormat(format, ElementPointer());
    }


    void AggregationsByCapability::addFormat(ElementPointer element) {
        FormatPointer format = element->format();
        if (!format.isNull()) {
            addFormat(format, element);
        }
    }


    AggregationsByCapability& AggregationsByCapability::operator=(const AggregationsByCapability& other) {
        currentAggregationsByCapability = other.currentAggregationsByCapability;
        copyOnWriteNeeded               = true;
        other.copyOnWriteNeeded         = true;

        return *this;
    }


    const Format::Aggregation& AggregationsByCapability::getAggregation(const QString& capabilityName) const {
        return *currentAggregationsByCapability.value(capabilityName);
    }


    void AggregationsByCapability::addFormat(FormatPointer format, ElementPointer element) {
        if (copyOnWriteNeeded) {
            HashType newHash;

            for (  HashType::const_iterator pos = currentAggregationsByCapability.constBegin(),
                                            end = currentAggregationsByCapability.constEnd()
                 ; pos != end
                 ; ++pos
                ) {
                QString                                 capabilityName = pos.key();
                QSharedPointer<Ld::Format::Aggregation> aggregation    = pos.value();
                newHash.insert(capabilityName, copyAggregation(aggregation));
            }

            currentAggregationsByCapability = newHash;
            copyOnWriteNeeded               = false;
        }

        Format::Capabilities capabilities = format->capabilities();
        for (  Format::Capabilities::const_iterator it  = capabilities.constBegin(),
                                                    end = capabilities.constEnd()
             ; it!=end
             ; ++it
            ) {
            QString capabilityName = *it;

            QSharedPointer<Format::Aggregation> aggregation = currentAggregationsByCapability.value(capabilityName);
            if (aggregation.isNull()) {
                aggregation = Format::Aggregation::create(capabilityName);
                if (!aggregation.isNull()) {
                    currentAggregationsByCapability.insert(capabilityName, aggregation);
                }
            }

            if (!aggregation.isNull()) {
                aggregation->addFormat(format);
                if (!element.isNull()) {
                    aggregation->insert(element.toWeakRef());
                }
            }
        }
    }


    QSharedPointer<Format::Aggregation> AggregationsByCapability::copyAggregation(
            QSharedPointer<Format::Aggregation> oldAggregation
        ) {
        QString                             aggregationType = oldAggregation->typeName();
        QSharedPointer<Format::Aggregation> newAggregation  = Format::Aggregation::create(aggregationType);

        FormatPointerSet formats = oldAggregation->formats();
        for (  FormatPointerSet::const_iterator formatIterator    = formats.constBegin(),
                                                formatEndIterator = formats.constEnd()
             ; formatIterator != formatEndIterator
             ; ++formatIterator
            ) {
            newAggregation->addFormat(*formatIterator);
        }

        ElementWeakPointerSet elements = *oldAggregation;
        for (  ElementWeakPointerSet::const_iterator elementIterator = elements.constBegin(),
                                                     elementEndIterator = elements.constEnd()
             ; elementIterator != elementEndIterator
             ; ++elementIterator
            ) {
            newAggregation->insert(*elementIterator);
        }

        return newAggregation;
    }
}
