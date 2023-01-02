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
* This file implements the \ref Ld::PageFormat class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>
#include <QStringList>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QPageSize>
#include <QPageLayout>
#include <QMarginsF>

#include <cassert>

#include <util_page_size.h>

#include "ld_format_structures.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_program_file.h"
#include "ld_page_format.h"

/***********************************************************************************************************************
 * Ld::PageFormat::Aggregation
 */

namespace Ld {
    PageFormat::Aggregation::Aggregation() {
        clear();
    }


    PageFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* PageFormat::Aggregation::creator(const QString&) {
        return new PageFormat::Aggregation;
    }


    QString PageFormat::Aggregation::typeName() const {
        return PageFormat::formatName;
    }


    void PageFormat::Aggregation::clear() {
        currentPageSizes.clear();

        currentLeftMargins.clear();
        currentRightMargins.clear();
        currentTopMargins.clear();
        currentBottomMargins.clear();

        currentLandscape.clear();
        currentPortrait.clear();

        currentMinimumPageWidth  = -1;
        currentMinimumPageHeight = -1;

        currentMaximumLeftMargin   = -1;
        currentMaximumRightMargin  = -1;
        currentMaximumTopMargin    = -1;
        currentMaximumBottomMargin = -1;

        currentMaximumAllowableLeftMargin   = -1;
        currentMaximumAllowableRightMargin  = -1;
        currentMaximumAllowableTopMargin    = -1;
        currentMaximumAllowableBottomMargin = -1;

        currentPageMinimumHorizontalExtents = -1;
        currentPageMinimumVerticalExtents   = -1;

        Format::Aggregation::clear();
    }


