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
* This file implements the \ref Ld::Format class.
***********************************************************************************************************************/

#include <QString>
#include <QMap>
#include <QList>
#include <QSet>
#include <QHash>
#include <QSharedPointer>
#include <QWeakPointer>

#include <util_hash_functions.h>

#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_xml_element.h"
#include "ld_program_file.h"
#include "ld_element.h"
#include "ld_format.h"

/***********************************************************************************************************************
 * Ld::Format::BooleanAggregation
 */

namespace Ld {
    void Format::BooleanAggregation::include(bool newValue) {
        if (currentValue == EnumeratedValue::INVALID) {
            currentValue = newValue ? EnumeratedValue::ALL_TRUE : EnumeratedValue::ALL_FALSE;
        } else if ((currentValue == EnumeratedValue::ALL_TRUE && !newValue) ||
                   (currentValue == EnumeratedValue::ALL_FALSE && newValue)    ) {
            currentValue = EnumeratedValue::BOTH;
        }
    }
}

/***********************************************************************************************************************
 * Ld::Format::Aggregation
 */

namespace Ld {
    QHash<QString, Format::Aggregation::CreatorFunction> Format::Aggregation::creatorFunctions;

    Format::Aggregation::Aggregation() {}


    Format::Aggregation::~Aggregation() {}


    bool Format::Aggregation::registerCreator(const QString& formatName, CreatorFunction creatorFunction) {
        bool success;

        if (!creatorFunctions.contains(formatName)) {
            creatorFunctions.insert(formatName, creatorFunction);
            success = true;
        } else {
            success = false;
        }

        return success;
    }


    QSharedPointer<Format::Aggregation> Format::Aggregation::create(const QString& formatName) {
        QSharedPointer<Format::Aggregation> newAggregation;

        if (creatorFunctions.contains(formatName)) {
            CreatorFunction creatorFunction = creatorFunctions.value(formatName);
            Aggregation*    aggregation     = (*creatorFunction)(formatName);

            newAggregation.reset(aggregation);
        }

        return newAggregation;
    }


    void Format::Aggregation::clear() {
        currentFormats.clear();
        ElementWeakPointerSet::clear();
    }


    bool Format::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool result;

        if (currentFormats.contains(formatInstance)) {
            result = includeExisting;
        } else {
            currentFormats.insert(formatInstance.toWeakRef());
            result = true;
        }

        return result;
    }


    bool Format::Aggregation::removeFormat(FormatPointer formatInstance) {
        bool result;

        if (currentFormats.contains(formatInstance)) {
            currentFormats.remove(formatInstance);

            formatChanged();

            result = true;
        } else {
            result = false;
        }

        return result;
    }


    FormatPointerSet Format::Aggregation::formats() const {
        FormatPointerSet result;

        for (  FormatWeakPointerSet::const_iterator it = currentFormats.constBegin(), end = currentFormats.constEnd()
             ; it != end
             ; ++it
            ) {
            FormatPointer format = it->toStrongRef();
            if (!format.isNull()) {
                result.insert(format);
            }
        }

        return result;
    }


    void Format::Aggregation::formatChanged() {
        FormatPointerSet allFormats = formats();

        clear();
        for (  FormatPointerSet::const_iterator it = allFormats.constBegin(), end = allFormats.constEnd()
             ; it != end
             ; ++it
            ) {
            FormatPointer format = *it;
            addFormat(format);
        }
    }
}

/***********************************************************************************************************************
 * Ld::Format
 */

namespace Ld {
    const QString                          Format::formatName("Format");
    QMap<QString, Format::CreatorFunction> Format::creators;

    FormatPointer Format::create(const QString& typeName) {
        Format*         newFormat       = nullptr;
        CreatorFunction creatorFunction = creators.value(typeName);

        if (creatorFunction != nullptr) {
            newFormat = (*creatorFunction)(typeName);
        }

        return FormatPointer(newFormat);
    }


