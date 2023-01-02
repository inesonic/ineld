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
* This header defines the \ref Ld::DefaultFormatSetting class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_DEFAULT_FORMAT_SETTING_H
#define LD_DEFAULT_FORMAT_SETTING_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QSharedPointer>

#include "ld_handle.h"
#include "ld_format_container.h"
#include "ld_xml_attributes.h"
#include "ld_document_setting.h"

namespace Ld {
    class RootElement;
}

namespace Ld {
    /**
     * Class you can use to embed a default format, by identifier, into the document settings data.
     */
    class DefaultFormatSetting:public Ld::DocumentSetting {
        Q_DECLARE_TR_FUNCTIONS(Ld::DefaultFormatSetting)

        public:
            /**
             * The setting name used to reference the text format setting.
             */
            static const char defaultTextFormatSettingName[];

            /**
             * The setting name used to reference the math text format setting.
             */
            static const char defaultMathFormatSettingName[];

            /**
             * The setting name used to reference the math identifier format setting.
             */
            static const char defaultMathIdentifierFormatSettingName[];

            /**
             * The setting name used to reference the math function format setting.
             */
            static const char defaultMathFunctionFormatSettingName[];

            /**
             * The setting name used to reference the brace conditional format setting.
             */
            static const char defaultBraceConditionalFormatSettingName[];

            /**
             * Constructor
             *
             * \param[in] settingName The setting name to be reported.
             */
            DefaultFormatSetting(const QString& settingName);

            ~DefaultFormatSetting() override;

            /**
             * Creator method you can use to create instances of this class.
             *
             * \param[in] typeName The name used to reference this creator function.  A single creator function can be
             *                     used to create multiple object types using this value to identify what type of
             *                     format to be created.
             *
             * \return Returns a pointer to this instance.
             */
            static Ld::DocumentSetting* creator(const QString& typeName);

            /**
             * Method that returns the type name associated with this object.
             *
             * \return Returns the typename associated with this object.  The value must match the type used to invoke
             *         the static Element::create and Element::registerCreator methods.
             */
            QString typeName() const override;

            /**
             * Pure virtual method that indicates if this format contains valid information.
             *
             * \return Returns true if the format is valid.  Returns false if the format is invalid.
             */
            bool isValid() const override;

            /**
             * Method you can use to set the format identifier.
             *
             * \param[in] newFormatIdentifier The new format identifier value.
             */
            void setFormatIdentifier(FormatIdentifier newFormatIdentifier);

            /**
             * Method you can use to obtain the saved format identifier.
             *
             * \return Returns the saved format identifier.
             */
            FormatIdentifier formatIdentifier() const;

        protected:
            /**
             * Method you can optionally overload to add additional attributes to the XML description of this element.
             *
             * \param[in] attributes A reference to the list of attributes to be modified.
             */
            void writeAddAttributes(Ld::XmlAttributes& attributes) override;

            /**
             * Method you can overload to parse incoming XML attributes.  This method will only be called if there are
             * provided attributes.
             *
             * Errors are reported directly to the reader using the raiseError method.
             *
             * \param[in] reader     The reader used to parse the incoming XML stream.
             *
             * \param[in] attributes The XML attributes to be parsed.
             *
             * \param[in] xmlVersion The XML major version code.  The value can be used to modify how the incoming XML
             *                       stream is parsed.
             */
            void readAttributes(
                QSharedPointer<Ld::XmlReader> reader,
                const Ld::XmlAttributes&      attributes,
                unsigned                      xmlVersion
            ) override;

        private:
            /**
             * The setting name.
             */
            QString currentSettingName;

            /**
             * The format identifier.
             */
            FormatIdentifier currentFormatIdentifier;
    };
}


#endif
