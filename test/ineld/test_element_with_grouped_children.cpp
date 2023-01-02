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
* This file implements tests of the Ld::ElementWithGroupedChildren class.
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
#include <ld_visual_with_grouped_children.h>
#include <ld_element_with_grouped_children.h>

#include "test_element_with_grouped_children.h"

/***********************************************************************************************************************
 * GroupedElement:
 */

class GroupedElement:public Ld::ElementWithGroupedChildren {
    public:
        static const char elementName[];

        GroupedElement(const QString& identifer = QString());

        ~GroupedElement() override;

        static Ld::Element* creator(const QString& typeName);

        static QSharedPointer<GroupedElement> create(const QString& identifier = QString());

        static QSharedPointer<GroupedElement> create(
            Ld::VisualWithGroupedChildren* visual,
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

        void removeGroups(
            unsigned                   startingGroupIndex,
            unsigned                   numberGroups,
            Ld::CursorStateCollection* cursorStateCollection
        );

        void insertGroupsBefore(unsigned groupIndex, unsigned numberGroups);

        void insertGroupsAfter(unsigned groupIndex, unsigned numberGroups);

        void writeChildren(
            QSharedPointer<Ld::XmlWriter>       writer,
            QSharedPointer<Ld::FormatOrganizer> formats,
            Ld::ProgramFile&                    programFile,
            const Ld::XmlAttributes&            inheritedAttributes
        ) const override;

        void readChild(
            QSharedPointer<Ld::XmlReader>  reader,
            const QString&                 tagName,
            const Ld::FormatsByIdentifier& formats,
            Ld::ProgramFile&               programFile,
            const Ld::XmlAttributes&       attributes,
            unsigned                       xmlVersion
        ) override;

    private:
        QString elementIdentifier;
};

const char GroupedElement::elementName[] = "GroupedElement";

GroupedElement::GroupedElement(const QString& identifier) {
    elementIdentifier = identifier; // Assists with debugging.
}


GroupedElement::~GroupedElement() {}


Ld::Element* GroupedElement::creator(const QString&) {
    return new GroupedElement;
}


QSharedPointer<GroupedElement> GroupedElement::create(const QString& identifier) {
    QSharedPointer<GroupedElement> element = QSharedPointer<GroupedElement>(new GroupedElement(identifier));
    element->setWeakThis(element.toWeakRef());

    return element;
}


QSharedPointer<GroupedElement> GroupedElement::create(
        Ld::VisualWithGroupedChildren* visual,
        const QString&                 identifier
    ) {
    QSharedPointer<GroupedElement> element = GroupedElement::create(identifier);
    element->setVisual(visual);

    return element;
}


QString GroupedElement::typeName() const {
    return QString("GroupedElement");
}


QString GroupedElement::plugInName() const {
    return QString();
}


QString GroupedElement::description() const {
    return QString();
}


Ld::DataType::ValueType GroupedElement::valueType() const {
    return Ld::DataType::ValueType::NONE;
}


Ld::ElementPointer GroupedElement::clone(bool includeChildren) const {
    Ld::ElementPointer element = ElementWithGroupedChildren::clone(includeChildren);
    element.dynamicCast<GroupedElement>()->elementIdentifier = elementIdentifier;

    return element;
}


Ld::Capabilities GroupedElement::parentRequires(unsigned long) const {
    return Ld::Capabilities();
}


Ld::Capabilities GroupedElement::childProvidesCapabilities() const {
    return Ld::Capabilities();
}


QString GroupedElement::identifier() const {
    return elementIdentifier;
}

void GroupedElement::removeGroups(
        unsigned                   startingGroupIndex,
        unsigned                   numberGroups,
        Ld::CursorStateCollection* cursorStateCollection
    ) {
    Ld::ElementWithGroupedChildren::removeGroups(startingGroupIndex, numberGroups, cursorStateCollection);
}


void GroupedElement::insertGroupsBefore(unsigned groupIndex, unsigned numberGroups) {
    Ld::ElementWithGroupedChildren::insertGroupsBefore(groupIndex, numberGroups);
}


void GroupedElement::insertGroupsAfter(unsigned groupIndex, unsigned numberGroups) {
    Ld::ElementWithGroupedChildren::insertGroupsAfter(groupIndex, numberGroups);
}


void GroupedElement::writeChildren(
        QSharedPointer<Ld::XmlWriter>       writer,
        QSharedPointer<Ld::FormatOrganizer> formats,
        Ld::ProgramFile&                    programFile,
        const Ld::XmlAttributes&            inheritedAttributes
    ) const {
    Ld::ElementWithGroupedChildren::writeChildren(writer, formats, programFile, inheritedAttributes);
}


void GroupedElement::readChild(
        QSharedPointer<Ld::XmlReader>  reader,
        const QString&                 tagName,
        const Ld::FormatsByIdentifier& formats,
        Ld::ProgramFile&               programFile,
        const Ld::XmlAttributes&       attributes,
        unsigned                       xmlVersion
    ) {
    Ld::ElementWithGroupedChildren::readChild(reader, tagName, formats, programFile, attributes, xmlVersion);
}

/***********************************************************************************************************************
 * GroupedVisual:
 */

class GroupedVisual:public Ld::VisualWithGroupedChildren {
    public:
        enum class LastCall: unsigned char {
            NONE = 0,
            ABOUT_TO_REMOVE_CHILD = 1,
            CHILD_INSERTED_BEFORE = 2,
            CHILD_INSERTED_AFTER = 3,
            ABOUT_TO_REMOVE_CHILD_FROM_GROUP = 4,
            ABOUT_TO_REMOVE_CHILDREN_FROM_GROUP = 5,
            CHILD_INSERTED_INTO_GROUP_BEFORE = 6,
            CHILD_INSERTED_INTO_GROUP_AFTER = 7,
            GROUPS_REMOVED = 8,
            GROUPS_INSERTED = 9
        };

        GroupedVisual(const QString& identifer = QString(), bool ignorePositionalElementMessages = true);

        ~GroupedVisual() override;

        QString typeName() const final;

        QString plugInName() const final;

        QString identifier() const;

        void clearCallData();

        LastCall lastCall() const;

        unsigned long lastChildIndex() const;

        unsigned long lastNumberChildren() const;

        Ld::ElementPointer lastElement() const;

        unsigned lastGroupIndex() const;

        unsigned long lastChildIndexInGroup() const;

        unsigned lastStartingGroupIndex() const;

        unsigned lastNumberGroups() const;

        static Ld::Visual* creator(const QString& typeName);

    protected:
        void aboutToRemoveChild(unsigned long childIndex, Ld::ElementPointer childElement) final;

        void childInsertedBefore(unsigned long childIndex, Ld::ElementPointer childElement) final;

        void childInsertedAfter(unsigned long childIndex, Ld::ElementPointer childElement) final;

        void aboutToRemoveChildFromGroup(
            unsigned long      childIndex,
            unsigned           groupIndex,
            unsigned long      childIndexInGroup,
            Ld::ElementPointer childElement
        ) final;

        void aboutToRemoveChildrenFromGroup(
            unsigned long  startingChildIndex,
            unsigned long  numberChildren,
            unsigned       groupIndex
        ) final;

        void childInsertedIntoGroupBefore(
            unsigned long      childIndex,
            unsigned           groupIndex,
            unsigned long      childIndexInGroup,
            Ld::ElementPointer childElement
        ) final;

        void childInsertedIntoGroupAfter(
            unsigned long      childIndex,
            unsigned           groupIndex,
            unsigned long      childIndexInGroup,
            Ld::ElementPointer childElement
        ) final;

        void groupsRemoved(unsigned startingGroupIndex, unsigned numberGroups) final;

        void groupsInserted(unsigned groupIndex, unsigned numberGroups) final;

    private:
        QString                elementIdentifier;

        bool                   currentIgnorePositionalElementMessages;

        LastCall               currentLastCall;

        unsigned long          currentLastIndex;

        unsigned long          currentLastNumberChildren;

        unsigned               currentLastGroupIndex;

        unsigned long          currentLastChildInGroup;

        unsigned               currentLastStartingGroupIndex;

        unsigned               currentLastNumberGroups;

