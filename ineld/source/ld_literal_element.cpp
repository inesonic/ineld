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
* This file implements the \ref Ld::LiteralElement class.
***********************************************************************************************************************/

#include <QList>
#include <QSharedPointer>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QLocale>
#include <QList>

#include <QDebug>

#include <cassert>
#include <string>

#include <model_intrinsic_types.h>
#include <model_complex.h>
#include <model_tuple.h>
#include <model_variant.h>

#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_data_type.h"
#include "ld_program_file.h"
#include "ld_format_organizer.h"
#include "ld_visual_with_no_children.h"
#include "ld_format.h"
#include "ld_format_container.h"
#include "ld_character_format.h"
#include "ld_capabilities.h"
#include "ld_xml_writer.h"
#include "ld_xml_attributes.h"
#include "ld_program_file.h"
#include "ld_element_cursor.h"
#include "ld_cursor.h"
#include "ld_cursor_state_collection.h"
#include "ld_element_with_no_children.h"
#include "ld_root_element.h"
#include "ld_literal_visual.h"
#include "ld_literal_element.h"

/**********************************************************************************************************************
 * Ld::LiteralElement::SectionType
 */

namespace Ld {
    LiteralElement::Section::Section() {
        currentSectionType = SectionType::INVALID;
    }


    LiteralElement::Section::Section(
            const QString&              text,
            LiteralElement::SectionType sectionType
        ):QString(
            text
        ),currentSectionType(
            sectionType
        ) {}


    LiteralElement::Section::Section(
            const LiteralElement::Section& other
        ):QString(
            other
        ),currentSectionType(
            other.currentSectionType
        ) {}


    LiteralElement::Section::Section(
            const LiteralElement::Section&& other
        ):QString(
            other
        ),currentSectionType(
            other.currentSectionType
        ) {}


    LiteralElement::Section::~Section() {}


    LiteralElement::SectionType LiteralElement::Section::sectionType() const {
        return currentSectionType;
    }


    LiteralElement::Section& LiteralElement::Section::operator=(const LiteralElement::Section& other) {
        QString::operator=(other);
        currentSectionType = other.currentSectionType;

        return *this;
    }
}

/**********************************************************************************************************************
 * Ld::LiteralElement
 */

namespace Ld {
    const QString      LiteralElement::elementName("Literal");
    const Capabilities LiteralElement::childProvides = Capabilities::numericLiterals;
    QRegularExpression LiteralElement::integerRegex;
    QRegularExpression LiteralElement::realRegex;
    QRegularExpression LiteralElement::complexRegex;

    LiteralElement::LiteralElement() {}


    LiteralElement::~LiteralElement() {}


    Element* LiteralElement::creator(const QString&) {
        return new LiteralElement();
    }


    QString LiteralElement::typeName() const {
        return elementName;
    }


    QString LiteralElement::plugInName() const {
        return QString();
    }


    QString LiteralElement::description() const {
        return tr("numeric literal");
    }


    DataType::ValueType LiteralElement::valueType() const {
        DataType::ValueType result;

        if (!currentText.isEmpty()) {
            QChar firstCharacter = currentText.front();
            if (firstCharacter == QChar('<') || firstCharacter == QChar('"') || firstCharacter == QChar('\'')) {
                result = DataType::ValueType::TUPLE;
            } else {
                prepareIntegerRegex();

                QRegularExpressionMatch integerMatch = integerRegex.match(currentText);
                if (integerMatch.hasMatch()) {
                    result = DataType::ValueType::INTEGER;
                } else {
                    prepareRealRegex();

                    QRegularExpressionMatch realMatch = realRegex.match(currentText);
                    if (realMatch.hasMatch()) {
                        result = DataType::ValueType::REAL;
                    } else {
                        prepareComplexRegex();

                        QRegularExpressionMatch complexMatch = complexRegex.match(currentText);
                        if (complexMatch.hasMatch()) {
                            result = DataType::ValueType::COMPLEX;
                        } else {
                            result = DataType::ValueType::NONE;
                        }
                    }
                }
            }
        } else {
            result = DataType::ValueType::NONE;
        }

        return result;
    }


