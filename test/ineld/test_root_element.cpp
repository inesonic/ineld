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
* This file implements tests of the Ld::RootElement class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QtTest/QtTest>

#include <ld_handle.h>
#include <ld_element_structures.h>
#include <ld_data_type.h>
#include <ld_character_format.h>
#include <ld_page_format.h>
#include <plug_in_data.h>
#include <ld_capabilities.h>
#include <ld_element.h>
#include <ld_element_with_fixed_children.h>
#include <ld_paragraph_element.h>
#include <ld_paragraph_format.h>
#include <ld_text_element.h>
#include <ld_character_format.h>
#include <ld_element_cursor.h>

#include <ld_root_visual.h>
#include <ld_root_element.h>

#include "test_root_element.h"

/***********************************************************************************************************************
 * ChildElement:
 */

class ChildElement:public Ld::ElementWithFixedChildren {
    public:
        ChildElement(const QString& plugIn = QString());

        ~ChildElement() override;

        QString typeName() const final;

        static Ld::Element* creator(const QString& typeName);

        QString plugInName() const final;

        QString description() const final;

        Ld::DataType::ValueType valueType() const final;

        Ld::Capabilities parentRequires(unsigned long index) const final;

        Ld::Capabilities childProvidesCapabilities() const final;

        void markChildChanged();

    protected:
        void writeAddAttributes(
            Ld::XmlAttributes&                  attributes,
            QSharedPointer<Ld::FormatOrganizer> formats,
            Ld::ProgramFile&                    programFile
        ) const final;

        void readAttributes(
            QSharedPointer<Ld::XmlReader>  reader,
            const Ld::XmlAttributes&       attributes,
            const Ld::FormatsByIdentifier& formats,
            Ld::ProgramFile&               programFile,
            unsigned                       xmlVersion
        ) final;

    private:
        QString currentPlugInName;
};


ChildElement::ChildElement(const QString& plugIn) {
    setNumberChildren(3, nullptr);
    currentPlugInName = plugIn;
}


ChildElement::~ChildElement() {}


QString ChildElement::typeName() const {
    return QString("ChildElement");
}


Ld::Element* ChildElement::creator(const QString&) {
    return new ChildElement();
}


QString ChildElement::plugInName() const {
    return currentPlugInName;
}


QString ChildElement::description() const {
    return QString();
}


Ld::DataType::ValueType ChildElement::valueType() const {
    return Ld::DataType::ValueType::NONE;
}


Ld::Capabilities ChildElement::parentRequires(unsigned long) const {
    return Ld::Capabilities();
}


Ld::Capabilities ChildElement::childProvidesCapabilities() const {
    return Ld::Capabilities();
}


void ChildElement::markChildChanged() {
    childChanged(weakThis().toStrongRef());
}


void ChildElement::writeAddAttributes(
        Ld::XmlAttributes&                  attributes,
        QSharedPointer<Ld::FormatOrganizer> formats,
        Ld::ProgramFile&                    programFile
    ) const {
    ElementWithFixedChildren::writeAddAttributes(attributes, formats, programFile);
}


void ChildElement::readAttributes(
        QSharedPointer<Ld::XmlReader>  reader,
        const Ld::XmlAttributes&       attributes,
        const Ld::FormatsByIdentifier& formats,
        Ld::ProgramFile&               programFile,
        unsigned                       xmlVersion
    ) {
    ElementWithFixedChildren::readAttributes(reader, attributes, formats, programFile, xmlVersion);
}

/***********************************************************************************************************************
 * TestRootVisual:
 */

class TestRootVisual:public Ld::RootVisual {
    public:
        enum class LastCall : quint8 {
            NONE = 0,
            CAN_DELETE_ROOT_ELEMENT,
            ELEMENT_CHANGED,
            ELEMENT_ADDED,
            ELEMENT_REMOVED,
            NOW_PRISTINE,
            REFERENCES_CHANGED,
            PROGRAM_SAVED,
            PROGRAM_SAVED_AS,
            PROGRAM_LOADED
        };

        TestRootVisual();

        ~TestRootVisual() override;

        void clearCallData();

        LastCall reportedLastCall() const;

        Ld::ElementPointer reportedElement() const;

        QString reportedFilename() const;

        unsigned reportedMajorVersion() const;

        unsigned reportedMinorVersion() const;

