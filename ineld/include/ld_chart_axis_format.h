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
* This header defines the \ref Ld::ChartAxisFormat class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_CHART_AXIS_FORMAT_H
#define LD_CHART_AXIS_FORMAT_H

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
#include "ld_font_format.h"
#include "ld_chart_line_style.h"
#include "ld_format.h"

namespace Ld {
    /**
     * Class that tracks information about a chart axis. The class is designed to provide many of the capability of a
     * full format class.
     *
     * Note that the methods to load/save XML currently excluded the embedded fonts and line styles.  You must manually
     * save and load their state via a different mechanism.
     */
    class LD_PUBLIC_API ChartAxisFormat:public virtual Format {
        friend LD_PUBLIC_API Util::HashResult qHash(const ChartAxisFormat& value, Util::HashSeed seed);

        public:
            /**
             * Enumeration of supported axis scales.
             */
            enum class AxisScale {
                /**
                 * Indicates linear scaling (x * multiplier).
                 */
                LINEAR,

                /**
                 * Indicates log scaling (log(x * multiplier).
                 */
                LOG,

                /**
                 * Indicates dB power scale (10*log(x * multiplier))
                 */
                DB_10_LOG,

                /**
                 * Indicates dB voltage/current scale (20*log(x * multiplier))
                 */
                DB_20_LOG
            };

            /**
             * Enumeration of supported axis tick styles.
             */
            enum class TickStyle {
                /**
                 * Indicates no tick mark.
                 */
                NONE,

                /**
                 * Indicates outer tick marks.
                 */
                OUTER,

                /**
                 * Indicates inner tick marks.
                 */
                INNER,

                /**
                 * Indicates outer + inner tick marks.
                 */
                SPANNING,

                /**
                 * Indicates axis lines without an outer tick.
                 */
                LINE,

                /**
                 * Indicates axis lines with outer tick.
                 */
                LINE_AND_TICK
            };

            /**
             * Class you can use to track an aggregation of multiple \ref Ld::ChartAxisFormat instances.
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
                     * Method you can use to obtain the axis scaling.
                     *
                     * \return Returns a set holding the axis scaling values.
                     */
                    const QSet<AxisScale>& axisScales() const;

                    /**
                     * Method you can use to obtain the axis minimum values.
                     *
                     * \return Returns a set holding the axis minimum values.
                     */
                    const QSet<double>& minimumValues() const;

                    /**
                     * Method you can use to obtain the axis maximum values.
                     *
                     * \return Returns a set holding the axis maximum values.
                     */
                    const QSet<double>& maximumValues() const;

                    /**
                     * Method you can use to obtain the axis major step sizes.
                     *
                     * \return Returns a set holding the axis major step sizes.
                     */
                    const QSet<double>& majorStepSizes() const;

                    /**
                     * Method you can use to obtain the axis minor step sizes.
                     *
                     * \return Returns a set holding the axis minor step sizes.
                     */
                    const QSet<double>& minorStepSizes() const;

                    /**
                     * Method you can use to obtain the axis title font formats.
                     *
                     * \return Returns a set of axis title font formats.
                     */
                    const QSet<FontFormat>& titleFontFormats() const;

                    /**
                     * Method you can use to obtain the axis number font formats.
                     *
                     * \return Returns a set of axis number font formats.
                     */
                    const QSet<FontFormat>& numberFontFormats() const;

                    /**
                     * Method you can use to obtain the major tick styles.
                     *
                     * \return Returns a set of major tick styles.
                     */
                    const QSet<TickStyle>& majorTickStyles() const;

                    /**
                     * Method you can use to obtain the minor tick styles.
                     *
                     * \return Returns a set of minor tick styles.
                     */
                    const QSet<TickStyle>& minorTickStyles() const;

                    /**
                     * Method you can use to obtain the major tick line styles.
                     *
                     * \return Returns a set of major tick line styles.
                     */
                    const QSet<ChartLineStyle>& majorTickLineStyles() const;

