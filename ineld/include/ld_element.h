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
* This header defines the \ref Ld::Element class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_ELEMENT_H
#define LD_ELEMENT_H

#include <QString>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QMap>
#include <QList>
#include <QSet>

#include <cstdint>

#include <model_variant.h>

#include "ld_common.h"
#include "ld_handle.h"
#include "ld_element_structures.h"
#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_data_type.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_xml_element.h"
#include "ld_capabilities.h"
#include "ld_diagnostic_structures.h"
#include "ld_element_cursor.h"

namespace Ld {
    class Visual;
    class ElementWithNoChildren;
    class ElementWithFixedChildren;
    class ElementWithPositionalChildren;
    class ElementWithFloatingChildren;
    class Format;
    class CharacterFormat;
    class FormatOrganizer;
    class Cursor;
    class CursorStateCollection;
    class DataType;
    class CalculatedValue;
    class SelectionData;

    /**
     * Pure virtual base class for the internal representation of all language elements.  The internal representation of
     * all language constructs will be based on this class.
     *
     * Elements understand the concept of a parent/child heirarchy.  Unlike Qt's QObject class.  \ref Element instances
     * are not created with a parent and are assigned a parent when they are added as children.  This difference in
     * behavior is due to the fact that some \ref Element instances must maintain a fixed positional relationship with
     * their parent \ref Element while other \ref Element instances can flow and move within their parent, maintaining
     * a looser ordered relationship with other child elements.
     *
     * The \ref Element class also includes support for a factory, allowing you to dynamically create elements from a
     * type name defined at run-time.  The factory allows \ref Element instances to be defined at run-time by plug-ins.
     *
     * You can optionally tie a \ref Ld::Visual instance to this class in order to manage notifications back to a GUI or
     * other type of user interface.  While the \ref Ld::Element class makes heavy use of smart pointers, the class
     * intentionally avoids using smart pointers for the \ref Ld::Visual instance.  This allows the user interface to
     * use it's own pointer management mechanisms.  You should take care to inform the element when a visual is about to
     * be destroyed.
     */
    class LD_PUBLIC_API Element:public XmlElement {
        friend class Visual;
        friend class Format;
        friend class ElementWithNoChildren;
        friend class ElementWithFixedChildren;
        friend class ElementWithPositionalChildren;
        friend class ElementWithFloatingChildren;
        friend class SelectionData;

        // Helper function used by ElementWithFixedChildren, ElementWithPositionalChildren, and
        // ElementWithFloatingChildren to manage the object tree in a very controlled manner.  Function is very
        // intentionally not exposed as part of the API.
        friend void privateSetChildsParent(ElementPointer child, ElementPointer parent);

        public:
            /**
             * Enumeration used to indicate the supported mechanisms used to handle child elements.
             */
            enum class ChildPlacement : std::uint8_t {
                /**
                 * Indicates this element does not support any child nodes.
                 */
                NONE = 0,

                /**
                 * Indicates this element supports a fixed number of children, also implies a fixed function for each
                 * child class.
                 */
                FIXED = 1,

                /**
                 * Indicates this element supports a variable number of children with the position of each child
                 * tracked by an index.
                 */
                POSITIONAL = 2,

                /**
                 * Indicates this element supports a variant of the positional element where each child is tracked by
                 * index.  Unlike the POSITIONAL element, children can be organized into groups where each group tracks
                 * the relative position of each child.
                 */
                GROUPED = 3,

                /**
                 * Indicates this element supports a variant of the fixed element where children are arranged in a
                 * two dimensional grid.
                 */
                GRID = 4,

                /**
                 * Indicates this element supports a variable number of children where the position of each child floats
                 * with respect to each other.  Child elements are ordered based on their relative position.
                 */
                FLOATING = 5
            };

            /**
             * Enumeration of supported cursor park modes.
             */
            enum class CursorParkMode: std::uint8_t {
                /**
                 * Indicates the cursor can never park on this element.  The cursor will try to park on children of
                 * this element.
                 */
                CAN_NOT_PARK,

                /**
                 * Indicates the cursor can park on text in the element but not the whole element.  This mark mode
                 * implies no child elements.
                 */
                PARK_ON_TEXT_ONLY,

                /**
                 * Indicates the cursor can park on the entire element but not text within the element.
                 */
                PARK_ON_ELEMENT_ONLY,

                /**
                 * Indicates the cursor can park on both text and the entire element.
                 */
                PARK_ON_TEXT_AND_ELEMENT
            };

            /**
             * Enumeration of different parenthesis styles.  This enumeration is intended to use by elements
             * representing mathematical operators.
             */
            enum class ParenthesisStyle: std::uint8_t {
                /**
                 * Indicates an invalid or no parenthesis style.  Meaning depends on context.
                 */
                INVALID_OR_NONE = 0,

                /**
                 * Indicates the default parenthesis style for the operator should be used.
                 */
                DEFAULT = 1,

                /**
                 * Indicates traditional parenthesis.
                 */
                PARENTHESIS = 2,

                /**
                 * Indicates bracket parenthesis.
                 */
                BRACKETS = 3,

                /**
                 * Indicates brace parenthesis.
                 */
                BRACES = 4
            };

            /**
             * Enumeration indicating requirements for file export from this element.
             */
            enum class ExportImageCapability : std::uint8_t {
                /**
                 * Indicates that this element does not or should not support image export.
                 */
                NONE,

                /**
                 * Indicates that this element can only export an image using the visual class.  This value also
                 * implies that code generators are expected to be able to render this element through their own
                 * mechanisms.
                 */
                THROUGH_VISUAL,

                /**
                 * Indicates that this element can only export an image using the visual class and requiring ephermal
                 * data.  This value exists to inform selections that they must capture an image of this element from
                 * the original in order to render the selection later through a code generator.
                 */
                THROUGH_VISUAL_EPHEMERAL,

                /**
                 * Indicates that this element can export an image through an embedded payload or rendering engine.
                 */
                INTRINSIC
            };

