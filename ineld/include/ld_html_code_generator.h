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
* This header defines the \ref Ld::HtmlCodeGenerator class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_HTML_CODE_GENERATOR_H
#define LD_HTML_CODE_GENERATOR_H

#include <QString>
#include <QSharedPointer>
#include <QList>

#include "ld_common.h"
#include "ld_element_structures.h"
#include "ld_code_generator.h"

namespace Ld {
    class CodeGeneratorVisual;
    class CodeGeneratorOutputTypeContainer;
    class RootElement;
    class XmlExportContext;

    /**
     * THe HTML code generator that converts the syntax tree into HTML and additional side payloads such as images.
     */
    class LD_PUBLIC_API HtmlCodeGenerator:public CodeGenerator {
        public:
            /**
             * Enumeration of available math generation modes.
             */
            enum class MathMode {
                /**
                 * Value indicating that no math should be included in the generated output.
                 */
                NO_MATH,

                /**
                 * Value indicating that MathML should be used for the generated output.
                 */
                MATHML,

                /**
                 * Value indicating that MathML with MathJax should be used for the generated output.
                 */
                MATHJAX_MATHML,

                /**
                 * Value indicating that LaTeX with MathJax should be used for the generated output.
                 */
                MATHJAX_LATEX,

                /**
                 * Value indicating that equations should be rendered as images.
                 */
                IMAGES
            };

            /**
             * Enumeration of supported HTML types.
             */
            enum class HtmlStyle {
                /**
                 * Value indicating HTML 5 with CSS should be generated.
                 */
                HTML5_WITH_CSS,

                /**
                 * Value indicating HTML 4.01 transitional without CSS should be generated.
                 */
                HTML4_WITHOUT_CSS
            };

            /**
             * Enumeration of supported image handling modes.
             */
            enum class ImageHandlingMode {
                /**
                 * Value indicating that images should be excluded from the HTML output.
                 */
                EXCLUDE,

                /**
                 * Value indicating that images should be included as distinct payloads.
                 */
                SEPARATE_PAYLOADS,

                /**
                 * Value indicating that images should be included, base-64 encoded directly into the HTML.
                 */
                EMBEDDED
            };

            /**
             * String holding this code generator's name.
             */
            static const char codeGeneratorName[];

            /**
             * The default math mode.
             */
            static const MathMode defaultMathMode;

            /**
             * The default HTML version.
             */
            static const HtmlStyle defaultHtmlStyle;

            /**
             * The URI used for the MathJax script.
             */
            static const char mathJaxUri[];

            /**
             * The MathJax configuration file applied to MathJax.
             */
            static const char mathJaxConfigurationFile[];

            /**
             * Constructor
             *
             * \param[in] visual The visual that can be used to obtain feedback from the code generator.
             */
            HtmlCodeGenerator(CodeGeneratorVisual* visual = nullptr);

            HtmlCodeGenerator(const HtmlCodeGenerator& other) = delete;

            ~HtmlCodeGenerator() override;

            /**
             * Method you should overload to specify the name of the code generator.
             *
             * \return Returns the name of the code generator.
             */
            QString typeName() const final;

            /**
             * Method that indicates that this code generator supports selections as an input type.
             *
             * \return Returns true if the generator supports selections as an input.  Returns false if the code
             *         generator does not support selections as an input.  The default implementation always returns
             *         false.
             */
            bool supportsSelectionsAsInput() const final;

            /**
             * Method that returns a map listing the supported output types for this class.
             *
             * \return Returns a map of the supported output types.
             */
            QList<CodeGeneratorOutputTypeContainer> supportedOutputTypes() const final;

            /**
             * Method you can use to obtain the context used to generate the XHTML.
             *
             * \return Returns a shared pointer to the context used to generate the XHTML.
             */
            QSharedPointer<XmlExportContext> context() const;

            /**
             * Method you can use to determine if per-element translators will be ignored or reported.
             *
             * \return Returns true if missing per-element translators will be ignored and skipped.  Returns false if
             *         missing per-element translators will generate a diagnostic.
             */
            bool ignoreMissingPerElementTranslators() const;

            /**
             * Method you can use to determine if per-element translators will be reported or ignored.
             *
             * \return Returns true if missing per-element translators will generate a diagnostic.  Returns false if
             *         missing per-element translators will be ignored and skipped.
             */
            bool reportMissingPerElementTranslators() const;