    protected:
        bool canDeleteRootElement() final;

        void elementChanged(Ld::ElementPointer changedElement) final;

        void elementAdded(Ld::ElementPointer newElement) final;

        void elementRemoved(Ld::ElementPointer removedElement) final;

        void nowPristine() final;

        void referencesChanged() final;

        void programSaved(const QString& filename) final;

        void programSavedAs(const QString& filename) final;

        void programLoaded(const QString& filename, unsigned majorVersion, unsigned minorVersion) final;

    private:
        LastCall               currentLastCall;
        Ld::ElementWeakPointer currentReportedElement;
        QString                currentReportedFilename;
        unsigned               currentReportedMajorVersion;
        unsigned               currentReportedMinorVersion;
};


TestRootVisual::TestRootVisual() {
    clearCallData();
}


TestRootVisual::~TestRootVisual() {}


void TestRootVisual::clearCallData() {
    currentLastCall             = LastCall::NONE;
    currentReportedElement      = nullptr;
    currentReportedFilename     = QString();
    currentReportedMajorVersion = Ld::Element::invalidXmlVersion;
    currentReportedMinorVersion = Ld::Element::invalidXmlVersion;
}


TestRootVisual::LastCall TestRootVisual::reportedLastCall() const {
    return currentLastCall;
}


Ld::ElementPointer TestRootVisual::reportedElement() const {
    return currentReportedElement.toStrongRef();
}


QString TestRootVisual::reportedFilename() const {
    return currentReportedFilename;
}


unsigned TestRootVisual::reportedMajorVersion() const {
    return currentReportedMajorVersion;
}


unsigned TestRootVisual::reportedMinorVersion() const {
    return currentReportedMinorVersion;
}


bool TestRootVisual::canDeleteRootElement() {
    currentLastCall = LastCall::CAN_DELETE_ROOT_ELEMENT;
    return false;
}


void TestRootVisual::elementChanged(Ld::ElementPointer changedElement) {
    currentLastCall        = LastCall::ELEMENT_CHANGED;
    currentReportedElement = changedElement.toWeakRef();
}


void TestRootVisual::elementAdded(Ld::ElementPointer newElement) {
    currentLastCall        = LastCall::ELEMENT_ADDED;
    currentReportedElement = newElement.toWeakRef();
}


void TestRootVisual::elementRemoved(Ld::ElementPointer removedElement) {
    currentLastCall        = LastCall::ELEMENT_REMOVED;
    currentReportedElement = removedElement.toWeakRef();
}


void TestRootVisual::nowPristine() {
    currentLastCall = LastCall::NOW_PRISTINE;
}


void TestRootVisual::referencesChanged() {
    currentLastCall = LastCall::REFERENCES_CHANGED;
}


void TestRootVisual::programSaved(const QString& filename) {
    currentLastCall         = LastCall::PROGRAM_SAVED;
    currentReportedFilename = filename;
}


void TestRootVisual::programSavedAs(const QString& filename) {
    currentLastCall         = LastCall::PROGRAM_SAVED_AS;
    currentReportedFilename = filename;
}


void TestRootVisual::programLoaded(const QString& filename, unsigned majorVersion, unsigned minorVersion) {
    currentLastCall             = LastCall::PROGRAM_LOADED;
    currentReportedFilename     = filename;
    currentReportedMajorVersion = majorVersion;
    currentReportedMinorVersion = minorVersion;
}

/***********************************************************************************************************************
 * Ld::PlugInManager: - Dummy class instance to work around private inheritance for test purposes.
 */

namespace Ld {
    class PlugInManager {
        public:
            static PlugInsByName buildDummyPlugInList();
    };


