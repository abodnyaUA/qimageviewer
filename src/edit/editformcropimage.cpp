#include "editformcropimage.h"
#include <QDebug>
editformcropimage::editformcropimage(QWidget *)
{
    isMouseGrabbed = false;
    isFrame = false;
    left = 0; right = 0; top = 0; down = 0;
}

void editformcropimage::setindets()
{
    this->width_indent = (width() - sceneRect().width()) / 2;
    this->height_indent = (height() - sceneRect().height()) / 2;
}

void editformcropimage::mousePressEvent(QMouseEvent *event)
{
    if (isFrame && (event->x() - width_indent) > left
            && (event->x() - width_indent) < right
            && (event->y() - height_indent) > top
            && (event->y() - height_indent) < down)
    {
        isMouseGrabbed = true;
        lastpos_x = event->x();
        lastpos_y = event->y();
        return;
    }
    if (event->x() > width_indent && event->y() > height_indent)
    {
        left = event->x() - width_indent;
        top = event->y() - height_indent;
    }
    else
    {
        left = 0;
        top = 0;
    }
    right = left;
    down = top;
    isMouseGrabbed = true;
    mousepressPos_x = event->x();
    mousepressPos_y = event->y();
    emit lefttopWasChanged(left,top);
}

void editformcropimage::mouseMoveEvent(QMouseEvent *event)
{
    if (isMouseGrabbed)
    {
        if (isFrame && fabs(event->x() - lastpos_x) <= 1
                && fabs(event->x() - lastpos_x) <= 1
                && left > 0 && right < sceneRect().width()
                && top > 0 && down < sceneRect().height())
        {
            qDebug() << "evX =" << event->x() <<
                        "evY =" << event->y() <<
                        "lpX =" << lastpos_x <<
                        "lpY =" << lastpos_y;
            qDebug() << "was left = "<<left<<
                        "right = "<<right<<
                        "top = "<<top<<
                        "down = "<<down;
            //if (event->x() - lastpos_x > 1) lastpos_x = event->x();
            //if (event->y() - lastpos_y > 1) lastpos_y = event->y();
            left += (event->x() - lastpos_x);
            right += (event->x() - lastpos_x);
            top += (event->y() - lastpos_y);
            down += (event->y() - lastpos_y);
            qDebug() << "is  left = "<<left<<
                        "right = "<<right<<
                        "top = "<<top<<
                        "down = "<<down;
            emit lefttopWasChanged(left,top);
            emit rightdownWasChanged(right,down);
            lastpos_x = event->x();
            lastpos_y = event->y();
            return;
        }/*
        if ((event->x() - width_indent) > left
            && (event->y() - height_indent) > top)
        {*/
            right = event->x() - width_indent;
            down = event->y() - height_indent;
            if (right > sceneRect().width()) right = sceneRect().width();
            if (down > sceneRect().height()) down = sceneRect().height();
            emit rightdownWasChanged(right,down);
            return;
        /*}
        else if ((event->x() - width_indent) <= left
                 && (event->y() - height_indent) > top)
        {
            //right = left;
            left = event->x() - width_indent;
            down = event->y() - height_indent;
            if (left < width_indent) left = width_indent;
            if (down > sceneRect().height()) down = sceneRect().height();
            emit lefttopWasChanged(left,top);
            emit rightdownWasChanged(right,down);
            return;
        }
        else if ((event->x() - width_indent) <= left
                 && (event->y() - height_indent) <= top)
        {
            //right = left;
            //down = top;
            left = event->x() - width_indent;
            top = event->y() - height_indent;
            if (left < width_indent) left = width_indent;
            if (top < height_indent) top = height_indent;
            emit lefttopWasChanged(left,top);
            emit rightdownWasChanged(right,down);
            return;
        }
        else if ((event->x() - width_indent) > left
                 && (event->y() - height_indent) <= top)
        {
            //down = top;
            right = event->x() - width_indent;
            top = event->y() - height_indent;
            if (right > sceneRect().width()) right = sceneRect().width();
            if (top < height_indent) top = height_indent;
            emit lefttopWasChanged(left,top);
            emit rightdownWasChanged(right,down);
            return;
        }*/

    }
}

void editformcropimage::mouseReleaseEvent(QMouseEvent *event)
{
    isMouseGrabbed = false;
    if (fabs(mousepressPos_x - event->x()) < 2 && fabs(mousepressPos_y - event->y()) < 2) isFrame = false;
    else isFrame = true;
}


void editformcropimage::changeFrame(int right, int down)
{
    isFrame = true;
    this->right = right;
    this->down = down;
}

void editformcropimage::drawForeground(QPainter * painter, const QRectF &)
{
    //qDebug() << "Draw: left="<<left<<"right="<<right<<"top="<<top<<"down="<<down;
    QPen pen(QBrush(QColor(20,20,20)),3,Qt::DashDotLine);
    painter->setPen(pen);
    painter->drawRect(left,top,fabs(right-left),fabs(down-top));
}
