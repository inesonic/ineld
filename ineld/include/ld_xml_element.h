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
* This header defines the \ref Ld::XmlElement class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_XML_ELEMENT_H
#define LD_XML_ELEMENT_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QSharedPointer>
#include <QMap>

#include "ld_common.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_format_container.h"

namespace Ld {
    class FormatOrganizer;
    class ProgramFile;

    /**
     * Pure virtual base class for any object that can be loaded/saved to a program file as part of the XML description.
     * The base class provides a set of methods and method prototypes that provide a common API used to both write XML
     * and parse XML.
     */
    class LD_PUBLIC_API XmlElement {
        Q_DECLARE_TR_FUNCTIONS(Ld::XmlElement)

        public:
            /**
             * Value used to indicate the XML version being read is invalid or unspecified.
             */
            static constexpr unsigned invalidXmlVersion = static_cast<unsigned>(-1);

            XmlElement();

            virtual ~XmlElement();

            /**
             * Pure virtual method that returns the type name associated with this object.
             *
             * \return Returns the typename associated with this object.  The value must match the type used to invoke
             *         the static \ref Element::create and \ref Element::registerCreator methods.
             */
            virtual QString typeName() const = 0;

            /**
             * Method that you can use to add this object to a program file's XML description.  You can overload this
             * method if you want to customize how an object instance is added to the XML based program description.
             *
             * The default implementation will create an XML start tag using the object's typename.  Inherited
             * attributes passed to this method as well as attributes added using the
             * \ref XmlElement::writeAddAttributes method will be added to the XML start tag.  PCDATA is included based
             * on the data returned by the \ref XmlElement::writeData method.  Lastly, child tags are added using the
             * \ref XmlElement::writeChildren method.
             *
             * \param[in]     writer              The writer used to write the XML description of the program.
             *
             * \param[in]     formats             A format organizer used to track all known formats within a program.
             *
             * \param[in,out] programFile         The program file instance that can be used to store (or retrieve)
             *                                    additional side-band information.
             *
             * \param[in]     inheritedAttributes Attributes provided by the calling class to this class.  Provided
             *                                    attributes should be provided with child's XML start tag.  This
             *                                    parameter exists to allow the parent to include positional
             *                                    information as attributes of each child.
             */
            virtual void writeXml(
                QSharedPointer<XmlWriter>       writer,
                QSharedPointer<FormatOrganizer> formats,
                ProgramFile&                    programFile,
                const XmlAttributes&            inheritedAttributes
            ) const;

            /**
             * Method that you can use to update the state of this object instance based on an incoming XML stream.  The
             * method will generally be called on an instance immediately after construction by the creator factor
             * function and will be called during parsing of the XML stream immediately after encountering the XML start
             * tag.  You can overload this method if you wish to modify how the XML stream is parsed for this object.
             *
             * The default implementation will call the \ref XmlElement::readAttributes method to parse the provided
             * attributes.  Any PCDATA will trigger one or more calls to \ref XmlElement::readData.  Any encountered
             * XML tags will trigger calls to the \ref XmlElement::readChild method.  The method will returns when the
             * XML end tag associated with this XML tag is encountered.
             *
             * Errors can be reported directly to the reader using the raiseError method.
             *
             * \param[in]     reader      The reader used to parse the incoming XML stream.
             *
             * \param[in]     formats     A map of format instances by format identifier.  You can use this parameter
             *                            to either tie a format to an object.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             *
             * \param[in]     attributes  Attributes tied to the XML start tag.
             *
             * \param[in]     xmlVersion  The XML major version code.  The value can be used to modify how the incoming XML
             *                            stream is parsed.
             */
            virtual void readXml(
                QSharedPointer<XmlReader>  reader,
                const FormatsByIdentifier& formats,
                ProgramFile&               programFile,
                const XmlAttributes&       attributes,
                unsigned                   xmlVersion = invalidXmlVersion
            );

        protected:
            /**
             * Method you can overload to include PCDATA into the XML description of this element.  Returning an empty
             * string will cause PCDATA to be omitted.
             *
             * \param[in]     formats     A \ref FormatOrganizer instance used to track all known formats in the
             *                            program.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             *
             * \return Returns the PCDATA to be included in the written XML description.  Note that the XML writer will
             *         automatically translate special characters such as ">" to their XML escaped sequences (e.g.
             *         &gt;).  The default implementation returns an empty string.
             */
            virtual QString writeData(QSharedPointer<const FormatOrganizer> formats, ProgramFile& programFile) const;

