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
* This file implements the \ref Ld::FunctionElement class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>

#include "ld_format_structures.h"
#include "ld_data_type.h"
#include "ld_element.h"
#include "ld_function_format.h"
#include "ld_parenthesis_format.h"
#include "ld_variable_name.h"
#include "ld_identifier_container.h"
#include "ld_identifier_database.h"
#include "ld_function_database.h"
#include "ld_root_element.h"
#include "ld_assignment_operator_element.h"
#include "ld_power_operator_element.h"
#include "ld_function_visual.h"
#include "ld_list_placeholder_element.h"
#include "ld_function_element.h"

namespace Ld {
    const QString      FunctionElement::elementName("Function");
    const Capabilities FunctionElement::childProvides = (
          Capabilities::functionDeclarations
        | Capabilities::allFunctions
    );

    FunctionElement::FunctionElement() {}


    FunctionElement::~FunctionElement() {}


    Element* FunctionElement::creator(const QString&) {
        return new FunctionElement();
    }


    QString FunctionElement::typeName() const {
        return elementName;
    }


    QString FunctionElement::plugInName() const {
        return QString();
    }


    QString FunctionElement::description() const {
        return tr("function");
    }


    DataType::ValueType FunctionElement::valueType() const {
        DataType::ValueType result;

        if (currentIdentifier.isValid()) {
            result = currentIdentifier.dataType().valueType();
        } else {
            ElementPointer parent = FunctionElement::parent();
            if (!parent.isNull()) {
                FunctionData& function = FunctionDatabase::function(VariableName(text(0), text(1)));
                if (function.isValid()) {
                    QList<DataType::ValueType> parameterValueTypes  = FunctionElement::parameterValueTypes();
                    unsigned long              indexOfThisChild     = parent->indexOfChild(weakThis().toStrongRef());
                    Capabilities               requiredCapabilities = parent->parentRequires(indexOfThisChild);

                    FunctionVariant bestFitVariant = function.bestFitVariant(
                        requiredCapabilities,
                        parameterValueTypes
                    );

                    result = bestFitVariant.returnValueType();
                } else {
                    result = DataType::ValueType::NONE;
                }
            } else {
                result = DataType::ValueType::NONE;
            }
        }

        return result;
    }


    Element::ParenthesisStyle FunctionElement::parameterParenthesisStyle() const {
        ParenthesisStyle result = ParenthesisStyle::INVALID_OR_NONE;

        FormatPointer format = FunctionElement::format();
        if (format->capabilities().contains(FunctionFormat::formatName)) {
            QSharedPointer<FunctionFormat> functionFormat = format.dynamicCast<FunctionFormat>();
            if (functionFormat->forceParameterParenthesis()                                ||
                parenthesisRequiredBasedOnPosition(functionFormat->subscriptedParameter())    ) {
                result = functionFormat->parameterParenthesisStyle();
            }
        } else {
            if (parenthesisRequiredBasedOnPosition(false)) {
                if (format->capabilities().contains(ParenthesisFormat::formatName)) {
                    QSharedPointer<ParenthesisFormat> parenthesisFormat = format.dynamicCast<ParenthesisFormat>();
                    result = parenthesisFormat->parenthesisStyle();
                } else {
                    result = ParenthesisStyle::PARENTHESIS;
                }
            }
        }

        return result;
    }


    Capabilities FunctionElement::parentRequires(unsigned long) const {
        Capabilities result;

        ElementPointer parent = FunctionElement::parent();
        if (!parent.isNull()) {
            if (parent->typeName() == AssignmentOperatorElement::elementName) {
                unsigned long position = parent->indexOfChild(weakThis().toStrongRef());
                if (position == 0) {
                    result = Capabilities::functionDeclarationParameters;
                } else {
                    result = Capabilities::allRValues;
                }
            } else {
                result = Capabilities::allRValues;
            }
        }

        return result;
    }


    Capabilities FunctionElement::childProvidesCapabilities() const {
        return childProvides;
    }


    Element::Precedence FunctionElement::childPrecedence() const {
        return functionOperatorPrecedence;
    }


    Element::Precedence FunctionElement::parentPrecedence(unsigned long /* childIndex */) const {
        Precedence result;

        bool forceParameterParenthesis;
        bool hasSubscriptedParameter;
        FormatPointer format = FunctionElement::format();
        if (format->capabilities().contains(FunctionFormat::formatName)) {
            QSharedPointer<FunctionFormat> functionFormat = format.dynamicCast<FunctionFormat>();
            forceParameterParenthesis = functionFormat->forceParameterParenthesis();
            hasSubscriptedParameter   = functionFormat->subscriptedParameter();
        } else {
            forceParameterParenthesis = false;
            hasSubscriptedParameter   = false;
        }

        if (forceParameterParenthesis) {
            result = defaultPrecedence;
        } else {
            unsigned long numberRealChildren = 0;
            unsigned long numberChildren     = FunctionElement::numberChildren();
            unsigned long i                  = hasSubscriptedParameter ? 1 : 0;

            while (i<numberChildren && numberRealChildren < 2) {
                ElementPointer child = FunctionElement::child(i);
                if (!child.isNull() && !child->isPlaceholder()) {
                    ++numberRealChildren;
                }

                ++i;
            }

            if (numberRealChildren == 1) {
                ParenthesisStyle parenthesisStyle = parameterParenthesisStyle();
                if (parenthesisStyle == ParenthesisStyle::INVALID_OR_NONE) {
                    result = functionOperatorPrecedence;
                } else {
                    result = defaultPrecedence;
                }
            } else {
                result = defaultPrecedence;
            }
        }

        return result;
    }


