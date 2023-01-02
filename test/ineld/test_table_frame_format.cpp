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
* This file implements tests of the Ld::TableFrameFormat class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QColor>
#include <QSharedPointer>
#include <QtTest/QtTest>

#include <ld_table_column_width.h>
#include <ld_table_line_settings.h>
#include <ld_table_frame_format.h>
#include <ld_xml_writer.h>
#include <ld_xml_reader.h>

#include "test_table_frame_format.h"

TestTableFrameFormat::TestTableFrameFormat() {}


TestTableFrameFormat::~TestTableFrameFormat() {}


void TestTableFrameFormat::initTestCase() {
    Ld::Format::registerCreator(Ld::TableFrameFormat::formatName, &Ld::TableFrameFormat::creator);
}


void TestTableFrameFormat::testConstructorsAndDestructors() {
    Ld::TableFrameFormat tableFormat1;

    QVERIFY(tableFormat1.isValid());
    QCOMPARE(tableFormat1.defaultColumnWidth(), Ld::TableFrameFormat::initialDefaultColumnWidth);
    QCOMPARE(tableFormat1.defaultColumnLineSetting(), Ld::TableFrameFormat::defaultLineSetting);
    QCOMPARE(tableFormat1.defaultRowLineSetting(), Ld::TableFrameFormat::defaultLineSetting);
    QVERIFY(!tableFormat1.defaultColor().isValid());

    tableFormat1.setLeftIndentation(36);
    tableFormat1.setRightIndentation(37);
    tableFormat1.setTopSpacing(38);
    tableFormat1.setBottomSpacing(39);
    tableFormat1.setJustification(Ld::TableFrameFormat::Justification::RIGHT);
    tableFormat1.setDefaultColumnWidth(Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::FIXED, 18.0F));
    tableFormat1.setColumnWidth(2, Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::PROPORTIONAL, 1.0F));
    tableFormat1.setDefaultColumnLineSetting(Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 0.5F));
    tableFormat1.setColumnLineSetting(1, Ld::TableLineSettings(Ld::TableLineSettings::Style::NO_LINE, 0.0F));
    tableFormat1.setDefaultRowLineSetting(Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 0.5F));
    tableFormat1.setRowLineSetting(2, Ld::TableLineSettings(Ld::TableLineSettings::Style::SINGLE, 2.0F));
    tableFormat1.setDefaultColor(QColor(Qt::red));
    tableFormat1.setColumnColor(2, QColor(Qt::blue));
    tableFormat1.setRowColor(3, QColor(Qt::green));
    tableFormat1.setCellColor(4, 5, QColor(Qt::cyan));

    Ld::TableFrameFormat tableFormat2 = tableFormat1;

    QCOMPARE(tableFormat2.leftIndentation(), 36.0F);
    QCOMPARE(tableFormat2.rightIndentation(), 37.0F);
    QCOMPARE(tableFormat2.topSpacing(), 38.0F);
    QCOMPARE(tableFormat2.bottomSpacing(), 39.0F);
    QCOMPARE(tableFormat2.justificationMode(), Ld::TableFrameFormat::Justification::RIGHT);
    QCOMPARE(tableFormat2.defaultColumnWidth(), Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::FIXED, 18.0F));
    QCOMPARE(tableFormat2.columnWidth(2), Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::PROPORTIONAL, 1.0F));
    QCOMPARE(
        tableFormat2.defaultColumnLineSetting(),
        Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 0.5F)
    );
    QCOMPARE(tableFormat2.columnLineSetting(1), Ld::TableLineSettings(Ld::TableLineSettings::Style::NO_LINE, 0.0F));
    QCOMPARE(tableFormat2.defaultRowLineSetting(), Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 0.5F));
    QCOMPARE(tableFormat2.rowLineSetting(2), Ld::TableLineSettings(Ld::TableLineSettings::Style::SINGLE, 2.0F));
    QCOMPARE(tableFormat2.defaultColor(), QColor(Qt::red));
    QCOMPARE(tableFormat2.columnColor(2), QColor(Qt::blue));
    QCOMPARE(tableFormat2.rowColor(3), QColor(Qt::green));
    QCOMPARE(tableFormat2.cellColor(4, 5), QColor(Qt::cyan));
}



