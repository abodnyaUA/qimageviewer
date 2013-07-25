#ifndef IMAGE_H
#define IMAGE_H
#include <QtGui>
#include <QGraphicsView>
#include <QImage>
#include <QMenu>
#include <QAction>
#include <QFileInfo>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QGraphicsPixmapItem>
#include <QScrollBar>
#include <qdrawutil.h>
#include <QMovie>
#include "settings/hotkeys.h"
#include "algorithms.h"
#include "qhiddenbutton.h"

namespace Ui {
class Properties;
}


class image : public QGraphicsView
{
    Q_OBJECT

public:
    image();
    QHiddenButton *prevButton, *nextButton;
    void loadhotkeys(hotkeysStruct * hotkeys);
    void loadiconpack(QString iconpack);
    void loadiconnames(QMap<QString,QString> icons);
    //program blocks//
    void loadimage(QString path);
    void addToBuffer(QPixmap * pixmap);
    void resetZoom();
    void loadimagelist(QStringList list);

    //returned//
    bool isZoomed();
    int size();
    int currentImage();
    QString currentImageName();
    QPixmap currentPixmap();
    bool isReady();
    void setReady(bool);
    QStringList getImageList();
    void setFullscreen(QColor fullscreencolor);
    void unsetFullscreen();

public slots:
    void reloadImage();
    void setOriginalSize();
    void setImage(int indx);
    void insertImage(QString filename,int pos);
    void zoomDec();
    void zoomInc();
    void rotateLeft();
    void rotateRight();
    void flipHorizontal();
    void flipVertical();
    void saveimage(QString filename);
    void setSaved();
    void setEdited();
    bool isSaved();
    bool deleteCurrentItem();
    bool deleteCurrentItemWithoutAsc();
    void removeFromList(QString name);
    void prevBuffer();
    void nextBuffer();
    void setAsWallpaper();
    void setAsWallpaperExtern(QString image);
    void viewProperties();
    ///Mouse///
    void setMouseZoom(bool);
    void setMouseFullscreen(bool);
    bool getMouseZoom();
    bool getMouseFullscreen();

private slots:
    void onMovieUpdated();

signals:
    void currentImageWasChanged(int indx);
    void itsPossibleToRedo(bool);
    void itsPossibleToUndo(bool);
    void itsSaved(bool);
    void needFullscreen();
    void needSlideshow();
    void listIsEmpty();

private:
    QColor fullscreencolor;
    QString iconiconpack;
    QMap<QString,QString> icon;

    bool isActiveFullscreen;
    hotkeysStruct * hotkeys;
    QDesktopWidget desk;
    QPixmap * imagePixmap;
    QGraphicsScene *imageScene;
    QGraphicsPixmapItem *imagePixmapItem;
    QMovie *imageMovie;
    bool isMovie;
    bool wasMovie;
    bool mouseGrabbed;
    bool isPixmap;
    double zoom;
    bool zoomMax;
    bool zoomMin;
    bool zoomOriginal;
    bool mousezoomCtrlPressed;

    ////image list////
    QString imagename;
    QStringList imagelist;
    int imagelist_indx;

    ////Context menu////
    void createContextMenu();
    QAction * actionProperties;
    QAction * actionDelete;
    QAction * actionFullscreen;
    QAction * actionWallpaper;
    QAction * actionSlideshow;
    QAction * actionZoomIn;
    QAction * actionZoomOut;
    QAction * actionZoomOriginal;
    QAction * actionZoomWindow;
    QDialog * propertiesWidget;
    Ui::Properties *ui_prop;

    ////Editing////
    bool wasEdited;

    ////Buffers////
    QList<QPixmap> buffer;
    int buffer_indx;

    ///Zoom grabbed move///
    QRectF lastMousePos;
    QRectF sumMousePos;
    bool isMouseGrabbed;

    ///Mouse///
    bool mouseZoom;
    bool mouseFullscreen;

private slots:
    void horizontalSliderMoverd(int);
    void verticalSliderMoverd(int);

protected:
    //zoom grabbed move
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    //Zoom in-out / Change images
    void wheelEvent( QWheelEvent * event );
    //fullscreen
    void mouseDoubleClickEvent(QMouseEvent *event);
    //changepictures (keyboard event)
    bool eventFilter(QObject *obj, QEvent *event);
    //context menu
    void contextMenuEvent(QContextMenuEvent *event);
    void dropEvent(QDropEvent *event);
    //background
    void resizeEvent(QResizeEvent *event);
    void drawBackground(QPainter * painter, const QRectF & rect);
};

#endif // IMAGE_H
