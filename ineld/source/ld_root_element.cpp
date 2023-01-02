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
* This file implements the \ref Ld::RootElement class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QList>
#include <QSet>
#include <QBitArray>
#include <QByteArray>
#include <QSharedPointer>
#include <QHash>
#include <QFileInfo>

#include <cstring>

#include <model_rng.h>

#include "ld_handle.h"
#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_data_type.h"
#include "ld_calculated_value.h"
#include "ld_document_settings.h"
#include "ld_default_format_setting.h"
#include "ld_identifier_database.h"
#include "ld_operation_database.h"
#include "ld_plug_in_information.h"
#include "ld_payload_data.h"
#include "ld_program_file.h"
#include "ld_xml_writer.h"
#include "ld_xml_attributes.h"
#include "ld_element.h"
#include "ld_format.h"
#include "ld_character_format.h"
#include "ld_brace_conditional_format.h"
#include "ld_visual.h"
#include "ld_root_import.h"
#include "ld_root_visual.h"
#include "ld_element_with_positional_children.h"
#include "ld_root_element.h"

namespace Ld {
    const Capabilities                RootElement::childProvides;

    const QString                     RootElement::elementName("Root");
    const QString                     RootElement::doctypedecl("inesonic.ms");

    QSharedPointer<PageFormat>        RootElement::currentApplicationDefaultPageFormat;
    RootElement::RngType              RootElement::currentApplicationDefaultRngType = RootElement::defaultRngType;

    QBitArray                         RootElement::globalDocumentTracker;
    RootElement::GlobalRootElementMap RootElement::currentRootElements;

    RootElement::RootElement() {
        currentDocumentNumber = unassignedDocumentNumber;
        blockReporting        = false;
        currentIsModified     = false;

        if (currentApplicationDefaultPageFormat) {
            currentDefaultPageFormat = currentApplicationDefaultPageFormat->clone().dynamicCast<Ld::PageFormat>();
        } else {
            currentDefaultPageFormat = PageFormat::defaultPageFormat.clone().dynamicCast<Ld::PageFormat>();
        }

        currentRngType          = currentApplicationDefaultRngType;
        currentUsePresetRngSeed = false;
        std::memset(currentRngSeed, 0, sizeof(RngSeed));

        currentDefaultTextFormat             = CharacterFormat::applicationDefaultTextFont();
        currentDefaultMathTextFormat         = CharacterFormat::applicationDefaultMathFont();
        currentDefaultMathIdentifierFormat   = CharacterFormat::applicationDefaultMathIdentifierFont();
        currentDefaultMathFunctionFormat     = CharacterFormat::applicationDefaultMathFunctionFont();

        currentDefaultBraceConditionalFormat = BraceConditionalFormat::applicationDefaultBraceConditionalFormat();
        currentDefaultBraceConditionalElseClauseShown = BraceConditionalFormat::applicationDefaultShowElseClause();

        currentNumberPages = 0;
    }


    RootElement::~RootElement() {
        if (programFile.openMode() != ProgramFile::OpenMode::CLOSED) {
            programFile.close();
        }
    }


    Element* RootElement::creator(const QString&) {
        return new RootElement();
    }


    void RootElement::registerRootElement(QSharedPointer<RootElement> newRootElement) {
        if (!newRootElement->excludeFromUserVisibleRoots()) {
            if (newRootElement->currentDocumentNumber == unassignedDocumentNumber) {
                newRootElement->currentDocumentNumber = newDocumentNumber();
            }

            currentRootElements.insert(newRootElement->identifier(), newRootElement);
        }
    }


    void RootElement::unregisterRootElement(QSharedPointer<RootElement> rootElement) {
        QString identifier = rootElement->identifier();
        if (currentRootElements.contains(identifier)) {
            currentRootElements.remove(identifier);
        }
    }


    QString RootElement::typeName() const {
        return elementName;
    }


    QString RootElement::plugInName() const {
        return QString();
    }


    QString RootElement::description() const {
        return tr("root");
    }


    DataType::ValueType RootElement::valueType() const {
        return DataType::ValueType::NONE;
    }


    QSet<QString> RootElement::requiredPlugIns() const {
        QSet<QString> plugIns;

        for (  ElementMap::const_iterator pos = currentElementsByHandle.constBegin(),
                                          end = currentElementsByHandle.constEnd()
             ; pos != end
             ; ++pos
            ) {
            ElementPointer element = pos.value().toStrongRef();
            assert(element);

            QString plugInName = element->plugInName();

            if (!plugInName.isEmpty()) {
                plugIns.insert(plugInName);
            }
        }

        return plugIns;
    }


    DocumentSettings& RootElement::documentSettings() {
        return currentDocumentSettings;
    }


    const DocumentSettings& RootElement::documentSettings() const {
        return currentDocumentSettings;
    }


    IdentifierDatabase& RootElement::identifierDatabase() {
        return currentIdentifierDatabase;
    }


    const IdentifierDatabase& RootElement::identifierDatabase() const {
        return currentIdentifierDatabase;
    }


    OperationDatabase& RootElement::operationDatabase() {
        return currentOperationDatabase;
    }


    const OperationDatabase& RootElement::operationDatabase() const {
        return currentOperationDatabase;
    }


    Capabilities RootElement::parentRequires(unsigned long) const {
        return Capabilities::frame | Capabilities::expressions;
    }


    Capabilities RootElement::childProvidesCapabilities() const {
        return childProvides;
    }


    PlugInsByName RootElement::requiredPlugIns(const PlugInsByName& allPlugInsByName, bool* ok) const {
        bool          isOK = true;
        PlugInsByName plugIns;

        for (  ElementMap::const_iterator pos = currentElementsByHandle.begin(),
                                          end = currentElementsByHandle.end()
             ; pos != end
             ; ++pos
            ) {
            const ElementPointer element = pos.value().toStrongRef();
            assert(element);

            QString plugInName = element->plugInName();
            if (!plugInName.isEmpty() && !plugIns.contains(plugInName)) {
                if (allPlugInsByName.contains(plugInName)) {
                    plugIns.insert(plugInName, allPlugInsByName.value(plugInName));
                } else{
                    isOK = false;
                }
            }
        }

        if (ok != nullptr) {
            *ok = isOK;
        }

        return plugIns;
    }


    QSharedPointer<FormatOrganizer> RootElement::formatOrganizer() const {
        QSharedPointer<FormatOrganizer> organizer(new FormatOrganizer);

        for (  ElementMap::const_iterator elementIterator    = currentElementsByHandle.constBegin(),
                                          elementEndIterator = currentElementsByHandle.constEnd()
             ; elementIterator != elementEndIterator
             ; ++elementIterator
            ) {
            ElementPointer element = elementIterator.value().toStrongRef();
            assert(element);

            FormatPointer format = element->format();

            if (format) {
                organizer->addFormat(format);
            }
        }

        FormatContainer defaultPageFormatContainer(currentDefaultPageFormat);
        organizer->addFormat(defaultPageFormatContainer);

        FormatContainer defaultTextFormatContainer(currentDefaultTextFormat);
        organizer->addFormat(defaultTextFormatContainer);

        FormatContainer defaultMathFormatContainer(currentDefaultMathTextFormat);
        organizer->addFormat(defaultMathFormatContainer);

        FormatContainer defaultMathIdentifierFormatContainer(currentDefaultMathIdentifierFormat);
        organizer->addFormat(defaultMathIdentifierFormatContainer);

        FormatContainer defaultMathFunctionFormatContainer(currentDefaultMathFunctionFormat);
        organizer->addFormat(defaultMathFunctionFormatContainer);

        FormatContainer defaultBraceConditionalFormatContainer(currentDefaultBraceConditionalFormat);
        organizer->addFormat(defaultBraceConditionalFormatContainer);

        for (   PageFormatMap::const_iterator pageFormatIterator    = currentPageFormats.constBegin(),
                                              pageFormatEndIterator = currentPageFormats.constEnd()
             ; pageFormatIterator != pageFormatEndIterator
             ; ++pageFormatIterator
            ) {
            QSharedPointer<PageFormat> pageFormat = pageFormatIterator.value();
            if (pageFormat) {
                FormatContainer pageFormatContainer(pageFormat);
                organizer->addFormat(pageFormatContainer);
            }
        }

        return organizer;
    }


