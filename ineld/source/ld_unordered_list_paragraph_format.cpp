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
* This file implements the \ref Ld::UnorderedListParagraphFormat class.
***********************************************************************************************************************/

#include <QChar>
#include <QString>
#include <QCoreApplication> // For translation macros.
#include <QSharedPointer>
#include <QSet>

#include <cstdint>

#include "ld_format_structures.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_program_file.h"
#include "ld_format.h"
#include "ld_block_format.h"
#include "ld_justified_block_format.h"
#include "ld_text_block_format.h"
#include "ld_paragraph_format.h"
#include "ld_font_format.h"
#include "ld_list_paragraph_format_base.h"
#include "ld_unordered_list_paragraph_format.h"

/***********************************************************************************************************************
 * Ld::UnorderedListParagraphFormat::Aggregation
 */

namespace Ld {
    UnorderedListParagraphFormat::Aggregation::Aggregation() {}


    UnorderedListParagraphFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* UnorderedListParagraphFormat::Aggregation::creator(const QString&) {
        return new UnorderedListParagraphFormat::Aggregation;
    }


    QString UnorderedListParagraphFormat::Aggregation::typeName() const {
        return UnorderedListParagraphFormat::formatName;
    }


    void UnorderedListParagraphFormat::Aggregation::clear() {
        ListParagraphFormatBase::Aggregation::clear();
        currentBulletStyles.clear();
    }


    bool UnorderedListParagraphFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<UnorderedListParagraphFormat> format =
            formatInstance.dynamicCast<UnorderedListParagraphFormat>();

        if (!format.isNull()) {
            success = ListParagraphFormatBase::Aggregation::addFormat(format, includeExisting);

            if (success) {
                currentBulletStyles << format->bulletStyle();
            }
        }

        return success;
    }


    QSet<QString> UnorderedListParagraphFormat::Aggregation::bulletStyles() const {
        return currentBulletStyles;
    }
}

/***********************************************************************************************************************
 * Ld::UnorderedListParagraphFormat::StandardBullet
 */

namespace Ld {
    UnorderedListParagraphFormat::StandardBullet::StandardBullet() {
        currentUnicodeValue = 0;
        currentDescription  = QString();
        currentCategory     = Category::INVALID;
        currentHtmlCategory = HtmlCategory::INVALID;
    }


    UnorderedListParagraphFormat::StandardBullet::StandardBullet(
            const UnorderedListParagraphFormat::StandardBullet& other
        ) {
        currentUnicodeValue = other.currentUnicodeValue;
        currentDescription  = other.currentDescription;
        currentCategory     = other.currentCategory;
        currentHtmlCategory = other.currentHtmlCategory;
    }


    UnorderedListParagraphFormat::StandardBullet::~StandardBullet() {}


    bool UnorderedListParagraphFormat::StandardBullet::isValid() const {
        return currentUnicodeValue != 0;
    }


    bool UnorderedListParagraphFormat::StandardBullet::isInvalid() const {
        return !isValid();
    }


    UnorderedListParagraphFormat::StandardBullet::UnicodeValue
            UnorderedListParagraphFormat::StandardBullet::unicodeValue() const {
        return currentUnicodeValue;
    }


    QString UnorderedListParagraphFormat::StandardBullet::bullet() const {
        return QString(QChar(currentUnicodeValue));
    }


    QString UnorderedListParagraphFormat::StandardBullet::description() const {
        return currentDescription;
    }


    UnorderedListParagraphFormat::StandardBullet::Category
    UnorderedListParagraphFormat::StandardBullet::category() const {
        return currentCategory;
    }


    UnorderedListParagraphFormat::StandardBullet::HtmlCategory
    UnorderedListParagraphFormat::StandardBullet::htmlCategory() const {
        return currentHtmlCategory;
    }


    UnorderedListParagraphFormat::StandardBullet& UnorderedListParagraphFormat::StandardBullet::operator=(
            const StandardBullet& other
        ) {
        currentUnicodeValue = other.currentUnicodeValue;
        currentDescription  = other.currentDescription;
        currentCategory     = other.currentCategory;
        currentHtmlCategory = other.currentHtmlCategory;

        return *this;
    }


    bool UnorderedListParagraphFormat::StandardBullet::operator==(
            const UnorderedListParagraphFormat::StandardBullet& other
        ) const {
        return currentUnicodeValue == other.currentUnicodeValue;
    }


    bool UnorderedListParagraphFormat::StandardBullet::operator!=(
            const UnorderedListParagraphFormat::StandardBullet& other
        ) const {
        return currentUnicodeValue != other.currentUnicodeValue;
    }


    UnorderedListParagraphFormat::StandardBullet::StandardBullet(
            UnorderedListParagraphFormat::StandardBullet::UnicodeValue unicode,
            const QString&                                             description,
            UnorderedListParagraphFormat::StandardBullet::Category     category,
            UnorderedListParagraphFormat::StandardBullet::HtmlCategory htmlCategory
        ) {
        currentUnicodeValue = unicode;
        currentDescription  = description;
        currentCategory     = category;
        currentHtmlCategory = htmlCategory;
    }
}

