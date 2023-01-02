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
* This file implements tests of the Ld::TableFrameElement class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QSharedPointer>
#include <QByteArray>
#include <QtTest/QtTest>

#include <QDebug>

#include <random>

#include <util_bit_array.h>

#include <ld_element_structures.h>
#include <ld_root_element.h>
#include <ld_text_element.h>
#include <ld_table_frame_visual.h>
#include <ld_table_frame_element.h>

#include "test_table_frame_element.h"

TestTableFrameElement::TestTableFrameElement() {}


TestTableFrameElement::~TestTableFrameElement() {}


void TestTableFrameElement::initTestCase() {
    Ld::Handle::initialize(0x12345678UL);
    Ld::Element::registerCreator(Ld::RootElement::elementName,  &Ld::RootElement::creator);
    Ld::Element::registerCreator(Ld::TableFrameElement::elementName, &Ld::TableFrameElement::creator);
    Ld::Element::registerCreator(Ld::TextElement::elementName, &Ld::TextElement::creator);
}


void TestTableFrameElement::testIdentificationMethods() {
    QSharedPointer<Ld::TableFrameElement> element = Ld::Element::create(Ld::TableFrameElement::elementName)
                                                    .dynamicCast<Ld::TableFrameElement>();

    QCOMPARE(element->typeName(), Ld::TableFrameElement::elementName);
    QCOMPARE(element->typeName(), QString("TableFrame"));

    QCOMPARE(element->plugInName(), QString());
    QCOMPARE(element->description(), tr("table"));

    QCOMPARE(element->parentRequires(0), Ld::Capabilities::frame);
    QCOMPARE(element->childProvidesCapabilities(), Ld::Capabilities::frame);

    QCOMPARE(element->cursorParkMode(), Ld::Element::CursorParkMode::CAN_NOT_PARK);
}


void TestTableFrameElement::testInsertColumnsBeforeMethod() {
    QSharedPointer<Ld::TableFrameElement> element = Ld::Element::create(Ld::TableFrameElement::elementName)
                                                    .dynamicCast<Ld::TableFrameElement>();

    Ld::ElementPointerList children;

    QSharedPointer<Ld::TextElement> textElement1 = Ld::Element::create(Ld::TextElement::elementName)
                                                   .dynamicCast<Ld::TextElement>();

    QSharedPointer<Ld::TextElement> textElement2 = Ld::Element::create(Ld::TextElement::elementName)
                                                   .dynamicCast<Ld::TextElement>();

    textElement1->setText(QString("Foo"));
    textElement2->setText(QString("Bar"));

    children << textElement1 << textElement2;

    element->insertColumnsBefore(0, 1, true, children);

    QCOMPARE(element->numberColumns(), 2U);
    QCOMPARE(element->numberRows(), 1U);
    QCOMPARE(element->numberGroups(), 2U);
    QCOMPARE(element->numberChildren(), static_cast<unsigned>(children.size()));

    Ld::ElementPointerList cell1Children = element->childrenInGroup(0); // This should be the cell we added
    Ld::ElementPointerList cell2Children = element->childrenInGroup(1);

    QCOMPARE(cell1Children.size(), 2);
    QCOMPARE(cell2Children.size(), 0);

    QCOMPARE(cell1Children.at(0)->typeName(), Ld::TextElement::elementName);
    QCOMPARE(cell1Children.at(1)->typeName(), Ld::TextElement::elementName);

    QCOMPARE(cell1Children.at(0)->text(), QString("Foo"));
    QCOMPARE(cell1Children.at(1)->text(), QString("Bar"));

    textElement1->setText("Bang");
    textElement2->setText("Boom");

    element->insertColumnsBefore(2, 1, true, children);

    QCOMPARE(element->numberColumns(), 3U);
    QCOMPARE(element->numberRows(), 1U);
    QCOMPARE(element->numberGroups(), 3U);
    QCOMPARE(element->numberChildren(), 2 * static_cast<unsigned>(children.size()));

    cell1Children = element->childrenInGroup(0); // This should be the first cell we added
    cell2Children = element->childrenInGroup(1);

    Ld::ElementPointerList cell3Children = element->childrenInGroup(2); // This should be the last cell we added

    QCOMPARE(cell1Children.size(), 2);
    QCOMPARE(cell2Children.size(), 0);
    QCOMPARE(cell3Children.size(), 2);

    QCOMPARE(cell1Children.at(0)->typeName(), Ld::TextElement::elementName);
    QCOMPARE(cell1Children.at(1)->typeName(), Ld::TextElement::elementName);

    QCOMPARE(cell1Children.at(0)->text(), QString("Foo"));
    QCOMPARE(cell1Children.at(1)->text(), QString("Bar"));

    QCOMPARE(cell3Children.at(0)->typeName(), Ld::TextElement::elementName);
    QCOMPARE(cell3Children.at(1)->typeName(), Ld::TextElement::elementName);

    QCOMPARE(cell3Children.at(0)->text(), QString("Bang"));
    QCOMPARE(cell3Children.at(1)->text(), QString("Boom"));

    textElement1->setText("Sproing");
    textElement2->setText("Pfff");

    element->insertColumnsBefore(1, 1, true, children);

    QCOMPARE(element->numberColumns(), 4U);
    QCOMPARE(element->numberRows(), 1U);
    QCOMPARE(element->numberGroups(), 4U);
    QCOMPARE(element->numberChildren(), 3 * static_cast<unsigned>(children.size()));

    cell1Children = element->childrenInGroup(0); // This should be the first cell we added
    cell2Children = element->childrenInGroup(1); // This should be the cell we just added.
    cell3Children = element->childrenInGroup(2);
    Ld::ElementPointerList cell4Children = element->childrenInGroup(3);

    QCOMPARE(cell1Children.size(), 2);
    QCOMPARE(cell2Children.size(), 2);
    QCOMPARE(cell3Children.size(), 0);
    QCOMPARE(cell4Children.size(), 2);

    QCOMPARE(cell1Children.at(0)->typeName(), Ld::TextElement::elementName);
    QCOMPARE(cell1Children.at(1)->typeName(), Ld::TextElement::elementName);

    QCOMPARE(cell1Children.at(0)->text(), QString("Foo"));
    QCOMPARE(cell1Children.at(1)->text(), QString("Bar"));

    QCOMPARE(cell2Children.at(0)->typeName(), Ld::TextElement::elementName);
    QCOMPARE(cell2Children.at(1)->typeName(), Ld::TextElement::elementName);

    QCOMPARE(cell2Children.at(0)->text(), QString("Sproing"));
    QCOMPARE(cell2Children.at(1)->text(), QString("Pfff"));

    QCOMPARE(cell4Children.at(0)->typeName(), Ld::TextElement::elementName);
    QCOMPARE(cell4Children.at(1)->typeName(), Ld::TextElement::elementName);

    QCOMPARE(cell4Children.at(0)->text(), QString("Bang"));
    QCOMPARE(cell4Children.at(1)->text(), QString("Boom"));
}


