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
* This file implements the \ref Ld::ImageFormat class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>
#include <QHash>
#include <QSet>

#include <cassert>

#include <util_hash_functions.h>

#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_program_file.h"
#include "ld_format.h"
#include "ld_image_format.h"

/***********************************************************************************************************************
 * Ld::ImageFormat::Axis
 */

namespace Ld {
    ImageFormat::Axis::Axis() {
        currentValue       = ImageFormat::defaultAxisScalingValue;
        currentScalingMode = ImageFormat::defaultScalingMode;
    }


    ImageFormat::Axis::Axis(float value, ImageFormat::ImageScalingMode scalingMode) {
        currentValue       = value;
        currentScalingMode = scalingMode;
    }


    ImageFormat::Axis::Axis(const ImageFormat::Axis& other) {
        currentValue       = other.currentValue;
        currentScalingMode = other.currentScalingMode;
    }


    ImageFormat::Axis::~Axis() {}


    void ImageFormat::Axis::setValue(float value) {
        currentValue = value;
    }


    float ImageFormat::Axis::value() const {
        return currentValue;
    }


    void ImageFormat::Axis::setScalingMode(ImageFormat::ImageScalingMode scalingMode) {
        currentScalingMode = scalingMode;
    }


    ImageFormat::ImageScalingMode ImageFormat::Axis::scalingMode() const {
        return currentScalingMode;
    }


    ImageFormat::Axis::operator float() const {
        return currentValue;
    }


    ImageFormat::Axis& ImageFormat::Axis::operator=(const ImageFormat::Axis& other) {
        currentValue       = other.currentValue;
        currentScalingMode = other.currentScalingMode;

        return *this;
    }


    bool ImageFormat::Axis::operator==(const ImageFormat::Axis& other) const {
        return other.currentValue == currentValue && other.currentScalingMode == currentScalingMode;
    }


    bool ImageFormat::Axis::operator!=(const ImageFormat::Axis& other) const {
        return other.currentValue != currentValue || other.currentScalingMode != currentScalingMode;
    }
}

/***********************************************************************************************************************
 * Ld::ImageFormat::Aggregation
 */

namespace Ld {
    ImageFormat::Aggregation::Aggregation() {}


    ImageFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* ImageFormat::Aggregation::creator(const QString&) {
        return new ImageFormat::Aggregation;
    }


    QString ImageFormat::Aggregation::typeName() const {
        return ImageFormat::formatName;
    }


    void ImageFormat::Aggregation::clear() {
        currentHorizontalAxis.clear();
        currentVerticalAxis.clear();

        currentHorizontalScalingModes.clear();
        currentHorizontalValues.clear();
        currentVerticalScalingModes.clear();
        currentVerticalValues.clear();

        currentRotations.clear();
    }


    bool ImageFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<ImageFormat> imageFormat = formatInstance.dynamicCast<ImageFormat>();
        if (!imageFormat.isNull()) {
            Q_ASSERT(!imageFormat.isNull());

            success = Format::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentHorizontalAxis         << imageFormat->horizontalAxis();
                currentVerticalAxis           << imageFormat->verticalAxis();

                currentHorizontalScalingModes << imageFormat->horizontalAxis().scalingMode();
                currentHorizontalValues       << imageFormat->horizontalAxis().value();
                currentVerticalScalingModes   << imageFormat->verticalAxis().scalingMode();
                currentVerticalValues         << imageFormat->verticalAxis().value();

                currentRotations              << imageFormat->rotation();
            }
        }

        return success;
    }


    QSet<ImageFormat::Axis> ImageFormat::Aggregation::horizontalAxis() const {
        return currentHorizontalAxis;
    }


    QSet<ImageFormat::Axis> ImageFormat::Aggregation::verticalAxis() const {
        return currentVerticalAxis;
    }


    QSet<ImageFormat::ImageScalingMode> ImageFormat::Aggregation::horizontalScalingModes() const {
        return currentHorizontalScalingModes;
    }


    QSet<float> ImageFormat::Aggregation::horizontalValues() const {
        return currentHorizontalValues;
    }


    QSet<ImageFormat::ImageScalingMode> ImageFormat::Aggregation::verticalScalingModes() const {
        return currentVerticalScalingModes;
    }


    QSet<float> ImageFormat::Aggregation::verticalValues() const {
        return currentVerticalValues;
    }


    QSet<ImageFormat::Rotation> ImageFormat::Aggregation::rotations() const {
        return currentRotations;
    }
}

/***********************************************************************************************************************
 * Ld::ImageFormat
 */

namespace Ld {
    const QString ImageFormat::formatName("ImageFormat");

    const ImageFormat::ImageScalingMode ImageFormat::defaultScalingMode      = ImageFormat::ImageScalingMode::INVALID;
    const float                         ImageFormat::defaultAxisScalingValue = 0;
    const ImageFormat::Rotation         ImageFormat::defaultRotation         = ImageFormat::Rotation::NO_ROTATION;

    ImageFormat::ImageFormat() {
        currentRotation = defaultRotation;
    }


    ImageFormat::ImageFormat(const ImageFormat& other):Format(other) {
        currentHorizontalAxis = other.currentHorizontalAxis;
        currentVerticalAxis   = other.currentVerticalAxis;
        currentRotation       = other.currentRotation;
    }


    ImageFormat::~ImageFormat() {}


    FormatPointer ImageFormat::clone() const {
        return FormatPointer(new ImageFormat(*this));
    }


    Format* ImageFormat::creator(const QString&) {
        return new ImageFormat();
    }


    QString ImageFormat::typeName() const {
        return formatName;
    }


    bool ImageFormat::isValid() const {
        return (
               currentHorizontalAxis.scalingMode() != ImageScalingMode::INVALID
            && currentVerticalAxis.scalingMode()   != ImageScalingMode::INVALID
        );
    }


