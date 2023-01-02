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
* This header defines the \ref Ld::XmlAttributes class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_XML_ATTRIBUTES_H
#define LD_XML_ATTRIBUTES_H

#include <cstdint>
#include <type_traits>
#include <string>
#include <cassert>

#include <QString>
#include <QColor>
#include <QXmlStreamAttributes>

#include "ld_common.h"
#include "ld_handle.h"

namespace Ld {
    /**
     * Class you can use to read and write XML attributes.  The class extends the Qt QXmlStreamAttributes class to
     * both provide abstraction and to provide some additional functionality.
     */
    class LD_PUBLIC_API XmlAttributes:public QXmlStreamAttributes {

        public:
            XmlAttributes();

            /**
             * Copy constructor.
             *
             * \param[in] other The instance to be copied.
             */
            XmlAttributes(const XmlAttributes& other);

            /**
             * Copy constructor.
             *
             * \param[in] other The instance to be copied.
             */
            XmlAttributes(const QXmlStreamAttributes& other);

            ~XmlAttributes();

            /**
             * Method you can use to append a numeric attribute value.
             *
             * \param[in] name The attribute name.
             *
             * \param[in] value The attribute value.
             */
            void append(const QString& name, const QString& value);

            /**
             * Method you can use to append a numeric attribute value.
             *
             * \param[in] name The attribute name.
             *
             * \param[in] value The attribute value.
             */
            void append(const QString& name, const char* value);

            /**
             * Method you can use to append a numeric attribute value.
             *
             * \param[in] name The attribute name.
             *
             * \param[in] value The attribute value.
             */
            void append(const QString& name, bool value);

            /**
             * Method you can use to append a numeric attribute value.
             *
             * \param[in] name The attribute name.
             *
             * \param[in] value The attribute value.
             */
            void append(const QString& name, const Handle& value);

            /**
             * Method you can use to append a color attribute.
             *
             * \param[in] name  The attribute name.
             *
             * \param[in] value The attribute color.
             */
            void append(const QString& name, const QColor& value);

            /**
             * Method you can use to append a numeric attribute value.
             *
             * \param[in] name The attribute name.
             *
             * \param[in] value The attribute value.
             */
            template<typename T> void append(const QString& name, T value) {
                if (std::is_same<T, float>::value) {
                    QXmlStreamAttributes::append(name, QString::number(static_cast<float>(value), 'g', 7));
                } else if (std::is_same<T, double>::value) {
                    QXmlStreamAttributes::append(name, QString::number(static_cast<double>(value), 'g', 14));
                } else if (std::is_signed<T>::value) {
                    QXmlStreamAttributes::append(name, QString::number(static_cast<std::int64_t>(value)));
                } else {
                    QXmlStreamAttributes::append(name, QString::number(static_cast<std::uint64_t>(value)));
                }
            }

            /**
             * Method you can use to obtain an attribute converted to a signed 16-bit value.
             *
             * \param[in] name The attribute name.
             *
             * \param[in] ok   An optional pointer to a boolean value that will be set to true on success, set to false
             *                 on error.
             *
             * \return Returns the desired value. Note that 0 is returned on error.
             */
            std::int8_t signed8BitValue(const QString& name, bool* ok = nullptr) const;

            /**
             * Method you can use to obtain an attribute converted to a signed 16-bit value.
             *
             * \param[in] name The attribute name.
             *
             * \param[in] ok   An optional pointer to a boolean value that will be set to true on success, set to false
             *                 on error.
             *
             * \return Returns the desired value. Note that 0 is returned on error.
             */
            std::int16_t signed16BitValue(const QString& name, bool* ok = nullptr) const;

            /**
             * Method you can use to obtain an attribute converted to a signed 32-bit value.
             *
             * \param[in] name The attribute name.
             *
             * \param[in] ok   An optional pointer to a boolean value that will be set to true on success, set to false
             *                 on error.
             *
             * \return Returns the desired value. Note that 0 is returned on error.
             */
            std::int32_t signed32BitValue(const QString& name, bool* ok = nullptr) const;

            /**
             * Method you can use to obtain an attribute converted to a signed 64-bit value.
             *
             * \param[in] name The attribute name.
             *
             * \param[in] ok   An optional pointer to a boolean value that will be set to true on success, set to false
             *                 on error.
             *
             * \return Returns the desired value. Note that 0 is returned on error.
             */
            std::int64_t signed64BitValue(const QString& name, bool* ok = nullptr) const;

