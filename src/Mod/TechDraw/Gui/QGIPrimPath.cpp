/***************************************************************************
 *   Copyright (c) 2016 WandererFan <wandererfan@gmail.com>                *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/

#include "PreCompiled.h"
#ifndef _PreComp_
# include <cassert>

# include <QGraphicsScene>
# include <QGraphicsSceneHoverEvent>
# include <QPainter>
# include <QStyleOptionGraphicsItem>
#endif

#include <App/Application.h>

#include "QGIPrimPath.h"
#include "PreferencesGui.h"
#include "QGIView.h"


using namespace TechDrawGui;
using namespace TechDraw;

QGIPrimPath::QGIPrimPath():
    m_capStyle(Qt::RoundCap),
//    m_fillStyleCurrent (Qt::SolidPattern),
    m_fillOverride(false)
{
    setCacheMode(QGraphicsItem::NoCache);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setAcceptHoverEvents(true);

    isHighlighted = false;

    m_colOverride = false;
    m_colNormal = getNormalColor();
    m_pen.setColor(m_colNormal);
    m_pen.setStyle(Qt::SolidLine);
    m_capStyle = prefCapStyle();
    m_pen.setCapStyle(m_capStyle);
    m_pen.setWidthF(0);

    m_styleDef = Qt::NoBrush;
    m_styleSelect = Qt::SolidPattern;
    m_styleNormal = m_styleDef;
    m_brush.setStyle(m_styleNormal);

    m_colDefFill = Qt::white;
//    m_colDefFill = Qt::transparent;
    setFillColor(m_colDefFill);

    setPrettyNormal();
}

QVariant QGIPrimPath::itemChange(GraphicsItemChange change, const QVariant &value)
{
//    Base::Console().Message("QGIPP::itemChange(%d) - type: %d\n", change, type() - QGraphicsItem::UserType);
    if (change == ItemSelectedHasChanged && scene()) {
        if(isSelected()) {
            setPrettySel();
        } else {
            setPrettyNormal();
        }
    }
    return QGraphicsPathItem::itemChange(change, value);
}

void QGIPrimPath::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
//    Base::Console().Message("QGIPP::hoverEnter() - selected; %d\n", isSelected());
    if (!isSelected()) {
        setPrettyPre();
    }
    QGraphicsPathItem::hoverEnterEvent(event);
}

void QGIPrimPath::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
//    Base::Console().Message("QGIPP::hoverLeave() - selected; %d\n", isSelected());
    if(!isSelected()) {
        setPrettyNormal();
    }

    QGraphicsPathItem::hoverLeaveEvent(event);
}

//set highlighted is obsolete
void QGIPrimPath::setHighlighted(bool b)
{
    isHighlighted = b;
    if(isHighlighted) {
        setPrettySel();
    } else {
        setPrettyNormal();
    }
}

void QGIPrimPath::setPrettyNormal() {
//    Base::Console().Message("QGIPP::setPrettyNormal()\n");
    m_pen.setColor(m_colNormal);
    m_brush.setColor(m_colNormalFill);
}

void QGIPrimPath::setPrettyPre() {
//    Base::Console().Message("QGIPP::setPrettyPre()\n");
    m_pen.setColor(getPreColor());
    if (!m_fillOverride) {
        m_brush.setColor(getPreColor());
    }
}

void QGIPrimPath::setPrettySel() {
//    Base::Console().Message("QGIPP::setPrettySel()\n");
    m_pen.setColor(getSelectColor());
    if (!m_fillOverride) {
        m_brush.setColor(getSelectColor());
    }
}

//wf: why would a face use it's parent's normal colour?
//this always goes to parameter
QColor QGIPrimPath::getNormalColor()
{
    QGIView *parent;

    if (m_colOverride) {
        return m_colNormal;
    }

    QGraphicsItem* qparent = parentItem();
    if (!qparent) {
        parent = nullptr;
    } else {
        parent = dynamic_cast<QGIView *> (qparent);
    }

    if (parent) {
        return parent->getNormalColor();
    }
    return PreferencesGui::normalQColor();
}

QColor QGIPrimPath::getPreColor()
{
    QGIView *parent;
    QGraphicsItem* qparent = parentItem();
    if (!qparent) {
        parent = nullptr;
    } else {
        parent = dynamic_cast<QGIView *> (qparent);
    }

    if (parent) {
        return parent->getPreColor();
    }
    return PreferencesGui::preselectQColor();
}

QColor QGIPrimPath::getSelectColor()
{
    QGIView *parent;
    QGraphicsItem* qparent = parentItem();
    if (!qparent) {
        parent = nullptr;
    } else {
        parent = dynamic_cast<QGIView *> (qparent);
    }

    if (parent) {
        return parent->getSelectColor();
    }
    return PreferencesGui::selectQColor();
}

void QGIPrimPath::setWidth(double width)
{
    m_pen.setWidthF(width);
}

void QGIPrimPath::setStyle(Qt::PenStyle style)
{
//    Base::Console().Message("QGIPP::setStyle(QTPS: %d)\n", s);
    m_pen.setStyle(style);
}

void QGIPrimPath::setStyle(int s)
{
//    Base::Console().Message("QGIPP::setStyle(int: %d)\n", s);
    m_pen.setStyle(static_cast<Qt::PenStyle>(s));
}


void QGIPrimPath::setNormalColor(QColor c)
{
    m_colNormal = c;
    m_colOverride = true;
    m_pen.setColor(m_colNormal);
}

void QGIPrimPath::setCapStyle(Qt::PenCapStyle c)
{
    m_capStyle = c;
    m_pen.setCapStyle(c);
}

Base::Reference<ParameterGrp> QGIPrimPath::getParmGroup()
{
    return Preferences::getPreferenceGroup("Colors");
}

//EdgeCapStyle param changed from UInt (Qt::PenCapStyle) to Int (QComboBox index)
Qt::PenCapStyle QGIPrimPath::prefCapStyle()
{
    Qt::PenCapStyle result;
    int newStyle;
    newStyle = Preferences::getPreferenceGroup("General")->GetInt("EdgeCapStyle", 32);    //0x00 FlatCap, 0x10 SquareCap, 0x20 RoundCap
    switch (newStyle) {
        case 0:
            result = static_cast<Qt::PenCapStyle>(0x20);   //round;
            break;
        case 1:
            result = static_cast<Qt::PenCapStyle>(0x10);   //square;
            break;
        case 2:
            result = static_cast<Qt::PenCapStyle>(0x00);   //flat
            break;
        default:
            result = static_cast<Qt::PenCapStyle>(0x20);
    }
    return result;
}

void QGIPrimPath::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    //wf: this seems a bit of a hack. does it mess up selection of QGIPP??
    QGIView *parent;
    QGraphicsItem* qparent = parentItem();
    if (qparent) {
        parent = dynamic_cast<QGIView *> (qparent);
        if (parent) {
//            Base::Console().Message("QGIPP::mousePressEvent - passing event to QGIV parent\n");
            parent->mousePressEvent(event);
        } else {
//            qparent->mousePressEvent(event);  //protected!
            QGraphicsPathItem::mousePressEvent(event);
        }
    } else {
//        Base::Console().Message("QGIPP::mousePressEvent - passing event to ancestor\n");
        QGraphicsPathItem::mousePressEvent(event);
    }
}

void QGIPrimPath::setFill(QColor color, Qt::BrushStyle style) {
    setFillColor(color);
    m_styleNormal = style;
    m_brush.setStyle(style);
}

void QGIPrimPath::setFill(QBrush b) {
    setFillColor(b.color());
    m_styleNormal = b.style();
    m_brush.setStyle(b.style());
}

void QGIPrimPath::resetFill() {
    m_colNormalFill = m_colDefFill;
    m_styleNormal = m_styleDef;
    m_brush.setStyle(m_styleDef);
}

//set PlainFill
void QGIPrimPath::setFillColor(QColor color)
{
    m_colNormalFill = color;
    m_brush.setColor(m_colNormalFill);
//    m_colDefFill = c;
}


void QGIPrimPath::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {
    QStyleOptionGraphicsItem myOption(*option);
    myOption.state &= ~QStyle::State_Selected;

    setPen(m_pen);
    setBrush(m_brush);

    QGraphicsPathItem::paint (painter, &myOption, widget);
}