            /**
             * Type used to define function pointers to default class creation functions.  In most cases, a creator
             * function can simply instantiate a new, default, instance of an object on the heap.
             *
             * The \ref XmlElement::readXml method will be called on default instances if those instances need to be
             * populated from an XML description.
             *
             * \param[in] typeName The name used to reference this creator function.  A single creator function can be
             *                     used to create multiple object types using this value to identify what type of
             *                     element to be created.
             *
             * \return Returns a pointer to the newly created element instance.
             */
            typedef Element* (*CreatorFunction)(const QString& typeName);

            /**
             * Type used to represent a reported error.
             */
            typedef unsigned ErrorCode;

            /**
             * Type used to represent a precedence value.
             */
            typedef unsigned short Precedence;

            /**
             * Value used to indicate an invalid child index.
             */
            static const unsigned long invalidChildIndex;

            /**
             * Value used to represent no error.
             */
            static const ErrorCode noError;

            /**
             * Value used to indicate an invalid text region.
             */
            static const unsigned invalidRegionIndex;

            /**
             * Value used to indicate an invalid text location.
             */
            static const unsigned long invalidTextIndex;

            /**
             * Value used as the default precedence.  This value will represent the lowest possible precedence.
             */
            static const Precedence defaultPrecedence;

            /**
             * Value used as the precedence for special symbols.
             */
            static const Precedence specialSymbolPrecedence;

            /**
             * Value used as the as the precedence for large operators (e.g. roots, summations, etc.)
             */
            static const Precedence largeOperatorPrecedence;

            /**
             * Value used as the precedence for power operators.
             */
            static const Precedence powerOperatorPrecedence;

            /**
             * Value used as the precedence for the function operator.
             */
            static const Precedence functionOperatorPrecedence;

            /**
             * Value used as the precedence for unary operators.
             */
            static const Precedence unaryOperatorPrecedence;

            /**
             * Value used as the precedence for unary plus/minus operators.
             */
            static const Precedence unaryPlusMinusOperatorPrecedence;

            /**
             * Value used as the precedence for most literal values.
             */
            static const Precedence simpleLiteralPrecedence;

            /**
             * Value used as the precedence for division operators.
             */
            static const Precedence divisionOperatorPrecedence;

            /**
             * Value used as the precedence for multiplication operators.
             */
            static const Precedence multiplicationOperatorPrecedence;

            /**
             * Value used as the precedence for addition operators.
             */
            static const Precedence additionOperatorPrecedence;

            /**
             * Value used as the precedence for the subtrahend parameter.
             */
            static const Precedence subtrahendOperatorPrecedence;

            /**
             * Value used as the precedence for bitwise AND operators.
             */
            static const Precedence bitwiseAndOperatorPrecedence;

            /**
             * Value used as the precedence for bitwise OR operators.
             */
            static const Precedence bitwiseOrOperatorPrecedence;

            /**
             * Value used as the precedence for set difference and symmetric difference operations.
             */
            static const Precedence setDifferencePrecedence;

            /**
             * Value used as the precedence for cartesian products.
             */
            static const Precedence cartesianProductPrecedence;

            /**
             * Value used as the precedence for set intersection operators.
             */
            static const Precedence intersectionOperatorPrecedence;

            /**
             * Value used as the precedence for set union operators.
             */
            static const Precedence unionOperatorPrecedence;

            /**
             * Value used as the precedence for relational operators.
             */
            static const Precedence relationalOperatorPrecedence;

            /**
             * Value used as the precedence for logical AND operators.
             */
            static const Precedence logicalAndOperatorPrecedence;

            /**
             * Value used as the precedence for logical OR operators.
             */
            static const Precedence logicalOrOperatorPrecedence;

            /**
             * Value used as the precedence for other logical operators.
             */
            static const Precedence logicalSpecialOperatorPrecedence;

            /**
             * Value used as the predence for complex literal values (remember that complex values are represented
             * as the sum of two values and thus need parenthesis to be represented correctly.
             */
            static const Precedence complexLiteralPrecedence;

            /**
             * Value as the precedence for the exponent of a power operator.
             */
            static const Precedence powerOperatorExponentPrecedence;

            /**
             * Value used as the precedence for range operators.
             */
            static const Precedence rangeOperatorPrecedence;

            /**
             * Value used as the precedence for assignments.
             */
            static const Precedence assignmentOperatorPrecedence;

            /**
             * Value used as the precedence for grouping operators.
             */
            static const Precedence groupingOperatorPrecedence;

            /**
             * Value used as the precedence for the matrix operator.
             */
            static const Precedence matrixOperatorPrecedence;

            /**
             * Value used as the precedence for subscript operators.
             */
            static const Precedence subscriptOperatorPrecedence;

            /**
             * Static method you can use to create an element given a name.  In addition to creating a new instance
             * of the specified element, by type name, the static method will:
             *
             *     * Set the weak pointer of the element.
             *     * Assign a visual to the element, if a visual with the same type-name has been registered.
             *
             * \param[in] typeName The name used to reference this element type.
             *
             * \return Returns a pointer to a class derived from \ref Element.
             */
            static ElementPointer create(const QString& typeName);

            /**
             * Static template method you can use to create a properly cast element.
             *
             * \param[in] T The type of the element to be created.
             */
            template<class T> LD_PUBLIC_TEMPLATE_METHOD static QSharedPointer<T> createElement() {
                ElementPointer newElement = create(T::elementName);
                return newElement.dynamicCast<T>();
            }

            /**
             * Static method you can use to register methods used to create default objects by type name.
             *
             * \param[in] typeName        The name used to reference this element type.
             *
             * \param[in] creatorFunction Pointer to the function that should be called to create a default instance of
             *                            the correct object type.
             *
             * \return Returns true on success, returns false if an object creator function has already been registered
             *         for the specified object.
             */
            static bool registerCreator(const QString& typeName, CreatorFunction creatorFunction);