void TestTableFrameElement::testInsertColumnsAfterMethod() {
    QSharedPointer<Ld::TableFrameElement> element = Ld::Element::create(Ld::TableFrameElement::elementName)
                                                    .dynamicCast<Ld::TableFrameElement>();

    Ld::ElementPointerList children;

    QSharedPointer<Ld::TextElement> textElement1 = Ld::Element::create(Ld::TextElement::elementName)
                                                   .dynamicCast<Ld::TextElement>();

    QSharedPointer<Ld::TextElement> textElement2 = Ld::Element::create(Ld::TextElement::elementName)
                                                   .dynamicCast<Ld::TextElement>();

    textElement1->setText(QString("Foo"));
    textElement2->setText(QString("Bar"));

    children << textElement1 << textElement2;

    element->insertColumnsAfter(0, 1, true, children);

    QCOMPARE(element->numberColumns(), 2U);
    QCOMPARE(element->numberRows(), 1U);
    QCOMPARE(element->numberGroups(), 2U);
    QCOMPARE(element->numberChildren(), static_cast<unsigned>(children.size()));

    Ld::ElementPointerList cell1Children = element->childrenInGroup(0);
    Ld::ElementPointerList cell2Children = element->childrenInGroup(1); // This should be the cell we added

    QCOMPARE(cell1Children.size(), 0);
    QCOMPARE(cell2Children.size(), 2);

    QCOMPARE(cell2Children.at(0)->typeName(), Ld::TextElement::elementName);
    QCOMPARE(cell2Children.at(1)->typeName(), Ld::TextElement::elementName);

    QCOMPARE(cell2Children.at(0)->text(), QString("Foo"));
    QCOMPARE(cell2Children.at(1)->text(), QString("Bar"));

    textElement1->setText("Bang");
    textElement2->setText("Boom");

    element->insertColumnsAfter(0, 1, true, children);

    QCOMPARE(element->numberColumns(), 3U);
    QCOMPARE(element->numberRows(), 1U);
    QCOMPARE(element->numberGroups(), 3U);
    QCOMPARE(element->numberChildren(), 2 * static_cast<unsigned>(children.size()));

    cell1Children = element->childrenInGroup(0);
    cell2Children = element->childrenInGroup(1); // This should be the cell we just added.

    Ld::ElementPointerList cell3Children = element->childrenInGroup(2); // This should be the first cell we added

    QCOMPARE(cell1Children.size(), 0);
    QCOMPARE(cell2Children.size(), 2);
    QCOMPARE(cell3Children.size(), 2);

    QCOMPARE(cell2Children.at(0)->typeName(), Ld::TextElement::elementName);
    QCOMPARE(cell2Children.at(1)->typeName(), Ld::TextElement::elementName);

    QCOMPARE(cell2Children.at(0)->text(), QString("Bang"));
    QCOMPARE(cell2Children.at(1)->text(), QString("Boom"));

    QCOMPARE(cell3Children.at(0)->typeName(), Ld::TextElement::elementName);
    QCOMPARE(cell3Children.at(1)->typeName(), Ld::TextElement::elementName);

    QCOMPARE(cell3Children.at(0)->text(), QString("Foo"));
    QCOMPARE(cell3Children.at(1)->text(), QString("Bar"));
}


