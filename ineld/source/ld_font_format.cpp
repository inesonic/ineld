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
* This file implements the \ref Ld::FontFormat class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QString>
#include <QSharedPointer>
#include <QColor>
#include <QFont>
#include <QSet>

#include <cassert>

#include <util_hash_functions.h>

#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_program_file.h"
#include "ld_font_format.h"

/***********************************************************************************************************************
 * Static functions:
 */

static bool operator<(const QColor& font1, const QColor& font2) {
    bool lessThan = false;

    if (font1.cyanF() < font2.cyanF()) {
        lessThan = true;
    } else if (font1.cyanF() == font2.cyanF()) {
        if (font1.magentaF() < font2.magentaF()) {
            lessThan = true;
        } else if (font1.magentaF() == font2.magentaF()) {
            if (font1.yellowF() < font2.yellowF()) {
                lessThan = true;
            } else if (font1.yellowF() == font2.yellowF()) {
                if (font1.blackF() < font2.blackF()) {
                    lessThan = true;
                } else if (font1.blackF() == font2.blackF()) {
                    if (font1.alphaF() < font2.alphaF()) {
                        lessThan = true;
                    }
                }
            }
        }
    }

    return lessThan;
}

/***********************************************************************************************************************
 * Ld::FontFormat::Aggregation:
 */

namespace Ld {
    FontFormat::Aggregation::Aggregation() {
        clear();
    }


    FontFormat::Aggregation::~Aggregation() {}


    Format::Aggregation* FontFormat::Aggregation::creator(const QString&) {
        return new FontFormat::Aggregation;
    }


    QString FontFormat::Aggregation::typeName() const {
        return FontFormat::formatName;
    }


    void FontFormat::Aggregation::clear() {
        Format::Aggregation::clear();

        currentFontFamilies.clear();
        currentFontSizes.clear();
        currentWeights.clear();
        currentItalics.clear();
        currentUnderline.clear();
        currentOverline.clear();
        currentStrikeOut.clear();
        currentFontColor.clear();
        currentQtFonts.clear();
        currentFontBackgroundColor.clear();
        currentLetterSpacing.clear();
    }


    bool FontFormat::Aggregation::addFormat(FormatPointer formatInstance, bool includeExisting) {
        bool success = false;

        QSharedPointer<FontFormat> fontFormat = formatInstance.dynamicCast<FontFormat>();
        if (!fontFormat.isNull()) {
            success = Format::Aggregation::addFormat(formatInstance, includeExisting);

            if (success) {
                currentFontFamilies        << fontFormat->family();
                currentFontSizes           << fontFormat->fontSize();
                currentWeights             << fontFormat->fontWeight();
                currentItalics             << fontFormat->italics();
                currentUnderline           << fontFormat->underline();
                currentOverline            << fontFormat->overline();
                currentStrikeOut           << fontFormat->strikeout();
                currentLetterSpacing       << fontFormat->letterSpacing();
                currentQtFonts             << fontFormat->toQFont();
                currentFontColor           << fontFormat->fontColor();
                currentFontBackgroundColor << fontFormat->fontBackgroundColor();
            }
        }

        return success;
    }


    const QSet<QString>& FontFormat::Aggregation::families() const {
        return currentFontFamilies;
    }


    const QSet<unsigned>& FontFormat::Aggregation::sizes() const {
        return currentFontSizes;
    }


    const QSet<FontFormat::Weight>& FontFormat::Aggregation::weights() const {
        return currentWeights;
    }


    const Format::BooleanAggregation& FontFormat::Aggregation::italics() const {
        return currentItalics;
    }


    const Format::BooleanAggregation& FontFormat::Aggregation::underline() const {
        return currentUnderline;
    }


    const Format::BooleanAggregation& FontFormat::Aggregation::overline() const {
        return currentOverline;
    }


    const Format::BooleanAggregation& FontFormat::Aggregation::strikeOut() const {
        return currentStrikeOut;
    }


    const QSet<float>& FontFormat::Aggregation::letterSpacing() const {
        return currentLetterSpacing;
    }


    const QSet<QFont>& FontFormat::Aggregation::qtFonts() const {
        return currentQtFonts;
    }


    const QSet<QColor>& FontFormat::Aggregation::fontColor() const {
        return currentFontColor;
    }


    const QSet<QColor>& FontFormat::Aggregation::fontBackgroundColor() const {
        return currentFontBackgroundColor;
    }
}
/***********************************************************************************************************************
 * Ld::FontFormat::ImmutableData:
 */