    Capabilities LiteralElement::childProvidesCapabilities() const {
        return childProvides;
    }


    Element::Precedence LiteralElement::childPrecedence() const {
        Precedence result;

        prepareIntegerRegex();

        QRegularExpressionMatch integerMatch = integerRegex.match(currentText);
        if (integerMatch.hasMatch()) {
            result = simpleLiteralPrecedence;
        } else {
            prepareRealRegex();

            QRegularExpressionMatch realMatch = realRegex.match(currentText);
            if (realMatch.hasMatch()) {
                result = simpleLiteralPrecedence;
            } else {
                prepareComplexRegex();

                QRegularExpressionMatch complexMatch = complexRegex.match(currentText);
                if (complexMatch.hasMatch()) {
                    QString realString      = complexMatch.captured("real2") + complexMatch.captured("real3");
                    QString imaginaryString = (
                          complexMatch.captured("imag1")
                        + complexMatch.captured("imag2")
                        + complexMatch.captured("imag3")
                    );

                    if (realString.isEmpty() || imaginaryString.isEmpty()) {
                        result = simpleLiteralPrecedence;
                    } else {
                        result = complexLiteralPrecedence;
                    }
                } else {
                    result = simpleLiteralPrecedence;
                }
            }
        }

        return result;
    }


    Capabilities LiteralElement::textCapabilities(unsigned long textOffset, unsigned regionIndex) const {
        Capabilities result;

        if (regionIndex == 0) {
            if (currentText.isEmpty()) {
                result = (
                        Capabilities::stringIdentifierSymbols
                      | Capabilities::numericDecimalSymbols
                      | Capabilities::numericImaginaryUnitSymbols
                      | Capabilities::numericSignSymbols
                      | Capabilities::numericDecimalPointSymbols
                );
            } else {
                QChar firstCharacter = currentText.at(0);
                if (firstCharacter == QChar('<') || firstCharacter == QChar('\'') || firstCharacter == QChar('"')) {
                    result = Capabilities::allSymbols;
                } else {
                    QString preText  = currentText.left(textOffset);
                    QString postText = currentText.mid(textOffset);

                    QLocale locale   = QLocale::system();
                    QString decimal  = locale.decimalPoint();
                    QString exponent = locale.exponential();

                    if (currentText.contains(tr("i")) || currentText.contains(tr("j"))) {
                        result |= Capabilities::numericDecimalSymbols;

                        if (canConvert(preText + decimal + postText)) {
                            result |= Capabilities::numericDecimalPointSymbols;
                        }

                        if (canConvert(preText + tr("-1") + postText)) {
                            result |= Capabilities::numericSignSymbols;
                        }

                        if (canConvert(preText + decimal + postText)) {
                            result |= Capabilities::numericDecimalPointSymbols;
                        }

                        if (canConvert(preText + exponent + tr("0") + postText)) {
                            result |= Capabilities::numericExponentialSymbols;
                        }
                    } else {
                        QString basePrefix = currentText.length() >= 2 ? currentText.left(2) : QString();
                        if (basePrefix == tr("0x")) {
                            result |= Capabilities::numericHexidecimalSymbols;
                        } else if (basePrefix == tr("0b")) {
                            result |= Capabilities::numericBinarySymbols;
                        } else {
                            result |= Capabilities::numericDecimalSymbols;

                            if (currentText.left(1) == tr("0") && textOffset == 1) {
                                if (canConvert(preText + tr("x0") + postText)) {
                                    result |= Capabilities::numericHexidecimalBaseIdentifierSymbols;
                                }

                                if (canConvert(preText + tr("0b") + postText)) {
                                    result |= Capabilities::numericBinaryBaseIdentifierSymbols;
                                }
                            }

                            if (canConvert(preText + tr("i") + postText)) {
                                result |= Capabilities::numericImaginaryUnitSymbols;
                            }

                            QString imaginaryPostText = postText + tr("0i");
                            if (canConvert(preText + tr("-1") + imaginaryPostText)) {
                                result |= Capabilities::numericSignSymbols;
                            }

                            if (canConvert(preText + decimal + tr("1") + imaginaryPostText)) {
                                result |= Capabilities::numericDecimalPointSymbols;
                            }

                            if (canConvert(preText + exponent + imaginaryPostText)) {
                                result |= Capabilities::numericExponentialSymbols;
                            }
                        }
                    }
                }
            }
        }

        return result;
    }