void TestTableFrameElement::testInsertRowsBeforeMethod() {
    QSharedPointer<Ld::TableFrameElement> element = Ld::Element::create(Ld::TableFrameElement::elementName)
                                                    .dynamicCast<Ld::TableFrameElement>();

    element->insertColumnsBefore(0, 5, true);
    QCOMPARE(element->numberColumns(), 6U);
    QCOMPARE(element->numberRows(), 1U);

    Ld::ElementPointerList children;
    QSharedPointer<Ld::TextElement> textElement = Ld::Element::create(Ld::TextElement::elementName)
                                                  .dynamicCast<Ld::TextElement>();
    textElement->setText("Foo");
    children << textElement;

    element->insertRowsBefore(1, 2, true, children);
    QCOMPARE(element->numberColumns(), 6U);
    QCOMPARE(element->numberRows(), 3U);

    for (unsigned rowIndex=0 ; rowIndex<3 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<6 ; columnIndex++) {
            Ld::ElementPointerList cellContents = element->childrenInCell(rowIndex, columnIndex);
            if (rowIndex == 0) {
                QCOMPARE(cellContents.size(), 0);
            } else {
                QCOMPARE(cellContents.size(), 1);
                QCOMPARE(cellContents.at(0)->text(), QString("Foo"));
            }
        }
    }

    textElement->setText("Bar");

    element->insertRowsBefore(0, 1, true, children);
    QCOMPARE(element->numberColumns(), 6U);
    QCOMPARE(element->numberRows(), 4U);

    for (unsigned rowIndex=0 ; rowIndex<3 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<6 ; columnIndex++) {
            unsigned groupIndex = element->groupAt(rowIndex, columnIndex);
            Ld::ElementPointerList cellContents = element->childrenInGroup(groupIndex);
            if (rowIndex == 0) {
                QCOMPARE(cellContents.size(), 1);
                QCOMPARE(cellContents.at(0)->text(), QString("Bar"));
            } else if (rowIndex == 1) {
                QCOMPARE(cellContents.size(), 0);
            } else {
                QCOMPARE(cellContents.size(), 1);
                QCOMPARE(cellContents.at(0)->text(), QString("Foo"));
            }
        }
    }

    textElement->setText("Bang");

    element->insertRowsBefore(1, 1, true, children);
    QCOMPARE(element->numberColumns(), 6U);
    QCOMPARE(element->numberRows(),5U);

    for (unsigned rowIndex=0 ; rowIndex<3 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<6 ; columnIndex++) {
            Ld::ElementPointerList cellContents = element->childrenInCell(rowIndex, columnIndex);
            if (rowIndex == 0) {
                QCOMPARE(cellContents.size(), 1);
                QCOMPARE(cellContents.at(0)->text(), QString("Bar"));
            } else if (rowIndex == 1) {
                QCOMPARE(cellContents.size(), 1);
                QCOMPARE(cellContents.at(0)->text(), QString("Bang"));
            } else if (rowIndex == 2) {
                QCOMPARE(cellContents.size(), 0);
            } else {
                QCOMPARE(cellContents.size(), 1);
                QCOMPARE(cellContents.at(0)->text(), QString("Foo"));
            }
        }
    }
}


void TestTableFrameElement::testInsertRowsAfterMethod() {
    QSharedPointer<Ld::TableFrameElement> element = Ld::Element::create(Ld::TableFrameElement::elementName)
                                                    .dynamicCast<Ld::TableFrameElement>();

    element->insertColumnsBefore(0, 5, true);
    QCOMPARE(element->numberColumns(), 6U);
    QCOMPARE(element->numberRows(), 1U);

    Ld::ElementPointerList children;
    QSharedPointer<Ld::TextElement> textElement = Ld::Element::create(Ld::TextElement::elementName)
                                                  .dynamicCast<Ld::TextElement>();
    textElement->setText("Foo");
    children << textElement;

    element->insertRowsAfter(0, 2, true, children);
    QCOMPARE(element->numberColumns(), 6U);
    QCOMPARE(element->numberRows(), 3U);

    for (unsigned rowIndex=0 ; rowIndex<3 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<6 ; columnIndex++) {
            Ld::ElementPointerList cellContents = element->childrenInCell(rowIndex, columnIndex);
            if (rowIndex == 0) {
                QCOMPARE(cellContents.size(), 0);
            } else {
                QCOMPARE(cellContents.size(), 1);
                QCOMPARE(cellContents.at(0)->text(), QString("Foo"));
            }
        }
    }

    textElement->setText("Bar");

    element->insertRowsAfter(0, 1, true, children);
    QCOMPARE(element->numberColumns(), 6U);
    QCOMPARE(element->numberRows(), 4U);

    for (unsigned rowIndex=0 ; rowIndex<3 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<6 ; columnIndex++) {
            unsigned groupIndex = element->groupAt(rowIndex, columnIndex);
            Ld::ElementPointerList cellContents = element->childrenInGroup(groupIndex);
            if (rowIndex == 0) {
                QCOMPARE(cellContents.size(), 0);
            } else if (rowIndex == 1) {
                QCOMPARE(cellContents.size(), 1);
                QCOMPARE(cellContents.at(0)->text(), QString("Bar"));
            } else if (rowIndex == 2) {
                QCOMPARE(cellContents.size(), 1);
                QCOMPARE(cellContents.at(0)->text(), QString("Foo"));
            }
        }
    }
}