void TestTableFrameFormat::testCommonAccessorsAndFactories() {
    QSharedPointer<Ld::TableFrameFormat> tableFormat1 = Ld::Format::create(Ld::TableFrameFormat::formatName)
                                                        .dynamicCast<Ld::TableFrameFormat>();

    QCOMPARE(Ld::TableFrameFormat::formatName, QString("TableFrameFormat"));
    QCOMPARE(tableFormat1->typeName(), QString("TableFrameFormat"));
    QVERIFY(tableFormat1->capabilities().contains(Ld::BlockFormat::formatName));
    QVERIFY(tableFormat1->capabilities().contains(Ld::JustifiedBlockFormat::formatName));
    QVERIFY(tableFormat1->capabilities().contains(Ld::TableFrameFormat::formatName));

    QVERIFY(tableFormat1->isValid());
    QCOMPARE(tableFormat1->defaultColumnWidth(), Ld::TableFrameFormat::initialDefaultColumnWidth);
    QCOMPARE(tableFormat1->defaultColumnLineSetting(), Ld::TableFrameFormat::defaultLineSetting);
    QCOMPARE(tableFormat1->defaultRowLineSetting(), Ld::TableFrameFormat::defaultLineSetting);
    QVERIFY(!tableFormat1->defaultColor().isValid());

    tableFormat1->setLeftIndentation(36);
    tableFormat1->setRightIndentation(37);
    tableFormat1->setTopSpacing(38);
    tableFormat1->setBottomSpacing(39);
    tableFormat1->setJustification(Ld::TableFrameFormat::Justification::RIGHT);
    tableFormat1->setDefaultColumnWidth(Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::FIXED, 18.0F));
    tableFormat1->setColumnWidth(2, Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::PROPORTIONAL, 1.0F));
    tableFormat1->setDefaultColumnLineSetting(Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 0.5F));
    tableFormat1->setColumnLineSetting(1, Ld::TableLineSettings(Ld::TableLineSettings::Style::NO_LINE, 0.0F));
    tableFormat1->setDefaultRowLineSetting(Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 0.5F));
    tableFormat1->setRowLineSetting(2, Ld::TableLineSettings(Ld::TableLineSettings::Style::SINGLE, 2.0F));
    tableFormat1->setDefaultColor(QColor(Qt::red));
    tableFormat1->setColumnColor(2, QColor(Qt::blue));
    tableFormat1->setRowColor(3, QColor(Qt::green));
    tableFormat1->setCellColor(4, 5, QColor(Qt::cyan));

    QSharedPointer<Ld::TableFrameFormat> tableFormat2 = tableFormat1->clone().dynamicCast<Ld::TableFrameFormat>();

    QCOMPARE(tableFormat2->leftIndentation(), 36.0F);
    QCOMPARE(tableFormat2->rightIndentation(), 37.0F);
    QCOMPARE(tableFormat2->topSpacing(), 38.0F);
    QCOMPARE(tableFormat2->bottomSpacing(), 39.0F);
    QCOMPARE(tableFormat2->justificationMode(), Ld::TableFrameFormat::Justification::RIGHT);
    QCOMPARE(tableFormat2->defaultColumnWidth(), Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::FIXED, 18.0F));
    QCOMPARE(tableFormat2->columnWidth(2), Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::PROPORTIONAL, 1.0F));
    QCOMPARE(
        tableFormat2->defaultColumnLineSetting(),
        Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 0.5F)
    );
    QCOMPARE(tableFormat2->columnLineSetting(1), Ld::TableLineSettings(Ld::TableLineSettings::Style::NO_LINE, 0.0F));
    QCOMPARE(tableFormat2->defaultRowLineSetting(), Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 0.5F));
    QCOMPARE(tableFormat2->rowLineSetting(2), Ld::TableLineSettings(Ld::TableLineSettings::Style::SINGLE, 2.0F));
    QCOMPARE(tableFormat2->defaultColor(), QColor(Qt::red));
    QCOMPARE(tableFormat2->columnColor(2), QColor(Qt::blue));
    QCOMPARE(tableFormat2->rowColor(3), QColor(Qt::green));
    QCOMPARE(tableFormat2->cellColor(4, 5), QColor(Qt::cyan));
}


