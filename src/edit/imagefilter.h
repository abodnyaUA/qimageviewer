#ifndef IMAGEFILTER_H
#define IMAGEFILTER_H

#include <QObject>
#include <QPixmap>
#include <QImage>
#include <QRgb>

namespace Filter
{
    enum FilterType
    { Gray_Scale, Brightness, Temperature, Saturation, Blur, Sharpen, Sepia };
}

class ImageFilter : public QObject
{
    Q_OBJECT
public:
    QPixmap operator () (QPixmap sourcePixmap, Filter::FilterType filter, int delta = NULL);
    
signals:
    
public slots:
    
private:
    QPixmap grayScale(QImage origin);
    QPixmap brightness(QImage origin, int delta);
    QPixmap temperature(QImage origin, int delta);
    QPixmap saturate(QImage origin, int delta);
    QPixmap blur(QImage origin);
    QPixmap sharpen(QImage origin);
    QPixmap sepia(QImage origin);
};

#endif // IMAGEFILTER_H
