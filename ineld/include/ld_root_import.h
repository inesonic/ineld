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
* This header defines the \ref Ld::RootImport class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_ROOT_IMPORT_H
#define LD_ROOT_IMPORT_H

#include <QSharedPointer>
#include <QString>

#include "ld_common.h"

namespace Ld {
    class RootElement;

    /**
     * Class that is used to track information regarding an imported a root element.  The class includes accessors to
     * track the associated root element and to track whether the root element's path should be saved as an absolute
     * or relative path.
     *
     * In addition, there are several trivial accessors that provide the correct filepath to use to save or load the
     * root element.
     *
     * Note that this class does not take ownership of the root element.
     */
    class LD_PUBLIC_API RootImport {
        public:
            RootImport();

            /**
             * Constructor.  Creates a valid root import.
             *
             * \param[in] rootElement      The root element to access using this structure.
             *
             * \param[in] isRelativeImport If true, the import will be relative.  If false, the import will be
             *                             bsolute.
             */
            RootImport(QSharedPointer<RootElement> rootElement, bool isRelativeImport = true);

            /**
             * Constructor.  Creates an invalid root import.
             *
             * \param[in] invalidFilename     The filename of the root element that should have been loaded.
             *
             * \param[in] isRelativeImport If true, the import will be relative.  If false, the import will be
             *                                absolute.
             */
            RootImport(const QString& invalidFilename, bool isRelativeImport = true);

            /**
             * Copy constructor.
             *
             * \param[in] other Instance to be copied.
             */
            RootImport(const RootImport& other);

            ~RootImport();

            /**
             * Method that indicates if the root element is valid or invalid.
             *
             * \return Returns true if the root element is valid.  Returns false if the root element is invalid.
             */
            bool isValid() const;

            /**
             * Method that indicates if the root element is invalid or valid.
             *
             * \return Returns true if the root element is invalid.  Returns false if the root element is valid.
             */
            bool isInvalid() const;

            /**
             * Method that sets the shared pointer to the root element.  This method also marks this root import as
             * valid.
             *
             * \param[in] newRootElement The root element to assign to this import.
             */
            void setRootElement(QSharedPointer<RootElement> newRootElement);

            /**
             * Method that marks this import as invalid and sets a filename to associate with this root element.
             * Calling this method marks the root import as invalid and removes any existing import to a root element.
             *
             * \param[in] newInvalidFilename The filename of the root element that should have been loaded.
             */
            void setInvalidFilename(const QString& newInvalidFilename);

            /**
             * Method that returns a pointer to the root element.
             *
             * \return Returns a pointer to the associated root element.
             */
            QSharedPointer<RootElement> rootElement() const;

            /**
             * Method you can use to determine if this root element has an associated filename.
             *
             * \return Returns true if the root element exists on the filesystem.  Returns false if the root element
             *         resides only in local system memory.
             */
            bool hasFilename() const;

            /**
             * Method you can use to indicate if the import is relative or absolute.
             *
             * \param[in] nowRelative If true, the import will now be a relative import.  If false, the import will now
             *                        be an absolute import.
             */
            void setRelativeImport(bool nowRelative = true);

            /**
             * Method you can use to indicate if the import is absolute or relative.
             *
             * \param[in] nowAbsolute If true, the import will now be an absolute import.  If false, the import will
             *                        now be a relative import.
             */
            void setAbsoluteImport(bool nowAbsolute = true);

            /**
             * Method you can use to determine if this is a relative import.
             *
             * \return Returns true if this is a relative import.  Returns false if this is an absolute import.
             */
            bool isRelativeImport() const;

            /**
             * Method you can use to determine if this is an absolute import.
             *
             * \return Returns true if this is an absolute import.  Returns false if this is a relative import.
             */
            bool isAbsoluteImport() const;

            /**
             * Returns the absolute path to the root element.
             *
             * \return Returns the absolute path to the root element.  If the root element does not exist on the
             *         file system, the method returns the root element's short form name.
             */
            QString absolutePath() const;

            /**
             * Returns the relative path to the root element.
             *
             * \param[in] importPath The import filepath to use to calculate the relative path from.
             *
             * \return Returns the relative path to the root element.  If the root element does not exist on the
             *         file system, the method returns the root element's short form name.  Also note that, if the
             *         relative path is an empty string the method will return the absolute path.
             */
            QString relativePath(const QString& importPath) const;

            /**
             * Returns the path, relative or absolute, to the root element.
             *
             * \param[in] importPath The import filepath to use to calculate relative paths from.  The value may
             *                          not be used in all cases.
             *
             * \return Returns the path to the root element.  If the root element does not exist on the file system,
             *         the method returns the root element's short form name.
             */
            QString path(const QString& importPath) const;

            /**
             * Assignment operator.
             *
             * \param[in] other The other instance to be copied.
             *
             * \return Returns a import to this object.
             */
            RootImport& operator=(const RootImport& other);

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the instances are equal.  Returns false if the instances are different.
             */
            bool operator==(const RootImport& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the instances are different.  Returns false if the instances are identical.
             */
            bool operator!=(const RootImport& other) const;

        private:
            /**
             * Pointer to the associated root element.
             */
            QSharedPointer<RootElement> currentRootElement;

            /**
             * String holding the name of the invalid file.  This value is only used if currentRootElement is null.
             */
            QString currentInvalidFilename;

            /**
             * Flag used to indicate if a relative or absolute path should be used.
             */
            bool useRelativePath;
    };
};

#endif
