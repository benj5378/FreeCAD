#include "QGIFrame.h"
#include <QPainter>

#include <Base/Console.h>
#include <Base/Tools.h>
#include <numbers>
#include <cmath>

#include "Rez.h"

using std::numbers::pi;

namespace TechDrawGui {

QGIFrame::QGIFrame(Shape shape) : QGraphicsPathItem()
{
    setShape(shape);
}

// void QGIFrame::paint(
//     QPainter* painter,
//     const QStyleOptionGraphicsItem* option,
//     QWidget* widget = nullptr
// ) {
 
void QGIFrame::setShape(Shape shape)
{
    m_shape = shape;
    updatePath();
}

void QGIFrame::setRect(QRectF rect)
{
    m_rect = rect;
    updatePath();
}

void QGIFrame::updatePath()
{
    if(m_shape == Shape::Triangle) {
        setPath(trianglePath(m_rect));
    }
    else if(m_shape == Shape::Rectangle) {
        setPath(rectPath(m_rect));
    }
    // else if(m_shape == Shape::Hexagon) {
    //     setPath(hexagonPath(painter));
    // }
    // else if(shape == Shape::Circle) {
    //     float radius = rect.height();
    //     if (rect.width() > radius) {
    //         radius = rect.width();
    //     }

    //     painter.drawEllipse(rect.center(), radius, radius);
    // }

    //setPath(path);
}

QPainterPath QGIFrame::rectPath(QRectF r)
{
    QPainterPath path;
    path.addRect(r);
    return path;
}

//! 
// double QGIFrame::shapeHeight()
// {
//     return m_rect.
// }

// Acute isosceles triangle
QPainterPath QGIFrame::trianglePath(QRectF rect)
{
    QPainterPath path;
    path.moveTo(rect.bottomLeft());
    path.lineTo(rect.topLeft());
    path.lineTo(rect.left() + 2 * rect.width(), rect.center().y());
    path.closeSubpath();
    return path;
}

// https://upload.wikimedia.org/wikipedia/commons/8/8a/01-Sechseck-Seite-vorgegeben-wiki.svg
// w = the x length from F to E
// h = the y length from E to A
// tan(30deg) = the angle between FE and vertical
QPainterPath QGIFrame::hexagonPath(QRectF rect)
{
    QPainterPath path;
    constexpr qreal verticalMargin = 1.3;
    const qreal h = rect.height() * verticalMargin;
    const qreal w = std::tan(Base::toRadians(30.0)) * 0.5 * h;
    const QPointF F = {rect.left() - w , rect.center().y()          };
    const QPointF E = {rect.left()     , rect.center().y() + 0.5 * h};
    const QPointF D = {rect.right()    , rect.center().y() + 0.5 * h};
    const QPointF C = {rect.right() + w, rect.center().y()          };
    const QPointF B = {rect.left()     , rect.center().y() - 0.5 * h};
    const QPointF A = {rect.left()     , rect.center().y() - 0.5 * h};
    path.addPolygon({F, E, D, C, B, A});
    return path;
}

// void QGIFrame::paint(
//     QPainter* painter,
//     const QStyleOptionGraphicsItem* option,
//     QWidget* widget
// ) {
//     painter->setBrush(Qt::red);
//     // painter->drawRect(m_rect);
//     // painter->drawRect(QRectF(0.0, 0.0, 100000.0, 100000.0));
//     // auto a = painter->brush();
//     QPen p = painter->pen();
//     QString rgba = p.color().name(QColor::HexArgb);
//     Base::Console().Message(rgba.toStdString().c_str());
//     QGraphicsPathItem::paint(painter, option, widget);
// }

// QPainterPath QGIFrame::trianglePath(QRectF rect)
// {
//     QPainterPath path;
//     double radius = sqrt(pow((rect.height() / 2.0), 2) + pow((rect.width() / 2.0), 2));
//     radius = radius;// * scale;
//     radius += Rez::guiX(3.0);
//     //offsetLR = tan(Base::toRadians(30.0)) * radius;
//     QPolygonF triangle;
//     double startAngle = -pi / 2;
//     double angle = startAngle;
//     for (int i = 0; i < 4; i++) {
//         triangle +=
//             QPointF(m_rect.center().x() + (radius * cos(angle)), m_rect.center().y() + (radius * sin(angle)));
//         angle += (2 * pi / 3);
//     }
//     path.moveTo(m_rect.center().x() + (radius * cos(startAngle)),
//                        m_rect.center().y() + (radius * sin(startAngle)));
//     path.addPolygon(triangle);
//     return path;
// }

// QGIFrame::drawHexagon(QPainter* painter)
// {
//     double radius = sqrt(pow((rect.height() / 2.0), 2) + pow((rect.width() / 2.0), 2));
//     radius = radius * scale;
//     radius += Rez::guiX(1.0);
//     offsetLR = radius;
//     QPolygonF triangle;
//     double startAngle = -2 * pi / 3;
//     double angle = startAngle;
//     for (int i = 0; i < 7; i++) {
//         triangle +=
//             QPointF(lblCenter.x + (radius * cos(angle)), lblCenter.y + (radius * sin(angle)));
//         angle += (2 * pi / 6);
//     }
//     balloonPath.moveTo(lblCenter.x + (radius * cos(startAngle)),
//                     lblCenter.y + (radius * sin(startAngle)));
//     balloonPath.addPolygon(triangle);
// }

}
