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
* This file implements the \ref Ld::Cursor class.
***********************************************************************************************************************/

#include <QSharedPointer>
#include <QString>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QMap>

#include <cmath>

#include <util_hash_functions.h>

#include "ld_element_structures.h"
#include "ld_element.h"
#include "ld_location.h"
#include "ld_capabilities.h"
#include "ld_element_cursor.h"
#include "ld_root_element.h"
#include "ld_cursor.h"

namespace Ld {
    Cursor::Cursor(QWeakPointer<RootElement> newRootElement) {
        currentElementCursor.setInvalid();
        currentSelectionCursor.setInvalid();

        lastWasSelection   = false;
        currentRootElement = newRootElement;
    }


    Cursor::Cursor(const Cursor& other) {
        currentElementCursor   = other.currentElementCursor;
        currentSelectionCursor = other.currentSelectionCursor;
        currentRootElement     = other.currentRootElement;
        lastWasSelection       = other.lastWasSelection;
    }


    Cursor::~Cursor() {}


    void Cursor::setRootElement(QWeakPointer<RootElement> newRootElement) {
        currentRootElement = newRootElement;
    }


    QWeakPointer<RootElement> Cursor::rootElement() const {
        return currentRootElement;
    }


    Capabilities Cursor::requirementsAtCursor() const {
        Capabilities requirements;

        if (currentSelectionCursor.isValid()) {
            requirements = Capabilities::selection;
        } else if (currentElementCursor.isValid()) {
            ElementPointer element = currentElementCursor.element();

            if (currentElementCursor.isWholeElement()) {
                if (element.isNull()) {
                    ElementPointer parent     = currentElementCursor.element();
                    unsigned long  childIndex = currentElementCursor.childIndex();

                    requirements = parent->parentRequires(childIndex);
                } else {
                    ElementPointer parent     = currentElementCursor.parent();
                    unsigned long  childIndex = currentElementCursor.childIndex();

                    requirements = parent->parentRequires(childIndex);
                }
            } else {
                Q_ASSERT(!element.isNull());

                unsigned      regionIndex = currentElementCursor.regionIndex();
                unsigned long textOffset  = currentElementCursor.textIndex();

                requirements = element->textCapabilities(textOffset, regionIndex);
            }
        }

        return requirements;
    }


    const ElementCursor& Cursor::elementCursor() const {
        return currentElementCursor;
    }


    const ElementCursor& Cursor::selectionCursor() const {
        return currentSelectionCursor;
    }


    void Cursor::updateFromElementCursor(const ElementCursor& newPosition, bool keepSelection) {
        if (newPosition.isValid()) {
            ElementCursor oldPosition = currentElementCursor;

            if (currentRootElement.toStrongRef().isNull()) {
                ElementPointer              parent      = newPosition.parent();
                QSharedPointer<RootElement> rootElement = parent->root().dynamicCast<RootElement>();
                currentRootElement = rootElement;
            }

            currentElementCursor = newPosition;
            currentElementCursor.fixPosition(true, false);

            if (!keepSelection || currentSelectionCursor == currentElementCursor) {
                currentSelectionCursor = ElementCursor();
                lastWasSelection       = false;

                selectionCursorChanged();
            }

            bool movingForward = oldPosition.isInvalid() || newPosition > oldPosition;
            cursorChanged(movingForward);
        } else {
            currentElementCursor.setInvalid();
            currentSelectionCursor.setInvalid();
            cursorChanged(false);
        }
    }


    void Cursor::updateSelectionFromElementCursor(const ElementCursor& newPosition) {
        currentSelectionCursor = newPosition;
        currentSelectionCursor.fixPosition(true, false);

        selectionCursorChanged();
    }


    bool Cursor::moveForwardByElement() {
        bool isLast = false;

        if (okToAdjustCursor(isLast)) {
            isLast = currentElementCursor.moveForwardByElement();
            isLast = currentElementCursor.fixPosition(true, isLast);

            cursorChanged(true);
        }

        return isLast;
    }


    bool Cursor::moveBackwardByElement() {
        bool isFirst;

        if (okToAdjustCursor(isFirst)) {
            isFirst = currentElementCursor.moveBackwardByElement();
            isFirst = currentElementCursor.fixPosition(false, isFirst);

            cursorChanged(false);
        }

        return isFirst;
    }


