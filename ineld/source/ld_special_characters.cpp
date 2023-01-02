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
* This file implements the \ref Ld::SpecialCommands class.
***********************************************************************************************************************/

#include <QCoreApplication>
#include <QLocale>
#include <QMap>
#include <QMultiMap>
#include <QHash>
#include <QSet>

#include <utility>

#include "ld_capabilities.h"
#include "ld_special_characters.h"

/***********************************************************************************************************************
 * File scope character database
 */

struct SpecialCharacter {
    QString                             command;
    Ld::SpecialCharacters::UnicodeValue unicode;
    QString                             description;
};

static const SpecialCharacter greekCharacters[] = {
//    command                 unicode description
    { QT_TR_NOOP("alpha"),    0x03B1, QT_TR_NOOP("Greek lowercase alpha") },
    { QT_TR_NOOP("beta"),     0x03B2, QT_TR_NOOP("Greek lowercase beta") },
    { QT_TR_NOOP("gamma"),    0x03B3, QT_TR_NOOP("Greek lowercase gamma") },
    { QT_TR_NOOP("delta"),    0x03B4, QT_TR_NOOP("Greek lowercase delta") },
    { QT_TR_NOOP("epsilon"),  0x03B5, QT_TR_NOOP("Greek lowercase epsilon") },
    { QT_TR_NOOP("zeta"),     0x03B6, QT_TR_NOOP("Greek lowercase zeta") },
    { QT_TR_NOOP("eta"),      0x03B7, QT_TR_NOOP("Greek lowercase eta") },
    { QT_TR_NOOP("theta"),    0x03B8, QT_TR_NOOP("Greek lowercase theta") },
    { QT_TR_NOOP("iota"),     0x03B9, QT_TR_NOOP("Greek lowercase iota") },
    { QT_TR_NOOP("kappa"),    0x03BA, QT_TR_NOOP("Greek lowercase kappa") },
    { QT_TR_NOOP("lambda"),   0x03BB, QT_TR_NOOP("Greek lowercase lambda") },
    { QT_TR_NOOP("mu"),       0x03BC, QT_TR_NOOP("Greek lowercase mu") },
    { QT_TR_NOOP("nu"),       0x03BD, QT_TR_NOOP("Greek lowercase nu") },
    { QT_TR_NOOP("xi"),       0x03BE, QT_TR_NOOP("Greek lowercase xi") },
    { QT_TR_NOOP("omicron"),  0x03BF, QT_TR_NOOP("Greek lowercase omicron") },
    { QT_TR_NOOP("pi"),       0x03C0, QT_TR_NOOP("Greek lowercase pi") },
    { QT_TR_NOOP("rho"),      0x03C1, QT_TR_NOOP("Greek lowercase rho") },
    { QT_TR_NOOP("varsigma"), 0x03C2, QT_TR_NOOP("Greek lowercase sigma") },
    { QT_TR_NOOP("sigma"),    0x03C3, QT_TR_NOOP("Greek lowercase sigma") },
    { QT_TR_NOOP("tau"),      0x03C4, QT_TR_NOOP("Greek lowercase tau") },
    { QT_TR_NOOP("upsilon"),  0x03C5, QT_TR_NOOP("Greek lowercase upsilon") },
    { QT_TR_NOOP("varphi"),   0x03C6, QT_TR_NOOP("Greek lowercase phi") },
    { QT_TR_NOOP("chi"),      0x03C7, QT_TR_NOOP("Greek lowercase chi") },
    { QT_TR_NOOP("psi"),      0x03C8, QT_TR_NOOP("Greek lowercase psi") },
    { QT_TR_NOOP("omega"),    0x03C9, QT_TR_NOOP("Greek lowercase omega") },
    { QT_TR_NOOP("vartheta"), 0x03D1, QT_TR_NOOP("Greek lowercase theta") },
    { QT_TR_NOOP("phi"),      0x03D5, QT_TR_NOOP("Greek lowercase phi") },
    { QT_TR_NOOP("varpi"),    0x03D6, QT_TR_NOOP("Greek lowercase pi") },
    { QT_TR_NOOP("digamma"),  0x03DD, QT_TR_NOOP("Greek lowercase digamma") },
    { QT_TR_NOOP("varkappa"), 0x03F0, QT_TR_NOOP("Greek lowercase kappa") },
    { QT_TR_NOOP("Alpha"),    0x0391, QT_TR_NOOP("Greek uppercase alpha") },
    { QT_TR_NOOP("Beta"),     0x0392, QT_TR_NOOP("Greek uppercase beta") },
    { QT_TR_NOOP("Gamma"),    0x0393, QT_TR_NOOP("Greek uppercase gamma") },
    { QT_TR_NOOP("Delta"),    0x0394, QT_TR_NOOP("Greek uppercase delta") },
    { QT_TR_NOOP("Epsilon"),  0x0395, QT_TR_NOOP("Greek uppercase epsilon") },
    { QT_TR_NOOP("Zeta"),     0x0396, QT_TR_NOOP("Greek uppercase zeta") },
    { QT_TR_NOOP("Eta"),      0x0397, QT_TR_NOOP("Greek uppercase eta") },
    { QT_TR_NOOP("Theta"),    0x0398, QT_TR_NOOP("Greek uppercase theta") },
    { QT_TR_NOOP("Iota"),     0x0399, QT_TR_NOOP("Greek uppercase iota") },
    { QT_TR_NOOP("Kappa"),    0x039A, QT_TR_NOOP("Greek uppercase kappa") },
    { QT_TR_NOOP("Lambda"),   0x039B, QT_TR_NOOP("Greek uppercase lambda") },
    { QT_TR_NOOP("Mu"),       0x039C, QT_TR_NOOP("Greek uppercase mu") },
    { QT_TR_NOOP("Nu"),       0x039D, QT_TR_NOOP("Greek uppercase nu") },
    { QT_TR_NOOP("Xi"),       0x039E, QT_TR_NOOP("Greek uppercase xi") },
    { QT_TR_NOOP("Omicron"),  0x039F, QT_TR_NOOP("Greek uppercase omicron") },
    { QT_TR_NOOP("Pi"),       0x03A0, QT_TR_NOOP("Greek uppercase pi") },
    { QT_TR_NOOP("Rho"),      0x03A1, QT_TR_NOOP("Greek uppercase rho") },
    { QT_TR_NOOP("Sigma"),    0x03A3, QT_TR_NOOP("Greek uppercase sigma") },
    { QT_TR_NOOP("Tau"),      0x03A4, QT_TR_NOOP("Greek uppercase tau") },
    { QT_TR_NOOP("Upsilon"),  0x03A5, QT_TR_NOOP("Greek uppercase upsilon") },
    { QT_TR_NOOP("Phi"),      0x03A6, QT_TR_NOOP("Greek uppercase phi") },
    { QT_TR_NOOP("Chi"),      0x03A7, QT_TR_NOOP("Greek uppercase chi") },
    { QT_TR_NOOP("Psi"),      0x03A8, QT_TR_NOOP("Greek uppercase psi") },
    { QT_TR_NOOP("Omega"),    0x03A9, QT_TR_NOOP("Greek uppercase omega") },
    { QT_TR_NOOP("Digamma"),  0x03DC, QT_TR_NOOP("Greek uppercase digamma") },

    { QString(),              0x0000, QString() } // Ends the list.
};

