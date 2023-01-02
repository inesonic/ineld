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
* This header defines the \ref Ld::XmlWriter class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_XML_WRITER_H
#define LD_XML_WRITER_H

#include <QXmlStreamWriter>

#include <qvirtual_file.h>

#include "ld_common.h"

class QVirtualFile;

namespace Ld {
    /**
     * Class you can use to write XML.  The class is a very thin wrapper on the Qt QXmlStreamWriter class.
     */
    class LD_PUBLIC_API XmlWriter:public QXmlStreamWriter {

        public:
            XmlWriter();

            /**
             * Constructor.
             *
             * \param[in] virtualFile The virtual file to write the XML data to.
             */
            XmlWriter(QVirtualFile* virtualFile);

            ~XmlWriter();

            /**
             * Method you can use to set the virtual file tied to this XML writer.
             *
             * \param[in] newVirtualFile The virtual file to tie to this XML writer.
             */
            void setVirtualFile(QVirtualFile* newVirtualFile);

            /**
             * Method you can use to obtain a pointer to the virtual file this writer is operating on.
             *
             * \return Returns a pointer to the virtual file tied to this XML writer.
             */
            QVirtualFile* virtualFile() const;
    };
};

#endif
