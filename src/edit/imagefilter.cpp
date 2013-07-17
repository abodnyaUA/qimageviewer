#include "imagefilter.h"
#include <QDebug>

QPixmap ImageFilter::operator ()(QPixmap sourcePixmap, Filter::FilterType filter, int delta)
{
    QImage origin(sourcePixmap.toImage());

    switch (filter)
    {
    case Filter::Gray_Scale:
        return grayScale(origin);
        break;
    case Filter::Brightness:
        return brightness(origin, delta);
        break;
    case Filter::Temperature:
        return temperature(origin, delta);
        break;
    case Filter::Saturation:
        return saturate(origin, delta);
        break;
    case Filter::Blur:
        return blur(origin);
        break;
    case Filter::Sharpen:
        return sharpen(origin);
        break;
    case Filter::Sepia:
        return sepia(origin);
        break;
    }
}

QPixmap ImageFilter::grayScale(QImage origin)
{
    QImage newImage(origin.width(), origin.height(), QImage::Format_ARGB32);
    QColor oldColor;
    for (int x = 0; x < newImage.width(); x++)
    {
        for (int y = 0; y < newImage.height(); y++)
        {
            oldColor = QColor(origin.pixel(x, y));
            int average = (oldColor.red() + oldColor.green() + oldColor.blue())/3;
            newImage.setPixel(x, y, qRgb(average, average, average));
        }
    }
    QPixmap processedImage(QPixmap::fromImage(newImage));
    return processedImage;
}

QPixmap ImageFilter::brightness(QImage origin, int delta)
{
    QImage newImage(origin.width(), origin.height(), QImage::Format_ARGB32);

    QColor oldColor;
    int r, g, b;

    for (int x=0; x < newImage.width(); x++)
    {
        for (int y=0; y<newImage.height(); y++)
        {
            oldColor = QColor(origin.pixel(x,y));
            r = oldColor.red() + delta;
            g = oldColor.green() + delta;
            b = oldColor.blue() + delta;
            //we check if the new values are between 0 and 255
            r = qBound(0, r, 255);
            g = qBound(0, g, 255);
            b = qBound(0, b, 255);

            newImage.setPixel(x, y, qRgb(r, g, b));
        }
    }
    QPixmap processedImage(QPixmap::fromImage(newImage));

    return processedImage;
}

QPixmap ImageFilter::temperature(QImage origin, int delta)
{
    QImage newImage(origin.width(), origin.height(), QImage::Format_ARGB32);

    QColor oldColor;
    int r, g, b;

    for (int x=0; x < newImage.width(); x++)
    {
        for (int y=0; y < newImage.height(); y++)
        {
            oldColor = QColor(origin.pixel(x, y));

            r = oldColor.red() + delta;
            g = oldColor.green() + delta;
            b = oldColor.blue();

            //we check if the new values are between 0 and 255
            r = qBound(0, r, 255);
            g = qBound(0, g, 255);

            newImage.setPixel(x, y, qRgb(r, g, b));
        }
    }
    QPixmap processedImage(QPixmap::fromImage(newImage));

    return processedImage;
}

QPixmap ImageFilter::saturate(QImage origin, int delta)
{
    QImage newImage(origin.width(), origin.height(), QImage::Format_ARGB32);

    QColor oldColor;
    QColor newColor;
    int h,s,l;

    for(int x=0; x < newImage.width(); x++)
    {
        for(int y=0; y < newImage.height(); y++)
        {
            oldColor = QColor(origin.pixel(x, y));

            newColor = oldColor.toHsl();
            h = newColor.hue();
            s = newColor.saturation() + delta;
            l = newColor.lightness();

            //we check if the new value is between 0 and 255
            s = qBound(0, s, 255);

            newColor.setHsl(h, s, l);

            newImage.setPixel(x, y, qRgb(newColor.red(), newColor.green(), newColor.blue()));
        }
    }
    QPixmap processedImage(QPixmap::fromImage(newImage));

    return processedImage;
}

QPixmap ImageFilter::blur(QImage origin)
{
    QImage newImage(origin);

    int kernel [5][5]= {{0,0,1,0,0},
                        {0,1,3,1,0},
                        {1,3,7,3,1},
                        {0,1,3,1,0},
                        {0,0,1,0,0}};
    int kernelSize = 5;
    int sumKernel = 27;
    int r,g,b;
    QColor color;

    for(int x=kernelSize/2; x < newImage.width()-(kernelSize/2); x++)
    {
        for(int y=kernelSize/2; y < newImage.height()-(kernelSize/2); y++)
        {

            r = 0;
            g = 0;
            b = 0;

            for(int i = -kernelSize/2; i <= kernelSize/2; i++){
                for(int j = -kernelSize/2; j <= kernelSize/2; j++){
                    color = QColor(origin.pixel(x+i, y+j));
                    r += color.red()*kernel[kernelSize/2+i][kernelSize/2+j];
                    g += color.green()*kernel[kernelSize/2+i][kernelSize/2+j];
                    b += color.blue()*kernel[kernelSize/2+i][kernelSize/2+j];
                }
            }

            r = qBound(0, r/sumKernel, 255);
            g = qBound(0, g/sumKernel, 255);
            b = qBound(0, b/sumKernel, 255);

            newImage.setPixel(x, y, qRgb(r,g,b));

        }
    }
    QPixmap processedImage(QPixmap::fromImage(newImage));

    return processedImage;
}

QPixmap ImageFilter::sharpen(QImage origin)
{
    QImage newImage(origin);

    int kernel [3][3]= {{0,-1,0},
                        {-1,5,-1},
                        {0,-1,0}};
    int kernelSize = 3;
    int sumKernel = 1;
    int r,g,b;
    QColor color;

    for(int x = kernelSize/2; x < newImage.width()-(kernelSize/2); x++){
        for(int y = kernelSize/2; y < newImage.height()-(kernelSize/2); y++){

            r = 0;
            g = 0;
            b = 0;

            for(int i = -kernelSize/2; i<= kernelSize/2; i++)
            {
                for(int j = -kernelSize/2; j <= kernelSize/2; j++)
                {
                    color = QColor(origin.pixel(x+i, y+j));
                    r += color.red() * kernel[kernelSize/2+i][kernelSize/2+j];
                    g += color.green() * kernel[kernelSize/2+i][kernelSize/2+j];
                    b += color.blue() * kernel[kernelSize/2+i][kernelSize/2+j];
                }
            }

            r = qBound(0, r/sumKernel, 255);
            g = qBound(0, g/sumKernel, 255);
            b = qBound(0, b/sumKernel, 255);

            newImage.setPixel(x, y, qRgb(r, g, b));

        }
    }
    QPixmap processedImage(QPixmap::fromImage(newImage));

    return processedImage;
}

QPixmap ImageFilter::sepia(QImage origin)
{
    QImage newImage(origin.width(), origin.height(), QImage::Format_ARGB32);
    QColor oldColor;
    int r, g, b;
    for (int x = 0; x < newImage.width(); x++)
    {
        for (int y = 0; y < newImage.height(); y++)
        {
            oldColor = QColor(origin.pixel(x, y));
            r = (oldColor.red() + oldColor.green() + oldColor.blue() + 112)/4;
            g = (oldColor.red() + oldColor.green() + oldColor.blue() + 66)/4;
            b = (oldColor.red() + oldColor.green() + oldColor.blue() + 20)/4;
            newImage.setPixel(x, y, qRgb(r, g, b));
        }
    }
    QPixmap processedImage(QPixmap::fromImage(newImage));

    return processedImage;
}