static const SpecialCharacter diacriticalCharacters[] = {
    { QT_TR_NOOP("`a"),   0x00E0, QT_TR_NOOP("Lowercase \"a\" with grave") },
    { QT_TR_NOOP("'a"),   0x00E1, QT_TR_NOOP("Lowercase \"a\" with acute") },
    { QT_TR_NOOP("^a"),   0x00E2, QT_TR_NOOP("Lowercase \"a\" with circumflex") },
    { QT_TR_NOOP("\"a"),  0x00E4, QT_TR_NOOP("Lowercase \"a\" with umlaut") },
    { QT_TR_NOOP("~a"),   0x00E3, QT_TR_NOOP("Lowercase \"a\" with tilde") },
    { QT_TR_NOOP("ka"),   0x0105, QT_TR_NOOP("Lowercase \"a\" with ogonek") },
    { QT_TR_NOOP("=a"),   0x0101, QT_TR_NOOP("Lowercase \"a\" with macron") },
    { QT_TR_NOOP(".a"),   0x0227, QT_TR_NOOP("Lowercase \"a\" with dot above") },
    { QT_TR_NOOP("da"),   0x1EA1, QT_TR_NOOP("Lowercase \"a\" with dot below") },
    { QT_TR_NOOP("ua"),   0x0103, QT_TR_NOOP("Lowercase \"a\" with breve") },
    { QT_TR_NOOP(".b"),   0x1E03, QT_TR_NOOP("Lowercase \"b\" with dot above") },
    { QT_TR_NOOP("db"),   0x1E05, QT_TR_NOOP("Lowercase \"b\" with dot below") },
    { QT_TR_NOOP("'c"),   0x0107, QT_TR_NOOP("Lowercase \"c\" with acute") },
    { QT_TR_NOOP("^c"),   0x0109, QT_TR_NOOP("Lowercase \"c\" with circumflex") },
    { QT_TR_NOOP(".c"),   0x010B, QT_TR_NOOP("Lowercase \"c\" with dot above") },
    { QT_TR_NOOP("vc"),   0x010D, QT_TR_NOOP("Lowercase \"c\" with caron") },
    { QT_TR_NOOP("cd"),   0x1E11, QT_TR_NOOP("Lowercase \"d\" with cedilla") },
    { QT_TR_NOOP("bd"),   0x1E0F, QT_TR_NOOP("Lowercase \"d\" with bar under") },
    { QT_TR_NOOP(".d"),   0x1E0B, QT_TR_NOOP("Lowercase \"d\" with dot above") },
    { QT_TR_NOOP("`e"),   0x00E8, QT_TR_NOOP("Lowercase \"e\" with grave") },
    { QT_TR_NOOP("'e"),   0x00E9, QT_TR_NOOP("Lowercase \"e\" with acute") },
    { QT_TR_NOOP("^e"),   0x00EA, QT_TR_NOOP("Lowercase \"e\" with circumflex") },
    { QT_TR_NOOP("\"e"),  0x00EB, QT_TR_NOOP("Lowercase \"e\" with umlaut") },
    { QT_TR_NOOP("ce"),   0x0229, QT_TR_NOOP("Lowercase \"e\" with cedilla") },
    { QT_TR_NOOP("ke"),   0x0119, QT_TR_NOOP("Lowercase \"e\" with ogonek") },
    { QT_TR_NOOP("=e"),   0x0113, QT_TR_NOOP("Lowercase \"e\" with macron") },
    { QT_TR_NOOP(".e"),   0x0117, QT_TR_NOOP("Lowercase \"e\" with dot above") },
    { QT_TR_NOOP("de"),   0x1E0D, QT_TR_NOOP("Lowercase \"e\" with dot below") },
    { QT_TR_NOOP("ue"),   0x0115, QT_TR_NOOP("Lowercase \"e\" with breve") },
    { QT_TR_NOOP("ve"),   0x011B, QT_TR_NOOP("Lowercase \"e\" with caron") },
    { QT_TR_NOOP(".f"),   0x1E1F, QT_TR_NOOP("Lowercase \"f\" with dot above") },
    { QT_TR_NOOP("'g"),   0x01F5, QT_TR_NOOP("Lowercase \"g\" with acute") },
    { QT_TR_NOOP("^g"),   0x011D, QT_TR_NOOP("Lowercase \"g\" with circumflex") },
    { QT_TR_NOOP("cg"),   0x0123, QT_TR_NOOP("Lowercase \"g\" with cedilla") },
    { QT_TR_NOOP("=g"),   0x1E21, QT_TR_NOOP("Lowercase \"g\" with macron") },
    { QT_TR_NOOP(".g"),   0x0121, QT_TR_NOOP("Lowercase \"g\" with dot above") },
    { QT_TR_NOOP("ug"),   0x011F, QT_TR_NOOP("Lowercase \"g\" with breve") },
    { QT_TR_NOOP("vg"),   0x01E7, QT_TR_NOOP("Lowercase \"g\" with caron") },
    { QT_TR_NOOP("^h"),   0x0125, QT_TR_NOOP("Lowercase \"h\" with circumflex") },
    { QT_TR_NOOP("\"h"),  0x1E27, QT_TR_NOOP("Lowercase \"h\" with umlaut") },
    { QT_TR_NOOP("ch"),   0x1E29, QT_TR_NOOP("Lowercase \"h\" with cedilla") },
    { QT_TR_NOOP(".h"),   0x1E23, QT_TR_NOOP("Lowercase \"h\" with dot above") },
    { QT_TR_NOOP("d{h}"), 0x1E25, QT_TR_NOOP("Lowercase \"h\" with dot below") },
    { QT_TR_NOOP("vh"),   0x021F, QT_TR_NOOP("Lowercase \"h\" with caron") },
    { QT_TR_NOOP("`i"),   0x00EC, QT_TR_NOOP("Lowercase \"i\" with grave") },
    { QT_TR_NOOP("'i"),   0x00ED, QT_TR_NOOP("Lowercase \"i\" with acute") },
    { QT_TR_NOOP("^i"),   0x00EE, QT_TR_NOOP("Lowercase \"i\" with circumflex") },
    { QT_TR_NOOP("\"i"),  0x00EF, QT_TR_NOOP("Lowercase \"i\" with umlaut") },
    { QT_TR_NOOP("~i"),   0x0129, QT_TR_NOOP("Lowercase \"i\" with tilde") },
    { QT_TR_NOOP("ki"),   0x012F, QT_TR_NOOP("Lowercase \"i\" with ogonek") },
    { QT_TR_NOOP("di"),   0x1ECB, QT_TR_NOOP("Lowercase \"i\" with dot below") },
    { QT_TR_NOOP("ui"),   0x012D, QT_TR_NOOP("Lowercase \"i\" with breve") },
    { QT_TR_NOOP("^j"),   0x0135, QT_TR_NOOP("Lowercase \"j\" with circumflex") },
    { QT_TR_NOOP("'k"),   0x1E31, QT_TR_NOOP("Lowercase \"k\" with acute") },
    { QT_TR_NOOP("ck"),   0x0137, QT_TR_NOOP("Lowercase \"k\" with cedilla") },
    { QT_TR_NOOP("dk"),   0x1E33, QT_TR_NOOP("Lowercase \"k\" with dot below") },
    { QT_TR_NOOP("vk"),   0x01E9, QT_TR_NOOP("Lowercase \"k\" with caron") },
    { QT_TR_NOOP("cl"),   0x013C, QT_TR_NOOP("Lowercase \"l\" with cedilla") },
    { QT_TR_NOOP("=l"),   0x012B, QT_TR_NOOP("Lowercase \"l\" with macron") },
    { QT_TR_NOOP("dl"),   0x1E37, QT_TR_NOOP("Lowercase \"l\" with dot below") },
    { QT_TR_NOOP("'m"),   0x1E3F, QT_TR_NOOP("Lowercase \"m\" with acute") },
    { QT_TR_NOOP(".m"),   0x1E41, QT_TR_NOOP("Lowercase \"m\" with dot above") },
    { QT_TR_NOOP("dm"),   0x1E43, QT_TR_NOOP("Lowercase \"m\" with dot below") },
    { QT_TR_NOOP("`n"),   0x01F9, QT_TR_NOOP("Lowercase \"n\" with grave") },
    { QT_TR_NOOP("cn"),   0x0146, QT_TR_NOOP("Lowercase \"n\" with cedilla") },
    { QT_TR_NOOP(".n"),   0x1E45, QT_TR_NOOP("Lowercase \"n\" with dot above") },
    { QT_TR_NOOP("dn"),   0x1E47, QT_TR_NOOP("Lowercase \"n\" with dot below") },
    { QT_TR_NOOP("`o"),   0x00F2, QT_TR_NOOP("Lowercase \"o\" with grave") },
    { QT_TR_NOOP("'o"),   0x00F3, QT_TR_NOOP("Lowercase \"o\" with acute") },
    { QT_TR_NOOP("^o"),   0x00F4, QT_TR_NOOP("Lowercase \"o\" with circumflex") },
    { QT_TR_NOOP("\"o"),  0x00F6, QT_TR_NOOP("Lowercase \"o\" with umlaut") },
    { QT_TR_NOOP("Ho"),   0x0151, QT_TR_NOOP("Lowercase \"o\" with hungarumlaut") },
    { QT_TR_NOOP("~o"),   0x00F5, QT_TR_NOOP("Lowercase \"o\" with tilde") },
    { QT_TR_NOOP("=o"),   0x014D, QT_TR_NOOP("Lowercase \"o\" with macron") },
    { QT_TR_NOOP(".o"),   0x022F, QT_TR_NOOP("Lowercase \"o\" with dot above") },
    { QT_TR_NOOP("do"),   0x1ECD, QT_TR_NOOP("Lowercase \"o\" with dot below") },
    { QT_TR_NOOP("uo"),   0x014F, QT_TR_NOOP("Lowercase \"o\" with breve") },
    { QT_TR_NOOP("vo"),   0x01EB, QT_TR_NOOP("Lowercase \"o\" with caron") },
    { QT_TR_NOOP("'p"),   0x1E55, QT_TR_NOOP("Lowercase \"p\" with acute") },
    { QT_TR_NOOP(".p"),   0x1E57, QT_TR_NOOP("Lowercase \"p\" with dot above") },
    { QT_TR_NOOP("cr"),   0x0157, QT_TR_NOOP("Lowercase \"r\" with cedilla") },
    { QT_TR_NOOP(".r"),   0x1E59, QT_TR_NOOP("Lowercase \"r\" with dot above") },
    { QT_TR_NOOP("dr"),   0x1E5B, QT_TR_NOOP("Lowercase \"r\" with dot below") },
    { QT_TR_NOOP("vr"),   0x0159, QT_TR_NOOP("Lowercase \"r\" with caron") },
    { QT_TR_NOOP("^s"),   0x015D, QT_TR_NOOP("Lowercase \"s\" with circumflex") },
    { QT_TR_NOOP("cs"),   0x015F, QT_TR_NOOP("Lowercase \"s\" with cedilla") },
    { QT_TR_NOOP(".s"),   0x1E61, QT_TR_NOOP("Lowercase \"s\" with dot above") },
    { QT_TR_NOOP("ds"),   0x1E63, QT_TR_NOOP("Lowercase \"s\" with dot below") },
    { QT_TR_NOOP("vs"),   0x0161, QT_TR_NOOP("Lowercase \"s\" with caron") },
    { QT_TR_NOOP("\"t"),  0x1E97, QT_TR_NOOP("Lowercase \"t\" with umlaut") },
    { QT_TR_NOOP("ct"),   0x0163, QT_TR_NOOP("Lowercase \"t\" with cedilla") },
    { QT_TR_NOOP(".t"),   0x1E6B, QT_TR_NOOP("Lowercase \"t\" with dot above") },
    { QT_TR_NOOP("dt"),   0x1E6D, QT_TR_NOOP("Lowercase \"t\" with dot below") },
    { QT_TR_NOOP("`u"),   0x00F9, QT_TR_NOOP("Lowercase \"u\" with grave") },
    { QT_TR_NOOP("'u"),   0x00FA, QT_TR_NOOP("Lowercase \"u\" with acute") },
    { QT_TR_NOOP("^u"),   0x00FB, QT_TR_NOOP("Lowercase \"u\" with circumflex") },
    { QT_TR_NOOP("\"u"),  0x00FC, QT_TR_NOOP("Lowercase \"u\" with umlaut") },
    { QT_TR_NOOP("Hu"),   0x0171, QT_TR_NOOP("Lowercase \"u\" with hungarumlaut") },
    { QT_TR_NOOP("ku"),   0x0173, QT_TR_NOOP("Lowercase \"u\" with ogonek") },
    { QT_TR_NOOP("=u"),   0x016B, QT_TR_NOOP("Lowercase \"u\" with macron") },
    { QT_TR_NOOP("du"),   0x1EE5, QT_TR_NOOP("Lowercase \"u\" with dot below") },
    { QT_TR_NOOP("ru"),   0x016F, QT_TR_NOOP("Lowercase \"u\" with ring") },
    { QT_TR_NOOP("uu"),   0x016D, QT_TR_NOOP("Lowercase \"u\" with breve") },
    { QT_TR_NOOP("vu"),   0x01D4, QT_TR_NOOP("Lowercase \"u\" with caron") },
    { QT_TR_NOOP("~v"),   0x1E7D, QT_TR_NOOP("Lowercase \"v\" with tilde") },
    { QT_TR_NOOP("dv"),   0x1E7F, QT_TR_NOOP("Lowercase \"v\" with dot below") },
    { QT_TR_NOOP("`w"),   0x1E81, QT_TR_NOOP("Lowercase \"w\" with grave") },
    { QT_TR_NOOP("'w"),   0x1E83, QT_TR_NOOP("Lowercase \"w\" with acute") },
    { QT_TR_NOOP("^w"),   0x0175, QT_TR_NOOP("Lowercase \"w\" with circumflex") },
    { QT_TR_NOOP("\"w"),  0x1E85, QT_TR_NOOP("Lowercase \"w\" with umlaut") },
    { QT_TR_NOOP(".w"),   0x1E87, QT_TR_NOOP("Lowercase \"w\" with dot above") },
    { QT_TR_NOOP("dw"),   0x1E89, QT_TR_NOOP("Lowercase \"w\" with dot below") },
    { QT_TR_NOOP("rw"),   0x1E98, QT_TR_NOOP("Lowercase \"w\" with ring") },
    { QT_TR_NOOP("\"x"),  0x1E8D, QT_TR_NOOP("Lowercase \"x\" with umlaut") },
    { QT_TR_NOOP(".x"),   0x1E8B, QT_TR_NOOP("Lowercase \"x\" with dot above") },
    { QT_TR_NOOP("`y"),   0x1EF3, QT_TR_NOOP("Lowercase \"y\" with grave") },
    { QT_TR_NOOP("'y"),   0x00FD, QT_TR_NOOP("Lowercase \"y\" with acute") },
    { QT_TR_NOOP("^y"),   0x0177, QT_TR_NOOP("Lowercase \"y\" with circumflex") },
    { QT_TR_NOOP("\"y"),  0x00FF, QT_TR_NOOP("Lowercase \"y\" with umlaut") },
    { QT_TR_NOOP("~y"),   0x1EF9, QT_TR_NOOP("Lowercase \"y\" with tilde") },
    { QT_TR_NOOP(".y"),   0x1E8F, QT_TR_NOOP("Lowercase \"y\" with dot above") },
    { QT_TR_NOOP("dy"),   0x1EF5, QT_TR_NOOP("Lowercase \"y\" with dot below") },
    { QT_TR_NOOP("ry"),   0x1E99, QT_TR_NOOP("Lowercase \"y\" with ring") },
    { QT_TR_NOOP("^z"),   0x1E91, QT_TR_NOOP("Lowercase \"z\" with circumflex") },
    { QT_TR_NOOP(".z"),   0x017C, QT_TR_NOOP("Lowercase \"z\" with dot above") },
    { QT_TR_NOOP("dz"),   0x1E93, QT_TR_NOOP("Lowercase \"z\" with dot below") },
    { QT_TR_NOOP("vz"),   0x017E, QT_TR_NOOP("Lowercase \"z\" with caron") },
    { QT_TR_NOOP("`A"),   0x00C0, QT_TR_NOOP("Uppercase \"A\" with grave") },
    { QT_TR_NOOP("'A"),   0x00C1, QT_TR_NOOP("Uppercase \"A\" with acute") },
    { QT_TR_NOOP("^A"),   0x00C2, QT_TR_NOOP("Uppercase \"A\" with circumflex") },
    { QT_TR_NOOP("\"A"),  0x00C4, QT_TR_NOOP("Uppercase \"A\" with umlaut") },
    { QT_TR_NOOP("~A"),   0x00C3, QT_TR_NOOP("Uppercase \"A\" with tilde") },
    { QT_TR_NOOP("kA"),   0x0104, QT_TR_NOOP("Uppercase \"A\" with ogonek") },
    { QT_TR_NOOP("=A"),   0x0100, QT_TR_NOOP("Uppercase \"A\" with macron") },
    { QT_TR_NOOP(".A"),   0x0226, QT_TR_NOOP("Uppercase \"A\" with dot above") },
    { QT_TR_NOOP("dA"),   0x1EA0, QT_TR_NOOP("Uppercase \"A\" with dot below") },
    { QT_TR_NOOP("uA"),   0x0102, QT_TR_NOOP("Uppercase \"A\" with breve") },
    { QT_TR_NOOP(".B"),   0x1E02, QT_TR_NOOP("Uppercase \"B\" with dot above") },
    { QT_TR_NOOP("dB"),   0x1E04, QT_TR_NOOP("Uppercase \"B\" with dot below") },
    { QT_TR_NOOP("'C"),   0x0106, QT_TR_NOOP("Uppercase \"C\" with acute") },
    { QT_TR_NOOP("^C"),   0x0108, QT_TR_NOOP("Uppercase \"C\" with circumflex") },
    { QT_TR_NOOP(".C"),   0x010A, QT_TR_NOOP("Uppercase \"C\" with dot above") },
    { QT_TR_NOOP("vC"),   0x010C, QT_TR_NOOP("Uppercase \"C\" with caron") },
    { QT_TR_NOOP("cD"),   0x1E10, QT_TR_NOOP("Uppercase \"D\" with cedilla") },
    { QT_TR_NOOP("bD"),   0x1E0E, QT_TR_NOOP("Uppercase \"D\" with bar under") },
    { QT_TR_NOOP(".D"),   0x1E0A, QT_TR_NOOP("Uppercase \"D\" with dot above") },
    { QT_TR_NOOP("dD"),   0x1E0C, QT_TR_NOOP("Uppercase \"D\" with dot below") },
    { QT_TR_NOOP("vD"),   0x010E, QT_TR_NOOP("Uppercase \"D\" with caron") },
    { QT_TR_NOOP("`E"),   0x00C8, QT_TR_NOOP("Uppercase \"E\" with grave") },
    { QT_TR_NOOP("'E"),   0x00C9, QT_TR_NOOP("Uppercase \"E\" with acute") },
    { QT_TR_NOOP("^E"),   0x00CA, QT_TR_NOOP("Uppercase \"E\" with circumflex") },
    { QT_TR_NOOP("\"E"),  0x00CB, QT_TR_NOOP("Uppercase \"E\" with umlaut") },
    { QT_TR_NOOP("cE"),   0x0228, QT_TR_NOOP("Uppercase \"E\" with cedilla") },
    { QT_TR_NOOP("kE"),   0x0118, QT_TR_NOOP("Uppercase \"E\" with ogonek") },
    { QT_TR_NOOP("=E"),   0x0112, QT_TR_NOOP("Uppercase \"E\" with macron") },
    { QT_TR_NOOP(".E"),   0x0116, QT_TR_NOOP("Uppercase \"E\" with dot above") },
    { QT_TR_NOOP("uE"),   0x0114, QT_TR_NOOP("Uppercase \"E\" with breve") },
    { QT_TR_NOOP("vE"),   0x011A, QT_TR_NOOP("Uppercase \"E\" with caron") },
    { QT_TR_NOOP(".F"),   0x1E1E, QT_TR_NOOP("Uppercase \"F\" with dot above") },
    { QT_TR_NOOP("'G"),   0x01F4, QT_TR_NOOP("Uppercase \"G\" with acute") },
    { QT_TR_NOOP("^G"),   0x011C, QT_TR_NOOP("Uppercase \"G\" with circumflex") },
    { QT_TR_NOOP("cG"),   0x0122, QT_TR_NOOP("Uppercase \"G\" with cedilla") },
    { QT_TR_NOOP("=G"),   0x1E20, QT_TR_NOOP("Uppercase \"G\" with macron") },
    { QT_TR_NOOP(".G"),   0x0120, QT_TR_NOOP("Uppercase \"G\" with dot above") },
    { QT_TR_NOOP("uG"),   0x011E, QT_TR_NOOP("Uppercase \"G\" with breve") },
    { QT_TR_NOOP("vG"),   0x01E6, QT_TR_NOOP("Uppercase \"G\" with caron") },
    { QT_TR_NOOP("^H"),   0x0124, QT_TR_NOOP("Uppercase \"H\" with circumflex") },
    { QT_TR_NOOP("\"H"),  0x1E26, QT_TR_NOOP("Uppercase \"H\" with umlaut") },
    { QT_TR_NOOP(".H"),   0x1E22, QT_TR_NOOP("Uppercase \"H\" with dot above") },
    { QT_TR_NOOP("d{H}"), 0x1E24, QT_TR_NOOP("Uppercase \"H\" with dot below") },
    { QT_TR_NOOP("vH"),   0x021E, QT_TR_NOOP("Uppercase \"H\" with caron") },
    { QT_TR_NOOP("cH"),   0x1E28, QT_TR_NOOP("Uppercase \"H\" with cedilla") },
    { QT_TR_NOOP("`I"),   0x00CC, QT_TR_NOOP("Uppercase \"I\" with grave") },
    { QT_TR_NOOP("'I"),   0x00CD, QT_TR_NOOP("Uppercase \"I\" with acute") },
    { QT_TR_NOOP("^I"),   0x00CE, QT_TR_NOOP("Uppercase \"I\" with circumflex") },
    { QT_TR_NOOP("\"I"),  0x00CF, QT_TR_NOOP("Uppercase \"I\" with umlaut") },
    { QT_TR_NOOP("~I"),   0x0128, QT_TR_NOOP("Uppercase \"I\" with tilde") },
    { QT_TR_NOOP("kI"),   0x012E, QT_TR_NOOP("Uppercase \"I\" with ogonek") },
    { QT_TR_NOOP("=I"),   0x012A, QT_TR_NOOP("Uppercase \"I\" with macron") },
    { QT_TR_NOOP(".I"),   0x0130, QT_TR_NOOP("Uppercase \"I\" with dot above") },
    { QT_TR_NOOP("dI"),   0x1ECA, QT_TR_NOOP("Uppercase \"I\" with dot below") },
    { QT_TR_NOOP("uI"),   0x012C, QT_TR_NOOP("Uppercase \"I\" with breve") },
    { QT_TR_NOOP("^J"),   0x0134, QT_TR_NOOP("Uppercase \"J\" with circumflex") },
    { QT_TR_NOOP("`K"),   0x1E30, QT_TR_NOOP("Uppercase \"K\" with grave") },
    { QT_TR_NOOP("cK"),   0x0136, QT_TR_NOOP("Uppercase \"K\" with cedilla") },
    { QT_TR_NOOP("dK"),   0x1E32, QT_TR_NOOP("Uppercase \"K\" with dot below") },
    { QT_TR_NOOP("vK"),   0x01E8, QT_TR_NOOP("Uppercase \"K\" with caron") },
    { QT_TR_NOOP("cL"),   0x013B, QT_TR_NOOP("Uppercase \"L\" with cedilla") },
    { QT_TR_NOOP("dL"),   0x1E36, QT_TR_NOOP("Uppercase \"L\" with dot below") },
    { QT_TR_NOOP("'M"),   0x1E3E, QT_TR_NOOP("Uppercase \"M\" with acute") },
    { QT_TR_NOOP(".M"),   0x1E40, QT_TR_NOOP("Uppercase \"M\" with dot above") },
    { QT_TR_NOOP("dM"),   0x1E42, QT_TR_NOOP("Uppercase \"M\" with dot below") },
    { QT_TR_NOOP("`N"),   0x01F8, QT_TR_NOOP("Uppercase \"N\" with grave") },
    { QT_TR_NOOP("cN"),   0x0145, QT_TR_NOOP("Uppercase \"N\" with cedilla") },
    { QT_TR_NOOP(".N"),   0x1E44, QT_TR_NOOP("Uppercase \"N\" with dot above") },
    { QT_TR_NOOP("dN"),   0x1E46, QT_TR_NOOP("Uppercase \"N\" with dot below") },
    { QT_TR_NOOP("`O"),   0x00D2, QT_TR_NOOP("Uppercase \"O\" with grave") },
    { QT_TR_NOOP("'O"),   0x00D3, QT_TR_NOOP("Uppercase \"O\" with acute") },
    { QT_TR_NOOP("^O"),   0x00D4, QT_TR_NOOP("Uppercase \"O\" with circumflex") },
    { QT_TR_NOOP("\"O"),  0x00D6, QT_TR_NOOP("Uppercase \"O\" with umlaut") },
    { QT_TR_NOOP("HO"),   0x0150, QT_TR_NOOP("Uppercase \"O\" with hungarumlaut") },
    { QT_TR_NOOP("~O"),   0x00D5, QT_TR_NOOP("Uppercase \"O\" with tilde") },
    { QT_TR_NOOP("=O"),   0x014C, QT_TR_NOOP("Uppercase \"O\" with macron") },
    { QT_TR_NOOP(".O"),   0x022E, QT_TR_NOOP("Uppercase \"O\" with dot above") },
    { QT_TR_NOOP("dO"),   0x1ECC, QT_TR_NOOP("Uppercase \"O\" with dot below") },
    { QT_TR_NOOP("uO"),   0x014E, QT_TR_NOOP("Uppercase \"O\" with breve") },
    { QT_TR_NOOP("vO"),   0x01EA, QT_TR_NOOP("Uppercase \"O\" with caron") },
    { QT_TR_NOOP("'P"),   0x1E54, QT_TR_NOOP("Uppercase \"P\" with acute") },
    { QT_TR_NOOP(".P"),   0x1E56, QT_TR_NOOP("Uppercase \"P\" with dot above") },
    { QT_TR_NOOP("cR"),   0x0156, QT_TR_NOOP("Uppercase \"R\" with cedilla") },
    { QT_TR_NOOP(".R"),   0x1E58, QT_TR_NOOP("Uppercase \"R\" with dot above") },
    { QT_TR_NOOP("dR"),   0x1E5A, QT_TR_NOOP("Uppercase \"R\" with dot below") },
    { QT_TR_NOOP("vR"),   0x0158, QT_TR_NOOP("Uppercase \"R\" with caron") },
    { QT_TR_NOOP("^S"),   0x015C, QT_TR_NOOP("Uppercase \"S\" with circumflex") },
    { QT_TR_NOOP("cS"),   0x015E, QT_TR_NOOP("Uppercase \"S\" with cedilla") },
    { QT_TR_NOOP(".S"),   0x1E60, QT_TR_NOOP("Uppercase \"S\" with dot above") },
    { QT_TR_NOOP("dS"),   0x1E62, QT_TR_NOOP("Uppercase \"S\" with dot below") },
    { QT_TR_NOOP("vS"),   0x0160, QT_TR_NOOP("Uppercase \"S\" with caron") },
    { QT_TR_NOOP("cT"),   0x0162, QT_TR_NOOP("Uppercase \"T\" with cedilla") },
    { QT_TR_NOOP(".T"),   0x1E6A, QT_TR_NOOP("Uppercase \"T\" with dot above") },
    { QT_TR_NOOP("dT"),   0x1E6C, QT_TR_NOOP("Uppercase \"T\" with dot below") },
    { QT_TR_NOOP("vT"),   0x0164, QT_TR_NOOP("Uppercase \"T\" with caron") },
    { QT_TR_NOOP("`U"),   0x00D9, QT_TR_NOOP("Uppercase \"U\" with grave") },
    { QT_TR_NOOP("'U"),   0x00DA, QT_TR_NOOP("Uppercase \"U\" with acute") },
    { QT_TR_NOOP("^U"),   0x00DB, QT_TR_NOOP("Uppercase \"U\" with circumflex") },
    { QT_TR_NOOP("\"U"),  0x00DC, QT_TR_NOOP("Uppercase \"U\" with umlaut") },
    { QT_TR_NOOP("HU"),   0x0170, QT_TR_NOOP("Uppercase \"U\" with hungarumlaut") },
    { QT_TR_NOOP("kU"),   0x0172, QT_TR_NOOP("Uppercase \"U\" with ogonek") },
    { QT_TR_NOOP("=U"),   0x016A, QT_TR_NOOP("Uppercase \"U\" with macron") },
    { QT_TR_NOOP("dU"),   0x1EE4, QT_TR_NOOP("Uppercase \"U\" with dot below") },
    { QT_TR_NOOP("rU"),   0x016E, QT_TR_NOOP("Uppercase \"U\" with ring") },
    { QT_TR_NOOP("uU"),   0x016C, QT_TR_NOOP("Uppercase \"U\" with breve") },
    { QT_TR_NOOP("vU"),   0x01D3, QT_TR_NOOP("Uppercase \"U\" with caron") },
    { QT_TR_NOOP("~V"),   0x1E7C, QT_TR_NOOP("Uppercase \"V\" with tilde") },
    { QT_TR_NOOP("dV"),   0x1E7E, QT_TR_NOOP("Uppercase \"V\" with dot below") },
    { QT_TR_NOOP("`W"),   0x1E80, QT_TR_NOOP("Uppercase \"W\" with grave") },
    { QT_TR_NOOP("'W"),   0x1E82, QT_TR_NOOP("Uppercase \"W\" with acute") },
    { QT_TR_NOOP("^W"),   0x0174, QT_TR_NOOP("Uppercase \"W\" with circumflex") },
    { QT_TR_NOOP("\"W"),  0x1E84, QT_TR_NOOP("Uppercase \"W\" with umlaut") },
    { QT_TR_NOOP(".W"),   0x1E86, QT_TR_NOOP("Uppercase \"W\" with dot above") },
    { QT_TR_NOOP("dW"),   0x1E88, QT_TR_NOOP("Uppercase \"W\" with dot below") },
    { QT_TR_NOOP("\"X"),  0x1E8C, QT_TR_NOOP("Uppercase \"X\" with umlaut") },
    { QT_TR_NOOP(".X"),   0x1E8A, QT_TR_NOOP("Uppercase \"X\" with dot above") },
    { QT_TR_NOOP("`Y"),   0x1EF2, QT_TR_NOOP("Uppercase \"Y\" with grave") },
    { QT_TR_NOOP("'Y"),   0x00DD, QT_TR_NOOP("Uppercase \"Y\" with acute") },
    { QT_TR_NOOP("^Y"),   0x0176, QT_TR_NOOP("Uppercase \"Y\" with circumflex") },
    { QT_TR_NOOP("~Y"),   0x1EF8, QT_TR_NOOP("Uppercase \"Y\" with tilde") },
    { QT_TR_NOOP(".Y"),   0x1E8E, QT_TR_NOOP("Uppercase \"Y\" with dot above") },
    { QT_TR_NOOP("dY"),   0x1EF4, QT_TR_NOOP("Uppercase \"Y\" with dot below") },
    { QT_TR_NOOP("^Z"),   0x1E90, QT_TR_NOOP("Uppercase \"Z\" with circumflex") },
    { QT_TR_NOOP(".Z"),   0x017B, QT_TR_NOOP("Uppercase \"Z\" with dot above") },
    { QT_TR_NOOP("dZ"),   0x1E92, QT_TR_NOOP("Uppercase \"Z\" with dot below") },
    { QT_TR_NOOP("vZ"),   0x017D, QT_TR_NOOP("Uppercase \"Z\" with caron") },

    { QT_TR_NOOP("SS"),   0x1E9E, QT_TR_NOOP("Uppercase eszett") },
    { QT_TR_NOOP("ss"),   0x00DF, QT_TR_NOOP("Lowercase eszett") },
    { QT_TR_NOOP("o"),    0x00F8, QT_TR_NOOP("Lowercase \"o\" with slash") },
    { QT_TR_NOOP("O"),    0x00D8, QT_TR_NOOP("Uppercase \"O\" with slash") },
    { QT_TR_NOOP("l"),    0x0142, QT_TR_NOOP("Lowercase \"l\" with stroke") },
    { QT_TR_NOOP("L"),    0x0141, QT_TR_NOOP("Uppercase \"L\" with stroke") },
    { QT_TR_NOOP("AE"),   0x00C6, QT_TR_NOOP("Uppercase ash") },
    { QT_TR_NOOP("ae"),   0x00E6, QT_TR_NOOP("Lowercase ash") },
    { QT_TR_NOOP("OE"),   0x0152, QT_TR_NOOP("Uppercase ligature \"oe\"") },
    { QT_TR_NOOP("oe"),   0x0153, QT_TR_NOOP("Lowercase ligature \"oe\"") },
    { QT_TR_NOOP("NG"),   0x014A, QT_TR_NOOP("Uppercase eng") },
    { QT_TR_NOOP("ng"),   0x014B, QT_TR_NOOP("Lowercase eng") },
    { QT_TR_NOOP("IJ"),   0x0132, QT_TR_NOOP("Uppercase \"ij\" digraph") },
    { QT_TR_NOOP("ij"),   0x0133, QT_TR_NOOP("Lowercase \"ij\" digraph") },

    { QString(),          0x0000, QString() } // Ends the list.
};

