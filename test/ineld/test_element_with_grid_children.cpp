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
* This file implements tests of the Ld::ElementWithGridChildren class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QSet>
#include <QList>
#include <QByteArray>
#include <QSharedPointer>
#include <QtTest/QtTest>

#include <ld_element_structures.h>
#include <ld_data_type.h>
#include <ld_capabilities.h>
#include <ld_xml_writer.h>
#include <ld_xml_reader.h>
#include <ld_xml_attributes.h>
#include <ld_element.h>
#include <ld_element_cursor.h>
#include <ld_cursor.h>
#include <ld_cursor_state_collection.h>
#include <ld_visual_with_grid_children.h>
#include <ld_element_with_grid_children.h>

#include "test_element_with_grid_children.h"

/***********************************************************************************************************************
 * GridElement:
 */

class GridElement:public Ld::ElementWithGridChildren {
    public:
        static const char elementName[];

        GridElement(const QString& identifer = QString());

        ~GridElement() override;

        static Ld::Element* creator(const QString& typeName);

        static QSharedPointer<GridElement> create(const QString& identifier = QString());

        static QSharedPointer<GridElement> create(
            Ld::VisualWithGridChildren* visual,
            const QString&                 identifier = QString()
        );

        QString typeName() const final;

        QString plugInName() const final;

        QString description() const final;

        Ld::DataType::ValueType valueType() const final;

        Ld::ElementPointer clone(bool includeChildren) const override;

        Ld::Capabilities parentRequires(unsigned long index) const final;

        Ld::Capabilities childProvidesCapabilities() const final;

        QString identifier() const;

    private:
        QString elementIdentifier;
};

const char GridElement::elementName[] = "GridElement";

GridElement::GridElement(const QString& identifier) {
    elementIdentifier = identifier; // Assists with debugging.
}


GridElement::~GridElement() {}


Ld::Element* GridElement::creator(const QString&) {
    return new GridElement;
}


QSharedPointer<GridElement> GridElement::create(const QString& identifier) {
    QSharedPointer<GridElement> element = QSharedPointer<GridElement>(new GridElement(identifier));
    element->setWeakThis(element.toWeakRef());

    return element;
}


QSharedPointer<GridElement> GridElement::create(
        Ld::VisualWithGridChildren* visual,
        const QString&                 identifier
    ) {
    QSharedPointer<GridElement> element = GridElement::create(identifier);
    element->setVisual(visual);

    return element;
}


QString GridElement::typeName() const {
    return QString("GridElement");
}


QString GridElement::plugInName() const {
    return QString();
}


QString GridElement::description() const {
    return QString();
}


Ld::DataType::ValueType GridElement::valueType() const {
    return Ld::DataType::ValueType::NONE;
}


Ld::ElementPointer GridElement::clone(bool includeChildren) const {
    Ld::ElementPointer element = ElementWithGridChildren::clone(includeChildren);
    element.dynamicCast<GridElement>()->elementIdentifier = elementIdentifier;

    return element;
}


Ld::Capabilities GridElement::parentRequires(unsigned long) const {
    return Ld::Capabilities();
}


Ld::Capabilities GridElement::childProvidesCapabilities() const {
    return Ld::Capabilities();
}


QString GridElement::identifier() const {
    return elementIdentifier;
}

/***********************************************************************************************************************
 * CallbackData:
 */

class CallbackData {
    public:
        enum class Type: unsigned char {
            NONE = 0,
            ABOUT_TO_REMOVE_CHILD = 1,
            CHILD_SET = 2,
            GEOMETRY_CHANGED = 3,
            ABOUT_TO_REMOVE_ROW = 4,
            ABOUT_TO_REMOVE_COLUMN = 5,
            ROW_INSERTED = 6,
            COLUMN_INSERTED = 7
        };

        CallbackData(Type callbackType, unsigned long index, Ld::ElementPointer childElement = Ld::ElementPointer());

        CallbackData(const CallbackData& other);

        ~CallbackData();

        Type callbackType() const;

        unsigned long index() const;

        Ld::ElementPointer childElement() const;

        CallbackData& operator=(const CallbackData& other);

        bool operator==(const CallbackData& other) const;

        inline bool operator!=(const CallbackData& other) const {
            return !operator==(other);
        }

    private:
        Type                   currentType;
        unsigned long          currentIndex;
        Ld::ElementWeakPointer currentChildElement;
};


CallbackData::CallbackData(CallbackData::Type callbackType, unsigned long index, Ld::ElementPointer childElement) {
    currentType         = callbackType;
    currentIndex        = index;
    currentChildElement = childElement;
}


