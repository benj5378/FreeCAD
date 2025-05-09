#include "QGIFrame.h"
#include <QPainter>

// #include <Base/Console.h>
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
    if (m_shape == Shape::Circle) {
        setPath(circlePath(m_rect));
    }
    else if (m_shape == Shape::Triangle) {
        setPath(trianglePath(m_rect));
    }
    else if (m_shape == Shape::Rectangle) {
        setPath(rectPath(m_rect));
    }
    else if (m_shape == Shape::Hexagon) {
        setPath(hexagonPath(m_rect));
    }
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


namespace {
constexpr qreal lineWidth = 3.0;

//! Returns the shape height relative to the path, not to the bounding rect (`- lineWidth / 2`)
qreal getShapeHeight(const QRectF rect)
{
    qreal h = rect.height() - lineWidth / 2;
    // as per ISO 129-1:2019 annex A.1 l)
    return h * 2;
}
}

QPainterPath QGIFrame::circlePath(const QRectF rect)
{
    const qreal h = getShapeHeight(rect); // minus lineWidth? No, Qt draws within rect!
    QPainterPath path;
    path.addEllipse(rect.center(), h, h);
    return path;
}

// Acute isosceles triangle
QPainterPath QGIFrame::trianglePath(const QRectF rect)
{
    const qreal h = getShapeHeight(rect);
    QPainterPath path;
    path.moveTo(rect.left(), rect.center().y() - h / 2);
    path.lineTo(rect.left(), rect.center().y() + h / 2);
    path.lineTo(rect.left() + 2 * rect.width(), rect.center().y());
    path.closeSubpath();
    return path;
}

// https://upload.wikimedia.org/wikipedia/commons/8/8a/01-Sechseck-Seite-vorgegeben-wiki.svg
// w = the x length from F to E
// h = the y length from E to A
// l = segment length
// tan(30deg) = the angle between FE and vertical
QPainterPath QGIFrame::hexagonPath(const QRectF rect)
{
    const qreal h = getShapeHeight(rect);
    const qreal w = std::tan(Base::toRadians(30.0)) * h / 2;
    const qreal l = sqrt((h / 2) * (h / 2) + w * w);
    const QPointF F = {rect.center().x() - l / 2 - w, rect.center().y()        };
    const QPointF E = {rect.center().x() - l / 2    , rect.center().y() + h / 2};
    const QPointF D = {rect.center().x() + l / 2    , rect.center().y() + h / 2};
    const QPointF C = {rect.center().x() + l / 2 + w, rect.center().y()        };
    const QPointF B = {rect.center().x() + l / 2    , rect.center().y() - h / 2};
    const QPointF A = {rect.center().x() - l / 2    , rect.center().y() - h / 2};
    QPainterPath path;
    path.addPolygon({F, E, D, C, B, A});
    path.closeSubpath();
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

}  // namespace TechDrawGui
