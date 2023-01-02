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
* This file implements tests for the \ref Ld::LaTeXCodeGenerator class and several other associated classes.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QCoreApplication>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QProcess>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QPixmap>
#include <QImage>
#include <QBuffer>
#include <QtTest/QtTest>

#include <iostream> // Debug

#include <ld_element_structures.h>
#include <ld_diagnostic_structures.h>
#include <ld_configure.h>
#include <ld_diagnostic.h>
#include <ld_code_generator.h>
#include <ld_code_generator_output_type.h>
#include <ld_latex_code_generator.h>
#include <ld_latex_code_generator_diagnostic.h>
#include <ld_latex_code_generator_output_types.h>
#include <ld_text_export_context.h>
#include <ld_text_memory_export_context.h>

#include <ld_configure.h>
#include <ld_element.h>
#include <ld_root_element.h>
#include <ld_text_element.h>
#include <ld_character_format.h>
#include <ld_paragraph_element.h>
#include <ld_paragraph_format.h>
#include <ld_unordered_list_paragraph_format.h>
#include <ld_list_additional_paragraph_format.h>
#include <ld_ordered_list_paragraph_format.h>
#include <ld_page_break_element.h>
//#include <ld_table_element.h>
//#include <ld_table_format.h>
//#include <ld_table_cell_element.h>
//#include <ld_table_cell_format.h>
#include <ld_image_element.h>
#include <ld_image_format.h>
#include <ld_page_break_element.h>

#include "test_latex_code_generator.h"

#include "kitten-3.xpm"

TestLaTeXCodeGenerator::TestLaTeXCodeGenerator() {}


TestLaTeXCodeGenerator::~TestLaTeXCodeGenerator() {}


void TestLaTeXCodeGenerator::initTestCase() {
    Ld::Configure::configure(0x1234567890ABCDEF, nullptr);
}


void TestLaTeXCodeGenerator::testBasicFunctionality() {
    QSharedPointer<Ld::RootElement> rootElement = buildSampleProgram();

    QSharedPointer<Ld::LaTeXCodeGenerator>
        codeGenerator = Ld::CodeGenerator::codeGenerator(Ld::LaTeXCodeGenerator::codeGeneratorName)
                        .dynamicCast<Ld::LaTeXCodeGenerator>();

    QString exportedLaTeX = "exported/body.tex";

    codeGenerator->setReportMissingPerElementTranslators();
    codeGenerator->setImageMode(Ld::LaTeXCodeGenerator::ImageMode::FORCE_PNG);
    codeGenerator->setSingleFile();
    codeGenerator->setCopyrightIncluded();
    codeGenerator->setProcessNoImports();
    bool success = codeGenerator->translate(
        rootElement,
        exportedLaTeX,
        codeGenerator->supportedOutputTypes().first(),
        Ld::CodeGeneratorOutputType::ExportMode::EXPORT_AS_DIRECTORY
    );

    QVERIFY(success);

    codeGenerator->waitComplete();

    QList<Ld::DiagnosticPointer> diagnostics = codeGenerator->reportedDiagnostics();
    if (diagnostics.isEmpty()) {
        QSharedPointer<Ld::TextExportContext> context = codeGenerator->context();
        QByteArray                            program = context->payload("body.tex");
        qDebug() << QString("Program:");
        std::cout.flush();
        std::cout << program.data();
    } else {
        for (  QList<Ld::DiagnosticPointer>::const_iterator diagnosticsIterator = diagnostics.constBegin(),
                                                            diagnosticsEndIterator = diagnostics.constEnd()
             ; diagnosticsIterator != diagnosticsEndIterator
             ; ++diagnosticsIterator
            ) {
            Ld::DiagnosticPointer diagnostic = *diagnosticsIterator;
            qDebug() << "Diagnostic: " << diagnostic->diagnosticMessage();
        }
    }
}


void TestLaTeXCodeGenerator::testDiagnosticHandling() {}


void TestLaTeXCodeGenerator::cleanupTestCase() {}


QSharedPointer<Ld::RootElement> TestLaTeXCodeGenerator::buildSampleProgram() {
    QSharedPointer<Ld::RootElement> root = Ld::Element::create(Ld::RootElement::elementName)
                                           .dynamicCast<Ld::RootElement>();

    addSimpleParagraph(root);
    addImage(root);
    addPageBreak(root);
    addList(root, true);
    addList(root, false);

    return root;
}


