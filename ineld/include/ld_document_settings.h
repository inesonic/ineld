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
* This header defines the \ref Ld::DocumentSettings class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_DOCUMENT_SETTINGS_H
#define LD_DOCUMENT_SETTINGS_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QMap>
#include <QSharedPointer>
#include <QWeakPointer>

#include "ld_common.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_xml_element.h"
#include "ld_document_setting.h"
#include "ld_document_settings.h"

namespace Ld {
    class Element;

    /**
     * Class that can be used to track and encapsulate a collection of \ref Ld::DocumentSetting instances.
     */
    class LD_PUBLIC_API DocumentSettings:public QMap<QString, QSharedPointer<DocumentSetting>> {
        Q_DECLARE_TR_FUNCTIONS(Ld::DocumentSettings)

        public:
            DocumentSettings();

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            DocumentSettings(const DocumentSettings& other);

            ~DocumentSettings();

            /**
             * Template method you can use to more easily obtain document setting instances.
             *
             * \param[in] T The format class type to be created.
             *
             * \return Returns a shared pointer to the requested instance.  A null pointer is returned if the instance
             *         has not been defined.
             */
            template<class T> LD_PUBLIC_TEMPLATE_METHOD QSharedPointer<T> fromType() {
                QSharedPointer<DocumentSetting> newSetting = value(T::settingName);
                return newSetting.dynamicCast<T>();
            }

            /**
             * Method you can use to add a new setting instance.
             *
             * \param[in] newInstance The instance to be added.  If an existing instance of the same type is already
             *                        present, then this instance replaces the old instance.
             */
            void insert(QSharedPointer<DocumentSetting> newInstance);

            /**
             * Method that you can use to add this object to a program file's XML description.  The method will
             * write XML descriptions for each setting entry but will not include top level start or end tags.
             *
             * \param[in] writer The writer used to write the XML description of the program.
             */
            void writeXml(QSharedPointer<XmlWriter> writer);

            /**
             * Method that you can use to update the state of this object instance based on an incoming XML stream.  The
             * method should be called immediately after seeing a start tag for the settings section and will run until
             * a matching end tag is found.
             *
             * Errors are be reported directly to the reader using the raiseError method.
             *
             * \param[in] reader     The reader used to parse the incoming XML stream.
             *
             * \param[in] xmlVersion The XML major version code.  The value can be used to modify how the incoming XML
             *                       stream is parsed.
             */
            void readXml(QSharedPointer<XmlReader> reader, unsigned xmlVersion);
    };
};

#endif