            /**
             * Method you can call to indicate whether elements should automatically delete any visual tied to them
             * when they are destroyed.
             *
             * \param[in] nowAutoDeleteVisuals If true, visuals tied to an element will automatically be deleted when
             *                                 the element is deleted.  If false, visuals will not be deleted.  Visuals
             *                                 are automatically deleted by default.
             */
            static void setAutoDeleteVisuals(bool nowAutoDeleteVisuals);

            /**
             * Method you can call to determine if visuals are automatically deleted when an element is destroyed.
             *
             * \return Returns true if visuals will be automatically deleted by the constructor.  Returns false if
             *         visuals will not be deleted automatically.
             */
            static bool autoDeleteVisuals();

            Element();

            /**
             * Destructor.
             *
             * On element destruction,  the element will automatically delete any visual that has been tied to it.  The
             * destructor will also call the parent's instance of Element::removeChild(ElementPointer) to remove itself
             * from the parent object.
             *
             * Derived class destructors are expected to iterate across their children, destroying them as needed.
             */
            ~Element() override;

            /**
             * Method that returns the name of the plug-in that provides this element.
             *
             * \return Returns the name of the plug-in that provides this element. The name must match the name provided
             *         in the plug-in structures.  Elements defined as part of the ineld library should return an empty
             *         string.
             */
            virtual QString plugInName() const = 0;

            /**
             * Method that returns a description of this element.
             *
             * \return Returns a description of this element suitable for use in a GUI.
             */
            virtual QString description() const = 0;

            /**
             * Method you can use to create a clone of this element.  Note that this method will also set the weak
             * this pointer on each element.
             *
             * \param[in] includeChildren If true, clones of every child will be included in generated clone.  If
             *                            false, children will be excluded from the clone.
             *
             * \return Returns a new instance of this element with clones of each child included.  Note that a null
             *         pointer may be returned in the unlikely event of an error.
             */
            virtual ElementPointer clone(bool includeChildren) const = 0;

            /**
             * Method you can use to create a clone of this element based on a starting and ending cursor position.
             * This version is intended to be used to clone elements containing text in one or more regions.  Note that
             * this method will set the weak this pointer on each element.
             *
             * \param[in] startingTextIndex   The starting text index of the first specified region to include in the
             *                                clone.
             *
             * \param[in] startingRegionIndex The startng text region to include in the clone.
             *
             * \param[in] endingTextIndex     The ending text index of the last specified region to include in the
             *                                clone.  A value of \ref Ld::Element::invalidTextIndex indicates all the
             *                                text in the ending region should be included.
             *
             * \param[in] endingRegionIndex   The ending text region to include in the clone.  A value of
             *                                \ref Ld::Element::invalidRegionIndex indicates the close should include
             *                                the last region.
             *
             * \param[in] includeChildren     If true, clones of every child will be included in generated clone.  If
             *                                false, children will be excluded from the clone.
             *
             * \return Returns a new instance of the element.  The clone may optionally include children if this
             *         element supports children.  A null pointer will be returned if the specified ranges are invalid.
             */
            virtual ElementPointer clone(
                unsigned long        startingTextIndex,
                unsigned             startingRegionIndex,
                unsigned long        endingTextIndex,
                unsigned             endingRegionIndex,
                bool                 includeChildren
            ) const = 0;

            /**
             * Method you should call to inform this class of the weak pointer used to point to itself.  The value is
             * used by child elements to point back to this element.  Be sure to call this before adding this element to
             * an element tree or assigning a visual or format to this element.
             *
             * Note that the \ref create method will call this method automatically so you do not need to use this
             * method if you use the \ref Element::create(const QString&) method.
             *
             * \param[in] newWeakThis The weak pointer representation of this object.
             */
            void setWeakThis(const ElementWeakPointer& newWeakThis);

            /**
             * Method you can use to obtain a weak pointer to this element.
             *
             * \return Returns a weak pointer to this element.
             */
            ElementWeakPointer weakThis() const;

            /**
             * Method you can use to determine the precedence for this element when comparing against the parent.
             *
             * \return Returns the element's precedence when comparing against the element's parent.  The default
             *         implementation returns the value reported by Ld::Element::translationPrecedence.
             */
            virtual Precedence childPrecedence() const;

            /**
             * Method you can use to determine the precedence for this element when comparing against a child.  The
             * default implementation returns the value reported by \ref Ld::Element::childPrecedence.
             *
             * \param[in] childIndex The index of the child we're comparing against.  This method will return a
             *                       reasonable default value if an invalid child index is supplied.
             */
            virtual Precedence parentPrecedence(unsigned long childIndex) const;

            /**
             * Convenience method you can use to determine the precedence for this element when comparing against a
             * child.  The default implementation returns the value reported by \ref Ld::Element::childPrecedence.
             *
             * \param[in] child The child we're comparing against.
             */
            Precedence parentPrecedence(ElementPointer child) const;

            /**
             * Method you can use to determine the value type this element represents.  Elements typically either
             * either report a fixed value or calculate the value based on their children.
             *
             * \return Returns the value type this element would represent.  A value of ValueType::NONE indicates
             *         either that the element does not return a value or the value type could not be determined at
             *         this time.
             */
            virtual DataType::ValueType valueType() const = 0;

            /**
             * Method you can use to tie a format to this element used when rendering the element in the visual.
             *
             * \note The use of shared pointers for the Format class is, perhaps, questionable if a 1-to-1 relationship
             *       between the format class and element is maintained.  Qt does not have an equivalent to the
             *       std::unique_ptr class that supports clean move sematics so the use of a QSharedPointer largely
             *       supports Moving forward, we *may* want to be more intelligent about sharing format classes.
             *
             * \param[in] newFormat A shared pointer to the format to tie to this element.
             */
            void setFormat(FormatPointer newFormat);

            /**
             * Convenience method you can use to tie a format to this element used when rendering the element in the
             * visual.
             *
             * \param[in] newFormat The format to tie to this element.  Note that the element will assign the format to
             *                      a shared pointer, thus taking shared ownership of it.
             */
            void setFormat(Format* newFormat);

            /**
             * Method you can use to obtain the current format tied to this element.
             *
             * \return Returns a shared pointer to the desired format.
             */
            FormatPointer format() const;