void TestTableFrameFormat::testColumnWidthMethods() {
    Ld::TableFrameFormat tableFormat;

    tableFormat.setDefaultColumnWidth(Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::FIXED, 36.0F));
    QCOMPARE(tableFormat.defaultColumnWidth(), Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::FIXED, 36.0F));

    for (unsigned columnIndex=0 ; columnIndex<maximumTestColumn ; ++columnIndex) {
        QCOMPARE(
            tableFormat.columnWidth(columnIndex),
            Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::FIXED, 36.0F)
        );
        QVERIFY(tableFormat.columnWidthMapsToDefault(columnIndex));
    }

    tableFormat.setColumnWidth(2, Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::PROPORTIONAL, 0.5F));

    for (unsigned columnIndex=0 ; columnIndex<maximumTestColumn ; ++columnIndex) {
        if (columnIndex == 2) {
            QCOMPARE(
                tableFormat.columnWidth(columnIndex),
                Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::PROPORTIONAL, 0.5F)
            );
            QVERIFY(!tableFormat.columnWidthMapsToDefault(columnIndex));

        } else {
            QCOMPARE(
                tableFormat.columnWidth(columnIndex),
                Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::FIXED, 36.0F)
            );
            QVERIFY(tableFormat.columnWidthMapsToDefault(columnIndex));
        }
    }

    tableFormat.setColumnToDefaultWidth(2);

    for (unsigned columnIndex=0 ; columnIndex<maximumTestColumn ; ++columnIndex) {
        QCOMPARE(
            tableFormat.columnWidth(columnIndex),
            Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::FIXED, 36.0F)
        );
        QVERIFY(tableFormat.columnWidthMapsToDefault(columnIndex));
    }

    tableFormat.setColumnWidth(3, Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::PROPORTIONAL, 0.7F));

    for (unsigned columnIndex=0 ; columnIndex<maximumTestColumn ; ++columnIndex) {
        if (columnIndex == 3) {
            QCOMPARE(
                tableFormat.columnWidth(columnIndex),
                Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::PROPORTIONAL, 0.7F)
            );
            QVERIFY(!tableFormat.columnWidthMapsToDefault(columnIndex));

        } else {
            QCOMPARE(
                tableFormat.columnWidth(columnIndex),
                Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::FIXED, 36.0F)
            );
            QVERIFY(tableFormat.columnWidthMapsToDefault(columnIndex));
        }
    }

    tableFormat.setColumnWidth(3, Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::FIXED, 36.0F));

    for (unsigned columnIndex=0 ; columnIndex<maximumTestColumn ; ++columnIndex) {
        QCOMPARE(
            tableFormat.columnWidth(columnIndex),
            Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::FIXED, 36.0F)
        );
        QVERIFY(tableFormat.columnWidthMapsToDefault(columnIndex));
    }
}


void TestTableFrameFormat::testColumnLineMethod() {
    Ld::TableFrameFormat tableFormat;

    tableFormat.setDefaultColumnLineSetting(Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 1.5F));
    QCOMPARE(
        tableFormat.defaultColumnLineSetting(),
        Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 1.5F)
    );

    for (unsigned lineIndex=0 ; lineIndex<maximumTestColumn ; ++lineIndex) {
        QCOMPARE(
            tableFormat.columnLineSetting(lineIndex),
            Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 1.5F)
        );
        QVERIFY(tableFormat.columnLineSettingMapsToDefault(lineIndex));
    }

    tableFormat.setColumnLineSetting(2, Ld::TableLineSettings(Ld::TableLineSettings::Style::NO_LINE, 0.0F));

    for (unsigned lineIndex=0 ; lineIndex<maximumTestColumn ; ++lineIndex) {
        if (lineIndex == 2) {
            QCOMPARE(
                tableFormat.columnLineSetting(lineIndex),
                Ld::TableLineSettings(Ld::TableLineSettings(Ld::TableLineSettings::Style::NO_LINE, 0.0F))
            );
            QVERIFY(!tableFormat.columnLineSettingMapsToDefault(lineIndex));

        } else {
            QCOMPARE(
                tableFormat.columnLineSetting(lineIndex),
                Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 1.5F)
            );
            QVERIFY(tableFormat.columnLineSettingMapsToDefault(lineIndex));
        }
    }

    tableFormat.setColumnLineSettingToDefault(2);

    for (unsigned lineIndex=0 ; lineIndex<maximumTestColumn ; ++lineIndex) {
        QCOMPARE(
            tableFormat.columnLineSetting(lineIndex),
            Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 1.5F)
        );
        QVERIFY(tableFormat.columnLineSettingMapsToDefault(lineIndex));
    }

    tableFormat.setColumnLineSetting(3, Ld::TableLineSettings(Ld::TableLineSettings::Style::SINGLE, 4.0F));

    for (unsigned lineIndex=0 ; lineIndex<maximumTestColumn ; ++lineIndex) {
        if (lineIndex == 3) {
            QCOMPARE(
                tableFormat.columnLineSetting(lineIndex),
                Ld::TableLineSettings(Ld::TableLineSettings(Ld::TableLineSettings::Style::SINGLE, 4.0F))
            );
            QVERIFY(!tableFormat.columnLineSettingMapsToDefault(lineIndex));

        } else {
            QCOMPARE(
                tableFormat.columnLineSetting(lineIndex),
                Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 1.5F)
            );
            QVERIFY(tableFormat.columnLineSettingMapsToDefault(lineIndex));
        }
    }

    tableFormat.setColumnLineSetting(3, Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 1.5F));

    for (unsigned lineIndex=0 ; lineIndex<maximumTestColumn ; ++lineIndex) {
        QCOMPARE(
            tableFormat.columnLineSetting(lineIndex),
            Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 1.5F)
        );
        QVERIFY(tableFormat.columnLineSettingMapsToDefault(lineIndex));
    }
}