    PlugInsByName PlugInManager::buildDummyPlugInList() {
        ::PlugInData pi1 = {
            "plug-in 1",
            "John Q. Normal",
            "Some Company 1",
            nullptr,
            "Brief description.",
            "Detailed description",
            "1.0",
            0,
            nullptr,
            nullptr,
        };

        PlugInInformation plugIn1(&pi1, true);

        ::PlugInData pi2 = {
            "plug-in 2",
            "Abby Normal",
            "Some Company 2",
            nullptr,
            "Brief description.",
            "Detailed description",
            "1.0",
            0,
            nullptr,
            nullptr,
        };

        PlugInInformation plugIn2(&pi2, true);

        ::PlugInData pi3 = {
            "plug-in 3",
            "Quincy Fruit",
            "Planters Nut Company",
            nullptr,
            "Brief description.",
            "Detailed description",
            "1.0",
            0,
            nullptr,
            nullptr
        };

        PlugInInformation plugIn3(&pi3, true);

        ::PlugInData pi4 = {
            "plug-in 4",
            "Alf A. Romero",
            "Fabbrica Italiana Automobili Torino",
            nullptr,
            "Breve descrizione",
            "Una descrizione piu dettagliata",
            "1.0",
            0,
            nullptr,
            nullptr
        };

        PlugInInformation plugIn4(&pi4, true);

        PlugInsByName pubn;
        pubn.insert(QString("plug-in 1"), plugIn1);
        pubn.insert(QString("plug-in 2"), plugIn2);
        pubn.insert(QString("plug-in 3"), plugIn3);
        pubn.insert(QString("plug-in 4"), plugIn4);

        return pubn;
    }
}

/***********************************************************************************************************************
 * TestElement:
 */

class TestElement:public Ld::ElementWithFixedChildren {
    public:
        static constexpr unsigned forcedNumberChildren = 3;

        TestElement(Ld::Format* format = nullptr);

        ~TestElement() override;

        QString typeName() const final;
};


TestElement::TestElement(Ld::Format* format) {
    setFormat(format);
}


TestElement::~TestElement() {}


QString TestElement::typeName() const {
    return QString("TestElement");
}

/***********************************************************************************************************************
 * TestRootElement:
 */

TestRootElement::TestRootElement() {}


TestRootElement::~TestRootElement() {}


void TestRootElement::initTestCase() {
    Ld::Handle::initialize(0x123456789ABCDEF0ULL);

    Ld::Element::registerCreator(Ld::RootElement::elementName, Ld::RootElement::creator);
    Ld::Element::registerCreator("ChildElement", ChildElement::creator);
    Ld::Element::registerCreator(Ld::ParagraphElement::elementName, Ld::ParagraphElement::creator);
    Ld::Element::registerCreator(Ld::TextElement::elementName, Ld::TextElement::creator);

    Ld::Format::registerCreator(Ld::CharacterFormat::formatName, Ld::CharacterFormat::creator);
    Ld::Format::registerCreator(Ld::PageFormat::formatName, Ld::PageFormat::creator);
    Ld::Format::registerCreator(Ld::ParagraphFormat::formatName, Ld::ParagraphFormat::creator);
}


void TestRootElement::testTypeName() {
    QSharedPointer<Ld::RootElement> rootElement(new Ld::RootElement);
    rootElement->setWeakThis(rootElement.toWeakRef());
    QVERIFY(rootElement->typeName() == QString("Root"));
}


void TestRootElement::testStateMethods() { // isEmpty, isNotEmpty, isModified, isPristine
    QSharedPointer<Ld::RootElement> rootElement(new Ld::RootElement);
    rootElement->setWeakThis(rootElement.toWeakRef());

    bool success = rootElement->openNew();
    QVERIFY(success);

    // Check initial state.

    QVERIFY(rootElement->isEmpty());
    QVERIFY(!rootElement->isNotEmpty());
    QVERIFY(rootElement->isPristine());
    QVERIFY(!rootElement->isModified());

    // Add an element, confirm the root element is not empty and is modified.

    QSharedPointer<ChildElement> child1(new ChildElement);
    child1->setWeakThis(child1.toWeakRef());

    rootElement->append(child1, nullptr);

    QVERIFY(!rootElement->isEmpty());
    QVERIFY(rootElement->isNotEmpty());
    QVERIFY(!rootElement->isPristine());
    QVERIFY(rootElement->isModified());

    // Now save the root element and confirm modified status changes.

    success = rootElement->saveAs(QString("test_file.ms"));
    QVERIFY(success);

    QVERIFY(!rootElement->isEmpty());
    QVERIFY(rootElement->isNotEmpty());
    QVERIFY(rootElement->isPristine());
    QVERIFY(!rootElement->isModified());

    success = rootElement->close();
    QVERIFY(success);
}