void TestLaTeXCodeGenerator::addSimpleParagraph(QSharedPointer<Ld::RootElement> root) {
    QSharedPointer<Ld::ParagraphElement> paragraph1 = Ld::Element::create("Paragraph")
                                                      .dynamicCast<Ld::ParagraphElement>();

    QSharedPointer<Ld::ParagraphFormat> paragraphFormat1 = Ld::Format::create("ParagraphFormat")
                                                           .dynamicCast<Ld::ParagraphFormat>();

    paragraphFormat1->setLeftIndentation(18.0);
    paragraphFormat1->setFirstLineLeftIndentation(36.0);
    paragraphFormat1->setJustification(Ld::ParagraphFormat::Justification::JUSTIFY);
    paragraphFormat1->setLineSpacing(1.25);

    paragraph1->setFormat(paragraphFormat1);

    QSharedPointer<Ld::TextElement> textElement1 = Ld::Element::create("Text").dynamicCast<Ld::TextElement>();
    QSharedPointer<Ld::CharacterFormat> textFormat1 = Ld::Format::create("CharacterFormat")
                                                     .dynamicCast<Ld::CharacterFormat>();
    textFormat1->setFamily("Helvetica");

    QString text = tr(
        "Four score an seven years ago our fathers brought forth on this continent a new nation, conceived in "
    );
    textElement1->setText(text);
    textElement1->setFormat(textFormat1);

    paragraph1->append(textElement1, nullptr);

    QSharedPointer<Ld::TextElement> textElement2 = Ld::Element::create("Text").dynamicCast<Ld::TextElement>();
    QSharedPointer<Ld::CharacterFormat> textFormat2 = Ld::Format::create("CharacterFormat")
                                                      .dynamicCast<Ld::CharacterFormat>();

    textFormat2->setFontWeight(Ld::CharacterFormat::Weight::BLACK);
    textFormat2->setFamily("Helvetica");

    text = tr("liberty");
    textElement2->setText(text);
    textElement2->setFormat(textFormat2);

    paragraph1->append(textElement2, nullptr);

    QSharedPointer<Ld::TextElement> textElement3 = Ld::Element::create("Text").dynamicCast<Ld::TextElement>();
    text = tr(", and dedicated to the proposition that all men are created equal.");
    textElement3->setText(text);
    textElement3->setFormat(textFormat1);

    paragraph1->append(textElement3, nullptr);

    root->append(paragraph1, nullptr);
}


