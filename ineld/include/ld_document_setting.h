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
* This header defines the \ref Ld::DocumentSetting class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_DOCUMENT_SETTING_H
#define LD_DOCUMENT_SETTING_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QMap>
#include <QList>
#include <QSet>
#include <QHash>
#include <QSharedPointer>
#include <QWeakPointer>

#include <util_hash_functions.h>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"

namespace Ld {
    class Element;

    /**
     * Pure virtual base class for all classes used to hold document global settings data.  All instances are expected
     * to have a static instance of a string called "settingName" that uniquely identify the name of the document
     * setting.
     */
    class LD_PUBLIC_API DocumentSetting {
        Q_DECLARE_TR_FUNCTIONS(Ld::DocumentSetting)

        public:
            /**
             * Type used to define function pointers to default document setting creation functions.  In most cases, a
             * creator function can simply instantiate a new, default, instance of an object on the heap.
             *
             * The \ref DocumentSetting::readXml method will be called on default instances if those instances need to
             * be populated from an XML description.
             *
             * \param[in] typeName The name used to reference this creator function.  A single creator function can be
             *                     used to create multiple object types using this value to identify what type of
             *                     format to be created.
             *
             * \return Returns a pointer to the newly created format instance.
             */
            typedef DocumentSetting* (*CreatorFunction)(const QString& typeName);

            /**
             * Static method you can use to create a new document setting instance given a name.
             *
             * \param[in] typeName The name used to reference this format type.
             *
             * \return Returns a pointer to a class derived from \ref DocumentSetting.
             */
            static QSharedPointer<DocumentSetting> create(const QString& typeName);

            /**
             * Static method you can use to register methods used to create default objects by type name.
             *
             * \param[in] typeName        The name used to reference this document setting.
             *
             * \param[in] creatorFunction Pointer to the function that should be called to create a default instance of
             *                            the correct object type.
             *
             * \return Returns true on success, returns false if an object creator function has already been registered
             *         for the specified object.
             */
            static bool registerCreator(const QString& typeName, CreatorFunction creatorFunction);

            /**
             * Template method you can use to more easily create document setting instances.
             *
             * \param[in] T The format class type to be created.
             */
            template<class T> LD_PUBLIC_TEMPLATE_METHOD static QSharedPointer<T> createDocumentSetting() {
                QSharedPointer<DocumentSetting> newSetting = create(T::settingName);
                return newSetting.dynamicCast<T>();
            }

            DocumentSetting();

            virtual ~DocumentSetting();

            /**
             * Pure virtual method that returns the type name associated with this object.
             *
             * \return Returns the typename associated with this object.  The value must be used consistently whenever
             *         referencing this type.
             */
            virtual QString typeName() const = 0;

            /**
             * Pure virtual method that indicates if this format contains valid information.
             *
             * \return Returns true if the format is valid.  Returns false if the format is invalid.
             */
            virtual bool isValid() const = 0;

            /**
             * Method that indicates if this format contains invalid information.  The method obtains its value by
             * calling the \ref Ld::Format::isValid method and inverting the result.
             *
             * \return Returns true if the format is invalid.  Returns false if the format is valid.
             */
            bool isInvalid() const;

            /**
             * Method that you can use to add this object to a program file's XML description.  You can overload this
             * method if you want to customize how an object instance is added to the XML based program description.
             *
             * The default implementation will create an XML start tag using the object's typename.  Attributes are
             * added using the \ref DocumentSetting::writeAddAttributes method.  PCDATA is included based on the data
             * returned by the \ref DocumentSetting::writeData method.
             *
             * \param[in] writer The writer used to write the XML description of the program.
             */
            virtual void writeXml(QSharedPointer<XmlWriter> writer);