    bool Cursor::moveForwardByRegion() {
        bool isLast;

        if (okToAdjustCursor(isLast)) {
            isLast = currentElementCursor.moveForwardByRegion();
            isLast = currentElementCursor.fixPosition(true, isLast);

            cursorChanged(true);
        }

        return isLast;
    }


    bool Cursor::moveBackwardByRegion() {
        bool isFirst;

        if (okToAdjustCursor(isFirst)) {
            isFirst = currentElementCursor.moveBackwardByRegion();
            isFirst = currentElementCursor.fixPosition(false, isFirst);

            cursorChanged(false);
        }

        return isFirst;
    }


    bool Cursor::moveForwardByWhitespace() {
        bool isLast;

        if (okToAdjustCursor(isLast)) {
            isLast = currentElementCursor.moveForwardByWhitespace();
            isLast = currentElementCursor.fixPosition(true, isLast);

            cursorChanged(true);
        }

        return isLast;
    }


    bool Cursor::moveBackwardByWhitespace() {
        bool isFirst;

        if (okToAdjustCursor(isFirst)) {
            isFirst = currentElementCursor.moveBackwardByWhitespace();
            isFirst = currentElementCursor.fixPosition(false, isFirst);

            cursorChanged(false);
        }

        return isFirst;
    }


    bool Cursor::moveForwardByCharacter() {
        bool isLast;

        if (okToAdjustCursor(isLast)) {
            isLast = currentElementCursor.moveForwardByCharacter();
            isLast = currentElementCursor.fixPosition(true, isLast);

            cursorChanged(true);
        }

        return isLast;
    }


    bool Cursor::moveBackwardByCharacter() {
        bool isFirst;

        if (okToAdjustCursor(isFirst)) {
            isFirst = currentElementCursor.moveBackwardByCharacter();
            isFirst = currentElementCursor.fixPosition(false, isFirst);

            cursorChanged(false);
        }

        return isFirst;
    }


    bool Cursor::moveToStartOfDocumentView() {
        bool isFirst;

        if (okToAdjustCursor(isFirst)) {
            currentElementCursor.moveToBeginning();
            isFirst = currentElementCursor.fixPosition(false, false);

            cursorChanged(false);
        }

        return isFirst;
    }


    bool Cursor::moveToEndOfDocumentView() {
        bool isLast;

        if (okToAdjustCursor(isLast)) {
            currentElementCursor.moveToEnd();
            isLast = currentElementCursor.fixPosition(true, false);

            cursorChanged(true);
        }

        return isLast;
    }


    void Cursor::startSelection() {
        currentSelectionCursor = currentElementCursor;
        lastWasSelection = true;

        selectionCursorChanged();
    }


    void Cursor::clearSelection() {
        currentSelectionCursor = ElementCursor();
        lastWasSelection = false;

        selectionCursorChanged();
    }


    void Cursor::selectAll() {
        moveToEndOfDocumentView();
        startSelection();
        moveToStartOfDocumentView();
    }


