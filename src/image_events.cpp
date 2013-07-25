#include "image.h"

#include <QDebug>
///Drag'n'Drop///
void image::dropEvent(QDropEvent *event)
{
    qDebug() << "dropped = " <<event->mimeData()->objectName();
}

void image::resizeEvent(QResizeEvent *event)
{
    prevButton->setGeometry(0, 0, 50, this->height());
    nextButton->setGeometry(this->width()-50, 0, 50, this->height());
}

//Background
void image::drawBackground(QPainter * painter, const QRectF & rect)
{
    if (isActiveFullscreen)
    {
        QPen pen(fullscreencolor);
        painter->setPen(pen);
        painter->fillRect(-width(),-height(),2*width(),2*height(),QBrush(fullscreencolor));
    }
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

/// mouse wheel for zooming/changing image ///
void image::wheelEvent( QWheelEvent * event )
{
    double delta = event->delta();
    if (!mouseZoom)
    {
        if (mousezoomCtrlPressed)
        {
            if (isPixmap)
            {
                if (delta > 0) zoomInc();
                else zoomDec();
            }
        }
        else
        {
            if (delta > 0)
            {
                if (imagelist_indx < size()-1)
                    setImage(imagelist_indx+1);
                else
                    setImage(0);
            }
            else
            {
                if (imagelist_indx > 0)
                    setImage(imagelist_indx-1);
                else
                    setImage(size()-1);
            }
        }
    }
    else
    {
        if (isPixmap)
        {
            if (delta > 0) zoomInc();
            else zoomDec();
        }
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

///Event, when user moved horizontal or vertical ScrollBar, ///
///automaticaly moved sumMousePos ///
void image::horizontalSliderMoverd(int arg)
{
    sumMousePos.setX(arg+300);
}
void image::verticalSliderMoverd(int arg)
{
    sumMousePos.setY(arg+100);
}

///Keyboard///
bool image::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress && isPixmap)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        Qt::KeyboardModifiers modifiers = keyEvent->modifiers();
        int keyInt = keyEvent->key();
        Qt::Key key = static_cast<Qt::Key>(keyInt);


        if(modifiers & Qt::ShiftModifier)
            keyInt += Qt::SHIFT;
        if(modifiers & Qt::ControlModifier)
            keyInt += Qt::CTRL;
        if(modifiers & Qt::AltModifier)
            keyInt += Qt::ALT;
        if(modifiers & Qt::MetaModifier)
            keyInt += Qt::META;
        QString textHotkey = QKeySequence(keyInt).toString(QKeySequence::PortableText);

        if (key == Qt::Key_Control && textHotkey != hotkeys->fileSaveAs && textHotkey != hotkeys->fileOpen
                && textHotkey != hotkeys->fileSettings && textHotkey != hotkeys->fileQuit)
        {
            mousezoomCtrlPressed = true;
        }
        //ZOOMING
        //Zoom In KEY
        if (textHotkey == hotkeys->zoomIn)
        {   zoomInc();
            return true;  }
        //Zoom Out KEY
        if (textHotkey == hotkeys->zoomOut)
        {   zoomDec();
            return true;  }
        //Zoom Window KEY
        if (textHotkey == hotkeys->zoomWindow)
        {   reloadImage();
            return true;  }
        //Zoom Original KEY
        if (textHotkey == hotkeys->zoomOriginal)
        {   setOriginalSize();
            return true;  }

        //NEXT/PREV IMAGE
        if ((zoom <= 1.0))
        {
            //LEFT KEY
            if ((int)keyInt == (int)Qt::Key_Left)
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
            if ((int)keyInt == (int)Qt::Key_Right)
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
            if ((int)keyInt == (int)Qt::Key_Left)
            {
                if (sumMousePos.x() > 100) sumMousePos.setX(sumMousePos.x()-100);
                horizontalScrollBar()->setValue(horizontalScrollBar()->value()-100);
                return true;
            }
            //RIGHT KEY
            if ((int)keyInt == (int)Qt::Key_Right)
            {
                if (horizontalScrollBar()->value() < horizontalScrollBar()->maximum()-100) sumMousePos.setX(sumMousePos.x()+100);
                horizontalScrollBar()->setValue(horizontalScrollBar()->value()+100);
                return true;
            }
            //UP KEY
            if ((int)keyInt == (int)Qt::Key_Up)
            {
                if (sumMousePos.y() > 100) sumMousePos.setY(sumMousePos.y()-100);
                verticalScrollBar()->setValue(verticalScrollBar()->value()-100);
                return true;
            }
            //DOWN KEY
            if ((int)keyInt == (int)Qt::Key_Down)
            {
                if (verticalScrollBar()->value() < verticalScrollBar()->maximum()-100) sumMousePos.setY(sumMousePos.y()+100);
                verticalScrollBar()->setValue(verticalScrollBar()->value()+100);
                return true;
            }
        }
        return false;
    }
    if (event->type() == QEvent::KeyRelease && isPixmap)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int keyInt = keyEvent->key();
        Qt::Key key = static_cast<Qt::Key>(keyInt);

        if (key == Qt::Key_Control)
            mousezoomCtrlPressed = false;
    }
    return false;
}