    bool Format::registerCreator(const QString& typeName, Format::CreatorFunction creatorFunction) {
        bool success;

        if (creatorFunction != nullptr && !creators.contains(typeName)) {
            creators.insert(typeName, creatorFunction);
            success = true;
        } else {
            success = false;
        }

        return success;
    }


    Format::Format() {}


    Format::Format(const Format& /* other */) {}


    Format::~Format() {}


    FormatPointer Format::formatForSplit(ElementPointer) {
        return FormatPointer();
    }


    bool Format::isInvalid() const {
        return !isValid();
    }


    Format::Capabilities Format::capabilities() const {
        return Capabilities() << formatName;
    }


    QString Format::toCssEntry(const QString& className, const QString& tag, const QString& id) const {
        QString result;
        QString css = toCss();

        if (!css.isEmpty()) {
            QString selector;

            if (!tag.isEmpty()) {
                selector = tag;
            }

            if (!className.isEmpty()) {
                selector += QString(".%1").arg(className);
            }

            if (!id.isEmpty()) {
                selector += QString("#%1").arg(id);
            }

            result = QString("%1 { %2}\n").arg(selector).arg(toCss());
        }

        return result;
    }


    ElementPointerList Format::elements() const {
        ElementPointerList result;

        for (  ElementWeakPointerSet::const_iterator it = currentElements.begin(), end = currentElements.end()
             ; it!=end
             ; ++it
            ) {
            ElementPointer element = it->toStrongRef();
            if (element) {
                result.append(element);
            }
        }

        return result;
    }


    QString Format::toString(Format::ImageScalingMode mode) {
        QString result;

        switch (mode) {
            case ImageScalingMode::INVALID: {
                result = QString("INVALID");
                break;
            }

            case ImageScalingMode::IGNORED: {
                result = QString("IGNORED");
                break;
            }

            case ImageScalingMode::FIXED_POINTS: {
                result = QString("FIXED");
                break;
            }

            case ImageScalingMode::FRACTIONAL: {
                result = QString("FRACTIONAL");
                break;
            }

            case ImageScalingMode::ASPECT_RATIO: {
                result = QString("ASPECT_RATIO");
                break;
            }

            default: {
                Q_ASSERT(false);
                break;
            }
        }

        return result;
    }


    Format::ImageScalingMode Format::toScalingMode(const QString& text, bool* ok) {
        ImageScalingMode result;
        bool             success;

        if (text == QString("INVALID")) {
            result  = ImageScalingMode::INVALID;
            success = true;
        } else if (text == QString("IGNORED")) {
            result  = ImageScalingMode::IGNORED;
            success = true;
        } else if (text == QString("FIXED")) {
            result  = ImageScalingMode::FIXED_POINTS;
            success = true;
        } else if (text == QString("FRACTIONAL")) {
            result  = ImageScalingMode::FRACTIONAL;
            success = true;
        } else if (text == QString("ASPECT_RATIO")) {
            result  = ImageScalingMode::ASPECT_RATIO;
            success = true;
        } else {
            result  = ImageScalingMode::INVALID;
            success = false;
        }

        if (ok != nullptr) {
            *ok = success;
        }

        return result;
    }


    void Format::reportFormatUpdated() {
        for (  ElementWeakPointerSet::const_iterator elementIterator    = currentElements.constBegin(),
                                                     elementEndIterator = currentElements.constEnd()
             ; elementIterator != elementEndIterator
             ; ++elementIterator
            ) {
            ElementPointer element = elementIterator->toStrongRef();
            if (element) {
                element->formatUpdated();
            }
        }
    }


    void Format::writeChildren(
            QSharedPointer<XmlWriter>,
            QSharedPointer<FormatOrganizer>, ProgramFile&,
            const XmlAttributes&
        ) const {}


    Util::HashResult qHash(Format::ImageScalingMode scalingFormat, Util::HashSeed seed) {
        return ::qHash(static_cast<unsigned>(scalingFormat), seed);
    }


    Util::HashResult qHash(Format::Rotation rotation, Util::HashSeed seed) {
        return ::qHash(static_cast<unsigned>(rotation), seed);
    }
}