    bool PageFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<PageFormat> pageFormat = formatInstance.dynamicCast<PageFormat>();
        if (!pageFormat.isNull()) {
            success = Format::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentPageSizes << pageFormat->pageSize();
                currentLandscape << pageFormat->landscape();
                currentPortrait  << pageFormat->portrait();

                double pageWidth = pageFormat->pageWidth();
                if (currentMinimumPageWidth < 0 || pageWidth < currentMinimumPageWidth) {
                    currentMinimumPageWidth = pageWidth;
                }

                double pageHeight = pageFormat->pageHeight();
                if (currentMinimumPageHeight < 0 || pageHeight < currentMinimumPageHeight) {
                    currentMinimumPageHeight = pageHeight;
                }

                double horizontalExtents = pageFormat->horizontalExtents();
                if (currentPageMinimumHorizontalExtents < 0                 ||
                    horizontalExtents < currentPageMinimumHorizontalExtents    ) {
                    currentPageMinimumHorizontalExtents = horizontalExtents;
                }

                double verticalExtents   = pageFormat->verticalExtents();
                if (currentPageMinimumVerticalExtents < 0 || verticalExtents < currentPageMinimumVerticalExtents) {
                    currentPageMinimumVerticalExtents = verticalExtents;
                }

                double leftMargin   = pageFormat->leftMargin();
                double rightMargin  = pageFormat->rightMargin();
                double topMargin    = pageFormat->topMargin();
                double bottomMargin = pageFormat->bottomMargin();

                currentLeftMargins   << leftMargin;
                currentRightMargins  << rightMargin;
                currentTopMargins    << topMargin;
                currentBottomMargins << bottomMargin;

                if (currentMaximumLeftMargin < 0 || leftMargin < currentMaximumLeftMargin) {
                    currentMaximumLeftMargin = leftMargin;
                }

                if (currentMaximumRightMargin < 0 || rightMargin < currentMaximumRightMargin) {
                    currentMaximumRightMargin = rightMargin;
                }

                if (currentMaximumTopMargin < 0 || topMargin < currentMaximumTopMargin) {
                    currentMaximumTopMargin = topMargin;
                }

                if (currentMaximumBottomMargin < 0 || bottomMargin < currentMaximumBottomMargin) {
                    currentMaximumBottomMargin = bottomMargin;
                }

                double allowedLeftMargin   = minimumActiveAreaFraction * horizontalExtents - rightMargin;
                double allowedRightMargin  = minimumActiveAreaFraction * horizontalExtents - leftMargin;
                double allowedTopMargin    = minimumActiveAreaFraction * verticalExtents - bottomMargin;
                double allowedBottomMargin = minimumActiveAreaFraction * verticalExtents - topMargin;

                if (currentMaximumAllowableLeftMargin < 0 || allowedLeftMargin < currentMaximumAllowableLeftMargin) {
                    currentMaximumAllowableLeftMargin = allowedLeftMargin;
                }

                if (currentMaximumAllowableRightMargin < 0                  ||
                    allowedRightMargin < currentMaximumAllowableRightMargin    ) {
                    currentMaximumAllowableRightMargin = allowedRightMargin;
                }

                if (currentMaximumAllowableTopMargin < 0 || allowedTopMargin < currentMaximumAllowableTopMargin) {
                    currentMaximumAllowableTopMargin = allowedTopMargin;
                }

                if (currentMaximumAllowableBottomMargin < 0                   ||
                    allowedBottomMargin < currentMaximumAllowableBottomMargin    ) {
                    currentMaximumAllowableBottomMargin = allowedBottomMargin;
                }
            }
        }

        return success;
    }


    QSet<Util::PageSize> PageFormat::Aggregation::pageSizes() const {
        return currentPageSizes;
    }


    float PageFormat::Aggregation::minimumPageWidth() const {
        return currentMinimumPageWidth;
    }


    float PageFormat::Aggregation::minimumPageHeight() const {
        return currentMinimumPageHeight;
    }


    float PageFormat::Aggregation::minimumPageHorizontalExtents() const {
        return currentPageMinimumHorizontalExtents;
    }


    float PageFormat::Aggregation::minimumPageVerticalExtents() const {
        return currentPageMinimumVerticalExtents;
    }


    QSet<float> PageFormat::Aggregation::leftMargins() const {
        return currentLeftMargins;
    }


    QSet<float> PageFormat::Aggregation::rightMargins() const {
        return currentRightMargins;
    }


    QSet<float> PageFormat::Aggregation::topMargins() const {
        return currentTopMargins;
    }


    QSet<float> PageFormat::Aggregation::bottomMargins() const {
        return currentBottomMargins;
    }


    float PageFormat::Aggregation::maximumLeftMargin() const {
        return currentMaximumLeftMargin;
    }


    float PageFormat::Aggregation::maximumRightMargin() const {
        return currentMaximumRightMargin;
    }


    float PageFormat::Aggregation::maximumTopMargin() const {
        return currentMaximumTopMargin;
    }


    float PageFormat::Aggregation::maximumBottomMargin() const {
        return currentMaximumBottomMargin;
    }


    float PageFormat::Aggregation::maximumAllowableLeftMargin() const {
        return currentMaximumAllowableLeftMargin;
    }


    float PageFormat::Aggregation::maximumAllowableRightMargin() const {
        return currentMaximumAllowableRightMargin;
    }


    float PageFormat::Aggregation::maximumAllowableTopMargin() const {
        return currentMaximumAllowableTopMargin;
    }


    float PageFormat::Aggregation::maximumAllowableBottomMargin() const {
        return currentMaximumAllowableBottomMargin;
    }


    Format::BooleanAggregation PageFormat::Aggregation::landscape() const {
        return currentLandscape;
    }


    Format::BooleanAggregation PageFormat::Aggregation::portrait() const {
        return currentPortrait;
    }


    bool PageFormat::Aggregation::operator==(const PageFormat::Aggregation& other) const {
        return (
               other.currentPageSizes == currentPageSizes
            && other.currentLandscape == currentLandscape
            && other.currentLeftMargins == currentLeftMargins
            && other.currentRightMargins == currentRightMargins
            && other.currentTopMargins == currentTopMargins
            && other.currentBottomMargins == currentBottomMargins
        );
    }


    bool PageFormat::Aggregation::operator!=(const PageFormat::Aggregation& other) const {
        return (
               other.currentPageSizes != currentPageSizes
            || other.currentLandscape != currentLandscape
            || other.currentLeftMargins != currentLeftMargins
            || other.currentRightMargins != currentRightMargins
            || other.currentTopMargins != currentTopMargins
            || other.currentBottomMargins != currentBottomMargins
        );
    }
}