    void RootElement::setApplicationDefaultPageFormat(QSharedPointer<PageFormat> newDefaultPageFormat) {
        currentApplicationDefaultPageFormat = newDefaultPageFormat->clone().dynamicCast<Ld::PageFormat>();
    }


    void RootElement::setApplicationDefaultPageFormat(PageFormat* newDefaultPageFormat) {
        currentApplicationDefaultPageFormat.reset(newDefaultPageFormat);
    }


    QSharedPointer<PageFormat> RootElement::applicationDefaultPageFormat() {
        return currentApplicationDefaultPageFormat;
    }


    void RootElement::setApplicationDefaultRngType(RngType newRngType) {
        currentApplicationDefaultRngType = newRngType;
    }


    RootElement::RngType RootElement::applicationDefaultRngType() {
        return currentApplicationDefaultRngType;
    }


    void RootElement::setDefaultPageFormat(QSharedPointer<PageFormat> newDefaultPageFormat, bool updateExisting) {
        if (!updateExisting && newDefaultPageFormat->toString() != currentDefaultPageFormat->toString()) {
            for (unsigned long pageNumber=0 ; pageNumber<currentNumberPages ; ++pageNumber) {
                if (!currentPageFormats.contains(pageNumber)) {
                    currentPageFormats.insert(pageNumber, currentDefaultPageFormat);
                }
            }

            currentDefaultPageFormat = newDefaultPageFormat;
        } else {
            QSharedPointer<PageFormat> oldDefaultPageFormat = currentDefaultPageFormat;
            currentDefaultPageFormat = newDefaultPageFormat;

            RootVisual* rootVisual = visual();
            if (rootVisual != nullptr && !blockReporting) {
                unsigned long pageNumber = 0;
                while (pageNumber < currentNumberPages && currentPageFormats.contains(pageNumber)) {
                    ++pageNumber;
                }

                if (pageNumber < currentNumberPages) {
                    rootVisual->pageFormatChanged(pageNumber, oldDefaultPageFormat, currentDefaultPageFormat);
                }
            }
        }
    }


    void RootElement::setDefaultPageFormat(PageFormat* newDefaultPageFormat, bool updateExisting) {
        setDefaultPageFormat(QSharedPointer<PageFormat>(newDefaultPageFormat), updateExisting);
    }


    QSharedPointer<PageFormat> RootElement::defaultPageFormat() const {
        return currentDefaultPageFormat;
    }


    void RootElement::setDefaultTextFormat(QSharedPointer<CharacterFormat> newTextFormat) {
        currentDefaultTextFormat = newTextFormat;
    }


    QSharedPointer<CharacterFormat> RootElement::defaultTextFormat() const {
        return currentDefaultTextFormat;
    }


    void RootElement::setDefaultMathTextFormat(QSharedPointer<CharacterFormat> newMathTextFormat) {
        currentDefaultMathTextFormat = newMathTextFormat;
    }


    QSharedPointer<CharacterFormat> RootElement::defaultMathTextFormat() const {
        return currentDefaultMathTextFormat;
    }


    void RootElement::setDefaultMathIdentifierFormat(QSharedPointer<CharacterFormat> newMathIdentifierFormat) {
        currentDefaultMathIdentifierFormat = newMathIdentifierFormat;
    }


    QSharedPointer<CharacterFormat> RootElement::defaultMathIdentifierFormat() const {
        return currentDefaultMathIdentifierFormat;
    }


    void RootElement::setDefaultMathFunctionFormat(QSharedPointer<CharacterFormat> newMathFunctionFormat) {
        currentDefaultMathFunctionFormat = newMathFunctionFormat;
    }


    QSharedPointer<CharacterFormat> RootElement::defaultMathFunctionFormat() const {
        return currentDefaultMathFunctionFormat;
    }


    void RootElement::setDefaultBraceConditionalFormat(
            QSharedPointer<BraceConditionalFormat> newBraceConditionalFormat
        ) {
        currentDefaultBraceConditionalFormat = newBraceConditionalFormat;
    }


    QSharedPointer<BraceConditionalFormat> RootElement::defaultBraceConditionalFormat() const {
        return currentDefaultBraceConditionalFormat;
    }


    void RootElement::setDefaultBraceConditionalElseClauseShown(bool nowShowExplicitElseClause) {
        currentDefaultBraceConditionalElseClauseShown = nowShowExplicitElseClause;
    }


    bool RootElement::defaultBraceConditionalElseClauseShown() const {
        return currentDefaultBraceConditionalElseClauseShown;
    }


    QSharedPointer<PageFormat> RootElement::setPageFormat(
            unsigned long              pageNumber,
            QSharedPointer<PageFormat> newPageFormat
        ) {
        QSharedPointer<PageFormat> result;
        QSharedPointer<PageFormat> currentPageFormat = pageFormat(pageNumber);

        if (currentPageFormat->toString() == newPageFormat->toString()
            ) {
            result = currentPageFormat;
        } else {
            if (newPageFormat->toString() == currentDefaultPageFormat->toString()) {
                result = currentDefaultPageFormat;
                currentPageFormats.remove(pageNumber);
            } else {
                PageFormatMap::const_iterator pit  = currentPageFormats.constBegin();
                PageFormatMap::const_iterator pend = currentPageFormats.constEnd();

                while (pit != pend && (*pit)->toString() != newPageFormat->toString()) {
                    ++pit;
                }

                if (pit != pend) {
                    result = *pit;
                } else {
                    result = newPageFormat;
                }

                currentPageFormats.insert(pageNumber, result);
            }

            RootVisual* rootVisual = visual();
            if (rootVisual != nullptr && !blockReporting) {
                rootVisual->pageFormatChanged(pageNumber, currentPageFormat, result);
            }
        }

        return result;
    }


    const QSharedPointer<PageFormat> RootElement::pageFormat(unsigned long pageNumber) {
        if (pageNumber >= currentNumberPages) {
            currentNumberPages = pageNumber + 1;
        }

        QSharedPointer<PageFormat> result;

        if (currentPageFormats.contains(pageNumber)) {
            result = currentPageFormats.value(pageNumber);
        } else {
            result = currentDefaultPageFormat;
        }

        return result;
    }


    void RootElement::setNumberPages(unsigned long newNumberPages) {
        currentNumberPages = newNumberPages;

        QList<unsigned long> pagesToRemove;
        PageFormatMap::const_iterator pit  = currentPageFormats.lowerBound(newNumberPages);
        PageFormatMap::const_iterator pend = currentPageFormats.constEnd();

        while (pit != pend) {
            pagesToRemove.append(pit.key());
            ++pit;
        }

        for (  QList<unsigned long>::const_iterator it  = pagesToRemove.constBegin(),
                                                    end = pagesToRemove.end()
             ; it!=end
             ; ++it
            ) {
            currentPageFormats.remove(*it);
        }
    }


    unsigned long RootElement::numberPages() const {
        return currentNumberPages;
    }


    void RootElement::setRngType(RootElement::RngType newRngType) {
        currentRngType = newRngType;
    }


    RootElement::RngType RootElement::rngType() const {
        return currentRngType;
    }


    void RootElement::setRngSeed(const RootElement::RngSeed& newRngSeed) {
        std::memcpy(currentRngSeed, newRngSeed, sizeof(RngSeed));
    }


    const RootElement::RngSeed& RootElement::rngSeed() const {
        return currentRngSeed;
    }


    void RootElement::setRngSeedIsPreset(bool useSeedAsPreset) {
        currentUsePresetRngSeed = useSeedAsPreset;
    }


    bool RootElement::rngSeedIsPreset() const {
        return currentUsePresetRngSeed;
    }


    void RootElement::setVisual(RootVisual* newVisual) {
        Element::setVisual(newVisual);
    }


    RootVisual* RootElement::visual() const {
        return dynamic_cast<RootVisual*>(Element::visual());
    }


    bool RootElement::isFunctionLogicalStop(unsigned long /* functionChildIndex */) const {
        return true;
    }


    bool RootElement::isEmpty() const {
        return numberChildren() == 0;
    }


    bool RootElement::isNotEmpty() const {
        return !isEmpty();
    }


    bool RootElement::isModified() const {
        return currentIsModified;
    }


