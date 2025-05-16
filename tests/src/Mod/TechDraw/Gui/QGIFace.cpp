#include <QTest>


class testQGIFace : public QObject
{
    void init()
    {
    }

    void cleanup()
    {

    }

    void testSetFill()
    {
        QRectF size{50.0, 50.0};
        QGIFace face{};
        QPainterPath path;
        path.addRect(size);
        face.setOutline(path);
        QPixmap pixmap{size};
        QPainter painter{pixmap};
        QStyleOptionGraphicsItem opt;
        face.paint(&painter, &opt);

        QFile file{"/home/bensay/Downloads/pixmap.png"};
        file.open(QIODevice::WriteOnly);
        pixmap.save(&file, "PNG");
    }

}