void TestRootElement::testSaveLoadMethods() {
    QSharedPointer<Ld::RootElement> rootElement1(new Ld::RootElement);
    rootElement1->setWeakThis(rootElement1.toWeakRef());

    bool success = rootElement1->openNew();
    QVERIFY(success);

    QSharedPointer<ChildElement> child0(new ChildElement);
    child0->setWeakThis(child0.toWeakRef());

    QSharedPointer<ChildElement> child1(new ChildElement);
    child1->setWeakThis(child1.toWeakRef());

    rootElement1->append(child0, nullptr);
    rootElement1->append(child1, nullptr);

    QSharedPointer<ChildElement> child00(new ChildElement);
    child00->setWeakThis(child00.toWeakRef());

    QSharedPointer<ChildElement> child01(new ChildElement);
    child01->setWeakThis(child01.toWeakRef());

    child0->setChild(0, child00, nullptr);
    child0->setChild(1, child01, nullptr);

    QSharedPointer<ChildElement> child10(new ChildElement);
    child10->setWeakThis(child10.toWeakRef());

    QSharedPointer<ChildElement> child11(new ChildElement);
    child11->setWeakThis(child11.toWeakRef());

    child1->setChild(0, child10, nullptr);
    child1->setChild(1, child11, nullptr);

    child0->setFormat(new Ld::CharacterFormat("Arial", 10));
    child1->setFormat(new Ld::CharacterFormat("Helvetica", 12));

    success = rootElement1->saveAs("test_container.dat");
    QVERIFY(success);

    success = rootElement1->close();
    QVERIFY(success);

    QSharedPointer<Ld::RootElement> rootElement2(new Ld::RootElement);
    rootElement2->setWeakThis(rootElement2.toWeakRef());

    Ld::PlugInsByName plugInsByName;
    success = rootElement2->openExisting("test_container.dat", true, plugInsByName);
    QVERIFY(success);

    rootElement2->close();
}


void TestRootElement::testDocumentSettingsMethods() {
//    QSharedPointer<Ld::RootElement> rootElement1(new Ld::RootElement);
//    rootElement1->setWeakThis(rootElement1.toWeakRef());

//    bool success = rootElement1->openNew();
//    QVERIFY(success);

//    QSharedPointer<ChildElement> child0(new ChildElement);
//    child0->setWeakThis(child0.toWeakRef());

//    QSharedPointer<ChildElement> child1(new ChildElement);
//    child1->setWeakThis(child1.toWeakRef());

//    rootElement1->append(child0);
//    rootElement1->append(child1);

//    QSharedPointer<Ld::CursorPositionSetting> cursorPositionSetting(new Ld::CursorPositionSetting);
//    cursorPositionSetting->setCursorPosition(Ld::ElementCursor(child1));

//    Ld::DocumentSettings& documentSettings = rootElement1->documentSettings();
//    documentSettings.insert(cursorPositionSetting);

//    success = rootElement1->saveAs("test_container.dat");
//    QVERIFY(success);

//    success = rootElement1->close();
//    QVERIFY(success);

//    QSharedPointer<Ld::RootElement> rootElement2(new Ld::RootElement);
//    rootElement2->setWeakThis(rootElement2.toWeakRef());

//    Ld::PlugInsByName plugInsByName;
//    success = rootElement2->openExisting("test_container.dat", false, plugInsByName);
//    QVERIFY(success);

//    QCOMPARE(rootElement2->numberChildren(), 2U);
//    QCOMPARE(rootElement2->documentSettings().count(), 1);

//    QSharedPointer<Ld::CursorPositionSetting> cursorPositionSetting2 =
//        rootElement2->documentSettings().fromType<Ld::CursorPositionSetting>();
//    QVERIFY(!cursorPositionSetting2.isNull());

//    Ld::ElementCursor elementCursor = cursorPositionSetting2->cursorPosition(rootElement2);
//    QVERIFY(elementCursor.isValid());
//    QVERIFY(elementCursor.isWholeElement());
//    QCOMPARE(elementCursor.element(), rootElement2->child(1));

//    success = rootElement2->close();
//    QVERIFY(success);
}