                    /**
                     * Method you can use to obtain the minor tick line styles.
                     *
                     * \return Returns a set of minor tick line styles.
                     */
                    const QSet<ChartLineStyle>& minorTickLineStyles() const;

                private:
                    /**
                     * Value holding all the axis scaling values.
                     */
                    QSet<AxisScale> currentAxisScales;

                    /**
                     * Value hold all the axis minimum values.
                     */
                    QSet<double> currentMinimumValues;

                    /**
                     * Value holding all the axis maximum values.
                     */
                    QSet<double> currentMaximumValues;

                    /**
                     * Vakue holding all the axis major step sizes.
                     */
                    QSet<double> currentMajorStepSizes;

                    /**
                     * Value holding the axis minor step sizes.
                     */
                    QSet<double> currentMinorStepSizes;

                    /**
                     * Value holding the axis title font formats.
                     */
                    QSet<FontFormat> currentTitleFontFormats;

                    /**
                     * Value holding the axis number font formats.
                     */
                    QSet<FontFormat> currentNumberFontFormats;

                    /**
                     * Value holding the major tick styles.
                     */
                    QSet<TickStyle> currentMajorTickStyles;

                    /**
                     * Value holding the minor tick styles.
                     */
                    QSet<TickStyle> currentMinorTickStyles;

                    /**
                     * Value holding the major tick line styles.
                     */
                    QSet<ChartLineStyle> currentMajorTickLineStyles;

                    /**
                     * Value holding the minor tick line styles.
                     */
                    QSet<ChartLineStyle> currentMinorTickLineStyles;
            };

            /**
             * The typename for the format.
             */
            static const QString formatName;

            /**
             * Value used to represent automatic scaling of values.  Maps to infinity.
             */
            static const double automaticScaling;

            /**
             * Default axis scaling values.
             */
            static const AxisScale defaultAxisScale;

            /**
             * Default axis minimum values.
             */
            static const double defaultMinimumValue;

            /**
             * Default axis maximum values.
             */
            static const double defaultMaximumValue;

            /**
             * Vakue holding all the axis major step sizes.
             */
            static const double defaultMajorStepSize;

            /**
             * Default axis minor step sizes.
             */
            static const double defaultMinorStepSize;

            /**
             * Default axis title font formats.
             */
            static const FontFormat defaultTitleFontFormat;

            /**
             * Default axis number font formats.
             */
            static const FontFormat defaultNumberFontFormat;

            /**
             * Default major tick styles.
             */
            static const TickStyle defaultMajorTickStyle;

            /**
             * Default minor tick styles.
             */
            static const TickStyle defaultMinorTickStyle;

            /**
             * Default major tick line styles.
             */
            static const ChartLineStyle defaultMajorTickLineStyle;

            /**
             * Default minor tick line styles.
             */
            static const ChartLineStyle defaultMinorTickLineStyle;

            /**
             * Constructor.
             *
             * \param[in] axisScale          The graph axis style type.
             *
             * \param[in] minimumValue       The minimum graph value.
             *
             * \param[in] maximumValue       The maximum graph value.
             *
             * \param[in] majorStepSize      The major step size.
             *
             * \param[in] minorStepSize      The minor step size.
             *
             * \param[in] majorTickStyle     The style or line to use for the major ticks.
             *
             * \param[in] minorTickStyle     The style or line to use for the minor ticks.
             *
             * \param[in] majorTickLineStyle The style or line to use for the major ticks.
             *
             * \param[in] minorTickLineStyle The style or line to use for the minor ticks.
             *
             * \param[in] titleFont          The font to apply to the axis title text.
             *
             * \param[in] numberFont         The font to apply to numbers.
             */
            ChartAxisFormat(
                AxisScale             axisScale = defaultAxisScale,
                double                minimumValue = defaultMinimumValue,
                double                maximumValue = defaultMaximumValue,
                double                majorStepSize = defaultMajorStepSize,
                double                minorStepSize = defaultMinorStepSize,
                TickStyle             majorTickStyle = defaultMajorTickStyle,
                TickStyle             minorTickStyle = defaultMinorTickStyle,
                const ChartLineStyle& majorTickLineStyle = defaultMajorTickLineStyle,
                const ChartLineStyle& minorTickLineStyle = defaultMinorTickLineStyle,
                const FontFormat&     titleFont = defaultTitleFontFormat,
                const FontFormat&     numberFont = defaultNumberFontFormat
            );

