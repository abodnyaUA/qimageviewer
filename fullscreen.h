#ifndef fullscreen_H
#define fullscreen_H

#include <QWidget>
#include <QVBoxLayout>
#include <QKeyEvent>
#include "image.h"

namespace Ui {
class fullscreen;
}

class fullscreen : public QWidget
{
    Q_OBJECT
    
public:
    explicit fullscreen(image *imagewidget);
    ~fullscreen();
    QVBoxLayout *lay;
    image *imagewidget;
    void startSlideShow();

    void setSlideshowSmoothTransition(bool);
    void setSlideshowInterval(int);
    bool getSlideshowSmoothTransition();
    int getSlideshowInterval();

signals:
    void fullscreenEnded();
    void needResize();
    void needCrop();
    void needSave();
    void needSaveAs();
    void needOpen();
    void needExit();

private:
    Ui::fullscreen *ui;

    //slideshow
    QTimer *timer;
    bool slideshowSmoothTransition;
    int slideshowInterval;

private slots:
    void nextSlide();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // fullscreen_H