void TestLaTeXCodeGenerator::addList(QSharedPointer<Ld::RootElement> root, bool ordered) {
    QSharedPointer<Ld::CharacterFormat> textFormat1 = Ld::Format::create("CharacterFormat")
                                                     .dynamicCast<Ld::CharacterFormat>();
    textFormat1->setFamily("Helvetica");

    // List header
    QSharedPointer<Ld::ParagraphElement> paragraph1 = Ld::Element::create("Paragraph")
                                                     .dynamicCast<Ld::ParagraphElement>();

    QSharedPointer<Ld::ParagraphFormat> paragraphFormat1 = Ld::Format::create("ParagraphFormat")
                                                           .dynamicCast<Ld::ParagraphFormat>();

    paragraphFormat1->setTopSpacing(12);
    paragraphFormat1->setBottomSpacing(12);
    paragraphFormat1->setJustification(Ld::ParagraphFormat::Justification::LEFT);

    paragraph1->setFormat(paragraphFormat1);

    QSharedPointer<Ld::TextElement> textElement1 = Ld::Element::create("Text").dynamicCast<Ld::TextElement>();
    QSharedPointer<Ld::CharacterFormat> textFormat2 = Ld::Format::create("CharacterFormat")
                                                    .dynamicCast<Ld::CharacterFormat>();
    textFormat2->setFamily("Courier");
    textFormat2->setFontSize(10);

    textElement1->setText(tr("Corporate culture: (numbered list)"));
    textElement1->setFormat(textFormat2);

    paragraph1->append(textElement1, nullptr);
    root->append(paragraph1, nullptr);

    // List entry 1
    QSharedPointer<Ld::ParagraphElement> paragraph2 = Ld::Element::create("Paragraph")
                                                     .dynamicCast<Ld::ParagraphElement>();

    QSharedPointer<Ld::ListParagraphFormatBase> paragraphFormat2;
    if (ordered) {
        QSharedPointer<Ld::OrderedListParagraphFormat> orderedParagraphFormat =
            Ld::Format::create("OrderedListParagraphFormat").dynamicCast<Ld::OrderedListParagraphFormat>();

        orderedParagraphFormat->setStyle(Ld::OrderedListParagraphFormat::Style::ROMAN_LOWER_CASE);
        orderedParagraphFormat->setPrefix(QString("("));
        orderedParagraphFormat->setSuffix(QString(")"));
        orderedParagraphFormat->setLeftIndentation(36.0F);

        paragraphFormat2 = orderedParagraphFormat;
        paragraph2->setFormat(orderedParagraphFormat);
    } else {
        QSharedPointer<Ld::UnorderedListParagraphFormat> unorderedParagraphFormat =
            Ld::Format::create("UnorderedListParagraphFormat").dynamicCast<Ld::UnorderedListParagraphFormat>();

        unorderedParagraphFormat->setBulletStyle(QString("-"));
        unorderedParagraphFormat->setLeftIndentation(18.0F);
        paragraphFormat2 = unorderedParagraphFormat;
        paragraph2->setFormat(unorderedParagraphFormat);
    }

    paragraphFormat2->setTopSpacing(12);
    paragraphFormat2->setFirstLineLeftIndentation(0);
    paragraphFormat2->setRightIndentation(0);
    paragraphFormat2->setBottomSpacing(12);
    paragraphFormat2->setJustification(Ld::ParagraphFormat::Justification::LEFT);
    paragraphFormat2->setListIndentation(36);

    QSharedPointer<Ld::TextElement> textElement2 = Ld::Element::create("Text").dynamicCast<Ld::TextElement>();
    textElement2->setFormat(textFormat1);
    textElement2->setText(
        tr("Values intellectual honesty between team members, towards our customers, and the larger community.")
    );

    paragraph2->append(textElement2, nullptr);
    root->append(paragraph2, nullptr);

    // List entry2
    QSharedPointer<Ld::ParagraphElement> paragraph3 = Ld::Element::create("Paragraph")
                                                     .dynamicCast<Ld::ParagraphElement>();

    QSharedPointer<Ld::ListAdditionalParagraphFormat>
        paragraphFormat3 = Ld::Format::create("ListAdditionalParagraphFormat")
                           .dynamicCast<Ld::ListAdditionalParagraphFormat>();

    paragraph3->setFormat(paragraphFormat3);

    QSharedPointer<Ld::TextElement> textElement3 = Ld::Element::create("Text").dynamicCast<Ld::TextElement>();
    textElement3->setFormat(textFormat1);
    textElement3->setText(
        tr(
           "Hires employees that excel at what they do and helps each other, our customers, and the company to "
           "excel."
        )
    );

    paragraph3->append(textElement3, nullptr);
    root->append(paragraph3, nullptr);

    // List entry3
    QSharedPointer<Ld::ParagraphElement> paragraph4 = Ld::Element::create("Paragraph")
                                                     .dynamicCast<Ld::ParagraphElement>();

    QSharedPointer<Ld::ListAdditionalParagraphFormat>
        paragraphFormat4 = Ld::Format::create("ListAdditionalParagraphFormat")
                           .dynamicCast<Ld::ListAdditionalParagraphFormat>();

    paragraphFormat4->setBulletNumberDisplayDisabled();
    paragraph4->setFormat(paragraphFormat4);

    QSharedPointer<Ld::TextElement> textElement4 = Ld::Element::create("Text").dynamicCast<Ld::TextElement>();
    textElement4->setFormat(textFormat1);
    textElement4->setText(tr("(and helps each other to improve)"));

    paragraph4->append(textElement4, nullptr);
    root->append(paragraph4, nullptr);

    // List entry4
    QSharedPointer<Ld::ParagraphElement> paragraph5 = Ld::Element::create("Paragraph")
                                                     .dynamicCast<Ld::ParagraphElement>();

    QSharedPointer<Ld::ListAdditionalParagraphFormat>
        paragraphFormat5 = Ld::Format::create("ListAdditionalParagraphFormat")
                           .dynamicCast<Ld::ListAdditionalParagraphFormat>();

    paragraph5->setFormat(paragraphFormat5);

    QSharedPointer<Ld::TextElement> textElement5 = Ld::Element::create("Text").dynamicCast<Ld::TextElement>();
    textElement5->setFormat(textFormat1);
    textElement5->setText(
        tr("Values data driven decisions using the best data that can be readily obtained.")
    );

    paragraph5->append(textElement5, nullptr);
    root->append(paragraph5, nullptr);

    // List entry5
    QSharedPointer<Ld::ParagraphElement> paragraph6 = Ld::Element::create("Paragraph")
                                                     .dynamicCast<Ld::ParagraphElement>();

    QSharedPointer<Ld::ListAdditionalParagraphFormat>
        paragraphFormat6 = Ld::Format::create("ListAdditionalParagraphFormat")
                           .dynamicCast<Ld::ListAdditionalParagraphFormat>();

    paragraph6->setFormat(paragraphFormat6);

    QSharedPointer<Ld::TextElement> textElement6 = Ld::Element::create("Text").dynamicCast<Ld::TextElement>();
    textElement6->setFormat(textFormat1);
    textElement6->setText(tr("Engenders a suppportive and compassionate work environment."));

    paragraph6->append(textElement6, nullptr);
    root->append(paragraph6, nullptr);

    // List entry6
    QSharedPointer<Ld::ParagraphElement> paragraph7 = Ld::Element::create("Paragraph")
                                                     .dynamicCast<Ld::ParagraphElement>();

    QSharedPointer<Ld::ListAdditionalParagraphFormat>
        paragraphFormat7 = Ld::Format::create("ListAdditionalParagraphFormat")
                           .dynamicCast<Ld::ListAdditionalParagraphFormat>();

    paragraph7->setFormat(paragraphFormat7);

    QSharedPointer<Ld::TextElement> textElement7 = Ld::Element::create("Text").dynamicCast<Ld::TextElement>();
    textElement7->setFormat(textFormat1);
    textElement7->setText(
        tr("Always strives to do the right thing for our employees, our customers, and our community.")
    );

    paragraph7->append(textElement7, nullptr);
    root->append(paragraph7, nullptr);

    // List entry7
    QSharedPointer<Ld::ParagraphElement> paragraph8 = Ld::Element::create("Paragraph")
                                                     .dynamicCast<Ld::ParagraphElement>();

    QSharedPointer<Ld::ListAdditionalParagraphFormat>
        paragraphFormat8 = Ld::Format::create("ListAdditionalParagraphFormat")
                           .dynamicCast<Ld::ListAdditionalParagraphFormat>();

    paragraph8->setFormat(paragraphFormat8);

    QSharedPointer<Ld::TextElement> textElement8 = Ld::Element::create("Text").dynamicCast<Ld::TextElement>();
    textElement8->setFormat(textFormat1);
    textElement8->setText(
        tr(
           "Strives to create an environment that is enjoyable to work in as well as products that are enjoyable "
           "for our customers to use."
          )
    );

    paragraph8->append(textElement8, nullptr);
    root->append(paragraph8, nullptr);
}


