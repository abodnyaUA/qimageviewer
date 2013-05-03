#include "image.h"

#include <QDebug>
///Drag'n'Drop///
void image::dropEvent(QDropEvent *event)
{
    qDebug() << "dropped = " <<event->mimeData()->objectName();
        //loadimage();

}

//zoom grabbed moving
void image::mousePressEvent(QMouseEvent *event)
{
    isMouseGrabbed = true;
    lastMousePos.setX(event->x());
    lastMousePos.setY(event->y());
    setCursor(Qt::SizeAllCursor);
}

void image::mouseMoveEvent(QMouseEvent *event)
{
    if (isMouseGrabbed)
    {
        sumMousePos.setX(sumMousePos.x() - (event->x()-lastMousePos.x())*1.2 );
        sumMousePos.setY(sumMousePos.y() - (event->y()-lastMousePos.y())*1.2 );
        qDebug() << "CurX="<<event->x()<<
                    "CurY="<<event->y();
        centerOn(sumMousePos.x(),sumMousePos.y());
        lastMousePos.setX(event->x());
        lastMousePos.setY(event->y());
        //ensureVisible(QRectF());
    }
}

void image::mouseReleaseEvent(QMouseEvent *event)
{
    isMouseGrabbed = false;
    setCursor(Qt::ArrowCursor);
}

///mouse wheel for zooming///
void image::wheelEvent( QWheelEvent * event )
{
    double delta = event->delta();
    if (isPixmap && mouseZoom)
    {
        imageScene->clear();
        imageScene->setSceneRect(0,0,1,1);
        imageScene->clear();
        if ((double)imagePixmap->width()/(double)width() > (double)imagePixmap->height()/(double)height())
        {
            if (delta > 0)
            {
                if ((zoom+0.2)*(width()*0.95) < imagePixmap->width()) zoom += 0.2;
            }
            if (delta < 0 && zoom > 0.2 &&  (zoom-0.2)*(width()*0.95) > 10) zoom -= 0.2;
            if ((zoom+0.2)*(width()*0.95) < imagePixmap->width())
            {
                imageScene->setSceneRect(0,0,(width()*0.95)*zoom,(*imagePixmap).scaledToWidth((width()*0.95)*zoom).height());
                imageScene->addPixmap((*imagePixmap).scaledToWidth((width()*0.95)*zoom));
            }
            else
            {
                imageScene->setSceneRect(0,0,(*imagePixmap).width(),(*imagePixmap).height());
                imageScene->addPixmap(*imagePixmap);
            }
        }
        else
        {
            if (delta > 0)
            {
                if ((zoom+0.2)*(height()*0.98) < imagePixmap->height()) zoom += 0.2;
            }
            if (delta < 0 && zoom > 0.2 && (zoom-0.2)*(height()*0.98) > 10) zoom -= 0.2;
            if ((zoom+0.2)*(height()*0.98)< imagePixmap->height())
            {
                imageScene->setSceneRect(0,0,(*imagePixmap).scaledToHeight(((height()*0.98))*zoom).width(),((height()*0.98))*zoom);
                imageScene->addPixmap((*imagePixmap).scaledToHeight(((height()*0.98))*zoom));
            }
            else
            {
                imageScene->setSceneRect(0,0,(*imagePixmap).width(),(*imagePixmap).height());
                imageScene->addPixmap(*imagePixmap);
            }
        }
        setSceneRect(0,0,imageScene->width(),imageScene->height());
        setScene(imageScene);
    }
}

///Double-click for fullscreen///
void image::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (mouseFullscreen)
    {
        emit needFullscreen();
        sumMousePos.setX( imageScene->width()/2.0 );
        sumMousePos.setY( imageScene->height()/2.0 );
    }
}

///Keyboard///
bool image::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress && isPixmap)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int k = keyEvent->key();
        //ZOOMING
        //+ KEY
        if ((int)k == (int)Qt::Key_Plus)
        {
            zoomInc();
            return true;
        }
        //- KEY
        if ((int)k == (int)Qt::Key_Minus)
        {
            zoomDec();
            return true;
        }
        //NEXT/PREV IMAGE
        if ((zoom <= 1.0))
        {
            //LEFT KEY
            if ((int)k == (int)Qt::Key_Left)
            {
                if (imagelist_indx>0)
                {
                    imagelist_indx--;
                    setImage(imagelist_indx);
                }
                else
                {
                    imagelist_indx = imagelist.size()-1;
                    setImage(imagelist_indx);
                }
                emit currentImageWasChanged(imagelist_indx);
                return true;
            }
            //RIGHT KEY
            if ((int)k == (int)Qt::Key_Right)
            {
                if (imagelist_indx < (imagelist.size()-1))
                {
                    imagelist_indx++;
                    setImage(imagelist_indx);
                }
                else
                {
                    imagelist_indx = 0;
                    setImage(imagelist_indx);
                }
                emit currentImageWasChanged(imagelist_indx);
                return true;
            }
        }
        //SCROLLING
        else
        {
            //LEFT KEY
            if ((int)k == (int)Qt::Key_Left)
            {
                if (sumMousePos.x() > 200) sumMousePos.setX(sumMousePos.x()-100);
                horizontalScrollBar()->setValue(horizontalScrollBar()->value()-100);
                return true;
            }
            //RIGHT KEY
            if ((int)k == (int)Qt::Key_Right)
            {
                if (horizontalScrollBar()->value() < horizontalScrollBar()->maximum()-100) sumMousePos.setX(sumMousePos.x()+100);
                horizontalScrollBar()->setValue(horizontalScrollBar()->value()+100);
                return true;
            }
            //UP KEY
            if ((int)k == (int)Qt::Key_Up)
            {
                if (sumMousePos.y() > 200) sumMousePos.setY(sumMousePos.y()-100);
                verticalScrollBar()->setValue(verticalScrollBar()->value()-100);
                return true;
            }
            //DOWN KEY
            if ((int)k == (int)Qt::Key_Down)
            {
                if (verticalScrollBar()->value() < verticalScrollBar()->maximum()-100) sumMousePos.setY(sumMousePos.y()+100);
                verticalScrollBar()->setValue(verticalScrollBar()->value()+100);
                return true;
            }
        }
        return false;
    }
    return false;
}