/***********************************************************************************************************************
 * Ld::PageFormat
 */

namespace Ld {
    const QString    PageFormat::formatName("PageFormat");
    const PageFormat PageFormat::defaultPageFormat = PageFormat::calculateDefaultPageFormat();
    const float      PageFormat::defaultLeftMarginPoints = 72.0;
    const float      PageFormat::defaultRightMarginPoints = 72.0;
    const float      PageFormat::defaultTopMarginPoints = 72.0;
    const float      PageFormat::defaultBottomMarginPoints = 72.0;
    const bool       PageFormat::defaultLandscape = false;


    PageFormat::PageFormat() {
        configure();
    }


    PageFormat::PageFormat(float pageWidthPoints, float pageHeightPoints) {
        configure();
        currentPageSize = Util::PageSize(pageWidthPoints, pageHeightPoints);
    }


    PageFormat::PageFormat(const QString& pageTypeName) {
        configure();
        currentPageSize = Util::PageSize(pageTypeName);
    }


    PageFormat::PageFormat(const char* pageTypeName) {
        configure();
        currentPageSize = Util::PageSize(pageTypeName);
    }


    PageFormat::PageFormat(const Util::PageSize& other) {
        configure();
        currentPageSize = other;
    }


    PageFormat::PageFormat(const QPageLayout& qPageLayout) {
        currentPageSize = Util::PageSize(qPageLayout.pageSize());

        QMarginsF margins = qPageLayout.margins(QPageLayout::Point);

        currentLeftMarginPoints   = margins.left();
        currentRightMarginPoints  = margins.right();
        currentTopMarginPoints    = margins.top();
        currentBottomMarginPoints = margins.bottom();

        currentLandscape          = (qPageLayout.orientation() == QPageLayout::Landscape);
    }


    PageFormat::PageFormat(const PageFormat& other):Format(other) {
        currentPageSize           = other.currentPageSize;

        currentLeftMarginPoints   = other.currentLeftMarginPoints;
        currentRightMarginPoints  = other.currentRightMarginPoints;
        currentTopMarginPoints    = other.currentTopMarginPoints;
        currentBottomMarginPoints = other.currentBottomMarginPoints;
        currentLandscape          = other.currentLandscape;
    }


    PageFormat::~PageFormat() {}


    FormatPointer PageFormat::clone() const {
        return FormatPointer(new PageFormat(*this));
    }


    Format* PageFormat::creator(const QString&) {
        return new PageFormat();
    }


    QString PageFormat::typeName() const {
        return formatName;
    }


    bool PageFormat::isValid() const {
        return isPageValid() && areMarginsValid();
    }


    Format::Capabilities PageFormat::capabilities() const {
        return Format::capabilities() << formatName;
    }


    bool PageFormat::isPageValid() const {
        return currentPageSize.isValid();
    }


    bool PageFormat::isPageInvalid() const {
        return currentPageSize.isInvalid();
    }


    bool PageFormat::areMarginsValid() const {
        bool marginsValid = true;

        if (currentLeftMarginPoints < 0 || currentRightMarginPoints < 0) {
            marginsValid = false;
        } else if (currentTopMarginPoints < 0 || currentBottomMarginPoints < 0) {
            marginsValid = false;
        } else {
            if (currentLeftMarginPoints + currentRightMarginPoints >= horizontalExtents()) {
                marginsValid = false;
            } else if (currentTopMarginPoints + currentBottomMarginPoints >= verticalExtents()) {
                marginsValid = false;
            }
        }

        return marginsValid;
    }


    bool PageFormat::areMarginsInvalid() const {
        return !areMarginsValid();
    }