            /**
             * Method you can use to obtain the parenthesis style for use by this element.  The default implementation
             * uses the following rules to determine the parenthesis style:
             *
             *     * If the format derives from \ref Ld::OperatorFormat and the format indicates forced parenthesis,
             *       then parenthesis will be indicated.
             *     * Otherwise:
             *         - If this element's precedence value is less than the parent's precendence value, then
             *           parenthesis are required.
             *
             * If parenthesis are required:
             *
             *     * If the format derives from \ref Ld::ParenthesisFormat, the parenthesis style will be indicated
             *       by the format.
             *     * Otherwise: The parenthesis style will be set to \ref Ld::Element::ParenthesisStyle::DEFAULT
             *
             * \return Returns the required parenthesis style.  The value
             *         \ref Ld::Element::ParenthesisStyle::INVALID_OR_NONE indicates no parenthesis are required.
             */
            virtual ParenthesisStyle parenthesisStyle() const;

            /**
             * Method you can use to obtain the parent of this element.
             *
             * \return Returns a pointer to the element's parent.
             */
            ElementPointer parent() const;

            /**
             * Method you can use to determine if this element is the only child under its parent.
             *
             * \return Returns true if this child is an only child.  Returns false if this element has siblings.
             */
            bool isOnlyChild() const;

            /**
             * Method you can use to determine if this element is the first child under the parent.
             *
             * \return Returns true if this child is the first child under the parent. Returns false if this element is
             *         not the first child.
             */
            bool isFirstChild() const;

            /**
             * Method you can use to determine if this element is the last child under the parent.
             *
             * \return Returns true if this child is the last child under the parent.  Returns false if this element is
             *         not the last child.
             */
            bool isLastChild() const;

            /**
             * Method you can use to obtain the previous sibling to this element.
             *
             * \param[out] firstSibling Pointer to a boolean that will hold true if this element was the first
             *                          sibling.
             *
             * \return Returns a shared pointer to the previous sibling under the parent.  A null pointer is returned
             *         if the previous element is null or if the previous element is the first element under the
             *         parent.
             */
            ElementPointer previousSibling(bool* firstSibling = nullptr) const;

            /**
             * Method you can use to obtain the next sibling to this element.
             *
             * \param[out] lastSibling Pointer to a boolean that will hold true if this element was the last
             *                         sibling.
             *
             * \return Returns a shared pointer to the previous sibling under the parent.  A null pointer is returned
             *         if the next element is null or if the previous element is the last element under the parent.
             */
            ElementPointer nextSibling(bool* lastSibling = nullptr) const;

            /**
             * Method you can use to determine the depth of this element in the tree.
             *
             * \return Returns the depth of this element.
             */
            unsigned depth() const;

            /**
             * Method you can use to obtain this element's unique handle.  The handle is guaranteed to be unique on any
             * given system.  Handles are based on the current system time when the program is started as well as the
             * processor ID.
             *
             * \return Returns the element's unique handle.
             */
            Ld::Handle handle() const;

            /**
             * Method you can use to change the object used to manage visual representation of this element.  Note that
             * using this method will also cause the \ref Visual::element method to point back to this \ref Element
             * instance.
             *
             * If the newly added \ref Visual already points to a different element, then those connections will be
             * broken in order to maintain a 1:1 relationship between any \ref Visual instance and \ref Element
             * instance.
             *
             * If this element currently has an associated visual, then that visual be will disconnected from this
             * element.  The disconnected visual will *not* be deleted.
             *
             * \param[in] newVisual A pointer to the new visual object.  Note that this element will take ownership of
             *                      the visual.
             */
            void setVisual(Visual* newVisual);

            /**
             * Method you can use to obtain the visual representation of this element.
             *
             * \return Returns a pointer to the visual representation of this element.  A null pointer is returned if
             *         no visual representation has been defined.
             */
            Visual* visual() const;

            /**
             * Method that points to the root or top-most element.
             *
             * \return Returns a pointer to the root element that ultimately owns this \ref Element instance.
             */
            ElementPointer root() const;

            /**
             * Method you can call to flag a diagnostic condition associated with this element.  Note that this method
             * may be called from multiple threads.
             *
             * \param[in] diagnostic A code indicating the type of error.
             */
            void flagDiagnostic(DiagnosticPointer diagnostic);

            /**
             * Method you can call to clear any diagnostic associated with this element.  Note that this method may be
             * called from multiple threads.
             */
            void clearDiagnostic();

            /**
             * Method you can call to determine if there is any diagnostic associated with this element.
             *
             * \return Returns true if there is a diagnostic associated with this element.  Returns false if there is no
             *         reported diagnostic condition.
             */
            bool diagnosticReported() const;

            /**
             * Returns the diagnostic associated with this element.
             *
             * \return Returns the diagnostic associated with this element.
             */
            DiagnosticPointer diagnostic() const;

            /**
             * Method you can use to determine if you can set a breakpoint directly on this element.
             *
             * \return Returns true if you can set an instruction breakpoint on this element.  Returns false if this
             *         type of element can-not support instruction breakpoints.  The default implementation will
             *         return the status reported by the parent widget or false if there is no parent widget.
             */
            virtual bool supportsInstructionBreakpoints() const;

            /**
             * Method you can use to set or clear an instruction breakpoint.  The default implementation will simply
             * call this method in the parent and return the result.
             *
             * \param[in] nowSet If true, an instruction breakpoint will now be set.  If false, any pending
             *                   instruction breakpoint will be cleared.
             *
             * \return Returns true on success, returns false on error.
             */
            virtual bool setInstructionBreakpoint(bool nowSet = true);

            /**
             * Method you can use to clear or set an instruction breakpoint.  The default implementation will simply
             * call \ref Ld::Element::setInstructionBreakpoint.
             *
             * \param[in] nowClear If true, any pending instruction breakpoint will now be cleared.  If false, an
             *                     instruction breakpoint will be set.
             *
             * \return Returns true on success, returns false on error.
             */
            bool clearInstructionBreakpoint(bool nowClear = true);