/***********************************************************************************************************************
 * Ld::UnorderedListParagraphFormat
 */

namespace Ld {
    const QString UnorderedListParagraphFormat::formatName("UnorderedListParagraphFormat");
    const QString UnorderedListParagraphFormat::defaultBulletStyle(QChar(0x2022));
    const QString UnorderedListParagraphFormat::fallbackBulletStyle(QChar(0x002A));

    QList<UnorderedListParagraphFormat::StandardBullet> UnorderedListParagraphFormat::currentStandardBullets;

    typedef UnorderedListParagraphFormat::StandardBullet::Category     BulletCategory;
    typedef UnorderedListParagraphFormat::StandardBullet::HtmlCategory HtmlCategory;
    typedef UnorderedListParagraphFormat::StandardBullet::UnicodeValue UnicodeValue;

    static const struct StandardBulletData {
        UnicodeValue   unicodeValue;
        QString        description;
        BulletCategory category;
        HtmlCategory   htmlCategory;
    } standardBulletData[] = {
        {
            0x2022,
            QT_TR_NOOP("small black bullet"),
            BulletCategory::FILLED_DISC,
            HtmlCategory::DISC
        },
        {
            0x25E6,
            QT_TR_NOOP("small white bullet"),
            BulletCategory::UNFILLED_DISC,
            HtmlCategory::CIRCLE
        },
        {
            0x25CF,
            QT_TR_NOOP("large black bullet"),
            BulletCategory::FILLED_DISC,
            HtmlCategory::DISC
        },
        {
            0x25CB,
            QT_TR_NOOP("large white bullet"),
            BulletCategory::UNFILLED_DISC,
            HtmlCategory::CIRCLE
        },
        {
            0x25AA,
            QT_TR_NOOP("small black square"),
            BulletCategory::FILLED_SQUARE,
            HtmlCategory::SQUARE
        },
        {
            0x25AB,
            QT_TR_NOOP("small white square"),
            BulletCategory::UNFILLED_SQUARE,
            HtmlCategory::SQUARE
        },
        {
            0x25FC,
            QT_TR_NOOP("large black square"),
            BulletCategory::FILLED_SQUARE,
            HtmlCategory::SQUARE
        },
        {
            0x25FB,
            QT_TR_NOOP("large white square"),
            BulletCategory::UNFILLED_SQUARE,
            HtmlCategory::SQUARE
        },
        {
            0x25B8,
            QT_TR_NOOP("small black right pointing triangle"),
            BulletCategory::FILLED_TRIANGLE,
            HtmlCategory::DISC
        },
        {
            0x25B9,
            QT_TR_NOOP("small white right pointing triangle"),
            BulletCategory::UNFILLED_TRIANGLE,
            HtmlCategory::CIRCLE
        },
        {
            0x25B6,
            QT_TR_NOOP("large black right pointing triangle"),
            BulletCategory::FILLED_TRIANGLE,
            HtmlCategory::DISC
        },
        {
            0x25B7,
            QT_TR_NOOP("large white right pointing triangle"),
            BulletCategory::UNFILLED_TRIANGLE,
            HtmlCategory::CIRCLE
        },
//        {
//            0x2B25,
//            QT_TR_NOOP("small black diamond"),
//            BulletCategory::FILLED_DIAMOND,
//            HtmlCategory::DISC
//        },
//        {
//            0x2B26,
//            QT_TR_NOOP("small white diamond"),
//            BulletCategory::UNFILLED_DIAMOND,
//            HtmlCategory::CIRCLE
//        },
        {
            0x25C6,
            QT_TR_NOOP("large black diamond"),
            BulletCategory::FILLED_DIAMOND,
            HtmlCategory::DISC,
        },
        {
            0x25C7,
            QT_TR_NOOP("large white diamond"),
            BulletCategory::UNFILLED_DIAMOND,
            HtmlCategory::CIRCLE
        },
        {
            0x2043,
            QT_TR_NOOP("hyphen bullet"),
            BulletCategory::HYPHEN,
            HtmlCategory::DISC
        },
        {
            0x2736,
            QT_TR_NOOP("six pointed star"),
            BulletCategory::FILLED_STAR,
            HtmlCategory::DISC
        },
        {
            0x2713,
            QT_TR_NOOP("check mark"),
            BulletCategory::CHECKMARK,
            HtmlCategory::SQUARE
        },
        {
            0x2714,
            QT_TR_NOOP("heavy check mark"),
            BulletCategory::CHECKMARK,
            HtmlCategory::SQUARE
        },
        {
            0x002A,
            QT_TR_NOOP("asterisk"),
            BulletCategory::ASTERISK,
            HtmlCategory::DISC
        },
        {
            0x002D,
            QT_TR_NOOP("hypen"),
            BulletCategory::HYPHEN,
            HtmlCategory::DISC
        },
        {
            0x0000,
            QString(),
            BulletCategory::INVALID,
            HtmlCategory::INVALID
        }
    };

    UnorderedListParagraphFormat::UnorderedListParagraphFormat() {
        currentBulletStyle = defaultBulletStyle;
    }