static const SpecialCharacter specialCharacters[] = {
    { QT_TR_NOOP("hbar"),   0x210F, QT_TR_NOOP("Planck's contant symbol") },
    { QT_TR_NOOP("aleph"),  0x05D0, QT_TR_NOOP("Hebrew character aleph") },
    { QT_TR_NOOP("beth"),   0x05D1, QT_TR_NOOP("Hebrew character beth") },
    { QT_TR_NOOP("gimel"),  0x05D2, QT_TR_NOOP("Hebrew character gimel") },
    { QT_TR_NOOP("daleth"), 0x05D3, QT_TR_NOOP("Hebrew character daleth") },
    { QT_TR_NOOP("dh"),     0x00F0, QT_TR_NOOP("Futhark lowercase eth") },
    { QT_TR_NOOP("DH"),     0x00D0, QT_TR_NOOP("Futhark uppercase eth") },
    { QT_TR_NOOP("th"),     0x00FE, QT_TR_NOOP("Futhark lowercase thorn") },
    { QT_TR_NOOP("TH"),     0x00DE, QT_TR_NOOP("Futhark uppercase thorn") },

    { QString(),            0x0000, QString() } // Ends the list.
};

static const SpecialCharacter specialSymbols[] = {
    { QT_TR_NOOP("textquestiondown"), 0x00BF, QT_TR_NOOP("Inverted question mark") },
    { QT_TR_NOOP("textexclaimdown"),  0x00A1, QT_TR_NOOP("Inverted exclamation point") },
    { QT_TR_NOOP("guillemotleft"),    0x00AB, QT_TR_NOOP("Left guillemet") },
    { QT_TR_NOOP("guillemotright"),   0x00BB, QT_TR_NOOP("Right guillemet") },
    { QT_TR_NOOP("leftarrow"),        0x2190, QT_TR_NOOP("Left arrow") },
    { QT_TR_NOOP("rightarrow"),       0x2192, QT_TR_NOOP("Right arrow") },
    { QT_TR_NOOP("uparrow"),          0x2191, QT_TR_NOOP("Upward arrow") },
    { QT_TR_NOOP("downarrow"),        0x2193, QT_TR_NOOP("Downward arrow") },
    { QT_TR_NOOP("Leftarrow"),        0x21D0, QT_TR_NOOP("Left arrow") },
    { QT_TR_NOOP("Rightarrow"),       0x21D2, QT_TR_NOOP("Right arrow") },
    { QT_TR_NOOP("Uparrow"),          0x21D1, QT_TR_NOOP("Upward arrow") },
    { QT_TR_NOOP("Downarrow"),        0x21D3, QT_TR_NOOP("Downward arrow") },
    { QT_TR_NOOP("leftrightarrow"),   0x2194, QT_TR_NOOP("Left, right arrow") },
    { QT_TR_NOOP("Leftrightarrow"),   0x21D4, QT_TR_NOOP("Left, right arrow") },
    { QT_TR_NOOP("updownarrow"),      0x2195, QT_TR_NOOP("Up, down arrow") },
    { QT_TR_NOOP("Updownarrow"),      0x21D5, QT_TR_NOOP("Up, down arrow") },
    { QT_TR_NOOP("mho"),              0x2127, QT_TR_NOOP("mho/admittance symbol") },
    { QT_TR_NOOP("dag"),              0x2020, QT_TR_NOOP("Daggar") },
    { QT_TR_NOOP("ddat"),             0x2021, QT_TR_NOOP("Double dagger" ) },
    { QT_TR_NOOP("P"),                0x00B6, QT_TR_NOOP("Pilcrow") },
    { QT_TR_NOOP("copyright"),        0x00A9, QT_TR_NOOP("Copyright") },
    { QT_TR_NOOP("S"),                0x00A7, QT_TR_NOOP("Section") },
    { QT_TR_NOOP("pounds"),           0x00A3, QT_TR_NOOP("Pounds") },
    { QT_TR_NOOP("textyen"),          0x00A5, QT_TR_NOOP("Yen") },
    { QT_TR_NOOP("infty"),            0x221E, QT_TR_NOOP("Infinity") },

    { QString(),                    0x0000, QString() } // Ends the list.
};