    Format::Capabilities ImageFormat::capabilities() const {
        return Format::capabilities() << formatName;
    }


    void ImageFormat::setHorizontalAxis(const ImageFormat::Axis& newHorizontalAxis) {
        currentHorizontalAxis = newHorizontalAxis;
        reportFormatUpdated();
    }


    void ImageFormat::setHorizontalAxis(float newValue, ImageFormat::ImageScalingMode newScalingMode) {
        currentHorizontalAxis.setValue(newValue);
        currentHorizontalAxis.setScalingMode(newScalingMode);
        reportFormatUpdated();
    }


    const ImageFormat::Axis& ImageFormat::horizontalAxis() const {
        return currentHorizontalAxis;
    }


    void ImageFormat::setVerticalAxis(const ImageFormat::Axis& newVerticalAxis) {
        currentVerticalAxis = newVerticalAxis;
        reportFormatUpdated();
    }


    void ImageFormat::setVerticalAxis(float newValue, ImageFormat::ImageScalingMode newScalingMode) {
        currentVerticalAxis.setValue(newValue);
        currentVerticalAxis.setScalingMode(newScalingMode);
        reportFormatUpdated();
    }


    const ImageFormat::Axis& ImageFormat::verticalAxis() const {
        return currentVerticalAxis;
    }


    void ImageFormat::setRotation(ImageFormat::Rotation newRotation) {
        currentRotation = newRotation;
        reportFormatUpdated();
    }


    ImageFormat::Rotation ImageFormat::rotation() const {
        return currentRotation;
    }


    QString ImageFormat::toString() const {
        return QString("ImageFormat,%1,%2,%3,%4,%5").arg(Format::toString(currentHorizontalAxis.scalingMode()))
                                                    .arg(currentHorizontalAxis.value())
                                                    .arg(Format::toString(currentVerticalAxis.scalingMode()))
                                                    .arg(currentVerticalAxis.value())
                                                    .arg(90 * static_cast<unsigned>(currentRotation));
    }


    QString ImageFormat::toCss() const {
        QString result;

        switch (currentRotation) {
            case Rotation::NO_ROTATION:   {                                         break; }
            case Rotation::ROTATE_CW_90:  { result = "transform: rotate(90deg); ";  break; }
            case Rotation::FLIP:          { result = "transform: rotate(180deg); "; break; }
            case Rotation::ROTATE_CCW_90: { result = "transform: rotate(270deg); "; break; }
            default:                      {                                         break; }
        }

        return result;
    }


    void ImageFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (currentHorizontalAxis.scalingMode() != defaultScalingMode) {
            attributes.append("horizontal_mode", Format::toString(currentHorizontalAxis.scalingMode()));
        }

        if (currentHorizontalAxis.value() != defaultAxisScalingValue) {
            attributes.append("horizontal_value", currentHorizontalAxis.value());
        }

        if (currentVerticalAxis.scalingMode() != defaultScalingMode) {
            attributes.append("vertical_mode", Format::toString(currentVerticalAxis.scalingMode()));
        }

        if (currentVerticalAxis.value() != defaultAxisScalingValue) {
            attributes.append("vertical_value", currentVerticalAxis.value());
        }

        if (currentRotation != defaultRotation) {
            attributes.append("rotation", 90 * static_cast<unsigned>(currentRotation));
        }
    }


    void ImageFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier&,
            ProgramFile&,
            unsigned
        ) {
        Rotation newRotation = Rotation::NO_ROTATION;
        Axis     newHorizontalAxis;
        Axis     newVerticalAxis;
        bool     ok;

        if (attributes.hasAttribute("horizontal_mode")) {
            ImageScalingMode scalingMode = toScalingMode(attributes.value<QString>("horizontal_mode", &ok));
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid \"horizontal_mode\" attribute").arg(tag));
            } else {
                newHorizontalAxis.setScalingMode(scalingMode);
            }
        }

        newHorizontalAxis.setValue(attributes.value<float>("horizontal_value", defaultAxisScalingValue, &ok));
        if (!ok) {
            QString tag = reader->qualifiedName().toString();
            reader->raiseError(tr("Tag \"%1\", invalid/missing \"horizontal_value\" attribute").arg(tag));
        }

        if (attributes.hasAttribute("vertical_mode")) {
            ImageScalingMode scalingMode = toScalingMode(attributes.value<QString>("vertical_mode", &ok));
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid \"vertical_mode\" attribute").arg(tag));
            } else {
                newVerticalAxis.setScalingMode(scalingMode);
            }
        }

        newVerticalAxis.setValue(attributes.value<float>("vertical_value", defaultAxisScalingValue, &ok));
        if (!ok) {
            QString tag = reader->qualifiedName().toString();
            reader->raiseError(tr("Tag \"%1\", invalid/missing \"vertical_value\" attribute").arg(tag));
        }

        if (attributes.hasAttribute("rotation")) {
            unsigned rotation = attributes.value<unsigned>("rotation", &ok);
            if (!ok || (rotation % 90) != 0 || rotation > 270) {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid \"rotation\" attribute").arg(tag));
            } else {
                newRotation = static_cast<Rotation>(rotation / 90);
            }
        } else {
            newRotation = defaultRotation;
        }

        if (!reader->hasError()) {
            currentHorizontalAxis = newHorizontalAxis;
            currentVerticalAxis   = newVerticalAxis;
            currentRotation       = newRotation;
        }
    }


    void ImageFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        ImageFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void ImageFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        ImageFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }


    Util::HashResult qHash(const ImageFormat::Axis& axis, Util::HashSeed seed) {
        return ::qHash(axis.value(), seed) ^ qHash(axis.scalingMode(), seed);
    }
}