namespace Ld {
    class FontFormat::ImmutableData {
        public:
            ImmutableData();

            ImmutableData(
                const QString&     family,
                unsigned           pointSize = FontFormat::defaultFontSizePoints,
                FontFormat::Weight weight    = FontFormat::Weight::NORMAL,
                bool               italic    = false
            );

            ImmutableData(const FontFormat::ImmutableData& other);

            ~ImmutableData();

            void setFamily(const QString& newFamily);

            void setFamily(const char* newFamily);

            QString family() const;

            void setFontSize(unsigned newPointSize);

            unsigned fontSize() const;

            void setFontWeight(FontFormat::Weight newWeight);

            FontFormat::Weight fontWeight() const;

            void setItalics(bool nowItalics = true);

            bool italics() const;

            void setUnderline(bool nowUnderlined = true);

            bool underline() const;

            void setOverline(bool nowOverlined = true);

            bool overline() const;

            void setStrikeout(bool nowStrikeout = true);

            bool strikeout() const;

            void setLetterSpacing(float newSpacing);

            float letterSpacing() const;

            void clearFormatting();

            void setFontColor(const QColor& newFontColor);

            QColor fontColor() const;

            void setFontBackgroundColor(const QColor& newFontBackgroundColor);

            QColor fontBackgroundColor() const;

            FontFormat::ImmutableData& operator=(const FontFormat::ImmutableData& other);

            bool operator==(const FontFormat::ImmutableData& other) const;

            bool operator!=(const FontFormat::ImmutableData& other) const;

            bool operator<(const FontFormat::ImmutableData& other) const;

        private:
            void configure(const QString& family, unsigned pointSize, FontFormat::Weight weight, bool italic);

            QString              currentFamily;
            int                  currentPointSize;
            FontFormat::Weight   currentWeight;
            bool                 currentItalicized;
            bool                 currentUnderlined;
            bool                 currentOverlined;
            bool                 currentStrikeout;
            float                currentLetterSpacing;
            QColor               currentFontColor;
            QColor               currentBackgroundColor;
    };


    FontFormat::ImmutableData::ImmutableData() {
        configure(
            QString(FontFormat::defaultFontFamily),
            FontFormat::defaultFontSizePoints,
            FontFormat::defaultFontWeight,
            false
        );
    }


    FontFormat::ImmutableData::ImmutableData(
            const QString&          family,
            unsigned                pointSize,
            FontFormat::Weight weight,
            bool                    italic
        ) {
        configure(family, pointSize, weight, italic);
    }


    FontFormat::ImmutableData::ImmutableData(const FontFormat::ImmutableData& other) {
        currentFamily          = other.currentFamily;
        currentPointSize       = other.currentPointSize;
        currentWeight          = other.currentWeight;
        currentItalicized      = other.currentItalicized;
        currentUnderlined      = other.currentUnderlined;
        currentOverlined       = other.currentOverlined;
        currentStrikeout       = other.currentStrikeout;
        currentLetterSpacing   = other.currentLetterSpacing;
        currentFontColor       = other.currentFontColor;
        currentBackgroundColor = other.currentBackgroundColor;
    }


    FontFormat::ImmutableData::~ImmutableData() {}


    void FontFormat::ImmutableData::setFamily(const QString& newFamily) {
        currentFamily = newFamily;
    }


    void FontFormat::ImmutableData::setFamily(const char* newFamily) {
        setFamily(QString(newFamily));
    }


    QString FontFormat::ImmutableData::family() const {
        return currentFamily;
    }


    void FontFormat::ImmutableData::setFontSize(unsigned newPointSize) {
        currentPointSize = newPointSize;
    }


    unsigned FontFormat::ImmutableData::fontSize() const {
        return currentPointSize;
    }


    void FontFormat::ImmutableData::setFontWeight(FontFormat::Weight newWeight) {
        currentWeight = newWeight;
    }


    FontFormat::Weight FontFormat::ImmutableData::fontWeight() const {
        return currentWeight;
    }


    void FontFormat::ImmutableData::setItalics(bool nowItalics) {
        currentItalicized = nowItalics;
    }


    bool FontFormat::ImmutableData::italics() const {
        return currentItalicized;
    }


    void FontFormat::ImmutableData::setUnderline(bool nowUnderlined) {
        currentUnderlined = nowUnderlined;
    }


    bool FontFormat::ImmutableData::underline() const {
        return currentUnderlined;
    }