/***********************************************************************************************************************
 * SpecialCharacters
 */

namespace Ld {
    const QString SpecialCharacters::defaultPrefix("\\");

    typedef SpecialCharacters::UnicodeValue UnicodeValue;

    QMap<QString, QString>      SpecialCharacters::currentGreekDescriptionsByCommand;
    QMap<QString, QString>      SpecialCharacters::currentDiacriticalDescriptionsByCommand;
    QMap<QString, QString>      SpecialCharacters::currentSpecialCharactersDescriptionsByCommand;
    QMap<QString, QString>      SpecialCharacters::currentSpecialSymbolsDescriptionsByCommand;
    QMap<UnicodeValue, QString> SpecialCharacters::currentGreekDescriptionsByUnicodeValue;
    QMap<UnicodeValue, QString> SpecialCharacters::currentDiacriticalDescriptionsByUnicodeValue;
    QMap<UnicodeValue, QString> SpecialCharacters::currentSpecialCharactersDescriptionsByUnicodeValue;
    QMap<UnicodeValue, QString> SpecialCharacters::currentSpecialSymbolsDescriptionsByUnicodeValue;
    QMap<UnicodeValue, QString> SpecialCharacters::currentGreekCommandsByUnicodeValue;
    QMap<UnicodeValue, QString> SpecialCharacters::currentDiacriticalCommandsByUnicodeValue;
    QMap<UnicodeValue, QString> SpecialCharacters::currentSpecialCharactersCommandsByUnicodeValue;
    QMap<UnicodeValue, QString> SpecialCharacters::currentSpecialSymbolsCommandsByUnicodeValue;

