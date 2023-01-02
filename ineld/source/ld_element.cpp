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
* This file implements the \ref Ld::Element class.
***********************************************************************************************************************/

#include <QString>
#include <QMap>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QList>
#include <QSet>
#include <QByteArray>

#include <cassert>

#include "model_variant.h"

#include "ld_handle.h"
#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_visual.h"
#include "ld_format.h"
#include "ld_format_container.h"
#include "ld_format_organizer.h"
#include "ld_parenthesis_format.h"
#include "ld_operator_format.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_xml_element.h"
#include "ld_program_file.h"
#include "ld_diagnostic.h"
#include "ld_diagnostic_structures.h"
#include "ld_data_type.h"
#include "ld_calculated_value.h"
#include "ld_element_cursor.h"
#include "ld_cursor.h"
#include "ld_cursor_state_collection.h"
#include "ld_element.h"

namespace Ld {
    const unsigned long       Element::invalidChildIndex                = static_cast<unsigned long>(-1);
    const Element::ErrorCode  Element::noError                          = 0;
    const unsigned            Element::invalidRegionIndex               = static_cast<unsigned>(-1);
    const unsigned long       Element::invalidTextIndex                 = static_cast<unsigned long>(-1);
    const Element::Precedence Element::defaultPrecedence                = 0;
    const Element::Precedence Element::specialSymbolPrecedence          = 1800;
    const Element::Precedence Element::simpleLiteralPrecedence          = 1700;
    const Element::Precedence Element::largeOperatorPrecedence          = 1600;
    const Element::Precedence Element::powerOperatorPrecedence          = 1500;
    const Element::Precedence Element::functionOperatorPrecedence       = 1400;
    const Element::Precedence Element::unaryOperatorPrecedence          = 1300;
    const Element::Precedence Element::unaryPlusMinusOperatorPrecedence = 1200;
    const Element::Precedence Element::divisionOperatorPrecedence       = 1200;
    const Element::Precedence Element::multiplicationOperatorPrecedence = 1200;
    const Element::Precedence Element::subtrahendOperatorPrecedence     = 1100;
    const Element::Precedence Element::additionOperatorPrecedence       = 1000;
    const Element::Precedence Element::bitwiseAndOperatorPrecedence     = 900;
    const Element::Precedence Element::bitwiseOrOperatorPrecedence      = 800;
    const Element::Precedence Element::setDifferencePrecedence          = 700;
    const Element::Precedence Element::cartesianProductPrecedence       = 700;
    const Element::Precedence Element::intersectionOperatorPrecedence   = 700;
    const Element::Precedence Element::unionOperatorPrecedence          = 700;
    const Element::Precedence Element::relationalOperatorPrecedence     = 600;
    const Element::Precedence Element::logicalAndOperatorPrecedence     = 500;
    const Element::Precedence Element::logicalOrOperatorPrecedence      = 400;
    const Element::Precedence Element::logicalSpecialOperatorPrecedence = 300;
    const Element::Precedence Element::complexLiteralPrecedence         = 200;
    const Element::Precedence Element::powerOperatorExponentPrecedence  = 200;
    const Element::Precedence Element::rangeOperatorPrecedence          = 100;
    const Element::Precedence Element::assignmentOperatorPrecedence     = 100;
    const Element::Precedence Element::groupingOperatorPrecedence       = 100;
    const Element::Precedence Element::matrixOperatorPrecedence         = 100;
    const Element::Precedence Element::subscriptOperatorPrecedence      = 100;

    QMap<QString, Element::CreatorFunction> Element::creators;
    bool                                    Element::currentAutoDeleteVisual = true;

    ElementPointer Element::create(const QString& typeName) {
        Element*        newElement      = nullptr;
        CreatorFunction creatorFunction = creators.value(typeName);

        if (creatorFunction != nullptr) {
            newElement = (*creatorFunction)(typeName);
        }

        ElementPointer sharedElement(newElement);
        sharedElement->setWeakThis(sharedElement.toWeakRef());

        Visual* visual = Visual::create(typeName);
        if (visual != nullptr) {
            newElement->setVisual(visual);
        }

        return sharedElement;
    }