    Cursor::SearchResult Cursor::searchForward(
            const QString& searchText,
            bool           caseSensitive,
            bool           wholeWordsOnly,
            bool           regularExpression,
            bool           okToLoop
        ) {
        SearchResult result = SearchResult::NOT_FOUND;

        ElementCursor newElementCursor;
        ElementCursor newSelectionCursor;

        QRegularExpression regexp;
        if (regularExpression) {
            regexp = QRegularExpression(searchText);
            if (!regexp.isValid()) {
                result = SearchResult::INVALID_REGULAR_EXPRESSION;
            }
        }

        if (result == SearchResult::NOT_FOUND && currentElementCursor.isInvalid()) {
            result = SearchResult::INVALID_STARTING_POSITION;
        }

        if (result == SearchResult::NOT_FOUND) {
            bool endAtSelectionCursor = (
                   currentSelectionCursor.isValid()
                && currentSelectionCursor < currentElementCursor
            );

            bool startAtSelectionCursor = (
                   currentSelectionCursor.isValid()
                && currentSelectionCursor > currentElementCursor
            );

            const ElementCursor& endingCursor = endAtSelectionCursor   ? currentSelectionCursor : currentElementCursor;
            ElementCursor        activeCursor = startAtSelectionCursor ? currentSelectionCursor : currentElementCursor;

            QString                            stream;
            QMap<unsigned long, ElementCursor> streamOffsets;
            bool                               looped = false;

            ElementPointer previousElement;
            unsigned       previousRegionIndex = Element::invalidRegionIndex;

            do {
                bool needToLoop = false;

                if (activeCursor.isWholeElement()) {
                    result = searchForwardThroughTextStream(
                        searchText,
                        regexp,
                        stream,
                        streamOffsets,
                        caseSensitive,
                        wholeWordsOnly,
                        regularExpression,
                        newElementCursor,
                       newSelectionCursor
                    );

                    if (result == SearchResult::NOT_FOUND) {
                        stream.clear();
                        streamOffsets.clear();

                        previousElement.clear();
                        previousRegionIndex = Element::invalidRegionIndex;

                        needToLoop = activeCursor.moveForwardByElement();
                        activeCursor.moveToFirstPositionInElement();
                        needToLoop = activeCursor.fixPosition(true, needToLoop);
                    }
                } else {
                    ElementPointer element = activeCursor.element();
                    if (element.isNull()) {
                        result = searchForwardThroughTextStream(
                            searchText,
                            regexp,
                            stream,
                            streamOffsets,
                            caseSensitive,
                            wholeWordsOnly,
                            regularExpression,
                            newElementCursor,
                           newSelectionCursor
                        );

                        if (result == SearchResult::NOT_FOUND) {
                            stream.clear();
                            streamOffsets.clear();

                            previousElement.clear();
                            previousRegionIndex = Element::invalidRegionIndex;

                            needToLoop = activeCursor.moveForwardByElement();
                            activeCursor.moveToFirstPositionInElement();
                            needToLoop = activeCursor.fixPosition(true, needToLoop);
                        }
                    } else {
                        unsigned currentRegionIndex = activeCursor.regionIndex();

                        if (!previousElement.isNull()                          &&
                            !previousElement->aggregateTextDuringSearchAllowed(
                                element,
                                currentRegionIndex,
                                previousRegionIndex
                            )                                                     ) {
                            result = searchForwardThroughTextStream(
                                searchText,
                                regexp,
                                stream,
                                streamOffsets,
                                caseSensitive,
                                wholeWordsOnly,
                                regularExpression,
                                newElementCursor,
                                newSelectionCursor
                            );

                            stream.clear();
                            streamOffsets.clear();

                            previousElement.clear();
                            previousRegionIndex = Element::invalidRegionIndex;
                        }

                        if (result == SearchResult::NOT_FOUND) {
                            unsigned long currentTextIndex = activeCursor.textIndex();

                            streamOffsets.insert(stream.length(), activeCursor);

                            QString textToAdd;
                            if (currentTextIndex > 0) {
                                textToAdd = element->text(currentRegionIndex).mid(currentTextIndex);
                            } else {
                                textToAdd = element->text(currentRegionIndex);
                            }

                            if (looped                                           &&
                                !endingCursor.isWholeElement()                   &&
                                currentRegionIndex == endingCursor.regionIndex() &&
                                element == endingCursor.element()                   ) {
                                textToAdd = textToAdd.left(endingCursor.textIndex());
                            }

                            stream += textToAdd;

                            needToLoop = activeCursor.moveForwardByRegion();
                            needToLoop = activeCursor.fixPosition(true, needToLoop);

                            previousElement     = element;
                            previousRegionIndex = currentRegionIndex;
                        }
                    }
                }

                if (needToLoop) {
                    looped = true;
                    activeCursor.moveToBeginning();
                    activeCursor.fixPosition(false, false);
                }
            } while (result == SearchResult::NOT_FOUND                         &&
                     ((!looped && activeCursor >= endingCursor)           ||
                      (looped && okToLoop && activeCursor < endingCursor)    )    );
        }

        if (result == SearchResult::FOUND) {
            ElementCursor oldElementCursor = currentElementCursor;

            currentElementCursor   = newElementCursor;
            currentSelectionCursor = newSelectionCursor;

            cursorChanged(oldElementCursor < currentElementCursor);
        }

        return result;
    }


