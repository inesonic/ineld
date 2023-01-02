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
* This file implements tests of the Ld::LiteralElement class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QtTest/QtTest>

#include <cmath>

#include <model_intrinsic_types.h>
#include <model_complex.h>
#include <model_tuple.h>
#include <model_variant.h>

#include <ld_element_structures.h>
#include <ld_element_cursor.h>
#include <ld_cursor.h>
#include <ld_cursor_state_collection.h>
#include <ld_root_element.h>
#include <ld_plug_in_manager.h>
#include <ld_character_format.h>
#include <ld_page_format.h>
#include <ld_literal_visual.h>
#include <ld_literal_element.h>

#include "test_literal_element.h"

/***********************************************************************************************************************
 * TestLiteralElement:
 */

TestLiteralElement::TestLiteralElement() {}


TestLiteralElement::~TestLiteralElement() {}


void TestLiteralElement::initTestCase() {
    Ld::Element::registerCreator("Root", Ld::RootElement::creator);
    Ld::Element::registerCreator("Text", Ld::LiteralElement::creator);
    Ld::Format::registerCreator("CharacterFormat", Ld::CharacterFormat::creator);
    Ld::Format::registerCreator("PageFormat", Ld::PageFormat::creator);
}


void TestLiteralElement::testTypeName() {
    QSharedPointer<Ld::LiteralElement> element(new Ld::LiteralElement());
    element->setWeakThis(element.toWeakRef());

    QVERIFY(element->typeName() == "Literal");
    QVERIFY(element->plugInName().isEmpty());
}


void TestLiteralElement::testSaveLoadMethods() {
//    QSharedPointer<Ld::RootElement> rootElement1(new Ld::RootElement);
//    rootElement1->setWeakThis(rootElement1.toWeakRef());

//    bool success = rootElement1->openNew();
//    QVERIFY(success);

//    QSharedPointer<Ld::LiteralElement> textElement1(new Ld::LiteralElement);
//    textElement1->setWeakThis(textElement1.toWeakRef());

//    textElement1->setText("Some text to be saved !");
//    textElement1->setFormat(new Ld::CharacterFormat("Courier", 10));

//    rootElement1->append(textElement1, nullptr);

//    success = rootElement1->saveAs("test_container.dat");
//    QVERIFY(success);

//    success = rootElement1->close();
//    QVERIFY(success);

//    QSharedPointer<Ld::RootElement> rootElement2(new Ld::RootElement);
//    rootElement2->setWeakThis(rootElement2.toWeakRef());

//    Ld::PlugInsByName plugInsByName;
//    success = rootElement2->openExisting("test_container.dat", true, plugInsByName);
//    QVERIFY(success);

//    QVERIFY(rootElement2->numberChildren() == 1);
//    Ld::ElementPointer childElement = rootElement2->child(0);
//    QVERIFY(childElement->typeName() == QString("Text"));

//    QSharedPointer<Ld::LiteralElement> textElement2 = childElement.dynamicCast<Ld::LiteralElement>();
//    QVERIFY(!textElement2.isNull());

//    QVERIFY(textElement2->text() == QString("Some text to be saved !"));

//    QSharedPointer<Ld::CharacterFormat> format = textElement2->format().dynamicCast<Ld::CharacterFormat>();
//    QVERIFY(!format.isNull());

//    QVERIFY(format->family() == QString("Courier"));
//    QVERIFY(format->fontSize() == 10);

//    success = rootElement2->close();
//    QVERIFY(success);
}