void TestTableFrameFormat::testRowLineMethod() {
    Ld::TableFrameFormat tableFormat;

    tableFormat.setDefaultRowLineSetting(Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 1.5F));
    QCOMPARE(
        tableFormat.defaultRowLineSetting(),
        Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 1.5F)
    );

    for (unsigned lineIndex=0 ; lineIndex<maximumTestRow ; ++lineIndex) {
        QCOMPARE(
            tableFormat.rowLineSetting(lineIndex),
            Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 1.5F)
        );
        QVERIFY(tableFormat.rowLineSettingMapsToDefault(lineIndex));
    }

    tableFormat.setRowLineSetting(2, Ld::TableLineSettings(Ld::TableLineSettings::Style::NO_LINE, 0.0F));

    for (unsigned lineIndex=0 ; lineIndex<maximumTestRow ; ++lineIndex) {
        if (lineIndex == 2) {
            QCOMPARE(
                tableFormat.rowLineSetting(lineIndex),
                Ld::TableLineSettings(Ld::TableLineSettings(Ld::TableLineSettings::Style::NO_LINE, 0.0F))
            );
            QVERIFY(!tableFormat.rowLineSettingMapsToDefault(lineIndex));

        } else {
            QCOMPARE(
                tableFormat.rowLineSetting(lineIndex),
                Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 1.5F)
            );
            QVERIFY(tableFormat.rowLineSettingMapsToDefault(lineIndex));
        }
    }

    tableFormat.setRowLineSettingToDefault(2);

    for (unsigned lineIndex=0 ; lineIndex<maximumTestRow ; ++lineIndex) {
        QCOMPARE(
            tableFormat.rowLineSetting(lineIndex),
            Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 1.5F)
        );
        QVERIFY(tableFormat.rowLineSettingMapsToDefault(lineIndex));
    }

    tableFormat.setRowLineSetting(3, Ld::TableLineSettings(Ld::TableLineSettings::Style::SINGLE, 4.0F));

    for (unsigned lineIndex=0 ; lineIndex<maximumTestRow ; ++lineIndex) {
        if (lineIndex == 3) {
            QCOMPARE(
                tableFormat.rowLineSetting(lineIndex),
                Ld::TableLineSettings(Ld::TableLineSettings(Ld::TableLineSettings::Style::SINGLE, 4.0F))
            );
            QVERIFY(!tableFormat.rowLineSettingMapsToDefault(lineIndex));

        } else {
            QCOMPARE(
                tableFormat.rowLineSetting(lineIndex),
                Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 1.5F)
            );
            QVERIFY(tableFormat.rowLineSettingMapsToDefault(lineIndex));
        }
    }

    tableFormat.setRowLineSetting(3, Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 1.5F));

    for (unsigned lineIndex=0 ; lineIndex<maximumTestRow ; ++lineIndex) {
        QCOMPARE(
            tableFormat.rowLineSetting(lineIndex),
            Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 1.5F)
        );
        QVERIFY(tableFormat.rowLineSettingMapsToDefault(lineIndex));
    }
}