            /**
             * Method you can use to obtain an attribute converted to a unsigned 16-bit value.
             *
             * \param[in] name The attribute name.
             *
             * \param[in] ok   An optional pointer to a boolean value that will be set to true on success, set to false
             *                 on error.
             *
             * \return Returns the desired value. Note that 0 is returned on error.
             */
            std::uint8_t unsigned8BitValue(const QString& name, bool* ok = nullptr) const;

            /**
             * Method you can use to obtain an attribute converted to a unsigned 16-bit value.
             *
             * \param[in] name The attribute name.
             *
             * \param[in] ok   An optional pointer to a boolean value that will be set to true on success, set to false
             *                 on error.
             *
             * \return Returns the desired value. Note that 0 is returned on error.
             */
            std::uint16_t unsigned16BitValue(const QString& name, bool* ok = nullptr) const;

            /**
             * Method you can use to obtain an attribute converted to a unsigned 32-bit value.
             *
             * \param[in] name The attribute name.
             *
             * \param[in] ok   An optional pointer to a boolean value that will be set to true on success, set to false
             *                 on error.
             *
             * \return Returns the desired value. Note that 0 is returned on error.
             */
            std::uint32_t unsigned32BitValue(const QString& name, bool* ok = nullptr) const;

            /**
             * Method you can use to obtain an attribute converted to a unsigned 64-bit value.
             *
             * \param[in] name The attribute name.
             *
             * \param[in] ok   An optional pointer to a boolean value that will be set to true on success, set to false
             *                 on error.
             *
             * \return Returns the desired value. Note that 0 is returned on error.
             */
            std::uint64_t unsigned64BitValue(const QString& name, bool* ok = nullptr) const;

            /**
             * Method you can use to obtain an attribute converted to handle.
             *
             * \param[in] name The attribute name.
             *
             * \param[in] ok   An optional pointer to a boolean value that will be set to true on success, set to false
             *                 on error.
             *
             * \return Returns the desired value. Note that 0 is returned on error.
             */
            bool boolValue(const QString& name, bool* ok = nullptr) const;

            /**
             * Method you can use to obtain an attribute converted to a floating point value.
             *
             * \param[in] name The attribute name.
             *
             * \param[in] ok   An optional pointer to a boolean value that will be set to true on success, set to false
             *                 on error.
             *
             * \return Returns the desired value. Note that 0 is returned on error.
             */
            float floatValue(const QString& name, bool* ok = nullptr) const;

            /**
             * Method you can use to obtain an attribute converted to a double value.
             *
             * \param[in] name The attribute name.
             *
             * \param[in] ok   An optional pointer to a boolean value that will be set to true on success, set to false
             *                 on error.
             *
             * \return Returns the desired value. Note that 0 is returned on error.
             */
            double doubleValue(const QString& name, bool* ok = nullptr) const;

            /**
             * Method you can use to obtain an attribute converted to a handle.
             *
             * \param[in] name The attribute name.
             *
             * \param[in] ok   An optional pointer to a boolean value that will be set to true on success, set to false
             *                 on error.
             *
             * \return Returns the desired value. An invalid handle is returned if an error is detected.
             */
            Handle handleValue(const QString& name, bool* ok = nullptr) const;

            /**
             * Method you can use to obtain an attribute converted to a color.
             *
             * \param[in] name The attribute name.
             *
             * \param[in] ok   An optional pointer to a boolean value that will be set to true on success, set to false
             *                 on error.
             *
             * \return Returns the desired value. An invalid handle is returned if an error is detected.
             */
            QColor colorValue(const QString& name, bool* ok = nullptr) const;

            /**
             * Method you can use to obtain an attribute converted to a string value.
             *
             * \param[in] name The attribute name.
             *
             * \param[in] ok   An optional pointer to a boolean value that will be set to true on success, set to false
             *                 on error.
             *
             * \return Returns the desired value.
             */
            QString stringValue(const QString& name, bool* ok = nullptr) const;