    bool Element::registerCreator(const QString& typeName, Element::CreatorFunction creatorFunction) {
        bool success;

        if (creatorFunction != nullptr && !creators.contains(typeName)) {
            creators.insert(typeName, creatorFunction);
            success = true;
        } else {
            success = false;
        }

        return success;
    }


    void Element::setAutoDeleteVisuals(bool nowAutoDeleteVisuals) {
        currentAutoDeleteVisual = nowAutoDeleteVisuals;
    }


    bool Element::autoDeleteVisuals() {
        return currentAutoDeleteVisual;
    }


    Element::Element() {
        currentVisual = nullptr;
        currentFormat = nullptr;

        currentDiagnostic.reset();

        currentHandle = Ld::Handle::create();
    }


    Element::~Element() {
        if (currentVisual != nullptr) {
            // If the visual is deleted explicitly outside of the Element/Visual implementation, the visual will
            // reach back into the element to inform the element that there is no longer a visual tied to it.  The
            // line below keeps the visual from doing then when deleted by the Element class's destructor.

            currentVisual->currentElement.clear();

            if (currentAutoDeleteVisual) {
                delete currentVisual;
            } else {
                currentVisual->decoupledFromElement();
            }
        }

        if (currentFormat) {
            currentFormat->currentElements.remove(currentWeakThis);
        }
    }


    void Element::setWeakThis(const ElementWeakPointer& newWeakThis) {
        currentWeakThis = newWeakThis;
    }


    ElementWeakPointer Element::weakThis() const {
        return currentWeakThis;
    }


    Element::Precedence Element::childPrecedence() const {
        return defaultPrecedence;
    }


    Element::Precedence Element::parentPrecedence(unsigned long /* childIndex */) const {
        return childPrecedence();
    }


    Element::Precedence Element::parentPrecedence(ElementPointer child) const {
        return parentPrecedence(indexOfChild(child));
    }


    void Element::setFormat(FormatPointer newFormat) {
        // The assert below will trigger if the weak pointer for this element is not set before this method is called.
        assert(currentWeakThis);

        FormatPointer oldFormat = currentFormat;
        currentFormat = newFormat;

        if (!oldFormat.isNull()) {
            oldFormat->currentElements.remove(currentWeakThis);
        }

        if (!currentFormat.isNull()) {
            currentFormat->currentElements.insert(currentWeakThis);
        }

        if (currentVisual != nullptr) {
            currentVisual->formatChanged(oldFormat, newFormat);
        }

        ElementPointer parent = currentParent.toStrongRef();
        if (!parent.isNull()) {
            parent->descendantFormatChanged(currentWeakThis.toStrongRef(), newFormat);
        }
    }


    void Element::setFormat(Ld::Format* newFormat) {
        setFormat(FormatPointer(newFormat));
    }


    FormatPointer Element::format() const {
        return currentFormat;
    }


    Element::ParenthesisStyle Element::parenthesisStyle() const {
        ParenthesisStyle result = ParenthesisStyle::INVALID_OR_NONE;

        if (currentFormat->capabilities().contains(OperatorFormat::formatName)) {
            QSharedPointer<OperatorFormat> operatorFormat = currentFormat.dynamicCast<OperatorFormat>();
            assert(!operatorFormat.isNull());

            if (operatorFormat->forcedParenthesis() || precedenceSuggestsParenthesis()) {
                result = operatorFormat->parenthesisStyle();
            }
        } else if (precedenceSuggestsParenthesis()) {
            if (currentFormat->capabilities().contains(ParenthesisFormat::formatName)) {
                QSharedPointer<ParenthesisFormat> parenthesisFormat = currentFormat.dynamicCast<ParenthesisFormat>();
                assert(!parenthesisFormat.isNull());

                result = parenthesisFormat->parenthesisStyle();
            } else {
                result = ParenthesisStyle::DEFAULT;
            }
        }

        return result;
    }


    ElementPointer Element::parent() const {
        return currentParent.toStrongRef();
    }