CallbackData::CallbackData(const CallbackData& other) {
    currentType         = other.currentType;
    currentIndex        = other.currentIndex;
    currentChildElement = other.currentChildElement;
}


CallbackData::~CallbackData() {}


CallbackData::Type CallbackData::callbackType() const {
    return currentType;
}


unsigned long CallbackData::index() const {
    return currentIndex;
}


Ld::ElementPointer CallbackData::childElement() const {
    return currentChildElement;
}


CallbackData& CallbackData::operator=(const CallbackData& other) {
    currentType         = other.currentType;
    currentIndex        = other.currentIndex;
    currentChildElement = other.currentChildElement;

    return *this;
}


bool CallbackData::operator==(const CallbackData& other) const {
    return (
           currentType == other.currentType
        && currentIndex == other.currentIndex
        && currentChildElement == other.currentChildElement
    );
}

/***********************************************************************************************************************
 * GridVisual:
 */

class GridVisual:public Ld::VisualWithGridChildren {
    public:
        GridVisual(const QString& identifer = QString());

        ~GridVisual() override;

        QString typeName() const final;

        QString plugInName() const final;

        QString identifier() const;

        void clearCallData();

        const QList<CallbackData>& callData() const;

    protected:
        void aboutToRemoveChild(unsigned long childIndex, Ld::ElementPointer childElement) final;

        void childSet(unsigned long childIndex, Ld::ElementPointer childElement) final;

        void geometryChanged(
            unsigned long oldNumberRows,
            unsigned long oldNumberColumns,
            unsigned long newNumberRows,
            unsigned long newNumberColumns
        ) final;

        void aboutToRemoveRow(unsigned long rowIndex) final;

        void aboutToRemoveColumn(unsigned long columnIndex) final;

        void rowInserted(unsigned long rowIndex) final;

        void columnInserted(unsigned long columnIndex) final;

    private:
        QString             elementIdentifier;
        QList<CallbackData> currentCallbackData;
};

GridVisual::GridVisual(const QString& identifier) {
    elementIdentifier = identifier;
}


GridVisual::~GridVisual() {}


QString GridVisual::typeName() const {
    return QString("GridVisual");
}


QString GridVisual::plugInName() const {
    return QString();
}


QString GridVisual::identifier() const {
    return elementIdentifier;
}


void GridVisual::clearCallData() {
    currentCallbackData.clear();
}


const QList<CallbackData>& GridVisual::callData() const {
    return currentCallbackData;
}


void GridVisual::aboutToRemoveChild(unsigned long childIndex, Ld::ElementPointer childElement) {
    currentCallbackData.append(CallbackData(CallbackData::Type::ABOUT_TO_REMOVE_CHILD, childIndex, childElement));
}


void GridVisual::childSet(unsigned long childIndex, Ld::ElementPointer childElement) {
    currentCallbackData.append(CallbackData(CallbackData::Type::CHILD_SET, childIndex, childElement));
}


void GridVisual::geometryChanged(
        unsigned long oldNumberRows,
        unsigned long oldNumberColumns,
        unsigned long newNumberRows,
        unsigned long newNumberColumns
    ) {
    unsigned long index = (
          (oldNumberRows << 24)
        | (oldNumberColumns << 16)
        | (newNumberRows << 8)
        | (newNumberColumns)
    );

    currentCallbackData.append(CallbackData(CallbackData::Type::GEOMETRY_CHANGED, index));
}


void GridVisual::aboutToRemoveRow(unsigned long rowIndex) {
    currentCallbackData.append(CallbackData(CallbackData::Type::ABOUT_TO_REMOVE_ROW, rowIndex));
}


void GridVisual::aboutToRemoveColumn(unsigned long columnIndex) {
    currentCallbackData.append(CallbackData(CallbackData::Type::ABOUT_TO_REMOVE_COLUMN, columnIndex));
}


void GridVisual::rowInserted(unsigned long rowIndex) {
    currentCallbackData.append(CallbackData(CallbackData::Type::ROW_INSERTED, rowIndex));
}


void GridVisual::columnInserted(unsigned long columnIndex) {
    currentCallbackData.append(CallbackData(CallbackData::Type::COLUMN_INSERTED, columnIndex));
}

/***********************************************************************************************************************
 * TestElementWithGridChildren:
 */

TestElementWithGridChildren::TestElementWithGridChildren() {}


TestElementWithGridChildren::~TestElementWithGridChildren() {}


void TestElementWithGridChildren::initTestCase() {
    Ld::Handle::initialize(0x12345678UL);
    Ld::Element::registerCreator(GridElement::elementName, &GridElement::creator);
}