void TestTableFrameElement::testRemoveColumnMethod() {
    QSharedPointer<Ld::TableFrameElement> element = Ld::Element::create(Ld::TableFrameElement::elementName)
                                                    .dynamicCast<Ld::TableFrameElement>();

    element->insertColumnsAfter(0, 5, true); // 6 columns, 6 rows, 36 cells.
    element->insertRowsAfter(0, 5, true);

    for (unsigned rowIndex=0 ; rowIndex<6 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<6 ; ++columnIndex) {
            QString  cellText = QString("%1,%2").arg(rowIndex).arg(columnIndex);
            unsigned groupIndex = element->groupAt(rowIndex, columnIndex);
            QSharedPointer<Ld::TextElement> textElement = Ld::Element::create(Ld::TextElement::elementName)
                                                          .dynamicCast<Ld::TextElement>();

            textElement->setText(cellText);
            element->appendToGroup(groupIndex, textElement, nullptr);
        }
    }

    for (unsigned rowIndex=0 ; rowIndex<6 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<6 ; ++columnIndex) {
            QCOMPARE(element->numberChildrenInCell(rowIndex, columnIndex), 1U);

            QString  expectedCellText = QString("%1,%2").arg(rowIndex).arg(columnIndex);
            QString  actualCellText = element->childInCell(rowIndex, columnIndex, 0)->text();

            QCOMPARE(actualCellText, expectedCellText);
        }
    }

    element->removeColumn(0, true, nullptr);

    QCOMPARE(element->numberRows(), 6U);
    QCOMPARE(element->numberColumns(), 5U);

    for (unsigned rowIndex=0 ; rowIndex<6 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<5 ; ++columnIndex) {
            unsigned originalColumnIndex = columnIndex + 1;
            unsigned groupIndex         = element->groupAt(rowIndex, columnIndex);
            QCOMPARE(element->numberChildrenInGroup(groupIndex), 1U);

            QString  expectedCellText = QString("%1,%2").arg(rowIndex).arg(originalColumnIndex);
            QString  actualCellText = element->childInCell(rowIndex, columnIndex, 0)->text();

            QCOMPARE(actualCellText, expectedCellText);
        }
    }

    element->removeColumn(4, true, nullptr);

    QCOMPARE(element->numberRows(), 6U);
    QCOMPARE(element->numberColumns(), 4U);

    for (unsigned rowIndex=0 ; rowIndex<6 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<4 ; ++columnIndex) {
            unsigned originalColumnIndex = columnIndex + 1;
            unsigned groupIndex          = element->groupAt(rowIndex, columnIndex);

            QCOMPARE(element->numberChildrenInGroup(groupIndex), 1U);

            QString  expectedCellText = QString("%1,%2").arg(rowIndex).arg(originalColumnIndex);
            QString  actualCellText   = element->childInGroup(groupIndex, 0)->text();

            QCOMPARE(actualCellText, expectedCellText);
        }
    }

    element->removeColumn(1, true, nullptr);

    QCOMPARE(element->numberRows(), 6U);
    QCOMPARE(element->numberColumns(), 3U);

    for (unsigned rowIndex=0 ; rowIndex<6 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<3 ; ++columnIndex) {
            unsigned originalColumnIndex = columnIndex >= 1 ? columnIndex + 2 : columnIndex + 1;
            unsigned groupIndex          = element->groupAt(rowIndex, columnIndex);

            QCOMPARE(element->numberChildrenInGroup(groupIndex), 1U);

            QString  expectedCellText = QString("%1,%2").arg(rowIndex).arg(originalColumnIndex);
            QString  actualCellText = element->childInGroup(groupIndex, 0)->text();

            QCOMPARE(actualCellText, expectedCellText);
        }
    }
}


