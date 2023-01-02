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
* This file implements tests for the \ref Ld::ElementGroup class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QList>
#include <QSharedPointer>

#include <cassert>
#include <algorithm>
#include <random>

#include <ld_element_structures.h>
#include <ld_element.h>
#include <ld_paragraph_element.h>
#include <ld_text_element.h>
#include <ld_element_group.h>

#include "test_element_group.h"

TestElementGroup::TestElementGroup() {}


TestElementGroup::~TestElementGroup() {}


void TestElementGroup::initTestCase() {}


void TestElementGroup::testConstructorsAndDestructors() {
    Ld::ElementGroup group1;

    QVERIFY(group1.isEmpty());
    QVERIFY(group1.precedingElement().isNull());
    QVERIFY(group1.followingElement().isNull());

    QSharedPointer<Ld::ParagraphElement> element1(new Ld::ParagraphElement);
    element1->setWeakThis(element1.toWeakRef());

    QSharedPointer<Ld::ParagraphElement> element2(new Ld::ParagraphElement);
    element2->setWeakThis(element2.toWeakRef());

    QSharedPointer<Ld::ParagraphElement> element3(new Ld::ParagraphElement);
    element3->setWeakThis(element3.toWeakRef());

    Ld::ElementPointerList elementList;
    elementList.append(element1);
    elementList.append(element2);
    elementList.append(element3);

    Ld::ElementGroup group2(elementList);

    QCOMPARE(group2.size(), 3);
    QVERIFY(group2.contains(element1));
    QVERIFY(group2.contains(element2));
    QVERIFY(group2.contains(element3));

    Ld::ElementGroup group3(group2);

    QCOMPARE(group3.size(), 3);
    QVERIFY(group3.contains(element1));
    QVERIFY(group3.contains(element2));
    QVERIFY(group3.contains(element3));
}


void TestElementGroup::testPrecedingElementMethod() {
    Ld::ElementGroup group1;

    QVERIFY(group1.precedingElement().isNull());

    QSharedPointer<Ld::ParagraphElement> element1(new Ld::ParagraphElement);
    element1->setWeakThis(element1.toWeakRef());

    QSharedPointer<Ld::ParagraphElement> element2(new Ld::ParagraphElement);
    element2->setWeakThis(element2.toWeakRef());

    QSharedPointer<Ld::ParagraphElement> element3(new Ld::ParagraphElement);
    element3->setWeakThis(element3.toWeakRef());

    QSharedPointer<Ld::ParagraphElement> parent(new Ld::ParagraphElement);
    parent->setWeakThis(parent.toWeakRef());

    parent->append(element1, nullptr);
    parent->append(element2, nullptr);
    parent->append(element3, nullptr);

    group1.append(element2);
    group1.append(element3);

    QCOMPARE(group1.precedingElement(), element1);

    Ld::ElementGroup group2;
    group2.append(element1);
    group2.append(element2);

    QVERIFY(group2.precedingElement().isNull());
}


void TestElementGroup::testFollowingElementMethod() {
    Ld::ElementGroup group1;

    QVERIFY(group1.followingElement().isNull());

    QSharedPointer<Ld::ParagraphElement> element1(new Ld::ParagraphElement);
    element1->setWeakThis(element1.toWeakRef());

    QSharedPointer<Ld::ParagraphElement> element2(new Ld::ParagraphElement);
    element2->setWeakThis(element2.toWeakRef());

    QSharedPointer<Ld::ParagraphElement> element3(new Ld::ParagraphElement);
    element3->setWeakThis(element3.toWeakRef());

    QSharedPointer<Ld::ParagraphElement> parent(new Ld::ParagraphElement);
    parent->setWeakThis(parent.toWeakRef());

    parent->append(element1, nullptr);
    parent->append(element2, nullptr);
    parent->append(element3, nullptr);

    group1.append(element1);
    group1.append(element2);

    QCOMPARE(group1.followingElement(), element3);

    Ld::ElementGroup group2;
    group2.append(element2);
    group2.append(element3);

    QVERIFY(group2.followingElement().isNull());
}