    bool RootElement::canDelete() const {
        return importedBy().isEmpty() && (visual() == nullptr || visual()->canDeleteRootElement());
    }


    bool RootElement::isPristine() const {
        return !isModified();
    }


    bool RootElement::openExisting(const QString& filename, bool readOnly, const PlugInsByName& plugInsByName) {
        // The assert below will fire if setWeakThis was not called before using the root element.
        Q_ASSERT(weakThis());

        bool success = true;

        QString oldIdentifier = identifier();

        unsigned majorVersion;
        unsigned minorVersion;

        blockReporting = true;

        if (programFile.openMode() != ProgramFile::OpenMode::CLOSED) {
            success = false;
            lastError = tr("Program needs to be closed first.");
        } else {
            Q_ASSERT(isEmpty());
            Q_ASSERT(currentImports.count() == 0);

            removeChildren(nullptr);
            setImports(QList<RootImport>());

            success = programFile.openExisting(filename, readOnly);

            if (!success) {
                lastError = programFile.errorString();
            }
        }

        if (success) {
            success = readXmlDescription(filename, plugInsByName, majorVersion, minorVersion);
        }

        if (success) {
            if (currentDocumentNumber != invalidDocumentNumber && currentDocumentNumber != unassignedDocumentNumber) {
                freeDocumentNumber(currentDocumentNumber);
                currentDocumentNumber = invalidDocumentNumber;
            }

            if (!excludeFromUserVisibleRoots()) {
                currentRootElements.remove(oldIdentifier);
                currentRootElements.insert(identifier(), weakThis().toStrongRef().dynamicCast<RootElement>());
            }

            blockReporting = false;
            markPristine();

            if (visual() != nullptr) {
                visual()->programLoaded(programFile.filename(), majorVersion, minorVersion);
            }
        } else {
            programFile.close();
            removeChildren(nullptr);
            setImports(QList<RootImport>());

            blockReporting = false;
            markPristine();

            RootVisual* rootVisual = visual();
            if (rootVisual != nullptr) {
                rootVisual->programLoadFailed(lastError);
            }
        }

        return success;
    }


    bool RootElement::openNew() {
        // The assert below will fire if setWeakThis was not called before using the root element.
        Q_ASSERT(weakThis());

        bool success = true;

        QString oldIdentifier = identifier();

        blockReporting = true;

        if (programFile.openMode() != ProgramFile::OpenMode::CLOSED) {
            success = false;
            lastError = tr("Program needs to be closed first.");
        } else {
            Q_ASSERT(numberChildren() == 0);
            Q_ASSERT(currentImports.count() == 0);

            success = programFile.openNew();

            if (!success) {
                lastError = programFile.errorString();
            }
        }

        if (success) {
            if (currentDocumentNumber == invalidDocumentNumber) {
                currentDocumentNumber = newDocumentNumber();
            }

            if (!excludeFromUserVisibleRoots()) {
                currentRootElements.remove(oldIdentifier);
                currentRootElements.insert(identifier(), weakThis().toStrongRef().dynamicCast<RootElement>());
            }

            blockReporting = false;
            markPristine();

            if (visual() != nullptr) {
                visual()->newProgramCreated();
            }
        } else {
            blockReporting = false;
        }

        return success;
    }


    bool RootElement::save(bool purgeOnSave) {
        // The assert below will fire if setWeakThis was not called before using the root element.
        Q_ASSERT(weakThis());

        bool success = true;

        if (programFile.openMode() == ProgramFile::OpenMode::CLOSED) {
            success = false;
            lastError = tr("Program closed, can't save.");
        } else if (programFile.openMode() == ProgramFile::OpenMode::READ_ONLY) {
            success = false;
            lastError = tr("Program previously opened read-only.  Save under a new name.");
        }

        if (success && purgeOnSave) {
            success = programFile.purgeUnreferencedPayloads();
        }

        if (success) {
            success = writeXmlDescription(programFile.filename());
        }

        if (success) {
            success = programFile.save();
            if (!success) {
                lastError = programFile.errorString();
            }
        }

        if (success) {
            markPristine();

            RootVisual* rootVisual = visual();
            if (rootVisual != nullptr) {
                rootVisual->programSaved(programFile.filename());
            }
        } else {
            RootVisual* rootVisual = visual();
            if (rootVisual != nullptr) {
                rootVisual->programSaveFailed(lastError);
            }
        }

        return success;
    }


    bool RootElement::saveAs(const QString& newFilename, bool purgeOnSave) {
        // The assert below will fire if setWeakThis was not called before using the root element.
        Q_ASSERT(weakThis());

        bool success = true;

        QString oldIdentifier = identifier();

        if (programFile.openMode() == ProgramFile::OpenMode::CLOSED) {
            success = false;
            lastError = tr("Program closed, can't save.");
        } else if (programFile.openMode() == ProgramFile::OpenMode::READ_ONLY) {
            if (currentDocumentNumber == invalidDocumentNumber              &&
                QFileInfo(newFilename) == QFileInfo(programFile.filename())    ) {
                success = false;
                lastError = tr("Can-not overwrite program marked read-only.");
            }
        }

        if (success && purgeOnSave) {
            success = programFile.purgeUnreferencedPayloads();
        }

        if (success) {
            success = writeXmlDescription(newFilename);
        }

        if (success) {
            success = programFile.saveAs(newFilename);
            if (!success) {
                lastError = programFile.errorString();
            }
        }

        if (success) {
            if (currentDocumentNumber != invalidDocumentNumber && currentDocumentNumber != unassignedDocumentNumber) {
                freeDocumentNumber(currentDocumentNumber);
                currentDocumentNumber = invalidDocumentNumber;
            }

            if (!excludeFromUserVisibleRoots()) {
                currentRootElements.remove(oldIdentifier);
                currentRootElements.insert(identifier(), weakThis().toStrongRef().dynamicCast<RootElement>());
            }

            markPristine();

            RootVisual* rootVisual = visual();
            if (rootVisual != nullptr) {
                rootVisual->programSavedAs(programFile.filename());
            }
        } else {
            RootVisual* rootVisual = visual();
            if (rootVisual != nullptr) {
                rootVisual->programSaveFailed(lastError);
            }
        }

        return success;
    }


    bool RootElement::close() {
        // The assert below will fire if setWeakThis was not called before using the root element.
        Q_ASSERT(weakThis());

        bool success;

        QString oldIdentifier = identifier();

        blockReporting = true;

        if (programFile.openMode() == ProgramFile::OpenMode::CLOSED) {
            success = false;
            lastError = tr("Program already closed.");
        } else {
            if (visual() != nullptr) {
                visual()->aboutToClose();
            }

            success = programFile.close();
            if (!success) {
                lastError = programFile.errorString();
            }
        }

        if (success) {
            setImports(QList<RootImport>());

            removeChildren(nullptr);

            if (currentDocumentNumber == invalidDocumentNumber) {
                QString oldIdentifier = identifier();

                currentDocumentNumber = newDocumentNumber();

                if (!excludeFromUserVisibleRoots()) {
                    currentRootElements.remove(oldIdentifier);
                    currentRootElements.insert(identifier(), weakThis().toStrongRef().dynamicCast<RootElement>());
                }
            }

            blockReporting = false;
            markPristine();;
        } else {
            blockReporting = false;
        }

        return success;
    }


    QString RootElement::filename() const {
        QString currentFilename;

        if (currentDocumentNumber == invalidDocumentNumber) {
            currentFilename = programFile.filename();
            Q_ASSERT(!currentFilename.isEmpty());
        }

        return currentFilename;
    }


    bool RootElement::hasFilename() const {
        return currentDocumentNumber == invalidDocumentNumber;
    }


    PayloadData RootElement::payload(PayloadData::PayloadId payloadId) {
        return programFile.payload(payloadId);
    }


    PayloadData RootElement::newPayload() {
        return programFile.newPayload();
    }


    PayloadData RootElement::clonePayload(PayloadData& payloadData) {
        return programFile.clonePayload(payloadData);
    }


    RootElement::OpenMode RootElement::openMode() const {
        return programFile.openMode();
    }


    QString RootElement::errorString() const {
        return lastError;
    }


