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
* This header defines the \ref Ld::BraceConditionalOperatorElement class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_BRACE_CONDITIONAL_OPERATOR_ELEMENT_H
#define LD_BRACE_CONDITIONAL_OPERATOR_ELEMENT_H

#include <QString>
#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_data_type.h"
#include "ld_capabilities.h"
#include "ld_cursor_state_collection.h"
#include "ld_operator_element_base.h"

namespace Ld {
    class BraceConditionalOperatorVisual;

    /**
     * Element that provides a brace conditional operator.  The operator's mapping of children is:
     *
     *     * All even numbered children represent the calculated values.
     *     * If the number of children is even:
     *         * All odd numbered children represent the test required for the preceding even numbered value.
     *         * The last odd numbered child represents the "otherwise" or "else" value.  This child is provided for
     *           notational purposes and is otherwise ignored.
     *     * If the number of children is odd:
     *         * All odd numbered children represent the test required for the preceding even numbered value.
     *         * The last even numbered child does not have an associated test.  For this scenario, region 1 text can
     *           contain the "otherwise" text.
     *     * Region 0 text is placed in front of each test child entry.  Region 0 text is not used if there are an odd
     *       number of children.
     *
     * This element contains a small number of convenience methods to help formalize these rules.
     */
    class LD_PUBLIC_API BraceConditionalOperatorElement:public OperatorElementBase {
        Q_DECLARE_TR_FUNCTIONS(Ld::BraceConditionalOperatorElement)

        public:
            /**
             * The typename for the text element.
             */
            static const QString elementName;

            /**
             * Static value indicating the capabilities provides by this element.
             */
            static const Capabilities childProvides;

            BraceConditionalOperatorElement();

            ~BraceConditionalOperatorElement() override;

            /**
             * Factory method that can be used to create a default instance of a text element.
             *
             * \param[in] typeName The name used to reference the creator function.
             *
             * \return Returns a newly created, default, text element instance.
             */
            static Element* creator(const QString& typeName);

            /**
             * Method that returns the type name associated with this object.
             *
             * \return Returns the typename associated with this object.  The value must match the type used to invoke
             *         the static \ref Element::create and \ref Element::registerCreator methods.
             */
            QString typeName() const override;

            /**
             * Method that returns the name of the plug-in that provides this element.
             *
             * \return Returns the name of the plug-in that provides this element. The name must match the name provided
             *         in the plug-in structures.  Elements defined as part of the ineld library should return an empty
             *         string.
             */
            QString plugInName() const override;

            /**
             * Method that returns a description of this element.
             *
             * \return Returns a description of this element suitable for use in a GUI.
             */
            QString description() const override;

            /**
             * Method you can use to determine the value type this element represents.  Elements typically either
             * either report a fixed value or calculate the value based on their children.
             *
             * \return Returns the value type this element would represent.  This version always returns
             *         Ld::DataType::ValueType::NONE.
             */
            DataType::ValueType valueType() const override;

            /**
             * Method you can call to determine the requirements imposed by this element on a specific child element.
             * Note that no testing is performed by elements to confirm that requirements are met.  The information is
             * expected to be used by the user interface.
             *
             * \param[in] childIndex The zero based child index.
             *
             * \return Returns the requirements imposed by this parent on the child element.  The default
             *         implementation returns Ld::Capabilities::rValue.
             */
            Capabilities parentRequires(unsigned long childIndex) const override;

            /**
             * Method you can call to determine what capabilities this element will provide.  Note that this object
             * also provides a static constant value named "childProvides" returning the same information.
             *
             * \return Returns the requirements met by this element.  The default implementation returns
             *         Ld::Capabilities::operator
             */
            Capabilities childProvidesCapabilities() const override;

            /**
             * Convenience method you can use to set the number of supported conditions as well as how the else
             * condition should be interpreted.
             *
             * \param[in]     numberExplicitConditions This value holds the number of conditions that need to be stated
             *                                         explicitly.
             *
             * \param[in]     includeElseCondition     If true, a conditional for the else case will be included.
             *
             * \param[in]     defaultChild             A child element that will be cloned into empty locations.
             *
             * \param[in,out] cursorStateCollection    A pointer to a collection of cursor instances to be adjusted by
             *                                         this method.  No adjustments are performed if a null pointer is
             *                                         provided.
             */
            void setNumberConditions(
                unsigned long          numberExplicitConditions,
                bool                   includeElseCondition,
                ElementPointer         defaultChild,
                CursorStateCollection* cursorStateCollection
            );

            /**
             * Convenience method you can use to determine the number of explicit conditions associated with this
             * element.
             *
             * \return Returns the number of explicit conditions, excluding the else condition.
             */
            unsigned long numberExplicitConditions() const;

            /**
             * Convenience method you can use to determine if the else condition has an dummy operator.
             *
             * \return Returns true if there is an explicit else condition.  Returns false if there is an implicit else
             *         condition.
             */
            bool hasExplicitElseCondition() const;

            /**
             * Convenience method you can use to obtain the condition operator for a specific condition.
             *
             * \param[in] conditionNumber The zero based condition number.
             *
             * \return Returns a shared pointer to the associated element.
             */
            ElementPointer conditionElement(unsigned long conditionNumber) const;

            /**
             * Convenience method you can use to obtain the value for a specific condition.
             *
             * \param[in] conditionNumber The zero based condition number.
             *
             * \return Returns a shared pointer to the associated element.
             */
            ElementPointer valueElement(unsigned long conditionNumber) const;

            /**
             * Convenience method you can use to obtain the else condition element.
             *
             * \return Returns the current else condition element.  A null pointer will be returned if there is no
             *         explicit else condition.
             */
            ElementPointer elseConditionElement() const;

            /**
             * Convenience method you can use to obtain the else value element.
             *
             * \return Returns the current else value element.  A null pointer will be returned if there is no
             *         explicit else condition.
             */
            ElementPointer elseValueElement() const;

            /**
             * Method you can use to change the object used to manage visual representation of this element.  Note that
             * using this method will also cause the \ref Visual::element method to point back to this
             * \ref BraceConditionalOperatorElement instance.
             *
             * If the newly added \ref BraceConditionalOperatorVisual already points to a different element, then those
             * connections will be broken in order to maintain a 1:1 relationship between any
             * \ref BraceConditionalOperatorVisual instance and \ref BraceConditionalOperatorElement instance.
             *
             * If this element currently has an associated visual, then that visual be will disconnected from this
             * element.  The disconnected visual will *not* be deleted.
             *
             * \param[in] newVisual A pointer to the new visual object.  Note that this element will take ownership of
             *                      the visual.
             */
            void setVisual(BraceConditionalOperatorVisual* newVisual);

            /**
             * Method you can use to obtain the visual representation of this element.
             *
             * \return Returns a pointer to the visual representation of this element.  A null pointer is returned if
             *         no visual representation has been defined.
             */
            BraceConditionalOperatorVisual* visual() const;

        protected:
            /**
             * The default number of children.
             */
            static const unsigned defaultNumberChildren;

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
             * Method you can overload to report the intrinsic precedence of the operator.  The intrinsic precedence is
             * the translation precedence without parenthesis.
             *
             * \return Returns the intrinsic precedence of the operator.  THe default implementation returns the
             *         default precedence.
             */
            Precedence intrinsicPrecedence() const override;
    };
};

#endif