    void FontFormat::ImmutableData::setOverline(bool nowOverlined) {
        currentOverlined = nowOverlined;
    }


    bool FontFormat::ImmutableData::overline() const {
        return currentOverlined;
    }


    void FontFormat::ImmutableData::setStrikeout(bool nowStrikeout) {
        currentStrikeout = nowStrikeout;
    }


    bool FontFormat::ImmutableData::strikeout() const {
        return currentStrikeout;
    }


    void FontFormat::ImmutableData::setLetterSpacing(float newSpacing) {
        currentLetterSpacing = newSpacing;
    }


    float FontFormat::ImmutableData::letterSpacing() const {
        return currentLetterSpacing;
    }


    void FontFormat::ImmutableData::clearFormatting() {
        currentWeight      = FontFormat::defaultFontWeight;
        currentItalicized  = false;
        currentUnderlined  = false;
        currentOverlined   = false;
        currentStrikeout   = false;
    }


    void FontFormat::ImmutableData::setFontColor(const QColor& newFontColor) {
        currentFontColor = newFontColor;
    }


    QColor FontFormat::ImmutableData::fontColor() const {
        return currentFontColor;
    }


    void FontFormat::ImmutableData::setFontBackgroundColor(const QColor& newFontBackgroundColor) {
        currentBackgroundColor = newFontBackgroundColor;
    }


    QColor FontFormat::ImmutableData::fontBackgroundColor() const {
        return currentBackgroundColor;
    }


    FontFormat::ImmutableData& FontFormat::ImmutableData::operator=(
            const FontFormat::ImmutableData& other
        ) {
        currentFamily          = other.currentFamily;
        currentPointSize       = other.currentPointSize;
        currentWeight          = other.currentWeight;
        currentItalicized      = other.currentItalicized;
        currentUnderlined      = other.currentUnderlined;
        currentOverlined       = other.currentOverlined;
        currentStrikeout       = other.currentStrikeout;
        currentLetterSpacing   = other.currentLetterSpacing;
        currentFontColor       = other.currentFontColor;
        currentBackgroundColor = other.currentBackgroundColor;

        return *this;
    }


    bool FontFormat::ImmutableData::operator==(const FontFormat::ImmutableData& other) const {
        return (
               currentFamily          == other.currentFamily
            && currentPointSize       == other.currentPointSize
            && currentWeight          == other.currentWeight
            && currentItalicized      == other.currentItalicized
            && currentUnderlined      == other.currentUnderlined
            && currentOverlined       == other.currentOverlined
            && currentStrikeout       == other.currentStrikeout
            && currentLetterSpacing   == other.currentLetterSpacing
            && currentFontColor       == other.currentFontColor
            && currentBackgroundColor == other.currentBackgroundColor
        );
    }


    bool FontFormat::ImmutableData::operator!=(const FontFormat::ImmutableData& other) const {
        return (
               currentFamily          != other.currentFamily
            || currentPointSize       != other.currentPointSize
            || currentWeight          != other.currentWeight
            || currentItalicized      != other.currentItalicized
            || currentUnderlined      != other.currentUnderlined
            || currentOverlined       != other.currentOverlined
            || currentStrikeout       != other.currentStrikeout
            || currentLetterSpacing   != other.currentLetterSpacing
            || currentFontColor       != other.currentFontColor
            || currentBackgroundColor != other.currentBackgroundColor
        );
    }