    void LiteralElement::setVisual(LiteralVisual *newVisual) {
        ElementWithNoChildren::setVisual(newVisual);
    }


    LiteralVisual* LiteralElement::visual() const {
        return dynamic_cast<LiteralVisual*>(ElementWithNoChildren::visual());
    }


    unsigned LiteralElement::minimumNumberTextRegions() const {
        return 1;
    };


    unsigned LiteralElement::maximumNumberTextRegions() const {
        return 1;
    }


    bool LiteralElement::setNumberTextRegions(unsigned newNumberTextRegions) {
        return newNumberTextRegions == 1 ? true : false;
    }


    unsigned LiteralElement::numberTextRegions() const {
        return 1;
    }


    bool LiteralElement::setText(const QString& newText, unsigned regionNumber) {
        bool success;
        if (regionNumber <= 0) {
            success = true;

            if (newText != currentText) {
                currentText = newText;

                LiteralVisual* literalVisual = visual();
                if (literalVisual != nullptr) {
                    elementDataChanged();
                    literalVisual->textChanged(newText, regionNumber);
                }
            }
        } else {
            success = false;
        }

        return success;
    }


    QString LiteralElement::text(unsigned regionNumber) const {
        return regionNumber <= 0 ? currentText : QString();
    }


    Element::CursorParkMode LiteralElement::cursorParkMode() const {
        return CursorParkMode::PARK_ON_TEXT_AND_ELEMENT;
    }


    Model::Variant LiteralElement::convert(const QString& text) {
        Model::Variant result;

        if (!text.isEmpty()) {
            QChar firstCharacter = text.at(0);
            if (firstCharacter == QChar('<') || firstCharacter == QChar('"') || firstCharacter == QChar('\'')) {
                QString massaged = text.mid(1);
                if (!massaged.isEmpty()) {
                    QString endingTerminator = firstCharacter == QChar('<') ? QChar('>') : firstCharacter;
                    if (massaged.back() == endingTerminator) {
                        massaged.chop(1);
                    }
                }

                result = Model::Tuple(massaged.toUtf8().data());
            } else {
                prepareIntegerRegex();

                QRegularExpressionMatch integerMatch = integerRegex.match(text);
                if (integerMatch.hasMatch()) {
                    unsigned base;
                    QString  valueString;

                    if (text.length() > 2) {
                        QString baseString = text.left(2);
                        if (baseString == tr("0b")) {
                            base        = 2;
                            valueString = text.mid(2);
                        } else if (baseString == tr("0x")) {
                            base        = 16;
                            valueString = text.mid(2);
                        } else {
                            base        = 10;
                            valueString = text;
                        }
                    } else {
                        base        = 10;
                        valueString = text;
                    }

                    bool           ok           = false;
                    Model::Integer integerValue = valueString.toLongLong(&ok, base);

                    if (ok) {
                        result = integerValue;
                    }
                } else {
                    prepareRealRegex();

                    QRegularExpressionMatch realMatch = realRegex.match(text);
                    if (realMatch.hasMatch()) {
                        bool ok;
                        Model::Real realValue = toLongDouble(text, &ok);

                        if (ok) {
                            result = realValue;
                        }
                    } else {
                        prepareComplexRegex();

                        QRegularExpressionMatch complexMatch = complexRegex.match(text);
                        if (complexMatch.hasMatch()) {
                            QString realString      = complexMatch.captured("real2") + complexMatch.captured("real3");
                            QString imaginaryString = (
                                  complexMatch.captured("imag1")
                                + complexMatch.captured("imag2")
                                + complexMatch.captured("imag3")
                            );

                            Model::Real realValue      = Model::Real(0);
                            Model::Real imaginaryValue = Model::Real(0);
                            bool        ok             = true;

                            if (!realString.isEmpty()) {
                                realValue = toLongDouble(realString, &ok);
                            }

                            if (ok && !imaginaryString.isEmpty()) {
                                imaginaryValue = toLongDouble(imaginaryString, &ok);
                            }

                            if (ok) {
                                result = Model::Complex(realValue, imaginaryValue);
                            }
                        }
                    }
                }
            }
        }

        return result;
    }