void TestTableFrameElement::testRemoveRowMethod() {
    QSharedPointer<Ld::TableFrameElement> element = Ld::Element::create(Ld::TableFrameElement::elementName)
                                                    .dynamicCast<Ld::TableFrameElement>();

    element->insertColumnsBefore(0, 5, true);
    QCOMPARE(element->numberColumns(), 6U);
    QCOMPARE(element->numberRows(), 1U);

    Ld::ElementPointerList children;
    QSharedPointer<Ld::TextElement> textElement = Ld::Element::create(Ld::TextElement::elementName)
                                                  .dynamicCast<Ld::TextElement>();
    textElement->setText("Foo");
    children << textElement;

    element->insertRowsAfter(0, 1, true, children);

    textElement->setText("Bar");
    element->insertRowsBefore(1, 1, true, children);

    textElement->setText("Bang");
    element->insertRowsBefore(1, 1, true, children);

    textElement->setText("Boom");
    element->insertRowsBefore(1, 1, true, children);

    QCOMPARE(element->numberRows(), 5U);
    QCOMPARE(element->numberColumns(), 6U);

    QCOMPARE(element->numberChildrenInGroup(element->groupAt(0, 0)), 0U);
    QCOMPARE(element->numberChildrenInGroup(element->groupAt(0, 5)), 0U);

    QCOMPARE(element->numberChildrenInGroup(element->groupAt(1, 0)), 1U);
    QCOMPARE(element->numberChildrenInGroup(element->groupAt(1, 5)), 1U);

    QCOMPARE(element->numberChildrenInGroup(element->groupAt(2, 0)), 1U);
    QCOMPARE(element->numberChildrenInGroup(element->groupAt(2, 5)), 1U);

    QCOMPARE(element->numberChildrenInGroup(element->groupAt(3, 0)), 1U);
    QCOMPARE(element->numberChildrenInGroup(element->groupAt(3, 5)), 1U);

    QCOMPARE(element->numberChildrenInGroup(element->groupAt(4, 0)), 1U);
    QCOMPARE(element->numberChildrenInGroup(element->groupAt(4, 5)), 1U);

    QCOMPARE(element->childInGroup(element->groupAt(1, 0), 0)->text(), QString("Boom"));
    QCOMPARE(element->childInGroup(element->groupAt(1, 5), 0)->text(), QString("Boom"));

    QCOMPARE(element->childInGroup(element->groupAt(2, 0), 0)->text(), QString("Bang"));
    QCOMPARE(element->childInGroup(element->groupAt(2, 5), 0)->text(), QString("Bang"));

    QCOMPARE(element->childInGroup(element->groupAt(3, 0), 0)->text(), QString("Bar"));
    QCOMPARE(element->childInGroup(element->groupAt(3, 5), 0)->text(), QString("Bar"));

    QCOMPARE(element->childInGroup(element->groupAt(4, 0), 0)->text(), QString("Foo"));
    QCOMPARE(element->childInGroup(element->groupAt(4, 5), 0)->text(), QString("Foo"));



    bool success = element->removeRow(0, true, nullptr);
    QVERIFY(success);

    QCOMPARE(element->numberRows(), 4U);
    QCOMPARE(element->numberColumns(), 6U);

    QCOMPARE(element->numberChildrenInGroup(element->groupAt(0, 0)), 1U);
    QCOMPARE(element->numberChildrenInGroup(element->groupAt(0, 5)), 1U);

    QCOMPARE(element->numberChildrenInGroup(element->groupAt(1, 0)), 1U);
    QCOMPARE(element->numberChildrenInGroup(element->groupAt(1, 5)), 1U);

    QCOMPARE(element->numberChildrenInGroup(element->groupAt(2, 0)), 1U);
    QCOMPARE(element->numberChildrenInGroup(element->groupAt(2, 5)), 1U);

    QCOMPARE(element->numberChildrenInGroup(element->groupAt(3, 0)), 1U);
    QCOMPARE(element->numberChildrenInGroup(element->groupAt(3, 5)), 1U);

    QCOMPARE(element->childInGroup(element->groupAt(0, 0), 0)->text(), QString("Boom"));
    QCOMPARE(element->childInGroup(element->groupAt(0, 5), 0)->text(), QString("Boom"));

    QCOMPARE(element->childInGroup(element->groupAt(1, 0), 0)->text(), QString("Bang"));
    QCOMPARE(element->childInGroup(element->groupAt(1, 5), 0)->text(), QString("Bang"));

    QCOMPARE(element->childInGroup(element->groupAt(2, 0), 0)->text(), QString("Bar"));
    QCOMPARE(element->childInGroup(element->groupAt(2, 5), 0)->text(), QString("Bar"));

    QCOMPARE(element->childInGroup(element->groupAt(3, 0), 0)->text(), QString("Foo"));
    QCOMPARE(element->childInGroup(element->groupAt(3, 5), 0)->text(), QString("Foo"));



    success = element->removeRow(1, true, nullptr);
    QVERIFY(success);

    QCOMPARE(element->numberRows(), 3U);
    QCOMPARE(element->numberColumns(), 6U);

    QCOMPARE(element->numberChildrenInGroup(element->groupAt(0, 0)), 1U);
    QCOMPARE(element->numberChildrenInGroup(element->groupAt(0, 5)), 1U);

    QCOMPARE(element->numberChildrenInGroup(element->groupAt(1, 0)), 1U);
    QCOMPARE(element->numberChildrenInGroup(element->groupAt(1, 5)), 1U);

    QCOMPARE(element->numberChildrenInGroup(element->groupAt(2, 0)), 1U);
    QCOMPARE(element->numberChildrenInGroup(element->groupAt(2, 5)), 1U);

    QCOMPARE(element->childInGroup(element->groupAt(0, 0), 0)->text(), QString("Boom"));
    QCOMPARE(element->childInGroup(element->groupAt(0, 5), 0)->text(), QString("Boom"));

    QCOMPARE(element->childInGroup(element->groupAt(1, 0), 0)->text(), QString("Bar"));
    QCOMPARE(element->childInGroup(element->groupAt(1, 5), 0)->text(), QString("Bar"));

    QCOMPARE(element->childInGroup(element->groupAt(2, 0), 0)->text(), QString("Foo"));
    QCOMPARE(element->childInGroup(element->groupAt(2, 5), 0)->text(), QString("Foo"));



    success = element->removeRow(2, true, nullptr);
    QVERIFY(success);

    QCOMPARE(element->numberRows(), 2U);
    QCOMPARE(element->numberColumns(), 6U);

    QCOMPARE(element->numberChildrenInGroup(element->groupAt(0, 0)), 1U);
    QCOMPARE(element->numberChildrenInGroup(element->groupAt(0, 5)), 1U);

    QCOMPARE(element->numberChildrenInGroup(element->groupAt(1, 0)), 1U);
    QCOMPARE(element->numberChildrenInGroup(element->groupAt(1, 5)), 1U);

    QCOMPARE(element->childInGroup(element->groupAt(0, 0), 0)->text(), QString("Boom"));
    QCOMPARE(element->childInGroup(element->groupAt(0, 5), 0)->text(), QString("Boom"));

    QCOMPARE(element->childInGroup(element->groupAt(1, 0), 0)->text(), QString("Bar"));
    QCOMPARE(element->childInGroup(element->groupAt(1, 5), 0)->text(), QString("Bar"));
}


