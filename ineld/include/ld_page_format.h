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
* This header defines the \ref Ld::PageFormat class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_PAGE_FORMAT_H
#define LD_PAGE_FORMAT_H

#include <QPageLayout>
#include <QSharedPointer>

#include <util_page_size.h>

#include "ld_common.h"
#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_format.h"

namespace Ld {
    /**
     * Class that tracks information regarding margins and other aspects of the usable page.
     */
    class LD_PUBLIC_API PageFormat:public virtual Format {
        public:
            /**
             * Class you can use to track an aggregation of multiple \ref Ld::PageFormat instances.
             */
            class LD_PUBLIC_API Aggregation:public Format::Aggregation {
                public:
                    Aggregation();

                    ~Aggregation() override;

                    /**
                     * Creator function for this aggregator.
                     *
                     * \param[in] formatName The name of the format to create an aggregator for.
                     *
                     * \return Returns a pointer to the newly created aggregator.
                     */
                    static Format::Aggregation* creator(const QString& formatName);

                    /**
                     * Method that returns the type name of the format tracked by this aggregation.
                     *
                     * \return Returns the format name for formats tied to this aggregation.
                     */
                    QString typeName() const final;

                    /**
                     * Method you can use to clear the aggregation.  Calling the base class instance will clear the
                     * underlying format database.
                     */
                    void clear() final;

                    /**
                     * Virtual method you can call to add a format instance to the aggregation.  Derived class
                     * derived class instances should call this method to add the format to the aggregation.
                     *
                     * \param[in] formatInstance  The format instance to be added.
                     *
                     * \param[in] includeExisting If true, this format will be included in the aggregation even if it
                     *                            has already been accounted for.
                     *
                     * \return Returns true if the format was added to the database.  Returns false if the format is
                     *         already contained in the underlying database.
                     */
                    bool addFormat(FormatPointer formatInstance, bool includeExisting = false) final;

                    /**
                     * Method you can use to obtain a set of every unique page size in the aggregation.
                     *
                     * \return Returns a set of Util::PageSize instances.
                     */
                    QSet<Util::PageSize> pageSizes() const;

                    /**
                     * Method you can use to obtain the current minimum page width.
                     *
                     * \return Returns the minimum page width.  A negative value will be returned if no page formats
                     *         are registered.
                     */
                    float minimumPageWidth() const;

                    /**
                     * Method you can use to obtain the current minimum page height.
                     *
                     * \return Returns the minimum page height.  A negative value will be returned if no page formats
                     *         are registered.
                     */
                    float minimumPageHeight() const;

                    /**
                     * Method you can use to obtain the current minimum page horizontal extent.  This method will use
                     * either the page width or page height depending on the page orientation.
                     *
                     * \return Returns the minimum page horizontal extents.  A negative value will be returned if no
                     *         page formats are registered.
                     */
                    float minimumPageHorizontalExtents() const;

                    /**
                     * Method you can use to obtain the current minimum page vertical extent.  This method will use
                     * either the page width or page height depending on the page orientation.  A negative value will
                     * be returned if no page formats are registered.
                     *
                     * \return Returns the minimum page vertical extents.  A negative value will be returned if no
                     *         page formats are registered.
                     */
                    float minimumPageVerticalExtents() const;

                    /**
                     * Method you can use to obtain a set of every unique left margin value.
                     *
                     * \return Returns a set of unique left margin values.
                     */
                    QSet<float> leftMargins() const;

                    /**
                     * Method you can use to obtain a set of every unique right margin value.
                     *
                     * \return Returns a set of unique right margin values.
                     */
                    QSet<float> rightMargins() const;

                    /**
                     * Method you can use to obtain a set of every unique top margin value.
                     *
                     * \return Returns a set of unique top margin values.
                     */
                    QSet<float> topMargins() const;

                    /**
                     * Method you can use to obtain a set of every unique bottom margin value.
                     *
                     * \return Returns a set of unique bottom margin values.
                     */
                    QSet<float> bottomMargins() const;

                    /**
                     * Method you can use to obtain the maximum left margin value across the aggregation.
                     *
                     * \return Returns the maximum left margin.  A negative value is returned if no page formats have
                     *         been registered.
                     */
                    float maximumLeftMargin() const;

                    /**
                     * Method you can use to obtain the maximum right margin value across the aggregation.
                     *
                     * \return Returns the maximum right margin.  A negative value is returned if no page formats have
                     *         been registered.
                     */
                    float maximumRightMargin() const;

