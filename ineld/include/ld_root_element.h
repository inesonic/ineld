/*-*-c++-*-************************************************************************************************************
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
* This header defines the \ref Ld::RootElement class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_ROOT_ELEMENT_H
#define LD_ROOT_ELEMENT_H

#include <QtGlobal>
#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QSharedPointer>
#include <QWeakPointer>
#include <QString>
#include <QMap>
#include <QList>
#include <QSet>
#include <QSharedPointer>
#include <QBitArray>
#include <QByteArray>

#include <model_rng.h>

#include "ld_common.h"
#include "ld_handle.h"
#include "ld_element_structures.h"
#include "ld_data_type.h"
#include "ld_calculated_value.h"
#include "ld_page_format.h"
#include "ld_plug_in_information.h"
#include "ld_payload_data.h"
#include "ld_program_file.h"
#include "ld_format_organizer.h"
#include "ld_xml_writer.h"
#include "ld_root_import.h"
#include "ld_identifier_database.h"
#include "ld_operation_database.h"
#include "ld_capabilities.h"
#include "ld_document_settings.h"
#include "ld_element_with_positional_children.h"

namespace Ld {
    class RootVisual;
    class PageFormat;
    class CharacterFormat;
    class BraceConditionalFormat;

    /**
     * Base class for an element that can be placed at the top of the \ref Ld::Element tree.  The class includes a
     * number of critical additional features, including:
     *
     *     * The ability to lookup \ref Element instances by handle, including this root element.
     *     * The ability to perform code generation.
     *     * Tracking of other root elements that import this root element.
     *     * Ability to import other root elements.
     *
     * The \ref RootElement class is intended to be used as the top level container for a specific program.
     */
    class LD_PUBLIC_API RootElement:public Ld::ElementWithPositionalChildren {
        Q_DECLARE_TR_FUNCTIONS(Ld::RootElement)

        public:
            /**
             * Type holding a list of root element.
             */
            typedef QList<QSharedPointer<RootElement>> RootElementList;

            /**
             * Type holding an unordered set of root element.
             */
            typedef QSet<QSharedPointer<RootElement>> RootElementSet;

            /**
             * Type indicating the open-mode for a root element.  A root element must be opened before being used and
             * must be closed prior to being destroyed.
             */
            typedef ProgramFile::OpenMode OpenMode;

            /**
             * Type you can use to iterate through a list of elements.  The type maps to a map or hash with the element
             * handle as the key and a pointer to the element as the handle.
             */
            typedef QMap<Handle, ElementWeakPointer>::const_iterator ElementIterator;

            /**
             * Type used to represent the type of RNG to be used.
             */
            typedef Model::Rng::RngType RngType;

            /**
             * Type used to represent an initial seed used by the RNG.
             */
            typedef Model::Rng::RngSeed RngSeed;

            /**
             * The typename for the root element.
             */
            static const QString elementName;

            /**
             * The maximum default length to allow for a shortform name.
             */
            static constexpr unsigned maximumShortformNameLength = 30;

            /**
             * Static value indicating the capabilities provides by this element.
             */
            static const Capabilities childProvides;

            /**
             * The application default RNG type.
             */
            static constexpr RngType defaultRngType = RngType::MT19937;

            RootElement();

            ~RootElement() override;

            /**
             * Factory method that can be used to create a default instance of a root element.
             *
             * \param[in] typeName The name used to reference the creator function.
             *
             * \return Returns a newly created, default, root element instance.
             */
            static Element* creator(const QString& typeName);

            /**
             * Method you should call to register a root element into the list of known root elements.
             *
             * \param[in] newRootElement The root element to be registered.
             */
            static void registerRootElement(QSharedPointer<RootElement> newRootElement);

            /**
             * Method you should call to unregister a root element from the list of known root elements.
             *
             * \param[in] rootElement The root element to be unregistered.
             */
            static void unregisterRootElement(QSharedPointer<RootElement> rootElement);

            /**
             * Method that returns the type name associated with this element.
             *
             * \return Returns the typename associated with this element.  The value must match the type used to invoke
             *         the static \ref Element::create and \ref Element::registerCreator methods.  This method will
             *         return the string "root" to indicate that this is the program root element.
             */
            QString typeName() const override;

            /**
             * Method that returns the name of the plug-in that provides this element.
             *
             * \return Returns an empty string to indicate that this element is not associated with a plug-in.
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
             * \return Returns the value type this element would represent.  A value of ValueType::NONE indicates
             *         either that the element does not return a value or the value type could not be determined at
             *         this time.
             */
            DataType::ValueType valueType() const override;

            /**
             * Method you can call to obtain a list of all the plug-ins required by child elements.
             *
             * \return Returns a list of plug-in names.
             */
            QSet<QString> requiredPlugIns() const;

            /**
             * Method you can call to determine the requirements imposed by this element on a specific child element.
             * Note that no testing is performed by elements to confirm that requirements are met.  The information is
             * expected to be used by the user interface.
             *
             * \param[in] childIndex The zero based child index.
             *
             * \return Returns the requirements imposed by this parent on the child element.
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
             * Method you can call to obtain a list of all the plug-ins required by child elements.
             *
             * \param[in] allPlugInsByName A mapping, by name, of every loaded plug-in.
             *
             * \param[in] ok               Optional pointer to a boolean value.  Will returns true on success, will
             *                             return false if an error is detected.
             *
             * \return Returns a map of plug-ins used by this program.
             */
            PlugInsByName requiredPlugIns(const PlugInsByName& allPlugInsByName, bool* ok = nullptr) const;

            /**
             * Method you can use to obtain a reference to a document settings instance used to track settings for
             * this root instance.
             *
             * \return Returns a reference to an underlying document settings instance.
             */
            DocumentSettings& documentSettings();

            /**
             * Method you can use to obtain a reference to a document settings instance used to track settings for
             * this root instance.
             *
             * \return Returns a reference to an underlying document settings instance.
             */
            const DocumentSettings& documentSettings() const;

            /**
             * Method you can use to obtain the current identifier database tied to this root instance.
             *
             * \return Returns a reference to the underlying identifier database.
             */
            IdentifierDatabase& identifierDatabase();

            /**
             * Method you can use to obtain the current identifier database tied to this root instance.
             *
             * \return Returns a constant reference to the underlying identifier database.
             */
            const IdentifierDatabase& identifierDatabase() const;

            /**
             * Method you can use to obtain the current operation database tied to this root instance.
             *
             * \return Returns a reference to the underlying operation database.
             */
            OperationDatabase& operationDatabase();

            /**
             * Method you can use to obtain the current operation database tied to this root instance.
             *
             * \return Returns a constant reference to the underlying operation database.
             */
            const OperationDatabase& operationDatabase() const;

            /**
             * Method you can call to generate a \ref Ld::FormatOrganizer instance initialized to provide data on all
             * the formats in this program.
             *
             * \return Returns a shared pointer to a configured instance of a \ref Ld::FormatOrganizer for this program.
             */
            QSharedPointer<FormatOrganizer> formatOrganizer() const;

            /**
             * Method you can use to change the application default page format.  The this value is used as the default
             * page format for all newly created root elements.
             *
             * \param[in] newDefaultPageFormat The new default page format.
             */
            static void setApplicationDefaultPageFormat(QSharedPointer<PageFormat> newDefaultPageFormat);

            /**
             * Method you can use to change the application default page format.  The this value is used as the default
             * page format for all newly created root elements.
             *
             * \param[in] newDefaultPageFormat The new default page format.  Note that the class will take ownership of
             *                                 the pointer.
             */
            static void setApplicationDefaultPageFormat(PageFormat* newDefaultPageFormat);

            /**
             * Method you can use to obtain the application default page format.
             *
             * \return Returns the application default page format.
             */
            static QSharedPointer<PageFormat> applicationDefaultPageFormat();

            /**
             * Method you can use to set the application default RNG type.  The specified value will be used by new
             * root elements as the initial RNG type.
             *
             * \param[in] newRngType The newly desired RNG type.
             */
            static void setApplicationDefaultRngType(RngType newRngType);

            /**
             * Method you can use to obtain the current application default RNG type.  The specified value will be used
             * by new root elements as the initial RNG type.
             *
             * \return Returns the current application default RNG type.
             */
            static RngType applicationDefaultRngType();

            /**
             * Method you can use to change the default page format.  The page format will be changed for all newly
             * added pages and, optionally, on all pages employing this format.
             *
             * \param[in] newDefaultPageFormat The new default page format.
             *
             * \param[in] updateExisting       If true, the page format will be updated for all existing pages.  If
             *                                 false, the page format will only be applied to newly added pages.
             */
            void setDefaultPageFormat(QSharedPointer<PageFormat> newDefaultPageFormat, bool updateExisting = false);

            /**
             * Method you can use to change the default page format.  The page format will be changed for all newly
             * added pages and, optionally, on all pages employing this format.
             *
             * \param[in] newDefaultPageFormat The new default page format.  Note that the new format will be assigned
             *                                 to a shared pointer internally which will manage ownership.
             *
             * \param[in] updateExisting       If true, the page format will be updated for all existing pages.  If
             *                                 false, the page format will only be applied to newly added pages.
             */
            void setDefaultPageFormat(PageFormat* newDefaultPageFormat, bool updateExisting = false);

            /**
             * Method you can use to obtain the default page format.
             *
             * \return Returns the default page format.
             */
            QSharedPointer<PageFormat> defaultPageFormat() const;

            /**
             * Method you can use to change the default text format.
             *
             * \param[in] newTextFormat The new text format.
             */
            void setDefaultTextFormat(QSharedPointer<CharacterFormat> newTextFormat);

            /**
             * Method you can use to obtain the default text format.
             *
             * \return Returns the current document global default text format.
             */
            QSharedPointer<CharacterFormat> defaultTextFormat() const;

            /**
             * Method you can use to change the default math text format.
             *
             * \param[in] newMathTextFormat The new math format.
             */
            void setDefaultMathTextFormat(QSharedPointer<CharacterFormat> newMathTextFormat);

            /**
             * Method you can use to obtain the default math text format.
             *
             * \return Returns the current document global default math text format.
             */
            QSharedPointer<CharacterFormat> defaultMathTextFormat() const;

            /**
             * Method you can use to change the default math identifier format.
             *
             * \param[in] newMathIdentifierFormat The new math identifier format.
             */
            void setDefaultMathIdentifierFormat(QSharedPointer<CharacterFormat> newMathIdentifierFormat);

            /**
             * Method you can use to obtain the default math identifier format.
             *
             * \return Returns the current document global default math identifier format.
             */
            QSharedPointer<CharacterFormat> defaultMathIdentifierFormat() const;

            /**
             * Method you can use to change the default math function format.
             *
             * \param[in] newMathFunctionFormat The new math function format.
             */
            void setDefaultMathFunctionFormat(QSharedPointer<CharacterFormat> newMathFunctionFormat);

            /**
             * Method you can use to obtain the default math function format.
             *
             * \return Returns the current document global default math function format.
             */
            QSharedPointer<CharacterFormat> defaultMathFunctionFormat() const;

            /**
             * Method you can use to change the default brace conditional format.
             *
             * \param[in] newBraceConditionalFormat The new brace conditional format.
             */
            void setDefaultBraceConditionalFormat(QSharedPointer<BraceConditionalFormat> newBraceConditionalFormat);

            /**
             * Method you can use to obtain the default brace conditional format.
             *
             * \return Returns the current document global brace conditional format.
             */
            QSharedPointer<BraceConditionalFormat> defaultBraceConditionalFormat() const;

            /**
             * Method you can use to change whether the default brace conditional style should show an "else" clause.
             *
             * \param[in] nowShowExplicitElseClause If true, the else clause should be included.  If false, the else
             *                                      clause should be excluded.
             */
            void setDefaultBraceConditionalElseClauseShown(bool nowShowExplicitElseClause);

            /**
             * Method you can use to determine whether the default brace conditional style should show an "else"
             * clause.
             *
             * \return Returns true if the else clause should be included.  If false, the else clause should be
             *         excluded.
             */
            bool defaultBraceConditionalElseClauseShown() const;

            /**
             * Method you can use to specify the page format to use for a given page.
             *
             * \param[in] pageNumber The zero-based page number of the page to be updated.  Note that this value also
             *                       implicitly updates the number of pages if the value is equal to or greather than
             *                       the current number of pages.
             *
             * \param[in] newPageFormat The page format to assign to this page.  Note that the page format will be
             *                          compared against other pages and a different pointer may, in fact, be used if
             *                          it matches this value.
             *
             * \return Returns the pointer actually assigned to this page.  This pointer may, or may not, refer to
             *         another object containing identical values.
             */
            QSharedPointer<PageFormat> setPageFormat(
                unsigned long              pageNumber,
                QSharedPointer<PageFormat> newPageFormat
            );

            /**
             * Method you can use to obtain the format to apply to a specific page.
             *
             * \param[in] pageNumber The zero-based page number of the desired page.  Note that this value also
             *                       implicitly updates the number of pages if the value is equal to or greater than
             *                       the current number of pages.
             *
             * \return Returns the format that should be used for this page.
             */
            const QSharedPointer<PageFormat> pageFormat(unsigned long pageNumber);

            /**
             * Method you can use to specify the current number of pages.
             *
             * \param[in] newNumberPages The new number of pages in the document.
             */
            void setNumberPages(unsigned long newNumberPages);

            /**
             * Method you can use to obtain the last reported number of pages.
             *
             * \return Returns the last reported number of pages.
             */
            unsigned long numberPages() const;

            /**
             * Method you can use to set the document default RNG type.
             *
             * \param[in] newRngType The newly desired RNG type.
             */
            void setRngType(RngType newRngType);

            /**
             * Method you can use to obtain the document default RNG type.
             *
             * \return Returns the desired RNG type.
             */
            RngType rngType() const;

            /**
             * Method you can use to set the last used or preset RNG seed.
             *
             * \param[in] newRngSeed The newly desired RNG seed.
             */
            void setRngSeed(const RngSeed& newRngSeed);

            /**
             * Method you can use to obtain the last used or preset RNG seed.
             *
             * \return Returns the last used or preset RNG seed.
             */
            const RngSeed& rngSeed() const;

            /**
             * Method you can use to indicate if the preset RNG seed should be used in subsequent runs.
             *
             * \param[in] useSeedAsPreset If true, the seed will be used as a preset.  If false, the seed will not be
             *                            used, only updated.
             */
            void setRngSeedIsPreset(bool useSeedAsPreset);

            /**
             * Method you can use to determine if the preset RNG seed should be used for subsequent runs.
             *
             * \return Returns true if the seed should be used as a preset.  Returns false if the seed should *not*
             *         be used as a preset.
             */
            bool rngSeedIsPreset() const;

            /**
             * Method you can use to change the object used to manage visual representation of this element.  Note that
             * using this method will also cause the \ref RootVisual::element method to point back to this root element
             * instance.
             *
             * If the newly added \ref RootVisual already points to a different element, then those connections will be
             * broken in order to maintain a 1:1 relationship between any \ref Visual instance and \ref Element
             * instance.
             *
             * If this element currently has an associated visual, then that visual will disconnected from this
             * element and will be deleted.
             *
             * \param[in] newVisual A pointer to the new visual object.
             */
            void setVisual(RootVisual* newVisual);

            /**
             * Method you can use to obtain the visual representation of this element.
             *
             * \return Returns a pointer to the visual representation of this element.  A null pointer is returned if
             *         no visual representation has been defined.
             */
            RootVisual* visual() const;

            /**
             * Method you can call to detemrine if this element provides a logical stop for function parenthesis.  If
             * true, functions will act as if they're top level.  If false, functions will act as if they're within a
             * larger equation and will always place parenthesis around parameters.
             *
             * \param[in] functionChildIndex The index of the function element being queried.
             *
             * \return Returns true if the function can assume it's at a logical stop in the equation such as the last
             *         position in an equation or at a position, such as directly under a matrix, that can be treated
             *         as the last position.  Returns false in all other cases.  This version always returns true.
             */
            bool isFunctionLogicalStop(unsigned long functionChildIndex) const override;

            /**
             * Method that can be used to determine if the root element is empty.  An empty root element is expected
             * to have either no content or contain only a single paragraph with an empty text element.
             *
             * \return Returns true if the document is empty.  Returns false if the document has content.
             */
            bool isEmpty() const;

            /**
             * Method that can be used to determine if the document is not empty.
             *
             * \return Returns true if the document has content.  Returns false if the document is empty.
             */
            bool isNotEmpty() const;

            /**
             * Method that indicates if there have been changes under this root node since it was last saved.
             *
             * \return Returns true if changed have been made since the program was last saved.  Returns false if
             *         the program is pristine.
             */
            bool isModified() const;

            /**
             * Method that indicates if no changes have been made under this root node since the contents were last
             * saved.
             *
             * \return Returns true if no changes have been made since the program was last saved.  Returns false if any
             *         changes have been made.
             */
            bool isPristine() const;

            /**
             * Method that can be called to determine if this root import can be deleted.  The method will return
             * true if:
             *
             *     * No other root element imports this root element.
             *     * and either:
             *         - There is no associated root visual
             *         - or the root visual's \ref Ld::RootVisual::canDeleteRootElement mehtod returns true.
             *
             * \return Returns true if this root element can be deleted.  Returns false if this root element should not
             *         be deleted.
             */
            bool canDelete() const;

            /**
             * Method you can call to open a program file into this root element for reading and, optionally, writing.
             * All elements under this root element will be removed and then replaced with new data based on the
             * contents of the specified file.
             *
             * \param[in] filename      The name of the file to be read.
             *
             * \param[in] readOnly      If true, the file will be opened read-only.  If false, the file will be opened
             *                          in read/write mode.
             *
             * \param[in] plugInsByName A map holding plug-in information, indexed by plug-in name.
             *
             * \return Returns true on success, returns false on error.
             */
            bool openExisting(const QString& filename, bool readOnly, const PlugInsByName& plugInsByName);

            /**
             * Method you can call to create a new program file for reading and writing.  The new file will be opened
             * in read/write mode.  The method will fail if root element is already open.
             *
             * Note that the newly created root element will contain one \ref ParagraphElement instance containing a
             * single empty \ref TextElement instance.
             *
             * \return Returns true on success, returns false on error.
             */
            bool openNew();

            /**
             * Method you can use to save the program state.  The method will fail if the container was opened
             * read-only.
             *
             * \param[in] purgeOnSave If true, payloads that are no longer referened by the program will be purged from
             *                        the file.
             *
             * \return Returns true on success, returns false on error.
             */
            bool save(bool purgeOnSave = true);

            /**
             * Method you can use to save the program state under a new filename.
             *
             * \param[in] newFilename The new name to associate with this program.
             *
             * \param[in] purgeOnSave If true, payloads that are no longer referened by the program will be purged from
             *                        the file.
             *
             * \return Returns the name of the program file.
             */
            bool saveAs(const QString& newFilename, bool purgeOnSave = true);

            /**
             * Method you can use to close the working program file behind this root node.  The contents of the root
             * element will not be saved.
             *
             * \return Returns true on success, returns false on error.
             */
            bool close();

            /**
             * Method you can call to get the name fo the currently open file.
             *
             * \return Returns the name of the currently open container.
             */
            QString filename() const;

            /**
             * Determines if the root element has been assigned a filename.
             *
             * \return Returns true if the root element has a filename.  Returns false if the root element does not have
             *         a filename.
             */
            bool hasFilename() const;

            /**
             * Method you can use to obtain a payload instance by payload ID.
             *
             * \param[in] payloadId The ID of the desired payload.
             *
             * \return Returns the requested payload data.  An invalid payload will be returned on error.
             */
            PayloadData payload(PayloadData::PayloadId payloadId);

            /**
             * Method you can use to obtain a new-pristine payload instance.
             *
             * \return Returns a new and pristine payload instance.
             */
            PayloadData newPayload();

            /**
             * Method you can use to clone a payload into this program file.  When this method is called, a copy of the
             * payload indicated by a provided \ref Ld::PayloadData instance will be created and stored in this
             * container.
             *
             * \param[in] payloadData The payload data to be cloned.
             *
             * \return Returns a new \ref Ld::PayloadData instance containing the new clone stored in this program
             *         file.
             */
            PayloadData clonePayload(PayloadData& payloadData);

            /**
             * Method you can use to determine how the root element was opened.
             *
             * \return Returns the current open mode.
             */
            OpenMode openMode() const;

            /**
             * Method you can use to obtain a description of the last reported error.
             *
             * \return A string description of the last reported error.
             */
            QString errorString() const;

            /**
             * Determines the short form name of this root element to present on tabs and other user interface items.
             *
             * \return If the root element has a filename, this method will return the filename, possibly elided to fit
             *         within a maximum number of characters.  If the root element does not have a filename, a name
             *         suitable for use in a GUI will be returned.
             */
            QString shortformName(unsigned maximumLength = maximumShortformNameLength) const;

            /**
             * Calculates a unique identifier for this document based on the document's filename or the document's
             * current default name.
             *
             * \return Returns a unique identifier name for the document.
             */
            QString identifier() const;

            /**
             * Determines if this document is a new document.
             *
             * \return Returns true if this root element has never been saved to disk.  Returns false if this root
             *         element has been saved to disk.
             */
            bool isNew() const;

            /**
             * Method you can use to quickly look up an element by handle.
             *
             * \param[in] handle The handle of the desired element.
             *
             * \return Returns a pointer to the desired element.  A null pointer is returned if the desired element
             *         does not exist under this root.
             */
            ElementPointer element(const Handle& handle) const;

            /**
             * Method you can use to specify a list of import root elements.  In addition to updating imports,
             * this method will also update backward imports (imported by) on all root elements that this root node
             * imports, indicating that those programs are imported by this program.  Links to root elements that
             * are no longer imported are broken.
             *
             * \param[in] newImports A list of imports this program should adopt.
             */
            void setImports(const QList<RootImport>& newImports);

            /**
             * Method you can use to obtain an ordered list of program this program should import.
             *
             * \return Returns a list of imports made by this program.
             */
            QList<RootImport> imports() const;

            /**
             * Method you can use to obtain an ordered list of programs this program depends on.  This method differs
             * from \ref Ld::RootElement::imports in that is also includes program imports indirectly by this program.
             *
             * Imports are returned in the order they should be processed.
             */
            RootElementList allDependencies() const;

            /**
             * Method you can use to identify all the programs that directly import this program.
             *
             * \return Returns a list of all the root elements that import this program.  Note that the returned
             *         order is arbitrary.
             */
            RootElementList importedBy() const;

            /**
             * Method you can use to identify all the programs that directly or indirectly import this program.
             *
             * \return Returns a list of all the root elements that import this program.  Note that the returned
             *         order is arbitrary.
             */
            RootElementSet indirectlyImportedBy() const;

            /**
             * Method that is called when the document becomes prisine.
             */
            void markPristine();

            /**
             * Method that is called when the document becomes modified.
             */
            void markModified();

            /**
             * Method you can use to obtain an iterator to the first element under this root.
             *
             * \return Returns a constant iterator to the first element in the list.
             */
            ElementIterator elementBegin() const;

            /**
             * Method you can use to obtain an iterator past the last element under this root.
             *
             * \return Returns a constant iterator past the last element in this list under this root.
             */
            ElementIterator elementEnd() const;

            /**
             * Method you can use to obtain a calculated value for an element.  Placed in the root element and made
             * virtual so that selections can modify how calculated values are obtained by the code generators.  The
             * default implementation calls Ld::Element::calculatedValue to obtain its values.
             *
             * \param[in] element    The element to obtain the calculated value for.
             *
             * \param[in] valueIndex The zero based index of the value to obtain.
             *
             * \return Returns the reported calculated value.
             */
            virtual CalculatedValue elementCalculatedValue(ElementPointer element, unsigned valueIndex) const;

            /**
             * Method you can use to obtain an image for an element.  Placed in the root element and made virtual so
             * that selections can modify this mechanism.  The default implementation calls the
             * \ref Ld::Element::exportImage method of the provided element.
             *
             * \param[in] element The element to obtain the image for.
             *
             * \param[in] dpi     The desired image resolution in DPI.  The DPI setting is intended to be used as a
             *                    hint and may not be honored.
             *
             * \return Returns a byte array holding the desired image.
             */
            virtual QByteArray exportElementImage(ElementPointer element, float dpi) const;

            /**
             * Calculates a unique identifier for the document that would be associated with a specified file.
             *
             * \param[in] filename The filename that would be used to load/save this file.
             *
             * \return Returns a unique identifier that will apply to the document, once opened.
             */
            static QString identifier(const QString& filename);

            /**
             * Method that obtains a list of all loaded root elements.
             *
             * \return Returns a list of identifiers for all loaded root elements.
             */
            static RootElementList rootElements();

            /**
             * Method that obtains a list of all the identifiers for loaded root elements.
             *
             * \return Returns a list of identifiers for all loaded root elements.
             */
            static QList<QString> identifiers();

            /**
             * Method that obtains a root element by identifier.
             *
             * \param[in] identifier The identifier used to reference the desired root element.
             *
             * \return Returns the requested root element.  A null pointer is returned if the requested root element
             *         does not exist.
             */
            static QSharedPointer<RootElement> byIdentifier(const QString& identifier);

            /**
             * Method you can use to convert an RNG type to a string.
             *
             * \param[in] rngType The RNG type to convert to a string.
             *
             * \return Returns the RNG type as a string.
             */
            static QString toString(RngType rngType);

            /**
             * Method you can use to convert a string to an RNG type.
             *
             * \param[in]     s  The string to be converted.
             *
             * \param[in,out] ok An optional pointer to a boolean type.  Holds true on exit if the string was valid.
             *
             * \return Returns the translated RNG type.
             */
            static RngType toRngType(const QString& s, bool* ok = nullptr);

        protected:
            /**
             * Method that receives notification from the elements under this root element when a new element is added.
             *
             * \param[in] descendantElement The descendant element that was added to the tree.
             */
            void descendantAdded(ElementPointer descendantElement) final;

            /**
             * Method that receives notification from the elements under this root element when a new element is
             * removed.
             *
             * \param[in] descendantElement The descendant element that was added to the tree.
             */
            void descendantRemoved(ElementPointer descendantElement) final;

            /**
             * Method that is triggered whenever a descendant's format has been changed.  The default implementation of
             * this method both triggers a call to the visual's \ref Ld::Visual::descendantFormatChanged method and
             * propagates the message upwards in the element tree.
             *
             * If you overload this method, please be sure to call the base class implementation in order to allow for
             * event propagation to occur.
             *
             * \param[in] changedElement The element who's format has been changed.
             *
             * \param[in] newFormat      The new format associated with this element.
             */
            void descendantFormatChanged(ElementPointer changedElement, FormatPointer newFormat) final;

            /**
             * Method that is triggered whenever a descendant's format has been updated.  The default implementation of
             * this method both triggers a call to the visual's \ref Ld::Visual::descendantFormatUpdated method and
             * propagates the message upwards in the element tree.
             *
             * If you overload this method, please be sure to call the base class implementation in order to allow for
             * event propagation to occur.
             *
             * \param[in] changedElement The element who's format has been changed.
             *
             * \param[in] newFormat      The updated format associated with this element.
             */
            void descendantFormatUpdated(ElementPointer changedElement, FormatPointer newFormat) final;

            /**
             * Method that is called by any child element when a change occurs.
             *
             * \param[in] changedChild Pointer to the child element that changed.
             */
            void childChanged(ElementPointer changedChild) final;

            /**
             * Method you can overload to exclude this root element from the list of user visible root elements.
             *
             * \return Returns true if this root element should be excluded.  Returns false if this root element should
             *         be included.  The default implementation returns false.
             */
            virtual bool excludeFromUserVisibleRoots() const;

        private:
            /**
             * Type used to track pages with non-default page formats.
             */
            typedef QMap<unsigned long, QSharedPointer<PageFormat>> PageFormatMap;

            /**
             * Type used to track elements by handle.
             */
            typedef QMap<Handle, ElementWeakPointer> ElementMap;

            /**
             * Type used to track the root elements managed by the program.
             */
            typedef QMap<QString, QSharedPointer<RootElement>> GlobalRootElementMap;

            /**
             * Value used to indicate an invalid document number.
             */
            static constexpr unsigned invalidDocumentNumber = static_cast<unsigned>(-1);

            /**
             * Value used to indicate an unassigned document number.
             */
            static constexpr unsigned unassignedDocumentNumber = static_cast<unsigned>(-2);

            /**
             * Value used to indicate if the generated XML should be pretty-printed/formatted.  Useful for debugging.
             * Setting to false will reduce the program file sizes slightly.
             */
            static constexpr bool formatXml = true;

            /**
             * Value holding the doctypedecl to insert into the XML description.  The value will also be checked against
             * this value when the XML description is read.
             */
            static const QString doctypedecl;

            /**
             * Value used to indicate the major version number of the XML description.  The major version number should
             * be bumped when the XML description becomes incompatible with an older version, thus requiring a distinct
             * XML parsing engine.
             */
            static constexpr unsigned xmlMajorVersion = 1;

            /**
             * Value used to indicate the minor version number of the XML description.
             */
            static constexpr unsigned xmlMinorVersion = 0;

            /**
             * Method that is called recursively to build our ordered dependency list.
             *
             * \param[in]     rootElement    The root element to be processed.
             *
             * \param[in,out] dependencyList The ordered list of dependencies.
             *
             * \param[in,out] dependencySet  The unordered set of dependencies added to the dependency list.  This
             *                               set is used to more quickly determine if an entry has been added to the
             *                               dependency list.
             *
             * \param[in,out] pendingSet     An unordered set of dependencies that are have not yet added but will be
             *                               adding upon completion of this operation.  This set is used to properly
             *                               order the entries.
             */
            static void buildDependencyList(
                QSharedPointer<RootElement> rootElement,
                RootElementList&            dependencyList,
                RootElementSet&             dependencySet,
                RootElementSet&             pendingSet
            );

            /**
             * Method that is called to write the program's XML description.
             *
             * \param[in] filePath The path to the file used as a reference location for relative imports.
             *
             * \return Returns true on success, returns false if an error is detected.  This method is expected to
             *         update the error string and perform any necessary clean-up related to the XML description if an
             *         error is detected.
             */
            bool writeXmlDescription(const QString& filePath);

            /**
             * Method that is called to read the program's XML description and build the element tree from the root
             * element downward.
             *
             * \param[in]  filePath      The path to the file used as a reference location for relative imports.
             *
             * \param[in]  plugInsByName A map of plug=ins referenced by name.  Used to validate that the program being
             *                           loaded can be supported.
             *
             * \param[out] majorVersion  The major version number of the XML description.
             *
             * \param[out] minorVersion  The minor version number of the XML description.
             *
             * \return Returns true on success, returns false if an error is detected.  This method is expected to
             *         update the error string and perform any necessary clean-up related to the XML description if an
             *         error is detected.
             */
            bool readXmlDescription(
                const QString&       filePath,
                const PlugInsByName& plugInsByName,
                unsigned&            majorVersion,
                unsigned&            minorVersion
            );

            /**
             * Method that is called to write the XML requires section of the XML description.
             *
             * \param[in] writer A shared pointer to the XML writer.
             */
            void writeRequiresSection(QSharedPointer<XmlWriter> writer);

            /**
             * Method that is called to write the XML imports section of the XML description.
             *
             * \param[in] writer   A shared pointer to the XML writer.
             *
             * \param[in] filePath The path to the file used as a reference location for relative imports.
             */
            void writeImportsSection(QSharedPointer<XmlWriter> writer, const QString& filePath);

            /**
             * Method that is called to write the format information into the XML description.
             *
             * \param[in] writer  A shared pointer to the XML writer.
             *
             * \param[in] formats A list of formats, by format identifier.
             */
            void writeFormats(QSharedPointer<XmlWriter> writer, QSharedPointer<FormatOrganizer> formats);

            /**
             * Method that is called to write page format information into the XML description.
             *
             * \param[in] writer            A shared pointer to the XML writer.
             *
             * \param[in] formats           A list of formats, by format identifier.
             *
             * \param[in] defaultPageFormat The default page format.
             *
             * \param[in] pageFormats       A map of page formats, by page number.
             */
            void writePageData(
                QSharedPointer<XmlWriter>       writer,
                QSharedPointer<FormatOrganizer> formats,
                QSharedPointer<PageFormat>      defaultPageFormat,
                const PageFormatMap&            pageFormats
            );

            /**
             * Method that is called to writen the document settings section into the XML description.
             *
             * \param[in] writer           A shared pointer to the XML writer.
             *
             * \param[in] documentSettings The document settings instance to be written.
             */
            void writeDocumentSettings(QSharedPointer<XmlWriter> writer, DocumentSettings& documentSettings);

            /**
             * Method that is called to read and parse the XML program tag and every child tag.  Errors can be reported
             * directly to the reader using the raiseError method.
             *
             * \param[in] reader        A shared pointer to the XML reader.
             *
             * \param[in] filePath      The path to the file used as a reference location for relative imports.
             *
             * \param[in] plugInsByName A map of plug=ins referenced by name.  Used to validate that the program being
             *                          loaded can be supported.
             *
             * \param[in] majorVersion  The major version number for this XML description.
             */
            void readProgramSection(
                QSharedPointer<XmlReader> reader,
                const QString&            filePath,
                const PlugInsByName&      plugInsByName,
                unsigned                  majorVersion
            );

            /**
             * Method that reads and parses the requires section of the XML description.  Errors can be reported
             * directly to the reader using the raiseError method.
             *
             * \param[in] reader        A shared pointer to the XML reader.
             *
             * \param[in] plugInsByName A map of plug-ins, by plug-in name, used to validate that the program being
             *                          loaded can be supported.
             *
             * \param[in] majorVersion  The major version number for this XML description.
             */
            void readRequiresSection(
                QSharedPointer<XmlReader> reader,
                const PlugInsByName&      plugInsByName,
                unsigned                  majorVersion
            );

            /**
             * Method that reads the imports section of the XML description.  Errors can be reported directly to the
             * reader using the raiseError method.
             *
             * \param[in] reader        A shared pointer to the XML reader.
             *
             * \param[in] filePath      The path to the program file.  Used to resolve relative imports.
             *
             * \param[in] majorVersion  The major version number for this XML description.
             *
             * \param[in] plugInData    The list of plug-ins.  The list is passed to \ref RootElement::openExisting
             *                          when loading new root elements.
             */
            void readImportsSection(
                QSharedPointer<XmlReader> reader,
                const QString&            filePath,
                unsigned                  majorVersion,
                const PlugInsByName&      plugInData
            );

            /**
             * Method that reads the formats section of the XML description.  Errors can be reported directly to the
             * reader using the raiseError method.
             *
             * \param[in] reader       A shared pointer to the XML reader.
             *
             * \param[in] formats      A map of formats, by identifier.
             *
             * \param[in] majorVersion The major version number for this XML description.
             */
            void readFormats(QSharedPointer<XmlReader> reader, FormatsByIdentifier& formats, unsigned majorVersion);

            /**
             * Method that reads the page data section of the XML description.  Errors can be reported directly to the
             * reader using the raiseError method.
             *
             * \param[in] reader       A shared pointer to the XML reader.
             *
             * \param[in] formats      A map of formats, by identifier.
             *
             * \param[in] majorVersion The major version number for this XML description.
             */
            void readPageData(
                QSharedPointer<XmlReader>  reader,
                const FormatsByIdentifier& formats,
                unsigned                   majorVersion
            );

            /**
             * Method that reads the settings data from the XML description.  Errors are reported directly to the
             * reader using the raiseError method.
             *
             * \param[in] reader           A shared pointer to the XML reader.
             *
             * \param[in] documentSettings A reference to the \ref Ld::DocumentSettings instance to be populated.
             *
             * \param[in] majorVersion     The major version number for this XML description.
             */
            void readDocumentSettings(
                QSharedPointer<XmlReader> reader,
                Ld::DocumentSettings&     documentSettings,
                unsigned                  majorVersion
            );

            /**
             * Method that updates internal document settings.
             *
             * \param[in] formats A list of formats, by format identifier.
             */
            void updateInternalDocumentSettings(QSharedPointer<FormatOrganizer> formats);

            /**
             * Method that updates the root element from internal document settings.
             *
             * \param[in] formats      A map of formats, by identifier.
             *
             * \param[in] majorVersion The major version number for this XML description.
             */
            void updateFromInternalDocumentSettings(const FormatsByIdentifier& formats, unsigned majorVersion);

            /**
             * Method that is called to add a root import to this class instance.  If needed, a new root element will
             * be loaded.
             *
             * \param[in] reader            A shared pointer to the XML reader.  You can use the reader to report
             *                              errors.
             *
             * \param[in] importedFilePath  The path to the imported root element.
             *
             * \param[in] referenceFilePath The file path that is used as a referenced for other relative imports.
             *
             * \param[in] isRelative        If true, the root import is a relative import.  If false, the root import
             *                              is an absolute import.
             *
             * \param[in] plugInData        The list of plug-ins.  The list is passed to \ref RootElement::openExisting
             *                              when loading new root elements.
             */
            void addRootImport(
                QSharedPointer<XmlReader> reader,
                const QString&            importedFilePath,
                const QString&            referenceFilePath,
                bool                      isRelative,
                const PlugInsByName&      plugInData
            );

            /**
             * Finds a unique document number value for a new document.
             *
             * \return Returns a unique document number.
             */
            static unsigned newDocumentNumber();

            /**
             * Free's a previously assigned document number.
             *
             * \param[in] documentNumber The document number of the document to be free'd up.
             */
            static void freeDocumentNumber(unsigned documentNumber);

            /**
             * The program file instance backing this root element/program.
             */
            ProgramFile programFile;

            /**
             * A string holding the last reported file error.
             */
            QString lastError;

            /**
             * Flag that is used to block messages from the root to the visual during complex operations.
             */
            bool blockReporting;

            /**
             * Flag that indicates if a modification was detected.
             */
            bool currentIsModified;

            /**
             * A document number used to provide this document a unique name.  Names will be generated from
             * Document::currentFileInformation if this value is set to Document::invalidDocumentNumber.
             */
            unsigned  currentDocumentNumber;

            /**
             * The current document settings instance.
             */
            DocumentSettings currentDocumentSettings;

            /**
             * The application default page format.
             */
            static QSharedPointer<PageFormat> currentApplicationDefaultPageFormat;

            /**
             * The application default RNG type.
             */
            static RngType currentApplicationDefaultRngType;

            /**
             * The current default page format.
             */
            QSharedPointer<PageFormat> currentDefaultPageFormat;

            /**
             * The current default text format.
             */
            QSharedPointer<CharacterFormat> currentDefaultTextFormat;

            /**
             * The current default math text format.
             */
            QSharedPointer<CharacterFormat> currentDefaultMathTextFormat;

            /**
             * The current default math identifier format.
             */
            QSharedPointer<CharacterFormat> currentDefaultMathIdentifierFormat;

            /**
             * The current default math function format.
             */
            QSharedPointer<CharacterFormat> currentDefaultMathFunctionFormat;

            /**
             * The current default brace conditional format.
             */
            QSharedPointer<BraceConditionalFormat> currentDefaultBraceConditionalFormat;

            /**
             * A map holding all the page formats, by page number, that deviate from the default page format.
             */
            PageFormatMap currentPageFormats;

            /**
             * Value holding the current number of pages.
             */
            unsigned long currentNumberPages;

            /**
             * Map of \ref Ld::Element instances by \ref Ld::Handle
             */
            ElementMap currentElementsByHandle;

            /**
             * Ordered list of other root elements imported by this root element.
             */
            QList<RootImport> currentImports;

            /**
             * A database of identifiers.
             */
            IdentifierDatabase currentIdentifierDatabase;

            /**
             * A database of operations that we can pause execution on.
             */
            OperationDatabase currentOperationDatabase;

            /**
             * Flag indicating the default brace condition else clause.
             */
            bool currentDefaultBraceConditionalElseClauseShown;

            /**
             * The document RNG type.
             */
            RngType currentRngType;

            /**
             * The document preset or last used RNG seed.
             */
            RngSeed currentRngSeed;

            /**
             * Flag indicating if the current RNG seed should be used.
             */
            bool currentUsePresetRngSeed;

            /**
             * List of root elements tracked in the system.
             */
            static GlobalRootElementMap currentRootElements;

            /**
             * A bit array used to create unique names for new documents that haven't been linked to a filename.
             */
            static QBitArray globalDocumentTracker;
    };
};

#endif