    void PageFormat::setPageSize(const Util::PageSize& newPageSize) {
        currentPageSize = newPageSize;
        forceValidMargins();
        reportFormatUpdated();
    }


    Util::PageSize PageFormat::pageSize() const {
        return currentPageSize;
    }


    float PageFormat::pageWidth() const {
        return currentPageSize.width();
    }


    float PageFormat::pageHeight() const {
        return currentPageSize.height();
    }


    float PageFormat::horizontalExtents() const {
        return currentLandscape ? currentPageSize.height() : currentPageSize.width();
    }


    float PageFormat::verticalExtents() const {
        return currentLandscape ? currentPageSize.width() : currentPageSize.height();
    }


    float PageFormat::horzontalActiveArea() const {
        return horizontalExtents() - currentLeftMarginPoints - currentRightMarginPoints;
    }


    float PageFormat::verticalActiveArea() const {
        return verticalExtents() - currentTopMarginPoints - currentBottomMarginPoints;
    }


    void PageFormat::setLeftMargin(float newLeftMarginPoints) {
        currentLeftMarginPoints = newLeftMarginPoints;
        reportFormatUpdated();
    }


    float PageFormat::leftMargin() const {
        return currentLeftMarginPoints;
    }


    void PageFormat::setRightMargin(float newRightMarginPoints) {
        currentRightMarginPoints = newRightMarginPoints;
        reportFormatUpdated();
    }


    float PageFormat::rightMargin() const {
        return currentRightMarginPoints;
    }


    void PageFormat::setTopMargin(float newTopMarginPoints) {
        currentTopMarginPoints = newTopMarginPoints;
        reportFormatUpdated();
    }


    float PageFormat::topMargin() const {
        return currentTopMarginPoints;
    }


    void PageFormat::setBottomMargin(float newBottomMarginPoints) {
        currentBottomMarginPoints = newBottomMarginPoints;
        reportFormatUpdated();
    }


    float PageFormat::bottomMargin() const {
        return currentBottomMarginPoints;
    }


    void PageFormat::setLandscape(bool nowLandscape) {
        currentLandscape = nowLandscape;
        forceValidMargins();
        reportFormatUpdated();
    }


    void PageFormat::setPortrait(bool nowPortrait) {
        setLandscape(!nowPortrait);
    }


    bool PageFormat::landscape() const {
        return currentLandscape;
    }


    bool PageFormat::portrait() const {
        return !landscape();
    }


    QPageLayout PageFormat::toQPageLayout() const {
        return QPageLayout(
            currentPageSize.toQPageSize(),
            currentLandscape ? QPageLayout::Landscape : QPageLayout::Portrait,
            QMarginsF(
                currentLeftMarginPoints,
                currentTopMarginPoints,
                currentRightMarginPoints,
                currentBottomMarginPoints
            ),
            QPageLayout::Point
        );
    }


    QString PageFormat::toString() const {
        QString pageFormatString;

        if (isInvalid()) {
            pageFormatString = QString("PageFormat,Invalid");
        } else {
            pageFormatString = QString("PageFormat,%1,%2,%3,%4,%5,%6")
                               .arg(currentPageSize.toString())
                               .arg(currentLeftMarginPoints)
                               .arg(currentRightMarginPoints)
                               .arg(currentTopMarginPoints)
                               .arg(currentBottomMarginPoints)
                               .arg(currentLandscape ? "landscape" : "portrait");
        }

        return pageFormatString;
    }


    QString PageFormat::toCss() const {
        return QString();
    }