                    /**
                     * Method you can use to obtain the maximum top margin value across the aggregation.
                     *
                     * \return Returns the maximum top margin.  A negative value is returned if no page formats have
                     *         been registered.
                     */
                    float maximumTopMargin() const;

                    /**
                     * Method you can use to obtain the maximum bottom margin value across the aggregation.
                     *
                     * \return Returns the maximum bottom margin.  A negative value is returned if no page formats have
                     *         been registered.
                     */
                    float maximumBottomMargin() const;

                    /**
                     * Method you can use to obtain the maximum left margin value that not cause the left margin to
                     * exceed the current right margin across all page formats in the aggregation.
                     *
                     * \return Returns the maximum allowable left margin.  A negative value is returned if no page
                     *         formats have been registered.
                     */
                    float maximumAllowableLeftMargin() const;

                    /**
                     * Method you can use to obtain the maximum right margin value that not cause the right margin to
                     * exceed the current left margin across all page formats in the aggregation.
                     *
                     * \return Returns the maximum allowable right margin.  A negative value is returned if no page
                     *         formats have been registered.
                     */
                    float maximumAllowableRightMargin() const;

                    /**
                     * Method you can use to obtain the maximum top margin value that not cause the top margin to
                     * exceed the current bottom margin across all page formats in the aggregation.
                     *
                     * \return Returns the maximum allowable top margin.  A negative value is returned if no page
                     *         formats have been registered.
                     */
                    float maximumAllowableTopMargin() const;

                    /**
                     * Method you can use to obtain the maximum bottom margin value  that not cause the bottom margin
                     * to exceed the current top margin across all page formats in the aggregation.
                     *
                     * \return Returns the maximum allowable bottom margin.  A negative value is returned if no page
                     *         formats have been registered.
                     */
                    float maximumAllowableBottomMargin() const;

                    /**
                     * Method you can use to determine if the aggregation of landscape orientation values.
                     *
                     * \return Returns a boolean aggregation of landscape orientation values.
                     */
                    Format::BooleanAggregation landscape() const;

                    /**
                     * Method you can use to determine if the aggregation of portrait orientation values.
                     *
                     * \return Returns a boolean aggregation of portrait orientation values.
                     */
                    Format::BooleanAggregation portrait() const;

                    /**
                     * Comparison operator.  Note that this operator will check the values in the aggregation but not
                     * formats contained in the aggregation.
                     *
                     * \param[in] other The instance to be compared against.
                     *
                     * \return Returns true if the aggregations are equal.  Returns false if the aggregations are
                     *         different.
                     */
                    bool operator==(const Aggregation& other) const;

                    /**
                     * Comparison operator.  Note that this operator will check the values in the aggregation but not
                     * formats contained in the aggregation.
                     *
                     * \param[in] other The instance to be compared against.
                     *
                     * \return Returns true if the aggregations are different.  Returns false if the aggregations are
                     *         equal.
                     */
                    bool operator!=(const Aggregation& other) const;

                private:
                    /**
                     * Set of current pages sizes.
                     */
                    QSet<Util::PageSize> currentPageSizes;

                    /**
                     * The current minimum page width.
                     */
                    float currentMinimumPageWidth;

                    /**
                     * The current minimum page height.
                     */
                    float currentMinimumPageHeight;

                    /**
                     * The current minimum page horizontal extent.
                     */
                    float currentPageMinimumHorizontalExtents;

                    /**
                     * The current minimum page horizontal extent.
                     */
                    float currentPageMinimumVerticalExtents;

                    /**
                     * Set of current left margin values.
                     */
                    QSet<float> currentLeftMargins;

                    /**
                     * Set of current right margin values.
                     */
                    QSet<float> currentRightMargins;

                    /**
                     * Set of current top margin values.
                     */
                    QSet<float> currentTopMargins;

                    /**
                     * Set of current bottom margin values.
                     */
                    QSet<float> currentBottomMargins;

                    /**
                     * The current maximum left margin across all the page formats.
                     */
                    float currentMaximumLeftMargin;

                    /**
                     * The current maximum right margin across all the page formats.
                     */
                    float currentMaximumRightMargin;

                    /**
                     * The current maximum top margin across all the page formats.
                     */
                    float currentMaximumTopMargin;

                    /**
                     * The current maximum bottom margin across all the page formats.
                     */
                    float currentMaximumBottomMargin;

                    /**
                     * The current maximum allowable left margin across all page formats.
                     */
                    float currentMaximumAllowableLeftMargin;

                    /**
                     * The current maximum allowable right margin across all page formats.
                     */
                    float currentMaximumAllowableRightMargin;