    bool FontFormat::ImmutableData::operator<(const FontFormat::ImmutableData& other) const {
        bool lessThan = false;
        if (currentFamily < other.currentFamily) {
            lessThan = true;
        } else if (currentFamily == other.currentFamily) {
            if (currentPointSize < other.currentPointSize) {
                lessThan = true;
            } else if (currentPointSize == other.currentPointSize) {
                if (static_cast<unsigned>(currentWeight) < static_cast<unsigned>(other.currentWeight)) {
                    lessThan = true;
                } else if (static_cast<unsigned>(currentWeight) == static_cast<unsigned>(other.currentWeight)) {
                    if (currentItalicized < other.currentItalicized) {
                        lessThan = true;
                    } else if (currentItalicized == other.currentItalicized) {
                        if (currentUnderlined < other.currentUnderlined) {
                            lessThan = true;
                        } else if (currentUnderlined == other.currentUnderlined) {
                            if (currentOverlined < other.currentOverlined) {
                                lessThan = true;
                            } else if (currentOverlined == other.currentOverlined) {
                                if (currentStrikeout < other.currentStrikeout) {
                                    lessThan = true;
                                } else if (currentStrikeout == other.currentStrikeout) {
                                    if (currentLetterSpacing < other.currentLetterSpacing) {
                                        lessThan = true;
                                    } else if (currentLetterSpacing == other.currentLetterSpacing) {
                                        if (currentFontColor < other.currentFontColor) {
                                            lessThan = true;
                                        } else if (currentFontColor == other.currentFontColor) {
                                            if (currentBackgroundColor < other.currentBackgroundColor) {
                                                lessThan = true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        return lessThan;
    }


    void FontFormat::ImmutableData::configure(
            const QString&     family,
            unsigned           pointSize,
            FontFormat::Weight weight,
            bool               italic
        ) {
        currentFamily          = family;
        currentPointSize       = pointSize;
        currentWeight          = weight;
        currentItalicized      = italic;
        currentUnderlined      = false;
        currentOverlined       = false;
        currentStrikeout       = false;
        currentLetterSpacing   = FontFormat::defaultLetterSpacing;
        currentFontColor       = FontFormat::defaultFontColor;
        currentBackgroundColor = FontFormat::defaultFontBackgroundColor;
    }

};

/***********************************************************************************************************************
 * Ld::FontFormat::MutableData
 */

namespace Ld {
    class FontFormat::MutableData {
        public:
            MutableData();

            MutableData(const Ld::FontFormat::MutableData& other);

            ~MutableData();

            void increaseReferenceCount();

            bool decreaseReferenceCount();

            bool canDelete() const;

            QFont qFont(const Ld::FontFormat::ImmutableData& immutableData);

            FontFormat::MutableData& operator=(const FontFormat::MutableData& other);

        private:
            unsigned long referenceCount;
            QFont*        qFontInstance;
    };


    FontFormat::MutableData::MutableData() {
        referenceCount = 1;
        qFontInstance  = nullptr;
    }


    FontFormat::MutableData::MutableData(const FontFormat::MutableData& other) {
        referenceCount = other.referenceCount;

        if (other.qFontInstance == nullptr) {
            qFontInstance = nullptr;
        } else {
            qFontInstance  = new QFont(*other.qFontInstance);
        }
    }


    FontFormat::MutableData::~MutableData() {
        if (qFontInstance != nullptr) {
            delete qFontInstance;
        }
    }


    void FontFormat::MutableData::increaseReferenceCount() {
        ++referenceCount;
    }


    bool FontFormat::MutableData::decreaseReferenceCount() {
        assert(referenceCount > 0);
        --referenceCount;

        return referenceCount == 0;
    }


    bool FontFormat::MutableData::canDelete() const {
        return referenceCount == 0;
    }


    QFont FontFormat::MutableData::qFont(const FontFormat::ImmutableData& immutableData) {
        if (qFontInstance == nullptr) {
            QFont* fontInstance = new QFont(
                immutableData.family(),
                immutableData.fontSize(),
                static_cast<int>(immutableData.fontWeight()),
                immutableData.italics()
            );

            fontInstance->setUnderline(immutableData.underline());
            fontInstance->setOverline(immutableData.overline());
            fontInstance->setStrikeOut(immutableData.strikeout());
            fontInstance->setLetterSpacing(QFont::PercentageSpacing, (1.0 + immutableData.letterSpacing()) * 100.0);

            qFontInstance = fontInstance;
        }

        return *qFontInstance;
    }


    FontFormat::MutableData& FontFormat::MutableData::operator=(const FontFormat::MutableData& other) {
        referenceCount = other.referenceCount;

        if (other.qFontInstance == nullptr) {
            qFontInstance = nullptr;
        } else {
            qFontInstance  = new QFont(*other.qFontInstance);
        }

        return *this;
    }
}

/***********************************************************************************************************************
 * Ld::FontFormat
 */

namespace Ld {
    const QString              FontFormat::formatName("FontFormat");

    const unsigned             FontFormat::minimumFontSize            = 8;
    const unsigned             FontFormat::maximumFontSize            = 144;
    const float                FontFormat::minimumLetterSpacing       = -0.05F;
    const float                FontFormat::maximumLetterSpacing       = 0.2F;

    const char                 FontFormat::defaultFontFamily[]        = "Helvetica";
    const unsigned             FontFormat::defaultFontSizePoints      = 12;
    const FontFormat::Weight   FontFormat::defaultFontWeight          = FontFormat::Weight::NORMAL;
    const float                FontFormat::defaultLetterSpacing       = 0.0;
    const QColor               FontFormat::defaultFontColor;           // Invalid color -- default
    const QColor               FontFormat::defaultFontBackgroundColor; // Invalid color -- no background.

    QMap<FontFormat::ImmutableData, FontFormat::MutableData> FontFormat::fontFormats;

    FontFormat::FontFormat() {
        configure(defaultFontFamily, defaultFontSizePoints, defaultFontWeight, false);
    }


    FontFormat::FontFormat(
            const QString&     family,
            unsigned           pointSize,
            FontFormat::Weight weight,
            bool               italic
        ) {
        configure(family, pointSize, weight, italic);
    }


    FontFormat::FontFormat(
            const char*        family,
            unsigned           pointSize,
            FontFormat::Weight weight,
            bool               italic
        ) {
        configure(QString(family), pointSize, weight, italic);
    }


    FontFormat::FontFormat(const FontFormat& other):Format(other) {
        iterator = other.iterator;
        iterator.value().increaseReferenceCount();
    }


    FontFormat::~FontFormat() {
        bool canDelete = iterator.value().decreaseReferenceCount();

        if (canDelete) {
            fontFormats.erase(iterator);
        }
    }


    FormatPointer FontFormat::clone() const {
        return FormatPointer(new FontFormat(*this));
    }


    Format* FontFormat::creator(const QString&) {
        return new FontFormat();
    }


    QString FontFormat::typeName() const {
        return formatName;
    }


    bool FontFormat::isValid() const {
        return true;
    }


    bool FontFormat::isDefault() const {
        return (
               family() == defaultFontFamily
            && fontSize() == defaultFontSizePoints
            && fontWeight() == defaultFontWeight
            && italics() == false
            && underline() == false
            && overline() == false
            && strikeout() == false
            && letterSpacing() == defaultLetterSpacing
            && fontColor() == defaultFontColor
            && fontBackgroundColor() == defaultFontBackgroundColor
        );
    }


    bool FontFormat::isNotDefault() const {
        return !isDefault();
    }


    Format::Capabilities FontFormat::capabilities() const {
        return Format::capabilities() << formatName;
    }


    void FontFormat::setFamily(const QString& newFamily) {
        if (family() != newFamily) {
            ImmutableData data = obtainImmutableFontFormatData();
            data.setFamily(newFamily);
            updateFontFormatDatabase(data);

            reportFormatUpdated();
        }
    }


    void FontFormat::setFamily(const char* newFamily) {
        setFamily(QString(newFamily));
    }


    QString FontFormat::family() const {
        return iterator.key().family();
    }


    void FontFormat::setFontSize(unsigned newPointSize) {
        if (fontSize() != newPointSize) {
            ImmutableData data = obtainImmutableFontFormatData();
            data.setFontSize(newPointSize);
            updateFontFormatDatabase(data);

            reportFormatUpdated();
        }
    }


    unsigned FontFormat::fontSize() const {
        return iterator.key().fontSize();
    }


    void FontFormat::setFontWeight(FontFormat::Weight newWeight) {
        if (fontWeight() != newWeight) {
            ImmutableData data = obtainImmutableFontFormatData();
            data.setFontWeight(newWeight);
            updateFontFormatDatabase(data);

            reportFormatUpdated();
        }
    }


    FontFormat::Weight FontFormat::fontWeight() const {
        return iterator.key().fontWeight();
    }


    void FontFormat::setItalics(bool nowItalics) {
        if (italics() != nowItalics) {
            ImmutableData data = obtainImmutableFontFormatData();
            data.setItalics(nowItalics);
            updateFontFormatDatabase(data);

            reportFormatUpdated();
        }
    }


    bool FontFormat::italics() const {
        return iterator.key().italics();
    }


    void FontFormat::setUnderline(bool nowUnderlined) {
        if (underline() != nowUnderlined) {
            ImmutableData data = obtainImmutableFontFormatData();
            data.setUnderline(nowUnderlined);
            updateFontFormatDatabase(data);

            reportFormatUpdated();
        }
    }


    bool FontFormat::underline() const {
        return iterator.key().underline();
    }


    void FontFormat::setOverline(bool nowOverlined) {
        if (overline() != nowOverlined) {
            ImmutableData data = obtainImmutableFontFormatData();
            data.setOverline(nowOverlined);
            updateFontFormatDatabase(data);

            reportFormatUpdated();
        }
    }


    bool FontFormat::overline() const {
        return iterator.key().overline();
    }


    void FontFormat::setStrikeout(bool nowStrikeout) {
        if (strikeout() != nowStrikeout) {
            ImmutableData data = obtainImmutableFontFormatData();
            data.setStrikeout(nowStrikeout);
            updateFontFormatDatabase(data);

            reportFormatUpdated();
        }
    }


    bool FontFormat::strikeout() const {
        return iterator.key().strikeout();
    }


    void FontFormat::setLetterSpacing(float newSpacing) {
        if (letterSpacing() != newSpacing) {
            ImmutableData data = obtainImmutableFontFormatData();
            data.setLetterSpacing(newSpacing);
            updateFontFormatDatabase(data);

            reportFormatUpdated();
        }
    }


    float FontFormat::letterSpacing() const {
        return iterator.key().letterSpacing();
    }


    void FontFormat::clearFormatting() {
        ImmutableData data = obtainImmutableFontFormatData();
        data.clearFormatting();
        updateFontFormatDatabase(data);

        reportFormatUpdated();
    }


    void FontFormat::setFontColor(const QColor& newFontColor) {
        if (fontColor() != newFontColor) {
            ImmutableData data = obtainImmutableFontFormatData();
            data.setFontColor(newFontColor);
            updateFontFormatDatabase(data);

            reportFormatUpdated();
        }
    }


    QColor FontFormat::fontColor() const {
        return iterator.key().fontColor();
    }


    void FontFormat::setFontBackgroundColor(const QColor& newFontBackgroundColor) {
        if (fontBackgroundColor() != newFontBackgroundColor) {
            ImmutableData data = obtainImmutableFontFormatData();
            data.setFontBackgroundColor(newFontBackgroundColor);
            updateFontFormatDatabase(data);

            reportFormatUpdated();
        }
    }


    QColor FontFormat::fontBackgroundColor() const {
        return iterator.key().fontBackgroundColor();
    }


    QString FontFormat::toString() const {
        QString modifiers;
        if (iterator.key().italics()) {
            modifiers += "I";
        }

        if (iterator.key().underline()) {
            modifiers += "U";
        }

        if (iterator.key().overline()) {
            modifiers += "O";
        }

        if (iterator.key().strikeout()) {
            modifiers += "S";
        }

        QColor fontColor        = iterator.key().fontColor();
        QString fontColorString = fontColor.isValid() ? fontColor.name(QColor::NameFormat::HexArgb) : QString("-");

        QColor backgroundColor        = iterator.key().fontBackgroundColor();
        QString backgroundColorString =   backgroundColor.isValid()
                                        ? backgroundColor.name(QColor::NameFormat::HexArgb)
                                        : QString("-");

        QString result = QString("%1,%2,%3,%4,%5,%6,%7,%8")
                         .arg(typeName())
                         .arg(iterator.key().family())
                         .arg(iterator.key().fontSize())
                         .arg(static_cast<double>(iterator.key().fontWeight()) / 100.0)
                         .arg(modifiers)
                         .arg(iterator.key().letterSpacing())
                         .arg(fontColorString)
                         .arg(backgroundColorString);

        return result;
    }


    QString FontFormat::toCss() const {
        QString result = QString("font-family: %1; font-size: %2pt; ")
                         .arg(iterator.key().family())
                         .arg(iterator.key().fontSize());

        Weight fontWeight = iterator.key().fontWeight();
        switch (fontWeight) {
            case FontFormat::Weight::THIN:        { result += "font-weight: 100; "; break; }
            case FontFormat::Weight::EXTRA_LIGHT: { result += "font-weight: 200; "; break; }
            case FontFormat::Weight::LIGHT:       { result += "font-weight: 300; "; break; }
            case FontFormat::Weight::NORMAL:      {                                 break; }
            case FontFormat::Weight::MEDIUM:      { result += "font-weight: 500; "; break; }
            case FontFormat::Weight::DEMI_BOLD:   { result += "font-weight: 600; "; break; }
            case FontFormat::Weight::BOLD:        { result += "font-weight: 700; "; break; }
            case FontFormat::Weight::EXTRA_BOLD:  { result += "font-weight: 800; "; break; }
            case FontFormat::Weight::BLACK:       { result += "font-weight: 900; "; break; }
            default:                              {                                 break; }
        }

        if (iterator.key().italics()) {
            result += "font-style: italics; ";
        }

        QString decoration;
        if (iterator.key().underline()) {
            decoration = "underline ";
        }

        if (iterator.key().overline()) {
            decoration += "overline ";
        }

        if (iterator.key().strikeout()) {
            decoration += "line-through ";
        }

        if (!decoration.isEmpty()) {
            result += QString("text-decoration: %1; ").arg(decoration.trimmed());
        }

        float letterSpacing = iterator.key().letterSpacing();
        if (letterSpacing != 0) {
            result += QString("letter-spacing: %1em; ").arg(letterSpacing);
        }

        QColor fontColor = iterator.key().fontColor();
        if (fontColor.isValid()) {
            result += QString("color: rgba(%1,%2,%3,%4; ")
                      .arg(fontColor.red())
                      .arg(fontColor.green())
                      .arg(fontColor.blue())
                      .arg(fontColor.alphaF());
        } else {
            result += QString("color: black; ");
        }

        QColor fontBackgroundColor = iterator.key().fontBackgroundColor();
        if (fontBackgroundColor.isValid()) {
            result += QString("background-color: rgba(%1,%2,%3,%4); ")
                      .arg(fontBackgroundColor.red())
                      .arg(fontBackgroundColor.green())
                      .arg(fontBackgroundColor.blue())
                      .arg(fontBackgroundColor.alphaF());
        }

        return result;
    }


    QFont FontFormat::toQFont() {
        return iterator.value().qFont(iterator.key());
    }


    FontFormat& FontFormat::operator=(const FontFormat& other) {
        if (iterator != other.iterator) {
            other.iterator.value().increaseReferenceCount();

            bool canDelete = iterator.value().decreaseReferenceCount();
            if (canDelete) {
                fontFormats.erase(iterator);
            }

            iterator = other.iterator;

            reportFormatUpdated();
        }

        return *this;
    }


    bool FontFormat::operator==(const FontFormat& other) const {
        return iterator == other.iterator;
    }


    bool FontFormat::operator!=(const FontFormat& other) const {
        return iterator != other.iterator;
    }


    void FontFormat::writeAddImmediateAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer>,
            ProgramFile&
        ) const {
        if (family() != defaultFontFamily) {
            attributes.append("family", family());
        }

        if (fontSize() != defaultFontSizePoints) {
            attributes.append("size", fontSize());
        }

        if (fontWeight() != defaultFontWeight) {
            attributes.append("weight", static_cast<unsigned>(fontWeight()) / 100.0);
        }

        QColor color = fontColor();
        if (color != defaultFontColor) {
            attributes.append("color", color);
        }

        color = fontBackgroundColor();
        if (color != defaultFontBackgroundColor) {
            attributes.append("bgcolor", color);
        }

        if (italics()) {
            attributes.append("italics", true);
        }

        if (underline()) {
            attributes.append("underline", true);
        }

        if (overline()) {
            attributes.append("overline", true);
        }

        if (strikeout()) {
            attributes.append("strikeout", true);
        }

        if (letterSpacing() != defaultLetterSpacing) {
            attributes.append("spacing", letterSpacing());
        }
    }


    void FontFormat::readImmediateAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier&,
            ProgramFile&,
            unsigned
        ) {
        bool ok;

        QString fontFamily = attributes.value<QString>("family", defaultFontFamily);

        unsigned fontSize = attributes.value<unsigned>("size", defaultFontSizePoints, &ok);
        if (!ok) {
            QString tag = reader->qualifiedName().toString();
            QString attributeString = attributes.value<QString>("size");
            reader->raiseError(tr("Tag \"%1\", invalid font size \"%1\"").arg(tag).arg(attributeString));
        }

        double fractionalFontWeight = 0;
        if (!reader->hasError()) {
            double defaultFractionalWeight = static_cast<double>(defaultFontWeight) / 100.0;
            fractionalFontWeight = attributes.value<double>("weight", defaultFractionalWeight, &ok);
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                QString attributeString = attributes.value<QString>("weight");
                reader->raiseError(tr("Tag \"%1\", invalid font weight \"%1\"").arg(tag).arg(attributeString));
            }
        }

        QColor fontColor = defaultFontColor;
        if (!reader->hasError() && attributes.hasAttribute("color")) {
            fontColor = attributes.colorValue("color", &ok);
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                QString attributeString = attributes.value<QString>("color");
                reader->raiseError(tr("Tag \"%1\", invalid font color \"%1\"").arg(tag).arg(attributeString));
            }
        }

        QColor fontBackgroundColor = defaultFontBackgroundColor;
        if (!reader->hasError() && attributes.hasAttribute("bgcolor")) {
            fontBackgroundColor = attributes.colorValue("bgcolor", &ok);
            if (!ok) {
                QString tag             = reader->qualifiedName().toString();
                QString attributeString = attributes.value<QString>("color");
                reader->raiseError(
                    tr("Tag \"%1\" has invalid font background color \"%2\"").arg(tag).arg(attributeString)
                );
            }
        }

        bool fontItalics = false;
        if (!reader->hasError()) {
            fontItalics = attributes.value<bool>("italics", false, &ok);
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                QString value = attributes.value<QString>("italics");
                reader->raiseError(tr("Tag \"%1\" has invalid italics setting \"%2\"").arg(tag).arg(value));
            }
        }

        bool fontUnderline = false;
        if (!reader->hasError()) {
            fontUnderline = attributes.value<bool>("underline", false, &ok);
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                QString value = attributes.value<QString>("underline");
                reader->raiseError(tr("Tag \"%1\" has invalid underline setting \"%2\"").arg(tag).arg(value));
            }
        }

        bool fontStrikeout = false;
        if (!reader->hasError()) {
            fontStrikeout = attributes.value<bool>("strikeout", false, &ok);
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                QString value = attributes.value<QString>("strikeout");
                reader->raiseError(tr("Tag \"%1\" has invalid strikeout setting \"%2\"").arg(tag).arg(value));
            }
        }

        float letterSpacing = defaultLetterSpacing;
        if (!reader->hasError()) {
            letterSpacing = attributes.value<float>("spacing", defaultLetterSpacing, &ok);
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                QString value = attributes.value<QString>("spacing");
                reader->raiseError(tr("Tag \"%1\" has invalid letter spacing setting \"%2\"").arg(tag).arg(value));
            }
        }

        bool fontOverline = false;
        if (!reader->hasError()) {
            fontOverline = attributes.value<bool>("overline", false, &ok);
            if (!ok) {
                QString tag = reader->qualifiedName().toString();
                QString value = attributes.value<QString>("overline");
                reader->raiseError(tr("Tag \"%1\" has invalid overline setting \"%2\"").arg(tag).arg(value));
            }
        }

        if (!reader->hasError()) {
            setFamily(fontFamily);
            setFontSize(fontSize);
            setFontWeight(static_cast<Weight>(static_cast<unsigned>(fractionalFontWeight * 100.0 + 0.5)));
            setFontColor(fontColor);
            setFontBackgroundColor(fontBackgroundColor);
            setItalics(fontItalics);
            setUnderline(fontUnderline);
            setOverline(fontOverline);
            setStrikeout(fontStrikeout);
            setLetterSpacing(letterSpacing);
        }
    }


    void FontFormat::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        FontFormat::writeAddImmediateAttributes(attributes, formats, programFile);
    }


    void FontFormat::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        FontFormat::readImmediateAttributes(reader, attributes, formats, programFile, xmlVersion);
    }