void TestElementWithGridChildren::testSetNumberRows() {
    GridVisual* visual = new GridVisual; // Auto-delete is currently enabled.

    QSharedPointer<GridElement> element(new GridElement("parent"));
    element->setWeakThis(element.toWeakRef());
    element->setVisual(visual);

    QCOMPARE(element->numberRows(), GridElement::defaultNumberRows);
    QCOMPARE(element->numberColumns(), GridElement::defaultNumberColumns);

    element->setNumberColumns(2, nullptr);
    element->setNumberRows(2, nullptr);

    QCOMPARE(element->numberRows(), 2U);
    QCOMPARE(element->numberColumns(), 2U);

    for (unsigned rowIndex=0 ; rowIndex<2 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<2 ; ++columnIndex) {
            QSharedPointer<GridElement> child(new GridElement(QString("c%1,%2").arg(rowIndex).arg(columnIndex)));
            child->setWeakThis(child.toWeakRef());
            element->setChild(rowIndex, columnIndex, child, nullptr);
        }
    }

    for (unsigned rowIndex=0 ; rowIndex<2 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<2 ; ++columnIndex) {
            QSharedPointer<GridElement> child = element->child(rowIndex, columnIndex).dynamicCast<GridElement>();
            QCOMPARE(child->identifier(), QString("c%1,%2").arg(rowIndex).arg(columnIndex));
        }
    }

    element->setNumberRows(4U, nullptr);

    QCOMPARE(element->numberRows(), 4U);
    QCOMPARE(element->numberColumns(), 2U);

    for (unsigned rowIndex=0 ; rowIndex<2 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<2 ; ++columnIndex) {
            QSharedPointer<GridElement> child = element->child(rowIndex, columnIndex).dynamicCast<GridElement>();
            QCOMPARE(child->identifier(), QString("c%1,%2").arg(rowIndex).arg(columnIndex));
        }
    }

    for (unsigned rowIndex=2 ; rowIndex<4 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<2 ; ++columnIndex) {
            Ld::ElementPointer child = element->child(rowIndex, columnIndex);
            QCOMPARE(child.isNull(), true);

            QSharedPointer<GridElement> newChild(new GridElement(QString("c%1,%2").arg(rowIndex).arg(columnIndex)));
            newChild->setWeakThis(child.toWeakRef());
            element->setChild(rowIndex, columnIndex, newChild, nullptr);
        }
    }

    for (unsigned rowIndex=0 ; rowIndex<4 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<2 ; ++columnIndex) {
            QSharedPointer<GridElement> child = element->child(rowIndex, columnIndex).dynamicCast<GridElement>();
            QCOMPARE(child->identifier(), QString("c%1,%2").arg(rowIndex).arg(columnIndex));
        }
    }

    element->setNumberRows(3U, nullptr);

    QCOMPARE(element->numberRows(), 3U);
    QCOMPARE(element->numberColumns(), 2U);

    for (unsigned rowIndex=0 ; rowIndex<3 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<2 ; ++columnIndex) {
            QSharedPointer<GridElement> child = element->child(rowIndex, columnIndex).dynamicCast<GridElement>();
            QCOMPARE(child->identifier(), QString("c%1,%2").arg(rowIndex).arg(columnIndex));
        }
    }

    const QList<CallbackData>& callbacks = visual->callData();

    QCOMPARE(callbacks.size(), 8 + 4);
    QCOMPARE(callbacks.contains(CallbackData(CallbackData::Type::GEOMETRY_CHANGED, 0x01010102)), true);
    QCOMPARE(callbacks.contains(CallbackData(CallbackData::Type::GEOMETRY_CHANGED, 0x01020202)), true);
    QCOMPARE(callbacks.contains(CallbackData(CallbackData::Type::GEOMETRY_CHANGED, 0x02020402)), true);
    QCOMPARE(callbacks.contains(CallbackData(CallbackData::Type::GEOMETRY_CHANGED, 0x04020302)), true);
}


