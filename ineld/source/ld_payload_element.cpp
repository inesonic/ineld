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
* This file implements the \ref Ld::PayloadElement class.
***********************************************************************************************************************/

#include <QSharedPointer>
#include <QByteArray>

#include "ld_element_structures.h"
#include "ld_format_organizer.h"
#include "ld_visual_with_no_children.h"
#include "ld_format_container.h"
#include "ld_payload_data.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_root_element.h"
#include "ld_element.h"
#include "ld_payload_visual.h"
#include "ld_element_with_no_children.h"
#include "ld_payload_element.h"

namespace Ld {
    PayloadElement::PayloadElement() {}


    PayloadElement::~PayloadElement() {}


    ElementPointer PayloadElement::clone(bool) const {
        ElementPointer element = cloneThisElementOnly();
        element.dynamicCast<PayloadElement>()->currentPayload = currentPayload;

        return element;
    }


    ElementPointer PayloadElement::clone(
        unsigned long        startingTextIndex,
        unsigned             startingRegionIndex,
        unsigned long        endingTextIndex,
        unsigned             endingRegionIndex,
        bool
    ) const {
        ElementPointer element = cloneThisElementOnly(
            startingTextIndex,
            startingRegionIndex,
            endingTextIndex,
            endingRegionIndex
        );
        element.dynamicCast<PayloadElement>()->currentPayload = currentPayload;

        return element;
    }


    void PayloadElement::setVisual(PayloadVisual* newVisual) {
        ElementWithNoChildren::setVisual(newVisual);
    }


    PayloadVisual* PayloadElement::visual() const {
        return dynamic_cast<PayloadVisual*>(ElementWithNoChildren::visual());
    }


    PayloadElement::StorageMode PayloadElement::storageMode() const {
        return currentPayload.storageMode();
    }


    bool PayloadElement::supportsPayloads() const {
        return true;
    }


    bool PayloadElement::updatePayload(const QByteArray& newData) {
        bool success;

        if (currentPayload.isInvalid()) {
            success = currentPayload.writeData(newData);
        } else if (currentPayload.numberReferences() == 1) {
            success = currentPayload.writeData(newData);
        } else {
            ElementPointer rawRootElement = root();
            if (rawRootElement->typeName() == RootElement::elementName) {
                QSharedPointer<RootElement> rootElement = rawRootElement.dynamicCast<RootElement>();
                PayloadData newPayloadData = rootElement->newPayload();

                success = newPayloadData.writeData(newData);
                if (success) {
                    currentPayload = newPayloadData;
                }
            } else {
                success = false;
            }
        }

        if (success) {
            payloadChanged();
        }

        return success;
    }


    bool PayloadElement::getPayload(QByteArray& newData) {
        bool success = currentPayload.readData(newData);

        if (!success) {
            payloadCouldNotBeLoaded();
        }

        return success;
    }


    void PayloadElement::graftedToTree() {
        ElementWithNoChildren::graftedToTree();

        ElementPointer rawRootElement = root();
        if (rawRootElement->typeName() == RootElement::elementName) {
            QSharedPointer<RootElement> rootElement = rawRootElement.dynamicCast<RootElement>();
            assert(rootElement != nullptr);

            if (currentPayload.isInvalid()) {
                currentPayload = rootElement->newPayload();
            } else {
                PayloadData newPayloadData = rootElement->clonePayload(currentPayload);
                if (newPayloadData.isValid()) {
                    currentPayload = newPayloadData;
                } else {
                    payloadCouldNotBeLoaded();
                }
            }
        }
    }


    void PayloadElement::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        ElementWithNoChildren::writeAddAttributes(attributes, formats, programFile);
        attributes.append("payload_id", currentPayload.payloadId());
    }


    void PayloadElement::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        ElementWithNoChildren::readAttributes(reader, attributes, formats, programFile, xmlVersion);

        bool ok;
        PayloadData::PayloadId payloadId = attributes.value<PayloadData::PayloadId>(
            "payload_id",
            PayloadData::invalidPayloadId,
            &ok
        );

        if (!ok || payloadId == PayloadData::invalidPayloadId) {
            reader->raiseError(
                tr("Tag \"%1\" has invalid or missing payload ID").arg(reader->qualifiedName().toString())
            );
        } else {
            currentPayload = programFile.payload(payloadId);
            if (currentPayload.isInvalid()) {
                reader->raiseError(
                    tr("Tag \"%1\" payload %1 missing.").arg(reader->qualifiedName().toString()).arg(payloadId)
                );
            }
        }
    }


    void PayloadElement::payloadChanged() {
        PayloadVisual* payloadVisual = dynamic_cast<PayloadVisual*>(visual());
        if (payloadVisual != nullptr) {
            payloadVisual->payloadChanged();
        }

        elementDataChanged();
    }


    void PayloadElement::payloadCouldNotBeLoaded() {
        PayloadVisual* payloadVisual = dynamic_cast<PayloadVisual*>(visual());
        if (payloadVisual != nullptr) {
            payloadVisual->payloadCouldNotBeLoaded();
        }
    }
}
