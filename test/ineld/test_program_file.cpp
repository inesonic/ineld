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
* This file implements tests of the Ld::ProgramFile class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QObject>
#include <QSet>
#include <QVector>
#include <QFile>
#include <QImage>
#include <QSharedPointer>
#include <QString>
#include <QByteArray>
#include <QtTest/QtTest>

#include <ld_handle.h>
#include <ld_xml_writer.h>
#include <ld_xml_reader.h>
#include <ld_xml_attributes.h>
#include <ld_payload_data.h>
#include <ld_program_file.h>

#include "test_program_file.h"


TestProgramFile::TestProgramFile() {}


TestProgramFile::~TestProgramFile() {}


void TestProgramFile::initTestCase() {
    Ld::Handle::initialize(0x123456789ABCDEF0ULL);
}


void TestProgramFile::testConstructorsAndCopyMethods() {
    Ld::ProgramFile programFile;

    bool success = programFile.openNew();
    QVERIFY(success);

    success = programFile.saveAs(QString("test_container.dat"));
    QVERIFY(success);

    success = programFile.close();
    QVERIFY(success);

    QVERIFY(programFile.openMode() == Ld::ProgramFile::OpenMode::CLOSED);
}


void TestProgramFile::testFileLevelMethods() {
    QFile("test_container.dat").remove();
    QFile(".~working-test_container.dat").remove();

    Ld::ProgramFile programFile;

    bool success = programFile.openNew();
    QVERIFY(success);

    QVERIFY(programFile.openMode() == Ld::ProgramFile::OpenMode::OVERWRITE);

    success = programFile.saveAs(QString("test_container.dat"));
    QVERIFY(success);

    QVERIFY(programFile.openMode() == Ld::ProgramFile::OpenMode::READ_WRITE);

    QVERIFY(QFile(QString("test_container.dat")).exists());
    QVERIFY(QFile(QString(".~working-test_container.dat")).exists());

    success = programFile.close();
    QVERIFY(success);

    QVERIFY(programFile.openMode() == Ld::ProgramFile::OpenMode::CLOSED);

    QVERIFY(QFile(QString("test_container.dat")).exists());
    QVERIFY(!QFile(QString(".~working-test_container.dat")).exists());

    success = programFile.openExisting(QString("test_container.dat"), true);
    QVERIFY(success);

    QVERIFY(programFile.openMode() == Ld::ProgramFile::OpenMode::READ_ONLY);

    QVERIFY(QFile(QString("test_container.dat")).exists());
    QVERIFY(!QFile(QString(".~working-test_container.dat")).exists());

    success = programFile.close();
    QVERIFY(success);

    QVERIFY(programFile.openMode() == Ld::ProgramFile::OpenMode::CLOSED);

    success = programFile.openExisting(QString("test_container.dat"), false);
    QVERIFY(success);

    QVERIFY(programFile.openMode() == Ld::ProgramFile::OpenMode::READ_WRITE);

    QVERIFY(QFile(QString("test_container.dat")).exists());
    QVERIFY(QFile(QString(".~working-test_container.dat")).exists());

    success = programFile.save();
    QVERIFY(success);

    QVERIFY(programFile.openMode() == Ld::ProgramFile::OpenMode::READ_WRITE);

    QVERIFY(QFile(QString("test_container.dat")).exists());
    QVERIFY(QFile(QString(".~working-test_container.dat")).exists());

    success = programFile.close();
    QVERIFY(success);

    QVERIFY(programFile.openMode() == Ld::ProgramFile::OpenMode::CLOSED);

    QVERIFY(QFile(QString("test_container.dat")).exists());
    QVERIFY(!QFile(QString(".~working-test_container.dat")).exists());
}