void TestElementWithGridChildren::testSetNumberColumns() {
    GridVisual* visual = new GridVisual; // Auto-delete is currently enabled.

    QSharedPointer<GridElement> element(new GridElement("parent"));
    element->setWeakThis(element.toWeakRef());

    element->setVisual(visual);

    QCOMPARE(element->numberRows(), GridElement::defaultNumberRows);
    QCOMPARE(element->numberColumns(), GridElement::defaultNumberColumns);

    element->setNumberColumns(2, nullptr);
    element->setNumberRows(2, nullptr);

    QCOMPARE(element->numberRows(), 2U);
    QCOMPARE(element->numberColumns(), 2U);

    for (unsigned rowIndex=0 ; rowIndex<2 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<2 ; ++columnIndex) {
            QSharedPointer<GridElement> child(new GridElement(QString("c%1,%2").arg(rowIndex).arg(columnIndex)));
            child->setWeakThis(child.toWeakRef());
            element->setChild(rowIndex, columnIndex, child, nullptr);
        }
    }

    for (unsigned rowIndex=0 ; rowIndex<2 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<2; ++columnIndex) {
            QSharedPointer<GridElement> child = element->child(rowIndex, columnIndex).dynamicCast<GridElement>();
            QCOMPARE(child->identifier(), QString("c%1,%2").arg(rowIndex).arg(columnIndex));
        }
    }

    element->setNumberColumns(4U, nullptr);

    QCOMPARE(element->numberRows(), 2U);
    QCOMPARE(element->numberColumns(), 4U);

    for (unsigned rowIndex=0 ; rowIndex<2 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<2 ; ++columnIndex) {
            QSharedPointer<GridElement> child = element->child(rowIndex, columnIndex).dynamicCast<GridElement>();
            QCOMPARE(child->identifier(), QString("c%1,%2").arg(rowIndex).arg(columnIndex));
        }
    }

    for (unsigned rowIndex=0 ; rowIndex<2 ; ++rowIndex) {
        for (unsigned columnIndex=2 ; columnIndex<4 ; ++columnIndex) {
            Ld::ElementPointer child = element->child(rowIndex, columnIndex);
            QCOMPARE(child.isNull(), true);

            QSharedPointer<GridElement> newChild(new GridElement(QString("c%1,%2").arg(rowIndex).arg(columnIndex)));
            newChild->setWeakThis(child.toWeakRef());
            element->setChild(rowIndex, columnIndex, newChild, nullptr);
        }
    }

    for (unsigned rowIndex=0 ; rowIndex<2 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<4 ; ++columnIndex) {
            QSharedPointer<GridElement> child = element->child(rowIndex, columnIndex).dynamicCast<GridElement>();
            QCOMPARE(child->identifier(), QString("c%1,%2").arg(rowIndex).arg(columnIndex));
        }
    }

    element->setNumberColumns(3U, nullptr);

    QCOMPARE(element->numberRows(), 2U);
    QCOMPARE(element->numberColumns(), 3U);

    for (unsigned rowIndex=0 ; rowIndex<2 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<3 ; ++columnIndex) {
            QSharedPointer<GridElement> child = element->child(rowIndex, columnIndex).dynamicCast<GridElement>();
            QCOMPARE(child->identifier(), QString("c%1,%2").arg(rowIndex).arg(columnIndex));
        }
    }

    const QList<CallbackData>& callbacks = visual->callData();

    QCOMPARE(callbacks.size(), 19 + 4);
    QCOMPARE(callbacks.contains(CallbackData(CallbackData::Type::GEOMETRY_CHANGED, 0x01010102)), true);
    QCOMPARE(callbacks.contains(CallbackData(CallbackData::Type::GEOMETRY_CHANGED, 0x01020202)), true);
    QCOMPARE(callbacks.contains(CallbackData(CallbackData::Type::GEOMETRY_CHANGED, 0x02020204)), true);
    QCOMPARE(callbacks.contains(CallbackData(CallbackData::Type::GEOMETRY_CHANGED, 0x02040203)), true);
}


void TestElementWithGridChildren::testChildAccessors() {
    QSharedPointer<GridElement> element(new GridElement("parent"));
    element->setWeakThis(element.toWeakRef());

    element->setNumberRows(2, nullptr);
    element->setNumberColumns(3, nullptr);

    for (unsigned rowIndex=0 ; rowIndex<2 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<3 ; ++columnIndex) {
            QSharedPointer<GridElement> child(new GridElement(QString("c%1,%2").arg(rowIndex).arg(columnIndex)));
            child->setWeakThis(child.toWeakRef());

            element->setChild(rowIndex, columnIndex, child, nullptr);
        }
    }

    QCOMPARE(element->numberChildren(), 6U);
    QCOMPARE(element->numberRows(), 2U);
    QCOMPARE(element->numberColumns(), 3U);

    unsigned childIndex = 0;
    for (unsigned rowIndex=0 ; rowIndex<2 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<3 ; ++columnIndex) {
            QSharedPointer<GridElement> child1 = element->child(rowIndex, columnIndex).dynamicCast<GridElement>();
            QString measured1 = child1->identifier();

            QSharedPointer<GridElement> child2 = element->child(childIndex).dynamicCast<GridElement>();
            QString measured2 = child2->identifier();

            QString expected = QString("c%1,%2").arg(rowIndex).arg(columnIndex);

            QCOMPARE(measured1, expected);
            QCOMPARE(measured2, expected);

            ++childIndex;
        }
    }
}