void TestTableFrameFormat::testColorMethod() {
    Ld::TableFrameFormat tableFormat;

    tableFormat.setDefaultColor(QColor(0x00, 0x00, 0xFF));
    QCOMPARE(tableFormat.defaultColor(), QColor(0x00, 0x00, 0xFF));

    for (unsigned rowIndex=0 ; rowIndex<maximumTestRow ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<maximumTestColumn ; ++columnIndex) {
            QCOMPARE(tableFormat.blendedColor(rowIndex, columnIndex), QColor(0x00, 0x00, 0xFF));
            QVERIFY(!tableFormat.cellColor(rowIndex, columnIndex).isValid());
            QVERIFY(tableFormat.columnColorMapsToDefault(columnIndex));
        }

        QVERIFY(tableFormat.rowColorMapsToDefault(rowIndex));
    }

    tableFormat.setColumnColor(2, QColor(0xFF, 0x00, 0x00));
    tableFormat.setRowColor(3, QColor(0x00, 0xFF,0x00));

    for (unsigned rowIndex=0 ; rowIndex<maximumTestRow ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<maximumTestColumn ; ++columnIndex) {
            QColor   cellColor = tableFormat.blendedColor(rowIndex, columnIndex);
            unsigned red       = cellColor.red();
            unsigned green     = cellColor.green();
            unsigned blue      = cellColor.blue();
            unsigned alpha     = cellColor.alpha();

            if (rowIndex == 3) {
                if (columnIndex == 2) {
                    QCOMPARE(red,   0x55U); // 0x55U = 255 / 3
                    QCOMPARE(green, 0x55U);
                    QCOMPARE(blue,  0x55U);
                    QCOMPARE(alpha, 0xFFU);

                    QVERIFY(!tableFormat.columnColorMapsToDefault(columnIndex));
                } else {
                    QCOMPARE(red,   0x00U);
                    QCOMPARE(green, 0x80U);
                    QCOMPARE(blue,  0x80U);
                    QCOMPARE(alpha, 0xFFU);

                    QVERIFY(tableFormat.columnColorMapsToDefault(columnIndex));
                }

                QVERIFY(!tableFormat.rowColorMapsToDefault(rowIndex));
            } else {
                if (columnIndex == 2) {
                    QCOMPARE(red,   0x80U);
                    QCOMPARE(green, 0x00U);
                    QCOMPARE(blue,  0x80U);
                    QCOMPARE(alpha, 0xFFU);

                    QVERIFY(!tableFormat.columnColorMapsToDefault(columnIndex));
                } else {
                    QCOMPARE(red,   0x00U);
                    QCOMPARE(green, 0x00U);
                    QCOMPARE(blue,  0xFFU);
                    QCOMPARE(alpha, 0xFFU);

                    QVERIFY(tableFormat.columnColorMapsToDefault(columnIndex));
                }

                QVERIFY(tableFormat.rowColorMapsToDefault(rowIndex));
            }
        }
    }

    tableFormat.setColumnColorToDefault(2);
    tableFormat.setRowColorToDefault(3);

    for (unsigned rowIndex=0 ; rowIndex<maximumTestRow ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<maximumTestColumn ; ++columnIndex) {
            QColor   cellColor = tableFormat.blendedColor(rowIndex, columnIndex);
            unsigned red       = cellColor.red();
            unsigned green     = cellColor.green();
            unsigned blue      = cellColor.blue();
            unsigned alpha     = cellColor.alpha();

            QCOMPARE(red,   0x00U);
            QCOMPARE(green, 0x00U);
            QCOMPARE(blue,  0xFFU);
            QCOMPARE(alpha, 0xFFU);

            QVERIFY(tableFormat.columnColorMapsToDefault(columnIndex));
            QVERIFY(tableFormat.rowColorMapsToDefault(rowIndex));
        }
    }
}


void TestTableFrameFormat::testColumnRemovedMethod() {
    Ld::TableFrameFormat tableFormat;

    tableFormat.setDefaultColumnWidth(Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::PROPORTIONAL, 0.125F));
    tableFormat.setDefaultColumnLineSetting(Ld::TableLineSettings(Ld::TableLineSettings::Style::SINGLE, 1.0F));
    tableFormat.setDefaultColor(QColor());

    tableFormat.setColumnWidth(1, Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::FIXED, 1.0F));
    tableFormat.setColumnWidth(2, Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::FIXED, 2.0F));
    tableFormat.setColumnWidth(3, Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::FIXED, 3.0F));

    tableFormat.setColumnLineSetting(0, Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 0.0F));
    tableFormat.setColumnLineSetting(1, Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 1.0F));
    tableFormat.setColumnLineSetting(2, Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 2.0F));
    tableFormat.setColumnLineSetting(3, Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 3.0F));
    tableFormat.setColumnLineSetting(4, Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 4.0F));

    tableFormat.setColumnColor(1, QColor(0x10, 0x10, 0x10));
    tableFormat.setColumnColor(2, QColor(0x20, 0x20, 0x20));
    tableFormat.setColumnColor(3, QColor(0x30, 0x30, 0x30));

    tableFormat.setCellColor(1, 1, QColor(0x10, 0x00, 0x00));
    tableFormat.setCellColor(2, 2, QColor(0x20, 0x00, 0x00));
    tableFormat.setCellColor(3, 3, QColor(0x30, 0x00, 0x00));

    tableFormat.columnRemoved(2);

    QCOMPARE(tableFormat.columnWidth(1), Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::FIXED, 1.0F));
    QCOMPARE(tableFormat.columnWidth(2), Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::FIXED, 3.0F));
    QCOMPARE(tableFormat.columnWidth(3), Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::PROPORTIONAL, 0.125F));

    QCOMPARE(tableFormat.columnLineSetting(0), Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 0.0F));
    QCOMPARE(tableFormat.columnLineSetting(1), Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 1.0F));
    QCOMPARE(tableFormat.columnLineSetting(2), Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 2.0F));
    QCOMPARE(tableFormat.columnLineSetting(3), Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 4.0F));
    QCOMPARE(tableFormat.columnLineSetting(4), Ld::TableLineSettings(Ld::TableLineSettings::Style::SINGLE, 1.0F));

    QCOMPARE(tableFormat.columnColor(1), QColor(0x10, 0x10, 0x10));
    QCOMPARE(tableFormat.columnColor(2), QColor(0x30, 0x30, 0x30));
    QCOMPARE(tableFormat.columnColor(3), QColor());

    QCOMPARE(tableFormat.cellColor(1, 1), QColor(0x10, 0x00, 0x00));
    QCOMPARE(tableFormat.cellColor(3, 2), QColor(0x30, 0x00, 0x00));
}