void TestTableFrameElement::testMergeCellsMethod() {
    const unsigned numberRows    = 6;
    const unsigned numberColumns = 6;

    std::mt19937 rng;

    for (unsigned mergeRowIndex=0 ; mergeRowIndex<numberRows ; ++mergeRowIndex) {
        for (unsigned mergeColumnIndex=0 ; mergeColumnIndex<numberColumns ; ++mergeColumnIndex) {
            QSharedPointer<Ld::TableFrameElement> element = Ld::Element::create(Ld::TableFrameElement::elementName)
                                                            .dynamicCast<Ld::TableFrameElement>();

            element->insertColumnsAfter(0, numberColumns - 1, true);
            element->insertRowsAfter(0, numberRows - 1, true);

            for (unsigned mergeRowIndex=0 ; mergeRowIndex<numberRows ; ++mergeRowIndex) {
                for (unsigned mergeColumnIndex=0 ; mergeColumnIndex<numberColumns ; ++mergeColumnIndex) {
                    QString  cellText = QString("%1,%2").arg(mergeRowIndex).arg(mergeColumnIndex);
                    unsigned groupIndex = element->groupAt(mergeRowIndex, mergeColumnIndex);
                    QSharedPointer<Ld::TextElement> textElement = Ld::Element::create(Ld::TextElement::elementName)
                                                                  .dynamicCast<Ld::TextElement>();

                    textElement->setText(cellText);
                    element->appendToGroup(groupIndex, textElement, nullptr);
                }
            }

            std::uniform_int_distribution<unsigned> randomRowCount(0U, numberRows - mergeRowIndex);
            std::uniform_int_distribution<unsigned> randomColumnCount(0U, numberColumns - mergeColumnIndex);

            unsigned numberCellsToMergeDown  = randomRowCount(rng);
            unsigned numberCellsToMergeRight = randomColumnCount(rng);

            bool validInput = (
                   mergeRowIndex + numberCellsToMergeDown     < numberRows
                && mergeColumnIndex + numberCellsToMergeRight < numberColumns
            );

            bool success = element->mergeCells(
                mergeRowIndex,
                mergeColumnIndex,
                numberCellsToMergeRight,
                numberCellsToMergeDown,
                true,
                nullptr
            );

            QCOMPARE(success, validInput);

            if (success) {
                unsigned numberCellsInMerge = (numberCellsToMergeRight + 1) * (numberCellsToMergeDown + 1);
                unsigned mergedGroupIndex   = element->groupAt(mergeRowIndex, mergeColumnIndex);

                unsigned endingRowIndex    = mergeRowIndex + numberCellsToMergeDown;
                unsigned endingColumnIndex = mergeColumnIndex + numberCellsToMergeRight;

                Util::BitArray assignedGroups;
                assignedGroups.resize(numberRows * numberColumns);
                assignedGroups.clearBits(0, numberRows * numberColumns - 1);

                for (unsigned rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                    for (unsigned columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                        unsigned groupIndex = element->groupAt(rowIndex, columnIndex);
                        if (rowIndex >= mergeRowIndex        &&
                            rowIndex <= endingRowIndex       &&
                            columnIndex >= mergeColumnIndex  &&
                            columnIndex <= endingColumnIndex    ) {
                            QCOMPARE(groupIndex, mergedGroupIndex);
                            QCOMPARE(element->numberChildrenInCell(rowIndex, columnIndex), numberCellsInMerge);
                        } else {
                            unsigned cellIndex = rowIndex * numberColumns + columnIndex;
                            QCOMPARE(assignedGroups.isSet(cellIndex), false);
                            assignedGroups.setBit(cellIndex);

                            QCOMPARE(element->numberChildrenInCell(rowIndex, columnIndex), 1U);

                            QString cellText = QString("%1,%2").arg(rowIndex).arg(columnIndex);
                            QCOMPARE(element->childInCell(rowIndex, columnIndex, 0)->text(), cellText);
                        }
                    }
                }
            }
        }
    }
}