    bool Element::isOnlyChild() const {
        bool onlyChild;

        ElementPointer parent = currentParent.toStrongRef();
        if (parent.isNull()) {
            onlyChild = true;
        } else {
            if (parent->numberChildren() == 1) {
                onlyChild = true;
            } else {
                onlyChild = false;
            }
        }

        return onlyChild;
    }


    bool Element::isFirstChild() const {
        bool firstChild;

        ElementPointer parent = currentParent.toStrongRef();
        if (parent.isNull()) {
            firstChild = true;
        } else {
            if (parent->indexOfChild(currentWeakThis.toStrongRef()) == 0) {
                firstChild = true;
            } else {
                firstChild = false;
            }
        }

        return firstChild;
    }


    bool Element::isLastChild() const {
        bool lastChild;

        ElementPointer parent = currentParent.toStrongRef();
        if (parent.isNull()) {
            lastChild = true;
        } else {
            if (parent->indexOfChild(currentWeakThis.toStrongRef()) == parent->numberChildren() - 1) {
                lastChild = true;
            } else {
                lastChild = false;
            }
        }

        return lastChild;
    }


    ElementPointer Element::previousSibling(bool* firstSibling) const {
        ElementPointer result;

        ElementPointer parent    = currentParent.toStrongRef();
        unsigned long  thisIndex = parent->indexOfChild(currentWeakThis.toStrongRef());

        if (thisIndex > 0) {
            result = parent->child(thisIndex - 1);

            if (firstSibling != nullptr) {
                *firstSibling = false;
            }
        } else {
            if (firstSibling != nullptr) {
                *firstSibling = true;
            }
        }

        return result;
    }


    ElementPointer Element::nextSibling(bool* lastSibling) const {
        ElementPointer result;

        ElementPointer parent         = currentParent.toStrongRef();
        unsigned long  thisIndex      = parent->indexOfChild(currentWeakThis.toStrongRef());
        unsigned long  numberChildren = parent->numberChildren();

        if (thisIndex < numberChildren - 1) {
            result = parent->child(thisIndex + 1);

            if (lastSibling != nullptr) {
                *lastSibling = false;
            }
        } else {
            if (lastSibling != nullptr) {
                *lastSibling = true;
            }
        }

        return result;
    }


    unsigned Element::depth() const {
        unsigned       count = 0;
        ElementPointer node  = currentParent.toStrongRef();

        while (!node.isNull()) {
            node = node->currentParent.toStrongRef();
            ++count;
        }

        return count;
    }


    Ld::Handle Element::handle() const {
        return currentHandle;
    }


    void Element::setVisual(Visual* newVisual) {
        // The assert below will trigger if the weak pointer for this element is not set before this method is called.
        assert(currentWeakThis);

        Visual* oldVisual = currentVisual;

        if (currentVisual != nullptr) {
            currentVisual->currentElement = nullptr;
        }

        if (newVisual != nullptr) {
            ElementPointer oldElement = newVisual->currentElement.toStrongRef();
            if (oldElement) {
                oldElement->currentVisual = nullptr;
            }

            newVisual->currentElement = currentWeakThis;
        }

        currentVisual = newVisual;

        if (oldVisual != nullptr) {
            oldVisual->decoupledFromElement();
        }

        if (currentVisual != nullptr) {
            currentVisual->coupledToElement(currentWeakThis.toStrongRef());
        }
    }


    Visual* Element::visual() const {
        return currentVisual;
    }


    ElementPointer Element::root() const {
        ElementPointer result;

        if (!currentParent) {
            result = currentWeakThis.toStrongRef();
        } else {
            ElementPointer parent = currentParent.toStrongRef();

            while (parent) {
                result = parent;
                parent = parent->currentParent;
            }
        }

        return result;
    }


    void Element::flagDiagnostic(DiagnosticPointer diagnostic) {
        currentDiagnostic = diagnostic;

        if (currentVisual != nullptr) {
            currentVisual->diagnosticReported(currentDiagnostic);
        }
    }


    void Element::clearDiagnostic() {
        currentDiagnostic.reset();

        if (currentVisual != nullptr) {
            currentVisual->diagnosticCleared();
        }
    }