    QString RootElement::shortformName(unsigned maximumLength) const {
        QString name;

        if (currentDocumentNumber != invalidDocumentNumber) {
            name = tr("Unsaved Document %1").arg(currentDocumentNumber+1);
        } else if (currentDocumentNumber != unassignedDocumentNumber) {
            name = RootElement::identifier(programFile.filename());

            if (name.length() > static_cast<int>(maximumLength)) {
                QFileInfo currentFileInformation(programFile.filename());

                #if (defined(Q_OS_WIN) || defined(Q_OS_DARWIN))

                    QString baseName = currentFileInformation.baseName();

                #elif (defined(Q_OS_LINUX))

                    QString baseName = currentFileInformation.fileName();

                #else

                    #error Unknown platform.

                #endif

                if (baseName.length() >= static_cast<int>(maximumLength)) {
                    name = baseName;
                } else if (baseName.length() >= static_cast<int>((maximumLength-5))) {

                    #if (defined(Q_OS_WIN))

                        name = tr("\\...\\%1").arg(baseName);

                    #elif (defined(Q_OS_LINUX) || defined(Q_OS_DARWIN))

                        name = tr("/.../%1").arg(baseName);

                    #else

                        #error Unknown platform.

                    #endif

                } else {
                    unsigned leftLength = maximumLength - baseName.length() - 4;

                    #if (defined(Q_OS_WIN))

                        name = tr("%1...\\%2").arg(name.left(leftLength)).arg(baseName);

                    #elif (defined(Q_OS_LINUX) || defined(Q_OS_DARWIN))

                        name = tr("%1.../%2").arg(name.left(leftLength)).arg(baseName);

                    #else

                        #error Unknown platform.

                    #endif
                }
            }
        }

        return name;
    }


    QString RootElement::identifier() const {
        QString documentIdentifier;

        if (currentDocumentNumber == invalidDocumentNumber) {
            documentIdentifier = identifier(programFile.filename()); // This always includes a leading \, x:\, or /
        } else if (currentDocumentNumber != unassignedDocumentNumber) {
            documentIdentifier = QString::number(currentDocumentNumber); // This never includes a leading \, x:\, or /
        }

        return documentIdentifier;
    }


    bool RootElement::isNew() const {
        return currentDocumentNumber != invalidDocumentNumber && currentDocumentNumber != unassignedDocumentNumber;
    }


    ElementPointer RootElement::element(const Handle& handle) const {
        return currentElementsByHandle.value(handle).toStrongRef();
    }


    void RootElement::setImports(const QList<RootImport>& newImports) {
        if (currentImports != newImports) {
            currentImports = newImports;

            markModified();

            RootVisual* rootVisual = visual();
            if (rootVisual != nullptr) {
                rootVisual->referencesChanged();
                rootVisual->nowChanged();
            }

        }
    }


    QList<RootImport> RootElement::imports() const {
        return currentImports;
    }


    RootElement::RootElementList RootElement::allDependencies() const {
        RootElementList dependencies;
        RootElementSet  dependencySet;
        RootElementSet  pendingSet;

        pendingSet.insert(weakThis().toStrongRef().dynamicCast<Ld::RootElement>());
        for (  QList<RootImport>::const_iterator it  = currentImports.constBegin(),
                                                 end = currentImports.constEnd()
             ; it != end
             ; ++it
            ) {
            buildDependencyList(it->rootElement(), dependencies, dependencySet, pendingSet);
        }

        return dependencies;
    }


    RootElement::RootElementList RootElement::importedBy() const {
        RootElementList backImports;

        for (  GlobalRootElementMap::const_iterator pos = currentRootElements.begin(),
                                                    end = currentRootElements.end()
             ; pos != end
             ; ++pos
            ) {
            QSharedPointer<RootElement>       rootElement       = pos.value();
            QList<RootImport>::const_iterator importIterator    = rootElement->currentImports.begin();
            QList<RootImport>::const_iterator importEndIterator = rootElement->currentImports.end();

            while (importIterator != importEndIterator && importIterator->rootElement() != this) {
                ++importIterator;
            }

            if (importIterator != importEndIterator) {
                backImports.append(rootElement);
            }
        }

        return backImports;
    }


    RootElement::RootElementSet RootElement::indirectlyImportedBy() const {
        RootElementList directlyImportsUs = importedBy();
        RootElementSet  indirectlyImportsUs(directlyImportsUs.begin(), directlyImportsUs.end());

        for (  RootElementList::const_iterator it=directlyImportsUs.constBegin(), end=directlyImportsUs.constEnd()
             ; it!=end
             ; ++it
            ) {
            QSharedPointer<RootElement> rootElement = *it;
            indirectlyImportsUs.unite(rootElement->indirectlyImportedBy());
        }

        return indirectlyImportsUs;
    }


    void RootElement::markPristine() {
        bool wasModified = currentIsModified;
        currentIsModified = false;

        if (wasModified) {
            RootVisual* rootVisual = visual();
            if (rootVisual != nullptr && !blockReporting) {
                rootVisual->nowPristine();
            }
        }
    }


    void RootElement::markModified() {
        RootVisual* rootVisual  = visual();
        bool        wasPristine = !currentIsModified;
        currentIsModified = true;

        if (rootVisual != nullptr && !blockReporting) {
            rootVisual->nowChanged();
        }

        if (wasPristine) {
            if (rootVisual != nullptr && !blockReporting) {
                rootVisual->nowModified();
            }
        }
    }


    RootElement::ElementIterator RootElement::elementBegin() const {
        return currentElementsByHandle.constBegin();
    }


    RootElement::ElementIterator RootElement::elementEnd() const {
        return currentElementsByHandle.constEnd();
    }


    CalculatedValue RootElement::elementCalculatedValue(ElementPointer element, unsigned valueIndex) const {
        return element->calculatedValue(valueIndex);
    }


    QByteArray RootElement::exportElementImage(ElementPointer element, float dpi) const {
        return element->exportImage(dpi);
    }


    QString RootElement::identifier(const QString& filename) {
        QFileInfo fileInformation(filename);
        QString   documentIdentifier = fileInformation.canonicalFilePath();

        if (documentIdentifier.isEmpty()) {
            documentIdentifier = fileInformation.absoluteFilePath();
        }

        return documentIdentifier;
    }


    RootElement::RootElementList RootElement::rootElements() {
        RootElementList roots;

        for (  GlobalRootElementMap::const_iterator pos = currentRootElements.begin(),
                                                    end = currentRootElements.end()
             ; pos != end
             ; ++pos
            ) {
            QSharedPointer<RootElement> element = pos.value();
            roots.append(element);
        }

        return roots;
    }


    QList<QString> RootElement::identifiers() {
        return currentRootElements.keys();
    }


    QSharedPointer<RootElement> RootElement::byIdentifier(const QString& identifier) {
        return currentRootElements.value(identifier);
    }


    QString RootElement::toString(RootElement::RngType rngType) {
        QString result;

        switch (rngType) {
            case RngType::MT19937:           { result = QString("MT19937");        break; }
            case RngType::MT216091:          { result = QString("MT216091");       break; }
            case RngType::XORSHIRO256_PLUS:  { result = QString("XORSHIRO256+");   break; }
            case RngType::XORSHIRO256_STARS: { result = QString("XORSHIRO256**");  break; }
            case RngType::TRNG:              { result = QString("TRNG");           break; }
            default: {
                assert(false);
            }
        }

        return result;
    }


    RootElement::RngType RootElement::toRngType(const QString& s, bool* ok) {
        QString str  = s.trimmed().toUpper();
        bool    isOk = true;

        RngType result;

        if (str == QString("MT19937")) {
            result = RngType::MT19937;
        } else if (str == QString("MT216091")) {
            result = RngType::MT216091;
        } else if (str == QString("XORSHIRO256+")) {
            result = RngType::XORSHIRO256_PLUS;
        } else if (str == QString("XORSHIRO256**")) {
            result = RngType::XORSHIRO256_STARS;
        } else if (str == QString("TRNG")) {
            result = RngType::TRNG;
        } else {
            result = defaultRngType;
            isOk   = false;
        }

        if (ok != nullptr) {
            *ok = isOk;
        }

        return result;
    }


    void RootElement::descendantAdded(ElementPointer descendantElement) {
        Ld::Handle handle = descendantElement->handle();
        Q_ASSERT(!currentElementsByHandle.contains(handle));

        currentElementsByHandle.insert(handle, descendantElement.toWeakRef());

        markModified();

        RootVisual* rootVisual = visual();
        if (rootVisual != nullptr && !blockReporting) {
            rootVisual->elementAdded(descendantElement);
        }
    }