void TestElementWithGridChildren::testInsertRowBefore() {
    GridVisual* visual = new GridVisual; // Auto-delete is currently enabled.

    QSharedPointer<GridElement> element(new GridElement("parent"));
    element->setWeakThis(element.toWeakRef());

    element->setVisual(visual);

    element->setNumberRows(3, nullptr);
    element->setNumberColumns(2, nullptr);

    unsigned childIndex = 0;
    for (unsigned rowIndex=0 ; rowIndex<3 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<2 ; ++columnIndex) {
            QSharedPointer<GridElement> child(new GridElement(QString::number(childIndex)));
            child->setWeakThis(child.toWeakRef());

            element->setChild(rowIndex, columnIndex, child, nullptr);

            ++childIndex;
        }
    }

    QSharedPointer<GridElement> t(new GridElement(QString("a0")));
    element->insertRowBefore(0, nullptr, t);

    QCOMPARE(element->numberRows(), 4U);
    QCOMPARE(element->numberColumns(), 2U);

    childIndex = 0;
    for (unsigned rowIndex=0 ; rowIndex<4 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<2 ; ++columnIndex) {
            QSharedPointer<GridElement> child    = element->child(rowIndex, columnIndex).dynamicCast<GridElement>();
            QString                     measured = child.isNull() ? QString("!NULL!") : child->identifier();

            QString expected;
            if (rowIndex == 0) {
                expected = QString("a0");
            } else {
                expected = QString::number(childIndex);
                ++childIndex;
            }

            QCOMPARE(measured, expected);
        }
    }

    t.reset(new GridElement(QString("a2")));
    element->insertRowBefore(2, nullptr, t);

    QCOMPARE(element->numberRows(), 5U);
    QCOMPARE(element->numberColumns(), 2U);

    childIndex = 0;
    for (unsigned rowIndex=0 ; rowIndex<5 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<2 ; ++columnIndex) {
            QSharedPointer<GridElement> child    = element->child(rowIndex, columnIndex).dynamicCast<GridElement>();
            QString                     measured = child.isNull() ? QString("!NULL!") : child->identifier();

            QString expected;
            if (rowIndex == 0) {
                expected = QString("a0");
            } else if (rowIndex == 2) {
                expected = QString("a2");
            } else {
                expected = QString::number(childIndex);
                ++childIndex;
            }

            QCOMPARE(measured, expected);
        }
    }

    element->insertRowBefore(GridElement::invalidChildIndex, nullptr);

    QCOMPARE(element->numberRows(), 6U);
    QCOMPARE(element->numberColumns(), 2U);

    childIndex = 0;
    for (unsigned rowIndex=0 ; rowIndex<6 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<2 ; ++columnIndex) {
            QSharedPointer<GridElement> child    = element->child(rowIndex, columnIndex).dynamicCast<GridElement>();
            QString                     measured = child.isNull() ? QString("!NULL!") : child->identifier();

            QString expected;
            if (rowIndex == 0) {
                expected = QString("a0");
            } else if (rowIndex == 2) {
                expected = QString("a2");
            } else if (rowIndex == 5) {
                expected = QString("!NULL!");
            } else {
                expected = QString::number(childIndex);
                ++childIndex;
            }

            QCOMPARE(measured, expected);
        }
    }

    const QList<CallbackData>& callbacks = visual->callData();

    QCOMPARE(callbacks.contains(CallbackData(CallbackData::Type::ROW_INSERTED, 0)), true);
    QCOMPARE(callbacks.contains(CallbackData(CallbackData::Type::ROW_INSERTED, 2)), true);
    QCOMPARE(callbacks.contains(CallbackData(CallbackData::Type::ROW_INSERTED, 5)), true);
}


void TestElementWithGridChildren::testInsertRowAfter() {
    // ElementWithGridChildren::insertRowAfter uses ElementWithGridChildren::insertRowBefore so we only lightly test
    // this method.

    QSharedPointer<GridElement> element(new GridElement("parent"));
    element->setWeakThis(element.toWeakRef());

    element->setNumberRows(3, nullptr);
    element->setNumberColumns(2, nullptr);

    unsigned childIndex = 0;
    for (unsigned rowIndex=0 ; rowIndex<3 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<2 ; ++columnIndex) {
            QSharedPointer<GridElement> child(new GridElement(QString::number(childIndex)));
            child->setWeakThis(child.toWeakRef());

            element->setChild(rowIndex, columnIndex, child, nullptr);

            ++childIndex;
        }
    }

    QSharedPointer<GridElement> t(new GridElement(QString("a1")));
    element->insertRowAfter(0, nullptr, t);

    QCOMPARE(element->numberRows(), 4U);
    QCOMPARE(element->numberColumns(), 2U);

    childIndex = 0;
    for (unsigned rowIndex=0 ; rowIndex<4 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<2 ; ++columnIndex) {
            QSharedPointer<GridElement> child    = element->child(rowIndex, columnIndex).dynamicCast<GridElement>();
            QString                     measured = child.isNull() ? QString("!NULL!") : child->identifier();

            QString expected;
            if (rowIndex == 1) {
                expected = QString("a1");
            } else {
                expected = QString::number(childIndex);
                ++childIndex;
            }

            QCOMPARE(measured, expected);
        }
    }
}


