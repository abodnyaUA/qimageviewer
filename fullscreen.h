#ifndef fullscreen_H
#define fullscreen_H

#include <QWidget>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QCloseEvent>
#include "image.h"
#include "hotkeys.h"

namespace Ui {
class fullscreen;
}

class fullscreen : public QWidget
{
    Q_OBJECT
    
public:
    explicit fullscreen(image *imagewidget, hotkeysStruct * hotkeys);
    ~fullscreen();
    QVBoxLayout *lay;
    image *imagewidget;
    void startSlideShow();

    void setSlideshowSmoothTransition(bool);
    void setSlideshowInterval(double);
    bool getSlideshowSmoothTransition();
    double getSlideshowInterval();

signals:
    void fullscreenEnded();
    ///not working///
    void needResize();
    void needCrop();
    void needSave();
    void needSaveAs();
    void needOpen();
    void needExit();

private:
    Ui::fullscreen *ui;
    hotkeysStruct * hotkeys;

    //slideshow
    QTimer *timer;
    bool slideshowSmoothTransition;
    double slideshowInterval;
    bool slideshowStarted;

    QLabel* photo_;
    QLabel* photo_2;

    QGraphicsView * nextimage;
    QGraphicsScene * nextscene;
    QPixmap * nextpixmap;

private slots:
    void nextSlide();
    void nextSlideAnimation();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void closeEvent(QCloseEvent *event);
};

#endif // fullscreen_H