    void RootElement::descendantRemoved(ElementPointer descendantElement) {
        Ld::Handle handle = descendantElement->handle();

        currentElementsByHandle.remove(handle);

        markModified();

        RootVisual* rootVisual = visual();
        if (rootVisual != nullptr && !blockReporting) {
            rootVisual->elementRemoved(descendantElement);
        }
    }


    void RootElement::descendantFormatChanged(ElementPointer changedElement, FormatPointer newFormat) {
        ElementWithPositionalChildren::descendantFormatChanged(changedElement, newFormat);
        markModified();
    };


    void RootElement::descendantFormatUpdated(ElementPointer changedElement, FormatPointer newFormat) {
        ElementWithPositionalChildren::descendantFormatUpdated(changedElement, newFormat);
        markModified();
    };


    void RootElement::childChanged(ElementPointer changedChild) {
        markModified();

        RootVisual* rootVisual = visual();
        if (rootVisual != nullptr && !blockReporting) {
            rootVisual->elementChanged(changedChild);
        }
    }


    bool RootElement::excludeFromUserVisibleRoots() const {
        return false;
    }


    void RootElement::buildDependencyList(
            QSharedPointer<RootElement>   rootElement,
            RootElement::RootElementList& dependencyList,
            RootElement::RootElementSet&  dependencySet,
            RootElement::RootElementSet&  pendingSet
        ) {
        if (!pendingSet.contains(rootElement) && !dependencySet.contains(rootElement)) {
            pendingSet.insert(rootElement);

            QList<RootImport> imports = rootElement->imports();
            for (QList<RootImport>::const_iterator it=imports.constBegin(),end=imports.constEnd() ; it!=end ; ++it) {
                buildDependencyList(it->rootElement(), dependencyList, dependencySet, pendingSet);
            }

            dependencyList.append(rootElement);
            dependencySet.insert(rootElement);

            pendingSet.remove(rootElement);
        }
    }


    bool RootElement::writeXmlDescription(const QString& filePath) {
        bool success = programFile.initializeXmlWriter();

        if (success) {
            QSharedPointer<XmlWriter> writer = programFile.writer();

            writer->setAutoFormatting(formatXml);

            writer->writeStartDocument();
            writer->writeDTD(QString("<!DOCTYPE %1>").arg(doctypedecl));

            writer->writeStartElement("Program");
            XmlAttributes attributes;
            attributes.append("version", QString("%1.%2").arg(xmlMajorVersion).arg(xmlMinorVersion));

            if (currentRngType != defaultRngType) {
                attributes.append("rng_type", toString(currentRngType));
            }

            if (currentUsePresetRngSeed) {
                attributes.append("use_preset_seed", true);

                QString seedString = QString("%1,%2,%3,%4")
                                     .arg(currentRngSeed[0])
                                     .arg(currentRngSeed[1])
                                     .arg(currentRngSeed[2])
                                     .arg(currentRngSeed[3]);
                attributes.append("rng_seed", seedString);
            }

            writer->writeAttributes(attributes);

            QSharedPointer<FormatOrganizer> formats = formatOrganizer();
            updateInternalDocumentSettings(formats);

            writeFormats(writer, formats);

            writeRequiresSection(writer);
            writeDocumentSettings(writer, currentDocumentSettings);
            writeImportsSection(writer, filePath);

            writePageData(writer, formats, currentDefaultPageFormat, currentPageFormats);

            XmlAttributes noInheritedAttributes;
            Element::writeXml(writer, formats, programFile, noInheritedAttributes);

            writer->writeEndElement();
            writer->writeEndDocument();

            success = programFile.releaseXmlWriter();
        }

        if (!success) {
            lastError = programFile.errorString();
        }

        return success;
    }


    bool RootElement::readXmlDescription(
            const QString&       filePath,
            const PlugInsByName& plugInsByName,
            unsigned&            majorVersion,
            unsigned&            minorVersion
        ) {
        bool success = programFile.initializeXmlReader();

        if (success) {
            QSharedPointer<XmlReader> reader = programFile.reader();

            while (!reader->isEndDocument() && !reader->hasError()) {
                XmlReader::TokenType tokenType = reader->readNext();

                if (tokenType == XmlReader::DTD) {
                    QString dtd = reader->dtdName().toString();
                    if (dtd != doctypedecl) {
                        reader->raiseError(tr("Invalid doctypedecl"));
                    }
                } else if (tokenType == XmlReader::StartElement) {
                    QString tagName = reader->qualifiedName().toString();

                    if (tagName == "Program") {
                        XmlAttributes attributes = reader->attributes();
                        bool          ok;
                        QString       versionString = attributes.value<QString>("version", &ok);
                        QStringList   versionFields;

                        if (ok) {
                            versionFields = versionString.split('.');
                            ok = (versionFields.count() == 2);

                            if (ok) {
                                majorVersion = versionFields.at(0).toUInt(&ok);
                            }

                            if (ok) {
                                minorVersion = versionFields.at(1).toUInt(&ok);
                            }

                            if (!ok) {
                                reader->raiseError(tr("Invalid version \"%1\"").arg(versionString));
                            } else {
                                if (majorVersion > xmlMajorVersion) {
                                    reader->raiseError(tr("Incompatible file version %1").arg(majorVersion));
                                    ok = false;
                                }
                            }
                        }

                        if (ok) {
                            if (attributes.hasAttribute("rng_type")) {
                                QString rngTypeString = attributes.value<QString>("rng_type", &ok);
                                if (ok) {
                                    currentRngType = toRngType(rngTypeString, &ok);
                                } else {
                                    currentRngType = defaultRngType;
                                }
                            } else {
                                currentRngType = defaultRngType;
                            }
                        }

                        if (ok) {
                            if (attributes.hasAttribute("use_preset_seed")) {
                                currentUsePresetRngSeed = attributes.value<bool>("use_preset_seed", &ok);
                            } else {
                                currentUsePresetRngSeed = false;
                            }
                        }

                        if (ok && currentUsePresetRngSeed) {
                            QString        seedString     = attributes.value<QString>("rng_seed");
                            QList<QString> seedStringList = seedString.split(QChar(','));

                            if (seedStringList.size() == 4) {
                                std::uint64_t s1;
                                std::uint64_t s2;
                                std::uint64_t s3;
                                std::uint64_t s4;

                                s1 = seedStringList[0].toULongLong(&ok);

                                if (ok) {
                                    s2 = seedStringList[1].toULongLong(&ok);
                                }

                                if (ok) {
                                    s3 = seedStringList[2].toULongLong(&ok);
                                }

                                if (ok) {
                                    s4 = seedStringList[3].toULongLong(&ok);
                                }

                                if (ok) {
                                    currentRngSeed[0] = s1;
                                    currentRngSeed[1] = s2;
                                    currentRngSeed[2] = s3;
                                    currentRngSeed[3] = s4;
                                }
                            } else {
                                ok = false;
                            }
                        }

                        if (ok && !currentUsePresetRngSeed) {
                            std::memset(currentRngSeed, 0, sizeof(RngSeed));
                        }

                        if (ok) {
                            readProgramSection(reader, filePath, plugInsByName, majorVersion);
                        }
                    } else {
                        reader->raiseError(tr("Unexpected tag \"%1\"").arg(tagName));
                    }
                } else if (tokenType == XmlReader::Characters && !reader->isWhitespace()) {
                    reader->raiseError(tr("Unexpected description content at root level"));
                } else if (tokenType == XmlReader::EndElement) {
                    QString tagName = reader->qualifiedName().toString();

                    if (tagName != "Program") {
                        reader->raiseError(tr("Unexpected end tag \"%1\"").arg(tagName));
                    }
                }
            }

            if (reader->hasError()) {
                lastError = tr("%1 (%2, %3, %4)").arg(reader->errorString())
                                                 .arg(reader->characterOffset())
                                                 .arg(reader->lineNumber())
                                                 .arg(reader->columnNumber());
                success = false;
            }

            if (success) {
                success = programFile.releaseXmlReader();
                if (!success) {
                    lastError = programFile.errorString();
                }
            }
        }

        return success;
    }