    PageFormat PageFormat::fromString(const QString& identifier, bool* ok) {
        bool       isOK = true;
        PageFormat layout;

        if (identifier == QString("Invalid")) {
            layout = PageFormat(QPageLayout());
            layout.setTopMargin(-1);
            layout.setBottomMargin(-1);
            layout.setLeftMargin(-1);
            layout.setRightMargin(-1);
        } else {
            QStringList pieces = identifier.split(",");

            if (pieces.length() != 7 || pieces[0] != "PageFormat") {
                isOK = false;
            }

            if (isOK) {
                layout = PageFormat(Util::PageSize::fromString(pieces[1], &isOK));
            }

            if (isOK) {
                float leftMargin           = -1;
                float rightMargin          = -1;
                float topMargin            = -1;
                float bottomMargin         = -1;
                bool  landscapeOrientation = false;

                if (isOK) {
                    leftMargin = pieces[2].toFloat(&isOK);
                }

                if (isOK) {
                    rightMargin = pieces[3].toFloat(&isOK);
                }

                if (isOK) {
                    topMargin = pieces[4].toFloat(&isOK);
                }

                if (isOK) {
                    bottomMargin = pieces[5].toFloat(&isOK);
                }

                if (isOK) {
                    if (pieces[6] == QString("landscape")) {
                        landscapeOrientation = true;
                    } else if (pieces[6] == QString("portrait")) {
                        landscapeOrientation = false;
                    } else {
                        isOK = false;
                    }
                }

                layout.setLeftMargin(leftMargin);
                layout.setRightMargin(rightMargin);
                layout.setTopMargin(topMargin);
                layout.setBottomMargin(bottomMargin);
                layout.setLandscape(landscapeOrientation);
            }
        }

        if (!isOK) {
            layout = PageFormat(QPageLayout());
            layout.setTopMargin(-1);
            layout.setBottomMargin(-1);
            layout.setLeftMargin(-1);
            layout.setRightMargin(-1);
        }

        if (ok != nullptr) {
            *ok = isOK;
        }

        return layout;
    }


    PageFormat& PageFormat::operator=(const PageFormat& other) {
        currentPageSize           = other.currentPageSize;
        currentLandscape          = other.currentLandscape;
        currentLeftMarginPoints   = other.currentLeftMarginPoints;
        currentRightMarginPoints  = other.currentRightMarginPoints;
        currentTopMarginPoints    = other.currentTopMarginPoints;
        currentBottomMarginPoints = other.currentBottomMarginPoints;

        return *this;
    }


