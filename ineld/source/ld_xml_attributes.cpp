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
* This file implements the \ref Ld::XmlAttributes class.
***********************************************************************************************************************/

#include <cstdint>

#include <QString>
#include <QColor>
#include <QXmlStreamAttributes>

#include "ld_format_organizer.h"
#include "ld_handle.h"
#include "ld_xml_attributes.h"

namespace Ld {
    XmlAttributes::XmlAttributes() {}


    XmlAttributes::XmlAttributes(const XmlAttributes &other):QXmlStreamAttributes(other) {}


    XmlAttributes::XmlAttributes(const QXmlStreamAttributes &other):QXmlStreamAttributes(other) {}


    XmlAttributes::~XmlAttributes() {}


    void XmlAttributes::append(const QString& name, const QString& value) {
        QXmlStreamAttributes::append(name, value);
    }


    void XmlAttributes::append(const QString& name, const char* value) {
        QXmlStreamAttributes::append(name, QString(value));
    }


    void XmlAttributes::append(const QString& name, bool value) {
        QXmlStreamAttributes::append(name, (value ? QString("true") : QString("false")));
    }


    void XmlAttributes::append(const QString& name, const Ld::Handle& value) {
        QXmlStreamAttributes::append(name, value.toQString());
    }


    void XmlAttributes::append(const QString& name, const QColor& value) {
        if (value.isValid()) {
            QXmlStreamAttributes::append(name, value.name(QColor::NameFormat::HexArgb));
        } else {
            QXmlStreamAttributes::append(name, QString("-"));
        }
    }


    std::int8_t XmlAttributes::signed8BitValue(const QString& name, bool* ok) const {
        bool         isOK      = true;
        QString      rawValue  = QXmlStreamAttributes::value(name).toString();
        std::int64_t converted = rawValue.toLongLong(&isOK);

        if (isOK) {
            isOK = (converted >= INT8_MIN && converted <= INT8_MAX);
        }

        if (ok != nullptr) {
            *ok = isOK;
        }

        return static_cast<std::int8_t>(converted);
    }


    std::int16_t XmlAttributes::signed16BitValue(const QString& name, bool* ok) const {
        bool         isOK      = true;
        QString      rawValue  = QXmlStreamAttributes::value(name).toString();
        std::int64_t converted = rawValue.toLongLong(&isOK);

        if (isOK) {
            isOK = (converted >= INT16_MIN && converted <= INT16_MAX);
        }

        if (ok != nullptr) {
            *ok = isOK;
        }

        return static_cast<std::int16_t>(converted);
    }


    std::int32_t XmlAttributes::signed32BitValue(const QString& name, bool* ok) const {
        bool         isOK      = true;
        QString      rawValue  = QXmlStreamAttributes::value(name).toString();
        std::int64_t converted = rawValue.toLongLong(&isOK);

        if (isOK) {
            isOK = (converted >= INT32_MIN && converted <= INT32_MAX);
        }

        if (ok != nullptr) {
            *ok = isOK;
        }

        return static_cast<std::int32_t>(converted);
    }


    std::int64_t XmlAttributes::signed64BitValue(const QString& name, bool* ok) const {
        QString rawValue = QXmlStreamAttributes::value(name).toString();
        return rawValue.toLongLong(ok);
    }


    std::uint8_t XmlAttributes::unsigned8BitValue(const QString& name, bool* ok) const {
        bool          isOK      = true;
        QString       rawValue  = QXmlStreamAttributes::value(name).toString();
        std::uint64_t converted = rawValue.toULongLong(&isOK);

        if (isOK) {
            isOK = (converted <= UINT8_MAX);
        }

        if (ok != nullptr) {
            *ok = isOK;
        }

        return static_cast<std::uint8_t>(converted);
    }


    std::uint16_t XmlAttributes::unsigned16BitValue(const QString& name, bool* ok) const {
        bool          isOK      = true;
        QString       rawValue  = QXmlStreamAttributes::value(name).toString();
        std::uint64_t converted = rawValue.toULongLong(&isOK);

        if (isOK) {
            isOK = (converted <= UINT16_MAX);
        }

        if (ok != nullptr) {
            *ok = isOK;
        }

        return static_cast<std::uint16_t>(converted);
    }


    std::uint32_t XmlAttributes::unsigned32BitValue(const QString& name, bool* ok) const {
        bool          isOK      = true;
        QString       rawValue  = QXmlStreamAttributes::value(name).toString();
        std::uint64_t converted = rawValue.toULongLong(&isOK);

        if (isOK) {
            isOK = (converted <= UINT32_MAX);
        }

        if (ok != nullptr) {
            *ok = isOK;
        }

        return static_cast<std::uint32_t>(converted);
    }


    std::uint64_t XmlAttributes::unsigned64BitValue(const QString& name, bool* ok) const {
        QString rawValue = QXmlStreamAttributes::value(name).toString();
        return rawValue.toULongLong(ok);
    }


    bool XmlAttributes::boolValue(const QString& name, bool* ok) const {
        bool    isOK     = true;
        bool    result   = false;
        QString rawValue = QXmlStreamAttributes::value(name).toString().trimmed().toLower();

        if (rawValue == "true" || rawValue == "yes" || rawValue == "1" || rawValue == "t" || rawValue == "y") {
            result = true;
        } else if (rawValue == "false" || rawValue == "no" || rawValue == "0" || rawValue == "f" || rawValue == "n") {
            result = false;
        } else if (ok != nullptr) {
            isOK = false;
        }

        if (ok != nullptr) {
            *ok = isOK;
        }

        return result;
    }


    float XmlAttributes::floatValue(const QString& name, bool* ok) const {
        QString rawValue = QXmlStreamAttributes::value(name).toString().trimmed().toLower();
        return rawValue.toFloat(ok);
    }


    double XmlAttributes::doubleValue(const QString& name, bool* ok) const {
        QString rawValue = QXmlStreamAttributes::value(name).toString().trimmed().toLower();
        return rawValue.toDouble(ok);
    }


    Handle XmlAttributes::handleValue(const QString& name, bool* ok) const {
        QString rawValue = QXmlStreamAttributes::value(name).toString();
        Handle  result   = Handle::fromString(rawValue);

        if (ok != nullptr) {
            *ok = result.isValid();
        }

        return result;
    }


    QColor XmlAttributes::colorValue(const QString& name, bool* ok) const {
        QColor result;

        QString rawValue = QXmlStreamAttributes::value(name).toString();
        if (rawValue == QString("-")) {
            if (ok != nullptr) {
                *ok = true;
            }
        } else {
            result = QColor(rawValue);
            if (ok != nullptr) {
                *ok = result.isValid();
            }
        }

        return result;
    }


    QString XmlAttributes::stringValue(const QString& name, bool* ok) const {
        QString result = QXmlStreamAttributes::value(name).toString();

        if (ok != nullptr) {
            *ok = true;
        }

        return result;
    }


    template<> QString XmlAttributes::value<QString>(const QString& name, bool* ok) const {
        return stringValue(name, ok);
    }


    template<> std::string XmlAttributes::value<std::string>(const QString& name, bool* ok) const {
        return stringValue(name, ok).toStdString();
    }


    template<> const char* XmlAttributes::value<const char*>(const QString& name, bool* ok) const {
        return stringValue(name, ok).toLocal8Bit().data();
    }


    template<> Handle XmlAttributes::value<Handle>(const QString& name, bool* ok) const {
        return handleValue(name, ok);
    }
};