                    /**
                     * The current maximum allowable top margin across all page formats.
                     */
                    float currentMaximumAllowableTopMargin;

                    /**
                     * The current maximum allowable bottom margin across all page formats.
                     */
                    float currentMaximumAllowableBottomMargin;

                    /**
                     * Aggregation of landscape values.
                     */
                    Format::BooleanAggregation currentLandscape;

                    /**
                     * Aggregation of portrait values.
                     */
                    Format::BooleanAggregation currentPortrait;
            };

            /**
             * The typename for the format.
             */
            static const QString formatName;

            /**
             * The default page layout to use when creating a page.  The default will be set based on locale.
             */
            static const Ld::PageFormat defaultPageFormat;

            /**
             * The minimum percentage of the page we must keep as active area.
             */
            static constexpr float minimumActiveAreaFraction = 0.9F;

            /**
             * Constructor.  Creates a layout with an invalid page and invalid margin settings.
             */
            PageFormat();

            /**
             * Constructor.
             *
             * \param[in] pageWidthPoints  The page width, in points.
             *
             * \param[in] pageHeightPoints The page height, in points.
             */
            PageFormat(float pageWidthPoints, float pageHeightPoints);

            /**
             * Constructor
             *
             * \param[in] pageTypeName The name used to describe the page.  If the page size is unknown, the created
             *                         page will be marked as invalid.
             */
            PageFormat(const QString& pageTypeName);

            /**
             * Constructor.
             *
             * \param[in] pageTypeName The name used to describe the page.  If the page size is unknown, the created
             *                         page will be marked as invalid.
             */
            PageFormat(const char* pageTypeName);

            /**
             * Constructor.
             *
             * \param[in] other The page size instance to use to define the page size.
             */
            PageFormat(const Util::PageSize& other);

            /**
             * Constructor.
             *
             * \param[in] qPageLayout An instance of QPageLayout that can be used to initialize this object.
             */
            PageFormat(const QPageLayout& qPageLayout);

            /**
             * Copy Constructor.  Note that the copy constructor does not copy elements from the underlying
             * \ref Ld::Format class.  All format parameters are copied by the format are not tied to any element.
             *
             * \param[in] other The page layout instance to copy.
             */
            PageFormat(const PageFormat& other);

            ~PageFormat() override;

            /**
             * Overloaded method that creates a clone of this class instance.  Note that the clone method does not copy
             * elements from the underlying \ref Ld::Format class.  All format parameters are copied by the format are
             * not tied to any element.
             *
             * \return Returns a deep copy of this class instance.
             */
            FormatPointer clone() const final;

            /**
             * Factory method that can be used to create a default instance of this format.
             *
             * \param[in] formatType The name used to reference this creator function.
             *
             * \return Returns a newly created, default, format instance.
             */
            static Format* creator(const QString& formatType);

            /**
             * Overloaded method that returns a name for this format.  The value is used to track and categorize
             * types of format data.
             *
             * \return Returns the format identifying name, in this case, "PageFormat".
             */
            QString typeName() const final;

            /**
             * Overloaded method that indicates if thisformat contains valid information.
             *
             * \return Returns true if the page size and margins are valid.  Returns false if the page size or margins
             *         are invalid.
             */
            bool isValid() const final;

            /**
             * Method you can call to determine the capabilities of this format.  The capabilities is a set containing
             * all the classes that make up the format instance.
             *
             * \return Returns a \ref Ld::Format::Capabilities instance containing the names of all the classes that
             *         define this format.
             */
            Capabilities capabilities() const final;

            /**
             * Method that indicates if this page size is valid or invalid.
             *
             * \return Returns true if the page size is valid.  Returns false if the page size is invalid.
             */
            bool isPageValid() const;

            /**
             * Method that indicates if this page size is invalid or valid.
             *
             * \return Returns true if the page size is invalid.  Returns false if the page size is valid.
             */
            bool isPageInvalid() const;

            /**
             * Method that indicates if this page margins are valid or invalid.
             *
             * \return Returns true if the page margins are valid.  Returns false if the page margins is invalid.
             */
            bool areMarginsValid() const;

            /**
             * Method that indicates if this page margins are invalid or valid.
             *
             * \return Returns true if the page margins are invalid.  Returns false if the page margins are valid.
             */
            bool areMarginsInvalid() const;

            /**
             * Method you can use to set the Util::PageSize associated with this layout.  Note that the page margins
             * may be adjusted if they become invalid based on this change.
             *
             * \param[in] newPageSize The page size instance to associate with this layout.
             */
            void setPageSize(const Util::PageSize& newPageSize);