void TestElementGroup::testConvertToGroupsMethod() {
    QSharedPointer<Ld::ParagraphElement> element1(new Ld::ParagraphElement);
    element1->setWeakThis(element1.toWeakRef());

    QSharedPointer<Ld::ParagraphElement> element2(new Ld::ParagraphElement);
    element2->setWeakThis(element2.toWeakRef());

    QSharedPointer<Ld::ParagraphElement> element3(new Ld::ParagraphElement);
    element3->setWeakThis(element3.toWeakRef());

    QSharedPointer<Ld::ParagraphElement> element4(new Ld::ParagraphElement);
    element4->setWeakThis(element4.toWeakRef());

    QSharedPointer<Ld::ParagraphElement> element5(new Ld::ParagraphElement);
    element5->setWeakThis(element5.toWeakRef());

    QSharedPointer<Ld::ParagraphElement> element6(new Ld::ParagraphElement);
    element6->setWeakThis(element6.toWeakRef());

    QSharedPointer<Ld::ParagraphElement> element7(new Ld::ParagraphElement);
    element7->setWeakThis(element7.toWeakRef());

    QSharedPointer<Ld::ParagraphElement> element8(new Ld::ParagraphElement);
    element8->setWeakThis(element8.toWeakRef());

    QSharedPointer<Ld::ParagraphElement> parent1(new Ld::ParagraphElement);
    parent1->setWeakThis(parent1.toWeakRef());

    QSharedPointer<Ld::ParagraphElement> parent2(new Ld::ParagraphElement);
    parent2->setWeakThis(parent2.toWeakRef());

    parent1->append(element1, nullptr);
    parent1->append(element2, nullptr);
    parent1->append(element3, nullptr);
    parent1->append(element4, nullptr);
    parent1->append(element5, nullptr);
    parent1->append(element6, nullptr);

    parent2->append(element7, nullptr);
    parent2->append(element8, nullptr);

    Ld::ElementPointerList orderedElements;
    orderedElements << element1;
    orderedElements << element2;
    orderedElements << element3;

    //           skip: element4;

    orderedElements << element5;
    orderedElements << element6;
    orderedElements << element7;
    orderedElements << element8;

    std::mt19937 rng;
    std::uniform_int_distribution<unsigned> randomLocation(0U, 6U);

    for (unsigned iteration=0 ; iteration<numberIterations ; ++iteration) {
        Ld::ElementPointerList elements = orderedElements;
        for (unsigned i=0 ; i<static_cast<unsigned>(3 * orderedElements.size()) ; ++i) {
            unsigned location1 = randomLocation(rng);
            unsigned location2;
            do {
                location2 = randomLocation(rng);
            } while (location2 == location1);

            Ld::ElementPointer temporary = elements.at(location1);
            elements[location1] = elements[location2];
            elements[location2] = temporary;
        }

        QList<Ld::ElementGroup> groups = Ld::ElementGroup::convertToGroups(elements);

        QCOMPARE(groups.size(), 3);
        unsigned group1Index = static_cast<unsigned>(-1);
        unsigned group2Index = static_cast<unsigned>(-1);
        unsigned group3Index = static_cast<unsigned>(-1);

        if (groups[0].contains(element1)) {
            group1Index = 0;
        } else if (groups[1].contains(element1)) {
            group1Index = 1;
        } else if (groups[2].contains(element1)) {
            group1Index = 2;
        }

        if (groups[0].contains(element5)) {
            group2Index = 0;
        } else if (groups[1].contains(element5)) {
            group2Index = 1;
        } else if (groups[2].contains(element5)) {
            group2Index = 2;
        }

        if (groups[0].contains(element7)) {
            group3Index = 0;
        } else if (groups[1].contains(element7)) {
            group3Index = 1;
        } else if (groups[2].contains(element7)) {
            group3Index = 2;
        }

        QVERIFY(group1Index != static_cast<unsigned>(-1));
        QVERIFY(group2Index != static_cast<unsigned>(-1));
        QVERIFY(group3Index != static_cast<unsigned>(-1));

        Ld::ElementGroup& group1 = groups[group1Index];
        Ld::ElementGroup& group2 = groups[group2Index];
        Ld::ElementGroup& group3 = groups[group3Index];

        QCOMPARE(group1.size(), 3);
        QCOMPARE(group2.size(), 2);
        QCOMPARE(group3.size(), 2);

        QCOMPARE(group1.at(0), element1);
        QCOMPARE(group1.at(1), element2);
        QCOMPARE(group1.at(2), element3);

        QCOMPARE(group2.at(0), element5);
        QCOMPARE(group2.at(1), element6);

        QCOMPARE(group3.at(0), element7);
        QCOMPARE(group3.at(1), element8);
    }
}


void TestElementGroup::testAssignment() {
    Ld::ElementGroup group1;
    Ld::ElementGroup group2;

    QSharedPointer<Ld::ParagraphElement> element1(new Ld::ParagraphElement);
    element1->setWeakThis(element1.toWeakRef());

    QSharedPointer<Ld::ParagraphElement> element2(new Ld::ParagraphElement);
    element2->setWeakThis(element2.toWeakRef());

    QSharedPointer<Ld::ParagraphElement> element3(new Ld::ParagraphElement);
    element3->setWeakThis(element3.toWeakRef());

    QSharedPointer<Ld::ParagraphElement> element4(new Ld::ParagraphElement);
    element4->setWeakThis(element4.toWeakRef());

    group1.append(element1);
    group1.append(element2);

    QCOMPARE(group1.size(), 2);
    QVERIFY( group1.contains(element1));
    QVERIFY( group1.contains(element2));
    QVERIFY(!group1.contains(element3));
    QVERIFY(!group1.contains(element4));

    QCOMPARE(group2.size(), 0);

    group2 = group1;

    QCOMPARE(group2.size(), 2);
    QVERIFY( group2.contains(element1));
    QVERIFY( group2.contains(element2));
    QVERIFY(!group2.contains(element3));
    QVERIFY(!group2.contains(element4));

    group1.append(element3);

    QCOMPARE(group1.size(), 3);
    QVERIFY( group1.contains(element1));
    QVERIFY( group1.contains(element2));
    QVERIFY( group1.contains(element3));
    QVERIFY(!group1.contains(element4));

    QCOMPARE(group2.size(), 2);
    QVERIFY( group2.contains(element1));
    QVERIFY( group2.contains(element2));
    QVERIFY(!group2.contains(element3));
    QVERIFY(!group2.contains(element4));

    group2.append(element4);

    QCOMPARE(group1.size(), 3);
    QVERIFY( group1.contains(element1));
    QVERIFY( group1.contains(element2));
    QVERIFY( group1.contains(element3));
    QVERIFY(!group1.contains(element4));

    QCOMPARE(group2.size(), 3);
    QVERIFY( group2.contains(element1));
    QVERIFY( group2.contains(element2));
    QVERIFY(!group2.contains(element3));
    QVERIFY( group2.contains(element4));
}
