#ifndef QImageViewer_H
#define QImageViewer_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QPixmap>
#include <QSlider>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include "image.h"
#include "preview.h"
#include "previewlist.h"
#include <QDebug>
#include "fullscreen.h"
#include "editformresize.h"
#include "editformcrop.h"
//#include <sys/utsname.h>
#include <QCloseEvent>
#include <QDropEvent>
#include <QScrollArea>

namespace Ui {
class QImageViewer;
}

class QImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit QImageViewer(QString path = QString::null, QWidget *parent = 0);
    ~QImageViewer();
    
private slots:
    void fileOpen();
    void fileSave();
    void fileSaveAs();
    void helpAbout();
    void filesFind();
    void nextImage();
    void prevImage();
    void nextImageArrows();
    void prevImageArrows();
    void fullScreen();
    void fullScreenOvered();
    void fullScreenFromImage();
    void resizeImage();
    void resizeImageOvered(bool result);
    void cropImage();
    void cropImageOvered(bool result);
    void currentIndexWasChanged(int indx);
    void setUndoEnable(bool);
    void setRedoEnable(bool);
    void setStatusName(bool);

private:
    /// Settings ///
    QString lastdirectory;


    /// Other stuff ///
    QDesktopWidget desk;
    Ui::QImageViewer *ui;
    image *imagewidget;
    QSlider * zoomslider;

    QScrollArea previewArea;
    previewList * prevList;
    QHBoxLayout prevLayout;

    QFileDialog * dialog;

    QString defaultpath;

    bool isfullScreenActive;
    fullscreen * fullScreenWidget;

    editformResize *editFormResize;
    editformCrop *editFormCrop;

    void createActions();
    void createDesign();
    void loadsettings();

protected:
    void resizeEvent(QResizeEvent *);
    bool eventFilter(QObject *obj, QEvent *event);
    void closeEvent(QCloseEvent *event);
};

#endif // QImageViewer_H