    Cursor::SearchResult Cursor::searchBackward(
            const QString& searchText,
            bool           caseSensitive,
            bool           wholeWordsOnly,
            bool           regularExpression,
            bool           okToLoop
        ) {
        SearchResult result = SearchResult::NOT_FOUND;

        ElementCursor newElementCursor;
        ElementCursor newSelectionCursor;

        QRegularExpression regexp;
        if (regularExpression) {
            regexp = QRegularExpression(searchText);
            if (!regexp.isValid()) {
                result = SearchResult::INVALID_REGULAR_EXPRESSION;
            }
        }

        if (result == SearchResult::NOT_FOUND && currentElementCursor.isInvalid()) {
            result = SearchResult::INVALID_STARTING_POSITION;
        }

        if (result == SearchResult::NOT_FOUND) {
            bool endAtSelectionCursor = (
                   currentSelectionCursor.isValid()
                && currentSelectionCursor > currentElementCursor
            );

            bool startAtSelectionCursor = (
                   currentSelectionCursor.isValid()
                && currentSelectionCursor < currentElementCursor
            );

            const ElementCursor& endingCursor = endAtSelectionCursor   ? currentSelectionCursor : currentElementCursor;
            ElementCursor        activeCursor = startAtSelectionCursor ? currentSelectionCursor : currentElementCursor;

            QString                            stream;
            QMap<unsigned long, ElementCursor> streamOffsets;
            bool                               looped = false;

            ElementPointer previousElement;
            unsigned       previousRegionIndex = Element::invalidRegionIndex;

            do {
                bool needToLoop = false;

                if (activeCursor.isWholeElement()) {
                    result = searchBackwardThroughTextStream(
                        searchText,
                        regexp,
                        stream,
                        streamOffsets,
                        caseSensitive,
                        wholeWordsOnly,
                        regularExpression,
                        newElementCursor,
                       newSelectionCursor
                    );

                    if (result == SearchResult::NOT_FOUND) {
                        stream.clear();
                        streamOffsets.clear();

                        previousElement.clear();
                        previousRegionIndex = Element::invalidRegionIndex;

                        needToLoop = activeCursor.moveBackwardByElement();
                        activeCursor.moveToLastPositionInElement();
                        needToLoop = activeCursor.fixPosition(false, needToLoop);
                    }
                } else {
                    ElementPointer element = activeCursor.element();
                    if (element.isNull()) {
                        result = searchBackwardThroughTextStream(
                            searchText,
                            regexp,
                            stream,
                            streamOffsets,
                            caseSensitive,
                            wholeWordsOnly,
                            regularExpression,
                            newElementCursor,
                           newSelectionCursor
                        );

                        if (result == SearchResult::NOT_FOUND) {
                            stream.clear();
                            streamOffsets.clear();

                            previousElement.clear();
                            previousRegionIndex = Element::invalidRegionIndex;

                            needToLoop = activeCursor.moveBackwardByElement();
                            activeCursor.moveToLastPositionInElement();
                            needToLoop = activeCursor.fixPosition(false, needToLoop);
                        }
                    } else {
                        unsigned currentRegionIndex = activeCursor.regionIndex();

                        if (!previousElement.isNull()                           &&
                            !element->aggregateTextDuringSearchAllowed(
                                previousElement,
                                previousRegionIndex,
                                currentRegionIndex
                            )                                                      ) {
                            result = searchBackwardThroughTextStream(
                                searchText,
                                regexp,
                                stream,
                                streamOffsets,
                                caseSensitive,
                                wholeWordsOnly,
                                regularExpression,
                                newElementCursor,
                                newSelectionCursor
                            );

                            stream.clear();
                            streamOffsets.clear();

                            previousElement.clear();
                            previousRegionIndex = Element::invalidRegionIndex;
                        }

                        if (result == SearchResult::NOT_FOUND) {
                            unsigned long currentTextIndex = activeCursor.textIndex();

                            QString textToAdd = element->text(currentRegionIndex).left(currentTextIndex);
                            if (looped                                           &&
                                !endingCursor.isWholeElement()                   &&
                                currentRegionIndex == endingCursor.regionIndex() &&
                                element == endingCursor.element()                   ) {
                                textToAdd = textToAdd.mid(endingCursor.textIndex());
                            }

                            ElementCursor referenceCursor = ElementCursor(
                                activeCursor.textIndex() - textToAdd.length(),
                                activeCursor.regionIndex(),
                                element
                            );

                            stream.prepend(textToAdd);
                            streamOffsets.insert(stream.length(), referenceCursor);

                            needToLoop = activeCursor.moveBackwardByRegion();
                            needToLoop = activeCursor.fixPosition(false, needToLoop);

                            previousElement     = element;
                            previousRegionIndex = currentRegionIndex;
                        }
                    }
                }

                if (needToLoop) {
                    looped = true;
                    activeCursor.moveToEnd();
                    activeCursor.fixPosition(true, false);
                }
            } while (result == SearchResult::NOT_FOUND                         &&
                     ((!looped && activeCursor <= endingCursor)           ||
                      (looped && okToLoop && activeCursor > endingCursor)    )    );
        }

        if (result == SearchResult::FOUND) {
            ElementCursor oldElementCursor = currentElementCursor;

            currentElementCursor   = newElementCursor;
            currentSelectionCursor = newSelectionCursor;

            cursorChanged(oldElementCursor < currentElementCursor);
        }

        return result;
    }