    void RootElement::writeRequiresSection(QSharedPointer<XmlWriter> writer) {
        QSet<QString> plugIns = requiredPlugIns();

        if (!plugIns.isEmpty()) {
            writer->writeStartElement("Requires");

            for (QSet<QString>::const_iterator it=plugIns.constBegin(),end=plugIns.constEnd() ; it!=end ; ++it) {
                writer->writeTextElement("PlugIn", *it);
            }

            writer->writeEndElement();
        }
    }


    void RootElement::writeImportsSection(QSharedPointer<XmlWriter> writer, const QString& filePath) {
        if (!currentImports.isEmpty()) {
            writer->writeStartElement("Imports"); // This was called "References" at one point.

            QList<RootImport>::const_iterator it = currentImports.constBegin();
            QList<RootImport>::const_iterator end = currentImports.constEnd();

            while (it != end) {
                RootImport import = *it;

                if (import.isRelativeImport()) {
                    writer->writeStartElement("Relative");
                } else {
                    writer->writeStartElement("Absolute");
                }

                writer->writeCharacters(import.path(filePath));
                writer->writeEndElement();

                ++it;
            }

            writer->writeEndElement();
        }
    }


    void RootElement::writeFormats(QSharedPointer<XmlWriter> writer, QSharedPointer<FormatOrganizer> formats) {
        FormatsByIdentifier formatsByIdentifier = formats->formatsByIdentifier();

        if (!formatsByIdentifier.isEmpty()) {
            writer->writeStartElement("Formats");

            FormatsByIdentifier::const_iterator pos = formatsByIdentifier.constBegin();
            FormatsByIdentifier::const_iterator end = formatsByIdentifier.constEnd();

            while (pos != end) {
                FormatIdentifier identifier = pos.key();
                FormatPointer    format     = pos.value().format();

                XmlAttributes inheritedAttributes;
                inheritedAttributes.append("id", identifier);

                format->writeXml(writer, formats, programFile, inheritedAttributes);

                ++pos;
            }

            writer->writeEndElement();
        }
    }


    void RootElement::writePageData(
            QSharedPointer<XmlWriter>       writer,
            QSharedPointer<FormatOrganizer> formats,
            QSharedPointer<PageFormat>      defaultPageFormat,
            const PageFormatMap&            pageFormats
        ) {
        writer->writeStartElement("Pages");

        FormatIdentifier defaultIdentifier = formats->identifier(defaultPageFormat);
        XmlAttributes defaultAttributes;
        defaultAttributes.append("format", defaultIdentifier);

        writer->writeStartElement("DefaultPage");
        writer->writeAttributes(defaultAttributes);
        writer->writeEndElement();

        for (PageFormatMap::const_iterator it=pageFormats.constBegin(),end=pageFormats.constEnd() ; it!=end ; ++it) {
            unsigned long    pageNumber = it.key();
            FormatIdentifier identifier = formats->identifier(it.value());

            XmlAttributes pageAttributes;
            pageAttributes.append("index", pageNumber);
            pageAttributes.append("format", identifier);

            writer->writeStartElement("Page");
            writer->writeAttributes(pageAttributes);
            writer->writeEndElement();
        }

        writer->writeEndElement();
    }


    void RootElement::writeDocumentSettings(QSharedPointer<XmlWriter> writer, DocumentSettings& documentSettings) {
        if (!documentSettings.isEmpty()) {
            writer->writeStartElement("Settings");
            documentSettings.writeXml(writer);
            writer->writeEndElement();
        }
    }


    void RootElement::readProgramSection(
            QSharedPointer<XmlReader> reader,
            const QString&            filePath,
            const PlugInsByName&      plugInsByName,
            unsigned                  majorVersion
        ) {
        bool done = false;

        QString currentTag;

        FormatsByIdentifier formats;

        while (!done && !reader->isEndDocument() && !reader->hasError()) {
            XmlReader::TokenType tokenType = reader->readNext();

            if (tokenType == XmlReader::StartElement) {
                currentTag = reader->qualifiedName().toString();
                XmlAttributes attributes = reader->attributes();

                if (currentTag == "Requires") {
                    if (attributes.isEmpty()) {
                        readRequiresSection(reader, plugInsByName, majorVersion);
                    } else {
                        reader->raiseError(tr("Unexpected attributes in requirements header"));
                    }
                } else if (currentTag == "Settings") {
                    if (attributes.isEmpty()) {
                        readDocumentSettings(reader, currentDocumentSettings, majorVersion);

                        if (!reader->hasError()) {
                            updateFromInternalDocumentSettings(formats, majorVersion);
                        }
                    } else {
                        reader->raiseError(tr("Unexpected attributes in settings header"));
                    }
                } else if (currentTag == "Imports" || currentTag == "References") {
                    if (attributes.isEmpty()) {
                        readImportsSection(reader, filePath, majorVersion, plugInsByName);
                    } else {
                        reader->raiseError(tr("Unexpected attributes in references header"));
                    }
                } else if (currentTag == "Formats") {
                    if (attributes.isEmpty()) {
                        readFormats(reader, formats, majorVersion);
                    } else {
                        reader->raiseError(tr("Unexpected attributes in formats header"));
                    }
                } else if (currentTag == "Pages") {
                    readPageData(reader, formats, majorVersion);
                } else if (currentTag == "Root") {
                    readXml(reader, formats, programFile, attributes, majorVersion);
                }
            } else if (tokenType == XmlReader::Characters && !reader->isWhitespace()) {
                reader->raiseError(tr("Unexpected description content"));
            } else if (tokenType == XmlReader::EndElement) {
                QString endTag = reader->qualifiedName().toString();

                if (endTag == "Program") {
                    done = true;
                } else {
                    reader->raiseError(tr("Unexpected end tag \"%1\"").arg(endTag));
                }
            }
        }
    }


    void RootElement::readRequiresSection(
            QSharedPointer<XmlReader> reader,
            const PlugInsByName&      plugInsByName,
            unsigned                  /* majorVersion */
        ) {
        bool done = false;

        QString currentTag;

        while (!done && !reader->isEndDocument() && !reader->hasError()) {
            XmlReader::TokenType tokenType = reader->readNext();

            if (tokenType == XmlReader::StartElement) {
                currentTag = reader->qualifiedName().toString();

                if (currentTag != "PlugIn") {
                    reader->raiseError(tr("Unexpected tag \"%1\"").arg(currentTag));
                }
            } else if (tokenType == XmlReader::Characters) {
                if (!reader->isWhitespace()) {
                    if (currentTag == "PlugIn") {
                        QString plugInName = reader->text().toString();
                        if (!plugInsByName.contains(plugInName)) {
                            reader->raiseError(tr("Requires Plug-In \"%1\"").arg(plugInName));
                        }
                    } else {
                        reader->raiseError(tr("Invalid PCDATA"));
                    }
                }
            } else if (tokenType == XmlReader::EndElement) {
                QString endTag = reader->qualifiedName().toString();
                if (currentTag == "PlugIn" && endTag == "PlugIn") {
                    currentTag = QString();
                } else if (currentTag.isEmpty() && endTag == "Requires") {
                    done = true;
                } else {
                    reader->raiseError(tr("Unexpected end tag \"%1\"").arg(endTag));
                }
            } else {
                reader->raiseError(tr("Unexpected construct"));
            }
        }
    }


