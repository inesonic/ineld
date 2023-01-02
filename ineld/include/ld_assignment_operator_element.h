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
* This header defines the \ref Ld::AssignmentOperatorElement class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_ASSIGNMENT_OPERATOR_ELEMENT_H
#define LD_ASSIGNMENT_OPERATOR_ELEMENT_H

#include <QString>
#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_capabilities.h"
#include "ld_binary_relational_operator_element_base.h"

namespace Ld {
    class AssignmentOperatorVisual;

    /**
     * Element that is used to represent an assignment operator or a relational test against equality.
     */
    class LD_PUBLIC_API AssignmentOperatorElement:public BinaryRelationalOperatorElementBase {
        Q_DECLARE_TR_FUNCTIONS(Ld::AssignmentOperatorElement)

        public:
            /**
             * The typename for the text element.
             */
            static const QString elementName;

            /**
             * Static value indicating the capabilities provides by this element.
             */
            static const Capabilities childProvides;

            AssignmentOperatorElement();

            ~AssignmentOperatorElement() override;

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
             * \return Returns the value type this element would represent.  This version will either return
             *         DataType::ValueType::NONE if the element is top level or DataType::ValueType::BOOLEAN if the
             *         operator is being used as a relational operator.
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
             * \return Returns the requirements met by this element.
             */
            Capabilities childProvidesCapabilities() const override;

            /**
             * Method you can use to change the object used to manage visual representation of this element.  Note that
             * using this method will also cause the \ref Visual::element method to point back to this
             * \ref AssignmentOperatorElement instance.
             *
             * If the newly added \ref AssignmentOperatorVisual already points to a different element, then those
             * connections will be broken in order to maintain a 1:1 relationship between any
             * \ref AssignmentOperatorVisual instance and \ref AssignmentOperatorElement instance.
             *
             * If this element currently has an associated visual, then that visual be will disconnected from this
             * element.  The disconnected visual will *not* be deleted.
             *
             * \param[in] newVisual A pointer to the new visual object.  Note that this element will take ownership of
             *                      the visual.
             */
            void setVisual(AssignmentOperatorVisual* newVisual);

            /**
             * Method you can use to obtain the visual representation of this element.
             *
             * \return Returns a pointer to the visual representation of this element.  A null pointer is returned if
             *         no visual representation has been defined.
             */
            AssignmentOperatorVisual* visual() const;

            /**
             * Method you can call to determine if this element represents a top level operator.
             *
             * \return Returns true if this element is a top level operator.  Returns false if this element is not a
             *         top level operator.  This version returns true if this operator is being used as an assignment,
             *         returns false if this operator is being used as a relational operator.
             */
            bool isTopLevelOperator() const override;

            /**
             * Method you can use to determine if you can set a breakpoint directly on this element.
             *
             * \return Returns true if you can set an instruction breakpoint on this element.  Returns false if this
             *         type of element can-not support instruction breakpoints.  This version returns if this element
             *         is top level.
             */
            bool supportsInstructionBreakpoints() const override;

            /**
             * Method you can use to set or clear an instruction breakpoint.
             *
             * \param[in] nowSet If true, an instruction breakpoint will now be set.  If false, any pending
             *                   instruction breakpoint will be cleared.
             *
             * \return Returns true on success, returns false on error.  This version always returns true.
             */
            bool setInstructionBreakpoint(bool nowSet = true) override;

            /**
             * Method you can use to determine if an instruction breakpoint has been set.
             *
             * \return Returns true if this element has a set instruction breakpoint.
             */
            bool instructionBreakpointSet() const override;

        protected:
            /**
             * Method you can use to determine the intrinsic precedence for this element.
             *
             * \return Returns the element's precedence.
             */
            Precedence intrinsicPrecedence() const override;

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

        private:
            /**
             * Method that determines if this element is being used as an assignment operator or a relational operator.
             *
             * \return Returns true if the operator is being used as a relational operator.  Returns false if the
             *         operator is being used as an assignment operator.
             */
            bool isRelationalOperator() const;

            /**
             * Flag that indicates if we should note a breakpoint on this element.
             */
            bool currentBreakpointSet;
    };
};

#endif