            /**
             * Convenience template method you can use to obtain an attribute converted to an arbitrary value.
             *
             * \param[in] name The attribute name.
             *
             * \param[in] ok   An optional pointer to a boolean value that will be set to true on success, set to false
             *                 on error.
             *
             * \return Returns the desired value.
             */
            template<typename T> T value(const QString& name, bool* ok = nullptr) const {
                T result;

                // Note that are careful examination of ISO 14882 will show that basic types such as char, int, long,
                // and long long are not necessarily identical to the types defined in cstdint.  The basic types allow
                // for more flexibility on the maximum size of the values.
                //
                // This distinction causes the tests against std::int*_t and std::uint*_t types to fail with some basic
                // types on at least some versions of the Apple CLang compiler.  We work around this by having
                // additional tests for the basic types.

                if (std::is_same<T, std::int8_t>::value) {
                    result = signed8BitValue(name, ok);
                } else if (std::is_same<T, std::int16_t>::value) {
                    result = signed16BitValue(name, ok);
                } else if (std::is_same<T, std::int32_t>::value) {
                    result = signed32BitValue(name, ok);
                } else if (std::is_same<T, std::int64_t>::value) {
                    result = signed64BitValue(name, ok);
                } else if (std::is_same<T, std::uint8_t>::value) {
                    result = unsigned8BitValue(name, ok);
                } else if (std::is_same<T, std::uint16_t>::value) {
                    result = unsigned16BitValue(name, ok);
                } else if (std::is_same<T, std::uint32_t>::value) {
                    result = unsigned32BitValue(name, ok);
                } else if (std::is_same<T, std::uint64_t>::value) {
                    result = unsigned64BitValue(name, ok);
                } else if (std::is_same<T, signed char>::value) {
                    if (sizeof(signed char) == 1) {
                        result = signed8BitValue(name, ok);
                    } else if (sizeof(signed char) == 2) {
                        result = signed16BitValue(name, ok);
                    } else if (sizeof(signed char) == 4) {
                        result = signed32BitValue(name, ok);
                    } else if (sizeof(signed char) == 8) {
                        result = signed64BitValue(name, ok);
                    } else {
                        assert(false);
                        result = T();
                    }
                } else if (std::is_same<T, signed short>::value) {
                    if (sizeof(signed short) == 2) {
                        result = signed16BitValue(name, ok);
                    } else if (sizeof(signed short) == 4) {
                        result = signed32BitValue(name, ok);
                    } else if (sizeof(signed short) == 8) {
                        result = signed64BitValue(name, ok);
                    } else {
                        assert(false);
                        result = T();
                    }
                } else if (std::is_same<T, signed long>::value) {
                    if (sizeof(signed long) == 4) {
                        result = signed32BitValue(name, ok);
                    } else if (sizeof(signed long) == 8) {
                        result = signed64BitValue(name, ok);
                    } else {
                        assert(false);
                        result = T();
                    }
                } else if (std::is_same<T, signed long long>::value) {
                    if (sizeof(signed long long) == 8) {
                        result = signed64BitValue(name, ok);
                    } else {
                        assert(false);
                        result = T();
                    }
                } else if (std::is_same<T, unsigned char>::value) {
                    if (sizeof(unsigned char) == 1) {
                        result = unsigned8BitValue(name, ok);
                    } else if (sizeof(unsigned char) == 2) {
                        result = unsigned16BitValue(name, ok);
                    } else if (sizeof(unsigned char) == 4) {
                        result = unsigned32BitValue(name, ok);
                    } else if (sizeof(unsigned char) == 8) {
                        result = unsigned64BitValue(name, ok);
                    } else {
                        assert(false);
                        result = T();
                    }
                } else if (std::is_same<T, unsigned short>::value) {
                    if (sizeof(unsigned short) == 2) {
                        result = unsigned16BitValue(name, ok);
                    } else if (sizeof(unsigned short) == 4) {
                        result = unsigned32BitValue(name, ok);
                    } else if (sizeof(unsigned short) == 8) {
                        result = unsigned64BitValue(name, ok);
                    } else {
                        assert(false);
                        result = T();
                    }
                } else if (std::is_same<T, unsigned long>::value) {
                    if (sizeof(unsigned long) == 4) {
                        result = unsigned32BitValue(name, ok);
                    } else if (sizeof(unsigned long) == 8) {
                        result = unsigned64BitValue(name, ok);
                    } else {
                        assert(false);
                        result = T();
                    }
                } else if (std::is_same<T, unsigned long long>::value) {
                    if (sizeof(unsigned long long) == 8) {
                        result = unsigned64BitValue(name, ok);
                    } else {
                        assert(false);
                        result = T();
                    }
                } else if (std::is_same<T, float>::value) {
                    result = floatValue(name, ok);
                } else if (std::is_same<T, double>::value) {
                    result = doubleValue(name, ok);
                } else if (std::is_same<T, bool>::value) {
                    result = boolValue(name, ok);
                } else {
                    assert(false);
                    result = T();
                }

                return result;
            }

