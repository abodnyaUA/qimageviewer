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
    explicit fullscreen(image *imagewidget, hotkeysStruct * hotkeys, QColor fullscreencolor);
    ~fullscreen();
    QVBoxLayout *lay;
    image *imagewidget;
    void startSlideShow();

    void setSlideshowSmoothTransition(bool);
    void setSlideshowInterval(double);
    bool getSlideshowSmoothTransition();
    double getSlideshowInterval();
    QColor getBkgColor();
    void setBkgColor(QColor);

signals:
    void fullscreenEnded();
    ///not working///
    void needSave();
    void needSaveAs();
    void needOpen();
    void needQuit();

    void needResize();
    void needCrop();
    void needResizeItems();
    void needRotateLeft();
    void needRotateRight();
    void needFlipHorizontal();
    void needFlipVertical();
    void needUndo();
    void needRedo();

private:
    Ui::fullscreen *ui;
    hotkeysStruct * hotkeys;
    QColor fullscreencolor;

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