void TestLaTeXCodeGenerator::addImage(QSharedPointer<Ld::RootElement> root) {
    QSharedPointer<Ld::ParagraphElement> paragraph1 = Ld::Element::create("Paragraph")
                                                      .dynamicCast<Ld::ParagraphElement>();

    QSharedPointer<Ld::ParagraphFormat> paragraphFormat1 = Ld::Format::create("ParagraphFormat")
                                                           .dynamicCast<Ld::ParagraphFormat>();

    paragraphFormat1->setJustification(Ld::ParagraphFormat::Justification::CENTER);
    paragraph1->setFormat(paragraphFormat1);

    QSharedPointer<Ld::ImageElement> imageElement = Ld::Element::create("Image").dynamicCast<Ld::ImageElement>();

    QPixmap kittenPixmap(kitten_3_xpm);
    QByteArray kittenImageData;
    QBuffer kittenImageBuffer(&kittenImageData);
    kittenImageBuffer.open(QBuffer::WriteOnly);
    kittenPixmap.save(&kittenImageBuffer, "JPG");
    kittenImageBuffer.close();

    imageElement->updatePayload(kittenImageData);

    QSharedPointer<Ld::ImageFormat>  imageFormat = Ld::Format::create("ImageFormat").dynamicCast<Ld::ImageFormat>();
    imageFormat->setRotation(Ld::ImageFormat::Rotation::ROTATE_CW_90);
    imageFormat->setHorizontalAxis(Ld::ImageFormat::Axis(0, Ld::ImageFormat::ImageScalingMode::ASPECT_RATIO));
    imageFormat->setVerticalAxis(Ld::ImageFormat::Axis(72.0 * 2, Ld::ImageFormat::ImageScalingMode::FIXED_POINTS));
    imageElement->setFormat(imageFormat);
    paragraph1->append(imageElement, nullptr);

    root->append(paragraph1, nullptr);
}


void TestLaTeXCodeGenerator::addPageBreak(QSharedPointer<Ld::RootElement> root) {
    QSharedPointer<Ld::PageBreakElement> pageBreak = Ld::Element::create("PageBreak")
                                                     .dynamicCast<Ld::PageBreakElement>();

    root->append(pageBreak, nullptr);
}
