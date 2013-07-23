#include "fullscreen.h"
#include "ui_fullscreen.h"
#include <QDebug>

fullscreen::fullscreen(image *imagewidget, hotkeysStruct *hotkeys, QColor fullscreencolor):
    ui(new Ui::fullscreen)
{
    ui->setupUi(this);
    lay = ui->verticalLayout_2;
    this->imagewidget = imagewidget;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(nextSlide()));
    slideshowStarted = false;
    this->hotkeys = hotkeys;
    this->fullscreencolor = fullscreencolor;

    this->installEventFilter(this);
    this->installEventFilter(imagewidget);

    //Fullscreen only for first screen
    QDesktopWidget *desktop = QApplication::desktop();
    QRect rect = desktop->screenGeometry(0);
    setGeometry(rect);
}


void fullscreen::setSlideshowSmoothTransition(bool arg)
{   slideshowSmoothTransition = arg;   }
void fullscreen::setSlideshowInterval(double arg)
{   slideshowInterval = arg;}
void fullscreen::setBkgColor(QColor arg)
{   fullscreencolor = arg;  }
bool fullscreen::getSlideshowSmoothTransition()
{   return slideshowSmoothTransition;  }
double fullscreen::getSlideshowInterval()
{   return slideshowInterval;   }
QColor fullscreen::getBkgColor()
{   return fullscreencolor;     }

void fullscreen::startSlideShow()
{
    timer->start(slideshowInterval*1000);
    slideshowStarted = true;
}

/// NOT WORKING YET ////
void fullscreen::nextSlideAnimation()
{
    nextimage = new QGraphicsView(this);
    nextscene = new QGraphicsScene;
    nextpixmap = new QPixmap(imagewidget->getImageList()[imagewidget->currentImage()+2]);

    if ((double)nextpixmap->width()/(double)width() > (double)nextpixmap->height()/(double)height())
        nextscene->addPixmap((*nextpixmap).scaledToWidth(width()));
    else
        nextscene->addPixmap((*nextpixmap).scaledToHeight(height()));
    lay->removeWidget(imagewidget);
    nextimage->setScene(nextscene);

    lay->addWidget(nextimage);
    //    imagewidget->setGeometry(0,0,width()-i*0.0001,height()-i*0.0001);
    //    nextimage->setGeometry(0,0,i*0.0001,i*0.0001);
    //    imagewidget->updateGeometry();
    //    nextimage->updateGeometry();
    //getchar();

}

void fullscreen::nextSlide()
{
    if (slideshowSmoothTransition)
    {
        if (imagewidget->currentImage() < imagewidget->size()-1)
        {
            //lay->removeWidget(imagewidget);
            nextSlideAnimation();
            lay->removeWidget(nextimage);
            imagewidget->setImage(imagewidget->currentImage()+1);
            //lay->removeWidget(nextimage);

            delete nextimage; delete nextpixmap;
            delete nextscene;
        }
        else
        {
            timer->stop();
            emit fullscreenEnded();
        }
    }
    else
    {
        qDebug() << "NEED NEXT SLIDE";
        if (imagewidget->currentImage() < imagewidget->size()-1)
            imagewidget->setImage(imagewidget->currentImage()+1);
        else
        {
            timer->stop();
            emit fullscreenEnded();
        }
    }
}

/** close fullscreen with ESC or F10 key **/
bool fullscreen::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        Qt::KeyboardModifiers modifiers = keyEvent->modifiers();
        int keyInt = keyEvent->key();

        if(modifiers & Qt::ShiftModifier)
            keyInt += Qt::SHIFT;
        if(modifiers & Qt::ControlModifier)
            keyInt += Qt::CTRL;
        if(modifiers & Qt::AltModifier)
            keyInt += Qt::ALT;
        if(modifiers & Qt::MetaModifier)
            keyInt += Qt::META;
        QString textHotkey = QKeySequence(keyInt).toString(QKeySequence::PortableText);

        //'ESC' || Fullscreen KEY
        if ((int)keyInt == (int)Qt::Key_Escape || textHotkey == hotkeys->watchFullscreen)
        {
            timer->stop();
            emit fullscreenEnded();
            return true;
        }
        //Slideshow KEY
        else if (textHotkey == hotkeys->watchSlideshow)
        {
            if (!slideshowStarted) startSlideShow();
            else
            {
                timer->stop();
                slideshowStarted = false;
            }
            return true;
        }
        else if (textHotkey == hotkeys->fileOpen)
        {   emit needOpen(); return true; }
        else if (textHotkey == hotkeys->fileSave)
        {   emit needSave(); return true; }
        else if (textHotkey == hotkeys->fileSaveAs)
        {   emit needSaveAs(); return true; }
        else if (textHotkey == hotkeys->fileQuit)
        {   emit needQuit(); return true; }
        else if (textHotkey == hotkeys->editCrop)
        {   emit needCrop(); return true; }
        else if (textHotkey == hotkeys->editResize)
        {   emit needResize(); return true; }
        else if (textHotkey == hotkeys->editResizeItems)
        {   emit needResizeItems(); return true; }
        else if (textHotkey == hotkeys->editRotateLeft)
        {   emit needRotateLeft(); return true; }
        else if (textHotkey == hotkeys->editRotateRight)
        {   emit needRotateRight(); return true; }
        else if (textHotkey == hotkeys->editFlipHorizontal)
        {   emit needFlipHorizontal(); return true; }
        else if (textHotkey == hotkeys->editFlipVertical)
        {   emit needFlipVertical(); return true; }
        else if (textHotkey == hotkeys->editUndo)
        {   emit needUndo(); return true; }
        else if (textHotkey == hotkeys->editRedo)
        {   emit needRedo(); return true; }
        else if (textHotkey == QKeySequence(QKeySequence::Delete).toString())
        {   emit needDelete(); return true; }



    }
    return false;
}

void fullscreen::closeEvent(QCloseEvent *event)
{
    timer->stop();
    event->accept();
}

fullscreen::~fullscreen()
{
    delete ui;
}
