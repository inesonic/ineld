##-*-makefile-*-########################################################################################################
# Copyright 2016 - 2023 Inesonic, LLC
#
# This file is licensed under two licenses.
#
# Inesonic Commercial License, Version 1:
#   All rights reserved.  Inesonic, LLC retains all rights to this software, including the right to relicense the
#   software in source or binary formats under different terms.  Unauthorized use under the terms of this license is
#   strictly prohibited.
#
# GNU Public License, Version 2:
#   This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
#   License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later
#   version.
#
#   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
#   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
#   details.
#
#   You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
#   Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
########################################################################################################################

########################################################################################################################
# Basic build characteristics
#

TEMPLATE = lib

QT += core
CONFIG += shared embed_manifest_exe c++14

SOURCES = inesonic_test_plug_in.cpp

########################################################################################################################
# Libraries
#

INCLUDEPATH += $${INECONTAINER_INCLUDE}
INCLUDEPATH += $${INEQCONTAINER_INCLUDE}
INCLUDEPATH += $${INECBE_INCLUDE}
INCLUDEPATH += $${INEUTIL_INCLUDE}

LIBS += -L$${INECONTAINER_LIBDIR} -linecontainer
LIBS += -L$${INEQCONTAINER_LIBDIR} -lineqcontainer
LIBS += -L$${INECBE_LIBDIR} -linecbe
LIBS += -L$${INEUTIL_LIBDIR} -lineutil

INELD_BASE = ../../ineld
INCLUDEPATH += ../../ineld/include

unix {
    CONFIG(debug, debug|release) {
        LIBS += -L$${INELD_BASE}/build/debug/ -lineld
        !macx {
            PRE_TARGETDEPS += $${INELD_BASE}/build/debug/libineld.so
        } else {
            PRE_TARGETDEPS += $${INELD_BASE}/build/debug/libineld.dylib
        }
    } else {
        LIBS += -L$${INELD_BASE}/build/release/ -lineld
        !macx {
            PRE_TARGETDEPS += $${INELD_BASE}/build/release/libineld.so
        } else {
            PRE_TARGETDEPS += $${INELD_BASE}/build/release/libineld.dylib
        }
    }
}

win32 {
    CONFIG(debug, debug|release) {
        LIBS += $${INELD_BASE}/build/Debug/ineld.lib
        PRE_TARGETDEPS += $${INELD_BASE}/build/Debug/ineld.lib
    } else {
        LIBS += $${INELD_BASE}/build/Release/ineld.lib
        PRE_TARGETDEPS += $${INELD_BASE}/build/Release/ineld.lib
    }
}

INCLUDEPATH += $${INEM_INCLUDE}
LIBS += -L$${INEM_LIBDIR} -linem

include("$${SOURCE_ROOT}/libraries/inecontainer/inecontainer.pri")
include("$${SOURCE_ROOT}/libraries/ineqcontainer/ineqcontainer.pri")
include("$${SOURCE_ROOT}/libraries/ineutil/ineutil.pri")
include("$${SOURCE_ROOT}/libraries/inecbe/inecbe.pri")
include("$${SOURCE_ROOT}/libraries/ineld/ineld.pri")
include("$${SOURCE_ROOT}/libraries/inem/inem.pri")

########################################################################################################################
# Locate build intermediate and output products
#

TARGET = inesonic_test_plug_in

CONFIG(debug, debug|release) {
    unix:DESTDIR = build/debug
    win32:DESTDIR = build/Debug
} else {
    unix:DESTDIR = build/release
    win32:DESTDIR = build/Release
}