    void FontFormat::configure(
            const QString&     family,
            unsigned           pointSize,
            FontFormat::Weight weight,
            bool               italic
        ) {
        ImmutableData data(family, pointSize, weight, italic);
        updateFontFormatDatabase(data);
    }


    FontFormat::ImmutableData FontFormat::obtainImmutableFontFormatData() {
        assert(iterator != fontFormats.end());

        ImmutableData data = iterator.key();

        bool canDelete = iterator.value().decreaseReferenceCount();
        if (canDelete) {
            fontFormats.erase(iterator);
            iterator = fontFormats.end();
        }

        return data;
    }


    void FontFormat::updateFontFormatDatabase(FontFormat::ImmutableData& newFontData) {
        QMap<ImmutableData, MutableData>::iterator it = fontFormats.find(newFontData);

        if (it == fontFormats.end()) {
            iterator = fontFormats.insert(newFontData, FontFormat::MutableData());
        } else {
            it.value().increaseReferenceCount();
            iterator = it;
        }
    }


    FontFormat::Weight toFontFormatWeight(QFont::Weight qFontWeight) {
        return static_cast<FontFormat::Weight>(qFontWeight);
    }


    QFont::Weight toQFontWeight(FontFormat::Weight fontWeight) {
        return static_cast<QFont::Weight>(fontWeight);
    }


    Util::HashResult qHash(FontFormat::Weight weight, Util::HashSeed seed) {
        return ::qHash(static_cast<unsigned>(weight), seed);
    }


    Util::HashResult qHash(const FontFormat& fontFormat, Util::HashSeed seed) {
        return ::qHash(static_cast<const void*>(&(fontFormat.iterator.key())), seed);
    }
}