            /**
             * Method you can use to indicate if this code generator should ignore missing per-element
             * translators.  Missing per-element translators will be reported by default.
             *
             * \param[in] nowIgnoreMissingPerElementTranslators If true, missing per-element translators will be
             *                                                  silently skipped.  If false, a diagnostic will be
             *                                                  generated if a missing per-element translator
             *                                                  is reported.
             */
            void setIgnoreMissingPerElementTranslators(bool nowIgnoreMissingPerElementTranslators = true);

            /**
             * Method you can use to indicate if this code generator should report missing per-element
             * translators.  Missing per-element translators will be reported by default.
             *
             * \param[in] nowReportMissingPerElementTranslators If true, missing per-element translators will be
             *                                                  reported by a diagnostic.  If false, missing
             *                                                  per-element translators will be skipped and ignored.
             */
            void setReportMissingPerElementTranslators(bool nowReportMissingPerElementTranslators = true);

            /**
             * Method you can use to determine the selected math generation mode
             *
             * \return Returns the selected math generation mode.
             */
            MathMode mathMode() const;

            /**
             * Method you can use to specify the desired math generation mode.
             *
             * \param[in] newMathMode The newly requested math mode.
             */
            void setMathMode(MathMode newMathMode);

            /**
             * Method you can use to obtain the desired HTML version.
             *
             * \return Returns the currently desired HTML version.
             */
            HtmlStyle htmlStyle() const;

            /**
             * Method you can use to set the desired HTML version to generate.
             *
             * \param[in] newHtmlStyle The desired HTML version.
             */
            void setHtmlStyle(HtmlStyle newHtmlStyle);

            /**
             * Method you can use to determine how images are handled.
             *
             * \return Returns the current image handling mode.
             */
            ImageHandlingMode imageHandlingMode() const;

            /**
             * Method you can use to specify the desired image handling mode.
             *
             * \param[in] newImageHandlingMode The new image handling mode to be used.
             */
            void setImageHandlingMode(ImageHandlingMode newImageHandlingMode);

            /**
             * Method you can use to determine if all imports will be processed or if only the supplied root element
             * will be processed.
             *
             * \return Returns true if all imports will be processed.  Returns false if only the main root element will
             *         be processed.
             */
            bool processImports() const;

            /**
             * Method you can use to determine if only the supplied root element will be processed or if all imports
             * will be processed.
             *
             * \return Returns true if only the supplied root element will be processed.  Returns false if all imports
             *         will be processed.
             */
            bool processNoImports() const;

            /**
             * Method you can use to specify if all imports will be processed or if only the supplied root element
             * will be processed.
             *
             * \param[in] nowProcessImports If true, all imports will be processed.  If false, only the supplied root
             *                              element will be processed.
             */
            void setProcessImports(bool nowProcessImports = true);

            /**
             * Method you can use to specify if only the supplied root element will be processed or if all imports
             * will be processed.
             *
             * \param[in] nowProcessNoImports If true, no imports will be processed.  If false, then imports will be
             *                                processed.
             */
            void setProcessNoImports(bool nowProcessNoImports = true);

            HtmlCodeGenerator& operator=(const HtmlCodeGenerator& other) = delete;

        protected:
            /**
             * Method that creates the HTML code generation engine.
             *
             * \param[in] rootElement The root element to translate.
             *
             * \param[in] outputFile  The name of the output file to be created.
             *
             * \param[in] outputType  The output type to generate code for.
             *
             * \param[in] exportMode  A value indicating the export mode to be used.
             *
             * \param[in] usageData   The usage data instance.
             *
             * \return Returns a shared pointer to the code generation engine to be used.
             */
            QSharedPointer<CodeGenerationEngine> createEngine(
                QSharedPointer<RootElement>             rootElement,
                const QString&                          outputFile,
                const CodeGeneratorOutputTypeContainer& outputType,
               CodeGeneratorOutputType::ExportMode      exportMode,
               Ud::UsageData*                           usageData
            ) final;

        private:
            /**
             * Flag indicating if missing per-element translators are ignored or reported.
             */
            bool currentIgnoreMissingPerElementTranslators;

            /**
             * The currently selected math mode.
             */
            MathMode currentMathMode;

            /**
             * The currently selected HTML version.
             */
            HtmlStyle currentHtmlStyle;

            /**
             * The currently selected image handling mode.
             */
            ImageHandlingMode currentImageHandlingMode;

            /**
             * Flag indicating if imports should be included on the next run.
             */
            bool currentProcessImports;
    };
};

#endif
