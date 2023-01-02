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
* This header defines the \ref Ld::HtmlCodeGenerationEngine class.
***********************************************************************************************************************/

/* .. sphinx-project ineld */

#ifndef LD_HTML_CODE_GENERATION_ENGINE_H
#define LD_HTML_CODE_GENERATION_ENGINE_H

#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS macro.
#include <QString>
#include <QList>
#include <QSet>
#include <QSharedPointer>

#include "ld_common.h"
#include "ld_format_container.h"
#include "ld_format_organizer.h"
#include "ld_element_structures.h"
#include "ld_html_code_generator.h"
#include "ld_code_generator_output_type_container.h"
#include "ld_code_generation_engine.h"

namespace Ud {
    class UsageData;
};

namespace Ld {
    class CodeGenerator;
    class Diagnostic;
    class RootElement;
    class Translator;
    class TranslationPhase;
    class XmlExportContext;

    /**
     * This class is the per-run HTML code generation engine.
     */
    class LD_PUBLIC_API HtmlCodeGenerationEngine:public CodeGenerationEngine {
        Q_DECLARE_TR_FUNCTIONS(Ld::HtmlCodeGenerationEngine)

        public:
            /**
             * Type used to indicate the math generation mode.
             */
            typedef HtmlCodeGenerator::MathMode MathMode;

            /**
             * Type used to indicate the HTML style to be generated.
             */
            typedef HtmlCodeGenerator::HtmlStyle HtmlStyle;

            /**
             * Type used to indicate the image handling mode to be used.
             */
            typedef HtmlCodeGenerator::ImageHandlingMode ImageHandlingMode;

            /**
             * Constructor.
             *
             * \param[in] codeGenerator            The code generator that instantiated this class instance.
             *
             * \param[in] rootElement              The root element used during the translation.
             *
             * \param[in] outputFile               The name of the output file to be generated.  If the output type is
             *                                     not exportable, then the translated output file will be deleted by
             *                                     this class at the end of scope.
             *
             * \param[in] outputType               The type of output to be generated by this code generation engine.
             *
             * \param[in] exportMode               A value indicating the export mode to be used.
             *
             * \param[in] usageData                The usage data instance to use with this engine.
             *
             * \param[in] ignoreMissingTranslators If true, missing per-element translators will be ignored.  If false,
             *                                     missing per-element translators will generate an error.
             *
             * \param[in] mathMode                 The desired math generation mode.
             *
             * \param[in] htmlStyle                The style of HTML to be generated.
             *
             * \param[in] imageHandlingMode        The desired image handling mode.
             *
             * \param[in] includeImports           If true, root imports will be included.  If false, root imports will
             *                                     be excluded.
             */
            HtmlCodeGenerationEngine(
                CodeGenerator*                          codeGenerator,
                QSharedPointer<RootElement>             rootElement,
                const QString&                          outputFile,
                const CodeGeneratorOutputTypeContainer& outputType,
                CodeGeneratorOutputType::ExportMode     exportMode,
                Ud::UsageData*                          usageData,
                bool                                    ignoreMissingTranslators,
                MathMode                                mathMode,
                HtmlStyle                               htmlStyle,
                ImageHandlingMode                       imageHandlingMode,
                bool                                    includeImports
            );

            ~HtmlCodeGenerationEngine() override;

            /**
             * Method you can use to determine the require math mode.
             *
             * \return Returns the required math mode.
             */
            MathMode mathMode() const;

            /**
             * Method you can use to determine the required HTML style to generate.
             *
             * \return Returns the required HTML style.
             */
            HtmlStyle htmlStyle() const;

            /**
             * Method you can use to determine the desired image handling mode.
             *
             * \return Returns the current image handling mode.
             */
            ImageHandlingMode imageHandlingMode() const;

            /**
             * Method you can use to obtain the context used to write the HTML and side-band data.
             *
             * \return Returns a reference to the requested context.
             */
            XmlExportContext& context();

            /**
             * Method you can use to obtain the context used to write the HTML and side-band data.
             *
             * \return Returns a reference to the requested context.
             */
            const XmlExportContext& context() const;

            /**
             * Method you can use to obtain a shared pointer to the context used to write the HTML and side-band data.
             *
             * \return Returns a shared pointer to the requested context.
             */
            QSharedPointer<XmlExportContext> contextPointer() const;

            /**
             * Method you can use to register a new format.
             *
             * \param[in] format A shared pointer to the format to be registered.
             */
            void registerFormat(FormatPointer format);

            /**
             * Method you can use to obtain the identifier to use for a format.
             *
             * \param[in] format A shared pointer to the format to obtain the identifier for.  This format must have
             *                   been previously registered during the
             *                   Ld::HtmlTranslationPhase::Phase:REGISTER_FORMATS phase.
             *
             * \return Returns the format identifier for the requested format.  A value of invalidFormatIdentifier will
             *         be returned if the format has not been registered.
             */
            FormatIdentifier identiferForFormat(FormatPointer format);

            /**
             * Method you can use to obtain the CSS class name assigned to a format.
             *
             * \param[in] format A shared pointer to the format to obtain the identifier for.  This format must have
             *                   been previously registered during the
             *                   Ld::HtmlTranslationPhase::Phase:REGISTER_FORMATS phase.
             *
             * \return Returns the CSS class name assigned to a format.  An empty string is returned if the format has
             *         not been registered.
             */
            QString cssClassForFormat(FormatPointer format);