    QHash<QString, SpecialCharacters::UnicodeValue> SpecialCharacters::currentUnicodeValuesByCommand;

    QList<Capabilities> SpecialCharacters::currentLatin1TextCapabilitiesByUnicodeValue;

    QMap<QString, QString> SpecialCharacters::characterDescriptionsByCommand(
            bool includeGreekSymbols,
            bool includeDiacriticalSymbols,
            bool includeSpecialCharacters,
            bool includeSpecialSymbols
        ) {
        prepareDatabase();

        QMap<QString, QString> result;

        if (includeGreekSymbols) {
            mergeMap(result, currentGreekDescriptionsByCommand);
        }

        if (includeDiacriticalSymbols) {
            mergeMap(result, currentDiacriticalDescriptionsByCommand);
        }

        if (includeSpecialCharacters) {
            mergeMap(result, currentSpecialCharactersDescriptionsByCommand);
        }

        if (includeSpecialSymbols) {
            mergeMap(result, currentSpecialSymbolsDescriptionsByCommand);
        }

        return result;
    }


    QMap<SpecialCharacters::UnicodeValue, QString> SpecialCharacters::characterDescriptionsByUnicodeValue(
            bool includeGreekSymbols,
            bool includeDiacriticalSymbols,
            bool includeSpecialCharacters,
            bool includeSpecialSymbols
        ) {
        prepareDatabase();

        QMap<UnicodeValue, QString> result;

        if (includeGreekSymbols) {
            mergeMap(result, currentGreekDescriptionsByUnicodeValue);
        }

        if (includeDiacriticalSymbols) {
            mergeMap(result, currentDiacriticalDescriptionsByUnicodeValue);
        }

        if (includeSpecialCharacters) {
            mergeMap(result, currentSpecialCharactersDescriptionsByUnicodeValue);
        }

        if (includeSpecialSymbols) {
            mergeMap(result, currentSpecialSymbolsDescriptionsByUnicodeValue);
        }

        return result;
    }


