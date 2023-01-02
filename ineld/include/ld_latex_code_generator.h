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
* This header defines the \ref Ld::LaTeXCodeGenerator class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_LATEX_CODE_GENERATOR_H
#define LD_LATEX_CODE_GENERATOR_H

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
    class TextExportContext;

    /**
     * THe LaTeX code generator that converts the syntax tree into LaTeX source and additional side payloads such as
     * images.
     */
    class LD_PUBLIC_API LaTeXCodeGenerator:public CodeGenerator {
        public:
            /**
             * Enumeration of image generation modes.
             */
            enum class ImageMode {
                /**
                 * Value indicating that no image files should be generated.
                 */
                NO_IMAGES,

                /**
                 * Value indicating that images should retain their original format.
                 */
                ORIGINAL,

                /**
                 * Indicates images should be converted to PNG format.
                 */
                FORCE_PNG,

                /**
                 * Indicates images should be converted to JPG format.
                 */
                FORCE_JPEG,

                /**
                 * Indicates images will be converted to PNG if not PNG or JPEG.
                 */
                PREFER_PNG,

                /**
                 * Indicates images will be converted to JPEG if not PNG or JPEG.
                 */
                PREFER_JPEG
            };

            /**
             * Enumeration of supported Unicode translation modes.
             */
            enum class UnicodeMode {
                /**
                 * Indicates Unicode should be inserted without modification.
                 */
                INSERT_UNICODE,

                /**
                 * Indicates Unicode should be converted to LaTeX hat notation.
                 */
                CONVERT_TO_HAT_NOTATION,

                /**
                 * Indicates Unicode should be turned into LaTeX commands, if possible.
                 */
                CONVERT_TO_LATEX_COMMAND
            };

            /**
             * The name of this code generator.
             */
            static const char codeGeneratorName[];

            /**
             * The default image mode.
             */
            static const ImageMode defaultImageMode;

            /**
             * The name used for the preamble file.
             */
            static const QString latexPreambleFilename;

            /**
             * The name used for the document body file.
             */
            static const QString latexBodyFilename;

            /**
             * The name used for the top level header that includes the preamble and body.
             */
            static const QString latexTopFilename;

            /**
             * Constructor
             *
             * \param[in] visual The visual that can be used to obtain feedback from the code generator.
             */
            LaTeXCodeGenerator(CodeGeneratorVisual* visual = nullptr);

            LaTeXCodeGenerator(const LaTeXCodeGenerator& other) = delete;

            ~LaTeXCodeGenerator() override;

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
             * Method you can use to obtain the context used to generate the LaTeX.
             *
             * \return Returns a shared pointer to the context used to generate the LaTeX.
             */
            QSharedPointer<TextExportContext> context() const;

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
             * Method you can use to determine the requested image mode.
             */
            ImageMode imageMode() const;

            /**
             * Method you can use to specify the requested image mode.
             *
             * \param[in] newImageMode The newly selected image mode.
             */
            void setImageMode(ImageMode newImageMode);

            /**
             * Method you can use to determine if the generated LaTeX source will be contained in a single file or in
             * multiple files.
             *
             * \return Returns true if the LaTeX source will be in a single file.  Returns false if the LaTeX source
             *         will be in multiple files.
             */
            bool singleFile() const;

            /**
             * Method you can use to determine if the generated LaTeX source will be contained in multiple files or in
             * a single file.
             *
             * \return Returns true if the LaTeX source will be in a single file.  Returns false if the LaTeX source
             *         will be in multiple files.
             */
            bool multipleFiles() const;

            /**
             * Method you can use to specify whether the LaTeX source should be in a single file or in multiple files.
             *
             * \param[in] nowSingleFile If true, the LaTeX source will be placed into a single file.  If false, the
             *                          LaTeX source will be placed into multiple files.
             */
            void setSingleFile(bool nowSingleFile = true);

            /**
             * Method you can use to specify whether the LaTeX source should be in multiple files or in a single file.
             *
             * \param[in] nowMultipleFiles If true, the LaTeX source will be placed into multiple files.  If false, the
             *                             LaTeX source will be placed into a single file.
             */
            void setMultipleFiles(bool nowMultipleFiles = true);

            /**
             * Method you can use to determine if the copyright message will be included or excluded.
             *
             * \return Returns true if the copyright message will be included.  Returns false if the copyright message
             *         will be excluded.
             */
            bool includeCopyright() const;

            /**
             * Method you can use to determine if the copyright message will be included or excluded.
             *
             * \return Returns true if the copyright message will be included.  Returns false if the copyright message
             *         will be excluded.
             */
            bool excludeCopyright() const;

            /**
             * Method you can use to explicitly include or exclude the copyright message.
             *
             * \param[in] nowIncludeCopyright If true, the copyright message will be included.  If false, the copyright
             *                                message will be excluded.
             */
            void setCopyrightIncluded(bool nowIncludeCopyright = true);

            /**
             * Method you can use to explicitly exclude or include the copyright message.
             *
             * \param[in] nowExcludeCopyright If true, the copyright message will be excluded.  If false, the copyright
             *                                message will be included.
             */
            void setCopyrightExcluded(bool nowExcludeCopyright = true);

            /**
             * Method you can use to determine the currently selected Unicode translation mode.
             *
             * \return Returns the currently selected Unicode translation mode.
             */
            UnicodeMode unicodeTranslationMode() const;

            /**
             * Method you can use to set the desired Unicode translation mode.
             *
             * \param[in] newUnicodeTranslationMode The desired Unicode translation mode.
             */
            void setUnicodeTranslationMode(UnicodeMode newUnicodeTranslationMode);

            /**
             * Method you can use to determine if all imports will be processed or if only the supplied root element
             * will be processed.
             *
             * \return Returns true if all imports will be processed.  Returns false if only the main root element
             *         will be processed.
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

            LaTeXCodeGenerator& operator=(const LaTeXCodeGenerator& other) = delete;

        protected:
            /**
             * Method that creates the LaTeX code generation engine.
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
                CodeGeneratorOutputType::ExportMode     exportMode,
                Ud::UsageData*                          usageData
            ) final;

        private:
            /**
             * Flag indicating if missing per-element translators are ignored or reported.
             */
            bool currentIgnoreMissingPerElementTranslators;

            /**
             * The currently selected math mode.
             */
            ImageMode currentImageMode;

            /**
             * Flag indicating if imports should be included on the next run.
             */
            bool currentProcessImports;

            /**
             * Flag indicating if the LaTeX source should be placed into a single file.
             */
            bool currentSingleFile;

            /**
             * Flag indicating if the copyright should be included.
             */
            bool currentIncludeCopyright;

            /**
             * Value indicating the desired Unicode translation mode.
             */
            UnicodeMode currentUnicodeTranslationMode;
    };
};

#endif