void TestTableFrameFormat::testRowRemovedMethod() {
    Ld::TableFrameFormat tableFormat;

    tableFormat.setDefaultRowLineSetting(Ld::TableLineSettings(Ld::TableLineSettings::Style::SINGLE, 1.0F));
    tableFormat.setDefaultColor(QColor());

    tableFormat.setRowLineSetting(0, Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 0.0F));
    tableFormat.setRowLineSetting(1, Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 1.0F));
    tableFormat.setRowLineSetting(2, Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 2.0F));
    tableFormat.setRowLineSetting(3, Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 3.0F));
    tableFormat.setRowLineSetting(4, Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 4.0F));

    tableFormat.setRowColor(1, QColor(0x10, 0x10, 0x10));
    tableFormat.setRowColor(2, QColor(0x20, 0x20, 0x20));
    tableFormat.setRowColor(3, QColor(0x30, 0x30, 0x30));

    tableFormat.setCellColor(1, 1, QColor(0x10, 0x00, 0x00));
    tableFormat.setCellColor(2, 2, QColor(0x20, 0x00, 0x00));
    tableFormat.setCellColor(3, 3, QColor(0x30, 0x00, 0x00));

    tableFormat.rowRemoved(2);

    QCOMPARE(tableFormat.rowLineSetting(0), Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 0.0F));
    QCOMPARE(tableFormat.rowLineSetting(1), Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 1.0F));
    QCOMPARE(tableFormat.rowLineSetting(2), Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 2.0F));
    QCOMPARE(tableFormat.rowLineSetting(3), Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 4.0F));
    QCOMPARE(tableFormat.rowLineSetting(4), Ld::TableLineSettings(Ld::TableLineSettings::Style::SINGLE, 1.0F));

    QCOMPARE(tableFormat.rowColor(1), QColor(0x10, 0x10, 0x10));
    QCOMPARE(tableFormat.rowColor(2), QColor(0x30, 0x30, 0x30));
    QCOMPARE(tableFormat.rowColor(3), QColor());

    QCOMPARE(tableFormat.cellColor(1, 1), QColor(0x10, 0x00, 0x00));
    QCOMPARE(tableFormat.cellColor(2, 3), QColor(0x30, 0x00, 0x00));
}


void TestTableFrameFormat::testColumnsInsertedMethod() {
    Ld::TableFrameFormat tableFormat;

    tableFormat.setDefaultColumnWidth(Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::PROPORTIONAL, 0.125F));
    tableFormat.setDefaultColumnLineSetting(Ld::TableLineSettings(Ld::TableLineSettings::Style::SINGLE, 1.0F));
    tableFormat.setDefaultColor(QColor());

    tableFormat.setColumnWidth(1, Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::FIXED, 1.0F));
    tableFormat.setColumnWidth(2, Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::FIXED, 2.0F));
    tableFormat.setColumnWidth(3, Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::FIXED, 3.0F));

    tableFormat.setColumnLineSetting(0, Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 0.0F));
    tableFormat.setColumnLineSetting(1, Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 1.0F));
    tableFormat.setColumnLineSetting(2, Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 2.0F));
    tableFormat.setColumnLineSetting(3, Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 3.0F));
    tableFormat.setColumnLineSetting(4, Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 4.0F));

    tableFormat.setColumnColor(1, QColor(0x10, 0x10, 0x10));
    tableFormat.setColumnColor(2, QColor(0x20, 0x20, 0x20));
    tableFormat.setColumnColor(3, QColor(0x30, 0x30, 0x30));

    tableFormat.setCellColor(1, 1, QColor(0x10, 0x00, 0x00));
    tableFormat.setCellColor(2, 2, QColor(0x20, 0x00, 0x00));
    tableFormat.setCellColor(3, 3, QColor(0x30, 0x00, 0x00));

    tableFormat.columnsInserted(2, 2);

    QCOMPARE(tableFormat.columnWidth(1), Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::FIXED, 1.0F));
    QCOMPARE(tableFormat.columnWidth(2), Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::PROPORTIONAL, 0.125F));
    QCOMPARE(tableFormat.columnWidth(3), Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::PROPORTIONAL, 0.125F));
    QCOMPARE(tableFormat.columnWidth(4), Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::FIXED, 2.0F));
    QCOMPARE(tableFormat.columnWidth(5), Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::FIXED, 3.0F));

    QCOMPARE(tableFormat.columnLineSetting(0), Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 0.0F));
    QCOMPARE(tableFormat.columnLineSetting(1), Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 1.0F));
    QCOMPARE(tableFormat.columnLineSetting(2), Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 2.0F));
    QCOMPARE(tableFormat.columnLineSetting(3), Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 3.0F));
    QCOMPARE(tableFormat.columnLineSetting(4), Ld::TableLineSettings(Ld::TableLineSettings::Style::SINGLE, 1.0F));
    QCOMPARE(tableFormat.columnLineSetting(5), Ld::TableLineSettings(Ld::TableLineSettings::Style::SINGLE, 1.0F));
    QCOMPARE(tableFormat.columnLineSetting(6), Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 4.0F));

    QCOMPARE(tableFormat.columnColor(1), QColor(0x10, 0x10, 0x10));
    QCOMPARE(tableFormat.columnColor(2), QColor());
    QCOMPARE(tableFormat.columnColor(3), QColor());
    QCOMPARE(tableFormat.columnColor(4), QColor(0x20, 0x20, 0x20));
    QCOMPARE(tableFormat.columnColor(5), QColor(0x30, 0x30, 0x30));

    QCOMPARE(tableFormat.cellColor(1, 1), QColor(0x10, 0x00, 0x00));
    QCOMPARE(tableFormat.cellColor(2, 4), QColor(0x20, 0x00, 0x00));
    QCOMPARE(tableFormat.cellColor(3, 5), QColor(0x30, 0x00, 0x00));
}