void TestProgramFile::testPayloadData() {
    Ld::ProgramFile programFile1;

    bool success = programFile1.openNew();
    QVERIFY(success);

    Ld::PayloadData payloadData1 = programFile1.newPayload();
    QCOMPARE(payloadData1.payloadId(), 0U);

    QString testString1(
        "Oh freddled gruntbuggly, Thy micturations are to me As plurdled gabbleblotchits in a lurgid bee."
    );
    QByteArray testData1 = testString1.toLocal8Bit();

    success = payloadData1.writeData(testData1);
    QVERIFY(success);

    Ld::PayloadData payloadData2 = programFile1.newPayload();
    QCOMPARE(payloadData2.payloadId(), 1U);

    QString testString2(
        "Groop, I implore thee, my foonting turlingdromes, And hooptiosly drangle me with crinkly binglewurdles, "
        "Or I will rend three in the gobberwarts With my blurglecruncheon, see if I don't!"
    );
    QByteArray testData2 = testString2.toLocal8Bit();

    success = payloadData2.writeData(testData2);
    QVERIFY(success);

    success = programFile1.saveAs("test_container.aion");
    QVERIFY(success);

    success = programFile1.close();
    QVERIFY(success);

    Ld::ProgramFile programFile2;

    success = programFile2.openExisting("test_container.aion", false);
    QVERIFY(success);

    Ld::PayloadData payloadData3 = programFile2.payload(0);
    QVERIFY(payloadData3.isValid());
    QVERIFY(!payloadData3.isInvalid());
    QCOMPARE(payloadData3.payloadId(), 0U);

    { // Controls scope of payloadData4 and payloadData5 so they get purged later.

        Ld::PayloadData payloadData4 = programFile2.payload(1);
        QVERIFY(payloadData4.isValid());
        QVERIFY(!payloadData4.isInvalid());
        QCOMPARE(payloadData4.payloadId(), 1U);

        Ld::PayloadData payloadData5 = programFile2.payload(2);
        QVERIFY(payloadData5.isInvalid());
        QVERIFY(!payloadData5.isValid());
        QCOMPARE(payloadData5.payloadId(), Ld::PayloadData::invalidPayloadId);

        QByteArray testData3;
        success = payloadData3.readData(testData3);
        QVERIFY(success);

        QByteArray testData4;
        success = payloadData4.readData(testData4);
        QVERIFY(success);

        QCOMPARE(testData1, testData3);
        QCOMPARE(testData2, testData4);
    }

    programFile2.purgeUnreferencedPayloads();

    success = programFile2.save();
    QVERIFY(success);

    success = programFile2.close();
    QVERIFY(success);

    QVERIFY(payloadData3.isValid());
    QVERIFY(!payloadData3.isInvalid());
    QCOMPARE(payloadData3.payloadId(), Ld::PayloadData::invalidPayloadId);

    QByteArray testData5;
    success = payloadData3.readData(testData5);
    QVERIFY(success);

    QCOMPARE(testData5, testData1);

    Ld::ProgramFile programFile3;

    success = programFile3.openExisting("test_container.aion", false);
    QVERIFY(success);

    Ld::PayloadData payloadData6 = programFile3.payload(0);
    QVERIFY(payloadData6.isValid());
    QVERIFY(!payloadData6.isInvalid());
    QCOMPARE(payloadData6.payloadId(), 0U);

    Ld::PayloadData payloadData7 = programFile3.payload(1);
    QVERIFY(!payloadData7.isValid());
    QVERIFY(payloadData7.isInvalid());
    QCOMPARE(payloadData7.payloadId(), Ld::PayloadData::invalidPayloadId);

    success = programFile3.close();
    QVERIFY(success);
}


void TestProgramFile::testClonePayload() {
    Ld::ProgramFile programFile1;

    bool success = programFile1.openNew();
    QVERIFY(success);

    Ld::PayloadData payloadData1 = programFile1.newPayload();
    QCOMPARE(payloadData1.payloadId(), 0U);

    QString testString(
        "Twas brillig, and the slithy toves\n"
        "Did gyre and gimble in the wabe:\n"
        "All mimsy were the borogoves,\n"
        "And the mome raths outgrabe.\n"
        "\n"
        "Beware the Jabberwock, my son!\n"
        "The jaws that bite, the claws that catch!\n"
        "Beware the Jubjub bird, and shun\n"
        "The frumious Bandersnatch!\n"
    );
    QByteArray testData = testString.toLocal8Bit();

    success = payloadData1.writeData(testData);
    QVERIFY(success);

    Ld::ProgramFile programFile2;

    success = programFile2.openNew();
    QVERIFY(success);

    Ld::PayloadData            payloadData2 = programFile2.clonePayload(payloadData1);
    Ld::PayloadData::PayloadId payloadId = payloadData2.payloadId();

    success = programFile2.saveAs("test_container.aion");
    QVERIFY(success);

    success = programFile2.close();
    QVERIFY(success);

    success = programFile1.close();
    QVERIFY(success);

    Ld::ProgramFile programFile3;

    success = programFile3.openExisting("test_container.aion", true);
    QVERIFY(success);

    Ld::PayloadData payloadData3 = programFile3.payload(payloadId);
    QVERIFY(payloadData3.isValid());

    QByteArray readData;
    success = payloadData3.readData(readData);
    QVERIFY(success);

    QCOMPARE(testData, readData);

    success = programFile3.close();
    QVERIFY(success);
}


void TestProgramFile::testXmlReaderWriter() {
    Ld::ProgramFile programFile;

    bool success = programFile.openNew();
    QVERIFY(success);

    success = programFile.initializeXmlWriter();
    QVERIFY(success);

    QSharedPointer<Ld::XmlWriter> writer = programFile.writer();
    QVERIFY(!writer.isNull());

    writer->writeStartDocument();
    writer->writeDTD("<!DOCTYPE test>");
    writer->writeStartElement("foo");
    writer->writeAttribute("version", "1.0");
    writer->writeEndDocument();

    success = programFile.releaseXmlWriter();
    QVERIFY(success);

    success = programFile.saveAs("test_container.dat");
    QVERIFY(success);

    success = programFile.close();
    QVERIFY(success);

    success = programFile.openExisting("test_container.dat", true);
    QVERIFY(success);

    success = programFile.initializeXmlReader();
    QVERIFY(success);

    QSharedPointer<Ld::XmlReader> reader = programFile.reader();
    QVERIFY(!reader.isNull());

    QVERIFY(reader->readNextStartElement());

    #if (QT_VERSION < 0x060000)

        QVERIFY(reader->name() == "foo");

    #else

        QVERIFY(reader->name().toString() == "foo");

    #endif

    Ld::XmlAttributes attributes = reader->attributes();
    QVERIFY(attributes.count() == 1);
    QVERIFY(attributes.hasAttribute("version"));
    QVERIFY(attributes.value<QString>("version") == "1.0");

    success = programFile.releaseXmlReader();
    QVERIFY(success);

    success = programFile.close();
    QVERIFY(success);
}