    bool LiteralElement::canConvert(const QString& text) {
        bool result = false;

        if (!text.isEmpty()) {
            QChar firstCharacter = text.at(0);
            if (firstCharacter == QChar('<') || firstCharacter == QChar('"') || firstCharacter == QChar('\'')) {
                result = true;
            } else {
                prepareIntegerRegex();

                QRegularExpressionMatch integerMatch = integerRegex.match(text);
                if (integerMatch.hasMatch()) {
                    result = true;
                } else {
                    prepareRealRegex();

                    QRegularExpressionMatch realMatch = realRegex.match(text);
                    if (realMatch.hasMatch()) {
                        result = true;
                    } else {
                        prepareComplexRegex();

                        QRegularExpressionMatch complexMatch = complexRegex.match(text);
                        if (complexMatch.hasMatch()) {
                            result = true;
                        }
                    }
                }
            }
        }

        return result;
    }


    LiteralElement::SectionList LiteralElement::section(const QString& text) {
        SectionList result;

        if (!text.isEmpty()) {
            QChar firstCharacter = text.at(0);
            if (firstCharacter == QChar('<') || firstCharacter == QChar('"') || firstCharacter == QChar('\'')) {
                result.append(Section(text, SectionType::STRING_CONTENT));
            } else {
                QLocale locale   = QLocale::system();
                QString exponent = locale.exponential();

                unsigned index      = 0;
                unsigned length     = static_cast<unsigned>(text.length());
                bool     inExponent = false;
                QString  s;
                while (index < length) {
                    QString c = text.at(index);

                    if (inExponent) {
                        if (c == tr("+") || c == tr("-")) {
                            if (!s.isEmpty()) {
                                result.append(Section(s, SectionType::EXPONENT_VALUE));
                                s.clear();

                                result.append(Section(c, SectionType::MANTISSA_SIGN));

                                inExponent = false;
                            } else {
                                result.append(Section(c, SectionType::EXPONENT_SIGN));
                            }
                        } else if (c == tr("i") || c == tr("j")) {
                            if (!s.isEmpty()) {
                                result.append(Section(s, SectionType::EXPONENT_VALUE));
                                s.clear();
                            }

                            result.append(Section(c, SectionType::IMAGINARY_SYMBOL));
                            inExponent = false;
                        } else {
                            s += c;
                        }
                    } else {
                        if (c.toLower() == exponent.toLower()) {
                            if (text.length() > 2 && text.left(2) == tr("0x")) {
                                s += c;
                            } else {
                                if (!s.isEmpty()) {
                                    result.append(Section(s, SectionType::MANTISSA_VALUE));
                                    s.clear();
                                }

                                result.append(Section(c, SectionType::EXPONENT_SYMBOL));
                                inExponent = true;
                            }
                        } else if (c == tr("+") || c == tr("-")) {
                            if (!s.isEmpty()) {
                                result.append(Section(s, SectionType::MANTISSA_VALUE));
                                s.clear();
                            }

                            result.append(Section(c, SectionType::MANTISSA_SIGN));
                        } else if (c == tr("i") || c == tr("j")) {
                            if (!s.isEmpty()) {
                                result.append(Section(s, SectionType::MANTISSA_VALUE));
                                s.clear();
                            }

                            result.append(Section(c, SectionType::IMAGINARY_SYMBOL));
                        } else {
                            s += c;
                        }
                    }

                    ++index;
                }

                if (!s.isEmpty()) {
                    result.append(Section(s, inExponent ? SectionType::EXPONENT_VALUE : SectionType::MANTISSA_VALUE));
                }
            }
        }


        return result;
    }