            /**
             * Copy constructor.  Note that the copy constructor does not copy elements from the underlying
             * \ref Ld::Format class.  All format parameters are copied by the format are not tied to any element.
             *
             * \param[in] other The instance to be copied.
             */
            ChartAxisFormat(const ChartAxisFormat& other);

            ~ChartAxisFormat() override;

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
             * Method you can use to set the axis scaling.
             *
             * \param[in] newScaling The new axis scaling.
             */
            void setAxisScale(AxisScale newScaling);

            /**
             * Method you can use to obtain the current axis scaling.
             *
             * \return Returns the current axis scaling.
             */
            AxisScale axisScale() const;

            /**
             * Method you can use to set the axis minimum value.
             *
             * \param[in] newMinimumValue The new minimum value.  If greater than the maximum value, the axis will be
             *                            flipped.
             */
            void setMinimumValue(double newMinimumValue);

            /**
             * Method you can use to obtain the axis minimum value.
             *
             * \return Returns the axis minimum value.
             */
            double minimumValue() const;

            /**
             * Method you can use to set the axis maximum value.
             *
             * \param[in] newMaximumValue The new maximum value.  If less than the minimum value, the axis will be
             *                            flipped.
             */
            void setMaximumValue(double newMaximumValue);

            /**
             * Method you can use to obtain the axis maximum value.
             *
             * \return Returns the axis maximum value.
             */
            double maximumValue() const;

            /**
             * Method you can use to set the major step size.
             *
             * \param[in] newMajorStepSize The new major step size.
             */
            void setMajorStepSize(double newMajorStepSize);

            /**
             * Method you can use to obtain the major step size.
             *
             * \return Returns the major step size for the axis.
             */
            double majorStepSize() const;

            /**
             * Method you can use to set the minor step size.
             *
             * \param[in] newMinorStepSize The new minor step size.
             */
            void setMinorStepSize(double newMinorStepSize);

            /**
             * Method you can use to obtain the minor step size.
             *
             * \return Returns the minor step size for the axis.
             */
            double minorStepSize() const;

            /**
             * Method you can use to set the major tick style.
             *
             * \param[in] newTickStyle The new tick style.
             */
            void setMajorTickStyle(TickStyle newTickStyle);

            /**
             * Method you can use to obtain the major tick style.
             *
             * \return Returns the major tick style.
             */
            TickStyle majorTickStyle() const;

            /**
             * Method you can use to set the minor tick style.
             *
             * \param[in] newTickStyle The new tick style.
             */
            void setMinorTickStyle(TickStyle newTickStyle);

            /**
             * Method you can use to obtain the minor tick style.
             *
             * \return Returns the minor tick style.
             */
            TickStyle minorTickStyle() const;

            /**
             * Method you can use to set the major tick line style.
             *
             * \param[in] newTickLineStyle The new tick line style.
             */
            void setMajorTickLineStyle(const ChartLineStyle& newTickLineStyle);

            /**
             * Method you can use to obtain the major tick line style.
             *
             * \return Returns the major tick line style.
             */
            const ChartLineStyle& majorTickLineStyle() const;

            /**
             * Method you can use to set the minor tick line style.
             *
             * \param[in] newTickLineStyle The new tick line style.
             */
            void setMinorTickLineStyle(const ChartLineStyle& newTickLineStyle);

            /**
             * Method you can use to obtain the minor tick line style.
             *
             * \return Returns the minor tick line style.
             */
            const ChartLineStyle& minorTickLineStyle() const;

            /**
             * Method you can use to set the axis title font format.
             *
             * \param[in] newTitleFont The new title font.
             */
            void setTitleFont(const FontFormat& newTitleFont);

            /**
             * Method you can use to obtian the axis title font format.
             *
             * \return Returns the current title font format.
             */
            const FontFormat& titleFont() const;