    Capabilities FunctionElement::textCapabilities(unsigned long textOffset, unsigned regionIndex) const {
        return   regionIndex == 0 && textOffset == 0
               ? Capabilities::allAlphabeticSymbols
               : Capabilities::allAlphanumericSymbols;
    }


    unsigned FunctionElement::minimumNumberTextRegions() const {
        return 2;
    };


    unsigned FunctionElement::maximumNumberTextRegions() const {
        return 2;
    }


    bool FunctionElement::setNumberTextRegions(unsigned newNumberTextRegions) {
        return newNumberTextRegions == 2 ? true : false;
    }


    unsigned FunctionElement::numberTextRegions() const {
        return 2;
    }


    bool FunctionElement::setText(const QString& newText, unsigned regionNumber) {
        bool success;
        if (regionNumber <= 1) {
            success = true;

            if (newText != currentText[regionNumber]) {
                currentText[regionNumber] = newText;

                FunctionVisual* functionVisual = visual();
                if (functionVisual != nullptr) {
                    elementDataChanged();
                    functionVisual->textChanged(newText, regionNumber);
                }
            }
        } else {
            success = false;
        }

        return success;
    }


    QString FunctionElement::text(unsigned regionNumber) const {
        return regionNumber <= 1 ? currentText[regionNumber] : QString();
    }


    void FunctionElement::setVisual(FunctionVisual *newVisual) {
        ListElementBase::setVisual(newVisual);
    }


    FunctionVisual* FunctionElement::visual() const {
        return dynamic_cast<FunctionVisual*>(ListElementBase::visual());
    }


    void FunctionElement::setIdentifier(const IdentifierContainer& identifier) {
        currentIdentifier = identifier;
    }


    const IdentifierContainer& FunctionElement::identifier() const {
        return currentIdentifier;
    }


    ElementPointerList FunctionElement::parameterElements() const {
        ElementPointerList result;

        unsigned long numberChildren = FunctionElement::numberChildren();
        for (unsigned long childIndex=0 ; childIndex<numberChildren ; ++childIndex) {
            ElementPointer child = FunctionElement::child(childIndex);
            if (!child->isPlaceholder() || !child->text().isEmpty()) {
                result.append(child);
            }
        }

        return result;
    }


    void FunctionElement::writeAddAttributes(
            XmlAttributes&                  attributes,
            QSharedPointer<FormatOrganizer> formats,
            ProgramFile&                    programFile
        ) const {
        ListElementBase::writeAddAttributes(attributes, formats, programFile);

        attributes.append("t1", currentText[0]);

        if (!currentText[1].isEmpty()) {
            attributes.append("t2", currentText[1]);
        }
    }


    void FunctionElement::readAttributes(
            QSharedPointer<XmlReader>  reader,
            const XmlAttributes&       attributes,
            const FormatsByIdentifier& formats,
            ProgramFile&               programFile,
            unsigned                   xmlVersion
        ) {
        QString t1;
        QString t2;

        ListElementBase::readAttributes(reader, attributes, formats, programFile, xmlVersion);

        if (!reader->hasError()) {
            if (attributes.hasAttribute("t1")) {
                t1 = attributes.value<QString>("t1");
                if (t1.isEmpty()) {
                    QString tagName = reader->qualifiedName().toString();
                    reader->raiseError(tr("Tag \"%1\" has invalid attribute \"%1\"").arg(tagName).arg(t1));
                }
            } else {
                QString tagName = reader->qualifiedName().toString();
                reader->raiseError(tr("Tag \"%1\" is missing attribute t1").arg(tagName));
            }
        }

        if (!reader->hasError() && attributes.hasAttribute("t2")) {
            t2 = attributes.value<QString>("t2");
        }

        if (!reader->hasError()) {
            currentText[0] = t1;
            currentText[1] = t2;
        }
    }


    bool FunctionElement::parenthesisRequiredBasedOnPosition(bool firstParameterIsSubscripted) const {
        bool          parenthesisNeeded         = false;
        unsigned long numberChildren            = FunctionElement::numberChildren();
        unsigned long numberRealChildren        = 0;
        bool          multipleChildrenToProtect = false;

        ParenthesisStyle outsideParenthesisStyle = parenthesisStyle();
        if (outsideParenthesisStyle == ParenthesisStyle::INVALID_OR_NONE) {
            unsigned long i = firstParameterIsSubscripted ? 1 : 0;
            while (i<numberChildren && numberRealChildren < 2) {
                ElementPointer child = FunctionElement::child(i);
                assert(!child.isNull());

                if (!child->isPlaceholder()) {
                    ++numberRealChildren;
                }

                ++i;
            }

            multipleChildrenToProtect = (numberRealChildren >= 2);

            if (multipleChildrenToProtect) {
                parenthesisNeeded = true;
            } else {
                ElementPointer p = parent();
                if (!p.isNull()) {
                    parenthesisNeeded = !p->isFunctionLogicalStop(p->indexOfChild(weakThis().toStrongRef()));
                }
            }
        }

        return parenthesisNeeded;
    }


    QList<DataType::ValueType> FunctionElement::parameterValueTypes() const {
        QList<DataType::ValueType> result;

        unsigned long numberChildren = FunctionElement::numberChildren();
        for (unsigned long childIndex=0 ; childIndex<numberChildren ; ++childIndex) {
            ElementPointer child = FunctionElement::child(childIndex);
            if (!child->isPlaceholder()) {
                Ld::DataType::ValueType childValueType = child->valueType();
                if (childValueType == Ld::DataType::ValueType::NONE) {
                    childValueType = Ld::DataType::defaultDataType().valueType();
                }

                result.append(childValueType);
            }
        }

        return result;
    }
}