void TestTableFrameFormat::testRowsInsertedMethod() {
    Ld::TableFrameFormat tableFormat;

    tableFormat.setDefaultRowLineSetting(Ld::TableLineSettings(Ld::TableLineSettings::Style::SINGLE, 1.0F));
    tableFormat.setDefaultColor(QColor());

    tableFormat.setRowLineSetting(0, Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 0.0F));
    tableFormat.setRowLineSetting(1, Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 1.0F));
    tableFormat.setRowLineSetting(2, Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 2.0F));
    tableFormat.setRowLineSetting(3, Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 3.0F));
    tableFormat.setRowLineSetting(4, Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 4.0F));

    tableFormat.setRowColor(1, QColor(0x10, 0x10, 0x10));
    tableFormat.setRowColor(2, QColor(0x20, 0x20, 0x20));
    tableFormat.setRowColor(3, QColor(0x30, 0x30, 0x30));

    tableFormat.setCellColor(1, 1, QColor(0x10, 0x00, 0x00));
    tableFormat.setCellColor(2, 2, QColor(0x20, 0x00, 0x00));
    tableFormat.setCellColor(3, 3, QColor(0x30, 0x00, 0x00));

    tableFormat.rowsInserted(2, 2);

    QCOMPARE(tableFormat.rowLineSetting(0), Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 0.0F));
    QCOMPARE(tableFormat.rowLineSetting(1), Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 1.0F));
    QCOMPARE(tableFormat.rowLineSetting(2), Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 2.0F));
    QCOMPARE(tableFormat.rowLineSetting(3), Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 3.0F));
    QCOMPARE(tableFormat.rowLineSetting(4), Ld::TableLineSettings(Ld::TableLineSettings::Style::SINGLE, 1.0F));
    QCOMPARE(tableFormat.rowLineSetting(5), Ld::TableLineSettings(Ld::TableLineSettings::Style::SINGLE, 1.0F));
    QCOMPARE(tableFormat.rowLineSetting(6), Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 4.0F));

    QCOMPARE(tableFormat.rowColor(1), QColor(0x10, 0x10, 0x10));
    QCOMPARE(tableFormat.rowColor(2), QColor());
    QCOMPARE(tableFormat.rowColor(3), QColor());
    QCOMPARE(tableFormat.rowColor(4), QColor(0x20, 0x20, 0x20));
    QCOMPARE(tableFormat.rowColor(5), QColor(0x30, 0x30, 0x30));

    QCOMPARE(tableFormat.cellColor(1, 1), QColor(0x10, 0x00, 0x00));
    QCOMPARE(tableFormat.cellColor(4, 2), QColor(0x20, 0x00, 0x00));
    QCOMPARE(tableFormat.cellColor(5, 3), QColor(0x30, 0x00, 0x00));
}


