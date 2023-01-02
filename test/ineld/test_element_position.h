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
* This header provides tests for the Ld::ElementPosition class.  Also tests core functions of the
* Ld::Element class.
***********************************************************************************************************************/

#ifndef TEST_ELEMENT_POSITION_H
#define TEST_ELEMENT_POSITION_H

#include <QObject>
#include <QtTest/QtTest>
#include <QString>
#include <QList>

#include <ld_element_structures.h>
#include <ld_data_type.h>
#include <ld_capabilities.h>
#include <ld_element_with_fixed_children.h>

class TestElementPosition:public QObject {
    Q_OBJECT

    public:
        TestElementPosition();

        ~TestElementPosition() override;


    private slots:
        void initTestCase();

        void testConstructorsDestructors();

        void testValidInvalidMethods();

        void testAccessors();

        void testAssignmentOperator();

        void testMovementMethods();

        void testComparisonOperators();

        void testElementStack();
};


class PositionTestElement:public Ld::ElementWithFixedChildren {
    public:
        PositionTestElement(unsigned numberChildren = 0, const QString& identifier = QString());

        ~PositionTestElement() override;

        static Ld::Element* creator(const QString& typeName);

        QString typeName() const final;

        QString plugInName() const final;

        QString description() const final;

        Ld::DataType::ValueType valueType() const final;

        Ld::ElementPointer clone(bool includeChildren) const override;

        Ld::ElementPointer clone(
            unsigned long            startingTextIndex,
            unsigned                 startingRegionIndex,
            unsigned long            endingTextIndex,
            unsigned                 endingRegionIndex,
            bool                     includeChildren
        ) const override;

        Ld::Capabilities parentRequires(unsigned long index) const final;

        Ld::Capabilities childProvidesCapabilities() const final;

        QString identifier() const;

        unsigned minimumNumberTextRegions() const final;

        unsigned maximumNumberTextRegions() const final;

        bool setNumberTextRegions(unsigned newNumberTextRegions) final;

        unsigned numberTextRegions() const final;

        bool setText(const QString& newText, unsigned regionNumber = 0) final;

        QString text(unsigned regionNumber = 0) const final;

    private:
        QString        elementIdentifier;
        QList<QString> textRegions;
};

#endif