            /**
             * Method you can use to determine if an instruction breakpoint has been set.
             *
             * \return Returns true if this element has a set instruction breakpoint.  The default implementation
             *         returns the status reported by the parent or "false" if there is no parent widget.
             */
            virtual bool instructionBreakpointSet() const;

            /**
             * Method that can be called to determine how children are managed and tracked by this element.
             *
             * \return Returns a value indicating how children are tracked by this element.
             */
            virtual Element::ChildPlacement childPlacement() const = 0;

            /**
             * Method that can be used to determine the current number of children.
             *
             * \return Returns a count of the number of children.
             */
            virtual unsigned long numberChildren() const = 0;

            /**
             * Method that is called to remove a child from this class.  On elements that support a fixed child
             * relationship, the position for the child will be set to a null pointer.  On elements that support a
             * variable number of children, the child will be removed from the list and other children will be
             * re-ordered accordingly.
             *
             * \param[in]     childToRemove         The child to be removed.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             *
             * \return Returns true on success.  Returns false if the child does not belong to this element.
             */
            virtual bool removeChild(ElementPointer childToRemove, CursorStateCollection* cursorStateCollection) = 0;

            /**
             * Method that can be called to remove every child element.  The children are destroyed by this method.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             */
            virtual void removeChildren(CursorStateCollection* cursorStateCollection) = 0;

            /**
             * Method that can be used to obtain a child using its index.  Note that the index for each child may change
             * dynamically as children are added and removed.
             *
             * \param[in] childIndex The zero based index of the desired child.
             *
             * \return Returns a pointer to the desired child.
             */
            virtual ElementPointer child(unsigned long childIndex) const = 0;

            /**
             * Method that can be used to obtain a list of child elements.
             *
             * \return Returns a list of child elements.  The list may contain null pointers.
             */
            virtual ElementPointerList children() const = 0;

            /**
             * Method you can use to obtain a set containing all descendants of this element.
             *
             * \return Returns a set of shared pointers to every descendant of this element.  The set will include a
             *         null entry any child entries are null.  The set will also exclude this element.
             */
            ElementPointerSet descendants() const;

            /**
             * Method you can use to obtain an ordered list of all the ancestors.
             *
             * \return Returns a list of all the ancestors of this element.  Element 0 will be the root element.  The
             *         last element in the list will be this element's parent.  The list will also exclude this element.
             */
            ElementPointerList ancestors() const;

            /**
             * Method you can call to determine the index to use to reference a child.  The value will match the value
             * to be used with the Element::child(unsigned long) method to access the child.
             *
             * \param[in] childElement A shared pointer to the child element that we want to locate.
             *
             * \return Returns the zero based index of the child.  The value \ref Ld::Element::invalidChildIndex is
             *         returned if the provided element is not a child of this element.
             */
            virtual unsigned long indexOfChild(ElementPointer childElement) const = 0;

            /**
             * Method you can call to determine the requirements imposed by this element on a specific child element.
             * Note that no testing is performed by elements to confirm that requirements are met.  The information is
             * expected to be used by the user interface.
             *
             * \param[in] childIndex The zero based child index.
             *
             * \return Returns the requirements imposed by this parent on the child element.
             */
            virtual Capabilities parentRequires(unsigned long childIndex) const = 0;

            /**
             * Method you can call to determine what capabilities this element will provide.  Note that objects should
             * also provide a static constant value named "childProvides" returning the same information.
             *
             * \return Returns the requirements met by this element.
             */
            virtual Capabilities childProvidesCapabilities() const = 0;

            /**
             * Method you can call to determine what capabilities this element can support intrinisically.  This method
             * is used when the cursor is in text mode to determine what additional types of children or special
             * objects can be inserted.
             *
             * Generally this call indicates what types of special characters can be inserted although it can also be
             * used to indicate additional elements that can be processed while the cursor is pointing to text in this
             * element.
             *
             * \param[in] textOffset  The zero based offset into the text region.
             *
             * \param[in] regionIndex The zero based region index.
             *
             * \return Returns the capabilities this element can support while the cursor is pointing to text.  The
             *         default implementation returns an empty \ref Capabilities instance if there are no text regions
             *         and Capabilities::alphanumericSymbols | Capabilities::nonAsciiAlphanumericSymbols if there are
             *         one or more text regions.
             */
            virtual Capabilities textCapabilities(unsigned long textOffset, unsigned regionIndex) const;

            /**
             * Method you can call to determine if this element's text fields are allowed to be combined with another
             * element's text fields when performing a search operation.
             *
             * Note that this method flags this element only against the previous or next element, depending on the
             * direction of search.  Content may be additionally also combined with other possible elements.
             *
             * \param[in] otherElement     The other element to check against.  Note that this pointer may point to
             *                             this element when the element contains multiple regions.
             *
             * \param[in] otherRegionIndex The region index in the other element that would be combined.
             *
             * \param[in] thisRegionIndex  The region index of this element to be combined.
             *
             * \return Returns true if the text can be combined for search purposes.  Returns false if the text in each
             *         element must be treated as distinct.  The default implementation always returns false.
             */
            virtual bool aggregateTextDuringSearchAllowed(
                ElementPointer otherElement,
                unsigned       otherRegionIndex,
                unsigned       thisRegionIndex
            ) const;

            /**
             * Method you can call to determine if this element represents or can contain a list of elements, typically
             * separated by commas.  This method is intended to be used to indicate if commas can be used to add new
             * entries.
             *
             * \return Returns true if this element represents an open-ended list of elements.  Returns false if this
             *         element does not represent an open-ended list of elements.  The default implementation
             *         returns false.
             */
            virtual bool isListElement() const;

            /**
             * Method you can call to determine if this element represents a matrix.  This method is intended to
             * be used to indicate how rendering of parenthesis of child elements should be performed.
             *
             * \return Returns true if this element represents an matrix.  Returns false if this element does not
             *         represent a matrix.  The default implementation returns false.
             */
            virtual bool isMatrix() const;