            /**
             * Method you can use to obtain the Util::PageSize instance associated with this layout.
             *
             * \return Returns a Util::PageSize instance associated with this layout.
             */
            Util::PageSize pageSize() const;

            /**
             * Method that obtains the page width, in points.
             *
             * \return Returns the page width, in points.
             */
            float pageWidth() const;

            /**
             * Method that obtains the page height, in points.
             *
             * \return Returns the page height, in points.
             */
            float pageHeight() const;

            /**
             * Method that obtains the horizontal extents of the page.  The value will be the same as the page width or
             * page height depending on the page orientation.
             *
             * \return Returns the horizontal extents for the page, in points.
             */
            float horizontalExtents() const;

            /**
             * Method that obtains the vertical extents of the page.  The value will be the same as the page width or
             * page height depending on the page orientation.
             *
             * \return Returns the vertical extents for the page, in points.
             */
            float verticalExtents() const;

            /**
             * Method that obtains the horizontal active area in the page.
             *
             * \return Returns the horizontal active area across the page, in points.  The method takes into account the
             *         page orientation and margins.
             */
            float horzontalActiveArea() const;

            /**
             * Method that obtains the vertical active area in the page.
             *
             * \return Returns the vertical active area across the page, in points.  The method takes into account the
             *         page orientation and margins.
             */
            float verticalActiveArea() const;

            /**
             * Method you can use to set the page's left margin.
             *
             * \param[in] newLeftMarginPoints The new left margin for the page contents, in points.
             */
            void setLeftMargin(float newLeftMarginPoints);

            /**
             * Method you can use to obtain the page's current left margin setting.
             *
             * \return Returns the current page left margin setting.
             */
            float leftMargin() const;

            /**
             * Method you can use to set the page's right margin.
             *
             * \param[in] newRightMarginPoints The new right margin for the page contents, in points.
             */
            void setRightMargin(float newRightMarginPoints);

            /**
             * Method you can use to obtain the page's current right margin setting.
             *
             * \return Returns the current page right margin setting.
             */
            float rightMargin() const;

            /**
             * Method you can use to set the page's top margin.
             *
             * \param[in] newTopMarginPoints The new top margin for the page contents, in points.
             */
            void setTopMargin(float newTopMarginPoints);

            /**
             * Method you can use to obtain the page's current top margin setting.
             *
             * \return Returns the current page top margin setting.
             */
            float topMargin() const;

            /**
             * Method you can use to set the page's bottom margin.
             *
             * \param[in] newBottomMarginPoints The new bottom margin for the page contents, in points.
             */
            void setBottomMargin(float newBottomMarginPoints);

            /**
             * Method you can use to obtain the page's current bottom margin setting.
             *
             * \return Returns the current page bottom margin setting.
             */
            float bottomMargin() const;

            /**
             * Method you can use to set the text to landscape orientation.  Note that the page margins may be
             * adjusted if they become invalid based on this change.
             *
             * \param[in] nowLandscape If true, the page will be set to landscape orientation.  If false, the page will
             *                         be set to portrait orientation.
             */
            void setLandscape(bool nowLandscape = true);

            /**
             * Method you can use to set the text to portrait orientation.  Note that the page margins may be adjusted
             * if they become invalid based on this change.
             *
             * \param[in] nowPortrait If true, the page will be set to portrait orientation.  If false, the page will be
             *                         set to landscape orientation.
             */
            void setPortrait(bool nowPortrait = true);

            /**
             * Method you can use to determine if the page is in landscape orientation.
             *
             * \return Returns true if the page is in landscape orientation.  Returns false if the page is in portrait
             *         orientation.
             */
            bool landscape() const;

            /**
             * Method you can use to determine if the page is in portrait orientation.
             *
             * \return Returns true if the page is in portrait orientation.  Returns false if the page is in landscape
             *         orientation.
             */
            bool portrait() const;

            /**
             * Method that returns a QPageLayout instance that aligns with this page layout.
             *
             * \return Returns an instance of QPageLayout that best fits this page.
             */
            QPageLayout toQPageLayout() const;

            /**
             * Method that creates a string identifier suitable for recording details about this page layout.  The
             * generated string is suitable for reconstructing this \ref Ld::PageFormat instance at a later point in
             * time using the static \ref Ld::PageFormat::fromString method.
             *
             * \return Returns a string representing the PageFormat value.
             */
            QString toString() const final;

            /**
             * Method that returns CSS content that can be used to emulate this format.  The returned
             * string should be on a single line and should not include leading or trailing braces.
             *
             * \return Returns CSS content comparable to this format.  This version returns an empty string.
             */
            QString toCss() const final;