    bool Element::diagnosticReported() const {
        return !currentDiagnostic.isNull();
    }


    DiagnosticPointer Element::diagnostic() const {
        return currentDiagnostic;
    }


    bool Element::supportsInstructionBreakpoints() const {
        ElementPointer p = parent();
        return !p.isNull() && p->supportsInstructionBreakpoints();
    }


    bool Element::setInstructionBreakpoint(bool nowSet) {
        ElementPointer p = parent();
        return !p.isNull() && p->setInstructionBreakpoint(nowSet);
    }


    bool Element::clearInstructionBreakpoint(bool nowClear) {
        return setInstructionBreakpoint(!nowClear);
    }


    bool Element::instructionBreakpointSet() const {
        ElementPointer p = parent();
        return !p.isNull() && p->instructionBreakpointSet();
    }


    ElementPointerSet Element::descendants() const {
        ElementPointerSet result;

        ElementPointerList childList = children();
        for (  ElementPointerList::const_iterator it = childList.constBegin(), end = childList.constEnd()
             ; it != end
             ; ++it
            ) {
            ElementPointer child = *it;
            result.insert(child);

            if (!child.isNull()) {
                result += child->descendants();
            }
        }

        return result;
    }


    ElementPointerList Element::ancestors() const {
        ElementPointerList ancestorList;

        ElementPointer parent = currentParent;
        while (!parent.isNull()) {
            ancestorList.prepend(parent);
            parent = parent->currentParent.toStrongRef();
        }

        return ancestorList;
    }


    Capabilities Element::textCapabilities(unsigned long, unsigned) const {
        return (
              numberTextRegions() == 0
            ? Capabilities()
            : Capabilities::allAlphanumericSymbols
        );
    }


    bool Element::aggregateTextDuringSearchAllowed(ElementPointer, unsigned, unsigned) const {
        return false;
    }


    bool Element::isListElement() const {
        return false;
    }


    bool Element::isMatrix() const {
        return false;
    }


    bool Element::isPlaceholder() const {
        return false;
    }


    bool Element::isTopLevelOperator() const {
        return false;
    }


    bool Element::isFunctionLogicalStop(unsigned long functionChildIndex) const {
        bool result;

        ElementPointer p = parent();
        if (p.isNull()) {
            result = true;
        } else {
            unsigned long indexOfThisChild = p->indexOfChild(currentWeakThis.toStrongRef());
            result = (p->isFunctionLogicalStop(indexOfThisChild) && functionChildIndex == numberChildren() - 1);
        }

        return result;
    }


    unsigned Element::minimumNumberTextRegions() const {
        return 0;
    }


    unsigned Element::maximumNumberTextRegions() const {
        return 0;
    }


    bool Element::setNumberTextRegions(unsigned newNumberTextRegions) {
        return newNumberTextRegions == 0 ? true : false;
    }


    unsigned Element::numberTextRegions() const {
        return 0;
    }


    bool Element::setText(const QString&, unsigned) {
        return false;
    }


    bool Element::insertText(
            const QString&         textToInsert,
            unsigned long          textIndex,
            unsigned               regionIndex,
            CursorStateCollection* cursorStateCollection,
            bool                   preferEarlierPosition
        ) {
        bool     result;
        unsigned textRegions = numberTextRegions();

        if (textRegions > 0) {
            unsigned      region     = regionIndex < textRegions ? regionIndex : textRegions - 1;
            QString       regionText = text(region);
            unsigned long textLength = static_cast<unsigned long>(regionText.length());
            unsigned long index      = textIndex < textLength ? textIndex : textLength;

            QString  leftOfInsert  = regionText.left(index);
            QString  rightOfInsert = regionText.mid(index);
            QString  newText       = leftOfInsert + textToInsert + rightOfInsert;

            setText(newText, region);

            if (cursorStateCollection != nullptr) {
                for (  CursorStateCollection::iterator it  = cursorStateCollection->begin(),
                                                       end = cursorStateCollection->end()
                     ; it != end
                     ; ++it
                    ) {
                    if (it->isTextInElement() && it->element() == weakThis() && it->regionIndex() == region) {
                        unsigned cursorTextIndex = it->textIndex();
                        if (cursorTextIndex > index || (!preferEarlierPosition && cursorTextIndex == index)) {
                            it->update(cursorTextIndex + textToInsert.length(), region, weakThis().toStrongRef());
                        }
                    }
                }
            }

            result = true;
        } else {
            result = false;
        }

        return result;
    }


