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
* This header defines the \ref Ld::ChartLineStyle class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CHART_LINE_STYLE_H
#define LD_CHART_LINE_STYLE_H

#include <QString>
#include <QtGlobal>
#include <QColor>
#include <QSharedPointer>
#include <QSet>

#include <util_hash_functions.h>

#include "ld_common.h"
#include "ld_format_structures.h"
#include "ld_format_container.h"
#include "ld_xml_writer.h"
#include "ld_xml_reader.h"
#include "ld_xml_attributes.h"
#include "ld_format.h"

namespace Ld {
    /**
     * Class that tracks information about a chart line style. The class is designed to provide many of the capability
     * of a full format class.
     */
    class LD_PUBLIC_API ChartLineStyle:public virtual Format {
        public:
            /**
             * Enumeration of line styles.  Note that the values are designed to align with Qt::PenStyle
             */
            enum class LineStyle {
                /**
                 * Indicates no line.
                 */
                NO_LINE = static_cast<int>(Qt::PenStyle::NoPen),

                /**
                 * Indicates a solid line.
                 */
                SOLID_LINE = static_cast<int>(Qt::PenStyle::SolidLine),

                /**
                 * Indicates a dashed line.
                 */
                DASHED_LINE = static_cast<int>(Qt::PenStyle::DashLine),

                /**
                 * Indicates a dotted line.
                 */
                DOTTED_LINE = static_cast<int>(Qt::PenStyle::DotLine),

                /**
                 * Indicates a dash-dotted line.
                 */
                DASH_DOTTED_LINE = static_cast<int>(Qt::PenStyle::DashDotLine),

                /**
                 * Indicates a dash-dotted-dotted line.
                 */
                DASH_DOTTED_DOTTED_LINE = static_cast<int>(Qt::PenStyle::DashDotDotLine)
            };

            /**
             * Class you can use to track an aggregation of multiple \ref Ld::ChartFormat instances.
             */
            class LD_PUBLIC_API Aggregation:public virtual Format::Aggregation {
                public:
                    Aggregation();

                    ~Aggregation() override;

                    /**
                     * Method that returns the type name of the format tracked by this aggregation.
                     *
                     * \return Returns the format name for formats tied to this aggregation.
                     */
                    QString typeName() const override;

                    /**
                     * Method you can use to clear the aggregation.  Calling the base class instance will clear the
                     * underlying format database.
                     */
                    void clear() override;

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
                    bool addFormat(FormatPointer formatInstance, bool includeExisting = false) override;

                    /**
                     * Method you can use to obtain all the selected line styles.
                     *
                     * \return Returns a set containing all the supported line styles.
                     */
                    const QSet<LineStyle>& lineStyles() const;

                    /**
                     * Method you can use to obtain all the selected line widths.
                     */
                    const QSet<float>& lineWidths() const;

                    /**
                     * Method you can use to obtain all the selected line colors.
                     */
                    const QSet<QColor>& lineColors() const;

                private:
                    /**
                     * Set of current line styles.
                     */
                    QSet<LineStyle> currentLineStyles;

                    /**
                     * Set of current line widths.
                     */
                    QSet<float> currentLineWidths;

                    /**
                     * Set of current line colors.
                     */
                    QSet<QColor> currentLineColors;
            };

            /**
             * The typename for the format.
             */
            static const QString formatName;

            /**
             * Value indicating the default line style.
             */
            static const LineStyle defaultLineStyle;

            /**
             * Value indicating the default line width.
             */
            static const float defaultLineWidth;

            /**
             * Value indicating the default line color.
             */
            static const QColor defaultLineColor;

            /**
             * Constructor.
             *
             * \param[in] lineStyle The line style to assign to this format.
             *
             * \param[in] lineWidth The line width to assign to this format.
             *
             * \param[in] lineColor The line color to assign to this format.
             */
            ChartLineStyle(
                LineStyle     lineStyle = defaultLineStyle,
                float         lineWidth = defaultLineWidth,
                const QColor& lineColor = defaultLineColor
            );

            /**
             * Copy constructor.  Note that the copy constructor does not copy elements from the underlying
             * \ref Ld::Format class.  All format parameters are copied by the format are not tied to any element.
             *
             * \param[in] other The instance to be copied.
             */
            ChartLineStyle(const ChartLineStyle& other);

            ~ChartLineStyle() override;

            /**
             * Overloaded method that creates a clone of this class instance.  Note that the clone method does not copy
             * elements from the underlying \ref Ld::Format class.  All format parameters are copied by the format are
             * not tied to any element.
             *
             * \return Returns a deep copy of this class instance.
             */
            FormatPointer clone() const override;

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
             * different types of format data.
             *
             * \return Returns the format identifying name, in this case, "FontFormat".
             */
            QString typeName() const override;