void TestTableFrameFormat::testMergeCellsMethod() {
    Ld::TableFrameFormat tableFormat1;

    tableFormat1.setDefaultColor(QColor());

    tableFormat1.setCellColor(1, 1, QColor(0x10, 0x00, 0x00));
    tableFormat1.setCellColor(2, 2, QColor(0x20, 0x00, 0x00));
    tableFormat1.setCellColor(3, 3, QColor(0x30, 0x00, 0x00));
    tableFormat1.setCellColor(4, 4, QColor(0x40, 0x00, 0x00));
    tableFormat1.setCellColor(5, 5, QColor(0x50, 0x00, 0x00));

    tableFormat1.mergeCells(1, 2, 2, 2);

    //    0 1 2 3 4 5
    //  0
    //  1   x *****
    //  2     x   *
    //  3     **x**
    //  4         x
    //  5           x

    QCOMPARE(tableFormat1.cellColor(1, 1), QColor(0x10, 0x00, 0x00));
    QCOMPARE(tableFormat1.cellColor(2, 2), QColor());
    QCOMPARE(tableFormat1.cellColor(3, 3), QColor());
    QCOMPARE(tableFormat1.cellColor(4, 4), QColor(0x40, 0x00, 0x00));
    QCOMPARE(tableFormat1.cellColor(5, 5), QColor(0x50, 0x00, 0x00));

    Ld::TableFrameFormat tableFormat2;

    tableFormat2.setDefaultColor(QColor());

    tableFormat2.setCellColor(1, 1, QColor(0x10, 0x00, 0x00));
    tableFormat2.setCellColor(2, 2, QColor(0x20, 0x00, 0x00));
    tableFormat2.setCellColor(3, 3, QColor(0x30, 0x00, 0x00));
    tableFormat2.setCellColor(4, 4, QColor(0x40, 0x00, 0x00));
    tableFormat2.setCellColor(5, 5, QColor(0x50, 0x00, 0x00));

    tableFormat2.mergeCells(2, 2, 2, 2);

    //    0 1 2 3 4 5
    //  0
    //  1   x
    //  2     x****
    //  3     * x *
    //  4     ****x
    //  5           x

    QCOMPARE(tableFormat2.cellColor(1, 1), QColor(0x10, 0x00, 0x00));
    QCOMPARE(tableFormat2.cellColor(2, 2), QColor(0x20, 0x00, 0x00));
    QCOMPARE(tableFormat2.cellColor(3, 3), QColor());
    QCOMPARE(tableFormat2.cellColor(4, 4), QColor());
    QCOMPARE(tableFormat2.cellColor(5, 5), QColor(0x50, 0x00, 0x00));
}


void TestTableFrameFormat::testToStringMethod() {
    Ld::TableFrameFormat tableFormat;

    tableFormat.setDefaultColumnWidth(Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::PROPORTIONAL, 0.125F));
    tableFormat.setDefaultColumnLineSetting(Ld::TableLineSettings(Ld::TableLineSettings::Style::SINGLE, 1.0F));
    tableFormat.setDefaultColor(QColor());

    tableFormat.setColumnWidth(1, Ld::TableColumnWidth(Ld::TableColumnWidth::WidthType::FIXED, 1.0F));
    tableFormat.setColumnLineSetting(0, Ld::TableLineSettings(Ld::TableLineSettings::Style::DOUBLE, 0.0F));
    tableFormat.setColumnColor(1, QColor(0x10, 0x10, 0x10));
    tableFormat.setCellColor(1, 1, QColor(0x10, 0x00, 0x00));

    QString text = tableFormat.toString();
    QCOMPARE(
        text,
        QString(
            "TableFrameFormat,"
            "0,0,6,6,CENTER,"
            "PROPORTIONAL(0.125),SINGLE(1),SINGLE(1),#000000,"
            "2.88,2.88,2.88,2.88,"
            "W(1,FIXED(1)),CL(0,DOUBLE(0)),cC(1,1,#100000),CC(1,#101010)"
        )
    );
}


void TestTableFrameFormat::testToCssMethod() {
    Ld::TableFrameFormat tableFormat;

    tableFormat.setLeftIndentation(18.0F);
    tableFormat.setRightIndentation(24.0F);
    tableFormat.setJustification(Ld::TableFrameFormat::Justification::RIGHT);

    QString css = tableFormat.toCss();
    QCOMPARE(css, QString("border-collapse: collapse; table-layout: fixed; margin-left: auto: margin-right: 24pt; "));

    tableFormat.setLeftIndentation(18.0F);
    tableFormat.setRightIndentation(24.0F);
    tableFormat.setJustification(Ld::TableFrameFormat::Justification::LEFT);

    css = tableFormat.toCss();
    QCOMPARE(css, QString("border-collapse: collapse; table-layout: fixed; margin-left: 18pt; margin-right: auto; "));

    tableFormat.setLeftIndentation(18.0F);
    tableFormat.setRightIndentation(24.0F);
    tableFormat.setJustification(Ld::TableFrameFormat::Justification::CENTER);

    css = tableFormat.toCss();
    QCOMPARE(css, QString("border-collapse: collapse; table-layout: fixed; margin-left: auto; margin-right: auto; "));
}


void TestTableFrameFormat::testToCssEntryMethod() {
    qDebug() << "Test of Ld::TableFrameFormat::toCssEntry missing.";
}