    bool Element::removeText(
            unsigned long          startingTextIndex,
            unsigned               startingRegionIndex,
            unsigned long          endingTextIndex,
            unsigned               endingRegionIndex,
            CursorStateCollection* cursorStateCollection
        ) {
        unsigned numberRegions = numberTextRegions();

        if (numberRegions > 0) {
            unsigned endingIndex = std::min(endingRegionIndex, numberRegions - 1);
            unsigned regionIndex = startingRegionIndex;

            while (regionIndex <= endingIndex) {
                QString newText;

                if (regionIndex == startingRegionIndex) {
                    if (regionIndex == endingRegionIndex) {
                        QString currentText  = text(regionIndex);
                        QString leftPortion  = currentText.left(startingTextIndex);
                        QString rightPortion = currentText.mid(endingTextIndex);

                        newText = leftPortion + rightPortion;
                    } else {
                        QString currentText = text(regionIndex);
                        newText = currentText.left(startingTextIndex);
                    }
                } else if (regionIndex == endingRegionIndex) {
                    QString currentText = text(regionIndex);
                    newText = currentText.mid(endingTextIndex);
                }

                setText(newText, regionIndex);
                ++regionIndex;
            }

            if (cursorStateCollection != nullptr) {
                for (  CursorStateCollection::iterator it  = cursorStateCollection->begin(),
                                                       end = cursorStateCollection->end()
                     ; it != end
                     ; ++it
                    ) {
                    if (it->isTextInElement() && it->element() == weakThis()) {
                        unsigned      cursorRegionIndex = it->regionIndex();
                        unsigned long cursorTextIndex   = it->textIndex();

                        if (cursorRegionIndex == startingRegionIndex) {
                            if (cursorTextIndex > startingTextIndex) {
                                if (cursorRegionIndex != endingRegionIndex || cursorTextIndex <= endingTextIndex) {
                                    it->update(startingTextIndex, startingRegionIndex, weakThis().toStrongRef());
                                } else {
                                    it->update(
                                        cursorTextIndex + startingTextIndex - endingTextIndex,
                                        startingRegionIndex,
                                        weakThis().toStrongRef()
                                    );
                                }
                            }
                        } else if (cursorRegionIndex == endingRegionIndex) {
                            unsigned long newTextIndex =   cursorTextIndex <= endingTextIndex
                                                         ? 0
                                                         : cursorTextIndex - endingTextIndex;
                            it->update(newTextIndex, endingRegionIndex, weakThis().toStrongRef());
                        } else if (cursorRegionIndex > startingRegionIndex && cursorRegionIndex < endingRegionIndex) {
                            it->update(
                                text(startingRegionIndex).length(),
                                startingRegionIndex,
                                weakThis().toStrongRef()
                            );
                        }
                    }
                }
            }
        }

        return true;
    }


    QString Element::text(unsigned) const {
        return QString();
    }


    Element::CursorParkMode Element::cursorParkMode() const {
        return CursorParkMode::PARK_ON_ELEMENT_ONLY;
    }


    bool Element::requiresCalculatedDataToRender() const {
        return false;
    }


    unsigned Element::numberCalculatedValues() const {
        return 0;
    }


    void Element::setCalculatedValue(unsigned valueIndex, const CalculatedValue& calculatedValue) {
        if (currentVisual != nullptr) {
            currentVisual->calculatedValueUpdated(valueIndex, calculatedValue);
        }
    }


    void Element::clearCalculatedValue() {
        if (currentVisual != nullptr) {
            currentVisual->calculatedValueCleared();
        }
    }