            /**
             * Method you can call to determine if this element represents a placeholder.
             *
             * \return Returns true if this element is a placeholder.  Returns false if this element is not a
             *         placeholder.  The default implementation returns false.
             */
            virtual bool isPlaceholder() const;

            /**
             * Method you can call to determine if this element represents a top level operator.
             *
             * \return Returns true if this element is a top level operator.  Returns false if this element is not a
             *         top level operator.  The default implementation returns false.
             */
            virtual bool isTopLevelOperator() const;

            /**
             * Method you can call to detemrine if this element provides a logical stop for function parenthesis.  If
             * true, functions will act as if they're top level.  If false, functions will act as if they're within a
             * larger equation and will always place parenthesis around parameters.
             *
             * The default implementation will return true only if the parent returns true and the index represents
             * the last index.
             *
             * \param[in] functionChildIndex The index of the function element being queried.
             *
             * \return Returns true if the function can assume it's at a logical stop in the equation such as the last
             *         position in an equation or at a position, such as directly under a matrix, that can be treated
             *         as the last position.  Returns false in all other cases.
             */
            virtual bool isFunctionLogicalStop(unsigned long functionChildIndex) const;

            /**
             * Method that can be used to determine the minimum number of distinct text regions used by this element.
             *
             * \return Returns the minimum number of text regions.  The default implementation always returns 0.
             */
            virtual unsigned minimumNumberTextRegions() const;

            /**
             * Method that can be used to determine the maximum number of distinct text regions used by this element.
             *
             * \return Returns the maximum number of text regions.  The default implementation always returns 0.
             */
            virtual unsigned maximumNumberTextRegions() const;

            /**
             * Method you can use to set the number of expected text regions.
             *
             * \param[in] newNumberTextRegions The new number of text regions.
             *
             * \return Returns true on success, returns false if the number of text regions is invalid.  The default
             *         implementationr returns false for all values except 0.
             */
            virtual bool setNumberTextRegions(unsigned newNumberTextRegions);

            /**
             * Method you can use to obtain the current number of text regions.
             *
             * \return Returns the current number of text regions.  The default implementation returns 0.
             */
            virtual unsigned numberTextRegions() const;

            /**
             * Method that sets the text associated with a text region of this element.
             *
             * \param[in] newText      The new text to associate with this element and region.
             *
             * \param[in] regionNumber The zero based region number.
             *
             * \return Returns true if the region is valid.  Returns false if the region is invalid.
             */
            virtual bool setText(const QString& newText, unsigned regionNumber = 0);

            /**
             * Method you can use to insert text into a single text region of this element.  The default implementation
             * will leverage the \ref text and \ref setText methods to perform the necessary tasks.
             *
             * \param[in]     textToInsert          The text to be inserted.
             *
             * \param[in]     textIndex             The text index where the text should be inserted.  An invalid
             *                                      text index will cause the text to be inserted at the end of the
             *                                      specified region.
             *
             * \param[in]     regionIndex           The region index where the text should be inserted.  An invalid
             *                                      region index will cause the text to be inserted into the last
             *                                      region.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             *
             * \param[in]     preferEarlierPosition If true, cursors at the insertion point will remain untouched.  if
             *                                      false, cursors at the insertion point will be advanced to just
             *                                      after the inserted text.
             *
             * \return Returns true if the text was inserted.  Returns false if this element does not support text
             *         insertion.
             */
            virtual bool insertText(
                const QString&         textToInsert,
                unsigned long          textIndex,
                unsigned               regionIndex,
                CursorStateCollection* cursorStateCollection = nullptr,
                bool                   preferEarlierPosition = true
            );

            /**
             * Method that removes text over a given span.  The default implementation will remove the text using other
             * existing methods such as \ref Ld::Element::text and \ref Ld::Element::setText.
             *
             * \param[in] startingTextIndex         The starting text index of the first specified region to remove.
             *
             * \param[in] startingRegionIndex       The startng text region to remove.
             *
             * \param[in] endingTextIndex           The ending text index of the last specified region to remove.  A
             *                                      value of \ref Ld::Element::invalidTextIndex indicates the removed
             *                                      text should include everything up-to the end of the last indicated
             *                                      region.
             *
             * \param[in] endingRegionIndex         The ending text region to removed.
             *                                      \ref Ld::Element::invalidRegionIndex indicates the removed text
             *                                      should include the last region.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             *
             * \return Returns true on success, returns false if the starting index values are invalid.
             */
            virtual bool removeText(
                unsigned long          startingTextIndex,
                unsigned               startingRegionIndex,
                unsigned long          endingTextIndex,
                unsigned               endingRegionIndex,
                CursorStateCollection* cursorStateCollection = nullptr
            );

            /**
             * Method you can use to obtain the text associated with a text region of this element.
             *
             * \param[in] regionNumber The zero based region number.
             *
             * \return Returns the text currently associated with this element's region.  Returns empty text if the
             *         region is invalid.
             */
            virtual QString text(unsigned regionNumber = 0) const;

            /**
             * Method that indicates if the \ref Ld::Cursor can place the cursor on this element.
             *
             * \return Returns the allowed cursor park mode.  The default implementation returns
             *         \ref CursorParkMode::PARK_ON_ELEMENT_ONLY.
             */
            virtual CursorParkMode cursorParkMode() const;

            /**
             * Method you can overload to indicate if this element requires calculated data to fully render content.
             *
             * \return Returns true if this element requires calculated data to fully render its content.  Returns
             *         false if this element does not require calculated data to fully render content.  The default
             *         implementation always returns false.
             */
            virtual bool requiresCalculatedDataToRender() const;

            /**
             * Method you can overload to determine the number of distinct calculated values supported by this
             * element.
             *
             * \return Returns the number of calculated values supported by this element.  The default implementation
             *         always returns 0.
             */
            virtual unsigned numberCalculatedValues() const;

