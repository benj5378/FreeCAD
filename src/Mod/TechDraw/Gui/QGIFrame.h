
#include <cstdint>
#include <QGraphicsPathItem>

namespace TechDrawGui {

class QGIFrame : public QGraphicsPathItem
{
public:
    enum class Shape : std::uint8_t {
        Triangle,
        Rectangle,
        Hexagon,
        Circle
    };

    QGIFrame(Shape shape);
    
    // void paint(QPainter* painter,
    //            const QStyleOptionGraphicsItem* option,
    //            QWidget* widget=nullptr) override;

    Shape m_shape;

    QPainterPath rectPath(QRectF rect);
    QPainterPath trianglePath(QRectF rect);
    QPainterPath hexagonPath(QRectF rect);

    void setShape(Shape shape);
    void setRect(QRectF rect);

protected:
    void updatePath();

    //! Defines the rect the shape should be drawn around
    //Q_PROPERTY(QRectF m_rect READ rect WRITE setRect)

    QRectF m_rect;
    
    //! Margin between shape and innerRect
    float margin;
};

}