void TestRootElement::testFileAndIdentifierMethods() {
    // filename, hasFilename, shortformName, identifier, static identifier
    QSharedPointer<Ld::RootElement> rootElement(new Ld::RootElement);
    rootElement->setWeakThis(rootElement.toWeakRef());

    bool success = rootElement->openNew();
    QVERIFY(success);

    QVERIFY(!rootElement->hasFilename());
    QVERIFY(rootElement->filename().isEmpty());
    QVERIFY(rootElement->shortformName() == tr("Unsaved Document 1"));
    QVERIFY(rootElement->identifier() == QString("0"));

    QString filename = QString("test.ms");
    QString absolutePath = QDir(QDir::currentPath()).absoluteFilePath(filename);

    success = rootElement->saveAs(filename);
    QVERIFY(success);

    QFileInfo fileInfo(filename);
    QVERIFY(rootElement->filename() == fileInfo.absoluteFilePath());

    QVERIFY(rootElement->shortformName() != tr("Unsaved Document 1"));
    QVERIFY(rootElement->identifier() == absolutePath);

    QVERIFY(Ld::RootElement::identifier(filename) == absolutePath);

    QVERIFY(Ld::RootElement::byIdentifier(absolutePath) == rootElement);

    success = rootElement->close();
    QVERIFY(success);
}


void TestRootElement::testElementByHandle() {
    QSharedPointer<Ld::RootElement> rootElement(new Ld::RootElement);
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<ChildElement> child0(new ChildElement);
    child0->setWeakThis(child0.toWeakRef());

    QSharedPointer<ChildElement> child1(new ChildElement);
    child1->setWeakThis(child1.toWeakRef());

    rootElement->append(child0, nullptr);
    rootElement->append(child1, nullptr);

    QSharedPointer<ChildElement> child00(new ChildElement);
    child00->setWeakThis(child00.toWeakRef());

    QSharedPointer<ChildElement> child01(new ChildElement);
    child01->setWeakThis(child01.toWeakRef());

    child0->setChild(0, child00, nullptr);
    child0->setChild(1, child01, nullptr);

    QSharedPointer<ChildElement> child10(new ChildElement);
    child10->setWeakThis(child10.toWeakRef());

    QSharedPointer<ChildElement> child11(new ChildElement);
    child11->setWeakThis(child11.toWeakRef());

    child1->setChild(0, child10, nullptr);
    child1->setChild(1, child11, nullptr);

    QVERIFY(rootElement->element(child0->handle()) == child0);
    QVERIFY(rootElement->element(child1->handle()) == child1);

    QVERIFY(rootElement->element(child00->handle()) == child00);
    QVERIFY(rootElement->element(child01->handle()) == child01);
    QVERIFY(rootElement->element(child10->handle()) == child10);
    QVERIFY(rootElement->element(child11->handle()) == child11);
}


void TestRootElement::testRootElementMap() { // static rootElements, identifiers, byIdentifier methods.
    QSharedPointer<Ld::RootElement> rootElement1(new Ld::RootElement);
    rootElement1->setWeakThis(rootElement1.toWeakRef());

    QSharedPointer<Ld::RootElement> rootElement2(new Ld::RootElement);
    rootElement2->setWeakThis(rootElement2.toWeakRef());

    QString rootElement3Identifier;

    {
        QSharedPointer<Ld::RootElement> rootElement3(new Ld::RootElement);
        rootElement3->setWeakThis(rootElement3.toWeakRef());

        QVERIFY(rootElement1->identifier() != rootElement2->identifier());
        QVERIFY(rootElement2->identifier() != rootElement3->identifier());
        QVERIFY(rootElement3->identifier() != rootElement1->identifier());

        QVERIFY(Ld::RootElement::byIdentifier(rootElement1->identifier()) == rootElement1);
        QVERIFY(Ld::RootElement::byIdentifier(rootElement2->identifier()) == rootElement2);
        QVERIFY(Ld::RootElement::byIdentifier(rootElement3->identifier()) == rootElement3);

        QList<QString> identifiers = Ld::RootElement::identifiers();
        QVERIFY(identifiers.count() == 3);

        QVERIFY(identifiers.contains(rootElement1->identifier()));
        QVERIFY(identifiers.contains(rootElement2->identifier()));
        QVERIFY(identifiers.contains(rootElement3->identifier()));

        Ld::RootElement::RootElementList rootElements = Ld::RootElement::rootElements();
        QVERIFY(rootElements.count() == 3);

        QVERIFY(rootElements.contains(rootElement1));
        QVERIFY(rootElements.contains(rootElement2));
        QVERIFY(rootElements.contains(rootElement3));

        QString rootElement3Identifier = rootElement3->identifier();
    } // RootElement3 should be deleted at the end of this scope.

    QVERIFY(!Ld::RootElement::byIdentifier(rootElement3Identifier));
}