    CalculatedValue Element::calculatedValue(unsigned /* valueIndex */) const {
        return CalculatedValue();
    }


    bool Element::supportsPayloads() const {
        return false;
    }


    bool Element::getPayload(QByteArray& /* newData */) {
        return false;
    }


    Element::ExportImageCapability Element::exportImageCapability() const {
        return ExportImageCapability::THROUGH_VISUAL;
    }


    QByteArray Element::exportImage(float dpi) {
        QByteArray result;

        if (currentVisual != nullptr) {
            result = currentVisual->generateImage(dpi);
        }

        return result;
    }


    void Element::descendantAdded(ElementPointer descendantElement) {
        ElementPointer parent = currentParent.toStrongRef();
        if (parent) {
            parent->descendantAdded(descendantElement);
        }
    }


    void Element::descendantRemoved(ElementPointer descendantElement) {
        ElementPointer parent = currentParent.toStrongRef();
        if (parent) {
            parent->descendantRemoved(descendantElement);
        }
    }


    void Element::graftedToTree() {
        updateAfterGraft();

        if (currentVisual != nullptr) {
            currentVisual->graftedToTree();
        }
    }


    void Element::aboutToUngraftFromTree() {
        if (currentVisual != nullptr) {
            currentVisual->aboutToUngraftFromTree();
        }
    }


    void Element::childChanged(ElementPointer changedChild) {
        ElementPointer parent = currentParent.toStrongRef();
        if (parent) {
            parent->childChanged(changedChild);
        }
    }


    void Element::formatUpdated() {
        if (currentVisual != nullptr) {
            assert(currentFormat != nullptr);
            currentVisual->formatChanged(currentFormat, currentFormat);
        }

        ElementPointer parent = currentParent.toStrongRef();
        if (!parent.isNull()) {
            parent->descendantFormatUpdated(currentWeakThis.toStrongRef(), currentFormat);
        }
    }


    void Element::descendantFormatChanged(ElementPointer element, FormatPointer format) {
        if (currentVisual != nullptr) {
            currentVisual->descendantFormatChanged(element, format);
        }

        ElementPointer parent = currentParent.toStrongRef();
        if (!parent.isNull()) {
            parent->descendantFormatChanged(element, format);
        }
    }


    void Element::descendantFormatUpdated(ElementPointer element, FormatPointer format) {
        if (currentVisual != nullptr) {
            currentVisual->descendantFormatUpdated(element, format);
        }

        ElementPointer parent = currentParent.toStrongRef();
        if (!parent.isNull()) {
            parent->descendantFormatUpdated(element, format);
        }
    }


    void Element::elementDataChanged() {
        if (currentVisual != nullptr) {
            currentVisual->elementDataChanged();
        }

        ElementPointer strongThis = currentWeakThis.toStrongRef();
        childChanged(strongThis);
    }


    void Element::instructionBreakpointStatusChanged(bool breakpointNowSet) {
        if (currentVisual != nullptr) {
            currentVisual->instructionBreakpointUpdated(breakpointNowSet);
        }
    }