            /**
             * Method that you can use to update the state of this object instance based on an incoming XML stream.
             * The method will generally be called on an instance immediately after construction by the creator factor
             * function and will be called during parsing of the XML stream immediately after encountering the XML
             * start tag.  You can overload this method if you wish to modify how the XML stream is parsed for this
             * object.
             *
             * The default implementation will call the \ref DocumentSetting::readAttributes method to parse the
             * provided attributes.  Any PCDATA will trigger one or more calls to \ref DocumentSetting::readData.  Any
             * encountered XML tags will cause an error to be reported.  The method will return when the XML end tag
             * associated with this XML tag is encountered.
             *
             * Errors can be reported directly to the reader using the raiseError method.
             *
             * \param[in] reader     The reader used to parse the incoming XML stream.
             *
             * \param[in] attributes Attributes tied to the XML start tag.
             *
             * \param[in] xmlVersion The XML major version code.  The value can be used to modify how the incoming XML
             *                       stream is parsed.
             */
            virtual void readXml(
                QSharedPointer<XmlReader> reader,
                const XmlAttributes&      attributes,
                unsigned                  xmlVersion
            );

        protected:
            /**
             * Method you can overload to include PCDATA into the XML description of this element.  Returning an empty
             * string will cause PCDATA to be omitted.
             *
             * \return Returns the PCDATA to be included in the written XML description.  Note that the XML writer will
             *         automatically translate special characters such as ">" to their XML escaped sequences (e.g.
             *         &gt;).  The default implementation returns an empty string.
             */
            virtual QString writeData();

            /**
             * Method you can optionally overload to add additional attributes to the XML description of this element.
             * The default implementation simply returns without modifying the attribute list.
             *
             * \param[in] attributes A reference to the list of attributes to be modified.
             */
            virtual void writeAddAttributes(XmlAttributes& attributes);

            /**
             * Method you can overload to parse incoming XML attributes.  This method will only be called if there are
             * provided attributes.
             *
             * Errors can be reported directly to the reader using the raiseError method.  The default implementation
             * will raise an error indicating that attributes are not allowed.
             *
             * \param[in] reader     The reader used to parse the incoming XML stream.
             *
             * \param[in] attributes The XML attributes to be parsed.
             *
             * \param[in] xmlVersion The XML major version code.  The value can be used to modify how the incoming XML
             *                       stream is parsed.
             */
            virtual void readAttributes(
                QSharedPointer<XmlReader> reader,
                const XmlAttributes&      attributes,
                unsigned                  xmlVersion
            );

            /**
             * Method you can overload to parse incoming PCDATA tied to the XML description for this object.  This
             * method will only be called if non-empty PCDATA is provided.
             *
             * Errors can be reported directly to the reader using the raiseError method.  The default implementation
             * will raise an error if non-whitespace text is detected indicating that text is now allowed for the tag.
             *
             * \param[in] reader     The reader used to parse the incoming XML stream.
             *
             * \param[in] text       The PCDATA to be parsed.  XML tokens such as &gt; are pre-processed and converted
             *                       prior to the call to this method.
             *
             * \param[in] xmlVersion The XML major version code.  The value can be used to modify how the incoming XML
             *                       stream is parsed.
             */
            virtual void readData(
                QSharedPointer<XmlReader> reader,
                const QString&            text,
                unsigned                  xmlVersion
            );

        private:
            /**
             * Dictionary used to identify empty format creators based on a format type name.
             */
            static QMap<QString, CreatorFunction> creators;

            /**
             * The set of elements referencing this element.
             */
            ElementWeakPointerSet currentElements;
    };
}

/**
 * Hash function used to manage smart pointers to document settings within a set.
 *
 * \param[in] documentSetting A shared pointer to the setting instance to hash.  We hash based on the pointer address.
 *
 * \param[in] seed            An optional seed to apply to the hash.
 *
 * \return Returns a hash for this smart pointer.
 */
LD_PUBLIC_API Util::HashResult qHash(QSharedPointer<Ld::DocumentSetting> documentSetting, Util::HashSeed seed = 0);

/**
 * Hash function used to manage smart pointers to document settings within a set.
 *
 * \param[in] documentSetting A weak pointer to the document setting instance to hash.  We hash based on the pointer
 *                            address.
 *
 * \param[in] seed   An optional seed to apply to the hash.
 *
 * \return Returns a hash for this smart pointer.
 */
LD_PUBLIC_API Util::HashResult qHash(QWeakPointer<Ld::DocumentSetting> documentSetting, Util::HashSeed seed = 0);

#endif