            /**
             * Method that converts a string identifier to a \ref Ld::PageFormat instance.
             *
             * \param[in]  identifier The string identifier for the page layout.
             *
             * \param[out] ok         Optional pointer to an instance of bool used to determine if the string identifier
             *                        is correct.  The boolean will be set to true if the string is valid and will be
             *                        set to false if the string is invalid.  You can provide a null pointer if you
             *                        don't need to determine validity.
             *
             * \return Returns a \ref Ld::PageFormat described by the string.
             */
            static PageFormat fromString(const QString& identifier, bool* ok = Q_NULLPTR);

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            PageFormat& operator=(const PageFormat& other);

        protected:
            /**
             * Method you can optionally overload to add additional attributes to the XML description of this element.
             * This method should write attributes for this class only and not the class's children.  To cleanly handle
             * multiple virtual inheritance, the Format:writeAddAttributes method should be coded to call the
             * appropriate child class methods before calling this method.
             *
             * \param[in]     attributes  A reference to the list of attributes to be modified.
             *
             * \param[in]     formats     A \ref FormatOrganizer instance used to track all known formats in the
             *                            program.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             */
            void writeAddImmediateAttributes(
                XmlAttributes&                  attributes,
                QSharedPointer<FormatOrganizer> formats,
                ProgramFile&                    programFile
            ) const override;

            /**
             * Method you can overload to parse incoming XML attributes.  This method will only be called if there are
             * provided attributes.  To cleanly handle multiple virtual inheritance, the
             * \ref Ld::Format::readAttributes method should be coded to call the appropriate child class methods
             * before calling this method.
             *
             * Errors are reported directly to the reader using the raiseError method.
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
            void readImmediateAttributes(
                QSharedPointer<XmlReader>  reader,
                const XmlAttributes&       attributes,
                const FormatsByIdentifier& formats,
                ProgramFile&               programFile,
                unsigned                   xmlVersion
            ) override;

            /**
             * Method you can optionally overload to add additional attributes to the XML description of this element.
             *
             * \param[in]     attributes  A reference to the list of attributes to be modified.
             *
             * \param[in]     formats     A \ref FormatOrganizer instance used to track all known formats in the
             *                            program.
             *
             * \param[in,out] programFile The program file instance that can be used to store (or retrieve) additional
             *                            side-band information.
             */
            void writeAddAttributes(
                XmlAttributes&                  attributes,
                QSharedPointer<FormatOrganizer> formats,
                ProgramFile&                    programFile
            ) const override;

            /**
             * Method you can overload to parse incoming XML attributes.  This method will only be called if there are
             * provided attributes.
             *
             * Errors are reported directly to the reader using the raiseError method.
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
            void readAttributes(
                QSharedPointer<XmlReader>  reader,
                const XmlAttributes&       attributes,
                const FormatsByIdentifier& formats,
                ProgramFile&               programFile,
                unsigned                   xmlVersion
            ) override;

        private:
            /**
             * The default left margin, in points.
             */
            static const float defaultLeftMarginPoints;

            /**
             * The default right margin, in points.
             */
            static const float defaultRightMarginPoints;

            /**
             * The default top margin, in points.
             */
            static const float defaultTopMarginPoints;

            /**
             * The default bottom margin, in points.
             */
            static const float defaultBottomMarginPoints;

            /**
             * The default lanscape mode (false means portrait orientation).
             */
            static const bool defaultLandscape;

            /**
             * Method that performs configuration common to all constructors.
             */
            void configure();

            /**
             * Method that checks if the margins are still valid after a page size change.  If the margins are invalid,
             * the margins will be scaled such that they are now valid.
             */
            void forceValidMargins();

            /**
             * Method that returns the best default layout for the given localization.
             *
             * \return Return a pre-configured \ref Ld::PageFormat instance suitable for this domain.
             */
            static PageFormat calculateDefaultPageFormat();

            /**
             * The Util::PageSize instance associated with this layout.
             */
            Util::PageSize currentPageSize;

            /**
             * Indicates if the page is landscape or portrait orientation.
             */
            bool currentLandscape;

            /**
             * The page left margin, in points.
             */
            float currentLeftMarginPoints;

            /**
             * The page right margin, in points.
             */
            float currentRightMarginPoints;

            /**
             * The page top margin, in points.
             */
            float currentTopMarginPoints;

            /**
             * The page bottom margin, in points.
             */
            float currentBottomMarginPoints;
    };
}
#endif