void TestTableFrameElement::testUnmergeCellsMethod() {
    const unsigned numberRows    = 6;
    const unsigned numberColumns = 6;

    QSharedPointer<Ld::TableFrameElement> element = Ld::Element::create(Ld::TableFrameElement::elementName)
                                                    .dynamicCast<Ld::TableFrameElement>();

    element->insertColumnsAfter(0, numberColumns - 1, true);
    element->insertRowsAfter(0, numberRows - 1, true);

    for (unsigned mergeRowIndex=0 ; mergeRowIndex<numberRows ; ++mergeRowIndex) {
        for (unsigned mergeColumnIndex=0 ; mergeColumnIndex<numberColumns ; ++mergeColumnIndex) {
            QString  cellText = QString("%1,%2").arg(mergeRowIndex).arg(mergeColumnIndex);
            unsigned groupIndex = element->groupAt(mergeRowIndex, mergeColumnIndex);
            QSharedPointer<Ld::TextElement> textElement = Ld::Element::create(Ld::TextElement::elementName)
                                                          .dynamicCast<Ld::TextElement>();

            textElement->setText(cellText);
            element->appendToGroup(groupIndex, textElement, nullptr);
        }
    }

    bool success = element->mergeCells(1, 1, 3, 3, true, nullptr);
    QVERIFY(success);

    Ld::ElementPointerList children;
    QSharedPointer<Ld::TextElement> textElement = Ld::Element::create(Ld::TextElement::elementName)
                                                  .dynamicCast<Ld::TextElement>();
    textElement->setText(QString("split"));
    children << textElement;

    success = element->unmergeCells(1, 1, children);
    QVERIFY(success);

    for (unsigned rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
            Ld::ElementPointerList children = element->childrenInCell(rowIndex, columnIndex);
            if (rowIndex < 1 || rowIndex > 4 || columnIndex < 1 || columnIndex > 4) {
                QCOMPARE(children.size(), 1);
                Ld::ElementPointer child = children.first();
                QVERIFY(!child.isNull());
                QCOMPARE(child->typeName(), Ld::TextElement::elementName);

                QString childText    = child->text();
                QString expectedText = QString("%1,%2").arg(rowIndex).arg(columnIndex);
                QCOMPARE(childText, expectedText);
            } else if (rowIndex == 1 && columnIndex == 1) {
                QCOMPARE(children.size(), 4 * 4);
            } else {
                QCOMPARE(children.size(), 1);
                Ld::ElementPointer child = children.first();
                QVERIFY(!child.isNull());
                QCOMPARE(child->typeName(), Ld::TextElement::elementName);

                QString childText = child->text();
                QCOMPARE(childText, QString("split"));
            }
        }
    }
}