    QMap<SpecialCharacters::UnicodeValue, QString> SpecialCharacters::characterCommandsByUnicodeValue(
            bool includeGreekSymbols,
            bool includeDiacriticalSymbols,
            bool includeSpecialCharacters,
            bool includeSpecialSymbols
        ) {
        prepareDatabase();

        QMap<UnicodeValue, QString> result;

        if (includeGreekSymbols) {
            mergeMap(result, currentGreekCommandsByUnicodeValue);
        }

        if (includeDiacriticalSymbols) {
            mergeMap(result, currentDiacriticalCommandsByUnicodeValue);
        }

        if (includeSpecialCharacters) {
            mergeMap(result, currentSpecialCharactersCommandsByUnicodeValue);
        }

        if (includeSpecialSymbols) {
            mergeMap(result, currentSpecialSymbolsCommandsByUnicodeValue);
        }

        return result;
    }


    SpecialCharacters::UnicodeValue SpecialCharacters::unicodeForCommand(const QString& commandString) {
        return currentUnicodeValuesByCommand.value(commandString, 0);
    }


    QList<SpecialCharacters::UnicodeValue> SpecialCharacters::recommendedCharacterOrder(
            bool includeGreekSymbols,
            bool includeDiacriticalSymbols,
            bool includeSpecialCharacters,
            bool includeSpecialSymbols
        ) {
        QList<UnicodeValue> result;

        if (includeGreekSymbols) {
            const SpecialCharacter* specialCharacter = greekCharacters;
            while (specialCharacter->unicode != 0x0000) {
                result.append(specialCharacter->unicode);
                ++specialCharacter;
            }
        }

        if (includeDiacriticalSymbols) {
            const SpecialCharacter* specialCharacter = diacriticalCharacters;
            while (specialCharacter->unicode != 0x0000) {
                result.append(specialCharacter->unicode);
                ++specialCharacter;
            }
        }

        if (includeSpecialCharacters) {
            const SpecialCharacter* specialCharacter = specialCharacters;
            while (specialCharacter->unicode != 0x0000) {
                result.append(specialCharacter->unicode);
                ++specialCharacter;
            }
        }

        if (includeSpecialSymbols) {
            const SpecialCharacter* specialCharacter = specialSymbols;
            while (specialCharacter->unicode != 0x0000) {
                result.append(specialCharacter->unicode);
                ++specialCharacter;
            }
        }

        return result;
    }