    void Element::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&
        ) const {
        attributes.append("handle", handle());

        if (currentFormat) {
            FormatIdentifier identifier = formats->identifier(currentFormat);
            Q_ASSERT(identifier != invalidFormatIdentifier);

            attributes.append("format", identifier);
        }
    }


    void Element::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&,
            unsigned
        ) {
        if (attributes.hasAttribute("handle")) {
            bool ok;
            Handle handle = attributes.value<Handle>("handle", &ok);

            if (!ok) {
                reader->raiseError(tr("Tag \"%1\" has invalid handle").arg(reader->qualifiedName().toString()));
            } else {
                currentHandle = handle;
            }
        } else {
            reader->raiseError(tr("Tag \"%1\" is missing handle").arg(reader->qualifiedName().toString()));
        }

        if (!reader->hasError() && attributes.hasAttribute("format")) {
            FormatIdentifier formatIdentifier;
            bool ok;
            formatIdentifier = attributes.value<FormatIdentifier>("format", &ok);

            if (!ok) {
                reader->raiseError(tr("Tag \"%1\" has invalid format ID").arg(reader->qualifiedName().toString()));
            } else if (!formats.contains(formatIdentifier)) {
                reader->raiseError(tr("Tag \"%1\" format undefined").arg(reader->qualifiedName().toString()));
            } else {
                FormatPointer newFormat = formats.value(formatIdentifier).format();
                setFormat(newFormat);
            }
        }
    }


    void Element::updateCursorsFromDescendantsToThis(CursorStateCollection *cursorStateCollection) const {
        if (cursorStateCollection != nullptr) {
            ElementPointerSet descendants = Element::descendants();
            for (  CursorStateCollection::iterator it = cursorStateCollection->begin(),
                                                   end = cursorStateCollection->end()
                 ; it != end
                 ; ++it
                ) {
                if (it->isValid()) {
                    ElementPointer cursorElement = it->element();
                    ElementPointer cursorParent  = it->parent();

                    if ((!cursorElement.isNull()             &&
                         descendants.contains(cursorElement)    ) ||
                        descendants.contains(cursorParent)           ) {
                        if (!parent().isNull()) {
                            it->updateToLastPositionIn(weakThis().toStrongRef(), true);
                        } else {
                            it->invalidate();
                        }
                    }
                }
            }
        }
    }


    void Element::updateAfterGraft() {}


    ElementPointer Element::cloneThisElementOnly() const {
        ElementPointer newElement = create(typeName());

        if (!newElement.isNull()) {
            FormatPointer oldFormat = format();

            if (!oldFormat.isNull()) {
                newElement->setFormat(oldFormat->clone());
            }

            unsigned countTextRegions = numberTextRegions();
            bool     success          = newElement->setNumberTextRegions(countTextRegions);

            unsigned regionIndex = 0;
            while (success && regionIndex < countTextRegions) {
                success = newElement->setText(text(regionIndex), regionIndex);
                ++regionIndex;
            }

            if (!success) {
                newElement.clear();
            }
        }

        return newElement;
    }


    ElementPointer Element::cloneThisElementOnly(
            unsigned long startingTextIndex,
            unsigned      startingRegionIndex,
            unsigned long endingTextIndex,
            unsigned      endingRegionIndex
        ) const {
        ElementPointer newElement = create(typeName());

        if (!newElement.isNull()) {
            FormatPointer oldFormat = format();

            if (!oldFormat.isNull()) {
                newElement->setFormat(oldFormat->clone());
            }

            unsigned countTextRegions = numberTextRegions();
            bool     success          = newElement->setNumberTextRegions(countTextRegions);

            unsigned regionIndex = 0;
            while (success && regionIndex < countTextRegions) {
                if (regionIndex < startingRegionIndex || regionIndex > endingRegionIndex) {
                    success = newElement->setText(QString());
                } else {
                    QString  currentText = text(regionIndex);
                    unsigned textLength  = static_cast<unsigned>(currentText.length());

                    if (textLength > 0) {
                        unsigned startingIndex = 0;
                        unsigned endingIndex   = textLength;

                        if (regionIndex == startingRegionIndex) {
                            if (startingTextIndex > textLength) {
                                success = false;
                            } else {
                                startingIndex = startingTextIndex;
                            }
                        }

                        if (regionIndex == endingRegionIndex && endingTextIndex < textLength) {
                            endingIndex = endingTextIndex;
                        }

                        currentText = currentText.mid(startingIndex, endingIndex - startingIndex);
                    }

                    success = newElement->setText(currentText, regionIndex);
                }

                ++regionIndex;
            }

            if (!success) {
                newElement.clear();
            }
        }

        return newElement;
    }


    bool Element::precedenceSuggestsParenthesis() const {
        bool           result;
        ElementPointer parent = currentParent.toStrongRef();
        if (!parent.isNull()) {
            unsigned long thisChildIndex   = parent->indexOfChild(currentWeakThis.toStrongRef());
            Precedence    parentPrecedence = parent->parentPrecedence(thisChildIndex);

            result = childPrecedence() < parentPrecedence;
        } else {
            result = false;
        }

        return result;
    }
}