void TestElementWithGridChildren::testInsertColumnBefore() {
    GridVisual* visual = new GridVisual; // Auto-delete is currently enabled.

    QSharedPointer<GridElement> element(new GridElement("parent"));
    element->setWeakThis(element.toWeakRef());

    element->setVisual(visual);

    element->setNumberRows(2, nullptr);
    element->setNumberColumns(3, nullptr);

    unsigned childIndex = 0;
    for (unsigned rowIndex=0 ; rowIndex<2 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<3 ; ++columnIndex) {
            QSharedPointer<GridElement> child(new GridElement(QString::number(childIndex)));
            child->setWeakThis(child.toWeakRef());

            element->setChild(rowIndex, columnIndex, child, nullptr);

            ++childIndex;
        }
    }

    QSharedPointer<GridElement> t(new GridElement(QString("c0")));
    element->insertColumnBefore(0, nullptr, t);

    QCOMPARE(element->numberRows(), 2U);
    QCOMPARE(element->numberColumns(), 4U);

    childIndex = 0;
    for (unsigned rowIndex=0 ; rowIndex<2 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<4 ; ++columnIndex) {
            QSharedPointer<GridElement> child    = element->child(rowIndex, columnIndex).dynamicCast<GridElement>();
            QString                     measured = child.isNull() ? QString("!NULL!") : child->identifier();

            QString expected;
            if (columnIndex == 0) {
                expected = QString("c0");
            } else {
                expected = QString::number(childIndex);
                ++childIndex;
            }

            QCOMPARE(measured, expected);
        }
    }

    t.reset(new GridElement(QString("c2")));
    element->insertColumnBefore(2, nullptr, t);

    QCOMPARE(element->numberRows(), 2U);
    QCOMPARE(element->numberColumns(), 5U);

    childIndex = 0;
    for (unsigned rowIndex=0 ; rowIndex<2 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<5 ; ++columnIndex) {
            QSharedPointer<GridElement> child    = element->child(rowIndex, columnIndex).dynamicCast<GridElement>();
            QString                     measured = child.isNull() ? QString("!NULL!") : child->identifier();

            QString expected;
            if (columnIndex == 0) {
                expected = QString("c0");
            } else if (columnIndex == 2) {
                expected = QString("c2");
            } else {
                expected = QString::number(childIndex);
                ++childIndex;
            }

            QCOMPARE(measured, expected);
        }
    }

    element->insertColumnBefore(GridElement::invalidChildIndex, nullptr);

    QCOMPARE(element->numberRows(), 2U);
    QCOMPARE(element->numberColumns(), 6U);

    childIndex = 0;
    for (unsigned rowIndex=0 ; rowIndex<2 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<6 ; ++columnIndex) {
            QSharedPointer<GridElement> child    = element->child(rowIndex, columnIndex).dynamicCast<GridElement>();
            QString                     measured = child.isNull() ? QString("!NULL!") : child->identifier();

            QString expected;
            if (columnIndex == 0) {
                expected = QString("c0");
            } else if (columnIndex == 2) {
                expected = QString("c2");
            } else if (columnIndex == 5) {
                expected = QString("!NULL!");
            } else {
                expected = QString::number(childIndex);
                ++childIndex;
            }

            QCOMPARE(measured, expected);
        }
    }

    const QList<CallbackData>& callbacks = visual->callData();

    QCOMPARE(callbacks.contains(CallbackData(CallbackData::Type::COLUMN_INSERTED, 0)), true);
    QCOMPARE(callbacks.contains(CallbackData(CallbackData::Type::COLUMN_INSERTED, 2)), true);
    QCOMPARE(callbacks.contains(CallbackData(CallbackData::Type::COLUMN_INSERTED, 5)), true);
}


