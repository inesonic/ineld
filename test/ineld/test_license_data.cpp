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
* This file implements tests for the \ref LicenseData class.
***********************************************************************************************************************/

#include <QObject>
#include <QCoreApplication>
#include <QString>
#include <QSettings>
#include <QtTest/QtTest>

//#include <ld_license_data.h>

#include "test_license_data.h"

void TestLicenseData::initTestCase() {}


void TestLicenseData::testReadOnlyAccessors() {
//    QCOMPARE(Ld::LicenseData::customerName(), QString("Paul H. Smith"));
//    QCOMPARE(Ld::LicenseData::companyName(), QString("Inesonic, LLC"));
//    QCOMPARE(Ld::LicenseData::customerId(), static_cast<unsigned long long>(-1));
}


void TestLicenseData::testLicenseKeyAccessors() {
//    Ld::LicenseData::setLicenseKey(QString("10A-25%6*8BCa~b9Fd x"));
//    QCOMPARE(Ld::LicenseData::licenseKey(), QString("10A25-68BCA-B9FDX"));
}


void TestLicenseData::testLicenseValidation() {
    // This test assumes current watermarking with a build configured as:
    // Customer Name:   "Paul H. Smith"
    // Company Name:    "Inesonic, LLC"
    // Customer ID:     -1
    // License Number:  0
    // Sequence Number: 0

//    Ld::LicenseData::setLicenseKey("KALN9-N3V9J-DYUWU-6F31C-T6BCK");
//    bool isValidLicense = Ld::LicenseData::isValidLicense();

//    QCOMPARE(isValidLicense, true);

//    Ld::LicenseData::setLicenseKey("ALN9-N3V9J-DYUWU-6F31C-T6BCK");
//    isValidLicense = Ld::LicenseData::isValidLicense();

//    QCOMPARE(isValidLicense, false);

//    Ld::LicenseData::setLicenseKey("KALN9-N3V9J-DYUWU-6F31C-T6BC");
//    isValidLicense = Ld::LicenseData::isValidLicense();

//    QCOMPARE(isValidLicense, false);

//    Ld::LicenseData::setLicenseKey("KALN8-N3V9J-DYUWU-6F31C-T6BCK");
//    isValidLicense = Ld::LicenseData::isValidLicense();

//    QCOMPARE(isValidLicense, false);

//    Ld::LicenseData::setLicenseKey("KBKN9-N3V9J-DYUWU-6F31C-T6BCK");
//    isValidLicense = Ld::LicenseData::isValidLicense();

//    QCOMPARE(isValidLicense, false);

//    Ld::LicenseData::setLicenseKey("KALN9-N3V9J-DYUWU-6F31C-T6BCKA");
//    isValidLicense = Ld::LicenseData::isValidLicense();

//    QCOMPARE(isValidLicense, false);

//    Ld::LicenseData::setLicenseKey("KALN9-N3V9J-DYUWU-6F31C-T6BCK");
//    isValidLicense = Ld::LicenseData::isValidLicense();

//    QCOMPARE(isValidLicense, true);
}


void TestLicenseData::testLicenseKeyEncryptionDecryption() {
//    QString licenseKey("KALN9-N3V9J-DYUWU-6F31C-T6BCK");

//    QByteArray encrypted = Ld::LicenseData::encryptLicenseKey(licenseKey.toUtf8());
//    QByteArray decrypted = Ld::LicenseData::decryptLicenseKey(encrypted);

//    QCOMPARE(licenseKey, QString::fromUtf8(decrypted));
}