void TestTableFrameElement::testCellBoundaryMethods() {
    const unsigned numberRows    = 6;
    const unsigned numberColumns = 6;

    std::mt19937 rng;
    std::uniform_int_distribution<unsigned> randomRow(0U, numberRows - 1);
    std::uniform_int_distribution<unsigned> randomColumn(0U, numberColumns - 1);

    for (unsigned iterationNumber=0 ; iterationNumber<numberIterations ; ++iterationNumber) {
        unsigned topRowIndex     = randomRow(rng);
        unsigned leftColumnIndex = randomColumn(rng);

        std::uniform_int_distribution<unsigned> randomRowCount(0U, numberRows - topRowIndex - 1);
        std::uniform_int_distribution<unsigned> randomColumnCount(0U, numberColumns - leftColumnIndex - 1);

        unsigned numberCellsToMergeDown  = randomRowCount(rng);
        unsigned numberCellsToMergeRight = randomColumnCount(rng);

        QSharedPointer<Ld::TableFrameElement> element = Ld::Element::create(Ld::TableFrameElement::elementName)
                                                        .dynamicCast<Ld::TableFrameElement>();

        element->insertColumnsAfter(0, numberColumns - 1, true);
        element->insertRowsAfter(0, numberRows - 1, true);

        for (unsigned mergeRowIndex=0 ; mergeRowIndex<numberRows ; ++mergeRowIndex) {
            for (unsigned mergeColumnIndex=0 ; mergeColumnIndex<numberColumns ; ++mergeColumnIndex) {
                QString  cellText = QString("%1,%2").arg(mergeRowIndex).arg(mergeColumnIndex);
                unsigned groupIndex = element->groupAt(mergeRowIndex, mergeColumnIndex);
                QSharedPointer<Ld::TextElement> textElement = Ld::Element::create(Ld::TextElement::elementName)
                                                              .dynamicCast<Ld::TextElement>();

                textElement->setText(cellText);
                element->appendToGroup(groupIndex, textElement, nullptr);
            }
        }

        bool success = element->mergeCells(
            topRowIndex,
            leftColumnIndex,
            numberCellsToMergeRight,
            numberCellsToMergeDown,
            true,
            nullptr
        );
        QVERIFY(success);

        unsigned bottomRowIndex   = topRowIndex + numberCellsToMergeDown;
        unsigned rightColumnIndex = leftColumnIndex + numberCellsToMergeRight;

        for (unsigned testIteration=0 ; testIteration<numberIterations ; ++testIteration) {
            unsigned rowIndex    = randomRow(rng);
            unsigned columnIndex = randomColumn(rng);

            bool cellInMergedArea = (
                   rowIndex    >= topRowIndex
                && rowIndex    <= bottomRowIndex
                && columnIndex >= leftColumnIndex
                && columnIndex <= rightColumnIndex
            );

            unsigned topRow      = element->topRow(rowIndex, columnIndex);
            unsigned bottomRow   = element->bottomRow(rowIndex, columnIndex);
            unsigned leftColumn  = element->leftColumn(rowIndex, columnIndex);
            unsigned rightColumn = element->rightColumn(rowIndex, columnIndex);

            if (cellInMergedArea) {
                QCOMPARE(topRow,      topRowIndex);
                QCOMPARE(bottomRow,   bottomRowIndex);
                QCOMPARE(leftColumn,  leftColumnIndex);
                QCOMPARE(rightColumn, rightColumnIndex);
            } else {
                QCOMPARE(topRow,      rowIndex);
                QCOMPARE(bottomRow,   rowIndex);
                QCOMPARE(leftColumn,  columnIndex);
                QCOMPARE(rightColumn, columnIndex);
            }
        }
    }
}


void TestTableFrameElement::testCloning() {
    const unsigned numberRows    = 6;
    const unsigned numberColumns = 6;

    const unsigned mergeRowIndex           = 3;
    const unsigned mergeColumnIndex        = 2;
    const unsigned numberCellsToMergeRight = 2;
    const unsigned numberCellsToMergeDown  = 2;

    QSharedPointer<Ld::TableFrameElement> element1 = Ld::Element::create(Ld::TableFrameElement::elementName)
                                                     .dynamicCast<Ld::TableFrameElement>();

    element1->insertColumnsAfter(0, numberColumns - 1, true);
    element1->insertRowsAfter(0, numberRows - 1, true);

    for (unsigned mergeRowIndex=0 ; mergeRowIndex<numberRows ; ++mergeRowIndex) {
        for (unsigned mergeColumnIndex=0 ; mergeColumnIndex<numberColumns ; ++mergeColumnIndex) {
            QString  cellText = QString("%1,%2").arg(mergeRowIndex).arg(mergeColumnIndex);
            unsigned groupIndex = element1->groupAt(mergeRowIndex, mergeColumnIndex);
            QSharedPointer<Ld::TextElement> textElement = Ld::Element::create(Ld::TextElement::elementName)
                                                          .dynamicCast<Ld::TextElement>();

            textElement->setText(cellText);
            element1->appendToGroup(groupIndex, textElement, nullptr);
        }
    }

    bool success = element1->mergeCells(
        mergeRowIndex,
        mergeColumnIndex,
        numberCellsToMergeRight,
        numberCellsToMergeDown,
        true,
        nullptr
    );

    QVERIFY(success);

    QSharedPointer<Ld::TableFrameElement> element2 = element1->clone(true).dynamicCast<Ld::TableFrameElement>();

    unsigned topRowIndex       = mergeRowIndex;
    unsigned bottomRowIndex    = mergeRowIndex + numberCellsToMergeDown;
    unsigned leftColumnIndex   = mergeColumnIndex;
    unsigned rightColumnIndex  = mergeColumnIndex + numberCellsToMergeRight;

    unsigned numberMergedCells = (numberCellsToMergeRight + 1) * (numberCellsToMergeDown + 1);

    for (unsigned rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
            QString  cellText   = QString("%1,%2").arg(mergeRowIndex).arg(mergeColumnIndex);
            if (rowIndex >= topRowIndex        &&
                rowIndex <= bottomRowIndex     &&
                columnIndex >= leftColumnIndex &&
                columnIndex <= rightColumnIndex    ) {
                QCOMPARE(element2->numberChildrenInCell(rowIndex, columnIndex), numberMergedCells);
            } else {
                QCOMPARE(element2->numberChildrenInCell(rowIndex, columnIndex), 1U);

                QString expectedCellText = QString("%1,%2").arg(rowIndex).arg(columnIndex);
                QString actualCellText   = element2->childInCell(rowIndex, columnIndex, 0)->text();

                QCOMPARE(actualCellText, expectedCellText);
            }
        }
    }
}


void TestTableFrameElement::testSaveLoadMethods() {
    // Lightly tested in TestProgramLoadSave
}