void TestElementWithGridChildren::testInsertColumnAfter() {
    // ElementWithGridChildren::insertColumnAfter uses ElementWithGridChildren::insertColumnBefore so we only lightly
    // test this method.

    QSharedPointer<GridElement> element(new GridElement("parent"));
    element->setWeakThis(element.toWeakRef());

    element->setNumberRows(2, nullptr);
    element->setNumberColumns(3, nullptr);

    unsigned childIndex = 0;
    for (unsigned rowIndex=0 ; rowIndex<2 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<3 ; ++columnIndex) {
            QSharedPointer<GridElement> child(new GridElement(QString::number(childIndex)));
            child->setWeakThis(child.toWeakRef());

            element->setChild(rowIndex, columnIndex, child, nullptr);

            ++childIndex;
        }
    }

    QSharedPointer<GridElement> t(new GridElement(QString("c1")));
    element->insertColumnAfter(0, nullptr, t);

    QCOMPARE(element->numberRows(), 2U);
    QCOMPARE(element->numberColumns(), 4U);

    childIndex = 0;
    for (unsigned rowIndex=0 ; rowIndex<2 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<4 ; ++columnIndex) {
            QSharedPointer<GridElement> child    = element->child(rowIndex, columnIndex).dynamicCast<GridElement>();
            QString                     measured = child.isNull() ? QString("!NULL!") : child->identifier();

            QString expected;
            if (columnIndex == 1) {
                expected = QString("c1");
            } else {
                expected = QString::number(childIndex);
                ++childIndex;
            }

            QCOMPARE(measured, expected);
        }
    }
}


void TestElementWithGridChildren::testRemoveRow() {
    GridVisual* visual = new GridVisual; // Auto-delete is currently enabled.

    QSharedPointer<GridElement> element(new GridElement("parent"));
    element->setWeakThis(element.toWeakRef());

    element->setVisual(visual);

    element->setNumberRows(5, nullptr);
    element->setNumberColumns(3, nullptr);

    unsigned long childIndex = 0;
    for (unsigned rowIndex=0 ; rowIndex<5 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<3 ; ++columnIndex) {
            QSharedPointer<GridElement> child(new GridElement(QString::number(childIndex)));
            child->setWeakThis(child.toWeakRef());

            element->setChild(rowIndex, columnIndex, child, nullptr);

            ++childIndex;
        }
    }

    element->removeRow(0, nullptr);

    QCOMPARE(element->numberRows(), 4U);
    QCOMPARE(element->numberColumns(), 3U);

    childIndex = element->numberColumns();
    for (unsigned rowIndex=0 ; rowIndex<4 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<3 ; ++columnIndex) {
            QSharedPointer<GridElement> child    = element->child(rowIndex, columnIndex).dynamicCast<GridElement>();
            QString                     measured = child.isNull() ? QString("!NULL!") : child->identifier();

            QString expected = QString::number(childIndex++);
            QCOMPARE(measured, expected);
        }
    }

    element->removeRow(2, nullptr);

    QCOMPARE(element->numberRows(), 3U);
    QCOMPARE(element->numberColumns(), 3U);

    childIndex = element->numberColumns();
    for (unsigned rowIndex=0 ; rowIndex<3 ; ++rowIndex) {
        if (rowIndex == 2) {
            childIndex += element->numberColumns();
        }

        for (unsigned columnIndex=0 ; columnIndex<3 ; ++columnIndex) {
            QSharedPointer<GridElement> child    = element->child(rowIndex, columnIndex).dynamicCast<GridElement>();
            QString                     measured = child.isNull() ? QString("!NULL!") : child->identifier();

            QString expected;
            expected = QString::number(childIndex++);
            QCOMPARE(measured, expected);
        }
    }

    element->removeRow(2, nullptr);

    QCOMPARE(element->numberRows(), 2U);
    QCOMPARE(element->numberColumns(), 3U);

    childIndex = element->numberColumns();
    for (unsigned rowIndex=0 ; rowIndex<2 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<3 ; ++columnIndex) {
            QSharedPointer<GridElement> child    = element->child(rowIndex, columnIndex).dynamicCast<GridElement>();
            QString                     measured = child.isNull() ? QString("!NULL!") : child->identifier();

            QString expected = QString::number(childIndex++);
            QCOMPARE(measured, expected);
        }
    }

    const QList<CallbackData>& callbacks = visual->callData();

    QCOMPARE(callbacks.contains(CallbackData(CallbackData::Type::ABOUT_TO_REMOVE_ROW, 0)), true);
    QCOMPARE(callbacks.contains(CallbackData(CallbackData::Type::ABOUT_TO_REMOVE_ROW, 2)), true);
}


