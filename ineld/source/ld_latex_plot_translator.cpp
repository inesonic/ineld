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
* This file implements the \ref Ld::LaTeXPlotTranslator class.
***********************************************************************************************************************/

#include <QString>
#include <QByteArray>

#include <utility>

#include "ld_element_structures.h"
#include "ld_plot_element.h"
#include "ld_chart_format.h"
#include "ld_text_export_context.h"
#include "ld_latex_code_generation_engine.h"
#include "ld_latex_translator.h"
#include "ld_latex_plot_translator.h"

namespace Ld {
    LaTeXPlotTranslator::~LaTeXPlotTranslator() {}


    QString LaTeXPlotTranslator::elementName() const {
        return PlotElement::elementName;
    }


    std::pair<float, float> LaTeXPlotTranslator::getSize(
            ElementPointer             /* element */,
            FormatPointer              format,
            LaTeXCodeGenerationEngine& /* generationEngine */
        ) {
        float width;
        float height;

        if (format->capabilities().contains(ChartFormat::formatName)) {
            QSharedPointer<ChartFormat> chartFormat = format.dynamicCast<ChartFormat>();
            width  = chartFormat->chartWidth();
            height = chartFormat->chartHeight();
        } else {
            width  = 0;
            height = 0;
        }

        return std::make_pair(width,height);
    }


    Format::ImageScalingMode LaTeXPlotTranslator::getHorizontalScaling(
            ElementPointer             /* element */,
            FormatPointer              /* format */,
            LaTeXCodeGenerationEngine& /* generationEngine */
        ) {
        return Format::ImageScalingMode::FIXED_POINTS;
    }


    Format::ImageScalingMode LaTeXPlotTranslator::getVerticalScaling(
            ElementPointer             /* element */,
            FormatPointer              /* format */,
            LaTeXCodeGenerationEngine& /* generationEngine */
        ) {
        return Format::ImageScalingMode::FIXED_POINTS;
    }
}