    void RootElement::readImportsSection(
            QSharedPointer<XmlReader> reader,
            const QString&            filePath,
            unsigned                  /* majorVersion */,
            const PlugInsByName&      plugInData
        ) {
        bool done       = false;
        bool isRelative = false;
        bool inImport   = false;

        QString currentTag;
        QString importText;

        QList<RootImport> imports;

        while (!done && !reader->isEndDocument() && !reader->hasError()) {
            XmlReader::TokenType tokenType = reader->readNext();

            if (tokenType == XmlReader::StartElement) {
                currentTag = reader->qualifiedName().toString();

                if (inImport) {
                    reader->raiseError(tr("Unexpected tag \"%1\"").arg(currentTag));
                } else {
                    if (currentTag == "Relative") {
                        inImport = true;
                        isRelative  = true;
                    } else if (currentTag == "Absolute") {
                        inImport = true;
                        isRelative  = false;
                    } else {
                        reader->raiseError(tr("Unexpected tag \"%1\"").arg(currentTag));
                    }
                }
            } else if (tokenType == XmlReader::Characters) {
                if (!reader->isWhitespace()) {
                    importText = reader->text().toString();

                    if (!inImport) {
                        reader->raiseError(tr("Unexpected text \"%1\"").arg(importText));
                    }
                }
            } else if (tokenType == XmlReader::EndElement) {
                QString endTag = reader->qualifiedName().toString();

                if (!inImport) {
                    if (endTag != "References" && endTag != "Imports") {
                        reader->raiseError(tr("Unexpected end tag for \"%1\"").arg(endTag));
                    } else {
                        done = true;
                    }
                } else {
                    if ((isRelative && endTag != "Relative") || (!isRelative && endTag != "Absolute")) {
                        reader->raiseError(tr("Unexpected end tag for \"%1\"").arg(endTag));
                    } else {
                        addRootImport(reader, importText, filePath, isRelative, plugInData);
                        inImport = false;
                    }
                }
            }
        }
    }


    void RootElement::readFormats(
            QSharedPointer<XmlReader> reader,
            FormatsByIdentifier&      formats,
            unsigned                  majorVersion
        ) {
        bool done = false;

        while (!done && !reader->isEndDocument() && !reader->hasError()) {
            XmlReader::TokenType tokenType = reader->readNext();

            if (tokenType == XmlReader::StartElement) {
                QString formatTypeName = reader->qualifiedName().toString();

                FormatPointer format(Format::create(formatTypeName));

                if (format.isNull()) {
                    reader->raiseError(tr("Unknown format type \"%1\"").arg(formatTypeName));
                } else {
                    bool             ok;
                    XmlAttributes    attributes = reader->attributes();
                    FormatIdentifier identifier = attributes.value<FormatIdentifier>("id", &ok);

                    if (!ok) {
                        reader->raiseError(tr("Bad/missing identifier for \"%1\"").arg(formatTypeName));
                    } else if (formats.contains(identifier)) {
                        reader->raiseError(
                            tr("Duplicate identifier %1 for \"%2\"").arg(identifier).arg(formatTypeName)
                        );
                    } else {
                        format->readXml(reader, formats, programFile, attributes, majorVersion);

                        if (!reader->hasError()) {
                            formats.insert(identifier, FormatContainer(format));
                        }
                    }
                }
            } else if (tokenType == XmlReader::Characters && !reader->isWhitespace()) {
                reader->raiseError(tr("Unexpected description content"));
            } else if (tokenType == XmlReader::EndElement) {
                QString endTag = reader->qualifiedName().toString();
                if (endTag == "Formats") {
                    done = true;
                } else {
                    reader->raiseError(tr("Invalid end tag \"%1\"").arg(endTag));
                }
            }
        }
    }


    void RootElement::readPageData(
            QSharedPointer<XmlReader>  reader,
            const FormatsByIdentifier& formats,
            unsigned                   /* majorVersion */
        ) {
        bool done = false;

        currentPageFormats.clear();

        while (!done && !reader->isEndDocument() && !reader->hasError()) {
            XmlReader::TokenType tokenType = reader->readNext();

            if (tokenType == XmlReader::StartElement) {
                QString       childTag        = reader->qualifiedName().toString();
                XmlAttributes childAttributes = reader->attributes();

                if (childTag == "DefaultPage") {
                    if (!childAttributes.hasAttribute("format")) {
                        reader->raiseError(tr("Tag \"%1\" missing required attribute").arg(childTag));
                    } else {
                        bool ok;
                        FormatIdentifier formatIdentifier = childAttributes.value<FormatIdentifier>("format", &ok);
                        FormatContainer  formatContainer;

                        if (ok && !formats.contains(formatIdentifier)) {
                            ok = false;
                        }

                        if (ok) {
                            formatContainer = formats.value(formatIdentifier);
                            if (formatContainer.format()->typeName() != "PageFormat") {
                                ok = false;
                            }
                        }

                        if (!ok) {
                            reader->raiseError(tr("Tag \"%1\" has invalid format attribute").arg(childTag));
                        } else {
                            currentDefaultPageFormat = formatContainer.format().dynamicCast<PageFormat>();
                        }
                    }
                } else if (childTag == "Page") {
                    if (!childAttributes.hasAttribute("format") || !childAttributes.hasAttribute("index")) {
                        reader->raiseError(tr("Tag \"%1\" missing required attribute").arg(childTag));
                    } else {
                        bool             ok;
                        unsigned long    pageIndex;
                        FormatIdentifier identifier = childAttributes.value<FormatIdentifier>("format", &ok);
                        FormatContainer  formatContainer;

                        if (ok && !formats.contains(identifier)) {
                            ok = false;
                        }

                        if (ok) {
                            formatContainer = formats.value(identifier);
                            if (formatContainer.format()->typeName() != "PageFormat") {
                                ok = false;
                            }
                        }

                        if (ok) {
                            pageIndex = childAttributes.value<unsigned long>("index", &ok);
                        }

                        if (ok) {
                            if (currentPageFormats.contains(pageIndex)) {
                                ok = false;
                            }
                        }

                        if (ok) {
                            currentPageFormats.insert(pageIndex, formatContainer.format().dynamicCast<PageFormat>());
                        } else {
                            reader->raiseError(tr("Tag \"%1\" has invalid attribute").arg(childTag));
                        }
                    }
                } else {
                    reader->raiseError(tr("Unexpected tag \"%1\"").arg(childTag));
                }
            } else if (tokenType == XmlReader::Characters && !reader->isWhitespace()) {
                reader->raiseError(tr("Unexpected content"));
            } else if (tokenType == XmlReader::EndElement) {
                QString endTag = reader->qualifiedName().toString();
                if (endTag == "Pages") {
                    done = true;
                } else if (endTag != "DefaultPage" && endTag != "Page") {
                    reader->raiseError(tr("Invalid end tag \"%1\"").arg(endTag));
                }
            }
        }
    }


    void RootElement::readDocumentSettings(
            QSharedPointer<XmlReader> reader,
            DocumentSettings&         documentSettings,
            unsigned                  majorVersion
        ) {
        documentSettings.readXml(reader, majorVersion);
    }


    void RootElement::updateInternalDocumentSettings(QSharedPointer<FormatOrganizer> formats) {
        QSharedPointer<DefaultFormatSetting> textFormatSetting;
        QSharedPointer<DefaultFormatSetting> mathFormatSetting;
        QSharedPointer<DefaultFormatSetting> mathIdentifierSetting;
        QSharedPointer<DefaultFormatSetting> mathFunctionSetting;
        QSharedPointer<DefaultFormatSetting> braceConditionalSetting;

        if (currentDocumentSettings.contains(DefaultFormatSetting::defaultTextFormatSettingName)) {
            textFormatSetting = currentDocumentSettings.value(DefaultFormatSetting::defaultTextFormatSettingName)
                                .dynamicCast<DefaultFormatSetting>();
        } else {
            textFormatSetting.reset(new DefaultFormatSetting(DefaultFormatSetting::defaultTextFormatSettingName));
            currentDocumentSettings.insert(textFormatSetting);
        }

        if (currentDocumentSettings.contains(DefaultFormatSetting::defaultMathFormatSettingName)) {
            mathFormatSetting = currentDocumentSettings.value(DefaultFormatSetting::defaultMathFormatSettingName)
                                .dynamicCast<DefaultFormatSetting>();
        } else {
            mathFormatSetting.reset(new DefaultFormatSetting(DefaultFormatSetting::defaultMathFormatSettingName));
            currentDocumentSettings.insert(mathFormatSetting);
        }

        if (currentDocumentSettings.contains(DefaultFormatSetting::defaultMathIdentifierFormatSettingName)) {
            mathIdentifierSetting = currentDocumentSettings.value(
                DefaultFormatSetting::defaultMathIdentifierFormatSettingName
            ).dynamicCast<DefaultFormatSetting>();
        } else {
            mathIdentifierSetting.reset(
                new DefaultFormatSetting(DefaultFormatSetting::defaultMathIdentifierFormatSettingName)
            );
            currentDocumentSettings.insert(mathIdentifierSetting);
        }

        if (currentDocumentSettings.contains(DefaultFormatSetting::defaultMathFunctionFormatSettingName)) {
            mathFunctionSetting = currentDocumentSettings.value(
                DefaultFormatSetting::defaultMathFunctionFormatSettingName
            ).dynamicCast<DefaultFormatSetting>();
        } else {
            mathFunctionSetting.reset(
                new DefaultFormatSetting(DefaultFormatSetting::defaultMathFunctionFormatSettingName)
            );
            currentDocumentSettings.insert(mathFunctionSetting);
        }

        if (currentDocumentSettings.contains(DefaultFormatSetting::defaultBraceConditionalFormatSettingName)) {
            braceConditionalSetting = currentDocumentSettings.value(
                DefaultFormatSetting::defaultBraceConditionalFormatSettingName
            ).dynamicCast<DefaultFormatSetting>();
        } else {
            braceConditionalSetting.reset(
                new DefaultFormatSetting(DefaultFormatSetting::defaultBraceConditionalFormatSettingName)
            );
            currentDocumentSettings.insert(braceConditionalSetting);
        }

        FormatIdentifier textFormatIdentifier             = formats->identifier(currentDefaultTextFormat);
        FormatIdentifier mathFormatIdentifier             = formats->identifier(currentDefaultMathTextFormat);
        FormatIdentifier mathIdentifierFormatIdentifier   = formats->identifier(currentDefaultMathIdentifierFormat);
        FormatIdentifier mathFunctionFormatIdentifier     = formats->identifier(currentDefaultMathFunctionFormat);
        FormatIdentifier braceConditionalFormatIdentifier = formats->identifier(currentDefaultBraceConditionalFormat);

        textFormatSetting->setFormatIdentifier(textFormatIdentifier);
        mathFormatSetting->setFormatIdentifier(mathFormatIdentifier);
        mathIdentifierSetting->setFormatIdentifier(mathIdentifierFormatIdentifier);
        mathFunctionSetting->setFormatIdentifier(mathFunctionFormatIdentifier);
        braceConditionalSetting->setFormatIdentifier(braceConditionalFormatIdentifier);
    }