void TestElementWithGridChildren::testRemoveColumn() {
    GridVisual* visual = new GridVisual; // Auto-delete is currently enabled.

    QSharedPointer<GridElement> element(new GridElement("parent"));
    element->setWeakThis(element.toWeakRef());

    element->setVisual(visual);

    element->setNumberRows(3, nullptr);
    element->setNumberColumns(5, nullptr);

    unsigned long childIndex = 0;
    for (unsigned rowIndex=0 ; rowIndex<3 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<5 ; ++columnIndex) {
            QSharedPointer<GridElement> child(new GridElement(QString::number(childIndex)));
            child->setWeakThis(child.toWeakRef());

            element->setChild(rowIndex, columnIndex, child, nullptr);

            ++childIndex;
        }
    }

    element->removeColumn(0, nullptr);

    QCOMPARE(element->numberRows(), 3U);
    QCOMPARE(element->numberColumns(), 4U);

    childIndex = 0;
    for (unsigned rowIndex=0 ; rowIndex<3 ; ++rowIndex) {
        ++childIndex;

        for (unsigned columnIndex=0 ; columnIndex<4 ; ++columnIndex) {
            QSharedPointer<GridElement> child    = element->child(rowIndex, columnIndex).dynamicCast<GridElement>();
            QString                     measured = child.isNull() ? QString("!NULL!") : child->identifier();

            QString expected = QString::number(childIndex++);
            QCOMPARE(measured, expected);
        }
    }

    element->removeColumn(2, nullptr);

    QCOMPARE(element->numberRows(), 3U);
    QCOMPARE(element->numberColumns(), 3U);

    childIndex = 0;
    for (unsigned rowIndex=0 ; rowIndex<3 ; ++rowIndex) {
        ++childIndex;
        for (unsigned columnIndex=0 ; columnIndex<3 ; ++columnIndex) {
            QSharedPointer<GridElement> child    = element->child(rowIndex, columnIndex).dynamicCast<GridElement>();
            QString                     measured = child.isNull() ? QString("!NULL!") : child->identifier();

            if (columnIndex == 2) {
                ++childIndex;
            }

            QString expected = QString::number(childIndex++);
            QCOMPARE(measured, expected);
        }
    }

    element->removeColumn(2, nullptr);

    QCOMPARE(element->numberRows(), 3U);
    QCOMPARE(element->numberColumns(), 2U);

    childIndex = 0;
    for (unsigned rowIndex=0 ; rowIndex<3 ; ++rowIndex) {
        ++childIndex;
        for (unsigned columnIndex=0 ; columnIndex<2 ; ++columnIndex) {
            QSharedPointer<GridElement> child    = element->child(rowIndex, columnIndex).dynamicCast<GridElement>();
            QString                     measured = child.isNull() ? QString("!NULL!") : child->identifier();

            QString expected = QString::number(childIndex++);
            QCOMPARE(measured, expected);
        }
        childIndex += 2;
    }

    const QList<CallbackData>& callbacks = visual->callData();

    QCOMPARE(callbacks.contains(CallbackData(CallbackData::Type::ABOUT_TO_REMOVE_COLUMN, 0)), true);
    QCOMPARE(callbacks.contains(CallbackData(CallbackData::Type::ABOUT_TO_REMOVE_COLUMN, 2)), true);
}


void TestElementWithGridChildren::testCloneMethod() {
    QSharedPointer<GridElement> element(new GridElement("parent"));
    element->setWeakThis(element.toWeakRef());

    element->setNumberRows(3, nullptr);
    element->setNumberColumns(4, nullptr);

    for (unsigned rowIndex=0 ; rowIndex<3 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<4 ; ++columnIndex) {
            QSharedPointer<GridElement> child(new GridElement(QString("c%1,%2").arg(rowIndex).arg(columnIndex)));
            child->setWeakThis(child.toWeakRef());

            element->setChild(rowIndex, columnIndex, child, nullptr);
        }
    }

    QSharedPointer<GridElement> cloned = element->clone(true).dynamicCast<GridElement>();

    QCOMPARE(cloned->numberChildren(), 9U);
    QCOMPARE(cloned->numberRows(), 3U);
    QCOMPARE(cloned->numberColumns(), 4U);

    for (unsigned rowIndex=0 ; rowIndex<3 ; ++rowIndex) {
        for (unsigned columnIndex=0 ; columnIndex<4 ; ++columnIndex) {
            QSharedPointer<GridElement> child = cloned->child(rowIndex, columnIndex).dynamicCast<GridElement>();
            QString measured = child->identifier();
            QString expected = QString("c%1,%2").arg(rowIndex).arg(columnIndex);

            QCOMPARE(measured, expected);
        }
    }
}


void TestElementWithGridChildren::testWriteReadChildrenMethods() {
    // Do we want a test here ?
}