    void PageFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer>,
            ProgramFile&
        ) const {
        if (pageWidth() != defaultPageFormat.pageWidth()) {
            attributes.append("width", pageWidth());
        }

        if (pageHeight() != defaultPageFormat.pageHeight()) {
            attributes.append("height", pageHeight());
        }

        if (landscape() != defaultLandscape) {
            QString orientationString = landscape() ? QString("landscape") : QString("portrait");
            attributes.append("orientation", orientationString);
        }

        if (leftMargin() != defaultLeftMarginPoints) {
            attributes.append("left", leftMargin());
        }

        if (rightMargin() != defaultRightMarginPoints) {
            attributes.append("right", rightMargin());
        }

        if (topMargin() != defaultTopMarginPoints) {
            attributes.append("top", topMargin());
        }

        if (bottomMargin() != defaultBottomMarginPoints) {
            attributes.append("bottom", bottomMargin());
        }
    }


    void PageFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier&,
            ProgramFile&,
            unsigned
        ) {
        bool ok;
        float pageWidth = attributes.value<float>("width", defaultPageFormat.pageWidth(), &ok);
        if (!ok) {
            QString tag = reader->qualifiedName().toString();
            QString attributeString = attributes.value<QString>("width");
            reader->raiseError(tr("Tag \"%1\", invalid width \"%1\"").arg(tag).arg(attributeString));
        }

        float pageHeight = defaultPageFormat.pageHeight();
        if (!reader->hasError()) {
            pageHeight = attributes.value<float>("height", defaultPageFormat.pageHeight(), &ok);
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                QString attributeString = attributes.value<QString>("height");
                reader->raiseError(tr("Tag \"%1\", invalid height \"%1\"").arg(tag).arg(attributeString));
            }
        }

        bool isLandscape = defaultLandscape;
        if (!reader->hasError() && attributes.hasAttribute("orientation")) {
            QString orientationString = attributes.value<QString>("orientation");

            if (orientationString == "landscape") {
                isLandscape = true;
            } else if (orientationString == "portrait") {
                isLandscape = false;
            } else {
                QString tag = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\", invalid orientation \"%1\"").arg(tag).arg(orientationString));
            }
        }

        float leftMargin = defaultLeftMarginPoints;
        if (!reader->hasError()) {
            leftMargin = attributes.value<float>("left", defaultLeftMarginPoints, &ok);
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                QString attributeString = attributes.value<QString>("left");
                reader->raiseError(tr("Tag \"%1\", invalid left margin \"%1\"").arg(tag).arg(attributeString));
            }
        }

        float rightMargin = defaultRightMarginPoints;
        if (!reader->hasError()) {
            rightMargin = attributes.value<float>("right", defaultRightMarginPoints, &ok);
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                QString attributeString = attributes.value<QString>("right");
                reader->raiseError(tr("Tag \"%1\", invalid right margin \"%1\"").arg(tag).arg(attributeString));
            }
        }

        float topMargin = defaultTopMarginPoints;
        if (!reader->hasError()) {
            topMargin = attributes.value<float>("top", defaultTopMarginPoints, &ok);
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                QString attributeString = attributes.value<QString>("top");
                reader->raiseError(tr("Tag \"%1\", invalid top margin \"%1\"").arg(tag).arg(attributeString));
            }
        }

        float bottomMargin = defaultBottomMarginPoints;
        if (!reader->hasError()) {
            bottomMargin = attributes.value<float>("bottom", defaultBottomMarginPoints, &ok);
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                QString attributeString = attributes.value<QString>("bottom");
                reader->raiseError(tr("Tag \"%1\", invalid bottom margin \"%1\"").arg(tag).arg(attributeString));
            }
        }

        if (!reader->hasError()) {
            setPageSize(Util::PageSize(pageWidth, pageHeight));
            setLandscape(isLandscape);
            setLeftMargin(leftMargin);
            setRightMargin(rightMargin);
            setTopMargin(topMargin);
            setBottomMargin(bottomMargin);
        }
    }


    void PageFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        PageFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void PageFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        PageFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }


    void PageFormat::configure() {
        setPageSize(defaultPageFormat.pageSize());

        currentLeftMarginPoints   = defaultLeftMarginPoints;
        currentRightMarginPoints  = defaultRightMarginPoints;
        currentTopMarginPoints    = defaultTopMarginPoints;
        currentBottomMarginPoints = defaultBottomMarginPoints;
        currentLandscape          = defaultLandscape;
    }


    void PageFormat::forceValidMargins() {
        double marginHorizontalExtents = PageFormat::horizontalExtents() * minimumActiveAreaFraction;
        if (currentLeftMarginPoints + currentRightMarginPoints > marginHorizontalExtents) {
            double adjustment = marginHorizontalExtents / (currentLeftMarginPoints + currentRightMarginPoints);
            currentLeftMarginPoints  *= adjustment;
            currentRightMarginPoints *= adjustment;
        }

        double marginVerticalExtents = PageFormat::verticalExtents() * minimumActiveAreaFraction;
        if (currentTopMarginPoints + currentBottomMarginPoints > marginVerticalExtents) {
            double adjustment = marginVerticalExtents / (currentTopMarginPoints + currentBottomMarginPoints);
            currentTopMarginPoints    *= adjustment;
            currentBottomMarginPoints *= adjustment;
        }
    }


    PageFormat PageFormat::calculateDefaultPageFormat() {
        PageFormat defaultLayout(Util::PageSize::defaultPageSize);

        defaultLayout.setLeftMargin(defaultLeftMarginPoints);
        defaultLayout.setRightMargin(defaultRightMarginPoints);
        defaultLayout.setTopMargin(defaultTopMarginPoints);
        defaultLayout.setBottomMargin(defaultBottomMarginPoints);
        defaultLayout.setLandscape(defaultLandscape);

        return defaultLayout;
    }
}
