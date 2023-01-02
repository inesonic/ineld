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
∂********************************************************************************************************************//**
* \file
*
* This header defines the \ref Ld::TextMemoryExportContext class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_TEXT_MEMORY_EXPORT_CONTEXT_H
#define LD_TEXT_MEMORY_EXPORT_CONTEXT_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QMap>
#include <QIODevice>
#include <QByteArray>

#include "ld_common.h"
#include "ld_text_export_context.h"

namespace Ld {
    /**
     * Class you can use to export contexts into one or more memory buffers.
     */
    class LD_PUBLIC_API TextMemoryExportContext:public TextExportContext {
        Q_DECLARE_TR_FUNCTIONS(Ld::TextMemoryExportContext)

        public:
            /**
             * Constructor
             *
             * \param[in] outputFile   The name of the output file to contain the exported text.
             *
             * \param[in] savePayloads If true, the additional payloads will be stored.  If false, the additional
             *                         payloads will be silently ignored.
             */
            TextMemoryExportContext(const QString& outputFile, bool savePayloads = true);

            ~TextMemoryExportContext() override;

            /**
             * Method you can call to add side-band payload to the XHTML description.
             *
             * \param[in] payloadName The name to assign to the payload.  This will generally be a filename of some
             *                        kind.
             *
             * \param[in] payload     The payload to be included.
             *
             * \return Returns the absolute filepath where the payload was written.  If the payload is stored to
             *         memory, then this method will return the payload name.  An empty string will be returned if an
             *         error occurred.
             */
            QString addPayload(const QString& payloadName, const QByteArray& payload) override;

            /**
             * Method you can call to obtain a payload, by name.
             *
             * \param[in] payloadName The name of the desired payload.
             *
             * \return Returns the requested payload.
             */
            QByteArray payload(const QString& payloadName) override;

            /**
             * Method you can call to obtain a list of all the known payloads, by name.
             *
             * \return Returns a list of payloads, by name.
             */
            QList<QString> payloads() const override;

        private:
            /**
             * The name assigned to the main output file.
             */
            QString currentOutputFilename;

            /**
             * The output file's memory buffer.
             */
            QByteArray currentOutputFile;

            /**
             * Flag indicating if we should save payloads.
             */
            bool currentSavePayloads;

            /**
             * Map of output files by payload name.
             */
            QMap<QString, QByteArray> payloadsByName;
    };
};

#endif