void TestRootElement::testSetReferences() { // setReferences, references, referencedBy, canDelete
    QSharedPointer<Ld::RootElement> rootElement1(new Ld::RootElement);
    rootElement1->setWeakThis(rootElement1.toWeakRef());

    QSharedPointer<Ld::RootElement> rootElement2(new Ld::RootElement);
    rootElement2->setWeakThis(rootElement2.toWeakRef());

    QSharedPointer<Ld::RootElement> rootElement3(new Ld::RootElement);
    rootElement3->setWeakThis(rootElement3.toWeakRef());

    {
        QSharedPointer<Ld::RootElement> rootElement4(new Ld::RootElement);
        rootElement4->setWeakThis(rootElement4.toWeakRef());

        // Add references, make certain they're reported back correctly.

        QList<Ld::RootImport> imports;
        imports.append(Ld::RootImport(rootElement2, false));
        imports.append(Ld::RootImport(rootElement3, true));
        imports.append(Ld::RootImport(rootElement4, false));

        rootElement1->setImports(imports);

        QList<Ld::RootImport> reportedImports = rootElement1->imports();
        QVERIFY(reportedImports.count() == 3);

        QVERIFY(reportedImports.at(0).rootElement() == rootElement2);
        QVERIFY(reportedImports.at(0).isAbsoluteImport());

        QVERIFY(reportedImports.at(1).rootElement() == rootElement3);
        QVERIFY(reportedImports.at(1).isRelativeImport());

        QVERIFY(reportedImports.at(2).rootElement() == rootElement4);
        QVERIFY(reportedImports.at(2).isAbsoluteImport());

        // Check back references as well.

        Ld::RootElement::RootElementList backwardImports = rootElement2->importedBy();
        QVERIFY(backwardImports.count() == 1);
        QVERIFY(backwardImports.at(0) == rootElement1);

        backwardImports = rootElement3->importedBy();
        QVERIFY(backwardImports.count() == 1);
        QVERIFY(backwardImports.at(0) == rootElement1);

        backwardImports = rootElement4->importedBy();
        QVERIFY(backwardImports.count() == 1);
        QVERIFY(backwardImports.at(0) == rootElement1);

        // Lastly, remove a reference from rootElement1 and verify that the removed root element is flagged as deletable.

        QVERIFY(Ld::RootElement::rootElements().count() == 4);

        imports.clear();
        imports.append(rootElement2);
        imports.append(rootElement3);

        rootElement1->setImports(imports);

        QVERIFY(!rootElement2->canDelete());
        QVERIFY(!rootElement3->canDelete());
        QVERIFY( rootElement4->canDelete());
    } // End of scope for rootElement4, should be deleted.

    QVERIFY(Ld::RootElement::rootElements().count() == 3);

    QVERIFY(Ld::RootElement::rootElements().contains(rootElement1));
    QVERIFY(Ld::RootElement::rootElements().contains(rootElement2));
    QVERIFY(Ld::RootElement::rootElements().contains(rootElement3));
}


void TestRootElement::testRequiredPlugInsMethods() {
    QSharedPointer<Ld::RootElement> rootElement(new Ld::RootElement);
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<ChildElement> child0(new ChildElement(QString("plug-in 1")));
    child0->setWeakThis(child0.toWeakRef());

    QSharedPointer<ChildElement> child1(new ChildElement(QString("plug-in 2")));
    child1->setWeakThis(child1.toWeakRef());

    rootElement->append(child0, nullptr);
    rootElement->append(child1, nullptr);

    QSharedPointer<ChildElement> child00(new ChildElement(QString("plug-in 3")));
    child00->setWeakThis(child00.toWeakRef());

    QSharedPointer<ChildElement> child01(new ChildElement(QString("plug-in 2")));
    child01->setWeakThis(child01.toWeakRef());

    child0->setChild(0, child00, nullptr);
    child0->setChild(1, child01, nullptr);

    QSharedPointer<ChildElement> child10(new ChildElement(QString("plug-in 3")));
    child10->setWeakThis(child10.toWeakRef());

    QSharedPointer<ChildElement> child11(new ChildElement);
    child11->setWeakThis(child11.toWeakRef());

    child1->setChild(0, child10, nullptr);
    child1->setChild(1, child11, nullptr);

    Ld::PlugInsByName allPlugIns = Ld::PlugInManager::buildDummyPlugInList();
    Ld::PlugInsByName plugIns = rootElement->requiredPlugIns(allPlugIns);

    QVERIFY(plugIns.count() == 3);

    QVERIFY( plugIns.contains(QString("plug-in 1")));
    QVERIFY( plugIns.contains(QString("plug-in 2")));
    QVERIFY( plugIns.contains(QString("plug-in 3")));
    QVERIFY(!plugIns.contains(QString("plug-in 4")));
}