            /**
             * Convenience template method you can use to obtain an attribute converted to an arbitrary value.
             *
             * \param[in] name The attribute name.
             *
             * \param[in] ok   An optional pointer to a boolean value that will be set to true on success, set to false
             *                 on error.
             *
             * \return Returns the desired value.
             */
            template<typename T> T value(const char* name, bool* ok = nullptr) const {
                return value<T>(QString(name), ok);
            }

            /**
             * Convenience template method you can use to obtain an attribute converted to an arbitrary value.  This
             * version will also support a default value if the attribute is not present.
             *
             * \param[in] defaultValue The default value.
             *
             * \param[in] name         The attribute name.
             *
             * \param[in] ok           An optional pointer to a boolean value that will be set to true on success, set
             *                         to false on error.  Note that a missing attribute will not be treated as an
             *                         error.
             *
             * \return Returns the desired value.
             */
            template<typename T> T value(const QString& name, const T& defaultValue, bool* ok = nullptr) const {
                if (hasAttribute(name)) {
                    return value<T>(name, ok);
                } else {
                    if (ok != nullptr) {
                        *ok = true;
                    }

                    return defaultValue;
                }
            }

            /**
             * Convenience template method you can use to obtain an attribute converted to an arbitrary value.  This
             * version will also support a default value if the attribute is not present.
             *
             * \param[in] defaultValue The default value.
             *
             * \param[in] name         The attribute name.
             *
             * \param[in] ok           An optional pointer to a boolean value that will be set to true on success, set
             *                         to false on error.  Note that a missing attribute will not be treated as an
             *                         error.
             *
             * \return Returns the desired value.
             */
            template<typename T> T value(const char* name, const T& defaultValue, bool* ok = nullptr) const {
                return value<T>(QString(name), defaultValue, ok);
            }

            /**
             * Assignment operator
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            XmlAttributes& operator=(const XmlAttributes& other) {
                QXmlStreamAttributes::operator=(other);
                return *this;
            }

            /**
             * Assignment operator (move semantics)
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            XmlAttributes& operator=(XmlAttributes&& other) {
                QXmlStreamAttributes::operator=(other);
                return *this;
            }
    };

    /**
     * Convenience template method you can use to obtain an attribute converted to an arbitrary value.
     *
     * \param[in] name The attribute name.
     *
     * \param[in] ok   An optional pointer to a boolean value that will be set to true on success, set to false on
     *                 error.
     *
     * \return Returns the desired value.
     */
    template<> LD_PUBLIC_API QString XmlAttributes::value<QString>(const QString& name, bool* ok) const;

    /**
     * Convenience template method you can use to obtain an attribute converted to an arbitrary value.
     *
     * \param[in] name The attribute name.
     *
     * \param[in] ok   An optional pointer to a boolean value that will be set to true on success, set to false on
     *                 error.
     *
     * \return Returns the desired value.
     */
    template<> LD_PUBLIC_API std::string XmlAttributes::value<std::string>(const QString& name, bool* ok) const;

    /**
     * Convenience template method you can use to obtain an attribute converted to an arbitrary value.
     *
     * \param[in] name The attribute name.
     *
     * \param[in] ok   An optional pointer to a boolean value that will be set to true on success, set to false on
     *                 error.
     *
     * \return Returns the desired value.
     */
    template<> LD_PUBLIC_API const char* XmlAttributes::value<const char*>(const QString& name, bool* ok) const;

    /**
     * Convenience template method you can use to obtain an attribute converted to an arbitrary value.
     *
     * \param[in] name The attribute name.
     *
     * \param[in] ok   An optional pointer to a boolean value that will be set to true on success, set to false on
     *                 error.
     *
     * \return Returns the desired value.
     */
    template<> LD_PUBLIC_API Handle XmlAttributes::value<Handle>(const QString& name, bool* ok) const;
};

#endif