        Ld::ElementWeakPointer currentLastElement;
};

GroupedVisual::GroupedVisual(const QString& identifier, bool ignorePositionalElementMessages) {
    elementIdentifier                      = identifier; // Assists with debugging.
    currentIgnorePositionalElementMessages = ignorePositionalElementMessages;
    clearCallData();
}


GroupedVisual::~GroupedVisual() {}


QString GroupedVisual::typeName() const {
    return QString("GroupedVisual");
}


QString GroupedVisual::plugInName() const {
    return QString();
}


QString GroupedVisual::identifier() const {
    return elementIdentifier;
}


void GroupedVisual::clearCallData() {
    currentLastCall           = LastCall::NONE;
    currentLastIndex          = Ld::Element::invalidChildIndex;
    currentLastNumberChildren = Ld::Element::invalidChildIndex;
    currentLastGroupIndex     = Ld::ElementWithGroupedChildren::invalidGroup;
    currentLastChildInGroup   = Ld::Element::invalidChildIndex;
    currentLastElement        = Ld::ElementPointer();
}


GroupedVisual::LastCall GroupedVisual::lastCall() const {
    return currentLastCall;
}


unsigned long GroupedVisual::lastChildIndex() const {
    return currentLastIndex;
}


unsigned long GroupedVisual::lastNumberChildren() const {
    return currentLastNumberChildren;
}


Ld::ElementPointer GroupedVisual::lastElement() const {
    return currentLastElement.toStrongRef();
}


unsigned GroupedVisual::lastGroupIndex() const {
    return currentLastGroupIndex;
}


unsigned long GroupedVisual::lastChildIndexInGroup() const {
    return currentLastChildInGroup;
}


unsigned GroupedVisual::lastStartingGroupIndex() const {
    return currentLastStartingGroupIndex;
}


unsigned GroupedVisual::lastNumberGroups() const {
    return currentLastNumberGroups;
}


Ld::Visual* GroupedVisual::creator(const QString&) {
    return new GroupedVisual;
}


void GroupedVisual::aboutToRemoveChild(unsigned long childIndex, Ld::ElementPointer childElement) {
    if (!currentIgnorePositionalElementMessages) {
        currentLastCall    = LastCall::ABOUT_TO_REMOVE_CHILD;
        currentLastIndex   = childIndex;
        currentLastElement = childElement;
    }
}


void GroupedVisual::aboutToRemoveChildrenFromGroup(
        unsigned long  startingChildIndex,
        unsigned long  numberChildren,
        unsigned       groupIndex
    ) {
    currentLastCall           = LastCall::ABOUT_TO_REMOVE_CHILDREN_FROM_GROUP;
    currentLastIndex          = startingChildIndex;
    currentLastNumberChildren = numberChildren;
    currentLastGroupIndex     = groupIndex;
}


void GroupedVisual::childInsertedBefore(unsigned long childIndex, Ld::ElementPointer childElement) {
    if (!currentIgnorePositionalElementMessages) {
        currentLastCall    = LastCall::CHILD_INSERTED_BEFORE;
        currentLastIndex   = childIndex;
        currentLastElement = childElement;
    }
}


void GroupedVisual::childInsertedAfter(unsigned long childIndex, Ld::ElementPointer childElement) {
    if (!currentIgnorePositionalElementMessages) {
        currentLastCall    = LastCall::CHILD_INSERTED_AFTER;
        currentLastIndex   = childIndex;
        currentLastElement = childElement;
    }
}


void GroupedVisual::aboutToRemoveChildFromGroup(
        unsigned long      childIndex,
        unsigned           groupIndex,
        unsigned long      childIndexInGroup,
        Ld::ElementPointer childElement
    ) {
    currentLastCall         = LastCall::ABOUT_TO_REMOVE_CHILD_FROM_GROUP;
    currentLastIndex        = childIndex;
    currentLastGroupIndex   = groupIndex;
    currentLastChildInGroup = childIndexInGroup;
    currentLastElement      = childElement;
}


void GroupedVisual::childInsertedIntoGroupBefore(
        unsigned long      childIndex,
        unsigned           groupIndex,
        unsigned long      childIndexInGroup,
    Ld::ElementPointer childElement
    ) {
    currentLastCall         = LastCall::CHILD_INSERTED_INTO_GROUP_BEFORE;
    currentLastIndex        = childIndex;
    currentLastGroupIndex   = groupIndex;
    currentLastChildInGroup = childIndexInGroup;
    currentLastElement      = childElement;
}


void GroupedVisual::childInsertedIntoGroupAfter(
        unsigned long      childIndex,
        unsigned           groupIndex,
        unsigned long      childIndexInGroup,
        Ld::ElementPointer childElement
    ) {
    currentLastCall         = LastCall::CHILD_INSERTED_INTO_GROUP_AFTER;
    currentLastIndex        = childIndex;
    currentLastGroupIndex   = groupIndex;
    currentLastChildInGroup = childIndexInGroup;
    currentLastElement      = childElement;
}


void GroupedVisual::groupsRemoved(unsigned startingGroupIndex, unsigned numberGroups) {
    currentLastCall               = LastCall::GROUPS_REMOVED;
    currentLastStartingGroupIndex = startingGroupIndex;
    currentLastNumberGroups       = numberGroups;
}

void GroupedVisual::groupsInserted(unsigned groupIndex, unsigned numberGroups) {
    currentLastCall         = LastCall::GROUPS_INSERTED;
    currentLastGroupIndex   = groupIndex;
    currentLastNumberGroups = numberGroups;
}

/***********************************************************************************************************************
 * TestElementWithGroupedChildren:
 */

TestElementWithGroupedChildren::TestElementWithGroupedChildren() {}


TestElementWithGroupedChildren::~TestElementWithGroupedChildren() {}


void TestElementWithGroupedChildren::initTestCase() {
    Ld::Handle::initialize(0x12345678UL);
    Ld::Element::registerCreator(GroupedElement::elementName, &GroupedElement::creator);
}


void TestElementWithGroupedChildren::testInsertGroupsBeforeMethod() {
    QSharedPointer<GroupedElement> element = Ld::Element::create(GroupedElement::elementName)
                                             .dynamicCast<GroupedElement>();

    GroupedVisual* visual = new GroupedVisual();
    element->setVisual(visual);

    QCOMPARE(element->numberGroups(), 1U);

    element->insertGroupsBefore(0, 5);

    QCOMPARE(visual->lastCall(), GroupedVisual::LastCall::GROUPS_INSERTED);
    QCOMPARE(visual->lastGroupIndex(), 0U);
    QCOMPARE(visual->lastNumberGroups(), 5U);

    QCOMPARE(element->numberGroups(), 6U);

    QSharedPointer<GroupedElement> child1 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(2, 0, child1, nullptr);

    QCOMPARE(element->numberChildren(), 1U);
    QCOMPARE(element->numberChildrenInGroup(0), 0U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 1U);
    QCOMPARE(element->numberChildrenInGroup(3), 0U);
    QCOMPARE(element->numberChildrenInGroup(4), 0U);
    QCOMPARE(element->numberChildrenInGroup(5), 0U);

    element->insertGroupsBefore(2, 2);

    QCOMPARE(visual->lastCall(), GroupedVisual::LastCall::GROUPS_INSERTED);
    QCOMPARE(visual->lastGroupIndex(), 2U);
    QCOMPARE(visual->lastNumberGroups(), 2U);

    QCOMPARE(element->numberGroups(), 8U);

    QCOMPARE(element->numberChildrenInGroup(0), 0U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 0U);
    QCOMPARE(element->numberChildrenInGroup(3), 0U);
    QCOMPARE(element->numberChildrenInGroup(4), 1U);
    QCOMPARE(element->numberChildrenInGroup(5), 0U);
    QCOMPARE(element->numberChildrenInGroup(6), 0U);
    QCOMPARE(element->numberChildrenInGroup(7), 0U);

    element->insertGroupsBefore(5, 1);
    QCOMPARE(element->numberGroups(), 9U);

    QCOMPARE(element->numberChildrenInGroup(0), 0U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 0U);
    QCOMPARE(element->numberChildrenInGroup(3), 0U);
    QCOMPARE(element->numberChildrenInGroup(4), 1U);
    QCOMPARE(element->numberChildrenInGroup(5), 0U);
    QCOMPARE(element->numberChildrenInGroup(6), 0U);
    QCOMPARE(element->numberChildrenInGroup(7), 0U);
    QCOMPARE(element->numberChildrenInGroup(8), 0U);

    delete visual;
}


void TestElementWithGroupedChildren::testInsertGroupsAfterMethod() {
    QSharedPointer<GroupedElement> element = Ld::Element::create(GroupedElement::elementName)
                                             .dynamicCast<GroupedElement>();

    GroupedVisual* visual = new GroupedVisual();
    element->setVisual(visual);

    QCOMPARE(element->numberGroups(), 1U);

    visual->clearCallData();
    element->insertGroupsAfter(0, 5);

    QCOMPARE(visual->lastCall(), GroupedVisual::LastCall::GROUPS_INSERTED);
    QCOMPARE(visual->lastGroupIndex(), 1U);
    QCOMPARE(visual->lastNumberGroups(), 5U);

    QCOMPARE(element->numberGroups(), 6U);

    QSharedPointer<GroupedElement> child1 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(2, 0, child1, nullptr);

    QCOMPARE(element->numberChildren(), 1U);
    QCOMPARE(element->numberChildrenInGroup(0), 0U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 1U);
    QCOMPARE(element->numberChildrenInGroup(3), 0U);
    QCOMPARE(element->numberChildrenInGroup(4), 0U);
    QCOMPARE(element->numberChildrenInGroup(5), 0U);

    visual->clearCallData();
    element->insertGroupsAfter(1, 2);

    QCOMPARE(visual->lastCall(), GroupedVisual::LastCall::GROUPS_INSERTED);
    QCOMPARE(visual->lastGroupIndex(), 2U);
    QCOMPARE(visual->lastNumberGroups(), 2U);

    QCOMPARE(element->numberGroups(), 8U);

    QCOMPARE(element->numberChildrenInGroup(0), 0U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 0U);
    QCOMPARE(element->numberChildrenInGroup(3), 0U);
    QCOMPARE(element->numberChildrenInGroup(4), 1U);
    QCOMPARE(element->numberChildrenInGroup(5), 0U);
    QCOMPARE(element->numberChildrenInGroup(6), 0U);
    QCOMPARE(element->numberChildrenInGroup(7), 0U);

    visual->clearCallData();
    element->insertGroupsAfter(4, 1);
    QCOMPARE(element->numberGroups(), 9U);

    QCOMPARE(element->numberChildrenInGroup(0), 0U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 0U);
    QCOMPARE(element->numberChildrenInGroup(3), 0U);
    QCOMPARE(element->numberChildrenInGroup(4), 1U);
    QCOMPARE(element->numberChildrenInGroup(5), 0U);
    QCOMPARE(element->numberChildrenInGroup(6), 0U);
    QCOMPARE(element->numberChildrenInGroup(7), 0U);
    QCOMPARE(element->numberChildrenInGroup(8), 0U);

    delete visual;
}


void TestElementWithGroupedChildren::testRemoveGroupsMethod() {
    QSharedPointer<GroupedElement> element = Ld::Element::create(GroupedElement::elementName)
                                             .dynamicCast<GroupedElement>();

    GroupedVisual* visual = new GroupedVisual();
    element->setVisual(visual);

    QCOMPARE(element->numberGroups(), 1U);

    element->insertGroupsBefore(0, 9);
    QCOMPARE(element->numberGroups(), 10U);

    QSharedPointer<GroupedElement> child1 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(2, 0, child1, nullptr);

    QSharedPointer<GroupedElement> child2 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(2, 0, child2, nullptr);

    QSharedPointer<GroupedElement> child3 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(2, 0, child3, nullptr);

    QSharedPointer<GroupedElement> child4 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(7, 0, child4, nullptr);

    QSharedPointer<GroupedElement> child5 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(9, 0, child5, nullptr);

    QSharedPointer<GroupedElement> child6 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(9, 0, child6, nullptr);

    QCOMPARE(element->numberChildren(), 6U);
    QCOMPARE(element->numberChildrenInGroup(0), 0U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 3U);
    QCOMPARE(element->numberChildrenInGroup(3), 0U);
    QCOMPARE(element->numberChildrenInGroup(4), 0U);
    QCOMPARE(element->numberChildrenInGroup(5), 0U);
    QCOMPARE(element->numberChildrenInGroup(6), 0U);
    QCOMPARE(element->numberChildrenInGroup(7), 1U);
    QCOMPARE(element->numberChildrenInGroup(8), 0U);
    QCOMPARE(element->numberChildrenInGroup(9), 2U);

    visual->clearCallData();
    element->removeGroups(4, 2, nullptr);

    QCOMPARE(visual->lastCall(), GroupedVisual::LastCall::GROUPS_REMOVED);
    QCOMPARE(visual->lastStartingGroupIndex(), 4U);
    QCOMPARE(visual->lastNumberGroups(), 2U);

    QCOMPARE(element->numberGroups(), 8U);
    QCOMPARE(element->numberChildren(), 6U);
    QCOMPARE(element->numberChildrenInGroup(0), 0U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 3U);
    QCOMPARE(element->numberChildrenInGroup(3), 0U);
    QCOMPARE(element->numberChildrenInGroup(4), 0U);
    QCOMPARE(element->numberChildrenInGroup(5), 1U);
    QCOMPARE(element->numberChildrenInGroup(6), 0U);
    QCOMPARE(element->numberChildrenInGroup(7), 2U);

    visual->clearCallData();
    element->removeGroups(6, 2, nullptr);

    QCOMPARE(visual->lastCall(), GroupedVisual::LastCall::GROUPS_REMOVED);
    QCOMPARE(visual->lastStartingGroupIndex(), 6U);
    QCOMPARE(visual->lastNumberGroups(), 2U);

    QCOMPARE(element->numberGroups(), 6U);
    QCOMPARE(element->numberChildren(), 4U);
    QCOMPARE(element->numberChildrenInGroup(0), 0U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 3U);
    QCOMPARE(element->numberChildrenInGroup(3), 0U);
    QCOMPARE(element->numberChildrenInGroup(4), 0U);
    QCOMPARE(element->numberChildrenInGroup(5), 1U);

    delete visual;
}


void TestElementWithGroupedChildren::testInsertIntoGroupBeforeMethod() {
    QSharedPointer<GroupedElement> element = Ld::Element::create(GroupedElement::elementName)
                                             .dynamicCast<GroupedElement>();

    GroupedVisual* visual = new GroupedVisual();
    element->setVisual(visual);

    element->insertGroupsBefore(0, 3);
    QCOMPARE(element->numberGroups(), 4U);

    QSharedPointer<GroupedElement> child1 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    visual->clearCallData();
    element->insertIntoGroupBefore(0, 0, child1, nullptr);

    QCOMPARE(visual->lastCall(), GroupedVisual::LastCall::CHILD_INSERTED_INTO_GROUP_BEFORE);
    QCOMPARE(visual->lastGroupIndex(), 0U);
    QCOMPARE(visual->lastChildIndexInGroup(), 0U);
    QCOMPARE(visual->lastElement(), child1);

    QCOMPARE(element->numberChildren(), 1U);
    QCOMPARE(element->numberChildrenInGroup(0), 1U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 0U);
    QCOMPARE(element->numberChildrenInGroup(3), 0U);

    QSharedPointer<GroupedElement> child2 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    visual->clearCallData();
    element->insertIntoGroupBefore(0, 0, child2, nullptr);

    QCOMPARE(visual->lastCall(), GroupedVisual::LastCall::CHILD_INSERTED_INTO_GROUP_BEFORE);
    QCOMPARE(visual->lastGroupIndex(), 0U);
    QCOMPARE(visual->lastChildIndexInGroup(), 0U);
    QCOMPARE(visual->lastElement(), child2);

    QCOMPARE(element->numberChildren(), 2U);
    QCOMPARE(element->numberChildrenInGroup(0), 2U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 0U);
    QCOMPARE(element->numberChildrenInGroup(3), 0U);

    QCOMPARE(element->childInGroup(0, 0), child2);
    QCOMPARE(element->childInGroup(0, 1), child1);
    QVERIFY(element->childInGroup(0, 2).isNull());

    QSharedPointer<GroupedElement> child3 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    visual->clearCallData();
    element->insertIntoGroupBefore(1, 1, child3, nullptr);

    QCOMPARE(visual->lastCall(), GroupedVisual::LastCall::CHILD_INSERTED_INTO_GROUP_BEFORE);
    QCOMPARE(visual->lastGroupIndex(), 1U);
    QCOMPARE(visual->lastChildIndexInGroup(), 0U);
    QCOMPARE(visual->lastElement(), child3);

    QCOMPARE(element->numberChildren(), 3U);
    QCOMPARE(element->numberChildrenInGroup(0), 2U);
    QCOMPARE(element->numberChildrenInGroup(1), 1U);
    QCOMPARE(element->numberChildrenInGroup(2), 0U);
    QCOMPARE(element->numberChildrenInGroup(3), 0U);

    QCOMPARE(element->childInGroup(0, 0), child2);
    QCOMPARE(element->childInGroup(0, 1), child1);
    QCOMPARE(element->childInGroup(1, 0), child3);
    QVERIFY(element->childInGroup(1, 1).isNull());

    QSharedPointer<GroupedElement> child4 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    visual->clearCallData();
    element->insertIntoGroupBefore(3, 0, child4, nullptr);

    QCOMPARE(visual->lastCall(), GroupedVisual::LastCall::CHILD_INSERTED_INTO_GROUP_BEFORE);
    QCOMPARE(visual->lastGroupIndex(), 3U);
    QCOMPARE(visual->lastChildIndexInGroup(), 0U);
    QCOMPARE(visual->lastElement(), child4);

    QCOMPARE(element->numberChildren(), 4U);
    QCOMPARE(element->numberChildrenInGroup(0), 2U);
    QCOMPARE(element->numberChildrenInGroup(1), 1U);
    QCOMPARE(element->numberChildrenInGroup(2), 0U);
    QCOMPARE(element->numberChildrenInGroup(3), 1U);

    QCOMPARE(element->childInGroup(0, 0), child2);
    QCOMPARE(element->childInGroup(0, 1), child1);
    QVERIFY(element->childInGroup(0, 2).isNull());
    QCOMPARE(element->childInGroup(1, 0), child3);
    QVERIFY(element->childInGroup(1, 1).isNull());
    QVERIFY(element->childInGroup(2, 0).isNull());
    QCOMPARE(element->childInGroup(3, 0), child4);
    QVERIFY(element->childInGroup(3, 1).isNull());

    QSharedPointer<GroupedElement> child5 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    visual->clearCallData();
    element->insertIntoGroupBefore(3, 1, child5, nullptr);

    QCOMPARE(visual->lastCall(), GroupedVisual::LastCall::CHILD_INSERTED_INTO_GROUP_BEFORE);
    QCOMPARE(visual->lastGroupIndex(), 3U);
    QCOMPARE(visual->lastChildIndexInGroup(), 1U);
    QCOMPARE(visual->lastElement(), child5);

    QCOMPARE(element->numberChildren(), 5U);
    QCOMPARE(element->numberChildrenInGroup(0), 2U);
    QCOMPARE(element->numberChildrenInGroup(1), 1U);
    QCOMPARE(element->numberChildrenInGroup(2), 0U);
    QCOMPARE(element->numberChildrenInGroup(3), 2U);

    QCOMPARE(element->childInGroup(0, 0), child2);
    QCOMPARE(element->childInGroup(0, 1), child1);
    QVERIFY(element->childInGroup(0, 2).isNull());
    QCOMPARE(element->childInGroup(1, 0), child3);
    QVERIFY(element->childInGroup(1, 1).isNull());
    QVERIFY(element->childInGroup(2, 0).isNull());
    QCOMPARE(element->childInGroup(3, 0), child4);
    QCOMPARE(element->childInGroup(3, 1), child5);
    QVERIFY(element->childInGroup(3, 2).isNull());

    Ld::ElementPointerList group0 = element->childrenInGroup(0);
    Ld::ElementPointerList group1 = element->childrenInGroup(1);
    Ld::ElementPointerList group2 = element->childrenInGroup(2);
    Ld::ElementPointerList group3 = element->childrenInGroup(3);

    QCOMPARE(group0.size(), 2);
    QCOMPARE(group1.size(), 1);
    QCOMPARE(group2.size(), 0);
    QCOMPARE(group3.size(), 2);

    QCOMPARE(group0.at(0), child2);
    QCOMPARE(group0.at(1), child1);

    QCOMPARE(group1.at(0), child3);

    QCOMPARE(group3.at(0), child4);
    QCOMPARE(group3.at(1), child5);

    delete visual;
}


void TestElementWithGroupedChildren::testInsertIntoGroupAfterMethod() {
    QSharedPointer<GroupedElement> element = Ld::Element::create(GroupedElement::elementName)
                                             .dynamicCast<GroupedElement>();

    GroupedVisual* visual = new GroupedVisual();
    element->setVisual(visual);

    element->insertGroupsBefore(0, 3);
    QCOMPARE(element->numberGroups(), 4U);

    QSharedPointer<GroupedElement> child1 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    visual->clearCallData();
    element->insertIntoGroupAfter(0, 0, child1, nullptr);

    QCOMPARE(visual->lastCall(), GroupedVisual::LastCall::CHILD_INSERTED_INTO_GROUP_AFTER);
    QCOMPARE(visual->lastGroupIndex(), 0U);
    QCOMPARE(visual->lastChildIndexInGroup(), Ld::Element::invalidChildIndex);
    QCOMPARE(visual->lastElement(), child1);

    QCOMPARE(element->numberChildren(), 1U);
    QCOMPARE(element->numberChildrenInGroup(0), 1U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 0U);
    QCOMPARE(element->numberChildrenInGroup(3), 0U);

    QSharedPointer<GroupedElement> child2 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    visual->clearCallData();
    element->insertIntoGroupAfter(0, 0, child2, nullptr);

    QCOMPARE(visual->lastCall(), GroupedVisual::LastCall::CHILD_INSERTED_INTO_GROUP_AFTER);
    QCOMPARE(visual->lastGroupIndex(), 0U);
    QCOMPARE(visual->lastChildIndexInGroup(), 0U);
    QCOMPARE(visual->lastElement(), child2);

    QCOMPARE(element->numberChildren(), 2U);
    QCOMPARE(element->numberChildrenInGroup(0), 2U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 0U);
    QCOMPARE(element->numberChildrenInGroup(3), 0U);

    QCOMPARE(element->childInGroup(0, 0), child1);
    QCOMPARE(element->childInGroup(0, 1), child2);
    QVERIFY(element->childInGroup(0, 2).isNull());

    QSharedPointer<GroupedElement> child3 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    visual->clearCallData();
    element->insertIntoGroupAfter(1, 1, child3, nullptr);

    QCOMPARE(visual->lastCall(), GroupedVisual::LastCall::CHILD_INSERTED_INTO_GROUP_AFTER);
    QCOMPARE(visual->lastGroupIndex(), 1U);
    QCOMPARE(visual->lastChildIndexInGroup(), Ld::Element::invalidChildIndex);
    QCOMPARE(visual->lastElement(), child3);

    QCOMPARE(element->numberChildren(), 3U);
    QCOMPARE(element->numberChildrenInGroup(0), 2U);
    QCOMPARE(element->numberChildrenInGroup(1), 1U);
    QCOMPARE(element->numberChildrenInGroup(2), 0U);
    QCOMPARE(element->numberChildrenInGroup(3), 0U);

    QCOMPARE(element->childInGroup(0, 0), child1);
    QCOMPARE(element->childInGroup(0, 1), child2);
    QCOMPARE(element->childInGroup(1, 0), child3);
    QVERIFY(element->childInGroup(1, 1).isNull());

    QSharedPointer<GroupedElement> child4 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    visual->clearCallData();
    element->insertIntoGroupAfter(3, 0, child4, nullptr);

    QCOMPARE(visual->lastCall(), GroupedVisual::LastCall::CHILD_INSERTED_INTO_GROUP_AFTER);
    QCOMPARE(visual->lastGroupIndex(), 3U);
    QCOMPARE(visual->lastChildIndexInGroup(), Ld::Element::invalidChildIndex);
    QCOMPARE(visual->lastElement(), child4);

    QCOMPARE(element->numberChildren(), 4U);
    QCOMPARE(element->numberChildrenInGroup(0), 2U);
    QCOMPARE(element->numberChildrenInGroup(1), 1U);
    QCOMPARE(element->numberChildrenInGroup(2), 0U);
    QCOMPARE(element->numberChildrenInGroup(3), 1U);

    QCOMPARE(element->childInGroup(0, 0), child1);
    QCOMPARE(element->childInGroup(0, 1), child2);
    QVERIFY(element->childInGroup(0, 2).isNull());
    QCOMPARE(element->childInGroup(1, 0), child3);
    QVERIFY(element->childInGroup(1, 1).isNull());
    QVERIFY(element->childInGroup(2, 0).isNull());
    QCOMPARE(element->childInGroup(3, 0), child4);
    QVERIFY(element->childInGroup(3, 1).isNull());

    QSharedPointer<GroupedElement> child5 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    visual->clearCallData();
    element->insertIntoGroupAfter(3, 1, child5, nullptr);

    QCOMPARE(visual->lastCall(), GroupedVisual::LastCall::CHILD_INSERTED_INTO_GROUP_AFTER);
    QCOMPARE(visual->lastGroupIndex(), 3U);
    QCOMPARE(visual->lastChildIndexInGroup(), 0U);
    QCOMPARE(visual->lastElement(), child5);

    QCOMPARE(element->numberChildren(), 5U);
    QCOMPARE(element->numberChildrenInGroup(0), 2U);
    QCOMPARE(element->numberChildrenInGroup(1), 1U);
    QCOMPARE(element->numberChildrenInGroup(2), 0U);
    QCOMPARE(element->numberChildrenInGroup(3), 2U);

    QCOMPARE(element->childInGroup(0, 0), child1);
    QCOMPARE(element->childInGroup(0, 1), child2);
    QVERIFY(element->childInGroup(0, 2).isNull());
    QCOMPARE(element->childInGroup(1, 0), child3);
    QVERIFY(element->childInGroup(1, 1).isNull());
    QVERIFY(element->childInGroup(2, 0).isNull());
    QCOMPARE(element->childInGroup(3, 0), child4);
    QCOMPARE(element->childInGroup(3, 1), child5);
    QVERIFY(element->childInGroup(3, 2).isNull());

    Ld::ElementPointerList group0 = element->childrenInGroup(0);
    Ld::ElementPointerList group1 = element->childrenInGroup(1);
    Ld::ElementPointerList group2 = element->childrenInGroup(2);
    Ld::ElementPointerList group3 = element->childrenInGroup(3);

    QCOMPARE(group0.size(), 2);
    QCOMPARE(group1.size(), 1);
    QCOMPARE(group2.size(), 0);
    QCOMPARE(group3.size(), 2);

    QCOMPARE(group0.at(0), child1);
    QCOMPARE(group0.at(1), child2);

    QCOMPARE(group1.at(0), child3);

    QCOMPARE(group3.at(0), child4);
    QCOMPARE(group3.at(1), child5);

    delete visual;
}


void TestElementWithGroupedChildren::testRemoveChildFromGroupMethod() {
    QSharedPointer<GroupedElement> element = Ld::Element::create(GroupedElement::elementName)
                                             .dynamicCast<GroupedElement>();

    GroupedVisual* visual = new GroupedVisual();
    element->setVisual(visual);

    QCOMPARE(element->numberGroups(), 1U);

    element->insertGroupsBefore(0, 9);
    QCOMPARE(element->numberGroups(), 10U);

    QSharedPointer<GroupedElement> child1 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(2, 0, child1, nullptr);

    QSharedPointer<GroupedElement> child2 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(2, 0, child2, nullptr);

    QSharedPointer<GroupedElement> child3 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(2, 0, child3, nullptr);

    QSharedPointer<GroupedElement> child4 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(3, 0, child4, nullptr);

    QSharedPointer<GroupedElement> child5 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(4, 0, child5, nullptr);

    QSharedPointer<GroupedElement> child6 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(7, 0, child6, nullptr);

    QSharedPointer<GroupedElement> child7 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(9, 0, child7, nullptr);

    QSharedPointer<GroupedElement> child8 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(9, 0, child8, nullptr);

    QCOMPARE(element->numberChildren(), 8U);
    QCOMPARE(element->numberChildrenInGroup(0), 0U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 3U);
    QCOMPARE(element->numberChildrenInGroup(3), 1U);
    QCOMPARE(element->numberChildrenInGroup(4), 1U);
    QCOMPARE(element->numberChildrenInGroup(5), 0U);
    QCOMPARE(element->numberChildrenInGroup(6), 0U);
    QCOMPARE(element->numberChildrenInGroup(7), 1U);
    QCOMPARE(element->numberChildrenInGroup(8), 0U);
    QCOMPARE(element->numberChildrenInGroup(9), 2U);

    visual->clearCallData();
    bool success = element->removeChildFromGroup(2, 1, nullptr);
    QVERIFY(success);

    QCOMPARE(visual->lastCall(), GroupedVisual::LastCall::ABOUT_TO_REMOVE_CHILD_FROM_GROUP);
    QCOMPARE(visual->lastChildIndex(), 1U);
    QCOMPARE(visual->lastGroupIndex(), 2U);
    QCOMPARE(visual->lastChildIndexInGroup(), 1U);
    QCOMPARE(visual->lastElement(), child2);

    QCOMPARE(element->numberChildren(), 7U);
    Ld::ElementPointerList group0 = element->childrenInGroup(0);
    Ld::ElementPointerList group1 = element->childrenInGroup(1);
    Ld::ElementPointerList group2 = element->childrenInGroup(2);
    Ld::ElementPointerList group3 = element->childrenInGroup(3);
    Ld::ElementPointerList group4 = element->childrenInGroup(4);
    Ld::ElementPointerList group5 = element->childrenInGroup(5);
    Ld::ElementPointerList group6 = element->childrenInGroup(6);
    Ld::ElementPointerList group7 = element->childrenInGroup(7);
    Ld::ElementPointerList group8 = element->childrenInGroup(8);
    Ld::ElementPointerList group9 = element->childrenInGroup(9);

    QCOMPARE(group0.size(), 0);
    QCOMPARE(group1.size(), 0);
    QCOMPARE(group2.size(), 2);
    QCOMPARE(group3.size(), 1);
    QCOMPARE(group4.size(), 1);
    QCOMPARE(group5.size(), 0);
    QCOMPARE(group6.size(), 0);
    QCOMPARE(group7.size(), 1);
    QCOMPARE(group8.size(), 0);
    QCOMPARE(group9.size(), 2);

    QCOMPARE(group2.at(0), child3);
    QCOMPARE(group2.at(1), child1);
    QCOMPARE(group3.at(0), child4);
    QCOMPARE(group4.at(0), child5);
    QCOMPARE(group7.at(0), child6);
    QCOMPARE(group9.at(0), child8);
    QCOMPARE(group9.at(1), child7);

    visual->clearCallData();
    success = element->removeChildFromGroup(9, 1, nullptr);
    QVERIFY(success);

    QCOMPARE(element->numberChildren(), 6U);
    group0 = element->childrenInGroup(0);
    group1 = element->childrenInGroup(1);
    group2 = element->childrenInGroup(2);
    group3 = element->childrenInGroup(3);
    group4 = element->childrenInGroup(4);
    group5 = element->childrenInGroup(5);
    group6 = element->childrenInGroup(6);
    group7 = element->childrenInGroup(7);
    group8 = element->childrenInGroup(8);
    group9 = element->childrenInGroup(9);

    QCOMPARE(group0.size(), 0);
    QCOMPARE(group1.size(), 0);
    QCOMPARE(group2.size(), 2);
    QCOMPARE(group3.size(), 1);
    QCOMPARE(group4.size(), 1);
    QCOMPARE(group5.size(), 0);
    QCOMPARE(group6.size(), 0);
    QCOMPARE(group7.size(), 1);
    QCOMPARE(group8.size(), 0);
    QCOMPARE(group9.size(), 1);

    QCOMPARE(group2.at(0), child3);
    QCOMPARE(group2.at(1), child1);
    QCOMPARE(group3.at(0), child4);
    QCOMPARE(group4.at(0), child5);
    QCOMPARE(group7.at(0), child6);
    QCOMPARE(group9.at(0), child8);

    delete visual;
}


void TestElementWithGroupedChildren::testRemoveChildrenFromGroupMethod() {
    QSharedPointer<GroupedElement> element = Ld::Element::create(GroupedElement::elementName)
                                             .dynamicCast<GroupedElement>();

    GroupedVisual* visual = new GroupedVisual();
    element->setVisual(visual);

    QCOMPARE(element->numberGroups(), 1U);

    element->insertGroupsBefore(0, 9);
    QCOMPARE(element->numberGroups(), 10U);

    QSharedPointer<GroupedElement> child1 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(2, 0, child1, nullptr);

    QSharedPointer<GroupedElement> child2 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(2, 0, child2, nullptr);

    QSharedPointer<GroupedElement> child3 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(2, 0, child3, nullptr);

    QSharedPointer<GroupedElement> child4 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(3, 0, child4, nullptr);

    QSharedPointer<GroupedElement> child5 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(4, 0, child5, nullptr);

    QSharedPointer<GroupedElement> child6 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(7, 0, child6, nullptr);

    QSharedPointer<GroupedElement> child7 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(9, 0, child7, nullptr);

    QSharedPointer<GroupedElement> child8 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(9, 0, child8, nullptr);

    QCOMPARE(element->numberChildren(), 8U);
    QCOMPARE(element->numberChildrenInGroup(0), 0U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 3U);
    QCOMPARE(element->numberChildrenInGroup(3), 1U);
    QCOMPARE(element->numberChildrenInGroup(4), 1U);
    QCOMPARE(element->numberChildrenInGroup(5), 0U);
    QCOMPARE(element->numberChildrenInGroup(6), 0U);
    QCOMPARE(element->numberChildrenInGroup(7), 1U);
    QCOMPARE(element->numberChildrenInGroup(8), 0U);
    QCOMPARE(element->numberChildrenInGroup(9), 2U);

    visual->clearCallData();
    bool success = element->removeChildrenFromGroup(7, nullptr);
    QVERIFY(success);

    QCOMPARE(visual->lastCall(), GroupedVisual::LastCall::ABOUT_TO_REMOVE_CHILDREN_FROM_GROUP);
    QCOMPARE(visual->lastChildIndex(), 5U);
    QCOMPARE(visual->lastNumberChildren(), 1U);
    QCOMPARE(visual->lastGroupIndex(), 7U);

    QCOMPARE(element->numberChildren(), 7U);
    Ld::ElementPointerList group0 = element->childrenInGroup(0);
    Ld::ElementPointerList group1 = element->childrenInGroup(1);
    Ld::ElementPointerList group2 = element->childrenInGroup(2);
    Ld::ElementPointerList group3 = element->childrenInGroup(3);
    Ld::ElementPointerList group4 = element->childrenInGroup(4);
    Ld::ElementPointerList group5 = element->childrenInGroup(5);
    Ld::ElementPointerList group6 = element->childrenInGroup(6);
    Ld::ElementPointerList group7 = element->childrenInGroup(7);
    Ld::ElementPointerList group8 = element->childrenInGroup(8);
    Ld::ElementPointerList group9 = element->childrenInGroup(9);

    QCOMPARE(group0.size(), 0);
    QCOMPARE(group1.size(), 0);
    QCOMPARE(group2.size(), 3);
    QCOMPARE(group3.size(), 1);
    QCOMPARE(group4.size(), 1);
    QCOMPARE(group5.size(), 0);
    QCOMPARE(group6.size(), 0);
    QCOMPARE(group7.size(), 0);
    QCOMPARE(group8.size(), 0);
    QCOMPARE(group9.size(), 2);

    QCOMPARE(group2.at(0), child3);
    QCOMPARE(group2.at(1), child2);
    QCOMPARE(group2.at(2), child1);
    QCOMPARE(group3.at(0), child4);
    QCOMPARE(group4.at(0), child5);
    QCOMPARE(group9.at(0), child8);
    QCOMPARE(group9.at(1), child7);

    visual->clearCallData();
    success = element->removeChildrenFromGroup(2, nullptr);
    QVERIFY(success);

    QCOMPARE(visual->lastCall(), GroupedVisual::LastCall::ABOUT_TO_REMOVE_CHILDREN_FROM_GROUP);
    QCOMPARE(visual->lastChildIndex(), 0U);
    QCOMPARE(visual->lastNumberChildren(), 3U);
    QCOMPARE(visual->lastGroupIndex(), 2U);

    QCOMPARE(element->numberChildren(), 4U);
    group0 = element->childrenInGroup(0);
    group1 = element->childrenInGroup(1);
    group2 = element->childrenInGroup(2);
    group3 = element->childrenInGroup(3);
    group4 = element->childrenInGroup(4);
    group5 = element->childrenInGroup(5);
    group6 = element->childrenInGroup(6);
    group7 = element->childrenInGroup(7);
    group8 = element->childrenInGroup(8);
    group9 = element->childrenInGroup(9);

    QCOMPARE(group0.size(), 0);
    QCOMPARE(group1.size(), 0);
    QCOMPARE(group2.size(), 0);
    QCOMPARE(group3.size(), 1);
    QCOMPARE(group4.size(), 1);
    QCOMPARE(group5.size(), 0);
    QCOMPARE(group6.size(), 0);
    QCOMPARE(group7.size(), 0);
    QCOMPARE(group8.size(), 0);
    QCOMPARE(group9.size(), 2);

    QCOMPARE(group3.at(0), child4);
    QCOMPARE(group4.at(0), child5);
    QCOMPARE(group9.at(0), child8);
    QCOMPARE(group9.at(1), child7);

    delete visual;
}


void TestElementWithGroupedChildren::testRemoveChildMethod() {
    QSharedPointer<GroupedElement> element = Ld::Element::create(GroupedElement::elementName)
                                             .dynamicCast<GroupedElement>();

    GroupedVisual* visual = new GroupedVisual(QString(), false);
    element->setVisual(visual);

    QCOMPARE(element->numberGroups(), 1U);

    element->insertGroupsBefore(0, 9);
    QCOMPARE(element->numberGroups(), 10U);

    QSharedPointer<GroupedElement> child1 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(2, 0, child1, nullptr);

    QSharedPointer<GroupedElement> child2 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(2, 0, child2, nullptr);

    QSharedPointer<GroupedElement> child3 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(2, 0, child3, nullptr);

    QSharedPointer<GroupedElement> child4 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(3, 0, child4, nullptr);

    QSharedPointer<GroupedElement> child5 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(4, 0, child5, nullptr);

    QSharedPointer<GroupedElement> child6 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(9, 0, child6, nullptr);

    QSharedPointer<GroupedElement> child7 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(9, 0, child7, nullptr);

    QSharedPointer<GroupedElement> child8 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(9, 0, child8, nullptr);

    QCOMPARE(element->numberChildren(), 8U);
    QCOMPARE(element->numberChildrenInGroup(0), 0U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 3U);
    QCOMPARE(element->numberChildrenInGroup(3), 1U);
    QCOMPARE(element->numberChildrenInGroup(4), 1U);
    QCOMPARE(element->numberChildrenInGroup(5), 0U);
    QCOMPARE(element->numberChildrenInGroup(6), 0U);
    QCOMPARE(element->numberChildrenInGroup(7), 0U);
    QCOMPARE(element->numberChildrenInGroup(8), 0U);
    QCOMPARE(element->numberChildrenInGroup(9), 3U);

    visual->clearCallData();
    bool success = element->removeChild(1, nullptr); // child2
    QVERIFY(success);

    QCOMPARE(visual->lastCall(), GroupedVisual::LastCall::ABOUT_TO_REMOVE_CHILD);
    QCOMPARE(visual->lastChildIndex(), 1U);
    QCOMPARE(visual->lastElement(), child2);

    QCOMPARE(element->numberChildren(), 7U);
    Ld::ElementPointerList group0 = element->childrenInGroup(0);
    Ld::ElementPointerList group1 = element->childrenInGroup(1);
    Ld::ElementPointerList group2 = element->childrenInGroup(2);
    Ld::ElementPointerList group3 = element->childrenInGroup(3);
    Ld::ElementPointerList group4 = element->childrenInGroup(4);
    Ld::ElementPointerList group5 = element->childrenInGroup(5);
    Ld::ElementPointerList group6 = element->childrenInGroup(6);
    Ld::ElementPointerList group7 = element->childrenInGroup(7);
    Ld::ElementPointerList group8 = element->childrenInGroup(8);
    Ld::ElementPointerList group9 = element->childrenInGroup(9);

    QCOMPARE(group0.size(), 0);
    QCOMPARE(group1.size(), 0);
    QCOMPARE(group2.size(), 2);
    QCOMPARE(group3.size(), 1);
    QCOMPARE(group4.size(), 1);
    QCOMPARE(group5.size(), 0);
    QCOMPARE(group6.size(), 0);
    QCOMPARE(group7.size(), 0);
    QCOMPARE(group8.size(), 0);
    QCOMPARE(group9.size(), 3);

    QCOMPARE(group2.at(0), child3);
    QCOMPARE(group2.at(1), child1);
    QCOMPARE(group3.at(0), child4);
    QCOMPARE(group4.at(0), child5);
    QCOMPARE(group9.at(0), child8);
    QCOMPARE(group9.at(1), child7);
    QCOMPARE(group9.at(2), child6);

    visual->clearCallData();
    success = element->removeChild(4, nullptr); // child8
    QVERIFY(success);

    QCOMPARE(visual->lastCall(), GroupedVisual::LastCall::ABOUT_TO_REMOVE_CHILD);
    QCOMPARE(visual->lastChildIndex(), 4U);
    QCOMPARE(visual->lastElement(), child8);

    QCOMPARE(element->numberChildren(), 6U);
    group0 = element->childrenInGroup(0);
    group1 = element->childrenInGroup(1);
    group2 = element->childrenInGroup(2);
    group3 = element->childrenInGroup(3);
    group4 = element->childrenInGroup(4);
    group5 = element->childrenInGroup(5);
    group6 = element->childrenInGroup(6);
    group7 = element->childrenInGroup(7);
    group8 = element->childrenInGroup(8);
    group9 = element->childrenInGroup(9);

    QCOMPARE(group0.size(), 0);
    QCOMPARE(group1.size(), 0);
    QCOMPARE(group2.size(), 2);
    QCOMPARE(group3.size(), 1);
    QCOMPARE(group4.size(), 1);
    QCOMPARE(group5.size(), 0);
    QCOMPARE(group6.size(), 0);
    QCOMPARE(group7.size(), 0);
    QCOMPARE(group8.size(), 0);
    QCOMPARE(group9.size(), 2);

    QCOMPARE(group2.at(0), child3);
    QCOMPARE(group2.at(1), child1);
    QCOMPARE(group3.at(0), child4);
    QCOMPARE(group4.at(0), child5);
    QCOMPARE(group9.at(0), child7);
    QCOMPARE(group9.at(1), child6);

    visual->clearCallData();
    success = element->removeChild(5, nullptr); // child6
    QVERIFY(success);

    QCOMPARE(visual->lastCall(), GroupedVisual::LastCall::ABOUT_TO_REMOVE_CHILD);
    QCOMPARE(visual->lastChildIndex(), 5U);
    QCOMPARE(visual->lastElement(), child6);

    QCOMPARE(element->numberChildren(), 5U);
    group0 = element->childrenInGroup(0);
    group1 = element->childrenInGroup(1);
    group2 = element->childrenInGroup(2);
    group3 = element->childrenInGroup(3);
    group4 = element->childrenInGroup(4);
    group5 = element->childrenInGroup(5);
    group6 = element->childrenInGroup(6);
    group7 = element->childrenInGroup(7);
    group8 = element->childrenInGroup(8);
    group9 = element->childrenInGroup(9);

    QCOMPARE(group0.size(), 0);
    QCOMPARE(group1.size(), 0);
    QCOMPARE(group2.size(), 2);
    QCOMPARE(group3.size(), 1);
    QCOMPARE(group4.size(), 1);
    QCOMPARE(group5.size(), 0);
    QCOMPARE(group6.size(), 0);
    QCOMPARE(group7.size(), 0);
    QCOMPARE(group8.size(), 0);
    QCOMPARE(group9.size(), 1);

    QCOMPARE(group2.at(0), child3);
    QCOMPARE(group2.at(1), child1);
    QCOMPARE(group3.at(0), child4);
    QCOMPARE(group4.at(0), child5);
    QCOMPARE(group9.at(0), child7);

    visual->clearCallData();
    success = element->removeChild(4, nullptr); // child7
    QVERIFY(success);

    QCOMPARE(visual->lastCall(), GroupedVisual::LastCall::ABOUT_TO_REMOVE_CHILD);
    QCOMPARE(visual->lastChildIndex(), 4U);
    QCOMPARE(visual->lastElement(), child7);

    QCOMPARE(element->numberChildren(), 4U);
    group0 = element->childrenInGroup(0);
    group1 = element->childrenInGroup(1);
    group2 = element->childrenInGroup(2);
    group3 = element->childrenInGroup(3);
    group4 = element->childrenInGroup(4);
    group5 = element->childrenInGroup(5);
    group6 = element->childrenInGroup(6);
    group7 = element->childrenInGroup(7);
    group8 = element->childrenInGroup(8);
    group9 = element->childrenInGroup(9);

    QCOMPARE(group0.size(), 0);
    QCOMPARE(group1.size(), 0);
    QCOMPARE(group2.size(), 2);
    QCOMPARE(group3.size(), 1);
    QCOMPARE(group4.size(), 1);
    QCOMPARE(group5.size(), 0);
    QCOMPARE(group6.size(), 0);
    QCOMPARE(group7.size(), 0);
    QCOMPARE(group8.size(), 0);
    QCOMPARE(group9.size(), 0);

    QCOMPARE(group2.at(0), child3);
    QCOMPARE(group2.at(1), child1);
    QCOMPARE(group3.at(0), child4);
    QCOMPARE(group4.at(0), child5);

    delete visual;
}


void TestElementWithGroupedChildren::testRemoveChildrenMethod() {
    QSharedPointer<GroupedElement> element = Ld::Element::create(GroupedElement::elementName)
                                             .dynamicCast<GroupedElement>();

    QCOMPARE(element->numberGroups(), 1U);

    element->insertGroupsBefore(0, 9);
    QCOMPARE(element->numberGroups(), 10U);

    QSharedPointer<GroupedElement> child1 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(2, 0, child1, nullptr);

    QSharedPointer<GroupedElement> child2 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(2, 0, child2, nullptr);

    QSharedPointer<GroupedElement> child3 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(2, 0, child3, nullptr);

    QSharedPointer<GroupedElement> child4 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(3, 0, child4, nullptr);

    QSharedPointer<GroupedElement> child5 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(4, 0, child5, nullptr);

    QSharedPointer<GroupedElement> child6 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(7, 0, child6, nullptr);

    QSharedPointer<GroupedElement> child7 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(9, 0, child7, nullptr);

    QSharedPointer<GroupedElement> child8 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(9, 0, child8, nullptr);

    QCOMPARE(element->numberChildren(), 8U);
    QCOMPARE(element->numberChildrenInGroup(0), 0U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 3U);
    QCOMPARE(element->numberChildrenInGroup(3), 1U);
    QCOMPARE(element->numberChildrenInGroup(4), 1U);
    QCOMPARE(element->numberChildrenInGroup(5), 0U);
    QCOMPARE(element->numberChildrenInGroup(6), 0U);
    QCOMPARE(element->numberChildrenInGroup(7), 1U);
    QCOMPARE(element->numberChildrenInGroup(8), 0U);
    QCOMPARE(element->numberChildrenInGroup(9), 2U);

    element->removeChildren(nullptr);

    QCOMPARE(element->numberChildren(), 0U);
    QCOMPARE(element->numberChildrenInGroup(0), 0U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 0U);
    QCOMPARE(element->numberChildrenInGroup(3), 0U);
    QCOMPARE(element->numberChildrenInGroup(4), 0U);
    QCOMPARE(element->numberChildrenInGroup(5), 0U);
    QCOMPARE(element->numberChildrenInGroup(6), 0U);
    QCOMPARE(element->numberChildrenInGroup(7), 0U);
    QCOMPARE(element->numberChildrenInGroup(8), 0U);
    QCOMPARE(element->numberChildrenInGroup(9), 0U);
}


void TestElementWithGroupedChildren::testInsertBeforeMethod() {
    QSharedPointer<GroupedElement> element = Ld::Element::create(GroupedElement::elementName)
                                             .dynamicCast<GroupedElement>();

    QCOMPARE(element->numberGroups(), 1U);

    element->insertGroupsBefore(0, 9);
    QCOMPARE(element->numberGroups(), 10U);

    QSharedPointer<GroupedElement> child1 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(2, 0, child1, nullptr);

    QSharedPointer<GroupedElement> child2 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(3, 0, child2, nullptr);

    QSharedPointer<GroupedElement> child3 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(4, 0, child3, nullptr);

    QSharedPointer<GroupedElement> child4 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(7, 0, child4, nullptr);

    QSharedPointer<GroupedElement> child5 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(9, 0, child5, nullptr);

    QCOMPARE(element->numberChildren(), 5U);
    QCOMPARE(element->numberChildrenInGroup(0), 0U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 1U); // child1
    QCOMPARE(element->numberChildrenInGroup(3), 1U); // child2
    QCOMPARE(element->numberChildrenInGroup(4), 1U); // child3
    QCOMPARE(element->numberChildrenInGroup(5), 0U);
    QCOMPARE(element->numberChildrenInGroup(6), 0U);
    QCOMPARE(element->numberChildrenInGroup(7), 1U); // child4
    QCOMPARE(element->numberChildrenInGroup(8), 0U);
    QCOMPARE(element->numberChildrenInGroup(9), 1U); // child5

    QSharedPointer<GroupedElement> child6 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertBefore(0, child6, nullptr);

    QCOMPARE(element->numberChildren(), 6U);
    QCOMPARE(element->numberChildrenInGroup(0), 0U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 2U); // child6, child1
    QCOMPARE(element->numberChildrenInGroup(3), 1U); // child2
    QCOMPARE(element->numberChildrenInGroup(4), 1U); // child3
    QCOMPARE(element->numberChildrenInGroup(5), 0U);
    QCOMPARE(element->numberChildrenInGroup(6), 0U);
    QCOMPARE(element->numberChildrenInGroup(7), 1U); // child4
    QCOMPARE(element->numberChildrenInGroup(8), 0U);
    QCOMPARE(element->numberChildrenInGroup(9), 1U); // child5

    QSharedPointer<GroupedElement> child7 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertBefore(1, child7, nullptr);

    QCOMPARE(element->numberChildren(), 7U);
    QCOMPARE(element->numberChildrenInGroup(0), 0U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 3U); // child6, child7, child1
    QCOMPARE(element->numberChildrenInGroup(3), 1U); // child2
    QCOMPARE(element->numberChildrenInGroup(4), 1U); // child3
    QCOMPARE(element->numberChildrenInGroup(5), 0U);
    QCOMPARE(element->numberChildrenInGroup(6), 0U);
    QCOMPARE(element->numberChildrenInGroup(7), 1U); // child4
    QCOMPARE(element->numberChildrenInGroup(8), 0U);
    QCOMPARE(element->numberChildrenInGroup(9), 1U); // child5

    QSharedPointer<GroupedElement> child8 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertBefore(6, child8, nullptr);

    QCOMPARE(element->numberChildren(), 8U);
    QCOMPARE(element->numberChildrenInGroup(0), 0U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 3U); // child6, child7, child1
    QCOMPARE(element->numberChildrenInGroup(3), 1U); // child2
    QCOMPARE(element->numberChildrenInGroup(4), 1U); // child3
    QCOMPARE(element->numberChildrenInGroup(5), 0U);
    QCOMPARE(element->numberChildrenInGroup(6), 0U);
    QCOMPARE(element->numberChildrenInGroup(7), 1U); // child4
    QCOMPARE(element->numberChildrenInGroup(8), 0U);
    QCOMPARE(element->numberChildrenInGroup(9), 2U); // child8, child5


    QSharedPointer<GroupedElement> child9 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertBefore(10, child9, nullptr);

    QCOMPARE(element->numberChildren(), 9U);
    QCOMPARE(element->numberChildrenInGroup(0), 0U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 3U); // child6, child7, child1
    QCOMPARE(element->numberChildrenInGroup(3), 1U); // child2
    QCOMPARE(element->numberChildrenInGroup(4), 1U); // child3
    QCOMPARE(element->numberChildrenInGroup(5), 0U);
    QCOMPARE(element->numberChildrenInGroup(6), 0U);
    QCOMPARE(element->numberChildrenInGroup(7), 1U); // child4
    QCOMPARE(element->numberChildrenInGroup(8), 0U);
    QCOMPARE(element->numberChildrenInGroup(9), 3U); // child8, child5, child9

    QCOMPARE(element->numberChildren(), 9U);
    Ld::ElementPointerList group0 = element->childrenInGroup(0);
    Ld::ElementPointerList group1 = element->childrenInGroup(1);
    Ld::ElementPointerList group2 = element->childrenInGroup(2);
    Ld::ElementPointerList group3 = element->childrenInGroup(3);
    Ld::ElementPointerList group4 = element->childrenInGroup(4);
    Ld::ElementPointerList group5 = element->childrenInGroup(5);
    Ld::ElementPointerList group6 = element->childrenInGroup(6);
    Ld::ElementPointerList group7 = element->childrenInGroup(7);
    Ld::ElementPointerList group8 = element->childrenInGroup(8);
    Ld::ElementPointerList group9 = element->childrenInGroup(9);

    QCOMPARE(group0.size(), 0);
    QCOMPARE(group1.size(), 0);
    QCOMPARE(group2.size(), 3);
    QCOMPARE(group3.size(), 1);
    QCOMPARE(group4.size(), 1);
    QCOMPARE(group5.size(), 0);
    QCOMPARE(group6.size(), 0);
    QCOMPARE(group7.size(), 1);
    QCOMPARE(group8.size(), 0);
    QCOMPARE(group9.size(), 3);

    QCOMPARE(group2.at(0), child6);
    QCOMPARE(group2.at(1), child7);
    QCOMPARE(group2.at(2), child1);
    QCOMPARE(group3.at(0), child2);
    QCOMPARE(group4.at(0), child3);
    QCOMPARE(group7.at(0), child4);
    QCOMPARE(group9.at(0), child8);
    QCOMPARE(group9.at(1), child5);
    QCOMPARE(group9.at(2), child9);
}


void TestElementWithGroupedChildren::testInsertAfterMethod() {
    QSharedPointer<GroupedElement> element = Ld::Element::create(GroupedElement::elementName)
                                             .dynamicCast<GroupedElement>();

    QCOMPARE(element->numberGroups(), 1U);

    element->insertGroupsBefore(0, 9);
    QCOMPARE(element->numberGroups(), 10U);

    QSharedPointer<GroupedElement> child1 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(2, 0, child1, nullptr);

    QSharedPointer<GroupedElement> child2 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(3, 0, child2, nullptr);

    QSharedPointer<GroupedElement> child3 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(4, 0, child3, nullptr);

    QSharedPointer<GroupedElement> child4 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(7, 0, child4, nullptr);

    QSharedPointer<GroupedElement> child5 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertIntoGroupBefore(9, 0, child5, nullptr);

    QCOMPARE(element->numberChildren(), 5U);
    QCOMPARE(element->numberChildrenInGroup(0), 0U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 1U); // child1
    QCOMPARE(element->numberChildrenInGroup(3), 1U); // child2
    QCOMPARE(element->numberChildrenInGroup(4), 1U); // child3
    QCOMPARE(element->numberChildrenInGroup(5), 0U);
    QCOMPARE(element->numberChildrenInGroup(6), 0U);
    QCOMPARE(element->numberChildrenInGroup(7), 1U); // child4
    QCOMPARE(element->numberChildrenInGroup(8), 0U);
    QCOMPARE(element->numberChildrenInGroup(9), 1U); // child5

    QSharedPointer<GroupedElement> child6 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertAfter(0, child6, nullptr);

    QCOMPARE(element->numberChildren(), 6U);
    QCOMPARE(element->numberChildrenInGroup(0), 0U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 2U); // child1, child6
    QCOMPARE(element->numberChildrenInGroup(3), 1U); // child2
    QCOMPARE(element->numberChildrenInGroup(4), 1U); // child3
    QCOMPARE(element->numberChildrenInGroup(5), 0U);
    QCOMPARE(element->numberChildrenInGroup(6), 0U);
    QCOMPARE(element->numberChildrenInGroup(7), 1U); // child4
    QCOMPARE(element->numberChildrenInGroup(8), 0U);
    QCOMPARE(element->numberChildrenInGroup(9), 1U); // child5

    QSharedPointer<GroupedElement> child7 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertAfter(0, child7, nullptr);

    QCOMPARE(element->numberChildren(), 7U);
    QCOMPARE(element->numberChildrenInGroup(0), 0U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 3U); // child1, child7, child6
    QCOMPARE(element->numberChildrenInGroup(3), 1U); // child2
    QCOMPARE(element->numberChildrenInGroup(4), 1U); // child3
    QCOMPARE(element->numberChildrenInGroup(5), 0U);
    QCOMPARE(element->numberChildrenInGroup(6), 0U);
    QCOMPARE(element->numberChildrenInGroup(7), 1U); // child4
    QCOMPARE(element->numberChildrenInGroup(8), 0U);
    QCOMPARE(element->numberChildrenInGroup(9), 1U); // child5

    QSharedPointer<GroupedElement> child8 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertAfter(6, child8, nullptr);

    QCOMPARE(element->numberChildren(), 8U);
    QCOMPARE(element->numberChildrenInGroup(0), 0U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 3U); // child1, child7, child6
    QCOMPARE(element->numberChildrenInGroup(3), 1U); // child2
    QCOMPARE(element->numberChildrenInGroup(4), 1U); // child3
    QCOMPARE(element->numberChildrenInGroup(5), 0U);
    QCOMPARE(element->numberChildrenInGroup(6), 0U);
    QCOMPARE(element->numberChildrenInGroup(7), 1U); // child4
    QCOMPARE(element->numberChildrenInGroup(8), 0U);
    QCOMPARE(element->numberChildrenInGroup(9), 2U); // child5, child8


    QSharedPointer<GroupedElement> child9 = Ld::Element::create(GroupedElement::elementName)
                                            .dynamicCast<GroupedElement>();

    element->insertAfter(10, child9, nullptr);

    QCOMPARE(element->numberChildren(), 9U);
    QCOMPARE(element->numberChildrenInGroup(0), 0U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 3U); // child1, child7, child6
    QCOMPARE(element->numberChildrenInGroup(3), 1U); // child2
    QCOMPARE(element->numberChildrenInGroup(4), 1U); // child3
    QCOMPARE(element->numberChildrenInGroup(5), 0U);
    QCOMPARE(element->numberChildrenInGroup(6), 0U);
    QCOMPARE(element->numberChildrenInGroup(7), 1U); // child4
    QCOMPARE(element->numberChildrenInGroup(8), 0U);
    QCOMPARE(element->numberChildrenInGroup(9), 3U); // child5, child8, child9

    QCOMPARE(element->numberChildren(), 9U);
    Ld::ElementPointerList group0 = element->childrenInGroup(0);
    Ld::ElementPointerList group1 = element->childrenInGroup(1);
    Ld::ElementPointerList group2 = element->childrenInGroup(2);
    Ld::ElementPointerList group3 = element->childrenInGroup(3);
    Ld::ElementPointerList group4 = element->childrenInGroup(4);
    Ld::ElementPointerList group5 = element->childrenInGroup(5);
    Ld::ElementPointerList group6 = element->childrenInGroup(6);
    Ld::ElementPointerList group7 = element->childrenInGroup(7);
    Ld::ElementPointerList group8 = element->childrenInGroup(8);
    Ld::ElementPointerList group9 = element->childrenInGroup(9);

    QCOMPARE(group0.size(), 0);
    QCOMPARE(group1.size(), 0);
    QCOMPARE(group2.size(), 3);
    QCOMPARE(group3.size(), 1);
    QCOMPARE(group4.size(), 1);
    QCOMPARE(group5.size(), 0);
    QCOMPARE(group6.size(), 0);
    QCOMPARE(group7.size(), 1);
    QCOMPARE(group8.size(), 0);
    QCOMPARE(group9.size(), 3);

    QCOMPARE(group2.at(0), child1);
    QCOMPARE(group2.at(1), child7);
    QCOMPARE(group2.at(2), child6);
    QCOMPARE(group3.at(0), child2);
    QCOMPARE(group4.at(0), child3);
    QCOMPARE(group7.at(0), child4);
    QCOMPARE(group9.at(0), child5);
    QCOMPARE(group9.at(1), child8);
    QCOMPARE(group9.at(2), child9);
}


void TestElementWithGroupedChildren::testCloneMethod() {
    QSharedPointer<GroupedElement> element(new GroupedElement("parent"));
    element->setWeakThis(element.toWeakRef());

    element->insertGroupsBefore(0, 9);
    QCOMPARE(element->numberGroups(), 10U);

    QSharedPointer<GroupedElement> child1(new GroupedElement("child1"));
    child1->setWeakThis(child1.toWeakRef());

    element->insertIntoGroupBefore(2, 0, child1, nullptr);

    QSharedPointer<GroupedElement> child2(new GroupedElement("child2"));
    child2->setWeakThis(child2.toWeakRef());

    element->insertIntoGroupBefore(3, 0, child2, nullptr);

    QSharedPointer<GroupedElement> child3(new GroupedElement("child3"));
    child3->setWeakThis(child3.toWeakRef());

    element->insertIntoGroupBefore(4, 0, child3, nullptr);

    QSharedPointer<GroupedElement> child4(new GroupedElement("child4"));
    child4->setWeakThis(child4.toWeakRef());

    element->insertIntoGroupBefore(7, 0, child4, nullptr);

    QSharedPointer<GroupedElement> child5(new GroupedElement("child5"));
    child5->setWeakThis(child5.toWeakRef());

    element->insertIntoGroupBefore(9, 0, child5, nullptr);

    QCOMPARE(element->numberChildren(), 5U);
    QCOMPARE(element->numberChildrenInGroup(0), 0U);
    QCOMPARE(element->numberChildrenInGroup(1), 0U);
    QCOMPARE(element->numberChildrenInGroup(2), 1U); // child1
    QCOMPARE(element->numberChildrenInGroup(3), 1U); // child2
    QCOMPARE(element->numberChildrenInGroup(4), 1U); // child3
    QCOMPARE(element->numberChildrenInGroup(5), 0U);
    QCOMPARE(element->numberChildrenInGroup(6), 0U);
    QCOMPARE(element->numberChildrenInGroup(7), 1U); // child4
    QCOMPARE(element->numberChildrenInGroup(8), 0U);
    QCOMPARE(element->numberChildrenInGroup(9), 1U); // child5

    QSharedPointer<GroupedElement> cloned = element->clone(true).dynamicCast<GroupedElement>();

    QCOMPARE(cloned->numberChildren(), 5U);
    QCOMPARE(cloned->numberChildrenInGroup(0), 0U);
    QCOMPARE(cloned->numberChildrenInGroup(1), 0U);
    QCOMPARE(cloned->numberChildrenInGroup(2), 1U); // child1 clone
    QCOMPARE(cloned->numberChildrenInGroup(3), 1U); // child2 clone
    QCOMPARE(cloned->numberChildrenInGroup(4), 1U); // child3 clone
    QCOMPARE(cloned->numberChildrenInGroup(5), 0U);
    QCOMPARE(cloned->numberChildrenInGroup(6), 0U);
    QCOMPARE(cloned->numberChildrenInGroup(7), 1U); // child4 clone
    QCOMPARE(cloned->numberChildrenInGroup(8), 0U);
    QCOMPARE(cloned->numberChildrenInGroup(9), 1U); // child5 clone

    Ld::ElementPointerList group2 = element->childrenInGroup(2);
    Ld::ElementPointerList group3 = element->childrenInGroup(3);
    Ld::ElementPointerList group4 = element->childrenInGroup(4);
    Ld::ElementPointerList group7 = element->childrenInGroup(7);
    Ld::ElementPointerList group9 = element->childrenInGroup(9);

    QCOMPARE(group2.at(0).dynamicCast<GroupedElement>()->identifier(), QString("child1"));
    QCOMPARE(group3.at(0).dynamicCast<GroupedElement>()->identifier(), QString("child2"));
    QCOMPARE(group4.at(0).dynamicCast<GroupedElement>()->identifier(), QString("child3"));
    QCOMPARE(group7.at(0).dynamicCast<GroupedElement>()->identifier(), QString("child4"));
    QCOMPARE(group9.at(0).dynamicCast<GroupedElement>()->identifier(), QString("child5"));
}


void TestElementWithGroupedChildren::testWriteReadChildrenMethods() {
    // Do we want a test here ?
}
