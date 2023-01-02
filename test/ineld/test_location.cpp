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
* This file implements tests of the Ld::Location class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QtTest/QtTest>

#include <cmath>
#include <ld_location.h>

#include "test_location.h"

TestLocation::TestLocation() {}


TestLocation::~TestLocation() {}


void TestLocation::initTestCase() {
    QVERIFY2(Ld::Location::leftToRightOrder() == true, "Left-to-right ordering not default.");
    QVERIFY2(Ld::Location::rightToLeftOrder() == false, "Right-to-left ordering is default.");
}


void TestLocation::testConstructorsAndAssignments() {
    Ld::Location location1;
    Ld::Location location2(1,0);
    Ld::Location location3(location2);

    QVERIFY2(location1.isInvalid(), "Default constructor does not create an invalid location.");
    QVERIFY2(location2 == location3, "Copy constructor did not create valid copy.");

    location1 = location2;
    QVERIFY2(location1 == location2, "Assignment operator didn't copy.");
}


void TestLocation::testBasicMethods() {
    Ld::Location location1;
    Ld::Location location2(2, 3);

    QVERIFY(location1.isInvalid());
    QVERIFY(!location1.isValid());

    QVERIFY(std::isnan(location1.x()));
    QVERIFY(std::isnan(location1.y()));

    QVERIFY(location2.isValid());
    QVERIFY(!location2.isInvalid());

    QVERIFY(location2.x() == 2);
    QVERIFY(location2.y() == 3);

    location1.setPosition(4, 5);

    QVERIFY(location1.x() == 4);
    QVERIFY(location1.y() == 5);
}


void TestLocation::testEqualityChecks() {
    Ld::Location locationI;
    Ld::Location location0(0, 0);
    Ld::Location location1(0, 1);
    Ld::Location location2(1, 0);
    Ld::Location location3(1, 1);
    Ld::Location location4(1, 1);

    Ld::Location locationE(location3);

    QVERIFY2(locationI != locationI, "Invalid locations should always appear to be inequal.");
    QVERIFY2((locationI == locationI) == false, "Invalid locations should never compare as equal.");

    QVERIFY2(location0 != locationI, "Invalid locations should always be inequal to other locations.");
    QVERIFY2((location0 == locationI) == false, "Invalid locations should never be equal to other locations.");

    QVERIFY2(location0 != location1, "Different locations should be inequal (1).");
    QVERIFY2((location0 == location1) == false, "Different locations should never be equal (1).");

    QVERIFY2(location0 != location2, "Different locations should be inequal (2).");
    QVERIFY2((location0 == location2) == false, "Different locations should never be equal (2).");

    QVERIFY2(locationE == location3, "Identical locations should appear equal.");
    QVERIFY2((locationE != location3) == false, "Identical locations should not appear inequal");
}


void TestLocation::testOperatorsLeftToRight() {
    Ld::Location::setLeftToRightOrder();

    Ld::Location location1(0, 0);
    Ld::Location location2(1, 0);
    Ld::Location location3(1, 1);
    Ld::Location location4(0, 1);

    QVERIFY(location1 < location2);
    QVERIFY(location1 < location3);
    QVERIFY(location4 < location3);
    QVERIFY(location1 < location4);

    QVERIFY(location2 > location1);
    QVERIFY(location3 > location1);
    QVERIFY(location3 > location4);
    QVERIFY(location4 > location1);

    QVERIFY(location1 <= location2);
    QVERIFY(location1 <= location3);
    QVERIFY(location4 <= location3);
    QVERIFY(location1 <= location4);

    QVERIFY(location2 >= location1);
    QVERIFY(location3 >= location1);
    QVERIFY(location3 >= location4);
    QVERIFY(location4 >= location1);

}


void TestLocation::testOperatorsRightToLeft() {
    Ld::Location::setRightToLeftOrder();

    Ld::Location location1(1, 0);
    Ld::Location location2(0, 0);
    Ld::Location location3(0, 1);
    Ld::Location location4(1, 1);

    QVERIFY(location1 < location2);
    QVERIFY(location1 < location3);
    QVERIFY(location4 < location3);
    QVERIFY(location1 < location4);

    QVERIFY(location2 > location1);
    QVERIFY(location3 > location1);
    QVERIFY(location3 > location4);
    QVERIFY(location4 > location1);

    QVERIFY(location1 <= location2);
    QVERIFY(location1 <= location3);
    QVERIFY(location4 <= location3);
    QVERIFY(location1 <= location4);

    QVERIFY(location2 >= location1);
    QVERIFY(location3 >= location1);
    QVERIFY(location3 >= location4);
    QVERIFY(location4 >= location1);
}
