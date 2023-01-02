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
* This header defines the \ref Ld::RootVisual class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_ROOT_VISUAL_H
#define LD_ROOT_VISUAL_H

#include <QtGlobal>
#include <QSharedPointer>
#include <QString>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_visual_with_positional_children.h"

namespace Ld {
    class RootElement;
    class PageFormat;

    /**
     * Pure virtual base class for the root visual.
     */
    class LD_PUBLIC_API RootVisual:public virtual VisualWithPositionalChildren {
        friend class RootElement;

        public:
            RootVisual();

            ~RootVisual() override;

            /**
             * Method you can use to obtain the \ref ElementWithPositionalChildren class associated with this visual.
             *
             * \return Returns a pointer to the associated \ref ElementWithPositionalChildren instance.
             */
            QSharedPointer<RootElement> element() const;

            /**
             * Method that returns the type name associated with this visual.
             *
             * \return Returns the typename associated with this visual.  The value must match the type used to invoke
             *         the static \ref Visual::create and \ref Visual::registerCreator methods.
             */
            QString typeName() const final;

            /**
             * Method that returns the name of the plug-in that provides this visual.
             *
             * \return Returns the name of the plug-in that provides this visual. The name must match the name provided
             *         in the plug-in structures.   Visuals defined as part of the ineld library should return an empty
             *         string.
             */
            QString plugInName() const final;

        protected:
            /**
             * Virtual method that is called to determine if the associated root element and visual can be deleted.
             * You should overload this method if you wish to be able to block automatic destruction when there are no
             * more referenced to the associated root element.
             *
             * \return Returns true if the root element and visual can be deleted.  Returns false if the element must
             *         be preserved.  Note that the visual is responsible for triggering removal of the element if it
             *         is no longer referenced.  The default implementation always returns true.
             */
            virtual bool canDeleteRootElement();

            /**
             * Virtual method that is called when a change to the program is detected.
             *
             * \param[in] changedElement Pointer to the changed element.
             */
            virtual void elementChanged(ElementPointer changedElement);

            /**
             * Virtual method that is called when a new element is added.
             *
             * \param[in] newElement Pointer to the newly added element.
             */
            virtual void elementAdded(ElementPointer newElement);

            /**
             * Virtual method that is called when an element is removed.
             *
             * \param[in] removedElement Pointer to the element that is about to be removed.
             */
            virtual void elementRemoved(ElementPointer removedElement);

            /**
             * Virtual method that is called when a page format is changed.
             *
             * \param[in] pageNumber The page number of the first page touched by the format change.
             *
             * \param[in] oldFormat  The old page format that is being replaced.
             *
             * \param[in] newFormat  The new format assigned to the page.
             */
            virtual void pageFormatChanged(
                unsigned long              pageNumber,
                QSharedPointer<PageFormat> oldFormat,
                QSharedPointer<PageFormat> newFormat
            );

            /**
             * Virtual method that is called when the root element and children are loaded or saved and become pristine.
             */
            virtual void nowPristine();

            /**
             * Virtual method that is called when the root element or children are first modified.
             */
            virtual void nowModified();

            /**
             * Virtual method that is called whenever the root element or any child is modified, including changes to
             * references.  This method will not be called when the root element is loaded or saved.
             */
            virtual void nowChanged();

            /**
             * Virtual method that is called when references are updated.
             */
            virtual void referencesChanged();

            /**
             * Virtual method that is called when the program is saved under its current name.
             *
             * \param[in] filename The filename the program was saved under.
             */
            virtual void programSaved(const QString& filename);

            /**
             * Virtual method that is called when the program is saved under a new name.
             *
             * \param[in] filename The filename the program was saved under.
             */
            virtual void programSavedAs(const QString& filename);

            /**
             * Virtual method that is called when an error is detected during a program load operation.
             *
             * \param[in] errorMessage A string explaining the reported error.
             */
            virtual void programSaveFailed(const QString& errorMessage);

            /**
             * Virtual method that is called when a new program is loaded under this root node.
             *
             * \param[in] filename     The filename the program was saved under.
             *
             * \param[in] majorVersion The major version number of the XML description.
             *
             * \param[in] minorVersion The minor version number of the XML description.
             */
            virtual void programLoaded(const QString& filename, unsigned majorVersion, unsigned minorVersion);

            /**
             * Virtual method that is called when an error is detected during a program load operation.
             *
             * \param[in] errorMessage A string explaining the reported error.
             */
            virtual void programLoadFailed(const QString& errorMessage);

            /**
             * Virtual method that is called when a new and pristine program is created under this root node.
             */
            virtual void newProgramCreated();

            /**
             * Virtual method that is called just before a program is closed.
             */
            virtual void aboutToClose();
    };
};

#endif
