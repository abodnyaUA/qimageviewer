#include "fullscreen.h"
#include "ui_fullscreen.h"
#include <QDebug>

fullscreen::fullscreen(image *imagewidget):
    ui(new Ui::fullscreen)
{
    ui->setupUi(this);
    lay = ui->verticalLayout;
    this->imagewidget = imagewidget;
    this->installEventFilter(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(nextSlide()));
}


void fullscreen::setSlideshowSmoothTransition(bool arg)
{   slideshowSmoothTransition = arg;   }
void fullscreen::setSlideshowInterval(int arg)
{   slideshowInterval = arg;}
bool fullscreen::getSlideshowSmoothTransition()
{   return slideshowSmoothTransition;  }
int fullscreen::getSlideshowInterval()
{   return slideshowInterval;   }

void fullscreen::startSlideShow()
{
    timer->start(slideshowInterval);
}

void fullscreen::nextSlide()
{
    if (slideshowSmoothTransition)
    {
        if (imagewidget->currentImage() < imagewidget->size()-1)
            imagewidget->setImage(imagewidget->currentImage()+1);
        else
        {
            timer->stop();
            emit fullscreenEnded();
        }
    }
    else
    {
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
bool fullscreen::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int k = keyEvent->key();
        //'ESC' | 'F10' KEY
        if ((int)k == (int)Qt::Key_Escape || (int)k == (int)Qt::Key_F10)
        {
            timer->stop();
            emit fullscreenEnded();
            return true;
        }
    }
    return false;
}


fullscreen::~fullscreen()
{
    delete ui;
}