            /**
             * Method you can use to set the axis number font format.
             *
             * \param[in] newNumberFont The new number font.
             */
            void setNumberFont(const FontFormat& newNumberFont);

            /**
             * Method you can use to obtian the axis number font format.
             *
             * \return Returns the current number font format.
             */
            const FontFormat& numberFont() const;

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
            ChartAxisFormat& operator=(const ChartAxisFormat& other);

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instances are the same.  Returns false if the instances are different.
             */
            bool operator==(const ChartAxisFormat& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instances are different.  Returns false if the instances are the same.
             */
            bool operator!=(const ChartAxisFormat& other) const;

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
             * Method that converts an axis scale to a string.
             *
             * \param[in] mode The mode to be converted.
             *
             * \return Returns the axis scale value converted to a string.
             */
            static QString toString(AxisScale mode);

            /**
             * Method that converts a string back to an axis scaling.
             *
             * \param[in]  str The string to be converted.
             *
             * \param[out] ok  A pointer to an optional boolean value.  The boolean value will hold true on success
             *                 or false on error.
             */
            AxisScale toAxisScale(const QString& str, bool* ok = nullptr);

            /**
             * Method that converts an tick style to a string.
             *
             * \param[in] mode The mode to be converted.
             *
             * \return Returns the tick style value converted to a string.
             */
            static QString toString(TickStyle mode);

            /**
             * Method that converts a string back to an tick style.
             *
             * \param[in]  str The string to be converted.
             *
             * \param[out] ok  A pointer to an optional boolean value.  The boolean value will hold true on success
             *                 or false on error.
             */
            TickStyle toTickStyle(const QString& str, bool*ok = nullptr);

            /**
             * The current axis scaling values.
             */
            AxisScale currentAxisScale;

            /**
             * The current axis minimum values.
             */
            double currentMinimumValue;

            /**
             * The current axis maximum values.
             */
            double currentMaximumValue;

            /**
             * Vakue holding all the axis major step sizes.
             */
            double currentMajorStepSize;

            /**
             * The current axis minor step sizes.
             */
            double currentMinorStepSize;

            /**
             * The current axis title font formats.
             */
            FontFormat currentTitleFontFormat;

            /**
             * The current axis number font formats.
             */
            FontFormat currentNumberFontFormat;

            /**
             * The current major tick styles.
             */
            TickStyle currentMajorTickStyle;

            /**
             * The current minor tick styles.
             */
            TickStyle currentMinorTickStyle;

            /**
             * The current major tick line styles.
             */
            ChartLineStyle currentMajorTickLineStyle;

            /**
             * The current minor tick line styles.
             */
            ChartLineStyle currentMinorTickLineStyle;
    };

    /**
     * Hash function for the \ref Ld::ChartAxisFormat::AxisScale enumeration.
     *
     * \param[in] value The value to be hashed.
     *
     * \param[in] seed  An optional seed to apply to the hash operation.
     *
     * \return Returns a hash for the provided line style.
     */
    LD_PUBLIC_API Util::HashResult qHash(ChartAxisFormat::AxisScale value, Util::HashSeed seed = 0);

    /**
     * Hash function for the \ref Ld::ChartAxisFormat::TickStyle enumeration.
     *
     * \param[in] value The value to be hashed.
     *
     * \param[in] seed  An optional seed to apply to the hash operation.
     *
     * \return Returns a hash for the provided line style.
     */
    LD_PUBLIC_API Util::HashResult qHash(ChartAxisFormat::TickStyle value, Util::HashSeed seed = 0);

    /**
     * Hash function for the \ref Ld::ChartAxisFormat class.
     *
     * \param[in] value The \ref Ld::ChartAxisFormat to be hashed.
     *
     * \param[in] seed  An optional seed for the hash operation.
     *
     * \return Returns a hash for the scaling mode.
     */
    LD_PUBLIC_API Util::HashResult qHash(const ChartAxisFormat& value, Util::HashSeed seed = 0);
}

#endif