            /**
             * Method you can optionally overload to add additional attributes to the XML description of this element.
             * The default implementation simply returns without modifying the attribute list.
             *
             * \param[in]     attributes  A reference to the list of attributes to be modified.
             *
             * \param[in]     formats     A \ref FormatOrganizer instance used to track all known formats in the
             *                            program.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             */
            virtual void writeAddAttributes(
                XmlAttributes&                  attributes,
                QSharedPointer<FormatOrganizer> formats,
                ProgramFile&                    programFile
            ) const;

            /**
             * Pure virtual method that writes child elements to the program file.
             *
             * \param[in]     writer              The writer used to write the XML description of the program.
             *
             * \param[in]     formats             A \ref FormatOrganizer instance used to track all known formats in
             *                                    the program.
             *
             * \param[in,out] programFile         The program file instance that can be used to store (or retrieve)
             *                                    additional side-band information.
             *
             * \param[in]     inheritedAttributes Attributes provided by the calling class to this class.  Provided
             *                                    attributes should be provided with child's XML element.  This
             *                                    parameter exists to allow the parent to include positional
             *                                    information as attributes of each child.
             */
            virtual void writeChildren(
                QSharedPointer<XmlWriter>       writer,
                QSharedPointer<FormatOrganizer> formats,
                ProgramFile&                    programFile,
                const XmlAttributes&            inheritedAttributes
            ) const = 0;

            /**
             * Method you can overload to parse incoming XML attributes.  This method will only be called if there are
             * provided attributes.
             *
             * Errors can be reported directly to the reader using the raiseError method.  The default implementation
             * will raise an error indicating that attributes are not allowed.
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
            virtual void readAttributes(
                QSharedPointer<XmlReader>  reader,
                const XmlAttributes&       attributes,
                const FormatsByIdentifier& formats,
                ProgramFile&               programFile,
                unsigned                   xmlVersion
            );

            /**
             * Method you can overload to parse incoming PCDATA tied to the XML description for this object.  This
             * method will only be called if non-empty PCDATA is provided.
             *
             * Errors can be reported directly to the reader using the raiseError method.  The default implementation
             * will raise an error if non-whitespace text is detected indicating that text is now allowed for the tag.
             *
             * \param[in]     reader       The reader used to parse the incoming XML stream.
             *
             * \param[in]     text         The PCDATA to be parsed.  XML tokens such as &gt; are pre-processed and
             *                             converted prior to the call to this method.
             *
             * \param[in]     formats      A map of format instances by format identifier.  You can use this parameter
             *                             to either tie a format to an object or add a new format to the list of known
             *                             formats.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             *
             * \param[in]     xmlVersion  The XML major version code.  The value can be used to modify how the incoming
             *                            XML stream is parsed.
             */
            virtual void readData(
                QSharedPointer<XmlReader>  reader,
                const QString&             text,
                const FormatsByIdentifier& formats,
                ProgramFile&               programFile,
                unsigned                   xmlVersion
            );

            /**
             * Method that you can use to read and process a child XML tag.  The method is called right after the XML
             * tag is detected in the XML stream.
             *
             * Errors can be reported directly to the reader using the raiseError method.  The default implementation
             * will raise an error indicating that child tags are not allowed.
             *
             * \param[in]     reader      The reader used to parse the incoming XML stream.
             *
             * \param[in]     tagName     The name of the spotted XML tag.
             *
             * \param[in]     formats     A map of format instances by format identifier.  You can use this parameter
             *                            to either tie a format to an object or add a new format to the list of known
             *                            formats.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             *
             * \param[in]     attributes  Attributes tied to the XML start tag.
             *
             * \param[in]     xmlVersion  The XML major version code.  The value can be used to modify how the incoming
             *                            XML stream is parsed.
             */
            virtual void readChild(
                QSharedPointer<XmlReader>  reader,
                const QString&             tagName,
                const FormatsByIdentifier& formats,
                ProgramFile&               programFile,
                const XmlAttributes&       attributes,
                unsigned                   xmlVersion
            );

            /**
             * Assignment operator
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            XmlElement& operator=(const XmlElement& other) {
                (void) other;
                return *this;
            }
    };
};

#endif