void TestRootElement::testFormatOrganizerMethod() {
    QSharedPointer<Ld::RootElement> rootElement(new Ld::RootElement);
    rootElement->setWeakThis(rootElement.toWeakRef());

    QSharedPointer<ChildElement> child0(new ChildElement);
    child0->setWeakThis(child0.toWeakRef());

    QSharedPointer<ChildElement> child1(new ChildElement);
    child1->setWeakThis(child1.toWeakRef());

    QSharedPointer<ChildElement> child2(new ChildElement);
    child2->setWeakThis(child2.toWeakRef());

    rootElement->append(child0, nullptr);
    rootElement->append(child1, nullptr);
    rootElement->append(child2, nullptr);

    QSharedPointer<ChildElement> child00(new ChildElement);
    child00->setWeakThis(child00.toWeakRef());

    QSharedPointer<ChildElement> child01(new ChildElement);
    child01->setWeakThis(child01.toWeakRef());

    child0->setChild(0, child00, nullptr);
    child0->setChild(1, child01, nullptr);

    QSharedPointer<ChildElement> child10(new ChildElement);
    child10->setWeakThis(child10.toWeakRef());

    QSharedPointer<ChildElement> child11(new ChildElement);
    child11->setWeakThis(child11.toWeakRef());

    child1->setChild(0, child10, nullptr);
    child1->setChild(1, child11, nullptr);

    Ld::CharacterFormat* f1 = new Ld::CharacterFormat("Helvetica");
    Ld::CharacterFormat* f2 = new Ld::CharacterFormat("Arial");
    Ld::CharacterFormat* f3 = new Ld::CharacterFormat("Courier");
    Ld::PageFormat*      f4 = new Ld::PageFormat(8.5 * 72.0, 11.0 * 72.0);
    Ld::CharacterFormat* f5 = new Ld::CharacterFormat("Helvetica");
    Ld::CharacterFormat* f6 = new Ld::CharacterFormat("Arial");
    Ld::CharacterFormat* f7 = new Ld::CharacterFormat("Helvetica");

    f4->setTopMargin(0.75 * 72.0);
    f4->setRightMargin(0.75 * 72.0);
    f4->setBottomMargin(0.75 * 72.0);
    f4->setLeftMargin(0.75 * 72.0);

    child0->setFormat(f1);
    child1->setFormat(f2);
    child2->setFormat(f3);
    child00->setFormat(f4);
    child01->setFormat(f5);
    child10->setFormat(f6);
    child11->setFormat(f7);

    Ld::FormatContainer format1(f1);
    Ld::FormatContainer format2(f2);
    Ld::FormatContainer format3(f3);
    Ld::FormatContainer format4(f4);

    QSharedPointer<Ld::FormatOrganizer> organizer = rootElement->formatOrganizer();

    Ld::FormatIdentifiersByFormat identifiersByFormat = organizer->identifiersByFormat();
    QVERIFY(identifiersByFormat.count() == 5);
    QVERIFY(identifiersByFormat.contains(format1));
    QVERIFY(identifiersByFormat.contains(format2));
    QVERIFY(identifiersByFormat.contains(format3));
    QVERIFY(identifiersByFormat.contains(format4));

    // The default page format will take position 1.
    QVERIFY(identifiersByFormat.value(format1) == 0); // Helvetica char format was most commonly used.
    QVERIFY(identifiersByFormat.value(format2) == 1); // Arial char format was next most commonly used.
    QVERIFY(identifiersByFormat.value(format3) >= 2 || identifiersByFormat.value(format3) <= 4);
    QVERIFY(identifiersByFormat.value(format4) >= 2 || identifiersByFormat.value(format4) <= 4);
    QVERIFY(identifiersByFormat.value(format3) != identifiersByFormat.value(format4));

    // We rely on the FormatOrganizer unit test to verify functionality of that class.
}