    Model::Variant LiteralElement::convert() const {
        return convert(currentText);
    }


    LiteralElement::SectionList LiteralElement::section() const {
        return section(currentText);
    }


    void LiteralElement::updateAfterGraft() {
        if (format().isNull()) {
            ElementPointer rootElement = root();
            assert(!rootElement.isNull());

            if (rootElement->typeName() == Ld::RootElement::elementName) {
                QSharedPointer<Ld::RootElement> root = rootElement.dynamicCast<Ld::RootElement>();
                assert(!root.isNull());

                setFormat(root->defaultMathIdentifierFormat());
            }
        }
    }


    QString LiteralElement::writeData(QSharedPointer<const FormatOrganizer>, ProgramFile&) const {
        return currentText;
    }


    void LiteralElement::readData(
            QSharedPointer<XmlReader>,
            const QString&             text,
            const FormatsByIdentifier&,
            ProgramFile&,
            unsigned
        ) {
        currentText = text;
    }


    QString LiteralElement::realRegularExpressionPatternString(bool includeIntegerValues) {
        QString result;

        QLocale locale   = QLocale::system();
        QString decimal  = locale.decimalPoint(); // %1
        QString exponent = locale.exponential();  // %2

        if (includeIntegerValues) {
            result = tr(
                "(?:" "[0-9]+[%1][0-9]*(?:[%2][+-]?[0-9]+)?"
                "|"   "[%1][0-9]+(?:[%2][+-]?[0-9]+)"
                "|"   "[0-9]+[%2][+-]?[0-9]+"
                "|"   "[0-9]+"
                ")"
            ).arg(decimal).arg(exponent.toLower() + exponent.toUpper());
        } else {
            result = tr(
                "(?:" "[0-9]+[%1][0-9]*(?:[%2][+-]?[0-9]+)?"
                "|"   "[%1][0-9]+(?:[%2][+-]?[0-9]+)"
                "|"   "[0-9]+[%2][+-]?[0-9]+"
                ")"
            ).arg(decimal).arg(exponent.toLower() + exponent.toUpper());
        }

        return result;
    }


    long double LiteralElement::toLongDouble(const QString& text, bool* ok) {
        long double result = false;
        bool        isOk   = false;

        std::string stringValue = text.toStdString();
        std::size_t position;

        try {
            result = std::stold(stringValue, &position);
            isOk   = (position == stringValue.length());
        } catch (...) {
            isOk = false;
        }

        if (ok != nullptr) {
            *ok = isOk;
        }

        return result;
    }


    void LiteralElement::prepareIntegerRegex() {
        if (integerRegex.pattern().isEmpty()) {
            integerRegex.setPattern(tr("^(?:[+-]?[0-9]+|0x[0-9a-fA-F]+|0b[01]+)$"));
        }
    }


    void LiteralElement::prepareRealRegex() {
        if (realRegex.pattern().isEmpty()) {
            realRegex.setPattern(tr("^[+-]?%1$").arg(realRegularExpressionPatternString()));
            assert(realRegex.isValid());
        }
    }


    void LiteralElement::prepareComplexRegex() {
        if (complexRegex.pattern().isEmpty()) {
            QString numberPatternString = realRegularExpressionPatternString(true);

            QString patternString = tr(
                "^(?:" "(?<imag1>[+-]?%1)[ij]"
                "|"    "(?<real2>[+-]?%1)(?<imag2>[+-]%1)[ij]"
                "|"    "(?<imag3>[+-]?%1)[ij](?<real3>[+-]%1)"
                ")$"
            ).arg(numberPatternString);

            complexRegex.setPattern(patternString);
            assert(complexRegex.isValid());
        }
    }
}