    Capabilities SpecialCharacters::requiredCapabilities(const QString& text) {
        prepareDatabase();

        Capabilities requirements;

        for (QString::const_iterator it=text.constBegin(),end=text.constEnd(); it!=end ; ++it) {
            UnicodeValue unicode = it->unicode();
            if (unicode <= 0x7F) {
                if (it->isDigit()) {
                    requirements |= Capabilities::numericDecimalSymbols;
                } else if (it->isLetter()) {
                    requirements |= Capabilities::alphabeticSymbols;
                } else if (it->isSymbol() || it->isSpace() || it->isPunct()) {
                    requirements |= Capabilities::specialSymbols;

                    if (*it == QChar('<') || *it == QChar('"') || *it == QChar('\'')) {
                        requirements |= Capabilities::stringIdentifierSymbols;
                    }
                }
            } else {
                if (currentGreekCommandsByUnicodeValue.contains(unicode)) {
                    requirements |= Capabilities::nonAsciiAlphabeticSymbols;
                } else if (currentDiacriticalCommandsByUnicodeValue.contains(unicode)) {
                    requirements |= Capabilities::nonAsciiAlphabeticSymbols;
                } else if (currentSpecialCharactersCommandsByUnicodeValue.contains(unicode)) {
                    requirements |= Capabilities::nonAsciiAlphabeticSymbols;
                } else if (currentSpecialSymbolsCommandsByUnicodeValue.contains(unicode)) {
                    requirements |= Capabilities::specialSymbols;
                } else {
                    requirements |= Capabilities::specialSymbols;
                }
            }
        }

        return requirements;
    }


    bool SpecialCharacters::textMeetsRequirements(const QString& text, const Capabilities& requirements) {
        prepareDatabase();
        prepareCapabilitiesByCharacter();

        bool                    meetsRequirements = true;
        QString::const_iterator it                = text.constBegin();
        QString::const_iterator end               = text.constEnd();

        while (meetsRequirements && it != end) {
            QChar        character = *it;
            UnicodeValue unicode   = character.unicode();
            if (unicode < 128) {
                if (!requirements.intersects(currentLatin1TextCapabilitiesByUnicodeValue.at(unicode))) {
                    meetsRequirements = false;
                }
            } else if (character.isLetter()) {
                if (!requirements.intersects(Capabilities::nonAsciiAlphabeticSymbols)) {
                    meetsRequirements = false;
                }
            } else if (it->isSymbol() || it->isSpace() || it->isPunct()) {
                if (!requirements.intersects(Capabilities::specialSymbols)) {
                    meetsRequirements  = false;
                }
            }

            ++it;
        }

        return meetsRequirements;
    }


    void SpecialCharacters::prepareDatabase() {
        // We assume there's always at least one greek character.

        if (currentGreekDescriptionsByCommand.isEmpty()) {
            QSet<UnicodeValue> unicodeValues; // Used to check for duplicates.

            const SpecialCharacter* specialCharacter = greekCharacters;
            while (specialCharacter->unicode != 0x0000) {
                QString      command         = specialCharacter->command;
                UnicodeValue unicode         = specialCharacter->unicode;
                QString      descriptionText = specialCharacter->description;
                QString      description     = QString("%1 (%2)").arg(descriptionText).arg(QChar(unicode));

                Q_ASSERT(!unicodeValues.contains(unicode));
                Q_ASSERT(!currentUnicodeValuesByCommand.contains(command));

                unicodeValues.insert(unicode);
                currentUnicodeValuesByCommand.insert(command, unicode);

                currentGreekDescriptionsByCommand.insert(command, description);
                currentGreekDescriptionsByUnicodeValue.insert(unicode, description);
                currentGreekCommandsByUnicodeValue.insert(unicode, command);

                ++specialCharacter;
            }

            specialCharacter = diacriticalCharacters;
            while (specialCharacter->unicode != 0x0000) {
                QString      command         = specialCharacter->command;
                UnicodeValue unicode         = specialCharacter->unicode;
                QString      descriptionText = specialCharacter->description;
                QString      description     = QString("%1 (%2)").arg(descriptionText).arg(QChar(unicode));

                Q_ASSERT(!unicodeValues.contains(unicode));
                Q_ASSERT(!currentUnicodeValuesByCommand.contains(command));

                unicodeValues.insert(unicode);
                currentUnicodeValuesByCommand.insert(command, unicode);

                currentDiacriticalDescriptionsByCommand.insert(command, description);
                currentDiacriticalDescriptionsByUnicodeValue.insert(unicode, description);
                currentDiacriticalCommandsByUnicodeValue.insert(unicode, command);

                ++specialCharacter;
            }

            specialCharacter = specialCharacters;
            while (specialCharacter->unicode != 0x0000) {
                QString      command         = specialCharacter->command;
                UnicodeValue unicode         = specialCharacter->unicode;
                QString      descriptionText = specialCharacter->description;
                QString      description     = QString("%1 (%2)").arg(descriptionText).arg(QChar(unicode));

                Q_ASSERT(!unicodeValues.contains(unicode));
                Q_ASSERT(!currentUnicodeValuesByCommand.contains(command));

                unicodeValues.insert(unicode);
                currentUnicodeValuesByCommand.insert(command, unicode);

                currentSpecialCharactersDescriptionsByCommand.insert(command, description);
                currentSpecialCharactersDescriptionsByUnicodeValue.insert(unicode, description);
                currentSpecialCharactersCommandsByUnicodeValue.insert(unicode, command);

                ++specialCharacter;
            }

            specialCharacter = specialSymbols;
            while (specialCharacter->unicode != 0x0000) {
                QString      command         = specialCharacter->command;
                UnicodeValue unicode         = specialCharacter->unicode;
                QString      descriptionText = specialCharacter->description;
                QString      description     = QString("%1 (%2)").arg(descriptionText).arg(QChar(unicode));

                Q_ASSERT(!unicodeValues.contains(unicode));
                Q_ASSERT(!currentUnicodeValuesByCommand.contains(command));

                unicodeValues.insert(unicode);
                currentUnicodeValuesByCommand.insert(command, unicode);

                currentSpecialSymbolsDescriptionsByCommand.insert(command, description);
                currentSpecialSymbolsDescriptionsByUnicodeValue.insert(unicode, description);
                currentSpecialSymbolsCommandsByUnicodeValue.insert(unicode, command);

                ++specialCharacter;
            }
        }
    }