void TestLiteralElement::testConversionMethods() {
    // Integers

    Model::Variant v = Ld::LiteralElement::convert(QString("123"));
    QCOMPARE(v.valueType(), Model::ValueType::INTEGER);
    QCOMPARE(v.toInteger(), Model::Integer(123));

    v = Ld::LiteralElement::convert(QString("-321"));
    QCOMPARE(v.valueType(), Model::ValueType::INTEGER);
    QCOMPARE(v.toInteger(), Model::Integer(-321));

    v = Ld::LiteralElement::convert(QString("0b100100011"));
    QCOMPARE(v.valueType(), Model::ValueType::INTEGER);
    QCOMPARE(v.toInteger(), Model::Integer(0x123));

    v = Ld::LiteralElement::convert(QString("0x123456789ABCDEF"));
    QCOMPARE(v.valueType(), Model::ValueType::INTEGER);
    QCOMPARE(v.toInteger(), Model::Integer(0x123456789ABCDEFULL));

    // Real values

    v = Ld::LiteralElement::convert(QString("123."));
    QCOMPARE(v.valueType(), Model::ValueType::REAL);
    QCOMPARE(v.toReal(), Model::Real(123));

    v = Ld::LiteralElement::convert(QString("1.23"));
    QCOMPARE(v.valueType(), Model::ValueType::REAL);
    QVERIFY(abs(v.toReal() - Model::Real(1.23))/Model::Real(1.23) < 1.0E-10);

    v = Ld::LiteralElement::convert(QString("-3.21"));
    QCOMPARE(v.valueType(), Model::ValueType::REAL);
    QVERIFY(abs(v.toReal() - Model::Real(-3.21))/Model::Real(3.21) < 1.0E-10);

    v = Ld::LiteralElement::convert(QString("123E4"));
    QCOMPARE(v.valueType(), Model::ValueType::REAL);
    QVERIFY(abs(v.toReal() - Model::Real(123E4))/Model::Real(123E4) < 1.0E-10);

    v = Ld::LiteralElement::convert(QString("123E-4"));
    QCOMPARE(v.valueType(), Model::ValueType::REAL);
    QVERIFY(abs(v.toReal() - Model::Real(123E-4))/Model::Real(123E-4) < 1.0E-10);

    v = Ld::LiteralElement::convert(QString("-3.21E4"));
    QCOMPARE(v.valueType(), Model::ValueType::REAL);
    QVERIFY(abs(v.toReal() - Model::Real(-3.21E4))/Model::Real(3.21E4) < 1.0E-10);

    v = Ld::LiteralElement::convert(QString("-3.21E-4"));
    QCOMPARE(v.valueType(), Model::ValueType::REAL);
    QVERIFY(abs(v.toReal() - Model::Real(-3.21E-4))/Model::Real(3.21E-4) < 1.0E-10);

    v = Ld::LiteralElement::convert(QString("1.23E4"));
    QCOMPARE(v.valueType(), Model::ValueType::REAL);
    QVERIFY(abs(v.toReal() - Model::Real(1.23E4))/Model::Real(1.23E4) < 1.0E-10);

    v = Ld::LiteralElement::convert(QString("1.23E-4"));
    QCOMPARE(v.valueType(), Model::ValueType::REAL);
    QVERIFY(abs(v.toReal() - Model::Real(1.23E-4))/Model::Real(1.23E-4) < 1.0E-10);

    v = Ld::LiteralElement::convert(QString("-3.21E4"));
    QCOMPARE(v.valueType(), Model::ValueType::REAL);
    QVERIFY(abs(v.toReal() - Model::Real(-3.21E4))/Model::Real(3.21E4) < 1.0E-10);

    v = Ld::LiteralElement::convert(QString("-3.21E-4"));
    QCOMPARE(v.valueType(), Model::ValueType::REAL);
    QVERIFY(abs(v.toReal() - Model::Real(-3.21E-4))/Model::Real(3.21E-4) < 1.0E-10);

    // Complex values

    v = Ld::LiteralElement::convert(QString("123+456i"));
    QCOMPARE(v.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(v.toComplex(), Model::Complex(123, 456));

    v = Ld::LiteralElement::convert(QString("+123-456i"));
    QCOMPARE(v.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(v.toComplex(), Model::Complex(123, -456));

    v = Ld::LiteralElement::convert(QString("-123-456i"));
    QCOMPARE(v.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(v.toComplex(), Model::Complex(-123, -456));

    v = Ld::LiteralElement::convert(QString("123i+456"));
    QCOMPARE(v.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(v.toComplex(), Model::Complex(456, 123));

    v = Ld::LiteralElement::convert(QString("+123i-456"));
    QCOMPARE(v.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(v.toComplex(), Model::Complex(-456, 123));

    v = Ld::LiteralElement::convert(QString("-123i-456"));
    QCOMPARE(v.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(v.toComplex(), Model::Complex(-456, -123));

    v = Ld::LiteralElement::convert(QString("123i"));
    QCOMPARE(v.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(v.toComplex(), Model::Complex(0, 123));

    v = Ld::LiteralElement::convert(QString("-123i"));
    QCOMPARE(v.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(v.toComplex(), Model::Complex(0, -123));

    v = Ld::LiteralElement::convert(QString("123+456j"));
    QCOMPARE(v.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(v.toComplex(), Model::Complex(123, 456));

    v = Ld::LiteralElement::convert(QString("+123-456j"));
    QCOMPARE(v.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(v.toComplex(), Model::Complex(123, -456));

    v = Ld::LiteralElement::convert(QString("-123-456j"));
    QCOMPARE(v.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(v.toComplex(), Model::Complex(-123, -456));

    v = Ld::LiteralElement::convert(QString("123j+456"));
    QCOMPARE(v.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(v.toComplex(), Model::Complex(456, 123));

    v = Ld::LiteralElement::convert(QString("+123j-456"));
    QCOMPARE(v.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(v.toComplex(), Model::Complex(-456, 123));

    v = Ld::LiteralElement::convert(QString("-123j-456"));
    QCOMPARE(v.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(v.toComplex(), Model::Complex(-456, -123));

    v = Ld::LiteralElement::convert(QString("123j"));
    QCOMPARE(v.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(v.toComplex(), Model::Complex(0, 123));

    v = Ld::LiteralElement::convert(QString("-123j"));
    QCOMPARE(v.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(v.toComplex(), Model::Complex(0, -123));

    // Note that we use simple powers of 2 in the tests below so that we end up with no round-off errors.

    v = Ld::LiteralElement::convert(QString("1.25+4.75i"));
    QCOMPARE(v.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(v.toComplex(), Model::Complex(1.25, 4.75));

    // Strings

    v = Ld::LiteralElement::convert(QString("<foo"));
    QCOMPARE(v.valueType(), Model::ValueType::TUPLE);
    QCOMPARE(v.toTuple(), Model::Tuple("foo"));

    v = Ld::LiteralElement::convert(QString("\"bar"));
    QCOMPARE(v.valueType(), Model::ValueType::TUPLE);
    QCOMPARE(v.toTuple(), Model::Tuple("bar"));

    v = Ld::LiteralElement::convert(QString("\'bar"));
    QCOMPARE(v.valueType(), Model::ValueType::TUPLE);
    QCOMPARE(v.toTuple(), Model::Tuple("bar"));

    v = Ld::LiteralElement::convert(QString("<foo>"));
    QCOMPARE(v.valueType(), Model::ValueType::TUPLE);
    QCOMPARE(v.toTuple(), Model::Tuple("foo"));

    v = Ld::LiteralElement::convert(QString("\"bar\""));
    QCOMPARE(v.valueType(), Model::ValueType::TUPLE);
    QCOMPARE(v.toTuple(), Model::Tuple("bar"));

    v = Ld::LiteralElement::convert(QString("'bar'"));
    QCOMPARE(v.valueType(), Model::ValueType::TUPLE);
    QCOMPARE(v.toTuple(), Model::Tuple("bar"));

    v = Ld::LiteralElement::convert(QString("<foo\""));
    QCOMPARE(v.valueType(), Model::ValueType::TUPLE);
    QCOMPARE(v.toTuple(), Model::Tuple("foo\""));

    v = Ld::LiteralElement::convert(QString("\"bar>"));
    QCOMPARE(v.valueType(), Model::ValueType::TUPLE);
    QCOMPARE(v.toTuple(), Model::Tuple("bar>"));

    v = Ld::LiteralElement::convert(QString("'bar\""));
    QCOMPARE(v.valueType(), Model::ValueType::TUPLE);
    QCOMPARE(v.toTuple(), Model::Tuple("bar\""));
}


void TestLiteralElement::testSectionMethods() {
    Ld::LiteralElement::SectionList sections = Ld::LiteralElement::section("+1.05e-24+51i");

    QCOMPARE(sections.size(), 8);

    QCOMPARE(sections.at(0).sectionType(), Ld::LiteralElement::SectionType::MANTISSA_SIGN);
    QCOMPARE(sections.at(0), QString("+"));

    QCOMPARE(sections.at(1).sectionType(), Ld::LiteralElement::SectionType::MANTISSA_VALUE);
    QCOMPARE(sections.at(1), QString("1.05"));

    QCOMPARE(sections.at(2).sectionType(), Ld::LiteralElement::SectionType::EXPONENT_SYMBOL);
    QCOMPARE(sections.at(2), QString("e"));

    QCOMPARE(sections.at(3).sectionType(), Ld::LiteralElement::SectionType::EXPONENT_SIGN);
    QCOMPARE(sections.at(3), QString("-"));

    QCOMPARE(sections.at(4).sectionType(), Ld::LiteralElement::SectionType::EXPONENT_VALUE);
    QCOMPARE(sections.at(4), QString("24"));

    QCOMPARE(sections.at(5).sectionType(), Ld::LiteralElement::SectionType::MANTISSA_SIGN);
    QCOMPARE(sections.at(5), QString("+"));

    QCOMPARE(sections.at(6).sectionType(), Ld::LiteralElement::SectionType::MANTISSA_VALUE);
    QCOMPARE(sections.at(6), QString("51"));

    QCOMPARE(sections.at(7).sectionType(), Ld::LiteralElement::SectionType::IMAGINARY_SYMBOL);
    QCOMPARE(sections.at(7), QString("i"));


    sections = Ld::LiteralElement::section("2E+3j-16");

    QCOMPARE(sections.size(), 7);

    QCOMPARE(sections.at(0).sectionType(), Ld::LiteralElement::SectionType::MANTISSA_VALUE);
    QCOMPARE(sections.at(0), QString("2"));

    QCOMPARE(sections.at(1).sectionType(), Ld::LiteralElement::SectionType::EXPONENT_SYMBOL);
    QCOMPARE(sections.at(1), QString("E"));

    QCOMPARE(sections.at(2).sectionType(), Ld::LiteralElement::SectionType::EXPONENT_SIGN);
    QCOMPARE(sections.at(2), QString("+"));

    QCOMPARE(sections.at(3).sectionType(), Ld::LiteralElement::SectionType::EXPONENT_VALUE);
    QCOMPARE(sections.at(3), QString("3"));

    QCOMPARE(sections.at(4).sectionType(), Ld::LiteralElement::SectionType::IMAGINARY_SYMBOL);
    QCOMPARE(sections.at(4), QString("j"));

    QCOMPARE(sections.at(5).sectionType(), Ld::LiteralElement::SectionType::MANTISSA_SIGN);
    QCOMPARE(sections.at(5), QString("-"));

    QCOMPARE(sections.at(6).sectionType(), Ld::LiteralElement::SectionType::MANTISSA_VALUE);
    QCOMPARE(sections.at(6), QString("16"));


    sections = Ld::LiteralElement::section("0x1e54");

    QCOMPARE(sections.size(), 1);

    QCOMPARE(sections.at(0).sectionType(), Ld::LiteralElement::SectionType::MANTISSA_VALUE);
    QCOMPARE(sections.at(0), QString("0x1e54"));


    sections = Ld::LiteralElement::section("0b1011");

    QCOMPARE(sections.size(), 1);

    QCOMPARE(sections.at(0).sectionType(), Ld::LiteralElement::SectionType::MANTISSA_VALUE);
    QCOMPARE(sections.at(0), QString("0b1011"));


    sections = Ld::LiteralElement::section("<foo");

    QCOMPARE(sections.size(), 1);
    QCOMPARE(sections.at(0).sectionType(), Ld::LiteralElement::SectionType::STRING_CONTENT);
    QCOMPARE(sections.at(0), QString("<foo"));

    sections = Ld::LiteralElement::section("\"bar");

    QCOMPARE(sections.size(), 1);
    QCOMPARE(sections.at(0).sectionType(), Ld::LiteralElement::SectionType::STRING_CONTENT);
    QCOMPARE(sections.at(0), QString("\"bar"));

    sections = Ld::LiteralElement::section("'bang");

    QCOMPARE(sections.size(), 1);
    QCOMPARE(sections.at(0).sectionType(), Ld::LiteralElement::SectionType::STRING_CONTENT);
    QCOMPARE(sections.at(0), QString("'bang"));

    sections = Ld::LiteralElement::section("<foo>");

    QCOMPARE(sections.size(), 1);
    QCOMPARE(sections.at(0).sectionType(), Ld::LiteralElement::SectionType::STRING_CONTENT);
    QCOMPARE(sections.at(0), QString("<foo>"));

    sections = Ld::LiteralElement::section("\"bar\"");

    QCOMPARE(sections.size(), 1);
    QCOMPARE(sections.at(0).sectionType(), Ld::LiteralElement::SectionType::STRING_CONTENT);
    QCOMPARE(sections.at(0), QString("\"bar\""));

    sections = Ld::LiteralElement::section("'bang'");

    QCOMPARE(sections.size(), 1);
    QCOMPARE(sections.at(0).sectionType(), Ld::LiteralElement::SectionType::STRING_CONTENT);
    QCOMPARE(sections.at(0), QString("'bang'"));

    sections = Ld::LiteralElement::section("<foo\"");

    QCOMPARE(sections.size(), 1);
    QCOMPARE(sections.at(0).sectionType(), Ld::LiteralElement::SectionType::STRING_CONTENT);
    QCOMPARE(sections.at(0), QString("<foo\""));

    sections = Ld::LiteralElement::section("\"bar>");

    QCOMPARE(sections.size(), 1);
    QCOMPARE(sections.at(0).sectionType(), Ld::LiteralElement::SectionType::STRING_CONTENT);
    QCOMPARE(sections.at(0), QString("\"bar>"));

    sections = Ld::LiteralElement::section("'bang\"");

    QCOMPARE(sections.size(), 1);
    QCOMPARE(sections.at(0).sectionType(), Ld::LiteralElement::SectionType::STRING_CONTENT);
    QCOMPARE(sections.at(0), QString("'bang\""));
}