            /**
             * Method you can call from your translator to convert a returned filename to a URI suitable for use in
             * HTML.
             *
             * The method will automatically return either a relative URI or an absolute URI depending on the
             * current generator settings.
             *
             * \param[in] filename The provided filename to convert to a URI.
             *
             * \return Returns the translated URI.
             */
            QString toUri(const QString& filename);

            /**
             * Method you can call to enter math mode.  You can call this method as many times as you like.  For each
             * call to this method, you are expected to call \ref Ld::HtmlCodeGenerationEngine::exitMathMode.  Math
             * mode will be left only when the number of calls to  \ref Ld::HtmlCodeGenerationEngine::exitMathMode
             * matches the number of calls to this method.
             *
             * \param[in] inlineMode If true, this method will enter math inline mode.  If false this method will
             *                       enter math block mode.  This parameter is ignored if the Html translator is
             *                       already in math mode.
             */
            void enterMathMode(bool inlineMode);

            /**
             * Method you can call to exit math mode.  For details, see the description of
             * \ref Ld::HtmlCodeGenerationEngine::enterMathMode.
             */
            void exitMathMode();

            /**
             * Method you can call to determine if an element is positioned such that it should be rendered in block
             * math mode or inline math mode.
             *
             * \param[in] element The element to be checked.
             *
             * \return Returns true if the element should be rendered in inline mode.  Returns false if the element
             *         should be rendered in block math mode.
             */
            static bool useInlineMathMode(ElementPointer element);

        protected:
            /**
             * Method that returns the translation phase instance to be used.
             *
             * \return Returns a pointer to the currect translation phase instance.
             */
            TranslationPhase* createTranslationPhase() const final;

            /**
             * Method that is called to determine if all root imports should be used.  You should overload this method
             * to determine if root imports should be included.
             *
             * \return Returns true if all root imports should be included.  Returns false if only the main root
             *         element should be used.
             */
            bool includeRootImports() const final;

            /**
             * Method that calls the correct pre-translation method for the required translation.
             *
             * \return Returns true on success, returns false on error.
             */
            bool preTranslate() final;

            /**
             * Method that calls the correct post-translation method for the required translation.
             *
             * \return Returns true on success, returns false on error.
             */
            bool postTranslate() final;

            /**
             * Method you must overload to report a missing translator.
             *
             * \param[in] element The element that was missing a translator.
             *
             * \return Returns true if the code generator should abort.  Returns false if the code
             *         generator should ignore the missing translator and continue.
             */
            bool missingTranslator(ElementPointer element) final;

        private:
            /**
             * Method that is called at the start of the DTD translation phase.
             *
             * \return Returns true on success, returns false on error.
             */
            bool preDtd();

            /**
             * Method that is called at the end of the DTD translation phase.
             *
             * \return Returns true on success, returns false on error.
             */
            bool postDtd();

            /**
             * Method that is called at the start of the register format translation phase.
             *
             * \return Returns true on success, returns false on error.
             */
            bool preRegisterFormats();

            /**
             * Method that is called at the end of the register format translation phase.
             *
             * \return Returns true on success, returns false on error.
             */
            bool postRegisterFormats();

            /**
             * Method that is called at the start of the header translation phase.
             *
             * \return Returns true on success, returns false on error.
             */
            bool preHeader();

            /**
             * Method that is called at the end of the header translation phase.
             *
             * \return Returns true on success, returns false on error.
             */
            bool postHeader();

            /**
             * Method that is called at the start of the BODY translation phase.
             *
             * \return Returns true on success, returns false on error.
             */
            bool preBody();

            /**
             * Method that is called at the end of the BODY translation phase.
             *
             * \return Returns true on success, returns false on error.
             */
            bool postBody();

        private:
            /**
             * Method that is called to create the export context.
             *
             * \param[in] outputFile The output file string passed to the XmlExportContext.
             *
             * \param[in] exportMode The requested export mode.  This value determines the type of context to create.
             */
            void createExportContext(const QString& outputFile, CodeGeneratorOutputType::ExportMode exportMode);

            /**
             * Flag that indicates if we should ignore missing per-element translators during this translation process.
             */
            bool currentIgnoreMissingPerElementTranslators;

            /**
             * The requested math mode.
             */
            MathMode currentMathMode;

            /**
             * The requested HTML style.
             */
            HtmlStyle currentHtmlStyle;

            /**
             * The requested image handling mode.
             */
            ImageHandlingMode currentImageHandingMode;

            /**
             * Flag indicating if root imports should be included or excluded.
             */
            bool currentIncludeImports;

            /**
             * The context used to export the HTML representation.
             */
            QSharedPointer<XmlExportContext> currentContext;

            /**
             * The format organizer used to track formats.
             */
            FormatOrganizer formatOrganizer;

            /**
             * The current math mode nesting level.
             */
            unsigned currentMathModeNesting;

            /**
             * Indicates if we entered math inline mode or math block mode.
             */
            bool currentInlineMode;
    };
};

#endif