    void SpecialCharacters::prepareCapabilitiesByCharacter() {
        if (currentLatin1TextCapabilitiesByUnicodeValue.isEmpty()) {
            struct NumericSymbols {
                QChar            character;
                Ld::Capabilities capabilities;
            };

            QLocale locale       = QLocale::system();

            #if (QT_VERSION < 0x060000)

                QChar decimalPoint = locale.decimalPoint();
                QChar exponent     = locale.exponential();

            #else

                QChar decimalPoint = locale.decimalPoint().at(0);
                QChar exponent     = locale.exponential().at(0);

            #endif

            const NumericSymbols numericSymbols[] = {
                { QChar('0'),     Ld::Capabilities::numericDecimalSymbols
                                | Ld::Capabilities::numericBinarySymbols
                                | Ld::Capabilities::numericHexidecimalSymbols
                },
                { QChar('1'),     Ld::Capabilities::numericDecimalSymbols
                                | Ld::Capabilities::numericBinarySymbols
                                | Ld::Capabilities::numericHexidecimalSymbols
                },
                { QChar('2'),     Ld::Capabilities::numericDecimalSymbols
                                | Ld::Capabilities::numericHexidecimalSymbols
                },
                { QChar('3'),     Ld::Capabilities::numericDecimalSymbols
                                | Ld::Capabilities::numericHexidecimalSymbols
                },
                { QChar('4'),     Ld::Capabilities::numericDecimalSymbols
                                | Ld::Capabilities::numericHexidecimalSymbols
                },
                { QChar('5'),     Ld::Capabilities::numericDecimalSymbols
                                | Ld::Capabilities::numericHexidecimalSymbols
                },
                { QChar('6'),     Ld::Capabilities::numericDecimalSymbols
                                | Ld::Capabilities::numericHexidecimalSymbols
                },
                { QChar('7'),     Ld::Capabilities::numericDecimalSymbols
                                | Ld::Capabilities::numericHexidecimalSymbols
                },
                { QChar('8'),     Ld::Capabilities::numericDecimalSymbols
                                | Ld::Capabilities::numericHexidecimalSymbols
                },
                { QChar('9'),     Ld::Capabilities::numericDecimalSymbols
                                | Ld::Capabilities::numericHexidecimalSymbols
                },
                { QChar('A'),     Ld::Capabilities::numericHexidecimalSymbols },
                { QChar('B'),     Ld::Capabilities::numericHexidecimalSymbols },
                { QChar('C'),     Ld::Capabilities::numericHexidecimalSymbols },
                { QChar('D'),     Ld::Capabilities::numericHexidecimalSymbols },
                { QChar('E'),     Ld::Capabilities::numericHexidecimalSymbols },
                { QChar('F'),     Ld::Capabilities::numericHexidecimalSymbols },
                { QChar('a'),     Ld::Capabilities::numericHexidecimalSymbols },
                { QChar('b'),     Ld::Capabilities::numericHexidecimalSymbols
                                | Ld::Capabilities::numericBinaryBaseIdentifierSymbols
                },
                { QChar('c'),     Ld::Capabilities::numericHexidecimalSymbols },
                { QChar('d'),     Ld::Capabilities::numericHexidecimalSymbols },
                { QChar('e'),     Ld::Capabilities::numericHexidecimalSymbols },
                { QChar('f'),     Ld::Capabilities::numericHexidecimalSymbols },
                { QChar('x'),     Ld::Capabilities::numericHexidecimalBaseIdentifierSymbols },
                { QChar('i'),     Ld::Capabilities::numericImaginaryUnitSymbols },
                { QChar('j'),     Ld::Capabilities::numericImaginaryUnitSymbols },
                { QChar('+'),     Ld::Capabilities::numericSignSymbols },
                { QChar('-'),     Ld::Capabilities::numericSignSymbols },
                { decimalPoint,   Ld::Capabilities::numericDecimalPointSymbols },
                { QChar('<'),     Ld::Capabilities::stringIdentifierSymbols },
                { QChar('\''),    Ld::Capabilities::stringIdentifierSymbols },
                { QChar('"'),     Ld::Capabilities::stringIdentifierSymbols },
                { QChar('\0'),    Ld::Capabilities() }
            };

            currentLatin1TextCapabilitiesByUnicodeValue.clear();
            for (unsigned unicode=0 ; unicode<128 ; ++unicode) {
                Capabilities capabilities;
                QChar        character(unicode);

                if (character.isDigit()) {
                    capabilities |= Capabilities::numericDecimalSymbols;
                } else if (character.isLetter()) {
                    capabilities |= Capabilities::alphabeticSymbols;
                } else if (character.isSymbol() || character.isSpace() || character.isPunct()) {
                    capabilities |= Capabilities::specialSymbols;
                }

                currentLatin1TextCapabilitiesByUnicodeValue.append(capabilities);
            }

            const NumericSymbols* symbols = numericSymbols;
            while (symbols->character != QChar('\0')) {
                UnicodeValue unicode         = static_cast<UnicodeValue>(symbols->character.unicode());
                Capabilities newCapabilities = symbols->capabilities;
                currentLatin1TextCapabilitiesByUnicodeValue[unicode] |= newCapabilities;

                ++symbols;
            }

            Capabilities numericExponentialSymbols = Capabilities::numericExponentialSymbols;
            currentLatin1TextCapabilitiesByUnicodeValue[exponent.toLower().unicode()] |= numericExponentialSymbols;
            currentLatin1TextCapabilitiesByUnicodeValue[exponent.toUpper().unicode()] |= numericExponentialSymbols;
        }
    }


    QMap<QString, QString>& SpecialCharacters::mergeMap(
            QMap<QString, QString>&       toBeUpdated,
            const QMap<QString, QString>& source
        ) {
        for (QMap<QString, QString>::const_iterator it=source.constBegin(),end=source.constEnd() ; it!=end ; ++it) {
            toBeUpdated[it.key()] = it.value();
        }

        return toBeUpdated;
    }


    QMap<SpecialCharacters::UnicodeValue, QString>& SpecialCharacters::mergeMap(
            QMap<SpecialCharacters::UnicodeValue, QString>&       toBeUpdated,
            const QMap<SpecialCharacters::UnicodeValue, QString>& source
        ) {
        for (  QMap<UnicodeValue, QString>::const_iterator it = source.constBegin(), end = source.constEnd()
             ; it!=end
             ; ++it
            ) {
            toBeUpdated[it.key()] = it.value();
        }

        return toBeUpdated;
    }
}