            /**
             * Overloaded method that returns true to indicate that this format is valid.
             *
             * \return Returns true if the format is valid.  Returns false if the format is invalid.  This version
             *         always returns true.
             */
            bool isValid() const override;

            /**
             * Overloaded method that returns true if this format is set to the default values.
             *
             * \return Returns true if the format holds default values.  Returns false if the format does not hold
             *         default values.
             */
            bool isDefault() const;

            /**
             * Overloaded method that returns true if this format is not set to the default values.
             *
             * \return Returns true if the format holds non-default values.  Returns false if the format holds default
             *         values.
             */
            bool isNotDefault() const;

            /**
             * Method you can use to set the reported line style.
             *
             * \param[in] newLineStyle The newly reported line
             */
            void setLineStyle(LineStyle newLineStyle);

            /**
             * Method you can use to get the current line style.
             *
             * \return Returns the currently reported line style.
             */
            LineStyle lineStyle() const;

            /**
             * Method you can use to set the reported line width.
             *
             * \param[in] newLineWidth The new line width.
             */
            void setLineWidth(float newLineWidth);

            /**
             * Method you can use to get the current line widht.
             *
             * \return Returns the current line width.
             */
            float lineWidth() const;

            /**
             * Method you can use to set the reported line color.
             *
             * \param[in] newLineColor The new line color.
             */
            void setLineColor(const QColor& newLineColor);

            /**
             * Method you can use to obtain the reported line color.
             *
             * \return Returns the currently reported line color.
             */
            const QColor& lineColor() const;

            /**
             * Method that returns a description of this format as a string.  The string format should be in the form of
             * a comma separated group of fields with the first field being the type-name of the format.
             *
             * While not always supported, the string should be such that the format can be constructed later from the
             * string.
             *
             * This version excludes the format name from the generated string.
             *
             * \return Returns a description of this format as a string.
             */
            QString toString() const override;

            /**
             * Method that returns CSS content that can be used to emulate this format.  The returned string contains
             * no newline and does not include leading or trailing braces.
             *
             * \return Returns CSS content comparable to this format.
             */
            QString toCss() const override;

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            ChartLineStyle& operator=(const ChartLineStyle& other);

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instances are the same.  Returns false if the instances are different.
             */
            bool operator==(const ChartLineStyle& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instances are different.  Returns false if the instances are the same.
             */
            bool operator!=(const ChartLineStyle& other) const;

            /**
             * Method you can optionally overload to add additional attributes to the XML description of this element.
             * This method should write attributes for this class only and not the class's children.  To cleanly handle
             * multiple virtual inheritance, the Ld::Format::writeAddAttributes method should be coded to call the
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
            void writeAddAttributes(
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
            void readAttributes(
                QSharedPointer<XmlReader>  reader,
                const XmlAttributes&       attributes,
                const FormatsByIdentifier& formats,
                ProgramFile&               programFile,
                unsigned                   xmlVersion
            ) override;

            /**
             * Method you can optionally overload to add additional attributes to the XML description of this element.
             * This method should write attributes for this class only and not the class's children.  To cleanly handle
             * multiple virtual inheritance, the Ld::Format::writeAddAttributes method should be coded to call the
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

        private:
            /**
             * Method that converts a scaling mode to a string.
             *
             * \param[in] mode The mode to be converted.
             *
             * \return Returns the scaling mode converted to a string.
             */
            static QString toString(LineStyle mode);

            /**
             * Method that converts a string back to a line style.
             *
             * \param[in]  str The string to be converted.
             *
             * \param[out] ok  A pointer to an optional boolean value.  The boolean value will hold true on success
             *                 or false on error.
             */
            LineStyle toLineStyle(const QString& str, bool* ok = nullptr);

            /**
             * The current line style.
             */
            LineStyle currentLineStyle;

            /**
             * The current line width.
             */
            float currentLineWidth;

            /**
             * The current line color.
             */
            QColor currentLineColor;
    };

    /**
     * Hash function for the \ref Ld::ChartLineStyle::LineStyle enumeration.
     *
     * \param[in] value The value to be hashed.
     *
     * \param[in] seed  An optional seed to apply to the hash operation.
     *
     * \return Returns a hash for the provided line style.
     */
    LD_PUBLIC_API Util::HashResult qHash(ChartLineStyle::LineStyle value, Util::HashSeed seed = 0);

    /**
     * Hash function for the \ref Ld::ChartLineStyle class.
     *
     * \param[in] value The \ref Ld::ChartLineStyle to be hashed.
     *
     * \param[in] seed  An optional seed for the hash operation.
     *
     * \return Returns a hash for the scaling mode.
     */
    LD_PUBLIC_API Util::HashResult qHash(const ChartLineStyle& value, Util::HashSeed seed = 0);
}

#endif