            /**
             * Method you can overload to receive notification of calculated values associated with this element.  The
             * default method simply reports the data to any visual tied to this element.
             *
             * \param[in] valueIndex      A zero based index used to indicate which calculated value to update.
             *
             * \param[in] calculatedValue The calculated value.
             */
            virtual void setCalculatedValue(unsigned valueIndex, const CalculatedValue& calculatedValue);

            /**
             * Method you can overload to receive notification when the calculated value should be cleared.  The
             * default method simply reports the clearing event to the visual.
             */
            virtual void clearCalculatedValue();

            /**
             * Method you can overload to obtain the last reported calculated value.
             *
             * \param[in] valueIndex A zero based index used to reference the desired calculated value.
             *
             * \return Returns the last reported calculated value.  An invalid calculated value is returned if the
             *         index is invalid.  The default implementation always returns an invalid calculated value.
             */
            virtual CalculatedValue calculatedValue(unsigned valueIndex = 0) const;

            /**
             * Method you can use to determine if this element supports payloads.
             *
             * \return Returns true if this element supports payloads.  Returns false if this element does not support
             *         payloads.  The default implementation always returns false.
             */
            virtual bool supportsPayloads() const;

            /**
             * Method you can use to obtain the payload data tracked by this element.  Note that this method triggers
             * file I/O.
             *
             * \param[out] newData A QByteArray that will be populated with the payload data.
             *
             * \return Returns true on success, returns false on error.  The default implementation returns false.
             */
            virtual bool getPayload(QByteArray& newData);

            /**
             * Method you can use to determine if this element supports image export capability.
             *
             * \return Returns the export image capability for this element.  The default implementation returns
             *         \ref Ld::Element::ExportImageCapability::THROUGH_VISUAL.
             */
            virtual ExportImageCapability exportImageCapability() const;

            /**
             * Method you can use to obtain a visual representation of this element and children as an image in a byte
             * array.
             *
             * This method works by requesting the image from the visual.  This method may block the current thread
             * if the code that generates the image is running in a different thread.
             *
             * \param[in] dpi The desired image resolution in DPI.  The DPI setting is intended to be used as a hint
             *                and may not be honored.
             *
             * \return Returns a byte array holding the captured image.  An empty byte array is returned if the image
             *         could not be generated.
             */
            virtual QByteArray exportImage(float dpi);

        protected:
            /**
             * Method that is called when this element is grafted to a new tree to perform any additional
             * initialization that may be needed.  This method is called by \ref Ld::Element::graftedToTree just before
             * triggering \ref Ld::Visual::graftedToTree.
             *
             * The default implementation simply returns.
             */
            virtual void updateAfterGraft();

            /**
             * Method you can use to create a clone of this element.  Note that this method will also set the weak
             * this pointer on each element.
             *
             * This method can be used by \ref Ld::Element::clone methods in derived classes to clone just this
             * element.  The method will:
             *
             *     * Using the typeName of the element to create a version using using the element.
             *
             *     * Use the \ref Ld::Format::clone method to create a clone of the format.
             *
             *     * Using the \ref Ld::Element::setNumberTextRegions and \ref Ld::Element::setText to populate any
             *       text regions for the element.
             *
             * \return Returns a new instance of this element with clones of each child included.  Note that a null
             *         pointer may be returned in the unlikely event of an error.
             */
            virtual ElementPointer cloneThisElementOnly() const;

            /**
             * Method you can use to create a clone of this element based on a starting and ending cursor position.
             * This version is intended to be used to clone elements containing text in one or more regions.  Note that
             * this method will set the weak this pointer on each element.
             *
             * This method can be used by \ref Ld::Element::clone methods in derived classes to clone just this
             * element.  The method will:
             *
             *     * Using the typeName of the element to create a version using using the element.
             *
             *     * Use the \ref Ld::Format::clone method to create a clone of the format.
             *
             *     * Using the \ref Ld::Element::setNumberTextRegions and \ref Ld::Element::setText to populate any
             *       text regions for the element.  Text regions and contents will be limited by the provided index
             *       values.  All other text will either be excluded or set to an empty value.
             *
             * \param[in] startingTextIndex   The starting text index of the first specified region to include in the
             *                                clone.
             *
             * \param[in] startingRegionIndex The startng text region to include in the clone.
             *
             * \param[in] endingTextIndex     The ending text index of the last specified region to include in the
             *                                clone.  A value of \ref Ld::Element::invalidTextIndex indicates all the
             *                                text in the ending region should be included.
             *
             * \param[in] endingRegionIndex   The ending text region to include in the clone.  A value of
             *                                \ref Ld::Element::invalidRegionIndex indicates the close should include
             *                                the last region.
             *
             * \return Returns a new instance of the element.  The clone may optionally include children if this
             *         element supports children.  A null pointer will be returned if the specified ranges are invalid.
             */
            virtual ElementPointer cloneThisElementOnly(
                unsigned long startingTextIndex,
                unsigned      startingRegionIndex,
                unsigned long endingTextIndex,
                unsigned      endingRegionIndex
            ) const;

            /**
             * Method you can overload to intercept notification of a new descendant being added to the tree.  A
             * descendant element is any child directly or indirectly under this element.  The default implementation
             * passes notification upwards through the tree towards the root note from every discendant under this node.
             * If you overload this method, be sure to call the base class implementation so that notification messages
             * can propagate.
             *
             * \param[in] descendantElement The descendant element that was added to the tree.
             */
            virtual void descendantAdded(ElementPointer descendantElement);

            /**
             * Method you can overload to intercept notification of a descendant element being removed from the tree.  A
             * descendant element is any child element directly or indirectly under this element.  The default
             * implementation passes notification upwards through the tree towards the root note from every descendant
             * under this node.  If you overload this method, be sure to call the base class implementation so that
             * notification messages can propagate.
             *
             * This method will be triggered any time a decendant is about to be removed, including when it is
             * destroyed.
             *
             * \param[in] descendantElement The descendant element that is about to be removed from the tree.
             */
            virtual void descendantRemoved(ElementPointer descendantElement);