    UnorderedListParagraphFormat::UnorderedListParagraphFormat(
            const UnorderedListParagraphFormat& other
        ):Format(
            other
        ),BlockFormat(
            other
        ),JustifiedBlockFormat(
            other
        ),TextBlockFormat(
            other
        ),ParagraphFormat(
            other
        ),FontFormat(
              other
        ),ListParagraphFormatBase(
            other
        ) {
        currentBulletStyle = other.currentBulletStyle;
    }


    UnorderedListParagraphFormat::~UnorderedListParagraphFormat() {}


    FormatPointer UnorderedListParagraphFormat::clone() const {
        return FormatPointer(new UnorderedListParagraphFormat(*this));
    }


    Format* UnorderedListParagraphFormat::creator(const QString&) {
        return new UnorderedListParagraphFormat();
    }


    QString UnorderedListParagraphFormat::typeName() const {
        return formatName;
    }


    Format::Capabilities UnorderedListParagraphFormat::capabilities() const {
        return ListParagraphFormatBase::capabilities() << formatName;
    }


    void UnorderedListParagraphFormat::setBulletStyle(const QString& newBulletStyle) {
        currentBulletStyle = newBulletStyle;
        reportFormatUpdated();
    }


    QString UnorderedListParagraphFormat::bulletStyle() const {
        return currentBulletStyle;
    }


    QString UnorderedListParagraphFormat::toString() const {
        QString escaped = currentBulletStyle;
        int     index   = escaped.indexOf("'");
        while (index >= 0) {
            QString left  = escaped.left(index);
            QString right = escaped.mid(index);

            escaped = left + "\\" + right;
            index = escaped.indexOf("'", index + 2);
        }

        return QString("%1,'%2'").arg(ListParagraphFormatBase::toString()).arg(escaped);
    }


    QString UnorderedListParagraphFormat::toCss() const {
        return ParagraphFormat::toCss();
    }


    QString UnorderedListParagraphFormat::toCssEntry(
            const QString& className,
            const QString& tag,
            const QString& id
        ) const {
        QString selectorBullet;
        QString selectorNoBullet;

        if (!tag.isEmpty()) {
            selectorBullet   = tag;
            selectorNoBullet = tag;
        }

        if (!className.isEmpty()) {
            selectorBullet   += QString(".%1_bullet").arg(className);
            selectorNoBullet += QString(".%1_no_bullet").arg(className);
        }

        if (!id.isEmpty()) {
            selectorBullet += QString("#%1").arg(id);
            selectorNoBullet += QString("#%1").arg(id);
        }

        UnicodeValue unicodeValue = currentBulletStyle.at(0).unicode();
        QString beforeCssBullet = QString("content: '\\%1'; float: left; display: inline-block; width: %2pt; ")
                                  .arg(unicodeValue, 4, 16, QChar('0'))
                                  .arg(listIndentation());

        QString beforeCssNoBullet = QString("content: '\\200c'; float: left; display: inline-block; width: %1pt; ")
                                    .arg(listIndentation());

        QString css = ListParagraphFormatBase::toCss();

        QString result = QString("%1 { %2}\n%3:before { %4}\n%5 { %6}\n%7:before { %8}\n")
                         .arg(selectorBullet)
                         .arg(css)
                         .arg(selectorBullet)
                         .arg(beforeCssBullet)
                         .arg(selectorNoBullet)
                         .arg(css)
                         .arg(selectorNoBullet)
                         .arg(beforeCssNoBullet);

        return result;
    }



    const QList<UnorderedListParagraphFormat::StandardBullet>& UnorderedListParagraphFormat::standardBullets() {
        if (currentStandardBullets.isEmpty()) {
            const StandardBulletData* bullet = standardBulletData;
            while (bullet->unicodeValue) {
                currentStandardBullets << UnorderedListParagraphFormat::StandardBullet(
                    bullet->unicodeValue,
                    bullet->description,
                    bullet->category,
                    bullet->htmlCategory
                );

                ++bullet;
            }
        }

        return currentStandardBullets;
    }


    void UnorderedListParagraphFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> /* formats */,
            ProgramFile&                    /* programFile */
        ) const {
        if (currentBulletStyle != defaultBulletStyle) {
            attributes.append("bullet", firstLineLeftIndentation());
        }
    }


    void UnorderedListParagraphFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& /* formats */,
            ProgramFile&               /* programFile */,
            unsigned                   /* xmlVersion */
        ) {
        if (!reader->hasError()) {
            if (attributes.hasAttribute("bullet")) {
                setBulletStyle(attributes.value<QString>("bullet"));
            } else {
                setBulletStyle(defaultBulletStyle);
            }
        }
    }


    void UnorderedListParagraphFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        BlockFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        JustifiedBlockFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        TextBlockFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        ParagraphFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        FontFormat::writeAddImmediateAttributes(attributes, formats, programFile);
        ListParagraphFormatBase::writeAddImmediateAttributes(attributes, formats, programFile);
        UnorderedListParagraphFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void UnorderedListParagraphFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        BlockFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        JustifiedBlockFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        TextBlockFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        ParagraphFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        FontFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        ListParagraphFormatBase::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
        UnorderedListParagraphFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }
}
