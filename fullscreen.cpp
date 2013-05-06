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
    slideshowStarted = false;
}


void fullscreen::setSlideshowSmoothTransition(bool arg)
{   slideshowSmoothTransition = arg;   }
void fullscreen::setSlideshowInterval(double arg)
{   slideshowInterval = arg;}
bool fullscreen::getSlideshowSmoothTransition()
{   return slideshowSmoothTransition;  }
double fullscreen::getSlideshowInterval()
{   return slideshowInterval;   }

void fullscreen::startSlideShow()
{
    timer->start(slideshowInterval*1000);
    slideshowStarted = true;
}

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
        //'F5' KEY
        if ((int)k == (int)Qt::Key_F5)
        {
            if (!slideshowStarted) startSlideShow();
            else
            {
                timer->stop();
                slideshowStarted = false;
            }
            return true;
        }
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
