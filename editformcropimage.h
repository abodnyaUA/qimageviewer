#ifndef EDITFORMCROPIMAGE_H
#define EDITFORMCROPIMAGE_H

#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
#include <qdrawutil.h>
#include <math.h>
#include "image.h"

class editformcropimage : public QGraphicsView
{
    Q_OBJECT
public:
    explicit editformcropimage(QWidget *parent = 0);
    void setindets();
    
signals:
    void lefttopWasChanged(int left, int top);
    void rightdownWasChanged(int right, int down);
    
public slots:
    void changeFrame(int right, int down);

private:
    int left,right,top,down;
    int width_indent, height_indent;
    bool isMouseGrabbed;
    bool isFrame;
    int lastpos_x, lastpos_y;
    int mousepressPos_x,mousepressPos_y;
    QPainter painter;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void drawForeground(QPainter * painter, const QRectF & rect);
};

#endif // EDITFORMCROPIMAGE_H
