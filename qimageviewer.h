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
#include <QDebug>
#include <QCloseEvent>
#include <QDropEvent>
#include <QScrollArea>
#include "image.h"
#include "preview.h"
#include "previewlist.h"
#include "fullscreen.h"
#include "editformresize.h"
#include "editformcrop.h"
#include "settings.h"

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
    //FILE
    void fileOpen();
    void fileSave();
    void fileSaveAs();
    void filesFind();
    void currentIndexWasChanged(int indx);
    void settingsWindow();
    void updateSettings(QString language,
                        QString defaultfolder,
                        bool mouseZoom, bool mouseFullscreen,
                        bool slideshowSmoothTransition, double slideshowInterval,
                        QString cropHotkey, QString resizeHotkey,
                        QString fullscreenHotkey, QString slideshowHotkey,
                        QString undoHotkey, QString redoHotkey);
    //HELP
    void helpAbout();
    //NAVIGATION
    void nextImage();
    void prevImage();
    void nextImageArrows();
    void prevImageArrows();
    //WATCH
    void fullScreen();
    void fullScreenOvered();
    void fullScreenFromImage();
    void slideShow();
    //EDIT
    void resizeImage();
    void resizeImageOvered(bool result);
    void cropImage();
    void cropImageOvered(bool result);
    void setUndoEnable(bool);
    void setRedoEnable(bool);
    void setStatusName(bool);

private:
    /// Settings ///
    //Default Folder//
    QString lastdirectory;

    //Language//
    QString language;

    //Fullscreen//
    bool slideshowSmoothTransition;
    double slideshowInterval;

    //Hotkeys//
    QString cropHotkey;
    QString resizeHotkey;
    QString fullscreenHotkey;
    QString slideshowHotkey;
    QString undoHotkey;
    QString redoHotkey;
    Settings *settings;

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