    ElementPointerList Cursor::elementsUnderCursor(bool includeAncestors) const {
        ElementPointerList elements;

        if (currentSelectionCursor.isInvalid()) {
            if (includeAncestors) {
                elements = currentElementCursor.elementStack();
            } else {
                elements << currentElementCursor.element();
            }
        } else {
            ElementPointerSet elementsInRange = ElementCursor::elementsInRange(
                currentElementCursor,
                currentSelectionCursor
            );

            ElementPointerSet allElements;
            for (  ElementPointerSet::const_iterator it  = elementsInRange.constBegin(),
                                                     end = elementsInRange.constEnd()
                 ; it != end
                 ; ++it
                ) {
                ElementPointer element = *it;
                allElements.insert(element);
                if (includeAncestors) {
                    ElementPointerList ancestors = element->ancestors();
                    allElements += ElementPointerSet(ancestors.begin(), ancestors.end());
                }
            }

            elements = allElements.values();
        }

        return elements;
    }


    Cursor& Cursor::operator=(const Cursor& other) {
        currentElementCursor   = other.currentElementCursor;
        currentSelectionCursor = other.currentSelectionCursor;
        currentRootElement     = other.currentRootElement;
        lastWasSelection       = other.lastWasSelection;


        return *this;
    }


    void Cursor::cursorChanged(bool /* movingForward */) {}


    void Cursor::selectionCursorChanged() {}


    bool Cursor::okToAdjustCursor(bool& canNotMove) {
        bool doNormalAdjustment;

        QSharedPointer<RootElement> rootElement = currentRootElement.toStrongRef();
        if (!rootElement.isNull()) {
            if (rootElement->numberChildren() == 0) {
                currentElementCursor.setInvalid();

                doNormalAdjustment = false;
                canNotMove         = true;
            } else if (currentElementCursor.isInvalid()) {
                currentElementCursor.setElementUnderParent(rootElement, 0);

                doNormalAdjustment = false;
                canNotMove         = false;
            } else {
                doNormalAdjustment = true;
                canNotMove         = false;
            }
        } else {
            doNormalAdjustment = false;
            canNotMove         = true;
        }

        return doNormalAdjustment;
    }


    Cursor::SearchResult Cursor::searchForwardThroughTextStream(
            const QString&                            searchText,
            const QRegularExpression&                 regularExpression,
            const QString&                            textStream,
            const QMap<unsigned long, ElementCursor>& streamOffsets,
            bool                                      caseSensitive,
            bool                                      wholeWordsOnly,
            bool                                      useRegularExpressions,
            ElementCursor&                            elementCursor,
            ElementCursor&                            selectionCursor
        ) {
        Cursor::SearchResult result = SearchResult::NOT_FOUND;

        if (!textStream.isEmpty()) {
            if (useRegularExpressions) {
                QRegularExpressionMatch match = regularExpression.match(textStream);
                if (match.hasMatch()) {
                    unsigned long index  = static_cast<unsigned long>(match.capturedStart());
                    unsigned long offset = static_cast<unsigned long>(match.capturedLength());

                    result = SearchResult::FOUND;

                    elementCursor   = elementCursorFromStreamIndex(index, streamOffsets);
                    selectionCursor = elementCursorFromStreamIndex(index + offset, streamOffsets);
                }
            } else {
                unsigned long offset = static_cast<unsigned long>(searchText.length());
                long          index  = textStream.indexOf(
                    searchText,
                    caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive
                );

                if (index >= 0 && wholeWordsOnly) {
                    if (index > 0 && textStream.at(index - 1).isLetterOrNumber()) {
                        index = -1;
                    } else {
                        if (index < textStream.length() - 1 && textStream.at(index + offset).isLetterOrNumber()) {
                            index = -1;
                        }
                    }
                }

                if (index >= 0) {
                    result = SearchResult::FOUND;

                    elementCursor   = elementCursorFromStreamIndex(index, streamOffsets);
                    selectionCursor = elementCursorFromStreamIndex(index + offset, streamOffsets);
                }
            }
        }

        return result;
    }