    void RootElement::updateFromInternalDocumentSettings(const FormatsByIdentifier& format, unsigned) {
        if (currentDocumentSettings.contains(DefaultFormatSetting::defaultTextFormatSettingName)) {
            QSharedPointer<DefaultFormatSetting>
                textFormatSetting = currentDocumentSettings.value(DefaultFormatSetting::defaultTextFormatSettingName)
                                    .dynamicCast<DefaultFormatSetting>();

            FormatIdentifier identifier = textFormatSetting->formatIdentifier();

            currentDefaultTextFormat = format.value(identifier).format()->clone().dynamicCast<CharacterFormat>();
        } else {
            currentDefaultTextFormat = CharacterFormat::applicationDefaultTextFont();
        }

        if (currentDocumentSettings.contains(DefaultFormatSetting::defaultMathFormatSettingName)) {
            QSharedPointer<DefaultFormatSetting>
                mathFormatSetting = currentDocumentSettings.value(DefaultFormatSetting::defaultMathFormatSettingName)
                                    .dynamicCast<DefaultFormatSetting>();

            FormatIdentifier identifier = mathFormatSetting->formatIdentifier();

            currentDefaultMathTextFormat = format.value(identifier).format()->clone()
                                           .dynamicCast<CharacterFormat>();
        } else {
            currentDefaultMathTextFormat = CharacterFormat::applicationDefaultMathFont();
        }

        if (currentDocumentSettings.contains(DefaultFormatSetting::defaultMathIdentifierFormatSettingName)) {
            QSharedPointer<DefaultFormatSetting>
                mathIdentifierFormatSetting = currentDocumentSettings.value(
                    DefaultFormatSetting::defaultMathIdentifierFormatSettingName
                ).dynamicCast<DefaultFormatSetting>();

            FormatIdentifier identifier = mathIdentifierFormatSetting->formatIdentifier();

            currentDefaultMathIdentifierFormat = format.value(identifier).format()->clone()
                                                 .dynamicCast<CharacterFormat>();
        } else {
            currentDefaultMathIdentifierFormat = CharacterFormat::applicationDefaultMathIdentifierFont();
        }

        if (currentDocumentSettings.contains(DefaultFormatSetting::defaultMathFunctionFormatSettingName)) {
            QSharedPointer<DefaultFormatSetting>
                mathFunctionFormatSetting = currentDocumentSettings.value(
                    DefaultFormatSetting::defaultMathFunctionFormatSettingName
                ).dynamicCast<DefaultFormatSetting>();

            FormatIdentifier identifier = mathFunctionFormatSetting->formatIdentifier();

            currentDefaultMathFunctionFormat = format.value(identifier).format()->clone()
                                               .dynamicCast<CharacterFormat>();
        } else {
            currentDefaultMathFunctionFormat = CharacterFormat::applicationDefaultMathIdentifierFont();
        }

        if (currentDocumentSettings.contains(DefaultFormatSetting::defaultBraceConditionalFormatSettingName)) {
            QSharedPointer<DefaultFormatSetting>
                braceConditionalFormatSetting = currentDocumentSettings.value(
                    DefaultFormatSetting::defaultBraceConditionalFormatSettingName
                ).dynamicCast<DefaultFormatSetting>();

            FormatIdentifier identifier = braceConditionalFormatSetting->formatIdentifier();

            currentDefaultBraceConditionalFormat = format.value(identifier).format()->clone()
                                                   .dynamicCast<BraceConditionalFormat>();
        } else {
            currentDefaultBraceConditionalFormat = BraceConditionalFormat::applicationDefaultBraceConditionalFormat();
        }
    }


    void RootElement::addRootImport(
            QSharedPointer<XmlReader>,
            const QString&            importedFilePath,
            const QString&            referenceFilePath,
            bool                      isRelative,
            const PlugInsByName&      plugInData
        ) {
        QString referringPath;

        if (isRelative) {
            QString referencePath = QFileInfo(referenceFilePath).canonicalPath();
            referringPath = QString("%1/%2").arg(referencePath).arg(importedFilePath);
        } else {
            referringPath = importedFilePath;
        }

        QFileInfo referencedFileInfo(referringPath);
        QString absolutePath = referencedFileInfo.canonicalFilePath();
        if (absolutePath.isEmpty()) {
            absolutePath = referencedFileInfo.absoluteFilePath();
        }

        RootImport newImport;

        if (currentRootElements.contains(absolutePath)) {
            QSharedPointer<RootElement> importedRootElement = currentRootElements.value(absolutePath);
            newImport = RootImport(importedRootElement, isRelative);
        } else {
            QSharedPointer<RootElement> importedRootElement = Element::create(elementName)
                                                              .dynamicCast<RootElement>();
            registerRootElement(importedRootElement);

            bool success = importedRootElement->openExisting(absolutePath, false, plugInData);

            if (!success) {
                newImport = RootImport(absolutePath, isRelative);

                RootVisual* rootVisual = visual();
                if (rootVisual != nullptr) {
                    QString referenceType = isRelative ? tr("relative") : tr("absolute");
                    QString message = tr("Could not load referenced program %1 (%2)")
                                      .arg(importedFilePath)
                                      .arg(referenceType);
                    rootVisual->programLoadFailed(message);
                }
            } else {
                newImport = RootImport(importedRootElement, isRelative);
            }
        }

        currentImports.append(newImport);
    }


    unsigned RootElement::newDocumentNumber() {
        unsigned count  = globalDocumentTracker.count();
        unsigned result = 0;

        while (result < count && globalDocumentTracker.testBit(result) == true) {
            ++result;
        }

        if (result >= count) {
            globalDocumentTracker.resize(count+1);
            result = count;
        }

        globalDocumentTracker.setBit(result);

        return result;
    }


    void RootElement::freeDocumentNumber(unsigned documentNumber) {
        Q_ASSERT(documentNumber < static_cast<unsigned>(globalDocumentTracker.count()));

        globalDocumentTracker.clearBit(documentNumber);

        if (documentNumber == static_cast<unsigned>(globalDocumentTracker.count()-1)) {
            unsigned lastSet = documentNumber;
            while (lastSet > 0 && globalDocumentTracker.testBit(lastSet) == false) {
                --lastSet;
            }

            globalDocumentTracker.resize(lastSet+1);
        }
    }
}