            /**
             * Method that is triggered when this element or an ancestor has been inserted into a tree of elements.
             * You can overload this method if you want notification that this element is now under a different tree.
             * Implementations in immediately derived classes manage event propagation.  You must be certain to call
             * the base class implementation should you overload this method.
             */
            virtual void graftedToTree();

            /**
             * Method that is triggered when this element or an ancestor is about to be removed from a tree of elements.
             * You can overload this method if you want notification that the element has been disconnected from the
             * previous tree structure.  If you do overload this method, be sure to call the base class implementation
             * as it manages event propagation to the visual.
             *
             * Note that this virtual method will *not* be triggered when this element or an ascestor is being
             * destroyed.
             */
            virtual void aboutToUngraftFromTree();

            /**
             * Method you can call to indicate if an element has received a change.  You can call this method from any\
             * element to inform parent elements, including the root element, that a child has changed.
             *
             * If you overload this method, please be sure to call the base class implementation in order to allow for
             * event propagation.
             *
             * \param[in] changedChild Pointer to the child element that changed.
             */
            virtual void childChanged(ElementPointer changedChild);

            /**
             * Method that is triggered by a call to the \ref Ld::Format::reportFormatUpdated method.  The default
             * implementation marshalls a call to the associated \ref Ld::Visual::formatChanged method.  Be sure
             * to call the base class implementation if you decide to overload this method.
             */
            virtual void formatUpdated();

            /**
             * Method that is triggered whenever a descendant's format has been changed.  The default implementation of
             * this method both triggers a call to the visual's \ref Ld::Visual::descendantFormatChanged method and
             * propagates the message upwards in the element tree.
             *
             * If you overload this method, please be sure to call the base class implementation in order to allow for
             * event propagation to occur.
             *
             * \param[in] changedElement The element who's format has been changed.
             *
             * \param[in] newFormat      The new format associated with this element.
             */
            virtual void descendantFormatChanged(ElementPointer changedElement, FormatPointer newFormat);

            /**
             * Method that is triggered whenever a descendant's format has been updated.  The default implementation of
             * this method both triggers a call to the visual's \ref Ld::Visual::descendantFormatUpdated method and
             * propagates the message upwards in the element tree.
             *
             * If you overload this method, please be sure to call the base class implementation in order to allow for
             * event propagation to occur.
             *
             * \param[in] changedElement The element who's format has been changed.
             *
             * \param[in] newFormat      The updated format associated with this element.
             */
            virtual void descendantFormatUpdated(ElementPointer changedElement, FormatPointer newFormat);

            /**
             * Method you can call to inform the associated \ref Ld::Visual that the element's internal data
             * has changed.  You should call this method when data tracked directly by the element is updated. This
             * method should not be called when the format is updated or when child elements are added and removed.
             *
             * This method will call \ref Ld::Element::childChanged to report the change up the element tree to the
             * root.
             */
            void elementDataChanged();

            /**
             * Method you can call to inform the assocaited \ref Ld::Visual that the instruction breakpoint status for
             * this element has changed.
             *
             * \param[in] breakpointNowSet If true, there is now an instruction breakpoint tied to this element.  If
             *                             false, there is no longer an instruction breakpoint tied to this element.
             */
            void instructionBreakpointStatusChanged(bool breakpointNowSet);

            /**
             * Method you can optionally overload to add additional attributes to the XML description of this element.
             *
             * \param[in]     attributes  A reference to the list of attributes to be modified.
             *
             * \param[in]     formats     A \ref FormatOrganizer instance used to track all known formats in the
             *                            program.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             */
            void writeAddAttributes(
                XmlAttributes&                  attributes,
                QSharedPointer<FormatOrganizer> formats,
                ProgramFile&                    programFile
            ) const override;

            /**
             * Method you can overload to parse incoming XML attributes.  This method will only be called if there are
             * provided attributes.
             *
             * Errors are reported directly to the reader using the raiseError method.
             *
             * \param[in]     reader      The reader used to parse the incoming XML stream.
             *
             * \param[in]     attributes  The XML attributes to be parsed.
             *
             * \param[in]     formats     A map of format instances by format identifier.  You can use this parameter
             *                            to either tie a format to an object or add a new format to the list of known
             *                            formats.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             *
             * \param[in]     xmlVersion  The XML major version code.  The value can be used to modify how the incoming
             *                            XML stream is parsed.
             */
            void readAttributes(
                QSharedPointer<XmlReader>  reader,
                const XmlAttributes&       attributes,
                const FormatsByIdentifier& formats,
                ProgramFile&               programFile,
                unsigned                   xmlVersion
            ) override;

            /**
             * Method that sets every cursor that points to a descendant of this element to point to this element.
             *
             * \param[in,out] cursorStateCollection A pointer to a collection of cursor instances to be adjusted by
             *                                      this method.  No adjustments are performed if a null pointer is
             *                                      provided.
             */
            void updateCursorsFromDescendantsToThis(CursorStateCollection* cursorStateCollection) const;

        private:
            /**
             * Method used internally to compare the precedence of this element relative to the parent.
             *
             * \return Returns true if parenthesis should be used.  Returns false if precedence indicates that
             *         parenthesis should not be used.
             */
            bool precedenceSuggestsParenthesis() const;

            /**
             * Dictionary used to identify empty element creators based on an element type name.
             */
            static QMap<QString, CreatorFunction> creators;

            /**
             * Value indicating if element's should automatically delete the visual they are tied to.
             */
            static bool currentAutoDeleteVisual;

            /**
             * A weak pointer to this object.
             */
            ElementWeakPointer currentWeakThis;

            /**
             * Pointer to the element's parent.
             */
            ElementWeakPointer currentParent;

            /**
             * Pointer to the visual representation for this element.
             */
            Visual* currentVisual;

            /**
             * The font used when rendering the visual for this element.
             */
            FormatPointer currentFormat;

            /**
             * The handle used to reference this child.
             */
            Ld::Handle currentHandle;

            /**
             * The currently reported diagnostic code.
             */
            DiagnosticPointer currentDiagnostic;
    };
};

#endif