    Cursor::SearchResult Cursor::searchBackwardThroughTextStream(
            const QString&                            searchText,
            const QRegularExpression&                 regularExpression,
            const QString&                            textStream,
            const QMap<unsigned long, ElementCursor>& streamOffsets,
            bool                                      caseSensitive,
            bool                                      wholeWordsOnly,
            bool                                      useRegularExpressions,
            ElementCursor&                            elementCursor,
            ElementCursor&                            selectionCursor
        ) {
        Cursor::SearchResult result = SearchResult::NOT_FOUND;

        if (!textStream.isEmpty()) {
            if (useRegularExpressions) {
                QRegularExpressionMatchIterator iterator = regularExpression.globalMatch(textStream);
                if (iterator.hasNext()) {
                    QRegularExpressionMatch match;
                    while (iterator.hasNext()) {
                        match = iterator.next();
                    }

                    unsigned long index  = static_cast<unsigned long>(match.capturedStart());
                    unsigned long offset = static_cast<unsigned long>(match.capturedLength());

                    result = SearchResult::FOUND;

                    QMap<unsigned long, ElementCursor> adjustedStreamOffsets = adjustBackwardStreamOffsets(streamOffsets);
                    elementCursor   = elementCursorFromStreamIndex(index, adjustedStreamOffsets);
                    selectionCursor = elementCursorFromStreamIndex(index + offset, adjustedStreamOffsets);
                }
            } else {
                unsigned long offset = static_cast<unsigned long>(searchText.length());
                long          index  = textStream.lastIndexOf(
                    searchText,
                    -1,
                    caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive
                );

                if (index >= 0 && wholeWordsOnly) {
                    if (index > 0 && textStream.at(index - 1).isLetterOrNumber()) {
                        index = -1;
                    } else {
                        if (index < textStream.length() - 1 && textStream.at(index + offset).isLetterOrNumber()) {
                            index = -1;
                        }
                    }
                }

                if (index >= 0) {
                    result = SearchResult::FOUND;

                    QMap<unsigned long, ElementCursor> adjustedStreamOffsets = adjustBackwardStreamOffsets(streamOffsets);
                    elementCursor   = elementCursorFromStreamIndex(index, adjustedStreamOffsets);
                    selectionCursor = elementCursorFromStreamIndex(index + offset, adjustedStreamOffsets);
                }
            }
        }

        return result;
    }


    QMap<unsigned long, ElementCursor> Cursor::adjustBackwardStreamOffsets(
            const QMap<unsigned long, ElementCursor>& backwardsOffsets
        ) {
        QMap<unsigned long, ElementCursor> result;

        unsigned long streamLength = backwardsOffsets.lastKey();
        for (  QMap<unsigned long, ElementCursor>::const_iterator pos = backwardsOffsets.constBegin(),
                                                                  end = backwardsOffsets.constEnd()
             ; pos != end
             ; ++pos
            ) {
            result.insert(streamLength - pos.key(), pos.value());
        }

        return result;
    }


    ElementCursor Cursor::elementCursorFromStreamIndex(
            unsigned long                             index,
            const QMap<unsigned long, ElementCursor>& streamOffsets
        ) {
        QMap<unsigned long, ElementCursor>::const_iterator iterator = streamOffsets.lowerBound(index);
        if (iterator == streamOffsets.constEnd()) {
            --iterator;
        }

        if (iterator.key() > static_cast<unsigned long>(index)) {
            --iterator;
        }

        Q_ASSERT(iterator != streamOffsets.end());
        Q_ASSERT(iterator.key() <= static_cast<unsigned long>(index));

        const ElementCursor& referenceCursor = iterator.value();

        ElementCursor result = ElementCursor(
            index - iterator.key() + referenceCursor.textIndex(),
            referenceCursor.regionIndex(),
            referenceCursor.element()
        );

        return result;
    }


    Util::HashResult qHash(CursorWeakPointer cursor, Util::HashSeed seed) {
        return ::qHash(cursor.toStrongRef().data(), seed);
    }


    Util::HashResult qHash(CursorPointer cursor, Util::HashSeed seed) {
        return ::qHash(cursor.data(), seed);
    }
}
