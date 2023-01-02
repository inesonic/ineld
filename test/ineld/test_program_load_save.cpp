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
* This file implements tests of program save and load functions across multiple classes.  This test is intended to
* provide a central location to add tests for classes that must save and restore information to a program file.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QtTest/QtTest>

#include <ld_handle.h>
#include <ld_configure.h>
#include <ld_code_generator.h>
#include <ld_character_format.h>
#include <ld_page_format.h>
#include <plug_in_data.h>
#include <ld_element.h>
#include <ld_element_with_fixed_children.h>
#include <ld_paragraph_element.h>
#include <ld_paragraph_format.h>
#include <ld_text_element.h>
#include <ld_character_format.h>
#include <ld_table_frame_element.h>
#include <ld_table_frame_format.h>

#include <ld_table_column_width.h>
#include <ld_root_visual.h>
#include <ld_root_element.h>

#include "test_program_load_save.h"


const QString TestProgramLoadSave::programFileName("test_program.ms");

TestProgramLoadSave::TestProgramLoadSave() {
    Ld::CodeGenerator::releaseCodeGenerators();
    Ld::Configure::configure(0x123456789ABCDEF0, nullptr);
}


TestProgramLoadSave::~TestProgramLoadSave() {}


void TestProgramLoadSave::testProgramSave() {
    QSharedPointer<Ld::RootElement> rootElement(new Ld::RootElement);
    rootElement->setWeakThis(rootElement.toWeakRef());

    bool success = rootElement->openNew();
    QVERIFY(success);

    QSharedPointer<Ld::ParagraphElement> paragraphElement(new Ld::ParagraphElement);
    paragraphElement->setWeakThis(paragraphElement.toWeakRef());

    QSharedPointer<Ld::ParagraphFormat> paragraphFormat(new Ld::ParagraphFormat);
    paragraphFormat->setFirstLineLeftIndentation(12);
    paragraphFormat->setLeftIndentation(6);
    paragraphFormat->setRightIndentation(6);
    paragraphFormat->setTopSpacing(6);
    paragraphFormat->setBottomSpacing(6);
    paragraphFormat->setJustification(Ld::ParagraphFormat::Justification::CENTER);
    paragraphElement->setFormat(paragraphFormat);

    rootElement->append(paragraphElement, nullptr);

    QSharedPointer<Ld::TextElement> textElement(new Ld::TextElement);
    textElement->setWeakThis(textElement.toWeakRef());
    textElement->setText(QString("foo"));

    QSharedPointer<Ld::CharacterFormat> textCharacterFormat(new Ld::CharacterFormat);
    textCharacterFormat->setFamily("Courier");
    textCharacterFormat->setFontSize(14);
    textElement->setFormat(textCharacterFormat);

    paragraphElement->append(textElement, nullptr);

    QSharedPointer<Ld::TableFrameElement> tableFrameElement(new Ld::TableFrameElement);
    tableFrameElement->setWeakThis(tableFrameElement.toWeakRef());

    QSharedPointer<Ld::TableFrameFormat> tableFrameFormat(new Ld::TableFrameFormat);
    tableFrameFormat->setLeftIndentation(24.0F);
    tableFrameFormat->setRightIndentation(12.0F);
    tableFrameFormat->setTopSpacing(6.0F);
    tableFrameFormat->setBottomSpacing(8.0F);
    tableFrameFormat->setJustification(Ld::TableFrameFormat::Justification::RIGHT);
    tableFrameFormat->setDefaultColumnWidth(Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::FIXED, 36.0F));
    tableFrameFormat->setDefaultColumnLineSetting(Ld::TableLineSettings(Ld::TableLineSettings::Style::NO_LINE, 0.0F));
    tableFrameFormat->setDefaultRowLineSetting(Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 2.0F));
    tableFrameFormat->setDefaultColor(QColor(Qt::lightGray));

    tableFrameFormat->setColumnWidth(0, Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::PROPORTIONAL, 0.25F));
    tableFrameFormat->setColumnLineSetting(1, Ld::TableLineSettings(Ld::TableLineSettings::Style::SINGLE, 2.0F));
    tableFrameFormat->setRowLineSetting(2, Ld::TableLineSettings(Ld::TableLineSettings::Style::SINGLE, 1.0F));
    tableFrameFormat->setColumnColor(1, QColor(Qt::red));
    tableFrameFormat->setRowColor(2, QColor(Qt::blue));
    tableFrameFormat->setCellColor(3, 2, QColor(Qt::green));

    tableFrameFormat->setLeftGutter(0.05F * 72.0F);
    tableFrameFormat->setRightGutter(0.06F * 72.0F);
    tableFrameFormat->setTopGutter(0.07F * 72.0F);
    tableFrameFormat->setBottomGutter(0.08F * 72.0F);

    tableFrameElement->setFormat(tableFrameFormat);

    Ld::ElementPointerList tableChildren;
    QSharedPointer<Ld::ParagraphElement> tableParagraphElement(new Ld::ParagraphElement);
    tableParagraphElement->setWeakThis(tableParagraphElement.toWeakRef());

    QSharedPointer<Ld::TextElement> tableTextElement(new Ld::TextElement);
    tableTextElement->setWeakThis(tableTextElement.toWeakRef());
    tableParagraphElement->append(tableTextElement, nullptr);

    tableChildren << tableParagraphElement;

    tableFrameElement->insertRowsAfter(0, 5, true, tableChildren);
    tableFrameElement->insertColumnsAfter(0, 2, true, tableChildren);

    tableFrameElement->mergeCells(1, 1, 1, 2, true, nullptr);

    rootElement->append(tableFrameElement, nullptr);

    success = rootElement->saveAs(programFileName);
    QVERIFY(success);

    success = rootElement->close();
    QVERIFY(success);
}


void TestProgramLoadSave::testProgramLoad() {
    QSharedPointer<Ld::RootElement> rootElement(new Ld::RootElement);
    rootElement->setWeakThis(rootElement.toWeakRef());

    Ld::PlugInsByName plugInsByName;
    bool success = rootElement->openExisting(programFileName, true, plugInsByName);
    QVERIFY(success);

    // TODO: Verify root element contents (for now we manually test as we write code).

    success = rootElement->close();
    QVERIFY(success);
}
